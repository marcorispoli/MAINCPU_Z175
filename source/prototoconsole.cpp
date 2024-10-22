#include "appinclude.h"
#include "globvar.h"


void protoToConsole::activateConnections(void){

    QObject::connect(notificheTcp,SIGNAL(clientConnection(bool)),this,SLOT(notificheConnectionHandler(bool)),Qt::UniqueConnection);
    QObject::connect(this,SIGNAL(notificheTxHandler(QByteArray)), notificheTcp,SLOT(txData(QByteArray)),Qt::UniqueConnection);
    notificheTcp->Start(QHostAddress(_CONSOLE_IP),_CONSOLE_OUT_PORT);

    QObject::connect(msgErrorTcp,SIGNAL(clientConnection(bool)),this,SLOT(errConnectionHandler(bool)),Qt::UniqueConnection);
    QObject::connect(this,SIGNAL(errorTxHandler(QByteArray)), msgErrorTcp,SLOT(txData(QByteArray)),Qt::UniqueConnection);
    msgErrorTcp->Start(QHostAddress(_CONSOLE_IP),_CONSOLE_ERROR_PORT);

    QObject::connect(msgLogTcp,SIGNAL(clientConnection(bool)),this,SLOT(logConnectionHandler(bool)),Qt::UniqueConnection);
    msgLogTcp->Start(QHostAddress(_CONSOLE_IP),_CONSOLE_LOG_PORT);


    // Connette io logs
    connect(pInfo,SIGNAL(logTxHandler(QString)),this,SLOT(logMessages(QString)),Qt::UniqueConnection);

}

protoToConsole::protoToConsole(QObject *parent) :
    QObject(0)
{
    notificheConnected = FALSE;

    logConnected = FALSE;
    localDebugEnable=FALSE;

    // Socket per segnali asincroni
    notificheTcp = new TcpIpClient();


    // Socket per notifica errori
    msgErrorTcp = new TcpIpClient();    

    // Socket per notifica di loggin
    msgLogTcp = new TcpIpClient();

}


// Risposta di acknowledge a comando
void protoToConsole::endCommandAck(unsigned char id, unsigned char code)
{
    protoConsole ack(id,UNICODE_FORMAT);
    emit notificheTxHandler(ack.ackToQByteArray(code));
}



/*
 *  SLOT DI RICEZIONE DATI DA SEQUENZA RAGGI DI CALIBRAZIONE KV
 *
 *  CAMPI VALORE PARAMETRO data

    Stringa di Risposta:	<ID LEN %SetFineRaggiCalibKv PAR0 PAR1%>
    PARAMETRI:      Tipo        dato        Valore              Note
    PAR0            Int         Risultato
                                            0=OK
                                            1: KV BASSI
                                            2: KV ALTI
                                            3: IA BASSA
                                            4: IA ALTA
                                            5: ERRORE DI ALTRO TIPO

    PAR1            float         Valore Kv	Se PAR0 == 0 -> kV letti (Monitor)
                                            Se PAR0 == 5 -> Codice di errore
                                            Else: 0

    PAR2            int         Valore ImA	Se PAR0 == 0 -> ImA letti (Monitor)
                                            Else: 0
    Frame di risposta atteso: NESSUNO

 */
void protoToConsole::fineRaggiCalibKv(QByteArray data)
{
    int par0,  par2;
    float par1;
    protoConsole cmd(1,UNICODE_FORMAT);
    int tpls,mas,imean;

    disconnect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),pToConsole,SLOT(fineRaggiCalibKv(QByteArray)));

    float vmean = data.at(V_MEAN);
    mas = (data.at(MAS_PLS_L)+256*data.at(MAS_PLS_H)); // In 1/50 mAs unit
    tpls = (unsigned short) (data.at(T_MEAN_PLS_L)+256*data.at(T_MEAN_PLS_H));

    // Calcolo corrente media sulla durata dell'impulso
    if(tpls!=0) imean = mas * 20 / tpls;
    else if(data.at(I_MEAN)!=0) imean = pGeneratore->convertPcb190Ianode(data.at(I_MEAN));
    else imean=0;

    par1 = vmean;   // Valore riletto
    par2 = imean;   // Valore corrente riletto

    // Valutazione del risultato
    switch(data[RX_END_CODE])
    {
        case 0:
        case ERROR_IA_LOW:
        case ERROR_IA_HIGH:
        case ERROR_TMO_RX:
        case ERROR_HV_LOW:
        case ERROR_HV_HIGH:
            par0 = 0;        // Tutto OK
            break;
        default:
            par0 = 5;
            par1 = 0;
            par2 = 0;
            par1 = data[RX_END_CODE];
            break;
    }

    // Notifica la Console sul fine raggi
    cmd.addParam(QString("%1").arg(par0));
    cmd.addParam(QString("%1").arg(par1));
    cmd.addParam(QString("%1").arg(par2));


    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_FINE_RAGGI_CALIB_KV));

    // Disattiva simbolo grafico per raggi
    pConsole->handleSetXraySym(false);

    return;
}

