#define _SEQRX_STD_AEC_C
#include "dbt_m4.h"

#undef _SEQERROR
#undef _SEQERRORFUNC
#undef PARAM
#undef RESULT
#undef ISRUNNING
#undef ERROR

///////////////////////////////////////////////////////////////////////////////
// Customizzazione dati sequenza
#define _SEQERRORFUNC RxStdAecSeqError
#define PARAM rxStdAecParam
#define RESULT stdAecSeqResult
#define ISRUNNING rxStdAecIsRunning
#define ERROR stdAecError
///////////////////////////////////////////////////////////////////////////////

static void _SEQERRORFUNC(int code);
_RxStdSeq_Str PARAM;
bool RESULT=FALSE;
bool ISRUNNING=FALSE;
unsigned char ERROR;
#define Param (&PARAM)
#define _SEQERROR(code) {_SEQERRORFUNC(code); continue;}

void std_aec_rx_task(uint32_t taskRegisters)
{
  unsigned char data[10];
  unsigned char addr,banco;
  
  unsigned char i;
  unsigned short sval;
  unsigned char chk;
  
 
  printf("PARTENZA SEQUENZA PER GESTIONE RAGGI STANDARD IN AEC MODE\n");
  _EVCLR(_SEQEV_RX_STD_AEC_START);
  
  while(1)
  {
    // Attende fino a nuova partenza
    ISRUNNING=FALSE;
    _EVCLR(_SEQEV_RX_STD_AEC_START);
    _EVWAIT_ALL(_SEQEV_RX_STD_AEC_START);
    _EVCLR(_SEQEV_RX_STD_AEC_TERMINATED);
    RESULT=FALSE;
    ISRUNNING=TRUE;
    ERROR=0;

    if(generalConfiguration.demoMode) debugPrint("RX-2D-AEC START IN DEMO MODE");
    else  debugPrint("RX-2D-AEC START SEQUENCE");


    // Prima di andare in freeze bisogna accertarsi che la collimazione 2D sia andata a buon fine
    if(wait2DBackFrontCompletion(100)==false) _SEQERROR(ERROR_INVALID_COLLI);
    if(wait2DLeftRightTrapCompletion(100)==false) _SEQERROR(ERROR_INVALID_COLLI);
    // Specchio fuori campo se non è già stato  levato (comando compatibile FREEZE)
    if(pcb249U2MirrorHome()==FALSE)_SEQERROR(ERROR_MIRROR_LAMP);
    //if(pcb249U2Lamp(2,100,true) == FALSE) _SEQERROR(ERROR_MIRROR_LAMP);

    // Disabilita tutti i drivers
    Ser422DriverFreezeAll(0);    

    //________________________________________________________________________________________________
    // Prima di procedere bisogna verificare se il filtro ha terminato correttamente il posizionamento
    if(waitRxFilterCompletion()==FALSE)  _SEQERROR(ERROR_INVALID_FILTRO);


    // Verifica Chiusura porta
    if((SystemInputs.CPU_CLOSED_DOOR==0) && (!generalConfiguration.demoMode))
    {
      debugPrint("RX-2D-AEC ERRORE PORTA STUDIO APERTA");
      _SEQERROR(ERROR_CLOSED_DOOR);  
    }

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();

    // Attiva Starter precocemente
    if(!generalConfiguration.demoMode){
      if(Param->esposizione.HV & 0x4000)
      {
        if(pcb190StarterH()==FALSE) debugPrint("RX-2D-AEC COMANDO STARTER HS FALLITO");
        else debugPrint("RX-2D-AEC STARTER HS ATTIVATO");
      }else
      {
        if(pcb190StarterL()==FALSE) debugPrint("RX-2D-AEC COMANDO STARTER LS FALLITO");
        else debugPrint("RX-2D-AEC STARTER LS ATTIVATO");
      }
    }


    // Caricamento parametri di esposizione 
    if(pcb190UploadExpose(Param, FALSE)==FALSE) _SEQERROR(_SEQ_UPLOAD190_PARAM);      
    debugPrintI3("RX-2D-AEC PRE-EXP DATA, IDAC", Param->esposizione.I & 0x0FFF, "VDAC",Param->esposizione.HV & 0x0FFF,"MASDAC", Param->esposizione.MAS);

    // Verifica su XRAY_REQ(Pulsante raggi premuto)
    if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);

    if(!generalConfiguration.demoMode){
      
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=1;   // Attivazione segnale XRAY ENA
        SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Attivazione Buzzer
        _EVSET(_EV0_OUTPUT_CAMBIATI);         
        _mutex_unlock(&output_mutex);
        
        // Attende i segnali
        if(SystemInputs.CPU_XRAY_ENA_ACK==0)
        {   
          _EVCLR(_EV2_XRAY_ENA_ON);
          _EVCLR(_EV2_XRAY_REQ_OFF);  
           if(_EVWAIT_TANY(_MOR2(_EV2_XRAY_ENA_ON,_EV2_XRAY_REQ_OFF),_WAIT_XRAY_ENA)==FALSE) _SEQERROR(_SEQ_IO_TIMEOUT);
         }
        if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);
        
        // Comando Attivazione Raggi
        mAs_pre = Param->esposizione.MAS;// /50;
        if(waitPcb190Ready(50)==FALSE) _SEQERROR(_SEQ_PCB190_BUSY);
        _EVCLR(_EV2_WAIT_AEC);    

        int rc = pcb190StartRxAecStd();
        if(rc==SER422_BUSY) _SEQERROR(_SEQ_PCB190_BUSY);
        if(rc==SER422_ILLEGAL_FUNCTION) _SEQERROR(ERROR_PUSHRX_NO_PREP);


        aecIsValid =TRUE;
        debugPrint("RX-2D-AEC Attesa AEC");
  
        // Ciclo attesa dati AEC Attende dati AEC
        i = 15; // Massima attesa AEC
        while(i)
        {
          if(_EVWAIT_TALL(_EV2_WAIT_AEC,1000)==FALSE) 
          {
            if(SystemInputs.CPU_XRAY_COMPLETED) break; // Errore procedura raggi
            if(--i == 0) break;   
          }else break;
        }
    
        // Se nel frattempo la 190 è andata in Fault e ha terminato anticipatamente..
        if(SystemInputs.CPU_XRAY_COMPLETED)
        {
          // Upload manuale registri dati (il sistema è ancora in FREEZE)
          pcb190GetPostRxRegisters();
          debugPrint("RX-2D-AEC ERRORE SEQUENZA RAGGI DURANTE ATTESA AEC");
          _SEQERROR(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));      
        }
    
        // Dati AEC giunti
        if(aecExpIsValid==FALSE) _SEQERROR(_SEQ_AEC_NOT_AVAILABLE);
        debugPrintI3("RX-2D-AEC EXP DATA, IDAC", Param->esposizione.I & 0x0FFF, "VDAC",Param->esposizione.HV & 0x0FFF,"MASDAC", Param->esposizione.MAS);

        // Dati AEC giunti: ricarica i dati alla PCB190
        pcb190UploadExpose(Param, TRUE);

        // Attesa XRAY COMPLETED da Bus Hardware
        if(SystemInputs.CPU_XRAY_COMPLETED==0)
        {
          _EVCLR(_EV2_XRAY_COMPLETED);
          if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) _SEQERROR(_SEQ_PCB190_TMO);      
        }


        // Lettura esito raggi
        if(pcb190GetPostRxRegisters()==FALSE){
            debugPrint("RX-2D-AEC ERRORE DURANTE LETTURA REGISTRI FINE RAGGI");
            _SEQERROR(_SEQ_READ_REGISTER);
        }

        if(_TEST_BIT(PCB190_FAULT)) _SEQERROR(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));
        mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);
        
        // Calcolo dei dati di post esposizione
        data[0]=RXOK;       
        data[1]=(unsigned char) ((mAs_erogati)&0xFF);  // Aggiungere mas residui
        data[2]=(unsigned char) (((mAs_erogati)>>8)&0xFF);

        data[3]= _DEVREGL(RG190_HV_RXEND,PCB190_CONTEST);

        // Notifica anticipata per velocizzare il termine della sequenza
        mccGuiNotify(1,MCC_CMD_RAGGI_AEC,data,4); 

        // Carica i dati relativi all'esposizione
        rxNotifyData(1,RXOK);


    }else{
      
        // PRE-IMPULSO DEMO
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=0;   // Nessuna abilitazione ai raggi
        SystemOutputs.CPU_DEMO_ACTIVATION = 1;   // Buzzer Acceso
        _EVSET(_EV0_OUTPUT_CAMBIATI);         
        _mutex_unlock(&output_mutex);      
        
        _time_delay(600);
        
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=0;   // Nessuna abilitazione ai raggi
        SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Buzzer Spento
        _EVSET(_EV0_OUTPUT_CAMBIATI);         
        _mutex_unlock(&output_mutex);      
        
        _time_delay(2000);

        printf("DATI AEC ARRIVATI!\n");
        
        // IMPULSO DEMO
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=0;   // Nessuna abilitazione ai raggi
        SystemOutputs.CPU_DEMO_ACTIVATION = 1;   // Buzzer Acceso
        _EVSET(_EV0_OUTPUT_CAMBIATI);         
        _mutex_unlock(&output_mutex);      
        
        _time_delay(1500);
        
        _mutex_lock(&output_mutex);
        SystemOutputs.CPU_XRAY_ENA=0;   // Nessuna abilitazione ai raggi
        SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Buzzer Spento
        _EVSET(_EV0_OUTPUT_CAMBIATI);         
        _mutex_unlock(&output_mutex);      
        
        _time_delay(1000);

        // Lettura mas residui
        mAs_erogati = Param->esposizione.MAS;        
       
        // Calcolo dei dati di post esposizione
        data[0]=RXOK;       
        data[1]=(unsigned char) ((mAs_erogati)&0xFF);  // Aggiungere mas residui
        data[2]=(unsigned char) (((mAs_erogati)>>8)&0xFF);
        data[3]= 0;

        // Notifica anticipata per velocizzare il termine della sequenza
        mccGuiNotify(1,MCC_CMD_RAGGI_AEC,data,4); 

    }
    
    
    // Sblocca i drivers dallo stato di freeze
    if(Ser422DriverSetReadyAll(5000) == FALSE) _SEQERROR(_SEQ_DRIVER_READY);
    printf("SBLOCCO DRIVER OK\n");
    
    _mutex_lock(&output_mutex);
    SystemOutputs.CPU_XRAY_ENA=0;   // Disattivazione segnale XRAY ENA
    SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Disattivazione Buzzer
    _EVSET(_EV0_OUTPUT_CAMBIATI);         
    _mutex_unlock(&output_mutex);
       
 
    // Sblocca il compressore se deve
    if(Param->compressor_unlock)
        pcb215SetSblocco();
 

    //----------------------------------------------------------//
    //                       Fine sequenza
    //----------------------------------------------------------//  
    RESULT=TRUE;

    // Se richiesto viene spento lo starter
    if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
        if(generalConfiguration.pcb190Cfg.starter_off_with_brake) pcb190StopStarter();
        else pcb190OffStarter();
    }

    // Stringa di debug
    debugPrintF("RX-2D-AEC SEQUENZA COMPLETATA. mAs",(float) mAs_erogati/50);

    _EVSET(_SEQEV_RX_STD_AEC_TERMINATED);
     
  } // while
  
 
}

