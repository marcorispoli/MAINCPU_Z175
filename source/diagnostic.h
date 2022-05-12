#ifndef DIAGNOSTIC_INTERFACE_H
#define DIAGNOSTIC_INTERFACE_H

#include "application.h"

#define REVISION  "0.1"

/*___________________________________________________________________________________
                DESCRIZIONE REVISIONI INTERFACCIA DIAGNOSTICA REMOTA


___________________________________________________________________________________*/


class diagnosticInterface : public QObject
{
    Q_OBJECT
public:
    explicit diagnosticInterface(void);
    void activateConnections(void);

    // Comandi Gantry to Remote
    #define EVENT_GANTRY_REVISION     "GANTRY_REVISION"
    #define EVENT_GANTRY_ERRORS       "GANTRY_ERRORS"
    #define EVENT_GANTRY_EXPOSURE     "GANTRY_EXPOSURE"
    #define EVENT_GANTRY_STUDY        "GANTRY_STUDY"
    #define EVENT_GANTRY_STATUS       "GANTRY_STATUS"

    // Remote to Gantry
    #define CMD_GET_ERRORS          "GET_ERRORS"


signals:

public slots:
    QHostAddress setIpAddress( int val);
    bool isIp(int val);
    void commandConnectionHandler(bool stat);    // Handler cambio stato connessione
    void eventConnectionHandler(bool stat);    // Handler cambio stato connessione
    void exposureEventSlot(QByteArray buffer);

    void commandRxHandler(QByteArray data); // Handler ricezione comandi
    void alarmNotify(int codice, QString msg);
    void gantryStudy(bool stat);
    void gantryStatus(void);

    private:

    TcpIpServer*     commandTcp; // Ricezione comandi
    TcpIpServer*     eventTcp;   // Invio eventi
    bool eventConnectionStat;
    bool commandConnectionStat;

};

#endif // DIAGNOSTIC_INTERFACE_H
