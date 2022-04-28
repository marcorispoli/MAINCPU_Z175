#include "application.h"
#include "appinclude.h"
#include "globvar.h"



QHostAddress diagnosticInterface::setIpAddress( int val){
    int size = TcpIpServer::hostAddress().toString().length();
    return QHostAddress(QString("%1.%2").arg(TcpIpServer::hostAddress().toString().left(size-2)).arg(val)) ;
}

bool diagnosticInterface::isIp(int val){
    if(TcpIpServer::hostAddress().toString().right(1).toInt()==val) return true;
    return false;
}

void diagnosticInterface::activateConnections(void) {

    QObject::connect(commandTcp,SIGNAL(rxData(QByteArray)),this,SLOT(commandRxHandler(QByteArray)),Qt::UniqueConnection);
    QObject::connect(commandTcp,SIGNAL(serverConnection(bool)),this,SLOT(commandConnectionHandler(bool)),Qt::UniqueConnection);
    QObject::connect(eventTcp,SIGNAL(serverConnection(bool)),this,SLOT(eventConnectionHandler(bool)),Qt::UniqueConnection);

    commandTcp->Start(_DIAGNOSTIC_CMD_PORT);
    eventTcp->Start(_DIAGNOSTIC_EVENT_PORT);

}

/*
 * if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
 **/
diagnosticInterface::diagnosticInterface(void) :
    QObject(0)
{
    // Creazione del socket di comunicazione esterna con la Console
    commandTcp = new TcpIpServer();
    eventTcp = new TcpIpServer();
}

void diagnosticInterface::commandConnectionHandler(bool stat)
{
    commandConnectionStat = stat;
    if(stat)
    {
        protoConsole cmd(0,UNICODE_FORMAT);
        cmd.addParam(REVISION);
        commandTcp->txData(cmd.cmdToQByteArray(EVENT_GANTRY_REVISION));

    }

    return;
}

void diagnosticInterface::eventConnectionHandler(bool stat)
{
    eventConnectionStat = stat;
    if(stat)
    {
        protoConsole cmd(0,UNICODE_FORMAT);
        cmd.addParam(REVISION);
        eventTcp->txData(cmd.cmdToQByteArray(EVENT_GANTRY_REVISION));
        connect(paginaAllarmi,SIGNAL(newAlarmSgn(int,QString)),this,SLOT(alarmNotify(int,QString)),Qt::UniqueConnection);

    }else{
        disconnect(paginaAllarmi,SIGNAL(newAlarmSgn(int,QString)),this,SLOT(alarmNotify(int,QString)));
    }
    return;

}

void diagnosticInterface::commandRxHandler(QByteArray rxbuffer)
{

    // Trasforma in Unicode il buffer ricevuto
#if (UNICODE_FORMAT == 0)
    protoConsole protocollo(&rxbuffer);
#else
    QTextCodec *codec = QTextCodec::codecForName(UNICODE_TYPE);
    QString frame = codec->toUnicode(rxbuffer);
    protoConsole protocollo(&frame);
#endif


    if(protocollo.isValid==FALSE) return;

    protoConsole answ(protocollo.id,UNICODE_FORMAT);
    QString comando = protocollo.comando;

    if(comando == CMD_GET_ERRORS){

        int n = ApplicationDatabase.getDataU(_DB_NALLARMI_ATTIVI);
        answ.addParam(QString("%1").arg(n));

        if(n){
            for(int classe = _DB_NALLARMI_ATTIVI+1; classe <= _DB_ALLARMI_ALR_SOFT; classe++){
                int error = ApplicationDatabase.getDataU(classe);
                if(error) answ.addParam(paginaAllarmi->getErrorCode(classe, error));
            }
        }
        commandTcp->txData(answ.answToQByteArray("OK"));



    }else{
       commandTcp->txData(answ.answToQByteArray("NA"));
    }
}

void diagnosticInterface::alarmNotify(int codice, QString msg)
{
    protoConsole cmd(0,UNICODE_FORMAT);
    cmd.addParam(QString("%1").arg(codice));
    eventTcp->txData(cmd.answToQByteArray(EVENT_GANTRY_ERRORS));

    return;
}
