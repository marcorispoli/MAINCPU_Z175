#define BIOPSY_EXTENDED_PAGE_C
#include "../application.h"
#include "../appinclude.h"
#include "../globvar.h"

BiopsyExtendedPage::BiopsyExtendedPage(bool local, QString bgl, QString bgs , bool showLogo, int w,int h, qreal angolo,QPainterPath pn, int pgpn, QPainterPath pp, int pgpp, int pg) : GWindow(bgl,showLogo,w,h, angolo,pn,pgpn,pp,pgpp,pg)
{
    QFont font;

    localStudy = true;
    studyColor = QColor(_Y_COL);
    nextPageEnabled = false;
    prevPageEnabled = false;


    // Definizione del testo per la data
    font.setFamily("DejaVuSerif");
    font.setBold(true);
    font.setWeight(75);
    font.setItalic(false);
    font.setPointSize(22);
    font.setStretch(60);

    dateText=this->addText("----------",font);
    dateText->setDefaultTextColor(Qt::white);
    dateText->setPos(600,440);

    // Definizione del testo per la data
    font.setFamily("DejaVuSerif");
    font.setBold(true);
    font.setWeight(90);
    font.setItalic(false);
    font.setPointSize(70);
    font.setStretch(50);

    // Campi Tilt e Rot
    font.setPointSize(35);
    font.setStretch(40);
    #define BOUND_BIOPSY_ARM 169,255,59,26
    #define BOUND_BIOPSY_TRX 71,255,59,26
    armValue = new GLabel(this,QRectF(BOUND_BIOPSY_ARM),font,QColor(_W_TEXT),"",Qt::AlignCenter);
    trxValue = new GLabel(this,QRectF(BOUND_BIOPSY_TRX),font,QColor(_W_TEXT),QString(""),Qt::AlignCenter);


    // Campo Intestazione
    #define BOUND_BIOPSY_INTESTAZIONE 14,10,460,40
    font.setPointSize(30);
    font.setStretch(40);
    intestazioneValue = new GLabel(this,QRectF(BOUND_BIOPSY_INTESTAZIONE ),font,studyColor,"",Qt::AlignCenter);


    font.setPointSize(30);
    font.setStretch(35);
    biopXYZValue = new GLabel(this,QRectF(70,87,154,34),font,QColor(_W_TEXT),"",Qt::AlignCenter);


    font.setPointSize(30);
    font.setStretch(40);
    tubeTempValue = new GLabel(this,QRectF(85,187,139,34),font,QColor(_W_TEXT),"",Qt::AlignCenter);


    // Campo Valore Compressione
    #define BOUND_BIOPSY_COMPRESSIONE      55,360,112,38
    font.setPointSize(40);
    font.setStretch(40);
    compressioneValue = new GLabel(this,QRectF(BOUND_BIOPSY_COMPRESSIONE),font,QColor(Qt::white),"",Qt::AlignRight);
    font.setPointSize(20);
    font.setStretch(40);
    targetValue = new GLabel(this,QRectF(36,363,47,22),font,QColor(_DBR_COL),"",Qt::AlignLeft);


    // Campo Valore Spessore
    #define BOUND_BIOPSY_SPESSORE      55,301,112,38
    font.setPointSize(40);
    font.setStretch(40);
    spessoreValue = new GLabel(this,QRectF(BOUND_BIOPSY_SPESSORE),font,QColor(Qt::white),"",Qt::AlignRight);


    // Pulsante Immagine 2D
    pulsanteImgOn = new GPush((GWindow*) this, QPixmap("://BiopsyExtended/BiopsyExtended/imageIcon.png"),QPixmap("://BiopsyExtended/BiopsyExtended/imageIcon.png"),setPointPath(8,700,3,800,3,800,100,700,100),700,3,0,0);
    pulsanteImgOn->setEnable(true);
    pulsanteImgOn->setVisible(false);
    pulsanteImgOn->setOptions(DBase::_DB_NO_ECHO); // pulsante utilizzato esclusivamente dal terminale proprietario

    // Simbolo raggi
    xrayPix = addPixmap(QPixmap("://BiopsyExtended/BiopsyExtended/Xray.png"));
    xrayPix->setPos(310,35);

    // Simbolo Demo
    demoPix = addPixmap(QPixmap(""));
    demoPix->setPos(0,0);

    // Simbolo Calibrazione
    calibPix = addPixmap(QPixmap(""));
    calibPix->setPos(0,0);

    // Cursore SH
    cursorPix = addPixmap(QPixmap("://BiopsyExtended/BiopsyExtended/BiopsyPointerArrow.png"));
    cursorPix->setPos(118,286);
    cursorPix->hide();

    font.setPointSize(30);
    font.setStretch(35);

    cursorValue=this->addText("----",font);
    cursorValue->setDefaultTextColor(Qt::white);
    cursorValue->setPos(162,289);
    //cursorValue = new GLabel(this,QRectF(162,282,58,29),font,QColor(_W_TEXT),"",Qt::AlignCenter);

    specimenMode = false;

    disableTimedButtons = false; // Abilitazione pulsanti
    timerId = startTimer(1000);

    isOpen = false;
}

