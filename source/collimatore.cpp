#include "application.h"
#include "appinclude.h"
#include "globvar.h"

#define COLLICFG "/resource/config/collimazione.cnf"

void Collimatore::activateConnections(void){
   // connect(pCompressore,SIGNAL(padChangedSgn()),this,SLOT(changedPadNotify()),Qt::UniqueConnection);
    connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(guiNotifySlot(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    connect(pConsole,SIGNAL(mccPcb249U1Notify(unsigned char,unsigned char,QByteArray)),this,SLOT(pcb249U1Notify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
}

Collimatore::Collimatore(QObject *parent) :
    QObject(parent)
{
    alrCuffia = FALSE;
    alrSensCuffia = FALSE;
    accessorio = COLLI_ACCESSORIO_ND;


    mirrorStat = MIRROR_ND; // non determinato
    lampStat = FALSE;       // Lampada Spenta
    filtroStat  = filtroCmd = FILTRO_ND;
    manualCollimation = FALSE;
    manualFiltroCollimation = FALSE;
    manualFilter = FILTRO_Rh;
    manualF = 0;// front
    manualB = 0;// back
    manualL = 0;// Left Blade
    manualR = 0;// Right Blade
    manualT = 40;// back Trap


    // Legge la configurazione ed aggiorna le periferiche interessate
    pConfig->collimator_configured = readConfigFile();
    colli_model = _COLLI_TYPE_NOT_ASSIGNED;
    colliConfUpdated = FALSE; // Configurazione non ancora inviata a dispositivi

    colliTestNumber = 0;
    colliTestTimer = 0;


    collimazione_frustoli = COLLI_FRUSTOLI_ND;

    resetColliFlags();

}

void Collimatore::timerEvent(QTimerEvent* ev)
{

    // Dopo un certo tempo dall'apertura della finestra, questa viene chiusa
    // Alla chiusura vengono azzrati tutti gli allarmi one-shot
    if(ev->timerId()==colliTestTimer)
    {
        if(colliTestNumber==0){
            killTimer(colliTestTimer);
            colliTestTimer=0;
            manualCollimation = false;
            manualFiltroCollimation = false;
            PRINT("timerEvent: UPDATE COLLI");
            updateColli();
            setFiltro();
            return;
        }

        if(filter_test_status){
            manualSetFiltro();
            if(manualFilter == Collimatore::FILTRO_Rh) manualFilter = Collimatore::FILTRO_Al;
            else if(manualFilter == Collimatore::FILTRO_Al) manualFilter = Collimatore::FILTRO_Ag;
            else if(manualFilter == Collimatore::FILTRO_Ag) manualFilter = Collimatore::FILTRO_Cu;
            else manualFilter = Collimatore::FILTRO_Rh;
            filter_test_status = false;
            return;
        }
        filter_test_status = true;

        colliTestNumber--;
        if((colliTestNumber%4)==0){
            if(pCollimatore->manualF==40) pCollimatore->manualF = 0;
            else pCollimatore->manualF = 40;
        }

        if(colliTestStatus){
            colliTestStatus = false;
            pCollimatore->manualB = 190;
            pCollimatore->manualL = 0;
            pCollimatore->manualR = 0;
            pCollimatore->manualT = 80;
        }else{
            colliTestStatus = true;
            pCollimatore->manualB = 0;
            pCollimatore->manualL = 200;
            pCollimatore->manualR = 200;
            pCollimatore->manualT = 0;
        }
        manualColliUpdate();

    }


}

// Attiva un loop mdi formato open close per verificare il collimatore
void Collimatore::startColliTest(unsigned char nseq){

    if(nseq==0){
        // Ferma il test in corso
        colliTestNumber=1;
        return;
    }
    colliTestNumber = nseq;
    colliTestStatus = true;

    filter_test_status = true; // Inizia con il cambio filtro
    manualCollimation = true;
    manualFiltroCollimation=TRUE;
    manualF = 0;
    manualB = 0;
    manualL = 200;
    manualR = 200;
    manualT = 0;
    manualFilter = Collimatore::FILTRO_Rh;
    manualColliUpdate();
    colliTestTimer = startTimer(5000);
}

/*
 *  buffer[0] = SYS_FLAGS0
 *  buffer[1] = raw Temperatura cuffia
 *  buffer[2] = codice identificativo accessorio collimatore
 */
void Collimatore::pcb249U1Notify(unsigned char id, unsigned char notifyCode, QByteArray buffer)
{
    static bool startupEvent = true;

    int   tempCuffia; // Temperatura cuffia in °C

    if(id!=1) return;
    switch(notifyCode)
    {
        case PCB249U1_NOTIFY_DATA:
            if(pConfig->userCnf.demoMode){
                this->tempCuffia = 25;
                tempCuffia = 25; // 25° in demo
                alrCuffia = FALSE;
                alrSensCuffia = FALSE;
            }else{                
                this->tempCuffia = tempCuffia = (int) buffer.at(1);
                if((tempCuffia<5)||(tempCuffia>69)){
                    //if(!alrSensCuffia) PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_GEN, ERROR_SENS_CUFFIA);
                    alrSensCuffia = TRUE;
                }else if(tempCuffia > pConfig->userCnf.tempCuffiaAlr){
                    alrSensCuffia = FALSE;
                    //if(!alrCuffia) PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_GEN, ERROR_CUFFIA_CALDA);
                    alrCuffia = TRUE;
                }else if((alrCuffia) && (tempCuffia > pConfig->userCnf.tempCuffiaAlrOff)){
                    // Continua a rimanere in allarme fino a discesa
                    alrCuffia = TRUE;

                }else{
                    // Nessuna condizione di allarme attiva: reset allarmi
                    //if((alrCuffia)||(alrSensCuffia)) PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_GEN, 0);
                    alrCuffia = FALSE;
                    alrSensCuffia = FALSE;
                }

            }

            // Aggiunta dello status
            if(alrSensCuffia) tempCuffia|=0x0400;
            else if(tempCuffia > pConfig->userCnf.tempCuffiaAlr) tempCuffia|=0x0200;
            else if(tempCuffia > pConfig->userCnf.tempCuffiaAlrOff) tempCuffia|=0x0100;
            ApplicationDatabase.setData(_DB_T_CUFFIA,(int)tempCuffia, 0);

            // Cambio accessorio
            if(buffer.at(2) != accessorio){
                accessorio = buffer.at(2);
                ApplicationDatabase.setData(_DB_ACCESSORIO_COLLIMATORE, (int) accessorio);
                PRINT("pcb249U1Notify: UPDATE COLLI");
                updateColli();
                if(accessorio == COLLI_ACCESSORIO_FRUSTOLI) pToConsole->setSpecimen(true);
                else pToConsole->setSpecimen(false);
            }

            // Auto apprendimento accessorio 3D
            if((accessorio==COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D)&&(startupEvent)){
                if(pConfig->userCnf.enable3DCheckAccessorio == false){
                    pConfig->userCnf.enable3DCheckAccessorio = true;
                    pConfig->saveUserCfg();
                }
            }

            startupEvent = false;
        break;
    }

}

// Restituisce la posizione del PAD + mat nella lista
// o -1 se non lo trova
int Collimatore::getColli2DIndex(int pad)
{
    int i;

    // PAD TOMO
    if(pad == PAD_TOMO_24x30) return -1;

    // PAD 2D
    for(i=0; i< colliConf.colli2D.size();i++)
    {
        if(colliConf.colli2D.at(i).PadCode==pad) return i;
    }
    return -1;
}




/*____________________________________________________________________________________________________________________________
 *                          IMPOSTA LO STATO DELLO SPECCHIO
 *  I Possibili comandi sono: MIRROR_OUT, MIRROR_HOME
 *  La Funzione attiva l'attesa della notifica con l'esito del comando e
 *  con lo stato corrente. La notifica giunge al termine del comando
 */
void Collimatore::setMirror(_MirrorCmd_Enum cmd)
{
   unsigned char data;
   mirrorStat = MIRROR_ND; // Fino ad avvenuta notifica lo stato è indeterminato

   if(cmd == MIRROR_OUT) data = 1;
   else data = 0;
   pConsole->pGuiMcc->sendFrame(MCC_SET_MIRROR,_COLLI_ID,&data, 1);

}

/*____________________________________________________________________________________________________________________________
 *      IMPOSTA LO STATO DELLA LAMPADA + SPECCHIO + Timeout lampada
 *
 *      cmd = Comando: LAMP_OFF,LAMP_ON,LAMPMIRR_OFF,LAMPMIRR_ON
 *                     LAMP_OFF: Spegne la lampada senza muovere lo specchio
 *                     LAMP_ON:  Accende la lampada senza muovere lo specchio
 *                     LAMPMIRR_OFF: Spegne la lampada e fa rientrare lo specchio
 *                     LAMPMIRR_ON:  Accende la lampada e fa uscire lo specchio in campo
 *      tmo = TIMEOUT accensione lampada:
 *            0 = infinito;
 *            1:64 = 0.5 secondi unit, max 32 secondi
 *
 *      La funzione attiva la notifica e al termine del comando
 *      viene impostato lo stato corrente della lampada e dello specchio
 */
void Collimatore::setLamp(_LampCmd_Enum cmd, unsigned char tmo)
{
    unsigned char data[4];

   // Imposta il comando
   data[0] = (unsigned char) cmd;
   data[1] = tmo&0x3F;

   if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01){
       data[2] = (unsigned char) (colliConf.mirrorSteps_ASSY_01&0xFF); // Step Specchio in campo
       data[3] = (unsigned char) ((colliConf.mirrorSteps_ASSY_01>>8)&0xFF); // Step Specchio in campo
   }else{
       data[2] = (unsigned char) (colliConf.mirrorSteps_ASSY_02&0xFF); // Step Specchio in campo
       data[3] = (unsigned char) ((colliConf.mirrorSteps_ASSY_02>>8)&0xFF); // Step Specchio in campo
   }
   lampStat = pConsole->pGuiMcc->sendFrame(MCC_SET_LAMP,_COLLI_ID,data, 4);

}


/*
 *  Questa funzione effettua l'impostazione di tutti i parametri di collimazione
 *  e serve per agevolare il relativo comando ricevuto dalla console
 *  param ==TRUE -> Ritira lo specchio
 *  force == TRUE Forza l'update
 *
 * STRUTTURA MESSAGGIO MCC
 *

   // Collimazione 2D:
   data[3] = Front;// front
   data[4] = Back;// back
   data[5] = Left;// Left Blade
   data[6] = Right;// Right Blade
   data[7] = Trap;// back Trap

   // Collimazione tomo
   data[3] = Front;// front
   data[4] = Back;// back
   data[5:15] = 0° .. 2°.. 20° Left
   data[16:25] = -2° .. -20°   Left
   data[26:36] = 0° .. 2°.. 20° Right
   data[37:46] = -2° .. -20°   Right
   data[47:52] = 0° .. 4°.. 20° Trap
   data[53:57] = -4° .. -20°   Trap


NOTIFY:
    data[0] = Successo/Insuccesso

 *
 */


bool Collimatore::updateColli(void)
{
    unsigned char data[COLLI_LEN];
    int colliIndex;
    QString mat;
    int pad;


    // Controlli preliminari sui comandi
    if(pConfig->collimator_configured==FALSE)
    {
        ApplicationDatabase.setData(_DB_COLLIMAZIONE,QString(QApplication::translate("COLLIMATORE","NON DEFINITA", 0, QApplication::UnicodeUTF8)),0);
        return FALSE;
    }

    // Collimazione manuale attivata
    if(manualCollimation)
    {
        ApplicationDatabase.setData(_DB_COLLIMAZIONE,QString(QApplication::translate("COLLIMATORE","MANUALE", 0, QApplication::UnicodeUTF8)),0);
        return TRUE;
    }




    // In biopsia, con la piastrina per frustoli si utilizza una collimazione dedicata che dipende dall'angolo del braccio
    if((pBiopsy->connected) && (accessorio==COLLI_ACCESSORIO_FRUSTOLI)){

        ApplicationDatabase.setData(_DB_COLLIMAZIONE,"SPECIMEN",0);

        // Se il braccio si trova nella corretta posizione allora imposta la collimazione adeguata
        if(ApplicationDatabase.getDataI(_DB_TRX)>100){
            if(collimazione_frustoli == COLLI_FRUSTOLI_RIGHT) return true;
            data[COLLI_F] = 50;// front
            data[COLLI_B] = 50;// back
            data[COLLI_T] = 100;
            data[COLLI_L] = 255;
            data[COLLI_R] = 0;
            collimazione_frustoli = COLLI_FRUSTOLI_RIGHT;

        }else if(ApplicationDatabase.getDataI(_DB_TRX)<-100){
            if(collimazione_frustoli == COLLI_FRUSTOLI_LEFT) return true;
            data[COLLI_F] = 50;// front
            data[COLLI_B] = 50;// back
            data[COLLI_T] = 0;
            data[COLLI_L] = 0;
            data[COLLI_R] = 255;
            collimazione_frustoli = COLLI_FRUSTOLI_LEFT;
        }else{
            if(collimazione_frustoli == COLLI_FRUSTOLI_CENTER) return true;
            data[COLLI_F] = 50;// front
            data[COLLI_B] = 50;// back
            data[COLLI_T] = 50;
            data[COLLI_L] = 0;
            data[COLLI_R] = 0;
            collimazione_frustoli = COLLI_FRUSTOLI_CENTER;
        }



        // Invio comando
        if(pConsole->pGuiMcc->sendFrame(MCC_SET_COLLI,_COLLI_ID,data, COLLI_LEN)==FALSE)
        {
            LOG("MCC COLLI FRUSTOLI FALLITO");
            collimazione_frustoli = COLLI_FRUSTOLI_ND;
            return FALSE;
        }

        return TRUE;
    }


    // In modo operativo necessariamente ci deve essere una collimazione pre-definita.
    collimazione_frustoli = COLLI_FRUSTOLI_ND;

    // Non � ammessa una collimazione OPEN
    if(pConsole->isOperatingMode())
    {
        // Se non c'� potter e non c'e la Biopsia non viene effettuata nessuna collimazione
        if((!pPotter->isValid())&&(!pBiopsy->connected)){

            ApplicationDatabase.setData(_DB_COLLIMAZIONE,QString(QApplication::translate("COLLIMATORE","NON DEFINITA", 0, QApplication::UnicodeUTF8)),0);
            return FALSE;
        }

        pad = pCompressore->getPad();

        // Con il compressore sbloccato non viene effettuata nessuna collimazione
        if(pad>=PAD_ENUM_SIZE){
            ApplicationDatabase.setData(_DB_COLLIMAZIONE,QString(QApplication::translate("COLLIMATORE","NON DEFINITA", 0, QApplication::UnicodeUTF8)),0);
            return FALSE;
        }

        // Deriva il codice di collimazione
        if(pad!=PAD_TOMO_24x30)  colliIndex = getColli2DIndex(pad);
        else colliIndex = getColli2DIndex(PAD_24x30);

    }else
    {

        // In modalit�  non operativa utilizza la colimazione OPEN
        colliIndex=-1; // Collimazione OPEN in calibrazione
        ApplicationDatabase.setData(_DB_COLLIMAZIONE,QString(QApplication::translate("COLLIMATORE","APERTA", 0, QApplication::UnicodeUTF8)),0);
    }


   // Formato
   if(colliIndex<0) // OPEN
   {
       // Apre tutto il collimatore
       data[COLLI_F] = colliConf.colliOpen.F;// front
       data[COLLI_B] = colliConf.colliOpen.B;// back
       data[COLLI_L] = colliConf.colliOpen.L;// Left Blade
       data[COLLI_R] = colliConf.colliOpen.R;// Right Blade
       data[COLLI_T] = colliConf.colliOpen.T;// back Trap
   }else
   {
       ApplicationDatabase.setData(_DB_COLLIMAZIONE,pCompressore->getPadName((Pad_Enum)pad),0);
       _colliPadStr colli2D = colliConf.colli2D.at(colliIndex);

       // Carica le posizioni
       data[COLLI_F] = colli2D.F;// front
       data[COLLI_B] = colli2D.B;// back
       data[COLLI_L] = colli2D.L;// Left Blade
       data[COLLI_R] = colli2D.R;// Right Blade
       data[COLLI_T] = colli2D.T;// back Trap

   }

   PRINT(QString("CL:%1(%2), CR:%3(%4), CT:%5(%6) F:%7(%8) B:%9(%10)").arg(data[COLLI_L]).arg(cursen_left).arg(data[COLLI_R]).arg(cursen_right).arg(data[COLLI_T]).arg(cursen_trap).arg(data[COLLI_F]).arg(cursen_front).arg(data[COLLI_B]).arg(cursen_back));
   // Verifica se c'� stato un cambio di lame
   if(
       (data[COLLI_F] == cursen_front)&&
       (data[COLLI_B] == cursen_back)&&
       (data[COLLI_T] == cursen_trap)&&
       (data[COLLI_L] == cursen_left)&&
       (data[COLLI_R] == cursen_right)
      ){

       PRINT("UPDATE-COLLI: GIA COLLIMATO!");
       return true;
   }

   if(pConsole->pGuiMcc->sendFrame(MCC_SET_COLLI,_COLLI_ID,data, COLLI_LEN)==FALSE)
   {
       LOG("MCC COLLI COMMAND FALLITO");
       return FALSE;
   }
   cursen_front = data[COLLI_F];
   cursen_back = data[COLLI_B];
   cursen_trap = data[COLLI_T];
   cursen_left = data[COLLI_L];
   cursen_right = data[COLLI_R];

   return TRUE;
}

// Notifica di avvenuto cambio PAD: effettua un update della collimazione corrente
void Collimatore::changedPadNotify(void)
{
    PRINT("changedPadNotify: UPDATE COLLI");
    updateColli();

}

// Imposta il filtro utilizzando una tag tra quelle definite
// cmd � uno tra i codici filtro disponibili
// update==true impone la selezione del filtro
bool Collimatore::setFiltro(_FilterCmd_Enum cmd, bool update)
{
    unsigned char data[2];
    int i;

    // Collimazione manuale attivata
    if(manualFiltroCollimation)
    {
        DEBUG("setFiltro: collimatore in modo  manuale");
        return TRUE;
    }

    // Filtro fisso, non deve fare nulla
    if(!pConfig->sys.autoFilter){
        return true;
    }


    // Controlli preliminari sui comandi
    if(pConfig->collimator_configured==FALSE)
    {
        LOG("setFiltro: collimatore non configurato");
        return FALSE;
    }

    // Verifica se il filtro esiste
    for(i=0;i<4;i++) if(pCollimatore->colliConf.filterType[i] == cmd) break;
    if(i==4) return false;

    // Il filtro esiste
    filtroCmd = cmd;

    // non procede con l'effettiva impostazione del filtro (successivamente occorrer� un udate
    if(update==false) return true;
    if(filtroStat == filtroCmd) return true; // Filtro gi� selezionato

    data[0] = i;
    data[1] = pCollimatore->colliConf.filterPos[i];

    // Invio comando
    if(pConsole->pGuiMcc->sendFrame(MCC_SET_FILTRO,_COLLI_ID,data, sizeof(data))==FALSE)
    {
        LOG("MCC SET FILTRO FALLITO");
        return FALSE;
    }

    return TRUE;

}

/* Imposta esclusivamente il filtro */
bool Collimatore::setFiltro(void)
{
    unsigned char data[2];
    int i;

    DEBUG("COLLIMATORE: COMANDO SetFiltro()");

    // Collimazione manuale attivata
    if(manualFiltroCollimation)
    {
        DEBUG("COLLIMATORE: collimatore in modo manuale");
        return TRUE;
    }

    // Filtro fisso, non deve fare nulla
    if(!pConfig->sys.autoFilter){
        return true;
    }

    // Controlli preliminari sui comandi
    if(pConfig->collimator_configured==FALSE)
    {
        LOG("COLLIMATORE: collimatore non configurato");
        return FALSE;
    }

    // Se il filtro non è definito, non procede con l'impostazione
    if(filtroCmd == FILTRO_ND){
        LOG("COLLIMATORE: Filtro Non definito");
        return TRUE;
    }

    // Selezione la posizione del filtro
    for(i=0;i<4;i++) if(pCollimatore->colliConf.filterType[i] == filtroCmd) break;
    data[0] = i;
    data[1] = pCollimatore->colliConf.filterPos[i];

    PRINT(QString("COLLIMATORE: FILTRO INDEX=%1, POS=%2").arg(data[0]).arg(data[1]));

    if((data[0] == filterIndexExecuted) && (data[1] == filterPosExecuted) ){
        DEBUG("COLLIMATORE: FILTRO GIA IMPOSTATO");
        return true;
    }

    // Invio comando
    if(pConsole->pGuiMcc->sendFrame(MCC_SET_FILTRO,_COLLI_ID,data, sizeof(data))==FALSE)
    {
        LOG("MCC SET FILTRO FALLITO");
        return FALSE;
    }

    return TRUE;

}

QString Collimatore::getFiltroTag(unsigned char code)
{
    if(code == FILTRO_Ag) return "Ag";
    else if(code == FILTRO_Rh) return "Rh";
    else if(code == FILTRO_Al) return "Al";
    else if(code == FILTRO_Cu) return "Cu";
    else if(code == FILTRO_Mo) return "Mo";
    else return "";
}


/* Imposta esclusivamente il filtro */
bool Collimatore::manualSetFiltro(void)
{
    unsigned char data[2];
    int i;

    // Controlli preliminari sui comandi
    if(pConfig->collimator_configured==FALSE)
    {
        LOG("setFiltro: collimatore non configurato");
        return FALSE;
    }

    // Collimazione manuale attivata
    if(!manualFiltroCollimation)
    {
        LOG("manualSetFiltro: collimatore in AUTOMATICO");
        return FALSE;
    }

    // Filtro fisso, non deve fare nulla
    if(!pConfig->sys.autoFilter){
        LOG("manualSetFiltro: Collimatore con Filtro Fisso");
        return false;
    }

    // Selezione la posizione del filtro
    for(i=0;i<4;i++) if(pCollimatore->colliConf.filterType[i] == manualFilter) break;
    data[0] = i;
    data[1] = pCollimatore->colliConf.filterPos[i];

    // Invio comando
    return pConsole->pGuiMcc->sendFrame(MCC_SET_FILTRO,_COLLI_ID,data, sizeof(data));

}

// Imposta il filtro in base alla posizione numerica da 0 a 3
// il codice del filtro relativo viene scritto di conseguenza in funzione dell'impostazione corrente
bool Collimatore::manualSetFiltro(unsigned char index)
{
    unsigned char data[2];

    if(index>3) return false;


    // With Fixed filter there is not the manual filter activation
    if(!pConfig->sys.autoFilter){
        return FALSE;
    }

    // Collimazione manuale attivata
    if(!manualFiltroCollimation)
    {
        return FALSE;
    }


    if(!pConfig->sys.autoFilter){

        return false;
    }

    // Selezione la posizione del filtro
    data[0] = index;
    data[1] = pCollimatore->colliConf.filterPos[index];

    // Invio comando
    if(pConsole->pGuiMcc->sendFrame(MCC_SET_FILTRO,_COLLI_ID,data, sizeof(data))==FALSE)
    {
        return FALSE;
    }


    manualFilter = pCollimatore->colliConf.filterType[index];
    return TRUE;

}

bool Collimatore::manualColliUpdate(void)
{
    unsigned char data[COLLI_LEN];

    // Collimazione manuale attivata
    if(manualCollimation==FALSE) return FALSE;

    // Imposta a display la collimazione Manuale
    ApplicationDatabase.setData(_DB_COLLIMAZIONE,QString("MANUALE"),0);

    // Apre tutto il collimatore
    data[COLLI_F] = manualF;// front
    data[COLLI_B] = manualB;// back
    data[COLLI_L] = manualL;// Left Blade
    data[COLLI_R] = manualR;// Right Blade
    data[COLLI_T] = manualT;// back Trap

    collimazione_frustoli = COLLI_FRUSTOLI_ND;

    // Invio comando
    return pConsole->pGuiMcc->sendFrame(MCC_SET_COLLI,_COLLI_ID,data, COLLI_LEN);
}

void Collimatore::guiNotifySlot(unsigned char id, unsigned char mcccode, QByteArray buffer)
{
    if(id!=_COLLI_ID) return;
    switch(mcccode)
    {
        case MCC_CALIB_FILTRO:
            // Notifica delle posizioni correnti sul device PCB249U2 del filtro
            // Verifica se è necessario salvare nel file di configurazione i nuovi valori
            // pCollimatore->colliConf.filterPos[4] indica se i dati si riferiscono ad un device
            // calibrato. Se non è cosi' essi NON devono essere salvati
            pCollimatore->colliConf.filterPos[0] = buffer.at(0);
            pCollimatore->colliConf.filterPos[1] = buffer.at(1);
            pCollimatore->colliConf.filterPos[2] = buffer.at(2);
            pCollimatore->colliConf.filterPos[3] = buffer.at(3);
            pCollimatore->storeConfigFile();
            break;
        case MCC_SET_LAMP:
            if(buffer.at(1)==1) lampStat=TRUE;
            else lampStat=FALSE;

            if(buffer.at(2)==0) mirrorStat = MIRROR_HOME;
            else if(buffer.at(2)==1) mirrorStat = MIRROR_OUT;
            else  mirrorStat = MIRROR_ND;

            if(buffer.at(0)==0)
            {
                // CONDIZIONE DI ERRORE
                PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_COLLI, COLLI_LAMP_FALLITO, TRUE);
                return;
            }
        break;

    case MCC_SET_FILTRO:

        // Filtro fisso, non deve fare nulla
        if(!pConfig->sys.autoFilter){
            return ;
        }

        if(buffer.at(0)==0)
        {
            LOG("COLLIMATORE: POSIZIONAMENTO FILTRO FALLITO");
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_COLLI, COLLI_FILTRO_FALLITO, TRUE);
            filterIndexExecuted = 255;
            filterPosExecuted = 255;
            return;
        }else
        {
            filterIndexExecuted = buffer.at(1);
            filterPosExecuted = buffer.at(3);
            if(manualFiltroCollimation){
                filtroStat = (_FilterCmd_Enum) manualFilter;
                PRINT(QString("COLLIMATORE FILTRO MANUALE OK, IDX=%1, POS=%2").arg(filterIndexExecuted).arg(filterPosExecuted));
            }else{
                filtroStat = filtroCmd; // Aggiorna lo stato del filtro
                PRINT(QString("COLLIMATORE FILTRO AUTO OK, IDX=%1, POS=%2").arg(filterIndexExecuted).arg(filterPosExecuted));
            }
        }
    break;

    case MCC_SET_COLLI: // Notifica dal processo di collimazione
        // buffer.at(0) = esito collimazione: 1=OK, 0=NOK
        // buffer.at(1) = processo di collimazione: 1= laterali, 0=frontali+back
        // buffer.at(2) = left/frontf;
        // buffer.at(3) = right/back
        // buffer.at(4) = trap

        if(buffer.at(1)==1){
            // Collimazione lame laterali
            if(buffer.at(0)==0){
               PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_COLLI, COLLI_UPDATE_FALLITO, TRUE);
               LOG("COLLIMATORE ERRORE COLLIMAZIONE LAME LATERALI");
               cursen_left = 255;
               cursen_right = 255;
               cursen_trap =255;

            }else{
                cursen_left = buffer.at(2);
                cursen_right = buffer.at(3);
                cursen_trap = buffer.at(4);
                PRINT(QString("COLLIMATORE: LAME LATERALI OK. L=%1, R=%2, T=%3").arg(cursen_left).arg(cursen_right).arg(cursen_trap));
            }
        }else{
            // Collimazine lama frontale + back
            if(buffer.at(0)==0){
               PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_COLLI, COLLI_UPDATE_FALLITO, TRUE);
               LOG("COLLIMATORE ERRORE COLLIMAZIONE FRONTE/RETRO");
               cursen_front = 255;
               cursen_back = 255;
            }else{
                cursen_front = buffer.at(2);
                cursen_back = buffer.at(3);
                PRINT(QString("COLLIMATORE: LAME F+B OK. F=%1, B=%2").arg(cursen_front).arg(cursen_back));
            }
        }

    break;

    case MCC_SET_MIRROR:
        if(buffer.at(0)==0)
        {
            // Notifica l'errore a monitor
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_COLLI, COLLI_SPECCHIO_FALLITO, TRUE);
            return;
        }else{
            PRINT("COLLIMATORE: IMPOSTAZIONE FILTRO COMPLETATA");
        }
    break;

    }

    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//      COLLIMAZIONE DINAMICA
