#ifndef _MCC_SHARED_H
#define _MCC_SHARED_H
  
//////////////////////////////////////////////////////////////////////////////
//                   INTERFACCIA MCC SHARED AREA
//////////////////////////////////////////////////////////////////////////////
#define _DEF_MCC_GUI_TO_M4_MASTER              1,0,3 // M4-Master Riceve su questo canale dal DBTController Master
#define _DEF_MCC_MASTER_TO_APP_MASTER          0,0,4 // M4-Master Invia messaggi a DBTController Master

#define _DEF_APP_SLAVE_TO_M4_SLAVE             1,0,7 // M4-Slave Riceve su questo canale dal DBTController Slave
#define _DEF_M4_SLAVE_TO_APP_SLAVE             0,0,8 // M4-Slave Invia messaggi a DBTController Slave

#define _DEF_M4_MASTER_DEBUG_MESSAGES_MCC      0,0,1 // M4-Master Invia messaggi di debug a DBTController Master
#define _DEF_M4_SLAVE_DEBUG_MESSAGES_MCC       0,0,2 // M4-Slave Invia messaggi di debug a DBTController Slave


// _______________________________________________
// SOTTOGRUPPO COMANDI MCC_CALIB_ZERO
#define CALIB_ZERO_MANUAL_ACTIVATION_TRX_CALIB     1 // Attivazione manuale TRX lenta
#define CALIB_ZERO_MANUAL_ACTIVATION_ARM_CALIB     2 // Attivazione manuale ARM lenta
#define CALIB_ZERO_MANUAL_ACTIVATION_TRX_STANDARD  3 // Attivazione manuale TRX 2D
#define CALIB_ZERO_MANUAL_ACTIVATION_ARM_STANDARD  4 // Attivazione manuale ARM standard

#define CALIB_ZERO_ACTIVATE_TRX_ZERO_SETTING       5 // Attivazione azzeramento Tubo
#define CALIB_ZERO_ACTIVATE_ARM_ZERO_SETTING       6 // Attivazione azzeramento Arm
#define CALIB_ZERO_ACTIVATE_GONIO_ZERO_SETTING     7 // Attivazione azzeramento Gonio
// _______________________________________________

// Sottocomandi gruppo MCC_CONFIG
#define CONFIG_STATUS         1
#define CONFIG_GANTRY         2
#define CONFIG_GENERAL        3

#define CONFIG_PCB190         20
#define CONFIG_PCB269_0       30
#define CONFIG_PCB269_1       31
#define CONFIG_PCB249U2       40
#define CONFIG_PCB244         50
#define CONFIG_BIOPSY         60
#define CONFIG_TRX            70
#define CONFIG_ARM            80
#define CONFIG_LENZE          90


#define CONFIG_PCB249U1_1     110
#define CONFIG_PCB249U1_2     111
#define CONFIG_PCB249U1_3     112
#define CONFIG_PCB249U1_4     113
#define CONFIG_PCB249U1_5     114
#define CONFIG_PCB249U1_6     115
#define CONFIG_PCB249U1_7     116

#define CONFIG_COMPLETED      255

//_____________________________________________________________________________
// Sottocomandi gruppo MCC_ACTUATOR_NOTIFY
typedef enum
{
  ACTUATOR_STATUS = 1

}_MccActuatorNotify_Code;

//_____________________________________________________________________________
// Sottocomandi gruppo LOADER
typedef enum
{
  LOADER_ACTIVATION = 1,
  LOADER_CHIP_ERASE,
  LOADER_WRITE_BLK,
  LOADER_WRITE_CONFIG,
  LOADER_WRITE_COMPLETED,
  LOADER_READ_CONFIG,
  LOADER_ACTIVATION_TEST
}_MccLoaderNotify_Code;
//_____________________________________________________________________________

