#define _BIOPSY_STANDARD_C
#include "dbt_m4.h" 

static void BiopsySetX(void);
static void BiopsySetY(void);
static void BiopsySetZ(void);
static void BiopsySetStepUpZ(void);
static void BiopsySetStepDwnZ(void);
static bool biopsyGetZ(void);
static bool biopsyGetX(void);
static bool biopsyGetY(void);
static bool BiopsyGetStat(void);
static bool BiopsyGetRevision(void);
static bool moveX(unsigned char* dati);
static bool moveY(unsigned char* dati);
static bool moveZ(unsigned char* dati);

#define _DEF_BIOPSY_HIGH        193 // Posizione needle in zero rispetto al piano deel detector
#define _DEF_MARGINE_HIGH       10 // millimetri di margine sulla posizione massima


#define TARGET_C 0x0D
#define TARGET_X 0x0E
#define TARGET_Y 0x0F
#define TARGET_Z 0x10

#define _DEF_BIOPSY_DRIVER_DELAY_NC 1000        // Tempo di attesa quando scollegato
#define _DEF_BIOPSY_DRIVER_DELAY_CONNECTED 200   // Tempo di attesa quando scollegato
#define _DEF_BIOPSY_DRIVER_TIMEOUT 3000        // Tempo massimo di attesa per una risposta
#define _DEF_BIOP_MOT_TMO          30           // timeout movimenti


static unsigned short driverDelay;
static bool  verifyBiopsyDataInit=FALSE;
static bool  biopTestPosition = FALSE;
static MUTEX_STRUCT biopsy_standard_mutex;