#define _RAD(x) ((float) x * 3.141593 / 180.0)
#define APPROX(x) ((((x)-floor(x))>0.5) ? ceil(x): floor(x))

#define FC 613      // Distanza fuoco-centro di rotazione (mm)
#define CD 45       // Distanza centro di rotazione - piano del detector (mm)
#define LF 150      // Distanza Lame-Fuoco (mm)
#define rC 0.109    // 32/256 Risoluzione spostamento lame (mm/unit)



/*
 *  angolo: angolo in gradi
 *  dispersione: dispersione desiderata in millimetri oltre il bordo sinistro del detector. Se negativi,le lame entrano in campo
 *  nRef: posizione di riferimento in calibrazione della lama sinistra
 *  footprint: misura in millimetri dell'ombra della lama sinistra in corrispondenza di nRef
 */
unsigned char Collimatore::getDynamicColliTargetL(float angolo, float dispersione, unsigned char nRef, float footprint){
    float  U = atan((150 + dispersione) / CD);
    float  teta= -1* _RAD(angolo) + U;
    float  NTARGET = nRef - LF * (dispersione+footprint) / (rC * (FC + CD)); // Target N per apertura a 0° per avere le lame alla dispersione voluta
    float  A = sqrt((150+dispersione)*(150+dispersione)+CD*CD);
    float  A0 = LF * (sin(teta) / (FC/A + cos(teta)));
    return (unsigned char) APPROX(NTARGET - (LF * (sin(teta) / (FC/A + cos(teta))) - A0) / rC);
}

