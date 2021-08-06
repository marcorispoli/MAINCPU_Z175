#include "application.h"
#include "appinclude.h"
#include "globvar.h"



void biopsy::activateConnections(void){
    connect(pConsole, SIGNAL(mccBiopsyNotify(unsigned char,unsigned char,QByteArray)), this, SLOT(mccStatNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
}

biopsy::biopsy(QObject *parent) :
    QObject(parent)
{
    connected = FALSE;
    checksum_h=0;
    checksum_l=0;
    revisione=0;

    // Nel caso in cui ci fosse un errore nel file o il file non
    // esiste vengono caricati dei valori di default
    openCfg();


}

void biopsy::defaultConfigData(void){

    // Offset di puntamento
    config.Z_homePosition = 193;            // (mm) distanza zero torretta - fibra di carbonio
    config.Z_basePosizionatore = 189;       // Distanza base metallica - fibra di carbonio

    // Gestion Movimento Pad
    config.offsetPad = 59;              // Offset linea di calibrazione posizione - superficie staffe metalliche
    config.margineRisalita = 15;        // Margine di sicurezza per impatto con il compressore in risalita
    config.marginePosizionamento = 5;   // Margine di sicurezza impatto con il compressore in puntamento

}


bool biopsy::openCfg(void)
{
    QString filename;
    QList<QString> dati;

    // Default File di configurazione
    defaultConfigData();

    filename =  QString("/resource/config/biopsy.cnf");

    // Se nn esiste lo crea con i default
    QFile file(filename.toAscii());
    if(!file.exists()){
        return storeConfig();
    }

    // Se è corrotto lo crea di default
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return storeConfig();
    }

    // Procede con la lettura del formato corrente
    while(1){

        dati = Config::getNextArrayFields(&file);
        if(dati.isEmpty()) break;

        if(dati.at(0)=="Z_HOME"){
            config.Z_homePosition = dati.at(1).toInt();

        }else if(dati.at(0)=="Z_BASE"){
             config.Z_basePosizionatore = dati.at(1).toInt();

        }else if(dati.at(0)=="OFFSET_PAD"){
            config.offsetPad = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_RISALITA"){
            config.margineRisalita = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_POSIZIONAMENTO"){
            config.marginePosizionamento = dati.at(1).toInt();
        }

    }

    file.close();
    return true;
}

/*
 *
 *  Salva il file di configurazione della biopsia
 */
bool biopsy::storeConfig(void)
{
    QString filename;

    filename =  QString("/resource/config/biopsy.cnf");
    QFile file(filename.toAscii());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return FALSE;

    file.write( QString("<Z_HOME, %1>\n").arg((int) config.Z_homePosition).toAscii());
    file.write( QString("<Z_BASE, %1>\n").arg((int) config.Z_basePosizionatore).toAscii());

    file.write( QString("<OFFSET_PAD,%1>\n").arg((int) config.offsetPad).toAscii());
    file.write( QString("<MARGINE_RISALITA,%1>\n").arg((int) config.margineRisalita).toAscii());
    file.write( QString("<MARGINE_POSIZIONAMENTO,%1>\n").arg((int) config.marginePosizionamento).toAscii());


    file.flush();
    file.close();

    return true;
}


void biopsy::mccStatNotify(unsigned char id_notify,unsigned char cmd, QByteArray data)
{
    unsigned char errore;
    static unsigned char    bp_motion=0;
    static int              bp_movecommand=0;

    if(id_notify!=1) return;
    //if(pConfig->startupCompleted == false) return;

    errore=0;
    if(cmd!=BIOP_NOTIFY_STAT) return;


    // Se il sistema risulta NON connesso non fa altro..
    if(data.at(_BP_CONNESSIONE)==_BP_CONNESSIONE_DISCONNECTED)
    {
        connected = FALSE;
        model = _BP_MODEL_UNDEFINED;
        laterality = _BP_LATERALITY_UNDEFINED;
        if(movingCommand > _BIOPSY_MOVING_COMPLETED){
            movingCommand =_BIOPSY_MOVING_COMPLETED;
            movingError = _BIOPSY_MOVING_ERROR_TIMEOUT;

            if(activationId) pToConsole->endCommandAck(activationId, movingError);
            activationId = 0;
        }
        return;
    }

    // Cambio stato da Non connesso a connesso
    if(!connected){

        // Riconoscimento torretta di Biopsia inserita
        connected = TRUE;
        checksum_h=data[_BP_CHKH];
        checksum_l=data[_BP_CHKL];
        revisione=data[_BP_REVIS];
        model = data[_BP_MODEL];

        movingCommand =_BIOPSY_MOVING_NO_COMMAND;
        movingError = _BIOPSY_MOVING_NO_ERROR;
        activationId = 0;
        needle_home = 0;

        // Aggiorna le pagine con nil riconoscimento della Biopsia
        ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) BIOPSY_DEVICE,0);
        ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("BIOPSY","NOME ACCESSORIO", 0, QApplication::UnicodeUTF8)),0);        

    }

    // Pulsante di sblocco Braccio e funzioni biopsia
    if(data.at(_BP_PUSH_SBLOCCO) == _BP_PUSH_SBLOCCO_ATTIVO) unlock_button = true;
    else unlock_button = false;
    ApplicationDatabase.setData(_DB_BIOP_UNLOCK_BUTTON,(int) data.at(_BP_PUSH_SBLOCCO),0);

    //  Riconoscimento dell'adapter
    if(adapterId != data.at(_BP_ADAPTER_ID)){
        adapterId = data.at(_BP_ADAPTER_ID);
        pToConsole->setConfigChanged(pToConsole->ACCESSORIO);
        ApplicationDatabase.setData(_DB_BIOP_ADAPTER_ID,(int) data.at(_BP_ADAPTER_ID),0);
    }

    // Posizione attuale cursore
    curX_dmm = data.at(_BP_XL) + 256 * data.at(_BP_XH) ;
    curY_dmm = data.at(_BP_YL) + 256 * data.at(_BP_YH) ;
    curZ_dmm = data.at(_BP_ZL) + 256 * data.at(_BP_ZH) ;
    curSh_dmm= data.at(_BP_SHL) + 256 * data.at(_BP_SHH) ;

    ApplicationDatabase.setData(_DB_BIOP_X,(int) curX_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Y,(int) curY_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_Z,(int) curZ_dmm,0);
    ApplicationDatabase.setData(_DB_BIOP_SH,(int) curSh_dmm,0);

    // Calcolo del Margine tra Paddle e Staffa metallica
    paddle_margine  = data.at(_BP_PADDLE_MARGINE);
    max_z_paddle  = data.at(_BP_MAX_Z_PADDLE);

    // Ricalcolo dei vari parametri di limitazione
    calcoloMargini();

    if(bp_motion!=data.at(_BP_MOTION)){
        PRINT(QString("BIOPSIA: BP_MOTION=%1").arg((int) data.at(_BP_MOTION)));
        bp_motion = data.at(_BP_MOTION);
    }

    if(bp_movecommand!=movingCommand){
        PRINT(QString("BIOPSIA: MOVE CMD=%1").arg((int) movingCommand));
        bp_movecommand = movingCommand;
    }

    // ______________________________ Aggiornamento stato di posizionamento in corso _________________________________________________
    switch(data.at(_BP_MOTION))
    {

        case _BP_MOTION_ON:
            PRINT(QString("BIOPSIA:CONFERMA ATTIVAZIONE MOVIMENTO"));
        break;

        case _BP_MOTION_TERMINATED:

            // Valutazione del risultato del movimento
            if(data.at(_BP_MOTION_END) == _BP_ERROR_POSITIONINIG)
            {
                movingError = _BIOPSY_MOVING_ERROR_TARGET;
            }else if(data.at(_BP_MOTION_END) == _BP_TIMEOUT_COMANDO)
            {
                movingError = _BIOPSY_MOVING_ERROR_TIMEOUT;
            }else  movingError = _BIOPSY_MOVING_NO_ERROR;

            movingCommand =_BIOPSY_MOVING_COMPLETED;

            // Notifica di fine movimento
            if(activationId) pToConsole->endCommandAck(activationId, movingError);
            activationId = 0;

        break;


    } // data.at(_DB_MOTION)


}

