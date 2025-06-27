#define _SEQRX_TOMO_C
#include "dbt_m4.h"
#undef _SEQERROR
#undef Param

static void RxTomoSeqError(int code);
_RxStdSeq_Str tomoParam;
bool tomoSeqResult=FALSE;
bool tomoIsRunning=FALSE;
unsigned char tomoError;


int tomoCurrentFilterPosition;
int tomoFilterTarget;

#define Param (&tomoParam)
#define _SEQERROR(code) {RxTomoSeqError(code); continue;}

void tomo_rx_task(uint32_t taskRegisters)
{
  unsigned char data[5];
  unsigned short mAs_erogati=0;

  unsigned char i;
  unsigned short sval;
  unsigned char chk;
  _DeviceAppRegister_Str        ConfList;
  
   
  printf("PARTENZA SEQUENZA PER GESTIONE RAGGI TOMO\n");
  _EVCLR(_SEQEV_RX_TOMO_START);
  
  while(1)
  {
    // Attende fino a nuova partenza
    tomoIsRunning=FALSE;
    _EVCLR(_SEQEV_RX_TOMO_START);
    _EVWAIT_ALL(_SEQEV_RX_TOMO_START);
    _EVCLR(_SEQEV_RX_TOMO_TERMINATED);
    tomoSeqResult=FALSE;
    tomoIsRunning=TRUE;
    tomoError=0;
    
    
    if(generalConfiguration.demoMode) debugPrint("RX-3D START IN DEMO MODE");
    else  debugPrint("RX-3D START SEQUENCE");

    // ___________________________________________________________________________ POSIZIONAMENTI DEL COLLIMATORE GIA' IN ESSERE

    // Prima di andare in freeze bisogna accertarsi che la collimazione 2D sia andata a buon fine
    if(wait2DBackFrontCompletion(100)==false) _SEQERROR(ERROR_INVALID_COLLI);
    if(waitRxFilterCompletion()==FALSE)  _SEQERROR(ERROR_INVALID_FILTRO);
    if(pcb249U2MirrorHome()==FALSE)_SEQERROR(ERROR_MIRROR_LAMP);
    if(wait2DLeftRightTrapCompletion(100)==false) _SEQERROR(ERROR_INVALID_COLLI);

    // ___________________________________________________________________________ FREEZE DEI PROCESSI
    // Manda subito in FREEZE i drivers per non intralciare le operazioni
    // Non viene però atteso che effettivamente i drivers si fermino
    Ser422DriverFreezeAll(0);

    // Resetta eventuali fault del collimatore
    pcb249U1ResetFaults();

    // Impostazione della lama frontale e posteriore fin da subito
    if(!pcb249U2ColliCmd(generalConfiguration.colliCfg.dynamicArray.tomoBack, generalConfiguration.colliCfg.dynamicArray.tomoFront)){
        debugPrint("Errore collimazione formato tomo fronte retro!");
        _SEQERROR(_SEQ_ERR_COLLI_TOMO);
    }

    // Verifica Chiusura porta
    if((SystemInputs.CPU_CLOSED_DOOR==0) && (!generalConfiguration.demoMode))
    {
        debugPrint("RX-3D ERRORE PORTA STUDIO APERTA");
        _SEQERROR(ERROR_CLOSED_DOOR);
    }

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();

    // ___________________________________________________________________________ ATTIVAZIONE STARTER
    // Attiva Starter precocemente
    if(!generalConfiguration.demoMode){
      if(Param->esposizione.HV & 0x4000)
      {
        if(pcb190StarterH()==FALSE) debugPrint("RX-3D COMANDO STARTER HS FALLITO");
        else debugPrint("RX-3D STARTER HS ATTIVATO");
      }else
      {
        if(pcb190StarterL()==FALSE) debugPrint("RX-3D COMANDO STARTER LS FALLITO");
        else debugPrint("RX-3D STARTER LS ATTIVATO");
      }
    }

    // Verifica pulsante raggi
    if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);


    // ___________________________________________________________________________ TUBO IN HOME POSITION (senza attesa)
    // Manda subito il Braccio in Home Tomo.
    if(actuatorsMoveTomoTrxHome(Param->tomo_mode)==false) _SEQERROR(_SEQ_ERR_INTERMEDIATE_HOME);


    // Preparazione del collimatore per l'inseguimento di formato e di filtro,
    // valido per entrambi i metodi di collimazione dinamica (Inclinometro/Expwin)
    if((Param->tomo_mode!=_TOMO_MODE_STATIC)&&(!generalConfiguration.demoMode))
    {


      // Inizializzazione Inseguimento Collimatore:
      // impostazione dell'angolo attuale del braccio, nel caso di collimazione dinamica vecchio stile.
      // impostazioni numero di skip per collimazione ew.
      // impostazione timing di inseguimento, in funzione della velocità del braccio.      
      if(!pcb249U1_initTomoColli(Param)){
          _SEQERROR(_SEQ_ERR_COLLI_TOMO);
      }

      // Inizializzazione Inseguimento Filtro:
      // viene impostato il valore della posizione del filtro ad angolo 27°, corretto con l'aggiustamento.
      // viene impostato il primo angolo valido della scansione tomo (escludendo gli skips)      
      if(!pcb249U2_initTomoFilter()){
          _SEQERROR(ERROR_INVALID_FILTRO);
      }

      // Attivazione collimazione dinamica
      if(!pcb249U1SetColliCmd(3)) {
          debugPrint("Errore attivazione collimazione dinamica lame laterali!");
          _SEQERROR(_SEQ_ERR_COLLI_TOMO);
      }

      // Attiva modalità inseguimento filtro (se abilitato)
      if(generalConfiguration.filterTomoEnable){
          if(!pcb249U2_activateFilterTomo(tomoParam.first_gonio)){
               debugPrint("Attivazione filtro dinamico fallito!");
               _SEQERROR(ERROR_INVALID_FILTRO);
          }
      }

    }
    
    // Verifica pulsante raggi
    if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);
    
    // Impostazione dei segnali di abilitazione
    if(!generalConfiguration.demoMode){
      
        // Preparazione Registri 190 per attivazione raggi 
        if(pcb190UploadTomoExpose(Param,FALSE)==FALSE) _SEQERROR(_SEQ_UPLOAD190_PARAM);
        
        if(Param->tomo_mode==_TOMO_MODE_WIDE)
          debugPrint("RX-3D ESPOSIZIONE  WIDE");
        else if(Param->tomo_mode==_TOMO_MODE_NARROW)
          debugPrint("RX-3D ESPOSIZIONE  NARROW");
        else if(Param->tomo_mode==_TOMO_MODE_INTERMEDIATE)
          debugPrint("RX-3D ESPOSIZIONE  INTERMEDIATE");
        else
          debugPrint("RX-3D ESPOSIZIONE  BRACCIO FERMO");

        debugPrintI4("RX-3D EXP DATA, IDAC", Param->esposizione.I & 0x0FFF, "VDAC",Param->esposizione.HV & 0x0FFF,"MASDAC", Param->esposizione.MAS, "SMP",Param->tomo_samples );


        // Impostazione Segnale XRAY_ENA su Bus Hardware
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=1;   // Attivazione segnale XRAY ENA
        SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Attivazione Buzzer
        _EVSET(_EV0_OUTPUT_CAMBIATI);         
        _mutex_unlock(&output_mutex);
         
        // Attesa per debounce
        _time_delay(200);

        if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);

        // Attesa completamento movimento tubo + preparazione per nuovo movimento con EXP-WIN
        // Se si rilascia il pulsante durante il posizionamento verrà segnalato l'errore sul posizionamento
        debugPrint("ATTESA FINE POSIZIONAMENO TRX IN HOME");
        if(actuatorsTrxWaitReady(100)==false) _SEQERROR(_SEQ_ERR_WIDE_HOME);
        if(Param->tomo_mode!=_TOMO_MODE_STATIC) actuatorsMoveTomoTrxEnd(Param->tomo_mode,true);

        // Attende i segnali e verifica l'uscita con pulsante raggi
        if(SystemInputs.CPU_XRAY_ENA_ACK==0)
        {   
          _EVCLR(_EV2_XRAY_ENA_ON);
          _EVCLR(_EV2_XRAY_REQ_OFF);  
           if(_EVWAIT_TANY(_MOR2(_EV2_XRAY_ENA_ON,_EV2_XRAY_REQ_OFF),_WAIT_XRAY_ENA)==FALSE) _SEQERROR(_SEQ_IO_TIMEOUT);
        }
        if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);

        // Comando Attivazione Raggi
        if(waitPcb190Ready(50)==FALSE) _SEQERROR(_SEQ_PCB190_BUSY);

        debugPrint("ATTIVAZIONE GENERATORE...");
        int rc = pcb190StartRxTomo();
        if(rc==SER422_ILLEGAL_FUNCTION) _SEQERROR(ERROR_PUSHRX_NO_PREP);

        // Spin Lock ogni 100ms per testare il termine della sequenza
        long rxloop = (_WAIT_XRAY_COMPLETED / 100);        
        int delay = 4; // Per i primi 2 secondi non verifica il fine raggi
        while(rxloop--){

           // Controllo sull'attesa del comnpletamento della sequenza
           if(!delay){
             if(SystemInputs.CPU_XRAY_COMPLETED==1) break; // Fine sequenza
           }else delay--;
           _time_delay(100);
        }
        
        if(rxloop==0){_SEQERROR(_SEQ_PCB190_TMO);}
        debugPrint("GENERATORE IN IDLE ");

        // Disattivazione XRAY-ENA
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=0;   // Disattivazione segnale XRAY ENA
        SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Disattivazione Buzzer
        _EVSET(_EV0_OUTPUT_CAMBIATI);
        _mutex_unlock(&output_mutex);

       // Lettura esito raggi
       if(pcb190GetPostRxRegisters()==FALSE){
           debugPrint("RX-3D ERROR DURING PCB190 READ REGISTER AFTER XRAY EXPOSURE");
           _SEQERROR(_SEQ_READ_REGISTER);

       }

    }else{      
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //                                    SEZIONE SEQUENZA DEMO
        //////////////////////////////////////////////////////////////////////////////////////////////////

           
      // Attesa completamento movimento tubo       
      int delay = 0;

      // Attesa completamento movimento tubo NO EXP WIN
      // Se si rilascia il pulsante durante il posizionamento verrà segnalato l'errore sul posizionamento
      printf("ATTESA FINE POSIZIONAMENO..\n");
      if(actuatorsTrxWaitReady(100)==false) _SEQERROR(_SEQ_ERR_WIDE_HOME);

      if(Param->tomo_mode==_TOMO_MODE_WIDE) delay = generalConfiguration.trxCfg.tomo.w.samples;
      else if(Param->tomo_mode==_TOMO_MODE_NARROW) delay = generalConfiguration.trxCfg.tomo.n.samples;
      else if(Param->tomo_mode==_TOMO_MODE_INTERMEDIATE) delay = generalConfiguration.trxCfg.tomo.i.samples;

      printf("ATTIVAZIONE MOVIMENTO DEMO TO END");

      _time_delay(200);
      actuatorsMoveTomoTrxEnd(Param->tomo_mode,false); // Partenza immediata braccio
      _time_delay(200);

      
      _mutex_lock(&output_mutex);
      if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_5F){
          SystemOutputs.CPU_DEMO_MODEL = 1;
          SystemOutputs.CPU_DEMO_MODEH = 1;
      }else if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_4F){
          SystemOutputs.CPU_DEMO_MODEL = 1;
          SystemOutputs.CPU_DEMO_MODEH = 1;
      }else if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_3F){
          SystemOutputs.CPU_DEMO_MODEL = 0;
          SystemOutputs.CPU_DEMO_MODEH = 1;
      }else if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_2F){
          SystemOutputs.CPU_DEMO_MODEL = 1;
          SystemOutputs.CPU_DEMO_MODEH = 0;
      }else{
          SystemOutputs.CPU_DEMO_MODEL = 0;
          SystemOutputs.CPU_DEMO_MODEH = 0;
      }
      SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Buzzer Acceso
      SystemOutputs.CPU_DEMO_TOMO = 1;
      _EVSET(_EV0_OUTPUT_CAMBIATI);
      _mutex_unlock(&output_mutex);

      // Ciclo di attesa finta sequenza. La durata dipende dal modo WIDE/NARROW/INTERMEDIATE
      bool error = false;
      while(delay--){

          if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_5F){
              _time_delay(900);
          }else  if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_4F){
              _time_delay(250);
          }else if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_3F){
              _time_delay(330);
          }else if(generalConfiguration.trxCfg.tomo_mode == _TOMO_MODE_2F){
              _time_delay(500);
          }else{
              _time_delay(900);
          }

          if((SystemInputs.CPU_XRAY_REQ==0)) {
              error = true;
              break;                
          }
      } 

      _mutex_lock(&output_mutex);
      SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Buzzer Acceso
      SystemOutputs.CPU_DEMO_TOMO = 0;
      _EVSET(_EV0_OUTPUT_CAMBIATI);
      _mutex_unlock(&output_mutex);

      if(error) _SEQERROR(ERROR_PUSHRX_NO_PREP);
      _time_delay(1000);

      // Ferma subito il braccio
      //actuatorsTrxStop(20);

    }
    
    // Sblocca il compressore     
    if((Param->tomo_mode!=_TOMO_MODE_STATIC)&&(Param->compressor_unlock))   pcb215SetXRaySblocco();
   
    // Chiude con centratura braccio solo se non in calibrazione del detectror
    if(Param->tomo_mode!=_TOMO_MODE_STATIC){
        actuatorsTrxWaitReady(50); // Senza uomo morto attende il naturale fine movimento
        if(!tomoParam.tomo_deadman) actuatorsTrxMove(0); // Senza uomo morto muove sicuro
        else if(SystemInputs.CPU_XRAY_REQ) actuatorsTrxMove(0); // Se c'è l'uomo morto muove solo col pulsante raggi premuto
    }

    // Verifica  Esito da PCB190
    if(!generalConfiguration.demoMode){
       if(_TEST_BIT(PCB190_FAULT)) _SEQERROR(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));
    }



    // Ritardo acquisizione campionamenti
    _time_delay(50); // Attesa 50ms

    if(!generalConfiguration.demoMode){
      mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);
    }else{
        mAs_erogati = (unsigned short) (((float) Param->esposizione.MAS  * Param->tomo_samples) * 50);
    }
    
    tomoSeqResult=TRUE;
        
    debugPrintI("RX-3D SEQUENZA COMPLETATA. mAs",(int) mAs_erogati);

    data[0]=RXOK;
    data[1]=(unsigned char) (mAs_erogati&0xFF);  // Aggiungere mas residui        
    data[2]=(unsigned char) ((mAs_erogati>>8)&0xFF);  
    data[3]= _DEVREGL(RG190_HV_RXEND,PCB190_CONTEST);
    
    // Aggiungere le curve registrate
    mccGuiNotify(1,MCC_CMD_RAGGI_TOMO,data,4); 
 
    // Scarica i dati dell'esposizione se necessario
    if(!generalConfiguration.demoMode) rxNotifyData(2,RXOK);
    
    // Attende fine centratura braccio solo su uomo morto
    if(Param->tomo_mode!=_TOMO_MODE_STATIC){
       if(tomoParam.tomo_deadman){
           // Attende il completamento del posizionamento con il pulsante premuto
           int i = 300;
           while(i--){
              if(SystemInputs.CPU_XRAY_REQ==0){
                  actuatorsTrxStop(20);
                  break;
              }
              if(generalConfiguration.trxExecution.run==false) break;
              _time_delay(100);
           }
           if(i==0) actuatorsTrxStop(20);
       }
    }


   // Sblocco dei processi prima di procedere
   if(Ser422DriverSetReadyAll(5000) == FALSE) printf("FALLITO SBLOCCO DRIVER!!\n");
   else printf("SBLOCCO DRIVER OK\n");

   // Stop inseguimento Filtro
   pcb249U2_exitTomoFilter();

   // Re-imposta la collimazione 2D
   pcb249U2SetColli( generalConfiguration.colliCfg.lame2D.back , generalConfiguration.colliCfg.lame2D.front);
   pcb249U1SetColli(generalConfiguration.colliCfg.lame2D.left,generalConfiguration.colliCfg.lame2D.right,generalConfiguration.colliCfg.lame2D.trap);

    // Se richiesto viene spento lo starter
    if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
        if(generalConfiguration.pcb190Cfg.starter_off_with_brake) pcb190StopStarter();
        else pcb190OffStarter();
    }

    _EVSET(_SEQEV_RX_TOMO_TERMINATED);
    
  } // while
}

