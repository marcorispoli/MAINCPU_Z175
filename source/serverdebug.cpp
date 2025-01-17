#include "application.h"
#include "appinclude.h"
#include "globvar.h"

#define PROMPTH "IRS:>"
bool trx_loop=false;
bool trx_loop_stat = false;

QHostAddress serverDebug::setIpAddress( int val){
    int size = TcpIpServer::hostAddress().toString().length();
    return QHostAddress(QString("%1.%2").arg(TcpIpServer::hostAddress().toString().left(size-2)).arg(val)) ;
}

bool serverDebug::isIp(int val){
    if(TcpIpServer::hostAddress().toString().right(1).toInt()==val) return true;
    return false;
}

void serverDebug::activateConnections(void) {
    QObject::connect(serviceTcp,SIGNAL(rxData(QByteArray)),this,SLOT(serviceRxHandler(QByteArray)),Qt::UniqueConnection);
    QObject::connect(serviceTcp,SIGNAL(serverConnection(bool)),this,SLOT(notificheConnectionHandler(bool)),Qt::UniqueConnection);
    serviceTcp->Start(_LOCAL_SERVICE_PORT);

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
serverDebug::serverDebug(void) :
    QObject(0)
{
    // Creazione del socket di comunicazione esterna con la Console
    serviceTcp = new TcpIpServer();

    lastValidFrame.clear();
    cmdGroup.clear();


    _deviceRegisterItem item;

    deviceRegisterList.clear();

    //  --------   REGISTRI RELATIVI AL COLLIMATORE  -------------
    item.tag = "COLLI-TEMP";
    item.comment = "Temperatura della cuffia";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x38+5;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-GONIOREL";
    item.comment = "Angolo relativo TRX - ARM";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x5A;
    item.data_type = _8bit;
    item.sign_type = _SIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-TRX";
    item.comment = "Angolo assoluto TRX (0.025�/unit)";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x5E;
    item.data_type = _16bit;
    item.sign_type = _SIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-ARM";
    item.comment = "Angolo di confronto ARM (0.025�/unit)";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x60;
    item.data_type = _16bit;
    item.sign_type = _SIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-R-SENS";
    item.comment = "Posizione lama destra";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x144;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-L-SENS";
    item.comment = "Posizione lama sinistra";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x145;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-T-SENS";
    item.comment = "Posizione trapezio";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x146;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-L-ENCODER";
    item.comment = "Encoder virtuale lama sinistra";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x159;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-R-ENCODER";
    item.comment = "Encoder virtuale lama destra";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x15A;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-T-ENCODER";
    item.comment = "Encoder virtuale lama trapezio";
    item.deviceId = _CODEID_PCB249U1;
    item.address = 0x15B;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-B-SENS";
    item.comment = "Posizione lama posteriore";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x55;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-F-SENS";
    item.comment = "Posizione lama posteriore";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x57;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-TARGET-FRONT";
    item.comment = "Target posizione lama frontale";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x5C;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-TARGET-BACK";
    item.comment = "Target posizione lama posteriore";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x5D;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-FILTRO-SENS";
    item.comment = "Posizione filtro";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x66;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-MIRROR-COUNT";
    item.comment = "Step posizionamento specchio";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x4f;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-MIRROR-STAT";
    item.comment = "Registro di stato specchio";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x4e;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "COLLI-LAMP-STAT";
    item.comment = "Registro di stato luce";
    item.deviceId = _CODEID_PCB249U2;
    item.address = 0x6C;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    //  --------   REGISTRI RELATIVI AL COMPRESSORE  -------------
    item.tag = "CMP-SYS0";
    item.comment = "Registro SYS_FLAGS0";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x20;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-SYS1";
    item.comment = "Registro SYS_FLAGS1";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x21;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-SYS2";
    item.comment = "Registro SYS_FLAGS2";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x22;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-FAULT";
    item.comment = "Registro codici fault";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x27;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-PADDLE";
    item.comment = "Registro riconoscimento paddle";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x28;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-SENS-DOSE";
    item.comment = "Registro sensore di posizione";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x2a;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-DOSE";
    item.comment = "Registro posizione calibrata";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x2c;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-SFORCE";
    item.comment = "Registro sensore di forza";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x30;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-FORCE";
    item.comment = "Registro forza calibrata";
    item.deviceId = _CODEID_PCB269;
    item.address = 0x31;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-TARA";
    item.comment = "Registro peso del compressore";
    item.deviceId = _CODEID_PCB269;
    item.address = 0xA0+7;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-MAXPOS";
    item.comment = "Registro massima posizione del carrello";
    item.deviceId = _CODEID_PCB269;
    item.address = 0xA0+15;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-MINFORCE";
    item.comment = "Registro minima forza di compressione";
    item.deviceId = _CODEID_PCB269;
    item.address = 0xA0+33;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-TARGET";
    item.comment = "Registro target di compressione";
    item.deviceId = _CODEID_PCB269;
    item.address = 0xA0+35;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "CMP-LIMIT";
    item.comment = "Registro massima compressione";
    item.deviceId = _CODEID_PCB269;
    item.address = 0xA0+36;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    //  --------   REGISTRI RELATIVI ALLA PCB190  -------------

    item.tag = "INV-SYS0";
    item.comment = "Registro SYS_FLAGS0";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x20;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-SYS1";
    item.comment = "Registro SYS_FLAGS1";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x21;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-SYS2";
    item.comment = "Registro SYS_FLAGS2";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x22;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-FAULT";
    item.comment = "Registro codici di errore";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x3e;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-RXHV";
    item.comment = "Registro kV Esposizione";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x40;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-RXIANODE";
    item.comment = "Registro Anodica";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x42;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-RXMAS";
    item.comment = "Registro mAs Esposizione";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x44;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-MINHV";
    item.comment = "Registro valore minimo HV";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x46;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-MAXHV";
    item.comment = "Registro valore massimo HV";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x47;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-MINIA";
    item.comment = "Registro valore minimo I Anodica";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x48;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-MAXIA";
    item.comment = "Registro valore massimo I Anodica";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x49;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-RXMAS-FINE";
    item.comment = "Registro mAs a fine raggi";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x4B;
    item.data_type = _16bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-FUOCO";
    item.comment = "Registro Fuoco corrente";
    item.deviceId = _CODEID_PCB190;
    item.address = 0x57;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-ARSTAT";
    item.comment = "Registro Status Anodo rotante";
    item.deviceId = _CODEID_PCB190;
    item.address = 0xC0;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-AR-MAINRUN";
    item.comment = "Registro corrente MAIN RUN Anodo rotante";
    item.deviceId = _CODEID_PCB190;
    item.address = 0xC8;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-AR-SHIFTRUN";
    item.comment = "Registro corrente SHIHFT RUN Anodo rotante";
    item.deviceId = _CODEID_PCB190;
    item.address = 0xC9;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-AR-MAINKEEP";
    item.comment = "Registro corrente MAIN KEEP Anodo rotante";
    item.deviceId = _CODEID_PCB190;
    item.address = 0xCA;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);

    item.tag = "INV-AR-SHIFTKEEP";
    item.comment = "Registro corrente SHIFT KEEP Anodo rotante";
    item.deviceId = _CODEID_PCB190;
    item.address = 0xCB;
    item.data_type = _8bit;
    item.sign_type = _UNSIGNED;
    deviceRegisterList.append(item);
    return;

}

void serverDebug::notificheConnectionHandler(bool stat)
{
    if(stat)
    {
        // Connessione automatica al servizio di log eventi        
        serviceTcp->txData(QString("---------  IRS COMMAND INTERFACE %1.%2      -------------\r\n").arg((int)IRS_MAJ).arg((int)IRS_MIN).toAscii());
        serviceTcp->txData(QByteArray("See the technical note TN104 Revision 2.0 for details \r\n\r\n\r\n\r\n"));
        serviceTcp->txData(QString("---------------------------------------------------------\r\n").arg((int)IRS_MAJ).arg((int)IRS_MIN).toAscii());
        serviceTcp->txData(QByteArray(">"));
    }else
    {
        disconnect(this);
        pGeneratore->manualMode = false;

    }
    return;
}

void serverDebug::serviceRxHandler(QByteArray data)
{

    // Carattere di annullamento famiglia
    if(data.contains("..")) cmdGroup.clear();

    // Cambio famiglia
    if(data.contains(':')==FALSE) data.prepend(cmdGroup);

    if(data.contains("collimatore:"))
    {
        cmdGroup="collimatore: ";
        handleCollimatore(data);
    }else if(data.contains("config:"))
    {
        cmdGroup="config: ";
        handleConfig(data);
    } else if(data.contains("shell:"))
    {
        cmdGroup="shell: ";
        handleShell(data);
    } else if(data.contains("rotazioni:"))
    {
        cmdGroup="rotazioni: ";
        handleRotazioni(data);
    }else if(data.contains("setPage:"))
    {
        handleSetPage(data);
    }
    else if(data.contains("generator:"))
    {
        cmdGroup="generator: ";
        handleGeneratore(data);
    }else if(data.contains("aws:"))
    {
        cmdGroup="aws: ";
        handleConsole(data);
    }else if(data.contains("system:"))
    {
        cmdGroup="system: ";
        handleSystem(data);
    }else if(data.contains("drivers:"))
    {
        cmdGroup="drivers: ";
        handleDrivers(data);
    }else if(data.contains("biopsy:"))
    {
        cmdGroup="biopsy: ";
        if(pBiopsy->model== BYM_STANDARD_DEVICE)  handleStandardBiopsy(data);
        else handleExtendedBiopsy(data);
    }else if(data.contains("loader:"))
    {
        cmdGroup="loader: ";
        handleLoader(data);
    }else if(data.contains("compressore:"))
    {
        cmdGroup="compressore: ";
        handleCompressore(data);
    }else if(data.contains("master:"))
    {
        cmdGroup="master: ";
        handleMasterShell(data);
    }else if(data.contains("slave:"))
    {
        cmdGroup="slave: ";
        handleSlaveShell(data);
    }else if(data.contains("potter:"))
    {
        cmdGroup="potter: ";
        handlePotter(data);
    }else if(data.contains("canopen:"))
    {
        cmdGroup="canopen: ";
        handleCanOpen(data);
    }else if(data.contains("debug:"))
    {
        cmdGroup="debug: ";
        handleDebug(data);
    }else if(data.contains("?")) // DEVE ESSERE L'ULTIMO
    {
        handleList();
    }



    lastValidFrame = data; // Salva il comando
    if(cmdGroup.isEmpty()) serviceTcp->txData(QByteArray(">"));
    else serviceTcp->txData(QByteArray(cmdGroup).append(">"));
    return;
}

/*
 *  Funzione per la visualizzazione dei comandi a shell disponibili
 */
void serverDebug::handleList(void)
{
    serviceTcp->txData(QByteArray("-----------------------------------------------------\r\n"));
    serviceTcp->txData(QByteArray("system: -------- Comandi generali di sistema \r\n"));
    serviceTcp->txData(QByteArray("config: -------- Comandi generali di configurazione \r\n"));
    serviceTcp->txData(QByteArray("drivers:  ------ Comandi diretti ai drivers \r\n"));
    serviceTcp->txData(QByteArray("collimatore: --- Comandi di gestione del collimatore \r\n"));
    serviceTcp->txData(QByteArray("compressore: --- Comandi di gestione del compressore \r\n"));
    serviceTcp->txData(QByteArray("rotazioni: ----- Comandi di gestione delle rotazioni \r\n"));
    serviceTcp->txData(QByteArray("potter: -------- Comandi di gestione delle funzioni potter\r\n"));
    serviceTcp->txData(QByteArray("aws: ----------- Comandi relativi alla Console \r\n"));
    serviceTcp->txData(QByteArray("loader: -------- Comandi relativi al loader \r\n"));
    serviceTcp->txData(QByteArray("biopsy: -------- Comandi di gestione torretta \r\n"));
    serviceTcp->txData(QByteArray("generator: ----  Comandi relativi al generatore \r\n"));
    serviceTcp->txData(QByteArray("master: -------- Comandi shell su Master \r\n"));
    serviceTcp->txData(QByteArray("slave: --------- Comandi shell su Slave \r\n"));
    serviceTcp->txData(QByteArray("canopen: --------- Comandi shell su Slave \r\n"));
    serviceTcp->txData(QByteArray("debug: --------- Comandi per operazioni di debug \r\n"));
    serviceTcp->txData(QByteArray("------------------------------------------------------\r\n"));

}

