#define MAIN_C
#include "application.h"
#include "appinclude.h"
#include "globvar.h"
#include "startuppage.h"

#include "Service/servicepanelmenu.h"

#include "Service/Calib/calibmenu.h"
#include "Service/Calib/calibcompressorposition.h"
#include "Service/Calib/calibcompressorforce.h"
#include "Service/Calib/calibzerosetting.h"
#include "Service/Calib/calibfilter.h"
#include "Service/Calib/calibpower.h"
#include "Service/Calib/calibpot.h"
#include "Service/Calib/calibParking.h"
#include "Service/Calib/calibstarter.h"
#include "Service/Calib/calibColli.h"
#include "Service/Calib/calibBiopsyExtended.h"
#include "Service/Setup/system.h"
#include "Service/Tools/toolsmenu.h"
#include "Service/Tools/tiltingtool.h"
#include "Service/Tools/armtool.h"
#include "Service/Tools/lenzetool.h"
#include "Service/Tools/invertertool.h"

#define _BACKGROUND_Y_PG_MAIN       "://MainPage/MainPage/Pagina0Y.png"
#define _BACKGROUND_C_PG_MAIN       "://MainPage/MainPage/Pagina0C.png"


ServicePanelMenu* pServiceMenu;
CalibMenu* pCalibMenu;
calibCompressorPosition* pCalibCompressionPositionPage;
CalibCompressorForce* pCalibCompressionForcePage;
calibzerosetting* pCalibZeroSettingPage;
StartupPage* pStartupPage;
systemPage* pSystem;
calibfilter* pCalibFilterPage;
calibpower* pCalibPowerPage;
calibpot* pCalibPot;
calibParking* pCalibParking;
calibstarter* pCalibStarter;
calibColli* pCalibColli;
CalibBiopsyExtendedClass* pCalibBiopsyExtended;
toolsmenu* pToolsMenu;
tiltingtool* pTiltingTool;
armtool* pArmTool;
lenzetool* pLenzeTool;
invertertool* pInverterTool;
diagnosticInterface* pDiagnosticInterface;

/*
 *
 *  - RIABILITARE LO SPESSORE DEL SENO
 *  - RIABILITARE LO SBLOCCO COMPRESSORE
 *  - ATTIVARE IL CONTROLLO SUL COLLIMATORE VALIDO
 *  - ATTIVARE IL CONTROLLO SUL PAD
 *
 */
void  logSlaveOutput(QtMsgType type, const char *msg)
 {

     return;
}

#define MASTER_N_IP 2
#define SLAVE_N_IP 3

QHostAddress setIpAddress( int val){
    int size = TcpIpServer::hostAddress().toString().length();
    return QHostAddress(QString("%1.%2").arg(TcpIpServer::hostAddress().toString().left(size-2)).arg(val)) ;
}

bool isIp(int val){
    if(TcpIpServer::hostAddress().toString().right(1).toInt()==val) return true;
    return false;
}

