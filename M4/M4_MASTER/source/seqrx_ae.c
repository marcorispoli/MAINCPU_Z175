#define _SEQRX_AE_C
#include "dbt_m4.h"

#undef _SEQERROR
#undef _SEQERRORFUNC
#undef PARAM
#undef RESULT
#undef ISRUNNING
#undef ERROR

///////////////////////////////////////////////////////////////////////////////
// Customizzazione dati sequenza
#define _SEQERRORFUNC RxAeSeqError
#define PARAM rxAeParam
#define RESULT stdAeSeqResult
#define ISRUNNING rxAeIsRunning
#define ERROR stdAeError
///////////////////////////////////////////////////////////////////////////////

static void _SEQERRORFUNC(int code);
_RxStdSeq_Str PARAM;
bool RESULT=FALSE;
bool ISRUNNING=FALSE;
unsigned char ERROR;
#define Param (&PARAM)
#define _SEQERROR(code) {_SEQERRORFUNC(code); continue;}

void std_ae_rx_task(uint32_t taskRegisters)
{
  unsigned char data[10];  
  unsigned char i;
  bool isAEC = false;

 
  printf("PARTENZA THREAD SEQUENZA PER GESTIONE RAGGI ALTA ENERGIA\n");
  _EVCLR(_SEQEV_RX_AE_START);
  

  while(1)
  {
    // Attende fino a nuova partenza
    ISRUNNING=FALSE;
    _EVCLR(_SEQEV_RX_AE_START);
    _EVWAIT_ALL(_SEQEV_RX_AE_START);
    _EVCLR(_SEQEV_RX_AE_TERMINATED);
    RESULT=FALSE;
    ISRUNNING=TRUE;
    ERROR=0;

    aecIsValid =FALSE;

    // Definisce se la sequenza è AEC o normale
    isAEC = Param->isAEC;

    mAs_pre = 0;
    mAs_erogati = 0;
    mAs_erogati_AE = 0;

    // In Demo mode il comando non viene eseguito
    if(generalConfiguration.demoMode) debugPrint("RX-AE ATTIVAZIONE IN DEMO MODE");
    else  debugPrint("RX-AE ATTIVAZIONE SEQUENZA");



    // Prima di andare in freeze bisogna accertarsi che la collimazione 2D sia andata a buon fine
    if(wait2DBackFrontCompletion(100)==false) _SEQERROR(ERROR_INVALID_COLLI);
    if(wait2DLeftRightTrapCompletion(100)==false) _SEQERROR(ERROR_INVALID_COLLI);

    // Specchio fuori campo
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
      debugPrint("RX-AE PORTA STUDIO APERTA");
      _SEQERROR(ERROR_CLOSED_DOOR);  
    }

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();

    if(Param->esposizione.HV & 0x4000)
    {
      if(pcb190StarterH()==FALSE) debugPrint("RX-AE  COMANDO STARTER HS FALLITO");
      else debugPrint("RX-AE STARTER HS ATTIVATO");
    }else
    {
      if(pcb190StarterL()==FALSE) debugPrint("RX-AE COMANDO STARTER LS");
      else debugPrint("RX-AE STARTER LS ATTIVATO");
    }


    // Caricamento parametri di esposizione o pre esposizione per impulso bassa energia
    if(pcb190UploadExpose(Param, FALSE)==FALSE) _SEQERROR(_SEQ_UPLOAD190_PARAM);      
 
    if(isAEC) debugPrint("RX-AE DATI PRE-IMPULSO");
    else debugPrint("RX-AE DATI IMPULSO BASSA ENERGIA");
    debugPrintI3("RX-AE EXP DATA, IDAC", Param->esposizione.I & 0x0FFF, "VDAC",Param->esposizione.HV & 0x0FFF,"MASDAC", Param->esposizione.MAS);

    // Verifica su XRAY_REQ(Pulsante raggi premuto)
    if(SystemInputs.CPU_XRAY_REQ==0)  _SEQERROR(ERROR_PUSHRX_NO_PREP);
      
    _mutex_lock(&output_mutex);
    SystemOutputs.CPU_XRAY_ENA=1;   // Attivazione segnale XRAY ENA
    _EVSET(_EV0_OUTPUT_CAMBIATI);
    _mutex_unlock(&output_mutex);
        
    // Attende i segnali
    if(SystemInputs.CPU_XRAY_ENA_ACK==0)
    {
      _EVCLR(_EV2_XRAY_ENA_ON);
      _EVCLR(_EV2_XRAY_REQ_OFF);
       if(_EVWAIT_TANY(_MOR2(_EV2_XRAY_ENA_ON,_EV2_XRAY_REQ_OFF),_WAIT_XRAY_ENA)==FALSE) _SEQERROR(_SEQ_IO_TIMEOUT);
     }
        
    // Comando Attivazione Raggi
    if(waitPcb190Ready(50)==FALSE) _SEQERROR(_SEQ_PCB190_BUSY);

    _EVCLR(_EV2_WAIT_AEC);
    int rc;
    if(isAEC) rc = pcb190StartRxAecStd();
    else rc = pcb190StartRxStd();

    if(rc==SER422_BUSY) _SEQERROR(_SEQ_PCB190_BUSY);
    if(rc==SER422_ILLEGAL_FUNCTION) _SEQERROR(ERROR_PUSHRX_NO_PREP);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                          ATTESA FINE AEC  BASSA ENERGIA
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    if(isAEC){

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                //                          ATTESA DATI IMPULSO AEC
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                aecIsValid =TRUE;

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
                  debugPrint("RX-AE ERRORE SEQUENZA RAGGI DURANTE ATTESA AEC");
                  _SEQERROR(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));
                }

                // Dati AEC giunti
                if(aecExpIsValid==FALSE) _SEQERROR(_SEQ_AEC_NOT_AVAILABLE);
                debugPrintI3("RX-AE AEC EXPOSURE DATA, IDAC", Param->esposizione.I & 0x0FFF, "VDAC",Param->esposizione.HV & 0x0FFF,"MASDAC", Param->esposizione.MAS);

                // Dati AEC giunti: ricarica i dati alla PCB190
                pcb190UploadExpose(Param, TRUE);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                          ATTESA FINE IMPULSO BASSA ENERGIA
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Attesa Dati per impulso AE ... \n");
    _time_delay(500);
  
    // Attesa XRAY COMPLETED da Bus Hardware
    if(SystemInputs.CPU_XRAY_COMPLETED==0)
    {
      _EVCLR(_EV2_XRAY_COMPLETED);
      if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) _SEQERROR(_SEQ_PCB190_TMO);
    }

    // Lettura esito raggi
    if(pcb190GetPostRxRegisters()==FALSE){
        debugPrint("RX-AE ERRORE DURANTE LETTURA REGISTRI FINE RAGGI");
        _SEQERROR(_SEQ_READ_REGISTER);

    }

    if(_TEST_BIT(PCB190_FAULT)) _SEQERROR(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));

    // mAs_erogati contengono il valore dei mAs in Bassa
    mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);// /50;
    if(isAEC) mAs_pre = Param->esposizione.MAS;// /50;

    // Impostazione filtro Rame per impulso AE
    pcb249U2RxSetFiltroCmd(3); // Filtro al rame: da modificare con la corretta configurazione

    //________________________________________________________________________________________________
    // Prima di procedere bisogna verificare se il filtro ha terminato correttamente il posizionamento
    if(waitRxFilterCompletion()==FALSE)  _SEQERROR(ERROR_INVALID_FILTRO);

    // Ciclo attesa dati AE
    i = 150; // Massima attesa AE
    int j=20; // 2 secondi minimo per il rilascio pulsante raggi
    while((i>0 && j>0))
    {
      if(_EVWAIT_TALL(_EV2_WAIT_AE_CONTEXT,100)==FALSE)
      {
        // Verifica pulsante raggi
        if(SystemInputs.CPU_XRAY_REQ==0) j--;
        else j = 20;
        i--;
      }else break;
    }
    if(i==0) _SEQERROR(_SEQ_AEC_NOT_AVAILABLE);
    if(j==0) _SEQERROR(ERROR_PUSHRX_AFTER_PREP);

    // Caricamento parametri di esposizione ALTA ENERGIA
    if(pcb190UploadExpose(Param, FALSE)==FALSE) _SEQERROR(_SEQ_UPLOAD190_PARAM);
    debugPrintI3("RX-AE HIGH PULSE EXPOSURE DATA, IDAC", Param->esposizione.I & 0x0FFF, "VDAC",Param->esposizione.HV & 0x0FFF,"MASDAC", Param->esposizione.MAS);

    // Ricarica nuova sequenza
    rc = pcb190StartRxStd();
    if(rc==SER422_BUSY) _SEQERROR(_SEQ_PCB190_BUSY);
    if(rc==SER422_ILLEGAL_FUNCTION) _SEQERROR(ERROR_PUSHRX_NO_PREP);
    _time_delay(500);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //                          ATTESA FINE IMPULSO ALTA ENERGIA
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    // Attesa XRAY COMPLETED da Bus Hardware
    if(SystemInputs.CPU_XRAY_COMPLETED==0)
    {
      _EVCLR(_EV2_XRAY_COMPLETED);
      if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) _SEQERROR(_SEQ_PCB190_TMO);
    }

    // Lettura esito raggi
    if(pcb190GetPostRxRegisters()==FALSE){
        debugPrint("RX-AE ERRORE DURANTE LETTURA REGISTRI FINE RAGGI");
        _SEQERROR(_SEQ_READ_REGISTER);

    }

    if(_TEST_BIT(PCB190_FAULT)) _SEQERROR(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));

    debugPrint("RX-AE SEQUENZA ALTA ENERGIA COMPLETATA CON SUCCESSO");
    mAs_erogati_AE = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);// /50;
        
    // Calcolo dei dati di post esposizione
    data[0]=RXOK;
    data[1]=(unsigned char) ((mAs_erogati)&0xFF);
    data[2]=(unsigned char) (((mAs_erogati)>>8)&0xFF);
    data[3]= _DEVREGL(RG190_HV_RXEND,PCB190_CONTEST);
    data[4]=(unsigned char) ((mAs_erogati_AE)&0xFF);
    data[5]=(unsigned char) (((mAs_erogati_AE)>>8)&0xFF);

    // Notifica anticipata per velocizzare il termine della sequenza
    mccGuiNotify(1,MCC_CMD_RAGGI_AE,data,6);

    // Carica i dati relativi all'esposizione
    rxNotifyData(1,RXOK);

    
    // Sblocca i drivers dallo stato di freeze
    if(Ser422DriverSetReadyAll(5000) == FALSE) _SEQERROR(_SEQ_DRIVER_READY);
    printf("SBLOCCO DRIVER OK\n");
    
    _mutex_lock(&output_mutex);
    SystemOutputs.CPU_XRAY_ENA=0;   // Disattivazione segnale XRAY ENA
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

    _EVSET(_SEQEV_RX_AE_TERMINATED);
     
  } // while
  
 
}

