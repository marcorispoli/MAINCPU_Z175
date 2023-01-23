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

    connect(commandTcp,SIGNAL(rxData(QByteArray)),this,SLOT(commandRxHandler(QByteArray)),Qt::UniqueConnection);
    connect(commandTcp,SIGNAL(serverConnection(bool)),this,SLOT(commandConnectionHandler(bool)),Qt::UniqueConnection);
    connect(eventTcp,SIGNAL(serverConnection(bool)),this,SLOT(eventConnectionHandler(bool)),Qt::UniqueConnection);



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
        /*
        protoConsole cmd(0,UNICODE_FORMAT);
        cmd.addParam(REVISION);
        eventTcp->txData(cmd.cmdToQByteArray(EVENT_GANTRY_REVISION));
        */
        gantryStatus();

        connect(paginaAllarmi,SIGNAL(newAlarmSgn(int,QString)),this,SLOT(alarmNotify(int,QString)),Qt::QueuedConnection);
        connect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),this,SLOT(exposureEventSlot(QByteArray)),Qt::QueuedConnection);
        connect(pConfig,SIGNAL(changeStudy(bool)),this,SLOT(gantryStudy(bool)),Qt::QueuedConnection);

    }else{
        disconnect(paginaAllarmi,SIGNAL(newAlarmSgn(int,QString)),this,SLOT(alarmNotify(int,QString)));
        disconnect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),this,SLOT(exposureEventSlot(QByteArray)));
        disconnect(pConfig,SIGNAL(changeStudy(bool)),this,SLOT(gantryStudy(bool)));
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


    }else if(comando == CMD_GET_STATUS){

        gantryStatus();

    }else if(comando == CMD_SET_ERROR){

       PageAlarms::debugActivateNewAlarm(44, 1,true);

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

void diagnosticInterface::exposureEventSlot(QByteArray buffer){
    protoConsole dgnframe(0,UNICODE_FORMAT);
    int ival;

    // Solo in operativo
    if(pConsole->xSequence.workingMode != _EXPOSURE_MODE_OPERATING_MODE) return;

    int NAEC = buffer.at(NSAMPLES_AEC);
    int NPLS = buffer.at(NSAMPLES_PLS);
    int NTIME=0;
    int TPLS;

    // Codice di fine sequenza
    if(buffer.at(RX_END_CODE) == RXOK) ival = (int) 0;
    else if(buffer.at(RX_END_CODE) < LAST_ERROR_NO_PREP) ival = (int) 2;
    else if(buffer.at(RX_END_CODE) < LAST_ERROR_WITH_PREP ) ival = 1;
    else ival = 2;
    dgnframe.addParam(QString("%1").arg(ival));

    // Exposure Type
    if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_STD) dgnframe.addParam("MAN-2D");
    else if(pConsole->xSequence.exposureType  == MCC_CMD_RAGGI_AEC) dgnframe.addParam("AEC-2D");
    else if(pConsole->xSequence.exposureType  == MCC_CMD_RAGGI_TOMO) dgnframe.addParam("MAN-3D");
    else if(pConsole->xSequence.exposureType  == MCC_CMD_RAGGI_AEC_TOMO) dgnframe.addParam("AEC-3D");
    else if(pConsole->xSequence.exposureType  == MCC_CMD_RAGGI_AE) dgnframe.addParam("MAN-AE");
    else if(pConsole->xSequence.exposureType  == MCC_CMD_RAGGI_AE_AEC) dgnframe.addParam("AEC-AE");

    // Exposure context
    if(pBiopsy->connected) dgnframe.addParam("BIOPSY");
    else if(pConsole->xSequence.isCombo) dgnframe.addParam("COMBO");
    else if(pPotter->isMagnifier()) dgnframe.addParam("MAG");
    else dgnframe.addParam("STD");

    // Number of shots
    if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_STD) dgnframe.addParam("1");
    else if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AEC) dgnframe.addParam("2");
    else if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_TOMO)  dgnframe.addParam(QString("%1").arg(pConsole->xSequence.n_pulses));
    else if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AEC_TOMO) dgnframe.addParam(QString("%1").arg(pConsole->xSequence.n_pulses + 1));
    else if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AE) dgnframe.addParam("2");
    else if(pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AE_AEC) dgnframe.addParam("3");

    // Pre pulse data
    if((pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AEC) ||
       (pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AEC_TOMO) ||
       (pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AE_AEC)){

        dgnframe.addParam(QString("%1").arg(pConsole->xSequence.kVExposePRE));
        ival = (int) buffer.at(MAS_PRE_L) + 256 * (int) buffer.at(MAS_PRE_H);
        dgnframe.addParam(QString("%1").arg(ival/50));

    }else{
        dgnframe.addParam("0");
        dgnframe.addParam("0");
    }
    dgnframe.addParam(pConsole->xSequence.filtroPRE); // Pre Filtro

    // Pulse data
    dgnframe.addParam(QString("%1").arg(pConsole->xSequence.kVExposeLE));

    int mas_pls = (int) buffer.at(MAS_PLS_L) + 256 * (int) buffer.at(MAS_PLS_H);
    dgnframe.addParam(QString("%1").arg(mas_pls/50));

    dgnframe.addParam(pConsole->xSequence.filtroLE); // Pulse filter

    // Pulse AE data
    dgnframe.addParam(QString("%1").arg(pConsole->xSequence.kVExposeAE));
    dgnframe.addParam(QString("%1").arg(pConsole->xSequence.mAsAE));
    dgnframe.addParam(pConsole->xSequence.filtroAE); // AE filter

    // Effective kV
    if(buffer.at(NSAMPLES_PLS)){
        float vmean = pGeneratore->convertPcb190Kv(buffer.at(V_MEAN),1.0);
        dgnframe.addParam(QString("%1").arg(vmean));

    }else{
        dgnframe.addParam("0");
    }

    // Anodic current for pulse
    TPLS = 0;
    if((pConsole->xSequence.exposureType == MCC_CMD_RAGGI_TOMO) ||
       (pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AEC_TOMO) ||
       (pConsole->xSequence.exposureType == MCC_CMD_RAGGI_AEC_TOMO)){

        if(SAMPLES_BUFFER + 2 * (NAEC + NPLS) < buffer.size()){
            NTIME = buffer.at(SAMPLES_BUFFER + 2 * (NAEC + NPLS) );
            if(NTIME>1) TPLS = (int) buffer.at(SAMPLES_BUFFER + 2 * (NAEC + NPLS)+1+1);
        }
    }else{
        TPLS = (int) buffer.at(T_MEAN_PLS_L) + 256 * (int) buffer.at(T_MEAN_PLS_H);
    }

    if(TPLS){
        float imean = (float) mas_pls * 1000 / ((float) TPLS*50);
        dgnframe.addParam(QString("%1").arg(imean));
    }else {
        dgnframe.addParam("0");
    }

    // Pulse mean time
    dgnframe.addParam(QString("%1").arg((int) TPLS));

    // Power voltage
    dgnframe.addParam(QString("%1").arg((int) pGeneratore->convertHvexp(buffer.at(HV_POST_RAGGI))));

    // Tube Temperature
    dgnframe.addParam(QString("%1").arg(pCollimatore->tempCuffia));

    // Thicknes and force
    dgnframe.addParam(QString("%1").arg(pConsole->xSequence.breastThick));
    dgnframe.addParam(QString("%1").arg(pConsole->xSequence.breastForce));

    // Arm Angle
    dgnframe.addParam(QString("%1").arg(pConsole->xSequence.armAngle));

    eventTcp->txData(dgnframe.answToQByteArray(EVENT_GANTRY_EXPOSURE));
}

