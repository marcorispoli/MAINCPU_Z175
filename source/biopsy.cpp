#include "application.h"
#include "appinclude.h"
#include "globvar.h"
#include "ui_biopsy.h"

#define UI_PAGINA _PG_BIOPSY_DEVICE


void biopsy::activateConnections(void){
    connect(pConsole, SIGNAL(mccBiopsyNotify(unsigned char,unsigned char,QByteArray)), this, SLOT(mccStatNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
}

biopsy::biopsy(int rotview, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::biopsyUI)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    proxy = scene->addWidget(this);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800,480);    // Dimensione della vista
    scene->setSceneRect(0,0,800,480);
    view->rotate(rotview);       // Angolo di rotazione della vista corrente
    view->setAlignment(Qt::AlignRight);
    view->setScene(scene);
    this->parent = parent;
    this->rotview = rotview;

    connect(&GWindowRoot,SIGNAL(changePage(int,int)), this,SLOT(changePage(int,int)),Qt::UniqueConnection);

    connect(ui->buttonConfirmScrollCenter,SIGNAL(released()),this,SLOT(onConfirmButton()),Qt::UniqueConnection);
    connect(ui->buttonConfirmScrollLeft,SIGNAL(released()),this,SLOT(onConfirmButton()),Qt::UniqueConnection);
    connect(ui->buttonConfirmScrollRight,SIGNAL(released()),this,SLOT(onConfirmButton()),Qt::UniqueConnection);

    ui->frameMoveScrollCenter->setGeometry(0,0,800,480);
    ui->frameMoveScrollLeft->setGeometry(0,0,800,480);
    ui->frameMoveScrollRight->setGeometry(0,0,800,480);

    ui->frameMoveScrollYCenter->setGeometry(0,0,800,480);
    ui->frameMoveScrollYLeft->setGeometry(0,0,800,480);
    ui->frameMoveScrollYRight->setGeometry(0,0,800,480);

    ui->frameMoveXCenterToHome->setGeometry(0,0,800,480);
    ui->frameMoveXLeftToHome->setGeometry(0,0,800,480);
    ui->frameMoveXRightToHome->setGeometry(0,0,800,480);
    ui->frameMoveYToHome->setGeometry(0,0,800,480);
    ui->frameMoveZToHome->setGeometry(0,0,800,480);

    ui->frameMoveZToTarget->setGeometry(0,0,800,480);
    ui->frameMoveYToTarget->setGeometry(0,0,800,480);
    ui->frameMoveXToTarget->setGeometry(0,0,800,480);

    hideFrames();

    connected = FALSE;
    checksum_h=0;
    checksum_l=0;
    revisione=0;

    // Nel caso in cui ci fosse un errore nel file o il file non
    // esiste vengono caricati dei valori di default
    openCfg();


    // Inizializzazione sequenze
    req_sequence = _REQ_SEQ_NONE ;
    sub_sequence = 0;
    event_req_sequence = 0;
    bypass_y_scroll = false;

}
biopsy::~biopsy()
{
    delete ui;
}

// Funzione agganciata ai sistemi di menu custom
void biopsy::changePage(int pg,  int opt)
{
    if(UI_PAGINA==pg)
    {

        // Attivazione pagina
        if(GWindowRoot.curPageVisible== TRUE){
            // Disabilitazione allarmi di sistema
            paginaAllarmi->alarm_enable = false;

            //changePageTimer = startTimer(DISABLE_EXIT_TMO);
            view->show();
            initPage();

        }
        else view->hide();
        return;
    }
    else if(GWindowRoot.curPage==UI_PAGINA)
    {
        // Disattivazione pagina
        paginaAllarmi->alarm_enable = true;
        view->hide();
        exitPage();

    }

}

void biopsy::initPage(void){

    // Impostazione Dati generali
    ui->StartupTitleLabel->setText("Biopsy Activation Page");
    hideFrames();


    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    if(isMaster){

    }
}

// Operazioni da compiere all'uscita dalla pagina
void biopsy::exitPage(void){

    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));
    if(!isMaster) return;
    return;
}

