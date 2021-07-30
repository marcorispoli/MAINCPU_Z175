// system  system
// systemUI  systemUI

// _PG_SYSTEM_SETUP  codice pagina
// _PG_MAIN  codice pagina exit

#include "system.h"
#include "ui_system.h"


#include "../../application.h"
#include "../../appinclude.h"
#include "../../globvar.h"


#define UI_PAGINA _PG_SYSTEM_SETUP
#define EXIT_PAGINA _PG_MAIN
#define EXIT_BUTTON ui->exitButton
#define DISABLE_EXIT_TMO    1000

systemPage::systemPage(int rotview, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::systemUI)
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


    timerDisable = 0;
}

systemPage::~systemPage()
{
    delete ui;
}

// Funzione agganciata ai sistemi di menu custom
void systemPage::changePage(int pg,  int opt)
{
    if(UI_PAGINA==pg)
    {

        // Attivazione pagina
        if(GWindowRoot.curPageVisible== TRUE){
            // Disabilitazione allarmi di sistema
            paginaAllarmi->alarm_enable = false;

            // Disabilita il pulsante di uscita per un certo tempo
            // EXIT_BUTTON->hide();
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
void systemPage::onExitButton(void)
{
    if(ApplicationDatabase.getDataU(_DB_SYSTEM_CONFIGURATION) & _ARCH_GANTRY_DIGITAL)
        GWindowRoot.setNewPage(_PG_MAIN_DIGITAL,GWindowRoot.curPage,0);
    else
        GWindowRoot.setNewPage(_PG_MAIN_ANALOG,GWindowRoot.curPage,0);
}

void systemPage::initPage(void){


    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    connect(EXIT_BUTTON,SIGNAL(released()),this,SLOT(onExitButton()),Qt::UniqueConnection);

    //______________________________________________________________________________________________________
    connect(ui->buttonLeftGantry,SIGNAL(released()),this,SLOT(onLeftGantry()),Qt::UniqueConnection);
    connect(ui->buttonLeftStarter,SIGNAL(released()),this,SLOT(onLeftStarter()),Qt::UniqueConnection);
    connect(ui->buttonLeftDetector,SIGNAL(released()),this,SLOT(onLeftDetector()),Qt::UniqueConnection);
    connect(ui->buttonRightGantry,SIGNAL(released()),this,SLOT(onRightGantry()),Qt::UniqueConnection);
    connect(ui->buttonRightStarter,SIGNAL(released()),this,SLOT(onRightStarter()),Qt::UniqueConnection);
    connect(ui->buttonRightDetector,SIGNAL(released()),this,SLOT(onRightDetector()),Qt::UniqueConnection);
    connect(ui->buttonRightRotation,SIGNAL(released()),this,SLOT(onRightRotation()),Qt::UniqueConnection);
    connect(ui->buttonLeftRotation,SIGNAL(released()),this,SLOT(onLeftRotation()),Qt::UniqueConnection);
    connect(ui->buttonRightTilt,SIGNAL(released()),this,SLOT(onRightTilt()),Qt::UniqueConnection);
    connect(ui->buttonLeftTilt,SIGNAL(released()),this,SLOT(onLeftTilt()),Qt::UniqueConnection);


    if(!isMaster) return;

    ApplicationDatabase.setData(_DB_SERVICE1_INT,(int) pConfig->sys.gantryModel,DBase::_DB_FORCE_SGN);
    ApplicationDatabase.setData(_DB_SERVICE2_INT,(int) pConfig->sys.highSpeedStarter,DBase::_DB_FORCE_SGN);
    ApplicationDatabase.setData(_DB_SERVICE3_INT,(int) pConfig->sys.armMotor,DBase::_DB_FORCE_SGN);
    ApplicationDatabase.setData(_DB_SERVICE4_INT,(int) pConfig->sys.detectorType,DBase::_DB_FORCE_SGN);
    ApplicationDatabase.setData(_DB_SERVICE5_INT,(int) pConfig->sys.trxMotor,DBase::_DB_FORCE_SGN);

}

// Operazioni da compiere all'uscita dalla pagina
void systemPage::exitPage(void){
    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));
    if(timerDisable) killTimer(timerDisable);
    timerDisable = 0;

    if(isMaster){
        // Salvataggio della configurazione e reboot della macchina
        pConfig->sys.gantryModel = ApplicationDatabase.getDataI(_DB_SERVICE1_INT);
        pConfig->sys.highSpeedStarter = ApplicationDatabase.getDataI(_DB_SERVICE2_INT);
        pConfig->sys.armMotor = ApplicationDatabase.getDataI(_DB_SERVICE3_INT);
        pConfig->sys.trxMotor = ApplicationDatabase.getDataI(_DB_SERVICE5_INT);
        pConfig->sys.detectorType = ApplicationDatabase.getDataI(_DB_SERVICE4_INT);        
        pConfig->saveSysCfg();
        pConfig->executeReboot();
    }
    
}

void systemPage::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId()==changePageTimer)
    {
        killTimer(changePageTimer);
        // Abilita il pulsante di uscita
        //EXIT_BUTTON->show();
    }

    if(ev->timerId()==timerDisable)
    {
        killTimer(timerDisable);
        timerDisable=0;
    }
}

