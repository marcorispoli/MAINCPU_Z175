#include "../application.h"
#include "../appinclude.h"
#include "../globvar.h"
#include "ui_biopsy.h"

#define UI_PAGINA _PG_BIOPSY_EXTENDED_DEVICE


void biopsyExtendedDevice::activateConnections(void){
    connect(pConsole, SIGNAL(mccBiopsyExtendedNotify(unsigned char,unsigned char,QByteArray)), this, SLOT(mccStatNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
}

biopsyExtendedDevice::biopsyExtendedDevice(int rotview, QWidget *parent) :
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


    // Solo il Master gestisce il device
    if(!isMaster) return;



    // Inizializzazione sequenze
    req_sequence = _REQ_SEQ_NONE ;
    sub_sequence = 0;
    event_req_sequence = 0;
    bypass_y_scroll = false;

    // Assegnazione delle posizioni di home
    XHOME_LEFT = _DEF_EXT_XHOME_LEFT;
    XHOME_CENTER = _DEF_EXT_XHOME_CENTER;
    XHOME_RIGHT = _DEF_EXT_XHOME_RIGHT;
    YHOME_LEFT = _DEF_EXT_YHOME_LEFT;
    YHOME_CENTER = _DEF_EXT_YHOME_CENTER;
    YHOME_RIGHT = _DEF_EXT_YHOME_RIGHT;
    ZHOME_LEFT = _DEF_EXT_ZHOME_LEFT;
    ZHOME_CENTER = _DEF_EXT_ZHOME_CENTER;
    ZHOME_RIGHT= _DEF_EXT_ZHOME_RIGHT;



}
biopsyExtendedDevice::~biopsyExtendedDevice()
{
    delete ui;
}

// Funzione agganciata ai sistemi di menu custom
void biopsyExtendedDevice::changePage(int pg,  int opt)
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

void biopsyExtendedDevice::initPage(void){

    // Impostazione Dati generali
    ui->StartupTitleLabel->setText("Biopsy Activation Page");
    hideFrames();


    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    if(isMaster){

    }
}

// Operazioni da compiere all'uscita dalla pagina
void biopsyExtendedDevice::exitPage(void){

    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));
    if(!isMaster) return;
    return;
}

