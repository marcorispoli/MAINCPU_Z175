#include "application.h"
#include "appinclude.h"
#include "globvar.h"

// NOTA: SetFocus attiva il Fuoco
void console::activateConnections(void){
    // Creazione del socket di comunicazione esterna con la Console
    consoleSocketTcp = new TcpIpServer();
    if(consoleSocketTcp->Start(_CONSOLE_IN_PORT)<0)
    {
        PRINT("IMPOSSIBILE APIRE LA PORTA DI COMUNICAZIONE CON LA CONSOLE!!");
        return;
    }

    QObject::connect(consoleSocketTcp,SIGNAL(rxData(QByteArray)),this,SLOT(consoleRxHandler(QByteArray)),Qt::UniqueConnection);
    QObject::connect(consoleSocketTcp,SIGNAL(serverConnection(bool)),this,SLOT(consoleConnectionHandler(bool)),Qt::UniqueConnection);
    QObject::connect(this,SIGNAL(consoleTxHandler(QByteArray)), consoleSocketTcp,SLOT(txData(QByteArray)),Qt::UniqueConnection);

    //QObject::connect(this,SIGNAL(raggiStdNotify(QByteArray)),this,SLOT(raggiStdReceptionNotify(QByteArray)),Qt::UniqueConnection);
    connect(this,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(guiNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    connect(this,SIGNAL(raggiDataSgn(QByteArray)),this,SLOT(rxDataLog(QByteArray)),Qt::UniqueConnection);

}

console::console(QObject *parent) :
    QObject(parent)
{

    // Creazione comunicazione con core M4 MASTER
    pGuiMcc = new mccCom(_DEF_MCC_GUI_TO_M4_MASTER,FALSE);  // Comunicazione Comandi  Per M4 MASTER
    pMasterRxMcc = new mccMasterCom(_DEF_MCC_MASTER_TO_APP_MASTER,TRUE);// Comunicazione Comandi  Da M4 MASTER


    // Init parametri
    currentMas=40*50;

    // Alla partenza di default la modalit√  di calibrazione √® disabilitata
    xSequence.workingMode=_EXPOSURE_MODE_OPERATING_MODE;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    openStudy = FALSE;
    xSequence.disable_check_compression = false;


}



void console::consoleConnectionHandler(bool stat)
{
    consoleConnected = stat;



    return;
}



///////////////////////////////////////////////////////////////////
/*
void console::consoleRxHandler(QByteArray data)

    This handler function is connected to the Socket RX.
    This is the Console protocol entry point.
    Follows a Frame protocol description


    "<ID LEN ... %Corpo del messaggio% ..>"

    ID = progressive ID number of the message;
    LEN = lenght of the whole message

 Data: 15/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::consoleRxHandler(QByteArray rxbuffer)
{
    int code;
    QString dstr;


    // Trasforma in Unicode il buffer ricevuto
#if (UNICODE_FORMAT == 0)
    protoConsole protocollo(&rxbuffer);
#else
    QTextCodec *codec = QTextCodec::codecForName(UNICODE_TYPE);
    QString frame = codec->toUnicode(rxbuffer);
    protoConsole protocollo(&frame);
#endif

    // Segnale per eventuali debug
    emit consoleRxSgn(rxbuffer);

    if(protocollo.isValid==FALSE)
    {
        protoConsole answ(protocollo.id,UNICODE_FORMAT);
        emit consoleTxHandler(answ.answToQByteArray("NOK FORMATO"));
        return;
    }

    protoConsole answ(protocollo.id,UNICODE_FORMAT);
    QString comando = protocollo.comando;

    // Se lo startup non Ë completato, nessun comando puÚ essere eseguito
    if(!pConfig->startupCompleted){
        emit consoleTxHandler(answ.answToQByteArray("NOK GANTRY_STARTUP_NOT_COMPLETED"));
        return;
    }

    if(comando==GET_PUSH){
        unsigned char prx = ApplicationDatabase.getDataU(_DB_XRAY_PUSH_BUTTON);
        if(prx)  emit consoleTxHandler( answ.cmdToQByteArray("OK 1"));
        else emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
    }else if(comando==SET_AEC_DATA)
    {
        if(protocollo.parametri.count()!=2) return;
        handleSetAECdata(QString(protocollo.parametri[0]), QString(protocollo.parametri[1]));
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        return;
    }else if(comando==SET_AE_DATA)
    {
        if(protocollo.parametri.count()!=2) return;
        handleSetAEdata(QString(protocollo.parametri[0]), QString(protocollo.parametri[1]));
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        return;
    }else if(comando==SET_AEC_TOMO)
    {
        if(protocollo.parametri.count()!=3) return;
        handleSetTomoAecData(QString(protocollo.parametri[0]), QString(protocollo.parametri[1]), QString(protocollo.parametri[2]));
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        return;
    }else if(comando==GET_IA_RX)
    {
        if(pGeneratore->validated==FALSE) emit consoleTxHandler(answ.answToQByteArray("NOK 0"));
        else
        {
            answ.addParam(QString("%1").arg(pGeneratore->selectedIn));
            emit consoleTxHandler(answ.answToQByteArray("OK"));
            return;
        }

    }else if(comando == SET_XRAY_SYM)
    {
        if(protocollo.parametri.count()!=1) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        if(protocollo.parametri[0]=="ON") handleSetXraySym(true);
        else handleSetXraySym(false);
        return;

    }else if(comando==SET_SERIAL_NUMBER){
        if(protocollo.parametri.count()!=1) emit consoleTxHandler( answ.cmdToQByteArray("NOK 1"));
        else if(pConfig->SN_Configured ) emit consoleTxHandler( answ.cmdToQByteArray("NOK 2"));
        else{
            pConfig->userCnf.SN = protocollo.parametri[0];
            emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
            pConfig->saveUserCfg();
            pConfig->SN_Configured = true;
            pConfig->testConfigError(true,false);
        }
        return;

    }else if(comando==SET_SERVICE_PSW){
        if(protocollo.parametri.count()!=1) emit consoleTxHandler( answ.cmdToQByteArray("NOK 1"));
        else{
            pConfig->userCnf.ServicePassword = protocollo.parametri[0];
            emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
            pConfig->saveUserCfg();
        }
        return;

    }else if(comando==GET_SERIAL_NUMBER){
        if(!pConfig->SN_Configured) emit consoleTxHandler( answ.cmdToQByteArray("NOK 1"));
        else {
            answ.addParam(QString("%1").arg(pConfig->userCnf.SN));
            emit consoleTxHandler( answ.cmdToQByteArray("OK"));
        }
        return;

    }else if(comando == SET_CUR_DATE)
    {
        if(protocollo.parametri.count()!=2) return;

        // La funzione interna usa una system, quindi occorre attendere la trasmisiione della risposta..
        consoleSocketTcp->txData(answ.answToQByteArray(_IMMEDIATO),100000);
        // emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleSetDate(QString(protocollo.parametri[0]),QString(protocollo.parametri[1]));
        return;
    }else if(comando==RESET)
    {
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        //handleSetKv(param);
        return;
    }else if(comando==SET_READY)
    {
        if(protocollo.parametri.count()!=1) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        //handleSetKv(param);
        return;
    }else if(comando==GET_POTTER)
    {
        if(pBiopsy->connected){
            if(pBiopsy->model == BYM_STANDARD_DEVICE) emit consoleTxHandler(answ.answToQByteArray(QString("BP %1").arg(pBiopsyStandard->accessorio)));
            else  emit consoleTxHandler(answ.answToQByteArray(QString("LAT 0")));
        }
        else if(pPotter->getPotId()==POTTER_2D)  emit consoleTxHandler(answ.answToQByteArray("2D 0"));
        else if(pPotter->getPotId()==POTTER_TOMO)  emit consoleTxHandler(answ.answToQByteArray("3D 0"));
        else if(pPotter->getPotId()==POTTER_MAGNIFIER)  emit consoleTxHandler(answ.answToQByteArray(QString("MG %1").arg(pCompressore->config.fattoreIngranditore[pPotter->getPotFactor()])));        
        else  emit consoleTxHandler(answ.answToQByteArray("ND 0"));
        return;
    }
    else if(comando==SET_KV)
    {
        if(protocollo.parametri.count()!=1) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleSetKv(QString(protocollo.parametri[0]));
        return;
    }
    else if(comando==SET_MAS)
    {
        if(protocollo.parametri.count()!=1) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleSetMas(QString(protocollo.parametri[0]));
        return;
    }
    else if(comando==SET_TOMO_MAS)
    {
        if(protocollo.parametri.count()!=2) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleSetTomoMas(QString(protocollo.parametri[0]),QString(protocollo.parametri[1]));
        return;
    }
    else if(comando==SET_AF)
    {

        if(protocollo.parametri.count()!=1) emit consoleTxHandler( answ.cmdToQByteArray("NOK 1"));
        else if(! handleSetAf(QString(protocollo.parametri[0]))) emit consoleTxHandler( answ.cmdToQByteArray("NOK 2"));
        else emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        return;
    }else if(comando==SET_FOCUS)
    {
        if(protocollo.parametri.count()!=2){
            emit consoleTxHandler( answ.cmdToQByteArray("NOK 1"));
        }
        else if(!handleSetFocus(QString(protocollo.parametri[0]), QString(protocollo.parametri[1]))){
            emit consoleTxHandler( answ.cmdToQByteArray("NOK 2"));
        }
        else
        {
            emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        }

        return;
    }
    else if(comando==SET_MODE)
    {
        int ris = handleSetMode(&protocollo);
        answ.addParam(QString("%1").arg(ris));
        if(ris) emit consoleTxHandler( answ.cmdToQByteArray("NOK"));
        else emit consoleTxHandler( answ.cmdToQByteArray("OK"));
        return;
    }else if(comando==SET_CALIB_MODE)
    {
        QString intestazioneCalib;
        // Verifica tipologia di calibrazione
        if(protocollo.parametri.count()==0) return;

            if(protocollo.parametri[0]=="TOMO"){
                // Modalit‡ calibrazione Tomo
                if(protocollo.parametri.count()!=2) return;
                emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
                handleSetCalibTomo(QString(protocollo.parametri[1]));
                intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: CALIBRAZIONE TOMOGRAFIA");
            }else if(protocollo.parametri[0]=="SHOT"){
                emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
                if(protocollo.parametri.count()==1){
                    handleSetShotMode(TRUE); // Usa il detector
                    intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: ESPOSIZIONI CON DETECTOR");
                } else if(protocollo.parametri[1]=="NODETECTOR"){ handleSetShotMode(FALSE); // Non usa il detector
                    intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: ESPOSIZIONI SENZA DETECTOR");
                }

            }else if(protocollo.parametri[0]=="COLLITOMO"){
                if(pConfig->pcb249U1UpdateRequired) emit consoleTxHandler( answ.cmdToQByteArray("OK 255"));
                else emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
                handleSetColliTomoCalibMode(protocollo.id);
                intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: CALIBRAZIONE COLLIMAZIONE DINAMICA");

            }else if(protocollo.parametri[0]=="TOMOHOME"){
                emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
                handleSetTomoHomeCalibMode();
                intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: CALIBRAZIONE ZERO TOMOGRAFIA");
            }else if(protocollo.parametri[0]=="IA"){
                if(handleSetIaCalibMode()==FALSE) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
                else emit consoleTxHandler(answ.cmdToQByteArray("OK"));
                intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: CALIBRAZIONE CORRENTE DI FILAMENTO");
            }else if(protocollo.parametri[0]=="KV"){
                handleSetKvCalibMode();
                emit consoleTxHandler(answ.cmdToQByteArray("OK"));
                intestazioneCalib = QApplication::translate("OPENSTUDY-PAGE","MODO TEST: CALIBRAZIONE KV");
            }else {
                emit consoleTxHandler( answ.answToQByteArray("NOK 1"));
                return;
            }
            // Se l'interfaccia non si trova in modalit‡ open allora deve aprirla
            ApplicationDatabase.setData(_DB_CALIB_SYM,intestazioneCalib,0);
            setOpenStudy(true,""); // Apre uno studio locale anonimo
            return;
    }else if(comando==SET_OPER_MODE)
    {
        if(protocollo.parametri.count()!=0) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleSetOperatingMode();
        return;
    }else if(comando==SET_TUBE)
    {
        // Per i movimenti manuali il comando non deve avere effetti ulteriori
        if(!pConfig->sys.armMotor) {
            emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
            return;
        }

        if(protocollo.parametri.count()!=1) emit consoleTxHandler( answ.answToQByteArray("NOK 1"));
        else if(handleSetTube(QString(protocollo.parametri[0]),protocollo.id)==FALSE) emit consoleTxHandler( answ.answToQByteArray("NOK 2"));
        else {
            emit consoleTxHandler(answ.answToQByteArray("OK 255"));
            pToConsole->notifyMovingArm(); // Avvisa la AWS che sta per muovere
        }
        return;
    }else if(comando==SET_STOP_MOVE)
    {
        handleSetStopMove();
        emit consoleTxHandler(answ.answToQByteArray("OK 0"));
        return;
    }else if(comando==SET_ARM)
    {
        if(protocollo.parametri.count()<1) return;
        int angolo = (int) protocollo.parametri[0].toFloat();
        int min=angolo-5;
        int max=angolo+5;

        if(protocollo.parametri.count()>1) min = (int) protocollo.parametri[1].toFloat();
        if(protocollo.parametri.count()>2) max = (int) protocollo.parametri[2].toFloat();
        handleSetArm(angolo,min,max,protocollo.id);

        //pToConsole->notifyMovingArm(); // Avvisa la AWS che sta per muovere
        return;
    }else if(comando==GET_ARM)
    {
        emit consoleTxHandler( answ.answToQByteArray(QString("OK %1").arg(pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_DANGOLO)))));
    }else if(comando==GET_TRX)
    {
        emit consoleTxHandler( answ.answToQByteArray(QString("OK %1").arg(pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_TRX)))));
    }else if(comando==SET_DEAD_MAN)
    {
        if(protocollo.parametri.count()!=1) {
            answ.answToQByteArray(QString("NOK 1"));
            return;
        }
        emit consoleTxHandler( answ.answToQByteArray(QString("OK 0")));
        if(protocollo.parametri[0].toInt() == 1) handleSetDeadMan(true);
        else handleSetDeadMan(false);
        return;
    }else if(comando==SET_PUSH_ENA)
    {
        if(protocollo.parametri.count()!=1){
            emit consoleTxHandler( answ.answToQByteArray(QString("NOK 1")));
            return;
        }
        if(handleSetPushEna(QString(protocollo.parametri[0]))==FALSE) emit consoleTxHandler( answ.answToQByteArray(QString("NOK 2")));
        else emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        return;
    }
    else if(comando==START_SEQ)
    {
        if(protocollo.parametri.count()!=0) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        RxStart();
        return;
    }
    else if(comando==OPEN_STUDYL)
    {
        if(!pConfig->SN_Configured){
            emit consoleTxHandler( answ.answToQByteArray(QString("NOK 1")));
            return;
        }
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleOpenStudy(TRUE, &protocollo);
        return;
    }
    else if(comando==OPEN_STUDYW)
    {
        if(!pConfig->SN_Configured){
            emit consoleTxHandler( answ.answToQByteArray(QString("NOK 1")));
            return;
        }
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleOpenStudy(FALSE, &protocollo);
        return;
    }else if(comando==CLOSE_STUDY)
    {
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleCloseStudy();
        return;
    } else if(comando==GET_THICK)
    {
        answ.addParam(QString("%1").arg((unsigned short) pCompressore->breastThick));
        emit consoleTxHandler(answ.answToQByteArray());
        return;
    }
    else if(comando==GET_FORCE)
    {
        answ.addParam(QString("%1").arg((unsigned char) pCompressore->comprStrenght));
        emit consoleTxHandler( answ.answToQByteArray());
        return;
    }
    else if(comando==GET_COMPRESSOR)
    {
        // <id size %OK pad%>
        if(pCompressore->getPad()>=PAD_ENUM_SIZE) emit consoleTxHandler( answ.cmdToQByteArray(QString("OK %1").arg((int)PAD_ND)));
        else  emit consoleTxHandler( answ.cmdToQByteArray(QString("OK %1").arg((int)pCompressore->getPad())));
        return;
    }else if(comando==GET_TROLLEY)
    {   // Richiede la posizione del compressore. Richiesta asincrona
        emit consoleTxHandler(answ.cmdToQByteArray("OK 255"));

        // Invio comando
        unsigned char data=0;
        if(pConsole->pGuiMcc->sendFrame(MCC_GET_TROLLEY,protocollo.id,&data, 1)==FALSE)
        {            
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_SOFT,ERROR_MCC,TRUE); // Self resetting
        }

        return;
    }else if(comando==SET_EXPOSE_NO_COMPRESSION)
    { // Attiva la modalit‡ di esposizione senza compressione

        if(protocollo.parametri.count()!=1) {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
            return;
        }

        if(protocollo.parametri[0].toInt()==1)
            xSequence.disable_check_compression = true;
        else
            xSequence.disable_check_compression = false;
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        return;

    }else if(comando==SET_COMPRESSOR_RELEASE)
    { // Imposta la modalit‡ di rilascio compressione dopo esposizione
        if(protocollo.parametri.count()!=1) {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
            return;
        }
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));

        bool value;
        if(protocollo.parametri[0].toInt()==1) value=true;
        else value=false;

        if(value==pConfig->userCnf.enableSblocco) return;
        pConfig->userCnf.enableSblocco = value;
        if(value) ApplicationDatabase.setData(_DB_COMPRESSOR_UNLOCK,(unsigned char) 1);
        else  ApplicationDatabase.setData(_DB_COMPRESSOR_UNLOCK,(unsigned char) 0);

        return;


    }else if(comando==SET_XRAY_LAMP)
    {
        if(protocollo.parametri.count()!=1) return;
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        handleSetXrayLamp(QString(protocollo.parametri[0]));
        return;
    }else if(comando==SET_STARTER)
    {
        if(handleSetStarter(&protocollo)==TRUE) emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
    }else if(comando==GET_PACKAGE)
    {
        handleGetSoftwareRevisions(&answ);
    }else if(comando==GET_TUBES)
    {
        handleGetTubes(&answ);
    }else if(comando==SELECT_TUBE)
    {
        // Il comando richiede un parametro
        if(protocollo.parametri.size()!=1) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else handleSelectTube(protocollo.parametri.at(0), &answ);
    }else if(comando==STORE_TUBE_CONFIG_DATA)
    {
        // Il comando richiede un parametro
        if(protocollo.parametri.size()!=1) emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
        else if(protocollo.parametri.size()==1) handleStoreTube(protocollo.parametri.at(0), &answ); // Nuovo file

    }else if(comando==GET_KV_INFO)
    {
        handleGetKvInfo(&answ); // Stesso file
    }else if(comando==SET_KV_VDAC_DATA)
    {
        if(protocollo.parametri.size()!=30) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else handleSetKvVdacData(&protocollo, &answ);
    }else if(comando==SET_CALIB_KV_READ)
    {
        if(protocollo.parametri.size()!=2) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else handleSetCalibKvRead(&protocollo, &answ);
    }else if(comando==SET_KV_MONITOR_DATA)
    {
        if(protocollo.parametri.size()!=30) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else handleSetKvMonitorData(&protocollo, &answ);
    }else if(comando==SET_KV_RX_DATA)
    {
        if(handleSetKvRxData(&protocollo, &answ)==FALSE) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else emit consoleTxHandler(answ.cmdToQByteArray("OK"));
    }else if(comando==GET_IA_INFO)
    {
        handleGetIaInfo(&protocollo, &answ);

    }else if(comando==GET_IA_TOMO_INFO)
    {
        handleGetIaTomoInfo(&answ);

    }else if(comando==SET_IDAC_DATA)
    {

        // Prima verifica l'esattezza dei dati e poi eventualmente effettua le modifiche
        if(handleSetIdacData(&protocollo, FALSE)==FALSE) emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
        else
        {
            handleSetIdacData(&protocollo, TRUE);
            emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        }

    }else if(comando==SET_IDAC_TOMO_DATA)
    {
        if(handleSetIdacTomoData(&protocollo, &answ)==FALSE) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else emit consoleTxHandler(answ.cmdToQByteArray("OK"));

    }else if(comando==SET_IA_RX_DATA)
    {
        if(handleSetIaRxData(&protocollo, &answ)==FALSE) emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        else emit consoleTxHandler(answ.cmdToQByteArray("OK"));
    }else if(comando==SET_BIOPSY_EXTENDED_MOVE)
    {
        handleBiopsyExtendedMoveXYZ(&protocollo, &answ);
    }else if(comando==SET_BIOPSY_EXTENDED_HOME)
    {
        handleBiopsyExtendedMoveHome(&protocollo, &answ);
    }else if(comando==GET_BIOPSY_EXTENDED_DATA)
    {
        handleGetBiopsyExtendedData(&protocollo, &answ);
    }else if(comando==SET_BIOPSY_CONFIG)
    {
         handleSetBiopsyConfig(&protocollo, &answ);
    }else if(comando==SET_BIOPSY_DATA)
    {
        code = handleSetBiopsyData(&protocollo);
        if(code>0)
        {
            answ.addParam(QString("%1").arg((int) code));
            emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        }else
        {
            emit consoleTxHandler(answ.cmdToQByteArray("OK 30"));
        }
    }else if(comando==GET_BIOPSY_Z)
    {
        handleGetBiopsyZ(&answ);
    }else if(comando==SET_LINGUA)
    {
        code = handleSetLingua(&protocollo);
        if(code==0) emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        else
        {
            answ.addParam(QString("%1").arg(code));
            emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        }
    }else if(comando==GET_HU)
    {

        int temp = ApplicationDatabase.getDataI(_DB_T_CUFFIA) & 0x00FF;
        long hu;

        // Percentuale hu anodici consentiti rispetto al massimo fissato in 300kHU
        if((protocollo.parametri.size()==1)&&(protocollo.parametri[0]=="P")){
            hu = (long) pGeneratore->getCurrentHU()*100/300;
            // hu = ((long) ((temp-25) * 100 / 40)); // Percentuale
            if(hu<0) hu = 0;

        }else if((protocollo.parametri.size()==1)&&(protocollo.parametri[0]=="G"))
            hu = temp; // Gradi
        else{
            // Hu anodici in HU units
            hu = (long) pGeneratore->getCurrentHU()*1000;
            //hu = ((long) (temp-25)) * 12375;
            if(hu<0) hu=0;
        }

        answ.addParam(QString("%1").arg((long) hu));
        emit consoleTxHandler(answ.cmdToQByteArray("OK"));
    }else if(comando==SET_FIRMWARE_UPDATE){
        handleSetFirmwareUpdate(&protocollo, &answ);
        return;

    }else if(comando == SET_SYS_BACKUP){
        handleSetSystemBackup(&protocollo, &answ);
        return;
    }else if(comando == SET_SYS_RESTORE){
        handleSetSystemRestore(&protocollo, &answ);
        return;
    }else if(comando == SET_USER_DIR_CLEAN){
        handleSetUserDirClean(&protocollo, &answ);
        return;
    }else if (comando == SET_AUTO_COMPRESSION){
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        handleCompressorActivation(1);
    }else if (comando == SET_UNLOCK_COMPRESSION){
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        handleCompressorActivation(0);
    }else if (comando == SET_OUTPUT_PULSE){
        if(protocollo.parametri.size()!=2) emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
        else emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        handleOutputPulse(protocollo.parametri[0],protocollo.parametri[1]);
    }else if (comando == SET_DEMO_MODE){
        if(protocollo.parametri.size()!=1) emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
        bool demo;
        if(protocollo.parametri[0].toInt() == 1) demo = true;
        else demo = false;

        // In caso lo stato non corrisponda a quello atteso comunica che deve fare il cambio di stato
        if(pConfig->setDemoMode(demo))  emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler(answ.cmdToQByteArray("OK 1"));

    }//////// COMANDI COLLIMAZIONE /////////////////////////////////////////////////////////////////////////////////////////
    else if(comando==SET_COLLI_FILTRO)
    {
        if(handleSetColliFilter(&protocollo)==TRUE)  emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler( answ.cmdToQByteArray("NOK"));
    }else if(comando==SET_COLLI_MIRROR)
    {
        if(handleSetColliMirror(&protocollo)==TRUE)  emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler( answ.cmdToQByteArray("NOK"));
    }else if(comando==GET_COLLI_FILTRO)
    {
        handleGetColliFilter(&protocollo,&answ);
    }else if(comando==GET_COLLI_MIRROR)
    {
        handleGetColliMirror(&protocollo,&answ);
    }else if(comando==SET_COLLI_STORE)
    {
        // La risposta negativa indica che la periferica non deve essere aggiornata e quindi
        // l'operazione √® istantanea.
        if(handleSetColliStore()){
            idColliStore = protocollo.id;
            emit consoleTxHandler( answ.cmdToQByteArray("OK 255"));
        }
        else emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
    }else if(comando==SET_COLLI_MODE)
    {
        if(handleSetColliMode(&protocollo)) emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler( answ.cmdToQByteArray("NOK 0"));
    }else if(comando==SET_MANUAL_COLLI)
    {
        if(handleSetManualColli(&protocollo)) emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler( answ.cmdToQByteArray("NOK 0"));

    }else if(comando==SET_COLLI_2D)
    {
        if(handleSetColli2D(&protocollo)) emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        else emit consoleTxHandler( answ.cmdToQByteArray("NOK 0"));
    }else if(comando==GET_COLLI_2D)
    {
        handleGetColli2D(&protocollo, &answ);
    }else if(comando==SET_COLLI_TOMO){
        if(handleSetColliTomo(&protocollo)==false) emit consoleTxHandler(answ.cmdToQByteArray(QString("NOK")));
        else emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
    }else if(comando==GET_COLLI_TOMO)
    {
        handleGetColliTomo(&protocollo, &answ);
    }else if(comando==GET_SPECIMEN){
        if(pCollimatore->accessorio == COLLI_ACCESSORIO_FRUSTOLI)  emit consoleTxHandler( answ.cmdToQByteArray("OK 1"));
        else emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
    }else if(comando==SET_TEST_CMD){
        unsigned char data;
        pGuiMcc->sendFrame(MCC_TEST,1,&data,0);
        emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
    }else if(comando==GET_ACR){
        unsigned short acr = paginaAcr->getAcrView();
        unsigned char suffix = paginaAcr->getAcrSuffix();
        answ.addParam(QString("%1").arg((unsigned int) acr));
        answ.addParam(QString("%1").arg((unsigned char) suffix));
        emit consoleTxHandler(answ.cmdToQByteArray("OK"));

    }else if(comando==SET_TOMO_SPEED){
        if(protocollo.parametri.size()!=1) {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
            return;
        }

        if(pConfig->setTomoSpeedMode(protocollo.parametri[0],protocollo.id)==false) emit consoleTxHandler(answ.cmdToQByteArray("NOK 2"));
        else emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));


    }else if(comando==SET_DETECTOR_TYPE){

        if(protocollo.parametri.size()!=1) {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
            return;
        }

        // Controllo sul tipo di detector
        unsigned char val;
        if(protocollo.parametri[0]==DETECTOR_SCREENPLUS_STR)    val = DETECTOR_SCREENPLUS;
        else if(protocollo.parametri[0]==DETECTOR_LMAM2_STR)    val = DETECTOR_LMAM2;
        else if(protocollo.parametri[0]==DETECTOR_SOLO_STR)     val = DETECTOR_SOLO;
        else if(protocollo.parametri[0]==DETECTOR_FDI_STR)      val = DETECTOR_FDI;
        else if(protocollo.parametri[0]==DETECTOR_LMAMV2_STR)   val = DETECTOR_LMAMV2;
        else if(protocollo.parametri[0]==DETECTOR_SMAM_STR)     val = DETECTOR_SMAM;
        else if(protocollo.parametri[0]==DETECTOR_SMAMV2_STR)   val = DETECTOR_SMAMV2;
        else if(protocollo.parametri[0]==DETECTOR_FDIV2_STR)    val = DETECTOR_FDIV2;
        else if(protocollo.parametri[0]==DETECTOR_SCREENPLUSV2_STR)  val = DETECTOR_SCREENPLUSV2;
        else {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 2"));
            return;
        }

        // Se il valore Ë cambiato
        if(val!= pConfig->sys.detectorType){
            pConfig->sys.detectorType = val;
            pConfig->saveSysCfg();  // Aggiornamento del file di configurazione
            pConfig->updateSystemCfg(); // Aggiornamento del driver
        }
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));

    }else if(comando==RESET_ALARMS){
        paginaAllarmi->resetOneShotAlarms();
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        return;
    }else if(comando==GET_TUBE_STATISTICS){
        answ.addParam(pConfig->userCnf.tubeFileName);
        answ.addParam(QString("%1").arg(pGeneratore->numShots));
        answ.addParam(QString("%1").arg((float)pGeneratore->cumulatedJ * 1.33 / 1000));
        answ.addParam(QString("%1").arg(pGeneratore->nTomo));
        answ.addParam(QString("%1").arg(pGeneratore->nStandard));
        emit consoleTxHandler(answ.cmdToQByteArray("OK"));
        return;
    }else if(comando==SET_TOMO_HOME){
        emit consoleTxHandler( answ.cmdToQByteArray("OK 0"));
        handleSetTomoHome(&protocollo);
    }else if(comando==GET_TOMO_HOME){
        handleGetTomoHome(&answ);
    }else if(comando==GET_GEN_CONF){
        if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01) dstr = "COLLI_A";
        else dstr = "COLLI_B";

        // Configurazione generale Mammografo
        // KV STARTER_TYPE COLLI_TYPE
        if(pConfig->sys.highSpeedStarter)
            emit consoleTxHandler( answ.cmdToQByteArray(QString("OK %1 HS %2").arg(pGeneratore->max_selectable_kv).arg(dstr)));
        else
            emit consoleTxHandler( answ.cmdToQByteArray(QString("OK %1 LS %2").arg(pGeneratore->max_selectable_kv).arg(dstr)));

    }else if(comando==SET_POWER_OFF){
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        handlePowerOff();

    }else if(comando==SET_IMAGE){
        if(protocollo.parametri.size()==0) {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
            return;
        }

        if(protocollo.parametri.size()==1) {
            ApplicationDatabase.setData(_DB_IMAGE_NAME,QString(""),0);
            emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
            return;
        }


        if(protocollo.parametri[1].toInt()==0) {
            ApplicationDatabase.setData(_DB_IMAGE_NAME,QString(""),0);
        }else{
            ApplicationDatabase.setData(_DB_IMAGE_NAME,protocollo.parametri[0],0);
        }

        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
    }else if(comando==SET_PROIEZIONI){
        if(protocollo.parametri.size()==0){
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
            return; // Comando non valido
        }
        handleSetProiezioni(&protocollo);
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));

    }else if(comando==SEL_PROIEZIONE){
        if(protocollo.parametri.size()==0)  {
            emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
            ApplicationDatabase.setData(_DB_SEL_PROJ, "",0);
            return;
        }

        if(protocollo.parametri.size()==1)  {
            // Aggiorna la selezione corrente
            ApplicationDatabase.setData(_DB_SEL_PROJ, protocollo.parametri[0],0);
            emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
            return;
        }

        emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
        return;

    }else if(comando==ABORT_PROJECTION){
        // Abort della selezione corrente
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));
        ApplicationDatabase.setData(_DB_SEL_PROJ,"",0);
        return;

    }else if(comando==GET_TOMO_CONFIG){
        // SKIP - INTERLEAVE - N-SAMPLES
        if(protocollo.parametri.size()!=1)  {
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 1"));
        }else{
            if(protocollo.parametri[0]=="W"){                
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.w.pre_samples));
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.w.skip_samples));
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.w.samples));
                emit consoleTxHandler(answ.cmdToQByteArray("OK"));
            }else if(protocollo.parametri[0]=="I"){
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.i.pre_samples));
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.i.skip_samples));
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.i.samples));
                emit consoleTxHandler(answ.cmdToQByteArray("OK"));
            }else if(protocollo.parametri[0]=="N"){
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.n.pre_samples));
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.n.skip_samples));
                answ.addParam(QString("%1").arg(pConfig->trxConfig.tomo.n.samples));
                emit consoleTxHandler(answ.cmdToQByteArray("OK"));
            }
        }

        return;

    }else if(comando==SET_LOGO){
        // Non pi˘ usata
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));

    }else if(comando==SET_LAT){
        // Non pi˘ usata
        emit consoleTxHandler(answ.cmdToQByteArray("OK 0"));

    }else if(comando==GET_GANTRY_TYPE){
        emit consoleTxHandler( answ.cmdToQByteArray(QString("OK DMD")));
    }else{
        emit consoleTxHandler( answ.cmdToQByteArray(QString("NA")));
    }
    return;
}

// ______________________________________________________________________________
// Comando della console per impostare la sequenza di MEME disponibili per
// selezionare una data proiezione durante uno studio
// PARAM[0] = N. PROIEZIONI
// PARAM[1...8] = PROIEZIONI
// Il massimo numero di proiezioni disponibili e' 8
// ______________________________________________________________________________
void console::handleSetProiezioni(protoConsole* protocollo)
{
    QString proiezioni;
    int index;

    if(protocollo->parametri.size()==0) return; // Comando non valido

    if(protocollo->parametri[0].toInt()==0) {
        ApplicationDatabase.setData(_DB_PROIEZIONI,"",0);
        return;
    }

    // Limita il numero di meme a 8
    int items = protocollo->parametri[0].toInt();
    if(items>8) items=8;

    for(index=0; index<items;index++){
        proiezioni.append(protocollo->parametri[index+1]).append(" ");
    }

    ApplicationDatabase.setData(_DB_PROIEZIONI,proiezioni,0);
    return;

}

///////////////////////////////////////////////////////////////////
/*
void console::handleSetDate(QString param)
    This function set the interface date and time.
    The command format is:

    param = "dd/mm/aaaa hh:mm:ss"
    dd = day;
    mm = mounth;
    aaaa = year;
    hh = hour;
    mm = minute;
    ss = second;

 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////

void console::handleSetDate(QString data, QString tempo)
{
    QString Y,M,D,h,m,s,command;


    D = data.left(2);data = data.right(data.size()-3);
    M = data.left(2);data = data.right(data.size()-3);
    Y = data;

    h = tempo.left(2);tempo = tempo.right(tempo.size()-3);
    m = tempo.left(2);tempo = tempo.right(tempo.size()-3);
    s = tempo.left(2);tempo = tempo.right(tempo.size()-3);

    echoDisplay.echoDate(D,M,Y,h,m,s,DBase::_DB_NO_OPT);

    command = QString("date -u %1%2%3%4%5.%6").arg(M).arg(D).arg(h).arg(m).arg(Y).arg(s);
    system(command.toStdString().c_str());

    command = QString("hwclock -w");
    system(command.toStdString().c_str());

    systemTimeUpdated = TRUE;

    // Con la ricezione della data Ë possibile inizializzare il dispositivo
    // per la misura della quantit‡ di calore accumulata nell'Anodo
    pGeneratore->initAnodeHU();

    return;
}

///////////////////////////////////////////////////////////////////
/*


 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleSetKv(QString param)
{
    float kv;
    kv=param.toFloat();

    // Seleziona i KV che dovranno essere utilizzati
    pGeneratore->setkV(kv);
}

///////////////////////////////////////////////////////////////////
/*
    mAs sono i mas da assegnare all'array.
    Questa funzione vale per esposizioni 2D

 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleSetMas(QString qmAs)
{
    double mAs;

    mAs = qmAs.toDouble();
    if(pGeneratore->setmAs(mAs)==FALSE) LOG(QString("CONSOLE <SetmAs>: INVALID mAs:%1").arg(qmAs));

}

/*
 *  FUNZIONE PER IMPOSTARE I DATI AEC PER SEQEUNZE 2D
 *  Questa funzione viene utilizzata sia per esposizioni
 *  standard sia per esposizioni nell'ambito dell'alta energia.
 *  Nel caso di Alta energia dovr√  essere indicato nel buffer dati.
 *
 */
void console::handleSetAECdata(QString qkV, QString qmAs)
{
    float kv;
    double mAs;
    unsigned char data[14];
    unsigned char code;
    QByteArray ret;

    kv=qkV.toFloat();

    // Seleziona i KV che dovranno essere utilizzati
    pGeneratore->setkV(kv);

    mAs = qmAs.toDouble();

    if(pGeneratore->setmAs(mAs)==FALSE) LOG(QString("CONSOLE <SetAECdata>: INVALID mAs:%1").arg(qmAs));

    // Effettua la validazione dei dati del generatore
    pGeneratore->aecMode = TRUE;
    code = pGeneratore->validateData();
    if(code)
    {
        // Errore di validazione
        DEBUG(QString("CONSOLE <SetAECdata>: ERRORE DI VALIDAZIONE:")+code);
        ret.clear();
        ret.append(1);
        ret.append(code);
        pConsole->emitConsoleCmdAnswer(console::COMANDO_RAGGI, ret);
        return;
    }
    xSequence.kVExposeLE = pGeneratore->selectedKv;
    xSequence.filtroLE = pCollimatore->getFilterTag(pCollimatore->getFiltroStat());


    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDac&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDac>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;
    if(pGeneratore->starterHS) data[7]|=4;
    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;

    if(xSequence.isAE){
        data[13] = _AEC_AE;  // INDICA CHE SI TRATTA DI AEC AE

    }else{
        data[13] = _AEC_2D;  // INDICA CHE SI TRATTA DI AEC 2D
    }


    if(pGuiMcc->sendFrame(MCC_CMD_EXP_AEC,1,data,14)==FALSE)
    {
        DEBUG("CONSOLE <SetAECdata>: ERRORE COMANDO MCC");
        ret.clear();
        ret.append(1);
        ret.append(ERROR_MCC_COMMAND);
        pConsole->emitConsoleCmdAnswer(console::COMANDO_RAGGI, ret);
        return;
    }
    return;

}


void console::handleSetAEdata(QString qkV, QString qmAs)
{
    float kv;
    double mAs;
    unsigned char data[14];
    unsigned char code;
    QByteArray ret;

    kv=qkV.toFloat();

    // Seleziona i KV che dovranno essere utilizzati
    pGeneratore->setkV(kv);

    mAs = qmAs.toDouble();

    if(pGeneratore->setmAs(mAs)==FALSE) LOG(QString("CONSOLE <SetAEdata>: INVALID mAs:%1").arg(qmAs));

    // Effettua la validazione dei dati del generatore
    pGeneratore->aecMode = FALSE;
    code = pGeneratore->validateData();
    if(code)
    {
        // Errore di validazione
        DEBUG(QString("CONSOLE <SetAEdata>: ERRORE DI VALIDAZIONE:")+code);
        ret.clear();
        ret.append(1);
        ret.append(code);
        pConsole->emitConsoleCmdAnswer(console::COMANDO_RAGGI, ret);
        return;
    }
    xSequence.kVExposeAE = pGeneratore->selectedKv;
    xSequence.mAsAE = (int) mAs;
    xSequence.filtroAE = pCollimatore->getFilterTag(pCollimatore->getFiltroStat());

    // Invia il log con la stringa di dati dei raggi in corso
    QString stringa =  QString("RX_DATA_INPUT: AE PULSE mAs=%1 ").arg(pGeneratore->selectedmAsDac/50);
    stringa.append(QString("kV=%1 VDAC=%2 VMIN=%3 VMAX=%4 ").arg(pGeneratore->selectedKv).arg(pGeneratore->selectedVdac).arg(pGeneratore->minV).arg(pGeneratore->maxV));
    stringa.append(QString("In=%1 IDAC=%2 IMIN=%3 IMAX=%4 ").arg(pGeneratore->selectedIn).arg(pGeneratore->selectedIdac).arg(pGeneratore->minI).arg(pGeneratore->maxI));
    stringa.append(QString("TMO=%1 HS=%2").arg(pGeneratore->timeoutExp).arg(pGeneratore->starterHS));
    LOG(stringa);

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDac&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDac>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;
    if(pGeneratore->starterHS) data[7]|=4;
    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;
    if(pConfig->userCnf.enableSblocco) data[13] = 1;
    else data[13] = 0;

    if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AE_H,1,data,14)==FALSE)
    {
        DEBUG("CONSOLE <SetAEdata>: ERRORE COMANDO MCC");
        ret.clear();
        ret.append(1);
        ret.append(ERROR_MCC_COMMAND);
        pConsole->emitConsoleCmdAnswer(console::COMANDO_RAGGI, ret);
        return;
    }
    return;

}

