#define _PCB249U1_C
#include "dbt_m4.h" 

#define TARGET_ADDRESS 0x16
#define _DEF_PCB249U1_DRIVER_DELAY 500

#define CONTEST PCB249U1_CONTEST
#define STATUS  (*((_PCB249U1_Stat_Str*)(&PCB249U1_CONTEST.Stat)))
#define ERROR_HANDLER   pcb249U1DriverCommunicationError

// Funzione di servizio interne al modulo
static bool GetFwRevision(void);
static void enterFreezeMode(void);
static bool pcb249U1UpdateRegisters(void);
static void ERROR_HANDLER(void);

static bool resetDriver = TRUE;
static bool verifyUpdateRegisters = TRUE;

static bool identificazioneAccessorio(void);
static bool pcb249U1WaitBusy(int timeout);
static unsigned char left, right, trap;

// Valori compresi tra 118 (70�) (==0)  e 241 (==70)
static unsigned char airTemperature[] = {
    70, //118
    70,
    69, //120
    69,
    68, //122
    68,
    67, //124
    67,
    67, //126
    66,
    66, //128
    65,
    65, //130
    64,
    64, //132
    63,
    63, //134
    63,
    62, //136
    62,
    61, //138
    61,
    60, //140
    60,
    60, //142
    59,
    59, //144
    58,
    58, //146
    57,
    57, //148
    57,
    56, //150
    56,
    55, //152
    54,
    54, //154
    54,
    53,   //156
    53,
    52,   //158
    52,
    51,   //160
    51,
    51,   //162
    50,
    50,   //164
    49,
    49,   //166
    48,
    48,   //168
    48,
    47,   //170
    47,
    46,   //172
    46,
    45,   //174
    45,
    44,   //176
    44,
    43,   //178
    43,
    42,   //180
    42,
    42,   //182
    41,
    41,   //184
    40,
    40,   //186
    39,
    39,   //188
    38,
    38,   //190
    37,
    37,   //192
    36,
    36,   //194
    35,
    35,   //196
    34,
    34,   //198
    33,
    33,   //200
    32,
    32,   //202
    31,
    30,   //204
    30,
    29,   //206
    29,
    28,   //208
    27,
    27,   //210
    26,
    25,   //212
    25,
    24,   //214
    23,
    23,   //216
    22,
    21,   //218
    21,
    20,   //220
    19,
    19,   //222
    18,
    17,   //224
    16,
    16,   //226
    15,
    14,   //228
    13,
    12,   //230
    11,
    10,   //232
    9,
    8,   //234
    7,
    6,   //236
    5,
    4,   //238
    3,
    2,   //240
    1,   //241
    0    //242
};
void pcb249U1_driver(uint32_t taskRegisters)
{
  int i;
   _SER422_Error_Enum err_ret;
  bool write_ok;   
  unsigned char data[10];


    // Costruzione del contesto
   CONTEST.pReg = PCB249U1_Registers;
   CONTEST.nregisters = PCB249U1_NREGISTERS;
   CONTEST.evm = _EVM(_EV0_PCB249U1_CFG_UPD);
   CONTEST.evr = &_EVR(_EV0_PCB249U1_CFG_UPD);
   CONTEST.address = TARGET_ADDRESS;
   printf("ATTIVAZIONE DRIVER PCB249U1: \n");
    
   //////////////////////////////////////////////////////////////////////////
   //                   FINE FASE DI INIZIALIZZAZIONE DRIVER               //             
   //        Inizia il ciclo di controllo e gestione della periferica      //
   //////////////////////////////////////////////////////////////////////////

    // In caso di errore di compilazione in questo punto 
    // significa errore in compilazione della struttura registri
    SIZE_CHECK((sizeof(PCB249U1_Registers)/sizeof(_DeviceRegItem_Str))!=PCB249U1_NREGISTERS);
      
    // Segnalazione driver disconnesso
    _EVCLR(_EV1_PCB249U1_CONNECTED);
    
    // Retrive Task ID
    CONTEST.ID =  _task_get_id();
    
    // Init registro di stato
    memset((void*)&(STATUS), 0,sizeof(_Device_Stat_Str ));

   // Inizializzazione delle mutex
    if (_mutex_init(&(CONTEST.reglist_mutex), NULL) != MQX_OK)
    {
      printf("PCB249U1: ERRORE INIT MUTEX. FINE PROGRAMMA");
      _mqx_exit(-1);
    }

    if (_mutex_init(&(CONTEST.pollinglist_mutex), NULL) != MQX_OK)
    {
      printf("PCB249U1: ERRORE INIT MUTEX. FINE PROGRAMMA");
      _mqx_exit(-1);
    }
      
    // Reitera fino ad ottenere il risultato
    while(GetFwRevision()==FALSE)_time_delay(100);
    printf("PCB249U1:REVISIONE FW TARGET:%d.%d\n",STATUS.maj_code,STATUS.min_code); 

    // Legge il registro che identifica il modello del Collimatore
    while(Ser422ReadRegister(_REGID(PR_COLLI_MODEL),10,&CONTEST)!=_SER422_NO_ERROR) _time_delay(200);
    if(_DEVREGL(PR_COLLI_MODEL,CONTEST)==0){
        printf("PCB-14/249-2 ASSY 01 DETECTED\n");
        generalConfiguration.revisioni.pcb249U1.model = _COLLI_TYPE_ASSY_01;
    }else{
        printf("PCB-14/249-2 ASSY 02 DETECTED\n");
        generalConfiguration.revisioni.pcb249U1.model = _COLLI_TYPE_ASSY_02;
    }

    // Verifica della correttezza  dell'assy assegnato al collimatore rispetto alla revisione corrente
    generalConfiguration.collimator_model_error = false;
    if( ((STATUS.maj_code <3 ) && (generalConfiguration.revisioni.pcb249U1.model == _COLLI_TYPE_ASSY_02)) ||
         ((STATUS.maj_code >=3 ) && (generalConfiguration.revisioni.pcb249U1.model == _COLLI_TYPE_ASSY_01))
      ){
        generalConfiguration.collimator_model_error = true;
        printf("INVALID COLLIMATOR ASSEMBLY VS FIRMWARE DETECTED!\n");
    }

    // Segnalazione driver connesso
   _EVSET(_EV1_PCB249U1_CONNECTED);
    
    // Attende l'autorizzazione ad effetuare il refresh registri
    _EVWAIT_ANY(_EV1_UPDATE_REGISTERS);
        

    // Verifica inizializzazione Collimatore se necessario e se consentito
    if(!generalConfiguration.collimator_model_error){

        Ser422ReadRegister(_REGID(PR_L_OUT),10,&CONTEST);
        if(_DEVREGL(PR_L_OUT,CONTEST)==0) pcb249U1initCollimator();
   }

    // Upload contenuto registri
   for(i=0;i<PCB249U1_NREGISTERS;i++)
   {
      err_ret = Ser422ReadRegister(i,4,&CONTEST);
      if(err_ret!=_SER422_NO_ERROR)
      {        
         ERROR_HANDLER();
         break;
      }
   }
   
   // Attende la ricezione della configurazione se necessario
   _EVSET(_EV2_PCB249U1_STARTUP_OK);
   printf("PCB249U1: ATTENDE CONFIGURAZIONE..\n");

   _EVWAIT_ANY(_EV1_DEV_CONFIG_OK);
   printf("PCB249U1: CONFIGURAZIONE OK. INIZIO LAVORO\n");



   ////////////////////////////////////////////////////////////////////////
   /*
                  GESTIONE DEL CICLO DI LAVORO ORDINARIO
       Il driver effettua un polling sui principali registri di lettura
       iscritti nella lista di polling ed effettua un controllo periodico
       sui registri di scrittura per eventuali udates in caso di differenze 
   */
   /////////////////////////////////////////////////////////////////////////
   _EVCLR(_EV0_PCB249U1_COLLI);
   u1colli_result = false;


   while(1)
   {
     if(STATUS.freeze) enterFreezeMode();

     // Gestione collimazione 2D
     if(_IS_EVENT(_EV0_PCB249U1_COLLI)){

         u1colli_result = false;
         debugPrintI3("PCB249U1 GESTIONE COLLIMAZIONE LAME LATERALI -L",leftcolli_req,"R", rightcolli_req, "T", trapcolli_req);


         left = leftcolli_req;
         right = rightcolli_req;
         trap = trapcolli_req;

         // Impostazioni target lame
         if((Ser422WriteRegister(_REGID(RG249U1_PR_2D_L_USER),left,10,&PCB249U1_CONTEST) != _SER422_NO_ERROR) ||
           (Ser422WriteRegister(_REGID(RG249U1_PR_2D_R_USER),right,10,&PCB249U1_CONTEST) != _SER422_NO_ERROR)||
           (Ser422WriteRegister(_REGID(RG249U1_PR_2D_B_USER),trap,10,&PCB249U1_CONTEST) != _SER422_NO_ERROR)){
             debugPrint("PCB249U1 ERRORE SCRITTURA REGISTRI TARGET COLLI LEFT RIGHT TRAP");
             data[0]=0;
             data[1]=1; // Collimazione lame laterali
             data[2]=0;
             data[3]=0;
             data[4]=0;
             mccGuiNotify(_COLLI_ID,MCC_SET_COLLI,data,5);
             _EVCLR(_EV0_PCB249U1_COLLI);
             continue;
         }

         // Attesa READY dal dispositivo
         if(pcb249U1WaitBusy(80)==false){
             debugPrint("PCB249U1 TIMEOUT COLLI U1 IN ATTESA DEL READY");
             data[0]=0;
             data[1]=1; // Collimazione lame laterali
             data[2]=0;
             data[3]=0;
             data[4]=0;
             mccGuiNotify(_COLLI_ID,MCC_SET_COLLI,data,5);
             _EVCLR(_EV0_PCB249U1_COLLI);
             continue;
         }

         // Reset Faults
         if(_TEST_BIT(PCB249U1_FAULT))    pcb249U1ResetFaults();

         if(pcb249U1SetColliCmd(2)==false){
             debugPrint("PCB249U1 ERRORE COMANDO COLLIMAZIONE U1");
             data[0]=0;
             data[1]=1; // Collimazione lame laterali
             data[2]=0;
             data[3]=0;
             data[4]=0;
             mccGuiNotify(_COLLI_ID,MCC_SET_COLLI,data,5);
             _EVCLR(_EV0_PCB249U1_COLLI);
             continue;
         }

         _time_delay(50);

         // Attesa READY dal dispositivo
         if(pcb249U1WaitBusy(80)==false){
             debugPrint("PCB249U1 TIMEOUT POSIZIONAMENTO LAME U1");
             data[0]=0;
             data[1]=1; // Collimazione lame laterali
             data[2]=0;
             data[3]=0;
             data[4]=0;
             mccGuiNotify(_COLLI_ID,MCC_SET_COLLI,data,5);
             _EVCLR(_EV0_PCB249U1_COLLI);
             continue;
         }

         // Risultato del posizionamento
         if(_TEST_BIT(PCB249U1_FAULT)) {
             debugPrint("PCB249U1 ERRORE POSIZIONAMENTO LAME U1");
             data[0]=0;
             data[1]=1; // Collimazione lame laterali
             data[2]=0;
             data[3]=0;
             data[4]=0;
             mccGuiNotify(_COLLI_ID,MCC_SET_COLLI,data,5);
             _EVCLR(_EV0_PCB249U1_COLLI);
             continue;
         }

         // Operazione conclusa con successo
         // Se la richiesta � ancora uguale a quanto comandato
         // allora vuol dire che non ci sono altri comandi oppure che
         // il nuovo comando � uguale allo stato attuale
         if((left==leftcolli_req)&&(right==rightcolli_req)&&(trap==trapcolli_req)){
             _EVCLR(_EV0_PCB249U1_COLLI);
             u1colli_result = true;
             debugPrintI3("PCB249U1 COLLIMAZIONE U1 CONCLUSA - L",left, "R", right,"T",trap);
             data[0]=1; // Conclusa con successo
             data[1]=1; // Collimazione lame laterali
             data[2]=left;
             data[3]=right;
             data[4]=trap;
             mccGuiNotify(_COLLI_ID,MCC_SET_COLLI,data,5);
         }

         continue;
     }
     // Legge i registri di Base che devono sempre essere aggiornati
     if(pcb249U1UpdateRegisters()==FALSE)  ERROR_HANDLER();
      
     // Aggiornamento registri passati dall'applicazione: in caso di BUSY ripete 
     // La scrittura a meno che non venga annullata dall'applicazione
     _mutex_lock(&(CONTEST.reglist_mutex));
     if(STATUS.updconf)
     {
       // Passa tutta la lista di scrittura
       write_ok=TRUE;
       for(i=0; i<CONTEST.ConfigurationRegistersList.nitem; i++)
       {
          switch(Ser422WriteRegister(CONTEST.ConfigurationRegistersList.id[i],CONTEST.ConfigurationRegistersList.val[i],4,&CONTEST))
          {
          case _SER422_NO_ERROR: // Tutto oK
          break;
          case _SER422_TARGET_BUSY: // Interrompe il ciclo for
             i=CONTEST.ConfigurationRegistersList.nitem;
             write_ok=FALSE;
          break;
          default: // Errore 
              // Set the error condition and restart blocked the task
              ERROR_HANDLER();
          break;
          } 
       } // for
       
       if(write_ok==TRUE)
       {
        STATUS.updconf = 0;
        debugPrint("PCB249U1 CONFIG UPDATED!");
        
        // Invia segnale di aggiornamento cfg      
        _EVSET(_EV0_PCB249U1_CFG_UPD);
       }
     }
     _mutex_unlock(&(CONTEST.reglist_mutex));
     
     // Termine della routine di driver
      STATUS.ready=1;
      _EVSET(_EV1_PCB249U1_RUN);
     _time_delay(_DEF_PCB249U1_DRIVER_DELAY);
   }
}

