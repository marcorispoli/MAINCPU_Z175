#include "toolsmenu.h"
#include "ui_toolsmenu.h"
#include "../../application.h"
#include "../../appinclude.h"
#include "../../globvar.h"

#define UI_PAGINA _PG_SERVICE_TOOLS_MENU
#define EXIT_PAGINA _PG_SERVICE_MENU
#define EXIT_BUTTON ui->exitButton
#define DISABLE_EXIT_TMO    1000

toolsmenu::toolsmenu(int rotview, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::toolsMenuUI)
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

    // IMPOSTAZIONI STANDARD _______________________________________________________________________________
    connect(&GWindowRoot,SIGNAL(changePage(int,int)), this,SLOT(changePage(int,int)),Qt::UniqueConnection);
    connect(EXIT_BUTTON,SIGNAL(released()),this,SLOT(onExitButton()),Qt::UniqueConnection);

    //______________________________________________________________________________________________________


    connect(ui->tiltingToolButton,SIGNAL(released()),this,SLOT(onTiltingToolButton()),Qt::UniqueConnection);
    connect(ui->armToolButton,SIGNAL(released()),this,SLOT(onArmToolButton()),Qt::UniqueConnection);
    connect(ui->lenzeDriverButton,SIGNAL(released()),this,SLOT(onLenzeDriverButton()),Qt::UniqueConnection);
    connect(ui->inverterButton,SIGNAL(released()),this,SLOT(onInverterButton()),Qt::UniqueConnection);

}

toolsmenu::~toolsmenu()
{
    delete ui;
}

// Funzione agganciata ai sistemi di menu custom
void toolsmenu::changePage(int pg, int opt)
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
        exitPage();
    }

}

// Reazione alla pressione del pulsante di uscita
void toolsmenu::onExitButton(void)
{

    GWindowRoot.setNewPage(EXIT_PAGINA,GWindowRoot.curPage,0);
}


void toolsmenu::initPage(void){

    unsigned char architettura =  ApplicationDatabase.getDataU(_DB_SYSTEM_CONFIGURATION);
    if(architettura & _ARCH_TRX_MOTOR) ui->tiltingToolButton->show();
    else ui->tiltingToolButton->hide();
    if(architettura & _ARCH_ARM_MOTOR) ui->armToolButton->show();
    else ui->armToolButton->hide();

    return;
}

void toolsmenu::exitPage(void){


    return;
}

void toolsmenu::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId()==changePageTimer)
    {
        killTimer(changePageTimer);
        // Abilita il pulsante di uscita
        EXIT_BUTTON->show();
    }
}

//____________________________________________________________________________________________________________________________________
// Reazione alla pressione del pulsante di uscita


void toolsmenu::onTiltingToolButton(void)
{

    GWindowRoot.setNewPage(_PG_SERVICE_TOOLS_TILT,GWindowRoot.curPage,0);
}

void toolsmenu::onInverterButton(void)
{

    GWindowRoot.setNewPage(_PG_SERVICE_TOOLS_INVERTER,GWindowRoot.curPage,0);
}

void toolsmenu::onArmToolButton(void)
{

    GWindowRoot.setNewPage(_PG_SERVICE_TOOLS_ARM,GWindowRoot.curPage,0);
}

void toolsmenu::onLenzeDriverButton(void)
{

    GWindowRoot.setNewPage(_PG_SERVICE_TOOLS_LENZE,GWindowRoot.curPage,0);
}