void biopsyExtendedDevice::hideFrames(void){

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

void biopsyExtendedDevice::manageChangeHomeSeq(unsigned char sub_seq)
{
    switch(sub_seq){
    case _REQ_SUBSEQ_HOME_INIT:
        ui->StartupTitleLabel->setText(ApplicationDatabase.getDataS(BIOPSY_ACTIVATION_TITLE_DB));
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
void biopsyExtendedDevice::manageChangeMoveXYZSeq(unsigned char sub_seq)
{
    switch(sub_seq){
    case _REQ_SUBSEQ_XYZ_INIT:
        ui->StartupTitleLabel->setText(ApplicationDatabase.getDataS(BIOPSY_ACTIVATION_TITLE_DB));
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

void biopsyExtendedDevice::valueChanged(int index,int opt)
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

void biopsyExtendedDevice::timerEvent(QTimerEvent* ev)
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

void biopsyExtendedDevice::nextStepSequence(int tmo){
    // Aggiorna i display con la fase in corso
    int val = (req_sequence << 8) | (sub_sequence&0xFF);
    ApplicationDatabase.setData(BIOPSY_ACTIVATION_SEQUENCE_DB, (int) val, DBase::_DB_FORCE_SGN);

    // Riaccoda l'evento
    event_req_sequence = startTimer(1);
}

void biopsyExtendedDevice::manageHomeSequence(void){

    // GWindowRoot.setNewPage(_PG_ALARM,GWindowRoot.curPage,0);    
    //GWindowRoot.parentPage;

    switch(sub_sequence){

    // Inizializzazione della sequenza
    case _REQ_SUBSEQ_HOME_INIT:

        user_confirmation = false;

        // Assegna la fase in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED) {
            manageRequestErrors(_BIOPSY_MOVING_ERROR_BUSY);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_EXE_Z;
        nextStepSequence(1);
        break;

    // Attivazione dell'azzeramento Z
    case _REQ_SUBSEQ_HOME_EXE_Z:

        if(isTarget(curX_dmm, curY_dmm, req_Z)) sub_sequence = _REQ_SUBSEQ_HOME_EXE_Y;
        else{
            // Avvia il comando di posizionamento Z
            moveXYZ(curX_dmm, curY_dmm, req_Z);
            if( movingError != _BIOPSY_MOVING_NO_ERROR){
                manageRequestErrors(movingError);
                return;
            }
            sub_sequence = _REQ_SUBSEQ_HOME_WAIT_Z;
        }
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
        if(isTarget(curX_dmm, req_Y, curZ_dmm)) sub_sequence = _REQ_SUBSEQ_HOME_TEST_SCROLL_X;
        else{
            // Avvia il comando di posizionamento Y
            moveXYZ(curX_dmm, req_Y, curZ_dmm);
            if( movingError != _BIOPSY_MOVING_NO_ERROR){
                manageRequestErrors(movingError);
                return;
            }
            sub_sequence = _REQ_SUBSEQ_HOME_WAIT_Y;
        }
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
        if((req_home_lat == curLatX)&&(isTarget(req_X, curY_dmm, curZ_dmm))) {
            sub_sequence = _REQ_SUBSEQ_HOME_COMPLETED;
            nextStepSequence(1);
            return;
        }

        // Verifica se si deve richiedere lo scroll dell'asse X
        if(req_home_lat != curLatX) {
            buzzer_delay = 0;
            user_timeout = _DEF_TIMEOUT_USER_FEEDBACK;
            if(req_home_lat == _BP_EXT_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_X_LEFT;
            else if(req_home_lat == _BP_EXT_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_X_RIGHT;
            else if(req_home_lat == _BP_EXT_ASSEX_POSITION_CENTER) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_X_CENTER;
        }else sub_sequence = _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_LEFT:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_CENTER:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_X_RIGHT:

        // Attende che l'utente scrolli l'asse X nella corretta posizione
        if(req_home_lat != curLatX){
            user_timeout--;
            if(!user_timeout){
                manageRequestErrors(_BIOPSY_MOVING_ERROR_TIMEOUT);
                return;
            }

            if(!buzzer_delay){
                buzzer_delay = 20;
                setBuzzer();
            }else buzzer_delay--;
            event_req_sequence = startTimer(100);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y:

        // Verifica se Y deve essere ruotato
        if(testUpsidePosition(req_X)){
            user_timeout = _DEF_TIMEOUT_USER_FEEDBACK;
            buzzer_delay = 0;
            user_confirmation = false;
            if(req_home_lat == _BP_EXT_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_LEFT;
            else if(req_home_lat == _BP_EXT_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_RIGHT;
            else if(req_home_lat == _BP_EXT_ASSEX_POSITION_CENTER) sub_sequence = _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_CENTER;
        } else sub_sequence = _REQ_SUBSEQ_HOME_EXE_X;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_LEFT:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_CENTER:
    case _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_RIGHT:

        // Attende che l'utente confermi correttamente la regolazione dell'asse X
        if(!user_confirmation){
            user_timeout--;
            if(!user_timeout){
                manageRequestErrors(_BIOPSY_MOVING_ERROR_TIMEOUT);
                return;
            }
            if(!buzzer_delay){
                buzzer_delay = 20;
                setBuzzer();
            }else buzzer_delay--;
            event_req_sequence = startTimer(100);
            return;
        }

        sub_sequence = _REQ_SUBSEQ_HOME_EXE_X;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_EXE_X:

        // Se X già in posizione
        if(isTarget(req_X, curY_dmm, curZ_dmm)){
            sub_sequence = _REQ_SUBSEQ_HOME_COMPLETED;
            nextStepSequence(1);
            return;
        }

        // Avvia il comando di posizionamento X
        moveXYZ(req_X, curY_dmm, curZ_dmm);
        if( movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            return;
        }

        if(req_home_lat == _BP_EXT_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_HOME_WAIT_X_TO_LEFT;
        else if(req_home_lat == _BP_EXT_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_HOME_WAIT_X_TO_RIGHT;
        else if(req_home_lat == _BP_EXT_ASSEX_POSITION_CENTER) sub_sequence = _REQ_SUBSEQ_HOME_WAIT_X_TO_CENTER;
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
        if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, movingError);
        pBiopsy->activationId = 0;
        GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
        break;



    // _________________________________________________
    defaut:
        // Case anomalo: si chiude il comando come se fosse tutto ok
        if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, _BIOPSY_MOVING_NO_ERROR);
        pBiopsy->activationId = 0;
        GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
        break;
    }


    return ;
}


void biopsyExtendedDevice::manageXYZSequence(void){
    switch(sub_sequence){

    // Inizializzazione della sequenza
    case _REQ_SUBSEQ_XYZ_INIT:
        user_confirmation = false;


        // Assegna la fase in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED) {
            manageRequestErrors(_BIOPSY_MOVING_ERROR_BUSY);
            return;
        }

        // Verifica se ci può essere rischio di impatto
        if(testUpsidePosition(req_X)){
            if(curLatX == _BP_EXT_ASSEX_POSITION_LEFT) sub_sequence = _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_LEFT;
            else if(curLatX == _BP_EXT_ASSEX_POSITION_RIGHT) sub_sequence = _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_RIGHT;
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
        if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, movingError);
        pBiopsy->activationId = 0;
        GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
        break;
    }
}



/*
 *  Elenco codici errore gestiti

 */
void biopsyExtendedDevice::manageRequestErrors(int error){

    pToConsole->endCommandAck(pBiopsy->activationId, error);
    pBiopsy->activationId = 0;
    GWindowRoot.setNewPage(GWindowRoot.parentPage,GWindowRoot.curPage,0);
    sub_sequence = 0;
    req_sequence = 0;

    // Attiva una finestra di allarme
    PageAlarms::activateNewAlarm(_DB_ALLARMI_BIOPSIA,error,TRUE);
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
int biopsyExtendedDevice::requestBiopsyHome(int id, unsigned char lat){
    QString activationString;

    req_home_lat = lat;

    // Determina il target X in funzione della lateralità
    if(lat == _BP_EXT_ASSEX_POSITION_LEFT){
        req_X = XHOME_LEFT;
        req_Z = ZHOME_LEFT;
        req_Y = YHOME_LEFT;
        activationString = "BIOPSY ACTIVATED TO HOME LEFT SIDE";
    }else if(lat == _BP_EXT_ASSEX_POSITION_CENTER){
        req_X = XHOME_CENTER;
        req_Z = ZHOME_CENTER;
        req_Y = YHOME_CENTER;
        activationString = "BIOPSY ACTIVATED TO HOME CENTER SIDE";
    }else if(lat == _BP_EXT_ASSEX_POSITION_RIGHT){
        req_X = XHOME_RIGHT;
        req_Z = ZHOME_RIGHT;
        req_Y = YHOME_RIGHT;
        activationString = "BIOPSY ACTIVATED TO HOME RIGHT SIDE";
    }

    // Bym già in posizione
    if((req_home_lat == curLatX) && (isTarget(req_X, req_Y, req_Z))) return 0;

    // Prepara la sequenza di gestione del movimento
    ApplicationDatabase.setData(BIOPSY_ACTIVATION_TITLE_DB,activationString,0);

    pBiopsy->activationId = id;
    req_sequence = _REQ_SEQ_HOME;
    sub_sequence = _REQ_SUBSEQ_HOME_INIT;

    // Apre la pagina grafica di gestione delle attivazioni
    GWindowRoot.setNewPage(_PG_BIOPSY_EXTENDED_DEVICE,GWindowRoot.curPage,0);
    nextStepSequence(1);
    return 1;


}

int biopsyExtendedDevice::requestBiopsyMoveXYZ(unsigned short X, unsigned short Y,unsigned short Z,int id){


    // Se si trova già in posizione esce subito
    if(isTarget(X,Y,Z)) return 0;
    ApplicationDatabase.setData(BIOPSY_ACTIVATION_TITLE_DB,QString("BIOPSY ACTIVATED TO TARGET: %1,%2,%3").arg(req_X).arg(req_Y).arg(req_Z),0);


    pBiopsy->activationId = id;
    req_X = X;
    req_Y = Y;
    req_Z = Z;

    // Prepara la sequenza di gestione del movimento
    req_sequence = _REQ_SEQ_XYZ;
    sub_sequence = _REQ_SUBSEQ_XYZ_INIT;

    // Apre la pagina grafica di gestione delle attivazioni
    GWindowRoot.setNewPage(_PG_BIOPSY_EXTENDED_DEVICE,GWindowRoot.curPage,0);
    nextStepSequence(1);
    return 1;

}

void biopsyExtendedDevice::onConfirmButton(void){
    if(isMaster) user_confirmation = true;
    else ApplicationDatabase.setData(BIOPSY_USER_CONFIRMATION_DB,(int) 1, DBase::_DB_FORCE_SGN); // trigger to master
}

//________________________________________________________________________________________________________________



// To be completed
int biopsyExtendedDevice::calibrateSh(void){
    return (int) 150 - (int) curSh_dmm ;
}

void biopsyExtendedDevice::mccStatNotify(unsigned char id_notify,unsigned char cmd, QByteArray data)
{
    unsigned char errore;
    static unsigned char    bp_motion=0;
    static int              bp_movecommand = 0;
    static int              bCalibratedSh = -10;
    bool update_aws = false;

    if(id_notify!=1) return;

    errore=0;
    if(cmd!=BIOP_NOTIFY_STAT) return;
    if(pBiopsy->model == BYM_STANDARD_DEVICE) return;

    // Se arriva qui il modulo ha comunicato con successo con la torretta estesa
    pBiopsy->model = BYM_EXTENDED_DEVICE;

    // Se il sistema risulta NON connesso non fa altro..
    if(data.at(_BP_EXT_CONNESSIONE)==_BP_EXT_CONNESSIONE_DISCONNECTED)
    {
        pBiopsy->connected = FALSE;
        bypass_y_scroll = false;
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
            movingCommand =_BIOPSY_MOVING_COMPLETED;
            movingError = _BIOPSY_MOVING_ERROR_TIMEOUT;
        }
        return;
    }

    // Cambio stato da Non connesso a connesso
    if(!pBiopsy->connected){

        // Riconoscimento torretta di Biopsia inserita
        pBiopsy->connected = TRUE;
        pBiopsy->checksum_h=data[_BP_EXT_CHKH];
        pBiopsy->checksum_l=data[_BP_EXT_CHKL];
        pBiopsy->revisione=data[_BP_EXT_REVIS];


        movingCommand =_BIOPSY_MOVING_NO_COMMAND;
        movingError = _BIOPSY_MOVING_NO_ERROR;
        pBiopsy->activationId = 0;

        // Aggiorna le pagine con nil riconoscimento della Biopsia
        ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) BIOPSY_DEVICE,0);
        ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("BIOPSY","NOME ACCESSORIO", 0, QApplication::UnicodeUTF8)),0);        
        update_aws = true;
    }

    // Pulsante di sblocco Braccio e funzioni biopsia
    if(data.at(_BP_EXT_PUSH_SBLOCCO) == _BP_EXT_PUSH_SBLOCCO_ATTIVO) unlock_button = true;
    else unlock_button = false;
    ApplicationDatabase.setData(_DB_BIOP_UNLOCK_BUTTON,(int) data.at(_BP_EXT_PUSH_SBLOCCO),0);

    // Lateralità rilevata
    if(curLatX != data.at(_BP_EXT_ASSEX_POSITION)) update_aws = true;
    curLatX = data.at(_BP_EXT_ASSEX_POSITION);
    ApplicationDatabase.setData(_DB_BIOP_LAT_X,(int) data.at(_BP_EXT_ASSEX_POSITION),0);
    if( (curLatX == _BP_EXT_ASSEX_POSITION_CENTER) && (curX_dmm < 2263)  && (curX_dmm > 317)) bypass_y_scroll = true;

    //  Riconoscimento dell'adapter
    if(adapterId != data.at(_BP_EXT_ADAPTER_ID)){
        adapterId = data.at(_BP_EXT_ADAPTER_ID);
        update_aws = true;
        ApplicationDatabase.setData(_DB_BIOP_ADAPTER_ID,(int) data.at(_BP_EXT_ADAPTER_ID),0);
    }

    // Posizione attuale cursore
    curX_dmm = data.at(_BP_EXT_XL) + 256 * data.at(_BP_EXT_XH) ;
    curY_dmm = data.at(_BP_EXT_YL) + 256 * data.at(_BP_EXT_YH) ;
    curZ_dmm = data.at(_BP_EXT_ZL) + 256 * data.at(_BP_EXT_ZH) ;
    curSh_dmm= data.at(_BP_EXT_SHL) + 256 * data.at(_BP_EXT_SHH) ;
    int calibratedSh = calibrateSh();
    if( (calibratedSh > bCalibratedSh + 5) || (calibratedSh < bCalibratedSh - 5)){
        update_aws = true;
        bCalibratedSh = calibratedSh;
    }
    ApplicationDatabase.setData(_DB_BIOP_X,(int) curX_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Y,(int) curY_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Z,(int) curZ_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_SH,(int) calibratedSh,0);


    if(bp_motion!=data.at(_BP_EXT_MOTION)){
        PRINT(QString("BIOPSIA: BP_MOTION=%1").arg((int) data.at(_BP_EXT_MOTION)));
        bp_motion = data.at(_BP_EXT_MOTION);
    }

    if(bp_movecommand!=movingCommand){
        PRINT(QString("BIOPSIA: MOVE CMD=%1").arg((int) movingCommand));
        bp_movecommand = movingCommand;
    }

    // ______________________________ Aggiornamento stato di posizionamento in corso _________________________________________________
    switch(data.at(_BP_EXT_MOTION))
    {

        case _BP_EXT_MOTION_ON:
            PRINT(QString("BIOPSIA:CONFERMA ATTIVAZIONE MOVIMENTO"));
            bypass_y_scroll = false;
        break;

        case _BP_EXT_MOTION_TERMINATED:

            // Valutazione del risultato del movimento
            if(data.at(_BP_EXT_MOTION_END) == _BP_EXT_ERROR_POSITIONINIG)
            {
                movingError = _BIOPSY_MOVING_ERROR_TARGET;
            }else if(data.at(_BP_EXT_MOTION_END) == _BP_EXT_TIMEOUT_COMANDO)
            {
                movingError = _BIOPSY_MOVING_ERROR_TIMEOUT;
            }else  movingError = _BIOPSY_MOVING_NO_ERROR;

            movingCommand =_BIOPSY_MOVING_COMPLETED;
            update_aws = true;
        break;


    } // data.at(_DB_MOTION)


    // Aggiornamento AWS
    if(update_aws) pToConsole->setBiopsyExtendedData();
}



// Restituisce TRUE se la destinazione attraversa l'area di possibile impatto
bool biopsyExtendedDevice::testUpsidePosition(unsigned short X){

    // Con il bypass attivo non si richiede la rotazione dell'asse Y
    if(bypass_y_scroll) return false;

    int xh,xl;
    xh = 2580;
    xl = 0;
    if(curLatX == _BP_EXT_ASSEX_POSITION_RIGHT){
        xh = 2580;
        xl = 1460;
    }else if(curLatX == _BP_EXT_ASSEX_POSITION_CENTER){
        xh = 2263;
        xl = 317;
    }else if(curLatX == _BP_EXT_ASSEX_POSITION_LEFT){
        xh = 1120;
        xl = 0;
    }

    // Verifica se attraversa l'area proibita
    if(
        ((curX_dmm>xh) && (X<xh)) ||
        ((curX_dmm<xl) && (X>xl)) ){
        return true;

    }
    return false;
}

int biopsyExtendedDevice::moveXYZ(unsigned short X, unsigned short Y, unsigned short Z)
{
    unsigned char data[15];

    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return movingError;
    }

    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_XYZ; // Codice comando
    data[1]=(unsigned char) (X & 0x00FF);
    data[2]=(unsigned char) (X >>8);
    data[3]=(unsigned char) (Y & 0x00FF);
    data[4]=(unsigned char) (Y >>8);
    data[5]=(unsigned char) (Z & 0x00FF);
    data[6]=(unsigned char) (Z >>8);

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,7)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return movingError;
    }

    movingCommand =_BIOPSY_MOVING_XYZ;
    movingError = _BIOPSY_MOVING_NO_ERROR;
    return _BIOPSY_MOVING_NO_ERROR;
}

bool biopsyExtendedDevice::isTarget(unsigned short X, unsigned short Y, unsigned short Z){

    if(abs(curX_dmm-X) >= 5)  return false;
    if(abs(curY_dmm-Y) >= 5)  return false;
    if(abs(curZ_dmm-Z) >= 5)  return false;

    return true;
}

/*

int biopsyExtendedDevice::moveDecZ(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_DECZ; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECZ;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}


int biopsyExtendedDevice::moveIncZ(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_INCZ; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCZ;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsyExtendedDevice::moveDecX(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_DECX; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECX;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsyExtendedDevice::moveIncX(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_INCX; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCX;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsyExtendedDevice::moveDecY(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_DECY; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECY;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}
int biopsyExtendedDevice::moveIncY(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_EXT_BIOPSY_CMD_MOVE_INCY; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_UNDEFINED_ERROR;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCY;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}
*/

int biopsyExtendedDevice::setStepVal(unsigned char step)
{
    unsigned char data[2];
    data[0]=_MCC_EXT_BIOPSY_CMD_SET_STEPVAL; // Codice comando
    data[1] = step;
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,2)==FALSE) return 0;
    return 1;
}

// Calibra la distanza dalla fibra di carbonio della base dell'adsse X (quando Z è 0)
// per poter impostare la massima salita del compressore
void biopsyExtendedDevice::calibrateXbase(unsigned short val)
{
    pBiopsy->configExt.Z_basePosizionatore = val + curZ_dmm / 10;
    pBiopsy->storeConfigExtended();
    pBiopsy->updateConfig();

}

unsigned char biopsyExtendedDevice::getLatX(void){
    return (unsigned char) curLatX;
}

unsigned char biopsyExtendedDevice::getAdapterId(void){
    return (unsigned char) adapterId;

}

void  biopsyExtendedDevice::setBuzzer(void){
    unsigned char data[2];
    data[0]=_MCC_EXT_BIOPSY_CMD_SET_BUZZER; // Codice comando
    pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1);

}