//////////////////////////////////////////////////////////////////////////////
/*
_PCB249U1_Error_Enum GetFwRevision(void)
        La funzione legge il codice di revisione del firmware del 
        target.

PARAM:
        -
RETURN:
      TRUE: Lettura avvenuta con successo
      FALSE: Problema di comunicazione con il target

      PCB249U1_Stat.maj_code/PCB249U1_Stat.min_code = codice revisione
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
  frame.data1=_CMD1(PCB249U1_GET_REV);
  frame.data2=_CMD2(PCB249U1_GET_REV);
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  
  // Eventuali errori di comunicazione
  if(frame.retcode!=SER422_DATA) return FALSE;

  STATUS.maj_code = frame.data2;
  STATUS.min_code = frame.data1;
  generalConfiguration.revisioni.pcb249U1.maj = STATUS.maj_code; 
  generalConfiguration.revisioni.pcb249U1.min = STATUS.min_code; 

  
  return TRUE;

   
}

void ERROR_HANDLER(void)
{
   // Segnalazione driver disconnesso
   _EVCLR(_EV1_PCB249U1_CONNECTED);

   // Riconfigurazione del driver a seguito della ripartenza
   printf("PCB249U1 ERRORE: ATTESA RICONNESSIONE E RICONFIGURAZIONE REGISTRI\n"); 
  
   while(1){
    
    _time_delay(100);
    
    // Richiesta revisione firmware a target
    while(GetFwRevision()==FALSE) _time_delay(100);
    printf("PCB249U1:REVISIONE FW TARGET:%d.%d\n",STATUS.maj_code,STATUS.min_code);     

    // Carica sulla periferica lo stato dei registri cosi come erano prima del reset
    printf("PCB249U1: DOWNLOAD REGISTRI ...\n");
    if(Ser422UploadRegisters(10, &CONTEST)== FALSE)   continue;  
   
    // Carica Tutti i registri RD / RW
    int i;
    for(i=0;i<PCB249U1_NREGISTERS;i++) 
    {
      if(Ser422ReadRegister(i,4,&CONTEST)!=_SER422_NO_ERROR) break; 
    }
    if(i!=PCB249U1_NREGISTERS) continue;  
    break;
  }
  
  // Provoca l'inizializzazione delle variabili di cambio stato
  verifyUpdateRegisters = TRUE;   

  // Segnalazione driver connesso
  _EVSET(_EV1_PCB249U1_CONNECTED);
  
  // Ripartenza completata. Pu� tornare da dove aveva lasciato
  printf("PCB249U1 RIPARTITA CORRETTAMENTE\n"); 

  return;
}

// MOD_ACCESSORIO
/*
  Da schema elettrico
  0	        V0
  63,75	        V1      (Pb)
  95,625	V2
  127,5	        V3      (Cal)
  159,375	V4      (3D)
  191,25	V5      (2D)
  223,125	V6
  255	        V7
 
SOGLIE:
    31,875
    79,6875
    111,5625
    143,4375
    175,3125
    207,1875
    239,0625

Codici accessori: 
- Piombo:               V1
- Fantoccio Calib:      V3
- Fantoccio Prot 2D:    V5
- Fantoccio 3D eco      V4
*/

