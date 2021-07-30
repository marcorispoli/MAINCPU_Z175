#define _ANALOG_RX_SEQUENCES_C
#include "dbt_m4.h"


#undef PARAM
#undef RESULT
#undef ISRUNNING
#undef ERROR

///////////////////////////////////////////////////////////////////////////////
// Customizzazione dati sequenza
#define _SEQERRORFUNC RxAnalogSeqError
#define PARAM rxStdParam
#define ISRUNNING rxAnalogIsRunning
///////////////////////////////////////////////////////////////////////////////

static void RxAnalogSeqError(int codice);
static void fineSequenza(void);
static int AnalogPreCalibration(void);
static int AnalogProfileCalibration(void);
static int AnalogTubeCalibration(void);
void getRxSamplesData(float* kv, float* imed, int* time);

static int setXrayEna(void);

_RxStdSeq_Str PARAM;
bool ISRUNNING=FALSE;
#define Param (&PARAM)

void analog_rx_task(uint32_t taskRegisters)
{
  int result=0;

  printf("PARTENZA SEQUENZA PER GESTIONE RAGGI MACCHINE ANALOGICHE\n");
  _EVCLR(_SEQEV_RX_ANALOG_START);

  while(1)
  {
    // Attende fino a nuova partenza
    ISRUNNING=FALSE;
    _EVCLR(_SEQEV_RX_ANALOG_START);
    _EVWAIT_ALL(_SEQEV_RX_ANALOG_START);
    _EVCLR(_SEQEV_RX_ANALOG_TERMINATED);
    ISRUNNING=TRUE;

    // Condizioni comuni a tutte le esposizioni
    if(generalConfiguration.demoMode) printf("DEMO MODE\n");
    else  printf("ESPOSIZIONE REALE\n");

    // Prima di andare in freeze bisogna accertarsi che la collimazione 2D sia andata a buon fine
    if(wait2DBackFrontCompletion(50)==false){
        RxAnalogSeqError(ERROR_INVALID_COLLI);
        fineSequenza();
        continue;
    }

    if(wait2DLeftRightTrapCompletion(50)==false){
        RxAnalogSeqError(ERROR_INVALID_COLLI);
        fineSequenza();
        continue;
    }

    // Disabilita tutti i drivers ed attende che tutti i driver siano fermi
    Ser422DriverFreezeAll(0);

    //________________________________________________________________________________________________
    // Prima di procedere bisogna verificare se il filtro ha terminato correttamente il posizionamento
    if(waitRxFilterCompletion()==FALSE) {
        RxAnalogSeqError(ERROR_INVALID_FILTRO);
        fineSequenza();
        continue;
    }

    // Specchio fuori campo se non è già stato  levato (comando compatibile FREEZE)
    if(pcb249U2Lamp(2,100,true) == FALSE){
        RxAnalogSeqError(ERROR_MIRROR_LAMP);
        fineSequenza();
        continue;
    }


    // Selezione della sequenza da eseguire
    switch(Param->analog_sequence){
    case EXPOSIMETER_CALIBRATION_PULSE: result = AnalogPreCalibration();
        break;
    case EXPOSIMETER_CALIBRATION_PROFILE: result = AnalogProfileCalibration();
        break;
    case TUBE_CALIBRATION_PROFILE: result = AnalogTubeCalibration();
        break;
    case MANUAL_MODE_EXPOSURE: result = AnalogManualModeExposure();
        break;
    }

    if(result!=0) RxAnalogSeqError(result);
    else printf("SEQUENZA COMPLETATA CON SUCCESSO\n");

    fineSequenza();


  } // while
} // Thread

int setXrayEna(void){
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
            if(_EVWAIT_TANY(_MOR2(_EV2_XRAY_ENA_ON,_EV2_XRAY_REQ_OFF),_WAIT_XRAY_ENA)==FALSE) return _SEQ_IO_TIMEOUT;
        }
        if(SystemInputs.CPU_XRAY_REQ==0)  return ERROR_PUSHRX_NO_PREP;
        _time_delay(100); // Non levare il delay
        return 0;
}