// FUNZIONE DI AGGIORNAMENTO CAMPI VALORE CONNESSO AI CAMPI DEL DATABASE
void systemPage::valueChanged(int index,int opt)
{
    switch(index)
    {
    case _DB_SERVICE1_INT:
        ui->gantry->setText(getGantryStr(ApplicationDatabase.getDataI(index)));
    break;
    case _DB_SERVICE2_INT:
        ui->starter->setText(getStarterStr(ApplicationDatabase.getDataI(index)));
    break;
    case _DB_SERVICE3_INT:
        ui->rotation->setText(getRotationStr(ApplicationDatabase.getDataI(index)));
    break;
    case _DB_SERVICE4_INT:
        ui->detector->setText(getDetectorStr(ApplicationDatabase.getDataI(index)));
    break;
    case _DB_SERVICE5_INT:
        ui->tilt->setText(getTiltStr(ApplicationDatabase.getDataI(index)));
    break;


    }

}


void systemPage::onLeftGantry(void){
    int val = ApplicationDatabase.getDataI(_DB_SERVICE1_INT);
    if(val==GANTRY_MODEL_ANALOG) val = GANTRY_MODEL_DIGITAL;
    else val = GANTRY_MODEL_ANALOG;
    ApplicationDatabase.setData(_DB_SERVICE1_INT,val);
}
void systemPage::onRightGantry(void){
    onLeftGantry();
}



void systemPage::onLeftStarter(void){
    int val = ApplicationDatabase.getDataI(_DB_SERVICE2_INT);
    if(val) val = 0;
    else val = 1;
    ApplicationDatabase.setData(_DB_SERVICE2_INT,val);
}
void systemPage::onRightStarter(void){
    onLeftStarter();
}


void systemPage::onLeftRotation(void){
    int val = ApplicationDatabase.getDataI(_DB_SERVICE3_INT);
    if(val) val = 0;
    else val = 1;
    ApplicationDatabase.setData(_DB_SERVICE3_INT,val);
}

void systemPage::onRightRotation(void){
    onLeftRotation();
}

void systemPage::onLeftTilt(void){
    int val = ApplicationDatabase.getDataI(_DB_SERVICE5_INT);
    if(val) val = 0;
    else val = 1;
    ApplicationDatabase.setData(_DB_SERVICE5_INT,val);
}

void systemPage::onRightTilt(void){
    onLeftTilt();
}

void systemPage::onRightDetector(void){
    int val = ApplicationDatabase.getDataI(_DB_SERVICE4_INT);
    val++; if(val>=DETECTOR_NUMBERS) val=0;
    ApplicationDatabase.setData(_DB_SERVICE4_INT,val);

}
void systemPage::onLeftDetector(void){
    int val = ApplicationDatabase.getDataI(_DB_SERVICE4_INT);
    val--; if(val<0) val=DETECTOR_NUMBERS-1;
    ApplicationDatabase.setData(_DB_SERVICE4_INT,val);
}



QString systemPage::getGantryStr(unsigned char val){
    if(val==GANTRY_MODEL_ANALOG) return QString("ANALOGIC MODEL");
    else return QString("DIGITAL MODEL");
}

QString systemPage::getStarterStr(unsigned char val){
    if(val) return QString("HIGH SPEED STARTER");
    else return QString("LOW SPEED STARTER");
}

QString systemPage::getRotationStr(unsigned char val){
    if(val) return QString("ROTATION WITH MOTOR");
    else return QString("MANUAL ROTATION");
}

QString systemPage::getTiltStr(unsigned char val){
    if(val) return QString("TILT WITH MOTOR");
    else return QString("NO TILT");
}


QString systemPage::getDetectorStr(unsigned char val){
    if(val==DETECTOR_SCREENPLUS) return QString("SCREENPLUS");
    else if(val==DETECTOR_LMAM2) return QString("LMAM2");
    else if(val==DETECTOR_SOLO) return QString("SOLO");
    else if(val==DETECTOR_FDI) return QString("FDI");
    else if(val==DETECTOR_LMAMV2) return QString("LMAM2-V2");
    else if(val==DETECTOR_SMAM) return QString("SMAM");
    else if(val==DETECTOR_SMAMV2) return QString("SMAM-V2");
    else if(val==DETECTOR_FDIV2) return QString("FDI-V2");
    else if(val==DETECTOR_SCREENPLUSV2) return QString("SCREENPLUS-V2");
    else return "";
}