//_____________________________________________________________________________
// Comando MCC_SET_COLLI
#define COLLI_F 0   // Lama frontale
#define COLLI_B 1   // Lama posteriore
#define COLLI_L 2   // Lama Left
#define COLLI_R 3   // Lama Right
#define COLLI_T 4   // Lama Trapezio
#define COLLI_LEN 5 // Dimensione dati
//_____________________________________________________________________________

//_____________________________________________________________________________
// Comandi sottogruppo PCB244 (MCC_PCB244_COMMANDS)
#define MCC_PCB244_CMD_START_VC
#define MCC_PCB244_CMD_STOP_VC
#define MCC_PCB244_CMD_SET_FREQ
#define MCC_PCB244_CMD_SET_AMPL
#define MCC_PCB244_CMD_GET_PARAM
//_____________________________________________________________________________

//____________________________________________________
// BYTE PER COMANDO DI NOTIFICA COMPRESSORE
#define COMPRESSORE_FLAG0   0
#define COMPRESSORE_FLAG1   1
#define COMPRESSORE_FORZA   2
#define COMPRESSORE_THICKL  3
#define COMPRESSORE_THICKH  4
#define COMPRESSORE_PAD     5
#define COMPRESSORE_TARGET  6
#define COMPRESSORE_POSL     7
#define COMPRESSORE_POSH     8
#define PCB215_NOTIFY_COMPR_DATA_LEN 9
//_____________________________________________________________________________
// Codici per notifiche PCB215
typedef enum
{
  PCB215_NOTIFY_COMPR_DATA=0,

  PCB215_NOTIFY_CALIB_DATA,     // buffer[0]: SYS_FLAGS,
                                // buffer[1]: SYS_FLAGS1,
                                // buffer[2:3]: RAW PADDLE,
                                // buffer[4]: RAW STRENGHT,

  PCB215_NOTIFY_ERRORS          // buffer[0]: codice errore

}_MccPCB215Notify_Code;
//_____________________________________________________________________________


// PCB215_NOTIFY_ERRORS
    #define PCB215_NO_ERRORS            0
    #define PCB215_ERROR_PEDALS_STARTUP 1
//_____________________________________________________________________________
// Codici posizionali per messaggio dati di fine esposizione
#define RX_END_CODE     0
#define MAS_PRE_L       1
#define MAS_PRE_H       2
#define MAS_PLS_L       3
#define MAS_PLS_H       4
#define I_MEAN          5
#define V_MEAN          6
#define V_SIGMA         7
#define T_MEAN_PRE_L    8
#define T_MEAN_PRE_H    9
#define T_MEAN_PLS_L    10
#define T_MEAN_PLS_H    11
#define HV_POST_RAGGI   12
#define IFIL_POST_RAGGI 13
#define NSAMPLES_AEC    14
#define NSAMPLES_PLS    15
#define SAMPLES_BUFFER  16
#define RX_DATA_LEN     17 // Buffer dati ....



// Codici per notifiche PCB190
#define PCB190_FLAGS0           0
#define PCB190_DGN_HV_IDLE      1
#define PCB190_DGN_VTEMP        2
#define PCB190_I_FIL            3
#define PCB190_I_ANODICA        4
#define PCB190_MAS_TEST_L       5
#define PCB190_MAS_TEST_H       6
#define PCB190_V32              7
#define PCB190_VM32             8
#define PCB190_V12              9
#define PCB190_VM12             10
#define PCB190_V15              11
#define PCB190_V15EXT           12
#define PCB190_FAULTS           13
#define PCB190_ARLS             14
#define PCB190_ARHS             15
#define PCB190_DGN_LEN          16

typedef enum
{
  PCB190_NOTIFY_DGN=0

}_MccPCB190Notify_Code;


// Comandi tra SLAVE GUI e SLAVE M4 tramite MCC
typedef enum
{
    MCC_SLAVE_NOTIFY=0, // Notifica da M4 SLAVE
    MCC_SLAVE_GET_REV   // Richiesta revisione

}slaveToSlaveMccEnum;