//////////////////////////////////////////////////////////////////////////////
/*
void BIOPSY_driver(uint32_t taskRegisters)


Autore: M. Rispoli
Data: 22/06/2015
*/
//////////////////////////////////////////////////////////////////////////////
void biopsyStandardLoop(void)
{

  unsigned char dati[_BP_STD_DATA_LEN];
  static unsigned short timeout; // Tempo massimo di attesa senza risposta
  static bool sbloccoReq=FALSE; 
  static unsigned char adapterId;
  static unsigned short needleZ=0xFFFF;
  static unsigned char maxZ = 255;
  bool notifica=FALSE;
  int motionTmo =_DEF_BIOP_MOT_TMO;
  
  if (_mutex_init(&biopsy_standard_mutex, NULL) != MQX_OK)
  {
    printf("ERRORE NEL TASK DI GESTIONE ERRORI!! MUOIO!\n");
    _mqx_exit(-1);
  }


   debugPrint("BYM ATTIVAZIONE DRIVER BIOPSIA STANDARD");
   driverDelay =   _DEF_BIOPSY_DRIVER_DELAY_NC;

   timeout = _DEF_BIOPSY_DRIVER_TIMEOUT/driverDelay;
    

   while(1)
   {

     _mutex_lock(&biopsy_standard_mutex);

     notifica=FALSE;
     dati[_BP_STD_CONNESSIONE]=0;   // Connessione
     dati[_BP_STD_SBLOCCO]=0;       // Pulsante sblocco
     dati[_BP_STD_ACCESSORIO]=255;  // Codice accessorio
     dati[_BP_STD_ZL]=0;            // Posizione ZL
     dati[_BP_STD_ZH]=0;            // Posizione ZH
     dati[_BP_STD_MOTION]=0;        // movimento ..
     dati[_BP_STD_MOTION_END]=0xFF; // Esito movimenti (se applicabile)
     dati[_BP_STD_CHKH]=generalConfiguration.biopsyCfg.checksum_h;          // Checksum H
     dati[_BP_STD_CHKL]=generalConfiguration.biopsyCfg.checksum_l;          // Checksum L
     dati[_BP_STD_REVIS]=generalConfiguration.biopsyCfg.revisione;          // Revisione

    // ALLO STARTUP O QUANDO SERVE RINFRESCARE LO STATO ALL'APPLICAZIONE
    if(verifyBiopsyDataInit==TRUE)
    {
      verifyBiopsyDataInit = FALSE;
      notifica = TRUE; 
      if(generalConfiguration.biopsyCfg.biopsyConnected==TRUE) dati[_BP_STD_CONNESSIONE] = 1; // Notifica cambio stato in connessione
      else  dati[_BP_STD_CONNESSIONE] = 2; // Notifica cambio stato in connessione
      maxZ=255;
    }
  
     // Il test sulla presenza dipende dal fatto che non sia riconosciuto nessun POTTER
     if(generalConfiguration.potterCfg.potId == POTTER_UNDEFINED)
     {
       driverDelay =  _DEF_BIOPSY_DRIVER_DELAY_CONNECTED;
       
       // Se c'è un comando di movimento in corso lo gestisce in maniera speciale
       if(generalConfiguration.biopsyCfg.standardConf.movimento)
       {                    
          
          if((biopsyGetZ()==FALSE) || (biopsyGetY()==FALSE) || (biopsyGetX()==FALSE) ) 
          {
            // Verifica il timeout sul movimento
            motionTmo--;
            if(!motionTmo) 
            {
              generalConfiguration.biopsyCfg.standardConf.movimento = FALSE;
              generalConfiguration.biopsyCfg.standardConf.movimento_x = FALSE;
              generalConfiguration.biopsyCfg.standardConf.movimento_y = FALSE;
              generalConfiguration.biopsyCfg.standardConf.movimento_z = FALSE;
              dati[_BP_STD_MOTION ] = 5; // Timeout
              mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
              timeout = _DEF_BIOPSY_DRIVER_TIMEOUT/driverDelay;
              debugPrint("BYM TIMEOUT");
            }

            // Se non è in  timeout allora attende ancora ...
            goto termine_biop_driver;
          }
          motionTmo =_DEF_BIOP_MOT_TMO;
          

          // Aggiorno i dati sulla Z        
          dati[_BP_STD_ZL] = (unsigned char) (generalConfiguration.biopsyCfg.Z & 0x00FF);
          dati[_BP_STD_ZH] = (unsigned char) (generalConfiguration.biopsyCfg.Z >> 8);
          needleZ = generalConfiguration.biopsyCfg.Z;
          

          if(generalConfiguration.biopsyCfg.standardConf.stepUp_z)
          {
            generalConfiguration.biopsyCfg.standardConf.stepUp_z = FALSE;
            BiopsySetStepUpZ();           
            dati[_BP_STD_MOTION]=3;
            mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
            debugPrint("BYM MOVIMENTO STEP UP Z");
            goto termine_biop_driver;
          }

          if(generalConfiguration.biopsyCfg.standardConf.stepDwn_z)
          {
              generalConfiguration.biopsyCfg.standardConf.stepDwn_z = FALSE;
              BiopsySetStepDwnZ();           
              dati[_BP_STD_MOTION]=3;
              mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
              debugPrint("BYM MOVIMENTO STEP DOWN Z");
              goto termine_biop_driver;
          }

          // La sequenza di spostamento dipende dalla direzione della Z (up o Dwn)
          // Quando la Z muove verso l'alto allora è proprio la Z a muovere per prima.
          // Quando la Z muove verso il basso allora la Z è l'ultima a muovere
          if(generalConfiguration.biopsyCfg.standardConf.z_up)
          {
              if(moveZ(dati)) goto termine_biop_driver;
              else if(moveY(dati)) goto termine_biop_driver;
              else  if(moveX(dati)) goto termine_biop_driver;
          }else
          {
              if(moveX(dati)) goto termine_biop_driver;
              else if(moveY(dati)) goto termine_biop_driver;
              else  if(moveZ(dati)) goto termine_biop_driver;
          }
               
          // Se arriva qui allora il movimento è terminato
          generalConfiguration.biopsyCfg.standardConf.movimento = FALSE;
          dati[_BP_STD_MOTION] = 4;
          
          debugPrintI3("BYM FINE MOVIMENTO. X", generalConfiguration.biopsyCfg.X, "Y",generalConfiguration.biopsyCfg.Y,"Z",generalConfiguration.biopsyCfg.Z);
          
          // Controllo sul risultato se tale controllo viene abilitato
          if(biopTestPosition)
          {
            biopTestPosition = FALSE;
            int dif;
            
            // Il controllo sulla XYZ da HOME a HOME (movimenti verso l'origine) richiedono un controllo più lasco
            // perchè il sistema non è in grado di posizionarsi in salita a meno di 2 millimetri dall'origine             
            if(generalConfiguration.biopsyCfg.TGZ == 0){
              dif = generalConfiguration.biopsyCfg.TGZ-generalConfiguration.biopsyCfg.Z;
              if((dif> 30) || (dif<-30) ) dati[_BP_STD_MOTION_END]&= ~0x4;
            }else{
              dif = generalConfiguration.biopsyCfg.TGZ-generalConfiguration.biopsyCfg.Z;
              if((dif> 10) || (dif<-10) ) dati[_BP_STD_MOTION_END]&= ~0x4;
            }

            if(generalConfiguration.biopsyCfg.TGX == 0){
              dif = generalConfiguration.biopsyCfg.TGX-generalConfiguration.biopsyCfg.X;
              if((dif> 30) || (dif<-30) ) dati[_BP_STD_MOTION_END]&= ~0x1;
            }else{
              dif = generalConfiguration.biopsyCfg.TGX-generalConfiguration.biopsyCfg.X;
              if((dif> 10) || (dif<-10) ) dati[_BP_STD_MOTION_END]&= ~0x1;
            }

            if(generalConfiguration.biopsyCfg.TGY == 0){
              dif = generalConfiguration.biopsyCfg.TGY-generalConfiguration.biopsyCfg.Y;
              if((dif> 30) || (dif<-30) ) dati[_BP_STD_MOTION_END]&= ~0x2;
            }else{
              dif = generalConfiguration.biopsyCfg.TGY-generalConfiguration.biopsyCfg.Y;
              if((dif> 10) || (dif<-10) ) dati[_BP_STD_MOTION_END]&= ~0x2;
            }
          }
          
          mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
          timeout = _DEF_BIOPSY_DRIVER_TIMEOUT/driverDelay;
          debugPrintI("BYM FINE MOVIMENTO ESITO",dati[_BP_STD_MOTION_END]);
          goto termine_biop_driver;
       }
       else if(BiopsyGetStat()==TRUE)
       {      
         timeout = _DEF_BIOPSY_DRIVER_TIMEOUT/driverDelay;
         
         biopsyGetZ(); // Acquisisce la Z
        
         // Verifica prima connessione
         if(generalConfiguration.biopsyCfg.biopsyConnected==FALSE)
         {
           // Richiede revisione e checksum caricati
           BiopsyGetRevision();
           
           // Blocca il braccio
           generalConfiguration.biopsyCfg.biopsyArmEna = FALSE;
           actuatorsManageEnables();
           
           // Diminuisce il tempo di polling durante la connessione
           generalConfiguration.biopsyCfg.biopsyConnected = TRUE;
           debugPrint("BYM RICONOSCIUTA BIOPSIA");
           dati[_BP_STD_CONNESSIONE] = 1; // Notifica cambio stato in connessione
           notifica=TRUE;
         }
         
         // Controlla se è stato premuto il pulsante di sblocco
         if(sbloccoReq!=generalConfiguration.biopsyCfg.sbloccoReq)
         {
           notifica=TRUE;
           sbloccoReq=generalConfiguration.biopsyCfg.sbloccoReq;
           if(sbloccoReq) dati[_BP_STD_SBLOCCO] = 1;
           else dati[_BP_STD_SBLOCCO] = 2;
           if(sbloccoReq) 
           {
             debugPrint("BYM RICHIESTA SBLOCCO BRACCIO");
             generalConfiguration.biopsyCfg.biopsyArmEna = TRUE;
             actuatorsManageEnables();
           }else
           {
             debugPrint("BYM RICHIESTA BLOCCO BRACCIO");
             generalConfiguration.biopsyCfg.biopsyArmEna = FALSE;
             actuatorsManageEnables();
           }
         }
         
         // Verifica l'accessorio Id
         if(adapterId!=generalConfiguration.biopsyCfg.adapterId)
         {
           notifica=TRUE;
           adapterId=generalConfiguration.biopsyCfg.adapterId;
           debugPrintI("BYM CAMBIO ACCESSORIO",adapterId);
         }
         dati[_BP_STD_ACCESSORIO] = generalConfiguration.biopsyCfg.adapterId;
         
         // Verifica la posizione corrente della Z        
          if((needleZ > generalConfiguration.biopsyCfg.Z + 10) || (needleZ < generalConfiguration.biopsyCfg.Z - 10))
          {
            needleZ=generalConfiguration.biopsyCfg.Z;
            notifica=TRUE;
          }

         // Aggiornamento dati sul limite di posizionamento meccanico(in millimetri)
         // solo se c'è compressione in corso
         if(_TEST_BIT(PCB215_COMPRESSION))
         {
           dati[_BP_STD_MAX_Z] = generalConfiguration.biopsyCfg.standardConf.conf.Z_homePosition
                            + generalConfiguration.biopsyCfg.standardConf.conf.offsetPad
                            - generalConfiguration.biopsyCfg.standardConf.conf.marginePosizionamento
                            - _DEVREG(RG215_DOSE,PCB215_CONTEST);     

           static unsigned char valore=255;
           if(dati[_BP_STD_MAX_Z]!=valore){
               valore = dati[_BP_STD_MAX_Z];
               //printf("BIOPSIA: ofz=%d, ofsPad=%d, margPos=%d, pad=%d, maxZ=%d\n",generalConfiguration.biopsyCfg.conf.offsetZ,generalConfiguration.biopsyCfg.conf.offsetPad,
               //       generalConfiguration.biopsyCfg.conf.marginePosizionamento,_DEVREG(RG215_DOSE,PCB215_CONTEST),valore);
           }
         }else
         {
           dati[_BP_STD_MAX_Z] = 0; // Se non è in compressione non consente spostamenti di Z
          
         }

          if(maxZ!=dati[_BP_STD_MAX_Z])
          {
            maxZ=dati[_BP_STD_MAX_Z];
            notifica = TRUE;
          }
          
          //printf("OFFSET=%d, PAD=%d, MARG=%d, MAXZ = %d\n",generalConfiguration.biopsyCfg.conf.offsetZ,generalConfiguration.biopsyCfg.conf.offsetPad,generalConfiguration.biopsyCfg.conf.marginePosizionamento, dati[_BP_MAX_Z]); 
          // Sicuramente ogni movimento è attualmente terminato
       }else
       {
         if(!(timeout))
         {
             // Se non risponde entro un timeout il sistema viene dato per scollegato
              if(generalConfiguration.biopsyCfg.biopsyConnected==TRUE)
              {// Cambio stato
                generalConfiguration.biopsyCfg.biopsyConnected = FALSE;
                debugPrint("BYM SCOLLEGATA");
                notifica=TRUE;
                dati[_BP_STD_CONNESSIONE]=2;
              }
         }else 
         {           
           timeout--;
           // printf("BIOPSIA: TEMPO AL TIMEOUT:%d\n", timeout);
         }
       }
     }else 
     {
        // Il potter è stato rilevato: comunica l'avvenuto cambio contesto
        if(generalConfiguration.biopsyCfg.biopsyConnected==TRUE)
        {
          notifica=TRUE;
          dati[_BP_STD_CONNESSIONE]=2;
          generalConfiguration.biopsyCfg.biopsyConnected = FALSE;
          driverDelay =  _DEF_BIOPSY_DRIVER_DELAY_NC;
        }
     }
     // Effettua la notifica dello stato se necessario
     if(notifica) 
     {
        // Aggiorno i dati sulla Z        
        dati[_BP_STD_ZL] = (unsigned char) (generalConfiguration.biopsyCfg.Z & 0x00FF);
        dati[_BP_STD_ZH] = (unsigned char) (generalConfiguration.biopsyCfg.Z >> 8);
        mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
        //printf("NOTIFICA BIOPSIA\n");
     }
     
      // Termine della routine di driver
 termine_biop_driver:
     _mutex_unlock(&biopsy_standard_mutex);
     _time_delay(driverDelay);
   }
}

 // Gestisce il movimento sull'asse Z se necessario
 // Ritorna TRUE se il movimento è iniziato
 // FALSE se il movimento non è consentito/non è attivato