void console::handleSetTomoAecData(QString qkV, QString qnum, QString qmAs)
{
    unsigned char code;
    QByteArray ret;
    unsigned char data[14];

    handleSetTomoMas(qnum,qmAs); // Imposta i Mas
    handleSetKv(qkV); // Imposta i KV

    // Effettua la validazione dei dati del generatore
    pGeneratore->aecMode = TRUE;
    code = pGeneratore->validateData();
    if(code)
    {
        // Errore di validazione
        DEBUG(QString("CONSOLE <SetTomoAecData>: ERRORE DI VALIDAZIONE:") +  code);
        ret.clear();
        ret.append(1);
        ret.append(code);
        pConsole->emitConsoleCmdAnswer(console::COMANDO_RAGGI, ret);
        return;
    }
    xSequence.kVExposeLE = pGeneratore->selectedKv;
    xSequence.filtroLE = pCollimatore->getFilterTag(pCollimatore->getFiltroStat());

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDacTomo&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDacTomo>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;
    if(pGeneratore->starterHS) data[7]|=4;
    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;
    if(pGeneratore->starterHS) data[7]|=4;
    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;
    data[13] = _AEC_TOMO;  // INDICA CHE SI TRATTA DI AEC TOMO

    if(pGuiMcc->sendFrame(MCC_CMD_EXP_AEC,1,data,14)==FALSE)
    {
        DEBUG("CONSOLE <SetTomoAecData>: ERRORE COMANDO MCC");
        ret.clear();
        ret.append(1);
        ret.append(ERROR_MCC_COMMAND);
        pConsole->emitConsoleCmdAnswer(console::COMANDO_RAGGI, ret);
        return;
    }


}


