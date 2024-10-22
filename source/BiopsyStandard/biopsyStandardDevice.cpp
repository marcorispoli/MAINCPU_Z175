#include "../application.h"
#include "../appinclude.h"
#include "../globvar.h"

/*______________________________________________________________________
 *
 *  LIMITI DI POSIZIONAMENTO: MAXZ = OFFZ + OFSPAD - margPos - pos + 8
 *  ATTENZIONE: Il Pad BIOP_3D ha la posizione del Top p8 mm pi�
 *  bassa di quella del Pad 2D. Di conseguenza la posizione calibrata
 *  del compressore � falsata (pi� alta) di 8mm.
 *  Per compensare questo fattore si � corretto l'ofPad da 42 a 50mm
 *  e nel file di configurazione del compressore, l'offset di compressione
 *  � stato aumentato di 8 mm per aggiustare la misura del seno compresso
 *           _
 *          | |
 *          | |____
 *          |_|____|
 * POS...
 *  |8mm __              |  ---
 *      |  | ofspad=42   |  --- margPos = 5
 *      |  |__________   |
 *      |_____________|  |
 *                       |
 *                       !offz=189
 *_______________________|___________
 *///////////////////////////////////
 /*_______________________________________________________________________*/

void biopsyStandardDevice::activateConnections(void){

    connect(pConsole, SIGNAL(mccBiopsyStandardNotify(unsigned char,unsigned char,QByteArray)), this, SLOT(mccStatNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);
}

biopsyStandardDevice::biopsyStandardDevice(QObject *parent) :
    QObject(parent)
{

    Lago=0;
    Zlimit=0;
    Zlesione=0;
    codiceAccessorio="ND";
    codiceAgo="ND";

    curX = 0;
    curY = 0;
    curZ = 0;
    margZ = 0;
    accessorio = _BIOP_ACCESSORIO_ND;
    accessorioSelezionato = _BIOP_ACCESSORIO_ND;

}



/*
     Contenuto notifica m4-A5 biopsia
    #define _BP_CONNESSIONE 0
    #define _BP_SBLOCCO     1
    #define _BP_ACCESSORIO  2
    #define _BP_ZL          3
    #define _BP_ZH          4
    #define _BP_MOTION      5
    #define _BP_MOTION_END  6

     Notifica da driver di gestione
     data[_BP_CONNESSIONE]: 0=NULLA, 1 = ->CONNESSO, 2->NON CONNESSO
     data[_BP_SBLOCCO]: 0=NULLA, 1 = ->SBLOCCO ON, 2-> SBLOCCO OFF
     data[_BP_ACCESSORIO]: 0,1,2,7
     data[_BP_ZH :_BP_ZL], Posizione corrente Z
     data[_BP_MOTION]: 1= muove X, 2=muoveY, 3=muoveZ 4=fine movimenti 5 = TIMEOUT TORRETTA (reset)
     data[_BP_MOTION_END] Risultato movimento (se data[4]==4)
            .0: 1 = X-ok, X-nok
            .1: 1 = Y-ok, Y-nok
            .2: 1 = Z-ok, Z-nok

     data[_BP_CHKH] = Checksum H
     data[_BP_CHKL] = Checksum L
     data[_BP_REVIS]= Revisione

 */
void biopsyStandardDevice::mccStatNotify(unsigned char id_notify,unsigned char cmd, QByteArray data)
{
    unsigned char errore;
    QString stringaId;
    static bool flag_connected=false;
    static unsigned char flag_accessorio = _BIOP_ACCESSORIO_ND;

    if(id_notify!=1) return;
    //if(pConfig->startupCompleted == false) return;

    errore=0;
    if(cmd!=BIOP_NOTIFY_STAT) return;
    if(pBiopsy->model == BYM_EXTENDED_DEVICE) return;

    // Se arriva qui il modulo ha comunicato con successo con la torretta standard
    pBiopsy->model = BYM_STANDARD_DEVICE;
    ApplicationDatabase.setData(_DB_BIOP_MODEL, (int) BYM_STANDARD_DEVICE);

    // _____________________  Stato connessione____________________________________________________________________________________
    if(data.at(_BP_STD_CONNESSIONE)==1)
    {
        // Riconoscimento torretta di Biopsia inserita
        pBiopsy->connected = TRUE;
        pBiopsy->checksum_h=data[_BP_STD_CHKH];
        pBiopsy->checksum_l=data[_BP_STD_CHKL];
        pBiopsy->revisione=data[_BP_STD_REVIS];

        // Reset dati biopsia passati da consolle
        Lago=0;
        Zlimit=0;
        Zlesione=0;
        codiceAccessorio="ND";
        codiceAgo="ND";
        accessorioSelezionato = 0;
        pBiopsy->activationId = 0;

        // Disabilita i movimenti manuali
        ApplicationDatabase.setData(_DB_BIOP_MANUAL_ENA,(unsigned char) 0 ,0);

        // <0 viene visualizzato "ND"
        curX=curY=curZ=-1;
        ApplicationDatabase.setData(_DB_BIOP_X,(int) -1);
        ApplicationDatabase.setData(_DB_BIOP_Y,(int) -1);
        ApplicationDatabase.setData(_DB_BIOP_Z,(int) -1);


        // Notifies the AWS with Async message
        if(flag_connected!=true) pToConsole->setConfigChanged(pToConsole->ACCESSORIO);
        flag_connected=true;

        // Aggiorna le pagine con nil riconoscimento della Biopsia
        ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) BIOPSY_DEVICE,0);
        ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("BIOPSY","NOME ACCESSORIO", 0, QApplication::UnicodeUTF8)),0);

    }
    else if(data.at(_BP_STD_CONNESSIONE)==2)
    {
        pBiopsy->connected = FALSE;
        pBiopsy->activationId = 0;

        // Disabilita i movimenti manuali
        ApplicationDatabase.setData(_DB_BIOP_MANUAL_ENA,(unsigned char) 0 ,0);

        flag_connected=false;
        return;
    }

    // ___________________________Pulsante di sblocco Braccio____________________________________________________________________
    if(data.at(_BP_STD_SBLOCCO)==1)
    {
        ApplicationDatabase.setData(_DB_BIOP_UNLOCK_BUTTON,(unsigned char) 1,0);
    }
    else if(data.at(_BP_STD_SBLOCCO)==2)
    {
        ApplicationDatabase.setData(_DB_BIOP_UNLOCK_BUTTON,(unsigned char) 0,0);
    }

    // ____________________________Aggiorna Accessorio se necessario_____________________________________________________________
    if(data.at(_BP_STD_ACCESSORIO)!=255)
    {
        bool stat=false;
        switch(data.at(_BP_STD_ACCESSORIO))
        {
        case 0: // Errore corto circuito su riconoscimento accessorio
            accessorio = _BIOP_ACCESSORIO_ND;
            stringaId = QString(QApplication::translate("BIOPSY","ACCESSORIO GUASTO", 0, QApplication::UnicodeUTF8));
            stat = false;
            break;
        case 1: // Non definito
            //accessorio = _BIOP_ACCESSORIO_MAMMOTOME;
            accessorio = _BIOP_ACCESSORIO_ND;
            stringaId = QString(QApplication::translate("BIOPSY","ACCESSORIO NON AMMESSO", 0, QApplication::UnicodeUTF8));
            stat=false;
            break;
        case 2: // Porta Ago
            accessorio = _BIOP_ACCESSORIO_AGO;
            stringaId = QString(QApplication::translate("BIOPSY","PORTA AGO", 0, QApplication::UnicodeUTF8));
            stat = true;
            break;
        case 3: // non definito
            //accessorio = _BIOP_ACCESSORIO_GUN;
            accessorio = _BIOP_ACCESSORIO_ND;
            stringaId = QString(QApplication::translate("BIOPSY","ACCESSORIO NON AMMESSO", 0, QApplication::UnicodeUTF8));
            stat=false;
            break;
        default: // Non connesso
            accessorio = _BIOP_ACCESSORIO_ND;
            stringaId = QString(QApplication::translate("BIOPSY","ACCESSORIO ASSENTE", 0, QApplication::UnicodeUTF8));
            stat = false;
            break;
        }

        if(stat==false) ApplicationDatabase.setData(_DB_BIOP_HOLDER,QString(""),0);
        else ApplicationDatabase.setData(_DB_BIOP_HOLDER,stringaId,0);


        // Notifies the AWS with Async message
        if(flag_accessorio!=accessorio) pToConsole->setConfigChanged(pToConsole->ACCESSORIO);
        flag_accessorio = accessorio;

    }

    // _____________________________ Aggiornamento posizione puntatore ______________________________________________________________
    // Aggiorna massima posizione meccanica cursore senza impattare compressore
    maxZ = data.at(_BP_STD_MAX_Z);
    curZ = data.at(_BP_STD_ZL) + 256*data.at(_BP_STD_ZH);

    margZ = Zlimit -(curZ/10);
    if(margZ<0) margZ=0;


    ApplicationDatabase.setData(_DB_BIOP_MARG,QString("%1 (mm)").arg((int) margZ));
    ApplicationDatabase.setData(_DB_BIOP_Z,QString("%1").arg((float)curZ/10.0,0,'f',1));

    if(maxZ > (curZ/10)) ApplicationDatabase.setData(_DB_BIOP_MAXZ,QString("%1 (mm)").arg((int) (maxZ-(curZ/10))));
    else ApplicationDatabase.setData(_DB_BIOP_MAXZ, QString("0 (mm)"));


    // ______________________________ Aggiornamento stato di posizionamento in corso _________________________________________________
    switch(data.at(_BP_STD_MOTION))
    {
        case 1:

        break;
        case 2:

        break;
        case 3:

        break;
        case 4:

            // Valutazione del risultato del movimento
            if(!(data.at(_BP_STD_MOTION_END)&1))
            {// Errore su X
                errore = 0x2;
                curX = -1;
                ApplicationDatabase.setData(_DB_BIOP_X,(int) -1);
                if(pBiopsy->activationId) PageAlarms::activateNewAlarm(_DB_ALLARMI_BIOPSIA,ERROR_BIOP_MOVE_X,TRUE);
            }else{
                curX = targetX;
                ApplicationDatabase.setData(_DB_BIOP_X,(int) curX);
                //ApplicationDatabase.setData(_DB_BIOP_X,QString("%1").arg((float)curX/10.0,0,'f',1));
            }

            if(!(data.at(_BP_STD_MOTION_END)&2))
            {// Errore su Y
                errore = 0x4;
                curY = -1;
                ApplicationDatabase.setData(_DB_BIOP_Y,(int) -1);
                //ApplicationDatabase.setData(_DB_BIOP_Y,QString("ND"));
                if(pBiopsy->activationId) PageAlarms::activateNewAlarm(_DB_ALLARMI_BIOPSIA,ERROR_BIOP_MOVE_Y,TRUE);
            }else{
                curY = targetY;
                ApplicationDatabase.setData(_DB_BIOP_Y,(int) curY);
            }

            if(!(data.at(_BP_STD_MOTION_END)&4))
            {// Errore su Z
                errore = 0x8;
                curZ = -1;
                ApplicationDatabase.setData(_DB_BIOP_Z,(int) -1);
                if(pBiopsy->activationId) PageAlarms::activateNewAlarm(_DB_ALLARMI_BIOPSIA,ERROR_BIOP_MOVE_Z,TRUE);
            }else{
                curZ = targetZ;
                ApplicationDatabase.setData(_DB_BIOP_Z,(int) curZ);
            }

            // Anche in caso di errore abilita i movimenti manuali
            if(curZ>10) ApplicationDatabase.setData(_DB_BIOP_MANUAL_ENA,(unsigned char) 1 ,0);

            // Comunicazione dell'esito del comando in corso alla Console
            if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, errore);
            else pToConsole->setBiopsyStandardPosition(curX, curY,curZ);
            pBiopsy->activationId = 0;

        break;
        case 5: // TIMEOUT TORRETTA
            curX=curY=curZ=-1;
            ApplicationDatabase.setData(_DB_BIOP_X,(int) -1);
            ApplicationDatabase.setData(_DB_BIOP_Y,(int) -1);
            ApplicationDatabase.setData(_DB_BIOP_Z,(int) -1);

            if(pBiopsy->activationId) pToConsole->endCommandAck(pBiopsy->activationId, 0x1);
            pBiopsy->activationId = 0;

            // Disabilita i movimenti manuali
            ApplicationDatabase.setData(_DB_BIOP_MANUAL_ENA,(unsigned char) 0 ,0);

        break;
        case 6: // Posizione compressore troppo alta per step-z
            //this->id_console = 0;
            //PageAlarms::activateNewAlarm(_DB_ALLARMI_BIOPSIA, ERROR_BIOP_MOVE_Z_NON_CONSENTITO,TRUE);
        break;
        case 7:
            //this->id_console = 0;
            //PageAlarms::activateNewAlarm(_DB_ALLARMI_BIOPSIA, ERROR_BIOP_MOVE_Z_NON_CONSENTITO,TRUE);
        break;

    } // data.at(_DB_MOTION)

}