void _SEQERRORFUNC(int code)
{
    unsigned char data[10];
  
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
        if(Ser422ReadRegister(_REGID(RG190_MAS_EXIT),10,&PCB190_CONTEST) == _SER422_NO_ERROR)
        {
            if(mAs_erogati==0) mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);// /50;
            else mAs_erogati_AE = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);// /50;
        }
    }

    // Stringa di debug
    ERROR = code;
    data[0]=ERROR;       
    data[1]=(unsigned char) (mAs_erogati&0xFF);  // Aggiungere mas residui        
    data[2]=(unsigned char) ((mAs_erogati>>8)&0xFF);  
    data[3]=_DEVREGL(RG190_HV_RXEND,PCB190_CONTEST); 
    data[4]=(unsigned char) (mAs_erogati_AE&0xFF);  // Aggiungere mas residui
    data[5]=(unsigned char) ((mAs_erogati_AE>>8)&0xFF);
    mccGuiNotify(1,MCC_CMD_RAGGI_AE,data,6);

    // Reset errori per consentire di eseguire comandi sui dispositivi della PCB190
    pcb190ResetFault();
    debugPrintI4("RX-AE ERROR", code,"mAsLow", mAs_erogati,"mAsAE", mAs_erogati_AE,"kVBUS", data[3] );

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

    _EVSET(_SEQEV_RX_AE_TERMINATED);
    return;
}


/* EOF */