//_____________________________________________________________________________________________________________________________        >>>>>> DEBUG GROUP
void serverDebug::handleConfig(QByteArray data)
{
    QString stringa;

    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("setAutoFilter    [ON/OFF]   ON=AutoFilter OFF=Fixed Filter \r\n"));
        serviceTcp->txData(QByteArray("setHsStarter     [ON/OFF]   ON=HS Starter OFF=LS Starter \r\n"));
        serviceTcp->txData(QByteArray("setArmMotor      [ON/OFF]   ON=ARM with motor OFF=ARM without motor \r\n"));
        serviceTcp->txData(QByteArray("setTrxMotor      [ON/OFF]   ON=TRX with motor OFF=No TRX \r\n"));

        serviceTcp->txData(QByteArray("setDemoMode       [ON/OFF]   ON=Activate Demo OFF=Clear Demo \r\n"));
        serviceTcp->txData(QByteArray("enableAccessory   [ON/OFF]   ON=Enabled OFF=Disabled \r\n"));
        serviceTcp->txData(QByteArray("setSN             [n/.]       .=erase SN, n=gantry serial number (only digits)\r\n"));
        serviceTcp->txData(QByteArray("setPSW            [n]         n=Service panel password (only digits)\r\n"));
        serviceTcp->txData(QByteArray("enableStarterKeep [ON/OFF]   ON=Keeping enabled OFF=Keeping disabled\r\n"));
        serviceTcp->txData(QByteArray("setTubeTemp       [lval/hval] lval = reset Tube alarm temp, hval=Trip Tube alarm temp\r\n"));
        serviceTcp->txData(QByteArray("setLanguage       [ITA/ENG..] set the current language\r\n"));
        serviceTcp->txData(QByteArray("resetGantry                   reset the factory gantry configuration\r\n"));
        serviceTcp->txData(QByteArray("resetKvCalib                  reset readback kv calibration file\r\n"));
        serviceTcp->txData(QByteArray("sysBackup         filename    backup master: nome = serial number\r\n"));
        serviceTcp->txData(QByteArray("sysRestore        filename    Restore da home\r\n"));

        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    }else if(data.contains("setAutoFilter")){
        if(data.contains("ON")) {
            pConfig->sys.autoFilter = 1;
            serviceTcp->txData(QByteArray("AUTO FILTER ON. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }else if(data.contains("OFF")){
            pConfig->sys.autoFilter = 0;
            serviceTcp->txData(QByteArray("AUTO FILTER OFF. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }
    }else if(data.contains("setHsStarter")){
        if(data.contains("ON")) {
            pConfig->sys.highSpeedStarter = 1;
            serviceTcp->txData(QByteArray("HS Starter. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }else if(data.contains("OFF")){
            pConfig->sys.highSpeedStarter = 0;
            serviceTcp->txData(QByteArray("LS Starter. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }
    }else if(data.contains("setArmMotor")){
        if(data.contains("ON")) {
            pConfig->sys.armMotor = 1;
            serviceTcp->txData(QByteArray("ARM with motor. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }else if(data.contains("OFF")){
            pConfig->sys.armMotor = 0;
            serviceTcp->txData(QByteArray("ARM without motor. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }
    }else if(data.contains("setTrxMotor")){
        if(data.contains("ON")) {
            pConfig->sys.trxMotor = 1;
            serviceTcp->txData(QByteArray("TRX with motor. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }else if(data.contains("OFF")){
            pConfig->sys.trxMotor = 0;
            serviceTcp->txData(QByteArray("NO TRX. Request a system reboot!!\n\r"));
            pConfig->saveSysCfg();
        }
    }else if(data.contains("enableAccessory")){
                if(data.contains("ON")) {
                    pConfig->userCnf.enableCheckAccessorio = 1;
                    serviceTcp->txData(QByteArray("ACCESSORY TEST ENABLED!\n\r"));
                    pConfig->saveUserCfg();
                }else if(data.contains("OFF")){
                    pConfig->userCnf.enableCheckAccessorio = 0;
                    serviceTcp->txData(QByteArray("ACCESSORY TEST DISABLED!\n\r"));
                    pConfig->saveUserCfg();
                }else{
                    if(pConfig->userCnf.enableCheckAccessorio) serviceTcp->txData(QByteArray("ACCESSORY TEST ENABLED!\n\r"));
                    else serviceTcp->txData(QByteArray("ACCESSORY TEST DISABLED!\n\r"));
                }

     }else if(data.contains("setSN")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("setSN"));

        if(parametri.size() == 0) {
            stringa = QString("CURRENT SN: %1\r\n").arg(pConfig->userCnf.SN);
            serviceTcp->txData(stringa.toAscii());
            return;
        }
        if(parametri.size() != 1) {
            serviceTcp->txData(QString("Invalid serial number\n\r").toAscii());
            return;
        }
        if(parametri[0]=="."){
            pConfig->userCnf.SN ="";
            pConfig->SN_Configured = false;
            serviceTcp->txData(QByteArray("SERIAL NUMBER RESET!\r\n"));
        }else{
            pConfig->userCnf.SN = parametri[0];
            pConfig->SN_Configured = true;
            serviceTcp->txData(QByteArray("SERIAL NUMBER ASSIGNED!\r\n"));
        }

        pConfig->saveUserCfg();
        return;
    }else if(data.contains("setPSW")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("setPSW"));
        if(parametri.size() == 0) {
            stringa = QString("CURRENT PSW: %1\r\n").arg(pConfig->userCnf.ServicePassword);
            serviceTcp->txData(stringa.toAscii());
            return;
        }
        if(parametri.size() != 1) {
            serviceTcp->txData(QString("Invalid Password\n\r").toAscii());
            return;
        }
        pConfig->userCnf.ServicePassword = parametri[0];

        pConfig->saveUserCfg();
        pConfig->testConfigError(true, true);
        serviceTcp->txData(QByteArray("Password assigned!\r\n"));
        return;
    }else if(data.contains("enableStarterKeep")){
        if(data.contains("ON")) {
            pConfig->userCnf.starter_off_after_exposure = 0;
            serviceTcp->txData(QByteArray("STARTER KEEPING ENABLED!\n\r"));
            pConfig->saveUserCfg();
        }else if(data.contains("OFF")){
            pConfig->userCnf.starter_off_after_exposure = 1;
            serviceTcp->txData(QByteArray("STARTER KEEPING DISABLED!\n\r"));
            pConfig->saveUserCfg();
        }else{
            if(!pConfig->userCnf.starter_off_after_exposure) serviceTcp->txData(QByteArray("STARTER KEEPING ENABLED!\n\r"));
            else serviceTcp->txData(QByteArray("STARTER KEEPING DISABLED!\n\r"));
        }

    }else if(data.contains("setTubeTemp")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("setTubeTemp"));
        if(parametri.size() == 0 ) {
            stringa = QString("CURRENT PARAM: HIGH TEMP = %1, LOW TEMP=%2!\r\n").arg(pConfig->userCnf.tempCuffiaAlr).arg(pConfig->userCnf.tempCuffiaAlrOff);
            serviceTcp->txData(stringa.toAscii());
            return;
        }
        if(parametri.size() != 2) {
            serviceTcp->txData(QString("Invalid command: almost 2 parameters is requested\n\r").toAscii());
            return;
        }


        pConfig->userCnf.tempCuffiaAlr = parametri[1].toInt();     // Set allarme cuffia
        pConfig->userCnf.tempCuffiaAlrOff = parametri[0].toInt();  // Reset Allarme cuffia
        pConfig->saveUserCfg();
        stringa = QString("HIGH TEMP = %1, LOW TEMP=%2!\r\n").arg(pConfig->userCnf.tempCuffiaAlr).arg(pConfig->userCnf.tempCuffiaAlrOff);
        serviceTcp->txData(stringa.toAscii());
        return;
    }else if(data.contains("setLanguage")){
        handleSetLanguage(data);
    }else if(data.contains("resetGantry")){
        QString command = QString("rm /resource/config/sysCfg.cnf");
        system(command.toStdString().c_str());
        command = QString("sync");
        system(command.toStdString().c_str());
        serviceTcp->txData(QByteArray("DONE!\n\r"));
    }else if(data.contains("resetKvCalib")){

        QString command = QString("rm /resource/config/kvcalib.dat");
        system(command.toStdString().c_str());
        command = QString("sync");
        system(command.toStdString().c_str());
        serviceTcp->txData(QByteArray("DONE!\n\r"));
    }else if(data.contains("sysBackup")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("sysBackup"));

        if(parametri.size() != 1) {
            stringa = QString("MISSING FILENAME\r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }
        pConfig->sysBackup(TRUE,parametri[0],0);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;
    }else if(data.contains("sysRestore")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("sysRestore"));

        if(parametri.size() != 1) {
            stringa = QString("MISSING FILENAME\r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }
        pConfig->sysRestore(TRUE,parametri[0],0);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;
    }else if(data.contains("setDemoMode")){
        if(data.contains("ON")) {
            pConfig->setDemoMode(true);
            serviceTcp->txData(QByteArray("DEMO MODE ACTIVATED AFTER SYSTEM REBOOT!\n\r"));
        }else if(data.contains("OFF")){
            pConfig->setDemoMode(false);
            serviceTcp->txData(QByteArray("DEMO MODE DISABLED AFTER SYSTEM REBOOT!\n\r"));
        }else{
            serviceTcp->txData(QByteArray("WRONG PARAMETER!\n\r"));
        }
    }


}

//_____________________________________________________________________________________________________________________________        >>>>>> DEBUG GROUP
void serverDebug::handleDebug(QByteArray data)
{
    //int Generatore::getAecData(int plog, int modo_filtro, int odindex, int techmode, int* filtro,float* kV, int* dmAs, int* pulses){
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("updateTS                                   Aggiorna M4+DBT\r\n"));
        serviceTcp->txData(QByteArray("updateGUI                                  Aggiorna solo DBT \r\n"));
        serviceTcp->txData(QByteArray("updateM4                                   Aggiorna solo M4 \r\n"));

        serviceTcp->txData(QByteArray("setCompressorNotify: <frame-gui-notify>    Simula ricezione Gui notify per compressore\r\n"));
        serviceTcp->txData(QByteArray("setActuatorEnableNotify: <frame-actuator-notify>      Simula ricezione actuator enable notify \r\n"));
        serviceTcp->txData(QByteArray("setAutoAlarm: classe, codice               Simula attivazione allarme automatico\r\n"));
        serviceTcp->txData(QByteArray("setAlarm: classe, codice                   Simula attivazione allarme non rirpistinabile\r\n"));
        serviceTcp->txData(QByteArray("setDatabaseI     code,val \r\n"));
        serviceTcp->txData(QByteArray("setDatabaseU     code,val \r\n"));
        serviceTcp->txData(QByteArray("setDatabaseS     code,val \r\n"));
        serviceTcp->txData(QByteArray("getDatabaseU     code \r\n"));
        serviceTcp->txData(QByteArray("testCmd          Esegue il comando di test \r\n"));
        serviceTcp->txData(QByteArray("enablePrint      Abilita print su seriale \r\n"));
        serviceTcp->txData(QByteArray("freeze           Blocca ciclo automatico dei drivers\r\n"));
        serviceTcp->txData(QByteArray("run              Attiva ciclo automatico dei drivers\r\n"));
        serviceTcp->txData(QByteArray("setArmAng        Imposta l'angolo corrente\r\n"));
        serviceTcp->txData(QByteArray("generateAlarmList  \r\n"));
        serviceTcp->txData(QByteArray("setBurning val   Attiva il pulsante raggi per un tot ms  \r\n"));
        serviceTcp->txData(QByteArray("setRestorePoint   Crea un punto di restore  \r\n"));
        serviceTcp->txData(QByteArray("getRestorePoint   Ripristina un punto di restore  \r\n"));
        serviceTcp->txData(QByteArray("restartSlaveMcc     \r\n"));

        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    } else if(data.contains("setRestorePoint"))
    {
        pConfig->setRestorePoint();
        serviceTcp->txData(QByteArray("SET RESTORE POINT DONE\n\r"));
    } else if(data.contains("getRestorePoint"))
    {
        pConfig->getRestorePoint();
        serviceTcp->txData(QByteArray("GET RESTORE POINT DONE\n\r"));
    } else if(data.contains("generateAlarmList"))
    {
        paginaAllarmi->exportMessageList();
        serviceTcp->txData(QByteArray("DONE\n\r"));
    } else if(data.contains("setCompressorNotify"))
    {
        handleSetCompressorNotify(data);
    }else if(data.contains("setActuatorEnableNotify"))
    {
        handleSetActuatorEnableNotify(data);
    }else if(data.contains("setAutoAlarm"))
    {
        handleSetAlarm(data,true);
    }else if(data.contains("setAlarm"))
    {
        handleSetAlarm(data,false);
    }else if(data.contains("updateTS")){
        if(pConfig->executeUpdateIde()==true){
          serviceTcp->txData(QByteArray("AGGIORNAMENTO IDE EFFETTUATO: SYSTEM REBOOT ...\n\r"));
        }
    }else if(data.contains("updateGUI")){
        if(pConfig->executeUpdateGui()==true){
          serviceTcp->txData(QByteArray("AGGIORNAMENTO GUI EFFETTUATO: SYSTEM REBOOT ...\n\r"));
        }
    }else if(data.contains("updateM4")){
        if(pConfig->executeUpdateM4()==true){
          serviceTcp->txData(QByteArray("AGGIORNAMENTO GUI EFFETTUATO: SYSTEM REBOOT ...\n\r"));
        }
    }else if(data.contains("setDatabaseI")){
        handleSetDatabaseI(data);
   }else if(data.contains("setDatabaseU")){
       handleSetDatabaseU(data);
   }else if(data.contains("setDatabaseS")){
       handleSetDatabaseS(data);
   }else if(data.contains("getDatabaseU")){
        QList<QByteArray> parametri;
        parametri = getNextFieldsAfterTag(data, QString("getDatabaseU"));
        if(parametri.size()!=1) return;
        int indice = parametri[0].toInt();
        QString val = QString("DATABASE-%1 = %2\n\r").arg(indice).arg(ApplicationDatabase.getDataU(indice));
        serviceTcp->txData(val.toAscii());

    }else if(data.contains("setArmAng")){
        QList<QByteArray> parametri;
        parametri = getNextFieldsAfterTag(data, QString("setArmAng"));
        if(parametri.size()!=1) return;
        ApplicationDatabase.setData(_DB_DANGOLO, (int) parametri[0].toInt());
        serviceTcp->txData(QByteArray("OK\n\r"));

    }else if(data.contains("setBurning")){
        QList<QByteArray> parametri;
        parametri = getNextFieldsAfterTag(data, QString("setBurning"));
        if(parametri.size()!=1) {
            serviceTcp->txData(QByteArray("ERRORE: inserire il tempo in ms\n\r"));
            return;
        }
        io->setSpareOutputPulse(1, parametri[0].toInt());
        serviceTcp->txData(QByteArray("DONE \n\r"));

    }else if(data.contains("freeze")) handleDriverFreeze(TRUE);
    else if(data.contains("run")) handleDriverFreeze(FALSE);
    else if(data.contains("testCmd")){
        unsigned char data;
        data = 0;
        pConsole->pGuiMcc->sendFrame(MCC_TEST,1,&data,1);
   }else if(data.contains("enablePrint")){
        unsigned char data[2];
        data[0] = MCC_DRIVER_PRINT_ENABLE_CMD;
        data[1] = 1;
        pConsole->pGuiMcc->sendFrame(MCC_PRINT,1,data,2);
        serviceTcp->txData(QByteArray("DONE \n\r"));

        pConfig->enableSlavePrint();
   }else if(data.contains("restartSlaveMcc")){
        pConfig->slaveRestartMcc();
        serviceTcp->txData(QByteArray("DONE \n\r"));
   }
}

void serverDebug::handleSetCompressorNotify(QByteArray data){
    QList<QByteArray> parametri;

#ifndef __COMPRESSOR_PCB215_NOTIFY_STUB
    serviceTcp->txData(QByteArray("COMPILAZIONE NON ABILITATA\n\r"));
    return;
#else

    parametri = getNextFieldsAfterTag(data, QString("setCompressorNotify"));
    if(parametri.size()!=7){
        serviceTcp->txData(QByteArray("FLAG0, FLAG1, FORZA, SPESSORE, PAD, TARGET, POSITION!\n\r"));
        return;
    }
    QByteArray buffer;
    buffer.append((unsigned char) parametri[0].toUShort());
    buffer.append((unsigned char) parametri[1].toUShort());
    buffer.append((unsigned char) (parametri[2].toUShort()&0x00FF));
    buffer.append((unsigned char) (parametri[2].toUShort()>>8));
    buffer.append((unsigned char) parametri[3].toUShort());
    buffer.append((unsigned char) parametri[4].toUShort());
    buffer.append((unsigned char) (parametri[5].toUShort()&0x00FF));
    buffer.append((unsigned char) (parametri[5].toUShort()>>8));


    pCompressore->pcb215Notify(0, PCB215_NOTIFY_COMPR_DATA, buffer);
    serviceTcp->txData(QByteArray("DONE!\n\r"));
#endif
}

void serverDebug::handleSetActuatorEnableNotify(QByteArray data){
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("setActuatorEnableNotify"));
    if(parametri.size()!=3){
        serviceTcp->txData(QByteArray("ROTENA, PENDENA, LIFTENA \n\r"));
        return;
    }
    QByteArray buffer;
    buffer.append((unsigned char) parametri[0].toUShort());
    buffer.append((unsigned char) parametri[1].toUShort());
    buffer.append((unsigned char) parametri[2].toUShort());

    pConsole->emitMccActuatorNotify(1, ACTUATOR_STATUS,buffer);
    serviceTcp->txData(QByteArray("DONE!\n\r"));

}

void serverDebug::handleSetAlarm(QByteArray data,bool selfreset)
{
    QList<QByteArray> parametri;
    int classe, codice;

    if(selfreset)
        parametri = getNextFieldsAfterTag(data, QString("setAutoAlarm"));
    else
        parametri = getNextFieldsAfterTag(data, QString("setAlarm"));


    if(parametri.size()==2){
        classe = parametri[0].toInt();
        codice = parametri[1].toInt();
    }else{
        serviceTcp->txData("PARAMETRI NON CORRETTI \n");
        return;
    }

    PageAlarms::debugActivateNewAlarm(classe, codice,selfreset);
}

void serverDebug::handleSetDatabaseI(QByteArray data){
    QList<QByteArray> parametri;
    parametri = getNextFieldsAfterTag(data, QString("setDatabaseI"));
    if(parametri.size()<2){
        serviceTcp->txData("ERRORI PARAMETRI \n");
        return;
    }

    ApplicationDatabase.setData(parametri[0].toInt(), (int) parametri[1].toInt(),0);

}
void serverDebug::handleSetDatabaseU(QByteArray data){
    QList<QByteArray> parametri;
    parametri = getNextFieldsAfterTag(data, QString("setDatabaseU"));
    if(parametri.size()<2){
        serviceTcp->txData("ERRORI PARAMETRI \n");
        return;
    }

    ApplicationDatabase.setData(parametri[0].toInt(), (unsigned char) parametri[1].toInt(),0);
}
void serverDebug::handleSetDatabaseS(QByteArray data){
    QList<QByteArray> parametri;
    parametri = getNextFieldsAfterTag(data, QString("setDatabaseS"));



    QString val;
    if(parametri.size()==1) val="";
    else{
        for(int ciclo=1;ciclo<parametri.size();ciclo++ ){
            val.append(parametri[ciclo]);
        }
    }
    ApplicationDatabase.setData(parametri[0].toInt(), val,0);
}

// COMANDA L'ATTIVAZIONE DEL MODO FREEZE (ON/OFF)
// data[0] = 1->ON, 0->OFF
// RISPOSTA:
// BUFFER[0]= RISULTATO
// BUFFER[1] = STATO ESEGUITO
void serverDebug::handleDriverFreeze(bool stat)
{
    QByteArray val;
    val.clear();

    if(stat) val.append((char) 1);
    else val.append((char) 0);

    if(mccService(1,SRV_FREEZE_DEVICE,val)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverFreezeNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

}

void serverDebug::handleDriverFreezeNotify(unsigned char id,unsigned char cmd, QByteArray data)
{
    if(cmd!=SRV_FREEZE_DEVICE) return;
    disconnect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverFreezeNotify(unsigned char,unsigned char,QByteArray)));
    if(data.at(0)==0) serviceTcp->txData("DRIVER FREEZE FALLITO");
    else
    {
        if(data.at(1)) serviceTcp->txData("DRIVER FREEZED ");
        else serviceTcp->txData("DRIVER RUN ");
    }
}
//_____________________________________________________________________________________________________________________________        >>>>>> COMPRESSORE GROUP

void serverDebug::handleCompressore(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("setCalibPos: <offset, klin>  modifica la calibrazione della nacchera \r\n"));
        serviceTcp->txData(QByteArray("getCalibPos:                 restituisce valore calibrazione nacchera \r\n"));
        serviceTcp->txData(QByteArray("getPadList:                  restituisce lista pad configurabili \r\n"));
        serviceTcp->txData(QByteArray("setCalibPad: <ofs,kF,peso>   modifica calibrazione della nacchera \r\n"));
        serviceTcp->txData(QByteArray("getCalibPad:                 restituisce valore calibrazione pad corrente \r\n"));
        serviceTcp->txData(QByteArray("setThick: <val>              corregge spessore alla compressione corrente \r\n"));
        serviceTcp->txData(QByteArray("setKF: <val>                 imposta coefficiente di flessione spessore\r\n"));
        serviceTcp->txData(QByteArray("setLimitForce: <val>         imposta max compressione (range 70:200) \r\n"));
        serviceTcp->txData(QByteArray("setWeight: <val>             imposta Tara Paddle in (N) \r\n"));

        serviceTcp->txData(QByteArray("getTrolley                   Restituisce posizione trolley \r\n"));
        serviceTcp->txData(QByteArray("setCompressorRelease <val>   Imposta sblocco automatico dopo raggi-X \r\n"));
        serviceTcp->txData(QByteArray("readPadConfig:               Legge il file di configurazione \r\n"));
        serviceTcp->txData(QByteArray("storePadConfid:              Salva il file di configurazione \r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    } else if(data.contains("getTrolley"))
    {
        // Invio comando
        unsigned char data=0;
        if(pConsole->pGuiMcc->sendFrame(MCC_GET_TROLLEY,1,&data, 1)==TRUE)
        {
            serviceTcp->txData(QByteArray("Comando in esecuzione..... \r\n"));
            connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleGetTrolleyNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
        }
        return;

    }else if(data.contains("setCompressorRelease"))
    {
        handleSetCompressorRelease(data);
    } else if(data.contains("setThick"))
    {
        handleSetThick(data);
    }else if(data.contains("setKF"))
    {
        handleSetKF(data);
    }else if(data.contains("setWeight"))
    {
        handleSetPeso(data);
    }else if(data.contains("getCalibPad"))
    {
        handleGetCalibPad(data);
    }else if(data.contains("setCalibPad"))
    {
        handleSetCalibPad(data);
    }else if(data.contains("getCalibPos"))
    {
        handleGetCalibNacchera();
    }else if(data.contains("setCalibPos"))
    {
        handleSetCalibNacchera(data);
    }else if(data.contains("getPadList"))
    {
        handleGetPadList();
    }else if(data.contains("readPadConfig"))
    {
        pCompressore->readCompressorConfigFile();
        pConfig->updatePCB269();
    }else if(data.contains("storePadConfig"))
    {
        pCompressore->storeConfigFile();
        serviceTcp->txData(QByteArray("DATI SALVATI IN CONFIGURAZIONE!\r\n"));
    }else if(data.contains("setLimitForce"))
    {
        QList<QByteArray> parametri;

        parametri = getNextFieldsAfterTag(data, QString("setLimitForce"));
        if(parametri.size()!=1) serviceTcp->txData(QByteArray("WRONG PARAMETER!\n\r"));
        if(parametri[0].toInt() > 200){
            serviceTcp->txData(QByteArray("ERROR: the limit force cannot exceed 200N!\n\r"));
            return;
        }
        if(parametri[0].toInt() < 70){
            serviceTcp->txData(QByteArray("ERROR: the limit force cannot be lower than 70N!\n\r"));
            return;
        }

        // Salva il valore, aggiorna il file di configurazione e aggiorna la periferica
        pCompressore->config.max_compression_force = parametri[0].toInt();
        pCompressore->storeConfigFile();
        pConfig->updatePCB269();

        serviceTcp->txData(QByteArray("Pad Configuration file and peripheral device updated!\n\r"));
        return;
    }



}

void serverDebug::handleGetPadList(void)
{
    int i;
    QString stringa;

    serviceTcp->txData(QString("CODE  NAME\n\r").arg(stringa).toAscii());
    for(i=0; i<PAD_ENUM_SIZE; i++)
    {
        if(i>9) stringa = QString("%1)   %2").arg((int) i).arg( pCompressore->getPadName((Pad_Enum) i));
        else stringa = QString("%1)    %2").arg((int) i).arg( pCompressore->getPadName((Pad_Enum) i));       
        serviceTcp->txData(QString("%1\n\r").arg(stringa).toAscii());
    }
    return;
}


void serverDebug::handleGetCalibNacchera(void)
{
    serviceTcp->txData(QString("OFFSET=%1, kLIN=%2\n\r").arg(pCompressore->config.calibPosOfs).arg(pCompressore->config.calibPosK).toAscii());
    return;
}

void serverDebug::handleSetCalibNacchera(QByteArray data)
{
    QList<QByteArray> parametri;
    int i;

    parametri = getNextFieldsAfterTag(data, QString("setCalibPos"));
    if(parametri.size()!=2) serviceTcp->txData(QByteArray("PARAMETRI ERRATI!\n\r"));

    // Calcola l'offset da aggiungere
    pCompressore->config.calibPosOfs = parametri[0].toInt();
    pCompressore->config.calibPosK = parametri[1].toInt();

    // rende effettivi i valori di calibrazione
    unsigned char buf[4];
    buf[0] = 2; // Comando di calibrazione Pads
    buf[1] = (unsigned char) (pCompressore->config.calibPosOfs);
    buf[2] = (unsigned char) (pCompressore->config.calibPosOfs >> 8);
    buf[3] = (unsigned char) pCompressore->config.calibPosK;
    pConsole->pGuiMcc->sendFrame(MCC_CMD_PCB215_CALIB,1,buf,sizeof(buf));

    return;

}


void serverDebug::handleGetCalibPad(QByteArray data)
{
    int i;

    // Controlla che ci sia un valido PAD selezionato
    if(pCompressore->isValidPad()==FALSE)
    {
        serviceTcp->txData(QByteArray("Nessun Pad valido risulta risconosciuto dal sistema!\n\r"));
        return;
    }

    // Calcola l'offset da aggiungere
    int pad = pCompressore->getPad();
    QString padName = pCompressore->getPadName();
    serviceTcp->txData(QString("PAD:%1, OFFSET=%2, kF=%3, PESO=%4\n\r").arg(padName).arg(pCompressore->config.pads[pad].offset).arg(pCompressore->config.pads[pad].kF).arg(pCompressore->config.pads[pad].peso).toAscii());

    return;

}

void serverDebug::handleSetCalibPad(QByteArray data)
{
    QList<QByteArray> parametri;
    QString frame;
    int i;

    parametri = getNextFieldsAfterTag(data, QString("setCalibPad"));
    if(parametri.size()!=3)
    {
        frame = QString("Questa funzione imposta la calibrazione del pad attualmente utilizzato. \n\r"
                        "> setCalibPad offset,kF,peso(N)\n\r"
                        "  offset: Offset meccanico rispetto allo zero della nacchera\n\r"
                        "  kF: compensazione forza di compressione\n\r"
                        "  peso(N): peso nacchera+pad\n\r");
        serviceTcp->txData(frame.toAscii());
        return;
    }

    // Controlla che ci sia un valido PAD selezionato
    if(pCompressore->isValidPad()==FALSE)
    {
        serviceTcp->txData(QByteArray("Nessun Pad riconosciuto o configurato!\n\r"));
        return;
    }


    // Calcola l'offset da aggiungere
    int pad = pCompressore->getPad();
    pCompressore->config.pads[pad].offset = parametri[0].toInt();
    pCompressore->config.pads[pad].kF = parametri[1].toInt();
    pCompressore->config.pads[pad].peso = parametri[2].toInt();

    // rende effettivi i valori di calibrazione
    unsigned char buf[6];
    buf[0] = 4; // Comando di calibrazione Pads
    buf[1] = pad;
    buf[2] = (unsigned char) pCompressore->config.pads[pad].offset;
    buf[3] = (unsigned char) ((unsigned short) pCompressore->config.pads[pad].offset >> 8);
    buf[4] = (unsigned char) pCompressore->config.pads[pad].kF;
    buf[5] = (unsigned char) pCompressore->config.pads[pad].peso;
    pConsole->pGuiMcc->sendFrame(MCC_CMD_PCB215_CALIB,1,buf,sizeof(buf));

    serviceTcp->txData(QByteArray("PAD correttamente configurato e parametri operativi\n\r"));

    return;

}

void serverDebug::handleCalibThresholds(QByteArray data)
{
    QList<QByteArray> parametri;
    QString frame;

    parametri = getNextFieldsAfterTag(data, QString("calibThresholds"));
    if(parametri.size()!=1)
    {
        frame = QString("Questa funzione imposta la calibrazione delle soglie di rilevamento PAD. \n\r"
                        "> calibThreshold valore\n\r"
                        "  valore: Lettura registro PCB215 0x28 con sblocco inferiore nacchera aperto\n\r");
        serviceTcp->txData(frame.toAscii());
        return;
    }

    pCompressore->calibrateThresholds(parametri[0].toInt());
    return;

}


void serverDebug::handleSetCompressorRelease(QByteArray data)
{
    QList<QByteArray> parametri;
    bool value;
    unsigned char cpu_flags;

    parametri = getNextFieldsAfterTag(data, QString("setCompressorRelease"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("Wrong data forma: data=1-> enable, data=0 -> disable \r\n"));
        return;
    }

    if(parametri[0].toInt()==1) value=true;
    else value=false;
    if(value==pConfig->userCnf.enableSblocco) return;

    pConfig->userCnf.enableSblocco = value;
    if(value) ApplicationDatabase.setData(_DB_COMPRESSOR_UNLOCK,1,0);
    else  ApplicationDatabase.setData(_DB_COMPRESSOR_UNLOCK,0,0);


    serviceTcp->txData(QByteArray("COMPRESSOR MODE CHANGED \r\n"));
    return;


}
void serverDebug::handleSetThick(QByteArray data)
{
    QList<QByteArray> parametri;
    QString frame;
    parametri = getNextFieldsAfterTag(data, QString("setThick"));
    if(parametri.size()!=1)
    {
        frame = QString("Questa funzione imposta la calibrazione dell'offset del Pad selezionato.\n\r"
                        "> setThick <val>\n\r"
                        "  val: Valore in mm dello spessore reale\n\r"
                        "  Si consiglia di comprimere un fantoccio a circa 40/50 N.\n\r");
        serviceTcp->txData(frame.toAscii());
        return;
    }

    // Controlla che ci sia un valido PAD selezionato
    if(pCompressore->isValidPad()==FALSE)
    {
        serviceTcp->txData(QByteArray("Questa funzione può essere eseguita solo con un Pad configurato!\n\r"));
        return;
    }

    // Calcola l'offset da aggiungere
    int pad = pCompressore->getPad();
    int diff = pCompressore->breastThick - parametri[0].toInt();
    pCompressore->config.pads[pad].offset -= diff;

    // rende effettivi i valori di calibrazione
    unsigned char buf[6];
    buf[0] = 4; // Comando di calibrazione Pads
    buf[1] = pad;
    buf[2] = (unsigned char) pCompressore->config.pads[pad].offset;
    buf[3] = (unsigned char) ((unsigned short) pCompressore->config.pads[pad].offset >> 8);
    buf[4] = (unsigned char) pCompressore->config.pads[pad].kF;
    buf[5] = (unsigned char) pCompressore->config.pads[pad].peso;
    pConsole->pGuiMcc->sendFrame(MCC_CMD_PCB215_CALIB,1,buf,sizeof(buf));

    serviceTcp->txData(QString("PAD:%1, OFS:%2, kF:%3, PESO(N):%4\n\r").arg(pCompressore->getPadName()).arg(pCompressore->config.pads[pad].offset).arg(pCompressore->config.pads[pad].kF).arg(pCompressore->config.pads[pad].peso).toAscii());
    return;

}

void serverDebug::handleSetKF(QByteArray data)
{
    QList<QByteArray> parametri;
    QString frame;

    parametri = getNextFieldsAfterTag(data, QString("setKF"));
    if(parametri.size()!=1)
    {
        frame = QString("Questa funzione imposta la calibrazione della compensazione della forza (kF).\n\r"
                        "> setKF <val>\n\r"
                        "  val: coefficiente [0:255]\n\r");
        serviceTcp->txData(frame.toAscii());
        return;
    }

    // Controlla che ci sia un valido PAD selezionato
    if(pCompressore->isValidPad()==FALSE)
    {
        serviceTcp->txData(QByteArray("Questa funzione può essere eseguita solo con un Pad configurato!\n\r"));
        return;
    }

    // Calcola kF
    int pad = pCompressore->getPad();
    pCompressore->config.pads[pad].kF = parametri[0].toInt();

    // rende effettivi i valori di calibrazione
    unsigned char buf[6];
    buf[0] = 4; // Comando di calibrazione Pads
    buf[1] = pad;
    buf[2] = (unsigned char) pCompressore->config.pads[pad].offset;
    buf[3] = (unsigned char) ((unsigned short) pCompressore->config.pads[pad].offset >> 8);
    buf[4] = (unsigned char) pCompressore->config.pads[pad].kF;
    buf[5] = (unsigned char) pCompressore->config.pads[pad].peso;
    pConsole->pGuiMcc->sendFrame(MCC_CMD_PCB215_CALIB,1,buf,sizeof(buf));

    serviceTcp->txData(QString("PAD:%1, OFS:%2, kF:%3, PESO(N):%4\n\r").arg(pCompressore->getPadName()).arg(pCompressore->config.pads[pad].offset).arg(pCompressore->config.pads[pad].kF).arg(pCompressore->config.pads[pad].peso).toAscii());

    return;

}

void serverDebug::handleSetPeso(QByteArray data)
{
    QList<QByteArray> parametri;
    QString frame;
    parametri = getNextFieldsAfterTag(data, QString("setWeight"));
    if(parametri.size()!=1)
    {
        frame = QString("Questa funzione imposta la calibrazione del peso della nacchera+Pad.\n\r"
                        "> setPeso <val>\n\r"
                        "  val: peso in (N)\n\r");
        serviceTcp->txData(frame.toAscii());
        return;
    }

    // Controlla che ci sia un valido PAD selezionato
    if(pCompressore->isValidPad()==FALSE)
    {
        serviceTcp->txData(QByteArray("Questa funzione può essere eseguita solo con un Pad configurato!\n\r"));
        return;
    }


    // Calcola l'offset da aggiungere
    int pad = pCompressore->getPad();
    pCompressore->config.pads[pad].peso = parametri[0].toInt();

    // rende effettivi i valori di calibrazione
    unsigned char buf[6];
    buf[0] = 4; // Comando di calibrazione Pads
    buf[1] = pad;
    buf[2] = (unsigned char) pCompressore->config.pads[pad].offset;
    buf[3] = (unsigned char) ((unsigned short) pCompressore->config.pads[pad].offset >> 8);
    buf[4] = (unsigned char) pCompressore->config.pads[pad].kF;
    buf[5] = (unsigned char) pCompressore->config.pads[pad].peso;
    pConsole->pGuiMcc->sendFrame(MCC_CMD_PCB215_CALIB,1,buf,sizeof(buf));

    serviceTcp->txData(QString("PAD:%1, OFS:%2, kF:%3, PESO(N):%4\n\r").arg(pCompressore->getPadName()).arg(pCompressore->config.pads[pad].offset).arg(pCompressore->config.pads[pad].kF).arg(pCompressore->config.pads[pad].peso).toAscii());
    return;

}

/*
 */
void serverDebug::handleSetCalibTomo(QByteArray data)
{
    QList<QByteArray> parametri;
    QString frame;
    colliTomoConf_Str* pTomo;
    unsigned char* pLama;

    parametri = getNextFieldsAfterTag(data, QString("setCalibTomo"));
    if(parametri.size()!=28)
    {
        frame = QString("Numero di parametri errato.\n\r");
        serviceTcp->txData(frame.toAscii());
        return;
    }

    if(parametri.at(0)=="W") pTomo = &pCollimatore->colliConf.colliTomoW;
    else if(parametri.at(0)=="Mo") pTomo = &pCollimatore->colliConf.colliTomoMo;
    else {
        frame = QString("Anodo errato.\n\r");
        serviceTcp->txData(frame.toAscii());
        return ;
    }

    if((parametri.at(1)=="L") &&  (parametri.at(2)=="P")) pLama = (pTomo->tomoLeftBladeP);
    else if((parametri.at(1)=="L") &&  (parametri.at(2)=="N")) pLama = (pTomo->tomoLeftBladeN);
    else if((parametri.at(1)=="R") &&  (parametri.at(2)=="P")) pLama = (pTomo->tomoRightBladeP);
    else if((parametri.at(1)=="R") &&  (parametri.at(2)=="N")) pLama = (pTomo->tomoRightBladeN);
    else if((parametri.at(1)=="T") &&  (parametri.at(2)=="P")) pLama = (pTomo->tomoBackTrapP);
    else if((parametri.at(1)=="T") &&  (parametri.at(2)=="N")) pLama = (pTomo->tomoBackTrapN);
    else return ;

    for(int i=0; i<COLLI_DYNAMIC_SAMPLES; i++) pLama[i] = (unsigned char) parametri.at(3+i).toInt();

    serviceTcp->txData(QString("PARAMETRI aggiornati (DOWNLOAD PER SCARICARLI)\n\r").toAscii());
    return;

}

void serverDebug::handleGetCalib(QByteArray data)
{

    QString stringa,tag;
    int i;

    if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01) printf("COLLIMATOR MODEL: ASSY-01\n");
    else printf("COLLIMATOR MODEL: ASSY-02\n");

    for(i=0; i<4; i++){
        if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Rh) tag = "Rh";
        else if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Ag) tag="Ag" ;
        else if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Al) tag="Al" ;
        else if(pCollimatore->colliConf.filterType[i]==Collimatore::FILTRO_Mo) tag="Mo" ;
        else tag="Cu";
        stringa = QString("FILTRO-%1:%2, %3\n\r").arg(i).arg(pCollimatore->colliConf.filterPos[i]).arg(tag);
        serviceTcp->txData(stringa.toAscii());
    }

    // Hotfix 11C
    stringa = QString("TOMO-FILTER: ");
    for(i=0; i<4; i++)  stringa.append(QString("%1  ").arg(pCollimatore->colliConf.filterTomo[i]));
    stringa.append("\n\r");
    serviceTcp->txData(stringa.toAscii());

    if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01)
        stringa = QString("MIRROR STEPS:%1\n\r").arg(pCollimatore->colliConf.mirrorSteps_ASSY_01);
    else stringa = QString("MIRROR STEPS:%1\n\r").arg(pCollimatore->colliConf.mirrorSteps_ASSY_02);
    serviceTcp->txData(stringa.toAscii());

    for(i=0; i<pCollimatore->colliConf.colli2D.size(); i++){
        stringa = QString("%1,").arg(pCompressore->getPadTag((Pad_Enum) (pCollimatore->colliConf.colli2D[i].PadCode)).toAscii().data());
        stringa+=QString("L=%1,").arg(pCollimatore->colliConf.colli2D[i].L);
        stringa+=QString("R=%1,").arg(pCollimatore->colliConf.colli2D[i].R);
        stringa+=QString("F=%1,").arg(pCollimatore->colliConf.colli2D[i].F);
        stringa+=QString("B=%1,").arg(pCollimatore->colliConf.colli2D[i].B);
        stringa+=QString("T=%1\n\r").arg(pCollimatore->colliConf.colli2D[i].T);
        serviceTcp->txData(stringa.toAscii());
    }

    stringa = QString("OPEN: L=%1 R=%2 F=%3 B=%4 T=%5 \n\r").arg(pCollimatore->colliConf.colliOpen.L).arg(pCollimatore->colliConf.colliOpen.R).arg(pCollimatore->colliConf.colliOpen.F).arg(pCollimatore->colliConf.colliOpen.B).arg(pCollimatore->colliConf.colliOpen.T);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("CUSTOM: L=%1 R=%2 F=%3 B=%4 T=%5 \n\r").arg(pCollimatore->customL).arg(pCollimatore->customR).arg(pCollimatore->customF).arg(pCollimatore->customB).arg(pCollimatore->customT);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-L-P: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoW.tomoLeftBladeP[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-L-N: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoW.tomoLeftBladeN[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-R-P: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoW.tomoRightBladeP[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-R-N: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoW.tomoRightBladeN[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-T-P: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoW.tomoBackTrapP[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-T-N: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoW.tomoBackTrapN[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-W-FB: %1, %2").arg(pCollimatore->colliConf.colliTomoW.tomoFront).arg(pCollimatore->colliConf.colliTomoW.tomoBack);
    serviceTcp->txData(stringa.toAscii());


    stringa = QString("\n\rTOMO-Mo-L-P: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoMo.tomoLeftBladeP[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-Mo-L-N: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoMo.tomoLeftBladeN[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-Mo-R-P: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoMo.tomoRightBladeP[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-Mo-R-N: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoMo.tomoRightBladeN[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-Mo-T-P: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoMo.tomoBackTrapP[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-Mo-T-N: ");
    for(i=0; i<COLLI_DYNAMIC_SAMPLES;i++) stringa += QString("%1 ").arg(pCollimatore->colliConf.colliTomoMo.tomoBackTrapN[i]);
    serviceTcp->txData(stringa.toAscii());

    stringa = QString("\n\rTOMO-Mo-FB: %1, %2\n\r\n\r").arg(pCollimatore->colliConf.colliTomoMo.tomoFront).arg(pCollimatore->colliConf.colliTomoMo.tomoBack);
    serviceTcp->txData(stringa.toAscii());

}

// <pad,Mat,L,R,F,B,T>
void serverDebug::handleSetCalibCustom(QByteArray data){
    QList<QByteArray> parametri;


    parametri = getNextFieldsAfterTag(data, QString("setCalibCustom"));

    if(parametri.size()<5)
    {
        serviceTcp->txData(QByteArray("INVALID PARAMETERS!\n\r"));
        return;
    }

    pCollimatore->customL = parametri[0].toInt();
    pCollimatore->customR = parametri[1].toInt();
    pCollimatore->customF = parametri[2].toInt();
    pCollimatore->customB = parametri[3].toInt();
    pCollimatore->customT = parametri[4].toInt();
    serviceTcp->txData("OK \n\r");
    return;
}

// <pad,Mat,L,R,F,B,T>
void serverDebug::handleSetCalib2D(QByteArray data)
{
    QList<QByteArray> parametri;
    QString stringa;
    int i,code;


    parametri = getNextFieldsAfterTag(data, QString("setCalib2D"));
    if(parametri.size()==0)
    {
        serviceTcp->txData(QByteArray("PAD 2D DISPONIBILI:\n\r"));
        for(i=0; i<pCollimatore->colliConf.colli2D.size(); i++)
        {
            stringa = QString(" %1\n\r").arg(pCompressore->getPadTag((Pad_Enum) pCollimatore->colliConf.colli2D[i].PadCode));
            serviceTcp->txData(stringa.toAscii());
        }
        stringa = QString(" OPEN\n\r");
        serviceTcp->txData(stringa.toAscii());

        return;
    }

    if(parametri.size()<6)
    {
        serviceTcp->txData(QByteArray("PARAMETRI ERRATI!\n\r"));
        return;
    }

    // Verifica Pad OPEN
    if(parametri[0]=="OPEN")
    {
        pCollimatore->colliConf.colliOpen.L = parametri[1].toInt();
        pCollimatore->colliConf.colliOpen.R = parametri[2].toInt();
        pCollimatore->colliConf.colliOpen.F = parametri[3].toInt();
        pCollimatore->colliConf.colliOpen.B = parametri[4].toInt();
        pCollimatore->colliConf.colliOpen.T = parametri[5].toInt();
        stringa = QString("NUOVA COLLIMAZIONE OPEN: L=%1 R=%2 F=%3 B=%4 T=%5\n\r").arg(pCollimatore->colliConf.colliOpen.L).arg(pCollimatore->colliConf.colliOpen.R).arg(pCollimatore->colliConf.colliOpen.F).arg(pCollimatore->colliConf.colliOpen.B).arg(pCollimatore->colliConf.colliOpen.T);
        serviceTcp->txData(stringa.toAscii());
        return;
    }

    if(parametri.size()!=7)
    {
        serviceTcp->txData(QByteArray("PARAMETRI ERRATI!\n\r"));
        return;
    }


    // Verifica che la coppia pad+Anodo sia in configurazione

    // Cerca il codice associato al pad indicato
    code = pCompressore->getPadCodeFromTag(parametri[0]);
    if(code>=PAD_ENUM_SIZE)
    {
        serviceTcp->txData(QByteArray("PAD NON IDENTIFICATO\n\r"));
        return;
    }

    for(i=0; i<pCollimatore->colliConf.colli2D.size(); i++)
    {
        if(pCollimatore->colliConf.colli2D[i].PadCode == code)
        {
            pCollimatore->colliConf.colli2D[i].L = parametri[2].toInt();
            pCollimatore->colliConf.colli2D[i].R = parametri[3].toInt();
            pCollimatore->colliConf.colli2D[i].F = parametri[4].toInt();
            pCollimatore->colliConf.colli2D[i].B = parametri[5].toInt();
            pCollimatore->colliConf.colli2D[i].T = parametri[6].toInt();
            stringa = QString("NUOVA COLLIMAZIONE %1,%2: L=%3 R=%4 F=%5 B=%6 T=%7\n\r").arg(QString(parametri[0])).arg(QString(parametri[1])).arg(pCollimatore->colliConf.colli2D[i].L).arg(pCollimatore->colliConf.colli2D[i].R).arg(pCollimatore->colliConf.colli2D[i].F).arg(pCollimatore->colliConf.colli2D[i].B).arg(pCollimatore->colliConf.colli2D[i].T);
            serviceTcp->txData(stringa.toAscii());
            return;
        }
    }

    // Il Pad non era configurato: inserisce un nuovo campo alla configurazione
    _colliPadStr newColli2DItem;
    newColli2DItem.PadCode = code;
    newColli2DItem.L = parametri[2].toInt();
    newColli2DItem.R = parametri[3].toInt();
    newColli2DItem.F = parametri[4].toInt();
    newColli2DItem.B = parametri[5].toInt();
    newColli2DItem.T = parametri[6].toInt();
    pCollimatore->colliConf.colli2D.append(newColli2DItem);
    serviceTcp->txData(QByteArray("AGGIUNTO NUOVO PAD ALLA CONFIGURAZIONE DEI PADS\n\r"));

}

void serverDebug::handleSetCalibFiltro(QByteArray data)
{
    QList<QByteArray> parametri;
    QString tag;
    int i;


    parametri = getNextFieldsAfterTag(data, QString("setCalibFiltro"));
    if(parametri.size()!=2)
    {
        serviceTcp->txData(QByteArray("PARAMETRI ERRATI!\n\r"));
        return;
    }
    if(parametri[1].toInt()>255)
    {
        serviceTcp->txData(QByteArray("VALORE FUORI SCALA!\n\r"));
        return;
    }

    for(i=0; i<4; i++)
    {
        tag = pCollimatore->getFiltroTag(pCollimatore->colliConf.filterType[i]);
        if(parametri[0]==tag)
        {
            unsigned char buffer[4];
            pCollimatore->colliConf.filterPos[i] = parametri[1].toInt();
            buffer[0]=pCollimatore->colliConf.filterPos[0];
            buffer[1]=pCollimatore->colliConf.filterPos[1];
            buffer[2]=pCollimatore->colliConf.filterPos[2];
            buffer[3]=pCollimatore->colliConf.filterPos[3];

            // Invio comando
            pConsole->pGuiMcc->sendFrame(MCC_CALIB_FILTRO,1,buffer, 4);
            serviceTcp->txData(QString("NUOVA POSIZIONE %1:%2\n\r").arg(tag).arg(pCollimatore->colliConf.filterPos[i]).toAscii());
            return;
        }
    }
    serviceTcp->txData(QByteArray("FILTRO INESISTENTE\n\r"));

}

// Hotfix 11C
void serverDebug::handleSetCalibTomoFiltro(QByteArray data)
{
    QList<QByteArray> parametri;


    parametri = getNextFieldsAfterTag(data, QString("setCalibTomoFiltro"));
    if(parametri.size()!=4)
    {
        serviceTcp->txData(QByteArray("PARAMETRI ERRATI!\n\r"));
        return;
    }
    if(parametri[1].toInt()>255)
    {
        serviceTcp->txData(QByteArray("VALORE FUORI SCALA!\n\r"));
        return;
    }

    pCollimatore->colliConf.filterTomo[0] = parametri[0].toInt();
    pCollimatore->colliConf.filterTomo[1] = parametri[1].toInt();
    pCollimatore->colliConf.filterTomo[2] = parametri[2].toInt();
    pCollimatore->colliConf.filterTomo[3] = parametri[3].toInt();

    serviceTcp->txData(QByteArray("ESEGUITO: OCCORRE EFFETTUARE IL COMANDO STORE PER COMPLETARE!\n\r"));
    pConfig->updatePCB249U2();
}


void serverDebug::handleSetCalibMirror(QByteArray data)
{
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("setCalibMirror"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("PARAMETRI ERRATI!\n\r"));
        return;
    }

    if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01)
        pCollimatore->colliConf.mirrorSteps_ASSY_01 = parametri[0].toInt();
    else
        pCollimatore->colliConf.mirrorSteps_ASSY_02 = parametri[0].toInt();
    serviceTcp->txData(QString("NEW MIRROR STEP VALUE: %1\n\rDevice driver updated\n\r").arg(parametri[0].toInt()).toAscii());

    // Invio configurazione a dispositivo
    pConfig->updatePCB249U2();

    return;
}


bool serverDebug::mccService(int id, _MccServiceNotify_Code cmd, QByteArray data)
{
    data.prepend((unsigned char) cmd);
    return pConsole->pGuiMcc->sendFrame(MCC_SERVICE,id,(unsigned char*) data.data(), data.size());
}
bool serverDebug::mccService(_MccServiceNotify_Code cmd)
{
    QByteArray data;
    data.prepend((unsigned char) cmd);
    return pConsole->pGuiMcc->sendFrame(MCC_SERVICE,1,(unsigned char*) data.data(), data.size());
}

void serverDebug::handleGetInputs(void)
{
    disconnect(io,SIGNAL(ioUpdated()),this,SLOT(handleGetInputs()));


    return;
}


void serverDebug::handleGetOutputs(void)
{
    disconnect(io,SIGNAL(ioUpdated()),this,SLOT(handleGetOutputs()));


    return;
}

/*
 * Questa funzione forza l'impostazione del PAD come se fosse ricevuto da M4
 */
void serverDebug::handleSetPad(QByteArray data)
{
    int i;
    QByteArray buf;

    data=data.left(data.size()-2);
    data.append(" ");

    i=data.indexOf(":");
    data=data.right(data.size()-i-1);
    i=data.indexOf(" ");
    data=data.left(i);

    serviceTcp->txData(data);

    buf.append((char) pCompressore->comprFlags0);
    buf.append((char) pCompressore->comprFlags1);
    buf.append((char) pCompressore->comprStrenght);
    buf.append((char) pCompressore->breastThick&0xFF);
    buf.append((char) (pCompressore->breastThick>>8) &0xFF);
    buf.append((char) data.toInt());
    buf.append((char) 150);
    pCompressore->pcb215Notify(1,PCB215_NOTIFY_COMPR_DATA,buf);

    serviceTcp->txData(QString("OK %1\r\n").arg((unsigned char) data.toInt()).toAscii());
    return;
}

void serverDebug::handleSetPage(QByteArray data)
{
    int i;
    QByteArray buf;

    data=data.left(data.size()-2);
    data.append(" ");

    i=data.indexOf(":");
    data=data.right(data.size()-i-1);
    i=data.indexOf(" ");
    data=data.left(i);

    serviceTcp->txData(data);
    GWindowRoot.setNewPage(data.toInt(),GWindowRoot.curPage,0);

    return;
}

/*
 *  Questa funzione effettua il tunnel tra Service Terminal e Console
 *  Iniettando il messaggio ricevuto dal Service Terminal sul socket Console
 *  e quindi permettendo di nviare un comando Console da Terminale di servizio
 *  Il messaggio iniettato deve essere compliance con il protocollo della COnsole
 *  Sintax:
 *  console: <......%...%>
 */
void serverDebug::handleConsole(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("--------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("simulateRx <frame>      Simula ricezione comando da AWS \r\n"));
        serviceTcp->txData(QByteArray("getFormat <frame>       Restituisce info sul frame\r\n"));       
        serviceTcp->txData(QByteArray("logString <string>      Logga una stringa\r\n"));
        serviceTcp->txData(QByteArray("pcPowerOff              Richiede spegnimento PC\r\n"));
        serviceTcp->txData(QByteArray("---------------------------------------------------------\r\n"));
    } else if(data.contains("simulateRx "))
    {
        QTextCodec *codec = QTextCodec::codecForName(UNICODE_TYPE);
        pConsole->consoleRxHandler(codec->fromUnicode(data));
    }else if(data.contains("getFormat"))
    {
        protoConsole messaggio(&data);
        if(messaggio.frame_len!=0)
        {
            serviceTcp->txData(QString("FRAME LENGTH:%1\r\n").arg(messaggio.frame_len).toAscii());
            serviceTcp->txData(QString("NUM PARAMETERS:%1\r\n").arg(messaggio.parametri.size()).toAscii());
            for(int i = 0; i< messaggio.parametri.size(); i++){
                serviceTcp->txData(QString("PAR[%1]= %2\r\n").arg(i).arg(messaggio.parametri[i]).toAscii());
            }
        }
        else serviceTcp->txData(QByteArray("MESSAGGIO NON VALIDO\r\n"));
    }else if(data.contains("logString"))
    {
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("logString"));
        if(parametri.size() == 0) {
            serviceTcp->txData(QString("Formato: logString stringa da inviare..\r\n").toAscii());
            return;
        }

        QString stringa="";

        for(int i=0;i<parametri.size();i++) stringa+=parametri[i]+" ";
        LOG(stringa);
        return;

    }else if(data.contains("setMag")){
        ApplicationDatabase.setData(_DB_ACCESSORIO,(unsigned char) POTTER_MAGNIFIER,0);
    }else if(data.contains("set2D")){
        ApplicationDatabase.setData(_DB_ACCESSORIO,(unsigned char) POTTER_2D,0);
    }else if(data.contains("pcPowerOff")){
        pConfig->masterRequestAwsPowerOff();
    }



}

// TO BE DONE: get revisions
void serverDebug::handleGetRevisions(void)
{
    if(pConfig->checkPackage()==true) serviceTcp->txData("REVISIONI CORRETTE\n\r");
    else serviceTcp->txData(pConfig->revisionError.toAscii());
    serviceTcp->txData("\n\r");

}



void serverDebug::trxNotifySlot(unsigned char id, unsigned char cmd, QByteArray data){
    unsigned char buffer[10];

    if(trx_loop==false){
        disconnect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(trxNotifySlot(unsigned char,unsigned char,QByteArray)));
        return;
    }

    if(id!=255) return;
    if(cmd == MCC_CMD_TRX){
        if(trx_loop_stat==false){
            trx_loop_stat=true;
            buffer[0]=TRX_MOVE_END_W; // Comando WHOME
            buffer[1]=MOVE_WAIT_END; // Attende questo comando
            buffer[2]=0;

            // Invio comando
            pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,255,buffer, 3);
        }else {
            trx_loop_stat=false;
            buffer[0]=TRX_MOVE_HOME_W; // Comando WHOME
            buffer[1]=MOVE_WAIT_END; // Attende questo comando
            buffer[2]=0;

            // Invio comando
            pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,255,buffer, 3);
        }
    }
}

/*
 *
 *  Attivazione procedura di rodaggio. Occorre spegnere la macchina per
 *  fermare la pendolazione

    buffer[0] = num cicli
    buffer[1] = angolo
    buffer[2] = velocita: 0 = STD, 1 = WIDE, 2 = NARROW
 */
void serverDebug::handleRodaggioTubo(QByteArray data)
{
    unsigned char buffer[10];
    trx_loop = true;
    trx_loop_stat = false;

    connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(trxNotifySlot(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    buffer[0]=TRX_MOVE_HOME_W; // Comando WHOME
    buffer[1]=MOVE_WAIT_END; // Attende questo comando
    buffer[2]=0;

    // Invio comando
    pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,255,buffer, 3);
    return;


    QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("LOOP"));

    if(parametri.size()!=3){
        serviceTcp->txData("Wrong parameters\n\r");
        return;
    }

    if(parametri[0].toInt()>255){
        serviceTcp->txData("Wrong num cycles\n\r");
        return;
    }
    if(parametri[1].toInt()>26){
        serviceTcp->txData("Wrong Angle\n\r");
        return;
    }
    if((parametri[2]!="N")&&(parametri[2]!="W")&&(parametri[2]!="S")){
        serviceTcp->txData("Wrong Speed\n\r");
        return;
    }

    QByteArray buf;
    buf.append((unsigned char) parametri[0].toInt());
    buf.append((unsigned char) parametri[1].toInt());
    if(parametri[2]=="S"){
        buf.append((char) 0);
    }else if(parametri[2]=="W"){
        buf.append((unsigned char) 1);
    }else if(parametri[2]=="N"){
        buf.append((unsigned char) 2);
    }

    if(mccService(0,SRV_RODAGGIO_TUBO,buf)== FALSE) serviceTcp->txData("MCC FALLITO");
    else serviceTcp->txData(QString("TRX LOOP START: %1 %2 %3\n\r").arg(buf[0]).arg(buf[1]).arg(buf[2]).toAscii());

}


void serverDebug:: handleReadConfig(QByteArray data)
{
    QString stringa;
    int i;
    bool ok;
    unsigned char indirizzo;
    unsigned char uC;
    QString filename;

    data=data.left(data.size()-2);
    data.append(" ");

    i = data.indexOf("CONFIG");
    data = data.right(data.size()-i-7);

    // Cerca Target hex format
    i = data.indexOf(",");
    if(i==-1) return;
    stringa = data.left(i);
    indirizzo = (unsigned char) stringa.toInt(&ok,16);
    data = data.right(data.size()-i-1);

    // Cerca uC Target (1/2)
    i = data.indexOf(" ");
    if(i==-1) return;
    stringa = data.left(i);
    uC = (unsigned char) stringa.toInt(&ok,0);


    if(pLoader->readConfig(indirizzo,uC)==FALSE)
    {
        serviceTcp->txData(QString("FALLITO COMANDO MCC").toAscii());
    }else
    {
        connect(pLoader,SIGNAL(readConfigSgn(_picConfigStr)),this,SLOT(handleReadConfigNotify(_picConfigStr)),Qt::UniqueConnection);
    }
    return;
}


// DATA.AT(0) == RISULTATO
// DATA.AT(1) == OPERAZIONE ESEGUITA
void serverDebug::handleReadConfigNotify(_picConfigStr data)
{

    disconnect(pLoader,SIGNAL(readConfigSgn(_picConfigStr)),this,SLOT(handleReadConfigNotify(_picConfigStr)));
    QString id0=QString::number(data.ID[0],16);
    QString id1=QString::number(data.ID[1],16);
    QString id2=QString::number(data.ID[2],16);
    QString id3=QString::number(data.ID[3],16);
    QString cw=QString::number(data.configWord,16);
    serviceTcp->txData(QString("ID0:"+id0 +" ID1:"+id1 +" ID2:"+id2 +" ID3:"+id3 +" CW:"+cw+ "\n\r").toAscii() );

}


/*
 *   loader: UPLOAD Target, uC, file
 */
void serverDebug:: handleLoaderUpload(QByteArray data)
{


    unsigned char target;
    unsigned char uC;
    QString filename,tag;
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("UPLOAD"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB249U1") {
        target = 0x1E;
        uC = 1;
        tag = "PCB249U1";
        filename = QString("/home/user/FW249U1.hex");
    }else if(parametri[0]=="PCB249U2") {
        target = 0x1E;
        uC = 2;
        tag = "PCB249U2";
        filename = QString("/home/user/FW249U2.hex");
    }else if(parametri[0]=="PCB240") {
        target = 0;
        uC = 1;
        tag = "PCB240";
        filename = QString("/home/user/FW240.hex");
    }else if(parametri[0]=="PCB244") {
        target = 0x1D;
        uC = 1;
        tag = "PCB244";
        filename = QString("/home/user/FW244.hex");
    }else if(parametri[0]=="PCB244A") {
        target = 0x1A;
        uC = 1;
        tag = "PCB244A";
        filename = QString("/home/user/FW244A.hex");
    }else if(parametri[0]=="PCB190") {
        target = 0x1C;
        uC = 1;
        tag = "PCB190";
        filename = QString("/home/user/FW190.hex");
    }else if(parametri[0]=="PCB269") {
        target = 0x1B;
        uC = 1;
        tag = "PCB269";
        filename = QString("/home/user/FW269.hex");
    }else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }


    pLoader->manualFirmwareUpload(target,uC,filename,tag);
    return;
}

void serverDebug:: handleLoaderTestConnection(QByteArray data)
{
    unsigned char target;
    unsigned char uC;
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("TEST"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB249U1") {
        target = 0x1E;
        uC = 1;
    }else if(parametri[0]=="PCB249U2") {
        target = 0x1E;
        uC = 2;
    }else if(parametri[0]=="PCB240") {
        target = 0;
        uC = 1;
    }else if(parametri[0]=="PCB244") {
        target = 0x1D;
        uC = 1;
    }else if(parametri[0]=="PCB244A") {
        target = 0x1A;
        uC = 1;
    }else if(parametri[0]=="PCB190") {
        target = 0x1C;
        uC = 1;
    }else if(parametri[0]=="PCB269") {
        target = 0x1B;
        uC = 1;
    }else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }


    pLoader->manualFirmwareTest(target,uC);
    return;
}



/* Calcola il CRC di un file per il download
 * il file deve essere contenuto nella directory /home/user
 */
void serverDebug::handleGetCRC(QByteArray data){

    data=data.left(data.size()-2);
    data.append(" ");

    int i = data.indexOf("getCRC");
    data = data.right(data.size()-i-7);
    data.replace(" ","");

    QString rev;
    unsigned short crc;
    int ret = pLoader->verifyHeader(data,"",&crc,&rev);
    serviceTcp->txData(QString("FILE:" + data + QString("RETCODE=%1, CRC=%2,").arg(ret).arg(QString::number(crc,16)) + " REV="+rev+"\n\r").toAscii() );
}

void serverDebug::handleSetCRC(QByteArray data){
    QList<QByteArray> parametri;
    QString filename;
    parametri = getNextFieldsAfterTag(data, QString("setCRC"));
    if(parametri.size()!=2)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB249U1") {
        filename = QString("/home/user/FW249U1.hex");
    }else if(parametri[0]=="PCB249U2") {
        filename = QString("/home/user/FW249U2.hex");
    }else if(parametri[0]=="PCB240") {
        filename = QString("/home/user/FW240.hex");
    }else if(parametri[0]=="PCB244") {
        filename = QString("/home/user/FW244.hex");
    }else if(parametri[0]=="PCB244A") {
        filename = QString("/home/user/FW244A.hex");
    }else if(parametri[0]=="PCB190") {
        filename = QString("/home/user/FW190.hex");
    }else if(parametri[0]=="PCB269") {
        filename = QString("/home/user/FW269.hex");
    }else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }

    // Prepara il file
    QString command;
    QString rev;
    unsigned short crc;

    command = QString("echo \"<0x0000,%1>\" > /home/user/tmp").arg(QString(parametri[1]));
    system(command.toStdString().c_str());
    command = QString("cat %1 >> /home/user/tmp").arg(filename);
    system(command.toStdString().c_str());
    int ret = pLoader->verifyHeader("/home/user/tmp","",&crc,&rev);
    QString CRC = QString::number(crc,16);

    command = QString("echo \"<0x%1,%2>\" > /home/user/tmp").arg(CRC).arg(QString(parametri[1]));
    system(command.toStdString().c_str());
    command = QString("cat %1 >> /home/user/tmp").arg(filename);
    system(command.toStdString().c_str());
    command = QString("mv /home/user/tmp %1").arg(filename);
    system(command.toStdString().c_str());

    serviceTcp->txData(QString("Command Executed: CRC:0x%1\n\r").arg(CRC).toAscii());
}

void serverDebug::handleSetRemoteCRC(QByteArray data){
    QList<QByteArray> parametri;
    QString filename;
    parametri = getNextFieldsAfterTag(data, QString("setRemoteCRC"));
    if(parametri.size()!=2)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB249U1") {
        filename = QString("FW249U1.hex");
    }else if(parametri[0]=="PCB249U2") {
        filename = QString("FW249U2.hex");
    }else if(parametri[0]=="PCB240") {
        filename = QString("FW240.hex");
    }else if(parametri[0]=="PCB244") {
        filename = QString("FW244.hex");
    }else if(parametri[0]=="PCB244A") {
        filename = QString("FW244A.hex");
    }else if(parametri[0]=="PCB190") {
        filename = QString("FW190.hex");
    }else if(parametri[0]=="PCB269") {
        filename = QString("FW269.hex");
    }else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }



    // Prepara il file
    QString command;
    QString rev;
    unsigned short crc;

    command = QString("rm /home/user/%1").arg(filename);
    system(command.toStdString().c_str());

    command = QString("/monta.sh");
    system(command.toStdString().c_str());

    command = QString("cp /mnt/target/%1 /home/user/").arg(filename);
    system(command.toStdString().c_str());


    command = QString("echo \"<0x0000,%1>\" > /home/user/tmp").arg(QString(parametri[1]));
    system(command.toStdString().c_str());
    command = QString("cat /home/user/%1 >> /home/user/tmp").arg(filename);
    system(command.toStdString().c_str());
    int ret = pLoader->verifyHeader("/home/user/tmp","",&crc,&rev);
    QString CRC = QString::number(crc,16);

    command = QString("echo \"<0x%1,%2>\" > /home/user/tmp").arg(CRC).arg(QString(parametri[1]));
    system(command.toStdString().c_str());
    command = QString("cat /home/user/%1 >> /home/user/tmp").arg(filename);
    system(command.toStdString().c_str());
    command = QString("mv /home/user/tmp /home/user/%1").arg(filename);
    system(command.toStdString().c_str());


    serviceTcp->txData(QString("Command Executed: CRC:0x%1\n\r").arg(CRC).toAscii());
}

/*

 *
 */
void serverDebug::serviceNotifyFineRaggi(QByteArray buffer)
{
   float vmean;
   unsigned char imean;
   unsigned short iTmed, tpls, mAs;
   QString stringa;

   mAs = buffer.at(MAS_PLS_L) + 256*buffer.at(MAS_PLS_H);
   vmean = pGeneratore->convertPcb190Kv(buffer.at(V_MEAN),1.0);
   imean = pGeneratore->convertPcb190Ianode(buffer.at(I_MEAN));
   tpls = (unsigned short) (buffer.at(T_MEAN_PLS_L)+256*buffer.at(T_MEAN_PLS_H));
   if(tpls!=0) iTmed = mAs * 20 / tpls;
   else iTmed=0;

    switch(buffer.at(RX_END_CODE))
    {
        case 0: stringa.append(QString("MANUAL RX OK:\n\r"));break;
        case ERROR_HV_HIGH: stringa.append(QString("MANUAL RX: KV ALTI\n\r")); break;
        case ERROR_HV_LOW: stringa.append(QString("MANUAL RX: KV BASSI\n\r")); break;
        case ERROR_IA_HIGH:stringa.append(QString("MANUAL RX: I ANODICA ALTA\n\r")); break;
        case ERROR_IA_LOW: stringa.append(QString("MANUAL RX: I ANODICA BASSA\n\r")); break;
        case ERROR_IFIL: stringa.append(QString("MANUAL RX: I-FILAMENT ALTA\n\r")); break;
        case ERROR_TMO_RX: stringa.append(QString("TIMEOUT \n\r")); break;

    default:
        stringa.append(QString("MANUAL RX: ERRORE=%1\n\r").arg((int) buffer.at(RX_END_CODE)));
    break;
    }

    stringa.append(QString("mAs: %1 \n\r").arg((float) mAs/50));
    stringa.append(QString("KV(read): %1(kV) %2(RAW)\n\r").arg(vmean).arg((int) buffer.at(V_MEAN)));
    stringa.append(QString("IA(read): %1(mA) %2(RAW) \n\r").arg((int) imean).arg((int) buffer.at(I_MEAN)));
    stringa.append(QString("T: %1 (ms)\n\r").arg(tpls));
    stringa.append(QString("I(T): %1 (mA) \n\r").arg(iTmed));
    serviceTcp->txData(stringa.toAscii());
}




/*
 */
void serverDebug::handleGeneratore(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("--------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("reloadTube                       Rilegge dati Tubo e configura PCB190 \r\n"));
        serviceTcp->txData(QByteArray("getStatistics [store]            Legge e opzionalmente forza la scrittura\r\n"));
        serviceTcp->txData(QByteArray("getAnodeHU                       Legge gli Hu anodici accumulati\r\n"));
        serviceTcp->txData(QByteArray("--------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("selectFilament [WL/WS/MoL/MoS/OFF]    Imposta il fuoco corrente  \r\n"));
        serviceTcp->txData(QByteArray("activateStarter  [HS/LS/OFF]          Attivazione Starter  \r\n"));
        serviceTcp->txData(QByteArray("--------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("activateManualXray  [tube-model]      Attivazione Modalit� Manuale  \r\n"));
        serviceTcp->txData(QByteArray("closeManualXray                       Chiude Modalit� Manuale  \r\n"));
        serviceTcp->txData(QByteArray("setExposure  Focus kV mAs Idac        Impostazione dati di esposizione  \r\n"));
        serviceTcp->txData(QByteArray("--------------------------------------------------------------\r\n"));
    } else if(data.contains("selectFilament"))
    {
        if(data.contains("WL")){
            pGeneratore->setFuoco(QString("W"));
            pGeneratore->setFuoco(Generatore::FUOCO_LARGE);
        }else if(data.contains("WS")){
            pGeneratore->setFuoco(QString("W"));
            pGeneratore->setFuoco(Generatore::FUOCO_SMALL);
        }else if(data.contains("MoL")){
            pGeneratore->setFuoco(QString("Mo"));
            pGeneratore->setFuoco(Generatore::FUOCO_LARGE);
        }else if(data.contains("MoS")){
            pGeneratore->setFuoco(QString("Mo"));
            pGeneratore->setFuoco(Generatore::FUOCO_SMALL);
        }else {
            pGeneratore->fuocoOff();
            serviceTcp->txData(QByteArray("Filament set to OFF!\r\n"));
        }

        pGeneratore->updateFuoco();
        serviceTcp->txData(QByteArray("Command executed!\r\n"));
        return;

    }else if(data.contains("activateStarter"))
    {
        if(data.contains(" HS")){
            pGeneratore->setStarter(2);
            serviceTcp->txData(QString("Starter: HS mode\r\n").toAscii());
        }else if(data.contains(" LS")){
            pGeneratore->setStarter(1);
            serviceTcp->txData(QString("Starter: LS mode\r\n").toAscii());
        }else if(data.contains(" OFF")){
            pGeneratore->setStarter(0);
            serviceTcp->txData(QString("Starter: OFF\r\n").toAscii());
        }

    }else if(data.contains("getStatistics")){
        pGeneratore->statChanged = true;

        QString tube = QString(_TUBEPATH) + QString("/") + pConfig->userCnf.tubeFileName + QString("/");
        pGeneratore->saveTubeStatisticsFile(tube);
        pGeneratore->readTubeStatisticsFile(tube);

        serviceTcp->txData(QString("TUBE: %1\r\n").arg(pConfig->userCnf.tubeFileName).toAscii());
        serviceTcp->txData(QString("NSHOTS: %1\r\n").arg(pGeneratore->numShots).toAscii());
        serviceTcp->txData(QString("kHU: %1\r\n").arg((float) pGeneratore->cumulatedJ * 1.33 / 1000).toAscii());
        serviceTcp->txData(QString("N-TOMO: %1\r\n").arg(pGeneratore->nTomo).toAscii());
        serviceTcp->txData(QString("N-STANDARD: %1\r\n").arg(pGeneratore->nStandard).toAscii());
    }else if(data.contains("reloadTube"))
    {
        if(pGeneratore->openCurrentTube()){
            serviceTcp->txData("Command executed: configuration in progress ..\r\n");
            connect(pConfig,SIGNAL(configUpdatedSgn()),this,SLOT(configurationSlot()),Qt::UniqueConnection);
            pConfig->updatePCB190();
        }else serviceTcp->txData("Command failed!\r\n");
    }else if(data.contains("getAnodeHU")){
        serviceTcp->txData(QString("Current Anode HU:%1\r\n").arg((float)pGeneratore->getCurrentHU()).toAscii());
        return;
    }else if(data.contains("activateManualXray")){

        pGeneratore->manSelectedTube = pConfig->userCnf.tubeFileName;
        QString tubeDir;

        if((data.contains("35KV")) || (data.contains("35kV"))) tubeDir = QString(_TUBEPATH) + QString("/") + "TEMPLATE_35KV_XM1016THI" + QString("/");
        else if((data.contains("49KV")) || (data.contains("49kV"))) tubeDir = QString(_TUBEPATH) + QString("/") + "TEMPLATE_XM1016THI" + QString("/");
        else{
            serviceTcp->txData(QString("Wrong Template file type. Expected: 35KV or 49KV \r\n").toAscii());
            return;
        }

        if(!pGeneratore->openTube(tubeDir)) {
            pGeneratore->openTube(pGeneratore->manSelectedTube);
            serviceTcp->txData(QString("Wrong Template file selected\r\n").toAscii());
            return;
        }

        // Activates the modality
        pGeneratore->manualMode = true;
        pGeneratore->manFuoco = "WL";
        pGeneratore->manVnom = 20;
        pGeneratore->manIdac = 2000;
        pGeneratore->manMas = 10;

        // Open Study
        pConsole->setOpenStudy(true , "MANUAL EXPOSURE WITH IRS");
        connect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),this,SLOT(fineRaggiManualXray(QByteArray)),Qt::UniqueConnection);

        return;
    }else if(data.contains("closeManualXray")){
         if(!pGeneratore->manualMode){
             serviceTcp->txData(QString("Command Error! The session is already closed!\r\n").toAscii());
             return;
         }

         disconnect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),this,SLOT(fineRaggiManualXray(QByteArray)));
         pGeneratore->manualMode = false;
         pGeneratore->openTube(pGeneratore->manSelectedTube);
         pConsole->handleCloseStudy();
         serviceTcp->txData(QString("Manual XRAY session Closed!\r\n").toAscii());
         return;
    }else if(data.contains("setExposure")){
        if(!pGeneratore->manualMode){
            serviceTcp->txData(QString("Command Error! The session is closed!\r\n").toAscii());
            return;
        }

        QList<QByteArray> parametri;
        parametri = getNextFieldsAfterTag(data, QString("setExposure"));
        if(parametri.size() != 4){
            serviceTcp->txData(QString("Wrong number of expected parameters: Focus, kV, mAs, Idac!\r\n").toAscii());
            return;
        }

        if( (parametri[0] != "WL") && (parametri[0] != "WS") && (parametri[0] != "MoL") && (parametri[0] != "MoS")) {
            serviceTcp->txData(QString("Wrong Focus parameter. Expected: WS or WL or MoS or MoL\r\n").toAscii());
            return;
        }

        pGeneratore->manFuoco = parametri[0];

        int kV = parametri[1].toInt();
        if((kV < 20) || (kV>49)){
            serviceTcp->txData(QString("Wrong kV parameter. Expected range is 20 kV to 49 kV \r\n").toAscii());
            return;
        }
        pGeneratore->manVnom = kV;
        pGeneratore->manMas = parametri[2].toInt();
        if(parametri[3].toInt() > 2500){
            serviceTcp->txData(QString("Wrong Idac parameter. Expected range is < 2500 \r\n").toAscii());
            return;
        }
        pGeneratore->manIdac = parametri[3].toInt();

        serviceTcp->txData(QString("Manual Exposure data confirmed! \r\n").toAscii());
        return;
   }
}

void serverDebug::configurationSlot(void){
    disconnect(pConfig,SIGNAL(configUpdatedSgn()),this,SLOT(configurationSlot()));
    serviceTcp->txData("Configuration completed!\r\n");
}

QByteArray serverDebug::getNextFieldAfterTag(QByteArray data, QString tag)
{
    int i;
    QByteArray risultato;

    risultato.clear();

    i = data.indexOf(tag);
    data = data.right(data.size()-i-tag.size());

    // Cerca l'inizio
    for(i=0; i<data.size(); i++) if(data.at(i)!=' ') break;

    // Compila fino alla fine
    for(i=0; i<data.size(); i++)
    {
        if(data.at(i)==' ') return risultato;
        if(data.at(i)==',') return risultato;
        if(data.at(i)=='\n') return risultato;
        if(data.at(i)=='\r') return risultato;
        risultato.append(data.at(i));
    }

    return risultato;
}

QList<QByteArray> serverDebug::getNextFieldsAfterTag(QByteArray data, QString tag)
{
    int i;
    QList<QByteArray> risultato;
    QByteArray item;

    risultato.clear();

    i = data.indexOf(tag);
    if(i==-1) return risultato;

    data = data.right(data.size()-i-tag.size());

    // Cerca l'inizio
    for(i=0; i<data.size(); i++) if(data.at(i)!=' ') break;


    // Compila fino alla fine
    item.clear();
    for(i=0; i<data.size(); i++)
    {
        if((data.at(i)==' ') || (data.at(i)==','))
        {
            // Termine parametro
            if(!item.isEmpty()) risultato.append(item);
            item.clear();
        }else if((data.at(i)=='\n') || (data.at(i)=='\r'))
        {
            // Termine sequenza
            if(item.isEmpty()) return risultato;
            else
            {
                risultato.append(item);
                return risultato;
            }
        }else item.append(data.at(i));

    }

    if(item.isEmpty()) return risultato;
    else
    {
        risultato.append(item);
        return risultato;
    }

}

void serverDebug::setManualLameVal(QString lama, int val)
{
    if(lama=="L")         pCollimatore->manualL = val;
    else if(lama=="R")    pCollimatore->manualR = val;
    else if(lama=="B")    pCollimatore->manualB = val;
    else if(lama=="F")    pCollimatore->manualF = val;
    else if(lama=="T")    pCollimatore->manualT = val;
    return;
}


void serverDebug::handleRotazioni(QByteArray data)
{
    QByteArray field;
    int angolo;
    unsigned char buffer[3];


    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("rotazioni: ---------- Comandi relativi alla gestione bracci -----------------\r\n"));
        serviceTcp->txData(QByteArray("TRX STOP      Ferma Immediatamente TRX  \r\n"));
        serviceTcp->txData(QByteArray("TRX WHOME     Muove tubo a Wide Home  \r\n"));
        serviceTcp->txData(QByteArray("TRX NHOME     Muove tubo a Narrow Home  \r\n"));
        serviceTcp->txData(QByteArray("TRX IHOME     Muove tubo a Intermediate Home  \r\n"));
        serviceTcp->txData(QByteArray("TRX WEND      Muove tubo a Wide End  \r\n"));
        serviceTcp->txData(QByteArray("TRX NEND      Muove tubo a Wide End  \r\n"));
        serviceTcp->txData(QByteArray("TRX IEND      Muove tubo a Intermediate End  \r\n"));
        serviceTcp->txData(QByteArray("TRX LOOP val  Attiva la procedura di rodaggio tubo\r\n"));
        serviceTcp->txData(QByteArray("TRX [angolo]  Muove tubo a Angolo (+/-)  \r\n"));
        serviceTcp->txData(QByteArray("ARM [angolo]  Muove braccio a Angolo (+/-)  \r\n"));
        serviceTcp->txData(QByteArray("resetGonio    Reset Inclinometri\r\n"));
        serviceTcp->txData(QByteArray("getGonio      Legge inclinometro\r\n"));
        serviceTcp->txData(QByteArray("readTrxConfig Rilegge TRX config e download\r\n"));
        serviceTcp->txData(QByteArray("saveTrxConfig Salva il TRX config \r\n"));
        serviceTcp->txData(QByteArray("readArmConfig Rilegge ARM config e download\r\n"));
        serviceTcp->txData(QByteArray("saveArmConfig Salva il ARM config \r\n"));
        serviceTcp->txData(QByteArray("readLenzeConfig Rilegge Lenze config e download\r\n"));
        serviceTcp->txData(QByteArray("saveLenzeConfig Salva il Lenze config \r\n"));
        serviceTcp->txData(QByteArray("setRotManualMode [ARMS|ARMC|TRXS|TRXC] \r\n"));
        serviceTcp->txData(QByteArray("resetBusy    reset the busy condition \r\n"));
        serviceTcp->txData(QByteArray("pollingSlave trigger a can bus polling \r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    }else if(data.contains("setRotManualMode")){
        QList<QByteArray> parametri;
        unsigned char buffer[2];
        parametri = getNextFieldsAfterTag(data, QString("setRotManualMode"));
        if(parametri.size()!=1)
        {
            serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
            return;
        }

        if(parametri[0]=="ARMS") buffer[0] = CALIB_ZERO_MANUAL_ACTIVATION_ARM_STANDARD;
        else if(parametri[0]=="ARMC") buffer[0] = CALIB_ZERO_MANUAL_ACTIVATION_ARM_CALIB;
        else if(parametri[0]=="TRXC") buffer[0] = CALIB_ZERO_MANUAL_ACTIVATION_TRX_CALIB;
        else if(parametri[0]=="TRXS") buffer[0] = CALIB_ZERO_MANUAL_ACTIVATION_TRX_STANDARD;
        else {
             serviceTcp->txData(QByteArray("wrong type\n\r"));
             return;
        }

        pConsole->pGuiMcc->sendFrame(MCC_CALIB_ZERO,0,buffer, sizeof(buffer));
    }else if(data.contains("STOP"))
    {
        buffer[0]=TRX_MOVE_STOP; // Comando WHOME
        buffer[1]=0; // Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }
    else if(data.contains("TRX WHOME"))
    {
        buffer[0]=TRX_MOVE_HOME_W; // Comando WHOME
        buffer[1]=MOVE_WAIT_END; // Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }else if(data.contains("TRX NHOME"))
    {
        buffer[0]=TRX_MOVE_HOME_N;
        buffer[1]=MOVE_WAIT_END; // Non attende comando precedente + Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }else if(data.contains("TRX IHOME"))
    {
        buffer[0]=TRX_MOVE_HOME_I;
        buffer[1]=MOVE_WAIT_END; // Non attende comando precedente + Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }else if(data.contains("TRX WEND"))
    {
        buffer[0]=TRX_MOVE_END_W ;
        buffer[1]=MOVE_WAIT_END; // Non attende comando precedente + Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }else if(data.contains("TRX NEND"))
    {
        buffer[0]=TRX_MOVE_END_N;
        buffer[1]=MOVE_WAIT_END; // Non attende comando precedente + Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }else if(data.contains("TRX IEND"))
    {
        buffer[0]=TRX_MOVE_END_I;
        buffer[1]=MOVE_WAIT_END; // Non attende comando precedente + Attende questo comando
        buffer[2]=0;
        // Invio comando
        pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, 3);

    }else if(data.contains("TRX LOOP"))
    {
        if(trx_loop){
            trx_loop = false;
            disconnect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(trxNotifySlot(unsigned char,unsigned char,QByteArray)));
            serviceTcp->txData(QByteArray("TRX LOOP STOP\n\r"));
            return;
        }

        serviceTcp->txData(QByteArray("TRX LOOP START\n\r"));
        handleRodaggioTubo(data);
    } else if(data.contains("TRX"))
    {
        handleMoveTrx("TRX ", data);

    } else if(data.contains("ARM"))
    {
        handleMoveArm("ARM ", data);

    }else if(data.contains("getGonio")){

        if(pConsole->pGuiMcc->sendFrame(MCC_GET_GONIO,1,NULL, 0)==FALSE){
            serviceTcp->txData(QByteArray("COMANDO FALLITO!\n\r"));
            return;
        }
        serviceTcp->txData(QByteArray("ATTESA DATI..\n\r"));
        connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(getGonioNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    }else if(data.contains("resetGonio"))
    {
        serviceTcp->txData(QByteArray("GONIO RESETTING ..."));
        if(pConsole->pGuiMcc->sendFrame(MCC_RESET_GONIO,1,0, 0)==FALSE)
        {
            serviceTcp->txData(QByteArray("FALLITO!\n\r"));
            return;
        }
        connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(resetGonioNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    }else if(data.contains("readTrxConfig")){
        pConfig->readTrxConfig();
        pConfig->updateTrxDriver();
        serviceTcp->txData(QByteArray("FILE DI CONFIGURAZIONE TRX LETTO\n\r"));
        return;
    }else if(data.contains("saveTrxConfig")){
        pConfig->saveTrxConfig();
        serviceTcp->txData(QByteArray("TRX config file Stored!\n\r"));
    }else if(data.contains("readArmConfig")){
        pConfig->readArmConfig();
        pConfig->updateArmDriver();
        serviceTcp->txData(QByteArray("FILE DI CONFIGURAZIONE ARM LETTO\n\r"));
        return;
    }else if(data.contains("saveArmConfig")){
        pConfig->saveArmConfig();
        serviceTcp->txData(QByteArray("ARM config file Stored!\n\r"));
    }else if(data.contains("readLenzeConfig")){
        pConfig->readLenzeConfig();
        pConfig->updateLenzeDriver();
        serviceTcp->txData(QByteArray("FILE DI CONFIGURAZIONE LENZE LETTO\n\r"));
        return;
    }else if(data.contains("saveLenzeConfig")){
        pConfig->saveLenzeConfig();
        serviceTcp->txData(QByteArray("LENZE config file Stored!\n\r"));
    }else if(data.contains("resetBusy")){
        serviceTcp->txData(QByteArray("Busy RESETTING ..."));

        // Impostazione Parametro
        buffer[0] =(unsigned char) 1;
        pConsole->pGuiMcc->sendFrame(MCC_TEST,0,buffer, 1);

    }else if(data.contains("pollingSlave")){
        serviceTcp->txData(QByteArray("POLLING ..."));

        // Impostazione Parametro
        buffer[0] =(unsigned char) 2;
        pConsole->pGuiMcc->sendFrame(MCC_TEST,0,buffer, 1);
    }
}



/*
  Muove Tubo ad angolo
  - data è la stringa di comando
  - tag indica la stringa oltre la quale va cercato il parametro
*/
void serverDebug::handleMoveTrx(QString tag, QByteArray data)
{
    int angolo;
    unsigned char buffer[4];

    angolo =  getNextFieldAfterTag(data,tag).toInt(); // Espresso in gradi o centesimi di grado

    buffer[0]=TRX_MOVE_ANGLE;
    buffer[2] = (unsigned char) angolo;
    buffer[3] = (unsigned char) (angolo>>8);

    pConsole->pGuiMcc->sendFrame(MCC_CMD_TRX,0,buffer, sizeof(buffer));

}

/*
  Muove Tubo ad angolo

  Vengono controllati per sicurezza i limiti di rotazione

*/
void serverDebug::handleMoveArm(QString tag, QByteArray data)
{
    int target;
    unsigned char buffer[3];

    target = getNextFieldAfterTag(data,tag).toInt();
    if(target > 180) target = 180;
    else if(target<-180) target = -180;

    // Impostazione Parametro
    buffer[0] =(unsigned char) (target&0xFF);
    buffer[1] =(unsigned char) (target>>8);

    // Invio comando
    pConsole->pGuiMcc->sendFrame(MCC_CMD_ARM,0,buffer, 2);
}

void serverDebug::resetGonioNotify(unsigned char id, unsigned char mcccode, QByteArray buffer)
{
    if(id!=1) return;
    if(mcccode!=MCC_RESET_GONIO) return;

    disconnect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(resetGonioNotify(unsigned char,unsigned char,QByteArray)));
    if(buffer.size()!=0)


    if(buffer.at(0)==1) serviceTcp->txData(QByteArray("Completato con successo \n\r"));
    else serviceTcp->txData(QByteArray("Fallito reset inclinometro PCB249 \n\r"));
}

void serverDebug::getGonioNotify(unsigned char id, unsigned char mcccode, QByteArray buffer)
{

    // Filtra parte dei possibili ID
    if(id!=1) return;
    if(mcccode!=MCC_GET_GONIO) return;

    disconnect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(getGonioNotify(unsigned char,unsigned char,QByteArray)));


    short arm = buffer[0]+256*buffer[1]; // Espressi in decimi di grado
    short trx = buffer[2]+256*buffer[3]; // Espressi in centesimi di grado
    short gonio = buffer[4]+256*buffer[5]; // // Espressi in decimi di grado

    serviceTcp->txData(QString("TRX:(c)%1, ARM:(d)%2, GONIO:(d)%3\n\r").arg(trx).arg(arm).arg(gonio).toAscii());
}

/*
 *  Gestione delle funzioni legate al Collimatore
 *  FUNZIONI:
 *
 */
void serverDebug::handleCollimatore(QByteArray data)
{
    QByteArray field;

    // HELP
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("--------- CONFIGURAZIONE -------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("readColliConf     Rilegge il file di configurazione  \r\n"));
        serviceTcp->txData(QByteArray("STORE             Salva i dati di collimazione nel file di configurazione\r\n"));
        serviceTcp->txData(QByteArray("setCalib2D <pad,Mat,L,R,F,B,T> Impostazione collimazioni pad\r\n"));
        serviceTcp->txData(QByteArray("setCalibCustom <L,R,F,B,T> Impostazione collimazioni custom\r\n"));
        serviceTcp->txData(QByteArray("setCalibTomo   <Mat,Lama,P/N, n0..n25> <Mat,Front,Back>\r\n"));
        serviceTcp->txData(QByteArray("getCalib         Restituisce tutta la calibrazione\r\n"));
        serviceTcp->txData(QByteArray("download         Aggiorna configurazione su periferica\r\n"));


        serviceTcp->txData(QByteArray("\r\n--- MODALITA COLLIMATORE  -------------------------\r\n"));
        serviceTcp->txData(QByteArray("getStatus  Restituisce la collimazione correntemente impostata\r\n"));
        serviceTcp->txData(QByteArray("setAuto           Imposta la modalita' di collimazione Automatica\r\n"));
        serviceTcp->txData(QByteArray("setManual         Imposta modalit�  di collimazione Manuale\r\n"));
        serviceTcp->txData(QByteArray("setL  val _____ Imposta la lama sinistra della collimazione corrente   \r\n"));
        serviceTcp->txData(QByteArray("setR  val _____ Imposta la lama destra della collimazione corrente  \r\n"));
        serviceTcp->txData(QByteArray("setT  val _____ Imposta il trapezio della collimazione corrente  \r\n"));
        serviceTcp->txData(QByteArray("setB  val _____ Imposta la lama posteriore della collimazione corrente  \r\n"));
        serviceTcp->txData(QByteArray("setF  val _____ Imposta la lama frontale della collimazione corrente  \r\n"));
        serviceTcp->txData(QByteArray("update    _____ Effettua la collimazione con i valori su elencati  \r\n"));

        serviceTcp->txData(QByteArray("\r\n--- COMANDI FILTRO -------------------------\r\n"));
        serviceTcp->txData(QByteArray("setFiltro [Al/Rh/Ag/Us]    Imposta il filtro\r\n"));
        serviceTcp->txData(QByteArray("setCalibTomoFiltro <ENA,ang0,ang1,ang2>    Imposta gli angoli di incremento filtro\r\n"));
        serviceTcp->txData(QByteArray("setCalibFiltro <Filtro,val>    Modifica il valore di calibrazione del filtro\r\n"));

        serviceTcp->txData(QByteArray("--- COMANDI SPECCHIO/LUCE  -------------------------\r\n"));
        serviceTcp->txData(QByteArray("setMirror [HOME/OUT]  Imposta lo specchio\r\n"));
        serviceTcp->txData(QByteArray("setLamp [ON/OFF]  Accende /spegne la lampada\r\n"));
        serviceTcp->txData(QByteArray("setCalibMirror                 Modifica gli steps in campo per lo specchio  \r\n"));

        serviceTcp->txData(QByteArray("--- COMANDI DI SERVIZIO   -------------------------\r\n"));
        serviceTcp->txData(QByteArray("setTrx val      Muove Tubo ad angolo (copia del comando in rotazioni)\r\n"));
        serviceTcp->txData(QByteArray("testColli val   Muove ciclicamente il formato tra open e CLOSED per Val volte\r\n"));
        serviceTcp->txData(QByteArray("-------------------------------------------------------------------------------\r\n"));
    }

    if(data.contains("download"))
    {
        pConfig->updatePCB249U1();
        return;
    }

    if(data.contains("testColli"))
    {
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("testColli"));

        if(parametri.size() == 0) {
            QString stringa = QString("MISSING NUMBER OF TEST\r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }
        pCollimatore->startColliTest(parametri[0].toInt());
        serviceTcp->txData(QByteArray("COLLI TEST STARTED\r\n"));
        return;
    }

    if(data.contains("readColliConf"))
    {
        pCollimatore->readConfigFile();
        pConfig->updatePCB249U1();
        return;
    }

    if(data.contains("getCalib"))
    {
        handleGetCalib(data);
        return;
    }

    if(data.contains("setCalibTomoFiltro"))
    {
        handleSetCalibTomoFiltro(data);
        return;
    }

    if(data.contains("setCalibTomo"))
    {
        handleSetCalibTomo(data);
        return;
    }


    if(data.contains("setAuto")) // Imposta la modalit�  AUTOMATICA
    {
        serviceTcp->txData(QByteArray("---COLLIMAZIONE AUTOMATICA-----\r\n"));
        pCollimatore->manualCollimation=FALSE;
        pCollimatore->manualFiltroCollimation=FALSE;
        pCollimatore->setFiltro();
        pCollimatore->updateColli();
         return;
    }

    if(data.contains("getStatus")) // Richiede la collimazione corrente
    {
        if(pCollimatore->colli_model == _COLLI_TYPE_NOT_ASSIGNED){
            serviceTcp->txData(QByteArray("COLLI MODEL: NOT DETECTED\r\n"));
        }else if(pCollimatore->colli_model == _COLLI_TYPE_ASSY_01){
            serviceTcp->txData(QByteArray("COLLI MODEL: ASSY 01\r\n"));
        }else serviceTcp->txData(QByteArray("COLLI MODEL: ASSY 02\r\n"));

        if(pCollimatore->manualCollimation==FALSE)
        {
            serviceTcp->txData(QByteArray("COLLIMAZIONE LAME: AUTOMATICA \r\n"));

        }else
        {
            serviceTcp->txData(QByteArray("---COLLIMAZIONE MANUALE-----\r\n"));            
            serviceTcp->txData(QString("LAMA-L:%1\n\r").arg(pCollimatore->manualL).toAscii());
            serviceTcp->txData(QString("LAMA-R:%1\n\r").arg(pCollimatore->manualR).toAscii());
            serviceTcp->txData(QString("LAMA-T:%1\n\r").arg(pCollimatore->manualT).toAscii());
            serviceTcp->txData(QString("LAMA-B:%1\n\r").arg(pCollimatore->manualB).toAscii());
            serviceTcp->txData(QString("LAMA-F:%1\n\r").arg(pCollimatore->manualF).toAscii());

        }

        if(pCollimatore->manualFiltroCollimation==FALSE){
            serviceTcp->txData(QByteArray("COLLIMAZIONE FILTRO: AUTOMATICA \r\n"));
        }else{
            serviceTcp->txData(QString("FILTRO MANUALE:").append(pCollimatore->getFilterTag(pCollimatore->manualFilter)).append("\n\r").toAscii());
        }


        return;
    }

    if(data.contains("setManual")) // Imposta la modalit�  MANUALE di collimazione
    {
        pCollimatore->manualCollimation=TRUE;
        serviceTcp->txData(QByteArray("MODO MANUALE ATTIVATO\n\r"));
        return;
    }


    if(data.contains("update"))
    {
        pCollimatore->manualColliUpdate();
        return;
    }

    if(data.contains("setFiltro"))
    {
         pCollimatore->manualFiltroCollimation=TRUE;
        if(data.contains(" Al")) pCollimatore->manualFilter = Collimatore::FILTRO_Al;
        else if(data.contains(" Ag")) pCollimatore->manualFilter = Collimatore::FILTRO_Ag;
        else if(data.contains(" Rh")) pCollimatore->manualFilter = Collimatore::FILTRO_Rh;
        else if(data.contains(" Cu")) pCollimatore->manualFilter = Collimatore::FILTRO_Cu;
        else if(data.contains(" Mo")) pCollimatore->manualFilter = Collimatore::FILTRO_Mo;
        pCollimatore->manualSetFiltro();
        return;
    }

    if(data.contains("setCalibFiltro"))
    {

        handleSetCalibFiltro(data);
        return;
    }

    if(data.contains("STORE"))
    {

        pCollimatore->storeConfigFile();
        serviceTcp->txData(QString("DATI DI COLLIMAZIONE SALVATI IN CONFIGURAZIONE\n\r").toAscii());
        return;
    }
    if(data.contains("setMirror"))
    {
        if(data.contains("HOME")) pCollimatore->setMirror(Collimatore::MIRROR_HOME);
        else if(data.contains("OUT")) pCollimatore->setMirror(Collimatore::MIRROR_OUT);
        return;
    }

    if(data.contains("setCalibMirror"))
    {
        handleSetCalibMirror(data);
        return;
    }

    if(data.contains("setLamp"))
    {
        if(data.contains("ON")) pCollimatore->setLamp(Collimatore::LAMP_ON,20);
        else if(data.contains("OFF")) pCollimatore->setLamp(Collimatore::LAMP_OFF,0);
        return;
    }

    if(data.contains("setCalib2D"))
    {
        handleSetCalib2D(data);
        return;
    }
    if(data.contains("setCalibCustom"))
    {
        handleSetCalibCustom(data);
        return;
    }

    if(data.contains("setTrx"))
    {
        handleMoveTrx("setTrx ", data);
        return;
    }


    // Imposta le lame ma solo in modalit�  manuale
    if(pCollimatore->manualCollimation==TRUE)
    {
        if(data.contains("setL")) // Imposta le lame correnti
        {
            field = getNextFieldAfterTag(data,"setL ");
            serviceTcp->txData(QString("LAMA SINISTRA:%1 \r\n").arg(field.toInt()).toAscii());
            setManualLameVal("L",field.toInt());

        }else if(data.contains("setR")) // Imposta le lame correnti
        {
            field = getNextFieldAfterTag(data,"setR ");
            serviceTcp->txData(QString("LAMA DESTRA:%1 \r\n").arg(field.toInt()).toAscii());
            setManualLameVal("R",field.toInt());            

        }else if(data.contains("setT")) // Imposta le lame correnti
        {
            field = getNextFieldAfterTag(data,"setT ");
            serviceTcp->txData(QString("LAMA TRAPEZIO:%1 \r\n").arg(field.toInt()).toAscii());
            setManualLameVal("T",field.toInt());            

        }else if(data.contains("setB")) // Imposta le lame correnti
        {
            field = getNextFieldAfterTag(data,"setB ");
            serviceTcp->txData(QString("LAMA POSTERIORE:%1 \r\n").arg(field.toInt()).toAscii());
            setManualLameVal("B",field.toInt());            

        }else if(data.contains("setF")) // Imposta le lame correnti
        {
            field = getNextFieldAfterTag(data,"setF ");
            serviceTcp->txData(QString("LAMA FRONTALE:%1 \r\n").arg(field.toInt()).toAscii());
            setManualLameVal("F",field.toInt());           
        }
    }



}



/*
 *  Funzioni legate all'interazione con i drivers
 */
#define SER422_COMMAND  0x0
#define SER422_SPECIAL  0x40
#define SER422_READ     0x80
#define SER422_WRITE    0xC0
void serverDebug::handleDrivers(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("------------ Comandi diretti ai drivers ------------\r\n"));
        serviceTcp->txData(QByteArray("freeze                       Blocca ciclo automatico\r\n"));
        serviceTcp->txData(QByteArray("run                          Attiva ciclo automatico\r\n"));
        serviceTcp->txData(QByteArray("read8 <target,addr>          Legge indirizzo 8 bit\r\n"));
        serviceTcp->txData(QByteArray("read16 <target,addr>         Legge indirizzo 16 bit\r\n"));
        serviceTcp->txData(QByteArray("write8 <target,addr,val>     Scrive indirizzo 8 bit\r\n"));
        serviceTcp->txData(QByteArray("write16 <target,addr,val>    Scrive indirizzo 16 bit\r\n"));
        serviceTcp->txData(QByteArray("command <target,b1,b2>       Scrive frame di comando\r\n"));
        serviceTcp->txData(QByteArray("special <target,b1,b2>       Scrive frame speciale\r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("register <tag>               Legge un registro predefinito\r\n"));
        serviceTcp->txData(QByteArray("reglist  <filtro>            Lista dei registri pre definiti\r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("serCommTest <target>         Communication test with target device\r\n"));
        serviceTcp->txData(QByteArray("loaderCommTest <target>      Communication test with loader target device\r\n"));
        serviceTcp->txData(QByteArray("intercommTest  <cycles>      Internal Communication test \r\n"));

    }
    else if(data.contains("freeze")) handleDriverFreeze(TRUE);
    else if(data.contains("run")) handleDriverFreeze(FALSE);
    else if(data.contains("read8")) handleDriverRead8(data);
    else if(data.contains("read16")) handleDriverRead16(data);
    else if(data.contains("write8")) handleDriverWrite8(data);
    else if(data.contains("write16")) handleDriverWrite16(data);
    else if(data.contains("command")) handleDriverCommand(data);
    else if(data.contains("special")) handleDriverSpecial(data);
    else if(data.contains("register")) handleDriversReadReg(data);
    else if(data.contains("reglist")) handleDriversTagList(data);
    else if(data.contains("serCommTest")) handleDriversCommTest(data);
    else if(data.contains("loaderCommTest")) handleLoaderCommTest(data);
    else if(data.contains("intercommTest")) handleIntercommTest(data);
}


void serverDebug::handleIntercommTest(QByteArray data)
{
    static int cicli=0;
    QByteArray buffer;
    QList<QByteArray> parametri;

    if(data.size()==0){
        if(cicli==0){
            serviceTcp->txData(QByteArray("INTERCOMM TEST COMPLETED\n\r"));
            disconnect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)));
        }
        else if(mccService(1,SRV_TEST_INTERPROCESS,buffer)== FALSE){
            serviceTcp->txData("INTERCOMM TEST FAILED");
            disconnect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)));
        }else  cicli--;
        return;
    }

    parametri = getNextFieldsAfterTag(data, QString("intercommTest"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    cicli  = parametri[0].toInt();
    if(mccService(1,SRV_TEST_INTERPROCESS,buffer)== FALSE) serviceTcp->txData("INTERCOMM TEST FAILED");
    else{
        serviceTcp->txData(QByteArray("INTERCOMM TEST STARTED\n\r"));
        connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    }

}

void serverDebug::handleLoaderCommTest(QByteArray data)
{
    unsigned char target;
    unsigned char uC;
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("loaderCommTest"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB249U1") {
        target = 0x1E;
        uC = 1;
    }else if(parametri[0]=="PCB249U2") {
        target = 0x1E;
        uC = 2;
    }else if(parametri[0]=="PCB240") {
        target = 0;
        uC = 1;
    }else if(parametri[0]=="PCB244") {
        target = 0x1D;
        uC = 1;
    }else if(parametri[0]=="PCB244A") {
        target = 0x1A;
        uC = 1;
    }else if(parametri[0]=="PCB190") {
        target = 0x1C;
        uC = 1;
    }else if(parametri[0]=="PCB269") {
        target = 0x1B;
        uC = 1;
    }else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }


    pLoader->manualFirmwareTest(target,uC);
    return;
}
void serverDebug::handleDriversCommTest(QByteArray data)
{
    QByteArray buffer;
    unsigned char target;
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("serCommTest"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }

    // Costruisce il buffer
    buffer.append(target);

    if(mccService(1,SRV_TEST_422,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else{
        serviceTcp->txData(QByteArray("Test attivato\n\r"));
        connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
    }
    //else

    return;

}

void serverDebug::handleDriversTagList(QByteArray data)
{
    QList<QByteArray> parametri;
    QString filtro;

    parametri = getNextFieldsAfterTag(data, QString("reglist"));
    if(parametri.size()==1)
    {
        filtro = parametri[0];
    }else filtro ="";


    QString stringa;
    serviceTcp->txData(QByteArray("PRE-DEFINED REGISTE LIST:\n\r"));
    for(int ciclo=0; ciclo < deviceRegisterList.size(); ciclo++){
        if(filtro!=""){
            if(!deviceRegisterList.at(ciclo).tag.toAscii().contains(filtro.toAscii().data())) continue;
        }
        stringa = "- "+ deviceRegisterList.at(ciclo).tag + " > " + deviceRegisterList.at(ciclo).comment + "\r\n";
        serviceTcp->txData(stringa.toAscii());
    }
}

void serverDebug::handleDriversReadReg(QByteArray data)
{
    QByteArray buffer;

    isCommand = false;
    isSpecial = false;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("register"));
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    // Cerca il tag nella lista
    int ciclo;
    for(ciclo=0; ciclo < deviceRegisterList.size(); ciclo++){
        if(parametri[0] == deviceRegisterList.at(ciclo).tag) {
            break;
        }
    }

    if(ciclo == deviceRegisterList.size()){
         serviceTcp->txData(QByteArray("no register tag matched\n\r"));
         return;
    }

    frameTarget = deviceRegisterList.at(ciclo).tag;


    // Costruisce il buffer
    frameD0 = (unsigned char) (deviceRegisterList.at(ciclo).deviceId | SER422_READ);
    buffer.append(frameD0);
    frameD1 = (unsigned char) (unsigned char) (0x00FF & deviceRegisterList.at(ciclo).address);
    if(deviceRegisterList.at(ciclo).address > 255) frameD2 = 1;
    else frameD2 = 0;
    buffer.append(frameD1);
    buffer.append(frameD2);

    if(deviceRegisterList.at(ciclo).data_type==_8bit) frameFormat16 = false;
    else frameFormat16 = true;

    frameWrite = false;
    frameData = 0;
    frameDH = false;
    frameCompleted = false;


    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;

}

// Restituisce un intero valutando il formato della stringa
int serverDebug::getVal(QString val){
    bool ok;
    if(val.contains("0x")){
        val = val.replace("0x","");
        return val.toAscii().toInt(&ok,16);
    }
    return val.toInt();

}

// read8 target,address
void serverDebug::handleDriverRead8(QByteArray data)
{
    QByteArray buffer;
    unsigned char target;
    unsigned char indirizzo;
    unsigned char banco;

    isCommand = false;
    isSpecial = false;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("read8"));
    if(parametri.size()!=2)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }
    frameTarget = parametri[0];

    // Determina l'indirizzo
    int addr = getVal(parametri[1]);
    if(addr > 255) banco = 1;
    else banco = 0;
    indirizzo = (unsigned char) (0x00FF & addr);

    // Costruisce il buffer
    frameD0 = (unsigned char) (target | SER422_READ);
    buffer.append(frameD0);
    frameD1 = (unsigned char) indirizzo;
    buffer.append(frameD1);
    frameD2 = (unsigned char) banco;
    buffer.append(frameD2);

    frameWrite = false;
    frameData = 0;
    frameFormat16 = false;
    frameDH = false;
    frameCompleted = false;

    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;


}

// read16 target,address
void serverDebug::handleDriverRead16(QByteArray data){
    QByteArray buffer;
    unsigned char target;
    unsigned char indirizzo;
    unsigned char banco;

    isCommand = false;
    isSpecial = false;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("read16"));
    if(parametri.size()!=2)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }
    frameTarget = parametri[0];

    // Determina l'indirizzo
    int addr = getVal(parametri[1]);
    if(addr > 255) banco = 1;
    else banco = 0;
    indirizzo = (unsigned char) (0x00FF & addr);

    // Costruisce il buffer
    frameD0 = (unsigned char) (target | SER422_READ);
    buffer.append(frameD0);
    frameD1 = (unsigned char) indirizzo;
    buffer.append(frameD1);
    frameD2 = (unsigned char) banco;
    buffer.append(frameD2);

    frameWrite = false;
    frameData = 0;
    frameFormat16 = true;
    frameDH = false;
    frameCompleted = false;

    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;

}