bool moveZ(unsigned char* dati)
{           
    if(generalConfiguration.biopsyCfg.standardConf.movimento_z==FALSE) return FALSE;
    generalConfiguration.biopsyCfg.standardConf.movimento_z = FALSE;
    BiopsySetZ();           
    dati[_BP_STD_MOTION]=3;
    mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
    debugPrint("BYM MOVIMENTO Z");
    return TRUE; 
}

 // Gestisce il movimento sull'asse X se necessario
 // Ritorna TRUE se il movimento è iniziato
 // FALSE se il movimento non è consentito/non è attivato
bool moveX(unsigned char* dati)
{
  if(generalConfiguration.biopsyCfg.standardConf.movimento_x==FALSE) return FALSE;

  generalConfiguration.biopsyCfg.standardConf.movimento_x = FALSE;
  dati[_BP_STD_MOTION]=1;
  BiopsySetX();
  mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
  debugPrint("BYM MOVIMENTO X");
  return TRUE;
}
         

 // Gestisce il movimento sull'asse X se necessario
 // Ritorna TRUE se il movimento è iniziato
 // FALSE se il movimento non è consentito/non è attivato
bool moveY(unsigned char* dati)
{
  if(generalConfiguration.biopsyCfg.standardConf.movimento_y==FALSE) return FALSE;
  generalConfiguration.biopsyCfg.standardConf.movimento_y = FALSE;
  BiopsySetY();
  dati[_BP_STD_MOTION]=2;
  mccBiopsyStandardNotify(1,BIOP_NOTIFY_STAT,dati, sizeof(dati));
  debugPrint("BYM MOVIMENTO Y");
  return TRUE; 
}

