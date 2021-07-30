#include "calibColli.h"
#include "ui_calibColli.h"


#include "../../application.h"
#include "../../appinclude.h"
#include "../../globvar.h"


#define UI_PAGINA _PG_SERVICE_CALIB_COLLI
#define EXIT_PAGINA _PG_SERVICE_CALIB_MENU
#define EXIT_BUTTON ui->exitButton
#define DISABLE_EXIT_TMO    1000

#define _CALIB_COLLI_PAD_LABEL      _DB_SERVICE1_STR

#define _CALIB_COLLI_LEFT_LABEL     _DB_SERVICE1_INT
#define _CALIB_COLLI_RIGHT_LABEL    _DB_SERVICE2_INT
#define _CALIB_COLLI_FRONT_LABEL    _DB_SERVICE3_INT
#define _CALIB_COLLI_BACK_LABEL     _DB_SERVICE4_INT
#define _CALIB_COLLI_TRAP_LABEL     _DB_SERVICE5_INT
#define _CALIB_COLLI_BUTTON_EVENT   _DB_SERVICE6_INT
    #define _CALIB_COLLI_BUTTON_STORE        1
    #define _CALIB_COLLI_BUTTON_SCROLL_LEFT  2
    #define _CALIB_COLLI_BUTTON_SCROLL_RIGHT 3
    #define _CALIB_COLLI_BUTTON_UPDATE       4

#define _CALIB_COLLI_ENABLE_DATA   _DB_SERVICE7_INT
#define _CALIB_COLLI_INFO_FRAME    _DB_SERVICE8_INT

// Defines per calc pad
#define CALIB_COLLI_FRONT_DATA 1
#define CALIB_COLLI_BACK_DATA 2
#define CALIB_COLLI_LEFT_DATA 3
#define CALIB_COLLI_RIGHT_DATA 4
#define CALIB_COLLI_TRAP_DATA 5


calibColli::calibColli(int rotview, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calibColliUI)
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

    // IMPOSTAZIONI STANDARD _______________________________________________________________________________
    connect(&GWindowRoot,SIGNAL(changePage(int,int)), this,SLOT(changePage(int,int)),Qt::UniqueConnection);
    connect(EXIT_BUTTON,SIGNAL(released()),this,SLOT(onExitButton()),Qt::UniqueConnection);

    //______________________________________________________________________________________________________

    connect(ui->frontEdit,SIGNAL(selectionChanged()),this,SLOT(onFrontEdit()),Qt::UniqueConnection);
    connect(ui->backEdit,SIGNAL(selectionChanged()),this,SLOT(onBackEdit()),Qt::UniqueConnection);
    connect(ui->leftEdit,SIGNAL(selectionChanged()),this,SLOT(onLeftEdit()),Qt::UniqueConnection);
    connect(ui->rightEdit,SIGNAL(selectionChanged()),this,SLOT(onRightEdit()),Qt::UniqueConnection);
    connect(ui->trapEdit,SIGNAL(selectionChanged()),this,SLOT(onTrapEdit()),Qt::UniqueConnection);

    connect(ui->storeButton,SIGNAL(released()),this,SLOT(onStoreButton()),Qt::UniqueConnection);
    connect(ui->updateButton,SIGNAL(released()),this,SLOT(onUpdateButton()),Qt::UniqueConnection);
    connect(ui->scrollLeft,SIGNAL(released()),this,SLOT(onScrollLeftButton()),Qt::UniqueConnection);
    connect(ui->scrollRight,SIGNAL(released()),this,SLOT(onScrollRightButton()),Qt::UniqueConnection);

    ui->infoFrame->setGeometry(20,115,746,296);
    ui->infoFrame->hide();
    timerDisable = 0;
    timerInfoFrame = 0;
}

calibColli::~calibColli()
{
    delete ui;
}

