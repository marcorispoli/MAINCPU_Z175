#ifndef _COMMON_H
#define _COMMON_H
  
#define PRINTCFG  // DEFINE PER ATTIVARE LE STAMPE DELLA CONFIGURAZIONE

typedef union
{
  unsigned short val16;
  struct
  {
    unsigned char lb;
    unsigned char hb;
  }bytes;
}_ShortByte_t;
#define _HB(x)  ((_ShortByte_t*) (&(x)))->bytes.hb
#define _LB(x)  ((_ShortByte_t*) (&(x)))->bytes.lb

///////////////////////////////////////////////////////////////////////////
// Gestione dei registri dei dispositivi
///////////////////////////////////////////////////////////////////////////
// Struttura dati passata dall'applicazione per l'aggiornamento dei parametri
#define MAX_NLIST 80 // Massimo numero di registri assegnabili ad una lista
 
  // Struttura per gestire una lista di registri 
  typedef struct
  {
    unsigned char nitem; // Numero di elementi impostati nella struttura
    unsigned char id[MAX_NLIST];    // id<PCB215_NREGISTERS
    unsigned short val[MAX_NLIST];  // valore da assegnare 
    unsigned char  crc; // CRC di nitem elementi della struttura
  }_DeviceAppRegister_Str;

  // Struttura di definizione registri del dispositivo
  typedef struct
  {
    unsigned char address;
    unsigned char banco;
    unsigned char size;
    unsigned char access;
    unsigned char vl;
    _ShortByte_t  val;
 }_DeviceRegItem_Str;
#define SIZEREG(x) sizeof(x)/sizeof(_DeviceRegItem_Str)
 
 
// Macro per gestire la definizione dei registri 
#define __REGID(x1,x2,x3,x4,x5,x6,x7)   x1
#define _REGID(x) __REGID(x)
#define __REGDEF(x1,x2,x3,x4,x5,x6,x7) x2,x3,x4,x5,x6,x7
#define _REGDEF(x) __REGDEF(x)
#define _IDREG(x1)  x1,0,0,0,0,0,0      // Crea un set di valori tipo registro
#define __DEVADDR(x1,x2,x3,x4,x5,x6,x7) x2 // Restituisce l'indirizzo del registro
#define _DEVADDR(x) __DEVADDR(x)
#define __DEVBANCO(x1,x2,x3,x4,x5,x6,x7) x3 // Restituisce il banco del registro
#define _DEVBANCO(x) __DEVBANCO(x)

// Macro per testatre lo stato di un flag  
#define __TEST_BIT(x,bit,ptr)     (ptr[x].val.bytes.lb & (1<<bit)) ? TRUE : FALSE   
#define _TEST_BIT(x) __TEST_BIT(x)

#define __SET_BIT(x,bit,ptr)     ptr[x].val.bytes.lb |= (1<<bit)
#define _SET_BIT(x) __SET_BIT(x)

#define __CLR_BIT(x,bit,ptr)     ptr[x].val.bytes.lb &= ~(1<<bit)
#define _CLR_BIT(x) __CLR_BIT(x)

// Macro per definire i campi di un registro 
#define _BNK01 0       // Registro in banchi 0 / 1
#define _BNK23 1       // Registro in banchi 2 / 3

#define _RW     0       // Read write register
#define _RD     1       // Read only register

#define _8BIT   0       // Registro a 8 bit
#define _16BIT  1       // Registro a 16 bit

#define _VL     1        // Imposta il registro come Volatile
#define _NVL    0        // Imposta il registro come Non Volatile

// MACRO PER DEFINIRE E GESTIRE I COMANDI DI PROTOCOLLO 422
  #define __CMD1(x,y)       x 
  #define __CMD2(x,y)       y 
  
  #define _CMD1(x)       __CMD1(x) // Restituisce il Data1 del comando
  #define _CMD2(x)       __CMD2(x) // Restituisce il Data2 del comando
 
// Questa macro permette di controllare in fase di compilazione 
// una qualsiasi condizione sulla dimensione delle variabili o 
// dei tipi
#define SIZE_CHECK(condition)   ((void) sizeof(char[1-2*!!(condition)]))
 
#define __IDTASK(x,y) (x)
#define _IDTASK(x) __IDTASK(x)
#define __PRIOTASK(x,y) (y)
#define _PRIOTASK(x) __PRIOTASK(x)


//////////////////////////////////////////////////////////////////////////////
//                   INTERFACCIA MCC SHARED AREA
//////////////////////////////////////////////////////////////////////////////

 // PORTE DI PROTOCOLLO MCC
#define _MCCPORT(x,y,z) (z)


