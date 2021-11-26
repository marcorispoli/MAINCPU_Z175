#ifndef BIOPSY_EXTENDED_H
#define BIOPSY_EXTENDED_H

#include "../application.h"

namespace Ui {
class biopsyUI;
}


class biopsyExtendedDevice : public QWidget
{
    Q_OBJECT
public:

    explicit biopsyExtendedDevice(int rotview, QWidget *parent = 0);
    ~biopsyExtendedDevice();

    // Timer per gestire il pulsante
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

    int requestBiopsyHome(int id, unsigned char lat);
    int requestBiopsyMoveXYZ(unsigned short X, unsigned short Y,unsigned short Z,int id);
    void calibrateXbase(unsigned short val);
    void activateConnections(void);
    int setStepVal(unsigned char step);
    unsigned char getLatX(void);
    unsigned char getAdapterId(void);
    void  setBuzzer(void);


    #define _DEF_TIMEOUT_USER_FEEDBACK  100 // 100ms unit

    #define _DEF_EXT_XHOME_LEFT      2580
    #define _DEF_EXT_XHOME_CENTER    1290
    #define _DEF_EXT_XHOME_RIGHT     0

    #define _DEF_EXT_YHOME_LEFT      0
    #define _DEF_EXT_YHOME_CENTER    0
    #define _DEF_EXT_YHOME_RIGHT     0

    #define _DEF_EXT_ZHOME_LEFT      143
    #define _DEF_EXT_ZHOME_CENTER    143
    #define _DEF_EXT_ZHOME_RIGHT     143


    /*
    int moveDecZ(int id);
    int moveIncZ(int id);
    int moveDecX(int id);
    int moveIncX(int id);
    int moveDecY(int id);
    int moveIncY(int id);
    */

signals:
    
public slots:
    void mccStatNotify(unsigned char,unsigned char,QByteArray); // Notifica dal driver di gestione sul blocco STAT

    // Funzione associata alle windows
    void changePage(int pg,  int opt);
    void valueChanged(int index,int opt);

    void onConfirmButton(void);

public:
    Ui::biopsyUI *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsProxyWidget *proxy;
    QWidget *parent;
    int rotview;
    void initPage(void);
    void exitPage(void);

    // Comandi in corso
    int movingCommand;  // Codice comando in esecuzione

    #define _BIOPSY_MOVING_NO_COMMAND       0
    #define _BIOPSY_MOVING_COMPLETED        1
    #define _BIOPSY_MOVING_XYZ              2
    #define _BIOPSY_MOVING_HOME             3
    #define _BIOPSY_MOVING_DECZ             4
    #define _BIOPSY_MOVING_INCZ             5
    #define _BIOPSY_MOVING_DECX             6
    #define _BIOPSY_MOVING_INCX             7
    #define _BIOPSY_MOVING_DECY             8
    #define _BIOPSY_MOVING_INCY             9

    int  movingError;
    #define _BIOPSY_MOVING_NO_ERROR         0    
    #define _BIOPSY_MOVING_ERROR_TIMEOUT    1
    #define _BIOPSY_MOVING_ERROR_TARGET     2
    #define _BIOPSY_MOVING_ERROR_BUSY       3
    #define _BIOPSY_MOVING_UNDEFINED_ERROR  4 // Il codice sequenza non corrisponde. E' un problema software


    // Posizione corrente della torretta
    unsigned short curX_dmm;    // (0.1mm) Posizione corrente X
    unsigned short curY_dmm;    // (0.1mm) Posizione corrente Y
    unsigned short curZ_dmm;    // (0.1mm) Posizione corrente Z
    unsigned short curSh_dmm;   // (0.1mm) Posizione corrente Sh
    unsigned char  curLatX;     // Posizione dislocazione asse X




    #define BIOPSY_ACTIVATION_SEQUENCE_DB   _DB_SERVICE1_INT
    #define BIOPSY_USER_CONFIRMATION_DB     _DB_SERVICE2_INT
    #define BIOPSY_ACTIVATION_TITLE_DB      _DB_SERVICE1_STR


