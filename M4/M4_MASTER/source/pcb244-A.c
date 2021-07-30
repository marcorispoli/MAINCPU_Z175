#define _PCB244_A_C
#include "dbt_m4.h" 

#define TARGET_ADDRESS 0x17
#define _DEF_PCB244_DRIVER_DELAY 1000

#define CONTEST PCB244_A_CONTEST
#define STATUS  (*((_PCB244_A_Stat_Str*)(&PCB244_A_CONTEST.Stat)))

// Funzione di servizio interne al modulo
static bool GetFwRevision(void);
static void enterFreezeMode(void);
static void ERROR_HANDLER(void);



void pcb244_A_driver(uint32_t taskRegisters)
{
  int i;
  bool repeat;
  unsigned char dati[6];
  unsigned char polling=5;
  unsigned char mag=255;


    // Costruzione del contesto
   CONTEST.pReg = PCB244_A_Registers;
   CONTEST.nregisters = PCB244_A_NREGISTERS;
   CONTEST.evm = _EVM(_EV0_PCB244_CFG_UPD);
   CONTEST.evr = &_EVR(_EV0_PCB244_CFG_UPD);
   CONTEST.address = TARGET_ADDRESS;
   printf("ATTIVAZIONE DRIVER PCB244_A: \n");
    
   //////////////////////////////////////////////////////////////////////////
   //                   FINE FASE DI INIZIALIZZAZIONE DRIVER               //             
   //        Inizia il ciclo di controllo e gestione della periferica      //
   //////////////////////////////////////////////////////////////////////////

    // In caso di errore di compilazione in questo punto 
    // significa errore in compilazione della struttura registri
    SIZE_CHECK((sizeof(PCB244_A_Registers)/sizeof(_DeviceRegItem_Str))!=PCB244_A_NREGISTERS);
          
    // Retrive Task ID
    CONTEST.ID =  _task_get_id();
    
    // Init registro di stato
    memset((void*)&(STATUS), 0,sizeof(_Device_Stat_Str ));
  
    _EVCLR(_EV1_PCB244_CONNECTED);
    _EVCLR(_EV2_PCB244_STARTUP_OK);

    // Richiesta revisione firmware a target
    while(GetFwRevision()==FALSE) _time_delay(100);
    printf("PCB244_A:REVISIONE FW TARGET:%d.%d\n",STATUS.maj_code,STATUS.min_code);
    pcb244_A_isPresent=TRUE;

    // Segnalazione driver connesso
   _EVSET(_EV1_PCB244_CONNECTED);

    // Attende l'autorizzazione ad effetuare il refresh registri
    _EVWAIT_ANY(_EV1_UPDATE_REGISTERS);

   // Invia comando di reset faults al target e reitera fino a risposta
   pcb244_A_ResetFaults();

   // Carica Tutti i registri RD / RW
   for(i=0;i<PCB244_A_NREGISTERS;)
   {
      if(Ser422ReadRegister(i,4,&CONTEST)!=_SER422_NO_ERROR){
          _time_delay(50);
          continue;
      }

      i++;
   }

   // Attende la ricezione della configurazione se necessario
   _EVSET(_EV2_PCB244_STARTUP_OK);
   printf("PCB244_A: ATTENDE CONFIGURAZIONE..\n");
   _EVWAIT_ANY(_EV1_DEV_CONFIG_OK);
   printf("PCB244_A: CONFIGURAZIONE OK. INIZIO LAVORO\n");

   repeat = FALSE;
   ////////////////////////////////////////////////////////////////////////
   /*
                  GESTIONE DEL CICLO DI LAVORO ORDINARIO
       Il driver effettua un polling sui principali registri di lettura
       iscritti nella lista di polling ed effettua un controllo periodico
       sui registri di scrittura per eventuali udates in caso di differenze 
   */
   /////////////////////////////////////////////////////////////////////////
   while(1)
   {
     if(STATUS.freeze) enterFreezeMode();
     
     
     // POlling per testare lo stato corrente dell'accessorio
     if(--polling==0)
     {
       repeat = TRUE;
       polling=5;
     }

     // Identificazione Accessorio
     if(pcb244AGetAccessorio()) repeat = true;

     // Verifica se il mag factor � cambiato per aggiornare la GUI
     // Il fattore di ingrandimento viene calcolato dal driver compressore
     if(mag!=generalConfiguration.potterCfg.potMagFactor){
         mag = generalConfiguration.potterCfg.potMagFactor;
         repeat = TRUE;
     }

     // Identificazione Cassetta
     if(pcb244AGetCassette()) repeat = true;

    // Aggiorna i dati relativi al Potter
    if(repeat)
    {
      polling = 5;
      repeat = FALSE;
      dati[0] = generalConfiguration.potterCfg.potId;
      dati[1] = generalConfiguration.potterCfg.potMagFactor;// generalConfiguration.comprCfg.calibration.fattoreIngranditore[generalConfiguration.potterCfg.potMagFactor &0x7];
      dati[2] = generalConfiguration.potterCfg.cassette;
      dati[3] = generalConfiguration.potterCfg.cassetteExposed;
      dati[4] = generalConfiguration.potterCfg.potDescriptor;      // Aggiunto per qualificare il potter come 18x24/24x30
      mccGuiNotify(1,MCC_POTTER_ID,dati,5); // Notifica l'applicazione
    }

     
      // Termine della routine di driver
      STATUS.ready=1;
      _EVSET(_EV1_PCB244_RUN);
     _time_delay(_DEF_PCB244_DRIVER_DELAY);
   }
}