/*
  mcc_cmd.buffer[0]: XGO (1=muove X)
  mcc_cmd.buffer[1]: X L
  mcc_cmd.buffer[2]: X H
  mcc_cmd.buffer[3]: YGO (1=muove X)
  mcc_cmd.buffer[4]: Y L
  mcc_cmd.buffer[5]: Y H
  mcc_cmd.buffer[6]: ZGO (1=muove X)
  mcc_cmd.buffer[7]: Z L
  mcc_cmd.buffer[8]: Z H
  mcc_cmd.buffer[9]: ZlimitGO
  mcc_cmd.buffer[10]: Zl L
  mcc_cmd.buffer[11]: ZlesioneGO
  mcc_cmd.buffer[12]: Zles L
  mcc_cmd.buffer[13]: ZlagoGO
  mcc_cmd.buffer[14]: Zlago L

*/
bool biopsyStandardDevice::moveXYZ(unsigned short X, unsigned short Y, unsigned short Z)
{
    unsigned char data[15];

    data[0]=1;
    data[1]=(unsigned char) (X & 0x00FF);
    data[2]=(unsigned char) (X >>8);
    data[3]=1;
    data[4]=(unsigned char) (Y & 0x00FF);
    data[5]=(unsigned char) (Y >>8);
    data[6]=1;
    data[7]=(unsigned char) (Z & 0x00FF);
    data[8]=(unsigned char) (Z >>8);
    data[9]=0;
    data[11]=0;
    data[13]=0;

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_STD_XYZ,1,data,9)==FALSE)
    {
        LOG("BIOPSY <moveXYZ>: ERRORE COMANDO MCC");
        return FALSE;
    }


    return TRUE;
}