// Sequenza di calibrazione pre impulso esposimetro e calibrazione
// rilevamento PLOG
int AnalogTubeCalibration(void){

    int error=0;

    printf("ESECUZIONE PROCEDURA DI CALIBRAZIONE TUBO\n");

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();

    if(pcb190StarterH()==FALSE) printf("WARNING: COMANDO STARTER HIGH FALLITO\n");
    else printf("STARTER ATTIVATO AD ALTA VELOCITA'\n");

    // Caricamento parametri di esposizione
    if(pcb190UploadAnalogCalibTubeExpose(Param)==FALSE) return _SEQ_UPLOAD190_PARAM;

    printf("DATI IMPULSO --------------------------\n");
    printf("IDAC:%d\n",Param->esposizione.I & 0x0FFF);
    printf("VDAC:%d\n",Param->esposizione.HV & 0x0FFF);
    printf("MASDAC:%d\n",Param->esposizione.MAS);
    printf("--------------------------------------\n");


    // Verifica su XRAY_REQ(Pulsante raggi premuto)
    if(SystemInputs.CPU_XRAY_REQ==0)  return ERROR_PUSHRX_NO_PREP;

    // Impostazione Segnale XRAY_ENA su Bus Hardware
    error = setXrayEna();
    if(error) return error;

    // Legge il Busy per attendere che sia tutto pronto
    if(waitPcb190Ready(50)==FALSE) return _SEQ_PCB190_BUSY;


    // Parte l'esposimetro per la modalità manuale (libera il segnale di XRAY-DISABLE )
    if(pcb244_A_StartCalibTube()==false) return _SEQ_PCB190_BUSY;

    // Parte la PCB190
    int rc = pcb190StartRxStd();
    if(rc==SER422_BUSY) return _SEQ_PCB190_BUSY;
    if(rc==SER422_ILLEGAL_FUNCTION) return ERROR_PUSHRX_NO_PREP;

    // Un minimo di attesa per consentire ai vari segnali di sincronizzarsi
    _time_delay(1000);

    printf("Attesa Completamento \n");

    // Attesa XRAY COMPLETED da Bus Hardware
    if(SystemInputs.CPU_XRAY_COMPLETED==0)
    {
      _EVCLR(_EV2_XRAY_COMPLETED);
      if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) return _SEQ_PCB190_TMO;
    }
    printf("Completato\n");

    // Per sicurezza attiva il bit di stop sull'esposimetro
    PCB244_A_SetRxStop();

    // Lettura esito raggi
    if(pcb190GetPostRxRegisters()==FALSE){
        printf("ERRORE DURANTE LETTURA REGISTRI FINE RAGGI!!!!!!! \n");
        return _SEQ_READ_REGISTER;
    }
    mAs_erogati = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST);

    // Analisi della condizione di FAULT
    // ATTENZIONE BISOGNA ESCLUDERE IL TIMEOUT TRA LE CAUSE DI ERRORE
    if(_TEST_BIT(PCB190_FAULT)){
        if(_DEVREGL(RG190_FAULTS,PCB190_CONTEST) != ERROR_TMO_RX)   return(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));
    }

    // Rileva i dati campionati per la risposta che avverrà con il codice MCC_RAGGI_DATA
    rxNotifyData(0,RXOK);

    printf("FINE SEQUENZA OK\n");
    return 0; // RISULTATO POSITIVO

} // AnalogPreCalibration