bool pcb244AGetAccessorio(void){
    bool changed=false;

    if(Ser422ReadRegister(_REGID(RG244_A_BUCKY),2,&CONTEST)==_SER422_NO_ERROR)
    {

       // Identificazione Potter
       switch(_DEVREGL(RG244_A_BUCKY,CONTEST))
       {
         case 1:
           if( (generalConfiguration.potterCfg.potId!= POTTER_2D) || (generalConfiguration.potterCfg.potDescriptor != POTTER_DESCR_18x24)) changed = true;
           generalConfiguration.potterCfg.potId = POTTER_2D; // 18x24
           generalConfiguration.potterCfg.potDescriptor = POTTER_DESCR_18x24;

         break;
         case 2:
           if( (generalConfiguration.potterCfg.potId!= POTTER_2D) || (generalConfiguration.potterCfg.potDescriptor != POTTER_DESCR_24x30)) changed = true;
           generalConfiguration.potterCfg.potId = POTTER_2D ;// 24x30
           generalConfiguration.potterCfg.potDescriptor = POTTER_DESCR_24x30;
         break;
         case 4:
           if( (generalConfiguration.potterCfg.potId!= POTTER_MAGNIFIER)) changed = true;
           generalConfiguration.potterCfg.potId = POTTER_MAGNIFIER ;// Magnifier
         break;
         default:
           if( (generalConfiguration.potterCfg.potId!= POTTER_UNDEFINED)) changed = true;
           generalConfiguration.potterCfg.potId = POTTER_UNDEFINED ;
       }

   }

   return changed;
}

bool pcb244AGetCassette(void){
    bool changed=false;
    if(Ser422ReadRegister(_REGID(RG244_A_SETTINGS),2,&CONTEST)==_SER422_NO_ERROR)
    {
        if(_TEST_BIT(PCB244_A_CASSETTE)!=generalConfiguration.potterCfg.cassette){
            generalConfiguration.potterCfg.cassette=_TEST_BIT(PCB244_A_CASSETTE);
            changed = TRUE;
        }
        if(_TEST_BIT(PCB244_A_CASSETTE_EXPOSED)!=generalConfiguration.potterCfg.cassetteExposed){
            generalConfiguration.potterCfg.cassetteExposed=_TEST_BIT(PCB244_A_CASSETTE_EXPOSED);
            changed = TRUE;
        }
   }
   return changed;
}

//////////////////////////////////////////////////////////////////////////////
/*
_PCB244_Error_Enum GetFwRevision(void)
        La funzione legge il codice di revisione del firmware del 
        target.

PARAM:
        -
RETURN:
      TRUE: Lettura avvenuta con successo
      FALSE: Problema di comunicazione con il target

      PCB244_Stat.maj_code/PCB244_Stat.min_code = codice revisione
Autore: M. Rispoli
Data: 24/10/2014
*/
//////////////////////////////////////////////////////////////////////////////
bool GetFwRevision(void)
{
 _Ser422_Command_Str frame;

  
  frame.address = TARGET_ADDRESS;
  frame.attempt = 4;
  frame.cmd=SER422_COMMAND;
  
  // Scrive il codice comando
  frame.data1=_CMD1(PCB244_A_GET_FWREV);
  frame.data2=_CMD2(PCB244_A_GET_FWREV);
  Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);
  
  // Eventuali errori di comunicazione
  if(frame.retcode!=SER422_DATA) 
  {
    printf("ERRORE REVISIONE FW PCB244-A\n");
    return FALSE;
  }
  
  STATUS.maj_code = frame.data2;
  STATUS.min_code = frame.data1;
  
  generalConfiguration.revisioni.pcb244.maj = STATUS.maj_code; 
  generalConfiguration.revisioni.pcb244.min = STATUS.min_code; 
 
  printf("REVISIONE PCB244-A: %d.%d\n",STATUS.maj_code,STATUS.min_code);
  return TRUE;
   
}

