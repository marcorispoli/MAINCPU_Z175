// SYSTEM_SETUP  SYSTEM_SETUP
// system  system
// systemUI  systemUI

#ifndef SYSTEM_SETUP_H
#define SYSTEM_SETUP_H

#include <QWidget>
#include <QGraphicsScene>

// Include aggiuntivi __________

//______________________________

namespace Ui {
class systemUI;
}

class systemPage : public QWidget
{
    Q_OBJECT

public:
    explicit systemPage(int rotview, QWidget *parent = 0);
    ~systemPage();

    // Timer per gestire il pulsante
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

public slots:
    // Funzione associata a GWindw
    void changePage(int pg, int opt);
    void onExitButton(void);
    void valueChanged(int index,int opt);



    void onLeftStarter(void);
    void onLeftDetector(void);
    void onLeftRotation(void);
    void onRightStarter(void);
    void onRightDetector(void);
    void onRightRotation(void);
    void onRightTilt(void);
    void onLeftTilt(void);
    void onRightFilter(void);
    void onLeftFilter(void);

private:
    Ui::systemUI *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsProxyWidget *proxy;
    QWidget *parent;
    int rotview;

    // Timer per la disabilitazione a tempo del pulsante di ingresso
    int changePageTimer;
    int timerDisable;   // Disable a tempo pulsante next   
    void initPage(void);
    void exitPage(void);



    QString getGantryStr(unsigned char val);
    QString getStarterStr(unsigned char val);
    QString getRotationStr(unsigned char val);
    QString getTiltStr(unsigned char val);
    QString getDetectorStr(unsigned char val);
    QString getFilterStr(unsigned char val);
};

#endif
