#define _GUI_C
#include "dbt_m4.h"



// Registri privati
_MccFrame_Str mcc_cmd;          // Comandi ricevuti da GUI
_DeviceAppRegister_Str listaRegistri; // Lista di compilazione registri
MCC_MEM_SIZE mcc_len;                 // Dimensione messaggio ricevuto

//_RxStdSeq_Str rx_param; // Usato per passare i parametri di esposizione

MCC_ENDPOINT  ep={_DEF_MCC_GUI_TO_M4_MASTER};

//////////////////////////////////////////////////////////////////////////////
/*
void gui_interface_task(uint32_t initial_data)

  TASK di gestione comandi provenienti dalla GUI su core A5
  


Autore: M. Rispoli
Data: 30/09/2014
*/
//////////////////////////////////////////////////////////////////////////////
void gui_interface_task(uint32_t initial_data)
{
  
  printf("GUI INTERFACE STARTED\n");
  
  // Creazione Endpoint per ricezione comandi da GUI
  if(mccOpenPort(&ep)==FALSE)
  {
    printf ("GUI INTERFACE: MEMORIA NON DISPONIBILE PER END POINT\n");
    _mqx_exit(-1);
  }


  // Attesa completamento attivazione dispositivi su BUS
  // In questa il Master richiede lo Status per sapere cosa sta accadendo;
  // Il Master potrebbe anche richiedere l'attivazione del Loader
  // La gestione operativa avviene solo se i dispositivi sono configurati
  while(1){
    if(mccRxFrame(&ep, &mcc_cmd)){
      if(mcc_cmd.cmd == MCC_PRINT) mccPrint();
      else if((mcc_cmd.cmd == MCC_LOADER) || (generalConfiguration.loaderOn)) manageMccLoader();
      else if(mcc_cmd.cmd == MCC_CONFIG) manageMccConfig();
      else if(generalConfiguration.deviceConfigured){
          digitalManageMccOperativo();
      }
    }
  }
}
  
  //________________________________________________________________________________________________________________________
  //                                            INTERFACCIA LOADER
  // PROTOCOLLO SUL COMANDO:
  // mcc_cmd.buffer[0] = Sottocomando
  // mcc_cmd.buffer[1:N] = buffer dati del sottocomando

  // Prototipi dei sottocomandi
  void loader_activation(int id,unsigned char* data, int len);
  void loader_chip_erase(int id,unsigned char* data, int len);
  void loader_write_block(int id,unsigned char* data, int len);
  void loader_write_config(int id,unsigned char* data, int len);
  void loader_read_config(int id,unsigned char* data, int len);

  void manageMccLoader(void){
    // Accetta solo comandi di tipo Loader
    if(mcc_cmd.cmd!=MCC_LOADER) return;

    switch(mcc_cmd.buffer[0]){
       case LOADER_ACTIVATION:  // Attivazione Loader remoto a indirizzo ricevuto
        loader_activation(mcc_cmd.id,&mcc_cmd.buffer[1],mcc_cmd.len-1);
        break;
      case LOADER_CHIP_ERASE:
        loader_chip_erase(mcc_cmd.id,&mcc_cmd.buffer[1],mcc_cmd.len-1);
        break;
      case LOADER_WRITE_BLK:
        loader_write_block(mcc_cmd.id,&mcc_cmd.buffer[1],mcc_cmd.len-1);
        break;
      case LOADER_WRITE_CONFIG:
        loader_write_config(mcc_cmd.id,&mcc_cmd.buffer[1],mcc_cmd.len-1);
        break;
      case LOADER_READ_CONFIG:
        loader_read_config(mcc_cmd.id,&mcc_cmd.buffer[1],mcc_cmd.len-1);
        break;
    }

  }

/* COMANDA L'ATTIVAZIONE/DISATTIVAZIONE DEL LOADER
 data[0]: 1== ATTIVAZIONE, 0== DISATTIVAZIONE
 data[1]: Indirizzo Loader Periferica
 data[2]: 1 == uCA, 2== uCB (solo per attivazione)
 RISPOSTA:

 ATTIVAZIONE:
 La funzione effettua la connessione con il remoto richiesto
 ed effettua il Setup, acquisendo le info relative al target
 all'ID e alla config word (se disponibile).

*/
void loader_activation(int id,unsigned char* data, int len)
{       
  unsigned char buffer[1];
  
  if(loaderActivation(data[1], data[2])==FALSE)
  {
    loaderExit(TRUE); 
    buffer[0] = 0;
    generalConfiguration.loaderOn = false;
  }else  buffer[0] = 1;
  
  generalConfiguration.loaderOn = true;
  mccLoaderNotify(id,LOADER_ACTIVATION,buffer,1);
  return;
}

/* 
    COMANDA LA CANCELLAZIONE DELLA FLASH DEL TARGET REMOTO
    ED INIZIALIZZA LA PROGRAMMAZIONE
    Nessun parametro
  
   Il comando richiede che sia stato attivato il relativo loader
*/
void loader_chip_erase(int id,unsigned char* data, int len)
{       
  unsigned char buffer[1];
 
  if(loaderChipErase()==FALSE)
  {
    loaderExit(TRUE);
    generalConfiguration.loaderOn = false;
    buffer[0] = 0;
  }
  else buffer[0] = 1;
   
  mccLoaderNotify(id,LOADER_CHIP_ERASE,buffer,1);
  return;
}

void loader_write_block(int id,unsigned char* data, int len)
{       
  unsigned char buffer[1];
  int i;
  _addrStr blocco;
  
  blocco.startAddr = data[0] + 256*data[1];
  blocco.len = data[2];
 
  // Controllo sulla dimensione e coerenza del buffer dati
  if((3+ 2*blocco.len)!= len) 
  {
    printf("GUI: ERRORE LEN=%d BLOCCO LEN=%d\n",len,blocco.len );
    buffer[0] = 0;
    mccLoaderNotify(id,LOADER_WRITE_BLK,buffer,1);
    return;
  }

  // Carica i dati nella struttura
  for(i=0; i< blocco.len; i++) {
    blocco.val[i] = data[3+2*i] + 256 * data[4+2*i];
  }
  
  if(loaderLoadSegment(&blocco)==FALSE)
  {
    printf("GUI: SCRITTURA BLOCCO FALLITA\n");
    loaderExit(TRUE);
    buffer[0] = 0;
  }
  else buffer[0] = 1;
  
  mccLoaderNotify(id,LOADER_WRITE_BLK,buffer,1);
  return;
}

void loader_write_config(int id,unsigned char* data, int len)
{       
  unsigned char buffer[1];
  
  if(loaderWriteConfig(data)==FALSE)
  {
    buffer[0] = 0;
  }
  else buffer[0] = 1;
  
  // Questa deve essere l'ultima istruzione in un ciclo di scrittura dispositivo
  loaderExit(TRUE);
  generalConfiguration.loaderOn = false;
  mccLoaderNotify(id,LOADER_WRITE_CONFIG,buffer,1);
  return;
}

void loader_read_config(int id,unsigned char* data, int len)
{       
  unsigned char buffer[11];
  
  // Effettua un'attivazione per prelevare i valori richiesti
  if(loaderActivation(data[0], data[1])==FALSE)
  {
    loaderExit(TRUE);
    generalConfiguration.loaderOn = false;
    buffer[0]=0;
    mccLoaderNotify(id,LOADER_READ_CONFIG,buffer,1);
    return;
  }
  
  buffer[0]=1;
  buffer[1] = (unsigned char) Loader.cfg.id0;
  buffer[2] = (unsigned char) (Loader.cfg.id0>>8);
  buffer[3] = (unsigned char) Loader.cfg.id1;
  buffer[4] = (unsigned char) (Loader.cfg.id1>>8);
  buffer[5] = (unsigned char) Loader.cfg.id2;
  buffer[6] = (unsigned char) (Loader.cfg.id2>>8);
  buffer[7] = (unsigned char) Loader.cfg.id3;
  buffer[8] = (unsigned char) (Loader.cfg.id3>>8);
  buffer[9] = (unsigned char) Loader.cfg.config;
  buffer[10] = (unsigned char) (Loader.cfg.config>>8);
  
  mccLoaderNotify(id,LOADER_READ_CONFIG,buffer,11);
  loaderExit(TRUE);
  generalConfiguration.loaderOn = false;
  return;
}

  //_________________________________________________________________________________________________________________________
  
  
  
  //________________________________________________________________________________________________________________________
  //                                            INTERFACCIA CONFIGURAZIONE
  // PROTOCOLLO SUL COMANDO DI CONFIGURAZIONE:
  // mcc_cmd.buffer[0] = Dispositivo da configurare
  // mcc_cmd.buffer[1] = Blocco dati
  // mcc_cmd.buffer[2:len-3] = Blocco dati

