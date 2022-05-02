#include "calibBiopsyExtended.h"
#include "ui_calibBiopsyExtended.h"

#include "../../application.h"
#include "../../appinclude.h"
#include "../../globvar.h"

#define UI_PAGINA _PG_SERVICE_CALIB_EXTENDED_BIOPSY
#define EXIT_PAGINA _PG_SERVICE_CALIB_MENU
#define EXIT_BUTTON ui->exitButton
#define DISABLE_EXIT_TMO    1000

#define _BIOPCAL_BUTTON_HANDLER _DB_SERVICE1_INT
#define _BIOPCAL_CURSOR_SEQUENCE _DB_SERVICE2_INT
#define _BIOPCAL_X_SEQUENCE _DB_SERVICE3_INT
#define _BIOPCAL_Y_SEQUENCE _DB_SERVICE4_INT
#define _BIOPCAL_Z_SEQUENCE _DB_SERVICE5_INT
#define _BIOCAL_SHRAW_UPDATE  _DB_SERVICE6_INT
#define _BIOCAL_CURSOR_ERROR  _DB_SERVICE7_INT

CalibBiopsyExtendedClass::CalibBiopsyExtendedClass(int rotview, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibBiopsyExtended)
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


    connect(ui->CursorMenuButton,SIGNAL(released()),this,SLOT(onCursorMenuButton()),Qt::UniqueConnection);
    connect(ui->XMenuButton,SIGNAL(released()),this,SLOT(onXMenuButton()),Qt::UniqueConnection);
    connect(ui->YMenuButton,SIGNAL(released()),this,SLOT(onYMenuButton()),Qt::UniqueConnection);
    connect(ui->ZMenuButton,SIGNAL(released()),this,SLOT(onZMenuButton()),Qt::UniqueConnection);

    connect(ui->CursorCancelButton,SIGNAL(released()),this,SLOT(onCursorCancelButton()),Qt::UniqueConnection);
    connect(ui->ButtonCursorErr,SIGNAL(released()),this,SLOT(onCursorCancelButton()),Qt::UniqueConnection);
    connect(ui->CursorNextButton,SIGNAL(released()),this,SLOT(onCursorNextButton()),Qt::UniqueConnection);
    connect(ui->CursorStartButton,SIGNAL(released()),this,SLOT(onCursorStartButton()),Qt::UniqueConnection);
    connect(ui->CursorStoreButton,SIGNAL(released()),this,SLOT(onCursorStoreButton()),Qt::UniqueConnection);

    connect(ui->XCancelButton,SIGNAL(released()),this,SLOT(onXCancelButton()),Qt::UniqueConnection);
    connect(ui->XNextButton,SIGNAL(released()),this,SLOT(onXNextButton()),Qt::UniqueConnection);
    connect(ui->XStartButton,SIGNAL(released()),this,SLOT(onXStartButton()),Qt::UniqueConnection);
    connect(ui->XStoreButton,SIGNAL(released()),this,SLOT(onXStoreButton()),Qt::UniqueConnection);

    connect(ui->YCancelButton,SIGNAL(released()),this,SLOT(onYCancelButton()),Qt::UniqueConnection);
    connect(ui->YNextButton,SIGNAL(released()),this,SLOT(onYNextButton()),Qt::UniqueConnection);
    connect(ui->YStartButton,SIGNAL(released()),this,SLOT(onYStartButton()),Qt::UniqueConnection);
    connect(ui->YStoreButton,SIGNAL(released()),this,SLOT(onYStoreButton()),Qt::UniqueConnection);

    connect(ui->ZCancelButton,SIGNAL(released()),this,SLOT(onZCancelButton()),Qt::UniqueConnection);
    connect(ui->ZNextButton,SIGNAL(released()),this,SLOT(onZNextButton()),Qt::UniqueConnection);
    connect(ui->ZStartButton,SIGNAL(released()),this,SLOT(onZStartButton()),Qt::UniqueConnection);
    connect(ui->ZStoreButton,SIGNAL(released()),this,SLOT(onZStoreButton()),Qt::UniqueConnection);


    ui->BiopsyMenuFrame->setGeometry(0,100,800,380);
    ui->CursorCalibrationFrame->setGeometry(0,100,800,380);
    ui->XCalibrationFrame->setGeometry(0,100,800,380);
    ui->YCalibrationFrame->setGeometry(0,100,800,380);
    ui->ZCalibrationFrame->setGeometry(0,100,800,380);
    ui->cursorErrorFrame->setGeometry(215,100,380,260);

    ui->BiopsyMenuFrame->hide();
    ui->CursorCalibrationFrame->hide();
    ui->XCalibrationFrame->hide();
    ui->YCalibrationFrame->hide();
    ui->ZCalibrationFrame->hide();
    ui->cursorErrorFrame->hide();

    seqTimer = 0;

}