/* 
   Questa funzione richiede il contenuto della variabile di stato 
   della Biopsia, nella versione DBT. 
   Lo STATUS è così configurato:

   #bit [0:2] ACCESSORIO   
   #bit 7: pulsante di sblocco
   
   La funzione è interna al driver, pertanto non 
   richiede l'uso della mutex
       
*/
bool BiopsyGetStat(void)
{  
  unsigned char buffer[4];
  
  Ser422SendRaw(0x8E, 0x5B, 0, buffer, 5);    
  if(buffer[0]==0x8E)
  {
    // Gestione pressione del pulsante di sblocco
    if(buffer[2]&0x80) generalConfiguration.biopsyCfg.sbloccoReq = TRUE;
    else generalConfiguration.biopsyCfg.sbloccoReq = FALSE;
    
    // Gestione dell'identificazione dell'accessorio
    generalConfiguration.biopsyCfg.adapterId= buffer[2] & 0x07;
    return TRUE;
  }
  
  return FALSE;
}

bool biopsyStandardIsPresent(void){
     unsigned char buffer[4];
     Ser422SendRaw(0x90, 0x73, 0, buffer, 10);
     //printf("TEST STANDARD BIOPSY:%x %x %x %x\n",buffer[0], buffer[1], buffer[2], buffer[3]);
     if(buffer[0] == 0x90) return true;

     return false;
}