BiopsyExtendedPage::~BiopsyExtendedPage()
{
   // this->killTimer(timerWDG);
}

void BiopsyExtendedPage::setOpenStudy(void){
    // Aggancia il database
    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), paginaImmagine,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    connect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), paginaImmagine,SLOT(buttonActivationNotify(int,bool,int)),Qt::UniqueConnection);

    connect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), this,SLOT(buttonActivationNotify(int,bool,int)),Qt::UniqueConnection);

    // Con Apertura studio si apre anche la pagina
    InitBiopsyPage();

}
void BiopsyExtendedPage::setCloseStudy(void){
    // Sgancia il database
    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), this,SLOT(valueChanged(int,int)));
    disconnect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), paginaImmagine,SLOT(valueChanged(int,int)));
    disconnect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), paginaImmagine,SLOT(buttonActivationNotify(int,bool,int)));
    disconnect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), this,SLOT(buttonActivationNotify(int,bool,int)));
    paginaImmagine->eraseImage();

    isOpen = false;
    if(isMaster) pConfig->selectMainPage();
}

/*
 *
 *  Funzione chiamata al cambio pagina
 *  Il cambio pagina viene richiesto all'attivazione
 *  della Biopsia.
 *
 */
void BiopsyExtendedPage::InitBiopsyPage(void)
{    

    // Inizializza visualizzazione del puntatore
    ApplicationDatabase.setData(_DB_BIOP_SHOW_SH, (unsigned char) 0);

    // Discrimina fra modalità frustoli e standard
    if(ApplicationDatabase.getDataI(_DB_ACCESSORIO_COLLIMATORE) == COLLI_ACCESSORIO_FRUSTOLI){
        specimenMode = true;
    }else specimenMode = false;

    int angolo = pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_TRX));

    if(specimenMode){
        if(angolo > 0){
            setBackground("://BiopsyExtended/BiopsyExtended/specimen_r.png");
            specimenLeft = false;
        }  else if(angolo <= 0){
            setBackground("://BiopsyExtended/BiopsyExtended/specimen_l.png");
            specimenLeft = true;
        }

        spessoreValue->hide();
        targetValue->hide();
        compressioneValue->hide();
        biopXYZValue->hide();
        intestazioneValue->hide();
        cursorPix->hide();
        cursorValue->hide();
        tubeTempValue->hide();
        trxValue->hide();
        armValue->hide();

    }else{

        // Impostazione Background
        updateBiopsyView();
        intestazioneValue->show();

        targetValue->labelColor=studyColor;
        targetValue->labelText = QString("(%1)").arg(ApplicationDatabase.getDataI(_DB_TARGET_FORCE));
        targetValue->update();


        if(ApplicationDatabase.getDataS(_DB_CALIB_SYM)!=""){
            calibPix->show();
        }else{
            calibPix->hide();
            pulsanteImgOn->setVisible(ImagePage::existImage(ApplicationDatabase.getDataS(_DB_IMAGE_NAME)));
        }


        // Attiva i campi valore per la biopsia
        setIntestazione();
        setSpessore();
        setCompressione();
        setTubeTemp();
        updateCursorPointer();

    }

    // Common for speciment and standard
    pulsanteImgOn->setVisible(false);
    xrayPix->hide();

    if(ApplicationDatabase.getDataU(_DB_DEMO_MODE)==1){
        demoPix->show();
    }else{
        demoPix->hide();
    }

    setTrxAngolo();
    setArmAngolo();
    setBiopXYZ();

}