/*
 *  Sulla base dei dati presenti si calcolano i vari
 *  parametri di limitazione al movimento
 */
void biopsy::calcoloMargini(void){

    // Paddle Margine è calcolato dal driver
    ApplicationDatabase.setData(_DB_BIOP_PADDLE_MARGINE,(int) paddle_margine,0);

    // Needle margine in mm
    int margine = config.Z_homePosition - (curZ_dmm/10) - needle_home;
    if(margine < 0) margine = 0;
    needle_margine = margine;
    ApplicationDatabase.setData(_DB_BIOP_NEEDLE_MARG,(int) margine,0);

    int max_needle_z = config.Z_homePosition - needle_home - 5;

    // Calcolo della massima Z
    if(max_z_paddle < max_needle_z) abs_max_z = max_z_paddle;
    else abs_max_z = max_needle_z;
    ApplicationDatabase.setData(_DB_BIOP_MAXZ,(int) abs_max_z,0);

}

int biopsy::moveXYZ(unsigned short X, unsigned short Y, unsigned short Z, int id)
{
    unsigned char data[15];

    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return movingError;
    }

    data[0]=_MCC_BIOPSY_CMD_MOVE_XYZ; // Codice comando
    data[1]=(unsigned char) (X & 0x00FF);
    data[2]=(unsigned char) (X >>8);
    data[3]=(unsigned char) (Y & 0x00FF);
    data[4]=(unsigned char) (Y >>8);
    data[5]=(unsigned char) (Z & 0x00FF);
    data[6]=(unsigned char) (Z >>8);

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,7)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        pBiopsy->activationId = 0;
        return movingError;
    }

    pBiopsy->activationId = id;
    movingCommand =_BIOPSY_MOVING_XYZ;
    movingError = _BIOPSY_MOVING_NO_ERROR;
    return _BIOPSY_MOVING_NO_ERROR;
}