unsigned char getAccessorioCodice(unsigned char* raw, unsigned char* codice){
    unsigned char accessorio;

    Ser422ReadRegister(_REGID(RG249U1_RG_PROT),10,&CONTEST);
    *raw = _DEVREGL(RG249U1_RG_PROT,CONTEST);

    // Identificazione del codice
    if(*raw<31) *codice = 0;
    else if(*raw<80) *codice = 1;
    else if(*raw<112) *codice = 2;
    else if(*raw<143) *codice = 3;
    else if(*raw<175) *codice = 4;
    else if(*raw<207) *codice = 5;
    else if(*raw<239) *codice = 6;
    else *codice = 7;

    switch(*codice){
    case 7: accessorio = COLLI_ACCESSORIO_ND; break;
    case 1: accessorio = COLLI_ACCESSORIO_PIOMBO; break;
    case 3: accessorio = COLLI_ACCESSORIO_CALIB_PLEXYGLASS; break;
    case 4: accessorio = COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D; break;
    case 5: accessorio = COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_2D; break;
    case 6: accessorio = COLLI_ACCESSORIO_FRUSTOLI; break;
    default: accessorio = COLLI_ACCESSORIO_FAULT;
    }

    return accessorio;
}

bool identificazioneAccessorio(void)
{
  static unsigned char accessorio=255;
  unsigned char codice,raw;

  accessorio = getAccessorioCodice(&raw, &codice);
  if(generalConfiguration.colliCfg.codiceAccessorio == accessorio) return false;

  // Se cambiato, ripete la lettura per evitare rimbalzi
  _time_delay(500);
  accessorio = getAccessorioCodice(&raw, &codice);
  if(generalConfiguration.colliCfg.codiceAccessorio == accessorio) return false;

  // ASsegna il cambiamento
  generalConfiguration.colliCfg.codiceAccessorio = accessorio;

    switch(accessorio){
    case COLLI_ACCESSORIO_ND: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <NON DEFINITO> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    case COLLI_ACCESSORIO_FRUSTOLI: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <FRUSTOLI> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    case COLLI_ACCESSORIO_CALIB_PLEXYGLASS: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <PLEXYGLASS> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    case COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <PROT 3D> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    case COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_2D: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <PROT 2D> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    case COLLI_ACCESSORIO_PIOMBO: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <PIOMBO> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    default: debugPrintI3("PCB249U1 RILEVATO CAMBIO ACCESSORIO <FAULT> - ACC",accessorio,"Codice",codice,"RAW",raw);break;
    }

    return true;
}