///////////////////////////////////////////////////////////////////
/*
    index = indice dell'array delle esposizioni in caso di Tomo
    mAs sono i mas da assegnare all'array.

    Per ora l'array non √® definito ed implementato ma il comando
    √® pronto per gestirlo.

 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleSetTomoMas(QString qindex, QString qmAs)
{
    unsigned short index;
    float mAs;

    index = qindex.toUShort();
    mAs = qmAs.toDouble();

    if(index==0) pGeneratore->setMasTomo(index,mAs);
    else LOG(QString("CONSOLE <SetTomoMas>: INVALID INDEX:%1").arg(index));

}

///////////////////////////////////////////////////////////////////
/*mccGuiCom::


 Data: 14/10/2014
 Autore: M.Rispoli


  param: [W/Mo][Rh/Ag/Al]

*/
///////////////////////////////////////////////////////////////////
bool console::handleSetAf(QString param)
{
    // Imposta il materiale
    if(param=="WRh")
    {
        if(!pGeneratore->setFuoco(W_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Rh;
    }
    else if(param=="WAg")
    {
        if(!pGeneratore->setFuoco(W_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Ag;
    }
    else if(param=="WAl")
    {
        if(!pGeneratore->setFuoco(W_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Al;
    }else if(param=="WCu")
    {
        if(!pGeneratore->setFuoco(W_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Cu;
    }else if(param=="WMo")
    {
        if(!pGeneratore->setFuoco(W_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Mo;
    }
    else if(param=="MoRh")
    {
        if(!pGeneratore->setFuoco(Mo_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Rh;
    }
    else if(param=="MoAg")
    {
        if(!pGeneratore->setFuoco(Mo_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Ag;
    }
    else if(param=="MoAl")
    {
        if(!pGeneratore->setFuoco(Mo_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Al;
    }else if(param=="MoCu")
    {
        if(!pGeneratore->setFuoco(Mo_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Cu;
    }else if(param=="MoMo")
    {
        if(!pGeneratore->setFuoco(Mo_STR)) return FALSE;
        pCollimatore->filtroCmd = Collimatore::FILTRO_Mo;
    }else return FALSE;


    if(!pConfig->sys.autoFilter){
        // Filtro fisso, non deve fare nulla
    }else{
        // Impostazione del Filtro
        if(pCollimatore->setFiltro()==FALSE)
        {
            DEBUG("CONSOLE: <handleSetAf> FALLITA!");
            return FALSE;
        }
    }


    return TRUE;
}

bool console::handleSetFocus(QString materiale, QString dimensione)
{

    if(!pGeneratore->setFuoco(materiale))
    {
        DEBUG(QString("CONSOLE: <handleSetFocus> FUOCO NON VALIDO, ") + pGeneratore->confF1 + " " + pGeneratore->confF2);
        return FALSE;
    }


    // Imposta la dimensione
    if(dimensione=="Small") pGeneratore->setFuoco(Generatore::FUOCO_SMALL);
    else if(dimensione=="Large")  pGeneratore->setFuoco(Generatore::FUOCO_LARGE);
    else return FALSE;


    // Verifica se deve effettuare l'update del fuoco
    pGeneratore->updateFuoco();

    return TRUE;

}



/*
 *  Impostazione della modalit√  OPERATIVA
 */
void console::handleSetOperatingMode(void)
{
    // Impostazione modalit√  OPERATIVA
    xSequence.workingMode = _EXPOSURE_MODE_OPERATING_MODE;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);
    // Spegne il simbolo della calibrazione
     ApplicationDatabase.setData(_DB_CALIB_SYM,"",0);

     // Update collimazione
     PRINT("handleSetOperatingMode: COMPRESSOR UPDATE COLLI");
     pCollimatore->updateColli();

}

/*
 *  Modalit√  calibrazione 3D Detector
 */
void console::handleSetCalibTomo(QString samples)
{
    // Impostazione modalit√  OPERATIVA
    xSequence.workingMode = _EXPOSURE_MODE_CALIB_MODE_TOMO;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    // Impostazione numero di campioni richiesti dalla procedura
    xSequence.samples = samples.toInt();

    // Accende simbolo di calibrazione in corso
    // <TBD> impostare il simbolo relativo alla calibrazione in corso
    //pagina0->setCalibOn(TRUE);

    // Imposta la Modalit√  di funzionamento del generatore
    pGeneratore->tomoMode=TRUE;

    // Costringe a collimare OPEN
    PRINT("handleSetCalibTomo: COMPRESSOR UPDATE COLLI");
    pCollimatore->updateColli();

    // Annulla eventuali allarmi PAD
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_PAD,0);
}


/*
 *  Modalit√  calibrazione 2D Detector
 */
void console::handleSetShotMode(bool useDetector)
{
    // Impostazione modalit√  OPERATIVA
    if(useDetector) xSequence.workingMode = _EXPOSURE_MODE_RX_SHOT_MODE;
    else xSequence.workingMode = _EXPOSURE_MODE_RX_SHOT_NODET_MODE;

    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    // Costringe a collimare OPEN
    PRINT("handleSetShotMode: COMPRESSOR UPDATE COLLI");
    pCollimatore->updateColli();

    // Annulla eventuali allarmi PAD
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_PAD,0);


}


void console::handleSetColliTomoCalibMode(unsigned char id)
{
    // Impostazione modalit√  OPERATIVA
    xSequence.workingMode = _EXPOSURE_MODE_CALIB_MODE_COLLI_TOMO;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    // Accende simbolo di calibrazione in corso
    // <TBD> impostare il simbolo relativo alla calibrazione in corso
    //pagina0->setCalibOn(TRUE);

    // Annulla eventuali allarmi PAD
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_PAD,0);

    // Attiva la procedura di aggiornamento periferica se necessario
    if(!pConfig->pcb249U1UpdateRequired) return;
    pendingId = id;
    connect(pConfig,SIGNAL(configUpdatedSgn()),this,SLOT(handleSetColliTomoCalibModeConfigSlot()),Qt::UniqueConnection);
    pConfig->updatePCB249U1();


}

void console::handleSetTomoHomeCalibMode(void)
{
    // Impostazione modalit√  OPERATIVA
    xSequence.workingMode = _EXPOSURE_MODE_CALIB_MODE_TOMO_HOME;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    // Accende simbolo di calibrazione in corso
    // <TBD> impostare il simbolo relativo alla calibrazione in corso
    //pagina0->setCalibOn(TRUE);

    // Annulla eventuali allarmi PAD
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_PAD,0);
}


// Funzione per calibrare da AWS la posizione iniziale della seqeunza in Wide
void console::handleSetTomoHome(protoConsole* protocollo)
{
    if(protocollo->parametri[0]=="W")  pConfig->trxConfig.tomo.w.home_position =  protocollo->parametri[1].toInt();
    else if(protocollo->parametri[0]=="I")  pConfig->trxConfig.tomo.i.home_position =  protocollo->parametri[1].toInt();
    else if(protocollo->parametri[0]=="N")  pConfig->trxConfig.tomo.n.home_position =  protocollo->parametri[1].toInt();

    pConfig->saveTomoConfig(pConfig->getTomoFilename());
    pConfig->updateTrxDriver();

}

void console::handleGetTomoHome(protoConsole* answ)
{
    answ->addParam(QString("%1").arg(pConfig->trxConfig.tomo.w.home_position));
    answ->addParam(QString("%1").arg(pConfig->trxConfig.tomo.i.home_position));
    answ->addParam(QString("%1").arg(pConfig->trxConfig.tomo.n.home_position));
    emit consoleTxHandler(answ->cmdToQByteArray("OK"));

}


// Configurazione completata
void console::handleSetColliTomoCalibModeConfigSlot(){
    disconnect(pConfig,SIGNAL(configUpdatedSgn()),this,SLOT(handleSetColliTomoCalibModeConfigSlot()));
    pToConsole->endCommandAck(pendingId,0);
    return;
}

/*
 *  Modalit√  calibrazione KV Generatore
 */
void console::handleSetKvCalibMode(void)
{
    // Impostazione modalit√  OPERATIVA
    xSequence.workingMode = _EXPOSURE_MODE_CALIB_MODE_KV;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    // Accende simbolo di calibrazione in corso
    // <TBD> impostare il simbolo relativo alla calibrazione in corso
    //pagina0->setCalibOn(TRUE);

    // Imposta la Modalit√  di funzionamento del generatore
    pGeneratore->tomoMode=FALSE;    

    // Annulla eventuali allarmi PAD
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_PAD,0);

}


///////////////////////////////////////////////////////////////////
/*


 Data: 29/08/2017
 Autore: M.Rispoli

 [AEC/NOAEC] [TN/TW/TI/2D] [COMBO] [STATIC] [HIGHLOW]

 Questa funzione definisce la tipologi di sequenza raggi che verr√  eseguita.

 return 0 OK

*/
///////////////////////////////////////////////////////////////////
int console::handleSetMode(protoConsole* protocollo)
{
    // Inizializzazione sequenza di default
    xSequence.isTomoN=FALSE;
    xSequence.isTomoW=FALSE;
    xSequence.isTomoI=FALSE;
    xSequence.isTomoMoving=TRUE;
    xSequence.is2D=FALSE;
    xSequence.isCombo=FALSE;
    xSequence.isAEC=FALSE;
    xSequence.isAE= FALSE;


    // Passa in rassegna tutti i parametri
    for(int ciclo=0; ciclo < protocollo->parametri.size(); ciclo++){
        if(protocollo->parametri[ciclo]=="AEC")         xSequence.isAEC=TRUE;
        else if(protocollo->parametri[ciclo]=="TN")     xSequence.isTomoN=TRUE;
        else if(protocollo->parametri[ciclo]=="TW")     xSequence.isTomoW=TRUE;
        else if(protocollo->parametri[ciclo]=="TI")     xSequence.isTomoI=TRUE;
        else if(protocollo->parametri[ciclo]=="2D")     xSequence.is2D=TRUE;
        else if(protocollo->parametri[ciclo]== "COMBO") xSequence.isCombo=TRUE;
        else if(protocollo->parametri[ciclo]== "STATIC") xSequence.isTomoMoving=FALSE;
        else if(protocollo->parametri[ciclo]== "HIGHLOW") {
            xSequence.isAE= TRUE;
            xSequence.is2D=TRUE;
        }
    }

    // Imposta la Modalit√  di funzionamento nel generatore
    if(!xSequence.is2D) pGeneratore->tomoMode=TRUE;
    else        pGeneratore->tomoMode=FALSE;

    pGeneratore->aecMode = xSequence.isAEC;

    return 0;
}
///////////////////////////////////////////////////////////////////
/*


 Data: 29/06/2016
 Autore: M.Rispoli

 param:
 - CC   0¬∞
 - P15  15¬∞
 - M15 -15¬∞
 - HW  Home Wide
 - HN  Home Narrow
 - HI  Home Intermediate
 - val valore arbitrario +-26¬∞

*/
///////////////////////////////////////////////////////////////////
bool console::handleSetTube(QString param,unsigned char id)
{
    unsigned char data[4];


    // Imposta i parametri del comando
    data[1]=MOVE_WAIT_START; // Attende comando precedente + Attende questo comando
    data[2]=0;

    if(param=="CC")
        data[0]=TRX_MOVE_CC; // Comando CC
    else if(param=="P15"){
        data[0]=TRX_MOVE_P15; // Comando P15
    }
    else if(param=="M15"){
        data[0]=TRX_MOVE_M15; // Comando M15
    }
    else if(param=="HW"){
        data[0]=TRX_MOVE_HOME_W; // Comando Home Wide
    }
    else if(param=="HN"){
        data[0]=TRX_MOVE_HOME_N; // Comando Home Narrow
    }
    else if(param=="HI"){
        data[0]=TRX_MOVE_HOME_I; // Comando Home Intermediate
    }
    else
    {
        int angolo = param.toInt();
        if((angolo>26)||(angolo<-26)){            
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_TRX,TRX_INVALID_ANGOLO,TRUE); // Self resetting
            return FALSE;
        }

        data[0]=TRX_MOVE_ANGLE; // Movimento con angolo aggiuntivo
        data[1]=0;
        data[2] = (unsigned char) angolo;
        data[3] = (unsigned char) (angolo>>8);
    }

    // Invio comando
    if(pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,id,data, 4)==FALSE)
    {        
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_SOFT,ERROR_MCC,TRUE); // Self resetting
        return FALSE;
    }

    return TRUE;

}

/*
 *  Comando di interruzione immediato di qualsiasi movimento
 *  Utilizza il comando del gruppo service, per comodit√ 
 */
void console::handleSetStopMove(void){
    QByteArray buf;
    buf.append((unsigned char) SRV_ARM_STOP); // Comando di servizio

    // Invia il comando per fermare il movimento
    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());

}

/*
 *  Imposta la modalit√  dead-men
 */
void console::handleSetDeadMan(bool val){
    if(val==pConfig->userCnf.deadman) return; // E' gi√  impostata come richiesto
    pConfig->userCnf.deadman = val;
    pConfig->saveUserCfg();

    if(val) ApplicationDatabase.setData(_DB_DEAD_MEN,(unsigned char) 1,DBase::_DB_NO_OPT);
    else ApplicationDatabase.setData(_DB_DEAD_MEN,(unsigned char) 0,DBase::_DB_NO_OPT);

}

///////////////////////////////////////////////////////////////////
/*

 Questo comando richiede l'attivazione del braccio C-ARM
 Devono essere attivi i controlli su:
    - Compressione in corso
    - Angolo critico

 Il segnale hardware di disattivazione del movimento invece
 non viene controllato in quanto questa sequenza avviene al
 di fuori della sequenza raggi e senza una paziente sotto
 compressione..

 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleSetArm(int target,int minimo, int massimo,int id)
{
    unsigned char data[3];
    signed short angolo;
    int angolo_corrente;
    protoConsole answ(id,UNICODE_FORMAT);


    // Se c'Ë la biopsia il massimo angolo attivabile Ë 90∞
    if(pBiopsy->connected){
        if(abs(target)>90){
            answ.addParam(QString("%1").arg((int)ARM_RANGE_ERROR));
            emit consoleTxHandler(answ.cmdToQByteArray("NOK 9"));
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_ARM,ARM_RANGE_ERROR,TRUE);
            return;
        }
    }


    // Angolo in posizione: risponde immediato
    angolo_corrente =  pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_DANGOLO));
    angolo = target;

    xSequence.arm_angolo = angolo;
    xSequence.arm_min = minimo;
    xSequence.arm_max = massimo;


    // Per i movimenti manuali il comando non deve avere effetti ulteriori
    if(!pConfig->sys.armMotor) {
        emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
        return;
    }

    // SE si trova in un angolo tra +-2 il target allora non si muove
    if((angolo >= angolo_corrente-2) && (angolo <= angolo_corrente+2))
    {
       emit consoleTxHandler( answ.answToQByteArray(_IMMEDIATO));
       return;
    }

    // Se il braccio √® da spostare, vengono controllate le condizioni di movimento
    if(pCompressore->isCompressed())
    {
        answ.addParam(QString("%1").arg((int)ARM_ERROR_INVALID_STATUS));
        emit consoleTxHandler(answ.cmdToQByteArray("NOK"));
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_ARM,ARM_ERROR_INVALID_STATUS,TRUE);
        return;
    }

    // Risponde di attendere il completamento
    emit consoleTxHandler( answ.answToQByteArray(255));

    // Impostazione Parametro in gradi
    data[0] =(unsigned char) (angolo&0xFF);
    data[1] =(unsigned char) (angolo>>8);

    // Invio comando
    if(pConsole->pGuiMcc->sendFrame(MCC_CMD_ARM,id,data, 2)==FALSE)
    {        
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_SOFT,ERROR_MCC,TRUE); // Self resetting
    }

    pToConsole->notifyMovingArm(); // Avvisa la AWS che sta per muovere
    return;
}



///////////////////////////////////////////////////////////////////
/*

    Abilitazione pulsante raggi.

    Quando il pulsante raggi √® abilitato, alla pressione
    del pulsante raggi deve essere inviato alla console
    il comando di SET_PUSH per avvisare la console della
    richiesta di invio raggi.

    PARAMETRI:
        - param = 1 -> Abilitato
        - param = 0 -> Disabilitato

 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
bool console::handleSetPushEna(QString param)
{
    if(param=="ON")
    {
        // Verifica di alcune condizioni possibili di errore
        if(pConfig->testConfigError(true,true)) return false;

        // Verifica diagnostica generatore
        if(pGeneratore->dgn_fault){
            PageAlarms::reopenExistingAlarm(_DB_ALLARMI_ALR_GEN,pGeneratore->dgn_fault_code,FALSE);
            return FALSE;
        }

        // Verifica diagnostica compressore
        if(pCompressore->safety_fault){
            PageAlarms::reopenExistingAlarm(_DB_ALLARMI_ALR_COMPRESSORE,_COMPR_SAFETY_FAULT,FALSE);
            return FALSE;
        }

        // Verifica in caso di starter low speed che esso sia calibrato
        if(pConfig->sys.highSpeedStarter==false){
            if(!pGeneratore->isStarterCalibrated()){                
                PageAlarms::reopenExistingAlarm(_DB_ALLARMI_ALR_GEN, GEN_STARTER_NOT_CALIBRATED, TRUE);
                return FALSE;
            }
        }

        // Verifica che la porta dello studio sia chiusa
        if(ApplicationDatabase.getDataU(_DB_CLOSED_DOOR)==0){
            PageAlarms::reopenExistingAlarm(_DB_ALLARMI_ALR_RAGGI,ERROR_CLOSED_DOOR,TRUE);
            return FALSE;
        }

        // Aggiorname
        // Abilitazione pulsante raggi
        pToConsole->enableXrayPush(TRUE);

    }else
    {
        // Disabilitazione pulsante raggi
        pToConsole->enableXrayPush(FALSE);
    }


    return TRUE;
}

///////////////////////////////////////////////////////////////////
/*


 Data: 31/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleSetSblocco(void)
{
}
///////////////////////////////////////////////////////////////////
/*

 La console ha richiesto l'attivazione di una sequenza raggi.

 PARAM:
    - param = 0: sequenza raggi 2d standard
    - param = 1: sequenza raggi 2d AEC


    La sequenza richiede l'invio alla console del comando
    SEND_FINE_RAGGI(codice) per comunicare l'esito dei raggi.
    (Tale comando deve essere inviato alla ricezione dell'esito
    da MASTER M4)
    - codice = 0: raggi OK;
             = 1: raggi parziali
             = 2: fine raggi anticipato senza dose
             = 10: dati generatore non validi (mAs, Kv)

 PARAM:
    - param: 0 -> Raggi 2D standard
    - param: 1 -> Raggi 2D AEC

 Data: 30/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::RxStart(void)
{
    QByteArray ret;
    unsigned char code;
    unsigned char mcc_cmd;
    QString mccstr;

    // Salva i kV selezioni
    xSequence.kVExposePRE = xSequence.kVExposeLE = xSequence.kVExposeAE =pGeneratore->selectedKv;
    xSequence.filtroPRE = xSequence.filtroLE = xSequence.filtroAE = pCollimatore->getFilterTag(pCollimatore->getFiltroStat());

    xSequence.breastThick = pCompressore->breastThick;
    xSequence.breastForce = pCompressore->comprStrenght;
    xSequence.armAngle = pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_DANGOLO));

    // Allarme cuffia: non pu√≤ fare raggi

    if(pCollimatore->alrSensCuffia)
    {
        ret.clear();
        ret.append(ERROR_SENS_CUFFIA);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, MCC_CMD_RAGGI_STD,ret);
        return;
    }

    if(pCollimatore->alrCuffia)
    {
        ret.clear();
        ret.append(ERROR_CUFFIA_CALDA);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, MCC_CMD_RAGGI_STD,ret);
        return;
    }

    if(pGeneratore->alarmAnodeHU)
    {
        ret.clear();
        ret.append(ERROR_ANODE_HU);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, MCC_CMD_RAGGI_STD,ret);
        return;
    }



    // Comunque effettua il refresh dello starter
    pGeneratore->refreshStarter();

    // Verifica se trattasi di una modalit√  di calibrazione attiva
    if(xSequence.workingMode != _EXPOSURE_MODE_OPERATING_MODE)
    {
        if(pCollimatore->manualCollimation==TRUE)               pCollimatore->manualColliUpdate();
        if(xSequence.workingMode == _EXPOSURE_MODE_CALIB_MODE_COLLI_TOMO)     RxTomoColliCalibModeSequence();
        else if(xSequence.workingMode ==_EXPOSURE_MODE_CALIB_MODE_TOMO_HOME)  RxTomoColliCalibModeSequence();
        else if(xSequence.workingMode == _EXPOSURE_MODE_RX_SHOT_MODE)         RxShotModeSequence(TRUE);
        else if(xSequence.workingMode == _EXPOSURE_MODE_RX_SHOT_NODET_MODE)   RxShotModeSequence(FALSE);
        else if(xSequence.workingMode == _EXPOSURE_MODE_CALIB_MODE_KV)        RxKvCalibModeSequence();
        else if(xSequence.workingMode == _EXPOSURE_MODE_CALIB_MODE_IA)        RxIaCalibModeSequence();
        else if(xSequence.workingMode == _EXPOSURE_MODE_CALIB_MODE_TOMO)      RxTomoStaticModeSequence();
        return;
    }

    // Questa √® una sequenza Operativa per la quale sono attivi tutti i controlli del caso

    if(xSequence.isAE){
        if(xSequence.isAEC)
        {
            mcc_cmd = MCC_CMD_RAGGI_AE;
            mccstr=QString("-ALTA ENERGIA AEC-");
        }
        else
        {
            mcc_cmd = MCC_CMD_RAGGI_AE_AEC;
            mccstr=QString("ALTA ENERGIA STD");
        }

    }else if(xSequence.is2D)
    {
        if(xSequence.isAEC)
        {
            mcc_cmd = MCC_CMD_RAGGI_AEC;
            mccstr=QString("-2D AEC-");
        }
        else
        {
            mcc_cmd = MCC_CMD_RAGGI_STD;
            mccstr=QString("-2D-");
        }
    }else
    {
        // TOMO
        if(xSequence.isAEC)
        {
            mcc_cmd = MCC_CMD_RAGGI_AEC_TOMO;
            mccstr=QString("-TOMO AEC-");
        }
        else
        {
            mcc_cmd = MCC_CMD_RAGGI_TOMO;
            mccstr=QString("-TOMO-");
        }
    }


    // Verifica se il pulsante raggi risulta ancora premuto
    // Se non √® premuto termina direttamente la sequenza
    unsigned char prx =ApplicationDatabase.getDataU(_DB_XRAY_PUSH_BUTTON);
    if(!prx)
    {        

        ret.clear();
        ret.append(ERROR_PUSHRX_NO_PREP);
        ret.append((char) 0);
        ret.append((char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }


    // Effettua la validazione dei dati del generatore
    code = pGeneratore->validateData();
    if(code)
    {        
        ret.clear();
        ret.append(code);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }


    if((pConfig->sys.autoFilter) && (pCollimatore->getFiltroStat() == Collimatore::FILTRO_ND))
    {
        ret.clear();
        ret.append(ERROR_INVALID_FILTRO);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }

    // Controllo Carrello in compressione solo se non Ë stato disabilitato
    if(xSequence.disable_check_compression==false){

        if(pCompressore->isCompressed()==FALSE)
        {
            ret.clear();
            ret.append(ERROR_MISSING_COMPRESSION);
            ret.append(( char) 0);
            ret.append(( char) 0);
            guiNotify(1, mcc_cmd,ret);
            return;
        }
    }

    xSequence.disable_check_compression = false;
    int angolo_corrente =  pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_DANGOLO));

    // Controllo Angolo Braccio
    // Attenzione questo controllo non e' piu' valido con la Biopsia
    if((!pBiopsy->connected)&&((angolo_corrente>xSequence.arm_max) || (angolo_corrente<xSequence.arm_min)))
    {
        ret.clear();
        ret.append(ERROR_ANGOLO_ARM);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }

    // Controllo Su Potter
    if((!pPotter->isValid())&&(!pBiopsy->connected))
    {
        ret.clear();
        ret.append(ERROR_INVALID_POTTER);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }

    // Controlli in caso di Ingranditore inserito
    if(pPotter->isMagnifier())
    {
        // Controllo sulla validit√  del fattore di ingrandimento
        if(!pPotter->isValidMagFactor())
        {
            ret.clear();
            ret.append(ERROR_INVALID_MAG_FACTOR);
            ret.append(( char) 0);
            ret.append(( char) 0);
            guiNotify(1, mcc_cmd,ret);
            return;
        }

        // Controllo sul Fuoco Piccolo
        if(pGeneratore->getSelectedFuocoSize()==Generatore::FUOCO_LARGE)
        {
            ret.clear();
            ret.append(ERROR_INVALID_MAG_FUOCO);
            ret.append(( char) 0);
            ret.append(( char) 0);
            guiNotify(1, mcc_cmd,ret);
            return;
        }
    }else
    {
        // In tutti gli altri casi deve essere utilizzato il fuoco grande
        if(pGeneratore->getSelectedFuocoSize()==Generatore::FUOCO_SMALL)
        {
            ret.clear();
            ret.append(ERROR_INVALID_SMALL_FOCUS);
            ret.append(( char) 0);
            ret.append(( char) 0);
            guiNotify(1, mcc_cmd,ret);
            return;
        }
    }

    // Controllo sulla presenza del PAD
    if(!pCompressore->isValidPad())
    {
        ret.clear();
        ret.append(ERROR_MISSING_PAD);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }


    // Lancia la routine relativa
    if(xSequence.isAE) RxAESequence();
    else if(xSequence.is2D) Rx2DSequence();
    else Rx3DSequence();
}

void console::Rx2DSequence(void)
{

    unsigned char data[15];
    QByteArray ret;


    if(pBiopsy->connected){
        // In biopsia il flag deve sempre essere azzerato a scanso di equivoci
        xSequence.isCombo=false;
    }else{

        // Controllo sulla piastrina frustoli sempre se non si Ë in Biopsia
        if(pCollimatore->accessorio==COLLI_ACCESSORIO_FRUSTOLI){
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_COLLIMATION_PLATE,TRUE); // Self resetting
            ret.clear();
            ret.append(ERROR_INVALID_COLLIMATION_PLATE);
            ret.append(( char) 0);
            ret.append(( char) 0);
            if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
            else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
            return;
        }
    }

    // Mai consentito l'uso di accessori di collimazione
    if(
        (pCollimatore->accessorio==COLLI_ACCESSORIO_CALIB_PLEXYGLASS)||
        (pCollimatore->accessorio==COLLI_ACCESSORIO_PIOMBO)

       ){
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_COLLIMATION_PLATE,TRUE); // Self resetting
        ret.clear();
        ret.append(ERROR_INVALID_COLLIMATION_PLATE);
        ret.append(( char) 0);
        ret.append(( char) 0);
        if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
        else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
        return;
    }

    // Nel caso in cui sia una sequenza combo si richiesto che
    // ci sia il PAD24x30_TOMO per proseguire
    // Attenzione NON esiste la combo con la biopsia!!
    if(xSequence.isCombo)
    {        
        if(pCompressore->getPad()!=PAD_TOMO_24x30)
        {
            ret.clear();
            ret.append(ERROR_INVALID_PAD);
            ret.append(( char) 0);
            ret.append(( char) 0);
            if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
            else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
            return;
        }

        // Controllo protezione paziente 3D in combo obbligatorio
        if(pConfig->userCnf.enableCheckAccessorio){
            if(pConfig->userCnf.enable3DCheckAccessorio){
                if(pCollimatore->accessorio!=COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D)
                {
                    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MISS_PROT_PAZIENTE_3D,TRUE); // Self resetting
                    ret.clear();
                    ret.append(ERROR_MISS_PROT_PAZIENTE_3D);
                    ret.append(( char) 0);
                    ret.append(( char) 0);
                    if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
                    else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
                    return;
                }
            }
        }
    }else{

        // Ingranditore e Biopsia non necessitano di protezione paziente
        if((!pPotter->isMagnifier())&&(!pBiopsy->connected)){
            if(pConfig->userCnf.enableCheckAccessorio){
                if((pCollimatore->accessorio!=COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_2D)&&(pCollimatore->accessorio!=COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D)){
                    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MISSA_PROT_PAZIENTE,TRUE); // Self resetting
                    ret.clear();
                    ret.append(ERROR_MISSA_PROT_PAZIENTE);
                    ret.append(( char) 0);
                    ret.append(( char) 0);
                    if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
                    else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
                    return;
                }
            }
        }
    }

    // Invia il log con la stringa di dati dei raggi in corso
    QString stringa = QString("RX_DATA_INPUT: ");
    if(xSequence.isAEC) stringa.append(QString("RX_DATA_INPUT: PRE2D mAs=%1 ").arg(pGeneratore->selectedmAsDac/50));
    else stringa.append(QString("RX_DATA_INPUT: 2D mAs=%1 ").arg(pGeneratore->selectedmAsDac/50));

    stringa.append(QString("kV=%1 VDAC=%2 VMIN=%3 VMAX=%4 ").arg(pGeneratore->selectedKv).arg(pGeneratore->selectedVdac).arg(pGeneratore->minV).arg(pGeneratore->maxV));
    stringa.append(QString("In=%1 IDAC=%2 IMIN=%3 IMAX=%4 ").arg(pGeneratore->selectedIn).arg(pGeneratore->selectedIdac).arg(pGeneratore->minI).arg(pGeneratore->maxI));
    stringa.append(QString("TMO=%1 HS=%2").arg(pGeneratore->timeoutExp).arg(pGeneratore->starterHS));
    LOG(stringa);

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDac&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDac>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta Velocit√ 

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;

    // Gestione dello sblocco del compressore: NON IN BIOPSIA e non in combo
    if(pBiopsy->connected) data[13] = 0;
    else if (xSequence.isCombo) data[13] = 0;
    else if(pConfig->userCnf.enableSblocco) data[13] = 1;
    else data[13]=0;

    data[14]=_SHOT_WITH_DETECTOR; // Use of the detector

    //connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(notifyFineRaggi(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    if(xSequence.isAEC)
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AEC,1,data,14)==FALSE)
        {
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
            return;
        }
    }else
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_STD,1,data,15)==FALSE)
        {
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_STD,ret);
            return;
        }
    }

    handleSetXrayLamp("ON");
    handleSetXraySym(true);
    return;

}

void console::RxAESequence(void)
{

    unsigned char data[14];
    QByteArray ret;

    // Controllo sulla piastrine di collimazione non consentite
    if( (pCollimatore->accessorio==COLLI_ACCESSORIO_CALIB_PLEXYGLASS)||
        (pCollimatore->accessorio==COLLI_ACCESSORIO_PIOMBO)||
        (pCollimatore->accessorio==COLLI_ACCESSORIO_FRUSTOLI))
    {
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_COLLIMATION_PLATE,TRUE); // Self resetting
        ret.clear();
        ret.append(ERROR_INVALID_COLLIMATION_PLATE);
        ret.append(( char) 0);
        ret.append(( char) 0);
        if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
        else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
        return;
    }


    // Verifica la presenza dell'accessorio protezione paziente ma NON con l'ingranditore
    if(!pPotter->isMagnifier()){
        if(pConfig->userCnf.enableCheckAccessorio){
            if((pCollimatore->accessorio!=COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_2D)&&(pCollimatore->accessorio!=COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D)){
                PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MISSA_PROT_PAZIENTE,TRUE); // Self resetting
                ret.clear();
                ret.append(ERROR_MISSA_PROT_PAZIENTE);
                ret.append(( char) 0);
                ret.append(( char) 0);
                if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
                else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
                return;
            }
        }
    }


    // Invia il log con la stringa di dati dei raggi in corso
    QString stringa = QString("RX_DATA_INPUT: ");
    if(xSequence.isAEC) stringa.append(QString("RX_DATA_INPUT: PRE2D mAs=%1 ").arg(pGeneratore->selectedmAsDac/50));
    else stringa.append(QString("RX_DATA_INPUT: AE mAs=%1 ").arg(pGeneratore->selectedmAsDac/50));

    stringa.append(QString("kV=%1 VDAC=%2 VMIN=%3 VMAX=%4 ").arg(pGeneratore->selectedKv).arg(pGeneratore->selectedVdac).arg(pGeneratore->minV).arg(pGeneratore->maxV));
    stringa.append(QString("In=%1 IDAC=%2 IMIN=%3 IMAX=%4 ").arg(pGeneratore->selectedIn).arg(pGeneratore->selectedIdac).arg(pGeneratore->minI).arg(pGeneratore->maxI));
    stringa.append(QString("TMO=%1 HS=%2").arg(pGeneratore->timeoutExp).arg(pGeneratore->starterHS));
    LOG(stringa);

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDac&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDac>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta Velocit√ 

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;

    // Gestione dello sblocco del compressore: non sblocca
    if(pConfig->userCnf.enableSblocco) data[13] = 1;
    else data[13]=0;

    //connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(notifyFineRaggi(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    if(xSequence.isAEC)
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AE_AEC,1,data,14)==FALSE)
        {
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_AE_AEC,ret);
            return;
        }
    }else
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AE,1,data,14)==FALSE)
        {            
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_AE,ret);
            return;
        }
    }

    handleSetXrayLamp("ON");
    handleSetXraySym(true);
    return;

}


void console::RxTomoColliCalibModeSequence(void)
{
    unsigned char data[19];
    QByteArray ret;


    // Effettua la validazione dei dati del generatore
    pGeneratore->aecMode = FALSE;
    int code = pGeneratore->validateData();
    if(code)
    {
        ret.clear();
        ret.append(code);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
        return;
    }


    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDacTomo&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDacTomo>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta Velocit√ 

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;

    if(xSequence.isTomoN)
    {
        data[15] = _TOMO_MODE_NARROW;
        data[13] = pConfig->trxConfig.tomo.n.pre_samples;
        data[14] = pConfig->trxConfig.tomo.n.samples;
        data[16] = pConfig->trxConfig.tomo.n.skip_samples;

    }else if(xSequence.isTomoI)
    {
        data[15] = _TOMO_MODE_INTERMEDIATE;
        data[13] = pConfig->trxConfig.tomo.i.pre_samples;
        data[14] = pConfig->trxConfig.tomo.i.samples;
        data[16] = pConfig->trxConfig.tomo.i.skip_samples;
    }else
    {
        data[15] = _TOMO_MODE_WIDE;
        data[13] = pConfig->trxConfig.tomo.w.pre_samples;
        data[14] = pConfig->trxConfig.tomo.w.samples;
        data[16] = pConfig->trxConfig.tomo.w.skip_samples;

    }

    // Sblocco compressore non attivo
    data[17]=0;

    // Dead men
    if(pConfig->userCnf.deadman) data[18]=1;
    else data[18]=0;

    if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_TOMO,1,data,19)==FALSE)
    {
        ret.clear();
        ret.append(1);
        ret.append(ERROR_MCC_COMMAND);
        guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
        return;
    }


    handleSetXrayLamp("ON");
    handleSetXraySym(true);
    return;

}

/*______________________________________________________________________________
 *          SEQUENZA RAGGI PER CALIBRAZIONE DETECTOR IN TOMO
 *
 *
 */
void console::RxTomoStaticModeSequence(void)
{
    QByteArray ret;
    unsigned char code;


    // Effettua la validazione dei dati del generatore
    pGeneratore->aecMode = FALSE;
    code = pGeneratore->validateData();
    if(code)
    {        
        ret.clear();
        ret.append(code);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
        return;
    }


    // Costruisce il comando raggi da inviare al driver
    unsigned char data[19];
    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDacTomo&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDacTomo>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta Velocit√ 

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;
    data[13] = 0;  // Pre pulses da configurazione
    data[14] = xSequence.samples; //  pulses da configurazione
    data[15] = _TOMO_MODE_STATIC; //  Modo a braccio fermo
    data[16] = 0; // 0 Interleaves
    data[17] = FALSE; // No sblocco compressore

    // Dead men
    if(pConfig->userCnf.deadman) data[18]=1;
    else data[18]=0;



    if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_TOMO,1,data,19)==FALSE)
    {        
        ret.clear();
        ret.append(1);
        ret.append(ERROR_MCC_COMMAND);
        guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
        return;
    }

    handleSetXraySym(true);
    return;

}


/*______________________________________________________________________________
 *          SEQUENZA RAGGI PER CALIBRAZIONE GENERATORE
 *
 *
 *
 */
void console::RxKvCalibModeSequence(void)
{
    unsigned char data[14];
    QByteArray ret;

    // Collega il segnale per il termine sequenza
    connect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),pToConsole,SLOT(fineRaggiCalibKv(QByteArray)),Qt::UniqueConnection);

    //
    // Verifica che i dati siano stati validati: la validazione viene subito tolta
    // Per evitare di riutilizzare i dati uscendo dalla funzione
    if(kvCalibData.validated==FALSE)
    {
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_CALIB_DATA,TRUE); // Self resetting
        ret.clear();
        ret.resize(RX_DATA_LEN);
        ret[0] = ERROR_INVALID_CALIB_DATA;
        emit raggiDataSgn (ret);
        return;
    }
    kvCalibData.validated = FALSE;

    // Selezione del fuoco
    if(kvCalibData.anodo == pGeneratore->confF1) data[0] = 0; // F1 Grande
    else data[0] = 1; // F2 Grande

    // Dati di esposizione
    data[1] =  (unsigned char) (kvCalibData.Vdac & 0x00FF);
    data[2] =  (unsigned char) (kvCalibData.Vdac >>8);
    data[3] =  (unsigned char) (kvCalibData.Idac & 0x00FF);
    data[4] =  (unsigned char) (kvCalibData.Idac >> 8);

    //  mAs
    data[5] =  (unsigned char) (kvCalibData.mAs*50 & 0x00FF);
    data[6] =  (unsigned char) (kvCalibData.mAs*50 >> 8);
    data[7] =  50 | 0x80; // 100ms di Timeout usando il timer a 10msUnit

    data[8]=0;
    if(kvCalibData.SWA) data[8]|=1;
    if(kvCalibData.SWB) data[8]|=2;
    data[8]|=4; // Starter HS
    data[9]= 0; // No Griglia


    // Valori limite tensione e corrente
    data[10] = 255;
    data[11] = 0;

    // La protezione di corrente viene data dal timeout
    data[12] = 254;
    data[13] = 0;

    if(pGuiMcc->sendFrame(MCC_TEST_RX_SHOT,1,data, 14)==FALSE)
    {
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
        ret.clear();
        ret.resize(RX_DATA_LEN);
        ret[0] = ERROR_MCC_COMMAND;
        emit raggiDataSgn (ret);
        return;
    }

    // Attiva simbolo grafico per raggi
    handleSetXraySym(true);
    return;
}

/*______________________________________________________________________________
 *          SEQUENZA RAGGI PER CALIBRAZIONE IA-GENERATORE
 *
 *
 */
void console::RxIaCalibModeSequence(void)
{
    unsigned char data[14];
    QByteArray ret;

    // Collega il segnale per il termine sequenza
    connect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),pToConsole,SLOT(fineRaggiCalibIa(QByteArray)),Qt::UniqueConnection);


    // Verifica che i dati siano stati validati: la validazione viene subito tolta
    // Per evitare di riutilizzare i dati uscendo dalla funzione
    if(iACalibData.validated==FALSE)
    {
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_CALIB_DATA,TRUE); // Self resetting
        ret.clear();
        ret.resize(RX_DATA_LEN);
        ret[0] = ERROR_INVALID_CALIB_DATA;
        emit raggiDataSgn (ret);
        return;
    }
    iACalibData.validated = FALSE;

    // Selezione del fuoco
    if(iACalibData.anodo == pGeneratore->confF1)
    {
        if(iACalibData.fuoco == "G") data[0] = 0; // F1 Grande
        else data[0] = 2; // F1 piccolo
    }
    else
    {
        if(iACalibData.fuoco == "G") data[0] = 1; // F2 Grande
        else data[0] = 3; // F2 piccolo
    }

    // Dati di esposizione
    int vdac = pGeneratore->getVdac(iACalibData.Vnom);
    data[1] =  (unsigned char) (vdac & 0x00FF);
    data[2] =  (unsigned char) (vdac >>8);

    data[3] =  (unsigned char) (iACalibData.Idac & 0x00FF);
    data[4] =  (unsigned char) (iACalibData.Idac >> 8);
    data[5] =  (unsigned char) (iACalibData.mAs*50 & 0x00FF);
    data[6] =  (unsigned char) (iACalibData.mAs*50 >> 8);

    // ATTENZIOOOONEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    // Con la griglia 2D bisogna tenere in conto il tempo di disabilitazione della griglia stessa.
    // Quindi occorre scollegare il potter per garantire un corretto timeout

    //data[7] =  10 | 0x80; // 100ms massimi per ogni impulso: usa il timer corto da 10ms
    if(iACalibData.Vnom>35)
        data[7] =  2 | 0x80; // 20ms massimi per ogni impulso: usa il timer corto da 10ms
    else
        data[7] =  5 | 0x80; // 50ms massimi per ogni impulso: usa il timer corto da 10ms

    bool swa,swb;
    swa=pGeneratore->getSWA(iACalibData.Vnom);
    swb=pGeneratore->getSWB(iACalibData.Vnom);
    data[8]=0;

    if(swa) data[8]|=1;         // SWA=1, SWB =0
    if(swb) data[8]|=2;         // SWA=1, SWB =0

    data[8]|=  4;                                 // Gestione dello Starter:alta velocit√  sempre
    data[9] =  0;                               // Griglia da aggiungere

    // Valori limite tensione e corrente
    data[10] = pGeneratore->convertPcb190KvToRaw(iACalibData.Vnom+2);
    data[11] = pGeneratore->convertPcb190KvToRaw(iACalibData.Vnom-2);

    // Nessuna limitazione sulla corrente: la protezione viene effettuata con il timeout
    data[12] = 255;
    data[13] = 0;


    if(pGuiMcc->sendFrame(MCC_TEST_RX_SHOT,1,data, sizeof(data))==FALSE)
    {
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
        ret.clear();
        ret.resize(RX_DATA_LEN);
        ret[0] = ERROR_MCC_COMMAND;
        emit raggiDataSgn (ret);
        return;
    }

    // Attiva simbolo grafico per raggi
    handleSetXraySym(true);
    return;
}