// write8 target,address,val
void serverDebug::handleDriverWrite8(QByteArray data){
    QByteArray buffer;
    unsigned char target;
    unsigned char indirizzo;

    isCommand = false;
    isSpecial = false;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("write8"));
    if(parametri.size()!=3)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }
    frameTarget = parametri[0];

    // Determina l'indirizzo
    int addr = getVal(parametri[1]);
    if(addr > 255){
        serviceTcp->txData(QByteArray("invalid address\n\r"));
        return;
    }
    indirizzo = (unsigned char) (addr);

    // Determina il valore
    frameData = getVal(parametri[2]);

    // Costruisce il buffer
    frameD0 = (unsigned char) (target | SER422_WRITE);
    buffer.append(frameD0);
    frameD1 = (unsigned char) indirizzo;
    buffer.append(frameD1);
    frameD2 = (unsigned char) (frameData & 0x00FF);
    buffer.append(frameD2);

    frameWrite = true;
    frameFormat16 = false;
    frameDH = false;
    frameCompleted = false;

    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;

}
void serverDebug::handleDriverWrite16(QByteArray data){
    QByteArray buffer;
    unsigned char target;
    unsigned char indirizzo;

    isCommand = false;
    isSpecial = false;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("write16"));
    if(parametri.size()!=3)
    {
        serviceTcp->txData(QByteArray("wrong parameters\n\r"));
        return;
    }

    // Controllo indirizzo
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }
    frameTarget = parametri[0];

    // Determina l'indirizzo
    int addr = getVal(parametri[1]);
    if(addr > 255){
        serviceTcp->txData(QByteArray("invalid address\n\r"));
        return;
    }
    indirizzo = (unsigned char) (addr);

    // Determina il valore
    frameData = getVal(parametri[2]);

    // Costruisce il buffer
    frameD0 = (unsigned char) (target | SER422_WRITE);
    buffer.append(frameD0);
    frameD1 = (unsigned char) indirizzo;
    buffer.append(frameD1);
    frameD2 = (unsigned char) (frameData & 0x00FF);
    buffer.append(frameD2);

    frameWrite = true;
    frameFormat16 = true;
    frameDH = false;
    frameCompleted = false;

    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;


}