int main(int argc, char *argv[])
{

    QApplication mainApplication(argc, argv);
	#ifdef Q_WS_QWS
        QWSServer::setCursorVisible(false);
	#endif

    qRegisterMetaType<QAbstractSocket::SocketState>();
    qRegisterMetaType<QAbstractSocket::SocketError>();


    // Definizione degli indirizzi di sistema



#ifdef __CONSOLE_IP_ADDRESS
    _CONSOLE_IP = setIpAddress(__CONSOLE_IP_ADDRESS).toString();
#else
    _CONSOLE_IP = setIpAddress(1).toString();       //  "192.168.30.1"
#endif

    _MASTER_IP  = setIpAddress(2).toString();       //  "192.168.30.2"
    _SLAVE_IP   = setIpAddress(3).toString();       //  "192.168.30.3"
    _LOCAL_SERVICE_IP = _MASTER_IP;


#ifdef __APPLICATION_DESKTOP_COMPILE
    if (isIp(MASTER_N_IP)) isMaster=true;
    else isMaster=false;

#endif

    // Classe soipra tutte per gestire messaggi di log
    pInfo = new infoClass();


    // Init variabili globali non definite in  classi
    systemTimeUpdated = FALSE;

    // Costruzione DATABASE CAMPI VARIABILI:ApplicationDatabase

    ApplicationDatabase.append("");                 // _DB_SLAVE_GUI_REVISION
    ApplicationDatabase.append((unsigned char) 0);  // _DB_SYSTEM_CONFIGURATION
    ApplicationDatabase.append("");                 // _DB_SERVICE_PASSWORD
    ApplicationDatabase.append("");                 // _DB_PASSWORD
    ApplicationDatabase.append("");                 // _DB_LINGUA
    ApplicationDatabase.append("");                 // _DB_COLLIMAZIONE
    ApplicationDatabase.append("");                 // _DB_MANUAL_COLLIMAZIONE,
    ApplicationDatabase.append("");                 // _DB_COMPRESSOR_PAD    
    ApplicationDatabase.append((unsigned char) 0);  // _DB_COMPRESSOR_PAD_CODE
    ApplicationDatabase.append("");                 // _DB_ACCESSORY_NAME
    ApplicationDatabase.append((int)0);             // _DB_MAG_FACTOR
    ApplicationDatabase.append((unsigned char) 0);  // _DB_ACCESSORIO
    ApplicationDatabase.append((int)COLLI_ACCESSORIO_ND); // _DB_ACCESSORIO_COLLIMATORE
    ApplicationDatabase.append((int)0);             // _DB_FORZA
    ApplicationDatabase.append((int)0);             // _DB_TARGET_FORCE
    ApplicationDatabase.append((int)0);             // _DB_SPESSORE
    ApplicationDatabase.append((int)0);             // _DB_COMPRESSOR_POSITION
    ApplicationDatabase.append((int) 0);            // _DB_ANGOLO (decimi di grado)
    ApplicationDatabase.append((int) 0);            // _DB_TRX (decimi di grado)
    ApplicationDatabase.append((int) 0);            // _DB_GONIO (decimi di grado)
    ApplicationDatabase.append((int) 0);            // _DB_ACVOLT (V, 0=powerdown)
    ApplicationDatabase.append((int) 0);            // _DB_VPRIMARIO (0 not calibrated)
    ApplicationDatabase.append((unsigned char) 0);  // _DB_BATTCHARGE (%, 0 = disabled)
    ApplicationDatabase.append((unsigned char) 0);  // _DB_AWS_CONNECTION (0/1)
    ApplicationDatabase.append((unsigned char) 0);  // _DB_PCAWS_CONNECTION
    ApplicationDatabase.append("");                 // CAMPO INTESTAZIONE: STRING
    ApplicationDatabase.append((int) 0);            // CAMPO VALORI ACR VIEW
    ApplicationDatabase.append((unsigned char) 0);  // CAMPO VALORI ACR SUFFIX
    ApplicationDatabase.append((unsigned char) 0);  // _DB_XRAY_SYM
    ApplicationDatabase.append((unsigned char) 0);  // _DB_COMPRESSOR_UNLOCK
    ApplicationDatabase.append("");                 // _DB_CALIB_SYM
    ApplicationDatabase.append((unsigned char) 0);  // _DB_EXPOSURE_MODE
    ApplicationDatabase.append((unsigned char) 0);  // _DB_ENABLE_MOVIMENTI
    ApplicationDatabase.append((int) 0);            // _DB_CONNETCTED_DRIVER
    ApplicationDatabase.append("");                 // _DB_PACKAGE_ID
    ApplicationDatabase.append((unsigned char) 0);  // _DB_STARTUP_FASE
    ApplicationDatabase.append("");                 // _DB_IMAGE_NAME
    ApplicationDatabase.append((unsigned char) _CLOSED_STUDY_STATUS);  // _DB_STUDY_STAT
    ApplicationDatabase.append("");                 // _DB_PROIEZIONI
    ApplicationDatabase.append("");                 // _DB_SEL_PROJ
    ApplicationDatabase.append((unsigned char) 0);  // _DB_READY_EXPOSURE
    ApplicationDatabase.append((unsigned char) 0);  // _DB_XRAY_PUSH_BUTTON
    ApplicationDatabase.append((unsigned char) 0);  // _DB_CLOSED_DOOR
    //____________________________________________________________________________ SEZIONE ALLARMI
    ApplicationDatabase.append((unsigned char) 0);  //_DB_NALLARMI_ATTIVI      41
    ApplicationDatabase.append((int) 0);            // _DB_ALLARMI_POWERDOWN
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI BIOPSIA
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_PAD
    ApplicationDatabase.append((int) 0);            // CAMPO _DB_ALLARMI_ALR_COMPRESSORE,
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_POTTER
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_RAGGI  47
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_ARM
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_TRX
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_LENZE
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_GEN
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_COLLI
    ApplicationDatabase.append((int) 0);            // _DB_ALLARME_XRAY_PUSH
    ApplicationDatabase.append((int) 0);            // _DB_ALLARME_CMP_PUSH
    ApplicationDatabase.append((int) 0);            // _DB_ALLARME_LIFT_PUSH
    ApplicationDatabase.append((int) 0);            // _DB_ALLARME_ARM_PUSH
    ApplicationDatabase.append((int) 0);            // _DB_ALLARME_INFO_STAT
    ApplicationDatabase.append((int) 0);            // _DB_ALLARMI_SYSCONF    
    ApplicationDatabase.append((int) 0);            //_DB_ALLARMI_PARCHEGGIO
    ApplicationDatabase.append((int) 0);            // CAMPO ALLARMI ALR_SOFT
    //_____________________________________________________________________________
    ApplicationDatabase.append((unsigned char) 0);  // _DB_FAULT_CODE_GEN
    ApplicationDatabase.append((int) 0);           // CAMPO TEMPERATURA CUFFIA
    ApplicationDatabase.append((int) 1);           // _DB_HU_ANODE


    ApplicationDatabase.append((int) 0);            // _DB_BIOP_MODEL
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_ADAPTER_ID
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_PADDLE_MARGINE
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_X
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_Y
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_Z
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_SH    
    ApplicationDatabase.append((unsigned char) 0);  // _DB_BIOP_SHOW_SH


    ApplicationDatabase.append((int) 0);            // _DB_BIOP_LES_X
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_LES_Y
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_LES_Z
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_UNLOCK_BUTTON
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_LAT_X

    ApplicationDatabase.append((int) 0);            // _DB_BIOP_NEEDLE_MARG
    ApplicationDatabase.append((int) 0);            // _DB_BIOP_MAXZ
    ApplicationDatabase.append("");                 // _DB_BIOP_MARG,  solo Standard model
    ApplicationDatabase.append("");                 // _DB_BIOP_HOLDER, Solo Standard model
    ApplicationDatabase.append("");                 // _DB_BIOP_AGO, Solo Standard model
    ApplicationDatabase.append((unsigned char) 0);  // _DB_BIOP_MANUAL_ENA Solo Standard

    ApplicationDatabase.append((unsigned char) 0);  // CAMPO DEMO_MODE
    ApplicationDatabase.append((unsigned char) 0);  // CAMPO DEAD_MEN
    ApplicationDatabase.append((unsigned char) 0);  // _DB_POWER_STAT
    ApplicationDatabase.append((unsigned char) 0);  // _DB_REQ_POWEROFF
    ApplicationDatabase.append("");                 // _DB_REVISION_ERROR_STRING
    ApplicationDatabase.append((unsigned char) 0);  // _DB_PARKING_MODE



    // DATABASE VARIABILI DI SERVIZIO ______________________________________________________
    ApplicationDatabase.append(""); // _DB_SERVICE1_STR
    ApplicationDatabase.append(""); // _DB_SERVICE2_STR
    ApplicationDatabase.append(""); // _DB_SERVICE3_STR
    ApplicationDatabase.append(""); // _DB_SERVICE4_STR
    ApplicationDatabase.append(""); // _DB_SERVICE5_STR
    ApplicationDatabase.append(""); // _DB_SERVICE6_STR
    ApplicationDatabase.append(""); // _DB_SERVICE7_STR
    ApplicationDatabase.append(""); // _DB_SERVICE8_STR

    ApplicationDatabase.append(""); // _DB_SERVICE9_STR
    ApplicationDatabase.append(""); // _DB_SERVICE10_STR
    ApplicationDatabase.append(""); // _DB_SERVICE11_STR
    ApplicationDatabase.append(""); // _DB_SERVICE12_STR
    ApplicationDatabase.append(""); // _DB_SERVICE13_STR
    ApplicationDatabase.append(""); // _DB_SERVICE14_STR
    ApplicationDatabase.append(""); // _DB_SERVICE15_STR

    ApplicationDatabase.append((int) 0); // _DB_SERVICE1_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE2_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE3_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE4_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE5_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE6_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE7_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE8_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE9_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE10_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE11_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE12_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE13_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE14_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE15_INT

    ApplicationDatabase.append((int) 0); // _DB_SERVICE16_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE17_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE18_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE19_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE20_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE21_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE22_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE23_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE24_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE25_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE26_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE27_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE28_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE29_INT
    ApplicationDatabase.append((int) 0); // _DB_SERVICE30_INT


    //_______________________________________________________________________________________


    // Connessione database al modulo echoDisplay
    QObject::connect(&ApplicationDatabase,SIGNAL(dbEchoSignal(int,int)), &echoDisplay,SLOT(dbChanged(int,int)),Qt::UniqueConnection);

    // Connessione Windows page al modulo echoDisplay
    QObject::connect(&GWindowRoot,SIGNAL(setNewPageEcho(int,int,int)), &echoDisplay,SLOT(setNewPageEcho(int,int,int)),Qt::UniqueConnection);



    // Angolo di rotazione iniziale della vista relativa al MASTER    
#ifdef __APPLICATION_DESKTOP_COMPILE
    int rotView = 180;
#else
    int rotView = 0;
#endif

    if (isIp(MASTER_N_IP))
    {// MASTER
        isMaster=true;

        pWarningBox =   new msgBox(rotView,0);
        pLoader =       new Loader(rotView);
        io =            new sysIO(TRUE);
        pConfig =       new Config(TRUE);
        pCompressore =  new Compressor();
        pCollimatore =  new Collimatore();
        pGeneratore =   new Generatore();
        pPotter =       new Potter();
        pBiopsy =       new(biopsyModule);
        pBiopsyStandard =  new biopsyStandardDevice();
        pBiopsyExtended =  new biopsyExtendedDevice(rotView);
        pConsole =      new console();
        pToConsole =    new protoToConsole();
        pDebug =        new serverDebug();
        pDiagnosticInterface = new diagnosticInterface();

    }else
    {// SLAVE

        isMaster=false;
        slaveTcp = new TcpIpClient();
        pWarningBox = new msgBox(rotView,0);
        // Creazione IO di sistema
        io = new sysIO(FALSE);
        pConfig = new Config(FALSE);        
        pBiopsyExtended =  new biopsyExtendedDevice(rotView);
    }

    #define LEFT_ARROW_FRAME  8,0,0,100,0,100,100,0,100
    #define RIGHT_ARROW_FRAME  8,700,0,800,0,800,100,700,100

    pagina_language = new PageLanguages( &mainApplication, PageLanguages::_LNG_ENG, true,_BACKGROUND_Y_PG_SELLNG,_BACKGROUND_C_PG_SELLNG,TRUE, 800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_SELLNG,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_MAIN_DIGITAL,(int)_PG_SELLNG);
    paginaMainDigital = new MainPage(true,QString(_BACKGROUND_Y_PG_MAIN),QString(_BACKGROUND_C_PG_MAIN),TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_MAIN_DIGITAL,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_SERVICE_MENU,(int)_PG_MAIN_DIGITAL);


    paginaBiopsyStandard = new BiopsyStandardPage(true,QString("_BACKGROUND_Y_PG_MAIN"),QString(""),TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_ACR,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_BIOPSY_STANDARD_PAGE,(int)_PG_BIOPSY_STANDARD_PAGE);
    paginaBiopsyExtended = new BiopsyExtendedPage(true,QString("_BACKGROUND_Y_PG_MAIN"),QString(""),TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_ACR,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_BIOPSY_EXTENDED_PAGE,(int)_PG_BIOPSY_EXTENDED_PAGE);


    paginaOpenStudyDigital = new OpenStudyPage(false,QString(""),QString(""),TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_OPEN_STUDY_DIGITAL,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_OPEN_STUDY_DIGITAL,(int)_PG_OPEN_STUDY_DIGITAL);
    paginaProjections = new ProjectionPage(false,QString(""),QString(""),TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_PROJECTIONS,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_OPEN_STUDY_DIGITAL,(int)_PG_PROJECTIONS);


    paginaAcr= new PageACR(true,"","",TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_ACR,GWindow::setPointPath(LEFT_ARROW_FRAME),(int) _PG_OPEN_STUDY_DIGITAL,(int)_PG_ACR);
    paginaAllarmi = new PageAlarms(QString(_PG_ALARM_BACKGROUND),TRUE,800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_ALARM,GWindow::setPointPath(LEFT_ARROW_FRAME),(int)_PG_ALARM,(int)_PG_ALARM);
    paginaImmagine= new ImagePage(QString(""),800,480,rotView,GWindow::setPointPath(RIGHT_ARROW_FRAME),(int)_PG_XRAY_IMG,GWindow::setPointPath(LEFT_ARROW_FRAME),(int) _PG_XRAY_IMG,(int)_PG_XRAY_IMG);
    pStartupPage= new StartupPage(rotView);

    // CREAZIONE DI TUTTI I PANNELLI DI SERVIZIO
    pServiceMenu = new ServicePanelMenu(rotView);
    pCalibMenu= new CalibMenu(rotView);
    pCalibCompressionPositionPage = new calibCompressorPosition(rotView);
    pCalibCompressionForcePage = new CalibCompressorForce(rotView);
    pCalibZeroSettingPage = new calibzerosetting(rotView);
    pSystem = new systemPage(rotView);
    pCalibFilterPage = new calibfilter(rotView) ;
    pCalibPowerPage = new calibpower(rotView) ;
    pCalibPot = new calibpot(rotView) ;
    pCalibParking = new calibParking(rotView) ;
    pCalibColli = new calibColli(rotView) ;
    pCalibStarter = new calibstarter(rotView) ;
    pCalibBiopsyExtended = new CalibBiopsyExtendedClass(rotView) ;
    pToolsMenu = new toolsmenu(rotView) ;
    pTiltingTool = new tiltingtool(rotView) ;
    pArmTool = new armtool(rotView) ;
    pLenzeTool = new lenzetool(rotView) ;
    pInverterTool = new invertertool(rotView) ;


    QObject::connect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), pagina_language,SLOT(buttonActivationNotify(int,bool,int)),Qt::UniqueConnection);
    QObject::connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), pagina_language,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    QObject::connect(pagina_language,SIGNAL(changeLanguageSgn()), paginaAllarmi,SLOT(languageChanged()),Qt::UniqueConnection);
    QObject::connect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), paginaAllarmi,SLOT(buttonActivationNotify(int,bool,int)),Qt::UniqueConnection);
    QObject::connect(&ApplicationDatabase,SIGNAL(dbDataChanged(int,int)), paginaAllarmi,SLOT(valueChanged(int,int)),Qt::UniqueConnection);
    QObject::connect(&GWindowRoot,SIGNAL(pushActivationSgn(int,bool,int)), &echoDisplay,SLOT(buttonChanged(int,bool,int)),Qt::UniqueConnection);


    // Inizializzazioni Master
    if(isMaster){
        // Connessioni per Database
        masterTcp = new TcpIpServer();
        masterTcp->Start(_ECHO_PORT);

        QObject::connect(masterTcp,SIGNAL(rxData(QByteArray)),&echoDisplay,SLOT(rxData(QByteArray)),Qt::UniqueConnection);
        QObject::connect(masterTcp,SIGNAL(serverConnection(bool)),&echoDisplay,SLOT(connection(bool)),Qt::UniqueConnection);
        QObject::connect(&echoDisplay,SIGNAL(txData(QByteArray)), masterTcp,SLOT(txData1000(QByteArray)),Qt::UniqueConnection);

        // Attivazione servizi in connessione
        pConfig->activateMasterConnections();
        pCompressore->activateConnections();
        pCollimatore->activateConnections();
        pGeneratore->activateConnections();
        pPotter->activateConnections();
        pBiopsyStandard->activateConnections();
        pBiopsyExtended->activateConnections();
        pConsole->activateConnections();
        pToConsole->activateConnections();
        pDebug->activateConnections();
        pDiagnosticInterface->activateConnections();
        pInfo->activateConnections();


    }else{
        qInstallMsgHandler(logSlaveOutput);
        pConfig->activateSlaveConnections();
        QObject::connect(slaveTcp,SIGNAL(rxData(QByteArray)),&echoDisplay,SLOT(rxData(QByteArray)),Qt::UniqueConnection);
        QObject::connect(&echoDisplay,SIGNAL(txData(QByteArray)),slaveTcp,SLOT(txData(QByteArray)),Qt::UniqueConnection);
        QObject::connect(slaveTcp,SIGNAL(clientConnection(bool)),&echoDisplay,SLOT(connection(bool)),Qt::UniqueConnection);
        slaveTcp->Start(QHostAddress(_MASTER_IP),_ECHO_PORT);
        pInfo->activateConnections();
    }


    GWindowRoot.setNewPage(_PG_STARTUP,_PG_STARTUP,0);


    pConfig->systemInitialized = true;
    return mainApplication.exec();


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //                          CONNESSIONI A CAMBI PAGINA
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

}