void biopsy::hideFrames(void){

    ui->frameMoveScrollCenter->hide();
    ui->frameMoveScrollLeft->hide();
    ui->frameMoveScrollRight->hide();
    ui->frameMoveScrollYCenter->hide();
    ui->frameMoveScrollYLeft->hide();
    ui->frameMoveScrollYRight->hide();

    ui->frameMoveXCenterToHome->hide();
    ui->frameMoveXLeftToHome->hide();
    ui->frameMoveXRightToHome->hide();
    ui->frameMoveYToHome->hide();
    ui->frameMoveZToHome->hide();

    ui->frameMoveZToTarget->hide();
    ui->frameMoveYToTarget->hide();
    ui->frameMoveXToTarget->hide();
}

void biopsy::manageChangeHomeSeq(unsigned char sub_seq)
{
    switch(sub_seq){
    case _REQ_SUBSEQ_HOME_INIT:
        ui->StartupTitleLabel->setText(ApplicationDatabase.getDataS(BIOPSY_ACTIVATION_TITLE_DB));
        break;

    case _REQ_SUBSEQ_HOME_BUSY:
        break;
    case _REQ_SUBSEQ_HOME_EXE_Z:
        hideFrames();
        break;
    case _REQ_SUBSEQ_HOME_WAIT_Z:
        ui->frameMoveZToHome->show();
        break;
    case _REQ_SUBSEQ_HOME_EXE_Y:
        hideFrames();
        break;
    case _REQ_SUBSEQ_HOME_WAIT_Y:
        ui->frameMoveYToHome->show();
        break;
    case _REQ_SUBSEQ_HOME_TEST_SCROLL_X:
        hideFrames();
        break;
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_LEFT:
        ui->frameMoveScrollLeft->show();
        break;
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_CENTER:
        ui->frameMoveScrollCenter->show();
        break;
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_RIGHT:
        ui->frameMoveScrollRight->show();
        break;

    case _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y:
        hideFrames();
        break;

    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_LEFT:
        ui->frameMoveScrollYLeft->show();
        break;
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_RIGHT:
        ui->frameMoveScrollYRight->show();
        break;
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_CENTER:
        ui->frameMoveScrollYCenter->show();
        break;

    case _REQ_SUBSEQ_HOME_EXE_X:
        hideFrames();
        break;
    case _REQ_SUBSEQ_HOME_WAIT_X_TO_LEFT:
        ui->frameMoveXLeftToHome->show();
        break;
    case _REQ_SUBSEQ_HOME_WAIT_X_TO_CENTER:
        ui->frameMoveXCenterToHome->show();
        break;
    case _REQ_SUBSEQ_HOME_WAIT_X_TO_RIGHT:
        ui->frameMoveXRightToHome->show();
        break;
    case _REQ_SUBSEQ_HOME_COMPLETED:
        break;

    }
}
void biopsy::manageChangeMoveXYZSeq(unsigned char sub_seq)
{
    switch(sub_seq){
    case _REQ_SUBSEQ_XYZ_INIT:
        ui->StartupTitleLabel->setText(ApplicationDatabase.getDataS(BIOPSY_ACTIVATION_TITLE_DB));
        break;

    case _REQ_SUBSEQ_XYZ_BUSY:
        break;
    case _REQ_SUBSEQ_XYZ_EXE_Z:
        hideFrames();
        break;
    case _REQ_SUBSEQ_XYZ_WAIT_Z:
        ui->frameMoveZToTarget->show();
        break;
    case _REQ_SUBSEQ_XYZ_EXE_Y:
        hideFrames();
        break;
    case _REQ_SUBSEQ_XYZ_WAIT_Y:
        ui->frameMoveYToTarget->show();
        break;
    case _REQ_SUBSEQ_XYZ_EXE_X:
        hideFrames();
        break;
    case _REQ_SUBSEQ_XYZ_WAIT_X:
        ui->frameMoveXToTarget->show();
        break;
    case _REQ_SUBSEQ_XYZ_COMPLETED:
        break;

    case _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_LEFT:
        ui->frameMoveScrollYLeft->show();
        break;
    case _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_RIGHT:
        ui->frameMoveScrollYRight->show();
        break;
    case _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_CENTER:
        ui->frameMoveScrollYCenter->show();
        break;

    }
}