// Effettua l'update dei registri di sistema
bool pcb249U1UpdateRegisters(void)
{
  unsigned char data[3];
  static int back_temperatura = 25;
  
  static bool update = true;
  static int polling_timer = 20;
  static int temp_timer = 0;
  
  // Restart driver
  if(verifyUpdateRegisters)
  {
    verifyUpdateRegisters = FALSE;
    back_temperatura = 25;
    update=true;
  }

   // Richiede lo stato dei targets
   if(Ser422ReadRegister(_REGID(RG249U1_SYS_FLAGS0),10,&CONTEST)!=_SER422_NO_ERROR) return FALSE;
   
   // Attiva gli eventi relativi ai flags di interfaccia
   if ((_DEVREGL(RG249U1_SYS_FLAGS0,CONTEST)&0x0F)>0) _EVSET(_EV1_PCB249U1_TARGET);
   if (_TEST_BIT(PCB249U1_BUSY)) _EVSET(_EV1_PCB249U1_BUSY);
   else _EVSET(_EV1_PCB249U1_READY);

   // Richiede il codice accessorio   
   if(identificazioneAccessorio()==TRUE) update=TRUE;
   

   // Gestione temperatura diradata nel tempo

   // Richiede la temperatura
   if(temp_timer == 0){

       if(Ser422ReadRegister(_REGID(RG249U1_RG_TEMP),10,&CONTEST)==_SER422_NO_ERROR){


            float current_temperature;
            if(generalConfiguration.airTubeModel){
                 // Gestione della temperatura per il tubo ad aria
                  unsigned char val = _DEVREGL(RG249U1_RG_TEMP,CONTEST);
                  if(val < 118) current_temperature = 70;
                  else if(val>242) current_temperature = 0;
                  else{
                      val-=118;
                      current_temperature = (float) airTemperature[val];
                  }

            }else{
                 // Gestione temperatura per tubo ad Olio
                 unsigned char val = _DEVREGL(RG249U1_RG_TEMP,CONTEST);
                 if(val <= 140) current_temperature = 0;
                 else if(val >= 190) current_temperature = 70;
                 else{
                     current_temperature = (((float) _DEVREGL(RG249U1_RG_TEMP,CONTEST) * 500.0 / 255.0) - 273.0);
                 }
            }

            if(back_temperatura != (int) current_temperature)
            {
                back_temperatura = (int) current_temperature;
                debugPrintI("PCB249U1 TEMPERATURA TUBO", back_temperatura);
                update=TRUE;
            }

            // Se la temperatura inizia a salire meglio controllare pi� frequentemente
            if(back_temperatura < 40) temp_timer = 20;
            else temp_timer = 5;

       }

   }else temp_timer--;


   // Legge il contenuto dell'inclinometro di bordo ma solo se non ci sono movimenti del tubo in corso
   if((generalConfiguration.trxExecution.run == false) ){
       if(Ser422Read16BitRegister(_REGID(RG249U1_GONIO16_TRX),4,&CONTEST) == _SER422_NO_ERROR){
           // Trasforma da 0.025� a 0.1�
           generalConfiguration.armExecution.dAngolo_inclinometro = (short) _DEVREG(RG249U1_GONIO16_TRX,CONTEST) / 4;
          // printf("ANGOLO INCLINOMETRO=%d\n", generalConfiguration.armExecution.dAngolo_inclinometro);
           actuatorsUpdateAngles(); // Richiede di aggiornare l'insieme degli angoli
       }
   }


   // Aggiornamento comunque a polling
   if((update)||(--polling_timer == 0))
   {
     data[0] = _DEVREGL(RG249U1_SYS_FLAGS0,CONTEST);
     data[1] = back_temperatura;
     data[2] = generalConfiguration.colliCfg.codiceAccessorio;     
     update = ! mccPCB249U1Notify(1,PCB249U1_NOTIFY_DATA,data,sizeof(data));
     polling_timer = 20;
   }
     

    return TRUE;
}
 