/* Invio dati durata impulsi Tomo */
void protoToConsole::setSamples(QByteArray data)
{
    int ciclo;
    protoConsole cmd(1,UNICODE_FORMAT);

    cmd.addParam(QString("%1").arg((int) data.size()));
    for(ciclo=0; ciclo<data.size(); ciclo++){
        cmd.addParam(QString("%1").arg((int) data.at(ciclo)));
    }
    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_SET_PUSH_CMD));
    return;
}


/*
 *  SLOT DI RICEZIONE DATI DA SEQUENZA RAGGI DI CALIBRAZIONE KV
 *
 *  CAMPI VALORE PARAMETRO data

    Stringa di Risposta:	<ID LEN %SetFineRaggiCalibKv PAR0 PAR1%>
    PARAMETRI:      Tipo        dato        Valore              Note
    PAR0            Int         Risultato
                                            0=OK
                                            1: KV BASSI
                                            2: KV ALTI
                                            3: IA BASSA
                                            4: IA ALTA
                                            5: ERRORE DI ALTRO TIPO

    PAR1            float         Valore Kv	Se PAR0 == 0 -> kV letti (Monitor)
                                            Se PAR0 == 5 -> Codice di errore
                                            Else: 0

    PAR2            int         Valore ImA	Se PAR0 == 0 -> ImA letti (Monitor)
                                            Else: 0
    Frame di risposta atteso: NESSUNO

 */
void protoToConsole::fineRaggiCalibIa(QByteArray data)
{
    int par0,  par2;
    float par1;
    protoConsole cmd(1,UNICODE_FORMAT);
    int tpls,mas,imean;

    disconnect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),pToConsole,SLOT(fineRaggiCalibIa(QByteArray)));

    float vmean = pGeneratore->convertPcb190Kv(data.at(V_MEAN),1.0);
    mas = (data.at(MAS_PLS_L)+256*data.at(MAS_PLS_H)); // In 1/50 mAs unit
    tpls = (unsigned short) (data.at(T_MEAN_PLS_L)+256*data.at(T_MEAN_PLS_H));

    // Calcolo corrente media sulla durata dell'impulso
    if(tpls!=0) imean = mas * 20 / tpls;
    else if(data.at(I_MEAN)!=0) imean = pGeneratore->convertPcb190Ianode(data.at(I_MEAN));
    else imean=0;

    par1 = vmean;   // Valore riletto
    par2 = imean;   // Valore corrente riletto

    // Valutazione del risultato
    switch(data[RX_END_CODE])
    {
        case 0:
        case ERROR_IA_LOW:
        case ERROR_IA_HIGH:
        case ERROR_TMO_RX:
            par0 = 0;        // Tutto OK
            break;
        case ERROR_HV_LOW:
                par0 = 1;
            break;
        case ERROR_HV_HIGH:
                par0 = 2;
            break;
        default:
            par0 = 5;
            par1 = 0;
            par2 = 0;
            par1 = data[RX_END_CODE];
            break;
    }

    // Notifica la Console sul fine raggi
    cmd.addParam(QString("%1").arg(par0));
    cmd.addParam(QString("%1").arg(par1));
    cmd.addParam(QString("%1").arg(par2));


    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_FINE_RAGGI_CALIB_IA));

    // Disattiva simbolo grafico per raggi
    pConsole->handleSetXraySym(false);

    return;
}



void protoToConsole::notificheConnectionHandler(bool stat)
{
    notificheConnected = stat;
    if(stat)
    {

        ApplicationDatabase.setData(_DB_AWS_CONNECTION,(unsigned char) 1,0);
    }
    else
    {

        ApplicationDatabase.setData(_DB_AWS_CONNECTION,(unsigned char) 0,0);
    }
    return;
}


void protoToConsole::logConnectionHandler(bool stat)
{
    logConnected = stat;
    if(stat)
    {
        connect(this,SIGNAL(logTxHandler(QByteArray)), msgLogTcp,SLOT(txData(QByteArray)),Qt::UniqueConnection);

    }else
    {
        disconnect(this,SIGNAL(logTxHandler(QByteArray)), msgLogTcp,SLOT(txData(QByteArray)));

    }
    return;
}