// Sequenza di calibrazione pre impulso esposimetro e calibrazione
// rilevamento PLOG
int AnalogPreCalibration(void){

    int error=0;

    printf("ESECUZIONE PROCEDURA DI CALIBRAZIONE DETECTOR\n");

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();

    // Attiva Starter precocemente
    if(!generalConfiguration.demoMode){
      if(Param->esposizione.HV & 0x4000)
      {
        if(pcb190StarterH()==FALSE) printf("WARNING: COMANDO STARTER HIGH FALLITO\n");
        else printf("STARTER ATTIVATO AD ALTA VELOCITA'\n");
      }else
      {
        if(pcb190StarterL()==FALSE) printf("WARNING: COMANDO STARTER LOW FALLITO\n");
        else printf("STARTER ATTIVATO A BASSA VELOCITA'\n");
      }
    }

    // Caricamento parametri di esposizione
    if(pcb190UploadAnalogOnlyPreExpose(Param)==FALSE) return _SEQ_UPLOAD190_PARAM;

    printf("DATI IMPULSO --------------------------\n");
    printf("IDAC:%d\n",Param->esposizione.I & 0x0FFF);
    printf("VDAC:%d\n",Param->esposizione.HV & 0x0FFF);
    printf("MASDAC:%d\n",Param->esposizione.MAS);
    printf("--------------------------------------\n");


    // Verifica su XRAY_REQ(Pulsante raggi premuto)
    if(SystemInputs.CPU_XRAY_REQ==0)  return ERROR_PUSHRX_NO_PREP;

    // Impostazione Segnale XRAY_ENA su Bus Hardware
    error = setXrayEna();
    if(error) return error;

    // Legge il Busy per attendere che sia tutto pronto
    printf("Attesa PCB190 ready ..");
    if(waitPcb190Ready(50)==FALSE) return _SEQ_PCB190_BUSY;
    printf("OK\n");

    // Parte l'esposimetro
    printf("Start Detector Pre ..");
    if(pcb244_A_StartRxPre()==false) return _SEQ_PCB190_BUSY;
    printf("OK\n");

    // Parte la PCB190
    printf("Start PCB190 ..");
    int rc = pcb190StartRxStd();
    if(rc==SER422_BUSY) return _SEQ_PCB190_BUSY;
    if(rc==SER422_ILLEGAL_FUNCTION) return ERROR_PUSHRX_NO_PREP;
    printf("OK\n");

    // Un minimo di attesa per consentire ai vari segnali di sincronizzarsi
    _time_delay(1000);

    printf("Attesa Completamento \n");

    // Attesa XRAY COMPLETED da Bus Hardware
    if(SystemInputs.CPU_XRAY_COMPLETED==0)
    {
      _EVCLR(_EV2_XRAY_COMPLETED);
      if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) return _SEQ_PCB190_TMO;
    }
    printf("Completato\n");

    // Per sicurezza attiva il bit di stop sull'esposimetro
    PCB244_A_SetRxStop();

    // Lettura campionamenti
    if(PCB244_A_GetPreRad(5000)==false){
        printf("IMPOSSIBILE LEGGERE RAD");
        return _SEQ_PCB190_TMO;
    }

    // ATTENZIONE IL VALORE RAD E' LA SOMMA DI QUATTRO VALORI !!!
    // Il valore di riferimento progettuale fa corrispondere 2.2V all'ingresso del frequenzimetro
    // con 7cm di plexiglass senza cassetta (taratura detector)
    // In quelle condizioni il RAD deve essere convertito a 155
    // 2.2V -> 225*4 = 900. KRAD = 900/155 = 5.8
    int rawrad1 =  _DEVREG(RG244_A_RAD1,PCB244_A_CONTEST);
    int rawrad5 =  _DEVREG(RG244_A_RAD5,PCB244_A_CONTEST);
    int rawrad25 = _DEVREG(RG244_A_RAD25,PCB244_A_CONTEST);

    if(rawrad5 > 4080){
        rawrad5=0;
        rawrad25=0;
    }else if(rawrad25 > 4080) rawrad25=0;

/*    int rad1 =  (int) ((float) rawrad1 / 5.8);
    int rad5 =  (int) ((float) rawrad5 / (0.5 * 5.8 * 4.9));
    int rad25 = (int) (100 * (float) rawrad25 / (0.5 * 5.8 *4.9 * 4.9));
*/

    int rad1 =  (int) ((float) rawrad1 / 4);
    int rad5 =  (int) ((float) rawrad5 / (0.5 * 4 * 4.9));
    int rad25 = (int) (100 * (float) rawrad25 / (0.5 * 4 *4.9 * 4.9));

    // Calcolo PLOG
    int rad = rad1;
    int plog = getPlog(rad);


    // Lettura esito raggi
    if(pcb190GetPostRxRegisters()==FALSE){
        printf("ERRORE DURANTE LETTURA REGISTRI FINE RAGGI!!!!!!! \n");
        return _SEQ_READ_REGISTER;
    }

    // Analisi della condizione di FAULT
    if(_TEST_BIT(PCB190_FAULT)) return(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));

    // Acquisizione PLOG e RAD ed invio dati di fine esposizione al MASTER
    Param->dmAs_released = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST)*10/50;

    unsigned char  data[11];
    data[0]=RXOK;
    data[1]=(unsigned char) ((Param->dmAs_released)&0xFF);
    data[2]=(unsigned char) ((Param->dmAs_released>>8)&0xFF);
    data[3]=(unsigned char) ((plog)&0xFF);
    data[4]=(unsigned char) ((plog>>8)&0xFF);
    data[5]=(unsigned char) ((rad)&0xFF);
    data[6]=(unsigned char) ((rad>>8)&0xFF);
    data[7]=(unsigned char) ((rad5)&0xFF);
    data[8]=(unsigned char) ((rad5>>8)&0xFF);
    data[9]=(unsigned char) ((rad25)&0xFF);
    data[10]=(unsigned char) ((rad25>>8)&0xFF);


    mccGuiNotify(1,Param->mcc_code,data,sizeof(data));

    printf("mAs=%f\n", ((float)Param->dmAs_released)/10);
    printf("PLOG=%d\n", plog);
    printf("RAD=%d\n", rad,rad5);
    if(rawrad5) printf("RADx5=%d\n", rad5);
    else printf("RADx5=OVF\n");
    if(rawrad25) printf("RADx25=%f\n", (float) rad25 / 100);
    else printf("RADx25=OVF\n");

    printf("FINE SEQUENZA OK\n");
    return 0; // RISULTATO POSITIVO

} // AnalogPreCalibration



