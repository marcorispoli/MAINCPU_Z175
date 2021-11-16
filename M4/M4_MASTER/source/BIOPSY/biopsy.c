#define _BIOPSY_C
#include "dbt_m4.h" 

#define CONTEST BIOPSY_CONTEST
#define STATUS  (*((_BIOPSY_Stat_Str*)(&BIOPSY_CONTEST.Stat)))

#define _DEF_BIOPSY_HIGH        193 // Posizione needle in zero rispetto al piano deel detector
#define _DEF_MARGINE_HIGH       10 // millimetri di margine sulla posizione massima

// Definizione delays relativi agli stati
#define _BYM_DISCONNECTED_STAT_DELAY 500
#define _BYM_CONNECTED_STAT_DELAY    100
#define _BYM_ACTIVATED_STAT_DELAY    500

// Definizione macchina a stati
#define _BYM_DRIVER_STAT_DISCONNECTED 0
#define _BYM_DRIVER_STAT_CONNECTED    1
#define _BYM_DRIVER_STAT_ACTIVATED    2

// Definizione comandi di attivazione cursore
#define _BYM_NO_COMMAND             0
#define _BYM_MOVE_TO_XYZ            1
#define _BYM_MOVE_TO_HOME           2
#define _BYM_MOVE_TO_STEP_DECZ      3
#define _BYM_MOVE_TO_STEP_INCZ      4
#define _BYM_MOVE_TO_STEP_DECX      5
#define _BYM_MOVE_TO_STEP_INCX      6
#define _BYM_MOVE_TO_STEP_DECY      7
#define _BYM_MOVE_TO_STEP_INCY      8

// _________________________________________________________________________________
//  Prototipi funzioni locali
static void BIOPSY_manageDriverDisconnectedStatus(void);
static void BIOPSY_manageDriverConnectedStatus(void);
static void BIOPSY_manageDriverActivatedStatus(void);
static void BIOPSY_manageActivationLoop(void);

//___________________________________________________________________________________

// _________________________________________________________________________________
//  Registri driver
static unsigned char dati[_BP_DATA_LEN];
static unsigned char chg_dati[_BP_DATA_LEN];
static unsigned char driverStatus;
static unsigned char activationCommands;
static int targetX, targetY, targetZ;    // Target di posizionamento XYZ

//___________________________________________________________________________________


void BIOPSY_driver(uint32_t taskRegisters)
{

   driverStatus = _BYM_DRIVER_STAT_DISCONNECTED;
   activationCommands = _BYM_NO_COMMAND;


    printf("ATTIVAZIONE DRIVER BIOPSY: \n");


    generalConfiguration.biopsyCfg.connected = FALSE;
    generalConfiguration.biopsyCfg.adapterId = 0;    
    generalConfiguration.biopsyCfg.armEna = FALSE;

    generalConfiguration.biopsyCfg.stepVal = 10; // Default = 10


    // Attende la configurazione del sistema prima di procedere
    while(generalConfiguration.deviceConfigOk==FALSE) _time_delay(1000);
    
    // Inizializzazione dei campi di notifica alla GUI
    dati[_BP_MOTION]=_BP_NO_MOTION;
    dati[_BP_MOTION_END]=0;
    dati[_BP_CHKH]=0;
    dati[_BP_CHKL]=0;
    dati[_BP_REVIS]=0;
    dati[_BP_MODEL]=_BP_MODEL_UNDEFINED;
    dati[_BP_XL] = 0;
    dati[_BP_XH] = 0;
    dati[_BP_YL] = 0;
    dati[_BP_YH] = 0;
    dati[_BP_ZL] = 0;
    dati[_BP_ZH] = 0;
    dati[_BP_SHL] = 0;
    dati[_BP_SHH] = 0;
    dati[_BP_PADDLE_MARGINE] = 0;
    dati[_BP_PUSH_SBLOCCO] = _BP_PUSH_SBLOCCO_DISATTIVO;
    dati[_BP_ADAPTER_ID] = 0;
    dati[_BP_CONNESSIONE] = _BP_CONNESSIONE_DISCONNECTED;
    dati[_BP_MAX_Z_PADDLE] = 0;


    while(!mccBiopsyNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati))) _time_delay(200);
    for(int i =0; i< _BP_DATA_LEN; i++){
        chg_dati[i] = dati[i];
    }

#ifdef __BIOPSY_SIMULATOR
    // Partenza processo gestione biopsia (opzionale)
    _task_create(0,_IDTASK(BIOPSYM),(uint32_t) NULL);