void _SEQERRORFUNC(int code)
{
    unsigned char data[10];
    unsigned short mAs_erogati=0;
  
    _mutex_lock(&output_mutex);
    SystemOutputs.CPU_XRAY_ENA=0;   // Disattivazione segnale XRAY ENA
    SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Disattivazione Buzzer
    _EVSET(_EV0_OUTPUT_CAMBIATI);         
    _mutex_unlock(&output_mutex);

    // Segnala il risultato
    RESULT=FALSE;
    
    // Sblocca il compressore se deve
    if(Param->compressor_unlock)
        pcb215SetSblocco();
    
    // Verifica se c'è stata radiazione
    if((code>LAST_ERROR_NO_PREP)&&(code<LAST_ERROR_WITH_PREP))
    {
        // Lettura mas residui
        if(Ser422ReadRegister(_REGID(RG190_MAS_EXIT),10,&PCB190_CONTEST) == _SER422_NO_ERROR){
            // mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST)/50;
            mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);
        }
    }

    // Stringa di debug
    debugPrintI2("RX-2D-AEC SEQUENZA FALLITA, ERRORE",code, "mAs",mAs_erogati);

    ERROR = code;
    data[0]=ERROR;       
    data[1]=(unsigned char) (mAs_erogati&0xFF);  // Aggiungere mas residui        
    data[2]=(unsigned char) ((mAs_erogati>>8)&0xFF);  
    data[3]=_DEVREGL(RG190_HV_RXEND,PCB190_CONTEST); 

    mccGuiNotify(1,MCC_CMD_RAGGI_AEC,data,4);     

    // Reset errori per consentire di eseguire comandi sui dispositivi della PCB190
    pcb190ResetFault();

    // Carica i dati relativi all'esposizione se necessario
    if(!generalConfiguration.demoMode) rxNotifyData(1,code);

    // Attiva tutti i drivers
    Ser422DriverSetReadyAll(5000);
    printf("SBLOCCO DRIVER OK\n");
    
    // Se richiesto viene spento lo starter
    if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
        if(generalConfiguration.pcb190Cfg.starter_off_with_brake) pcb190StopStarter();
        else pcb190OffStarter();
    }

    _EVSET(_SEQEV_RX_STD_AEC_TERMINATED);
    return;
}


/* EOF */

