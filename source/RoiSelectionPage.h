#ifndef ROIPAGE_H
#define ROIPAGE_H


#include "application.h"

/**
 * @brief The RoiSelectionPage class
 *
 * # Descrizione Modulo
 *
 * + selectedRoi: questa variabile viene impostata con il numero relativo alla roi selezionata.
 *   Il valore selectedRoi = 0 indica nessuna selezione effettuata (modalità roi automatica)
 *
 *   Il valore selezionabile va da 1 a 7 ma può essere limitato in funzione del paddle.
 *
 */
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

    int selectedRoi;        // Roi selezionata
    int max_selectable_roi; // IMpostato dal chiamante per limitare la selezione

public slots:
    void valueChanged(int,int); // Link esterno alla fonte dei contenuti dei campi valore
    void buttonActivationNotify(int id,bool status,int opt);
    void languageChanged(); // Link esterno alla fonte dei contenuti dei campi valore

private:
    bool disableTimedButtons; //  Disabilitazione a tempo dei bottoni per evitare rimbalzi
    int timerDisableButton;

    void disableButtons(int t){
        if(timerDisableButton){
            killTimer(timerDisableButton);
        }
        disableTimedButtons=true;
        timerDisableButton = startTimer(t);
    }

    void initWindow(void);
    void initButtons(void);
    void updateSelectedButtons(void);

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
    QGraphicsPixmapItem* Sel1Pix;

    GPush* Sel2;
    QGraphicsPixmapItem* Sel2Pix;

    GPush* Sel3;
    QGraphicsPixmapItem* Sel3Pix;

    GPush* Sel4;
    QGraphicsPixmapItem* Sel4Pix;

    GPush* Sel5;
    QGraphicsPixmapItem* Sel5Pix;

    GPush* Sel6;
    QGraphicsPixmapItem* Sel6Pix;

    GPush* Sel7;
    QGraphicsPixmapItem* Sel7Pix;

    GLabel* selectedValue;

};

#endif // ROIPAGE_H