// Questa funzione viene chiamata ogni volta che viene ricevuto il segnale di cambio
// pagina dalla Classe Base. Viene utilizzata per effettuare tutte le inizializzazioni del caso
void BiopsyExtendedPage::childStatusPage(bool stat,int opt)
{
    if(stat==false){
        if(timerDisableButton) {
            killTimer(timerDisableButton);
            timerDisableButton = 0;
        }
        disableTimedButtons = false;

        return;
    }

    paginaAllarmi->alarm_enable=true;
    disableButtons(500); // Disabilita i pulsanti in ingresso

    if(!isOpen){
        isOpen = true;
        setOpenStudy();
    }else{
        // Caso di rientro da allarme o immagine
        if(ApplicationDatabase.getDataS(_DB_CALIB_SYM)==""){
            pulsanteImgOn->setVisible(ImagePage::existImage(ApplicationDatabase.getDataS(_DB_IMAGE_NAME)));
        }

    }
}

void BiopsyExtendedPage::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId()==timerDisableButton)
    {
        disableTimedButtons = false;
        killTimer(timerDisableButton);
        return;
    }

    if(ev->timerId()==timerId)
    {

        if(systemTimeUpdated)
            dateText->setPlainText(QDateTime::currentDateTime().toString("dd.MM.yy     hh.mm.ss ap"));
        else
            dateText->setPlainText(QString("--.--.--     --.--.--"));
        return;
    }


}


void BiopsyExtendedPage::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

    // Ogni azione di interazione TS viene filtrata per evitare rimbalzi
    if(disableTimedButtons) return;
    GWindow::mousePressEvent(event); // Lancia il default della classe
    disableTimedButtons=true;
    timerDisableButton = startTimer(500);

}


void BiopsyExtendedPage::setIntestazione()
{
    QString stringa = ApplicationDatabase.getDataS(_DB_CALIB_SYM) ;

    if(stringa==""){
        if(ApplicationDatabase.getDataU(_DB_STUDY_STAT)==_OPEN_STUDY_DICOM){
            stringa=QString(QApplication::translate("BIOPSY-PAGE","Nome Paziente"));
            stringa.append(": ");
            stringa.append(ApplicationDatabase.getDataS(_DB_INTESTAZIONE));
        }else{
            stringa=QString(QApplication::translate("BIOPSY-PAGE","Studio Locale"));
            stringa.append(QString::fromUtf8(" NÂ°: "));
            stringa.append(ApplicationDatabase.getDataS(_DB_INTESTAZIONE));
        }
    }

    intestazioneValue->labelText=stringa;
    intestazioneValue->update();

}

void BiopsyExtendedPage::setArmAngolo(void)
{
    QString str;

    // Prende l'angolo del Braccio dal database
    str.setNum(pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_DANGOLO)));
    str.append(QString::fromUtf8("Â°"));
    armValue->labelText=str;
    armValue->labelColor=QColor(_W_TEXT);
    armValue->update();
}

void BiopsyExtendedPage::setTrxAngolo(void)
{
    QString str;

    int angolo = pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_TRX));

    if(specimenMode){
        if((angolo > 0) && (specimenLeft==true)){
            setBackground("://BiopsyExtended/BiopsyExtended/specimen_r.png");
            specimenLeft = false;
        }  else if((angolo <= 0)&& (specimenLeft==false)){
            setBackground("://BiopsyExtended/BiopsyExtended/specimen_l.png");
            specimenLeft = true;
        }


        return;
    }

    if((angolo>=14) && (angolo<=16)) trxValue->labelText="L";
    else if((angolo>=-16) && (angolo<=-14)) trxValue->labelText="R";
    else if((angolo>=-1) && (angolo<=1)) trxValue->labelText="S";
    else
    {
        str.setNum(angolo);
        str.append(QString::fromUtf8("Â°"));
        trxValue->labelText=str;
        trxValue->labelColor=QColor(_R_COL);
        trxValue->update();
        return;
    }

    trxValue->labelColor=QColor(_W_TEXT);
    trxValue->update();
}


