#define _PCB190_C
#include "dbt_m4.h" 

#define TARGET_ADDRESS 0x13
#define _DEF_PCB190_DRIVER_DELAY 100

#define CONTEST PCB190_CONTEST
#define STATUS  (*((_PCB190_Stat_Str*)(&PCB190_CONTEST.Stat)))
#define ERROR_HANDLER   pcb190DriverCommunicationError

// Funzione di servizio interne al modulo
static bool GetFwRevision(void);
static void enterFreezeMode(void);
static void ERROR_HANDLER(void);
static bool starterStop=TRUE;
static bool updateStarterRegisterFlag=FALSE;

static int ls_starter_test=0;

// Funzioni locali 
void pcb190PrintParametri();
//////////////////////////////////////////////////////////////////////////////
/*
void pcb190_driver(uint32_t taskRegisters)

        La funzione � un task indipendente e si occupa di gestire
        la comunicazione con la PCB190 e il suo funzionamento.

        Il task effettua una prima fase di startup ed una seconda fase 
        di Loop. Le due fasi sono di seguito descritte:
  
        STARTUP:
            - Vengono inizializzate le variabili interne e di interfaccia;
            - Viene inviato il comando di seriale di Reset Faults;
              se il Target non � attivo/connesso e quindi non risponde
              il task riprova ad inviare il comando ogni 100ms;
            - Viene inviato il comando di richiesta codice firmware;
            - Vengono letti tutti i registri del Target configurati
              nella struttura PCB190_Registers.
            - Viene controllata (opzionale) la lista dei registri scrivibili
              passata al driver per la pre-impostazione sul Target. Se la lista
              � corretta, il driver tenta di scrivere sul Target il contenuto
              di tale lista;
        
        LOOP DI LAVORO:
            - (1)Vengono letti i registri di stato principali: 
              SYS_FLAGS0 . Viene quindi controllata un'eventuale 
              condizione di fault del Target e in caso di fault viene richiesto 
              al target il codice di fault;
            - Viene gestita la lista locPollingList che contiene 
              la lista di registri, passata/aggiornata dall'applicazione,
              che devono essere tenuti aggiornati. L'applicazione pu� in
              ogni momento modificare tale lista.
            - Viene gestita la lista locRegistersList che contiene 
              i registri da scrivere. L'applicazione tenter� la riscrittura
              di tale lista fino a successo completo. Solo allora attiver�
              un flag nel registro di stato PCB190_Stat.updated == 1
            - L'applicazione in ogni momento potr� utilizzare alcune funzioni
              di seguito descritte che invieranno comandi particolari
              al Target. In tale situazione il driver si sospender� 
              subito dopo la lettura dei registri di base (1) fino al termie 
              della sequenza di comando in corso.

      API MODULO ---------------------------------------------------------

      - CREAZIONE DI UNA LISTA DI REGISTRI DA POLLARE:
        L'applicazione pu� creare una lista di registri che il driver deve 
        tenere aggiornata. Per creare la lista occorre:
      - Dichiarare una lista di tipo _DeviceAppRegister_Str:
                    _DeviceAppRegister_Str lista;
      - Utilizzare le macro di riempimento della lista:
        _LISTCREATE(<id-registro>,<lista>,<valore 16bit registro>) 
        per il primo elemento della lista;
        _ADDITEMLIST(<id-registro>,<lista>,<valore 16bit registro>) 
        per gli elementi successivi della lista;
      - Formattare la lista con la funzione:
        bool pcb190FormatRegListCrc(&lista); 
      - Attivare la lista cosi' creata:
        bool pcb190SetPollingList(&lista) ; 

      - MODIFICA DI UNA LISTA ATTIVA DI REGISTRI DA POLLARE:
      Una lista attiva pu� essere modficata dall'applicazione, 
      aggiungendo o togliendo registri alla stessa senza dover ricreare
      una nuova lista. 
      - Per aggiungere un registro alla lista:
        bool pcb190AddRegToPollingList(unsigned char id);
      - Per rimuovere un registro dalla lista:
        bool pcb190DelRegToPollingList(unsigned char id);

      - CREAZIONE DI UNA LISTA DI REGISTRI DA MODIFICARE:
        L'applicazione pu� creare una lista di registri che il driver deve 
        aggiornare. Il driver effettua una copia interna di tale lista
        che cancella non appena le modifiche sono state trasmesse al Target.
        La lista originale dell'Applicazione tuttavia non viene distrutta
        e pu� essere riutilizzata in qualsiasi momento.
        Il driver NON invia comandi di scrittura al Target per registri
        il cui valore contenuto nella lista non sia variato rispetto 
        all'ultima scrittura. Ovviamente il meccanismo di scrittura tramite
        lista deve essere applicato ai soli registri che non siano scrivibili
        da altre fonti.
        Per poter gestire registri di tale natura occorrer� impostare la
        caratteristica di _VOLATILE al registro stesso. In tal caso 
        il driver forzer� la scrittura dello stesso senza controllare il valore 
        locale. Tale flag dovr� essere applicato ai soli registri scrivibili
        da altre fonti per ottimizzare i tempi di accesso alla seriale.

      - dichiarare una lista di tipo _DeviceAppRegister_Str:
        _DeviceAppRegister_Str lista;
      - utilizzare le macro di riempimento della lista:
        _LISTCREATE(<id-registro>,<lista>,<valore 16bit registro>) 
        per il primo elemento della lista;
        _ADDITEMLIST(<id-registro>,<lista>,<valore 16bit registro>) 
        per gli elementi successivi della lista;
      - formattare la lista con la funzione:
        bool pcb190FormatRegListCrc(&lista); 
      - Attivare la lista cosi' creata:
        bool pcb190SetRegisterList(&lista) ; 
        Immediatamente dopo la chiamata alla funzione, 
        il bit di stato PCB190_Stat.updated ==  0. 
        Non appena il driver completer� la scrittura, tale flag
        verr� impostato a 1. 

  
       
      
      ERRORI / FAULTS


Autore: M. Rispoli
Data: 24/10/2014
*/
//////////////////////////////////////////////////////////////////////////////
void pcb190_driver(uint32_t taskRegisters)
{
  int i;
  _Ser422_Command_Str frame;
  _SER422_Error_Enum err_ret;
  bool write_ok;   
  

    // Costruzione del contesto
   CONTEST.pReg = PCB190_Registers;
   CONTEST.nregisters = PCB190_NREGISTERS;
   CONTEST.evm = _EVM(_EV0_PCB190_CFG_UPD);
   CONTEST.evr = &_EVR(_EV0_PCB190_CFG_UPD);
   CONTEST.address = TARGET_ADDRESS;
   printf("ATTIVAZIONE DRIVER PCB190: \n");
    
   //////////////////////////////////////////////////////////////////////////
   //                   FINE FASE DI INIZIALIZZAZIONE DRIVER               //             
   //        Inizia il ciclo di controllo e gestione della periferica      //
   //////////////////////////////////////////////////////////////////////////

    // In caso di errore di compilazione in questo punto 
    // significa errore in compilazione della struttura registri
    SIZE_CHECK((sizeof(PCB190_Registers)/sizeof(_DeviceRegItem_Str))!=PCB190_NREGISTERS);
      
    // Segnalazione driver disconnesso
    _EVCLR(_EV1_PCB190_CONNECTED);
    
    // Retrive Task ID
    CONTEST.ID =  _task_get_id();
    
    // Init registro di stato
    memset((void*)&(STATUS), 0,sizeof(_Device_Stat_Str ));

   // Inizializzazione delle mutex
    if (_mutex_init(&(CONTEST.reglist_mutex), NULL) != MQX_OK)
    {
      printf("PCB190: ERRORE INIT MUTEX. FINE PROGRAMMA");
      _mqx_exit(-1);
    }

    if (_mutex_init(&(CONTEST.pollinglist_mutex), NULL) != MQX_OK)
    {
      printf("PCB190: ERRORE INIT MUTEX. FINE PROGRAMMA");
      _mqx_exit(-1);
    }
      
     // Chiede subito la revisione corrente e rimane in attesa
    while(GetFwRevision()==FALSE) _time_delay(100);
    printf("PCB190:REVISIONE FW TARGET:%d.%d\n",STATUS.maj_code,STATUS.min_code); 

    // Segnalazione driver connesso
   _EVSET(_EV1_PCB190_CONNECTED);
    
    // Attende l'autorizzazione ad effetuare il refresh registri
    _EVWAIT_ANY(_EV1_UPDATE_REGISTERS);
    
   // Invia comando di reset faults al target 
   pcb190ResetFault();

   // Upload contenuto registri 
   for(i=0;i<PCB190_NREGISTERS;i++)
   {
      err_ret = Ser422ReadRegister(i,4,&CONTEST);
      if(err_ret!=_SER422_NO_ERROR)
      {
         ERROR_HANDLER();
         break;
      }
   }
 
   
   // Attende la ricezione della configurazione se necessario
   _EVSET(_EV2_PCB190_STARTUP_OK);
   debugPrintForce("PCB190: ATTENDE CONFIGURAZIONE..\n");

   _EVWAIT_ANY(_EV1_DEV_CONFIG_OK);
   debugPrintForce("PCB190: CONFIGURAZIONE OK. INIZIO LAVORO\n");

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
     
     // Legge i registri di Base che devono sempre essere aggiornati
     if(pcb190UpdateRegisters()==FALSE)  ERROR_HANDLER();

     // Verifica se deve attivare il test per lo starter a bassa velocit�
     if(ls_starter_test){

         // Disabilita la diagnostica
         if(pcb190DisableLSStarterDiagnostic()==false){
             ls_starter_test = 0;
             printf("FALLITA PREPARAZIONE TEST STARTER\n");
             continue;
         }

         printf("TEST STARTER LS IN PREPARAZIONE..\n");
         // Disabilita eventuali faults
         pcb190ResetFault();

         unsigned char buffer[6];
         pcb190StarterL();
         _time_delay(5000);

         pcb190StopStarter();
         _time_delay(5000);

         // Campionamento
         Ser422ReadRegister(_REGID(RG190_AR_MAIN_IRUN), 10, &PCB190_CONTEST);
         Ser422ReadRegister(_REGID(RG190_AR_SHIFT_IRUN), 10, &PCB190_CONTEST);
         Ser422ReadRegister(_REGID(RG190_AR_MAIN_IKEEP), 10, &PCB190_CONTEST);
         Ser422ReadRegister(_REGID(RG190_AR_SHIFT_IKEEP), 10, &PCB190_CONTEST);
         Ser422ReadRegister(_REGID(RG190_AR_MAIN_IOFF), 10, &PCB190_CONTEST);
         Ser422ReadRegister(_REGID(RG190_AR_SHIFT_IOFF), 10, &PCB190_CONTEST);

         buffer[0] = _DEVREGL(RG190_AR_MAIN_IRUN,PCB190_CONTEST);
         buffer[1] = _DEVREGL(RG190_AR_SHIFT_IRUN,PCB190_CONTEST);
         buffer[2] = _DEVREGL(RG190_AR_MAIN_IKEEP,PCB190_CONTEST);
         buffer[3] = _DEVREGL(RG190_AR_SHIFT_IKEEP,PCB190_CONTEST);
         buffer[4] = _DEVREGL(RG190_AR_MAIN_IOFF,PCB190_CONTEST);
         buffer[5] = _DEVREGL(RG190_AR_SHIFT_IOFF,PCB190_CONTEST);

         printf("LOW SPEED STARTER DATA:MR=%d SR=%d MK=%d SK=%d MOFF=%d SOFF=%d\n", buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
         mccServiceNotify(1,SRV_TEST_LS_STARTER,buffer,sizeof(buffer));
         ls_starter_test = 0;

         // Riconfigurazione dello starter
         pcb190ConfigLSStarter();
     }
     
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
        printf("PCB190:CONFIG UPDATED!\n");
        
        // Invia segnale di aggiornamento cfg      
        _EVSET(_EV0_PCB190_CFG_UPD);
       }
     }
     _mutex_unlock(&(CONTEST.reglist_mutex));
     
     // Termine della routine di driver
      STATUS.ready=1;
      _EVSET(_EV1_PCB190_RUN);
     _time_delay(_DEF_PCB190_DRIVER_DELAY);
   }
}