// Sequenza di calibrazione profili AEC
int AnalogProfileCalibration(void){
    unsigned char  data[20];
    int error=0;

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();

    // Attiva Starter precocemente
    if(!generalConfiguration.demoMode){
      if(Param->esposizione.HV & 0x4000)
      {
        if(pcb190StarterH()==FALSE) printf("WARNING: COMANDO STARTER HIGH FALLITO\n");
        else printf("STARTER ATTIVATO AD ALTA VELOCITA'\n");
      }else
      {
        if(pcb190StarterL()==FALSE) printf("WARNING: COMANDO STARTER LOW FALLITO\n");
        else printf("STARTER ATTIVATO A BASSA VELOCITA'\n");
      }
    }

    // Caricamento parametri di esposizione
    if(pcb190UploadAnalogPreExpose(Param)==FALSE) return _SEQ_UPLOAD190_PARAM;

    // Verifica su XRAY_REQ(Pulsante raggi premuto)
    if(SystemInputs.CPU_XRAY_REQ==0)  return ERROR_PUSHRX_NO_PREP;

    // Impostazione Segnale XRAY_ENA su Bus Hardware
    error = setXrayEna();
    if(error) return error;

    // Legge il Busy per attendere che sia tutto pronto
    if(waitPcb190Ready(50)==FALSE) return _SEQ_PCB190_BUSY;

    // Parte l'esposimetro
    if(pcb244_A_StartRxAec()==false) return _SEQ_PCB190_BUSY;

    // Parte la PCB190 in modalità AEC
    int rc = pcb190StartRxAecStd();
    if(rc==SER422_BUSY) return _SEQ_PCB190_BUSY;
    if(rc==SER422_ILLEGAL_FUNCTION) return ERROR_PUSHRX_NO_PREP;

    // Un minimo di attesa per consentire ai vari segnali di sincronizzarsi
    _time_delay(1000);

    printf("ATTESA DATI ESPOSIMETRO..\n");
    int attempt=20;
    while(--attempt){
        if(PCB244_A_GetPreRad(50)==true) break;
        if(SystemInputs.CPU_XRAY_REQ==0)  return ERROR_PUSHRX_NO_PREP;
    }
    if(!attempt) return  _SEQ_WAIT_AEC_DATA;

    int rad1 = _DEVREG(RG244_A_RAD1,PCB244_A_CONTEST) / 4;
    int rad5 = _DEVREG(RG244_A_RAD5,PCB244_A_CONTEST) / 4;
    int rad25 = _DEVREG(RG244_A_RAD25,PCB244_A_CONTEST) ;
    int rad = rad1;
    int plog = getPlog(rad1);

    //__________________________________________________________   >>>>>>>>     FINE SEQEUNZA AEC
    printf("DATI ESPOSIMETRO: PLOG=%d, RAD=%d\n",plog,rad);

    // Richiede i dati per l'esposizione
    _EVCLR(_EV2_WAIT_AEC);
    data[0]=(unsigned char) ((plog)&0xFF);
    data[1]=(unsigned char) ((plog>>8)&0xFF);
    data[2]=(unsigned char) ((rad)&0xFF);
    data[3]=(unsigned char) ((rad>>8)&0xFF);
    data[4]=(unsigned char) ((rad5)&0xFF);
    data[5]=(unsigned char) ((rad5>>8)&0xFF);
    data[6]=(unsigned char) ((rad25)&0xFF);
    data[7]=(unsigned char) ((rad25>>8)&0xFF);
    mccGuiNotify(1,MCC_XRAY_ANALOG_REQ_AEC_PULSE,data,8);

    // Attesa dati da interfaccia
    printf("ATTESA DATI AEC..\n");
    attempt=40;
    while(--attempt){

        // Rilascio pulsante raggi
       if(SystemInputs.CPU_XRAY_REQ==0) return ERROR_PUSHRX_NO_PREP;

        // PCB190 in errore!
        if(SystemInputs.CPU_XRAY_COMPLETED){
            pcb190GetPostRxRegisters();
            printf("ERRORE SEQUENZA RAGGI DURANTE ATTESA AEC\n");
            return _DEVREGL(RG190_FAULTS,PCB190_CONTEST);
        }

        // Dati AEC arrivati
        if(_IS_EVENT(_EV2_WAIT_AEC)) break;
        _time_delay(100);
    }
    if(!attempt) return _SEQ_AEC_NOT_AVAILABLE;

    // Caricamento dati AEC su PCB190
    if(!pcb190UploadExpose(Param, TRUE)) return _SEQ_UPLOAD190_PARAM;

    // Caricamento impulsi esposimetro
    if(!pcb244_A_uploadAECPulses(Param->pulses)) return _SEQ_UPLOAD190_PARAM;


    printf("ATTESA COMPLETAMENTO..\n");

    // Attesa XRAY COMPLETED da Bus Hardware
    if(SystemInputs.CPU_XRAY_COMPLETED==0)
    {
      _EVCLR(_EV2_XRAY_COMPLETED);
      if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) return _SEQ_PCB190_TMO;
    }

    // Per sicurezza attiva il bit di stop sull'esposimetro
    PCB244_A_SetRxStop();

    // Lettura esito raggi
    if(pcb190GetPostRxRegisters()==FALSE){
        printf("ERRORE LETTURA PCB190 FINE RAGGI\n");
        return _SEQ_READ_REGISTER;
    }

    if(pcb244_A_GetPostRxRegisters()==FALSE){
        printf("ERRORE LETTURA PCB244A FINE RAGGI\n");
        return _SEQ_READ_REGISTER;
    }

    // Analisi della condizione di FAULT
    if(_TEST_BIT(PCB190_FAULT)) return(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));


    printf("SEQUENZA TEMINATA CON SUCCESSO\n");
    // Messaggio fine sequenza raggi ________________________________________
    Param->dmAs_released = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST)*10/50;
    Param->pulses_released = _DEVREG(RG244_A_PULSES_EXIT,PCB244_A_CONTEST);

    data[0]=RXOK;
    data[1]=(unsigned char) ((Param->dmAs_released)&0xFF);
    data[2]=(unsigned char) ((Param->dmAs_released>>8)&0xFF);    
    data[3]=(unsigned char) ((Param->pulses_released)&0xFF);
    data[4]=(unsigned char) ((Param->pulses_released>>8)&0xFF);
    mccGuiNotify(1,Param->mcc_code,data,5);

    printf("PLOG=%d, RAD=%d, mAs=%f\n", plog,rad,(float) Param->dmAs_released/10);

    printf("FINE SEQUENZA OK\n");
    return 0; // RISULTATO POSITIVO

} // AnalogProfileCalibration