void BiopsyExtendedPage::setBiopXYZ(void)
{
    biopXYZValue->labelText=QString("X:%1, Y:%2, Z:%3").arg(ApplicationDatabase.getDataI(_DB_BIOP_X)).arg(ApplicationDatabase.getDataI(_DB_BIOP_Y)).arg(ApplicationDatabase.getDataI(_DB_BIOP_Z));
    biopXYZValue->update();

}


void BiopsyExtendedPage::setCompressione(void)
{

    unsigned int val = ApplicationDatabase.getDataI(_DB_FORZA);

    if(val==0){     
        compressioneValue->labelText="---";
        compressioneValue->update();
    }else{
        compressioneValue->labelText=QString("%1").arg(val);
        compressioneValue->update();
    }

}


void BiopsyExtendedPage::setSpessore(void)
{
    unsigned int val = ApplicationDatabase.getDataI(_DB_SPESSORE);

    if(val==0){
        spessoreValue->labelText="---";
        spessoreValue->update();
    }else{
        spessoreValue->labelText=QString("%1").arg(val);
        spessoreValue->update();
    }

}



// FUNZIONE DI AGGIORNAMENTO CAMPI VALORE CONNESSO AI CAMPI DEL DATABASE
void BiopsyExtendedPage::valueChanged(int index,int opt)
{

    switch(index)
    {
    case _DB_T_CUFFIA:
        setTubeTemp();
        break;
    case _DB_REQ_POWEROFF:
        if(!isMaster) return;
        if(ApplicationDatabase.getDataU(index)) pConfig->activatePowerOff();
        break;

    case _DB_XRAY_PUSH_BUTTON:
        if(!isMaster) break;
        // Segnale di attivazione pulsante raggi
        if(ApplicationDatabase.getDataU(_DB_XRAY_PUSH_BUTTON)) pToConsole->activationXrayPush();
        break;


    case _DB_DANGOLO:
        setArmAngolo();
        break;

    case _DB_BIOP_UNLOCK_BUTTON:
    case _DB_BIOP_LAT_X:
    case _DB_BIOP_ADAPTER_ID:
        updateBiopsyView();
        break;

    case _DB_BIOP_SH:
        updateCursorPointer();
        break;

    case _DB_BIOP_X:
    case _DB_BIOP_Y:
    case _DB_BIOP_Z:       
        setBiopXYZ();
        break;

    case _DB_TRX:
        setTrxAngolo();
        if((isMaster) && (pCollimatore->accessorio == COLLI_ACCESSORIO_FRUSTOLI)) pCollimatore->updateColli();
        break;

    case _DB_SPESSORE:
        setSpessore();
       break;

    case _DB_FORZA:
        setCompressione();
        break;

    case _DB_INTESTAZIONE:
        setIntestazione();
        break;

    case _DB_DEMO_MODE:
        if(ApplicationDatabase.getDataU(_DB_DEMO_MODE)==1) demoPix->show();
        else   demoPix->hide();
        break;

    case _DB_XRAY_SYM:
        if(ApplicationDatabase.getDataU(_DB_DEMO_MODE)==1) return;

        if(ApplicationDatabase.getDataU(index)) xrayPix->show();
        else  xrayPix->hide();
        break;

    case _DB_TARGET_FORCE:
        targetValue->labelText = QString("(%1)").arg(ApplicationDatabase.getDataI(_DB_TARGET_FORCE));
        targetValue->update();
        break;

    case _DB_IMAGE_NAME:
        if(ApplicationDatabase.getDataS(_DB_CALIB_SYM)!="") return;
        pulsanteImgOn->setVisible(ImagePage::existImage(ApplicationDatabase.getDataS(index)));
        break;

    case _DB_CALIB_SYM:
        if(ApplicationDatabase.getDataS(_DB_CALIB_SYM)!="") calibPix->show();
        else   calibPix->hide();
        setIntestazione();
        break;


    case _DB_STUDY_STAT:
        if(!ApplicationDatabase.getDataU(index)) setCloseStudy();
        break;


    case _DB_AWS_CONNECTION:
        if(!isMaster) return;

        // Con la perdita di connessione lo studio deve essere chiuso
        if(ApplicationDatabase.getDataU(_DB_AWS_CONNECTION)==0){
             ApplicationDatabase.setData(_DB_STUDY_STAT,(unsigned char) 0); // Questo fa chiudere la pagina e uscire alla MAIN
        }
        break;

    case _DB_ACCESSORIO:
        if(!isMaster) return;

        // Si estrae la biopsia con lo studio aperto
        if(ApplicationDatabase.getDataU(index)!=BIOPSY_DEVICE){
            ApplicationDatabase.setData(_DB_STUDY_STAT,(unsigned char) 0); // Questo fa chiudere la pagina e uscire alla MAIN
        }
        break;

    case _DB_ACCESSORIO_COLLIMATORE:
        if((ApplicationDatabase.getDataI(_DB_ACCESSORIO_COLLIMATORE) == COLLI_ACCESSORIO_FRUSTOLI) && (specimenMode==false)) InitBiopsyPage();
        else if((ApplicationDatabase.getDataI(_DB_ACCESSORIO_COLLIMATORE) != COLLI_ACCESSORIO_FRUSTOLI) && (specimenMode==true)) InitBiopsyPage();
        break;

    }
}


