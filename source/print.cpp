#include "appinclude.h"
#include "globvar.h"

// Comandi da inviare a Console
#define MASTER_PRINT_PORT 10020
#define SLAVE_PRINT_PORT  10021
#define _PRINT_IP "192.168.30.10"


void printDebug::activateConnections(void){
    QObject::connect(printTcp,SIGNAL(clientConnection(bool)),this,SLOT(printConnectionHandler(bool)),Qt::UniqueConnection);
    QObject::connect(this,SIGNAL(printTxHandler(QByteArray)), printTcp,SLOT(txData(QByteArray)),Qt::UniqueConnection);

    if(isMaster)
        printTcp->Start(QHostAddress(_PRINT_IP),MASTER_PRINT_PORT);
    else
        printTcp->Start(QHostAddress(_PRINT_IP),SLAVE_PRINT_PORT);

}

printDebug::printDebug(QObject *parent) :
    QObject(0)
{
    printConnected = FALSE;
    // Socket per segnali asincroni
    printTcp = new TcpIpClient();
    activateConnections();
}



void printDebug::printConnectionHandler(bool stat)
{
    printConnected = stat;

    if(stat==true) print(QString("HELLO"));
    return;
}


/*
 *  Questa funzione viene lanciata per notificare la AWS di un comando di movimento braccio in corso
 */
void printDebug::print(QString stringa)
{
    static int idx=0;
    if(!printConnected) return;

    stringa.prepend(QString("[%1]:>").arg(idx++));
    stringa.append("\n\r");
    emit printTxHandler(stringa.toAscii());

}



