#define _BIOPSY_EXTENDED_C
#include "dbt_m4.h" 

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
static unsigned char dati[_BP_EXT_DATA_LEN];
static unsigned char chg_dati[_BP_EXT_DATA_LEN];
static unsigned char driverStatus;
static unsigned char activationCommands;
static unsigned short targetX,targetY,targetZ;

void biopsyExtendedLoop(void)
{

    driverStatus = _BYM_DRIVER_STAT_DISCONNECTED;
    activationCommands = _BYM_NO_COMMAND;


    printf("ATTIVAZIONE DRIVER BIOPSI ESTESA: \n");
    
    // Inizializzazione dei campi di notifica alla GUI
    dati[_BP_EXT_MOTION]=_BP_EXT_NO_MOTION;
    dati[_BP_EXT_MOTION_END]=0;
    dati[_BP_EXT_CHKH]=0;
    dati[_BP_EXT_CHKL]=0;
    dati[_BP_EXT_REVIS]=0;
    dati[_BP_EXT_XL] = 0;
    dati[_BP_EXT_XH] = 0;
    dati[_BP_EXT_YL] = 0;
    dati[_BP_EXT_YH] = 0;
    dati[_BP_EXT_ZL] = 0;
    dati[_BP_EXT_ZH] = 0;
    dati[_BP_EXT_SHL] = 0;
    dati[_BP_EXT_SHH] = 0;
    dati[_BP_EXT_PUSH_SBLOCCO] = _BP_EXT_PUSH_SBLOCCO_DISATTIVO;
    dati[_BP_EXT_ADAPTER_ID] = 0;
    dati[_BP_EXT_CONNESSIONE] = _BP_EXT_CONNESSIONE_DISCONNECTED;


    // Gestione dello stato del sistema secondo la macchina a stati
    while(1){


        if(driverStatus == _BYM_DRIVER_STAT_DISCONNECTED)   BIOPSY_manageDriverDisconnectedStatus();
        else if(driverStatus == _BYM_DRIVER_STAT_CONNECTED) BIOPSY_manageDriverConnectedStatus();
        else if(driverStatus == _BYM_DRIVER_STAT_ACTIVATED) BIOPSY_manageDriverActivatedStatus();
        else _time_delay(1000);

        // Verifica se bisogna notificare
        bool notifica = false;
        for(int i =0; i< _BP_EXT_DATA_LEN; i++){
            if(chg_dati[i] != dati[i]){
                chg_dati[i] = dati[i];
                notifica = true;
            }
        }

        // Effettua la notifica dello stato se necessario
        if(notifica) while(!mccBiopsyExtendedNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati))) _time_delay(50);

    }
}


void BIOPSY_manageDriverDisconnectedStatus(void){
    _time_delay(_BYM_DISCONNECTED_STAT_DELAY);
    if(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH, false)) return;

    // Se i motori sono attivi attende
    if(generalConfiguration.biopsyCfg.extendedConf.statusL & 0x01) return;

    // Lo stato della torretta ora è di connessione, ma occorre completare ancora qualche passaggio
    generalConfiguration.biopsyCfg.biopsyConnected = true;

    // Reset status
    if(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH, true)) return;

    // Acquisizione checksum
    if(!BiopsyDriverGetChecksum(&generalConfiguration.biopsyCfg.checksum_l, &generalConfiguration.biopsyCfg.checksum_h)) return;

    // Acquisizione revisione
    if(!BiopsyDriverGetRevision(&generalConfiguration.biopsyCfg.revisione)) return;

    // Cambio stato in stato di connessione
    driverStatus = _BYM_DRIVER_STAT_CONNECTED;
    activationCommands = _BYM_NO_COMMAND;

    // Notifica PC
    dati[_BP_EXT_CONNESSIONE] = _BP_EXT_CONNESSIONE_CONNECTED;
    dati[_BP_EXT_CHKH]=generalConfiguration.biopsyCfg.checksum_h;
    dati[_BP_EXT_CHKL]=generalConfiguration.biopsyCfg.checksum_l;
    dati[_BP_EXT_REVIS]=generalConfiguration.biopsyCfg.revisione;

    printf("BYM DRIVER: STATUS CHANGE TO CONNECTED STATUS\n");
    printf("BYM REVISION: %d CHKSUM %x%x\n",generalConfiguration.biopsyCfg.revisione,generalConfiguration.biopsyCfg.checksum_h,generalConfiguration.biopsyCfg.checksum_l );
    return;
}