//////////////////////////////////////////////////////////////////////////////
/*
_PCB190_Error_Enum GetFwRevision(void)
        La funzione legge il codice di revisione del firmware del 
        target.

PARAM:
        -
RETURN:
      TRUE: Lettura avvenuta con successo
      FALSE: Problema di comunicazione con il target

      PCB190_Stat.maj_code/PCB190_Stat.min_code = codice revisione
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
  frame.data1=_CMD1(PCB190_GET_FWREV);
  frame.data2=_CMD2(PCB190_GET_FWREV);
  Ser422Send(&frame, SER422_BLOCKING, CONTEST.ID);
  
  // Eventuali errori di comunicazione
  if(frame.retcode!=SER422_DATA) return FALSE;

  STATUS.maj_code = frame.data2;
  STATUS.min_code = frame.data1;
  generalConfiguration.revisioni.pcb190.maj = STATUS.maj_code; 
  generalConfiguration.revisioni.pcb190.min = STATUS.min_code; 
  
  return TRUE;
   
}

void ERROR_HANDLER(void)
{
  // Segnalazione driver disconnesso
   _EVCLR(_EV1_PCB190_CONNECTED);

   // Riconfigurazione del driver a seguito della ripartenza
   printf("PCB190 ERRORE: ATTESA RICONNESSIONE E RICONFIGURAZIONE REGISTRI\n"); 
  
   while(1){
    
    _time_delay(100);
    
    // Richiesta revisione firmware a target
    while(GetFwRevision()==FALSE) _time_delay(100);
    printf("PCB190:REVISIONE FW TARGET:%d.%d\n",STATUS.maj_code,STATUS.min_code);     

    // Carica sulla periferica lo stato dei registri cosi come erano prima del reset
    printf("PCB190: DOWNLOAD REGISTRI ...\n");
    if(Ser422UploadRegisters(10, &CONTEST)== FALSE)   continue;  
   
    // Carica Tutti i registri RD / RW
    int i;
    for(i=0;i<PCB190_NREGISTERS;i++) 
    {
      if(Ser422ReadRegister(i,4,&CONTEST)!=_SER422_NO_ERROR) break; 
    }
    if(i!=PCB190_NREGISTERS) continue;  
    break;
  }

  // Segnalazione driver connesso
  _EVSET(_EV1_PCB190_CONNECTED);
  
  // Ripartenza completata. Pu� tornare da dove aveva lasciato
  printf("PCB190 RIPARTITA CORRETTAMENTE\n"); 

  return;
}

//////////////////////////////////////////////////////////////////////////////
/*
bool pcb190StartRxStd(void)
      
      La Funzione Invia il comando di attivazione sequenza raggi standard
      La funzione presuppone che siano gi� stati caricati tutti i registri
      necessari.

      
PARAM:
        -
RETURN:
      TRUE: Comando attivato
      FALSE: Sequenza Busy


Autore: M. Rispoli
Data: 25/10/2014
*/
//////////////////////////////////////////////////////////////////////////////
int pcb190StartRxStd(void)
{
  _Ser422_Command_Str frame;

  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_START_RX_STD);
  frame.data2=_CMD2(PCB190_START_RX_STD);
  debugPrintI2("PCB190 START EXPOSURE 2D COMMAND, D1",frame.data1,"D2",frame.data2);
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  

  return (int) frame.retcode;
  
}