/*
 *  angolo: angolo in gradi
 *  dispersione: dispersione desiderata in millimetri oltre il bordo destro del detector. Se negativi,le lame entrano in campo
 *  nRef: posizione di riferimento in calibrazione della lama destra
 *  footprint: misura in millimetri dell'ombra della lama destra in corrispondenza di nRef
 */
unsigned char Collimatore::getDynamicColliTargetR(float angolo, float dispersione, unsigned char nRef, float footprint){
    float  U = atan((150 + dispersione) / CD);
    float  teta= _RAD(angolo) + U;
    float  NTARGET = nRef - LF * (dispersione+footprint) / (rC * (FC + CD)); // Target N per apertura a 0° per avere le lame alla dispersione voluta
    float  A = sqrt((150+dispersione)*(150+dispersione)+CD*CD);
    float  A0 = LF * (sin(teta) / (FC/A + cos(teta)));
    return (unsigned char) APPROX(NTARGET - (LF * (sin(teta) / (FC/A + cos(teta))) - A0) / rC);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONFIGURATION FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
 *  Nel caso in cui non vi siano PAD del materiale selezionato, NON viene generata una collimazione di default
 *  Tomografica. La tomografia per tale materiale viene mantenuta disabilitata
 */
void Collimatore::setTomoConfigDefault(QString mat){
    int i;
    colliTomoConf_Str* pTomo;
    bool padPresente = false;
    unsigned char trap;

    // Ricerca del PAD_24x30 sul materiale richiesto
    for(i=0;i<colliConf.colli2D.size();i++){        
        if(colliConf.colli2D.at(i).PadCode!=PAD_24x30) continue;
        trap = colliConf.colli2D.at(i).T;
        padPresente=true;
        break;
    }

    if(!padPresente) return;

    if(mat=="W") pTomo = &colliConf.colliTomoW;
    else pTomo = &colliConf.colliTomoMo;

    pTomo->enabled=true;
    for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++){
        pTomo->tomoLeftBladeP[i] = 0;
        pTomo->tomoLeftBladeN[i] = 0;
        pTomo->tomoRightBladeP[i] = 0;
        pTomo->tomoRightBladeN[i] = 0;
        pTomo->tomoBackTrapP[i] = trap+(trap*i)/COLLI_DYNAMIC_SAMPLES;
        if((trap-(trap*i)/COLLI_DYNAMIC_SAMPLES)>0) pTomo->tomoBackTrapN[i] = trap-(trap*i)/COLLI_DYNAMIC_SAMPLES;
        else pTomo->tomoBackTrapN[i] = 0;
    }
    pTomo->tomoFront = 0;
    pTomo->tomoBack = 0;

}