//_________________________________________________________________________________________________
//                               FUNZIONI DI INTERFACCIA

//////////////////////////////////////////////////////////////////////////////
/*
bool pcb249U1SetColliMode(unsigned char mode)
  La funzione imposta il target corrente per le lame di collimazione.
  
PARAM:
  - <mode>:
    0 = Target 2D, 24x30
    1 = Target 2D, 18x24
    2 = Target 2D, user
    3 = Target 2D, Tomo (inseguimento)

RETURN:
      TRUE: Comando eseguito con successo.
      FALSE: 
        STATUS.fault=1;
        STATUS.error = _SER422_TARGET_BUSY ---- La condizione � BUSY
        STATUS.error = _SER422_COMMUNICATION_ERROR ---- Problema sulla comunicazione

Autore: M. Rispoli
Data: 09/01/2015
*/
//////////////////////////////////////////////////////////////////////////////
bool pcb249U1SetColliCmd(unsigned char mode)
{
  if(generalConfiguration.collimator_model_error) return false;

  _Ser422_Command_Str frame;

  
  // Prepara il comando di sblocco
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;

  switch(mode)
  {
  case 0: // Modo 2D 24x30
      debugPrint("PCB249U1 COLLIMATORE IN MODALITA 24x30");
      frame.data1=_CMD1(PCB249U1_SET_COLLI_24x30);
      frame.data2=_CMD2(PCB249U1_SET_COLLI_24x30);
    break;
  case 1: // Modo 2D 18x24
      debugPrint("PCB249U1 COLLIMATORE IN MODALITA 18x24");
      frame.data1=_CMD1(PCB249U1_SET_COLLI_18x24);
      frame.data2=_CMD2(PCB249U1_SET_COLLI_18x24);
    break;
  case 2: // Modo 2D USER      
      debugPrint("PCB249U1 COLLIMATORE IN MODALITA USER");
      frame.data1=_CMD1(PCB249U1_SET_COLLI_USER);
      frame.data2=_CMD2(PCB249U1_SET_COLLI_USER);
    break;
  case 3: // Modo Tomografia con inseguimento su formato 24x30
      debugPrint("PCB249U1 COLLIMATORE IN MODALITA INSEGUIMENTO");
      frame.data1=_CMD1(PCB249U1_SET_COLLI_TOMO);
      frame.data2=_CMD2(PCB249U1_SET_COLLI_TOMO);
    break;
  }

  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);

  if(frame.retcode==SER422_COMMAND_OK) return TRUE;
  else return FALSE;

}