    // Sequenza guidata ________________________________________
    #define _REQ_SEQ_NONE           0
    #define _REQ_SEQ_HOME           1
    #define _REQ_SEQ_XYZ            2

    #define _REQ_SUBSEQ_HOME_INIT           0    
    #define _REQ_SUBSEQ_HOME_EXE_Z          4
    #define _REQ_SUBSEQ_HOME_WAIT_Z         5
    #define _REQ_SUBSEQ_HOME_EXE_Y          6
    #define _REQ_SUBSEQ_HOME_WAIT_Y         7

    #define _REQ_SUBSEQ_HOME_TEST_SCROLL_X          8
    #define _REQ_SUBSEQ_HOME_EXE_SCROLL_X_LEFT      9
    #define _REQ_SUBSEQ_HOME_EXE_SCROLL_X_CENTER    10
    #define _REQ_SUBSEQ_HOME_EXE_SCROLL_X_RIGHT     11

    #define _REQ_SUBSEQ_HOME_EXE_TEST_SCROLL_Y      12
    #define _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_LEFT      13
    #define _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_CENTER    14
    #define _REQ_SUBSEQ_HOME_EXE_SCROLL_Y_RIGHT     15

    #define _REQ_SUBSEQ_HOME_EXE_X                  16
    #define _REQ_SUBSEQ_HOME_WAIT_X_TO_LEFT         17
    #define _REQ_SUBSEQ_HOME_WAIT_X_TO_CENTER       18
    #define _REQ_SUBSEQ_HOME_WAIT_X_TO_RIGHT        19
    #define _REQ_SUBSEQ_HOME_COMPLETED              20


    #define _REQ_SUBSEQ_XYZ_INIT                    0
    #define _REQ_SUBSEQ_XYZ_EXE_X                   1
    #define _REQ_SUBSEQ_XYZ_WAIT_X                  2
    #define _REQ_SUBSEQ_XYZ_EXE_Y                   3
    #define _REQ_SUBSEQ_XYZ_WAIT_Y                  4
    #define _REQ_SUBSEQ_XYZ_EXE_Z                   5
    #define _REQ_SUBSEQ_XYZ_WAIT_Z                  6
    #define _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_LEFT       7
    #define _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_RIGHT      8
    #define _REQ_SUBSEQ_XYZ_EXE_SCROLL_Y_CENTER     9


    #define _REQ_SUBSEQ_XYZ_COMPLETED               21



private:
    int req_sequence;
    int sub_sequence;
    int event_req_sequence;

    unsigned char req_home_lat;
    unsigned short req_X;
    unsigned short req_Y;
    unsigned short req_Z;
    bool user_confirmation;
    unsigned char buzzer_delay;
    bool bypass_y_scroll;
    int user_timeout;

    unsigned short XHOME_LEFT;
    unsigned short XHOME_CENTER;
    unsigned short XHOME_RIGHT;
    unsigned short YHOME_LEFT;
    unsigned short YHOME_CENTER;
    unsigned short YHOME_RIGHT;
    unsigned short ZHOME_LEFT;
    unsigned short ZHOME_CENTER;
    unsigned short ZHOME_RIGHT;

    // Impostazione corrente step di incremento
    unsigned char  stepVal;      // STEPVAL corrente

    // Accessorio riconosciuto
    unsigned char adapterId;       // adapterId riconosciuto

    // Pulsante di sblocco
    bool unlock_button;


    void manageHomeSequence(void);
    void manageXYZSequence(void);
    void manageRequestErrors(int error);
    void nextStepSequence(int tmo);
    void manageChangeHomeSeq(unsigned char sub_seq);
    void manageChangeMoveXYZSeq(unsigned char sub_seq);
    void hideFrames(void);
    int calibrateSh(void);

    // Movimenti su tre assi
    int moveXYZ(unsigned short X, unsigned short Y, unsigned short Z); // Chiede il movimento sui tre assi

    bool testUpsidePosition(unsigned short X);
    bool isTarget(unsigned short X, unsigned short Y, unsigned short Z);

};

#endif // BIOPSY_EXTENDED_H