void biopsy::valueChanged(int index,int opt)
{
    unsigned char seq, subseq;
    switch(index){
    case BIOPSY_ACTIVATION_SEQUENCE_DB:
        seq = (ApplicationDatabase.getDataI(index) >> 8) & 0xFF;
        subseq = (ApplicationDatabase.getDataI(index)) & 0xFF;
        if(seq == _REQ_SEQ_HOME) manageChangeHomeSeq(subseq);
        else if(seq == _REQ_SEQ_XYZ) manageChangeMoveXYZSeq(subseq);
        break;
    case BIOPSY_USER_CONFIRMATION_DB:
        user_confirmation = true;
        break;
    }
}

void biopsy::timerEvent(QTimerEvent* ev)
{

    if(ev->timerId() == event_req_sequence)
    {
        killTimer(event_req_sequence);
        event_req_sequence=0;

        if(req_sequence == _REQ_SEQ_HOME) manageHomeSequence();
        else if(req_sequence == _REQ_SEQ_XYZ) manageXYZSequence();
        else  manageRequestErrors(_BIOPSY_MOVING_UNDEFINED_ERROR);

    }

    return;

}

void biopsy::nextStepSequence(int tmo){
    // Aggiorna i display con la fase in corso
    int val = (req_sequence << 8) | (sub_sequence&0xFF);
    ApplicationDatabase.setData(BIOPSY_ACTIVATION_SEQUENCE_DB, (int) val, DBase::_DB_FORCE_SGN);

    // Riaccoda l'evento
    event_req_sequence = startTimer(1);
}