bool biopsyStandardDevice::moveZ(unsigned short Z)
{
    unsigned char data[15];

    data[0]=0;
    data[3]=0;
    data[6]=1;
    data[7]=(unsigned char) (Z & 0x00FF);
    data[8]=(unsigned char) (Z >>8);
    data[9]=0;
    data[11]=0;
    data[13]=0;

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_STD_XYZ,1,data,9)==FALSE)
    {
        LOG("BIOPSY <moveZ>: ERRORE COMANDO MCC");
        return FALSE;
    }


    return TRUE;
}

// Muove di uno step definito dal parametro passato
// Step è definito in decimi di millimetro
bool biopsyStandardDevice::stepZ(int step)
{
    unsigned char data[2];


    // Il driver controller�  la posizione del compressore
    if(step>0)
    {
        // Controllo sulla posizione di Z
        if(curZ + step > maxZ * 10) return FALSE; // Controllo anti impatto
        if(curZ + step > Zlimit * 10) return FALSE;        // Controllo su Zlimit (margine)
        data[0]=1;
        data[1]=step;
    }
    else
    {
        data[0]=2;
        data[1]=-1*step;
    }

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_STD_XYZ,1,data,sizeof(data))==FALSE)
    {        
        return FALSE;
    }

    return TRUE;
}