// Codici per notifiche PCB249U1
typedef enum
{
  PCB249U1_NOTIFY_DATA=0        // buffer[0]: SYSTEM FLAGS0,
                                // buffer[1]: TEMPERATURA CUFFIA,

}_MccPCB249U1Notify_Code;


// _____________________________________________________________________
// Codici per notifiche Biopsia Standard
#define _BP_STD_CONNESSIONE 0
#define _BP_STD_SBLOCCO     1
#define _BP_STD_ACCESSORIO  2
#define _BP_STD_ZL          3
#define _BP_STD_ZH          4
#define _BP_STD_MOTION      5
#define _BP_STD_MOTION_END  6
#define _BP_STD_CHKH        7
#define _BP_STD_CHKL        8
#define _BP_STD_REVIS       9
#define _BP_STD_MAX_Z       10
#define _BP_STD_DATA_LEN    11





// _____________________________________________________________________
// Codici per notifiche Biopsia estesa
#define _BP_EXT_CONNESSIONE     0 // Stato della connessione
#define _BP_EXT_MOTION          1 // Stato dell'attivazione in corso
#define _BP_EXT_MOTION_END      2 // Risultato del movimento appena terminato
#define _BP_EXT_PUSH_SBLOCCO    3 // Stato del pulsante di sblocco
#define _BP_EXT_ADAPTER_ID      4 // Codice holder riconosciuto
#define _BP_EXT_ASSEX_POSITION  5 // Dislocamento asse-X
#define _BP_EXT_ASSEY_POSITION  6 // Dislocamento asse-Y (1 = Upright)

// Posizione torretta in dmm
#define _BP_EXT_XL              7
#define _BP_EXT_XH              8
#define _BP_EXT_YL              9
#define _BP_EXT_YH              10
#define _BP_EXT_ZL              11
#define _BP_EXT_ZH              12
#define _BP_EXT_SHL             13
#define _BP_EXT_SHH             14
#define _BP_EXT_MAN             15

// Dati per la revisione e checksum
#define _BP_EXT_CHKH            16
#define _BP_EXT_CHKL            17
#define _BP_EXT_REVIS           18
#define _BP_EXT_MODEL           19
//______________________________
#define _BP_EXT_DATA_LEN        20


//_____________________________

// Codici relativi al tipo di adapter ID
#define _BP_EXT_ADAPTER_OPEN            0
#define _BP_EXT_ADAPTER_A               1
#define _BP_EXT_ADAPTER_B               2
#define _BP_EXT_ADAPTER_C               3
#define _BP_EXT_ADAPTER_SHORT           4

#define _BP_EXT_ASSEX_POSITION_ND       0
#define _BP_EXT_ASSEX_POSITION_LEFT     3
#define _BP_EXT_ASSEX_POSITION_CENTER   2
#define _BP_EXT_ASSEX_POSITION_RIGHT    1


// Codici relativi allo stato della connessione
#define _BP_EXT_CONNESSIONE_DISCONNECTED            0
#define _BP_EXT_CONNESSIONE_CONNECTED               1

// Codici relativi allo stato del movimento
#define _BP_EXT_NO_MOTION                     0
#define _BP_EXT_MOTION_ON                     1
#define _BP_EXT_MOTION_TERMINATED             2

// Codici risultato movimento
#define _BP_EXT_TIMEOUT_COMANDO     1
#define _BP_EXT_ERROR_POSITIONINIG  2
#define _BP_EXT_POSITIONINIG_OK     3

// Codici pulsante di sblocco
#define _BP_EXT_PUSH_SBLOCCO_DISATTIVO           0
#define _BP_EXT_PUSH_SBLOCCO_ATTIVO              1

// Codici relativo al modello di torretta
#define _BP_EXT_MODEL_UNDEFINED     0
#define _BP_EXT_MODEL_UPRIGHT       1
#define _BP_EXT_MODEL_LATERAL       2