// Sequenza di esposizione manuale
int AnalogManualModeExposure(void){
    int error=0;

    // Reset Eventuale Fault della PCB190
    pcb190ResetFault();
    Param->dmAs_released = 0;
    Param->pulses_released = 0;

    // Attiva Starter precocemente
    if(Param->esposizione.HV & 0x4000)
    {
      if(pcb190StarterH()==FALSE) printf("WARNING: COMANDO STARTER HIGH FALLITO\n");
      else printf("STARTER ATTIVATO AD ALTA VELOCITA'\n");
    }else
    {
      if(pcb190StarterL()==FALSE) printf("WARNING: COMANDO STARTER LOW FALLITO\n");
      else printf("STARTER ATTIVATO A BASSA VELOCITA'\n");
    }

    // Caricamento parametri di esposizione
    if(pcb190UploadAnalogManualExpose(Param)==FALSE) return _SEQ_UPLOAD190_PARAM;

    // Caricamento impulsi esposimetro
    if(pcb244_A_uploadManualPulses(0xFFFF)==FALSE) return  _SEQ_UPLOAD_PCB244_A_PARAM ;

    // Verifica su XRAY_REQ(Pulsante raggi premuto)
    if(SystemInputs.CPU_XRAY_REQ==0)  return ERROR_PUSHRX_NO_PREP;

    // Impostazione Segnale XRAY_ENA su Bus Hardware
    error = setXrayEna();
    if(error) return error;

    // Legge il Busy per attendere che sia tutto pronto
    if(waitPcb190Ready(50)==FALSE) return _SEQ_PCB190_BUSY;

    // Parte l'esposimetro in modalità impulso singolo manuale
    // Bisogna prima caricare il valore degli impulsi da contare
    if(pcb244_A_StartManual()==false) return _SEQ_PCB244_A_BUSY;

    // Parte la PCB190 in modalità manuale
    int rc = pcb190StartRxStd();
    if(rc==SER422_BUSY) return _SEQ_PCB190_BUSY;
    if(rc==SER422_ILLEGAL_FUNCTION) return ERROR_PUSHRX_NO_PREP;

    // Un minimo di attesa per consentire ai vari segnali di sincronizzarsi
    _time_delay(1000);

    printf("Attesa Completamento \n");

    // Attesa XRAY COMPLETED da Bus Hardware
    if(SystemInputs.CPU_XRAY_COMPLETED==0)
    {
      _EVCLR(_EV2_XRAY_COMPLETED);
      if(_EVWAIT_TALL(_EV2_XRAY_COMPLETED,_WAIT_XRAY_COMPLETED)==FALSE) return _SEQ_PCB190_TMO;
    }
    printf("Completato\n");

    printf("Lettura campionamenti \n");

    // Attesa ripresa comunicazione seriale post esecuzione raggi
    while(!PCB244_A_GetRad1(1)) _time_delay(100);

    // Lettura campionamenti
    PCB244_A_GetRad1(10);
    PCB244_A_GetRad5(10);
    PCB244_A_GetRad25(10);



    // ATTENZIONE IL VALORE RAD E' LA SOMMA DI QUATTRO VALORI !!!
    // Il valore di riferimento progettuale fa corrispondere 2.2V all'ingresso del frequenzimetro
    // con 7cm di plexiglass senza cassetta (taratura detector)
    // In quelle condizioni il RAD deve essere convertito a 155
    // 2.2V -> 225*4 = 900. KRAD = 900/155 = 5.8
    int rawrad1 =  _DEVREG(RG244_A_RAD1,PCB244_A_CONTEST);
    int rawrad5 =  _DEVREG(RG244_A_RAD5,PCB244_A_CONTEST);
    int rawrad25 = _DEVREG(RG244_A_RAD25,PCB244_A_CONTEST);

    if(rawrad5 > 4080){
        rawrad5=0;
        rawrad25=0;
    }else if(rawrad25 > 4080) rawrad25=0;


    int rad1 =  (int) ((float) rawrad1 / 4);
    int rad5 =  (int) ((float) rawrad5 / (0.5 * 4 * 4.9));
    int rad25 = (int) (100 * (float) rawrad25 / (0.5 * 4 *4.9 * 4.9));

    // Calcolo PLOG
    int rad = rad1;
    int plog = getPlog(rad);


    // Lettura esito raggi
    if(pcb190GetPostRxRegisters()==FALSE){
        printf("ERRORE DURANTE LETTURA REGISTRI FINE RAGGI PCB190!!!!!!! \n");
        return _SEQ_READ_REGISTER;
    }
    Param->dmAs_released = _DEVREG(RG190_MAS_EXIT,PCB190_CONTEST)*10/50;

    if(pcb244_A_GetPostRxRegisters()==FALSE){
        printf("ERRORE DURANTE LETTURA REGISTRI FINE RAGGI ESPOSIMETRO!!!!!!! \n");
        return _SEQ_READ_REGISTER;
    }
    Param->pulses_released = _DEVREG(RG244_A_PULSES_EXIT,PCB244_A_CONTEST);


    // Analisi della condizione di FAULT
    if(_TEST_BIT(PCB190_FAULT)) return(_DEVREGL(RG190_FAULTS,PCB190_CONTEST));

    float KV,IMED;
    int TIME;
    getRxSamplesData(&KV,&IMED,&TIME);


    unsigned char  data[19];
    data[0]=RXOK;
    data[1]=(unsigned char) ((Param->dmAs_released)&0xFF);
    data[2]=(unsigned char) ((Param->dmAs_released>>8)&0xFF);
    data[3]=(unsigned char) ((Param->pulses_released)&0xFF);
    data[4]=(unsigned char) ((Param->pulses_released>>8)&0xFF);
    data[5]=(unsigned char) ((plog)&0xFF);
    data[6]=(unsigned char) ((plog>>8)&0xFF);
    data[7]=(unsigned char) ((rad)&0xFF);
    data[8]=(unsigned char) ((rad>>8)&0xFF);
    data[9]=(unsigned char) ((rad5)&0xFF);
    data[10]=(unsigned char) ((rad5>>8)&0xFF);
    data[11]=(unsigned char) ((rad25)&0xFF);
    data[12]=(unsigned char) ((rad25>>8)&0xFF);

    int val;
    val = (int) (KV * (float) 10);
    data[13]=(unsigned char) ((val)&0xFF);
    data[14]=(unsigned char) (((val)>>8)&0xFF);
    val = (int) (IMED * (float) 10);
    data[15]=(unsigned char) ((val)&0xFF);
    data[16]=(unsigned char) (((val)>>8)&0xFF);
    data[17]=(unsigned char) ((int)(TIME)&0xFF);
    data[18]=(unsigned char) (((int)(TIME)>>8)&0xFF);

    mccGuiNotify(1,Param->mcc_code,data,sizeof(data));

    printf("mAs=%f\n", ((float)Param->dmAs_released)/10);
    printf("Pulses=%f\n", ((float)Param->pulses_released));
    printf("PLOG=%d\n", plog);
    printf("RAD=%d\n", rad,rad5);
    if(rawrad5) printf("RADx5=%d\n", rad5);
    else printf("RADx5=OVF\n");
    if(rawrad25) printf("RADx25=%f\n", (float) rad25 / 100);
    else printf("RADx25=OVF\n");

    printf("FINE SEQUENZA OK\n");
    return 0; // RISULTATO POSITIVO

}