void manageMccConfig(){
     // Accetta solo comandi di tipo Config
    if(mcc_cmd.cmd!=MCC_CONFIG) return;
    unsigned char data[30];
    unsigned char size=0;
    

    switch(mcc_cmd.buffer[0]){
     // Configurazione iniziale di tutto il sistema: questa configurazione definisce la struttura hardware
     // della macchina e viene ricevuto all'inizio della fase di startup.
     case CONFIG_GANTRY:
        memcpy(&generalConfiguration.gantryCfg,&mcc_cmd.buffer[2],sizeof(generalConfiguration.gantryCfg));
        generalConfiguration.gantryConfigurationReceived = true;
        mainPrintHardwareConfig();
        // Actuators invia il comando di attivazione device sullo slave
        // Main attiva tutte le thread relative alla architettura

        data[0]=  generalConfiguration.canConnected;
        size = 1;
     break;
    case CONFIG_STATUS:

      // Restituisce lo stato dei drivers e le revisioni
      data[0]= generalConfiguration.deviceConnected; // Tutti i dispositivi sono correttamente connessi
      data[1]=generalConfiguration.revisioni.m4_master.maj;
      data[2]=generalConfiguration.revisioni.m4_master.min;
      data[3]=generalConfiguration.revisioni.pcb269.maj;
      data[4]=generalConfiguration.revisioni.pcb269.min;
      data[5]=generalConfiguration.revisioni.pcb249U1.maj;
      data[6]=generalConfiguration.revisioni.pcb249U1.min;
      data[7]=generalConfiguration.revisioni.pcb249U2.maj;
      data[8]=generalConfiguration.revisioni.pcb249U2.min;
      data[9]=generalConfiguration.revisioni.pcb190.maj;
      data[10]=generalConfiguration.revisioni.pcb190.min;
      data[11]=generalConfiguration.revisioni.pcb244.maj;
      data[12]=generalConfiguration.revisioni.pcb244.min;
      data[13] = generalConfiguration.revisioni.m4_slave.maj;
      data[14] = generalConfiguration.revisioni.m4_slave.min;
      data[15] = generalConfiguration.revisioni.pcb240.maj;
      data[16] = generalConfiguration.revisioni.pcb240.min;
      data[17]=  generalConfiguration.armConnectedStatus ;
      data[18]=  generalConfiguration.trxConnectedStatus ;
      data[19]=  generalConfiguration.lenzeConnectedStatus ;
      data[20]=  generalConfiguration.pcb240ConnectedStatus ;
      data[21]=  generalConfiguration.slaveDeviceConnected;  // Gruppo dei dispositivi slave sono correttamente connessi
      data[22]=  generalConfiguration.canConnected;         // Il CAN bus è operativo
      data[23] = generalConfiguration.candevice_error_startup; // Errori durante startup
      data[24] = generalConfiguration.revisioni.pcb249U1.model; // Modello collimatore

      size = 25;

      break;

     case CONFIG_GENERAL:      
      if(mcc_cmd.buffer[2]==1) {
        generalConfiguration.demoMode = 1;
        printf(" ----------------  DRIVERS IN DEMO MODE --------------------- \n");
      }else{
        generalConfiguration.demoMode = 0;
        printf(" ----------------  DRIVERS IN OPERATING MODE --------------------- \n");
      }
      if(mcc_cmd.buffer[3]==1) {
        generalConfiguration.airTubeModel = true;
        printf(" ----------------  GESTIONE TUBO AD ARIA --------------------- \n");

        // Nessun allarme attivabile e Fan sempre acceso
        pcb249U1SetTubeTemperatureThresholds(255, 255, 1, 0);

      }else{
        generalConfiguration.airTubeModel = false;
        printf(" ----------------  GESTIONE TUBO AD OLIO --------------------- \n");

        // Nessun allarme attivabile e Fan acceso sopra i 40° e scende sotto i 35
        pcb249U1SetTubeTemperatureThresholds(255, 255, 159 , 157);

      }

     break;
     case CONFIG_PCB190:  
        if(config_pcb190(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB269_0:
        if(config_pcb215(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
    case CONFIG_PCB269_1:
       if(config_pcb215(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
       else data[0]= 0;
       size = 1;
     break;
     case CONFIG_TRX:
        if(config_trx(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
    case CONFIG_ARM:
       if(config_arm(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
       else data[0]= 0;
       size = 1;
     break;
    case CONFIG_LENZE:
       if(config_lenze(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
       else data[0]= 0;
       size = 1;
     break;

    case CONFIG_PCB249U1_1:
        if(config_pcb249U1(true, 0, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB249U1_2:
        if(config_pcb249U1(true, 1, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB249U1_3:
        if(config_pcb249U1(true, 2, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB249U1_4:
        if(config_pcb249U1(true, 3, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB249U1_5:
        if(config_pcb249U1(true, 4, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB249U1_6:
        if(config_pcb249U1(true, 5, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
     case CONFIG_PCB249U1_7:
        if(config_pcb249U1(true, 6, &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
      break;
      
     case CONFIG_PCB249U2:  
       if(config_pcb249U2(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true){
         data[0]=1; // Configurazione OK. 
       }else{
         data[0]= 0; // Fallita configurazione per qualche motivo
       }
       size = 1;
       
      break;
     case CONFIG_PCB244:

        if(config_pcb244(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;

      break;
     case CONFIG_BIOPSY:  
         if(config_biopsy(true, mcc_cmd.buffer[1], &mcc_cmd.buffer[2], mcc_cmd.len-2)==true) data[0]=1;
        else data[0]= 0;
        size = 1;
     break;

    case  CONFIG_COMPLETED:                    
        // Questo sblocca la fase di startup attivando tutti i polling
        debugPrint("CONFIGURAZIONE DEVICES COMPLETATA");
        generalConfiguration.deviceConfigOk = TRUE; // La configurazione è arrivata
        _EVSET(_EV1_DEV_CONFIG_OK);

        // Attivazione del MASTER_ENA
        SystemOutputs.CPU_MASTER_ENA=1;
        _EVSET(_EV0_OUTPUT_CAMBIATI);
        data[0]= 1;
        size = 1;
      break;        


   //  case CONFIG_REVISIONS: Non usato
      // break;
    }

    // Notifica l'avvenuta esecuzione della configurazione
    // Viene restituito il numero di blocco salvato, per consentire eventualmente 
    // l'invio di più blocchi ..
    mccConfigNotify(1,mcc_cmd.buffer[0],data,size);
    return;
  }
  //_________________________________________________________________________________________________________________________
  
  void digitalManageMccOperativo(void){


     
        switch(mcc_cmd.cmd)
        {

        case MCC_CANOPEN:
            mcc_canopen();
        break;
        case MCC_CMD_EXP_AEC:
          mcc_exp_aec_std();
        break;
        case MCC_CMD_RAGGI_AEC:
          mcc_raggi_aec_std();
        break;
        case MCC_CMD_RAGGI_AE:
          mcc_raggi_bassa_energia(false);
        break;
        case MCC_CMD_RAGGI_AE_AEC:
          mcc_raggi_bassa_energia(true);
        break;


        case MCC_CMD_RAGGI_AE_H:
          mcc_raggi_alta_energia();

        break;
        case MCC_CMD_RAGGI_TOMO :
          mcc_raggi_tomo();
        break;
        case  MCC_CMD_RAGGI_AEC_TOMO:
          mcc_raggi_aec_tomo();
        break;

        case MCC_TEST:
            mcc_test();
          break;
          case  MCC_TEST_RX_SHOT:     // Attiva una sequenza manuale raggi standard senza detector
            mcc_manual_rx_shot();
          break;
          case MCC_SET_MIRROR:
            mcc_set_mirror(mcc_cmd.id, mcc_cmd.cmd,mcc_cmd.buffer[0]);
          break;
          case MCC_SET_LAMP:
            mcc_set_lamp(mcc_cmd.id, mcc_cmd.cmd);
          break;

          case MCC_GET_GONIO:
            mccGetGonio(mcc_cmd.id, mcc_cmd.cmd);
          break;
          case MCC_SET_ROT_TOOL_CONFIG: // Imposta la configurazione per il tool di gestione rotazioni
            mccSetRotationToolConfig();
          break;        
          
          case MCC_RESET_GONIO: // Effettua il reset dell'inclinometro ad angolo prestabilito
            mccResetGonio(mcc_cmd.id,mcc_cmd.cmd);
          break;
          
          case  MCC_SET_COLLI: // Effettua il setting della collimazione
            mccSetColli(mcc_cmd.id,mcc_cmd.cmd);
          break;
          case MCC_SET_FUOCO:
            mccSetFuoco(mcc_cmd.id,mcc_cmd.cmd);
          break;
          case MCC_SET_FILTRO:
            mccSetFiltro();
          break;
          case MCC_CALIB_FILTRO:
            mccSetCalibFiltro();
          break;
          
            case MCC_BIOPSY_SIMULATOR:
                mccBiopsySimulator();
            break;
            case MCC_BIOPSY_CMD:
                mccBiopsyCmd();
            break;
            case MCC_BIOPSY_STD_XYZ:
                mccBiopsyStdXYZ();
            break;


          case MCC_CMD_RAGGI_STD :
            printf("MCC_RAGGI STANDARD\n");
            mcc_raggi_std();
            break;
                          
            case MCC_CMD_SBLOCCO:      // Richiede una sequenza di sblocco compressore
              if(pcb215SetSblocco()==FALSE)
              {
                if(_TEST_BIT(PCB215_FAULT))
                {
                  printf("COMANDO SBLOCCO: ERRORE %d\n", PCB215_CONTEST.Stat.error);
                }else  printf("GUI: IMPOSSIBILE ESEGUIRE LO SBLOCCO\n");
              }else
              {
                printf("GUI: EXEC SBLOCCO\n");
              }
              break;
            case MCC_CMD_PAD_UP:       // Richiede una sequenza di attivazione carrello compressore 
              if(pcb215MovePadUpward(mcc_cmd.buffer[0])==FALSE)
              {
                if(_TEST_BIT(PCB215_FAULT))
                {// Caso di errore del driver
                  printf("COMANDO COMPRESSIONE: ERRORE %d\n", PCB215_CONTEST.Stat.error);
                }else  printf("GUI: IMPOSSIBILE ESEGUIRE IL COMANDO\n");
              }else
              {
                printf("GUI: EXEC UP %d\n",mcc_cmd.buffer[0]);
              }
              break;
              
            
            case MCC_CMD_CMP_STOP:
              if(pcb215SetIdle()==FALSE)
              {
                if(_TEST_BIT(PCB215_FAULT))
                {// Caso di errore del driver
                  printf("COMANDO STOP COMPRESSORE: ERRORE %d\n", PCB215_CONTEST.Stat.error);
                }else  printf("GUI: IMPOSSIBILE ESEGUIRE IL COMANDO\n");
              }else
              {
                printf("GUI: EXEC STOP\n");
              }
              break;

                         
            // Richiesta movimento Tubo   
            case MCC_CMD_TRX:
                mcc_cmd_trx();
             break;
            
            // Richiesta movimento Braccio: angoli in gradi (short)
            case MCC_CMD_ARM:
              mcc_cmd_arm();
            break;

            case MCC_CMD_PCB215_CALIB:
                mcc_pcb215_calibration();
   
            break;
            case MCC_GET_TROLLEY:
                mcc_pcb215_get_trolley();
            break;
            case MCC_STARTER:
              mcc_set_starter();
            break;
            
            case MCC_SET_OUTPUTS:
              setOutputs((_SystemOutputs_Str*) mcc_cmd.buffer, (_SystemOutputs_Str*) &(mcc_cmd.buffer[sizeof(SystemOutputs)]));
            break;

        
            case MCC_SERVICE: // Sottofamiglia di comandi per il service
              mcc_service_commands(mcc_cmd.id,mcc_cmd.buffer[0],&mcc_cmd.buffer[1],mcc_cmd.len-1);
            break;
            
            case MCC_CMD_CMP_AUTO:
              pcb215MovePadDownward(1,TRUE);
            break;
            case MCC_CALIB_ZERO:
                  mcc_calib_zero();
            break;

            case MCC_PARKING_MODE_COMMANDS:

                if(mcc_cmd.buffer[0] == MCC_PARKING_MODE_COMMANDS_START_PARKING)  mcc_parking_mode();
                else if(mcc_cmd.buffer[0] == MCC_PARKING_MODE_COMMANDS_START_UNPARKING)  mcc_unparking_mode();
            break;

            default:
              // printf("Ricevuto buffer di %d\n",mcc_len);
              break;       
        }    

}

/*_____________________________________________________________________________________
    data[0]= comando; // Comando variabile negativo
    data[2,3] = (se TRX_MOVE_ANGLE) angolo in gradi;
    data[1]=0; non usato
_____________________________________________________________________________________*/
void mcc_cmd_trx(void)
{

    if(mcc_cmd.buffer[0]==TRX_MOVE_STOP){
        debugPrint("GUI COMANDO STOP TRX");
        actuatorsTrxStop(0);
        return;

    }

    int timeout = 5;
    if(mcc_cmd.buffer[1] == MOVE_WAIT_END){
        while((!timeout) || (generalConfiguration.trxExecution.run == true) || (generalConfiguration.trxExecution.completed == false) || (!generalConfiguration.trxExecution.idle))   {
            debugPrint("GUI ATTESA COMANDO TRX IN CORSO");
            _time_delay(1000);
            timeout--;
        }
    }

    // Se il comando è già in esecuzione deve rispondere un errore
    if((generalConfiguration.trxExecution.run == true) || (generalConfiguration.trxExecution.completed == false) || (!generalConfiguration.trxExecution.idle)){
        unsigned char buffer[2];
        debugPrint("GUI ERRORE COMANDO TRX: BUSY");
        buffer[0] = TRX_BUSY;
        buffer[1] = 0; // sub codice in caso di errore da fault
        mccGuiNotify(mcc_cmd.id,MCC_CMD_TRX,buffer,2);
        return;
    }


    int angolo = (int) (mcc_cmd.buffer[2] + 256 * mcc_cmd.buffer[3]); // Imposta l'angolo
    generalConfiguration.trxExecution.id = mcc_cmd.id;
    generalConfiguration.trxExecution.test = false; // Comunque stoppa il loop di rodaggio in corso

    _tomo_profile* pTrxProfile;

    pTrxProfile=&generalConfiguration.trxCfg.tomo;

    // Attenzione, tutti gli angoli sono espressi in decimi di grado
    switch(mcc_cmd.buffer[0]){
    case TRX_MOVE_STOP:   actuatorsTrxStop(0);
    case TRX_MOVE_CC:     actuatorsTrxMove(0); break;
    case TRX_MOVE_M15:    actuatorsTrxMove(-1 * generalConfiguration.trxCfg.angolo_biopsia); break;
    case TRX_MOVE_P15:    actuatorsTrxMove(generalConfiguration.trxCfg.angolo_biopsia); break;
    case TRX_MOVE_HOME_W: actuatorsTrxMove(pTrxProfile->w.home_position); break;
    case TRX_MOVE_HOME_N: actuatorsTrxMove(pTrxProfile->n.home_position); break;
    case TRX_MOVE_HOME_I: actuatorsTrxMove(pTrxProfile->i.home_position); break;
    case TRX_MOVE_END_W:  actuatorsTrxMove(pTrxProfile->w.end_position); break;
    case TRX_MOVE_END_N:  actuatorsTrxMove(pTrxProfile->n.end_position); break;
    case TRX_MOVE_END_I:  actuatorsTrxMove(pTrxProfile->i.end_position); break;
    case TRX_MOVE_ANGLE:
        // Angolo deve essere espresso in centesimi di grado
        actuatorsTrxMove(100 * (int) angolo);
    break;

    }


    return;
}

/*_____________________________________________________________________________________

_____________________________________________________________________________________*/
void mcc_cmd_arm(void)
{
    unsigned char buffer[2];
    short angolo;

    // Carica l'angolo dai parametri ricevuti
    FROM_LE16(angolo,&mcc_cmd.buffer[0]);

    // Se il comando è già in esecuzione deve rispondere un errore
    if((generalConfiguration.armExecution.run == true)||(generalConfiguration.armExecution.completed == false)){
        debugPrint("GUI ERRORE COMANDO ARM: BUSY");
        buffer[0] = ARM_BUSY;
        buffer[1] = 0; // sub codice in caso di errore da fault
        mccGuiNotify(mcc_cmd.id,MCC_CMD_ARM,buffer,2);
        return;
    }

    int error=0;

    // Test pending compression
    if(!SystemOutputs.CPU_ROT_ENA)
    {
      // Rotation enable Bus Hardware test
      debugPrint("GUI ERRORE COMANDO ARM: ROT ENA NON ATTIVO");
      error = ARM_DISABLED_ERROR;
    }else if((angolo!=200)&&((angolo>180) ||(angolo<-180)))
    {
      // Errore angolo fuori range
      debugPrintI("GUI ERRORE COMANDO ARM: OUT OF RANGE, ANGOLO",angolo);
      error =  ARM_RANGE_ERROR;
    }else if(generalConfiguration.armCfg.direction_memory==MEM_ARM_DIR_UNDEF){
        // Errore per mancanza di informazioni relative alla posizione del braccio
        debugPrintI("GUI ERRORE COMANDO ARM: OUT OF RANGE, ANGOLO",angolo);
        error =  ARM_RANGE_ERROR;
    }

    if(error){
        buffer[0] = error;
        buffer[1] = 0; // sub codice in caso di errore da fault
        mccGuiNotify(mcc_cmd.id,MCC_CMD_ARM,buffer,2);
        return;

    }

    generalConfiguration.armExecution.id = mcc_cmd.id;

    if(angolo==200){
        if(generalConfiguration.armExecution.dAngolo<0) angolo = -200;
    }
    actuatorsArmMove(angolo);
    return;
}


//////////////////////////////////////////////////////////////////////////////
/*
void mcc_raggi_std(void)

    data[0] =  VADCL
    data[1] =  VDACH
    data[2] =  IDACL
    data[3] =  IDACH
    data[4] =  MASDACL
    data[5] =  MASDACH
    data[6] =  TIMEOUT (100ms)
    data[7] =  [x,hs,swb,swa]
    data[8] =  grid;
    data[9] =  maxV
    data[10] = minV
    data[11] = maxI
    data[12] = minI
    data[13] = sblocco compressore
    data[14] = 1 modalità normale, 0 modalità no detector

Autore: M. Rispoli
Data: 23/10/2014
Modificato: 
*/
//////////////////////////////////////////////////////////////////////////////
void mcc_raggi_std(void)
{
  unsigned char chk,i;
  int timeout;
  unsigned char data[3];
  
    // Verifica se la sequenza è partita
    if(rxStdIsRunning)
    {
      debugPrint("GUI ATTESA FINE SEQUENZA RX 2D IN ESECUZIONE");
      timeout = 30;
      while(rxStdIsRunning)
      {
        if(timeout-- == 0) 
        {
          data[0]=ERROR_WAIT_PENDING_XRAY;       
          data[1]= 0; 
          data[2]= 0 ;  
          mccGuiNotify(1,MCC_CMD_RAGGI_STD,data,sizeof(data));
          return;
        }
        _time_delay(1000);
      }
    }

    // Preparazione parametri sequenza
    if(mcc_cmd.buffer[14]==_SHOT_WITH_DETECTOR) rxStdParam.config=0; // Esposizione con detector
    else rxStdParam.config=1; // Esposizione senza detector

    rxStdParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    rxStdParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    rxStdParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    rxStdParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    rxStdParam.esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    rxStdParam.esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    rxStdParam.esposizione.MINHV=mcc_cmd.buffer[10];
    rxStdParam.esposizione.MAXHV=mcc_cmd.buffer[9];
    rxStdParam.esposizione.MINI=mcc_cmd.buffer[12];
    rxStdParam.esposizione.MAXI=mcc_cmd.buffer[11];
    rxStdParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &rxStdParam.esposizione) [i];
    rxStdParam.esposizione.CHK= chk;
    
    // Controllo sullo sblocco compressore dopo esame
    if(mcc_cmd.buffer[13]) rxStdParam.compressor_unlock = TRUE;
    else rxStdParam.compressor_unlock = FALSE;

   
   // Partenza sequenza
   _EVCLR(_SEQEV_RX_STD_TERMINATED);
   _EVSET(_SEQEV_RX_STD_START);
 
  return;

}

void mcc_raggi_aec_std(void)
{
  unsigned char chk,i;
  unsigned char data[13];
  MCC_ENDPOINT ep = {_DEF_MCC_MASTER_TO_APP_MASTER};
  int timeout;
  
    // Verifica se la sequenza è partita
    if(rxStdAecIsRunning)
    {
      debugPrint("GUI ATTESA FINE SEQUENZA RX AEC IN ESECUZIONE");
      timeout = 30;
      while(rxStdAecIsRunning)
      {
        if(timeout-- == 0) 
        {
          data[0]=ERROR_WAIT_PENDING_XRAY;       
          data[1]= 0; 
          data[2]= 0 ;  
          mccGuiNotify(1,MCC_CMD_RAGGI_AEC,data,3);
          return;
        }
        _time_delay(1000);
      }
    }

    // Preparazione parametri sequenza
    aecIsValid =FALSE;
    aecExpIsValid =FALSE;

    rxStdAecParam.config=0; // Esposizione con detector
    rxStdAecParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    rxStdAecParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    rxStdAecParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    rxStdAecParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    rxStdAecParam.esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    rxStdAecParam.esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    rxStdAecParam.esposizione.MINHV=mcc_cmd.buffer[10];
    rxStdAecParam.esposizione.MAXHV=mcc_cmd.buffer[9];
    rxStdAecParam.esposizione.MINI=mcc_cmd.buffer[12];
    rxStdAecParam.esposizione.MAXI=mcc_cmd.buffer[11];
    rxStdAecParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &rxStdAecParam.esposizione) [i];
    rxStdAecParam.esposizione.CHK= chk;

     // Controllo sullo sblocco compressore dopo esame
    if(mcc_cmd.buffer[13]) rxStdAecParam.compressor_unlock = TRUE;
    else rxStdAecParam.compressor_unlock = FALSE;
    
    // ACCESSORI ALL'ESPOSIZIONE
//    rxStdAecParam.potter_cfg = _POTTER_2D;         
    
    // Partenza sequenza
   _EVCLR(_SEQEV_RX_STD_AEC_TERMINATED);
   _EVSET(_SEQEV_RX_STD_AEC_START);
   return;
  
}

/*_______________________________________________________________________________________________________
    ROUTINES PER IL CONTROLLO DELLE ESPOSIZIONI PER L'ALTA ENERGIA
// Attivazione sequenza raggi AE
_________________________________________________________________________________________________________*/
void mcc_raggi_bassa_energia(bool isAEC)
{
  unsigned char chk,i;
  unsigned char data[13];
  MCC_ENDPOINT ep = {_DEF_MCC_MASTER_TO_APP_MASTER};
  int timeout;

    // La sequenza è ancora attiva ..
    if(rxAeIsRunning)
    {
      data[0]=ERROR_WAIT_PENDING_XRAY;
      data[1]= 0;
      data[2]= 0 ;
      mccGuiNotify(1,MCC_CMD_RAGGI_AE,data,3);
      return;
    }

    rxAeParam.isAEC = isAEC;

    rxAeParam.config=0; // Esposizione con detector
    rxAeParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    rxAeParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    rxAeParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    rxAeParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    rxAeParam.esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    rxAeParam.esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    rxAeParam.esposizione.MINHV=mcc_cmd.buffer[10];
    rxAeParam.esposizione.MAXHV=mcc_cmd.buffer[9];
    rxAeParam.esposizione.MINI=mcc_cmd.buffer[12];
    rxAeParam.esposizione.MAXI=mcc_cmd.buffer[11];
    rxAeParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &rxAeParam.esposizione) [i];
    rxAeParam.esposizione.CHK= chk;

    if(mcc_cmd.buffer[13]) rxAeParam.compressor_unlock = TRUE;
    else rxAeParam.compressor_unlock = FALSE;

    // Partenza sequenza
    _EVCLR(_SEQEV_RX_AE_TERMINATED); // Azzera evento di sequenza terminata
    _EVCLR(_EV2_WAIT_AE_CONTEXT);    // Azzera l'evento di ricezione dati AE
    _EVSET(_SEQEV_RX_AE_START);      // Attiva la thread
    return;

}

void mcc_raggi_alta_energia(void)
{
    unsigned char chk,i;
    unsigned char data[13];


    debugPrint("GUI DATI ALTA ENERGIA ARRIVATI");

    // La sequenza non è ancora attiva ..
    if(!rxAeIsRunning)
    {
        data[0]=ERROR_WAIT_PENDING_XRAY;
        data[1]= 0;
        data[2]= 0 ;
        mccGuiNotify(1,MCC_CMD_RAGGI_AE,data,3);
        return;
    }

    rxAeParam.config=0; // Esposizione con detector
    rxAeParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    rxAeParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    rxAeParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    rxAeParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    rxAeParam.esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    rxAeParam.esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    rxAeParam.esposizione.MINHV=mcc_cmd.buffer[10];
    rxAeParam.esposizione.MAXHV=mcc_cmd.buffer[9];
    rxAeParam.esposizione.MINI=mcc_cmd.buffer[12];
    rxAeParam.esposizione.MAXI=mcc_cmd.buffer[11];
    rxAeParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &rxAeParam.esposizione) [i];
    rxAeParam.esposizione.CHK= chk;

     // Controllo sullo sblocco compressore dopo esame
    if(mcc_cmd.buffer[13]) rxAeParam.compressor_unlock = TRUE;
    else rxAeParam.compressor_unlock = FALSE;

    // Invia l'evento di ripartenza
    _EVSET(_EV2_WAIT_AE_CONTEXT);
    return;
}


//____________________________________________________________________________________________________________________
void mcc_exp_aec_std(void)
{
  unsigned char chk,i;
  _RxStdSeq_Str* pParam=0;
  
  if(generalConfiguration.demoMode){
      aecExpIsValid = TRUE;
      _EVSET(_EV2_WAIT_AEC);
      return;
  }

  if(aecIsValid ==FALSE)
  {
    debugPrint("GUI DATI AEC ARRIVATI TROPPO PRESTO");
    return;
  }

  // Discrimina se Tomo o STandard
  if(mcc_cmd.buffer[13]==_AEC_2D)
  {
    debugPrint("GUI ARRIVATI DATI AEC (2D)");
    pParam = &rxStdAecParam;    
  }else if(mcc_cmd.buffer[13]==_AEC_TOMO)
  {
    debugPrint("GUI ARRIVATI DATI AEC (TOMO)");
    pParam = &tomoAecParam;
  }else if(mcc_cmd.buffer[13]==_AEC_AE)
  {
    debugPrint("GUI ARRIVATI DATI AEC (AE)");
    pParam = &rxAeParam;
  }
  
    pParam->esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    pParam->esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    pParam->esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    pParam->esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    pParam->esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    pParam->esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    pParam->esposizione.MINHV=mcc_cmd.buffer[10];
    pParam->esposizione.MAXHV=mcc_cmd.buffer[9];
    pParam->esposizione.MINI=mcc_cmd.buffer[12];
    pParam->esposizione.MAXI=mcc_cmd.buffer[11];
    pParam->esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &pParam->esposizione) [i];
    pParam->esposizione.CHK= chk;
    
    
    aecExpIsValid = TRUE;
    _EVSET(_EV2_WAIT_AEC);
    return;
}



//////////////////////////////////////////////////////////////////////////////
/*
void mcc_raggi_tomo(void)


    data[0] =  VADCL
    data[1] =  VDACH
    data[2] =  IDACL
    data[3] =  IDACH
    data[4] =  MASDACL
    data[5] =  MASDACH
    data[6] =  TIMEOUT (100ms)
    data[7] =  [x,hs,swb,swa]
    data[8] =  grid;
    data[9] =  maxV
    data[10] = minV
    data[11] = maxI
    data[12] = minI
    data[13] = num pre-pulses
    data[14] = num samples
    data[15] = tomo mode (0=Narrow, 1=Wide 2=Calib 3=Intermediate)
    data[16] = Compressore sblocco
    data[17] = Dead man attivo

Autore: M. Rispoli
Data: 04/11/2014
Modificato: 
*/

//////////////////////////////////////////////////////////////////////////////
void mcc_raggi_tomo(void)
{
  unsigned char chk,i;
  unsigned char data[16];
  int timeout;
  
    // Verifica se la sequenza è partita
    if(tomoIsRunning)
    {
      debugPrint("GUI ATTESA FINE SEQUENZA TOMO RX IN ESECUZIONE");
      timeout = 30;
      while(tomoIsRunning)
      {
        if(timeout-- == 0) 
        {
          data[0]=ERROR_WAIT_PENDING_XRAY;       
          data[1]= 0; 
          data[2]= 0 ;  
          mccGuiNotify(1,MCC_CMD_RAGGI_TOMO,data,3);
          return;
        }
        _time_delay(1000);
      }
    }

    tomoParam.config=0; // Esposizione con detector
    tomoParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    tomoParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    tomoParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    tomoParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    tomoParam.esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    tomoParam.esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    tomoParam.esposizione.MINHV=mcc_cmd.buffer[10];
    tomoParam.esposizione.MAXHV=mcc_cmd.buffer[9];
    tomoParam.esposizione.MINI=mcc_cmd.buffer[12];
    tomoParam.esposizione.MAXI=mcc_cmd.buffer[11];
    tomoParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &tomoParam.esposizione) [i];
    tomoParam.esposizione.CHK= chk;

    // ACCESSORI ALL'ESPOSIZIONE
    tomoParam.tomo_pre_pulses = mcc_cmd.buffer[13];      // Numero di impulsi da scartare
    tomoParam.tomo_samples = mcc_cmd.buffer[14];         // Numero totale di impulsi
    tomoParam.tomo_mode = mcc_cmd.buffer[15];            // Modalità Narrow/Wide
    tomoParam.tomo_skip = mcc_cmd.buffer[16];            // Modalità Narrow/Wide

    // STUB
    //if(tomoParam.tomo_skip) tomoParam.tomo_samples= 2 * tomoParam.tomo_samples -1;

    // Controllo sullo sblocco compressore dopo esame
    if(mcc_cmd.buffer[17]) tomoParam.compressor_unlock = TRUE;
    else tomoParam.compressor_unlock = FALSE;

    if(mcc_cmd.buffer[18]==1)  tomoParam.tomo_deadman = true;      // Modalità deadman
    else tomoParam.tomo_deadman = false;
    

    // Partenza sequenza
    _EVCLR(_SEQEV_RX_TOMO_TERMINATED);
    _EVSET(_SEQEV_RX_TOMO_START);
    
  return;
}

void mcc_raggi_aec_tomo(void)
{
  unsigned char chk,i;
  unsigned char data[16];

  int timeout;
  
    // Verifica se la sequenza è partita
    if(tomoAecIsRunning)
    {
      debugPrint("GUI ATTESA FINE SEQUENZA AEC TOMO IN ESECUZIONE");
      timeout = 30;
      while(tomoAecIsRunning)
      {
        if(timeout-- == 0) 
        {
          data[0]=ERROR_WAIT_PENDING_XRAY;       
          data[1]= 0; 
          data[2]= 0 ;  
          mccGuiNotify(1,MCC_CMD_RAGGI_AEC_TOMO,data,3);
          return;
        }
        _time_delay(1000);
      }
    }

    tomoAecParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[0]+256*mcc_cmd.buffer[1]));   // Vdac
    tomoAecParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[7]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    tomoAecParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[2]+256*mcc_cmd.buffer[3]));    // Idac
    tomoAecParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));       // Aggiunge la tensione di griglia
    tomoAecParam.esposizione.MAS=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];               // MAS-DAC
    tomoAecParam.esposizione.TMO=mcc_cmd.buffer[6];                                     // Timout Esposizione
    tomoAecParam.esposizione.MINHV=mcc_cmd.buffer[10];
    tomoAecParam.esposizione.MAXHV=mcc_cmd.buffer[9];
    tomoAecParam.esposizione.MINI=mcc_cmd.buffer[12];
    tomoAecParam.esposizione.MAXI=mcc_cmd.buffer[11];
    tomoAecParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &tomoAecParam.esposizione) [i];
    tomoAecParam.esposizione.CHK= chk;

    // ACCESSORI ALL'ESPOSIZIONE
//    tomoAecParam.potter_cfg = _POTTER_TOMO;         
    // ACCESSORI ALL'ESPOSIZIONE
    tomoAecParam.tomo_pre_pulses = mcc_cmd.buffer[13];      // Numero di impulsi da scartare
    tomoAecParam.tomo_samples = mcc_cmd.buffer[14];         // Numero totale di impulsi
    tomoAecParam.tomo_mode = mcc_cmd.buffer[15];            // Modalità Narrow/Wide
    tomoAecParam.tomo_skip = mcc_cmd.buffer[16];            // Modalità Narrow/Wide

    // Controllo sullo sblocco compressore dopo esame
    if(mcc_cmd.buffer[17]) tomoAecParam.compressor_unlock = TRUE;
    else tomoAecParam.compressor_unlock = FALSE;

    if(mcc_cmd.buffer[18]==1)  tomoAecParam.tomo_deadman = true;      // Modalità deadman
    else tomoAecParam.tomo_deadman = false;
    
    // Partenza sequenza
    _EVCLR(_SEQEV_RX_TOMO_AEC_TERMINATED);
    _EVSET(_SEQEV_RX_TOMO_AEC_START);
    
  return;
}




/*
  IMPOSTA LA CONFIGURAZIONE DEI REGISTRI PER IL TOOL
  DI GESTIONE DELLA ROTAZIONE
*/
void mccSetRotationToolConfig()
{
   _SystemOutputs_Str output, mask;    
   _DeviceAppRegister_Str        ConfList;

    
    debugPrint("GUI CONFIGURAZIONE PER TOOL ROTAZIONI");
}

/*_____________________________________________________________________________

    Richiesta angolo Braccio, Tubo e inclinometro.
    I datri restituiti sono quelli aggiornati dai rispettivi drivers

    buffer[0,1] = ARM
    buffer[2,3] = TRX
    buffer[4,5] = INCLINOMETRO

    !!! Tutti i dati sono espressi in decimi di grado


    TO BE DONE: verificare che il formato dei dati sia
    in decimi di grado anche sul richiedente

*/
void mccGetGonio(unsigned char id, unsigned char mcccode)
{
    unsigned char buffer[6];

    TO_LE16(&buffer[0],generalConfiguration.armExecution.dAngolo);
    TO_LE16(&buffer[2],generalConfiguration.trxExecution.cAngolo);
    TO_LE16(&buffer[4],generalConfiguration.armExecution.dAngolo_inclinometro);

    mccGuiNotify(1,mcccode, buffer, 6);
    return ;
    
}

void mcc_set_mirror(unsigned char id, unsigned char mcccode,unsigned char cmd)
{
  unsigned char data[2];
 
  if(cmd==0){
      // Home
      debugPrint("GUI ESECUZIONE MIRROR HOME");
      if(pcb249U2MirrorHome()==TRUE){
          debugPrint("GUI ESECUZIONE MIRROR HOME COMPLETATA");
          data[0]=1;
      }else{
          debugPrint("GUI ESECUZIONE MIRROR HOME FALLITA");
          data[0] = 0;
      }
  }else{
      // Out
      debugPrint("GUI ESECUZIONE MIRROR OUT");
      if(pcb249U2MirrorOut()==TRUE){
          debugPrint("GUI ESECUZIONE MIRROR OUT COMPLETATA");
          data[0]=1;
      }else{
          debugPrint("GUI ESECUZIONE MIRROR OUT FALLITA");
          data[0] = 0;
      }
  }


  // Consulta il registro RG249U2_MIRROR_STAT per lo stato corrente dello specchio
  if(_TEST_BIT(PCB249U2_MIR_HOME_FLG)) data[1]=0;
  else if(_TEST_BIT(PCB249U2_MIR_OUT_FLG)) data[1]=1;
  else data[1]=2;

  mccGuiNotify(id,mcccode,data,2);
  return; 
}
 
/*
    data[0] =  cmd;
    data[1] =  timeout;
    data[2] =  steps specchio L
    data[3] = steps specchio H
*/
void mcc_set_lamp(unsigned char id, unsigned char mcccode)
{
  unsigned char data[3];
  unsigned short steps;
  int attempt = 5;
 

  // Preconfigura gli steps dello specchio
  steps = mcc_cmd.buffer[2] + mcc_cmd.buffer[3] * 256;
  data[0]=0;
  if(Ser422WriteRegister(_REGID(RG249U2_PR_MIRROR_STEPS),steps,10,&PCB249U2_CONTEST) == _SER422_NO_ERROR)
  {
    // Ripete più volte il tentativo di comando
    while(attempt){
        if(pcb249U2Lamp(mcc_cmd.buffer[0],mcc_cmd.buffer[1],TRUE)==true){
            data[0]=1;
            break;
        }
        printf("Tentativo comando lampada:%d ..\n",attempt);
        _time_delay(1000);
        attempt --;
    }
    if(data[0]==0){
        debugPrint("GUI FALLITO COMANDO LAMPADA CENTRATORE");
    }

  }else{
      debugPrint("GUI FALLITO SCRITTURA STEPS PER SPECCHIO");
  }

  // Consulta il registro RG249U2_MIRROR_STAT per lo stato corrente dello specchio
  if(_TEST_BIT(PCB249U2_MIR_HOME_FLG)) data[2]=0;
  else if(_TEST_BIT(PCB249U2_MIR_OUT_FLG)) data[2]=1;
  else data[2]=2;
  
  if(_TEST_BIT(PCB249U2_LAMP_ON_FLG)) data[1]=1;
  else data[1]=0;

  mccGuiNotify(id,mcccode,data,3);
  
  if(data[0]){
      debugPrintI("GUI COMANDO LAMPADA COMPLETATO, COMANDO", mcc_cmd.buffer[0]);
  }
  return; 

}

/*
  BUFFER[0] == Fuoco da attivare:
  - 0 = F1G
  - 1 = F2G
  - 2 = F1P
  - 3 = F2P
  - 4 = FUOCO SPENTO

  NOTIFY: BUFFER[0] = RISULTATO
*/
void  mccSetFuoco(unsigned char id, unsigned char mcccode)
{
  bool ris;
  unsigned char data[1];
  
  printf("ESEGUE FUOCO %d\n",mcc_cmd.buffer[0]);
  switch(mcc_cmd.buffer[0])
  {
  case 0: ris = pcb190SetFuoco(PCB190_F1G);break;
  case 1: ris = pcb190SetFuoco(PCB190_F2G);break;
  case 2: ris = pcb190SetFuoco(PCB190_F1P);break;
  case 3: ris = pcb190SetFuoco(PCB190_F2P);break;
  default: ris = pcb190SetFuoco(0);break;               // Spegne il filamento
  }
   
  if(ris) data[0]=1;
  else data[0]=0;
  
  mccGuiNotify(id,mcccode,data,1);
}

/*
  mcc_cmd.buffer[0] = filtro
  mcc_cmd.buffer[1] = posizione target filtro
*/  
void mccSetFiltro(void)
{

  debugPrintI2("GUI COMANDO POSIZIONAMENTO FILTRO, INDEX",mcc_cmd.buffer[0], "POS",mcc_cmd.buffer[1]);
  pcb249U2SetFiltro(mcc_cmd.buffer[0],mcc_cmd.buffer[1], mcc_cmd.id);

}

// Effettua una nuova calibrazione del filtro, aggiornando la relativa configurazione
//   buffer[0] = filtro posizione 0
//   buffer[1] = filtro posizione 1
//   buffer[2] = filtro posizione 2
//   buffer[3] = filtro posizione 3
void mccSetCalibFiltro(void)
{
  bool ris;
  unsigned char data;
  
  generalConfiguration.colli_filter[0]=mcc_cmd.buffer[0];
  generalConfiguration.colli_filter[1]=mcc_cmd.buffer[1];
  generalConfiguration.colli_filter[2]=mcc_cmd.buffer[2];
  generalConfiguration.colli_filter[3]=mcc_cmd.buffer[3];           

  // Attiva il flag di aggiornamento e esce: l'aggiornamento verrà effettuato dal driver.
  generalConfiguration.colli_filter_update = true;
  return;
}

/*__________________________________________________________
  Impostazione collimazione 2D
NOTIFY:
    data[0] = Successo/Insuccesso

*/
void  mccSetColli(unsigned char id, unsigned char mcccode)
{  
  unsigned char data[1];
  
   debugPrint("GUI SCRITTURA LAME PER COLLIMAZIONE 2D");

    // Deve aspettare che le sequenze raggi terminino
   if(pcb249U1_GetFreeze() || pcb249U2_GetFreeze()){
       printf("BISOGNA ASPETTARE!\n");
   }
   int i = 50;
   while(pcb249U1_GetFreeze() || pcb249U2_GetFreeze()){
       _time_delay(100);
       if(i==0) break;
       i--;
   }

  generalConfiguration.colliCfg.colliDinamicaAbilitata = FALSE;
  
  // Assegnazione dell'impostazione statica delle lame
  generalConfiguration.colliCfg.lame2D.front = mcc_cmd.buffer[COLLI_F];
  generalConfiguration.colliCfg.lame2D.back = mcc_cmd.buffer[COLLI_B];
  generalConfiguration.colliCfg.lame2D.trap = mcc_cmd.buffer[COLLI_T];
  generalConfiguration.colliCfg.lame2D.left = mcc_cmd.buffer[COLLI_L];
  generalConfiguration.colliCfg.lame2D.right = mcc_cmd.buffer[COLLI_R];
  

  // Richiede l'esecuzione del posizionamento delle lame frontali e posteriori
  // Il comando non può fallire poichè semplicemente sovrascrive uno stato in corso d'opera..
  // Questo dovrebbe impedire una catena di continue collimazioni dovute al cambio di compressore
  // SOltanto l'ultimo stato vince
  pcb249U2SetColli( generalConfiguration.colliCfg.lame2D.back , generalConfiguration.colliCfg.lame2D.front,id);
  pcb249U1SetColli(generalConfiguration.colliCfg.lame2D.left,generalConfiguration.colliCfg.lame2D.right,generalConfiguration.colliCfg.lame2D.trap,id);

}

/*
    FUNZIONE DI LANCIO SHOT RX SENZA DETECTOR
    data[0] = (unsigned char) fuoco;
    data[1] = (unsigned char) (Vdac&0xFF);
    data[2] = (unsigned char) ((Vdac>>8)&0xFF);
    data[3] = (unsigned char) (Idac&0xFF);
    data[4] = (unsigned char) ((Idac>>8)&0xFF);
    data[5] = (unsigned char) ((mAs*50)&0xFF);
    data[6] = (unsigned char) (((mAs*50)>>8)&0xFF);
    data[7] = TIMEOUT (100ms)
    data[8] = [x,hs,swb,swa]
    data[9] = grid;
    data[10]= vraw max
    data[11]= vraw min
    data[12]= iraw max
    data[13]= iraw min

*/
void mcc_manual_rx_shot(void)
{
  bool ris1,ris2,ris3,ris4;
  unsigned char chk,i;
  int timeout;
  unsigned char data[10];
  

    // Verifica se la sequenza è partita
    if(rxStdIsRunning)
    {
      debugPrint("GUI MANUAL RX, ATTESA FINE SEQUENZA RX IN ESECUZIONE");
      timeout = 30;
      while(rxStdIsRunning)
      {
        if(timeout-- == 0)
        {
          data[0]=ERROR_WAIT_PENDING_XRAY;
          data[1]= 0;
          data[2]= 0 ;
          mccGuiNotify(1,MCC_CMD_RAGGI_STD,data,sizeof(data));
          return;
        }
        _time_delay(1000);
      }
    }


    switch(mcc_cmd.buffer[0])
    {
    case 0: ris1 = pcb190SetFuoco(PCB190_F1G);break;
    case 1: ris1 = pcb190SetFuoco(PCB190_F2G);break;
    case 2: ris1 = pcb190SetFuoco(PCB190_F1P);break;
    case 3: ris1 = pcb190SetFuoco(PCB190_F2P);break;
    }

    // Preparazione parametri sequenza
    rxStdParam.config=1; // Esposizione senza detector
    
    rxStdParam.esposizione.TMO=mcc_cmd.buffer[7];  // Timout Esposizione
    rxStdParam.esposizione.HV=(0x0fff & (mcc_cmd.buffer[1]+256*mcc_cmd.buffer[2]));   // Vdac
    rxStdParam.esposizione.HV|=(0x7000&((unsigned short)mcc_cmd.buffer[8]<<12));      // Aggiunge lo stato degli switch generatore + velocità starter
    rxStdParam.esposizione.I=(0x0FFF & (mcc_cmd.buffer[3]+256*mcc_cmd.buffer[4]));    // Idac
    rxStdParam.esposizione.I|=(0x7000&((unsigned short)mcc_cmd.buffer[9]<<12));       // Aggiunge la tensione di griglia
    rxStdParam.esposizione.MAS=mcc_cmd.buffer[5]+256*mcc_cmd.buffer[6];               // MAS-DAC
    rxStdParam.esposizione.MINHV=mcc_cmd.buffer[11];
    rxStdParam.esposizione.MAXHV=mcc_cmd.buffer[10];
    rxStdParam.esposizione.MINI=mcc_cmd.buffer[13];
    rxStdParam.esposizione.MAXI=mcc_cmd.buffer[12];
    rxStdParam.esposizione.CHK=0; // !!!!! DEVE ESSERE IMPOSTATO A ZERO PRIMA DEL CALCOLO
    chk=0;// Calcolo checksum sui parametri di esposizione
    for(i=0; i< sizeof(_RxParam_Str); i++) chk^=((unsigned char*) &rxStdParam.esposizione) [i];
    rxStdParam.esposizione.CHK= chk;

    // ACCESSORI ALL'ESPOSIZIONE
    rxStdParam.potter_cfg = 0;         // No Potter
   
    // Partenza sequenza
   _EVCLR(_SEQEV_RX_STD_TERMINATED);
   _EVSET(_SEQEV_RX_STD_START);
   return;
}

void mcc_test(void)
{   
    debugPrint("GUI TEST COMMAND");
    return;
}


/*
  ATTIVAZIONE STARTER IAE CON I SEGUENTI PARAMETRI
- 0 : STOP STARTER
- 1 : START LOW SPEED
- 2 : START HIGH SPEED
- 3 : OFF STARTER
*/
void mcc_set_starter(void)
{
  
  // Reset fault sulla pcb190
  pcb190ResetFault();
  if(mcc_cmd.buffer[0]==0) pcb190StopStarter();
  else if(mcc_cmd.buffer[0]==1) pcb190StarterL();
  else if(mcc_cmd.buffer[0]==2)pcb190StarterH();
  else if(mcc_cmd.buffer[0]==3)pcb190OffStarter();
}

/*
  Attiva il rodaggio del tubo
  data[0] = num cicli
  data[1] = angolo 
  data[2] = velocita: 0 = STD, 1 = WIDE, 2 = NARROW
*/
void srv_rodaggio_tubo(int id, unsigned char* data,int len)
{
// TO BE DONE
/*
  generalConfiguration.trxloop_cicli =data[0]; 
  generalConfiguration.trxloop_seq = 0;
  generalConfiguration.trxloop = true; 
*/
}



//_______________________________________________________________________________
// Sottofamiglia di comandi di Service
void srv_freeze_mode(int id,unsigned char* data, int len);
void srv_send(int id,unsigned char* data, int len);
void srv_rodaggio_tubo(int id, unsigned char* data,int len);
void mcc_service_commands(int id,int subcmd,unsigned char* data,int len)
{

  switch(subcmd)
  {
    case SRV_TEST_LS_STARTER:
      // Effettua uno Start/Stop dello Starter
      pcb190TestLSStarter();
      break;
    case SRV_SERIAL_SEND:
        srv_send(id,data,len);
    break;
    case SRV_RODAGGIO_TUBO:
        srv_rodaggio_tubo(id,data,len);
    break;
    case SRV_ARM_STOP: // Blocca qualsiasi movimento del braccio in corso
        generalConfiguration.trxExecution.run=false;
        generalConfiguration.armExecution.run=false;
        generalConfiguration.trxExecution.test=false;
        generalConfiguration.armExecution.test=false;
        //actuatorsStop();
    break;
    case  SRV_START_POTTER_3D_GRID:
        if( generalConfiguration.potterCfg.potId != POTTER_TOMO){
            printf("VC COMMAND FAILURE: No Potter Tomo available!\n");
            return;
        }
        pcb244StartVoiceCoil();
    break;

    case  SRV_STOP_POTTER_3D_GRID:
      if( generalConfiguration.potterCfg.potId != POTTER_TOMO){
          printf("VC COMMAND FAILURE: No Potter Tomo available!\n");
          return;
      }
      pcb244StopVoiceCoil();
    break;

    case SRV_PCB244_CMD_FREQ_VC:
      if( generalConfiguration.potterCfg.potId != POTTER_TOMO){
        printf("VC COMMAND FAILURE: No Potter Tomo available!\n");
        return;
      }
      if(len!=1) printf("SERVICE VC SET FREQ PARAMETER FAILURE!\n");
      else  pcb244SetFreqVoiceCoil(data[0]);
        
      break;
    case SRV_PCB244_CMD_AMPL_VC: 
      if( generalConfiguration.potterCfg.potId != POTTER_TOMO){
        printf("VC COMMAND FAILURE: No Potter Tomo available!\n");
        return;
      }
      if(len!=1) printf("SERVICE VC SET AMPLITUDE PARAMETER FAILURE!\n");
      else  pcb244SetAmplVoiceCoil(data[0]);
      break;

      case  SRV_START_POTTER_2D_GRID:
          if( generalConfiguration.potterCfg.potId != POTTER_2D){
              printf("COMMAND FAILURE: No Potter 2D available!\n");
              return;
          }
          pcb244Start2d();
      break;

      case  SRV_STOP_POTTER_2D_GRID:
          if( generalConfiguration.potterCfg.potId != POTTER_2D){
              printf("COMMAND FAILURE: No Potter 2D available!\n");
              return;
          }
          pcb244Stop2d();
      break;

      case  SRV_RESET_FAULT_PCB244:
          pcb244ResetFaults();
      break;

      case  SRV_RESET_PCB244:
          pcb244ResetBoard();
      break;

  }
}



// COMANDA L'ATTIVAZIONE DEL MODO FREEZE (ON/OFF)
// data[0] = 1->ON, 0->OFF
// RISPOSTA:
// BUFFER[0]= RISULTATO
// BUFFER[1] = STATO ESEGUITO
void srv_freeze_mode(int id,unsigned char* data, int len)
{       
  bool risultato;
  unsigned char buffer[2];
  if(data[0])
  {
    // Disabilita tutti i drivers ed attende che tutti i driver siano fermi
    if(Ser422DriverFreezeAll(4000)==FALSE) risultato=FALSE;
    else risultato=TRUE;
  }else
  {
    // Sblocca i drivers dallo stato di freeze
    if(Ser422DriverSetReadyAll(5000) == FALSE) risultato=FALSE;
    else risultato=TRUE;
  }
    
  buffer[0] = risultato;
  buffer[1] = data[0];
  mccServiceNotify(id,SRV_FREEZE_DEVICE,buffer,2);
  return;
}

// COMANDA L'ATTIVAZIONE DEL MODO FREEZE (ON/OFF)
// data[0] = Addr, data[1] = dato1 data[2]=dato2
// RISPOSTA:
// BUFFER[0][1][2] Frame di risposta se c'è
void srv_send(int id,unsigned char* data, int len)
{       
  unsigned char buffer[3];

  printf("SERVICE SEND RAW: %d %d %d \n", data[0], data[1], data[2]);
  Ser422SendRaw(data[0], data[1], data[2], buffer, 10);

  mccServiceNotify(id,SRV_SERIAL_SEND,buffer,3);
  return;
}




/* COMANDI VARI DURANTE LA CALIBRAZIONE DEL COMPRESSORE 
    mcc_cmd.buffer[0] = COMANDO
    0 = DISATTIVAZIONE CALIBRAZIONE
    1 = ATTIVAZIONE CALIBRAZIONE
        Entrando in calibrazione, vengono modificati alcuni registri della PCB215
        per favorire la calibrazione stessa.
    2 = IMPOSTAZIONE NUOVA CALIBRAZIONE NACCHERA
        buffer[1,2] = OFFSET, buffer[3] = K
    3 = IMPOSTAZIONE NUOVA CALIBRAZIONE FORZA
        buffer[1] = F0, buffer[2,3] = KF0, buffer[4] = F1, buffer[5] = KF1
    4 = IMPOSTAZIONE NUOVA CALIBRAZIONE PADS
       buffer[1] = pad, buffer[2,3] = Offset(int), buffer[4] = kF, buffer[5] = Peso
*/
void mcc_pcb215_calibration(void)
{
  switch(mcc_cmd.buffer[0])
  {
  case 0: // Disattivazione modo calibrazione        
          printf("PCB215 EXIT CALIB MODE\n");
          pcb215ActivateCalibMode(FALSE);
    break;
  case 1: // Attivazione modo calibrazione e impostazione particolare dei registri di calibrazione    
          printf("PCB215 ENTERING CALIB MODE\n");
          pcb215ActivateCalibMode(TRUE);
    break;
  case 2: // Configurazione Nacchera
          printf("CONFIGURAZIONE NACCHERA\n");
          generalConfiguration.comprCfg.calibration.calibPosOfs = mcc_cmd.buffer[1] + 256 * mcc_cmd.buffer[2];
          generalConfiguration.comprCfg.calibration.calibPosK = mcc_cmd.buffer[3];
          pcb215ConfigNacchera(FALSE);
          pcb215ForceUpdateData(); // Forza il rinnovo dei parametri di gestione del pad
    break;
    case 3: // Configurazione calibrazione forza
          printf("CONFIGURAZIONE FORZA\n");
          generalConfiguration.comprCfg.calibration.F0 = mcc_cmd.buffer[1];
          generalConfiguration.comprCfg.calibration.KF0 = mcc_cmd.buffer[2] + mcc_cmd.buffer[3] * 256;
          generalConfiguration.comprCfg.calibration.F1 = mcc_cmd.buffer[4];
          generalConfiguration.comprCfg.calibration.KF1 = mcc_cmd.buffer[5];
          pcb215ConfigForza(FALSE);
          pcb215ForceUpdateData(); // Forza il rinnovo dei parametri di gestione del pad
    break;
    case 4: // Configurazione pads
          printf("CONFIGURAZIONE PAD: %d\n",mcc_cmd.buffer[1]);
          generalConfiguration.comprCfg.calibration.pads[mcc_cmd.buffer[1]].offset = (int) (mcc_cmd.buffer[2] + mcc_cmd.buffer[3] * 256);
          generalConfiguration.comprCfg.calibration.pads[mcc_cmd.buffer[1]].kF = mcc_cmd.buffer[4];
          generalConfiguration.comprCfg.calibration.pads[mcc_cmd.buffer[1]].peso = mcc_cmd.buffer[5];
          printf("CONFIGURAZIONE PAD: %d offset=%d, kF=%d, Peso=%d\n",
                  mcc_cmd.buffer[1],
                  generalConfiguration.comprCfg.calibration.pads[mcc_cmd.buffer[1]].offset,
                  generalConfiguration.comprCfg.calibration.pads[mcc_cmd.buffer[1]].kF,
                  generalConfiguration.comprCfg.calibration.pads[mcc_cmd.buffer[1]].peso);
          
          pcb215ForceUpdateData(); // Forza il rinnovo dei parametri di gestione del pad
    
    break;

  }
  
}

/*
CODICI COMANDO BIOPSIA (DA GUI A M4): MCC_BIOPSY_CMD
#define _MCC_BIOPSY_CMD_MOVE_HOME   1
#define _MCC_BIOPSY_CMD_MOVE_XYZ    2
#define _MCC_BIOPSY_CMD_MOVE_INCX   3
#define _MCC_BIOPSY_CMD_MOVE_DECX   4
#define _MCC_BIOPSY_CMD_MOVE_INCY   5
#define _MCC_BIOPSY_CMD_MOVE_DECY   6
#define _MCC_BIOPSY_CMD_MOVE_INCZ   7
#define _MCC_BIOPSY_CMD_MOVE_DECZ   8
#define _MCC_BIOPSY_CMD_SET_STEPVAL 9

*/
void mccBiopsyCmd(void)
{
  unsigned short X,Y,Z,val;

  if(generalConfiguration.biopsyCfg.biopsyConnected==FALSE) return;

  switch(mcc_cmd.buffer[0]){
  case _MCC_EXT_BIOPSY_CMD_MOVE_HOME:
      //biopsyMoveHome();
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_XYZ:
      X=mcc_cmd.buffer[1]+256*mcc_cmd.buffer[2];
      Y=mcc_cmd.buffer[3]+256*mcc_cmd.buffer[4];
      Z=mcc_cmd.buffer[5]+256*mcc_cmd.buffer[6];
      biopsyExtendedMoveXYZ(X, Y,Z);
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_INCX:
      //biopsyStepIncX();
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_DECX:
      //biopsyStepDecX();
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_INCY:
      //biopsyStepIncY();
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_DECY:
      //biopsyStepDecY();
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_INCZ:
      //biopsyStepIncZ();
      break;
  case _MCC_EXT_BIOPSY_CMD_MOVE_DECZ:
      //biopsyStepDecZ();
      break;
  case _MCC_EXT_BIOPSY_CMD_SET_STEPVAL:
      generalConfiguration.biopsyCfg.extendedConf.stepVal =  mcc_cmd.buffer[1];
      break;
  case _MCC_EXT_BIOPSY_CMD_SET_BUZZER:
      BiopsyDriverSetBuzzer();
      break;
  }

  return;
}

void mccBiopsySimulator(void){
#ifdef __BIOPSY_SIMULATOR
    unsigned short val,X,Y,Z;

    if(mcc_cmd.buffer[0]== _BYM_SIM_CONNECTION){ // Impostazione stato della connessione
        printf("BIOPSY SIMULATOR: EXEC SIM CONNECTION = %d\n",mcc_cmd.buffer[1]);
        if(mcc_cmd.buffer[1]==1) SimConnessione(true);
        else SimConnessione(false);
    }else if(mcc_cmd.buffer[0]== _BYM_SIM_SBLOCCO){ // Impostazione stato del pulsante di sblocco
        printf("BIOPSY SIMULATOR: EXEC SIM SBLOCCO = %d\n", mcc_cmd.buffer[1]);
        if(mcc_cmd.buffer[1]==1) SimSetPush(true);
        else SimSetPush(false);
    }else if(mcc_cmd.buffer[0]== _BYM_SIM_ADAPTER){ // Impostazione Adapter Id
        printf("BIOPSY SIMULATOR: EXEC SIM ADAPTER = %d\n", mcc_cmd.buffer[1]);
        SimSetAdapter(mcc_cmd.buffer[1]);
    }else if(mcc_cmd.buffer[0]== _BYM_SIM_LAT){ // Impostazione Lateralità
        printf("BIOPSY SIMULATOR: EXEC SIM LATX = %d\n", mcc_cmd.buffer[1]);
        SimSetLatX(mcc_cmd.buffer[1]);
    }else if(mcc_cmd.buffer[0]== _BYM_SIM_SH){ // Impostazione Asse SH
        val = mcc_cmd.buffer[1] + 256 * mcc_cmd.buffer[2];
        printf("BIOPSY SIMULATOR: EXEC SIM SH, VAL = %d\n", val);
        SimSetSH((int) val );
    }else if(mcc_cmd.buffer[0]== _BYM_SIM_SET_CURSOR){ // Impostazione Assi X,Y,Z
        X = mcc_cmd.buffer[1] + 256 * mcc_cmd.buffer[2];
        Y = mcc_cmd.buffer[3] + 256 * mcc_cmd.buffer[4];
        Z = mcc_cmd.buffer[5] + 256 * mcc_cmd.buffer[6];
        printf("BIOPSY SIMULATOR: EXEC SIM SET XYZ, X = %d, Y=%d, Z=%d \n", X,Y,Z);
        SimSetXYZ(X,Y,Z);
    }
#endif
    return;
}


// Questo comando restituisce il valore della posizione del piano di compressione
void mcc_pcb215_get_trolley(void){
    unsigned char data;

    // Chiede alla 215 la posizione del piano compressore
    // Notifica esito impostazione filtro
    int posizione = pcb215GetSpessoreNonCompresso();
    if(posizione>255) posizione = 255;
    data = (unsigned char) posizione;
    mccGuiNotify(mcc_cmd.id,mcc_cmd.cmd,&data,1);

}

uint32_t getU32val(unsigned char* buf){
    return buf[0] + 256 * buf[1] + 256 * 256 * buf[2] + 256 * 256 * 256 * buf[3];
}

void mcc_canopen(void)
{
  unsigned char buffer[8];

  buffer[0]= ACTUATORS_CMD_TEST;
  memcpy(&buffer[1],mcc_cmd.buffer,7);
  CanSendToActuatorsSlave(buffer);

}

//______________________________________________________________________________________________________________
//                      CALIBRAZIONE ZERO MECCANICO E GONIO
// Istruzioni per la calibrazione degli zeri della macchina
// mcc_cmd.id
// mcc_cmd.cmd = MCC_CALIB_ZERO
// mcc_cmd.buffer[0] = comando specifico
void mcc_calib_zero(void){
    unsigned char data[2];
    data[0] = mcc_cmd.buffer[0];
    data[1] = 255; // Comando accettato

    if(mcc_cmd.buffer[0] == CALIB_ZERO_MANUAL_ACTIVATION_TRX_CALIB){
        generalConfiguration.manual_mode_activation = _MANUAL_ACTIVATION_TRX_CALIB;
        debugPrintI("GUI SELEZIONATA MODALITA DI MOVIMENTO MANUALE, CODE",generalConfiguration.manual_mode_activation);
    }else if(mcc_cmd.buffer[0] == CALIB_ZERO_MANUAL_ACTIVATION_ARM_CALIB){
        generalConfiguration.manual_mode_activation = _MANUAL_ACTIVATION_ARM_CALIB;
        debugPrintI("GUI SELEZIONATA MODALITA DI MOVIMENTO MANUALE, CODE",generalConfiguration.manual_mode_activation);
    }else if(mcc_cmd.buffer[0] == CALIB_ZERO_MANUAL_ACTIVATION_TRX_STANDARD){
        generalConfiguration.manual_mode_activation = _MANUAL_ACTIVATION_TRX_STANDARD;
        debugPrintI("GUI SELEZIONATA MODALITA DI MOVIMENTO MANUALE, CODE",generalConfiguration.manual_mode_activation);
    }else if(mcc_cmd.buffer[0] == CALIB_ZERO_MANUAL_ACTIVATION_ARM_STANDARD){
        generalConfiguration.manual_mode_activation = _MANUAL_ACTIVATION_ARM_STANDARD;
        debugPrintI("GUI SELEZIONATA MODALITA DI MOVIMENTO MANUALE, CODE",generalConfiguration.manual_mode_activation);
    }else if(mcc_cmd.buffer[0] == CALIB_ZERO_ACTIVATE_TRX_ZERO_SETTING){
        debugPrint("GUI TRX ZERO SETTING CALIBRATION");
        generalConfiguration.trxExecution.id = mcc_cmd.id;
        actuatorsTrxActivateZeroSetting();
    }else if(mcc_cmd.buffer[0] == CALIB_ZERO_ACTIVATE_ARM_ZERO_SETTING){
        debugPrint("GUI ARM ZERO SETTING CALIBRATION");
    }else if(mcc_cmd.buffer[0] == CALIB_ZERO_ACTIVATE_GONIO_ZERO_SETTING){

       // Ferma tutti i driver per non sollecitare il gonio
       Ser422DriverFreezeAll(5000);
       debugPrint("GUI COMANDO RESET INCLINOMETRO");

       // Carica i registri per impostare l'angolo
       if(pcb249U1ResetGonio(0)==FALSE) data[1]=0;
       else data[1]=1;
       _time_delay(500);
       Ser422DriverSetReadyAll(5000);
       if(data[1]) debugPrint("GUI RESET INCLINOMETRO COMPLETATO");
       else   debugPrint("GUI RESET INCLINOMETRO FALLITO");
    }

    // Feedback di ricezione
    mccGuiNotify(mcc_cmd.id,mcc_cmd.cmd,data,sizeof(data));


}

void mccResetGonio(unsigned char id, unsigned char mcccode)
{
   unsigned char risultato;

  // Ferma tutti i driver per non sollecitare il gonio
  Ser422DriverFreezeAll(5000);

  debugPrint("GUI COMANDO RESET INCLINOMETRO");

  // Carica i registri per impostare l'angolo
  if(pcb249U1ResetGonio(0)==FALSE) risultato=0;
  else risultato=1;
  _time_delay(500);
  Ser422DriverSetReadyAll(5000);

  if(risultato) debugPrint("GUI RESET INCLINOMETRO COMPLETATO");
  else   debugPrint("GUI RESET INCLINOMETRO FALLITO");

  mccGuiNotify(id,mcccode,&risultato,1);
  return ;
}



// Procedura per il resume dal parking mode
void mcc_parking_mode(void)
{
    unsigned char buffer[2];
    buffer[0] = MCC_PARKING_MODE_COMMANDS_START_PARKING;

    debugPrint("GUI PARKING MODE REQUEST");

    // Posizionament del Tilt a 0
    actuatorsTrxMove(0);

    for(int i=0; i<10; i++){
        if(!generalConfiguration.trxExecution.completed) _time_delay(1000);
        else break;
    }
    if(!generalConfiguration.trxExecution.success){
        // Errore timeout posizionamento lenze
        debugPrint("GUI TIMEOUT TRX PARKING");
        buffer[1] = ERROR_PARKING_TILT_SETTING;
        mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
        return;
    }


    // Posizionamento del Braccio in posizione Alta per assicurare una corretta rotazione del braccio
    if(generalConfiguration.gantryCfg.armMotor){
            if(generalConfiguration.armExecution.lenze_run){
                debugPrint("GUI LENZE BUSY DURING SAFE POSITIONING");
                buffer[1] = ERROR_PARKING_LENZE_BUSY;
                mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
                return;
            }
            actuatorsLenzeUnpark();

            // Attesa fine movimento di parcheggio
            for(int i=0; i<60; i++){
                if(generalConfiguration.armExecution.lenze_run) _time_delay(1000);
                else break;
            }

            if(generalConfiguration.armExecution.lenze_run){
                // Errore timeout posizionamento lenze
                debugPrint("GUI TIMEOUT LENZE IN SAFE POSITIONING");
                buffer[1] = ERROR_PARKING_LENZE_TMO;
                mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
                return;
            }

            // Verifica se il potenziometro ha raggiunto il target atteso
            if(generalConfiguration.armExecution.lenze_pot < generalConfiguration.lenzeCfg.parkingSafePoint){
                debugPrint("GUI LENZE NOT CORRECTLY POSITIONED");
                buffer[1] = ERROR_PARKING_LENZE_POSITION;
                mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
                return;
            }

            debugPrint("GUI ARM ROTATION FOR PARKING");

            // Attivazione Arm a 180 in modo parking (senza correzione altezza)
            if(generalConfiguration.armExecution.dAngolo>0) actuatorsArmMove(200);
            else actuatorsArmMove(-200);
            _time_delay(1000);

            // Attesa fine movimento di rotazione
            for(int i=0; i<20; i++){
                if(!generalConfiguration.armExecution.completed) _time_delay(1000);
                else break;
            }
            if(!generalConfiguration.armExecution.success){
                // Errore timeout posizionamento lenze
                debugPrint("GUI TIMEOUT ARM DURING PARKING");
                buffer[1] = ERROR_PARKING_ARM_TMO;
                mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
                return;
            }

    }

    // Disabilitazione generale ad eccezione del lenze
    generalConfiguration.lenzeCfg.startupInParkingMode = 1;
    generalConfiguration.lenze_park_enable_run = true;
    actuatorsManageEnables();

    // Attivazione parking
    actuatorsLenzPark();

    // Attesa fine movimento di parcheggio
    for(int i=0; i<60; i++){
        if(generalConfiguration.armExecution.lenze_run) _time_delay(1000);
        else break;
    }

    if(generalConfiguration.armExecution.lenze_run){
        // Errore timeout posizionamento lenze
        debugPrint("GUI TIMEOUT COMPRESSION PARKING LENZE");
        buffer[1] = ERROR_PARKING_LENZE_TMO;
        mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
        return;
    }


    // Operazione completata con successo
    generalConfiguration.lenze_park_enable_run = false;
    actuatorsManageEnables();

    buffer[1] = 0;
    mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);


    return ;
}



// Procedura per il resume dal parking mode
void mcc_unparking_mode(void)
{
    unsigned char buffer[2];
    buffer[0] = MCC_PARKING_MODE_COMMANDS_START_UNPARKING;

    debugPrint("GUI UNPARKING MODE GUI REQUEST");
    generalConfiguration.lenze_park_enable_run = true;
    actuatorsManageEnables();
    _time_delay(200);

    // Attivazione della modalità di sblocco parcheggio
    if(generalConfiguration.armExecution.lenze_run){
        debugPrint("GUI LENZE BUSY DURING UNPARKING");
        buffer[1] = ERROR_PARKING_LENZE_BUSY;
        mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
        return;
    }

    // Attivazione lenz verso Unpark.
    // Se va a buon fine il Lenze si troverà sbloccato dalla condizione di parcheggio
    actuatorsLenzeUnpark();

    // Attesa fine movimento di parcheggio
    for(int i=0; i<60; i++){
        if(generalConfiguration.armExecution.lenze_run) _time_delay(1000);
        else break;
    }

    if(generalConfiguration.armExecution.lenze_run){
        // Errore timeout posizionamento lenze
        debugPrint("GUI TIMEOUT LENZE DURING UNPARKING");
        buffer[1] = ERROR_PARKING_LENZE_TMO;
        mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
        return;
    }

    // Verifica se il potenziometro ha raggiunto il target atteso
    if(generalConfiguration.armExecution.lenze_pot < generalConfiguration.lenzeCfg.parkingSafePoint){
        debugPrint("GUI UNPARKING LENZ ERROR IN POSITIONNING");
        buffer[1] = ERROR_PARKING_LENZE_POSITION;
        mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);
        return;
    }

    debugPrintI("GUI UNPARKING LENZE OK, POT", generalConfiguration.armExecution.lenze_pot);
    generalConfiguration.lenzeCfg.startupInParkingMode = 0;
    generalConfiguration.lenze_park_enable_run = true;
    actuatorsManageEnables();
    _time_delay(500);

    if(generalConfiguration.gantryCfg.armMotor){
        // Attivazione Arm a 0
        actuatorsArmMove(0);
    }

    // DISPOSITIVO SBLOCCATO
    buffer[1] = 0;
    mccGuiNotify(1,MCC_PARKING_MODE_COMMANDS, buffer, 2);


  return ;
}

/*
  Questa funzione imposta il movimento della Biopsia (se presente)
  nei suoi tre assi XYZ (opzionali)

Buffer Dati:
  mcc_cmd.buffer[0]: XGO (1=muove X)
  mcc_cmd.buffer[1]: X L
  mcc_cmd.buffer[2]: X H
  mcc_cmd.buffer[3]: YGO (1=muove X)
  mcc_cmd.buffer[4]: Y L
  mcc_cmd.buffer[5]: Y H
  mcc_cmd.buffer[6]: ZGO (1=muove X)
  mcc_cmd.buffer[7]: Z L
  mcc_cmd.buffer[8]: Z H

  mcc_cmd.buffer[9]: ZlimitGO
  mcc_cmd.buffer[10]: Zl L

  mcc_cmd.buffer[11]: ZlesioneGO
  mcc_cmd.buffer[12]: Zles L

  mcc_cmd.buffer[13]: ZlagoGO
  mcc_cmd.buffer[14]: Zlago L

*/
void mccBiopsyStdXYZ(void)
{
  if(generalConfiguration.biopsyCfg.biopsyConnected==FALSE) return;

  // Verifica se si tratta del comando di incremento
  if(mcc_cmd.len==2)
  {
    biopsyStandardStepZ(mcc_cmd.buffer[0],mcc_cmd.buffer[1]);
    printf("BIOP STEPS");
    return;
  }
  unsigned short X=mcc_cmd.buffer[1]+256*mcc_cmd.buffer[2];
  unsigned short Y=mcc_cmd.buffer[4]+256*mcc_cmd.buffer[5];
  unsigned short Z=mcc_cmd.buffer[7]+256*mcc_cmd.buffer[8];

  if(mcc_cmd.buffer[9]) // ZlimitGO
  {
    // <TBD> aggiungere risposta in caso di comando fallito
    biopsyStandardSetZLimit(mcc_cmd.buffer[10]);
  }

  if(mcc_cmd.buffer[11]) // Zlesione
  {
    // <TBD> aggiungere risposta in caso di comando fallito
    biopsyStandardSetZLesione(mcc_cmd.buffer[12]);
  }

  if(mcc_cmd.buffer[13]) // ZLago
  {
    // <TBD> aggiungere risposta in caso di comando fallito
    biopsyStandardSetLago(mcc_cmd.buffer[14]);
  }

  // Movimento
  biopsyStandardSetXYZ(X,mcc_cmd.buffer[0], Y, mcc_cmd.buffer[3], Z, mcc_cmd.buffer[6]);
  return;
}

void mccPrint(void)
{
    if(mcc_cmd.buffer[0] == MCC_DEBUG_PRINT_ENABLE_CMD){
        if(mcc_cmd.buffer[1] == 1){
                debugPrintEna(true);
                debugPrint("ATTIVAZIONE DEBUG PRINT  DRIVER");
        }else{
                debugPrintEna(false);
                debugPrint("DISATTIVAZIONE DEBUG PRINT DRIVER");
        }
    }else if(mcc_cmd.buffer[0] == MCC_DRIVER_PRINT_ENABLE_CMD){
        if(mcc_cmd.buffer[1] == 1){
                printEna(true);
                debugPrint("ATTIVAZIONE PRINT DRIVER");
        }else{
                printEna(false);
                debugPrint("DISATTIVAZIONE PRINT DRIVER");
        }
    }
}
/* EOF */