// Codici relativo alla lateralità
#define _BP_EXT_LATERALITY_DISABLED  0
#define _BP_EXT_LATERALITY_UNDEFINED 1
#define _BP_EXT_LATERALITY_UPRIGHT   2
#define _BP_EXT_LATERALITY_LEFT      3
#define _BP_EXT_LATERALITY_RIGHT     4

// __________________________________________________________
// CODICI COMANDO BIOPSIA (DA GUI A M4): MCC_BIOPSY_CMD
#define _MCC_EXT_BIOPSY_CMD_MOVE_HOME   1
#define _MCC_EXT_BIOPSY_CMD_MOVE_XYZ    2
#define _MCC_EXT_BIOPSY_CMD_MOVE_INCX   3
#define _MCC_EXT_BIOPSY_CMD_MOVE_DECX   4
#define _MCC_EXT_BIOPSY_CMD_MOVE_INCY   5
#define _MCC_EXT_BIOPSY_CMD_MOVE_DECY   6
#define _MCC_EXT_BIOPSY_CMD_MOVE_INCZ   7
#define _MCC_EXT_BIOPSY_CMD_MOVE_DECZ   8
#define _MCC_EXT_BIOPSY_CMD_SET_STEPVAL 9
#define _MCC_EXT_BIOPSY_CMD_SET_ZLIMIT  10
#define _MCC_EXT_BIOPSY_CMD_SET_BUZZER  11
#define _MCC_EXT_BIOPSY_CMD_SET_POWERLED 12


//____________________________________________________________
// CODICI COMANDI SIMULATORE DI BIOPSIA SE COMPILATO
#define _BYM_SIM_CONNECTION     1
#define _BYM_SIM_SBLOCCO        2
#define _BYM_SIM_ADAPTER        3
#define _BYM_SIM_SH             4
#define _BYM_SIM_LAT            5
#define _BYM_SIM_SET_CURSOR     6

#define _BYM_SYM_ADAPTER_A      1
#define _BYM_SYM_ADAPTER_B      2
#define _BYM_SYM_ADAPTER_C      3
#define _BYM_SYM_ADAPTER_O      4
#define _BYM_SYM_ADAPTER_S      5


//________________________________________________________________________
typedef enum
{
  BIOP_NOTIFY_STAT=0,
  BIOP_NOTIFY_MOVE_CMD
}_MccBiopNotify_Code;




typedef struct
{
  unsigned char cmd;
  unsigned char buffer[100];     // Buffer con i dati dalla GUI
}_MccGuiToDevice_Str;

typedef enum
{
    SRV_FREEZE_DEVICE=0,          // Freeze dei devices
    SRV_SERIAL_SEND,              // Invio comandi seriali
    SRV_RODAGGIO_TUBO,            // Attivazione rodaggio del tubo
    SRV_ARM_STOP,                 // Fermo di qualsiasi movimento in corso
    SRV_START_POTTER_3D_GRID,     // Attivazione Griglia 3D
    SRV_STOP_POTTER_3D_GRID,      // Stop Griglia 3D
    SRV_PCB244_CMD_FREQ_VC,       // Modifica frequenza VC
    SRV_PCB244_CMD_AMPL_VC,        // Modifica Ampiezza oscillazione VC

    SRV_START_POTTER_2D_GRID,     // Attivazione Griglia 2D
    SRV_STOP_POTTER_2D_GRID,      // Stop Griglia 2D
    SRV_RESET_PCB244,             // Reset scheda PCB244
    SRV_RESET_FAULT_PCB244,       // Reset Fault PCB244
    SRV_TEST_LS_STARTER,          // RIchiesta attivazione test starter bassa velocità
    SRV_TEST_422,                  // RIchiesta attivazione test RS422
    SRV_TEST_LOADER,               // Test comunicazione con loader
    SRV_TEST_INTERPROCESS         // Test comunicazione interna



}_MccServiceNotify_Code;