int pcb190StartRxAecStd(void)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_START_RX_STD_AEC);
  frame.data2=_CMD2(PCB190_START_RX_STD_AEC);
  debugPrintI2("PCB190 START EXPOSURE 2D AEC COMMAND, D1",frame.data1,"D2",frame.data2);
  
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  return frame.retcode;
  
}

int pcb190StartRxTomoAec(void)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_START_RX_TOMO_AEC);
  frame.data2=_CMD2(PCB190_START_RX_TOMO_AEC);
  debugPrintI2("PCB190 START EXPOSURE 3D AEC COMMAND, D1",frame.data1,"D2",frame.data2);
  
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  return frame.retcode;

  
}


int pcb190StartRxTomo(void)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_START_RX_TOMO);
  frame.data2=_CMD2(PCB190_START_RX_TOMO);
  
  debugPrintI2("PCB190 START EXPOSURE 3D COMMAND, D1",frame.data1,"D2",frame.data2);

  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  return frame.retcode;

  
}

bool pcb190ResetFault(void)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_RST_FAULTS);
  frame.data2=_CMD2(PCB190_RST_FAULTS);
  
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  if(frame.retcode==SER422_COMMAND_OK) return TRUE; 
  return FALSE;
  
}

/*
    I codici associati al fuoco sono quelli definiti
    nel protocollo seriale e debbono essere rispettati.

    In caso di 0 viene spento il filamento
*/
bool pcb190SetFuoco(unsigned char fuoco)
{
  _Ser422_Command_Str frame;

  // In demo il fuoco viene sempre spento
  if(generalConfiguration.demoMode) fuoco = 0;
  
  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  
  if(fuoco==0) frame.data1=_CMD1(PCB190_SET_FILON);
  else frame.data1=_CMD1(PCB190_SELECT_FUOCO); 
  frame.data2=fuoco;
  
  debugPrintI("PCB190 ATTIVAZIONE FUOCO, CODICE",fuoco);
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  if(frame.retcode==SER422_COMMAND_OK) return TRUE; 
  else return FALSE;
  
}