/*______________________________________________________________________________
 *          SEQUENZA TOMOGRAFICA IN OPERATIVO
 *
 *
 */
void console::Rx3DSequence(void)
{

    unsigned char data[19];
    QByteArray ret;


    if(pBiopsy->connected){
        // In biopsia il flag deve sempre essere azzerato a scanso di equivoci
        xSequence.isCombo=false;
    }else{
        // Controllo sulla piastrina frustoli sempre se non si Ë in Biopsia
        if(pCollimatore->accessorio==COLLI_ACCESSORIO_FRUSTOLI){
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_COLLIMATION_PLATE,TRUE); // Self resetting
            ret.clear();
            ret.append(ERROR_INVALID_COLLIMATION_PLATE);
            ret.append(( char) 0);
            ret.append(( char) 0);
            if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
            else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
            return;
        }
    }

    // Mai consentito l'uso di accessori di collimazione
    if(
        (pCollimatore->accessorio==COLLI_ACCESSORIO_CALIB_PLEXYGLASS)||
        (pCollimatore->accessorio==COLLI_ACCESSORIO_PIOMBO)

       ){
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_INVALID_COLLIMATION_PLATE,TRUE); // Self resetting
        ret.clear();
        ret.append(ERROR_INVALID_COLLIMATION_PLATE);
        ret.append(( char) 0);
        ret.append(( char) 0);
        if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
        else guiNotify(1, MCC_CMD_RAGGI_STD,ret);
        return;
    }

    // Controllo sul compressore utilizzato
    if(!pBiopsy->connected){

        // Obbligatorio il potter 3D presente!
        if(ApplicationDatabase.getDataU(_DB_ACCESSORIO)!=POTTER_TOMO){
            ret.clear();
            ret.append(ERROR_INVALID_POTTER);
            ret.append(( char) 0);
            ret.append(( char) 0);
            if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC_TOMO,ret);
            else guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
            return;
        }

        // Obbligatorio il pad 3D presente!
        if(pCompressore->getPad()!=PAD_TOMO_24x30)
        {
            ret.clear();
            ret.append(ERROR_INVALID_PAD);
            ret.append(( char) 0);
            ret.append(( char) 0);
            if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC_TOMO,ret);
            else guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
            return;
        }

        // Obbigatoria protezione paziente 3D
        if((pConfig->userCnf.enableCheckAccessorio) && (pConfig->userCnf.enable3DCheckAccessorio)){
            if(pCollimatore->accessorio!=COLLI_ACCESSORIO_PROTEZIONE_PAZIENTE_3D)
            {
                PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MISS_PROT_PAZIENTE_3D,TRUE); // Self resetting
                ret.clear();
                ret.append(ERROR_MISS_PROT_PAZIENTE_3D);
                ret.append(( char) 0);
                ret.append(( char) 0);
                if(xSequence.isAEC) guiNotify(1, MCC_CMD_RAGGI_AEC_TOMO,ret);
                else guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
                return;
            }
        }
    }

    // Invia il log con la stringa di dati dei raggi in corso
    QString stringa = QString("RX_DATA_INPUT: ");
    if(xSequence.isAEC) stringa.append(QString("PRET mAs=%1 ").arg(pGeneratore->selectedmAsDacTomo/50.0));
    else stringa.append(QString("T mAs=%1 ").arg(pGeneratore->selectedmAsDacTomo/50.0));

    stringa.append(QString("kV=%1 VDAC=%2 VMIN=%3 VMAX=%4 ").arg(pGeneratore->selectedKv).arg(pGeneratore->selectedVdac).arg(pGeneratore->minV).arg(pGeneratore->maxV));
    stringa.append(QString("In=%1 IDAC=%2 IMIN=%3 IMAX=%4 ").arg(pGeneratore->selectedIn).arg(pGeneratore->selectedIdac).arg(pGeneratore->minI).arg(pGeneratore->maxI));
    stringa.append(QString("TMO=%1 HS=%2").arg(pGeneratore->timeoutExp).arg(pGeneratore->starterHS));
    LOG(stringa);

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDacTomo&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDacTomo>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta Velocit√ 

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;


    if(xSequence.isTomoN)
    {
        data[15] = _TOMO_MODE_NARROW;
        data[13] = pConfig->trxConfig.tomo.n.pre_samples;
        data[14] = pConfig->trxConfig.tomo.n.samples;
        data[16] = pConfig->trxConfig.tomo.n.skip_samples;

    }else if(xSequence.isTomoI)
    {
        data[15] = _TOMO_MODE_INTERMEDIATE;
        data[13] = pConfig->trxConfig.tomo.i.pre_samples;
        data[14] = pConfig->trxConfig.tomo.i.samples;
        data[16] = pConfig->trxConfig.tomo.i.skip_samples;
    }else
    {
        data[15] = _TOMO_MODE_WIDE;  //  Modo Wide
        data[13] = pConfig->trxConfig.tomo.w.pre_samples;
        data[14] = pConfig->trxConfig.tomo.w.samples;
        data[16] = pConfig->trxConfig.tomo.w.skip_samples;
    }

    // Gestione dello sblocco del compressore: NON IN BIOPSIA e non in combo
    if(pBiopsy->connected) data[17] = 0;
    else if (xSequence.isCombo) data[17] = 0;
    else if(pConfig->userCnf.enableSblocco) data[17] = 1;
    else data[17]=0;


    // Dead men
    if(pConfig->userCnf.deadman) data[18]=1;
    else data[18]=0;


    if(xSequence.isAEC)
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AEC_TOMO,1,data,19)==FALSE)
        {
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_AEC_TOMO,ret);
            return;
        }
    }
    else
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_TOMO,1,data,19)==FALSE)
        {            
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
            return;
        }
    }

    handleSetXrayLamp("ON");
    handleSetXraySym(true);
    return;

}