//____________________________________________________________________________________________________________________________
// FUNZIONE DI GESTIONE PARTICOLARE DELL'ERRORE
void RxAnalogSeqError(int codice){
    unsigned char data[10];
    // Stringa di debug
    printf("ERRORE SEQUENZA [%d] MCC[%d]: ERRORE=%d, mAs:%f, PULSES:%d\n",Param->analog_sequence, Param->mcc_code, codice,  (float)Param->dmAs_released / 10,Param->pulses_released);
    data[0]=codice;
    data[1]=(unsigned char) ((Param->dmAs_released)&0xFF);
    data[2]=(unsigned char) (((Param->dmAs_released)>>8)&0xFF);
    data[3]=(unsigned char) ((Param->pulses_released)&0xFF);
    data[4]=(unsigned char) (((Param->pulses_released)>>8)&0xFF);
    mccGuiNotify(1,Param->mcc_code,data,5);

    // Per sicurezza attiva il bit di stop sull'esposimetro
    PCB244_A_SetRxStop();
    return;
}

// Operazioni comuni di terminazione della seqeunza
void fineSequenza(void){

    // Sequenza terminata con successo
    if(Ser422DriverSetReadyAll(5000) == FALSE) RxAnalogSeqError(_SEQ_DRIVER_READY);
    else  printf("SBLOCCO DRIVER OK\n");

    // Reset degli IO
    _mutex_lock(&output_mutex);
    SystemOutputs.CPU_XRAY_ENA=0;   // Disattivazione segnale XRAY ENA
    SystemOutputs.CPU_DEMO_ACTIVATION = 0;   // Disattivazione Buzzer
    _EVSET(_EV0_OUTPUT_CAMBIATI);
    _mutex_unlock(&output_mutex);

    // Sblocca il compressore se deve
    if(Param->compressor_unlock)
        pcb215SetSblocco();

    _EVSET(_SEQEV_RX_ANALOG_TERMINATED);

    // Se richiesto viene spento lo starter
    if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
        if(generalConfiguration.pcb190Cfg.starter_off_with_brake) pcb190StopStarter();
        else pcb190OffStarter();
    }


}