CalibBiopsyExtendedClass::~CalibBiopsyExtendedClass()
{
    delete ui;
}

// Funzione agganciata ai sistemi di menu custom
void CalibBiopsyExtendedClass::changePage(int pg,  int opt)
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
void CalibBiopsyExtendedClass::onExitButton(void)
{

    GWindowRoot.setNewPage(EXIT_PAGINA,GWindowRoot.curPage,0);
}

void CalibBiopsyExtendedClass::initPage(void){

    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);

    selectionWorkingFrame(_BIOPCAL_FRAME_MENU);

    if(!isMaster) return;

    seqTimer = startTimer(500);
    pBiopsyExtended->calibrationMode = true;
    return;
}

void CalibBiopsyExtendedClass::exitPage(void){
    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));

    if(!isMaster) return;
    if(seqTimer) killTimer(seqTimer);
    seqTimer = 0;

    pBiopsyExtended->calibrationMode = false;
    return;
}

void CalibBiopsyExtendedClass::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId()==changePageTimer)
    {
        killTimer(changePageTimer);
        // Abilita il pulsante di uscita
        EXIT_BUTTON->show();
    }

    if(ev->timerId()==seqTimer)
    {
        if(workingFrame == _BIOPCAL_FRAME_CURSOR){
            if(viewShRaw){
                ApplicationDatabase.setData(_BIOCAL_SHRAW_UPDATE,(int) pBiopsyExtended->curSh_raw);
            }else {
                ApplicationDatabase.setData(_BIOCAL_SHRAW_UPDATE,(int) pBiopsyExtended->calibrateSh(pBiopsyExtended->curSh_raw, SH_M15,SH_P15, SH_0));
            }


        }
    }


}

void CalibBiopsyExtendedClass::onCursorMenuButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_SELECT_CALIB_CURSOR,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onXMenuButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_SELECT_CALIB_X,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onYMenuButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_SELECT_CALIB_Y,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onZMenuButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_SELECT_CALIB_Z,DBase::_DB_FORCE_SGN);
    return;
}

void CalibBiopsyExtendedClass::onCursorCancelButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_CURSOR_CANCEL,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onCursorNextButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_CURSOR_NEXT,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onCursorStartButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_CURSOR_START,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onCursorStoreButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_CURSOR_STORE,DBase::_DB_FORCE_SGN);
    return;
}

void CalibBiopsyExtendedClass::onXCancelButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_X_CANCEL,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onXNextButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_X_NEXT,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onXStartButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_X_START,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onXStoreButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_X_STORE,DBase::_DB_FORCE_SGN);
    return;
}


void CalibBiopsyExtendedClass::onYCancelButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Y_CANCEL,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onYNextButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Y_NEXT,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onYStartButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Y_START,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onYStoreButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Y_STORE,DBase::_DB_FORCE_SGN);
    return;
}

void CalibBiopsyExtendedClass::onZCancelButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Z_CANCEL,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onZNextButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Z_NEXT,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onZStartButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Z_START,DBase::_DB_FORCE_SGN);
    return;
}
void CalibBiopsyExtendedClass::onZStoreButton(void){
    ApplicationDatabase.setData(_BIOPCAL_BUTTON_HANDLER, (int) _BUTT_Z_STORE,DBase::_DB_FORCE_SGN);
    return;
}