bool GetPcb244AFwRevision(void){
    return GetFwRevision();
}




bool pcb244_A_ResetFaults(void)
{
    _Ser422_Command_Str frame;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_RESET_FAULT);
     frame.data2=_CMD2(PCB244_A_RESET_FAULT);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) {
         printf("PCB244-A - RESET FAULT OK!!!! \n");
         return true;
     }

     // Condizione di BUSY
     if(frame.retcode==SER422_BUSY) {
         printf("PCB244-A - BUSY!!!! \n");
         return false;
     }

     printf("PCB24-A - ERROR: [%d] [%d] !!!! \n", frame.data1,frame.data2);
     return FALSE;

 }

bool pcb244_A_ResetBoard(void)
{
    _Ser422_Command_Str frame;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_RESET_BOARD);
     frame.data2=_CMD2(PCB244_A_RESET_BOARD);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) {
         printf("PCB244-A - RESET FAULT OK!!!! \n");
         return true;
     }

     // Condizione di BUSY
     if(frame.retcode==SER422_BUSY) {
         printf("PCB244-A - BUSY!!!! \n");
         return false;
     }

     printf("PCB244-A - ERROR: [%d] [%d] !!!! \n", frame.data1,frame.data2);
     return FALSE;

 }

// Attende almeno due secondi per verificare che il sistema esca da un busy
bool pcb244_A_VerifyBusy(void){
    int tentativi = 20;

    // Verifica che non ci sia un BUSY
    while(tentativi--){
        if(Ser422ReadRegister(_REGID(RG244_A_SYSFLAGS0),2,&CONTEST)!=_SER422_NO_ERROR){
            _time_delay(50);
            continue;
        }else{
            if(_TEST_BIT(PCB244_A_BUSY)){
                _time_delay(100);
                continue;
            } else return true;
        }
    }

    printf("PCB244-A: VERIFY BUSY FALLITO!\n");
    return false;
}

// COMANDO DI ATTIVAZIONE ESPOSIMETTRO PER SEQUENZA SOLO PRE IMPULSO
bool pcb244_A_StartRxPre(void){
    _Ser422_Command_Str frame;


    // Attesa che la scheda non sia in busy
    if(!pcb244_A_VerifyBusy()) return false;

    frame.address = TARGET_ADDRESS;
    frame.attempt = 4;
    frame.cmd=SER422_COMMAND;

    // Scrive il codice comando
    frame.data1=_CMD1(PCB244_A_START_PRE);
    frame.data2=_CMD2(PCB244_A_START_PRE);
    Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

    if(frame.retcode==SER422_COMMAND_OK) return true;

    // Se c'� stato un errore di protocollo tuttavia
    // il comando pu� essere stato ricevuto correttamente.
    // Basta verificare la presenza del busy..
    if(Ser422ReadRegister(_REGID(RG244_A_SYSFLAGS0),10,&CONTEST)!=_SER422_NO_ERROR){
        printf("Verifica BUSY attivato..\n");
        if(_TEST_BIT(PCB244_A_BUSY)) return true;
    }

    printf("PCB244-A START PRE FALLITO!\n");
    return FALSE;
}


bool pcb244_A_StartRxAec(void){
    _Ser422_Command_Str frame;

    // Attesa che la scheda non sia in busy
    if(!pcb244_A_VerifyBusy()) return false;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_START_AEC);
     frame.data2=_CMD2(PCB244_A_START_AEC);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) return true;

     // Se c'� stato un errore di protocollo tuttavia
     // il comando pu� essere stato ricevuto correttamente.
     // Basta verificare la presenza del busy..
     if(Ser422ReadRegister(_REGID(RG244_A_SYSFLAGS0),10,&CONTEST)!=_SER422_NO_ERROR){
         printf("Verifica BUSY attivato..\n");
         if(_TEST_BIT(PCB244_A_BUSY)) return true;
     }

     printf("PCB244-A StartRxAec FALLITO!\n");
     return FALSE;
}