// Campionamenti interessanti ai fini della diagnostica
void getRxSamplesData(float* kv, float* imed, int* time)
{
    unsigned char  is[PCB190_NSAMPLES+1];
    unsigned char  vs[PCB190_NSAMPLES+1];
    int i;
    float scarto_v = 0;
    float imean,vmean;
    int  samples, naec;
    unsigned char addr, banco;
    unsigned short tmed_pls;
    unsigned char ifil_rxend;

    // Legge il numero di campioni AEC
    Ser422ReadRegister(_REGID(PR190_N_SAMPLE_AEC),10,&PCB190_CONTEST);
    naec = (float) _DEVREGL(PR190_N_SAMPLE_AEC,PCB190_CONTEST);
    if(naec>1) naec = 1;

    // Legge il numero dei campioni totali
    Ser422ReadRegister(_REGID(PR190_N_SAMPLE_I),10,&PCB190_CONTEST);
    samples = (int) _DEVREGL(PR190_N_SAMPLE_I,PCB190_CONTEST);
    if(samples > PCB190_NSAMPLES) samples = (int) PCB190_NSAMPLES;

    // Legge i campioni della corrente
    addr = _DEVADDR(PR190_SAMPLES_I);
    banco = _DEVBANCO(PR190_SAMPLES_I);
    for(i=0;i<samples;i++,addr++){
        if(i>=sizeof(is)) break;
        Ser422ReadAddrRegister(addr, banco,10,&is[i], &PCB190_CONTEST);
    }

    // Legge i campioni della tensione
    addr = _DEVADDR(PR190_SAMPLES_V);
    banco = _DEVBANCO(PR190_SAMPLES_V);
    for(i=0;i<samples;i++,addr++){
        if(i>=sizeof(vs)) break;
        Ser422ReadAddrRegister(addr, banco,10,&vs[i], &PCB190_CONTEST);
    }

    // Calcola medie Impulso se ci sono campioni
    imean=vmean=0;
    float kvmean=0;
    if((samples-naec)>0){
        for(i=naec;i<samples;i++){
            printf("(PLS-%d): I[%f(mA), %d(RAW)]  V[%f(kV), %d(RAW)] \n",(int) (i-naec),((((float) is[i])*200.0)/255.0), is[i],pcb190ConvertKvRead(vs[i]),vs[i]);
            imean+=(float) is[i];
            vmean+=(float) vs[i];
        }
        imean=imean/(float)(samples-naec);
        vmean=vmean/(float)(samples-naec);
        scarto_v = 0;
        kvmean = pcb190ConvertKvRead(nearest(vmean));

        // Calcola scarto medio Impulso se ci sono campioni
        for(i=naec;i<samples;i++){
            scarto_v +=  ( ((kvmean - pcb190ConvertKvRead(vs[i])))* ((kvmean - pcb190ConvertKvRead(vs[i]))));
        }
        scarto_v = (10.0 * sqrt(scarto_v/(float)(samples-naec)));
    }

    // Legge il valore medio del tempo di impulso
    tmed_pls = 0;
    if(Ser422ReadRegister(_REGID(REG190_RX_TIME_PLS),10,&PCB190_CONTEST) == _SER422_NO_ERROR)
      tmed_pls = (unsigned short)((float) _DEVREG(REG190_RX_TIME_PLS,PCB190_CONTEST) * 1.115);


    // Lettura I FILAMENTO DURANTE RAGGI
    Ser422ReadRegister(_REGID(RG_SAMPLED_IFIL),10,&PCB190_CONTEST);
    ifil_rxend = _DEVREGL(RG_SAMPLED_IFIL,PCB190_CONTEST);

    // Stampa dei valori
    printf("HV-BUS(V):%f\n",(float) _DEVREGL(RG190_HV_RXEND,PCB190_CONTEST) * ((float) generalConfiguration.pcb190Cfg.HV_CONVERTION / 1000.0));
    printf("I_FIL (mA):%f\n",ifil_rxend * 47.98);

    if(samples-naec){
      printf("PLS-I(mA)=%f\n",(imean*200.)/255.);
      printf("PLS-V(kV)=%f, dKv:%f\n",kvmean,scarto_v/10.);
      printf("Tmed_Pulse(ms)=%d\n",tmed_pls );
   }

   *kv = kvmean;
   *imed = imean * 200 / 255;
   *time =  tmed_pls;

}