/*
 *  Dati di post esposizione inviati come log a Console
 *  data (vedi m4.h)    
*/
void console::rxDataLog(QByteArray buffer)
{

    QString stringa;
    int i;
    int NAEC = buffer.at(NSAMPLES_AEC);
    int NPLS = buffer.at(NSAMPLES_PLS);


    stringa.append(QString("\n\r------ RX_DATA_OUTPUT:%1 ---------\n\r").arg((int) buffer.at(RX_END_CODE)));
    stringa.append(QString("VBUS(V):%1 \n\r").arg((int) pGeneratore->convertHvexp(buffer.at(HV_POST_RAGGI))));
    stringa.append(QString("IFIL(mA):%1 \n\r").arg(47.98 * (float) buffer.at(IFIL_POST_RAGGI)));
    stringa.append(QString("mAs(pre):%1 \n\r").arg((int) (buffer.at(MAS_PRE_L)+256*buffer.at(MAS_PRE_H))));
    stringa.append(QString("mAs(pls):%1 \n\r").arg((int) (buffer.at(MAS_PLS_L)+256*buffer.at(MAS_PLS_H))/50));
    stringa.append(QString("Tpre(ms):%1 \n\r").arg((int) (buffer.at(T_MEAN_PRE_L)+256*buffer.at(T_MEAN_PRE_H))));
    stringa.append(QString("Tpls(ms):%1 \n\r").arg((int) (buffer.at(T_MEAN_PLS_L)+256*buffer.at(T_MEAN_PLS_H))));

    if((NAEC+NPLS)==0){
        stringa.append(QString("NO VOLTAGE/CURRENT SAMPLES AVAILABLE \n\r"));
    }else{
        float vmean = pGeneratore->convertPcb190Kv(buffer.at(V_MEAN),1.0);
        unsigned char imean = pGeneratore->convertPcb190Ianode(buffer.at(I_MEAN));
        stringa.append(QString("READ-KV(kV):%1 \n\r").arg(vmean));
        float sigma = (float) buffer.at(V_SIGMA) / 10.;
        if(sigma > 0.5) stringa.append(QString("\n\r WARNING!!!:------------------> SIGMA-KV(kV):%1 \n\r\n\r").arg(sigma));
        else stringa.append(QString("SIGMA-KV(kV):%1 \n\r").arg(sigma));
        stringa.append(QString("READ-IA(mA):%1 \n\r").arg((int) imean));


        // Aggiunge in coda i campionamenti
        if(NAEC){
            stringa.append(QString("AEC SAMPLES:%1  \n\r").arg(NAEC));
            for(i = 0; i< NAEC;i++){
                int rawI = (int) (buffer.at(SAMPLES_BUFFER + i));
                int rawV = (int) (buffer.at(SAMPLES_BUFFER + NAEC + NPLS + i));
                int mA = rawI*200/255;
                float kV = pGeneratore->convertPcb190Kv((unsigned char) rawV,1.0);
                stringa.append(QString("%1: I=%2(mA) %3(RAW) V=%4(kV) %5(RAW)] \n\r").arg(i).arg(mA).arg(rawI).arg(kV).arg(rawV));
            }

        }

        if(NPLS){
            stringa.append(QString("PULSE SAMPLES:%1  \n\r").arg(NPLS));
            for(i = 0; i< NPLS;i++){
                int rawI = (int) (buffer.at(SAMPLES_BUFFER + NAEC+ i));
                int rawV = (int) (buffer.at(SAMPLES_BUFFER + NAEC + NPLS + NAEC + i));
                int mA = rawI*200/255;
                float kV = pGeneratore->convertPcb190Kv((unsigned char) rawV,1.0);
                stringa.append(QString("%1: I=%2(mA) %3(RAW) V=%4(kV) %5(RAW)] \n\r").arg(i).arg(mA).arg(rawI).arg(kV).arg(rawV));
            }

        }
    }

    // Campionamenti di tempo
    QByteArray data;
    int NTIME=0;
    if(SAMPLES_BUFFER + 2 * (NAEC + NPLS) < buffer.size()){
        NTIME = buffer.at(SAMPLES_BUFFER + 2 * (NAEC + NPLS) );
        if(NTIME){
            stringa.append(QString("PULSE TIME SAMPLES:%1  \n\r").arg(NTIME));
            for(i=0;i<NTIME; i++){
                stringa.append(QString("%1: T=%2(ms) \n\r").arg(i).arg((int) buffer.at(SAMPLES_BUFFER + 2 * (NAEC + NPLS)+1+i)));
                data.append(buffer.at(SAMPLES_BUFFER + 2 * (NAEC + NPLS)+1+i));
            }
            // Invio Samples
            pToConsole->setSamples(data);

        }else{
            stringa.append(QString("NO PULSE TIME SAMPLES AVAILABLE  \n\r"));
        }
    }else{
        stringa.append(QString("NO PULSE TIME SAMPLES AVAILABLE  \n\r"));
    }

    LOG(stringa);

}



///////////////////////////////////////////////////////////////////
/*
void console::handleOpenStudy(bool local, QString param)
    This function set the Study to Open.

 PARAM:
    - local: TRUE the sudy is local else is remote
    - param:
             if local it is a code string;
             in case of REMOTE it is in the format <name>^<surname>


 Data: 14/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleOpenStudy(bool local, protoConsole* pFrame)
{
    QString stringa;
    int i;


    // Costruisce il nome completo in caso di spazi
    for(i=0; i< pFrame->parametri.size(); i++)
    {
        if(i!=0) stringa.append(" ");
        stringa.append(pFrame->parametri[i]);
    }

    setOpenStudy(local , stringa);

}

// COMANDO PER L'APERTURA DELLA PAGINA OPERATIVA
// Prima di richiedere al config di scegliere quale pagina
// aprire Ë necessario impostare la tipologia di pagina che deve essere aperta
// tra _OPEN_STUDY_LOCAL/_OPEN_STUDY_DICOM
void console::setOpenStudy(bool local, QString stringa)
{

    xSequence.arm_angolo = pConfig->convertDangolo(ApplicationDatabase.getDataI(_DB_DANGOLO));
    xSequence.arm_min = xSequence.arm_angolo - 5;
    xSequence.arm_max = xSequence.arm_angolo + 5;


    ApplicationDatabase.setData(_DB_INTESTAZIONE,stringa);
    if(local) ApplicationDatabase.setData(_DB_STUDY_STAT,(unsigned char) _OPEN_STUDY_LOCAL );
    else ApplicationDatabase.setData(_DB_STUDY_STAT,(unsigned char) _OPEN_STUDY_DICOM );
    openStudy = TRUE;

    // Attiva la pagina operativa
    pConfig->selectOperatingPage();
}


///////////////////////////////////////////////////////////////////
/*

 Data: 31/10/2014
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void console::handleCloseStudy(void)
{
    openStudy = FALSE;    
    ApplicationDatabase.setData(_DB_STUDY_STAT,(unsigned char) 0); // Questo fa chiudere la pagina e uscire alla MAIN

    // Aggiornamento file di statistiche del generatore se necessario
    pGeneratore->saveTubeStatisticsFile(QString(_TUBEPATH) + QString("/") +pConfig->userCnf.tubeFileName + QString("/")) ;

    // Apertura pagina Main
    //pConfig->selectMainPage();
    return;
}

void console::handleSetXrayLamp(QString par)
{
    ioOutputs out;
    out.mask.CPU_LMP_SW1=1;

    if(par=="ON") out.outputs.CPU_LMP_SW1=1;
    else out.outputs.CPU_LMP_SW1=0;


    io->setOutputs(out);
    return;
}



#define NBYTE_FINE_RAGGI 6
void console::guiNotify(unsigned char id, unsigned char mcccode, QByteArray data)
{
    short arm ;
    short trx;
    short gonio;
    int ival;

    unsigned char arm_mem_dir;
    protoConsole cmd(id,UNICODE_FORMAT);

    switch(mcccode)
    {
    case MCC_CMD_RAGGI_STD:  //tipo=QString("2D ");
    case MCC_CMD_RAGGI_TOMO: //if(tipo.isEmpty()) tipo=QString("T ");
    case MCC_CMD_RAGGI_AEC:  //if(tipo.isEmpty()) tipo=QString("2DAEC ");
    case MCC_CMD_RAGGI_AEC_TOMO: //if(tipo.isEmpty()) tipo=QString("TAEC ");
    case MCC_CMD_RAGGI_AE: //if(tipo.isEmpty()) tipo=QString("TAEC ");
    case MCC_CMD_RAGGI_AE_AEC: //if(tipo.isEmpty()) tipo=QString("TAEC ");

        xSequence.exposureType = mcccode;

        // Nel caso in cui sia stato chiamato con un numero di byte inferiore al previsto
        // aggiunge la parte mancante
        data.resize(NBYTE_FINE_RAGGI);

        // Aggiornamento delle statistiche solo con generazione
        if((data.at(0)==RXOK)||(data.at(0)<LAST_ERROR_WITH_PREP)){
            bool isStandard;
            if((mcccode == MCC_CMD_RAGGI_AE)||(mcccode == MCC_CMD_RAGGI_STD)||(mcccode == MCC_CMD_RAGGI_AEC)) isStandard = true;
            else isStandard = false;
            pGeneratore->notifyStatisticData(pGeneratore->selectedKv, (data.at(1)+256*data.at(2)), isStandard);
        }

        // Questa notifica non ndeve partire per calibraziuone Kv e Ia
        if(xSequence.workingMode == _EXPOSURE_MODE_CALIB_MODE_KV) return;
        if(xSequence.workingMode == _EXPOSURE_MODE_CALIB_MODE_IA) return;
        if(pGeneratore->manualMode==true) return;

        // Se la fine raggi contiene un errore, viene inviato
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, data.at(0),TRUE); // Self resetting

        // Reset della collimazione in caso di errore per sicurezza
        if(data.at(0)!=RXOK){
            pCollimatore->resetColliFlags();
        }

        if(data.at(0)==RXOK) ival = (int) protoToConsole::_RAGGI_OK;
        else if(data.at(0)<LAST_ERROR_NO_PREP) ival = (int) protoToConsole::_NO_RAGGI;
        else if(data.at(0)<LAST_ERROR_WITH_PREP) ival = (int) protoToConsole::_RAGGI_PARZIALI;
        else ival = (int)protoToConsole::_NO_RAGGI;

        // Codice di fine raggi
        cmd.addParam(QString("%1").arg(ival));

        // Aggiunge i dati dell'ultima esposizione
        cmd.addParam(QString("%1").arg((int)data.at(0))); // Aggiunge il codice ritornato
        cmd.addParam(QString("%1").arg(xSequence.kVExposeLE)); // Aggiunge i Kv
        cmd.addParam(QString("%1").arg((float)(data.at(1)+256*data.at(2)) / 50.0)); // Aggiunge i mas totali effettuati a bassa energia
        cmd.addParam(QString("%1").arg(pGeneratore->convertHvexp((int) data.at(3)))); // HV minimo durante i raggi

        cmd.addParam(QString("%1").arg(xSequence.kVExposeAE)); // Aggiunge i Kv ad alta energia
        cmd.addParam(QString("%1").arg((float)(data.at(4)+256*data.at(5)) / 50.0)); // Aggiunge i mas totali effettuati a bassa energia

        pToConsole->sendNotificheTcp(cmd.cmdToQByteArray(NOTIFY_SEND_FINE_RAGGI));
        handleSetXraySym(false);
        handleSetXrayLamp("OFF");
        break;


    case MCC_GET_TROLLEY:

        // Notifica asincrona posizione del compressore
        pToConsole->sendNotificheTcp(cmd.ackToQByteArray(data.at(0)));
        break;
    case MCC_CMD_ARM:
        // Handler di segnali di fine movimento Braccio
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_ARM, data.at(0),TRUE);// in caso di fallimento viene segnalato un   errore auto-resettante
        pToConsole->sendNotificheTcp(cmd.ackToQByteArray(data.at(0))); // Segnalazione alla AWS di movimento terminato
        break;

    case MCC_CMD_TRX:
        trx =     (short) (data.at(2) + 256 * data.at(3)) / 10; // Espresso in decimi di grado
        ApplicationDatabase.setData(_DB_TRX,(int) trx,0);
        PRINT(QString("ANGOLO TRX A FINE MOVIMENTO:%1").arg(trx));

        // Handler di segnali di fine movimento Tubo
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_TRX, data.at(0),TRUE); // in caso di fallimento viene segnalato un   errore auto-resettante
        pToConsole->sendNotificheTcp(cmd.ackToQByteArray(data.at(0)));      // Segnalazione alla AWS di movimento terminato

        break;

    case MCC_ARM_ERRORS:
        // Handler di segnalazione di errori ARM direttamente dal driver
        // data.at(0) -> Codice errore, data.at(1) -> Sub-Codice
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_ARM, data.at(0),false);
        break;

    case MCC_TRX_ERRORS:
        // Handler di segnalazione di errori TRX direttamente dal driver
        // data.at(0) -> Codice errore, data.at(1) -> Sub-Codice
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_TRX, data.at(0),false);
        break;

    case MCC_LENZE_ERRORS:
        // Handler di segnalazione di errori TRX direttamente dal driver
        // data.at(0) -> Codice errore, data.at(1) -> Sub-Codice
        PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_LENZE, data.at(0),false);
        break;

    case MCC_GET_GONIO:
        // Formato: 0-1 -> ARM
        // Formato: 2-3 -> TRX
        // Formato: 4-5 -> GONIO
        // Tutti gli angoli sono in formatoo decimo di grado
        arm =     (short) (data.at(0) + 256 * data.at(1));      // Espresso in decimi di grado
        trx =     (short) (data.at(2) + 256 * data.at(3)) / 10; // Espresso in decimi di grado
        gonio =   (short) (data.at(4) + 256 * data.at(5));      // Espresso in decimi di grado
        arm_mem_dir = (unsigned char) data.at(6);

        // Salvataggio precoce dello stato del braccio
        if(pConfig->armConfig.direction_memory!= arm_mem_dir){
            pConfig->armConfig.direction_memory = arm_mem_dir;
            pConfig->saveArmConfig();
        }

        ApplicationDatabase.setData(_DB_DANGOLO,(int) arm,0);
        ApplicationDatabase.setData(_DB_TRX,(int) trx,0);
        ApplicationDatabase.setData(_DB_GONIO,(int) trx,0);

        break;

    case MCC_PARKING_MODE_COMMANDS:

        // Gestione errori
        if(data.at(1)){
            PageAlarms::activateNewAlarm(_DB_ALLARMI_PARCHEGGIO,(int) data.at(1),TRUE);
            return;
        }

        if(data.at(0)==MCC_PARKING_MODE_COMMANDS_START_PARKING){
            // Parking mode
            pConfig->lenzeConfig.startupInParkingMode = true;
            ApplicationDatabase.setData(_DB_PARKING_MODE, (unsigned char) 1, DBase::_DB_FORCE_SGN);
            pConfig->saveLenzeConfig();
            pConfig->activatePowerOff();
        }else if(data.at(0)==MCC_PARKING_MODE_COMMANDS_START_UNPARKING){
            ApplicationDatabase.setData(_DB_PARKING_MODE, (unsigned char) 0, DBase::_DB_FORCE_SGN);
            pConfig->lenzeConfig.startupInParkingMode = false;
            pConfig->saveLenzeConfig();
        }

        break;
    default:
            return;
        break;
    }
}

// ----------------------------------------------------------------------------- DA FARE
///////////////////////////////////////////////////////////////////
/*

            GESTIONE MESSAGGI PROVENIENTI DA MASTER M4
            PER COMUNICAZIONI FINE SEQUENZE

            QUESTA ROUTINE HA LA STESSA AFFINITA' DELLA GUI !!!

 Data: 25/02/2015
 Autore: M.Rispoli

*/
///////////////////////////////////////////////////////////////////
void mccMasterCom::mccRxHandler(_MccFrame_Str mccframe)
{
 int ciclo;
 QByteArray bufdata;
 unsigned char cval;

#ifdef __DISABLE_MCC_RX
 return;
#endif


 switch(mccframe.cmd)
  {
        case MCC_GUI_NOTIFY: // Stato di esecuzione del processo su M4

         bufdata.clear();
         for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
             bufdata.append(mccframe.buffer[2+ciclo]);
         pConsole->emitMccGuiNotify(mccframe.id, mccframe.buffer[0],bufdata);
         break;


        case MCC_PCB215_NOTIFY: // Notifiche da Driver PCB215

        bufdata.clear();
        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
          bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitPcb215Notify(mccframe.id, mccframe.buffer[0],bufdata);
        break;

        case MCC_PCB190_NOTIFY: // Notifiche da Driver PCB190

        bufdata.clear();
        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
           bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitPcb190Notify(mccframe.id, mccframe.buffer[0],bufdata);
        break;

        case MCC_PCB249U1_NOTIFY: // Notifiche da Driver PCB249U1

        bufdata.clear();
        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
           bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitPcb249U1Notify(mccframe.id, mccframe.buffer[0],bufdata);
        break;
        case MCC_SERVICE_NOTIFY:

        bufdata.clear();
        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
          bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitServiceNotify(mccframe.id, mccframe.buffer[0],bufdata);
        break;

        case MCC_LOADER_NOTIFY: // Notifiche dal Loader M4

        bufdata.clear();
        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
          bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitLoaderNotify(mccframe.id, mccframe.buffer[0],bufdata);
        break;


        case MCC_BIOP_STANDARD_NOTIFY:

        bufdata.clear();
        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
           bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitBiopsyStandardNotify(mccframe.id, mccframe.buffer[0],bufdata);
        break;

        case MCC_BIOP_EXTENDED_NOTIFY:

         bufdata.clear();
         for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
            bufdata.append(mccframe.buffer[2+ciclo]);
         pConsole->emitBiopsyExtendedNotify(mccframe.id, mccframe.buffer[0],bufdata);
         break;

        case MCC_CONFIG_NOTIFY: // Notifica configuratore M4

        for(ciclo=0; ciclo<mccframe.buffer[1]; ciclo++ )
            bufdata.append(mccframe.buffer[2+ciclo]);
        pConsole->emitConfigNotify(mccframe.id, mccframe.buffer[0],bufdata);
        break;


        // Questo messaggio viene inviato da M4 a seguito di un'esposizione
        // per comunicare i dati registrati durante l'esposizione.
        case  MCC_RAGGI_DATA:      // Messaggio di notifica dati di esposizione

         bufdata.clear();
         for(ciclo=0; ciclo<mccframe.len; ciclo++)
             bufdata.append(mccframe.buffer[ciclo]);
         pConsole->emitRaggiData(bufdata);
        break;
 }
}


/////////////// CALIBRAZIONI GUIDATE DA CONSOLE /////////////////////////////////////////////////////////////////////

/*  Comando di impostazione contenuto file di configurazione
 *  per l'impostazione dei filtri
 *
 *  Il comando rende immediatamente operative le modifiche, dato che in
 *  modalit√  2D, la collimazione viene impostata ogni volta che viene effettuata
 *  una sequenza raggi.
 *
 *  FORMATO:  %  MAT0 MAT1 MAT2 MAT3 POS0 POS1 POS2 POS3 %
 *  MAT0,MAT1,MAT2,MAT3 [Rh,Ag,Al,Us]
 *  POS0,POS1,POS2,POS3 [0:255] -> -1 non viene modificato
 *  return TRUE: %OK 0%
 *  return FALSE:%NOK 0%
 */
bool console::handleSetColliFilter(protoConsole* frame)
{
    int i;

    if(frame->parametri.size() != 8) return FALSE;

    // CONTROLLO MATERIALI
    for(i=0;i<4;i++)
        if((frame->parametri[i]!="Rh")&&(frame->parametri[i]!="Al")&&(frame->parametri[i]!="Ag")&&(frame->parametri[i]!="Us")&&(frame->parametri[i]!="Cu")&&(frame->parametri[i]!="Mo")) return FALSE;

    // CONTROLLO POSIZIONI
    for(i=4;i<8;i++)
        if((frame->parametri[i].toInt()<-1)||(frame->parametri[i].toInt()>255)) return FALSE;

    for(i=0;i<4;i++)
    {
        if(frame->parametri[i]=="Rh")      pCollimatore->colliConf.filterType[i] = (unsigned char) Collimatore::FILTRO_Rh;
        else if(frame->parametri[i]=="Ag") pCollimatore->colliConf.filterType[i] = (unsigned char) Collimatore::FILTRO_Ag;
        else if(frame->parametri[i]=="Al") pCollimatore->colliConf.filterType[i] = (unsigned char) Collimatore::FILTRO_Al;
        else if(frame->parametri[i]=="Mo") pCollimatore->colliConf.filterType[i] = (unsigned char) Collimatore::FILTRO_Mo;
        else                               pCollimatore->colliConf.filterType[i] = (unsigned char) Collimatore::FILTRO_Cu;
    }

    for(i=4;i<8;i++)
        if(frame->parametri[i].toInt()!=-1) pCollimatore->colliConf.filterPos[i-4] = frame->parametri[i].toInt();

    return TRUE;
}

/*  Comando di impostazione steps per specchio in campo
 *
 *  Il comando rende immediatamente operative le modifiche, dato che in
 *  modalit√  2D, la collimazione viene impostata ogni volta che viene effettuata
 *  una sequenza raggi.
 *
 *  FORMATO:  %  STEPS %
 *  STEPS [0:2000] -> -1 non viene modificato
 *  return TRUE: operazione eseguita
 *  return FALSE: dati non corretti
 */
bool console::handleSetColliMirror(protoConsole* frame)
{
    int* pToMirrorSteps;
    if(frame->parametri.size() != 1) return FALSE;

    if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01) pToMirrorSteps = & (pCollimatore->colliConf.mirrorSteps_ASSY_01);
    else pToMirrorSteps = & (pCollimatore->colliConf.mirrorSteps_ASSY_02);

    if((frame->parametri[0].toInt()<-1)||(frame->parametri[0].toInt()>2000)) return FALSE;
    if(frame->parametri[0].toInt()!=-1){
        if(*pToMirrorSteps != frame->parametri[0].toInt()){
            *pToMirrorSteps = frame->parametri[0].toInt();
            pCollimatore->storeConfigFile();
        }
    }
    // Forza una nuova collimazione se necessario
    if(pCollimatore->manualCollimation==TRUE) pCollimatore->manualColliUpdate();

    // Spegne lo specchio per forzare l'update ..
    pCollimatore->setLamp(Collimatore::LAMPMIRR_OFF,0);
    return TRUE;
}