void biopsy::manageHomeSequence(void){

    // GWindowRoot.setNewPage(_PG_ALARM,GWindowRoot.curPage,0);    
    //GWindowRoot.parentPage;

    switch(sub_sequence){

    // Inizializzazione della sequenza
    case _REQ_SUBSEQ_HOME_INIT:

        user_confirmation = false;

        // Assegna la fase in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED) sub_sequence = _REQ_SUBSEQ_HOME_BUSY;
        else sub_sequence = _REQ_SUBSEQ_HOME_EXE_Z;

        nextStepSequence(1);
        break;

    // Attivazione dell'azzeramento Z
    case _REQ_SUBSEQ_HOME_EXE_Z:
        if(curZ_dmm < 5) sub_sequence = _REQ_SUBSEQ_HOME_EXE_Y;
        else{
            // Avvia il comando di posizionamento Z
            moveXYZ(curX_dmm, curY_dmm, 0);
            if( movingError != _BIOPSY_MOVING_NO_ERROR){
                manageRequestErrors(movingError);
                return;
            }
        }
        sub_sequence = _REQ_SUBSEQ_HOME_WAIT_Z;
        nextStepSequence(100);
        break;

    // Attesa termine comando
    case _REQ_SUBSEQ_HOME_WAIT_Z:
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
              event_req_sequence = startTimer(100);
              return;
        }

        // Comando terminato
        if(movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }
        sub_sequence = _REQ_SUBSEQ_HOME_EXE_Y;
        nextStepSequence(1);
        break;

    // Attivazione dell'azzeramento Y
    case _REQ_SUBSEQ_HOME_EXE_Y:
        if(curY_dmm < 5) sub_sequence = _REQ_SUBSEQ_HOME_TEST_SCROLL_X;
        else{
            // Avvia il comando di posizionamento Y
            moveXYZ(curX_dmm, 0, curZ_dmm);
            if( movingError != _BIOPSY_MOVING_NO_ERROR){
                manageRequestErrors(movingError);
                return;
            }
        }
        sub_sequence = _REQ_SUBSEQ_HOME_WAIT_Y;
        nextStepSequence(100);
        break;

    // Attesa termine comando
    case _REQ_SUBSEQ_HOME_WAIT_Y:
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
              event_req_sequence = startTimer(100);
              return;
        }

        // Comando terminato
        if(movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_TEST_SCROLL_X;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_TEST_SCROLL_X:

        // Se già si trova in Home termina qui
        if(isHome(req_home_lat)){
            sub_sequence = _REQ_SUBSEQ_HOME_COMPLETED;
            nextStepSequence(1);
            return;
        }

        // Verifica se si deve richiedere lo scroll dell'asse X
        if(req_home_lat != curLatX) {
            if(req_home_lat == _BP_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_X_LEFT;
            else if(req_home_lat == _BP_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_X_RIGHT;
            else if(req_home_lat == _BP_ASSEX_POSITION_CENTER) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_X_CENTER;
        }else sub_sequence = _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_LEFT:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_CENTER:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_RIGHT:

        // Attende che l'utente scrolli l'asse X nella corretta posizione
        if(req_home_lat != curLatX){
              event_req_sequence = startTimer(100);
              return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y:
        // Verifica se Y deve essere ruotato
        user_confirmation = false;
        if(testUpsidePosition(req_X)){
            if(req_home_lat == _BP_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_LEFT;
            else if(req_home_lat == _BP_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_RIGHT;
            else if(req_home_lat == _BP_ASSEX_POSITION_CENTER) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_CENTER;
        } else sub_sequence = _REQ_SUBSEQ_HOME_EXE_X;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_LEFT:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_CENTER:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_RIGHT:

        // Attende che l'utente confermi correttamente la regolazione dell'asse X
        if(!user_confirmation){
              event_req_sequence = startTimer(100);
              return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_EXE_X;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_X:
        // Avvia il comando di posizionamento X
        moveXYZ(req_X, curY_dmm, curZ_dmm);
        if( movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        if(req_home_lat == _BP_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_HOME_WAIT_X_TO_LEFT;
        else if(req_home_lat == _BP_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_HOME_WAIT_X_TO_RIGHT;
        else if(req_home_lat == _BP_ASSEX_POSITION_CENTER) sub_sequence = _REQ_SUBSEQ_HOME_WAIT_X_TO_CENTER;
        nextStepSequence(100);
        break;

    case _REQ_SUBSEQ_HOME_WAIT_X_TO_LEFT:
    case _REQ_SUBSEQ_HOME_WAIT_X_TO_CENTER:
    case _REQ_SUBSEQ_HOME_WAIT_X_TO_RIGHT:
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
              event_req_sequence = startTimer(100);
              return;
        }

        // Comando terminato
        if(movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }
        sub_sequence = _REQ_SUBSEQ_HOME_COMPLETED;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_COMPLETED:
        // Notifica di fine movimento
        if(activationId) pToConsole->endCommandAck(activationId, movingError);
        activationId = 0;
        GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
        break;

    case _REQ_SUBSEQ_HOME_BUSY:
        // Attende il termine del comando in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED)
        {
            event_req_sequence = startTimer(100);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_EXE_Z;
        nextStepSequence(1);
        break;

    // _________________________________________________
    defaut:
        // Case anomalo: si chiude il comando come se fosse tutto ok
        if(activationId) pToConsole->endCommandAck(activationId, _BIOPSY_MOVING_NO_ERROR);
        activationId = 0;
        GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
        break;
    }


    return ;
}


void biopsy::manageXYZSequence(void){
    switch(sub_sequence){

    // Inizializzazione della sequenza
    case _REQ_SUBSEQ_XYZ_INIT:
        user_confirmation = false;

        // Assegna la fase in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
            sub_sequence = _REQ_SUBSEQ_XYZ_BUSY;
            nextStepSequence(1);
            return;
        }

        // Verifica se ci può essere rischio di impatto
        if(testUpsidePosition(req_X)){
            if(curLatX == _BP_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_LEFT;
            else if(curLatX == _BP_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_RIGHT;
            else sub_sequence = _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_CENTER;
        } else sub_sequence = _REQ_SUBSEQ_XYZ_EXE_X;

        nextStepSequence(1);
        break;

    // Richiede il posizionamento dell'asse Y
    case _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_LEFT:
    case _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_RIGHT:
    case _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_CENTER:

        // Attende che l'utente confermi correttamente la regolazione dell'asse X
        if(!user_confirmation){
              event_req_sequence = startTimer(100);
              return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_EXE_X;
        nextStepSequence(1);
        break;


    case _REQ_SUBSEQ_XYZ_EXE_X:

        // Avvia il comando di posizionamento X
        moveXYZ(req_X, curY_dmm, curZ_dmm);
        if( movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_WAIT_X;
        nextStepSequence(100);
        break;

    case _REQ_SUBSEQ_XYZ_WAIT_X:

        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
              event_req_sequence = startTimer(100);
              return;
        }

        // Comando terminato
        if(movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_EXE_Y;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_XYZ_EXE_Y:

        // Avvia il comando di posizionamento X
        moveXYZ(curX_dmm, req_Y, curZ_dmm);
        if( movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_WAIT_Y;
        nextStepSequence(100);
        break;

    case _REQ_SUBSEQ_XYZ_WAIT_Y:

        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
              event_req_sequence = startTimer(100);
              return;
        }

        // Comando terminato
        if(movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_EXE_Z;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_XYZ_EXE_Z:

        // Avvia il comando di posizionamento X
        moveXYZ(curX_dmm, curY_dmm, req_Z);
        if( movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_WAIT_Z;
        nextStepSequence(100);
        break;

    case _REQ_SUBSEQ_XYZ_WAIT_Z:

        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
              event_req_sequence = startTimer(100);
              return;
        }

        // Comando terminato
        if(movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_XYZ_COMPLETED;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_XYZ_COMPLETED:

        // Notifica di fine movimento
        if(activationId) pToConsole->endCommandAck(activationId, movingError);
        activationId = 0;
        GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
        break;
    }
}




void biopsy::manageRequestErrors(int error){
}

/*
 *  id = id del messaggio della console. 0 = se non richiede una risposta;
 *  lat = lateralità richiesta;
 *
 *  return:
 *  *  0: si trova già in HOME
 *  *  1: comando in esecuzione
 *  * -1: errore lateralità
 */
int biopsy::requestBiopsyHome(int id, unsigned char lat){

    // Se si trova già in home finisce subito
    if(isHome(lat)) return 0;

    req_home_lat = lat;
    req_Y = 0;
    req_Z = 0;

    // Determina il target X in funzione della lateralità
    if(lat == _BP_ASSEX_POSITION_LEFT){
        req_X = 2580;
        ApplicationDatabase.setData(BIOPSY_ACTIVATION_TITLE_DB,QString("BIOPSY ACTIVATED TO HOME LEFT SIDE"),0);
    }else if(lat == _BP_ASSEX_POSITION_CENTER){
        req_X = 1290;
        ApplicationDatabase.setData(BIOPSY_ACTIVATION_TITLE_DB,QString("BIOPSY ACTIVATED TO HOME CENTER SIDE"),0);
    }else if(lat == _BP_ASSEX_POSITION_RIGHT){
        req_X = 0;
        ApplicationDatabase.setData(BIOPSY_ACTIVATION_TITLE_DB,QString("BIOPSY ACTIVATED TO HOME RIGHT SIDE"),0);
    }

    // Prepara la sequenza di gestione del movimento
    activationId = id;
    req_sequence = _REQ_SEQ_HOME;
    sub_sequence = _REQ_SUBSEQ_HOME_INIT;

    // Apre la pagina grafica di gestione delle attivazioni
    GWindowRoot.setNewPage(_PG_BIOPSY_DEVICE,GWindowRoot.curPage,0);
    nextStepSequence(1);
    return 1;


}

int biopsy::requestBiopsyMoveXYZ(unsigned short X, unsigned short Y,unsigned short Z,int id){

    // Se la lateralità non è definita non consente di muovere
    if(curLatX == _BP_ASSEX_POSITION_ND) return -1;

    // Se si trova già in posizione esce subito
    if( (abs(curX_dmm-X) < 5) && (abs(curY_dmm-Y) < 5) && (abs(curZ_dmm-Z) < 5) ) return 0;


    activationId = id;
    req_X = X;
    req_Y = Y;
    req_Z = Z;
    ApplicationDatabase.setData(BIOPSY_ACTIVATION_TITLE_DB,QString("BIOPSY ACTIVATED TO TARGET: %1,%2,%3").arg(req_X).arg(req_Y).arg(req_Z),0);

    // Prepara la sequenza di gestione del movimento
    req_sequence = _REQ_SEQ_XYZ;
    sub_sequence = _REQ_SUBSEQ_XYZ_INIT;

    // Apre la pagina grafica di gestione delle attivazioni
    GWindowRoot.setNewPage(_PG_BIOPSY_DEVICE,GWindowRoot.curPage,0);
    nextStepSequence(1);
    return 1;

}

void biopsy::onConfirmButton(void){
    if(isMaster) user_confirmation = true;
    else ApplicationDatabase.setData(BIOPSY_USER_CONFIRMATION_DB,(int) 1, DBase::_DB_FORCE_SGN); // trigger to master
}

//________________________________________________________________________________________________________________

void biopsy::defaultConfigData(void){

    // Offset di puntamento
    config.Z_homePosition = 193;            // (mm) distanza zero torretta - fibra di carbonio
    config.Z_basePosizionatore = 189;       // Distanza base metallica - fibra di carbonio

    // Gestion Movimento Pad
    config.offsetPad = 59;              // Offset linea di calibrazione posizione - superficie staffe metalliche
    config.margineRisalita = 15;        // Margine di sicurezza per impatto con il compressore in risalita
    config.marginePosizionamento = 5;   // Margine di sicurezza impatto con il compressore in puntamento

}


bool biopsy::openCfg(void)
{
    QString filename;
    QList<QString> dati;

    // Default File di configurazione
    defaultConfigData();

    filename =  QString("/resource/config/biopsy.cnf");

    // Se nn esiste lo crea con i default
    QFile file(filename.toAscii());
    if(!file.exists()){
        return storeConfig();
    }

    // Se è corrotto lo crea di default
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return storeConfig();
    }

    // Procede con la lettura del formato corrente
    while(1){

        dati = Config::getNextArrayFields(&file);
        if(dati.isEmpty()) break;

        if(dati.at(0)=="Z_HOME"){
            config.Z_homePosition = dati.at(1).toInt();

        }else if(dati.at(0)=="Z_BASE"){
             config.Z_basePosizionatore = dati.at(1).toInt();

        }else if(dati.at(0)=="OFFSET_PAD"){
            config.offsetPad = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_RISALITA"){
            config.margineRisalita = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_POSIZIONAMENTO"){
            config.marginePosizionamento = dati.at(1).toInt();
        }

    }

    file.close();
    return true;
}

/*
 *
 *  Salva il file di configurazione della biopsia
 */
bool biopsy::storeConfig(void)
{
    QString filename;

    filename =  QString("/resource/config/biopsy.cnf");
    QFile file(filename.toAscii());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return FALSE;

    file.write( QString("<Z_HOME, %1>\n").arg((int) config.Z_homePosition).toAscii());
    file.write( QString("<Z_BASE, %1>\n").arg((int) config.Z_basePosizionatore).toAscii());

    file.write( QString("<OFFSET_PAD,%1>\n").arg((int) config.offsetPad).toAscii());
    file.write( QString("<MARGINE_RISALITA,%1>\n").arg((int) config.margineRisalita).toAscii());
    file.write( QString("<MARGINE_POSIZIONAMENTO,%1>\n").arg((int) config.marginePosizionamento).toAscii());


    file.flush();
    file.close();

    return true;
}

// To be completed
int biopsy::calibrateSh(void){
    return (int) 150 - (int) curSh_dmm ;
}

void biopsy::mccStatNotify(unsigned char id_notify,unsigned char cmd, QByteArray data)
{
    unsigned char errore;
    static unsigned char    bp_motion=0;
    static int              bp_movecommand=0;

    if(id_notify!=1) return;
    //if(pConfig->startupCompleted == false) return;

    errore=0;
    if(cmd!=BIOP_NOTIFY_STAT) return;


    // Se il sistema risulta NON connesso non fa altro..
    if(data.at(_BP_CONNESSIONE)==_BP_CONNESSIONE_DISCONNECTED)
    {
        connected = FALSE;
        bypass_y_scroll = false;
        model = _BP_MODEL_UNDEFINED;        
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
            movingCommand =_BIOPSY_MOVING_COMPLETED;
            movingError = _BIOPSY_MOVING_ERROR_TIMEOUT;
        }
        return;
    }

    // Cambio stato da Non connesso a connesso
    if(!connected){

        // Riconoscimento torretta di Biopsia inserita
        connected = TRUE;
        checksum_h=data[_BP_CHKH];
        checksum_l=data[_BP_CHKL];
        revisione=data[_BP_REVIS];
        model = data[_BP_MODEL];

        movingCommand =_BIOPSY_MOVING_NO_COMMAND;
        movingError = _BIOPSY_MOVING_NO_ERROR;
        activationId = 0;

        // Aggiorna le pagine con nil riconoscimento della Biopsia
        ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) BIOPSY_DEVICE,0);
        ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("BIOPSY","NOME ACCESSORIO", 0, QApplication::UnicodeUTF8)),0);        

    }

    // Pulsante di sblocco Braccio e funzioni biopsia
    if(data.at(_BP_PUSH_SBLOCCO) == _BP_PUSH_SBLOCCO_ATTIVO) unlock_button = true;
    else unlock_button = false;
    ApplicationDatabase.setData(_DB_BIOP_UNLOCK_BUTTON,(int) data.at(_BP_PUSH_SBLOCCO),0);

    // Lateralità rilevata
    curLatX = data.at(_BP_ASSEX_POSITION);
    ApplicationDatabase.setData(_DB_BIOP_LAT_X,(int) data.at(_BP_ASSEX_POSITION),0);
    if( (curLatX == _BP_ASSEX_POSITION_CENTER) && (curX_dmm < 2263)  && (curX_dmm > 317)) bypass_y_scroll = true;

    //  Riconoscimento dell'adapter
    if(adapterId != data.at(_BP_ADAPTER_ID)){
        adapterId = data.at(_BP_ADAPTER_ID);
        pToConsole->setConfigChanged(pToConsole->ACCESSORIO);
        ApplicationDatabase.setData(_DB_BIOP_ADAPTER_ID,(int) data.at(_BP_ADAPTER_ID),0);
    }

    // Posizione attuale cursore
    curX_dmm = data.at(_BP_XL) + 256 * data.at(_BP_XH) ;
    curY_dmm = data.at(_BP_YL) + 256 * data.at(_BP_YH) ;
    curZ_dmm = data.at(_BP_ZL) + 256 * data.at(_BP_ZH) ;

    curSh_dmm= data.at(_BP_SHL) + 256 * data.at(_BP_SHH) ;

    ApplicationDatabase.setData(_DB_BIOP_X,(int) curX_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Y,(int) curY_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Z,(int) curZ_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_SH,(int) calibrateSh(),0);

    // Calcolo del Margine tra Paddle e Staffa metallica
    paddle_margine  = data.at(_BP_PADDLE_MARGINE);
    max_z_paddle  = data.at(_BP_MAX_Z_PADDLE);

    // Ricalcolo dei vari parametri di limitazione
    calcoloMargini();

    if(bp_motion!=data.at(_BP_MOTION)){
        PRINT(QString("BIOPSIA: BP_MOTION=%1").arg((int) data.at(_BP_MOTION)));
        bp_motion = data.at(_BP_MOTION);
    }

    if(bp_movecommand!=movingCommand){
        PRINT(QString("BIOPSIA: MOVE CMD=%1").arg((int) movingCommand));
        bp_movecommand = movingCommand;
    }

    // ______________________________ Aggiornamento stato di posizionamento in corso _________________________________________________
    switch(data.at(_BP_MOTION))
    {

        case _BP_MOTION_ON:
            PRINT(QString("BIOPSIA:CONFERMA ATTIVAZIONE MOVIMENTO"));
            bypass_y_scroll = false;
        break;

        case _BP_MOTION_TERMINATED:

            // Valutazione del risultato del movimento
            if(data.at(_BP_MOTION_END) == _BP_ERROR_POSITIONINIG)
            {
                movingError = _BIOPSY_MOVING_ERROR_TARGET;
            }else if(data.at(_BP_MOTION_END) == _BP_TIMEOUT_COMANDO)
            {
                movingError = _BIOPSY_MOVING_ERROR_TIMEOUT;
            }else  movingError = _BIOPSY_MOVING_NO_ERROR;

            movingCommand =_BIOPSY_MOVING_COMPLETED;

        break;


    } // data.at(_DB_MOTION)


}

/*
 *  Sulla base dei dati presenti si calcolano i vari
 *  parametri di limitazione al movimento
 */
void biopsy::calcoloMargini(void){

    // Paddle Margine è calcolato dal driver
    ApplicationDatabase.setData(_DB_BIOP_PADDLE_MARGINE,(int) paddle_margine,0);

    // Needle margine in mm
    int margine = config.Z_homePosition - (curZ_dmm/10) - needle_home;
    if(margine < 0) margine = 0;
    needle_margine = margine;
    ApplicationDatabase.setData(_DB_BIOP_NEEDLE_MARG,(int) margine,0);

    int max_needle_z = config.Z_homePosition - needle_home - 5;

    // Calcolo della massima Z
    if(max_z_paddle < max_needle_z) abs_max_z = max_z_paddle;
    else abs_max_z = max_needle_z;
    ApplicationDatabase.setData(_DB_BIOP_MAXZ,(int) abs_max_z,0);

}

// Restituisce TRUE se la destinazione attraversa l'area di possibile impatto
bool biopsy::testUpsidePosition(unsigned short X){

    // Con il bypass attivo non si richiede la rotazione dell'asse Y
    if(bypass_y_scroll) return false;

    int xh,xl;
    xh = 2580;
    xl = 0;
    if(curLatX == _BP_ASSEX_POSITION_LEFT){
        xh = 2580;
        xl = 1460;
    }else if(curLatX == _BP_ASSEX_POSITION_CENTER){
        xh = 2263;
        xl = 317;
    }else if(curLatX == _BP_ASSEX_POSITION_RIGHT){
        xh = 1120;
        xl = 0;
    }

    // Verifica se attraversa l'area proibita
    if(
        ((curX_dmm>=xh) && (X<=xh)) ||
        ((curX_dmm<=xl) && (X>=xl)) ){
        return true;

    }
    return false;
}

int biopsy::moveXYZ(unsigned short X, unsigned short Y, unsigned short Z)
{
    unsigned char data[15];

    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return movingError;
    }

    data[0]=_MCC_BIOPSY_CMD_MOVE_XYZ; // Codice comando
    data[1]=(unsigned char) (X & 0x00FF);
    data[2]=(unsigned char) (X >>8);
    data[3]=(unsigned char) (Y & 0x00FF);
    data[4]=(unsigned char) (Y >>8);
    data[5]=(unsigned char) (Z & 0x00FF);
    data[6]=(unsigned char) (Z >>8);

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,7)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return movingError;
    }

    movingCommand =_BIOPSY_MOVING_XYZ;
    movingError = _BIOPSY_MOVING_NO_ERROR;
    return _BIOPSY_MOVING_NO_ERROR;
}

bool biopsy::isHome(unsigned char lat){
    if( lat != curLatX) return false;
    if(curY_dmm > 5)  return false;
    if(curZ_dmm > 5)  return false;
    if((curLatX == _BP_ASSEX_POSITION_LEFT) && (curX_dmm < 2575))    return false;
    if((curLatX == _BP_ASSEX_POSITION_CENTER) && (curX_dmm < 1285))  return false;
    if((curLatX == _BP_ASSEX_POSITION_CENTER) && (curX_dmm > 1295))  return false;
    if((curLatX == _BP_ASSEX_POSITION_RIGHT) && (curX_dmm > 5))      return false;

    return true;
}


int biopsy::moveDecZ(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_DECZ; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECZ;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}


int biopsy::moveIncZ(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_INCZ; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCZ;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::moveDecX(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_DECX; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECX;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::moveIncX(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_INCX; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCX;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::moveDecY(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_DECY; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECY;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}
int biopsy::moveIncY(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_INCY; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCY;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::setStepVal(unsigned char step)
{
    unsigned char data[2];
    data[0]=_MCC_BIOPSY_CMD_SET_STEPVAL; // Codice comando
    data[1] = step;
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,2)==FALSE) return 0;
    return 1;
}


bool biopsy::updateConfig(void)
{
    // Invia la configurazione al driver per aggiornarlo in diretta
    return pConfig->sendMccConfigCommand(CONFIG_BIOPSY);

}