/*
  La funzione chiede info sulla Z corrente del posizionamento
*/
bool biopsyGetZ(void)
{ 
  unsigned char buffer[4];
  
  // Si utilizza la funzione RAW perchè il comando è fuori protocollo
  Ser422SendRaw(0x90, 0x71, 0, buffer, 10);
  
  if(buffer[0]==0) return FALSE;
  generalConfiguration.biopsyCfg.Z = buffer[1]*256 + buffer[2];
  
  return TRUE;
}

/*
  La funzione chiede info sulla X corrente del posizionamento
*/
bool biopsyGetX(void)
{ 
  unsigned char buffer[4];
  
  // Si utilizza la funzione RAW perchè il comando è fuori protocollo
  Ser422SendRaw(0x90, 0x74, 0, buffer, 10);
  
  if(buffer[0]==0) return FALSE;
  generalConfiguration.biopsyCfg.X = buffer[1]*256 + buffer[2];
  
  return TRUE;
}

bool biopsyGetY(void)
{ 
  unsigned char buffer[4];
  
  // Si utilizza la funzione RAW perchè il comando è fuori protocollo
  Ser422SendRaw(0x90, 0x75, 0, buffer, 10);
  
  if(buffer[0]==0) return FALSE;
  generalConfiguration.biopsyCfg.Y = buffer[1]*256 + buffer[2];
  
  return TRUE;
}