#endif


    // Gestione dello stato del sistema secondo la macchina a stati
    while(1){
        if(STATUS.freeze)
        {
           // Entra in Freeze
           _EVCLR(_EV1_BIOPSY_RUN);
           _EVSET(_EV1_BIOPSY_FREEZED); // Notifica l'avvenuto Blocco
           _EVWAIT_ANY(_MOR2(_EV1_DEVICES_RUN,_EV1_BIOPSY_RUN)); // Attende lo sblocco
           _EVSET(_EV1_BIOPSY_RUN);
           STATUS.freeze = 0;
        }
        STATUS.ready=1;
        _EVSET(_EV1_BIOPSY_RUN);

        if(driverStatus == _BYM_DRIVER_STAT_DISCONNECTED)   BIOPSY_manageDriverDisconnectedStatus();
        else if(driverStatus == _BYM_DRIVER_STAT_CONNECTED) BIOPSY_manageDriverConnectedStatus();
        else if(driverStatus == _BYM_DRIVER_STAT_ACTIVATED) BIOPSY_manageDriverActivatedStatus();
        else _time_delay(1000);

        // Verifica se bisogna notificare
        bool notifica = false;
        for(int i =0; i< _BP_DATA_LEN; i++){
            if(chg_dati[i] != dati[i]){
                chg_dati[i] = dati[i];
                notifica = true;
            }
        }

        // Effettua la notifica dello stato se necessario
        if(notifica) while(!mccBiopsyNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati))) _time_delay(50);

    }
}


void BIOPSY_manageDriverDisconnectedStatus(void){
    _time_delay(_BYM_DISCONNECTED_STAT_DELAY);
    if(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH, false)) return;

    // Se i motori sono attivi attende
    if(generalConfiguration.biopsyCfg.statusL & 0x01) return;

    // Con la torretta disconnessa occorre verificare se è stata resettata
    /*
    if(!generalConfiguration.biopsyCfg.connected){

        // Verifica se la torretta non è ancora stata resettata (dovrebbe esserlo!!)
        if(!(generalConfiguration.biopsyCfg.statusH & 0x80)){
            printf("BYM: EXECUTE RESET\n");
            BiopsyDriverReset();
            return;
        }
    }
    */

    // Lo stato della torretta ora è di connessione, ma occorre completare ancora qualche passaggio
    generalConfiguration.biopsyCfg.connected = true;

    // Reset status
    if(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH, true)) return;

    // Acquisizione checksum
    if(!BiopsyDriverGetChecksum(&generalConfiguration.biopsyCfg.checksum_l, &generalConfiguration.biopsyCfg.checksum_h)) return;

    // Acquisizione revisione
    if(!BiopsyDriverGetRevision(&generalConfiguration.biopsyCfg.revisione)) return;

    // Cambio stato in stato di connessione
    driverStatus = _BYM_DRIVER_STAT_CONNECTED;
    activationCommands = _BYM_NO_COMMAND;

    // Notifica PC
    dati[_BP_CONNESSIONE] = _BP_CONNESSIONE_CONNECTED;
    dati[_BP_CHKH]=generalConfiguration.biopsyCfg.checksum_h;
    dati[_BP_CHKL]=generalConfiguration.biopsyCfg.checksum_l;
    dati[_BP_REVIS]=generalConfiguration.biopsyCfg.revisione;

    printf("BYM DRIVER: STATUS CHANGE TO CONNECTED STATUS\n");
    printf("BYM REVISION: %d CHKSUM %x%x\n",generalConfiguration.biopsyCfg.revisione,generalConfiguration.biopsyCfg.checksum_h,generalConfiguration.biopsyCfg.checksum_l );
    return;
}

