#define ROIPAGE_C
#include "application.h"
#include "appinclude.h"
#include "globvar.h"


#define BACKGROUNDC "://paginaRoi/paginaRoi/backgroundC.png"
#define BACKGROUNDY "://paginaRoi/paginaRoi/backgroundY.png"
#define BUTTON_UNSELECTED_PIX "://paginaRoi/paginaRoi/RoiSelection.png"
#define BUTTON_SELECTED_PIX "://paginaRoi/paginaRoi/RoiSelected.png"



#define BOUND_INTEST_LABEL     100,8,600,41            // TESTO PER INTESTAZIONE

#define X0 114
#define X1 255
#define X2 395
#define X3 536


#define Y0 69
#define Y1 253

#define DX 123
#define DY 140

#define YL0 228
#define YL1 412
#define LABELBOX    124,20

RoiSelectionPage::RoiSelectionPage(bool local, QString bgl, QString bgs , bool showLogo, int w,int h, qreal angolo,QPainterPath pn, int pgpn, QPainterPath pp, int pgpp, int pg) : GWindow(bgl,showLogo,w,h, angolo,pn,pgpn,pp,pgpp,pg)
{
    QFont font;
    timerId=0;


    // Definizione del testo per la data
    font.setFamily("DejaVuSerif");
    font.setBold(true);
    font.setWeight(75);
    font.setItalic(false);
    font.setPointSize(22);
    font.setStretch(60);

    //dateText=this->addText("----------",font);
    //dateText->setDefaultTextColor(Qt::white);
    //dateText->setPos(DATE_LABEL_POSITION);


    // Definizione del testo per la data
    font.setFamily("DejaVuSerif");
    font.setBold(true);
    font.setWeight(90);
    font.setItalic(false);
    font.setPointSize(70);
    font.setStretch(50);


    // Campo Intestazione
    font.setPointSize(30);
    font.setStretch(40);
    intestazioneValue = new GLabel(this,QRectF(BOUND_INTEST_LABEL ),font,QColor(_C_COL),"",Qt::AlignCenter);
    this->setIntestazione();

    selectedValue = new GLabel(this,QRectF(316,401,189,42 ),font,QColor(_W_TEXT),"",Qt::AlignCenter);


    OkButton = new GPush((GWindow*) this,setPointPath(8,650,370,760,370,760,480,650,480),650,370,0,0,false);
    OkButton->setEnable(true);
    CancButton = new GPush((GWindow*) this,setPointPath(8,0,370,130,370,130,480,0,480),0,370,0,0,false);
    CancButton->setEnable(true);

    Sel1Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel1Pix->setPos(338,347);
    Sel1 = new GPush((GWindow*) this,setPointPath(8, 338,347, 462,347, 462,381, 338,381),338,347,0,0,false);
    Sel1->setEnable(false);


    Sel2Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel2Pix->setPos(338,312);
    Sel2 = new GPush((GWindow*) this,setPointPath(8, 338,312, 462,312, 462,346, 338,346),338,312,0,0,false);
    Sel2->setEnable(false);


    Sel3Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel3Pix->setPos(338,277);
    Sel3 = new GPush((GWindow*) this,setPointPath(8, 338,277, 462,277, 462,311, 338,311),338,277,0,0,false);
    Sel3->setEnable(false);

    Sel4Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel4Pix->setPos(338,242);
    Sel4 = new GPush((GWindow*) this,setPointPath(8, 338,242, 462,242, 462,276, 338,276),338,242,0,0,false);
    Sel4->setEnable(false);

    Sel5Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel5Pix->setPos(338,207);
    Sel5 = new GPush((GWindow*) this,setPointPath(8, 338,207, 462,207, 462,241, 338,241),338,207,0,0,false);
    Sel5->setEnable(false);


    Sel6Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel6Pix->setPos(338,172);
    Sel6 = new GPush((GWindow*) this,setPointPath(8, 338,172, 462,172, 462,206, 338,206),338,172,0,0,false);
    Sel6->setEnable(false);

    Sel7Pix = this->addPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    Sel7Pix->setPos(338,137);
    Sel7 = new GPush((GWindow*) this,setPointPath(8, 338,137, 462,137, 462,171, 338,171),338,137,0,0,false);
    Sel7->setEnable(false);

    selectedRoi = 0;
    max_selectable_roi = 0;

    timerId = 0;
    //timerId = startTimer(1000);

    disableTimedButtons = false; // Abilitazione pulsanti
    timerDisableButton = 0;
}

RoiSelectionPage::~RoiSelectionPage()
{
    this->killTimer(timerId);

}

