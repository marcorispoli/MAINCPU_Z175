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
    connect(ui->buttonConfirmScrollLeftDown,SIGNAL(released()),this,SLOT(onConfirmButton()),Qt::UniqueConnection);
    connect(ui->buttonConfirmScrollRightDown,SIGNAL(released()),this,SLOT(onConfirmButton()),Qt::UniqueConnection);

    ui->frameMoveScrollCenter->setGeometry(0,0,800,480);
    ui->frameMoveScrollLeft->setGeometry(0,0,800,480);
    ui->frameMoveScrollRight->setGeometry(0,0,800,480);

    ui->frameMoveScrollYCenter->setGeometry(0,0,800,480);
    ui->frameMoveScrollYLeft->setGeometry(0,0,800,480);
    ui->frameMoveScrollYRight->setGeometry(0,0,800,480);

    ui->frameMoveScrollYLeftDown->setGeometry(0,0,800,480);
    ui->frameMoveScrollYRightDown->setGeometry(0,0,800,480);

    ui->frameMoveX->setGeometry(0,0,800,480);
    ui->frameMoveY->setGeometry(0,0,800,480);
    ui->frameMoveZ->setGeometry(0,0,800,480);

    hideFrames();


    // Solo il Master gestisce il device
    if(!isMaster) return;
    calibrationMode = false;


    // Inizializzazione sequenze
    req_sequence = _REQ_SEQ_NONE ;
    sub_sequence = 0;
    event_req_sequence = 0;


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

    exitPageCode = _PG_MAIN_DIGITAL;

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
    ui->frameMoveScrollYLeftDown->hide();
    ui->frameMoveScrollYRightDown->hide();

    ui->frameMoveX->hide();
    ui->frameMoveY->hide();
    ui->frameMoveZ->hide();

}

