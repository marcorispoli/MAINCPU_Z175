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

    int requestBiopsyHome(int id, unsigned char lat, int rot_holder);
    int requestBiopsyMoveXYZ(unsigned short X, unsigned short Y,unsigned short Z,int id);
    void calibrateXbase(unsigned short val);
    void activateConnections(void);
    int setStepVal(unsigned char step);
    unsigned char getLatX(void);
    unsigned char getAdapterId(void);
    void  setBuzzer(void);


    #define _DEF_TIMEOUT_USER_FEEDBACK  300 // 100ms unit

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
    #define _BIOPSY_MOVING_ERROR_TARGET     1
    #define _BIOPSY_MOVING_ERROR_TIMEOUT    2
    #define _BIOPSY_MOVING_ERROR_BUSY       3
    #define _BIOPSY_MOVING_UNDEFINED_ERROR  4 // Il codice sequenza non corrisponde. E' un problema software


    // Posizione corrente della torretta
    unsigned short curX_dmm;    // (0.1mm) Posizione corrente X
    unsigned short curY_dmm;    // (0.1mm) Posizione corrente Y
    unsigned short curZ_dmm;    // (0.1mm) Posizione corrente Z
    unsigned short curSh_dmm;   // (0.1mm) Posizione corrente Sh
    unsigned char  curLatX;     // Posizione dislocazione asse X





    #define BIOPSY_USER_CONFIRMATION_DB     _DB_SERVICE2_INT
    #define BIOPSY_ACTIVATION_TITLE_DB      _DB_SERVICE1_STR
    #define BIOPSY_ACTIVATION_SEQUENCE_DB   _DB_SERVICE2_STR


    // Sequenza guidata ________________________________________
    #define _REQ_SEQ_NONE           0
    #define _REQ_SEQ_HOME           1
    #define _REQ_SEQ_XYZ            2

    enum{
        _REQ_SUBSEQ_HOME_INIT  = 0,
        _REQ_SUBSEQ_HOME_X_SCROLL, // Common X-SCROLL status
        _REQ_SUBSEQ_HOME_Y_SCROLL, // Common Y-UP/DOWN-SCROLL status
        _REQ_SUBSEQ_HOME_X_MOVE,   // Common X move
        _REQ_SUBSEQ_HOME_Y_MOVE,   // Common Y move
        _REQ_SUBSEQ_HOME_Z_MOVE,   // Common Z move

        _REQ_SUBSEQ_HOME_EXE_Z,    // Moves to Home Z
        _REQ_SUBSEQ_HOME_EXE_Y,    // Moves to Home Y
        _REQ_SUBSEQ_HOME_TEST_Y_UP, // Test if the Y shall be reversed up
        _REQ_SUBSEQ_HOME_TEST_SCROLL_X, // Test if the X shall be scrolled
        _REQ_SUBSEQ_HOME_EXE_X,         // Activate the X position in home
        _REQ_SUBSEQ_HOME_TEST_Y_DOWN, // Test if the Y shall be reversed down
        _REQ_SUBSEQ_HOME_COMPLETED

    };





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


    // Params value
    #define _PARAM_CENTER           0
    #define _PARAM_LEFT             1
    #define _PARAM_RIGHT            2

    #define _PARAM_UP       5
    #define _PARAM_DOWN     6
    #define _PARAM_OUT      7
    #define _PARAM_IN       8



private:
    // Last X-Scroll sensor detected
    unsigned char last_xscroll_detected;

    // Command execution variables
    int req_sequence;
    int event_req_sequence;
    int sub_sequence;

    int next_seq;
    int seq_param1;
    int seq_param2;
    unsigned short move_X;
    unsigned short move_Y;
    unsigned short move_Z;


    // Requested positionning target
    int           req_rot_holder;
    unsigned char req_home_lat;    
    unsigned short req_X;
    unsigned short req_Y;
    unsigned short req_Z;

    bool            user_confirmation; // Confirmation button status
    unsigned char   buzzer_delay;      // Delay for buzzer pulse
    int             user_timeout;      // Timeout waiting for confirmation

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

    bool isHome;

    void manageHomeSequence(void);
    void manageXYZSequence(void);
    void manageRequestErrors(int error);
    void nextStepSequence(int tmo);
    void manageChangeHomeSeq(int sub_seq,int param1, int param2);
    void manageChangeMoveXYZSeq(int sub_seq,int param1, int param2);
    void hideFrames(void);
    int calibrateSh(void);

    // Movimenti su tre assi
    int moveXYZ(unsigned short X, unsigned short Y, unsigned short Z); // Chiede il movimento sui tre assi

    bool testUpsidePosition(unsigned short X);
    bool isTarget(unsigned short X, unsigned short Y, unsigned short Z);

    // Test if the Y block is turned Up based on the current X posiiton scenaro
    bool testYisUp(void){
        if( (curLatX == _BP_EXT_ASSEX_POSITION_CENTER) && (curX_dmm < 2263)  && (curX_dmm > 317)) return true;
        else if((curLatX == _BP_EXT_ASSEX_POSITION_LEFT) && (curX_dmm < 980)) return true;
        else if((curLatX == _BP_EXT_ASSEX_POSITION_RIGHT) && (curX_dmm > 1600)) return true;

        return false;
    }

    void handleXScroll(void);
    void handleYScroll(void);
    void handleMove(void);

};

#endif // BIOPSY_EXTENDED_H