// command target,b1,b2
void serverDebug::handleDriverCommand(QByteArray data){
    QByteArray buffer;
    unsigned char target;
    unsigned char b1;
    unsigned char b2;

    isCommand = true;
    isSpecial = false;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("command"));
    if(parametri.size()!=3)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    // Controllo target
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB204") target = 0x0B;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }
    frameTarget = parametri[0];

    // Determina b1
    int val = getVal(parametri[1]);
    if(val > 255){
        serviceTcp->txData(QByteArray("invalid b1 size\n\r"));
        return;
    }
    b1 = (unsigned char) val;

    // Determina b1
    val = getVal(parametri[2]);
    if(val > 255){
        serviceTcp->txData(QByteArray("invalid b2 size\n\r"));
        return;
    }
    b2 = (unsigned char) val;

    // Costruisce il buffer
    frameD0 = (unsigned char) (target | SER422_COMMAND);
    buffer.append(frameD0);
    frameD1 = (unsigned char) b1;
    buffer.append(frameD1);
    frameD2 = (unsigned char) b2;
    buffer.append(frameD2);

    frameWrite = false;
    frameData = 0;
    frameFormat16 = false;
    frameDH = false;
    frameCompleted = false;

    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;

}
void serverDebug::handleDriverSpecial(QByteArray data){
    QByteArray buffer;
    unsigned char target;
    unsigned char b1;
    unsigned char b2;

    isCommand = false;
    isSpecial = true;

    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("special"));
    if(parametri.size()!=3)
    {
        serviceTcp->txData(QByteArray("wrong parametrs\n\r"));
        return;
    }

    // Controllo target
    if(parametri[0]=="PCB269") target = 0x11;
    else if(parametri[0]=="PCB190") target = 0x13;
    else if(parametri[0]=="PCB249U1") target = 0x16;
    else if(parametri[0]=="PCB249U2") target = 0x15;
    else if(parametri[0]=="PCB244") target = 0x14;
    else if(parametri[0]=="PCB244A") target = 0x17;
    else{
        serviceTcp->txData(QByteArray("invalid target\n\r"));
        return;
    }
    frameTarget = parametri[0];

    // Determina b1
    int val = getVal(parametri[1]);
    if(val > 255){
        serviceTcp->txData(QByteArray("invalid b1 size\n\r"));
        return;
    }
    b1 = (unsigned char) val;

    // Determina b1
    val = getVal(parametri[2]);
    if(val > 255){
        serviceTcp->txData(QByteArray("invalid b2 size\n\r"));
        return;
    }
    b2 = (unsigned char) val;

    // Costruisce il buffer
    frameD0 = (unsigned char) (target | SER422_SPECIAL);
    buffer.append(frameD0);
    frameD1 = (unsigned char) b1;
    buffer.append(frameD1);
    frameD2 = (unsigned char) b2;
    buffer.append(frameD2);

    frameWrite = false;
    frameData = 0;
    frameFormat16 = false;
    frameDH = false;
    frameCompleted = false;

    if(mccService(1,SRV_SERIAL_SEND,buffer)== FALSE) serviceTcp->txData("MCC FALLITO");
    else connect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

    return;

}