typedef enum
{
    MCC_PCB244_A_GET_REV = 0,   // Restituisce la revisione corrente
    MCC_PCB244_A_GET_RADx1,     // Restituisce il campionamento x1
    MCC_PCB244_A_GET_RADx5,     // Restituisce il campionamento x5
    MCC_PCB244_A_GET_RADx25,    // Restituisce il campionamento x25
    MCC_PCB244_A_SET_OFFSET,    // Imposta un dato valore di offset
    MCC_PCB244_A_ACTIVATE_GRID, // Attiva la griglia per un certo numero di passate
    MCC_PCB244_A_GET_CASSETTE,  // Legge la presenza/stato della cassetta
    MCC_PCB244_A_GET_ID,        // Legge la codifica dell'accessorio
    MCC_PCB244_A_SET_FIELD,     // Imposta il campo corrente
    MCC_PCB244_A_RESET_BOARD,   // Effettua il reset della scheda
    MCC_PCB244_A_RX_ABORT       // Forza l'uscita da una sequenza

}_MccPCB244A_Code;

// Comandi per gestione Parcheggio
#define MCC_PARKING_MODE_COMMANDS_START_PARKING         1
#define MCC_PARKING_MODE_COMMANDS_START_UNPARKING       2
#define MCC_PARKING_MODE_COMMANDS_START_CALIBRATION     3
#define MCC_PARKING_MODE_COMMANDS_STOP_CALIBRATION      4
#define MCC_PARKING_MODE_COMMANDS_GET_POT               5


