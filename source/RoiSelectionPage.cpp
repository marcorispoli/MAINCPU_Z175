#define ROIPAGE_C
#include "application.h"
#include "appinclude.h"
#include "globvar.h"


#define BACKGROUNDC "://paginaRoi/paginaRoi/backgroundC.png"
#define BACKGROUNDY "://paginaRoi/paginaRoi/backgroundY.png"
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

    dateText=this->addText("----------",font);
    dateText->setDefaultTextColor(Qt::white);
    dateText->setPos(DATE_LABEL_POSITION);


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


    OkButton = new GPush((GWindow*) this,setPointPath(8,650,370,760,370,760,480,650,480),650,370,0,0,false);
    OkButton->setEnable(true);
    CancButton = new GPush((GWindow*) this,setPointPath(8,0,370,130,370,130,480,0,480),0,370,0,0,false);
    CancButton->setEnable(true);

    timerId = startTimer(1000);
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

    if(ev->timerId()==timerId)
    {

        if(systemTimeUpdated)
            dateText->setPlainText(QDateTime::currentDateTime().toString("dd.MM.yy     hh.mm.ss ap"));
        else
            dateText->setPlainText(QString("--.--.--     --.--.--"));

    }


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
    //case _DB_PROIEZIONI:
    default:
    break;

    }
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
        prevPageHandler();
        return;
    }

    if(pbutton==CancButton)
    {
        selectedRoi = 0;
        prevPageHandler();
        return;
    }


    // Disabilita i bottoni del pannello principale per evitare sovrapposizioni
    if(timerDisableButton) killTimer(timerDisableButton);
    timerDisableButton = 0;
    disableTimedButtons=true;

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