/*  Comando di impostazione contenuto file di configurazione collimazione
 *  per le lame di collimazione in modalit√  2D.
 *
 *  FORMATO:  % PAD ANODO INGRANDIMENTO FRONT BACK RIGHT LEFT TRAP%
 *  PAD = Stringa PAD
 *  FUOCO = [W/Mo]
 *  BACK, FRONT, LEFT, RIGHT, TRAP --> [-1/0:250] Se -1 il relativo valore non viene modificato
 *
 *  return true:  OK
 *  return false: error
 */
bool console::handleSetColli2D(protoConsole* frame)
{

    if(frame->parametri.size() != 8) return false;

    // Controllo sulla coppia PAD+ANODO
    QString pad = frame->parametri[0];
    QString anodo = frame->parametri[1];
    QString mag = frame->parametri[2]; // Predisposizione: ora non usato

    // Collimazione OPEN
    if(pad=="OPEN"){

        pCollimatore->colliConf.colliOpen.F = frame->parametri[3].toInt();
        pCollimatore->colliConf.colliOpen.B = frame->parametri[4].toInt();
        pCollimatore->colliConf.colliOpen.R = frame->parametri[5].toInt();
        pCollimatore->colliConf.colliOpen.L = frame->parametri[6].toInt();
        pCollimatore->colliConf.colliOpen.T = frame->parametri[7].toInt();
        return true;
    }

    if(pad=="PADTOMO"){

        if(anodo=="W"){           
            if( (pCollimatore->colliConf.colliTomoW.tomoFront != frame->parametri[3].toInt()) || (pCollimatore->colliConf.colliTomoW.tomoBack != frame->parametri[4].toInt()))
                pConfig->pcb249U1UpdateRequired = true;
            pCollimatore->colliConf.colliTomoW.tomoFront =  frame->parametri[3].toInt();
            pCollimatore->colliConf.colliTomoW.tomoBack =  frame->parametri[4].toInt();
        }else if(anodo=="Mo"){
            if( (pCollimatore->colliConf.colliTomoMo.tomoFront != frame->parametri[3].toInt()) || (pCollimatore->colliConf.colliTomoMo.tomoBack != frame->parametri[4].toInt()))
                pConfig->pcb249U1UpdateRequired = true;
            pCollimatore->colliConf.colliTomoMo.tomoFront =   frame->parametri[3].toInt();
            pCollimatore->colliConf.colliTomoMo.tomoBack = frame->parametri[4].toInt();
        }
        return true;
    }

    int code = pCompressore->getPadCodeFromTag(pad);
    if(code>=PAD_ENUM_SIZE) return false;

    for(int i=0; i<pCollimatore->colliConf.colli2D.size(); i++)
    {
        if(pCollimatore->colliConf.colli2D[i].PadCode == code)
        {
            pCollimatore->colliConf.colli2D[i].F = frame->parametri[3].toInt();
            pCollimatore->colliConf.colli2D[i].B = frame->parametri[4].toInt();
            pCollimatore->colliConf.colli2D[i].R = frame->parametri[5].toInt();
            pCollimatore->colliConf.colli2D[i].L = frame->parametri[6].toInt();
            pCollimatore->colliConf.colli2D[i].T = frame->parametri[7].toInt();

            return true;
        }
    }

    // Se √® arrivato qui allora il dato PAD non √® configurato
    _colliPadStr newColli2DItem;
    newColli2DItem.PadCode = code;
    newColli2DItem.F = frame->parametri[3].toInt();
    newColli2DItem.B = frame->parametri[4].toInt();
    newColli2DItem.R = frame->parametri[5].toInt();
    newColli2DItem.L = frame->parametri[6].toInt();
    newColli2DItem.T = frame->parametri[7].toInt();
    pCollimatore->colliConf.colli2D.append(newColli2DItem);
    return true;

}


bool console::handleSetColliMode(protoConsole* frame){
    if(frame->parametri.size() != 1) return false;

    if(frame->parametri[0]=="MANUAL"){
        pCollimatore->manualCollimation=TRUE;
        pCollimatore->manualColliUpdate();
        return true;
    }else if(frame->parametri[0]=="AUTO"){
        pCollimatore->manualCollimation=FALSE;
        pCollimatore->manualFiltroCollimation=FALSE;
        pCollimatore->setFiltro();
        PRINT("handleSetColliMode: COMPRESSOR UPDATE COLLI");
        pCollimatore->updateColli();

        return true;
    }

    return false;
}

bool console::handleSetManualColli(protoConsole* frame){

    if(frame->parametri.size() != 5) return false;
    if(pCollimatore->manualCollimation==FALSE) return false;

    pCollimatore->manualF = frame->parametri[0].toInt();
    pCollimatore->manualB = frame->parametri[1].toInt();
    pCollimatore->manualR = frame->parametri[2].toInt();
    pCollimatore->manualL = frame->parametri[3].toInt();
    pCollimatore->manualT = frame->parametri[4].toInt();


    return true;
}


void console::handleGetColli2D(protoConsole* frame, protoConsole* answer){

    if(frame->parametri.size() != 3) {
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK 1"));
        return ;
    }

    // Verifica se si tratta della collimazione OPEN
    QString pad = frame->parametri[0];
    QString anodo = frame->parametri[1];
    QString mag = frame->parametri[2];
    QString risultato;

    if(pad=="OPEN"){
        risultato = QString("OK %1 %2 %3 %4 %5").arg(pCollimatore->colliConf.colliOpen.F).arg(pCollimatore->colliConf.colliOpen.B).arg(pCollimatore->colliConf.colliOpen.R).arg(pCollimatore->colliConf.colliOpen.L).arg(pCollimatore->colliConf.colliOpen.T);
        emit  consoleTxHandler(answer->cmdToQByteArray(risultato));
        return;
    }

    if(pad=="PADTOMO"){
        if(anodo=="W"){
            risultato = QString("OK %1 %2 0 0 0").arg(pCollimatore->colliConf.colliTomoW.tomoFront).arg( pCollimatore->colliConf.colliTomoW.tomoBack);
        }else if(anodo=="Mo"){
            risultato = QString("OK %1 %2 0 0 0").arg(pCollimatore->colliConf.colliTomoMo.tomoFront).arg( pCollimatore->colliConf.colliTomoMo.tomoBack);
        }
        emit  consoleTxHandler(answer->cmdToQByteArray(risultato));
        return ;
    }

    int code = pCompressore->getPadCodeFromTag(pad);
    if((code>=PAD_ENUM_SIZE)||(code<0)) {
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK 2"));
        return ;
    }

    for(int i=0; i<pCollimatore->colliConf.colli2D.size(); i++)
    {
        if(pCollimatore->colliConf.colli2D[i].PadCode == code)
        {
            QString risultato = QString("OK %1 %2 %3 %4 %5").arg(pCollimatore->colliConf.colli2D[i].F).arg(pCollimatore->colliConf.colli2D[i].B).arg(pCollimatore->colliConf.colli2D[i].R).arg(pCollimatore->colliConf.colli2D[i].L).arg(pCollimatore->colliConf.colli2D[i].T);
            emit  consoleTxHandler(answer->cmdToQByteArray(risultato));
            return;
        }
    }

    // Se il collimatore non √® configurato restituisce la OPEN
    risultato = QString("OK %1 %2 %3 %4 %5").arg(pCollimatore->colliConf.colliOpen.F).arg(pCollimatore->colliConf.colliOpen.B).arg(pCollimatore->colliConf.colliOpen.R).arg(pCollimatore->colliConf.colliOpen.L).arg(pCollimatore->colliConf.colliOpen.T);
    emit  consoleTxHandler(answer->cmdToQByteArray(risultato));
    return ;

}


bool console::handleSetColliTomo(protoConsole* frame)
{
    int i;
    colliTomoConf_Str* pTomo;
    unsigned char* pLama;
    unsigned char val;

    // Imposta le lame
    if(frame->parametri.size() == COLLI_DYNAMIC_SAMPLES + 3){
        if(frame->parametri.at(0)=="W") pTomo = &pCollimatore->colliConf.colliTomoW;
        else if(frame->parametri.at(0)=="Mo") pTomo = &pCollimatore->colliConf.colliTomoMo;
        else return false;

        if((frame->parametri.at(1)=="L") &&  (frame->parametri.at(2)=="P")) pLama = (pTomo->tomoLeftBladeP);
        else if((frame->parametri.at(1)=="L") &&  (frame->parametri.at(2)=="N")) pLama = (pTomo->tomoLeftBladeN);
        else if((frame->parametri.at(1)=="R") &&  (frame->parametri.at(2)=="P")) pLama = (pTomo->tomoRightBladeP);
        else if((frame->parametri.at(1)=="R") &&  (frame->parametri.at(2)=="N")) pLama = (pTomo->tomoRightBladeN);
        else if((frame->parametri.at(1)=="T") &&  (frame->parametri.at(2)=="P")) pLama = (pTomo->tomoBackTrapP);
        else if((frame->parametri.at(1)=="T") &&  (frame->parametri.at(2)=="N")) pLama = (pTomo->tomoBackTrapN);
        else return false;

        for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++)
        {
            val = frame->parametri.at(3+i).toInt();
            if( val !=  pLama[i]){
                pConfig->pcb249U1UpdateRequired = true;
                pLama[i] = val;
            }
        }

        return true;
    }

    return false;

}

void console::handleGetColliTomo(protoConsole* frame, protoConsole* answer)
{
    int i;
    colliTomoConf_Str* pTomo;
    unsigned char* pLama;

    if(frame->parametri.size() == 3){
        if(frame->parametri.at(0)=="W") pTomo = &pCollimatore->colliConf.colliTomoW;
        else if(frame->parametri.at(0)=="Mo") pTomo = &pCollimatore->colliConf.colliTomoMo;
        else {
            emit consoleTxHandler(answer->cmdToQByteArray("NOK"));
            return;
        }
        if((frame->parametri.at(1)=="L") &&  (frame->parametri.at(2)=="P")) pLama = (pTomo->tomoLeftBladeP);
        else if((frame->parametri.at(1)=="L") &&  (frame->parametri.at(2)=="N")) pLama = (pTomo->tomoLeftBladeN);
        else if((frame->parametri.at(1)=="R") &&  (frame->parametri.at(2)=="P")) pLama = (pTomo->tomoRightBladeP);
        else if((frame->parametri.at(1)=="R") &&  (frame->parametri.at(2)=="N")) pLama = (pTomo->tomoRightBladeN);
        else if((frame->parametri.at(1)=="T") &&  (frame->parametri.at(2)=="P")) pLama = (pTomo->tomoBackTrapP);
        else if((frame->parametri.at(1)=="T") &&  (frame->parametri.at(2)=="N")) pLama = (pTomo->tomoBackTrapN);
        else {
            emit consoleTxHandler(answer->cmdToQByteArray("NOK"));
            return;
        }

        for(i=0; i<COLLI_DYNAMIC_SAMPLES; i++) answer->parametri.append(QString("%1").arg(pLama[i]));
        emit consoleTxHandler(answer->cmdToQByteArray("OK"));
        return ;
    }

    // Errore
    emit consoleTxHandler(answer->cmdToQByteArray("NOK"));
    return;

}

bool console::handleSetStarter(protoConsole* frame)
{
    unsigned char data;

    if(frame->parametri.size()!=1) return FALSE;
    if(frame->parametri[0]=="STOP") data = 0;
    else if(frame->parametri[0]=="L") data = 1;
    else if(frame->parametri[0]=="H") data = 2;
    else return FALSE;

    return pConsole->pGuiMcc->sendFrame(MCC_STARTER,1,&data, 1);

}



/* Comando di richiesta parametri filtro
 *
 *  FORMATO:  % no param %
 *
 *  return %OK MAT0 MAT1 MAT2 MAT3 POS0 POS1 POS2 POS3
 *  MAT0,MAT1,MAT2,MAT3 ->[Rh,Ag,Al,Us]
 *  POS0,POS1,POS2,POS3 ->[0:255]
 */
void console::handleGetColliFilter(protoConsole* frame,protoConsole* answ)
{
    int i;

    // Materiali associati alle posizioni del filtro
    for(i=0; i<4;i++)
    {
        if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Al) answ->parametri.append("Al");
        else if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Ag) answ->parametri.append("Ag");
        else if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Rh) answ->parametri.append("Rh");
        else if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Mo) answ->parametri.append("Mo");
        else answ->parametri.append("Cu");
    }

    // Posizioni associate ai filtri
    for(i=0; i<4;i++)
        answ->parametri.append(QString("%1").arg(pCollimatore->colliConf.filterPos[i]));

    // Invio risposta
    emit consoleTxHandler(answ->answToQByteArray("OK"));
    return;
}

/* Comando di richiesta parametri Specchio
 *
 *  FORMATO:  % no param %
 *
 *  return %OK STEPS%
 *  STEPS ->[0:2000]
 *
 */
void console::handleGetColliMirror(protoConsole* frame,protoConsole* answ)
{
    int* pToMirrorSteps;
    if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01) pToMirrorSteps = & (pCollimatore->colliConf.mirrorSteps_ASSY_01);
    else pToMirrorSteps = & (pCollimatore->colliConf.mirrorSteps_ASSY_02);

    // Steps specchio in campo
    answ->parametri.append(QString("%1").arg(*pToMirrorSteps));

    // Invio risposta
    emit consoleTxHandler(answ->answToQByteArray("OK"));

    return;
}

/*
 *  Effettua il salvataggio dei dati di collimazione. Se √® stato utilizzato
 *  il comando SetColliTomo, oltre al salvataggio dei dati verr√  anche aggiornata la periferica
 *  PCB249U1 con il velttore di dati di collimazione dinamica.
 *  RETURN:
 *  - FALSE: non viene effettuato l'aggiornamento della periferica
 *  - TRUE: √® in corso l'aggiornamento della periferica
 */
bool console::handleSetColliStore(void)
{

    // Salva tutto il contenuto in RAM nel file di configurazione
    pCollimatore->storeConfigFile();


    // Verifica se occorre aggiornare la perioferica
    if(!pConfig->pcb249U1UpdateRequired) return false;

    connect(pConfig,SIGNAL(configUpdatedSgn()),this,SLOT(pcb249U1ConfigCompletedSlot()),Qt::UniqueConnection);
    pConfig->updatePCB249U1();
    return true;
}

void console::pcb249U1ConfigCompletedSlot(void){
    disconnect(pConfig,SIGNAL(configUpdatedSgn()),this,SLOT(pcb249U1ConfigCompletedSlot()));
    pToConsole->endCommandAck(idColliStore,0);

}



/*
 *  Richiesta revisioni dei software di sistema
 *  FORMATO:  %no param%
 *  return %OK REVGUI-M REVGUI-S REVMS REVSL REVPCB215 REVPCB204 REVPCB240 REVPCB249U1 REVPCB249U2 REVPCB190%
 *  RVxx = STRINGA REVISIONE
 *
 */
void console::handleGetSoftwareRevisions(protoConsole* frame)
{
    //pendingId = frame->id;
    //connect(pConfig,SIGNAL(configRevisionSgn()),this,SLOT(consoleSoftwareRevisionsNotify()),Qt::UniqueConnection);
    //pConfig->updateSoftwareRevision();
    // Costruisce il frame di risposta
    frame->addParam("0");
    frame->addParam(pConfig->swConf.rvPackage); // Questo √® rilevato dal file di configurazione

    // I campi seguenti sono letti direttamente dalle periferiche
    frame->addParam(pConfig->rvGuiMaster);
    frame->addParam(pConfig->rvGuiSlave);
    frame->addParam(pConfig->rvM4Master);
    frame->addParam(pConfig->rvM4Slave);
    frame->addParam(pConfig->rv269);
    frame->addParam(pConfig->rv240);
    frame->addParam(pConfig->rv249U1);
    frame->addParam(pConfig->rv249U2);
    frame->addParam(pConfig->rv190);
    frame->addParam(pConfig->rv244);
    emit consoleTxHandler(frame->answToQByteArray()); // Invio a console

    return;
}

void console::consoleSoftwareRevisionsNotify(void)
{
    protoConsole frame(pendingId,UNICODE_FORMAT);
    disconnect(pConfig,SIGNAL(configRevisionSgn()),this,SLOT(consoleSoftwareRevisionsNotify()));

    // Costruisce il frame di risposta
    frame.addParam("0");
    frame.addParam(pConfig->swConf.rvPackage); // Questo √® rilevato dal file di configurazione

    // I campi seguenti sono letti direttamente dalle periferiche
    frame.addParam(pConfig->rvGuiMaster);
    frame.addParam(pConfig->rvGuiSlave);
    frame.addParam(pConfig->rvM4Master);
    frame.addParam(pConfig->rvM4Slave);
    frame.addParam(pConfig->rv269);
    frame.addParam(pConfig->rv240);
    frame.addParam(pConfig->rv249U1);
    frame.addParam(pConfig->rv249U2);
    frame.addParam(pConfig->rv190);
    frame.addParam(pConfig->rv244);

    emit consoleTxHandler(frame.answToQByteArray()); // Invio a console
}

/*
 *  Funzione per ottenere la lista dei Tubi configurati
 *  PARAMETRI: nessuno
 *  FRAME DI RISPOSTA:     <ID LEN %OK PAR0 PAR1 .. PAR30 %>

            Tipo    dato                Valore
    PAR0	Int     Numero Parametri	0 = Nessun Tubo configurato nel sistema, N = N tubi configurati

    PAR1 .. N  Elenco tubi
    PAR N+1 Tubo selezionato

 */
void console::handleGetTubes(protoConsole* answer)
{
    int i;
    QStringList list;
    QDir tubes(_TUBEPATH);

    // Legge il contrenuto della directory
    list = tubes.entryList();

    if(list.count()<3)
    {
        // Nessun file configurato
        emit  consoleTxHandler(answer->cmdToQByteArray("OK 0"));
        return        ;
    }

    // Costruisce la risposta scandendo i nomi dei tubi configurati
    answer->addParam(QString("%1").arg(list.count()-2));
    for(i=2; i< list.count(); i++)  answer->addParam(list.at(i));

    // Aggiunge il tubo corrrentemente selezionato
    answer->addParam(pConfig->userCnf.tubeFileName);
    emit  consoleTxHandler(answer->cmdToQByteArray("OK"));

    return;
}

/*
 *  Funzione per selezionare un Tubo configurato.
 *  Attenzione, la funzione modifica il file di configurazione syscfg.cnf
 *  Se il Tubo esiste, il sistema caricher√  i dati del Tubo in memoria.
 *  PARAMETRI: PAR0 = Nome File
 *  FRAME DI RISPOSTA:     <ID LEN %OK/NOK%>
 */
void console::handleSelectTube(QString tubeName, protoConsole* answer)
{
    int i;
    QDir tubes(_TUBEPATH);

    if(tubes.exists(tubeName)==FALSE)
    {
        // Il Tubo richiesto non √® valido
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK"));
        return;
    }

    // Procede con la selezione del Tubo. Per fare ci√≤ il sistema
    // deve prima modificare il nome del file di configurazione nella configurazione di sistema.
    // Successivamente deve rileggere la configurazione del Tubo.

    if(pConfig->userCnf.tubeFileName==tubeName)
    {
        // Termina perch√® il file √® gi√  quello selezionato
        emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
        return;
    }

    // Modifica e salvataggio nuova configurazione
    pConfig->userCnf.tubeFileName = tubeName;
    pConfig->saveUserCfg();

    // Rilettura file di configurazione del Tubo
    QString tubeDir = QString(_TUBEPATH) + QString("/") + pConfig->userCnf.tubeFileName + QString("/");

    pConfig->generator_configured = pGeneratore->openTube(tubeDir);

    if(pConfig->generator_configured == FALSE)
    {
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK"));
        return;
    }
    else
    {
        emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
        return;
    }


}

/*
 *  Funzione per salvare i dati in un file di configurazione con nome.
 *  ATTENZIONE: il contenuto del Tubo di pari nome verr√  cancellato e sostituito
 *
 *  PARAMETRI: PAR0 = Nome Tubo
 *  FRAME DI RISPOSTA:     <ID LEN %OK/NOK%>
 */
void console::handleStoreTube(QString nomeTubo, protoConsole* answer)
{
    if(nomeTubo.contains("TEMPLATE")){
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK 2"));
        return;
    }
    if(pGeneratore->saveTube(nomeTubo)==TRUE)
    {
        emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
        return;
    }
    else
    {
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK 3"));
        return;
    }

}

/*
 *  Funzione per salvare i dati nel file Tubo correntemente selezionato.
 *  ATTENZIONE: il contenuto del Tubo verr√  cancellato e sostituito (rigenerato)
 *
 *  La funzione richiama la versione con il parametro <nome file> passandogli il
 *  il nome del Tubo configurato in sysCfg.cnf
 *  PARAMETRI: -
 *  FRAME DI RISPOSTA:     <ID LEN %OK/NOK%>
 */
void console::handleStoreTube(protoConsole* answer)
{
    handleStoreTube(pConfig->userCnf.tubeFileName, answer);
    return;
}

/*  <ID LEN %GetKvInfo nome_file%>
 *  Funzione per richiedere info sulla calibrazione corrente dei Kv
 *
    Parametri: NESSUNO
    Risposta:    <ID LEN %OK PAR0 PAR1 .. PAR30 %>
    PARAMETRI	Tipo    dato        Valore
    PAR0        String	Nome Tubo	Tubo correntemente selezionato
    PAR1        String  Anodo       Anodo utilizzato per le sequenze

    PAR2        Int     VDAC(20kV)	Se il valore √® 0 significa che il KVI non √® attivo
    PAR3        Int     VDAC(21kV)	"
    ...			"
    PAR31       Int     VDAC(49kV)	"
 *
 */
void console::handleGetKvInfo(protoConsole* answer)
{
    int i;



    answer->addParam(pGeneratore->getKvCalibAnode()); // Aggiunge l'anodo che verr√  utilizzato

    // Carica i dati richiesti
    for(i=0; i<_MAX_KV-_MIN_KV+1; i++)
    {
        if(pGeneratore->tube[i].vRef.enable)
            answer->addParam(QString("%1").arg(pGeneratore->tube[i].vRef.Vdac));
        else
            answer->addParam(QString("0"));
    }


    // Ricarica in memoria quelli originali al momento selezionati
    emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
    return;
}

/* ATTENZIONE: QUESTA FUNZIONE E' ATTIVA SOLO IN CALIBRAZIONE KV!!
 * <ID LEN %SetKvVdacData PAR0 .. PAR29%>
 * Funzione per l'impostazionedei dati di calibrazione kV
 * in memoria, per il Tubo corrente
 *
    PARAMETRI:	Tipo dato	Valore      Note
    PAR0        Int         VDAC(20kV)	Se il valore √® 0 significa che il KVI non √® attivo
    ...			"
    PAR29       Int         VDAC(49kV)	"

    Frame di risposta:    <ID LEN %OK/NOK%>
 *
 */