void RxTomoSeqError(int code)
{
    unsigned char data[5];
    unsigned short mAs_erogati=0;
    
    
    // Ferma subito il braccio
    actuatorsTrxStop(20);
    
    _mutex_lock(&output_mutex);
    SystemOutputs.CPU_XRAY_ENA=0;   // Disattivazione segnale XRAY ENA
    SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Disattivazione Buzzer
    _EVSET(_EV0_OUTPUT_CAMBIATI);         
    _mutex_unlock(&output_mutex);

    // Segnala il risultato
    tomoSeqResult=FALSE;
    
    // Sblocca i drivers
    Ser422DriverSetReadyAll(5000);
    
     // Sblocca il compressore se deve
    if(Param->compressor_unlock)
        pcb215SetSblocco();
    
    // Stop inseguimento Filtro
    pcb249U2_exitTomoFilter();

    // Re-imposta la collimazione 2D
    pcb249U2SetColli( generalConfiguration.colliCfg.lame2D.back , generalConfiguration.colliCfg.lame2D.front);
    pcb249U1SetColli(generalConfiguration.colliCfg.lame2D.left,generalConfiguration.colliCfg.lame2D.right,generalConfiguration.colliCfg.lame2D.trap);

    // Verifica se c'è stata radiazione
    if((code>LAST_ERROR_NO_PREP)&&(code<LAST_ERROR_WITH_PREP))
    {
        
        // Lettura mas residui
        if(Ser422ReadRegister(_REGID(RG190_MAS_EXIT),10,&PCB190_CONTEST) == _SER422_NO_ERROR){
            // mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST)/50;
            mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);
        }
    }
    

    debugPrintI("RX-3D ERRORE SEQUENZA",code );
    tomoError = code;
    data[0]=tomoError;       
    data[1]=(unsigned char) (mAs_erogati&0xFF);  // Aggiungere mas residui        
    data[2]=(unsigned char) ((mAs_erogati>>8)&0xFF);      
    data[3]= _DEVREGL(RG190_HV_RXEND,PCB190_CONTEST); 
    mccGuiNotify(1,MCC_CMD_RAGGI_TOMO,data,4); 

    // Reset errori per consentire di eseguire comandi sui dispositivi della PCB190
    pcb190ResetFault();


    // Carica i dati relativi all'esposizione se necessario
    if(!generalConfiguration.demoMode) rxNotifyData(2,code);


    // Se richiesto viene spento lo starter
    if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
        if(generalConfiguration.pcb190Cfg.starter_off_with_brake) pcb190StopStarter();
        else pcb190OffStarter();
    }

    
    _EVSET(_SEQEV_RX_TOMO_TERMINATED);
    return;
}


/* EOF */