void biopsyExtendedDevice::manageChangeHomeSeq(int sub_seq,int param1, int param2)
{
    hideFrames();

    switch(sub_seq){
    case _REQ_SUBSEQ_HOME_INIT:
        ui->StartupTitleLabel->setText(ApplicationDatabase.getDataS(BIOPSY_ACTIVATION_TITLE_DB));
        break;

    case _REQ_SUBSEQ_HOME_Z_MOVE:
        ui->frameMoveZ->show();
        break;

    case _REQ_SUBSEQ_HOME_Y_MOVE:
        ui->frameMoveY->show();
        break;

    case _REQ_SUBSEQ_HOME_X_MOVE:
        ui->frameMoveX->show();
        break;

    case _REQ_SUBSEQ_HOME_X_SCROLL:
        if(param1 == _PARAM_CENTER) ui->frameMoveScrollCenter->show();
        else if(param1 == _PARAM_LEFT) ui->frameMoveScrollLeft->show();
        else ui->frameMoveScrollRight->show();
        break;

    case _REQ_SUBSEQ_HOME_Y_SCROLL:
        if(param2 == _PARAM_UP){
            if(param1 == _PARAM_CENTER) ui->frameMoveScrollYCenter->show();
            else if(param1 == _PARAM_LEFT) ui->frameMoveScrollYLeft->show();
            else ui->frameMoveScrollYRight->show();
        }else{
            if(param1 == _PARAM_RIGHT) ui->frameMoveScrollYRightDown->show();
            else ui->frameMoveScrollYLeftDown->show();
        }

    break;

    case _REQ_SUBSEQ_HOME_COMPLETED:
        break;

    }
}
void biopsyExtendedDevice::manageChangeMoveXYZSeq(int sub_seq,int param1, int param2)
{
    hideFrames();

    switch(sub_seq){
    case _REQ_SUBSEQ_XYZ_INIT:
        ui->StartupTitleLabel->setText(ApplicationDatabase.getDataS(BIOPSY_ACTIVATION_TITLE_DB));
        break;
    case _REQ_SUBSEQ_XYZ_WAIT_Z:
        ui->frameMoveZ->show();
        break;
    case _REQ_SUBSEQ_XYZ_WAIT_Y:
        ui->frameMoveY->show();
        break;
    case _REQ_SUBSEQ_XYZ_WAIT_X:
        ui->frameMoveX->show();
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
    QString stringa;
    QString valstr;
    int len;
    int i;
    unsigned char seq, subseq, param1, param2;

    switch(index){
    case BIOPSY_ACTIVATION_SEQUENCE_DB:


        stringa = ApplicationDatabase.getDataS(index);

        i = stringa.indexOf(" ");
        len = stringa.length();
        valstr = stringa.left(i);
        seq = valstr.toInt();
        stringa = stringa.right(len-i-1);

        i = stringa.indexOf(" ");
        len = stringa.length();
        valstr = stringa.left(i);
        subseq = valstr.toInt();
        stringa = stringa.right(len-i-1);

        i = stringa.indexOf(" ");
        len = stringa.length();
        valstr = stringa.left(i);
        param1 = valstr.toInt();
        stringa = stringa.right(len-i-1);
        param2 = stringa.toInt();


        if(seq == _REQ_SEQ_HOME) manageChangeHomeSeq(subseq,param1,param2);
        else if(seq == _REQ_SEQ_XYZ) manageChangeMoveXYZSeq(subseq,param1,param2);
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

    // Crea la stringa  di comando per lo slave
    QString stringa = QString("%1 %2 %3 %4").arg(req_sequence).arg(sub_sequence).arg(seq_param1).arg(seq_param2);
    ApplicationDatabase.setData(BIOPSY_ACTIVATION_SEQUENCE_DB, stringa, DBase::_DB_FORCE_SGN);

    // Riaccoda l'evento
    event_req_sequence = startTimer(tmo);
}

void biopsyExtendedDevice::handleXScroll(void){
    static bool init = true;

    if(init){
        // imout init
        init = false;
        buzzer_delay = 0;
        user_timeout = _DEF_TIMEOUT_USER_FEEDBACK;
        init = false;
    }

    // Attende che l'utente scrolli l'asse X nella corretta posizione
    if(req_home_lat != curLatX){
        user_timeout--;
        if(!user_timeout){
            manageRequestErrors(_BIOPSY_MOVING_ERROR_TIMEOUT);
            init = true;
            return;
        }

        if(!buzzer_delay){
            buzzer_delay = 20;
            setBuzzer();
        }else buzzer_delay--;
        event_req_sequence = startTimer(100);
        return;
    }

    // Assignes the exit sequence
    sub_sequence = next_seq;
    init = true;
    nextStepSequence(1);
}



void biopsyExtendedDevice::handleYScroll(void){
    static bool init = true;

    if(init){
        // imout init
        init = false;
        buzzer_delay = 0;
        user_timeout = _DEF_TIMEOUT_USER_FEEDBACK;
        init = false;
        user_confirmation = false;
    }

    // Attende che l'utente confermi
    if(!user_confirmation){
        user_timeout--;
        if(!user_timeout){
            manageRequestErrors(_BIOPSY_MOVING_ERROR_TIMEOUT);
            init = true;
            return;
        }

        if(!buzzer_delay){
            buzzer_delay = 20;
            setBuzzer();
        }else buzzer_delay--;
        event_req_sequence = startTimer(100);
        return;
    }

    sub_sequence = next_seq;
    init = true;
    nextStepSequence(1);
    return;

}

void biopsyExtendedDevice::handleMove(void){
    static bool init = true;

    if(init){
        init = false;

        // Se X già in posizione, verifica se deve controllare la rotazione in basso (left e down)
        if(isTarget(move_X, move_Y, move_Z)){
            sub_sequence = next_seq;
            nextStepSequence(1);
            init = true;
            return;
        }

        // Avvia il comando di posizionamento
        moveXYZ(move_X, move_Y, move_Z);

        if( movingError != _BIOPSY_MOVING_NO_ERROR){
            manageRequestErrors(movingError);
            init = true;
            return;
        }

    }

    if(movingCommand > _BIOPSY_MOVING_COMPLETED){
          event_req_sequence = startTimer(100);
          return;
    }

    if(movingError != _BIOPSY_MOVING_NO_ERROR){
        init = true;
        manageRequestErrors(movingError);
        return;
    }

    // Comando terminato con successo
    sub_sequence = next_seq;
    init = true;
    nextStepSequence(1);
    return;

}
void biopsyExtendedDevice::manageHomeSequence(void){

    switch(sub_sequence){

    case _REQ_SUBSEQ_HOME_X_SCROLL: handleXScroll();break;
    case _REQ_SUBSEQ_HOME_Y_SCROLL: handleYScroll();break;
    case _REQ_SUBSEQ_HOME_X_MOVE:   handleMove();break;
    case _REQ_SUBSEQ_HOME_Y_MOVE:   handleMove();break;
    case _REQ_SUBSEQ_HOME_Z_MOVE:   handleMove();break;

    // Inizializzazione della sequenza
    case _REQ_SUBSEQ_HOME_INIT:

        user_confirmation = false;
        // Assegna la fase in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED) {
            manageRequestErrors(_BIOPSY_MOVING_ERROR_BUSY);
            return;
        }


        // Va direttamente a muovere Y se il cursore sta al centro insieme all'asse X
        if((curLatX == _BP_EXT_ASSEX_POSITION_CENTER) && (testYisUp())){
            sub_sequence = _REQ_SUBSEQ_HOME_EXE_Y;
            nextStepSequence(1);
            break;
        }

        // Sposta il cursore al bordo piu' vicino
        if(curLatX == _BP_EXT_ASSEX_POSITION_RIGHT) move_X = _DEF_EXT_XHOME_RIGHT;
        else if(curLatX == _BP_EXT_ASSEX_POSITION_LEFT) move_X = _DEF_EXT_XHOME_LEFT;
        else if(curX_dmm > 1290) move_X = _DEF_EXT_XHOME_LEFT;
        else move_X = _DEF_EXT_XHOME_RIGHT;
        move_Y = curY_dmm;
        move_Z = curZ_dmm;
        sub_sequence = _REQ_SUBSEQ_HOME_X_MOVE;
        next_seq = _REQ_SUBSEQ_HOME_TEST_Y_UP;
        nextStepSequence(1);
        break;

        // Verifica se è il caso di ruotare Y verso l'alto
        case _REQ_SUBSEQ_HOME_TEST_Y_UP:

            // Se il target non è dove sta ora l'asse allora sicuramente deve chiedere
            if( (req_home_lat != curLatX) || (req_home_lat == _BP_EXT_ASSEX_POSITION_CENTER)){
                // Scroll Y -UP and continue
                sub_sequence = _REQ_SUBSEQ_HOME_Y_SCROLL;
                seq_param2 = _PARAM_UP;

                if(curLatX == _BP_EXT_ASSEX_POSITION_RIGHT) seq_param1 == _PARAM_RIGHT;
                else if(curLatX == _BP_EXT_ASSEX_POSITION_LEFT) seq_param1 == _PARAM_LEFT;
                else if(curLatX == _BP_EXT_ASSEX_POSITION_CENTER) seq_param1 == _PARAM_CENTER;

                next_seq =_REQ_SUBSEQ_HOME_EXE_Y;
                nextStepSequence(1);
                break;
            }

            sub_sequence = _REQ_SUBSEQ_HOME_EXE_Y;
            nextStepSequence(1);

        break;

        // Attivazione dell'azzeramento Y
        case _REQ_SUBSEQ_HOME_EXE_Y:
            move_X = curX_dmm;
            move_Y = req_Y;
            move_Z = curZ_dmm;
            if(req_Y < curY_dmm) seq_param1 = _PARAM_IN;
            else seq_param1 = _PARAM_OUT;
            sub_sequence = _REQ_SUBSEQ_HOME_Y_MOVE;

            next_seq = _REQ_SUBSEQ_HOME_EXE_Z;
            nextStepSequence(1);
        break;


        // Movimento asse Z verso posizione di parcheggio
        case _REQ_SUBSEQ_HOME_EXE_Z:
            move_X = curX_dmm;
            move_Y = curY_dmm;
            move_Z = req_Z;

            if(req_Z < curZ_dmm) seq_param1 = _PARAM_UP;
            else seq_param1 = _PARAM_DOWN;
            sub_sequence = _REQ_SUBSEQ_HOME_Z_MOVE;
            next_seq = _REQ_SUBSEQ_HOME_TEST_SCROLL_X;
            nextStepSequence(1);
        break;

        case _REQ_SUBSEQ_HOME_TEST_SCROLL_X:
            // Se la lateralità è già quella richiesta va direttamente a muovere X
            if(req_home_lat == curLatX)
            {
                sub_sequence = _REQ_SUBSEQ_HOME_EXE_X;
                nextStepSequence(1);
                return;
            }

            if(req_home_lat == _BP_EXT_ASSEX_POSITION_CENTER) seq_param1 = _PARAM_CENTER;
            else if(req_home_lat == _BP_EXT_ASSEX_POSITION_LEFT) seq_param1 = _PARAM_LEFT;
            else seq_param1 = _PARAM_RIGHT;
            next_seq = _REQ_SUBSEQ_HOME_EXE_X; // Return to the same sequence
            sub_sequence = _REQ_SUBSEQ_HOME_X_SCROLL;
            nextStepSequence(1);
        break;



    case _REQ_SUBSEQ_HOME_EXE_X:
        move_X = req_X;
        move_Y = curY_dmm;
        move_Z = curZ_dmm;
        seq_param1  = _PARAM_CENTER;

        sub_sequence = _REQ_SUBSEQ_HOME_X_MOVE;
        next_seq = _REQ_SUBSEQ_HOME_TEST_Y_DOWN;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_HOME_TEST_Y_DOWN:

        if(req_home_lat == _BP_EXT_ASSEX_POSITION_CENTER) {
            // Se home è al centro, finisce qui
            sub_sequence = _REQ_SUBSEQ_HOME_COMPLETED;
            nextStepSequence(1);
            break;
        }

        // Scroll Y
        sub_sequence = _REQ_SUBSEQ_HOME_Y_SCROLL;
        if(req_home_lat == _BP_EXT_ASSEX_POSITION_LEFT)   seq_param1 = _PARAM_LEFT;
        else seq_param1 = _PARAM_RIGHT;
        seq_param2 = _PARAM_DOWN;
        next_seq = _REQ_SUBSEQ_HOME_COMPLETED;
        nextStepSequence(1);
    break;


    case _REQ_SUBSEQ_HOME_COMPLETED:
        isHome = true;

        // Notifica di fine movimento
        movingError = _BIOPSY_MOVING_NO_ERROR;
        if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, _BIOPSY_MOVING_NO_ERROR);
        pBiopsy->activationId = 0;
        GWindowRoot.setNewPage( exitPageCode ,GWindowRoot.curPage,0);
        break;



    // _________________________________________________
    defaut:
        // Case anomalo: si chiude il comando come se fosse tutto ok
        if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, _BIOPSY_MOVING_NO_ERROR);
        pBiopsy->activationId = 0;
        GWindowRoot.setNewPage(exitPageCode ,GWindowRoot.curPage,0);
        break;
    }


    return ;
}