void diagnosticInterface::gantryStudy(bool stat){
    protoConsole dgnframe(0,UNICODE_FORMAT);

    if(stat) dgnframe.addParam("OPEN");
    else dgnframe.addParam("CLOSE");

    eventTcp->txData(dgnframe.answToQByteArray(EVENT_GANTRY_STUDY));
}

void diagnosticInterface::gantryStatus(void){
    protoConsole dgnframe(0,UNICODE_FORMAT);

    // Revision code
    dgnframe.addParam(REVISION);

    // X-RAY Tube
    dgnframe.addParam(pConfig->userCnf.tubeFileName);

    // Opzione uso starter alta velocità
    if(pConfig->sys.highSpeedStarter) dgnframe.addParam("YES");
    else dgnframe.addParam("NO");

    // Opzione rotazione motorizzata
    if(pConfig->sys.armMotor) dgnframe.addParam("YES");
    else dgnframe.addParam("NO");

    // Opzione Tilt motorizzata
    if(pConfig->sys.trxMotor) dgnframe.addParam("YES");
    else dgnframe.addParam("NO");

    // OpzioneAuto Filter
    if(pConfig->sys.autoFilter) dgnframe.addParam("YES");
    else dgnframe.addParam("NO");

    // Numero allarmi attivi
    dgnframe.addParam(QString("%1").arg(ApplicationDatabase.getDataU(_DB_NALLARMI_ATTIVI)));

    eventTcp->txData(dgnframe.answToQByteArray(EVENT_GANTRY_STATUS));
}