bool pcb190StarterHcommand(void)
{
  _Ser422_Command_Str frame;


  debugPrint("PCB190 COMMAND STARTER HS");

  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_START_AR_H);
  frame.data2=_CMD2(PCB190_START_AR_H);
  
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  
  if(frame.retcode==SER422_COMMAND_OK) return TRUE; 
  else return FALSE;  
}

bool pcb190StarterH(void)
{
    unsigned char count;
    debugPrint("New HS Starter Command execution");

    // Reads the actual command counter
    if(Ser422ReadRegister(_REGID(RG190_ARHS_COUNT),10,&CONTEST) == _SER422_NO_ERROR) {
        count = _DEVREGL(RG190_ARHS_COUNT,PCB190_CONTEST);
    }else return false;

    // Repeats the command until the PCB190 command counter is actually incremented
    for(int i=10; i>0; i--){

        // Command success
        if(pcb190StarterHcommand()) return true;

        // Failed: verifies if actually the command has been executed
        if(Ser422ReadRegister(_REGID(RG190_ARHS_COUNT),10,&CONTEST) == _SER422_NO_ERROR) {
            if(_DEVREGL(RG190_ARHS_COUNT,PCB190_CONTEST) != count){
                debugPrint("HS Starter verified as executed");
                return true;
            }
        }


        debugPrint("Repeats the HS starter command");
        _time_delay(100);
    }

    return false;
}



bool pcb190StarterLcommand(void)
{

    // Start the Starter
  _Ser422_Command_Str frame;

  debugPrint("PCB190 COMMAND STARTER LS");

  // Prepara il comando
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=_CMD1(PCB190_START_AR_L);
  frame.data2=_CMD2(PCB190_START_AR_L);

  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);

  if(frame.retcode==SER422_COMMAND_OK) return TRUE;
  else return FALSE;
}


bool pcb190StarterL(void)
{
    unsigned char count;

    // Reads the actual command counter
    if(Ser422ReadRegister(_REGID(RG190_ARLS_COUNT),10,&CONTEST) == _SER422_NO_ERROR) {
        count = _DEVREGL(RG190_ARLS_COUNT,PCB190_CONTEST);
    }else return false;

    debugPrintI("New LS Starter Command execution", (int) count);

    // Repeats the command until the PCB190 command counter is actually incremented
    for(int i=10; i>0; i--){

        // Command successfully executed
        if(pcb190StarterLcommand()){
            return true;
        }

        // Failed: verifies if actually the command has been executed
        if(Ser422ReadRegister(_REGID(RG190_ARLS_COUNT),10,&CONTEST) == _SER422_NO_ERROR) {
            if(_DEVREGL(RG190_ARLS_COUNT,PCB190_CONTEST) != count){
                debugPrint("LS Starter verified as executed");
                return true;
            }
        }

        debugPrint("Repeats the LS starter command");
        _time_delay(100);
    }

    return false;
}

bool pcb190StopStarter(void)
{
  _Ser422_Command_Str frame;
 
  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  
  debugPrint("PCB190 COMMAND STOP STARTER");
  for(int i=0; i<10; i++) {

      _mutex_lock(&(CONTEST.pollinglist_mutex));

      // Prepara il comando
      frame.address = TARGET_ADDRESS;
      frame.attempt = 10;
      frame.cmd=SER422_COMMAND;
      frame.data1=_CMD1(PCB190_STOP_AR);
      frame.data2=_CMD2(PCB190_STOP_AR);

      Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
      _mutex_unlock(&(CONTEST.pollinglist_mutex));

      if(frame.retcode==SER422_COMMAND_OK) return TRUE;
      else{
          _time_delay(100);
      }

  }

  // Fallito Stop Starter
  debugPrint("PCB190 COMANDO STOP STARTER FALLITO");
  return false;
}


// Spegne lo starter: ripete il comando se necessario
bool pcb190OffStarter(void)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  debugPrint("PCB190 COMMAND STOP STARTER");

  for(int i=0; i<10; i++) {

      _mutex_lock(&(CONTEST.pollinglist_mutex));

      // Prepara il comando
      frame.address = TARGET_ADDRESS;
      frame.attempt = 10;
      frame.cmd=SER422_COMMAND;
      frame.data1=_CMD1(PCB190_OFF_AR);
      frame.data2=_CMD2(PCB190_OFF_AR);

      Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
      _mutex_unlock(&(CONTEST.pollinglist_mutex));

      if(frame.retcode==SER422_COMMAND_OK){          
          return TRUE;
      }else{
          _time_delay(100);
      }

  }

  debugPrint("PCB190 COMANDO STOP STARTER FALLITO");
  return false;
}

// Attende ATTEMPT*100ms il ready dalla PCB190
bool waitPcb190Ready(unsigned char attempt)
{

  while(attempt--)
  {
    if(Ser422ReadRegister(_REGID(RG190_SEQ_RAGGI),4,&CONTEST) == _SER422_NO_ERROR) {
        if(_DEVREGL(RG190_SEQ_RAGGI,CONTEST) == 0) return TRUE;
    }
    _time_delay(100);
  }

  debugPrint("PCB190 TIMOUT ATTESA READY");
  return FALSE;
}