//////////////////////////////////////////////////////////////////////////////
/*
bool pcb249U1ResetGonio(unsigned short angolo)
  La funzione resetta il Gonio al valore angolo espresso in 0.025 �/unit
  
PARAM:
  - <angolo>: 0.025�/unit
    
RETURN:
      TRUE: Comando eseguito con successo.
      FALSE: 
        STATUS.fault=1;
        STATUS.error = _SER422_TARGET_BUSY ---- La condizione � BUSY
        STATUS.error = _SER422_COMMUNICATION_ERROR ---- Problema sulla comunicazione

Autore: M. Rispoli
Data: 09/01/2015
*/
//////////////////////////////////////////////////////////////////////////////
bool pcb249U1ResetGonio(unsigned short angolo)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Scrittura dei registri contenenti il valore di azzeramento
  if(Ser422WriteRegister(_REGID(RG249U1_GONIO_OFS),angolo,10,&CONTEST)!=_SER422_NO_ERROR)
  {   
    STATUS.fault=1;
    STATUS.error=_SER422_COMMUNICATION_ERROR;
    return FALSE;
  } 
  
  // Invio comando di reset inclinometro
  
  // Prepara il comando di sblocco
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;

  // Comando reset Gonio
  frame.data1=_CMD1(PCB249U1_RESET_GONIO);
  frame.data2=_CMD2(PCB249U1_RESET_GONIO);

  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  switch(frame.retcode)
  {
  case SER422_COMMAND_OK: return TRUE;  //Comando eseguito con successo
  case SER422_BUSY:
    STATUS.fault=1;
    STATUS.error=_SER422_TARGET_BUSY;
    return FALSE;
    break;
  default:
    STATUS.fault=1;
    STATUS.error=_SER422_COMMUNICATION_ERROR;
    return FALSE;
    break;
  }

}



bool pcb249U1ResetFaults(void)
{
   _Ser422_Command_Str frame;
  
  // Prepara il comando di download
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;

  frame.data1=_CMD1(PCB249U1_CLEAR_ERRORS);
  frame.data2=_CMD2(PCB249U1_CLEAR_ERRORS);
    
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  
  if(frame.retcode == SER422_COMMAND_OK) return TRUE;  
  return FALSE;  

}


// Imposta una richiesta di collimazione lame laterali.
// L'ultima richiesta che arriva sovrascrive quella in attesa precedente
void pcb249U1SetColli(unsigned char left, unsigned char right, unsigned char trap)
{
    if(generalConfiguration.collimator_model_error) return;

    leftcolli_req = left;
    rightcolli_req = right;
    trapcolli_req=trap;
    u1colli_result = false;
    _EVSET(_EV0_PCB249U1_COLLI);
    return;

}

// Attiva la procedura di ricerca degli zeri per il collimatore
bool pcb249U1initCollimator(void)
{
   if(generalConfiguration.collimator_model_error) return false;

   _Ser422_Command_Str frame;
  
   debugPrint("PCB249U1 PROCEDURA DI AZZERAMENTO AUTOMATICA COLLIMATORE");
   
  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando di download
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;

  frame.data1=_CMD1(PCB249U1_TEACH_ZERO);
  frame.data2=_CMD2(PCB249U1_TEACH_ZERO);
    
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  if(frame.retcode != SER422_COMMAND_OK) return FALSE;
  _time_delay(10000);
  
   // Attende il termine della procedura (BUSY==0)   
   int i=20;
   while(i--)
   {
     Ser422ReadRegister(_REGID(RG249U1_SYS_FLAGS0),10,&CONTEST);
     if(_DEVREGL(RG249U1_SYS_FLAGS0,CONTEST)&0x40) _time_delay(1000);
     else return TRUE;
   }
   
   debugPrint("PCB249U1 PROCEDURA DI AZZERAMENTO AUTOMATICA COLLIMATORE FALLITA!!");
   return FALSE;
   
}

void colliArrayPrint(void){
  int i;

#ifndef PRINTCFG
  return ;
#endif


  debugPrint("PCB249U1 CONFIGURAZIONE COLLIMATORE DINAMICO");
  
  printf("\nLEFT-P: ");
  for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) printf("%d ",generalConfiguration.colliCfg.dynamicArray.tomoLeftBladeP[i]);
  printf("\nLEFT-N: ");
  for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) printf("%d ",generalConfiguration.colliCfg.dynamicArray.tomoLeftBladeN[i]);
    
  printf("\nRIGHT-P: ");
  for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) printf("%d ",generalConfiguration.colliCfg.dynamicArray.tomoRightBladeP[i]);
  printf("\nRIGHT-N: ");
  for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) printf("%d ",generalConfiguration.colliCfg.dynamicArray.tomoRightBladeN[i]);

  printf("\nTRAP-P: ");
  for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) printf("%d ",generalConfiguration.colliCfg.dynamicArray.tomoBackTrapP[i]);
  printf("\nTRAP-N: ");
  for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) printf("%d ",generalConfiguration.colliCfg.dynamicArray.tomoBackTrapN[i]);
  
  printf("\n FRONT:%d BACK:%d: \n", generalConfiguration.colliCfg.dynamicArray.tomoFront, generalConfiguration.colliCfg.dynamicArray.tomoBack);
}
/*
  Funzione configuratrice
*/
bool config_pcb249U1(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len){
  switch(blocco){
  case 0:
    memcpy(&generalConfiguration.colliCfg.dynamicArray.tomoLeftBladeP, buffer, COLLI_DYNAMIC_SAMPLES);
    break;
  case 1:
    memcpy(&generalConfiguration.colliCfg.dynamicArray.tomoLeftBladeN, buffer, COLLI_DYNAMIC_SAMPLES);
    break;
  case 2:
    memcpy(&generalConfiguration.colliCfg.dynamicArray.tomoRightBladeP, buffer, COLLI_DYNAMIC_SAMPLES);
    break;
  case 3:
    memcpy(&generalConfiguration.colliCfg.dynamicArray.tomoRightBladeN, buffer, COLLI_DYNAMIC_SAMPLES);
    break;
  case 4:
    memcpy(&generalConfiguration.colliCfg.dynamicArray.tomoBackTrapP, buffer, COLLI_DYNAMIC_SAMPLES);
    break;
  case 5:
    memcpy(&generalConfiguration.colliCfg.dynamicArray.tomoBackTrapN, buffer, COLLI_DYNAMIC_SAMPLES);
    break;
  case 6:
    generalConfiguration.colliCfg.dynamicArray.tomoFront=buffer[0];
    generalConfiguration.colliCfg.dynamicArray.tomoBack=buffer[1];

    // Allarme temperatura cuffia disabilitato nel device per non andare in conflitto con la gestione del tubo ad Aria

    //generalConfiguration.colliCfg.tempcuffia_on = ((unsigned char)  ((float)buffer[2] * 0.51 + 139));
    //generalConfiguration.colliCfg.tempcuffia_off = ((unsigned char)  ((float)buffer[3] * 0.51 + 139));

    // Stampa il contenuto della collimazione dinamica
    colliArrayPrint();
    setColliArray(); // Aggiornamento periferica..
    break;
    
  }
  return true;
}