bool Collimatore::readConfigFile(void)
{
    QString filename;
    _colliPadStr colli24x30;
    QList<QString> dati;
    int i=0;
    colliTomoConf_Str* pTomo;
    int pad;
    int fileRevision = COLLI_CNF_REV;

    filename =  QString(COLLICFG);

    QFile file(filename.toAscii());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    // Init di tutte le grandezze legate al collimatore a valori di default
    colliConf.filterType[0]=FILTRO_Rh;
    colliConf.filterType[1]=FILTRO_Ag;
    colliConf.filterType[2]=FILTRO_Al;
    colliConf.filterType[3]=FILTRO_Cu;

    colliConf.filterPos[0] = 34;
    colliConf.filterPos[1] = 97;
    colliConf.filterPos[2] = 159;
    colliConf.filterPos[3] = 224;

    colliConf.filterTomo[0] = 1; // 1= Enabled, 0 = Disabled
    colliConf.filterTomo[1] = 2; // angolo per +1
    colliConf.filterTomo[2] = 10;// angolo per +2
    colliConf.filterTomo[3] = 21;// angolo per +3


    // Impostazione dipendente dal tipo di collimatore montato
    colliConf.mirrorSteps_ASSY_01 = 1530;
    colliConf.mirrorSteps_ASSY_02 = 1900;

    colliConf.colli2D.clear();

    colliConf.colliOpen.PadCode = PAD_ENUM_SIZE;
    colliConf.colliOpen.L = 0;
    colliConf.colliOpen.R = 0;
    colliConf.colliOpen.F = 0;
    colliConf.colliOpen.B = 0;
    colliConf.colliOpen.T = 45;

    colliConf.colliTomoMo.enabled=true;
    colliConf.colliTomoW.enabled=true;

    customL = 0;
    customR = 0;
    customT = 50;
    customB = 0;
    customF = 0;

    bool tomoW=false;
    bool tomoMo=false;

    // Procede con la lettura del formato corrente
    while(1){

        dati = Config::getNextArrayFields(&file);
        if(dati.isEmpty()) break;

        if(dati.at(0)=="REV"){
            fileRevision = dati.at(1).toInt();
        }

        // Assegnazione tipologia filtri
        if(dati.at(0)=="FILTRI"){
            for(i=1;i<5;i++)
            {
                if(dati.at(i)=="Rh") colliConf.filterType[i-1]=FILTRO_Rh;
                else if(dati.at(i)=="Ag") colliConf.filterType[i-1]=FILTRO_Ag;
                else if(dati.at(i)=="Al") colliConf.filterType[i-1]=FILTRO_Al;
                else if(dati.at(i)=="Mo") colliConf.filterType[i-1]=FILTRO_Mo;
                else colliConf.filterType[i-1]=FILTRO_Cu;
            }
            continue;
        }

        // Configurazione posizione slots filtri
        if(dati.at(0)=="PFILTRI"){
            for(i=0;i<4;i++) colliConf.filterPos[i] = (unsigned char) dati.at(i+1).toInt();
            continue;
        }

        // Hotfix 11C
        // Configurazione posizione Tomo filtri
        if(dati.at(0)=="TFILTRI"){
            for(i=0;i<4;i++) colliConf.filterTomo[i] = (unsigned char) dati.at(i+1).toInt();
            continue;
        }


        // Configurazione step specchio
        if(dati.at(0)=="SPECCHIO"){
            colliConf.mirrorSteps_ASSY_01 = dati.at(1).toInt();

            // Aggiunge gli steps in campo per il nuovo assy
            if(dati.size()==3)
                colliConf.mirrorSteps_ASSY_02 = dati.at(2).toInt();
            continue;
        }

        // Collimazione OPEN
        if(dati.at(0)=="OPEN"){
            colliConf.colliOpen.PadCode = PAD_ENUM_SIZE;
            colliConf.colliOpen.L = (unsigned char) dati.at(1).toInt();
            colliConf.colliOpen.R = (unsigned char) dati.at(2).toInt();
            colliConf.colliOpen.F = (unsigned char) dati.at(3).toInt();
            colliConf.colliOpen.B = (unsigned char) dati.at(4).toInt();
            colliConf.colliOpen.T = (unsigned char) dati.at(5).toInt();
            continue;
        }

        // ---------------------------- Collimazioni 2D --------------------------------------- //
        if(dati.at(0).contains("PAD")){

            // Legge il codice numerico del PAD associato al tag alfanumerico
            pad = pCompressore->getPadCodeFromTag(dati.at(0));
            if(pad==-1) continue;           

            // Verifica duplicazione o se il numero di parametri non corretto
            if((getColli2DIndex(pad)!=-1) ||(dati.size()!=7)) continue;

            // Crea il nuovo item di collimazione
            _colliPadStr newColli2DItem;
            newColli2DItem.PadCode = pad;
            newColli2DItem.L = (unsigned char) dati.at(2).toInt();
            newColli2DItem.R = (unsigned char) dati.at(3).toInt();
            newColli2DItem.F = (unsigned char) dati.at(4).toInt();
            newColli2DItem.B = (unsigned char) dati.at(5).toInt();
            newColli2DItem.T = (unsigned char) dati.at(6).toInt();
            colliConf.colli2D.append(newColli2DItem);
            if(pad == PAD_24x30) colli24x30 = newColli2DItem;
            continue;
        }

        // ---------------------------- Collimazioni Custom --------------------------------------- //
        if(dati.at(0).contains("CUSTOM")){
            customL = dati.at(1).toInt();
            customR = dati.at(2).toInt();
            customF = dati.at(3).toInt();
            customB = dati.at(4).toInt();
            customT = dati.at(5).toInt();
            continue;
        }

        // ---------------------------- Collimazioni Tomo --------------------------------------- //
        if(dati.at(0).contains("TOMO")){

            // Assegnazione dell'array materiale corretto e abilitazione della tomo
            if(dati.at(1)==W_STR)  tomoW = true;
            else tomoMo = true;

            if(dati.at(1)==W_STR) pTomo = &colliConf.colliTomoW;
            else pTomo = &colliConf.colliTomoMo;
            if(pTomo->enabled==false) continue;

            if(dati.at(2)=="L")
            {
                if(dati.size()!=4+COLLI_DYNAMIC_SAMPLES) {pTomo->enabled=false; continue;}
                if(dati.at(3)=="P")  for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++) pTomo->tomoLeftBladeP[i] = (unsigned char) dati.at(i+4).toInt();
                else for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++) pTomo->tomoLeftBladeN[i] = (unsigned char) dati.at(i+4).toInt();
            }else if(dati.at(2)=="R")
            {
                if(dati.size()!=4+COLLI_DYNAMIC_SAMPLES) {pTomo->enabled=false; continue;}
                if(dati.at(3)=="P")  for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++) pTomo->tomoRightBladeP[i] = (unsigned char) dati.at(i+4).toInt();
                else for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++) pTomo->tomoRightBladeN[i] = (unsigned char) dati.at(i+4).toInt();

            }else if(dati.at(2)=="T")
            {
                if(dati.size()!=4+COLLI_DYNAMIC_SAMPLES) {pTomo->enabled=false; continue;}
                if(dati.at(3)=="P")  for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++) pTomo->tomoBackTrapP[i] = (unsigned char) dati.at(i+4).toInt();
                else for(i=0;i<COLLI_DYNAMIC_SAMPLES;i++) pTomo->tomoBackTrapN[i] = (unsigned char) dati.at(i+4).toInt();
            }else if(dati.at(2)=="FB")
            {
                if(dati.size()!=5) {pTomo->enabled=false; continue;}
                pTomo->tomoFront = (unsigned char) dati.at(3).toInt();
                pTomo->tomoBack = (unsigned char) dati.at(4).toInt();
            }else {pTomo->enabled=false; continue;}

        }
    }

    // Nel caso la Tomo non sia correttamente configurata verr�  impostato il default
    if((colliConf.colliTomoW.enabled==false)||(!tomoW)) setTomoConfigDefault("W");
    if((colliConf.colliTomoMo.enabled==false)||(!tomoMo)) setTomoConfigDefault("Mo");


    file.close();

    // C'� stata una variazione di revisione
    if( fileRevision != COLLI_CNF_REV){

        if(fileRevision<= 2){
            // Crea il nuovo item di collimazione
            _colliPadStr newColli2DItem;
            newColli2DItem = colli24x30;
            newColli2DItem.PadCode = PAD_PROSTHESIS;
            colliConf.colli2D.append(newColli2DItem);

            newColli2DItem.PadCode = PAD_9x9_MAG;
            colliConf.colli2D.append(newColli2DItem);
        }

        // Salvataggio
        storeConfigFile();
    }

    return true;
}




