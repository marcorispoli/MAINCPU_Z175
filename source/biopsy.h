#ifndef BIOPSY_H
#define BIOPSY_H

#include "application.h"


class biopsy : public QObject
{
    Q_OBJECT
public:
    explicit biopsy(QObject *parent = 0);
    void activateConnections(void);
    void calcoloMargini(void);

    // Movimenti su tre assi
    int moveXYZ(unsigned short X, unsigned short Y, unsigned short Z, int id); // Chiede il movimento sui tre assi
    int moveHome(int id); // Chiede il movimento verso home

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

public:
    bool connected;
    biopsyConf_Str config;          // Dati di configurazione
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
    #define _BIOPSY_MOVING_ERROR_MCC        1
    #define _BIOPSY_MOVING_ERROR_TIMEOUT    2
    #define _BIOPSY_MOVING_ERROR_TARGET     3
    #define _BIOPSY_MOVING_ERROR_BUSY       6

    // Posizione corrente della torretta
    unsigned short curX_dmm;    // (0.1mm) Posizione corrente X
    unsigned short curY_dmm;    // (0.1mm) Posizione corrente Y
    unsigned short curZ_dmm;    // (0.1mm) Posizione corrente Z
    unsigned short curSh_dmm;   // (0.1mm) Posizione corrente Sh

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

    // Pulsante di sblocco
    unsigned char laterality;

    // Dati perifierica collegata
    unsigned char checksum_h;
    unsigned char checksum_l;
    unsigned char revisione;
    unsigned char model;
};

#endif // BIOPSY_H
