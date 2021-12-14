#ifndef PRINT_H
#define PRINT_H

#include "application.h"


class infoClass : public QObject
{
    Q_OBJECT
public:
    explicit infoClass(QObject *parent = 0);

signals:
    void printTxHandler(QString);   // Info di tipo Print
    void debugTxHandler(QString);   // Info di tipo Debug
    void logTxHandler(QString);     // Info di tipo Log
    void qtTxHandler(QString);      // Info dalla libreria Qt


public:

#ifdef __PRINT
    #define PRINT(x)    pInfo->emit_printTxHandler(x) // Messaggi attivi solo se compilati
#else
    #define PRINT(x)
#endif

#define DEBUG(x)    pInfo->emit_debugTxHandler(x) // Messaggi visualizzati esclusivamente da IRS
#define LOG(x)      pInfo->emit_logTxHandler(x)   // Messaggi inviati a console
#define QTMSG(x)    pInfo->emit_qtTxHandler(x)   // Messaggi inviati a console

public slots:
    void emit_printTxHandler(QString msg) {emit printTxHandler(msg); }
    void emit_debugTxHandler(QString msg) {emit debugTxHandler(msg); }
    void emit_logTxHandler(QString msg)   {emit logTxHandler(msg); }
    void emit_qtTxHandler(QString msg)    {emit qtTxHandler(msg); }

private:

};


#endif // PRINT_H