// Collimazione statica (2D)
typedef struct
{
    unsigned char    front;
    unsigned char    back;
    unsigned char    trap;
    unsigned char    left;
    unsigned char    right;
}colli2DConf_Str;

typedef struct
{
  bool colliDinamicaAbilitata;    // La collimazione dinamica � attivabile
  unsigned char codiceAccessorio; // Codice riconosciuto per l'accessorio
  colliTomoConf_Str dynamicArray; // Contiene il valore delle lame in collimazione dinamica
  colli2DConf_Str   lame2D;       // Configurazione corrente lame 2D 
  unsigned char tempcuffia_on;    // Attivazione allarme cuffia
  unsigned char tempcuffia_off;   // Reset allarme cuffia

}colliConf_Str;



typedef struct
{
  unsigned char potId;
  unsigned char potMagFactor; // 0 to 7 max
  unsigned char potDescriptor; // Dati aggiuntivi per Analogica
  bool cassette;        // Presenza cassetta
  bool cassetteExposed; // cassetta esposta


}potterCfg_Str;


typedef struct
{
    biopsyConfExt_Str  conf;

    // Registri interni al driver
    unsigned char  statusL;        // Registro di stato L
    unsigned char  statusH;        // Registro di stato H
    unsigned char  stepVal;        // Quantit� di dm per step

    bool power_led;                // Stato di accensione del led di potenza

}biopsyStatExtended_Str;

typedef struct
{
    biopsyConfStd_Str  conf;

    // Configurazione dalla GUI
    bool needlePresent;         // Presenza needle

    bool movimento;             // Flag generale di movimento in corso
    bool movimento_x;           // Un comando di movimento su asse X � in esecuzione
    bool movimento_y;           // Un comando di movimento su asse X � in esecuzione
    bool movimento_z;           // Un comando di movimento su asse X � in esecuzione
    bool stepUp_z;              // Muove uno step su
    bool stepDwn_z;             // Muove uno step giu
    bool z_up;                  // Il movimento di z � UP/DOWN


    // Valori da passare alla torretta
    unsigned char zlimit;
    unsigned char zlesione;
    unsigned char lago;

}biopsyStatStandard_Str;

#define _BYM_NOT_DEFINED    0
#define _BYM_STANDARD_MODEL 1
#define _BYM_EXTENDED_MODEL 2

typedef struct
{
    // Definisce se e quale modello � stato identificato
    int biopsyModel;

    biopsyStatExtended_Str extendedConf;
    biopsyStatStandard_Str standardConf;
    int Z_basePosizionatore;
    int offsetPad;
    int margineRisalita;



    // Pulsante sblocco e adapter
    unsigned short adapterId;    // Identificazione adattatore
    bool sbloccoReq ;           // Pulsante di sblocco premuto

    // Posizione torretta
    unsigned short Z;           // Posizione corrente Z
    unsigned short X;           // Posizione corrente X
    unsigned short Y;           // Posizione corrente Y
    unsigned short SH;             // Posizione corrente SH

    // Target corrente
    unsigned short TGX;            // Posizione corrente Z
    unsigned short TGY;            // Posizione corrente X
    unsigned short TGZ;           // Posizione corrente Y

    // Dati firmware remoto
    unsigned char checksum_h;
    unsigned char checksum_l;
    unsigned char revisione;

    // Stato connessione e abilitazione ARM
    bool biopsyConnected;
    bool biopsyArmEna;

}biopsyStat_Str;


//////////////////////////////////////////////////////////////////////////////
//                   MACRO PER LA GESTIONE DEGLI EVENTI                      
//      DICHIARARE UN EVENTO CON LA SEGUENTE FORMULA:
//      #define <EV NAME> <MASK>,<Register>   
//      <EV NAME>: NOme simbolico evento
//      <MASK>: Maschera a 32 bit relativo all'evento
//      <Register>: registro eventi su cui associare l'evento
//////////////////////////////////////////////////////////////////////////////

#define __EVVAR(x,y)    (y)
#define _EVVAR(x)   __EVVAR(x)

#define __EVBIT(x,y)    (x)
#define _EVBIT(x)   __EVBIT(x)

#define __IS_EVENT(x,y) ((y.VALUE) & (x))
#define _IS_EVENT(x) __IS_EVENT(x)