void biopsyExtendedDevice::manageXYZSequence(void){
    switch(sub_sequence){

    // Inizializzazione della sequenza
    case _REQ_SUBSEQ_XYZ_INIT:
        user_confirmation = false;
        isHome = false;

        // Assegna la fase in corso
        if(movingCommand > _BIOPSY_MOVING_COMPLETED) {
            manageRequestErrors(_BIOPSY_MOVING_ERROR_BUSY);
            return;
        }

        // Verifica se ci può essere rischio di impatto
        /*if(testUpsidePosition(req_X)){

            manageRequestErrors(_BIOPSY_MOVING_UNDEFINED_ERROR);
            return;
        } else sub_sequence = _REQ_SUBSEQ_XYZ_EXE_X;
        */
        sub_sequence = _REQ_SUBSEQ_XYZ_EXE_Z;
        nextStepSequence(1);
        break;

    case _REQ_SUBSEQ_XYZ_EXE_Z:

        // Avvia il comando di posizionamento Z
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

        //sub_sequence = _REQ_SUBSEQ_XYZ_COMPLETED;
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

        sub_sequence = _REQ_SUBSEQ_XYZ_COMPLETED;
        nextStepSequence(1);
        break;


    case _REQ_SUBSEQ_XYZ_COMPLETED:


        // Notifica di fine movimento
        if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, _BIOPSY_MOVING_NO_ERROR);
        pBiopsy->activationId = 0;
        GWindowRoot.setNewPage(exitPageCode,GWindowRoot.curPage,0);
        break;
    }
}