void protoToConsole::errConnectionHandler(bool stat)
{
    errorConnected = stat;
    if(stat)
    {
        connect(paginaAllarmi,SIGNAL(newAlarmSgn(int,QString)),this,SLOT(alarmNotify(int,QString)),Qt::UniqueConnection);

    }else
    {
        disconnect(paginaAllarmi,SIGNAL(newAlarmSgn(int,QString)),this,SLOT(alarmNotify(int,QString)));

    }
    return;
}

//////////////////////////////////////////////////////////////////////////////
/*

 */
//////////////////////////////////////////////////////////////////////////////
void protoToConsole::enableXrayPush(bool enable)
{
    xrayEnable = enable;
    if(enable) ApplicationDatabase.setData(_DB_READY_EXPOSURE,(unsigned char) 1,0);
    else ApplicationDatabase.setData(_DB_READY_EXPOSURE,(unsigned char) 0,0);

}

/* Questa funzione viene lanciata solo se il pulsante è stato abilitato
 */
void protoToConsole::activationXrayPush(void)
{
    if(xrayEnable)
    {
        protoConsole cmd(1,UNICODE_FORMAT);
        emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_PUSH_CMD));
    }

}


void protoToConsole::notifyReadyForExposure(bool status)
{
    protoConsole cmd(1,UNICODE_FORMAT);
    if(status) cmd.addParam("ON");
    else cmd.addParam("OFF");
    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SET_READY));

}

/*
 *  Questa funzione viene lanciata per notificare la AWS di un comando di movimento braccio in corso
 */
void protoToConsole::notifyMovingArm(void)
{
    protoConsole cmd(1,UNICODE_FORMAT);
    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_TUBE_MOVING));

}

/*
 *  Questa funzione viene lanciata per notificare la AWS di un comando di movimento braccio in corso
 */
void protoToConsole::notifyProjectionSelection(QString projection)
{
    protoConsole cmd(1,UNICODE_FORMAT);
    cmd.addParam(projection);

    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_SELECTED_PROJECTION));

}

void protoToConsole::notifyAbortProjection(void)
{
    protoConsole cmd(1,UNICODE_FORMAT);
    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_SEND_ABORT_PROJECTION));

}
void protoToConsole::notifyRequestPowerOff(void)
{
    protoConsole cmd(1,UNICODE_FORMAT);
    emit notificheTxHandler(cmd.cmdToQByteArray(NOTIFY_REQUEST_POWER_OFF));
}




/*  HANDLER DI INVIO MESSAGGIO DI ERRORE SUL SOCKET DELLA CONSOLE
 *  IL MESSAGGIO E' Composto secondo il protocollo toConsole con
 *  i seguenti campi:
 *  %CODEICE MESSAGGIO%
 *  Questo messaggio è attivo solo a studio aperto
 */
void protoToConsole::alarmNotify(int codice, QString msg)
{
    protoConsole cmd(0,UNICODE_FORMAT);
    cmd.addParam(msg);

    if((errorConnected)&&(ApplicationDatabase.getDataU(_DB_STUDY_STAT)!=_CLOSED_STUDY_STATUS)) emit errorTxHandler(cmd.cmdToQByteArray(QString("%1").arg(codice)));

    return;
}


/*  HANDLER DI INVIO MESSAGGI DI LOG SUL SOCKET DELLA CONSOLE
 *  IL MESSAGGIO E' Composto secondo il protocollo toConsole con
 *  i seguenti campi:
 *  %MESSAGGIO%
 *
 */
void protoToConsole::logMessages(QString msg)
{

    protoConsole cmd(0,UNICODE_FORMAT);
    QString parsed;
    char ch;
    int i;

    // Effettua il parsing della stringa per eliminare i caratteri speciali
    parsed.clear();
    for(i=0;i< msg.size();i++)
    {
        ch = msg.at(i).toAscii();
        // if(ch=='\n') ch=' ';
        // else if(ch=='\r') continue;
        if(ch=='%') ch = '$';
        else if(ch=='<') ch = '|';
        else if(ch=='>') ch = '|';
        parsed.append(ch);
    }

    // Invia al LOGGER una versione della stringa parserata per togliere i caratteri speciali
    QByteArray buffer = cmd.cmdToQByteArray(parsed);
    emit logTxHandler(buffer);

    /*
    // Trasforma in Unicode il buffer ricevuto
#ifdef __PRINT
    #if (UNICODE_FORMAT == 0)
        PRINT(QString(buffer));
    #else
        QTextCodec *codec = QTextCodec::codecForName(UNICODE_TYPE);
        QString frame = codec->toUnicode(buffer);
        PRINT(frame);
    #endif
#endif
*/

    return;
}

