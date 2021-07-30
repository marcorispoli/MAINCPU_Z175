#ifndef ANALOGPAGEOPEN_H
#define ANALOGPAGEOPEN_H

#include "../application.h"
#include "pannelloComandi.h"
#include "pannelloCompressione.h"
#include "pannelloSpessore.h"
#include "pannelloKv.h"
#include "pannelloMas.h"
#include "pannelloOpzioni.h"
#include "pannelloProiezioni.h"
#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class analogUI;
}


class AnalogPageOpen : public QWidget
{
    Q_OBJECT

public:
    explicit AnalogPageOpen(int rotview,QWidget *parent = 0);
    ~AnalogPageOpen();

    // Timer per gestire il pulsante
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

public slots:

    // Funzione associata a GWindw
    void changePage(int pg, int opt);
    void onExitButton(void);
    void valueChanged(int index,int opt);
    void onSblocco_compressore(void);
    void onAlarmButt(void);
    void guiNotify(unsigned char id, unsigned char mcccode, QByteArray data);



private:
    int timerReady;
    int timerDisable;
    Ui::analogUI *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsProxyWidget *proxy;

    void initPage(void);
    void exitPage(void);


    void rotView(int angolo);

    int timer_attesa_dati;
    bool data_ready;
    void startAttesaDati(int time);
    void stopAttesaDati(void);

    // Timer per la disabilitazione a tempo del pulsante di ingresso
    int changePageTimer;
    int  getArm(void);
    int getCurrentProjection(int angolo);
    void setKvRange(unsigned char opt);     // Impostazione del range di kV utilizzabile
    int  getMinKv(void) ;
    int  getMaxKv(void) ;


    int currentAngolo;

    pannelloComandi* commandPanel;
    pannelloCompressione* compressionPanel;
    pannelloSpessore* thicknessPanel;
    pannelloKv* kvPanel;
    pannelloMas* masPanel;
    pannelloOpzioni* optionPanel;
    pannelloProiezioni* projPanel;

    // Vettore per agevolare la gestione delle proiezioni
    int currentProjection;
    unsigned char currentBreast;

    int projectionsAngoli[17];

    void changePanel(int panel);
    void manageCallbacks(int opt);
    void saveOptions(void);

    void  activateProjection(void) ;
    void  setCurrentFuoco(void);
    void  setPad(void);
    void  setSbloccoCompressore(void);
    void  verifyReady(void);
    void  updateAlarmField(void);

    void  startXraySequence(void);
    void  xrayManualSequence(void);
    void  xraySemiAutoSequence(void);
    void  xrayFullAutoSequence(void);
    void  xrayReleasePushButton(void);
    void  xrayErrorInCommand(unsigned char code);


    // Dati di esposizione
    int   XselectedkV;
    int   XselectedDmAs;
    int   XselectedIa;
    int   XselectedFiltro;
    int   XselectedFuoco;
    int   XspessoreSeno;
    float Xdose;
    float XmAs;

};



#endif
