#ifndef ANALOGCALIBPAGEOPEN_H
#define ANALOGCALIBPAGEOPEN_H

#include "../../application.h"

#include <QWidget>
#include <QGraphicsScene>
#include "lib/numericpad.h"

namespace Ui {
class analog_calibUI;
}


class AnalogCalibPageOpen : public QWidget
{
    Q_OBJECT

public:
    explicit AnalogCalibPageOpen(int rotview,QWidget *parent = 0);
    ~AnalogCalibPageOpen();

    // Timer per gestire il pulsante
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

    void setAecField(int campo);

    bool detector_calibration;
    bool profile_calibration;
    bool tube_calibration;
    bool manual_exposure;


    bool pc_data_valid;
    unsigned char rmmi_reference;
    unsigned char rmmi_toll;
    unsigned char pc_selected_filtro;           // Filtro impostato da pc
    unsigned char pc_selected_pmmi;             // mm di plexiglass richiesti per la calibrazione
    unsigned char pc_selected_fuoco;            // 0=fuoco grande; 1 = fuoco piccolo
    unsigned char pc_selected_profile_index;    // Indice del profilo da utilizzare per la calibrazione
    unsigned char pc_selected_field;            // Indice del campo da utilizzare

    bool colli_ok;
    bool focus_ok;
    bool filter_ok;
    bool field_ok;

    // Calibrazione Tubo
    int pc_selected_kV;
    int pc_selected_vdac;
    int pc_selected_Ia;
    int pc_selected_Idac;
    int pc_selected_mAs;

public slots:

    // Funzione associata a GWindw
    void changePage(int pg, int opt);
    void onExitButton(void);
    void onAlarmButt(void);
    void valueChanged(int index,int opt);

    // Detector calibration slots
    void detectorValueChanged(int index,int opt);
    void onNextCampiButton(void);
    void detectorGuiNotify(unsigned char id, unsigned char mcccode, QByteArray data);

    void profileValueChanged(int index,int opt);
    void profileGuiNotify(unsigned char id, unsigned char mcccode, QByteArray data);
    void setProfileData(void); // Dati per la calibrazione del profilo

    void tubeValueChanged(int index,int opt);
    void tubeGuiNotify(unsigned char id, unsigned char mcccode, QByteArray data);
    void raggiTubeDataSlot(QByteArray);
    void setTubeData(void); // Dati per la calibrazione del profilo
    void selectTube(QString tube_name); // Agganciata al comando del protocollo AWS SelectTube()

    // Manual Exposure Panel
    void manualExposureGuiNotify(unsigned char id, unsigned char mcccode, QByteArray data);
    void onManualNextCampiButton(void);
    void manualExposureValueChanged(int index,int opt);
    void onManualFilter(void);
    void onManualFuoco(void);
    void onManualKV(void);
    void onManualMAS(void);
    void updateExposureData(int opt);
    void manualCalculatorSlot(bool);


private:
    int timerReady;
    int timerDisable;


    Ui::analog_calibUI *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsProxyWidget *proxy;
    int rotview;
    QWidget *parent;

    void initPage(void);
    void exitPage(void);


    // Timer per la disabilitazione a tempo del pulsante di ingresso
    int changePageTimer;


    // Pannello inserimento numerico
    QString dataField;
    numericPad* pCalculator;

    int timer_attesa_dati;
    bool data_ready;
    void startAttesaDati(int time);
    void stopAttesaDati(void);

    void  verifyReady(void);
    void  xrayErrorInCommand(unsigned char code);

    // Detecto Calibration callbacks
    void initDetectorCalibration(void);
    void exitDetectorCalibration(void);
    bool getDetectorCalibrationReady(unsigned char dbopt);
    void startDetectorCalibrationXraySequence(void);

    // Profile Calibration callbacks
    void initProfileCalibration(void);
    void exitProfileCalibration(void);
    bool getProfileCalibrationReady(unsigned char dbopt);
    void startProfileCalibrationXraySequence(void);

    // Tube Calibration callbacks
    void initTubeCalibration(void);
    void exitTubeCalibration(void);
    bool getTubeCalibrationReady(unsigned char dbopt);
    void startTubeCalibrationXraySequence(void);

    // Manual Exposure panel
    void initManualExposure(void);
    void exitManualExposure(void);
    void startManualExposureXraySequence(void);
    bool getManualExposureReady(unsigned char opt);
};



#endif