void BiopsyExtendedPage::buttonActivationNotify(int id, bool status,int opt)
{

    GPush* pbutton = (GPush*) GWindowRoot.pushList.at(id);
    if(pbutton->parentWindow!=this) return; // Scarta i segnali da altre pagine


    // Solo stati attivi
    if(opt&DBase::_DB_NO_ACTION) return; // Questa condizione si impone per evitare rimbalzi da echo

    if(pbutton == pulsanteImgOn){
        paginaImmagine->showPage();
        return;
    }

    // Gestione pulsanti di step Z per biopsia
    /*
    if(isMaster)
    {
        if(pbutton == pulsanteBiopStepUp) pBiopsy->moveDecZ(0);
        else if(pbutton == pulsanteBiopStepDown) pBiopsy->moveIncZ(0);

    }*/

}


// Rinfresca tutte le label cambiate
void BiopsyExtendedPage::languageChanged()
{
    setIntestazione();
}

void BiopsyExtendedPage::updateCursorPointer(void){

    int sh = ApplicationDatabase.getDataI(_DB_BIOP_SH);
    if(sh < -170) sh = -170;
    else if(sh > 150) sh = 150;
    if((sh < 5) && (sh>-5)) sh = 0;

    if(sh == -170) cursorValue->setPlainText(QString("^^^"));
    else if(sh == 150) cursorValue->setPlainText(QString("___"));
    else cursorValue->setPlainText(QString("%1").arg(sh));
    cursorValue->setPos(162,289 + (sh*8)/10);
    cursorPix->setPos(118,287 + (sh*8)/10);
}