// Funzione agganciata ai sistemi di menu custom
void calibColli::changePage(int pg,  int opt)
{
    if(UI_PAGINA==pg)
    {

        // Attivazione pagina
        if(GWindowRoot.curPageVisible== TRUE){
            // Disabilitazione allarmi di sistema
            paginaAllarmi->alarm_enable = false;

            // Disabilita il pulsante di uscita per un certo tempo
            EXIT_BUTTON->hide();
            changePageTimer = startTimer(DISABLE_EXIT_TMO);
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
        if(timerDisable){
            killTimer(timerDisable);
            timerDisable = 0;
        }
        exitPage();

    }

}


// Reazione alla pressione del pulsante di uscita
void calibColli::onExitButton(void)
{

    GWindowRoot.setNewPage(EXIT_PAGINA,GWindowRoot.curPage,0);
}

void calibColli::initPage(void){

    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);

    // Fondamentale per non corrompere il dato custom
    currentPad = 255;

    ApplicationDatabase.setData(_CALIB_COLLI_FRONT_LABEL,(int) 0,DBase::_DB_FORCE_SGN|DBase::_DB_NO_ECHO);
    ApplicationDatabase.setData(_CALIB_COLLI_BACK_LABEL,(int) 0,DBase::_DB_FORCE_SGN|DBase::_DB_NO_ECHO);
    ApplicationDatabase.setData(_CALIB_COLLI_LEFT_LABEL,(int) 0,DBase::_DB_FORCE_SGN|DBase::_DB_NO_ECHO);
    ApplicationDatabase.setData(_CALIB_COLLI_RIGHT_LABEL,(int) 0,DBase::_DB_FORCE_SGN|DBase::_DB_NO_ECHO);
    ApplicationDatabase.setData(_CALIB_COLLI_TRAP_LABEL,(int) 0,DBase::_DB_FORCE_SGN|DBase::_DB_NO_ECHO);

    if(isMaster){

        // Inizializza il dato relativo alla configurazione eventualmente da modificare
        customL = pCollimatore->customL;
        customR = pCollimatore->customR;
        customT = pCollimatore->customT;
        customB = pCollimatore->customB;
        customF = pCollimatore->customF;


        // Seleziona il pad custom
        selectPad(PAD_ENUM_SIZE) ;

    }



    pCalculator = new numericPad(rotview,view, parent);
    connect(pCalculator,SIGNAL(calcSgn(bool)),this,SLOT(calculatorSlot(bool)));
}

// Operazioni da compiere all'uscita dalla pagina
void calibColli::exitPage(void){

    if(timerDisable) killTimer (timerDisable);
    timerDisable=0;
    if(timerInfoFrame) killTimer (timerInfoFrame);
    timerInfoFrame=0;

    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));
    disconnect(pCalculator);
    pCalculator->deleteLater(); // importante !!!
    pCalculator = 0;

    if(!isMaster) return;
    pCollimatore->manualCollimation = false;
    pCollimatore->updateColli();
}

void calibColli::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId()==changePageTimer)
    {
        killTimer(changePageTimer);
        // Abilita il pulsante di uscita
        EXIT_BUTTON->show();
    }

    if(ev->timerId()==timerDisable)
    {
        killTimer(timerDisable);
        timerDisable=0;
    }

    if(ev->timerId()==timerInfoFrame)
    {
        killTimer(timerInfoFrame);
        timerInfoFrame=0;
        ApplicationDatabase.setData(_CALIB_COLLI_INFO_FRAME,0, DBase::_DB_FORCE_SGN);
    }


}

