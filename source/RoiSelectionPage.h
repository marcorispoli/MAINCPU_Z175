#ifndef ROIPAGE_H
#define ROIPAGE_H


#include "application.h"


class RoiSelectionPage : public GWindow
{
    Q_OBJECT

public:
    RoiSelectionPage(bool ls, QString bg,QString bgs ,bool showLogo, int w,int h, qreal angolo,QPainterPath pn, int pgpn, QPainterPath pp, int pgpp, int pg);
    virtual ~RoiSelectionPage();
    void childStatusPage(bool stat,int param); // Override funzione della classe base GWindow
                                                // Al cambio pagina riporta lo stato di attivazione

    void timerEvent(QTimerEvent* ev); // Override della classe QObject
    void mousePressEvent(QGraphicsSceneMouseEvent* event); // Override funzione della classe base GWindow
    void nextPageHandler(void);
    void prevPageHandler(void);

    int selectedRoi;

public slots:
    void valueChanged(int,int); // Link esterno alla fonte dei contenuti dei campi valore
    void buttonActivationNotify(int id,bool status,int opt);
    void languageChanged(); // Link esterno alla fonte dei contenuti dei campi valore

private:
    bool disableTimedButtons; //  Disabilitazione a tempo dei bottoni per evitare rimbalzi
    int timerDisableButton;
    void disableButtons(int t){
        if(disableTimedButtons) return;
        disableTimedButtons=true;
        timerDisableButton = startTimer(t);
    }

    void initWindow(void);

    QColor studyColor;   // Colore relativo allo studio in corso
    int timerId; // Usato per la gestione del timer della data

    // Testo per Intestazione
    GLabel* intestazioneValue;
    void setIntestazione();

    // Testo per la DATA DI SISTEMA
    QGraphicsTextItem* dateText;

    GPush* OkButton;
    GPush* CancButton;
    GPush* Sel1;
    GPush* Sel2;
    GPush* Sel3;
    GPush* Sel4;
    GPush* Sel5;
    GPush* Sel6;
    GPush* Sel7;

};

#endif // ROIPAGE_H
