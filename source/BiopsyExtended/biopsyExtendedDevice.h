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
    void activateConnections(void);


    void calcoloMargini(void);

    // Movimenti su tre assi
    int moveXYZ(unsigned short X, unsigned short Y, unsigned short Z); // Chiede il movimento sui tre assi

    bool testUpsidePosition(unsigned short X);
    bool isHome(unsigned char lat);

    // Movimenti per step
    int setStepVal(unsigned char step);
    int moveDecZ(int id);
    int moveIncZ(int id);
    int moveDecX(int id);
    int moveIncX(int id);
    int moveDecY(int id);
    int moveIncY(int id);


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


    bool connected;
    biopsyConfExtended_Str config;          // Dati di configurazione
    bool openCfg(void);             // Funzione per l'apertura del file di configurazione
    bool storeConfig(void);         // Salva la configurazione
    void defaultConfigData(void);

    bool updateConfig(void);        // Aggioirna M4 con i valori correnti

    // Comandi in corso
    int movingCommand;  // Codice comando in esecuzione
    int activationId;   // Codice ID per comando AWS
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

    // Limiti di movimento
    unsigned char  paddle_margine;  // (mm) distanza staffe paddle - base torretta (mm) calcolata da Gantry
    unsigned char  needle_home;     // (mm) distanza punta ago - home torretta
    unsigned char  needle_margine;  // Distanza punta ago - fibra di carbonio

    unsigned char  max_z_paddle;    // (mm) Massima Z ricalcolata sulla base della posizione del paddle
    unsigned char  abs_max_z;       // (mm) Massima Z assoluta rspetto sia al paddle che all'ago (se presente)

    // Target di movimento impostato
    unsigned short targetX_dmm;    // (0.1mm) Valore Target di movimento X
    unsigned short targetY_dmm;    // (0.1mm) Valore Target di movimento Y
    unsigned short targetZ_dmm;    // (0.1mm) Valore Target di movimento Z

    // Impostazione corrente step di incremento
    unsigned char  stepVal;      // STEPVAL corrente

    // Accessorio riconosciuto
    unsigned char adapterId;       // adapterId riconosciuto

    // Pulsante di sblocco
    bool unlock_button;

    // Dati perifierica collegata
    unsigned char checksum_h;
    unsigned char checksum_l;
    unsigned char revisione;


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

    int req_sequence;
    int sub_sequence;
    int event_req_sequence;

    unsigned char req_home_lat;
    unsigned short req_X;
    unsigned short req_Y;
    unsigned short req_Z;
    bool user_confirmation;
    bool bypass_y_scroll;

private:
    void manageHomeSequence(void);
    void manageXYZSequence(void);
    void manageRequestErrors(int error);
    void nextStepSequence(int tmo);
    void manageChangeHomeSeq(unsigned char sub_seq);
    void manageChangeMoveXYZSeq(unsigned char sub_seq);
    void hideFrames(void);
    int calibrateSh(void);

};

#endif // BIOPSY_EXTENDED_H