void BIOPSY_manageDriverConnectedStatus(void){
    static unsigned char slot = 0;

    static int timer_stat = 2000 /_BYM_CONNECTED_STAT_DELAY;
    _time_delay(_BYM_CONNECTED_STAT_DELAY);
    dati[_BP_EXT_MOTION]=_BP_EXT_NO_MOTION;

    // Evento di Timeout!!
    if(--timer_stat == 0){

        generalConfiguration.biopsyCfg.biopsyConnected = false;
        driverStatus = _BYM_DRIVER_STAT_DISCONNECTED;
        dati[_BP_EXT_CONNESSIONE] = _BP_EXT_CONNESSIONE_DISCONNECTED;
        timer_stat = 2000 /_BYM_CONNECTED_STAT_DELAY;
        return;
    }

    // Chiede lo status
    if(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH, true)) return;

    // Verifica il pulsante di sblocco
    if(generalConfiguration.biopsyCfg.extendedConf.statusH & 0x40){
        generalConfiguration.biopsyCfg.biopsyArmEna = true;
        dati[_BP_EXT_PUSH_SBLOCCO] = _BP_EXT_PUSH_SBLOCCO_ATTIVO;
    }else{
        generalConfiguration.biopsyCfg.biopsyArmEna = false;
        dati[_BP_EXT_PUSH_SBLOCCO] = _BP_EXT_PUSH_SBLOCCO_DISATTIVO;
    }

    // Determina la posizione di dislocazione dell'asse X
    dati[_BP_EXT_ASSEX_POSITION] = generalConfiguration.biopsyCfg.extendedConf.statusH &0x3;


    if(slot==0){
        // Gestione needle
        if(!BiopsyDriverGetNeedle(&generalConfiguration.biopsyCfg.adapterId)) return;

        // Conversione valori NEEDLE -> IDENTIFICATORE
        if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_A_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_A_LEVEL_H))
            dati[_BP_EXT_ADAPTER_ID] = _BP_EXT_ADAPTER_A;
        else if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_B_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_B_LEVEL_H))
            dati[_BP_EXT_ADAPTER_ID] = _BP_EXT_ADAPTER_B;
        else if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_C_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_C_LEVEL_H))
            dati[_BP_EXT_ADAPTER_ID] = _BP_EXT_ADAPTER_C;
        else if((generalConfiguration.biopsyCfg.adapterId >= _ADAPTER_SHORT_LEVEL_L) && (generalConfiguration.biopsyCfg.adapterId <= _ADAPTER_SHORT_LEVEL_H))
            dati[_BP_EXT_ADAPTER_ID] = _BP_EXT_ADAPTER_SHORT;
        else  dati[_BP_EXT_ADAPTER_ID] = _BP_EXT_ADAPTER_OPEN;


    }else if(slot==2){
        // Acquisisce posizione attuale
        if(!BiopsyDriverGetX(&generalConfiguration.biopsyCfg.X)) return;
        if(!BiopsyDriverGetY(&generalConfiguration.biopsyCfg.Y)) return;
        if(!BiopsyDriverGetZ(&generalConfiguration.biopsyCfg.Z)) return;
        if(!BiopsyDriverGetSH(&generalConfiguration.biopsyCfg.SH)) return;

        dati[_BP_EXT_XL] = (unsigned char) (generalConfiguration.biopsyCfg.X & 0x00FF);
        dati[_BP_EXT_XH] = (unsigned char) (generalConfiguration.biopsyCfg.X >> 8);
        dati[_BP_EXT_YL] = (unsigned char) (generalConfiguration.biopsyCfg.Y & 0x00FF);
        dati[_BP_EXT_YH] = (unsigned char) (generalConfiguration.biopsyCfg.Y >> 8);
        dati[_BP_EXT_ZL] = (unsigned char) (generalConfiguration.biopsyCfg.Z & 0x00FF);
        dati[_BP_EXT_ZH] = (unsigned char) (generalConfiguration.biopsyCfg.Z >> 8);
        dati[_BP_EXT_SHL] = (unsigned char) (generalConfiguration.biopsyCfg.SH & 0x00FF);
        dati[_BP_EXT_SHH] = (unsigned char) (generalConfiguration.biopsyCfg.SH >> 8);

    }else if(slot==3){

        // Scrittura dello stepval
        if(!BiopsyDriverSetStepVal(generalConfiguration.biopsyCfg.extendedConf.stepVal, 0 ));
    }

    // Tutto OK
    slot++;
    if(slot>3) slot = 0;
    timer_stat = 2000 /_BYM_CONNECTED_STAT_DELAY;

    // Gestione comandi posizionatore
    if(activationCommands != _BYM_NO_COMMAND){
        driverStatus = _BYM_DRIVER_STAT_ACTIVATED;
        dati[_BP_EXT_MOTION]=_BP_EXT_MOTION_ON;
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
                dati[_BP_EXT_MOTION] =_BP_EXT_MOTION_TERMINATED;
                dati[_BP_EXT_MOTION_END] =_BP_EXT_TIMEOUT_COMANDO;
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
                dati[_BP_EXT_MOTION] =_BP_EXT_MOTION_TERMINATED;
                dati[_BP_EXT_MOTION_END] =_BP_EXT_TIMEOUT_COMANDO;
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
                dati[_BP_EXT_MOTION] =_BP_EXT_MOTION_TERMINATED;
                dati[_BP_EXT_MOTION_END] =_BP_EXT_TIMEOUT_COMANDO;
                activationCommands =_BYM_NO_COMMAND;
                driverStatus = _BYM_DRIVER_STAT_CONNECTED;
                printf("BYM DRIVER: ERRORE CARICAMENTO TARGET\n");
                printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
                return;
            }
            _time_delay(50);
        }

        // Attivazione comando
        BiopsyDriverMoveXYZ(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_HOME:
        // Attivazione comando
        BiopsyDriverMoveHOME(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;


    case _BYM_MOVE_TO_STEP_DECZ:
        // Attivazione comando
        BiopsyDriverMoveDecZ(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_INCZ:
        // Attivazione comando
        BiopsyDriverMoveIncZ(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_DECX:
        // Attivazione comando
        BiopsyDriverMoveDecX(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_INCX:
        // Attivazione comando
        BiopsyDriverMoveIncX(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_DECY:
        // Attivazione comando
        BiopsyDriverMoveDecY(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
        BIOPSY_manageActivationLoop();
        break;

    case _BYM_MOVE_TO_STEP_INCY:
        // Attivazione comando
        BiopsyDriverMoveIncY(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH);
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
    while(!BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH, false)){
        _time_delay(500);
        tentativi--;
        if(tentativi == 0){
            dati[_BP_EXT_MOTION] =_BP_EXT_MOTION_TERMINATED;
            dati[_BP_EXT_MOTION_END] =_BP_EXT_TIMEOUT_COMANDO;
            activationCommands =_BYM_NO_COMMAND;
            driverStatus = _BYM_DRIVER_STAT_CONNECTED;
            printf("BYM DRIVER: TIMEOUT ATIVAZIONE\n");
            printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
            return;
        }
    }

    // Verifica risultato
    dati[_BP_EXT_MOTION] =_BP_EXT_MOTION_TERMINATED;
    if(generalConfiguration.biopsyCfg.extendedConf.statusL & 0x80) dati[_BP_EXT_MOTION_END] =_BP_EXT_ERROR_POSITIONINIG;
    else dati[_BP_EXT_MOTION_END] =_BP_EXT_POSITIONINIG_OK;

    // Azzeramento bit resettabili
    BiopsyDriverGetStat(&generalConfiguration.biopsyCfg.extendedConf.statusL, &generalConfiguration.biopsyCfg.extendedConf.statusH, true);

    // Acquisisce posizione attuale
    BiopsyDriverGetX(&generalConfiguration.biopsyCfg.X);
    BiopsyDriverGetY(&generalConfiguration.biopsyCfg.Y);
    BiopsyDriverGetZ(&generalConfiguration.biopsyCfg.Z);

    dati[_BP_EXT_XL] = (unsigned char) (generalConfiguration.biopsyCfg.X & 0x00FF);
    dati[_BP_EXT_XH] = (unsigned char) (generalConfiguration.biopsyCfg.X >> 8);
    dati[_BP_EXT_YL] = (unsigned char) (generalConfiguration.biopsyCfg.Y & 0x00FF);
    dati[_BP_EXT_YH] = (unsigned char) (generalConfiguration.biopsyCfg.Y >> 8);
    dati[_BP_EXT_ZL] = (unsigned char) (generalConfiguration.biopsyCfg.Z & 0x00FF);
    dati[_BP_EXT_ZH] = (unsigned char) (generalConfiguration.biopsyCfg.Z >> 8);

    activationCommands =_BYM_NO_COMMAND;
    driverStatus = _BYM_DRIVER_STAT_CONNECTED;
    printf("BYM DRIVER: ATIVAZIONE TERMINATA\n");
    printf("BYM DRIVER: CAMBIO STATO TO CONNECTED STATUS\n");
    return;
}



//____________________________________________________________________________________________________
// API DI PROTOCOLLO
bool biopsyExtendedMoveXYZ(unsigned short X, unsigned short Y, unsigned short Z)
{
  if(driverStatus != _BYM_DRIVER_STAT_CONNECTED) return FALSE; // E' già in corso

  targetX = X;
  targetY = Y;
  targetZ = Z;
  activationCommands = _BYM_MOVE_TO_XYZ;
  printf("BYM COMMAND: MOVE XYX, mTGX:%d, TGY:%d, TGZ:%d\n",X,Y,Z);
  return TRUE;
}
/*

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
*/