#define __EVSET(x,y)            _lwevent_set(&(y),(x))
#define __EVCLR(x,y)            _lwevent_clear(&(y),(x))
#define __EVWAIT_TALL(x,y,t)    msEventWaitAll((x),&(y),t)  
#define __EVWAIT_TANY(x,y,t)    msEventWaitAny((x),&(y),t)  
#define __EVWAIT_ALL(x,y)       _lwevent_wait_for(&(y),(x),TRUE,0)  
#define __EVWAIT_ANY(x,y)       _lwevent_wait_for(&(y),(x),FALSE,0)  
#define __EVM(x,y)              (x)
#define __EVR(x,y)              (y)
#define __MOR2(x1,x2,y1,y2)                     (x1|y1), (x2)
#define __MOR3(x1,x2,y1,y2,z1,z2)               (x1|y1|z1) , (x2)      
#define __MOR4(x1,x2,y1,y2,z1,z2,k1,k2)         (x1|y1|z1|k1), (x2)
#define __MOR5(x1,x2,y1,y2,z1,z2,k1,k2,j1,j2)   (x1|y1|z1|k1|j1), (x2)
  
#define _EVSET(x)       __EVSET(x)         // Setta un evento
#define _EVCLR(x)       __EVCLR(x)         // Azzera un evento
#define _EVWAIT_TANY(x,t)  __EVWAIT_TANY(x,t)    // Attende indefinitamente su almeno una condizione
#define _EVWAIT_TALL(x,t)  __EVWAIT_TALL(x,t)    // Attende indefinitamente su tutta la condizione
#define _EVWAIT_ANY(x)  __EVWAIT_ANY(x)    // Attende indefinitamente su almeno una condizione
#define _EVWAIT_ALL(x)  __EVWAIT_ALL(x)    // Attende indefinitamente su tutta la condizione
#define _EVM(x)         __EVM(x)           // Estrae la sola maschera
#define _EVR(x)         __EVR(x)           // Estrae il registro
#define _MOR2(x,y) __MOR2(x,y)             // Effettua OR di 2 eventi
#define _MOR3(x,y,z) __MOR3(x,y,z)         // Effettua OR di 3 condizioni
#define _MOR4(x,y,z,k) __MOR4(x,y,z,k)     // Effettua OR di 4 condizioni
#define _MOR5(x,y,z,k,j) __MOR5(x,y,z,k,j) // Effettua OR di 5 condizioni
    
// From short to Little Endian x[0:1] <--- (y)
#define TO_LE16(x,y) *(x)=(uint8_t)(y); *(x+1)=(uint8_t)(y>>8)
// From Little Endian to short  x <--- (y[0:1])
#define FROM_LE16(x,y) x=(*y) + *(y+1) * 256

// Il seguente tipo deve essere inferiore a 8 byte massimo
// senza canmbiare la struttura del framing CAN
typedef struct 
{
  _SystemInputs_Str inputs; // Pacchetto di inputs.
  
  // Flags di stato tra Terminali
  unsigned char changed:1; // Flag per dati cambiati  
  unsigned char flgspare:3;
  unsigned char frcnt:4;   // Progressivo messaggio corrente
}_CanSlaveFrame;

typedef struct
{
  unsigned char maj;   // Major number
  unsigned char min;   // Minor number
  unsigned char model; // Identifica il modello se necessario
}revStr;


// Codice segnalazioni da ACTUATORS SLAVE
typedef enum{

    ACUATORS_CMD_NONE=0,
    ACTUATORS_CMD_TEST,
    ACTUATORS_GET_STATUS,
    ACTUATORS_START_PROCESSES,

    // ______________________________ SEZIONE SEGNALAZIONI DA LENZE
    ACTUATORS_LENZE_CMD_SECTION,
    ACTUATORS_FAULT_LENZE,
    ACTUATORS_LENZE_RUN_STAT,
    ACTUATORS_SET_LENZE_CONFIG,
    ACTUATORS_MOVE_LENZE,   // tbd
    ACTUATORS_LENZE_UNPARK,
    ACTUATORS_LENZE_PARK,

    // ______________________________ SEZIONE SEGNALAZIONI DA TRX
    ACTUATORS_TRX_CMD_SECTION,
    ACTUATORS_TRX_QUICK_STOP,
    ACTUATORS_MOVE_TRX_ON,
    ACTUATORS_MOVE_TRX,
    ACTUATORS_MOVE_MANUAL_TRX,
    ACTUATORS_FAULT_TRX,
    ACTUATORS_TRX_POLLING_STATUS,
    ACTUATORS_TRX_IDLE,
    ACTUATORS_SET_TRX_ZERO,
    ACTUATORS_SET_TRX_CONFIG,

    // _______________________________ SEZIONE SEGNALAZIONI DA ARM
    ACTUATORS_ARM_CMD_SECTION,
    ACTUATORS_MOVE_ARM_ON,
    ACTUATORS_MOVE_ARM,
    ACTUATORS_MOVE_MANUAL_ARM,
    ACTUATORS_FAULT_ARM,
    ACTUATORS_ARM_POLLING_STATUS,
    ACTUATORS_SET_ARM_CONFIG,


    ACTUATORS_LAST_STATUS

}actuatorEnumCommands_t;