bool Collimatore::storeConfigFile(void)
{
    QString filename;
    QString filenamecpy;
    QString command;
    QString frame;

    int i=0;

    filename =  QString(COLLICFG);
    //filenamecpy = "collitemp.cnf";

    // Copia il file da modificare in file.cnf.<date time> per sicurezza
    //command = QString("cp %1 %1.%2.%3").arg(filename).arg(QDateTime::currentDateTime().toString("dd.MM.yy")).arg(QDateTime::currentDateTime().toString("hh.mm.ss"));
    //system(command.toStdString().c_str());

    QFile file(filename.toAscii());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        DEBUG(QString("Collimatore::storeConfigFile: IMPOSSIBILE APRIRE IL FILE IN SCRITTURA: ") + filename);
        return FALSE;
    }

    // Revisione corrente
    QString data = QString("<REV,%1>\n\n").arg((int)COLLI_CNF_REV);
    file.write(data.toAscii().data());

    data = QString("<FILTRI,%1,%2,%3,%4>\n").arg(getFilterTag(colliConf.filterType[0])).arg(getFilterTag(colliConf.filterType[1])).arg(getFilterTag(colliConf.filterType[2])).arg(getFilterTag(colliConf.filterType[3]));
    file.write(data.toAscii().data());

    data = QString("<PFILTRI,%1,%2,%3,%4>\n").arg(colliConf.filterPos[0]).arg(colliConf.filterPos[1]).arg(colliConf.filterPos[2]).arg(colliConf.filterPos[3]);
    file.write(data.toAscii().data());

    // Hotfix 11C
    data = QString("<TFILTRI,%1,%2,%3,%4>\n").arg(colliConf.filterTomo[0]).arg(colliConf.filterTomo[1]).arg(colliConf.filterTomo[2]).arg(colliConf.filterTomo[3]);
    file.write(data.toAscii().data());


    data = QString("<SPECCHIO,%1,%2>\n\n").arg(colliConf.mirrorSteps_ASSY_01).arg(colliConf.mirrorSteps_ASSY_02);
    file.write(data.toAscii().data());

    // Collimazione 2D
    for(i=0; i<colliConf.colli2D.size();i++)
    {
        data = QString("<%1,%2,%3,%4,%5,%6,%7>\n").arg(pCompressore->getPadTag((Pad_Enum) (colliConf.colli2D[i].PadCode))).arg(QString("W")).arg((int) colliConf.colli2D[i].L).arg((int) colliConf.colli2D[i].R).arg((int) colliConf.colli2D[i].F).arg((int) colliConf.colli2D[i].B).arg((int) colliConf.colli2D[i].T);
        file.write(data.toAscii().data());
    }

    // Collimazione OPEN
    data = QString("<OPEN,%1,%2,%3,%4,%5>\n\n").arg((int) colliConf.colliOpen.L).arg((int) colliConf.colliOpen.R).arg((int) colliConf.colliOpen.F).arg((int) colliConf.colliOpen.B).arg((int) colliConf.colliOpen.T);
    file.write(data.toAscii().data());

    // Collimazione Custom
    data = QString("<CUSTOM,%1,%2,%3,%4,%5>\n\n").arg((int) customL).arg((int) customR).arg((int) customF).arg((int) customB).arg((int) customT);
    file.write(data.toAscii().data());


    ///////////////////////////////////////////////////////////////
    // COLLIMAZIONE TOMOGRAFICA
    ///////////////////////////////////////////////////////////////

    // Verifica collimazione W
    if(colliConf.colliTomoW.enabled)
    {
        data = QString("<TOMO,W,L,P");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoW.tomoLeftBladeP[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,W,L,N");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoW.tomoLeftBladeN[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,W,R,P");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoW.tomoRightBladeP[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,W,R,N");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoW.tomoRightBladeN[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,W,T,P");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoW.tomoBackTrapP[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,W,T,N");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoW.tomoBackTrapN[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        // Front + Back
        data = QString("<TOMO,W,FB,%1,%2>\n\n").arg((int) colliConf.colliTomoW.tomoFront).arg((int) colliConf.colliTomoW.tomoBack);
        file.write(data.toAscii().data());
    }

    // Verifica collimazione Mo
    if(colliConf.colliTomoMo.enabled)
    {
        data = QString("<TOMO,Mo,L,P");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoMo.tomoLeftBladeP[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,Mo,L,N");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoMo.tomoLeftBladeN[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,Mo,R,P");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoMo.tomoRightBladeP[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,Mo,R,N");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoMo.tomoRightBladeN[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,Mo,T,P");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoMo.tomoBackTrapP[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        data = QString("<TOMO,Mo,T,N");
        for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) data.append(QString(",%1").arg((int) colliConf.colliTomoMo.tomoBackTrapN[i]));
        data.append(">\n");
        file.write(data.toAscii().data());

        // Front + Back
        data = QString("<TOMO,Mo,FB,%1,%2>\n").arg((int) colliConf.colliTomoMo.tomoFront).arg((int) colliConf.colliTomoMo.tomoBack);
        file.write(data.toAscii().data());
    }

    file.flush();
    file.close();


    // Copia il file modificato nel file finale
    //command = QString("mv %1 %2").arg(filenamecpy).arg(filename);
    //system(command.toStdString().c_str());

    // SYNC
    command = QString("sync");
    system(command.toStdString().c_str());
    return TRUE;
}

