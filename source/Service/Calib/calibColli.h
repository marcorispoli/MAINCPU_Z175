#ifndef CALIBCOLLI_H
#define CALIBCOLLI_H

#include <QWidget>
#include <QGraphicsScene>

// Include aggiuntivi __________
#include "lib/numericpad.h"
//______________________________

namespace Ui {
class calibColliUI;
}


class calibColli : public QWidget
{
    Q_OBJECT

public:
    explicit calibColli(int rotview, QWidget *parent = 0);
    ~calibColli();

    // Timer per gestire il pulsante
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

public slots:
    // Funzione associata a GWindw
    void changePage(int pg,  int opt);
    void onExitButton(void);
    void valueChanged(int index,int opt);

    void onScrollLeftButton(void);
    void onScrollRightButton(void);
    void onFrontEdit(void);
    void onBackEdit(void);
    void onLeftEdit(void);
    void onRightEdit(void);
    void onTrapEdit(void);


    void onStoreButton(void);
    void onUpdateButton(void);
    void calculatorSlot(bool state);    // Slot associato all'uscita del tool di calcolo

    void selectPad(unsigned char pad);

private:
    Ui::calibColliUI *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsProxyWidget *proxy;
    QWidget *parent;
    int rotview;

    // Timer per la disabilitazione a tempo del pulsante di ingresso
    int changePageTimer;
    int timerDisable;   // Disable a tempo pulsante next
    int timerInfoFrame;

    void initPage(void);
    void exitPage(void);


    numericPad* pCalculator;
    QString     dataField; // Stringa di riferimento per i calcoli

    int currentPad;
    int customL;
    int customR;
    int customT;
    int customB;
    int customF;
};

#endif