void BIOPSY_manageDriverConnectedStatus(void){
    static unsigned char slot = 0;

    static int timer_stat = 2000 /_BYM_CONNECTED_STAT_DELAY;
    _time_delay(_BYM_CONNECTED_STAT_DELAY);
    dati[_BP_MOTION]=_BP_NO_MOTION;

    // Evento di Timeout!!
    if(--timer_stat == 0){

        generalConfiguration.biopsyCfg.connected = false;
        driverStatus = _BYM_DRIVER_STAT_DISCONNECTED;
        dati[_BP_CONNESSIONE] = _BP_CONNESSIONE_DISCONNECTED;
        timer_stat = 2000 /_BYM_CONNECTED_STAT_DELAY;
        return;
    }

    // Chiede lo status
    if(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH, true)) return;

    // Verifica il pulsante di sblocco
    if(generalConfiguration.biopsyCfg.statusH & 0x40){
        generalConfiguration.biopsyCfg.armEna = true;
        dati[_BP_PUSH_SBLOCCO] = _BP_PUSH_SBLOCCO_ATTIVO;
    }else{
        generalConfiguration.biopsyCfg.armEna = false;
        dati[_BP_PUSH_SBLOCCO] = _BP_PUSH_SBLOCCO_DISATTIVO;
    }

    // Determina la posizione di dislocazione dell'asse X
    dati[_BP_ASSEX_POSITION] = generalConfiguration.biopsyCfg.statusH &0x3;


    // Calcolo del margine sul compressore in mm: se il pad non è riconosciuto viene messo al massimo
    if(generalConfiguration.comprCfg.padSelezionato >= PAD_ENUM_SIZE){
        dati[_BP_PADDLE_MARGINE] = generalConfiguration.biopsyCfg.conf.Z_homePosition;
    }else{        
        int pos_paddle = _DEVREG(RG215_DOSE,PCB215_CONTEST) - generalConfiguration.biopsyCfg.conf.offsetPad;
        int margine = generalConfiguration.biopsyCfg.conf.Z_basePosizionatore - pos_paddle - generalConfiguration.biopsyCfg.Z/10;
        if(margine < 0) margine = 0;
        if(margine > generalConfiguration.biopsyCfg.conf.Z_homePosition) margine = generalConfiguration.biopsyCfg.conf.Z_homePosition;
        dati[_BP_PADDLE_MARGINE] = (unsigned char) margine;
    }

    if(slot==0){
        // Gestione needle
        if(!BiopsyDriverGetNeedle(&generalConfiguration.biopsyCfg.adapterId)) return;

        // Conversione valori NEEDLE -> IDENTIFICATORE
        if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_NEEDLE_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_NEEDLE_LEVEL_L))
            dati[_BP_ADAPTER_ID] = _BP_ADAPTER_NEEDLE;
        else if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_A_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_A_LEVEL_H))
            dati[_BP_ADAPTER_ID] = _BP_ADAPTER_A;
        else if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_SHORT_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_SHORT_LEVEL_H))
            dati[_BP_ADAPTER_ID] = _BP_ADAPTER_SHORT;
        else if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_OPEN_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_OPEN_LEVEL_H))
            dati[_BP_ADAPTER_ID] = _BP_ADAPTER_OPEN;
        else
            dati[_BP_ADAPTER_ID] = _BP_ADAPTER_B;


    }else if(slot==2){
        // Acquisisce posizione attuale
        if(!BiopsyDriverGetX(&generalConfiguration.biopsyCfg.X)) return;
        if(!BiopsyDriverGetY(&generalConfiguration.biopsyCfg.Y)) return;
        if(!BiopsyDriverGetZ(&generalConfiguration.biopsyCfg.Z)) return;
        if(!BiopsyDriverGetZ(&generalConfiguration.biopsyCfg.SH)) return;

        dati[_BP_XL] = (unsigned char) (generalConfiguration.biopsyCfg.X & 0x00FF);
        dati[_BP_XH] = (unsigned char) (generalConfiguration.biopsyCfg.X >> 8);
        dati[_BP_YL] = (unsigned char) (generalConfiguration.biopsyCfg.Y & 0x00FF);
        dati[_BP_YH] = (unsigned char) (generalConfiguration.biopsyCfg.Y >> 8);
        dati[_BP_ZL] = (unsigned char) (generalConfiguration.biopsyCfg.Z & 0x00FF);
        dati[_BP_ZH] = (unsigned char) (generalConfiguration.biopsyCfg.Z >> 8);
        dati[_BP_SHL] = (unsigned char) (generalConfiguration.biopsyCfg.SH & 0x00FF);
        dati[_BP_SHH] = (unsigned char) (generalConfiguration.biopsyCfg.SH >> 8);

    }else if(slot==3){

        // Scrittura zlimit sul target sulla base della distanza dal paddle
        // int z_limit = generalConfiguration.biopsyCfg.Z + (int) dati[_BP_PADDLE_MARGINE] * 10; // Margine di impatto con il compressore

        // Scrittura dello stepval
        if(!BiopsyDriverSetStepVal(generalConfiguration.biopsyCfg.stepVal, 0 ));
    }

    // Tutto OK
    slot++;
    if(slot>3) slot = 0;
    timer_stat = 2000 /_BYM_CONNECTED_STAT_DELAY;

    // Gestione comandi posizionatore
    if(activationCommands != _BYM_NO_COMMAND){
        driverStatus = _BYM_DRIVER_STAT_ACTIVATED;
        dati[_BP_MOTION]=_BP_MOTION_ON;
        slot = 0;
        printf("BYM DRIVER: GESTIONE ATTIVAZIONE");
    }

    return;
}