void console::handleSetKvVdacData(protoConsole* frame, protoConsole* answer)
{
    int i;
    int val;
    int prev=0;


    // Prima di modificare il contenuto della memoria
    // vengono validati i dati ma solo per i kv consentiti
    for(i=0; i<_MAX_KV-_MIN_KV+1; i++)
    {
        val=frame->parametri[i].toInt();

        // Il valore massimo √® 4095
        if(val>4095) val=0; // Annulla per valori eccessivi

        // I valori di calibrazione devono essere crescenti (oppure 0)
        if((val!=0))
        {
            if(val<=prev)
            {
                emit  consoleTxHandler(answer->cmdToQByteArray(QString("NOK %1").arg(i)));
                return;
            }
            prev = val;
        }
    }

    // Procede con il salvataggio in memoria dei nuovi dati
    for(i=0; i<_MAX_KV-_MIN_KV+1; i++)
    {
        val=frame->parametri[i].toInt();
        if(val==0) pGeneratore->tube[i].vRef.enable = FALSE;
        pGeneratore->tube[i].vRef.Vdac = val;
    }
    emit  consoleTxHandler(answer->cmdToQByteArray("OK 0"));
    return;
}

/*
 * <ID LEN %SetKvMonitorData PAR0 .. PAR29%>
 * Funzione per l'impostazionedei dati di calibrazione della lettura dei kV
 *
 *
    PARAMETRI:	Tipo dato	Valore      Note
    PAR0        Int         VDAC(20kV)	Se il valore √® 0 significa che il valore corrente non va modificato
    ...			"
    PAR29       Int         VDAC(49kV)	"

    Frame di risposta:    <ID LEN %OK/NOK%>
 *  OBSOLETO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
void console::handleSetKvMonitorData(protoConsole* frame, protoConsole* answer)
{
    int i;
    int val;
    int prev=0;

    // Solo in calibrazione KV √® consentito
    if(xSequence.workingMode!=_EXPOSURE_MODE_CALIB_MODE_KV)
    {
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK"));
        return;
    }

    // Prima di modificare il contenuto della memoria
    // vengono validati i dati.
    for(i=0; i<_MAX_KV-_MIN_KV+1; i++)
    {
        val=frame->parametri[i].toInt();

        // Il valore massimo √® 4095
        if(val>4095) val=0;

        // I valori di calibrazione devono essere crescenti (oppure 0)
        if((val!=0))
        {
            if(val<=prev)
            {
                emit  consoleTxHandler(answer->cmdToQByteArray("NOK"));
                return;
            }
            prev = val;
        }
    }

    // Procede con il salvataggio in memoria dei nuovi dati
    for(i=0; i<_MAX_KV-_MIN_KV+1; i++)
    {
        val=frame->parametri[i].toInt();
        if(val==0) continue;
        pGeneratore->tube[i].vRef.Vread = val;
    }

    emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
    return;
}


/*
 *  Questo comando permette di calibrare la rilettura dei KV
 *  il comando deve essere utilizzato solo durante la calibrazione dei KV
 *  param[0] = k;
 *  param[1] = cost;
 *  kV = RAW * k + cost
 */
void console::handleSetCalibKvRead(protoConsole* frame, protoConsole* answer)
{


    // Imposta i nuovi coefficienti
    pGeneratore->genCnf.pcb190.kV_CALIB = (unsigned short) (frame->parametri[0].toFloat() * 1000);
    pGeneratore->genCnf.pcb190.kV_OFS = (short) (frame->parametri[1].toFloat() * 1000);

    // Salva i coefficienti
    pGeneratore->saveTubeKvReadCalibrationFile();

    // Aggiorna la configurazione
    pConfig->updatePCB190();

    emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
    return;
}
/*
 *	<ID LEN %SetKvRxData PAR0 PAR1 PAR2 PAR3 PAR4 PAR5 PAR6%>
    PARAMETRI:	Tipo        dato        Valore
    PAR0        Stringa     Filtro      Rh/Al/Ag/Us
    PAR1        Int         VDAC        [1:4095]
    PAR2        Int         KVI         Indica i KVI nominali attesi
    PAR3        Int         IDAC        [1:4095]
    PAR4        Int         Inom        Valore corrente in mA attesa
    PAR5        Int         mAs         mAs richiesti dall'esposizione

    RISPOSTA:    <ID LEN %OK/NOK%>

 *
 */
bool console::handleSetKvRxData(protoConsole* frame, protoConsole* answer)
{

        // In ogni caso toglie la validazione a quanto c'√® gi√  in memoria
        kvCalibData.validated = FALSE;

        // Solo in calibrazione KV √® consentito
        if(xSequence.workingMode!=_EXPOSURE_MODE_CALIB_MODE_KV) return FALSE;

        // Imposta l'anodo usato per la sequenza e il fuoco grande
        pGeneratore->setFuoco(pGeneratore->getKvCalibAnode());
        pGeneratore->setFuoco(Generatore::FUOCO_LARGE);
        if(pGeneratore->updateFuoco()==FALSE) return FALSE;
        kvCalibData.anodo = pGeneratore->getKvCalibAnode();

        // Verifica e selezione filtro
        if(pCollimatore->setFilterTag(frame->parametri[0])==FALSE) return FALSE;
        if(pCollimatore->setFiltro()==FALSE) return FALSE;
        kvCalibData.filtro = frame->parametri[0];

        // Valore analogico tensione
        kvCalibData.Vdac = frame->parametri[1].toInt();
        if((kvCalibData.Vdac == 0) || (kvCalibData.Vdac > 4095) ) return FALSE;

        // Valore atteso tensione
        kvCalibData.Vnom = frame->parametri[2].toInt();
        if((kvCalibData.Vnom<_MIN_KV) ||(kvCalibData.Vnom>_MAX_KV)) return FALSE;

        // La corrente anodica viene derivata dal template se non viene passata da parametro
        if(frame->parametri.size()>3){

            // Valore analogico corrente anodica
            int idac = frame->parametri[3].toInt();
            if(idac > pGeneratore->genCnf.pcb190.IFIL_MAX_SET) return false;
            if(idac < pGeneratore->genCnf.filData.IFILdac) return false;
            kvCalibData.Idac = idac;

            // Valore atteso corrente anodica
            kvCalibData.Inom = frame->parametri[4].toInt();
            if((kvCalibData.Inom==0) ||(kvCalibData.Inom>199)) return FALSE;

        }else{
            // Valore della corrente Idac viene caricata dal Tubo stesso
            if(pGeneratore->getIdacForKvCalibration(kvCalibData.Vnom, kvCalibData.anodo, &kvCalibData.Idac, &kvCalibData.Inom)==false) return false;

        }

        // Valore mAs da utilizzare durante la sequenza
        kvCalibData.mAs  = 640;

        // Impostazione scambio primari
        kvCalibData.SWA = pGeneratore->tube[kvCalibData.Vnom-_MIN_KV].vRef.SWA;
        kvCalibData.SWB = pGeneratore->tube[kvCalibData.Vnom-_MIN_KV].vRef.SWB;

        // Dati validati e caricati in memoria
        kvCalibData.validated = TRUE;


        return TRUE;

}

/*
 * <ID LEN %SetIaCalibMode%>
 * Funzione per l'attivazione della modalit√  di calibrazione
 * della corrente Anodica
 *
 *  PARAMETRI:	Nessuno
 *  Frame di risposta:    <ID LEN %OK/NOK%>
 *
 */
bool console::handleSetIaCalibMode(void)
{
    // Impostazione modalit√  OPERATIVA
    xSequence.workingMode = _EXPOSURE_MODE_CALIB_MODE_IA;
    ApplicationDatabase.setData(_DB_EXPOSURE_MODE,(unsigned char) xSequence.workingMode);

    // Accende simbolo di calibrazione in corso
    // <TBD> impostare il simbolo relativo alla calibrazione in corso
    //pagina0->setCalibOn(TRUE);

    // Imposta la Modalit√  di funzionamento del generatore
    pGeneratore->tomoMode=FALSE;

    // Annulla eventuali allarmi PAD
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_PAD,0);

    return TRUE;
}

/*
 *  Stringa di comando	<ID LEN %GetIaInfo PAR0%>
    PARAMETRI	Tipo	Valore          Note
    PAR0        Int     KVI             Correnti associate a KVI

    Frame di risposta: <ID LEN %OK PAR0 [BLOCCO WG] [BLOCCO WP] [BLOCCO MoG] [BLOCCO MoP] %>
    PARAMETRI	Tipo        Valore      Note
    PAR0        String      Nome Tubo	Tubo correntemente selezionato
        [BLOCCO DATI RELATIVI AL FUOCO WG]
    ...         String      "WG"        Tag di riconoscimento sezione
    ...         Int         N-Correnti	Numero totale correnti da calibrare per WG
                                        Se 0, passare alla sezione successiva
    ...         Int         IAN-1       Valore nominale corrente [1:200]
    ...         Int         IANC-1      Valore  calibrato [1:200]
    ...         Int         IDAC-1      Valore da calibrare [1:4095]
                Float       Derivata
    ...         Int         IAN-2       Valore nominale corrente [1:200]
    ...         Int         IANC-2      Valore  calibrato [1:200]
    ...         Int         IDAC-2      Valore da calibrare [1:4095]
    ...         Int         IAN-...n	Valore nominale corrente [1:200]
    ...         Int         IANC-n      Valore  calibrato [1:200]
    ...         Int         IDAC-...n	Valore da calibrare [1:4095]
        [BLOCCO DATI RELATIVI AL FUOCO WP]
    ...         String      "WP"        Tag di riconoscimento sezione
    ...
        [BLOCCO DATI RELATIVI AL FUOCO Mo]
    ...         String      "MoG"       Tag di riconoscimento sezione
    ...
        [BLOCCO DATI RELATIVI AL FUOCO MoP]
    ...         String      "MoP"       Tag di riconoscimento sezione
    ...
    ______________________________________________________________________________
    La funzione restituisce tutto il set di correnti da calibrare per ogni
    fuoco configurato nel tubo in esame
 *
 */
void console::handleGetIaInfo(protoConsole* frame, protoConsole* answer)
{
    int i,ii,n;
    if(frame->parametri.size()!=1){
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK 1"));
        return;
    }

    int KVI = frame->parametri[0].toInt() -_MIN_KV;
    if((KVI>_MAX_KV-_MIN_KV)||(KVI<0))
    {
        emit  consoleTxHandler(answer->cmdToQByteArray("NOK 3"));
        return;
    }

    // Blocco dati WG
    answer->addParam(QString("WG"));
    n=0;
    ii = answer->parametri.size();
    answer->addParam(QString("%1").arg(n));
    for(i=0; i<pGeneratore->tube[KVI].iTab.count(); i++)
    {
        if((pGeneratore->tube[KVI].iTab[i].anode=="W") && (pGeneratore->tube[KVI].iTab[i].fsize == Generatore::FUOCO_LARGE))
        {
            n++;
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].In));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].INcalib));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].Idac));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].derivata));
        }
    }
    answer->parametri[ii] = QString("%1").arg(n);

    // Blocco dati WP
    answer->addParam(QString("WP"));
    n=0;
    ii = answer->parametri.size();
    answer->addParam(QString("%1").arg(n));
    for(i=0; i<pGeneratore->tube[KVI].iTab.count(); i++)
    {
        if((pGeneratore->tube[KVI].iTab[i].anode=="W") && (pGeneratore->tube[KVI].iTab[i].fsize == Generatore::FUOCO_SMALL))
        {
            n++;
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].In));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].INcalib));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].Idac));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].derivata));
        }
    }
    answer->parametri[ii] = QString("%1").arg(n);

    // Blocco dati MoG
    answer->addParam(QString("MoG"));
    n=0;
    ii = answer->parametri.size();
    answer->addParam(QString("%1").arg(n));
    for(i=0; i<pGeneratore->tube[KVI].iTab.count(); i++)
    {
        if((pGeneratore->tube[KVI].iTab[i].anode=="Mo") && (pGeneratore->tube[KVI].iTab[i].fsize == Generatore::FUOCO_LARGE))
        {
            n++;
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].In));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].INcalib));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].Idac));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].derivata));
        }
    }
    answer->parametri[ii] = QString("%1").arg(n);

    // Blocco dati MoP
    answer->addParam(QString("MoP"));
    n=0;
    ii = answer->parametri.size();
    answer->addParam(QString("%1").arg(n));
    for(i=0; i<pGeneratore->tube[KVI].iTab.count(); i++)
    {
        if((pGeneratore->tube[KVI].iTab[i].anode=="Mo") && (pGeneratore->tube[KVI].iTab[i].fsize == Generatore::FUOCO_SMALL))
        {
            n++;
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].In));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].INcalib));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].Idac));
            answer->addParam(QString("%1").arg(pGeneratore->tube[KVI].iTab[i].derivata));
        }
    }
    answer->parametri[ii] = QString("%1").arg(n);

    emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
    return;
}

/*
 *  Stringa di comando:	<ID LEN %GetIaTomoInfo%>
    PARAMETRI:	Nessuno
    Frame di risposta: <ID LEN %OK PAR0 [BLOCCO WG] [BLOCCO MoG]%>
    PARAMETRI:      Tipo            Valore          Note
    PAR0            String          Nome Tubo       Tubo correntemente selezionato
        [BLOCCO DATI RELATIVI AL FUOCO WG]
    PAR1            String          "WG"            Tag di riconoscimento sezione
    ...             Int              N              Numero correnti definite (coppie di parametri)
                                                    - 0: il fuoco non √® configurato
    ...             Int             IAN(20KV)       Valore nominale corrente [1:200] per 20KV
    ...             Int             IDAC(20KV)      Valore da calibrare [1:4095] per 49KV
                    Float           Derivata
    ...             ...             ...
    ...             Int             IAN(49KV)       Valore nominale corrente [1:200] per 49KV
    ...             Int             IDAC(49KV)      Valore da calibrare [1:4095] per 49KV

        [BLOCCO DATI RELATIVI AL FUOCO MoG]
    ...             String          "MoG"           Tag di riconoscimento sezione
    ...             Int              N              Numero correnti definite (coppie di parametri)
                                                    - 0: il fuoco non √® configurato
    ...             Int             IAN(20KV)       Valore nominale corrente [1:200] per 20KV
    ...             Int             IDAC(20KV)      Valore da calibrare [1:4095] per 49KV
    ...             ...             ...
    ...             Int             IAN(49KV)       Valore nominale corrente [1:200] per 49KV
    ...             Int             IDAC(49KV)      Valore da calibrare [1:4095] per 49KV

    ______________________________________________________________________________
    La funzione restituisce tutto il set di correnti da calibrare per le scansioni
    Tomografiche

 *
 */
void console::handleGetIaTomoInfo(protoConsole* answer)
{
    int i,ii,n;

    // Non Ë possibile effettuare la Tomo senza starter
    if(pConfig->sys.highSpeedStarter == FALSE)  {
        answer->addParam(QString("WG"));
        n=0;
        answer->addParam(QString("%1").arg(n));
        answer->addParam(QString("MoG"));
        answer->addParam(QString("%1").arg(n));
        emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
        return;
    }


    // Blocco dati WG
    answer->addParam(QString("WG"));
    n=0;
    if((pGeneratore->confF1=="W")||(pGeneratore->confF2=="W"))
    {
        n = _MAX_KV-_MIN_KV +1;
        answer->addParam(QString("%1").arg(n));

        for(i=0; i<n; i++)
        {
            if(pGeneratore->tube[i].vRef.enable)
            {
                for(ii=0; ii<pGeneratore->tube[i].tomo.count();ii++)
                {
                    if((pGeneratore->tube[i].tomo[ii].anode=="W") && (pGeneratore->tube[i].tomo[ii].enabled))
                    {
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].In));
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].INcalib));
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].Idac));
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].derivata));
                        break;
                    }
                }
                if(ii==pGeneratore->tube[i].tomo.count())
                {
                    answer->addParam(QString("%1").arg((int) 0));
                    answer->addParam(QString("%1").arg((int) 0));
                    answer->addParam(QString("%1").arg((int) 0));
                    answer->addParam(QString("%1").arg((int) 0));
                }
            }else
            {   // KV non abilitati o calibrati
                answer->addParam(QString("%1").arg((int) 0));
                answer->addParam(QString("%1").arg((int) 0));
                answer->addParam(QString("%1").arg((int) 0));
                answer->addParam(QString("%1").arg((int) 0));
            }
        }
    }else  answer->addParam(QString("%1").arg(n));


    // Blocco dati MoG
    answer->addParam(QString("MoG"));
    n=0;
    if((pGeneratore->confF1=="Mo")||(pGeneratore->confF2=="Mo"))
    {
        n = _MAX_KV-_MIN_KV +1;
        answer->addParam(QString("%1").arg(n));

        for(i=0; i<n; i++)
        {
            if(pGeneratore->tube[i].vRef.enable)
            {
                for(ii=0; ii<pGeneratore->tube[i].tomo.count();ii++)
                {
                    if((pGeneratore->tube[i].tomo[ii].anode=="Mo") && (pGeneratore->tube[i].tomo[ii].enabled))
                    {
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].In));
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].INcalib));
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].Idac));
                        answer->addParam(QString("%1").arg(pGeneratore->tube[i].tomo[ii].derivata));
                        break;
                    }
                }
                if(ii==pGeneratore->tube[i].tomo.count())
                {
                    answer->addParam(QString("%1").arg((int) 0));
                    answer->addParam(QString("%1").arg((int) 0));
                }
            }else
            {   // KV non abilitati o calibrati
                answer->addParam(QString("%1").arg((int) 0));
                answer->addParam(QString("%1").arg((int) 0));
            }
        }
    }else  answer->addParam(QString("%1").arg(n));


    emit  consoleTxHandler(answer->cmdToQByteArray("OK"));
    return;

}

/*
 *  Stringa di comando:	<ID LEN %SetIdacData  PAR0 [WG][WP][MoG][MoP]%>
    PARAMETRI:          Tipo    	Valore              Note
    PAR0                Int         KVI                 KVI a cui i dati si riferiscono
    PAR1                String      "WG/WP/MoG/MoP"     Tag di riconoscimento fuoco da modificare
    ...                 Int         N                   Numero totale correnti da calibrare
                                                        0 se nessuna corrente √® presente su questo fuoco
    ...                 Int         IAN-1               Valore nominale corrente [1:200]
    ...                 Int         IANC-1           Valore nominale corrente [1:200]

    ...                 Int         IDAC-1          Valore da calibrare [1:4095]
    ...
    ...                 Int         IAN-...n        Valore nominale corrente [1:200]
    ...                 Int         IDAC-...n       Valore da calibrare [1:4095]


    Frame di risposta:  <ID LEN %OK/NOK%>
    ______________________________________________________________________________
    La funzione modifica il contenuto dei valori di calibrazione delle correnti
    utilizzate.
    La CPU effettua un controllo sulla coerenza dei dati: la funzione infatti
    non pu√≤ modificare i valori nominali e gli stessi devono corrispondere
    a quelli tabellati in memoria.

 */
bool console::handleSetIdacData(protoConsole* frame,  bool modifica)
{
    int i,ii,iii,n;
    int in,idac,ine;
    QString AnodoFuoco;
    QString anodo;
    unsigned char fsize;

    // Controllo sui parametri
    if(frame->parametri.size()<3) return FALSE;

    // Legge i KV corrispondenti alla modifica
    int KVI = frame->parametri[0].toInt() -_MIN_KV;
    if(KVI>_MAX_KV-_MIN_KV) return FALSE;

    i = 1;

    // Legge la combinazione Anodo/Fuoco
    AnodoFuoco = frame->parametri[i++];
    if(AnodoFuoco=="WG")
    {
        anodo = "W";
        fsize = Generatore::FUOCO_LARGE;

    }else if(AnodoFuoco=="WP")
    {
        anodo = "W";
        fsize = Generatore::FUOCO_SMALL;

    }else if(AnodoFuoco=="MoG")
    {
        anodo = "Mo";
        fsize = Generatore::FUOCO_LARGE;

    }else if(AnodoFuoco=="MoP")
    {
        anodo = "Mo";
        fsize = Generatore::FUOCO_SMALL;
    }else return FALSE;

    // Controllo sul numero dei parametri aspettati
    n = frame->parametri[i++].toInt();
    if(frame->parametri.size()!= 3 + n*3) return FALSE;

    if(n!=0)
    {
        // Passa tutte le correnti richieste
        for(iii=0; iii<n; iii++)
        {
            // Legge la coppia di correnti
           in = frame->parametri[i++].toInt();
           ine = frame->parametri[i++].toInt();
           idac = frame->parametri[i++].toInt();

           // Controlla valori
           if(in>200) return FALSE;
           if(idac>4095) return FALSE;

           // Cerca la corrispondente In da calibrare
           for(ii=0; ii<pGeneratore->tube[KVI].iTab.count();ii++)
               if((pGeneratore->tube[KVI].iTab[ii].anode==anodo)&&(pGeneratore->tube[KVI].iTab[ii].fsize == fsize))
                   if(pGeneratore->tube[KVI].iTab[ii].In == in) break;
           if(ii==pGeneratore->tube[KVI].iTab.count()) return FALSE; // Nessuna (In) corrispondente
           if(modifica)
           {
               // Procede con la modifica
               pGeneratore->tube[KVI].iTab[ii].In = in;
               pGeneratore->tube[KVI].iTab[ii].Idac = idac;
               pGeneratore->tube[KVI].iTab[ii].INcalib = ine;
           }
        }
    }


    return TRUE;
}