// FUNZIONE DI AGGIORNAMENTO CAMPI VALORE CONNESSO AI CAMPI DEL DATABASE
void calibColli::valueChanged(int index,int opt)
{
    switch(index){
    case _CALIB_COLLI_LEFT_LABEL:
        ui->leftEdit->setText(QString("%1").arg(ApplicationDatabase.getDataI(index)));
        if(currentPad == PAD_ENUM_SIZE) customL = ApplicationDatabase.getDataI(index);
        break;
    case _CALIB_COLLI_RIGHT_LABEL:
        ui->rightEdit->setText(QString("%1").arg(ApplicationDatabase.getDataI(index)));
        if(currentPad == PAD_ENUM_SIZE) customR = ApplicationDatabase.getDataI(index);
        break;
    case _CALIB_COLLI_FRONT_LABEL:
        ui->frontEdit->setText(QString("%1").arg(ApplicationDatabase.getDataI(index)));
        if(currentPad == PAD_ENUM_SIZE) customF = ApplicationDatabase.getDataI(index);
        break;
    case _CALIB_COLLI_BACK_LABEL:
        ui->backEdit->setText(QString("%1").arg(ApplicationDatabase.getDataI(index)));
        if(currentPad == PAD_ENUM_SIZE) customB = ApplicationDatabase.getDataI(index);
        break;
    case _CALIB_COLLI_TRAP_LABEL:
        ui->trapEdit->setText(QString("%1").arg(ApplicationDatabase.getDataI(index)));
        if(currentPad == PAD_ENUM_SIZE) customT = ApplicationDatabase.getDataI(index);
        break;
    case _CALIB_COLLI_PAD_LABEL:
        ui->padLabel->setText(QString("%1").arg(ApplicationDatabase.getDataS(index)));
        break;
    case _CALIB_COLLI_ENABLE_DATA:
        if(ApplicationDatabase.getDataI(index)) ui->disableEditFrame->hide();
        else ui->disableEditFrame->show();
        break;
    case _CALIB_COLLI_INFO_FRAME:
        if(ApplicationDatabase.getDataI(index)) ui->infoFrame->show();
        else ui->infoFrame->hide();
        break;

    case _CALIB_COLLI_BUTTON_EVENT:
        if(!isMaster) return;
        switch(ApplicationDatabase.getDataI(index)){
        case _CALIB_COLLI_BUTTON_STORE:
            ApplicationDatabase.setData(_CALIB_COLLI_INFO_FRAME,1, DBase::_DB_FORCE_SGN);
            if(timerInfoFrame) killTimer(timerInfoFrame);
            timerInfoFrame = startTimer(2000);
            if(  (customF!=pCollimatore->customF)||(customB!=pCollimatore->customB)||(customL!=pCollimatore->customL)||(customR!=pCollimatore->customR)||(customT!=pCollimatore->customT) ){
                pCollimatore->customF = customF;
                pCollimatore->customB = customB;
                pCollimatore->customL = customL;
                pCollimatore->customR = customR;
                pCollimatore->customT = customT;
                pCollimatore->storeConfigFile();
            }
            break;
        case _CALIB_COLLI_BUTTON_SCROLL_LEFT:
            currentPad--;
            if(currentPad < 0 ) currentPad = PAD_ENUM_SIZE;
            selectPad(currentPad);
            break;
        case _CALIB_COLLI_BUTTON_SCROLL_RIGHT:
            currentPad++;
            if(currentPad > PAD_ENUM_SIZE) currentPad = 0;
            selectPad(currentPad);
            break;
        case _CALIB_COLLI_BUTTON_UPDATE:
            pCollimatore->manualCollimation = true;
            pCollimatore->manualF = ApplicationDatabase.getDataI(_CALIB_COLLI_FRONT_LABEL);
            pCollimatore->manualB = ApplicationDatabase.getDataI(_CALIB_COLLI_BACK_LABEL);
            pCollimatore->manualL = ApplicationDatabase.getDataI(_CALIB_COLLI_LEFT_LABEL);
            pCollimatore->manualR = ApplicationDatabase.getDataI(_CALIB_COLLI_RIGHT_LABEL);
            pCollimatore->manualT = ApplicationDatabase.getDataI(_CALIB_COLLI_TRAP_LABEL);
            pCollimatore->manualColliUpdate();
            break;

        }

        break;
    }
}


void calibColli::onFrontEdit(void){
    dataField = "";
    pCalculator->activate(&dataField, QString("SET FRONT BLADE [0:255])"), CALIB_COLLI_FRONT_DATA);
}
void calibColli::onBackEdit(void){
    dataField = "";
    pCalculator->activate(&dataField, QString("SET BACK BLADE [0:255])"), CALIB_COLLI_BACK_DATA);
}
void calibColli::onLeftEdit(void){
    dataField = "";
    pCalculator->activate(&dataField, QString("SET LEFT BLADE [0:255])"), CALIB_COLLI_LEFT_DATA);

}
void calibColli::onRightEdit(void){
    dataField = "";
    pCalculator->activate(&dataField, QString("SET RIGHT BLADE [0:255])"), CALIB_COLLI_RIGHT_DATA);

}
void calibColli::onTrapEdit(void){
    dataField = "";
    pCalculator->activate(&dataField, QString("SET TRAP BLADE [0:255])"), CALIB_COLLI_TRAP_DATA);

}


void calibColli::onStoreButton(void){
    ApplicationDatabase.setData(_CALIB_COLLI_BUTTON_EVENT,(int) _CALIB_COLLI_BUTTON_STORE,DBase::_DB_FORCE_SGN);
}
void calibColli::onScrollLeftButton(void){
     ApplicationDatabase.setData(_CALIB_COLLI_BUTTON_EVENT,(int) _CALIB_COLLI_BUTTON_SCROLL_LEFT,DBase::_DB_FORCE_SGN);
}
void calibColli::onScrollRightButton(void){
     ApplicationDatabase.setData(_CALIB_COLLI_BUTTON_EVENT,(int) _CALIB_COLLI_BUTTON_SCROLL_RIGHT,DBase::_DB_FORCE_SGN);
}
void calibColli::onUpdateButton(void){
     ApplicationDatabase.setData(_CALIB_COLLI_BUTTON_EVENT,(int) _CALIB_COLLI_BUTTON_UPDATE,DBase::_DB_FORCE_SGN);
}