typedef enum{
    ACUATORS_ARM_POLLING_IDLE=1,
    ACTUATORS_ARM_POLLING_FAULT,
    ACTUATORS_ARM_POLLING_RUN,
    ACUATORS_TRX_POLLING_IDLE,
    ACTUATORS_TRX_POLLING_FAULT,
    ACTUATORS_TRX_POLLING_RUN,
    ACUATORS_LENZE_POLLING_IDLE,
    ACTUATORS_LENZE_POLLING_FAULT,
    ACTUATORS_LENZE_POLLING_RUN

}actuatorEnumPollingStatus_t;


// Set di possibili attivazioni
#define CONTEXT_TRX_ZEROSETTING      0
#define CONTEXT_TRX_2D               1
#define CONTEXT_TRX_NARROW           2
#define CONTEXT_TRX_INTERMEDIATE     3
#define CONTEXT_TRX_WIDE             4
#define CONTEXT_TRX_STEP_SHOT        5
#define CONTEXT_TRX_SLOW_MOTION      6

// Defines per il comando ACTUATORS_GET_STATUS
#define BYTE_GET_STATUS_CONNECTIONS   1
    #define SLAVE_DEVICE_CONNECTED  0x1
    #define TRX_CONNECTED_STATUS    0x2
    #define ARM_CONNECTED_STATUS    0x4
    #define LENZE_CONNECTED_STATUS  0x8
    #define PCB240_CONNECTED_STATUS 0x10

#define BYTE_GET_STATUS_PCB240_MAJ   2 // Revisione PCB240
#define BYTE_GET_STATUS_PCB240_MIN   3
#define BYTE_GET_STATUS_M4_MAJ       4 // Revisione driver M4
#define BYTE_GET_STATUS_M4_MIN       5

#define BYTE_GET_STATUS_ERRORS       6 // Errori importanti per la diagnostica durante lo startup
#define BYTE_GET_STATUS_7   7


#ifdef _MAIN_C_
  _SystemInputs_Str SystemInputs;
  _SystemOutputs_Str SystemOutputs;
  MUTEX_STRUCT input_mutex;   
  MUTEX_STRUCT output_mutex; 

#else
  extern _SystemInputs_Str SystemInputs;
  extern _SystemOutputs_Str SystemOutputs;
  extern MUTEX_STRUCT input_mutex;   
  extern MUTEX_STRUCT output_mutex; 

#endif


// Funzioni comuni // Common.c
#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _COMMON_C
  #define ext 
  #define extrd 
#else
  #define ext extern
  #define extrd extern const
#endif

ext bool msEventWaitAll(int mask, LWEVENT_STRUCT* ev, unsigned int ms);
ext bool msEventWaitAny(int mask, LWEVENT_STRUCT* ev, unsigned int ms);
ext void setOutputs(_SystemOutputs_Str* pSet, _SystemOutputs_Str* pMask); // Impostazione degli Outputs di sistema

ext int nearest(float v);
ext float absF(float f);
ext int absI(int i);
ext void debugPrint(const char* buffer);
ext void debugPrintEna(bool stat);
ext void printEna(bool stat);

void debugPrintIBuffer(char* destination, int* index, char* tag, int val);
void debugPrintXBuffer(char* destination, int* index, char* tag, unsigned int val);

ext void debugPrintI(const char* buffer, int var);
ext void debugPrintI2(const char* buffer1, int var1,const char* buffer2, int var2);
ext void debugPrintI3(const char* buffer1, int var1,const char* buffer2, int var2,const char* buffer3, int var3);
ext void debugPrintI4(const char* buffer1, int var1,const char* buffer2, int var2,const char* buffer3, int var3,const char* buffer4, int var4);

ext void debugPrintX(const char* buffer, unsigned int var);
ext void debugPrintX2(const char* buffer1, unsigned int var1,const char* buffer2, unsigned int var2);
ext void debugPrintX3(const char* buffer1, unsigned int var1,const char* buffer2, unsigned int var2,const char* buffer3, unsigned int var3);
ext void debugPrintX4(const char* buffer1, unsigned int var1,const char* buffer2, unsigned int var2,const char* buffer3, unsigned int var3,const char* buffer4, unsigned int var4);

ext void debugPrintF(const char* buffer, float var);


#endif