void CalibBiopsyExtendedClass::buttonHandler(_ButtonEnumeration button_code){
    uchar cval;

    switch(button_code){
    case _BUTT_SELECT_CALIB_CURSOR:
        selectionWorkingFrame(_BIOPCAL_FRAME_CURSOR);
        break;
    case _BUTT_SELECT_CALIB_X:
        selectionWorkingFrame(_BIOPCAL_FRAME_X);
        break;
    case _BUTT_SELECT_CALIB_Y:
        selectionWorkingFrame(_BIOPCAL_FRAME_Y);
        break;
    case _BUTT_SELECT_CALIB_Z:
        selectionWorkingFrame(_BIOPCAL_FRAME_Z);
        break;

    case _BUTT_CURSOR_START:
        if(isMaster) ApplicationDatabase.setData(_BIOPCAL_CURSOR_SEQUENCE,(int) 1, DBase::_DB_FORCE_SGN);
        break;
    case _BUTT_CURSOR_CANCEL:
        selectionWorkingFrame(_BIOPCAL_FRAME_MENU);
        break;
    case _BUTT_CURSOR_STORE:
        if(isMaster){
            if(cursorStore() == true) selectionWorkingFrame(_BIOPCAL_FRAME_MENU);
            else selectionWorkingFrame(_BIOPCAL_CURSOR_FRAME_ERROR);
        }
        break;
    case _BUTT_CURSOR_NEXT:
        if(isMaster) {
            cval = ApplicationDatabase.getDataI(_BIOPCAL_CURSOR_SEQUENCE);
            cval++;
            if(cval > 4) cval = 0;
            ApplicationDatabase.setData(_BIOPCAL_CURSOR_SEQUENCE,(int) cval, DBase::_DB_FORCE_SGN);
        }
        break;

    case _BUTT_X_START:
        break;
    case _BUTT_X_CANCEL:
        selectionWorkingFrame(_BIOPCAL_FRAME_MENU);
        break;
    case _BUTT_X_STORE:
        break;
    case _BUTT_X_NEXT:
        break;

    case _BUTT_Y_START:
        break;
    case _BUTT_Y_CANCEL:
        selectionWorkingFrame(_BIOPCAL_FRAME_MENU);
        break;
    case _BUTT_Y_STORE:
        break;
    case _BUTT_Y_NEXT:
        break;

    case _BUTT_Z_START:
        break;
    case _BUTT_Z_CANCEL:
        selectionWorkingFrame(_BIOPCAL_FRAME_MENU);
        break;
    case _BUTT_Z_STORE:
        break;
    case _BUTT_Z_NEXT:
        break;

    }
}

void CalibBiopsyExtendedClass::CursorSequenceHandler(uchar seq){
    switch(seq){
    case 0:

        if(isMaster){
            // Upload the current calibrated levels from the system
            SH_M15 = pBiopsy->configExt.sh_m150_level;
            SH_P15 = pBiopsy->configExt.sh_150_level;
            SH_0 =  pBiopsy->configExt.sh_zero_level;

        }
        viewShRaw = false;
        ui->Cursor_M15_Label->hide();
        ui->Cursor_0_Label->hide();
        ui->Cursor_P15_Label->hide();

        ui->cursor_M15_Ok->hide();
        ui->cursor_0_Ok->hide();
        ui->cursor_P15_Ok->hide();

        ui->CursorStoreButton->hide();
        ui->CursorNextButton->hide();
        ui->CursorStartLabel->show();

        break;
    case 1:
        viewShRaw = true;
        ui->Cursor_M15_Label->show();
        ui->Cursor_0_Label->hide();
        ui->Cursor_P15_Label->hide();

        ui->cursor_M15_Ok->hide();
        ui->cursor_0_Ok->hide();
        ui->cursor_P15_Ok->hide();

        ui->CursorStoreButton->hide();
        ui->CursorNextButton->show();
        ui->CursorStartLabel->hide();

        break;
    case 2:
        viewShRaw = true;
        SH_M15 = rawSh;
        ui->Cursor_M15_Label->show();
        ui->Cursor_0_Label->show();
        ui->Cursor_P15_Label->hide();

        ui->cursor_M15_Ok->show();
        ui->cursor_0_Ok->hide();
        ui->cursor_P15_Ok->hide();

        ui->CursorStoreButton->hide();
        ui->CursorNextButton->show();
        ui->CursorStartLabel->hide();
        break;
    case 3:
        viewShRaw = true;
        SH_0 = rawSh;
        ui->Cursor_M15_Label->show();
        ui->Cursor_0_Label->show();
        ui->Cursor_P15_Label->show();

        ui->cursor_M15_Ok->show();
        ui->cursor_0_Ok->show();
        ui->cursor_P15_Ok->hide();

        ui->CursorStoreButton->hide();
        ui->CursorNextButton->show();
        ui->CursorStartLabel->hide();
        break;
    case 4:
        viewShRaw = false;
        SH_P15 = rawSh;
        ui->Cursor_M15_Label->show();
        ui->Cursor_0_Label->show();
        ui->Cursor_P15_Label->show();

        ui->cursor_M15_Ok->show();
        ui->cursor_0_Ok->show();
        ui->cursor_P15_Ok->show();

        ui->CursorStoreButton->show();
        ui->CursorNextButton->hide();
        ui->CursorStartLabel->hide();

        break;
    }
}
void CalibBiopsyExtendedClass::XSequenceHandler(uchar seq){

}
void CalibBiopsyExtendedClass::YSequenceHandler(uchar seq){

}
void CalibBiopsyExtendedClass::ZSequenceHandler(uchar seq){

}