void serverDebug::handleDriverSendNotify(unsigned char id,unsigned char cmd, QByteArray data)
{


    if(cmd == SRV_TEST_INTERPROCESS){
        pConfig->intercommTest();
        return;
    }

    if(cmd == SRV_TEST_422){
        if(data.size()!=4) return;
        int tentativi = data.at(0) + data.at(1) * 256;
        int successi = data.at(2) + data.at(3) * 256;
        serviceTcp->txData(QString("TEST SERIAL RESULT: ATTEMPT:%1 SUCCESS:%2\n\r").arg(tentativi).arg(successi).toAscii());
        disconnect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)));
        return;
    }

    if(cmd!=SRV_SERIAL_SEND) return;
    disconnect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)));

    if(frameCompleted){
        disconnect(pConsole,SIGNAL(mccServiceNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleDriverSendNotify(unsigned char,unsigned char,QByteArray)));
        serviceTcp->txData(QByteArray(cmdGroup).append(">"));
        return;
    }


    if(isCommand){
        frameCompleted = true;
        serviceTcp->txData(QString("%1-COMMAND RET: %2 %3 (0x%4 0x%5)\n\r").arg(frameTarget).arg((int)data[1],0,10).arg((int)data[2],0,10).arg((int)data[1],0,16).arg((int)data[2],0,16).toAscii());
    }else if(isSpecial){
        frameCompleted = true;
        serviceTcp->txData(QString("%1-SPECIAL RET: %2 %3 (0x%4 0x%5)\n\r").arg(frameTarget).arg((int)data[1],0,10).arg((int)data[2],0,10).arg((int)data[1],0,16).arg((int)data[2],0,16).toAscii());
    }else if(frameWrite){ // Operazione di scrittura
        if(frameFormat16){
            if(frameDH){
                frameCompleted = true;
                serviceTcp->txData(QString("%1[0x%2]: Written!\n\r").arg(frameTarget).arg((int)(data[1]-1),0,16).toAscii());
            }else{
                QByteArray buffer;
                buffer.append(frameD0);
                buffer.append(frameD1+1);
                buffer.append((unsigned char) ((frameData & 0xFF00)>>8));
                mccService(1,SRV_SERIAL_SEND,buffer);
                frameDH = true;
                serviceTcp->txData(QByteArray(cmdGroup).append(">"));
                return;
            }

        }else{
             frameCompleted = true;
             serviceTcp->txData(QString("%1[0x%2]: Written!\n\r").arg(frameTarget).arg((int)data[1],0,16).toAscii());
        }
    }else{// Operazione di lettura
        if(frameFormat16){
            if(frameDH){
                frameCompleted = true;
                serviceTcp->txData(QString("%1[0x%2]:%3 (0x%4)\n\r").arg(frameTarget).arg((int)(data[1]-1),0,16).arg((int)data[2]*256+frameData).arg((int)data[2]*256+frameData,0,16).toAscii());
            }else{
                frameData = data[2];
                QByteArray buffer;
                buffer.append(frameD0);
                buffer.append(frameD1+1);
                buffer.append(frameD2);
                mccService(1,SRV_SERIAL_SEND,buffer);
                frameDH = true;
                serviceTcp->txData(QByteArray(cmdGroup).append(">"));
                return;
            }
        }else{
             frameCompleted = true;
             serviceTcp->txData(QString("%1[0x%2]:%3 (0x%4)\n\r").arg(frameTarget).arg((int)data[1],0,16).arg((int)data[2]).arg((int)data[2],0,16).toAscii());
        }
    }


    serviceTcp->txData(QByteArray(cmdGroup).append(">"));
}