bool biopsyStandardDevice::moveX(unsigned short X)
{
    unsigned char data[15];

    data[0]=1;
    data[1]=(unsigned char) (X & 0x00FF);
    data[2]=(unsigned char) (X >>8);
    data[3]=0;
    data[6]=0;
    data[9]=0;
    data[11]=0;
    data[13]=0;

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_STD_XYZ,1,data,9)==FALSE)
    {
        LOG("BIOPSY <moveX>: ERRORE COMANDO MCC");
        return FALSE;
    }


    return TRUE;
}

bool biopsyStandardDevice::moveY(unsigned short Y)
{
    unsigned char data[15];

    data[0]=0;
    data[3]=1;
    data[4]=(unsigned char) (Y & 0x00FF);
    data[5]=(unsigned char) (Y >>8);
    data[6]=0;
    data[9]=0;
    data[11]=0;
    data[13]=0;

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_STD_XYZ,1,data,9)==FALSE)
    {
        LOG("BIOPSY <moveY>: ERRORE COMANDO MCC");
        return FALSE;
    }


    return TRUE;
}

// Funzione chiamata dalla AWS per richiedere il movimento
// operativo.
// RETURN 0: OK
// cmd_id==-1 --> simula il comando senza attivare il movimento
int biopsyStandardDevice::setBiopsyData(unsigned int x, unsigned int y, unsigned int z, // Posizione da raggiungere
                          unsigned int z_limit,     // Massima Z calcolata dalla AWS c
                          unsigned int z_lesione,   // Posizione rilevata della lesione
                          unsigned int lAgo,        // Lunghezza dell'ago
                          unsigned int holder_code, // Codice holder utilizzato dalla AWS
                          QString  nome_accessorio,  // Nome dellk'accessorio montato da AWS
                          int cmd_id                 // Id del comando richiesto
                          )
{


    // Check presenza
    if(pBiopsy->connected == FALSE)  return 1;

    // Non consente di gestire un comando con un movimento gi�  in corso
    if((pBiopsy->activationId!=0)&&(cmd_id!=-1))  return 3;

    // Acquisisce i dati
    targetX = x;
    targetY = y;
    targetZ = z;
    Zlimit = z_limit; // mm
    Zlesione = z_lesione; // mm
    Lago = lAgo; // mm
    accessorioSelezionato = holder_code;
    codiceAgo = nome_accessorio;


    // Verifica se trattasi di ritorno in Home
    if((targetX==0)&&(targetY==0)&&(targetZ==0)){



        // Reset dati biopsia passati da consolle
        Lago=0;
        Zlimit=0;
        Zlesione=0;
        codiceAccessorio="ND";
        codiceAgo="ND";
        accessorioSelezionato = _BIOP_ACCESSORIO_ND;
        margZ=0;
        ApplicationDatabase.setData(_DB_BIOP_MARG,QString(""));
        ApplicationDatabase.setData(_DB_BIOP_MANUAL_ENA,(unsigned char) 0 ,0);
        ApplicationDatabase.setData(_DB_BIOP_AGO,QString(""),0);
        if(updateTorretta(cmd_id)==FALSE) return 7;

        pToConsole->setBiopsyStandardPosition(0,0,0); // Avvisa subito la AWS per evitare magheggi sull'interfaccia
        return 0;
    }

    // Controllo dei valori ricevuti
    if(targetZ > (maxZ+1) * 10) return 6;               // Controllo limite meccanico per impatto con il compressore

    // Imposta il nome dell'accessorio utilizzato
     ApplicationDatabase.setData(_DB_BIOP_AGO,codiceAgo,0);
    if(cmd_id==-1) return 0; // Solo simulato

    if( updateTorretta(cmd_id)==FALSE) return 7;
    return 0;

}
// Update dati torretta da Console
bool biopsyStandardDevice::updateTorretta(unsigned char id_command)
{
    unsigned char data[15];

    data[0]=1;
    data[1]=(unsigned char) (targetX & 0x00FF);
    data[2]=(unsigned char) (targetX >>8);
    data[3]=1;
    data[4]=(unsigned char) (targetY & 0x00FF);
    data[5]=(unsigned char) (targetY >>8);
    data[6]=1;
    data[7]=(unsigned char) (targetZ & 0x00FF);
    data[8]=(unsigned char) (targetZ >>8);

    data[9]=1;
    data[10]=Zlimit;
    data[11]=1;
    data[12]=Zlesione;
    data[13]=1;
    data[14]=Lago;

    if(pConsole->pGuiMcc->sendFrame(MCC_BIOPSY_STD_XYZ,1,data,sizeof(data))==FALSE)
    {
        LOG("BIOPSY <updateTorretta>: ERRORE COMANDO MCC");
        return FALSE;
    }

    pBiopsy->activationId = id_command;
    return TRUE;
}

