#include "tcpipserver.h"


/*****************************************************************************/
/*****************************************************************************/
//                          TcpIpServer CLASS                                     //
/*****************************************************************************/
/*****************************************************************************/

TcpIpServer::TcpIpServer():QTcpServer()
{
    socket=0; // Inizializza il puntatore al socket;
    connection_status=FALSE;

}
TcpIpServer::~TcpIpServer()
{
    if(socket)
    {
        socket->close();
        socket->deleteLater();
        socket=0;
        connection_status=FALSE;
    }
}


// FUNZIONE DI ATTIVAZIONE DELLA COMUNICAZIONE
int TcpIpServer::Start(int porta)
{
    bool valid_interface;
    bool valid_address;
    QNetworkInterface network;


    valid_interface = false;
    QList<QNetworkInterface> interfacesList = QNetworkInterface::allInterfaces();
    for(int i =0; i<interfacesList.size(); i++)
    {
        if (!interfacesList.at(i).name().contains("eth")) continue;
        if (!interfacesList.at(i).IsUp) continue;
        network = interfacesList.at(i);
        valid_interface = true;
    }

    if (valid_interface == false )
    {        
        return -1;

    }

    valid_address=false;
    QList<QHostAddress> ipAddressList = network.allAddresses();
    for(int i =0; i<ipAddressList.size(); i++)
    {
        // Scandisce la lista per trovare gli indirizzi ip validi
        if((ipAddressList.at(i) != QHostAddress::LocalHost) && // non è un local host
                ipAddressList.at(i).toIPv4Address())            // E' un indirizzo IPV4
        {
            localip= ipAddressList.at(i);
            localport = porta;
            valid_address=true;           
        }
    }
    if (valid_address==false)
    {        
        return -1;
    }

    // Attiva l'ascolto
    if (!this->listen(localip,localport))
    {        
        return -1;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// FUNZIONE ESEGUITA ALLA RICHIESTA DI CONNESSIONE
void TcpIpServer::incomingConnection(int socketDescriptor)
{
    // Se è una nuova richiesta allora abortisce quella vecchia e ne effettua una nuova
    if(socket)
    {

        disconnected();
        //return;
    }



    // Se c'era una connessione in corso la chiude
    if(socket)
    {
        socket->close();
        delete socket;
    }


    // Creazione del socket associato al client
    socket = new QTcpSocket(this);

    // Deve esere creato il socket relativo al chiamante
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        // ERRORE DA FARE

        delete socket;
        return;
    }
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);


    // Connette i segnali relativi al socket
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    connect(socket,SIGNAL(disconnected()),this, SLOT(disconnected()),Qt::UniqueConnection);
    connect(socket,SIGNAL(readyRead()), this, SLOT(socketRxData()),Qt::UniqueConnection);



    connection_status = true;

    // Segnala l'evento connessione ..
    emit serverConnection(true);

    return;
 }


///////////////////////////////////////////////////////////////////////////////////////
// DISCONNESSIONE DA SOCKET
void TcpIpServer::disconnected()
{
    connection_status = false;
    emit serverConnection(false);
    socket->close();
    socket->deleteLater();
    socket = 0; // Consente di poter accettare un nuovo client
}


void TcpIpServer::socketError(QAbstractSocket::SocketError error)
{

    if(connection_status) disconnected();

    return;
}


//////////////////////////////////////////////////////////////////////////////////////
//              RICEZIONE DATI DA SOCKET
void TcpIpServer::socketRxData()
{

    if(socket->bytesAvailable()==0)
    {

        return;
    }

    QByteArray data = socket->readAll();
    if(data.size()==0)
    {

        return;
    }


    // Legge tutto il buffer ricevuto e lo passa a chi deve gestirlo
    emit rxData(data);//(socket->bytesAvailable()));
}

//////////////////////////////////////////////////////////////////////////////////////
//              TRASMISSIONE  DATI PER SOCKET
void TcpIpServer::txData(QByteArray data)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!connection_status) return;
    if(!socket) return;

    socket->write(data);
    socket->waitForBytesWritten(5000);
}
void TcpIpServer::txData1000(QByteArray data)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!connection_status) return;
    if(!socket) return;


    socket->write(data);
    socket->waitForBytesWritten(5000);

}

void TcpIpServer::txData(QByteArray data, long timeout)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!connection_status) return;
    if(!socket) return;

    socket->write(data);
    socket->waitForBytesWritten(timeout);
}



// Determina l'indirizzo della macchina
QHostAddress TcpIpServer::hostAddress()
{
    QNetworkInterface network;

    bool valid_interface;


    valid_interface = false;
    QList<QNetworkInterface> interfacesList = QNetworkInterface::allInterfaces();
    for(int i =0; i<interfacesList.size(); i++)
    {
        if (!interfacesList.at(i).name().contains("eth")) continue;
        if (!interfacesList.at(i).IsUp) continue;
        network = interfacesList.at(i);
        valid_interface = true;
    }

    if (valid_interface == false )
    {
        return QHostAddress("127.0.0.1");
    }

    QList<QHostAddress> ipAddressList = network.allAddresses();
    for(int i =0; i<ipAddressList.size(); i++)
    {
        // Scandisce la lista per trovare gli indirizzi ip validi
        if((ipAddressList.at(i) != QHostAddress::LocalHost) && // non è un local host
                ipAddressList.at(i).toIPv4Address())            // E' un indirizzo IPV4
        {
            return ipAddressList.at(i);
        }
    }

    return QHostAddress("127.0.0.1");

}