/*
    COMANDO: SetIdacTomoData  (solo in calibrazione IA)
    Frame di risposta: <ID LEN %OK/NOK%>
        OK	Dato corretto
        NOK	Errore formato comando

*/
bool console::handleSetIdacTomoData(protoConsole* frame, protoConsole* answer)
{
    int kv,parOffset, i, ii;
    int In,Idac,Ine;
    bool check;
    QString anodoStr;


    // Controllo sulla lunghezza dei parametri
    if(frame->parametri.size()!=91) return FALSE;

    // Controlla quale anodo √® richiesto
    if((frame->parametri[0]!="W") && (frame->parametri[0]!="Mo")) return FALSE;

    // Controlla se l'anodo e' configurato
    if(!pGeneratore->isValidAnode(frame->parametri[0])) return FALSE;

    // Controllo parametri
    anodoStr=frame->parametri[0];
    parOffset = 1;
    for(i=0; i<=_MAX_KV-_MIN_KV; i++)
    {
        In = frame->parametri[parOffset + i*3].toInt();
        Ine = frame->parametri[parOffset + i*3+1].toInt();
        Idac = frame->parametri[parOffset + i*3+2].toInt();
        if(In==0) continue;
        if(Idac==0) return FALSE;
        if(Idac>4095) return FALSE;

        // Controllo che In sia configurato per il tubo in oggetto
        check = FALSE;
        for(ii=0; ii<pGeneratore->tube[i].tomo.count();ii++)
        {
            if((pGeneratore->tube[i].tomo[ii].anode==anodoStr)&&(pGeneratore->tube[i].tomo[ii].In == In))
            {
                check=TRUE;
                break;
            }
        }
        if(!check) return FALSE;
    }

    // Modifica parametri
    for(i=0; i<=_MAX_KV-_MIN_KV; i++)
    {
        In = frame->parametri[parOffset + i*3].toInt();
        Ine = frame->parametri[parOffset + i*3+1].toInt();
        Idac = frame->parametri[parOffset + i*3+2].toInt();
        if(In==0) continue;

        // Cerca la corrispondente In da calibrare
        for(ii=0; ii<pGeneratore->tube[i].tomo.count();ii++)
        {
            if((pGeneratore->tube[i].tomo[ii].anode==anodoStr)&&(pGeneratore->tube[i].tomo[ii].In == In))
            {
                pGeneratore->tube[i].tomo[ii].Idac = Idac;
                pGeneratore->tube[i].tomo[ii].INcalib = Ine;
                pGeneratore->tube[i].tomo[ii].enabled = TRUE;
            }
        }
    }

    return TRUE;
}

/*
 *
 *  PAR0	Stringa	Anodo	"W"/"Mo" seleziona tungsteno/Molibdeno
    PAR1	Stringa	Dimensione fuoco	"G"/"P" seleziona fuoco grande/piccolo
    PAR2	Int	KVI	Indica i KVI nominali da utilizzare
    PAR3	Int	IDAC	[1:4095] valore da calibrare
    PAR4	Int	Inom	Valore corrente in mA attesa
    PAR5	Int	mAs	mAs richiesti dall'esposizione

      bool  validated; // I dati sono stati rinfrescati dall'ultima sequenza
      QString anodo;   // Anodo selezionato per la sequenza (W/Mo)
      QString fuoco;   // Filtro selezionato per la sequenza
      int   Vnom;      // Tensione nominale (intera) aspettata
      int   Idac;      // Corrente analogica da utilizzare
      int   Inom;      // Corrente nominale aspettata
      int   mAs;       // mAs da utilizzare durante la sequenza
    }_iACalibData_Str;

 *
*/
bool console::handleSetIaRxData(protoConsole* frame, protoConsole* answer)
{
    // In ogni caso toglie la validazione a quanto c'√® gi√  in memoria
    iACalibData.validated = FALSE;

    // Solo in calibrazione KV √® consentito
    if(xSequence.workingMode!=_EXPOSURE_MODE_CALIB_MODE_IA) return FALSE;

    // Imposta l'anodo usato per la sequenza e il fuoco grande
    if(!pGeneratore->isValidAnode(frame->parametri[0])) return FALSE;
    iACalibData.anodo = frame->parametri[0];

    // Imposta il fuoco da utilizzare
    if((frame->parametri[1]!="P") && (frame->parametri[1]!="G")) return FALSE;
    iACalibData.fuoco = frame->parametri[1];

    // Seleziona subito il fuoco per riscaldarlo
    if(frame->parametri[1]=="P")
    {
        pGeneratore->setFuoco(Generatore::FUOCO_SMALL);
        pGeneratore->setFuoco(iACalibData.anodo);
    }
    else
    {
        pGeneratore->setFuoco(Generatore::FUOCO_LARGE);
        pGeneratore->setFuoco(iACalibData.anodo);
    }
    if(pGeneratore->updateFuoco()==FALSE) return FALSE;

    // Valore nominale tensione: verifica che sia calibrato e disponibile
    if(pGeneratore->isValidKv(frame->parametri[2].toInt())==FALSE) return FALSE;
    iACalibData.Vnom = frame->parametri[2].toInt();

    // Valore analogico corrente anodica: verifica che il valore massimo non ecceda quello previsto.
    int idac = frame->parametri[3].toInt();
    if(idac > pGeneratore->genCnf.pcb190.IFIL_MAX_SET) return false;
    if(idac < pGeneratore->genCnf.filData.IFILdac) return false;
    iACalibData.Idac = idac;

    // Valore atteso corrente anodica
    iACalibData.Inom = frame->parametri[4].toInt();
    if((iACalibData.Inom==0) ||(iACalibData.Inom>200)) return FALSE;

    // Valore mAs da utilizzare durante la sequenza
    iACalibData.mAs  = frame->parametri[5].toInt();

    // Modifica per la Tomo: occorrono almeno 20mAs
    if(iACalibData.mAs<20) iACalibData.mAs=20;
    if((iACalibData.mAs==0) ||(iACalibData.mAs>100)) return FALSE;

    // Dati validati e caricati in memoria
    iACalibData.validated = TRUE;

    return TRUE;
}

/*
 *  Comando di movimento cursori Biopsia:
 *  Parametri: [X,Y,Z,]
 *  Risposta:
 *      -> OK 0 Gi‡ in posizione;
        -> OK 255: comando attivato, completamento differito
        -> NOK 1: numero parametri errato
        -> NOK 2: lateralit‡ non definita o errore attivo
        -> NOK 3: Finestra di errore attiva
        -> NOK 4: BUSY
 *
 */
void console::handleBiopsyExtendedMoveXYZ(protoConsole* frame, protoConsole* answer)
{
    // Controllo sulla dimensione dei parametri
    if(frame->parametri.size() != 3 ) {
        emit consoleTxHandler(answer->answToQByteArray("NOK 1 ERRORE NUMERO DI PARAMETRI: ->  X,Y,Z"));
        return;
    }

    // Movimento a posizione arbitraria: la lateralit‡ deve corrispondere!
    if(pBiopsyExtended->curLatX == _BP_EXT_ASSEX_POSITION_ND){
        emit consoleTxHandler(answer->answToQByteArray("NOK 2 ERRORE LATERALITA' NON ATTIVA"));
        return;
    }

    if(pBiopsy->activationId) {
        emit consoleTxHandler(answer->answToQByteArray("NOK 4 BUSY"));
        return;
    }

    unsigned short X = frame->parametri[0].toUInt(); // dam
    unsigned short Y = frame->parametri[1].toUInt(); // dam
    unsigned short Z = frame->parametri[2].toUInt(); // dam

    int ret = pBiopsyExtended->requestBiopsyMoveXYZ(X,Y,Z,frame->id);
    if(ret < 0) emit consoleTxHandler(answer->answToQByteArray("NOK 3 FINESTRA ERRORE ATTIVO"));
    else if(ret ==0) emit consoleTxHandler(answer->answToQByteArray("OK 0"));
    else emit consoleTxHandler(answer->answToQByteArray("OK 255"));

    return;

}

/*
    Comando di impostazione (con attivazione) della Home:
    Parametro1: [L/C/R] Left / Center / Right
    Parametro2: [0/90/CALIB]Posizionatore o comando di visualizzazione cursore per calibrazione

    Risposta:
        -> OK 0 Gi‡ in posizione;
        -> OK 255: comando attivato, completamento differito
        -> NOK 1: numero parametri errato
        -> NOK 2: parametro non conforme
        -> NOK 3: Finestra di errore attiva o lateralit‡ indefinita
        -> NOK 4: BUSY
 */
void console::handleBiopsyExtendedMoveHome(protoConsole* frame, protoConsole* answer)
{
    if(frame->parametri.size() != 2 ) {
        emit consoleTxHandler(answer->answToQByteArray("NOK 1 ERRORE NUMERO DI PARAMETRI: ->  LAT"));
        return;
    }


    if(pBiopsy->activationId) {
        emit consoleTxHandler(answer->answToQByteArray("NOK 4 BUSY"));
        return;
    }

    unsigned char lat;

    if(frame->parametri[0]=="L") lat = _BP_EXT_ASSEX_POSITION_LEFT;
    else if(frame->parametri[0]=="C") lat = _BP_EXT_ASSEX_POSITION_CENTER;
    else if(frame->parametri[0]=="R") lat = _BP_EXT_ASSEX_POSITION_RIGHT;
    else {
        emit consoleTxHandler(answer->answToQByteArray("NOK 2 ERRORE VALORE PARAMETRO: -> [L/C/R]"));
        return;
    }

    if(frame->parametri[1] == "0"){
        ApplicationDatabase.setData(_DB_BIOP_SHOW_SH, (unsigned char) 0);
    }else if(frame->parametri[1] == "90"){
        ApplicationDatabase.setData(_DB_BIOP_SHOW_SH, (unsigned char) 0);
    }else{
        // Abilita la visualizzazione del cursore
        ApplicationDatabase.setData(_DB_BIOP_SHOW_SH, (unsigned char) 1);
    }

    /*   0: gi‡ in posizione
    *   1: comando accettato ed in esecuzione
    *  -1: Errore attivo
    *  -2: TRX fuori posiizone
    *  -3: ASSE X fuori lateralit‡
    */

    int ret = pBiopsyExtended->requestBiopsyHome(frame->id,lat,frame->parametri[0].toInt() );

    if(ret == -1) emit consoleTxHandler(answer->answToQByteArray("NOK 1 FINESTRA DI ERRORE ATTIVA"));
    else if(ret == -2) emit consoleTxHandler(answer->answToQByteArray("NOK 2 TUBO NON IN SCOUT"));
    else if(ret == -3) emit consoleTxHandler(answer->answToQByteArray("NOK 3 LATERALITA' ASSE-X INDETERMINATA"));
    else if(ret ==0) emit consoleTxHandler(answer->answToQByteArray("OK 0"));
    else emit consoleTxHandler(answer->answToQByteArray("OK 255"));

    return;
}


/*
    Stringa di comando	<ID LEN %SetBiopsyData PAR0 .. PAR7%>
    PARAMETRI:	Tipo    dato                        Valore
    PAR0        Int     Posizione X                 Valore in decimi di millimetro
    PAR1        Int     Posizione Y                 Valore in decimi di millimetro
    PAR2        Int     Posizione Z                 Valore in decimi di millimetro
    PAR3        Int     Posizione Z Limite          Valore in  millimetro
    PAR4        Int     POsizione Z Lesione         Valore in  millimetro
    PAR5    	Int     Lunghezza effettiva Ago     Valore in  millimetro

    PAR6        String	Nome descrittore Accessorio	Descrizione dell'accessorio da utilizzare
    PAR7        blank  SPARE
    PAR8        String	Nome descrittore Ago        Descrizione simbolica Ago utilizzato


    Frame di risposta: <ID LEN %OK/NOK PAR0%>
        OK	Torretta si muove verso XYZ
        NOK	Errore formato comando
        PARAMETRI	Tipo    dato                Valore
        PAR0        Int     Timeout Comando     Indica quanto tempo in secondi dovrebbe impiegare al massimo il posizionamento

     A questo comando seguir√  l'invio di un messaggio asincrono per notificare
     il completamento:
    Stringa di comando	<ID LEN %OK/NOK PAR0%>
    PARAMETRI	Tipo dato	Valore	Note
    OK/NOK	Stringa	Risultato	OK = Posizionamento corretto
    NOK = Errore posizionamento
    PAR0	Stringa	Stringa di errore	Se OK: == "" Nessuna stringa;
    Se NOK: Se l'operazione non √® andata a buon fine (NOK) questa √® la stringa di errore generata dalla CPU

 */
int console::handleSetBiopsyData(protoConsole* frame)
{

     // Check numero parametri
    if(frame->parametri.size()!=9) return -1;

    // Prepara i target di movimento
    unsigned short targetX = frame->parametri[0].toUInt(); // dam
    unsigned short targetY = frame->parametri[1].toUInt(); // dam
    unsigned short targetZ = frame->parametri[2].toUInt(); // dam

    unsigned short Zlimit = (unsigned char) frame->parametri[3].toUInt(); // mm
    unsigned short Zlesione = (unsigned char) frame->parametri[4].toUInt(); // mm
    unsigned short Lago = (unsigned char) frame->parametri[5].toUInt(); // mm
    unsigned short holder = (unsigned char) frame->parametri[6].toUInt();
    // spare frame->parametri[7];
    QString codiceAgo = frame->parametri[8];


    return pBiopsyStandard->setBiopsyData(targetX, targetY, targetZ, // Posizione da raggiungere
                              Zlimit,        // Massima Z calcolata dalla AWS
                              Zlesione,      // Posizione rilevata della lesione
                              Lago,          // Lunghezza dell'ago
                              holder,        // Codice holder utilizzato dalla AWS
                              codiceAgo,     // Nome dellk'accessorio montato da AWS
                              frame->id      // Id del comando richiesto
                              );

}

/*
  Restituisce la corsa massima prima di impattare con ili compressore
 */
void console::handleGetBiopsyZ(protoConsole* answer)
{
    answer->addParam(QString("%1").arg(pBiopsyStandard->maxZ*10));
    emit consoleTxHandler(answer->answToQByteArray());
}


/* 
    Restituisce i seguenti parametri funzionali della Biopsia:
    -> "OK [CONNECTED/DISCONNED] [X_L / X_R / X_C / X_N] [AD_A / AD_B / AD_C / AD_ND] [X,Y,Z,SH]"

 */
void console::handleGetBiopsyExtendedData(protoConsole* frame, protoConsole* answer)
{
    QString stringa = "OK ";


    // Stato della connessione
    if(pBiopsy->connected) stringa += "CONNECTED ";
    else stringa += "DISCONNECTED ";

    // Lateralit‡ rilevata
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
    emit consoleTxHandler(answer->answToQByteArray(stringa));

    return;
}



void console::handleSetBiopsyConfig(protoConsole* frame, protoConsole* answer)
{



    return ;

}
/*
    Stringa di comando	<ID LEN %SetLingua PAR0%>
    PARAMETRI:	Tipo    dato                        Valore
    PAR0        String  Codice stringua lingua

    Frame di risposta:
        <ID LEN %OK 0%>  ->OK
        <ID LEN %NOK 1%>  ->Formato non corretto
        <ID LEN %NOK 2%>  ->Lingua non gestita
 */
int  console::handleSetLingua(protoConsole* frame){

    // Controllo sul formato del comando
    if(frame->parametri.size()!=1) return 1;
    if(pagina_language->isLanguage(frame->parametri[0])==FALSE) return 2;
    ApplicationDatabase.setData(_DB_LINGUA,frame->parametri[0], 0);
    pConfig->userCnf.languageInterface = frame->parametri[0];
    pConfig->saveUserCfg();

    return 0;

}


/*
 *  Comando di attivazione dell'aggiornamento generale del sistema
 */
void  console::handleSetFirmwareUpdate(protoConsole* frame, protoConsole* answer){

    QString errstr;

    // Risponde subito
    //emit consoleTxHandler(answer->answToQByteArray("OK 0"));

    // Usa la funzione diretta con timeout perch√® a seguire vi sono delle system
    consoleSocketTcp->txData(answer->answToQByteArray("OK 0"),100000);

    // Connessione del segnale di fine aggiornamento della funzione sysUpdate
    // con lo slot del modulo toConsole per l'invio del comando asincrono di fine
    // aggiornamento, comunque sia andato.
    connect(pConfig,SIGNAL(sysUpdateCompletedSgn(bool,QString)), pToConsole,SLOT(fineSystemUpdate(bool,QString)),Qt::UniqueConnection);
    connect(pConfig,SIGNAL(sysUpdateCompletedSgn(bool,QString)),pConfig,SLOT(localInstallMonitorCompleted(bool,QString)),Qt::UniqueConnection);
    pConfig->sysUpdate(frame->parametri[0]);
    return;

}


void  console::handleSetSystemBackup(protoConsole* frame, protoConsole* answer){

    // Risponde subito
    emit consoleTxHandler(answer->answToQByteArray("OK 255"));
    //consoleSocketTcp->txData(answer->answToQByteArray("OK 255"));
    pConfig->sysBackup(TRUE,frame->parametri[0],frame->id);
    return;

}

void  console::handleSetSystemRestore(protoConsole* frame, protoConsole* answer){

    emit consoleTxHandler(answer->answToQByteArray("OK 255"));
    pConfig->sysRestore(TRUE,frame->parametri[0],frame->id);
    return;

}

void  console::handleSetUserDirClean(protoConsole* frame, protoConsole* answer){

    // Risponde subito
    emit consoleTxHandler(answer->answToQByteArray("OK 255"));
    pConfig->userDirClean(TRUE,frame->id);
    return;

}
void console::protocolAnswer(protoConsole* answer, QString cmd){
    emit consoleTxHandler(answer->answToQByteArray(cmd));
}

void console::handleCompressorActivation(unsigned char mode){
    unsigned char data[5];

    if(mode==1){
        pGuiMcc->sendFrame(MCC_CMD_CMP_AUTO,1,data,0);
    }else{
        data[0]=100;
        pGuiMcc->sendFrame(MCC_CMD_PAD_UP,1,data,1);
    }

}

void console::handleOutputPulse(QString nout, QString time){

    io->setSpareOutputPulse(nout.toInt(), time.toLong());
    //io->setSpareOutputClr(nout.toInt());
}




/*_______________________________________________________________
 *
 *  Funzione di attivazione sequenza 2D/3D di test
 *  La tipologia di sequenza viene impostata tramite il comando
 *  SET_MODE
  _______________________________________________________________*/
void console::RxShotModeSequence(bool useDetector){
    QByteArray ret;
    unsigned char code;
    unsigned char mcc_cmd;


    // Determina quale sia il comando che verr√  eseguito
    if(xSequence.isAEC) mcc_cmd = MCC_CMD_RAGGI_AEC;
    else mcc_cmd = MCC_CMD_RAGGI_STD;

    // Effettua la validazione dei dati del generatore
    code = pGeneratore->validateData();
    if(code)
    {
        ret.clear();
        ret.append(code);
        ret.append(( char) 0);
        ret.append(( char) 0);
        guiNotify(1, mcc_cmd,ret);
        return;
    }

    // Determina se si tratti di 2D o 3D
    if(xSequence.is2D) RxShot2DSequence(useDetector);
    else RxShot3DSequence();
}

/*_______________________________________________________________
 *
 *  Funzione di attivazione sequenza 2D in modalit√  TEST
 *
 */
void console::RxShot2DSequence(bool useDetector)
{
    QByteArray ret;


    // Crea il comando da mandare al driver
    unsigned char data[15];

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDac&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDac>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Starter Alta Velocit√  sempre durante il test
    if(pGeneratore->starterHS) data[7]|=4;

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;

    data[13]=0; // No sblocco compressore

    // Se non deve essere usato il detector allora usa la funzione di TEST
    if(useDetector==FALSE) data[14]=_SHOT_WITHOUT_DETECTOR; // Uso SENZA DETECTOR!
    else data[14]=_SHOT_WITH_DETECTOR; // Uso DETECTOR

    if(xSequence.isAEC)
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AEC,1,data,14)==FALSE)
        {
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_AEC,ret);
            return;
        }
    }else
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_STD,1,data,15)==FALSE)
        {
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_STD,ret);
            return;
        }
    }

    handleSetXrayLamp("ON");
    handleSetXraySym(true);
    return;

}


/*_________________________________________________________________________
 *
 *  Funzione di attivazione Tomo in modalit√  di test:
 *  Questa funzione permette di attivare la Tomo con le seguenti
 *  opzioni:
 *  + W, N, I
 *  + AEC/NOAEC
 *  + STATIC/MOVING
 *
 *  Utilizzare il comando SetTestTomo() per attivare la modalit√ 
 *  Utilizzare SetMode() per le opzioni relative:
 *  - TN/TW/TI, AEC/NOAEC, [STATIC]
 *
 */
void console:: RxShot3DSequence(void)
{

    unsigned char data[19];
    QByteArray ret;

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDacTomo&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDacTomo>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta Velocit√ 

    data[8] =  0; // Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;


    // Definizione delle modalit√  di azione del braccio
    if(xSequence.isTomoN)
    {
        if(xSequence.isTomoMoving==FALSE) data[15] = _TOMO_MODE_STATIC;  //  Modo static
        else data[15] = _TOMO_MODE_NARROW;  //  Modo Narrow

        data[13] = pConfig->trxConfig.tomo.n.pre_samples;
        data[14] = pConfig->trxConfig.tomo.n.samples;
        data[16] = pConfig->trxConfig.tomo.n.skip_samples;
        xSequence.n_pulses = pConfig->trxConfig.tomo.n.samples;
    }else if(xSequence.isTomoI)
    {
        if(xSequence.isTomoMoving==FALSE) data[15] = _TOMO_MODE_STATIC;  //  Modo static
        else data[15] = _TOMO_MODE_INTERMEDIATE;  //  Modo Narrow

        data[13] = pConfig->trxConfig.tomo.i.pre_samples;
        data[14] = pConfig->trxConfig.tomo.i.samples;
        data[16] = pConfig->trxConfig.tomo.i.skip_samples;
        xSequence.n_pulses = pConfig->trxConfig.tomo.n.samples;

    }else
    {
        if(xSequence.isTomoMoving==FALSE) data[15] = _TOMO_MODE_STATIC;  //  Modo static
        else data[15] = _TOMO_MODE_WIDE;  //  Modo Narrow

        data[13] = pConfig->trxConfig.tomo.w.pre_samples;
        data[14] = pConfig->trxConfig.tomo.w.samples;
        data[16] = pConfig->trxConfig.tomo.w.skip_samples;
        xSequence.n_pulses = pConfig->trxConfig.tomo.n.samples;

    }

    // Nessun sblocco compressore
    data[17]=0;

    // No Deadmen
    data[18]=0;


    if(xSequence.isAEC)
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_AEC_TOMO,1,data,19)==FALSE)
        {
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, ERROR_MCC_COMMAND,TRUE); // Self resetting
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_AEC_TOMO,ret);
            return;
        }
    }
    else
    {
        if(pGuiMcc->sendFrame(MCC_CMD_RAGGI_TOMO,1,data,19)==FALSE)
        {
            ret.clear();
            ret.append(1);
            ret.append(ERROR_MCC_COMMAND);
            guiNotify(1, MCC_CMD_RAGGI_TOMO,ret);
            return;
        }
    }

    handleSetXrayLamp("ON");
    handleSetXraySym(true);
    return;

}

//_____________________________________________________________
// Gestione attivazione segnale di raggi in corso
void console::handleSetXraySym(bool stat){
    if(stat) ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 1,0);
    else ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 0,0);
}

//______________________________________________________________________________________________________________________
void console::handlePowerOff(void){
    pConfig->executePoweroff(20);
    return;
}