bool pcb244_A_StartManual(void){
    _Ser422_Command_Str frame;

    // Attesa che la scheda non sia in busy
    if(!pcb244_A_VerifyBusy()) return false;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_START_STD);
     frame.data2=_CMD2(PCB244_A_START_STD);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) return true;

     // Se c'� stato un errore di protocollo tuttavia
     // il comando pu� essere stato ricevuto correttamente.
     // Basta verificare la presenza del busy..
     if(Ser422ReadRegister(_REGID(RG244_A_SYSFLAGS0),10,&CONTEST)!=_SER422_NO_ERROR){
         printf("Verifica BUSY attivato..\n");
         if(_TEST_BIT(PCB244_A_BUSY)) return true;
     }

     printf("PCB244-A StartManual FALLITO!\n");
     return FALSE;
}

bool pcb244_A_StartCalibTube(void){
    _Ser422_Command_Str frame;

     // Attesa che la scheda non sia in busy
     if(!pcb244_A_VerifyBusy()) return false;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_START_CALIB_TUBE);
     frame.data2=_CMD2(PCB244_A_START_CALIB_TUBE);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

    if(frame.retcode==SER422_COMMAND_OK) return true;

    // Se c'� stato un errore di protocollo tuttavia
    // il comando pu� essere stato ricevuto correttamente.
    // Basta verificare la presenza del busy..
    if(Ser422ReadRegister(_REGID(RG244_A_SYSFLAGS0),10,&CONTEST)!=_SER422_NO_ERROR){
        printf("Verifica BUSY attivato..\n");
        if(_TEST_BIT(PCB244_A_BUSY)) return true;
    }

    printf("PCB244-A StartCalibTube FALLITO!\n");
    return FALSE;
}

// Caricamento impulsi per l'esposimetro
bool pcb244_A_uploadAECPulses(unsigned short pulses){
    printf("AEC-PULSE UPLOAD PCB244-A\n");
    if(Ser422WriteRegister(_REGID(RG244_A_PULSES),pulses,10,&CONTEST)!=_SER422_NO_ERROR) return false;
    if(Ser422WriteRegister(_REGID(RG244_A_RXCMD),1,10,&CONTEST)!=_SER422_NO_ERROR) return false;
    return true;
}

// Caricamento impulsi per l'esposimetro
bool pcb244_A_uploadManualPulses(unsigned short pulses){
    printf("MANUAL-PULSE UPLOAD PCB244-A\n");
    if(Ser422WriteRegister(_REGID(RG244_A_PULSES),pulses,10,&CONTEST)!=_SER422_NO_ERROR) return false;
    if(Ser422WriteRegister(_REGID(RG244_A_RXCMD),1,10,&CONTEST)!=_SER422_NO_ERROR) return false;
    return true;
}

// Carica lo status della pcb 190
bool pcb244_A_GetPostRxRegisters(void){

  if(Ser422ReadRegister(_REGID(RG244_A_PULSES_EXIT),10,&CONTEST)!=_SER422_NO_ERROR) return false;

  return TRUE;
}

void PCB244_A_SetRxStop(void){
    Ser422WriteRegister(_REGID(RG244_A_RXCMD),2,10,&CONTEST);
}

bool PCB244_A_GetRad1(int attempt){
    _Ser422_Command_Str frame;


    while(attempt){
        frame.address = TARGET_ADDRESS;
        frame.attempt = attempt;
        frame.cmd=SER422_SPECIAL;

        // Legge il byte basso
        frame.data1=1;
        frame.data2=0;
        Ser422Send(&frame, SER422_BLOCKING,(unsigned char) CONTEST.ID);

        if(frame.retcode!=SER422_COMMAND_SPECIAL_OK){
             attempt --;
             _time_delay(50);
             continue;
        }
        break;
    }
    if(attempt==0) return false;

    _DEVREGL(RG244_A_RAD1,CONTEST) =  frame.data1;
    _DEVREGH(RG244_A_RAD1,CONTEST) =  frame.data2;

   return true;
}