// Carica lo status della pcb 190
bool pcb190GetPostRxRegisters(void){
  if(Ser422ReadRegister(_REGID(RG190_FLAGS0),10,&CONTEST) !=_SER422_NO_ERROR) return false;
  if(Ser422ReadRegister(_REGID(RG190_FAULTS),10,&CONTEST)!=_SER422_NO_ERROR) return false;
  if(Ser422ReadRegister(_REGID(RG190_MAS_EXIT),10,&CONTEST)!=_SER422_NO_ERROR) return false;
  if(Ser422ReadRegister(_REGID(RG190_HV_RXEND),10,&CONTEST)!=_SER422_NO_ERROR) return false;

  return TRUE;
}

// Effettua l'update dei registri di sistema (lanciata ogni 100ms)
bool pcb190UpdateRegisters(void)
{
  static unsigned char data[PCB190_DGN_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  unsigned char buffer[8];
  static unsigned char seq_v = 0;
  static bool update = false;
    
  
   // Gestione interna dello Starter  
   if(starterStop==TRUE)
   {
     starterStop=FALSE;
     pcb190StopStarter(); // Spegne lo starter
   }

   // Update ogni 2 secondi (se necessario)
  switch(seq_v++){
    case 0: // ------------------- SEQ 0 ----------------------------------------------
             // Lettura tensione di BUS HV
             #undef  REG
             #undef  VAR       
             #define REG  RG190_V_HV
             #define VAR  data[PCB190_DGN_HV_IDLE]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;               
             }                            
      
             // Lettura temperatura amplificatore
             #undef  REG
             #undef  VAR       
             #define REG  RG190_VAMPL_FIL
             #define VAR  data[PCB190_DGN_VTEMP]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }                            
             
       break;

    case 1: // ------------------- SEQ 1 ----------------------------------------------
          // Lettura corrente filamento
             #undef  REG
             #undef  VAR       
             #define REG  RG190_IFIL
             #define VAR  data[PCB190_I_FIL]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }                            
      
             // Lettura corrente anodica
             #undef  REG
             #undef  VAR       
             #define REG  RG190_I_ANODICA
             #define VAR  data[PCB190_I_ANODICA]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }                            
      break;

    case 2: // ------------------- SEQ 2 ----------------------------------------------
            // Lettura mAs di test: attenzione, la lettura � fatta su DT = 1.1 secondi
            // Viene dunque riportato a 1 secondo togliendo il 10% del valore letto
            if(Ser422Read16BitRegister(_REGID(RG190_MAS_TEST), 10, &CONTEST)==_SER422_NO_ERROR)
            {
              if((data[PCB190_MAS_TEST_L] != _DEVREGL(RG190_MAS_TEST,CONTEST)||(data[PCB190_MAS_TEST_H] != _DEVREGH(RG190_MAS_TEST,CONTEST)))){
                long val = _DEVREG(RG190_MAS_TEST,CONTEST);
                val = val * 9 / 10;
                data[PCB190_MAS_TEST_L] = (unsigned char) val;
                data[PCB190_MAS_TEST_H] = (unsigned char) (val>>8);      
                update = true;                

              }
            }
            
             // Lettura tensione 15V INVERTER 
             #undef  REG
             #undef  VAR       
             #define REG  RG190_V_15
             #define VAR  data[PCB190_V15]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }       

      break;

    case 3: // ------------------- SEQ 3 ----------------------------------------------
             // Lettura tensione 32V
             #undef  REG
             #undef  VAR       
             #define REG  RG190_V_32
             #define VAR  data[PCB190_V32]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }       
             
             // Lettura tensione -32V
             #undef  REG
             #undef  VAR       
             #define REG  RG190_V_M32
             #define VAR  data[PCB190_VM32]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }             
     break;
     
    case 4:// ------------------- SEQ 4 ----------------------------------------------
  
             // Lettura tensione +12V
             #undef  REG
             #undef  VAR       
             #define REG  RG190_V_12
             #define VAR  data[PCB190_V12]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }       

             // Lettura tensione -12V
             #undef  REG
             #undef  VAR       
             #define REG  RG190_V_M12
             #define VAR  data[PCB190_VM12]
             Ser422ReadRegister(_REGID(REG),10,&CONTEST);
             if(VAR != _DEVREGL(REG,CONTEST)) {
               VAR = _DEVREGL(REG,CONTEST);
               update = true;
             }                    
     break;
    case 5: // ------------------- SEQ 5 ----------------------------------------------
            // Lettura tensione +15V EXT
            #undef  REG
            #undef  VAR       
            #define REG  RG190_V_15EXT
            #define VAR  data[PCB190_V15EXT]
            Ser422ReadRegister(_REGID(REG),10,&CONTEST);
            if(VAR != _DEVREGL(REG,CONTEST)) {
             VAR = _DEVREGL(REG,CONTEST);
             update = true;
            }       
      break;
    case 6:
            // Lettura FLAGS0
            #undef  REG
            #undef  VAR       
            #define REG  RG190_FLAGS0
            #define VAR  data[PCB190_FLAGS0]
            Ser422ReadRegister(_REGID(REG),10,&CONTEST);
            if(VAR != _DEVREGL(REG,CONTEST)) {
                VAR = _DEVREGL(REG,CONTEST);
                update = true;
            }                            

            // In caso di Fault legge anche il registro errori
            if(_TEST_BIT(PCB190_FAULT))
            {
                #undef  REG
                #undef  VAR       
                #define REG  RG190_FAULTS
                #define VAR  data[PCB190_FAULTS]
                Ser422ReadRegister(_REGID(REG),10,&CONTEST);
                if(VAR != _DEVREGL(REG,CONTEST)) {
                    VAR = _DEVREGL(REG,CONTEST);
                    update = true;
                }                                  
            }

      break;
    case 7:
          // Lettura dei lanci effettivi dello starter Bassa o alta velocit�
          Ser422ReadRegister(_REGID(RG190_ARLS_COUNT),10,&CONTEST);
          data[PCB190_ARLS] = _DEVREGL(RG190_ARLS_COUNT,CONTEST);
          if(0 != _DEVREGL(RG190_ARLS_COUNT,CONTEST)) {
              Ser422WriteRegister(_REGID(RG190_ARLS_COUNT),0, 10,&CONTEST);
              update = true;
          }

          Ser422ReadRegister(_REGID(RG190_ARHS_COUNT),10,&CONTEST);
          data[PCB190_ARHS] = _DEVREGL(RG190_ARHS_COUNT,CONTEST);
          if(0 != _DEVREGL(RG190_ARHS_COUNT,CONTEST)) {
              Ser422WriteRegister(_REGID(RG190_ARHS_COUNT),0, 10,&CONTEST);
              update = true;
          }

    break;

    default:
      seq_v = 0;
      if(update){
        update=FALSE;
        
        mccPCB190Notify(1,PCB190_NOTIFY_DGN,data,sizeof(data)); 
        data[PCB190_ARLS] = 0;
        data[PCB190_ARHS] = 0;

      }
 
      break;
  } // Switch ..
  
  return TRUE;
}