void BIOPSY_manageDriverActivatedStatus(void){
    int tentativi;

    switch(activationCommands){
    case _BYM_MOVE_TO_XYZ:

        // caricamento target
        tentativi = 10;
        while(!BiopsyDriverSetTGX(targetX)){
            tentativi--;
            if(!tentativi){
                dati[_BP_MOTION] =_BP_MOTION_TERMINATED;
                dati[_BP_MOTION_END] =_BP_TIMEOUT_COMANDO;
                activationCommands =_BYM_NO_COMMAND;
                driverStatus = _BYM_DRIVER_STAT_CONNECTED;
                printf("BYM DRIVER: ERRORE CARICAMENTO TARGET\n");
                printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
                return;
            }
            _time_delay(50);
        }
        tentativi = 10;
        while(!BiopsyDriverSetTGY(targetY)){
            tentativi--;
            if(!tentativi){
                dati[_BP_MOTION] =_BP_MOTION_TERMINATED;
                dati[_BP_MOTION_END] =_BP_TIMEOUT_COMANDO;
                activationCommands =_BYM_NO_COMMAND;
                driverStatus = _BYM_DRIVER_STAT_CONNECTED;
                printf("BYM DRIVER: ERRORE CARICAMENTO TARGET\n");
                printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
                return;
            }
            _time_delay(50);
        }
        tentativi = 10;
        while(!BiopsyDriverSetTGZ(targetZ)){
            tentativi--;
            if(!tentativi){
                dati[_BP_MOTION] =_BP_MOTION_TERMINATED;
                dati[_BP_MOTION_END] =_BP_TIMEOUT_COMANDO;
                activationCommands =_BYM_NO_COMMAND;
                driverStatus = _BYM_DRIVER_STAT_CONNECTED;
                printf("BYM DRIVER: ERRORE CARICAMENTO TARGET\n");
                printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
                return;
            }
            _time_delay(50);
        }

        // Attivazione comando
        BiopsyDriverMoveXYZ(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_HOME:
        // Attivazione comando
        BiopsyDriverMoveHOME(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;


    case _BYM_MOVE_TO_STEP_DECZ:
        // Attivazione comando
        BiopsyDriverMoveDecZ(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_INCZ:
        // Attivazione comando
        BiopsyDriverMoveIncZ(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_DECX:
        // Attivazione comando
        BiopsyDriverMoveDecX(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_INCX:
        // Attivazione comando
        BiopsyDriverMoveIncX(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_DECY:
        // Attivazione comando
        BiopsyDriverMoveDecY(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_INCY:
        // Attivazione comando
        BiopsyDriverMoveIncY(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH);
        BIOPSY_manageActivationLoop();
        break;

    default:
        activationCommands =_BYM_NO_COMMAND;
        driverStatus = _BYM_DRIVER_STAT_CONNECTED;
        printf("BYM DRIVER: NESSUN COMANDO DI ATTIVAZIONE\n");
        printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
        return;
    }

}

void BIOPSY_manageActivationLoop(void){
    // Attesa completamento comando: max 20 secondi
    int tentativi = 40;
    while(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH, false)){
        _time_delay(500);
        tentativi--;
        if(tentativi == 0){
            dati[_BP_MOTION] =_BP_MOTION_TERMINATED;
            dati[_BP_MOTION_END] =_BP_TIMEOUT_COMANDO;
            activationCommands =_BYM_NO_COMMAND;
            driverStatus = _BYM_DRIVER_STAT_CONNECTED;
            printf("BYM DRIVER: TIMEOUT ATIVAZIONE\n");
            printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
            return;
        }
    }

    // Verifica risultato
    dati[_BP_MOTION] =_BP_MOTION_TERMINATED;
    if(generalConfiguration.biopsyCfg.statusL & 0x80) dati[_BP_MOTION_END] =_BP_ERROR_POSITIONINIG;
    else dati[_BP_MOTION_END] =_BP_POSITIONINIG_OK;

    // Azzeramento bit resettabili
    BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.statusL, &generalConfiguration.biopsyCfg.statusH, true);

    // Acquisisce posizione attuale
    BiopsyDriverGetX(&generalConfiguration.biopsyCfg.X);
    BiopsyDriverGetY(&generalConfiguration.biopsyCfg.Y);
    BiopsyDriverGetZ(&generalConfiguration.biopsyCfg.Z);

    dati[_BP_XL] = (unsigned char) (generalConfiguration.biopsyCfg.X & 0x00FF);
    dati[_BP_XH] = (unsigned char) (generalConfiguration.biopsyCfg.X >> 8);
    dati[_BP_YL] = (unsigned char) (generalConfiguration.biopsyCfg.Y & 0x00FF);
    dati[_BP_YH] = (unsigned char) (generalConfiguration.biopsyCfg.Y >> 8);
    dati[_BP_ZL] = (unsigned char) (generalConfiguration.biopsyCfg.Z & 0x00FF);
    dati[_BP_ZH] = (unsigned char) (generalConfiguration.biopsyCfg.Z >> 8);

    activationCommands =_BYM_NO_COMMAND;
    driverStatus = _BYM_DRIVER_STAT_CONNECTED;
    printf("BYM DRIVER: ATIVAZIONE TERMINATA\n");
    printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
    return;
}



//____________________________________________________________________________________________________
// API DI PROTOCOLLO
bool biopsyMoveXYZ(unsigned short X, unsigned short Y, unsigned short Z)
{
  if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso

  targetX = X;
  targetY = Y;
  targetZ = Z;
  activationCommands = _BYM_MOVE_TO_XYZ;
  printf("BYM COMMAND: MOVE XYX, mTGX:%d, TGY:%d, TGZ:%d\n",X,Y,Z);
  return TRUE;
}

bool biopsyMoveHome(void)
{
  if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
  activationCommands = _BYM_MOVE_TO_HOME;
  printf("BYM COMMAND: MOVE HOME\n");
  return TRUE;
}


bool  biopsyStepIncZ(void)
{
    if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
    activationCommands = _BYM_MOVE_TO_STEP_INCZ;
    printf("BYM COMMAND: MOVE STEP INC Z\n");
    return TRUE;
}

bool  biopsyStepDecZ(void)
{  
    if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
    activationCommands = _BYM_MOVE_TO_STEP_DECZ;
    printf("BYM COMMAND: MOVE STEP DEC Z\n");
    return TRUE;

}
bool  biopsyStepIncX(void)
{
    if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
    activationCommands = _BYM_MOVE_TO_STEP_INCX;
    printf("BYM COMMAND: MOVE STEP INC X\n");
    return TRUE;
}

bool  biopsyStepDecX(void)
{
    if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
    activationCommands = _BYM_MOVE_TO_STEP_DECX;
    printf("BYM COMMAND: MOVE STEP DEC X\n");
    return TRUE;

}
bool  biopsyStepIncY(void)
{
    if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
    activationCommands = _BYM_MOVE_TO_STEP_INCY;
    printf("BYM COMMAND: MOVE STEP INC Y\n");
    return TRUE;
}

bool  biopsyStepDecY(void)
{
    if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso
    activationCommands = _BYM_MOVE_TO_STEP_DECY;
    printf("BYM COMMAND: MOVE STEP DEC Y\n");
    return TRUE;

}


/*
  Funzione configuratrice biopsia
  buffer[0]: offsetFibra
  buffer[1]: offsetPad
  buffer[2]: margine risalita compressore
  buffer[3]: margine posizionamento
*/
bool config_biopsy(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len){

  printf("AGGIORNAMENTO CONFIG BIOPSIA:\n");

  printf(" z-Home=%d\n",buffer[0]);
  printf(" z-Base=%d\n",buffer[1]);
  printf(" offsetPad=%d\n",buffer[2]);
  printf(" margineRisalita=%d\n",buffer[3]);
  printf(" marginePosizionamento=%d\n",buffer[4]);

  generalConfiguration.biopsyCfg.conf.Z_homePosition = buffer[0];
  generalConfiguration.biopsyCfg.conf.Z_basePosizionatore = buffer[1];
  generalConfiguration.biopsyCfg.conf.offsetPad = buffer[2];
  generalConfiguration.biopsyCfg.conf.margineRisalita = buffer[3];
  generalConfiguration.biopsyCfg.conf.marginePosizionamento = buffer[4];

  // L'impostazione dei margini di posizionamento richiede un aggiornamento
  // dei parametri del compressore chge si deve adeguare a nuovi vincoli
  pcb215ForceUpdateData();
  return true;
}

/* EOF */
 

