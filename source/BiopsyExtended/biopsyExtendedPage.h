#ifndef BIOPSY_EXTENDED_PAGE_H
#define BIOPSY_EXTENDED_PAGE_H

#include "../application.h"

#include <QObject>

class BiopsyExtendedPage : public GWindow
{
    Q_OBJECT

public:
    BiopsyExtendedPage(bool ls, QString bg,QString bgs ,bool showLogo, int w,int h, qreal angolo,QPainterPath pn, int pgpn, QPainterPath pp, int pgpp, int pg);
    virtual ~BiopsyExtendedPage();
    void childStatusPage(bool stat,int param); // Override funzione della classe base GWindow
                                                // Al cambio pagina riporta lo stato di attivazione

    void timerEvent(QTimerEvent* ev); // Override della classe QObject
    void mousePressEvent(QGraphicsSceneMouseEvent* event); // Override funzione della classe base GWindow

    void setXrayOn(bool stat); // Attiva il simbolo raggi in corso
    void setCalibOn(bool stat); // Attivazione simboli modo Calibrazione

public slots:
    void InitBiopsyPage(void); // Funzione per entrare nella pagina di Biopsia
    void valueChanged(int,int); // Link esterno alla fonte dei contenuti dei campi valore
    void buttonActivationNotify(int id,bool status,int opt);
    void languageChanged(); // Link esterno alla fonte dei contenuti dei campi valore

    void setOpenStudy(void);
    void setCloseStudy(void);

private:
    bool disableTimedButtons; //  Disabilitazione a tempo dei bottoni per evitare rimbalzi
    int timerDisableButton;
    int timerId; // Usato per la gestione del timer della data

    void disableButtons(int t){
        if(disableTimedButtons) return;
        disableTimedButtons=true;
        timerDisableButton = startTimer(t);
    }


    bool localStudy;    // Definisce se lo studio è locale o con PC
    QColor studyColor;   // Colore relativo allo studio in corso


    // Testo per Intestazione
    GLabel* intestazioneValue;
    void setIntestazione();

    // Testo per la DATA DI SISTEMA
    QGraphicsTextItem* dateText;


    // Testo per l'angolo di inclinazione braccio
    GLabel* armValue;
    void setArmAngolo(void);

    // Testo per l'angolo di inclinazione TRX
    GLabel* trxValue;
    void setTrxAngolo(void);

    // Pulsante apertura pagina immagine presente
    GPush* pulsanteImgOn;

    // Pulsante accensione luce biopsia
    GPush* pulsanteLamp;
    int pulsanteLuceMode;
    bool powerledAutoOn; // Stato luce quando in modo automatico
    void managePulsanteLuce(void);
    void managePowerLed(void);

    // Campo Compressione
    GLabel* targetValue;
    GLabel* compressioneValue;
    void setCompressione(void);


    // Campo Spessore
    GLabel* spessoreValue;
    void setSpessore(void);


    // Pulsante apertura pagina allarmi
    GPush* pulsanteAlarmOn;

    // Testo per campo XYZ
    GLabel* biopXYZValue;


    void setBiopXYZ(void);
    void updateBiopsyView(void);
    void updateCursorPointer(void);
    QGraphicsPixmapItem* cursorPix;
    //GLabel* cursorValue;
    QGraphicsTextItem* cursorValue;
    GLabel* tubeTempValue;

    QGraphicsPixmapItem* xrayPix;
    QGraphicsPixmapItem* calibPix;
    QGraphicsPixmapItem* demoPix;

    bool specimenMode;
    bool specimenLeft;

    bool xRayStat;
    bool isOpen;

    void setTubeTemp(void);
    void setInfoPanelView(bool stat);
};

#endif // BIOPSY_EXTENDED_PAGE_H