// Attivazione corrnte di test del masmetro
bool pcb190SetITest(bool stat)
{

    _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  if(stat==TRUE)
  {
    frame.data1=_CMD1(PCB190_SET_IMAS);
    frame.data2=_CMD2(PCB190_SET_IMAS);
  }
  else
  {
    frame.data1=_CMD1(PCB190_CLR_IMAS);
    frame.data2=_CMD2(PCB190_CLR_IMAS);
  }
  
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  if(frame.retcode==SER422_COMMAND_OK) 
  {
    if(stat) STATUS.i_test=1;
    else STATUS.i_test=0;
    
    return TRUE; 
  }

  return FALSE;

}

// Invio comando generico
signed short pcb190SendCommand(unsigned char data1, unsigned char data2)
{
  _Ser422_Command_Str frame;

  // Sospende il driver bloccando la mutex del polling
  // Il driver si blocca esattamente dopo aver letto i registri di stato
  _mutex_lock(&(CONTEST.pollinglist_mutex));
  
  // Prepara il comando 
  frame.address = TARGET_ADDRESS;
  frame.attempt = 10;
  frame.cmd=SER422_COMMAND;
  frame.data1=data1;
  frame.data2=data2;
  
  Ser422Send(&frame, SER422_BLOCKING,CONTEST.ID);
  _mutex_unlock(&(CONTEST.pollinglist_mutex));
  
  if(frame.retcode==SER422_COMMAND_OK) return frame.data2;
  if(frame.retcode==SER422_DATA) return frame.data1*256+frame.data2;
  return -1;

}

/* Questa funzione gira con il driver freezed 
*/
bool pcb190UploadExpose(_RxStdSeq_Str* Param, bool isAEC)
{
  if (isAEC)
  {
   debugPrint("PCB190 UPLOAD PRE-EXPOSURE DATA");

   // DATI CARICATI DOPO IL PRE IMPULSO
   if(Ser422WriteRegister(_REGID(RG190_RXHVTMO),Param->esposizione.TMO, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXHVEXP),Param->esposizione.HV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXI),Param->esposizione.I, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXMAS),Param->esposizione.MAS, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_HV),Param->esposizione.MINHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_HV),Param->esposizione.MAXHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_IANODICA),Param->esposizione.MINI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_IANODIA),Param->esposizione.MAXI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_AEC),1, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   return TRUE;
  }


  // Scrive uno per uno tutti i registri
   if(Ser422WriteRegister(_REGID(RG190_RXHVTMO),Param->esposizione.TMO, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXHVEXP),Param->esposizione.HV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXI),Param->esposizione.I, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXMAS),Param->esposizione.MAS, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_HV),Param->esposizione.MINHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_HV),Param->esposizione.MAXHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_IANODICA),Param->esposizione.MINI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_IANODIA),Param->esposizione.MAXI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXCHK),Param->esposizione.CHK, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

   // Caricamento modalit� particolare di sparo senza detector
   if(Param->config & 0x81) debugPrint("PCB190 UPLOAD EXPOSURE DATA NO SYNC TO DETECTOR");
   else if(generalConfiguration.gantryCfg.detectorType == DETECTOR_SOLO){
       Param->config|=0x2;
       debugPrint("PCB190 UPLOAD EXPOSURE DATA WITH DETECTOR SOLO");
   }else{
       debugPrint("PCB190 UPLOAD EXPOSURE DATA");
   }

   if(Ser422WriteRegister(_REGID(PR_RX_OPT),Param->config, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

   printf("Param->esposizione.TMO=%d - 0x%x\n",Param->esposizione.TMO,Param->esposizione.TMO);
   return TRUE;
}

bool pcb190UploadExposeAE(_RxStdSeq_Str* Param)
{

    debugPrint("PCB190 UPLOAD EXPOSURE (AE) DATA");
  // Scrive uno per uno tutti i registri
   if(Ser422WriteRegister(_REGID(RG190_RXHVTMO),Param->esposizioneAE.TMO, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXHVEXP),Param->esposizioneAE.HV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXI),Param->esposizioneAE.I, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXMAS),Param->esposizioneAE.MAS, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_HV),Param->esposizioneAE.MINHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_HV),Param->esposizioneAE.MAXHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_IANODICA),Param->esposizioneAE.MINI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_IANODIA),Param->esposizioneAE.MAXI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXCHK),Param->esposizioneAE.CHK, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   //if(Ser422WriteRegister(_REGID(PR190_CONFIG),Param->config, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

   return TRUE;
}