// Comandi tra MASTER GUI e MASTER M4 tramite MCC
typedef enum
{
    MCC_CONFIG = 1,       // Gruppo Configurazione dispositivi
    MCC_SERVICE,          // Comandi specifici per porta di servizio
    MCC_LOADER,           // Comandi riservati al Loader

    // MESSAGGI SERIALI
    MCC_SERIAL_READ,      // Richiesta di lettura
    MCC_SERIAL_WRITE,     // Richiesta di scrittura
    MCC_SERIAL_CMD,       // Richiesta di invio comando
    MCC_SERIAL_CMDSPC,    // Richiesta di invio comando speciale

    // PCB240
    MCC_SET240_CMD,       // Richiesta di invio comando a PCB240
    MCC_SET_OUTPUTS,      // Richiesta di scrittura Outputs
    MCC_GET_IO,           // Richiesta stato Outputs

    // PCB-269
    MCC_CMD_SBLOCCO,      // Richiede una sequenza di sblocco compressore
    MCC_CMD_PAD_UP,       // Richiede una sequenza di attivazione carrello compressore UP
    MCC_CMD_PCB215_CALIB, // Richiede attivazione modo calibrazione compressore
    MCC_CMD_CMP_STOP,     // Ferma motore del compressore
    MCC_CMD_CMP_AUTO,     // Manda in com pressione il compressore
    MCC_GET_TROLLEY,      // Richiede la posizione del compressore anche se non in compressione
    MCC_NOTIFY_COMPR,     // NOTIFICHE dal compressore

    // Rotazioni motorizzate
    MCC_CMD_ARM,          // Comando completo di movimentazione Braccio
    MCC_CMD_TRX,          // Comando completo di movimentazione Tubo
    MCC_ARM_ERRORS,       // Comunicazione errori da ARM
    MCC_TRX_ERRORS,       // Comunicazione errori da TRX
    MCC_LENZE_ERRORS,     // Comunicazione errori da LENZE

    // PCB190
    MCC_CMD_RAGGI_STD,    // Richiesta attivazione sequenza raggi (MASTER M4)
    MCC_CMD_RAGGI_AEC,    // Richiesta attivazione sequenza raggi AEC
    MCC_CMD_EXP_AEC,      // Aggiornamento con dati di esposizione
    MCC_CMD_RAGGI_TOMO,   // Richiesta attivazione sequenza raggi (MASTER M4)
    MCC_CMD_RAGGI_AEC_TOMO, // Richiesta attivazione sequenza raggi (MASTER M4)
    MCC_CMD_RAGGI_AE,       // Richiesta sequenza raggi Bassa/Alta energia
    MCC_CMD_RAGGI_AE_AEC,   // Richiesta sequenza raggi Bassa/Alta energia con pre impulso
    MCC_CMD_RAGGI_AE_H,     // Dati per impulso alta energia
    MCC_PRINT,            // GEstione delle debugPrint
    MCC_TEST_RX_SHOT,     // Attiva una sequenza manuale raggi standard senza detector
    MCC_STARTER,          // GEstione attivazione Starter
    MCC_SET_FUOCO,        // Impostazione Fuoco

    // PCB244 - POTTER
    MCC_POTTER_ID,
    MCC_244_A_DETECTOR_FIELD, // Impostazione campo esposimetro


    // PCB249U1 - COLLIMAZIONE LAME LATERIALI e BACK TRAP
    MCC_RESET_GONIO,      // Effettua il reset degli inclinometri
    MCC_SET_COLLI,        // Effettua il setting della collimazione
    MCC_SET_MIRROR,       // Imposta lo stato corrente dello specchio
    MCC_SET_LAMP,         // Imposta lo stato corrente della luce di collimazione
    MCC_SET_FILTRO,       // Imposta il filtro
    MCC_CALIB_FILTRO,     // Calibrazione filtro

    // GENERALI
    MCC_GET_GONIO,       // Richiede i registri relativi agli angoli(*)
    MCC_RAGGI_DATA,      // Messaggio di notifica dati di esposizione

    // Comandi di impostazione configurazioni
    MCC_SET_ROT_TOOL_CONFIG,    // Configurazione del tool di gesitone rotazioni

    // BIOPSY
    MCC_BIOPSY_SIMULATOR,     // Comando di attivazione/Disattivazione demo
    MCC_BIOPSY_CMD,           // Comando di movimento Biopsia a XYZ
    MCC_BIOPSY_STD_XYZ,       // Comando di movimento Biopsia Standard a XYZ


    MCC_GUI_NOTIFY,          // Notifica per l'applicazione da GUI M4
    MCC_CONFIG_NOTIFY,       // Notifica dal configuratore
    MCC_PCB215_NOTIFY,       // Notifica ad A5 da parte di PCB215

    MCC_PCB249U1_NOTIFY,     // Notifica ad A5 da parte di PCB249U1

    MCC_SERVICE_NOTIFY,       // Notifica ad A5 da parte di Service
    MCC_LOADER_NOTIFY,        // Notifica ad A5 da parte di Loader
    MCC_BIOP_STANDARD_NOTIFY, // Notifica da Biopsia Standard
    MCC_BIOP_EXTENDED_NOTIFY, // (56) Notifica da Biopsia Extended
    MCC_PCB190_NOTIFY,        // Notifiche da driver della scheda PCB190

    // Sezione dedicata ai test
    MCC_TEST,

    MCC_CANOPEN,
    MCC_POWER_OFF,
    MCC_POWER_MANAGEMENT,
    MCC_SLAVE_ERRORS,
    MCC_CALIB_ZERO,          // Comandi e notifiche (MASTER) per calibrazione azzeramenti
    MCC_CALIB_LENZE,          // Slave: gestione calibrazione potenziometro lenze
    MCC_GET_TRX_INPUTS,
    MCC_GET_ARM_INPUTS,
    MCC_GET_LENZE_INPUTS,
    MCC_PARKING_MODE_COMMANDS


}_MccGuiToDevice_Cmd;

#define MCC_DEBUG_PRINT_ENABLE_CMD 1
#define MCC_DRIVER_PRINT_ENABLE_CMD 2

#endif