void pcb249U1SetTubeTemperatureThresholds(unsigned char alarm_on, unsigned char alarm_off, unsigned char fan_on, unsigned char fan_off){
    // Scrittura limiti di temperatura cuffia
    Ser422WriteRegister(_REGID(RG249U1_PR_TEMP_ALR),alarm_on, 10,&CONTEST);
    Ser422WriteRegister(_REGID(RG249U1_PR_TEMP_ALR_OFF),alarm_off, 10,&CONTEST);

    // Scrittura soglie di accensione ventola
    Ser422WriteRegister(_REGID(RG249U1_PR_TEMP_L),fan_off, 10,&CONTEST);
    Ser422WriteRegister(_REGID(RG249U1_PR_TEMP_H),fan_on, 10,&CONTEST);

}

void enterFreezeMode(void){
    // Entra in Freeze
    printf("PB249U1 ENTRA IN FREEZE\n");
    _EVCLR(_EV1_PCB249U1_RUN);
    _EVSET(_EV1_PCB249U1_FREEZED); // Notifica l'avvenuto Blocco
    _EVWAIT_ANY(_MOR2(_EV1_DEVICES_RUN,_EV1_PCB249U1_RUN)); // Attende lo sblocco
    printf("PB249U1 ESCE DAL FREEZE\n");
    STATUS.freeze = 0;
}

bool pcb249U1SetWriteMode01(void)
{
   _Ser422_Command_Str frame;
  
  // Prepara il comando di download
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;

  frame.data1=_CMD1(PCB249U1_WRITE_B01);
  frame.data2=_CMD2(PCB249U1_WRITE_B01);
    
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID); 
  if(frame.retcode != SER422_COMMAND_OK) return FALSE;
  return true;   
}
bool pcb249U1SetWriteMode23(void)
{
   _Ser422_Command_Str frame;
  
  // Prepara il comando di download
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;

  frame.data1=_CMD1(PCB249U1_WRITE_B23);
  frame.data2=_CMD2(PCB249U1_WRITE_B23);
    
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID); 
  if(frame.retcode != SER422_COMMAND_OK) return FALSE;
  return true;   
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//      COLLIMAZIONE DINAMEICA
#define _RAD(x) ((float) x * 3.141593 / 180.0)
#define APPROX(x) ((((x)-floor(x))>0.5) ? ceil(x): floor(x))

#define FC 613      // Distanza fuoco-centro di rotazione (mm)
#define CD 45       // Distanza centro di rotazione - piano del detector (mm)
#define LF 150      // Distanza Lame-Fuoco (mm)
#define rC 0.109    // 32/256 Risoluzione spostamento lame (mm/unit)

#define DISPERSIONE -3
#define NREF        80
#define U           1.2737 // atan((150+dispersione)/CD)
#define A           153.7335   // sqrt((150+dispersione)*(150+dispersione)+CD*CD);
#define A0          33.5106     // LF * (sin(teta) / (FC/A + cos(teta)));

#define NTARGET_R   74      // nRef - LF * (dispersione+footprint) / (rC * (FC + CD)); // Target N per apertura a 0� per avere le lame alla dispersione voluta
#define NTARGET_L   68      // nRef - LF * (dispersione+footprint) / (rC * (FC + CD)); // Target N per apertura a 0� per avere le lame alla dispersione voluta

unsigned char getDynamicColliTargetL(float angolo){
  int i;  
  float teta= -1* _RAD(angolo) + U;
 
  return (unsigned char) APPROX(NTARGET_L - (LF * (sin(teta) / (FC/A + cos(teta))) - A0) / rC);

}

unsigned char getDynamicColliTargetR(float angolo){
  int i;  
  float teta= _RAD(angolo) + U;
 
  return (unsigned char) APPROX(NTARGET_R - (LF * (sin(teta) / (FC/A + cos(teta))) - A0) / rC);

}