bool pcb190UploadTomoExpose(_RxStdSeq_Str* Param, bool isAEC)
{
  if (isAEC)
  {
   debugPrint("PCB190 UPLOAD AEC PULSE");

   if(Ser422WriteRegister(_REGID(RG190_RXHVTMO),Param->esposizione.TMO, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXHVEXP),Param->esposizione.HV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;   
   if(Ser422WriteRegister(_REGID(RG190_RXI),Param->esposizione.I, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXMAS),Param->esposizione.MAS, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_AEC),1, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_HV),Param->esposizione.MINHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_HV),Param->esposizione.MAXHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_IANODICA),Param->esposizione.MINI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_IANODIA),Param->esposizione.MAXI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   return TRUE;
  }
  
   debugPrint("PCB190 UPLOAD PRE or MAN-PULSE DATA");
   if(Ser422WriteRegister(_REGID(RG190_RXHVTMO),Param->esposizione.TMO, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXHVEXP),Param->esposizione.HV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXI),Param->esposizione.I, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_RXMAS),Param->esposizione.MAS, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_HV),Param->esposizione.MINHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_HV),Param->esposizione.MAXHV, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MIN_IANODICA),Param->esposizione.MINI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_MAX_IANODIA),Param->esposizione.MAXI, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_TOMO_SAMPLES),Param->tomo_samples, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
   if(Ser422WriteRegister(_REGID(RG190_TOMO_PREWIN),Param->tomo_pre_pulses, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE; 
   if(Ser422WriteRegister(_REGID(RG190_RXCHK),Param->esposizione.CHK, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
  
   // Caricamento modalit� particolare di sparo senza detector
   if(generalConfiguration.gantryCfg.detectorType == DETECTOR_SOLO){
       Param->config|=0x2;
       debugPrint("PCB190 UPLOAD EXPOSURE DATA FOR TOMO WITH SOLO DETECTOR");
   }else debugPrint("PCB190 UPLOAD EXPOSURE DATA FOR TOMO");

   if(Ser422WriteRegister(_REGID(PR_RX_OPT),Param->config, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

   return TRUE;
}


/*
  Funzione configuratrice
*/
bool config_pcb190(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len){

  if(len!= sizeof(pcb190Conf_Str)) return false;
  memcpy(&generalConfiguration.pcb190Cfg, buffer, len);
  
  // Caricamento sulla PCb190

  if(Ser422WriteRegister(_REGID(PR16_MAX_IFIL),generalConfiguration.pcb190Cfg.IFIL_MAX_SET, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
  if(Ser422WriteRegister(_REGID(PR_IFIL_LIMIT),(unsigned char) generalConfiguration.pcb190Cfg.IFIL_LIMIT, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
  if(Ser422WriteRegister(_REGID(PR190_IWARM),generalConfiguration.pcb190Cfg.IFIL_DAC_WARM, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

  unsigned char hs_config;
  if(generalConfiguration.gantryCfg.highSpeedStarter) hs_config = 1;
  else hs_config = 0;
  if(hs_config) printf("190: HIGH SPEED STARTER\n");
  else printf("190: LOW SPEED STARTER\n");

  if(Ser422WriteRegister(_REGID(PR190_STARTER),hs_config, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

  // Se lo starter interno � configurato vengono inviati i parametri della diagnostica
  if(hs_config==0) pcb190ConfigLSStarter();

  pcb190PrintParametri();
  return true;
}

bool pcb190ConfigLSStarter(void){

    if(Ser422WriteRegister(_REGID(PR_IMAIN_OFF_MAX),generalConfiguration.pcb190Cfg.cal_max_main_off, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_OFF_MAX),generalConfiguration.pcb190Cfg.cal_max_shift_off, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_IMAIN_RUN_MAX),generalConfiguration.pcb190Cfg.cal_max_main_run, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_RUN_MAX),generalConfiguration.pcb190Cfg.cal_max_shift_run, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_IMAIN_RUN_MIN),generalConfiguration.pcb190Cfg.cal_min_main_run, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_RUN_MIN),generalConfiguration.pcb190Cfg.cal_min_shift_run, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;


    if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
        // Se la modalita di mantenimento non � attiva viene anche azzerata la diagnostica
        if(Ser422WriteRegister(_REGID(PR_IMAIN_KEEP_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
        if(Ser422WriteRegister(_REGID(PR_ISHIFT_KEEP_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
        if(Ser422WriteRegister(_REGID(PR_IMAIN_KEEP_MIN),0, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
        if(Ser422WriteRegister(_REGID(PR_ISHIFT_KEEP_MIN),0, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

    }else{
        if(Ser422WriteRegister(_REGID(PR_IMAIN_KEEP_MAX),generalConfiguration.pcb190Cfg.cal_max_main_keep, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
        if(Ser422WriteRegister(_REGID(PR_ISHIFT_KEEP_MAX),generalConfiguration.pcb190Cfg.cal_max_shift_keep, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
        if(Ser422WriteRegister(_REGID(PR_IMAIN_KEEP_MIN),generalConfiguration.pcb190Cfg.cal_min_main_keep, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
        if(Ser422WriteRegister(_REGID(PR_ISHIFT_KEEP_MIN),generalConfiguration.pcb190Cfg.cal_min_shift_keep, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    }

    return true;
}

bool pcb190DisableLSStarterDiagnostic(void){

    if(Ser422WriteRegister(_REGID(PR_IMAIN_OFF_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_OFF_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_IMAIN_RUN_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_RUN_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_IMAIN_RUN_MIN),0, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_RUN_MIN),0, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_IMAIN_KEEP_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_KEEP_MAX),255, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_IMAIN_KEEP_MIN),0, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;
    if(Ser422WriteRegister(_REGID(PR_ISHIFT_KEEP_MIN),0, 10,&PCB190_CONTEST)!=_SER422_NO_ERROR) return FALSE;

    return true;
}




void pcb190PrintParametri(){
  
#ifndef PRINTCFG
   return ;
#endif

  printf("--------- PARAMETRI PCB190 --------------\n");

  if(generalConfiguration.gantryCfg.detectorType==DETECTOR_SCREENPLUS) printf("DETECTOR TYPE: %s\n",DETECTOR_SCREENPLUS_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_LMAM2) printf("DETECTOR TYPE: %s\n",DETECTOR_LMAM2_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_SOLO) printf("DETECTOR TYPE: %s\n",DETECTOR_SOLO_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_FDI) printf("DETECTOR TYPE: %s\n",DETECTOR_FDI_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_LMAMV2) printf("DETECTOR TYPE: %s\n",DETECTOR_LMAMV2_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_SMAM) printf("DETECTOR TYPE: %s\n",DETECTOR_SMAM_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_SMAMV2) printf("DETECTOR TYPE: %s\n",DETECTOR_SMAMV2_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_FDIV2) printf("DETECTOR TYPE: %s\n",DETECTOR_FDIV2_STR);
  else if(generalConfiguration.gantryCfg.detectorType==DETECTOR_SCREENPLUSV2) printf("DETECTOR TYPE: %s\n",DETECTOR_SCREENPLUSV2_STR);
  else printf("DETECTOR TYPE: INVALID CODE!!! \n");

  printf("FILAMENT-ABS-LIMIT:%d\n",generalConfiguration.pcb190Cfg.IFIL_LIMIT);
  printf("FILAMENT-SET-LIMIT:%d\n",generalConfiguration.pcb190Cfg.IFIL_MAX_SET);
  printf("FILAMENT-WARMING_DAC:%d\n",generalConfiguration.pcb190Cfg.IFIL_DAC_WARM);
  printf("HV CONVERSION:%f\n",((float) generalConfiguration.pcb190Cfg.HV_CONVERTION / 1000.0));
  printf("kV Conversion:k=%f, ofs = %f\n",(float) generalConfiguration.pcb190Cfg.kV_CALIB / 1000,  (float) generalConfiguration.pcb190Cfg.kV_OFS / 1000);
  if(generalConfiguration.gantryCfg.highSpeedStarter) printf("HS STARTER CONFIGURED\n");
  else{
      printf("LOW SPEED STARTER CONFIGURATION:\n");
      if(generalConfiguration.pcb190Cfg.starter_off_after_exposure){
          printf("STARTER OFF AFTER X-RAY  ");
          if(generalConfiguration.pcb190Cfg.starter_off_with_brake) printf("WITH BRAKE \n\n");
          else printf("WITHOUT BRAKES \n\n");
      }else{
          printf("STARTER KEEPS ALIVE WITH TIMOUT\n");
          if(generalConfiguration.pcb190Cfg.starter_off_with_brake) printf("THE BRAKE IS USED TO STOP THE STARTER\n\n");
          else printf("THE BRAKE IS NOT USED\n\n");
      }

      printf(" CAL MAX MAIN OFF:%d\n", generalConfiguration.pcb190Cfg.cal_max_main_off);
      printf(" CAL MAX SHIFT OFF:%d\n",generalConfiguration.pcb190Cfg.cal_max_shift_off);
      printf(" CAL MAX MAIN RUN:%d\n", generalConfiguration.pcb190Cfg.cal_max_main_run);
      printf(" CAL MAX SHIFT RUN:%d\n", generalConfiguration.pcb190Cfg.cal_max_shift_run);
      printf(" CAL MAX MAIN KEEP:%d\n", generalConfiguration.pcb190Cfg.cal_max_main_keep);
      printf(" CAL MAX SHIFT KEEP:%d\n", generalConfiguration.pcb190Cfg.cal_max_shift_keep);
      printf(" CAL MAX MAIN BRK:%d\n", generalConfiguration.pcb190Cfg.cal_max_main_brk);
      printf(" CAL MAX SHIFT BRK:%d\n", generalConfiguration.pcb190Cfg.cal_max_shift_brk);

      printf(" CAL MIN MAIN RUN:%d\n", generalConfiguration.pcb190Cfg.cal_min_main_run);
      printf(" CAL MIN SHIFT RUN:%d\n", generalConfiguration.pcb190Cfg.cal_min_shift_run);
      printf(" CAL MIN MAIN KEEP:%d\n", generalConfiguration.pcb190Cfg.cal_min_main_keep);
      printf(" CAL MIN SHIFT KEEP:%d\n",generalConfiguration.pcb190Cfg.cal_min_shift_keep);
      printf(" CAL MIN MAIN BRK:%d\n", generalConfiguration.pcb190Cfg.cal_min_main_brk);
      printf(" CAL MIN SHIFT BRK:%d\n", generalConfiguration.pcb190Cfg.cal_min_shift_brk);

    }




}

float pcb190ConvertKvRead(unsigned char val){
 return  (float) val *((float) generalConfiguration.pcb190Cfg.kV_CALIB / 1000) + ((float) generalConfiguration.pcb190Cfg.kV_OFS / 1000);   
}

void enterFreezeMode(void){
    // Entra in Freeze
     debugPrint("PB190 ENTRA IN FREEZE\n");
    _EVCLR(_EV1_PCB190_RUN);
    _EVSET(_EV1_PCB190_FREEZED); // Notifica l'avvenuto Blocco
    _EVWAIT_ANY(_MOR2(_EV1_DEVICES_RUN,_EV1_PCB190_RUN)); // Attende lo sblocco
     printf("PB190 ESCE DAL FREEZE\n");
     STATUS.freeze = 0;
}



void pcb190updateStarterRegisters(void){
    updateStarterRegisterFlag = true;
}

bool pcb190getUpdateStarterRegisters(void){
    return updateStarterRegisterFlag ;
}

// Esegue una sequenza di Start / Stop per lo starter a bassa velocit�
void pcb190TestLSStarter(void){
    if(generalConfiguration.gantryCfg.highSpeedStarter) return;
    ls_starter_test = 1;
}

/* EOF */
 