void serverDebug::handleLoader(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("loader: ------------   Comandi diretti ai drivers ------------------------------\r\n"));
        serviceTcp->txData(QByteArray("TEST <Target>  Test connection to 19200 \r\n"));
        serviceTcp->txData(QByteArray("UPLOAD <Target>  Carica il firmware relativo al target \r\n"));
        serviceTcp->txData(QByteArray("setFormat <target,revision>  Formatta il file hex con la revisione dichiarata\r\n"));
        serviceTcp->txData(QByteArray("CONFIG <Target>, <uC>              Richiede la Config Area di un target\r\n"));
        serviceTcp->txData(QByteArray("getCRC  nome_file      calcola il CRC da inserire nel file destinazione in /home/user \r\n"));
        serviceTcp->txData(QByteArray("setCRC  <target,revision>  Formatta il target file con <CRC,REV> \r\n"));
        serviceTcp->txData(QByteArray("setRemoteCRC  <target,revision>  Formatta il target file (remoto) con <CRC,REV> \r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    }
    else if(data.contains("UPLOAD")) handleLoaderUpload(data);
    else if(data.contains("CONFIG")) handleReadConfig(data);
    else if(data.contains("getCRC")) handleGetCRC(data);
    else if(data.contains("setCRC")) handleSetCRC(data);
    else if(data.contains("setRemoteCRC")) handleSetRemoteCRC(data);
    else if(data.contains("TEST")) handleLoaderTestConnection(data);


}


void serverDebug::handleSetLanguage(QByteArray data)
{
    QList<QByteArray> parametri;
    QString stringa;
    parametri = getNextFieldsAfterTag(data, QString("setLanguage"));
    if(parametri.size()==0)
    {
        stringa = QString("Current language: %1\n\r").arg(pConfig->userCnf.languageInterface);
        serviceTcp->txData(stringa.toAscii());
        return;
    }
    if(parametri.size()!=1)
    {
        serviceTcp->txData(QByteArray("Wrong parameter: insert the uppercase code (ITA,ENG..)\n\r"));
        return;
    }

    if((parametri[0]=="ITA") || (parametri[0]=="ENG") || (parametri[0]=="FRA") ||
            (parametri[0]=="DEU") || (parametri[0]=="PRT") || (parametri[0]=="RUS") ||
            (parametri[0]=="ESP") || (parametri[0]=="TUR") || (parametri[0]=="POL") ||
            (parametri[0]=="CHN")|| (parametri[0]=="LTU")){

            ApplicationDatabase.setData(_DB_LINGUA,QString(parametri[0]), 0);
            pConfig->userCnf.languageInterface = parametri[0];
            pConfig->saveUserCfg();

    }else{
        serviceTcp->txData(QByteArray("Lingue disponibili:\n\r"));
        serviceTcp->txData(QByteArray("ITA\n\r"));
        serviceTcp->txData(QByteArray("ENG\n\r"));
        serviceTcp->txData(QByteArray("FRA\n\r"));
        serviceTcp->txData(QByteArray("DEU\n\r"));
        serviceTcp->txData(QByteArray("PRT\n\r"));
        serviceTcp->txData(QByteArray("RUS\n\r"));
        serviceTcp->txData(QByteArray("ESP\n\r"));
        serviceTcp->txData(QByteArray("TUR\n\r"));
        serviceTcp->txData(QByteArray("POL\n\r"));
        serviceTcp->txData(QByteArray("CHN\n\r"));
        serviceTcp->txData(QByteArray("LTU\n\r"));
    }


    return;
}



void serverDebug::handleSystem(QByteArray data)
{
    QString field;
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("system:----------------- Comandi di gestione files di configurazione -\r\n"));
        serviceTcp->txData(QByteArray("setDATE Y M D h m s      imposta data e ora corrente \r\n"));        
        serviceTcp->txData(QByteArray("getRevisions             Richiede la configurazione del sistema\r\n"));
        serviceTcp->txData(QByteArray("setShotMode              Attiva la modalit�  di RX Test\r\n"));
        serviceTcp->txData(QByteArray("setShotNodetMode         Attiva la modalit�  di RX Test senza detector\r\n"));
        serviceTcp->txData(QByteArray("setCalibKv               Attiva la modalit�  di calibrazione KV\r\n"));
        serviceTcp->txData(QByteArray("setCalibIa               Attiva la modalit�  di calibrazione IA\r\n"));
        serviceTcp->txData(QByteArray("setCalibTomo <sample>    Attiva la modalit�  di calibrazione Tomo\r\n"));
        serviceTcp->txData(QByteArray("setOperatingMode         Attiva la modalit�  Operativa\r\n"));                
        serviceTcp->txData(QByteArray("reboot                   Effettua il reboot di entrambi i terminali\r\n"));
        serviceTcp->txData(QByteArray("setPowerOff              Attiva il powerdown \r\n"));
        serviceTcp->txData(QByteArray("setUnpark                Attiva procedura di unpark \r\n"));
        serviceTcp->txData(QByteArray("setPark                Attiva procedura di unpark \r\n"));
        serviceTcp->txData(QByteArray("------------------------------------------------------------------------\r\n"));
    } else
    {

        if(data.contains("setIp")){
            QHostAddress host = setIpAddress(10);
            serviceTcp->txData(QString("IP:%1\r\n").arg(host.toString()).toAscii());

        }else if(data.contains("setUnpark")){
            unsigned char buffer[2];
            buffer[0]=MCC_PARKING_MODE_COMMANDS_START_UNPARKING;
            pConsole->pGuiMcc->sendFrame(MCC_PARKING_MODE_COMMANDS,1,buffer, sizeof(buffer));

        }else if(data.contains("setPark")){
            pConfig->lenzeConfig.startupInParkingMode = true;
            pConfig->saveLenzeConfig();
            unsigned char buffer[2];
            buffer[0]=MCC_PARKING_MODE_COMMANDS_START_PARKING;
            pConsole->pGuiMcc->sendFrame(MCC_PARKING_MODE_COMMANDS,1,buffer, sizeof(buffer));

        }else if(data.contains("setDATE"))
        {
            if(systemTimeUpdated) {
                serviceTcp->txData(QString("System Time already updated!\n\r").toAscii());
                return;
            }

            QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("setDATE"));
            if(parametri.size() != 6) {
                serviceTcp->txData(QString("Invalid parameters number (should be six)\n\r").toAscii());
                return;
            }

            QString Y = QString(parametri[0]);
            QString M = QString(parametri[1]);
            QString D = QString(parametri[2]);
            QString h = QString(parametri[3]);
            QString m = QString(parametri[4]);
            QString s = QString(parametri[5]);

            echoDisplay.echoDate(D,M,Y,h,m,s,DBase::_DB_NO_OPT);

            QString command;
            command = QString("date -u %1%2%3%4%5.%6").arg(M).arg(D).arg(h).arg(m).arg(Y).arg(s);
            system(command.toStdString().c_str());

            command = QString("hwclock -w");
            system(command.toStdString().c_str());

            systemTimeUpdated = TRUE;

            // Con la ricezione della data � possibile inizializzare il dispositivo
            // per la misura della quantit� di calore accumulata nell'Anodo
            pGeneratore->initAnodeHU();

            return;

        }else  if(data.contains("getRevisions"))
        {
            handleGetRevisions();
        }else if(data.contains("setShotNodetMode"))
        {
            pConsole->handleSetShotMode(FALSE);
        }else if(data.contains("setShotMode"))
        {
            pConsole->handleSetShotMode(TRUE);
        }else if(data.contains("setCalibKv"))
        {
            pConsole->handleSetKvCalibMode();
        }else if(data.contains("setCalibIa"))
        {
            pConsole->handleSetIaCalibMode();
        }else if(data.contains("setCalibTomo"))
        {
            field = getNextFieldAfterTag(data,"setCalibTomo ");
            pConsole->handleSetCalibTomo(field);
        }else if(data.contains("setOperatingMode"))
        {
            pConsole->handleSetOperatingMode();
        }else if(data.contains("getAlarmInfo"))
        {
            handleGetAlarmInfo(data);
        }else if(data.contains("reboot"))
        {
            pConfig->executeReboot();
        }else if(data.contains("setOutputs"))
        {
            handleSetOutputs(data);
        }  else if(data.contains("setPowerOff")){
            QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("setPowerOff"));
            if(parametri.size() != 1) {
                serviceTcp->txData(QString("Invalid parameters number \n\r").toAscii());
                return;
            }

            pConfig->executePoweroff((unsigned char) parametri[0].toInt());

        }

    }
}