// Funzione di notifica per classi esterne
void  protoToConsole::sendNotificheTcp(QByteArray frame)
{
    // if(!notificheConnected) return;
    emit notificheTxHandler(frame);
}


/**
 * @brief protoToConsole::systemUpdateStatus
 * @param perc
 *
 * Invia la percentuale di agigornamnto del sistema
 */
void protoToConsole::systemUpdateStatus(int perc, QString filename){
    protoConsole cmd(1,UNICODE_FORMAT);

    cmd.addParam(QString("%1 0 '%2'").arg(perc).arg(filename));
    sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SEND_FINE_SYS_UPDATE));

    return;
}

void protoToConsole::fineSystemUpdate(bool ris, QString errstr)
{
    protoConsole cmd(1,UNICODE_FORMAT);

    if(ris==true){
        cmd.addParam(QString("100 0"));
        sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SEND_FINE_SYS_UPDATE));
    }else
    {
        cmd.addParam(QString("100 -1 '%1'").arg(errstr));
        sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SEND_FINE_SYS_UPDATE));
    }

    return;
}

void protoToConsole::setConfigChanged(_configCode config){
    protoConsole cmd(1,UNICODE_FORMAT);

    switch(config){
    case protoToConsole::ACCESSORIO:

        cmd.addParam(QString("POTTER")); // Parola chiave
        if(pBiopsy->connected){
            if(pBiopsy->model == BYM_STANDARD_DEVICE) cmd.addParam(QString("BP %1").arg(pBiopsyStandard->accessorio));
            else cmd.addParam(QString("LAT 0"));
        }else if(pPotter->getPotId()==POTTER_2D)  cmd.addParam(QString("2D 0"));
        else if(pPotter->getPotId()==POTTER_TOMO)  cmd.addParam(QString("3D 0"));
        else if(pPotter->getPotId()==POTTER_MAGNIFIER)  cmd.addParam(QString("MG %1").arg(pCompressore->config.fattoreIngranditore[pPotter->getPotFactor()]));
        else  cmd.addParam(QString("ND 0"));
        sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SET_CONFIG_CHANGED));
        break;
    }


    return;
}

/*
 *  Connessione
 *  [ON/OFF]
 *
 *
 */
void protoToConsole::setBiopsyExtendedData(){
    protoConsole cmd(1,UNICODE_FORMAT);

    QString stringa;

    // Stato della connessione
    if(pBiopsy->connected) stringa = "CONNECTED ";
    else stringa = "DISCONNECTED ";

    // Lateralit� rilevata
    unsigned char latX = pBiopsyExtended->getLatX();
    if( latX == _BP_EXT_ASSEX_POSITION_LEFT) stringa += "X_L ";
    else if(latX == _BP_EXT_ASSEX_POSITION_RIGHT) stringa += "X_R ";
    else if(latX == _BP_EXT_ASSEX_POSITION_CENTER) stringa += "X_C ";
    else stringa += "X_N ";

    // Adapter
    unsigned char adapterId = pBiopsyExtended->getAdapterId();
    if(adapterId == _BP_EXT_ADAPTER_A) stringa += "AD_A ";
    else if(adapterId == _BP_EXT_ADAPTER_B) stringa += "AD_B ";
    else if(adapterId == _BP_EXT_ADAPTER_C) stringa += "AD_C ";
    else stringa += "AD_ND ";

    // Coordinate
    stringa += QString("X=%1 Y=%2 Z=%3 SH=%4 ").arg(ApplicationDatabase.getDataI(_DB_BIOP_X)).arg(ApplicationDatabase.getDataI(_DB_BIOP_Y)).arg(ApplicationDatabase.getDataI(_DB_BIOP_Z)).arg(ApplicationDatabase.getDataI(_DB_BIOP_SH));

    if(pBiopsyExtended->outPosition) stringa+="OUT=1 ";
    else stringa+= "OUT=0";


    cmd.addParam(stringa);
    sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SET_BIOPSY_EXTENDED_DATA));
    return;
}

void protoToConsole::setBiopsyStandardPosition(int curX, int curY, int curZ){
    protoConsole cmd(1,UNICODE_FORMAT);
    cmd.addParam(QString("%1 %2 %3").arg(curX).arg(curY).arg(curZ));
    sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SET_BIOPSY_STANDARD_POSITION));

   return;
}

void protoToConsole::setSpecimen(bool stat){
    protoConsole cmd(1,UNICODE_FORMAT);
    if(stat) cmd.addParam(QString("1"));
    else cmd.addParam(QString("0"));
    sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SPECIMEN));

   return;
}