bool setColliArray(void){
  unsigned char array_ptr;
  int ii;
  unsigned char dato;

    
  // ----------- Array lama L -------------------------------------------------------
  if(N_ARRAY_L > 0xFF) {
    if(!pcb249U1SetWriteMode23()) goto fallito;        
  }else
  {
    if(!pcb249U1SetWriteMode01()) goto fallito;                
  }
  for(array_ptr = (N_ARRAY_L&0xFF),ii=0;ii<COLLI_DYNAMIC_SAMPLES;ii++,array_ptr++){
    dato = generalConfiguration.colliCfg.dynamicArray.tomoLeftBladeN[ii];
    if(!Ser422WriteRaw(TARGET_ADDRESS, array_ptr, dato, 10)) goto fallito;
   }
  
  for(array_ptr = (P_ARRAY_L&0xFF),ii=0;ii<COLLI_DYNAMIC_SAMPLES;ii++,array_ptr++){
    dato = generalConfiguration.colliCfg.dynamicArray.tomoLeftBladeP[ii];
    if(!Ser422WriteRaw(TARGET_ADDRESS, array_ptr, dato, 10)) goto fallito;        
  }

  // ----------- Array lama R -------------------------------------------------------
  if(N_ARRAY_R > 0xFF) {
    if(!pcb249U1SetWriteMode23()) goto fallito;                
  }else
  {
    if(!pcb249U1SetWriteMode01()) goto fallito;                
  }
  
  for(array_ptr = (N_ARRAY_R&0xFF),ii=0;ii<COLLI_DYNAMIC_SAMPLES;ii++,array_ptr++){
    dato = generalConfiguration.colliCfg.dynamicArray.tomoRightBladeN[ii];
    if(!Ser422WriteRaw(TARGET_ADDRESS, array_ptr, dato, 10)) goto fallito;        
  }
  for(array_ptr = (P_ARRAY_R&0xFF),ii=0;ii<COLLI_DYNAMIC_SAMPLES;ii++,array_ptr++){
    dato = generalConfiguration.colliCfg.dynamicArray.tomoRightBladeP[ii];
    if(!Ser422WriteRaw(TARGET_ADDRESS, array_ptr, dato, 10)) goto fallito;        
  }

  // ----------- Array lama T -------------------------------------------------------
  if(N_ARRAY_B > 0xFF) {
    if(!pcb249U1SetWriteMode23()) goto fallito;                
  }else
  {
    if(!pcb249U1SetWriteMode01()) goto fallito;                
  }
  
  for(array_ptr = (N_ARRAY_B&0xFF),ii=0;ii<COLLI_DYNAMIC_SAMPLES;ii++,array_ptr++){
    dato = generalConfiguration.colliCfg.dynamicArray.tomoBackTrapN[ii];
    if(!Ser422WriteRaw(TARGET_ADDRESS, array_ptr, dato, 10)) goto fallito;            
  }
  for(array_ptr = (P_ARRAY_B&0xFF),ii=0;ii<COLLI_DYNAMIC_SAMPLES;ii++,array_ptr++){
    dato = generalConfiguration.colliCfg.dynamicArray.tomoBackTrapP[ii];
    if(!Ser422WriteRaw(TARGET_ADDRESS, array_ptr, dato, 10)) goto fallito;            
  }


  debugPrint("PCB249U1 PCB249U1 AGGIORNATA");
  pcb249U1SetWriteMode01();
  return true;
  
fallito:
  debugPrint("PCB249U1 AGGIORNAMENTO FALLITO");
  pcb249U1SetWriteMode01();
  return false;
}


void pcb249U1_readGonio(void){

    // Legge il contenuto dell'inclinometro di bordo ma solo se non ci sono movimenti in corso
    if(Ser422Read16BitRegister(_REGID(RG249U1_GONIO16_TRX),10,&CONTEST) == _SER422_NO_ERROR){
        // Trasforma da 0.025� a 0.1�
        generalConfiguration.armExecution.dAngolo_inclinometro = (short) _DEVREG(RG249U1_GONIO16_TRX,CONTEST) / 4;
        actuatorsUpdateAngles(); // Richiede di aggiornare l'insieme degli angoli
    }

}

// Attende il READY con timeout di 100ms unit
bool pcb249U1WaitBusy(int timeout){
    while(timeout){
        if(Ser422ReadRegister(_REGID(RG249U1_SYS_FLAGS0),10,&CONTEST)!=_SER422_NO_ERROR) {
            _time_delay(100);
            timeout--;
            continue;
        };

        if(!_TEST_BIT(PCB249U1_BUSY)) return true;
        timeout--;
        _time_delay(100);
    }

    return false;
}

// Funzione usata dalle procedure raggi per accertarsi della
// corretta collimazione
bool wait2DLeftRightTrapCompletion(int timeout){

    int tmo = timeout;

    // Attende il completamento di una collimazione pendente
    while(_IS_EVENT(_EV0_PCB249U1_COLLI)){
        _time_delay(100);
        tmo--;
        if(!tmo){
            debugPrint("PCB249U1 TIMEOUT ATTESA COMPLETAMENTO COLLIMAZIONE LEFT+RIHT DURANTE RAGGI");
            return false;
        }
    }

    // Se il comando � fallito, riprova a collimare
    if(!u1colli_result){
        pcb249U1SetColli(leftcolli_req ,rightcolli_req,trapcolli_req); // ripete il comando
        _time_delay(50);
        tmo = timeout;
        while(_IS_EVENT(_EV0_PCB249U1_COLLI)){
            _time_delay(100);
            tmo--;
            if(!tmo){
                debugPrint("PCB249U1 TIMEOUT ATTESA COMPLETAMENTO COLLIMAZIONE LEFT+RIGHT DURANTE RAGGI");
                return false;
            }
        }
    }

    // Se infine non � riuscito allora si ferma qui
    if(!backfront_eseguito) return false;

    // Controllo completato con successo
    return true;
}

bool pcb249U1_GetFreeze(void){
    return STATUS.freeze;
}

/* EOF */
 
  
 