/*
 * Richiede la selezione di una collimazione standard/ custom
 * mandando il sistema in modalit� di collimazione manuale
 */
void Collimatore::selectManualColliFormat(unsigned char pad){
    int index;

    if(pad == PAD_ENUM_SIZE){
        // Imposta la collimazione Custom
        manualF = customF;  // front
        manualB = customB;  // back
        manualL = customL;  // Left Blade
        manualR = customR;  // Right Blade
        manualT = customT; // back Trap
        manualCollimation = true;

    }else if(pad<PAD_ENUM_SIZE){
        manualCollimation = true;
        // Impostazione di una delle collimazioni possibili
        // Deriva il codice di collimazione
        if(pad!=PAD_TOMO_24x30)  index = getColli2DIndex(pad);
        else index = getColli2DIndex(PAD_24x30);

        if(index<0){

            manualF = 0;  // front
            manualB = 0;  // back
            manualL = 0;  // Left Blade
            manualR = 0;  // Right Blade
            manualT = 50; // back Trap

        }else{
            _colliPadStr colli2D = colliConf.colli2D.at(index);
            // Carica le posizioni
            manualF = colli2D.F;// front
            manualB = colli2D.B;// back
            manualL = colli2D.L;// Left Blade
            manualR = colli2D.R;// Right Blade
            manualT = colli2D.T;// back Trap
        }

    }
}

// Effettua il reset dei flags di collimazione Filtro acquisiti
// per avere una strategia di correzione da eventuali disallineamenti
// tra driver e Gui
void Collimatore::resetColliFlags(void){

    PRINT("COLLIMATORE: RESET COLLI FLAGS");

    filterIndexExecuted = 255;
    filterPosExecuted = 255;
    cursen_left = 255;
    cursen_right = 255;
    cursen_trap =255;
}