void serverDebug::handleGetAlarmInfo(QByteArray data)
{
    QList<QByteArray> parametri;


    parametri = getNextFieldsAfterTag(data, QString("getAlarmInfo"));


    if(parametri.size()!=1){
        serviceTcp->txData("PARAMETRI NON CORRETTI \n\r");
        return;
    }

    PageAlarms::_alarmStruct* pErr = paginaAllarmi->getErrorInfo( parametri[0].toInt());
    if(pErr==0){
        serviceTcp->txData("NESSUN ERRORE ASSOCIATO \n\r");
        return;
    }

    serviceTcp->txData(QString("ERRORE: \"%1\"\n\r").arg(pErr->codestr).toAscii());
    serviceTcp->txData(QString("MSG: \"%1\"\n\r").arg(pErr->errmsg).toAscii());
    serviceTcp->txData(QString("INTERNAL CODE: %1 \n\r").arg(pErr->codeval).toAscii());
    serviceTcp->txData(QString("DESCRIPTION: \"%1\" \n\r").arg(pErr->errdescr).toAscii());

    return;
}



#define OUT_MASTER_ENA "MASTER_ENA"
#define OUT_COMPRESSOR_ENA "COMPRESSOR_ENA"
#define OUT_PEND_ENA "PEND_ENA"
#define OUT_ROT_ENA "ROT_ENA"
#define OUT_LIFT_ENA "LIFT_ENA"
#define OUT_XRAY_ENA "XRAY_ENA"
#define OUT_SYS_FAULT "SYS_FAULT"

#define OUT_LAMP_SW1 "LAMP_SW1"
#define OUT_LAMP_SW2 "LAMP_SW2"
#define OUT_XRAY_LED "XRAY_LED"
#define OUT_BUZ_ON "BUZ_ON"
#define OUT_BUZ_MODE "BUZ_MODE"
#define OUT_DEMO "DEMO"
#define OUT_DEMO_CODE "DEMO_CODE"
#define OUT_BURNING "BURNING"

#define OUT_LOADER_PWR_ON "LOADER_ON"
#define OUT_POWER_OFF_REQ "POWER_OFF"
#define OUT_BATT_TEST_REQ "BATT_TEST"

void serverDebug::handleSetOutputs(QByteArray data)
{
    /* TBD
    ioOutputs o;
    QList<QByteArray> parametri;
    parametri = getNextFieldsAfterTag(data, QString("setOutputs"));

    if(parametri.size()!=2){
        //Lista outputs disponibili
        serviceTcp->txData(QByteArray("Available Output tags:\r\n"));
        serviceTcp->txData(QString("%1\r\n").arg(OUT_MASTER_ENA).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_COMPRESSOR_ENA).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_PEND_ENA).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_ROT_ENA).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_LIFT_ENA).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_XRAY_ENA).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_SYS_FAULT).toAscii());


        serviceTcp->txData(QString("%1\r\n").arg(OUT_LAMP_SW1).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_LAMP_SW2).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_XRAY_LED).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_BUZ_ON).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_BUZ_MODE).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_DEMO).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_DEMO_CODE).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_BURNING).toAscii());

        serviceTcp->txData(QString("%1\r\n").arg(OUT_LOADER_PWR_ON).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_POWER_OFF_REQ).toAscii());
        serviceTcp->txData(QString("%1\r\n").arg(OUT_BATT_TEST_REQ).toAscii());
        return;
    }

    int val;
    if(parametri[1].toInt()) val = 1;
    else val = 0;

    if(parametri[0]==OUT_MASTER_ENA){
        o.mask.CPU_MASTER_ENA=1;
        o.outputs.CPU_MASTER_ENA=val;
    }else if(parametri[0]==OUT_COMPRESSOR_ENA){
        o.mask.CPU_COMPRESSOR_ENA=1;
        o.outputs.CPU_COMPRESSOR_ENA=val;
    }else if(parametri[0]==OUT_ROT_ENA){
        o.mask.CPU_ROT_ENA=1;
        o.outputs.CPU_ROT_ENA=val;
    }else if(parametri[0]==OUT_LIFT_ENA){
        o.mask.CPU_LIFT_ENA=1;
        o.outputs.CPU_LIFT_ENA=val;
    }else if(parametri[0]==OUT_PEND_ENA){
        o.mask.CPU_PEND_ENA=1;
        o.outputs.CPU_PEND_ENA=val;
    }else if(parametri[0]==OUT_SYS_FAULT){
        o.mask.CPU_SYS_FAULT=1;
        o.outputs.CPU_SYS_FAULT=val;
    }else if(parametri[0]==OUT_XRAY_ENA){
        o.mask.CPU_XRAY_ENA=1;
        o.outputs.CPU_XRAY_ENA=val;
    }

    else if(parametri[0]==OUT_LAMP_SW1){
            o.mask.CPU_LMP_SW1=1;
            o.outputs.CPU_LMP_SW1=val;
    }else if(parametri[0]==OUT_LAMP_SW2){
            o.mask.CPU_LMP_SW2=1;
            o.outputs.CPU_LMP_SW2=val;
    }else if(parametri[0]==OUT_XRAY_LED){
        o.mask.CPU_XRAY_LED=1;
        o.outputs.CPU_XRAY_LED=val;
    }else if(parametri[0]==OUT_BUZ_ON){
        o.mask.CPU_BUZ_ON=1;
        o.outputs.CPU_BUZ_ON=val;
    }else if(parametri[0]==OUT_BUZ_MODE){
        o.mask.CPU_BUZ_MODE=1;
        o.outputs.CPU_BUZ_MODE=val;
    }else if(parametri[0]==OUT_DEMO){
        o.mask.CPU_DEMO=1;
        o.outputs.CPU_DEMO=val;
    }else if(parametri[0]==OUT_DEMO_CODE){
        o.mask.CPU_DEMO_CODE=1;
        o.outputs.CPU_DEMO_CODE=val;
    }

    else if(parametri[0]==OUT_LOADER_PWR_ON){
        o.mask.CPU_LOADER_PWR_ON=1;
        o.outputs.CPU_LOADER_PWR_ON=val;
    }else if(parametri[0]==OUT_BATT_TEST_REQ){
        o.mask.CPU_BATT_TEST_REQ=1;
        o.outputs.CPU_BATT_TEST_REQ=val;
    }


    io->setOutputs(o);
    */
}


void serverDebug::handleStandardBiopsy(QByteArray data){

}

void serverDebug::handleExtendedBiopsy(QByteArray data)
{
    QList<QByteArray> parametri;

    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("--------------- ACTIVATION ----------------------------\r\n"));
        serviceTcp->txData(QByteArray("moveXYZ   x,y,z       ? X,Y,Z in dmm \r\n"));
        serviceTcp->txData(QByteArray("moveLoop   x0,y0,z0 x1,y1,z1   \r\n"));

        serviceTcp->txData(QByteArray("moveHome  [L,C,R]     ? Imposta lateralit� \r\n"));
        serviceTcp->txData(QByteArray("testBuzzer            ? attiva buzzer BYM X\r\n"));
        serviceTcp->txData(QByteArray("powerLed  ON|OFF      ? attiva Power led\r\n"));


        serviceTcp->txData(QByteArray("--------------- INQUIRY ----------------------------\r\n"));
        serviceTcp->txData(QByteArray("getAdapter            ? legge adapter rilevato \r\n"));
        serviceTcp->txData(QByteArray("getRevision           ? legge la revisione e il checksum\r\n"));
        serviceTcp->txData(QByteArray("getSignals            ? legge tutti i segnali analogici\r\n"));

        serviceTcp->txData(QByteArray("--------------- CALIBRATION ----------------------------\r\n"));
        serviceTcp->txData(QByteArray("calibXbase  val       ? calibrazione base X\r\n"));
        serviceTcp->txData(QByteArray("calibSh  zero,up,down ? calibrazione asse SH: zero, +150,-150 \r\n"));


#ifdef __BIOPSY_SIMULATOR
        serviceTcp->txData(QByteArray("\n\r BIOPSY SIMULATOR COMMANDS: --------\r\n"));
        serviceTcp->txData(QByteArray("simConn    [ON/OFF]   ? ON=CONNECTED \r\n"));
        serviceTcp->txData(QByteArray("simSetXYZ  X,Y,Z      ? SET THE VIRTUAL CURSOR (dmm) \r\n"));
        serviceTcp->txData(QByteArray("simAdapter [A/B/C/O/S]? SET ADAPTER CODE \r\n"));
        serviceTcp->txData(QByteArray("simSblocco [ON/OFF]   ? ON=UNLOCK \r\n"));        
        serviceTcp->txData(QByteArray("simSH      shval      ? SH val in (dmm)\r\n"));
        serviceTcp->txData(QByteArray("simLat     [L,C,R,N]  ? Laterality     \r\n"));
        serviceTcp->txData(QByteArray("-----------------------------------------------------------------\r\n"));