bool PCB244_A_GetRad5(int attempt){
    _Ser422_Command_Str frame;


    while(attempt){
        frame.address = TARGET_ADDRESS;
        frame.attempt = attempt;
        frame.cmd=SER422_SPECIAL;

        // Legge il byte basso
        frame.data1=2;
        frame.data2=0;
        Ser422Send(&frame, SER422_BLOCKING,(unsigned char) CONTEST.ID);

        if(frame.retcode!=SER422_COMMAND_SPECIAL_OK){
             attempt --;
             _time_delay(50);
             continue;
        }
        break;
    }
    if(attempt==0) return false;

    _DEVREGL(RG244_A_RAD5,CONTEST) =  frame.data1;
    _DEVREGH(RG244_A_RAD5,CONTEST) =  frame.data2;

   return true;
}

bool PCB244_A_GetRad25(int attempt){
    _Ser422_Command_Str frame;


    while(attempt){
        frame.address = TARGET_ADDRESS;
        frame.attempt = attempt;
        frame.cmd=SER422_SPECIAL;

        // Legge il byte basso
        frame.data1=4;
        frame.data2=0;
        Ser422Send(&frame, SER422_BLOCKING,(unsigned char) CONTEST.ID);

        if(frame.retcode!=SER422_COMMAND_SPECIAL_OK){
             attempt --;
             _time_delay(50);
             continue;
        }
        break;
    }
    if(attempt==0) return false;

    _DEVREGL(RG244_A_RAD25,CONTEST) =  frame.data1;
    _DEVREGH(RG244_A_RAD25,CONTEST) =  frame.data2;


   return true;
}


// Tentativi ogni 10ms
bool PCB244_A_GetPreRad(int attempt){
     // Legge registro di abilitazione

     int tentativi = attempt;
     while(tentativi){
        if(Ser422ReadRegister(_REGID(RG244_A_SETTINGS),4,&CONTEST)!=_SER422_NO_ERROR) return false;
        if(!(_DEVREGL(RG244_A_SETTINGS,CONTEST) & ENABLE_RD_RAD)) _time_delay(10);
        else break;
     }

     if(!tentativi) return false;

     // Legge i RAD campionati
     if(!PCB244_A_GetRad1(10)) return false;
     if(!PCB244_A_GetRad5(10)) return false;
     if(!PCB244_A_GetRad25(10)) return false;
    return true;
}


// Impostazione del campo del detector
bool PCB244_A_setDetectorField(unsigned char val){
    _Ser422_Command_Str frame;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_SET_FIELD_FRONT);
     frame.data2=val;
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) {
         printf("PCB244_A: CAMPO ESPOSIMETRO N:%d OK\n", val);
         return true;
     }
    printf("PCB244_A: CAMPO ESPOSIMETRO N:%d NOK\n", val);

     return FALSE;
}

// Effettua una campionatura completa
bool PCB244_A_sampleRad(void){
    _Ser422_Command_Str frame;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_SAMPLE_RAD);
     frame.data2=_CMD2(PCB244_A_SAMPLE_RAD);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) {
         printf("PCB244_A: Campionamento RAD OK\n");
         return true;
     }
     printf("PCB244_A: Campionamento RAD NOK\n");

     return FALSE;
}

// Effettua una campionatura completa
bool PCB244_A_setOffset(unsigned short val){

    if(Ser422WriteRegister(_REGID(RG244_A_OFFSET),val,10,&CONTEST)!=_SER422_NO_ERROR)
    {
        printf("PCB244_A: Scrittura Offset NOK\n");
        return false;
    }

    _Ser422_Command_Str frame;

     frame.address = TARGET_ADDRESS;
     frame.attempt = 4;
     frame.cmd=SER422_COMMAND;

     // Scrive il codice comando
     frame.data1=_CMD1(PCB244_A_SET_OFFSET);
     frame.data2=_CMD2(PCB244_A_SET_OFFSET);
     Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);

     if(frame.retcode==SER422_COMMAND_OK) {
         printf("PCB244_A: Impostazione Offset OK\n");
         return true;
     }
     printf("PCB244_A: Impostazione Offset NOK\n");

     return FALSE;
}

/*
  Funzione configuratrice
*/
bool config_pcb244_A(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len){
  return true;
}

void enterFreezeMode(void){
  // Entra in Freeze
  printf("PB244 ENTRA IN FREEZE\n");
  _EVCLR(_EV1_PCB244_RUN);
  _EVSET(_EV1_PCB244_FREEZED); // Notifica l'avvenuto Blocco
  _EVWAIT_ANY(_MOR2(_EV1_DEVICES_RUN,_EV1_PCB244_RUN)); // Attende lo sblocco
  _EVSET(_EV1_PCB244_RUN);
  STATUS.freeze = 0;

}

/* EOF */
 