// Questa funzione viene chiamata ogni volta che viene ricevuto il segnale di cambio
// pagina dalla Classe Base. Viene utilizzata per effettuare tutte le inizializzazioni del caso
void RoiSelectionPage::childStatusPage(bool stat,int opt)
{
    if(stat==false){
        if(timerDisableButton) {
            killTimer(timerDisableButton);
            timerDisableButton = 0;
        }
        disableTimedButtons = false;
        disconnect(pagina_language,SIGNAL(changeLanguageSgn()), this,SLOT(languageChanged()));
        disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));
        disconnect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), this,SLOT(buttonActivationNotify(int,bool,int)));

        return;
    }

    connect(pagina_language,SIGNAL(changeLanguageSgn()), this,SLOT(languageChanged()),Qt::UniqueConnection);
    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    connect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), this,SLOT(buttonActivationNotify(int,bool,int)),Qt::UniqueConnection);

    disableButtons(1000); // Disabilita i pulsanti in ingresso
    initWindow();
    return;

}


void RoiSelectionPage::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId()==timerDisableButton)
    {
        disableTimedButtons = false;
        killTimer(timerDisableButton);
        timerDisableButton = 0;
        return;
    }
/*
    if(ev->timerId()==timerId)
    {

        if(systemTimeUpdated)
            dateText->setPlainText(QDateTime::currentDateTime().toString("dd.MM.yy     hh.mm.ss ap"));
        else
            dateText->setPlainText(QString("--.--.--     --.--.--"));

    }
*/

}


void RoiSelectionPage::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

    GWindow::mousePressEvent(event); // Lancia il default della classe

}
void RoiSelectionPage::nextPageHandler(void)
{
    // Azione disabilitata
    if(disableTimedButtons) return ;

    GWindow::nextPageHandler();
}

void RoiSelectionPage::prevPageHandler(void)
{
    // Azione disabilitata
    if(disableTimedButtons) return ;

    GWindow::prevPageHandler();
}


// FUNZIONE DI AGGIORNAMENTO CAMPI VALORE CONNESSO AI CAMPI DEL DATABASE
void RoiSelectionPage::valueChanged(int index,int opt)
{
    QString val;

    switch(index)
    {
    // Chiamato all'apertura della pagina per impostare i pulsanti usabili
    // in funzione del massimo numero di roi selezionabili
    case _DB_SERVICE1_INT:
        max_selectable_roi = ApplicationDatabase.getDataI(_DB_SERVICE1_INT);
        initButtons();
    default:
    break;

    }
}

// Chiamato per entrambi i display per inizializzare i pulsanti
// attivabili (max_selectable_roi)
void RoiSelectionPage::initButtons(void){

    if(max_selectable_roi >= 1){
        Sel1->setEnable(true);
        Sel1Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel1Pix->show();
    }else{
        Sel1->setEnable(false);
        Sel1Pix->hide();
    }

    if(max_selectable_roi >= 2){
        Sel2->setEnable(true);
        Sel2Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel2Pix->show();
    }else{
        Sel2->setEnable(false);
        Sel2Pix->hide();
    }

    if(max_selectable_roi >= 3){
        Sel3->setEnable(true);
        Sel3Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel3Pix->show();
    }else{
        Sel3->setEnable(false);
        Sel3Pix->hide();
    }

    if(max_selectable_roi >= 4){
        Sel4->setEnable(true);
        Sel4Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel4Pix->show();
    }else{
        Sel4->setEnable(false);
        Sel4Pix->hide();
    }

    if(max_selectable_roi >= 5){
        Sel5->setEnable(true);
        Sel5Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel5Pix->show();
    }else{
        Sel5->setEnable(false);
        Sel5Pix->hide();
    }


    if(max_selectable_roi >= 6){
        Sel6->setEnable(true);
        Sel6Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel6Pix->show();
    }else{
        Sel6->setEnable(false);
        Sel6Pix->hide();
    }

    if(max_selectable_roi >= 7){
        Sel7->setEnable(true);
        Sel7Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
        Sel7Pix->show();
    }else{
        Sel7->setEnable(false);
        Sel7Pix->hide();
    }

}