void BiopsySetX(void)
{
  unsigned char H,L;
  unsigned char buffer[4];
  
  H = (unsigned char) (generalConfiguration.biopsyCfg.TGX>>8);
  L = (unsigned char) (generalConfiguration.biopsyCfg.TGX&0x00FF);
  Ser422SendRaw(0x4E, H, L, buffer, 10);
  
  if(buffer[0]==0) generalConfiguration.biopsyCfg.standardConf.movimento_x = TRUE; // Ripete
  biopTestPosition = TRUE;
}

void BiopsySetY(void)
{
  unsigned char H,L;
  unsigned char buffer[4];
  
  H = (unsigned char) (generalConfiguration.biopsyCfg.TGY>>8);
  L = (unsigned char) (generalConfiguration.biopsyCfg.TGY&0x00FF);
  Ser422SendRaw(0x4F, H, L, buffer, 10);
  
  if(buffer[0]==0) generalConfiguration.biopsyCfg.standardConf.movimento_y = TRUE; // Ripete
  biopTestPosition = TRUE;

}

void BiopsySetStepUpZ(void)
{
  unsigned char buffer[4];
  
  Ser422SendRaw(0x8D, 0x74, 0, buffer, 10);  
  if(buffer[0]==0) generalConfiguration.biopsyCfg.standardConf.stepUp_z = TRUE; // Ripete
  biopTestPosition = FALSE;

}

void BiopsySetStepDwnZ(void)
{
  unsigned char buffer[4];
  
  Ser422SendRaw(0x8D, 0x75, 0, buffer, 10);  
  if(buffer[0]==0) generalConfiguration.biopsyCfg.standardConf.stepDwn_z = TRUE; // Ripete
  biopTestPosition = FALSE;

}

void BiopsySetZ(void)
{
  unsigned char H,L;
  unsigned char buffer[4];
  
  H = (unsigned char) (generalConfiguration.biopsyCfg.TGZ>>8);
  L = (unsigned char) (generalConfiguration.biopsyCfg.TGZ&0x00FF);
  Ser422SendRaw(0x50, H, L, buffer, 10);
  
  if(buffer[0]==0) generalConfiguration.biopsyCfg.standardConf.movimento_z = TRUE; // Ripete
  biopTestPosition = TRUE;

}

/* 
   La funzione seguente permette di leggere revisione  e checksum della 
   torretta in un unico comando. 
   La funzione restituisce FALSE in caso uno dei comandi 
   non dovesse ricevere risposta. In tal caso entrambi
   i codici verranno azzerati
      generalConfiguration.biopsyCfg.checksum = 0;
      generalConfiguration.biopsyCfg.revisione = 0;
*/
bool BiopsyGetRevision(void)
{
  unsigned char i;
  unsigned char buffer[4];
  
  // Ripete il comando per un certo tempo per essere certi che 
  // non ci sia un problema semplicemente di comunicazione
  i = 20;
  buffer[0]=0;
  while(--i)
  {
    Ser422SendRaw(0x90, 0x72, 0, buffer, 10);  
    if(buffer[0]!=0x90) continue;
    break;
  }
  if(buffer[0]!=0x90)
  {
    generalConfiguration.biopsyCfg.checksum_h = 0;
    generalConfiguration.biopsyCfg.checksum_l = 0;
    generalConfiguration.biopsyCfg.revisione = 0;
    return FALSE;
  }
  generalConfiguration.biopsyCfg.checksum_h = buffer[1];
  generalConfiguration.biopsyCfg.checksum_l = buffer[2];

  // Acquisizione revisione corrente
  i = 20;
  buffer[0]=0;
  while(--i)
  {
    Ser422SendRaw(0x90, 0x73, 0, buffer, 10);  
    if(buffer[0]!=0x90) continue;
    break;
  }

  if(buffer[0]!=0x90)
  {
    generalConfiguration.biopsyCfg.checksum_h = 0;
    generalConfiguration.biopsyCfg.checksum_l = 0;
    generalConfiguration.biopsyCfg.revisione = 0;
    return FALSE;
  }
    
  generalConfiguration.biopsyCfg.revisione = buffer[2];
  debugPrintI("BYM REVISIONE",generalConfiguration.biopsyCfg.revisione);
  debugPrintX("BYM CHECKSUM",(unsigned int) generalConfiguration.biopsyCfg.checksum_h * 256 + (unsigned int) generalConfiguration.biopsyCfg.checksum_l);
  return TRUE;
}