int biopsy::moveHome(int id)
{
    unsigned char data[1];

    // Azzera l'informazione dell'Ago per il calcolo del margine
    needle_home = 0;

    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;        
        return movingError;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_HOME; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        pBiopsy->activationId = 0;
        return movingError;
    }

    pBiopsy->activationId = id;
    movingCommand =_BIOPSY_MOVING_HOME;
    movingError = _BIOPSY_MOVING_NO_ERROR;
    return _BIOPSY_MOVING_NO_ERROR;
}


int biopsy::moveDecZ(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_DECZ; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECZ;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}


int biopsy::moveIncZ(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_INCZ; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCZ;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::moveDecX(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_DECX; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECX;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::moveIncX(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_INCX; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCX;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::moveDecY(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_DECY; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_DECY;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}
int biopsy::moveIncY(int id)
{
    unsigned char data[1];
    if(movingCommand > _BIOPSY_MOVING_COMPLETED)
    {
        movingError = _BIOPSY_MOVING_ERROR_BUSY;
        movingCommand = _BIOPSY_MOVING_COMPLETED;
        return false;
    }


    data[0]=_MCC_BIOPSY_CMD_MOVE_INCY; // Codice comando
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,1)==FALSE)
    {
        movingCommand =_BIOPSY_MOVING_COMPLETED;
        movingError = _BIOPSY_MOVING_ERROR_MCC;
        return FALSE;
    }

    pBiopsy->activationId = id;
    movingCommand = _BIOPSY_MOVING_INCY;
    movingError   = _BIOPSY_MOVING_NO_ERROR;
    return TRUE;
}

int biopsy::setStepVal(unsigned char step)
{
    unsigned char data[2];
    data[0]=_MCC_BIOPSY_CMD_SET_STEPVAL; // Codice comando
    data[1] = step;
    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_CMD,1,data,2)==FALSE) return 0;
    return 1;
}


bool biopsy::updateConfig(void)
{
    // Invia la configurazione al driver per aggiornarlo in diretta
    return pConfig->sendMccConfigCommand(CONFIG_BIOPSY);

}
