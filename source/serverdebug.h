#ifndef SERVERDEBUG_H
#define SERVERDEBUG_H

#include "application.h"

#define IRS_MAJ  2
#define IRS_MIN  0


/*___________________________________________________________________________________
                DESCRIZIONE REVISIONI INTERFACCIA IRS


___________________________________________________________________________________*/


class serverDebug : public QObject
{
    Q_OBJECT
public:
    explicit serverDebug(void);
    void activateConnections(void);

signals:

public slots:
    void fineRaggiManualXray(QByteArray data);

    QHostAddress setIpAddress( int val);
    bool isIp(int val);

    void trxNotifySlot(unsigned char id, unsigned char cmd, QByteArray data);
    void configurationSlot(void);
    void serviceRxHandler(QByteArray data); // Handler ricezione frame IRS

    void handleMasterShell(QByteArray data);
    void handleSlaveShell(QByteArray data);
    void slotRemoteShell(QByteArray data);

    void handleShell(QByteArray data);
    void resetGonioNotify(unsigned char id, unsigned char mcccode, QByteArray buffer);
    void getGonioNotify(unsigned char id, unsigned char mcccode, QByteArray buffer);
    void handleGetTrolleyNotify(unsigned char id,unsigned char cmd, QByteArray data);
    void handleSetCompressorRelease(QByteArray data);


    void handleGeneratore(QByteArray data);    

    void handleConfig(QByteArray data);
    void handleSystem(QByteArray data);


        void handleSetLanguage(QByteArray data); // Imposta la lingua
        void handleGetRevisions(void);              // Comando di richiesta revisioni correnti
        void handleGetInputs(void); // Stampa lo stato degli inputs di sistema
        void handleGetOutputs(void);// Stampa lo stato degli Outputs di sistema
        void handleSetOutputs(QByteArray data);
        void handleSetDatabaseS(QByteArray data);
        void handleSetDatabaseI(QByteArray data);
        void handleSetDatabaseU(QByteArray data);
        void handleGetAlarmInfo(QByteArray data);

    void handleDebug(QByteArray data);
        void handleSetCompressorNotify(QByteArray data);
        void handleSetActuatorEnableNotify(QByteArray data);

    // Drivers Group
    void handleDrivers(QByteArray data);
        void handleDriverFreeze(bool stat);         // Attivazione Disattivazione drivers
        void handleDriverRead8(QByteArray data);
        void handleDriverRead16(QByteArray data);
        void handleDriverWrite8(QByteArray data);
        void handleDriverWrite16(QByteArray data);
        void handleDriverCommand(QByteArray data);
        void handleDriverSpecial(QByteArray data);
        void handleDriverFreezeNotify(unsigned char,unsigned char,QByteArray);
        void handleDriverSendNotify(unsigned char id,unsigned char cmd, QByteArray data);
        void handleDriversCommTest(QByteArray data);
        void handleLoaderCommTest(QByteArray data);
        void handleIntercommTest(QByteArray data);

    void notificheConnectionHandler(bool stat);    // Handler cambio stato connessione

    // Ricezione dati richiesti



    void handleReadConfigNotify(_picConfigStr data);
    void serviceNotifyFineRaggi(QByteArray data);

    QByteArray getNextFieldAfterTag(QByteArray buf, QString tag);
    QList<QByteArray> getNextFieldsAfterTag(QByteArray data, QString tag);

    void handleCollimatore(QByteArray data);
        void handleGetCalib(QByteArray data);
        void handleSetCalibFiltro(QByteArray data);
        void handleSetCalibMirror(QByteArray data);
        void handleSetCalib2D(QByteArray data);
        void handleSetCalibTomo(QByteArray data);
        void handleSetCalibTomoFiltro(QByteArray data);
        void handleSetCalibCustom(QByteArray data);

    void setManualLameVal(QString lama, int val);
    void handleRotazioni(QByteArray data);
    void handleMoveTrx(QString tag, QByteArray data);
    void handleMoveArm(QString tag, QByteArray data);

    void handleStandardBiopsy(QByteArray data);
    void handleExtendedBiopsy(QByteArray data);
    void handleBiopsySimulator(QByteArray data);



    void handleSetAlarm(QByteArray data, bool selfreset);

    // Funzioni per la gestione del compressore
    void handleCompressore(QByteArray data);
        void handleGetPadList(void);
        void handleSetThick(QByteArray data);
        void handleSetKF(QByteArray data);
        void handleSetPeso(QByteArray data);
        void handleGetCalibPad(QByteArray data);
        void handleSetCalibPad(QByteArray data);
        void handleGetCalibNacchera(void);
        void handleSetCalibNacchera(QByteArray data);
        void handleCalibThresholds(QByteArray data);



    // Funzioni per la gestione del potter
    void handlePotter(QByteArray data);
        void handleClearPCB244Errors(void);
        void handleResetPCB244(void);
        void handleSetGrid3D(QByteArray data);
        void handleSetGrid2D(QByteArray data);
        void handleSetGridFreq(QByteArray data);
        void handleSetGridAmp(QByteArray data);

    // Funzioni per device su canopen
    void handleCanOpen(QByteArray data);
        void handleCanOpen_test(QByteArray data);


private:
    bool mccService(int id, _MccServiceNotify_Code cmd, QByteArray data);
    bool mccService(_MccServiceNotify_Code cmd);

    void handleList(void);                      // Visualizza la lista di comandi disponibili

    void handleConsole(QByteArray frame);       // Inietta una stringa di protocollo Console nel canale di ricezione della console


    void handleRodaggioTubo(QByteArray data);              // Comando di attivazione rodaggio del tubo
    void handleSetPad(QByteArray data);
    void handleSetPage(QByteArray data);

    void handleLoader(QByteArray data);         // Gestione comandi Loader
        void handleGetCRC(QByteArray data);
        void handleSetCRC(QByteArray data);
        void handleSetRemoteCRC(QByteArray data);
        void handleReadConfig(QByteArray data);     // Lettura configurazione
        void handleLoaderUpload(QByteArray data);   // Attivazione procedura caricamentoi manuale firmware
        void handleLoaderTestConnection(QByteArray data);


    int getVal(QString val);

    private:
    TcpIpServer*     serviceTcp; // Connessione da monitor esterno
    QByteArray      lastValidFrame; // Ultimo comando eseguito. Utilizzato per il comando repeat..
    QByteArray      cmdGroup;       // Ultimo gruppo di comandi selezionato


    // Invio e ricezione di dati a 16bit dai drivers
    QString frameTarget;
    bool frameCompleted;
    unsigned char  frameD0;
    unsigned char  frameD1;
    unsigned char  frameD2;
    unsigned short frameData;
    bool frameWrite;
    bool frameFormat16;
    bool frameDH;
    bool isCommand;
    bool isSpecial;


    #define _8bit 0
    #define _16bit 1
    #define _UNSIGNED 0
    #define _SIGNED   1
    #define _CODEID_PCB249U1    0x16
    #define _CODEID_PCB249U2    0x15
    #define _CODEID_PCB190      0x13
    #define _CODEID_PCB244      0x14
    #define _CODEID_PCB269      0x11

    typedef struct {
        QString tag;
        QString comment;
        unsigned char  deviceId;

        unsigned short address;
        signed short   sval;
        unsigned short val;
        unsigned char data_type;
        unsigned char sign_type;

    } _deviceRegisterItem;

    QList<_deviceRegisterItem> deviceRegisterList;
    void handleDriversTagList(QByteArray data);
    void handleDriversReadReg(QByteArray data);

};

#endif // SERVERDEBUG_H