/* ATTIVAZIONE MOVIMENTI DA THREAD ESTERNA
    Occorre utilizzare la mutex per evitare conflitti
*/
bool biopsyStandardSetXYZ(unsigned short X, bool XGO, unsigned short Y, bool YGO, unsigned short Z, bool ZGO)
{
  if(generalConfiguration.biopsyCfg.standardConf.movimento) return FALSE; // E' già in corso
  
  debugPrintI3("BYM SETXYZ, X",X, "Y",Y, "Z",Z);

  _mutex_lock(&biopsy_standard_mutex);
  if(XGO)
  {
    generalConfiguration.biopsyCfg.TGX = X;
    generalConfiguration.biopsyCfg.standardConf.movimento_x=TRUE;
    generalConfiguration.biopsyCfg.standardConf.movimento=TRUE;
    biopTestPosition = TRUE;

  }
  if(YGO)
  {
    generalConfiguration.biopsyCfg.TGY = Y;
    generalConfiguration.biopsyCfg.standardConf.movimento_y=TRUE;
    generalConfiguration.biopsyCfg.standardConf.movimento=TRUE;
    biopTestPosition = TRUE;
 }
  if(ZGO)
  {
    // Determina se muove su o giù
    if(Z>generalConfiguration.biopsyCfg.Z) generalConfiguration.biopsyCfg.standardConf.z_up = FALSE;
    else generalConfiguration.biopsyCfg.standardConf.z_up = TRUE;
    
    generalConfiguration.biopsyCfg.TGZ = Z;
    generalConfiguration.biopsyCfg.standardConf.movimento_z=TRUE;
    generalConfiguration.biopsyCfg.standardConf.movimento=TRUE;
    biopTestPosition = TRUE;
  }
     
  _mutex_unlock(&biopsy_standard_mutex);
  
  return TRUE;
}

bool  biopsyStandardSetZLimit(unsigned char zlimit)
{
  unsigned char buffer[4];
  
  if(generalConfiguration.biopsyCfg.standardConf.movimento) return FALSE; // E' già in corso
  
  _mutex_lock(&biopsy_standard_mutex);
  Ser422SendRaw(0xCD, 0xF1, zlimit, buffer, 1);
  generalConfiguration.biopsyCfg.standardConf.zlimit = zlimit;
  _mutex_unlock(&biopsy_standard_mutex);
  return TRUE;
}
bool  biopsyStandardSetZLesione(unsigned char zlesione)
{
  unsigned char buffer[4];
  
  if(generalConfiguration.biopsyCfg.standardConf.movimento) return FALSE; // E' già in corso
  
  _mutex_lock(&biopsy_standard_mutex);
  Ser422SendRaw(0xCD, 0xF0, zlesione, buffer, 1);
  generalConfiguration.biopsyCfg.standardConf.zlesione = zlesione;
  _mutex_unlock(&biopsy_standard_mutex);
  return TRUE;
}
bool  biopsyStandardSetLago(unsigned char lago)
{
  unsigned char buffer[4];
  
  if(generalConfiguration.biopsyCfg.standardConf.movimento) return FALSE; // E' già in corso
  
  _mutex_lock(&biopsy_standard_mutex);
  Ser422SendRaw(0xCD, 0xF2, lago, buffer, 1);
  generalConfiguration.biopsyCfg.standardConf.lago = lago;
  _mutex_unlock(&biopsy_standard_mutex);
  return TRUE;
}

void biopsyStandardReset(void)
{
  _mutex_lock(&biopsy_standard_mutex);
  verifyBiopsyDataInit = TRUE;
  _mutex_unlock(&biopsy_standard_mutex);
}

void  biopsyStandardStepZ(unsigned char dir, unsigned char val)
{  
  if(dir==1)
  {// Incremento
    generalConfiguration.biopsyCfg.standardConf.stepDwn_z = TRUE;
  }else
  {// Decremento
    generalConfiguration.biopsyCfg.standardConf.stepUp_z = TRUE;
  }  
  generalConfiguration.biopsyCfg.standardConf.movimento=TRUE;

}


/* EOF */
 