/*
 *  Elenco codici errore gestiti

 */
void biopsyExtendedDevice::manageRequestErrors(int error){

    pToConsole->endCommandAck(pBiopsy->activationId, error);
    movingError = _BIOPSY_MOVING_NO_ERROR;
    pBiopsy->activationId = 0;
    GWindowRoot.setNewPage(exitPageCode,GWindowRoot.curPage,0);
    sub_sequence = 0;
    req_sequence = 0;
}

/*
 *  id = id del messaggio della console. 0 = se non richiede una risposta;
 *  lat = lateralità richiesta;
 *
 *  return:
 *  *  0: si trova già in HOME
 *  *  1: comando in esecuzione
 *  * -1: errore lateralità
 *  * -2: Asse X non in posizione
 *  * -3: braccio non in posizione
 */
int biopsyExtendedDevice::requestBiopsyHome(int id, unsigned char lat, int rot_holder){
    QString activationString;


    // Non può essere eseguita con una pagina di errore in corso
    if(paginaAllarmi->isCurrentPage()) return -1;

    // Prima di tutto controlla che il braccio si trovi in CC
    if(ApplicationDatabase.getDataI(_DB_TRX) > 50) return -2;
    if(ApplicationDatabase.getDataI(_DB_TRX) < -50) return -2;

    // Non può essere eseguita senza una rilevazione corretta della lateralità
    if(
            (curLatX !=_BP_EXT_ASSEX_POSITION_LEFT) &&
            (curLatX !=_BP_EXT_ASSEX_POSITION_CENTER) &&
            (curLatX !=_BP_EXT_ASSEX_POSITION_RIGHT) ) return -3;

    prev_home_lat = req_home_lat;
    req_home_lat = lat;
    req_rot_holder = rot_holder;


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

    // Non può essere eseguita con una pagina di errore in corso
    if(paginaAllarmi->isCurrentPage()) return -1;

    // Abilita la visualizzazione del cursore
    ApplicationDatabase.setData(_DB_BIOP_SHOW_SH, (unsigned char) 1);


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



// Calibrates the shaft value
// Return dmm offset over the 0 point
int biopsyExtendedDevice::calibrateSh(ushort sh,ushort sh_m15, ushort sh_p15, ushort sh0){


    float k = (float) 300 / (float) (sh_m15 - sh_p15);
    float  val = (((float) sh  - (float) sh0) * k);
    return - (int) val ;
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

        prev_home_lat = req_home_lat = _BP_EXT_ASSEX_POSITION_ND;

        movingCommand =_BIOPSY_MOVING_NO_COMMAND;
        movingError = _BIOPSY_MOVING_NO_ERROR;
        pBiopsy->activationId = 0;
        isHome = true;

        // Aggiorna le pagine con nil riconoscimento della Biopsia
        ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) BIOPSY_DEVICE,0);
        ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("BIOPSY","NOME ACCESSORIO", 0, QApplication::UnicodeUTF8)),0);        
        update_aws = true;
        nLoop = 0;
    }

    // Pulsante di sblocco Braccio e funzioni biopsia
    if(data.at(_BP_EXT_PUSH_SBLOCCO) == _BP_EXT_PUSH_SBLOCCO_ATTIVO) unlock_button = true;
    else unlock_button = false;
    ApplicationDatabase.setData(_DB_BIOP_UNLOCK_BUTTON,(int) data.at(_BP_EXT_PUSH_SBLOCCO),0);

    // Lateralità rilevata
    if(curLatX != data.at(_BP_EXT_ASSEX_POSITION)) update_aws = true;
    curLatX = data.at(_BP_EXT_ASSEX_POSITION);
    ApplicationDatabase.setData(_DB_BIOP_LAT_X,(int) data.at(_BP_EXT_ASSEX_POSITION),0);
    if(curLatX != _BP_EXT_ASSEX_POSITION_ND) last_xscroll_detected = curLatX;

    // Inizializzazione del flag di comando Home precedente
    if(req_home_lat == _BP_EXT_ASSEX_POSITION_ND){
        prev_home_lat = req_home_lat = curLatX;
    }



    // Quando in home forza la condizione di aperto: aperto o cuneo staccato hanno lo stesso valore riconosciuto!!
    if(isHome) adapterId = _BP_EXT_ADAPTER_OPEN;
    else{
        //  Riconoscimento dell'adapter
        if(adapterId != data.at(_BP_EXT_ADAPTER_ID)){
            adapterId = data.at(_BP_EXT_ADAPTER_ID);
            update_aws = true;
        }
    }
   ApplicationDatabase.setData(_DB_BIOP_ADAPTER_ID,(int) data.at(_BP_EXT_ADAPTER_ID),0);

    // Posizione attuale cursore
    curX_dmm = data.at(_BP_EXT_XL) + 256 * data.at(_BP_EXT_XH) ;
    curY_dmm = data.at(_BP_EXT_YL) + 256 * data.at(_BP_EXT_YH) ;
    curZ_dmm = data.at(_BP_EXT_ZL) + 256 * data.at(_BP_EXT_ZH) ;
    curSh_raw = data.at(_BP_EXT_SHL) + 256 * data.at(_BP_EXT_SHH) ;
    curSh_dmm = calibrateSh(curSh_raw, pBiopsy->configExt.sh_m150_level, pBiopsy->configExt.sh_150_level, pBiopsy->configExt.sh_zero_level);


    if( (curSh_dmm > bCalibratedSh + 5) || (curSh_dmm < bCalibratedSh - 5)){
        update_aws = true;
        bCalibratedSh = curSh_dmm;
    }
    ApplicationDatabase.setData(_DB_BIOP_X,(int) curX_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Y,(int) curY_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Z,(int) curZ_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_SH,(int) curSh_dmm,0);


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

            if(nLoop){
                nLoop--;
                if(seq==0){
                    seq = 1;
                    moveXYZ(x2Loop,y2Loop,z2Loop);
                }else{
                    seq = 0;
                    moveXYZ(x1Loop,y1Loop,z1Loop);
                }
            }
        break;


    } // data.at(_DB_MOTION)


    // Aggiornamento AWS
    if(update_aws) pToConsole->setBiopsyExtendedData();
}



// Restituisce TRUE se la destinazione attraversa l'area di possibile impatto
bool biopsyExtendedDevice::testUpsidePosition(unsigned short X){

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

void  biopsyExtendedDevice::setPowerled(bool stat){
    unsigned char data[2];

    data[0]=_MCC_EXT_BIOPSY_CMD_SET_POWERLED; // Codice comando
    if(stat) data[1]=1;
    else data[1]=0;
    pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,2);

}

bool  biopsyExtendedDevice::setBiopsyLoop(int n, int x1,int y1, int z1, int x2, int y2, int z2){
    if(n==0){
        nLoop = 0;

        return true;
    }

    x1Loop = x1;
    y1Loop = y1;
    z1Loop = z1;
    x2Loop = x2;
    y2Loop = y2;
    z2Loop = z2;
    seq = 1;

    moveXYZ(x2,y2,z2);
    nLoop = n;
    return true;

}