void calibColli::calculatorSlot(bool state){
    if(state==false) return;

    // Controllo sul valore acquisito
    if(dataField=="") return;
    int val = dataField.toInt();
    if(val>255) val=255;
    if(val<0) val=0;

    switch(pCalculator->activation_code){
        case CALIB_COLLI_FRONT_DATA:
            ApplicationDatabase.setData(_CALIB_COLLI_FRONT_LABEL,(int) val);
        break;
        case CALIB_COLLI_BACK_DATA:
            ApplicationDatabase.setData(_CALIB_COLLI_BACK_LABEL,(int) val);
        break;
        case CALIB_COLLI_LEFT_DATA:
            ApplicationDatabase.setData(_CALIB_COLLI_LEFT_LABEL,(int) val);
        break;
        case CALIB_COLLI_RIGHT_DATA:
            ApplicationDatabase.setData(_CALIB_COLLI_RIGHT_LABEL,(int) val);
        break;
        case CALIB_COLLI_TRAP_DATA:
            ApplicationDatabase.setData(_CALIB_COLLI_TRAP_LABEL,(int) val);
        break;
    }

}

// SOLO IL MASTER LO DEVE FARE
void calibColli::selectPad(unsigned char pad){
    int index;

    currentPad = pad;

    // Nome del pad
    if(pad >= PAD_ENUM_SIZE)
        ApplicationDatabase.setData(_CALIB_COLLI_PAD_LABEL,"CUSTOM");
    else
        ApplicationDatabase.setData(_CALIB_COLLI_PAD_LABEL,pCompressore->getPadName((Pad_Enum)pad));

    // Enable Disable Fields
    if(pad >= PAD_ENUM_SIZE) ApplicationDatabase.setData(_CALIB_COLLI_ENABLE_DATA,(int) 1);
    else ApplicationDatabase.setData(_CALIB_COLLI_ENABLE_DATA,(int) 0);

    // Riempimento campi
    if(pad >= PAD_ENUM_SIZE){
        ApplicationDatabase.setData(_CALIB_COLLI_FRONT_LABEL,(int) customF);
        ApplicationDatabase.setData(_CALIB_COLLI_BACK_LABEL,(int) customB);
        ApplicationDatabase.setData(_CALIB_COLLI_LEFT_LABEL,(int) customL);
        ApplicationDatabase.setData(_CALIB_COLLI_RIGHT_LABEL,(int) customR);
        ApplicationDatabase.setData(_CALIB_COLLI_TRAP_LABEL,(int) customT);
    }else{
        if(pad!=PAD_TOMO_24x30)  index = pCollimatore->getColli2DIndex(pad);
        else index = pCollimatore->getColli2DIndex(PAD_24x30);
        if(index<0){
            ApplicationDatabase.setData(_CALIB_COLLI_FRONT_LABEL,(int) 0);
            ApplicationDatabase.setData(_CALIB_COLLI_BACK_LABEL,(int) 0);
            ApplicationDatabase.setData(_CALIB_COLLI_LEFT_LABEL,(int) 0);
            ApplicationDatabase.setData(_CALIB_COLLI_RIGHT_LABEL,(int) 0);
            ApplicationDatabase.setData(_CALIB_COLLI_TRAP_LABEL,(int) 0);
        }else{
            _colliPadStr colli2D = pCollimatore->colliConf.colli2D.at(index);
            ApplicationDatabase.setData(_CALIB_COLLI_FRONT_LABEL,(int) colli2D.F);
            ApplicationDatabase.setData(_CALIB_COLLI_BACK_LABEL,(int) colli2D.B);
            ApplicationDatabase.setData(_CALIB_COLLI_LEFT_LABEL,(int) colli2D.L);
            ApplicationDatabase.setData(_CALIB_COLLI_RIGHT_LABEL,(int) colli2D.R);
            ApplicationDatabase.setData(_CALIB_COLLI_TRAP_LABEL,(int) colli2D.T);
        }

    }
}


