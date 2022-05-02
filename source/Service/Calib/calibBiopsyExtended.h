#ifndef CALIB_BIOPSY_EXTENDED_CLASS_H
#define CALIB_BIOPSY_EXTENDED_CLASS_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class CalibBiopsyExtended;
}

class CalibBiopsyExtendedClass: public QWidget
{
    Q_OBJECT

public:
    explicit CalibBiopsyExtendedClass(int rotview,QWidget *parent = 0);
    ~CalibBiopsyExtendedClass();

    // Timer per gestire il pulsante
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

    typedef enum {
        _BUTT_SELECT_CALIB_CURSOR = 0,
        _BUTT_SELECT_CALIB_X,
        _BUTT_SELECT_CALIB_Y,
        _BUTT_SELECT_CALIB_Z,

        _BUTT_CURSOR_START,
        _BUTT_CURSOR_CANCEL,
        _BUTT_ERR_CANCEL,
        _BUTT_CURSOR_STORE,
        _BUTT_CURSOR_NEXT,

        _BUTT_X_START,
        _BUTT_X_CANCEL,
        _BUTT_X_STORE,
        _BUTT_X_NEXT,

        _BUTT_Y_START,
        _BUTT_Y_CANCEL,
        _BUTT_Y_STORE,
        _BUTT_Y_NEXT,

        _BUTT_Z_START,
        _BUTT_Z_CANCEL,
        _BUTT_Z_STORE,
        _BUTT_Z_NEXT,

    }_ButtonEnumeration;
    typedef enum {
        _BIOPCAL_FRAME_MENU = 0,
        _BIOPCAL_FRAME_CURSOR,
        _BIOPCAL_FRAME_X,
        _BIOPCAL_FRAME_Y,
        _BIOPCAL_FRAME_Z,
        _BIOPCAL_CURSOR_FRAME_ERROR,
    }_WorkingFrame;

public slots:
    // Funzione associata a GWindw
    void changePage(int pg, int opt);
    void onExitButton(void);
    void valueChanged(int index,int opt);
    void buttonHandler(_ButtonEnumeration button_code);
    void selectionWorkingFrame(_WorkingFrame working);

    void onCursorMenuButton(void);
    void onXMenuButton(void);
    void onYMenuButton(void);
    void onZMenuButton(void);

    void onCursorCancelButton(void);
    void onCursorNextButton(void);
    void onCursorStartButton(void);
    void onCursorStoreButton(void);

    void onXCancelButton(void);
    void onXNextButton(void);
    void onXStartButton(void);
    void onXStoreButton(void);

    void onYCancelButton(void);
    void onYNextButton(void);
    void onYStartButton(void);
    void onYStoreButton(void);

    void onZCancelButton(void);
    void onZNextButton(void);
    void onZStartButton(void);
    void onZStoreButton(void);

private:
    Ui::CalibBiopsyExtended *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsProxyWidget *proxy;

    void initPage(void);
    void exitPage(void);

    // Timer per la disabilitazione a tempo del pulsante di ingresso
    int changePageTimer;
    int seqTimer;

    uchar   workingFrame;

    // Cursor calibration variables
    ushort     rawSh;
    ushort     SH_M15;
    ushort     SH_P15;
    ushort     SH_0;
    bool       viewShRaw;

    void CursorSequenceHandler(uchar seq);
    void XSequenceHandler(uchar seq);
    void YSequenceHandler(uchar seq);
    void ZSequenceHandler(uchar seq);

    bool cursorStore(void);
    int  cursor_errcode;
};

#endif // CALIB_BIOPSY_EXTENDED_CLASS_H