#endif

    } else
    {



#ifndef __BIOPSY_SIMULATOR
        if(!pBiopsy->connected){
            serviceTcp->txData(QByteArray("BIOPSY DEVICE DISCONNECTED!!\n\r"));
            return;
        }
#endif
        if(data.contains("moveXYZ")){
            parametri = getNextFieldsAfterTag(data, QString("moveXYZ"));
            if(parametri.size()!=3){
                serviceTcp->txData(QByteArray("PARAM ERROR: moveXYZ X,Y,Z \r\n"));
                return;
            }

            int retval = pBiopsyExtended->requestBiopsyMoveXYZ(parametri[0].toInt(), parametri[1].toInt(),parametri[2].toInt(), 0);
            if(retval==0)  serviceTcp->txData(QByteArray("BYM already in Target\r\n"));
            else if(retval<0) serviceTcp->txData(QString("Error condition %1\r\n").arg(retval).toAscii());
            else serviceTcp->txData(QByteArray("DONE \r\n"));

        }else if(data.contains("moveLoop")){
            parametri = getNextFieldsAfterTag(data, QString("moveLoop"));
            if(parametri.size()!=7){
                pBiopsyExtended->setBiopsyLoop(0,0,0,0,0,0,0);
                serviceTcp->txData(QByteArray("LOOP STOP\r\n"));
                return;
            }

            int n = parametri[0].toInt();
            int x1 = parametri[1].toInt();
            int y1 = parametri[2].toInt();
            int z1 = parametri[3].toInt();
            int x2 = parametri[4].toInt();
            int y2 = parametri[5].toInt();
            int z2 = parametri[6].toInt();

            if(pBiopsyExtended->setBiopsyLoop(n,x1,y1,z1,x2,y2,z2)) serviceTcp->txData(QByteArray("LOOP INITIATED \r\n"));
            else serviceTcp->txData(QByteArray("LOOP FAILED \r\n"));

        }else if(data.contains("moveHome")){
            parametri = getNextFieldsAfterTag(data, QString("moveHome"));
            if(parametri.size()!=1){
                serviceTcp->txData(QByteArray("PARAM ERROR: moveHome [C,L,R]\r\n"));
                return;
            }
            unsigned char lat = (unsigned char) parametri[0].toInt();

            // Determina il target X in funzione della lateralit�
            if(parametri[0] == "L") lat = _BP_EXT_ASSEX_POSITION_LEFT;
            else if(parametri[0] == "R") lat = _BP_EXT_ASSEX_POSITION_RIGHT;
            else if(parametri[0] == "C") lat = _BP_EXT_ASSEX_POSITION_CENTER;
            else {
                serviceTcp->txData(QByteArray("PARAM ERROR: moveHome [C,L,R]\n"));
                return;
            }

            int retval = pBiopsyExtended->requestBiopsyHome(0,lat,0);
            if(retval==0)  serviceTcp->txData(QByteArray("BYM already in Home\r\n"));
            else serviceTcp->txData(QByteArray("DONE \r\n"));

        }else if(data.contains("calibXbase")){
            parametri = getNextFieldsAfterTag(data, QString("calibXbase"));
            if(parametri.size()!=1){
                serviceTcp->txData(QByteArray("PARAM ERROR: calibXbase <val>\r\n"));
                return;
            }
            pBiopsyExtended->calibrateXbase(parametri[0].toUShort());
            serviceTcp->txData(QByteArray("DONE \r\n"));

        }else if(data.contains("calibSh")){
            parametri = getNextFieldsAfterTag(data, QString("calibSh"));
            if(parametri.size()!=3){
                serviceTcp->txData(QByteArray("PARAM ERROR: calibSh zero up150 down150 \r\n"));
                return;
            }

            pBiopsy->configExt.sh_zero_level = parametri[0].toUShort();
            pBiopsy->configExt.sh_150_level = parametri[1].toUShort();
            pBiopsy->configExt.sh_m150_level = parametri[2].toUShort();
            pBiopsy->storeConfigExtended();
            serviceTcp->txData(QByteArray("DONE \r\n"));
        }else if(data.contains("getAdapter")){
            serviceTcp->txData(QString("current adapter: %1\n\r").arg(pBiopsyExtended->getAdapterId()).toAscii().data());
        }else if(data.contains("getSignals")){
            QString lat;
            if(pBiopsyExtended->curLatX == _BP_EXT_ASSEX_POSITION_CENTER) lat = "CENTER";
            else if(pBiopsyExtended->curLatX == _BP_EXT_ASSEX_POSITION_LEFT) lat = "LEFT";
            else if(pBiopsyExtended->curLatX == _BP_EXT_ASSEX_POSITION_RIGHT) lat = "RIGHT";
            else lat = "UNDEF";

            QString stringa = QString("SIGNALS: X:%1, Y:%2, Z:%3, SHR:%4, SH:%5 LAT:%6 ").arg(pBiopsyExtended->curX_dmm).arg(pBiopsyExtended->curY_dmm).arg(pBiopsyExtended->curZ_dmm).arg(pBiopsyExtended->curSh_raw).arg(pBiopsyExtended->curSh_dmm).arg(lat);
            if(pBiopsyExtended->outPosition) stringa+= " OUT";
            else stringa += "IN ";

            if(pBiopsyExtended->getAdapterId() == _BP_EXT_ADAPTER_OPEN) stringa += "ADAPTER:ND";
            else if(pBiopsyExtended->getAdapterId() == _BP_EXT_ADAPTER_A) stringa += "ADAPTER:A";
            else if(pBiopsyExtended->getAdapterId() == _BP_EXT_ADAPTER_B) stringa += "ADAPTER:B";
            else if(pBiopsyExtended->getAdapterId()== _BP_EXT_ADAPTER_C) stringa += "ADAPTER:C";

            stringa+=" \r\n";

            serviceTcp->txData(stringa.toAscii().data());

        }else if(data.contains("getRevision")){
            serviceTcp->txData(QString("current revision: %1\n\r").arg(pBiopsy->revisione).toAscii().data());
            QString hex; hex.setNum((unsigned short) pBiopsy->checksum_h * 256 + (unsigned short)pBiopsy->checksum_l,16);
            QString stringa = "current checksum: oX" + hex + "\n\r";
            serviceTcp->txData(stringa.toAscii().data());
        }else if(data.contains("testBuzzer")){
            pBiopsyExtended->setBuzzer();
            serviceTcp->txData(QByteArray("DONE \r\n"));
        }else if(data.contains("powerLed")){
            parametri = getNextFieldsAfterTag(data, QString("powerLed"));
            if(parametri.size()!=1){
                serviceTcp->txData(QByteArray("PARAM ERROR: powerLed ON or OFF \r\n"));
                return;
            }
            if(parametri[0]=="ON") pBiopsyExtended->setPowerled(true);
            else pBiopsyExtended->setPowerled(false);
            serviceTcp->txData(QByteArray("DONE \r\n"));
        }else handleBiopsySimulator(data);
    }
}

void serverDebug::handleBiopsySimulator(QByteArray data)
{
#ifdef __BIOPSY_SIMULATOR

    QString stringa;
    unsigned char buffer[20];

    if(data.contains("simConn")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("simConn"));
        if(parametri.size() != 1) {
            stringa = QString("PARAMETRO: ON/OFF \r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }

        // Comando impostazione stato connessione
        buffer[0] = _BYM_SIM_CONNECTION;

        // Parametro
        if(parametri[0]=="ON") buffer[1] = 1;
        else buffer[1] = 0;

        pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_SIMULATOR,1,buffer,2);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;

    }else if(data.contains("simSetXYZ")){

        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("simSetXYZ"));
        if(parametri.size() != 3) {
            stringa = QString("PARAMETRO: X,Y,Z \r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }

        // Comando impostazione stato connessione
        buffer[0] = _BYM_SIM_SET_CURSOR;

        buffer[1] = parametri[0].toInt() & 0xFF;
        buffer[2] = parametri[0].toInt() / 256 ;
        buffer[3] = parametri[1].toInt() & 0xFF;
        buffer[4] = parametri[1].toInt() / 256 ;
        buffer[5] = parametri[2].toInt() & 0xFF;
        buffer[6] = parametri[2].toInt() / 256 ;

        pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_SIMULATOR,1,buffer,7);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;

    }


    else if(data.contains("simSblocco")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("simSblocco"));
        if(parametri.size() != 1) {
            stringa = QString("PARAMETRO: ON/OFF \r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }

        // Comando impostazione stato connessione
        buffer[0] = _BYM_SIM_SBLOCCO;

        if(parametri[0]=="ON") buffer[1] = 1;
        else buffer[1] = 0;

        pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_SIMULATOR,1,buffer,2);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;

    }else if(data.contains("simAdapter")){
        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("simAdapter"));
        if(parametri.size() != 1) {
            stringa = QString("PARAMETRO: [A/B/C/O/S] \r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }

        // Comando impostazione stato connessione
        buffer[0] = _BYM_SIM_ADAPTER;

        if(parametri[0] == "A") buffer[1] = _BYM_SYM_ADAPTER_A;
        else if(parametri[0] == "B") buffer[1] = _BYM_SYM_ADAPTER_B;
        else if(parametri[0] == "C") buffer[1] = _BYM_SYM_ADAPTER_C;
        else if(parametri[0] == "O") buffer[1] = _BYM_SYM_ADAPTER_O;
        else if(parametri[0] == "S") buffer[1] = _BYM_SYM_ADAPTER_S;

        pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_SIMULATOR,1,buffer,2);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;

    }else if(data.contains("simSH")){

        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("simSH"));
        if(parametri.size() != 1) {
            stringa = QString("PARAMETRO: SH_VAL \r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }

        // Comando impostazione stato connessione
        buffer[0] = _BYM_SIM_SH;

        buffer[1] = parametri[0].toInt() & 0xFF;
        buffer[2] = parametri[0].toInt() / 256 ;

        pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_SIMULATOR,1,buffer,3);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;

    }else if(data.contains("simLat")){

        QList<QByteArray> parametri = getNextFieldsAfterTag(data, QString("simLat"));
        if(parametri.size() != 1) {
            stringa = QString("PARAMETRO: [L,C,R,N] \r\n");
            serviceTcp->txData(stringa.toAscii());
            return;
        }

        // Comando impostazione stato connessione
        buffer[0] = _BYM_SIM_LAT;

        if(parametri[0] == "L") buffer[1] = _BP_EXT_ASSEX_POSITION_LEFT;
        else if(parametri[0] == "C") buffer[1] = _BP_EXT_ASSEX_POSITION_CENTER;
        else if(parametri[0] == "R") buffer[1] = _BP_EXT_ASSEX_POSITION_RIGHT;
        else buffer[1] = _BP_EXT_ASSEX_POSITION_ND;


        pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_SIMULATOR,1,buffer,2);
        serviceTcp->txData(QByteArray("DONE \r\n"));
        return;

    }

#endif
}



void serverDebug::handleShell(QByteArray data)
{

    data.replace("shell:","");
    data.replace("\n","");
    data.replace("\r","");

    QString command = QString(data) + QString(" > ppp");
    system(command.toStdString().c_str());


    QFile file("ppp");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        serviceTcp->txData(QByteArray("NON LEGGE IL RISULTATO\n"));
        return;
    }

    QByteArray buffer;
    while(!file.atEnd())
    {
        buffer = file.readLine();
        buffer.append("\r");
        serviceTcp->txData(buffer);
    }
    file.close();


}

void serverDebug::handleMasterShell(QByteArray data)
{

    data.replace("master:","");
    data.replace("\n","");
    data.replace("\r","");

    bool noCmd = true;
    for(int i = 0; i< data.size(); i++){
        if((data.at(i) != ' ' ) && (data.at(i) != 0)) {
            noCmd=false;
            break;
        }
    }
    if(noCmd) return;

    QByteArray risultato = pConfig->executeShell(data);
    serviceTcp->txData(risultato);


}

void serverDebug::handleSlaveShell(QByteArray data)
{

    data.replace("slave:","");
    data.replace("\n","");
    data.replace("\r","");

    bool noCmd = true;
    for(int i = 0; i< data.size(); i++){
        if((data.at(i) != ' ' ) && (data.at(i) != 0)) {
            noCmd=false;
            break;
        }
    }
    if(noCmd) return;


    connect(pConfig,SIGNAL(sgnRemoteShell(QByteArray)),this,SLOT(slotRemoteShell(QByteArray)),Qt::UniqueConnection);
    pConfig->executeSlaveShell(data);

}

void serverDebug::slotRemoteShell(QByteArray data)
{
    serviceTcp->txData(data);
    disconnect(pConfig,SIGNAL(sgnRemoteShell(QByteArray)),this,SLOT(slotRemoteShell(QByteArray)));
}



void serverDebug::handleCanOpen(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("test:  <val> [val val..]     \r\n"));
        serviceTcp->txData(QByteArray("lenzeConf                    \r\n"));
        serviceTcp->txData(QByteArray("trxConf                      \r\n"));
        serviceTcp->txData(QByteArray("armConf                      \r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    }else if(data.contains("test"))
    {
        handleCanOpen_test(data);
    }else if(data.contains("lenzeConf"))
    {
        pConfig->updateLenzeDriver();
    }else if(data.contains("trxConf"))
    {
        pConfig->updateTrxDriver();
    }else if(data.contains("armConf"))
    {
        pConfig->updateArmDriver();
    }

}

// Invia un certo numero di uint32_t verso i driver
// per debug generico su dispositivi CANOPEN
void serverDebug::handleCanOpen_test(QByteArray data)
{
    unsigned char buffer[8];
    QList<QByteArray> parametri;
    parametri = getNextFieldsAfterTag(data, QString("test"));

    int len = parametri.size();
    if(len){
        buffer[0]= (unsigned char) parametri.at(0).toInt();
        if(len>1) {
            int16_t val = parametri.at(1).toShort();
            buffer[1]= (unsigned char) val;
            buffer[2]= (unsigned char) (val>>8);
        }

        if(len>2) {
            int32_t val = parametri.at(2).toLong();
            buffer[3]= (unsigned char) val;val=val>>8;
            buffer[4]= (unsigned char) val;val=val>>8;
            buffer[5]= (unsigned char) val;val=val>>8;
            buffer[6]= (unsigned char) val;

        }

    }

    pConsole->pGuiMcc->sendFrame(MCC_CANOPEN,0,buffer, 7);

}

void serverDebug::handlePotter(QByteArray data)
{
    if(data.contains("?"))
    {
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
        serviceTcp->txData(QByteArray("clearErrors:                 Reset Fault\r\n"));
        serviceTcp->txData(QByteArray("resetBoard:                  Reset Board\r\n"));
        serviceTcp->txData(QByteArray("setGrid2D: <ON/OFF>          Attivazione griglia 2D\r\n"));
        serviceTcp->txData(QByteArray("setGrid3D: <ON/OFF>          Attivazione griglia 3D\r\n"));
        serviceTcp->txData(QByteArray("setGridFreq:<freq>           Impostazione freequenza (0.1Hz/unit)\r\n"));
        serviceTcp->txData(QByteArray("setGridAmp:<ampiezza>        Impostazione ampiezza\r\n"));
        serviceTcp->txData(QByteArray("----------------------------------------------------------------------------------\r\n"));
    }else if(data.contains("clearErrors"))
    {
        handleClearPCB244Errors();
    }else if(data.contains("resetBoard"))
    {
        handleResetPCB244();
    }
    else if(data.contains("setGridFreq"))
    {
        handleSetGridFreq(data);
    }else if(data.contains("setGridAmp"))
    {
        handleSetGridAmp(data);
    }else if(data.contains("setGrid3D"))
    {
        handleSetGrid3D(data);
    }else if(data.contains("setGrid2D"))
    {
        handleSetGrid2D(data);
    }
}

/*

 */
void serverDebug::handleSetGrid3D(QByteArray data)
{
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("setGrid3D"));
    if(parametri.size()!=1){
        serviceTcp->txData(QByteArray("WRONG PARAMETER! Expected ON/OFF\n\r"));
        return;
    }

    QByteArray buf;

    if(parametri.at(0)=="ON"){
        serviceTcp->txData(QByteArray("Grid ON executed\n\r"));
        buf.append((unsigned char) SRV_START_POTTER_3D_GRID); // Comando di servizio
    }else{
        serviceTcp->txData(QByteArray("Grid OFF executed\n\r"));
        buf.append((unsigned char) SRV_STOP_POTTER_3D_GRID); // Comando di servizio
    }

    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());

}
/*

 */
void serverDebug::handleSetGrid2D(QByteArray data)
{
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("setGrid2D"));
    if(parametri.size()!=1){
        serviceTcp->txData(QByteArray("WRONG PARAMETER! Expected ON/OFF\n\r"));
        return;
    }

    QByteArray buf;

    if(parametri.at(0)=="ON"){
        serviceTcp->txData(QByteArray("Grid ON executed\n\r"));
        buf.append((unsigned char) SRV_START_POTTER_2D_GRID); // Comando di servizio
    }else{
        serviceTcp->txData(QByteArray("Grid OFF executed\n\r"));
        buf.append((unsigned char) SRV_STOP_POTTER_2D_GRID); // Comando di servizio
    }

    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());

}

void serverDebug::handleClearPCB244Errors(void){
    QByteArray buf;
    buf.append((unsigned char) SRV_RESET_FAULT_PCB244);
    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());
}

void serverDebug::handleResetPCB244(void){
    QByteArray buf;
    buf.append((unsigned char) SRV_RESET_PCB244);
    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());

}


void serverDebug::handleSetGridFreq(QByteArray data)
{
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("setGridFreq"));
    if(parametri.size()!=1){
        serviceTcp->txData(QByteArray("WRONG PARAMETER! Expected frequency [0:255]\n\r"));
        return;
    }


    int freq = parametri.at(0).toInt();
    if(freq>255) {
        serviceTcp->txData(QByteArray("WRONG PARAMETER! Expected frequency [0:255]\n\r"));
        return;
    }

    serviceTcp->txData(QString("Grid frequency set to %1\n\r").arg(freq).toAscii());

    QByteArray buf;
    buf.append((unsigned char) SRV_PCB244_CMD_FREQ_VC); // Comando di servizio
    buf.append((unsigned char) freq);

    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());

}

void serverDebug::handleSetGridAmp(QByteArray data)
{
    QList<QByteArray> parametri;

    parametri = getNextFieldsAfterTag(data, QString("setGridAmp"));
    if(parametri.size()!=1){
        serviceTcp->txData(QByteArray("WRONG PARAMETER! Expected amplitude [0:255]\n\r"));
        return;
    }


    int amp = parametri.at(0).toInt();
    if(amp>255) {
        serviceTcp->txData(QByteArray("WRONG PARAMETER! Expected amplitude [0:255]\n\r"));
        return;
    }

    serviceTcp->txData(QString("Grid Amplitude set to %1\n\r").arg(amp).toAscii());

    QByteArray buf;
    buf.append((unsigned char) SRV_PCB244_CMD_AMPL_VC); // Comando di servizio
    buf.append((unsigned char) amp);

    pConsole->pGuiMcc->sendFrame(MCC_SERVICE,0,(unsigned char*) buf.data(), buf.size());

}

void serverDebug::handleGetTrolleyNotify(unsigned char id,unsigned char cmd, QByteArray data){

    if(id!=1) return;
    if(cmd!=MCC_GET_TROLLEY) return;
    serviceTcp->txData(QString("Trolley position:  %1\n\r").arg((int) data.at(0)).toAscii());
    disconnect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(handleGetTrolleyNotify(unsigned char,unsigned char,QByteArray)));
    return;
}

void serverDebug::fineRaggiManualXray(QByteArray data)
{

    int tpls,mas,imean;

    // disconnect(pConsole,SIGNAL(raggiDataSgn(QByteArray)),this,SLOT(fineRaggiManualXray(QByteArray)));
    float vmean = pGeneratore->convertPcb190Kv(data.at(V_MEAN),1.0);
    mas = (data.at(MAS_PLS_L)+256*data.at(MAS_PLS_H)); // In 1/50 mAs unit
    tpls = (unsigned short) (data.at(T_MEAN_PLS_L)+256*data.at(T_MEAN_PLS_H));

    // Calcolo corrente media sulla durata dell'impulso
    if(tpls!=0) imean = mas * 20 / tpls;
    else if(data.at(I_MEAN)!=0) imean = pGeneratore->convertPcb190Ianode(data.at(I_MEAN));
    else imean=0;

    serviceTcp->txData(QString("Fine Raggi: kV=%1, mAs=%2, Time=%3, Imean=%4 \n\r").arg(vmean).arg((float) mas / 50).arg(tpls).arg(imean).toAscii());

    // Disattiva simbolo grafico per raggi
    pConsole->handleSetXraySym(false);

    return;
}