void BiopsyExtendedPage::updateBiopsyView(void){
    // int adapter = ApplicationDatabase.getDataI(_DB_BIOP_ADAPTER_ID);
    unsigned char cursor_enabled = ApplicationDatabase.getDataU(_DB_BIOP_SHOW_SH);

    // Se il bottone di sblocco è premuto ha la precedenza
    if(ApplicationDatabase.getDataI(_DB_BIOP_UNLOCK_BUTTON)){
        if(ApplicationDatabase.getDataI(_DB_FORZA)) setBackground("://BiopsyExtended/BiopsyExtended/unlockWindowNotPermitted.png");
        else setBackground("://BiopsyExtended/BiopsyExtended/unlockWindow.png");
        setInfoPanelView(true);
        return;
    }

    switch(ApplicationDatabase.getDataI(_DB_BIOP_LAT_X)){
    case _BP_EXT_ASSEX_POSITION_ND:
        setBackground("://BiopsyExtended/BiopsyExtended/undefXScroll.png");
        setInfoPanelView(true);
        break;
    case _BP_EXT_ASSEX_POSITION_LEFT:
        if(cursor_enabled){
            setBackground("://BiopsyExtended/BiopsyExtended/homeLaccessorio.png");
            setInfoPanelView(false);
            // Aggiorna la posizione attuale del cursore SH
            updateCursorPointer();
        }else{
            setBackground("://BiopsyExtended/BiopsyExtended/homeL.png");
            setInfoPanelView(true);
        }
        /*
        if((adapter == _BP_EXT_ADAPTER_OPEN) || (adapter == _BP_EXT_ADAPTER_SHORT)){
            setBackground("://BiopsyExtended/BiopsyExtended/homeL.png");
            setInfoPanelView(true);
        }else{
            setBackground("://BiopsyExtended/BiopsyExtended/homeLaccessorio.png");
            setInfoPanelView(false);
            // Aggiorna la posizione attuale del cursore SH
            updateCursorPointer();
        }*/
        break;
    case _BP_EXT_ASSEX_POSITION_CENTER:
        if(cursor_enabled){
            setBackground("://BiopsyExtended/BiopsyExtended/homeCaccessorio.png");
            setInfoPanelView(false);
            // Aggiorna la posizione attuale del cursore SH
            updateCursorPointer();
        }else{
            setBackground("://BiopsyExtended/BiopsyExtended/homeC.png");
            setInfoPanelView(true);
        }
        /*
        if((adapter == _BP_EXT_ADAPTER_OPEN) || (adapter == _BP_EXT_ADAPTER_SHORT)){
            setBackground("://BiopsyExtended/BiopsyExtended/homeC.png");
            setInfoPanelView(true);
        }else{
            setBackground("://BiopsyExtended/BiopsyExtended/homeCaccessorio.png");
            setInfoPanelView(false);
            // Aggiorna la posizione attuale del cursore SH
            updateCursorPointer();
        }*/
        break;
    case _BP_EXT_ASSEX_POSITION_RIGHT:
        if(cursor_enabled){
            setBackground("://BiopsyExtended/BiopsyExtended/homeRaccessorio.png");
            setInfoPanelView(false);
            // Aggiorna la posizione attuale del cursore SH
            updateCursorPointer();
        }else{
            setBackground("://BiopsyExtended/BiopsyExtended/homeR.png");
            setInfoPanelView(true);
        }
        /*
        if((adapter == _BP_EXT_ADAPTER_OPEN) || (adapter == _BP_EXT_ADAPTER_SHORT)){
            setBackground("://BiopsyExtended/BiopsyExtended/homeR.png");
            setInfoPanelView(true);
        }else{
            setBackground("://BiopsyExtended/BiopsyExtended/homeRaccessorio.png");
            setInfoPanelView(false);
            // Aggiorna la posizione attuale del cursore SH
            updateCursorPointer();

        }*/
        break;
    }


}

void BiopsyExtendedPage::setTubeTemp(void){
    QString str;
    float hu;

    // Se c'è un allarme sensore, la temperatura non viene mostrata
    int temp = ApplicationDatabase.getDataI(_DB_T_CUFFIA);

    // Estrazione status cuffia
    int cuffiaStat = (temp >> 8)& 0x00FF;
    temp = temp &0x00FF;

    // Con errore sensore cuffia non si mostra la temperatura
    if(cuffiaStat==4) {
        tubeTempValue->labelColor=QColor(_RED_CUFFIA);
        str = "!!!";
        tubeTempValue->labelText=str;
        tubeTempValue->update();
        return;
    }

    if(cuffiaStat==2) tubeTempValue->labelColor=QColor(_RED_CUFFIA);
    else if(cuffiaStat==1) tubeTempValue->labelColor=QColor(_ORANGE_CUFFIA);
    else tubeTempValue->labelColor=QColor(_GREEN_CUFFIA);

    hu = (float) 100 * 0.9 * (temp - 25) / 40;
    if(hu<0) hu=0;

    str = QString("T:%1").arg(temp) + QString::fromUtf8("Â°  ") + QString("HU:%2\%").arg((int) hu);
    tubeTempValue->labelText=str;
    tubeTempValue->update();
}

// Visualizza o disattiva il pannello informativo
void BiopsyExtendedPage::setInfoPanelView(bool stat){
    if(stat){
        // Pannello info ON
        tubeTempValue->show();
        targetValue->show();
        compressioneValue->show();
        spessoreValue->show();
        trxValue->show();
        armValue->show();

        // Pannello cursor OFF
        cursorPix->hide();
        cursorValue->hide();

    }else{
        // Pannello info OFF
        tubeTempValue->hide();
        targetValue->hide();
        compressioneValue->hide();
        spessoreValue->hide();
        trxValue->hide();
        armValue->hide();

        // Pannello cursor ON
        cursorPix->show();
        cursorValue->show();

    }

}