void CalibBiopsyExtendedClass::selectionWorkingFrame(_WorkingFrame working){
    ui->BiopsyMenuFrame->hide();
    ui->CursorCalibrationFrame->hide();
    ui->XCalibrationFrame->hide();
    ui->YCalibrationFrame->hide();
    ui->ZCalibrationFrame->hide();
    ui->CursorCalibrationFrame->hide();

    workingFrame = working;

    switch(working){
    case _BIOPCAL_FRAME_MENU:
        ui->BiopsyMenuFrame->show();
        break;
    case _BIOPCAL_FRAME_CURSOR:
        ui->CursorCalibrationFrame->show();
        if(isMaster) ApplicationDatabase.setData(_BIOPCAL_CURSOR_SEQUENCE,(int) 0, DBase::_DB_FORCE_SGN);
        break;
    case _BIOPCAL_FRAME_X:
        ui->XCalibrationFrame->show();
        if(isMaster) ApplicationDatabase.setData(_BIOPCAL_X_SEQUENCE,(int) 0, DBase::_DB_FORCE_SGN);
        break;
    case _BIOPCAL_FRAME_Y:
        ui->YCalibrationFrame->show();
        if(isMaster) ApplicationDatabase.setData(_BIOPCAL_Y_SEQUENCE,(int) 0, DBase::_DB_FORCE_SGN);
        break;
    case _BIOPCAL_FRAME_Z:
        ui->ZCalibrationFrame->show();
        if(isMaster) ApplicationDatabase.setData(_BIOPCAL_Z_SEQUENCE,(int) 0, DBase::_DB_FORCE_SGN);
        break;

     case _BIOPCAL_CURSOR_FRAME_ERROR:
        ui->CursorCalibrationFrame->show();
        if(isMaster){
            ApplicationDatabase.setData(_BIOCAL_CURSOR_ERROR, (int) cursor_errcode);

        }
        break;
    }
}

// FUNZIONE DI AGGIORNAMENTO CAMPI VALORE CONNESSO AI CAMPI DEL DATABASE
void CalibBiopsyExtendedClass::valueChanged(int index,int opt)
{
    switch(index){
    case _BIOPCAL_BUTTON_HANDLER:
        buttonHandler((_ButtonEnumeration) ApplicationDatabase.getDataI(index));
        break;
    case _BIOPCAL_CURSOR_SEQUENCE:
        CursorSequenceHandler((_ButtonEnumeration) ApplicationDatabase.getDataI(index));
        break;
    case _BIOPCAL_X_SEQUENCE:
        XSequenceHandler((_ButtonEnumeration) ApplicationDatabase.getDataI(index));
        break;
    case _BIOPCAL_Y_SEQUENCE:
        YSequenceHandler((_ButtonEnumeration) ApplicationDatabase.getDataI(index));
        break;
    case _BIOPCAL_Z_SEQUENCE:
        ZSequenceHandler((_ButtonEnumeration) ApplicationDatabase.getDataI(index));
        break;

    case _BIOCAL_SHRAW_UPDATE:
        rawSh = ApplicationDatabase.getDataI(index);
        if(viewShRaw){
            ui->CursorTitle->setText("DIGITAL SENSOR  VALUE");
        }else{
            ui->CursorTitle->setText("CURRENT CURSOR VALUE");
        }
        ui->cursorVal->setText(QString("%1").arg(rawSh));
        break;
    case _BIOCAL_CURSOR_ERROR:
        int err = ApplicationDatabase.getDataI(index);
        if(err == 1) ui->CursorErrorLabel->setText("!1 (P15 - M15) > 500");
        else if(err == 2) ui->CursorErrorLabel->setText("!2 (P0 - M15) > (P15 - M15)/3");
        else  ui->CursorErrorLabel->setText("!3 (P15 - P0) > (P15 - M15)/3");
        break;
    }
}

bool CalibBiopsyExtendedClass::cursorStore(void){

     // Check the sampled values
     if(SH_P15 - SH_M15 < 500) {
         cursor_errcode = 1;
         return false;
     }
     float D = (SH_P15 - SH_M15) / 3;
     if(SH_0 < SH_M15 + D){
         cursor_errcode = 2;
         return false;
     }
     if(SH_0 > SH_P15 - D){
         cursor_errcode = 3;
         return false;
     }

     cursor_errcode = 0;
     pBiopsy->configExt.sh_m150_level = SH_M15;
     pBiopsy->configExt.sh_150_level = SH_P15;
     pBiopsy->configExt.sh_zero_level = SH_0;
     pBiopsy->storeConfigExtended();

     return true;

}