// Chiamato per entrambi i displays,
// Aggiorna lo stato dei pulsanti in funzione del valore aggiornato
// di selectedRoi
void RoiSelectionPage::updateSelectedButtons(void){
    if(selectedRoi >= 1){
       Sel1Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
    }else{
       Sel1Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
    }

    if(selectedRoi >= 2){
        Sel2Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
     }else{
        Sel2Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
     }

    if(selectedRoi >= 3){
        Sel3Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
     }else{
        Sel3Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
     }

    if(selectedRoi >= 4){
        Sel4Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
     }else{
        Sel4Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
     }

    if(selectedRoi >= 5){
        Sel5Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
     }else{
        Sel5Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
     }

    if(selectedRoi >= 6){
        Sel6Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
     }else{
        Sel6Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
     }

     if(selectedRoi >= 7){
        Sel7Pix->setPixmap(QPixmap(BUTTON_SELECTED_PIX));
     }else{
        Sel7Pix->setPixmap(QPixmap(BUTTON_UNSELECTED_PIX));
     }

    selectedValue->labelText = QString("%1").arg(selectedRoi);
    selectedValue->update();
}

/*_________________________________________________________________________________________
    // ATTENZIONE, A FUTURA MEMORIA: I PULSANTI SONO BI-STABILI (salvo i combo)
    // Se si vuole che interagiscano ad ogni click occorre sfruttare entrambi gli stati di ON
    // e OFF
 _________________________________________________________________________________________ */
void RoiSelectionPage::buttonActivationNotify(int id, bool status,int opt)
{
    GPush* pbutton = (GPush*) GWindowRoot.pushList.at(id);
    if(pbutton->parentWindow!=this) return; // Scarta i segnali da altre pagine
    if(opt&DBase::_DB_NO_ACTION) return; // Questa condizione si impone per evitare rimbalzi da echo
    if(disableTimedButtons) return;
    if(!isCurrentPage()) return;

    if(pbutton==OkButton)
    {
        if(isMaster) paginaOpenStudyDigital->updateAECMode(selectedRoi);
        prevPageHandler();              
        return;
    }

    if(pbutton==CancButton)
    {
        selectedRoi = 0;
        if(isMaster) paginaOpenStudyDigital->updateAECMode(selectedRoi);
        prevPageHandler();
        return;
    }

    // Arriva ad entrambi i display alla pressione del pulsante: aggiorna la grafica su entrambi
    if(pbutton==Sel1)
    {
        selectedRoi = 1;
        updateSelectedButtons();
    }
    if(pbutton==Sel2)
    {
        selectedRoi = 2;
        updateSelectedButtons();
    }
    if(pbutton==Sel3)
    {
        selectedRoi = 3;
        updateSelectedButtons();
    }
    if(pbutton==Sel4)
    {
        selectedRoi = 4;
        updateSelectedButtons();
    }
    if(pbutton==Sel5)
    {
        selectedRoi = 5;
        updateSelectedButtons();
    }
    if(pbutton==Sel6)
    {
        selectedRoi = 6;
        updateSelectedButtons();
    }
    if(pbutton==Sel7)
    {
        selectedRoi = 7;
        updateSelectedButtons();
    }

    // Updates the parent page
    if(isMaster) paginaOpenStudyDigital->updateAECMode(selectedRoi);

    // Disabilita i bottoni del pannello principale per evitare sovrapposizioni    
    disableButtons(500);

}



/*_________________________________________________________________
 *
 *  APERTURA DELLA PAGINA CON LA RICEZIONE DELL'EVENTO OPEN STUDY
 *  Inizializzazione di tutti gli elementi grafici e di tutte
 *  le opzioni di ingresso pagina
 ________________________________________________________________ */

void RoiSelectionPage::initWindow(void){

    if (ApplicationDatabase.getDataU(_DB_STUDY_STAT)==_OPEN_STUDY_DICOM)
    {
        setBackground(BACKGROUNDC);
        studyColor = QColor(_C_COL);

    }else
    {
        studyColor = QColor(_Y_COL);
        setBackground(BACKGROUNDY);

    }

    setIntestazione();

    selectedRoi = 0;
    selectedValue->labelText = "(A)";

    // Solo il master può determinare l'inizializzazione dei pulsanti su entambi i display
    // Si utilizza una variabile tempranea del database:
    if(isMaster){
        ApplicationDatabase.setData(_DB_SERVICE1_INT,max_selectable_roi, DBase::_DB_FORCE_SGN);

    }

}

void RoiSelectionPage::setIntestazione()
{
    intestazioneValue->labelText="AEC - ROI "; //QString(QApplication::translate("PROJECTION-PAGE","SELEZIONE PROIEZIONE"));
    intestazioneValue->labelColor=studyColor;
    intestazioneValue->update();
    return;
}

// Rinfresca tutte le label cambiate
void RoiSelectionPage::languageChanged()
{
    setIntestazione();
}

