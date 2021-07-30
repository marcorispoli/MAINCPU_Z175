#ifndef COLLIMATORE_H
#define COLLIMATORE_H


#include "application.h"

//#define COLLI_DBG

class Collimatore : public QObject
{
    Q_OBJECT
public:
    explicit Collimatore(QObject *parent = 0);
    void activateConnections(void);


typedef enum
{
    MIRROR_HOME= 0,
    MIRROR_OUT,
    MIRROR_ND
}_MirrorCmd_Enum;

typedef enum
{
   LAMP_OFF=0,
   LAMP_ON,
   LAMPMIRR_OFF,
   LAMPMIRR_ON
}_LampCmd_Enum;

typedef enum
{
    FILTRO_Rh=0,
    FILTRO_Al,
    FILTRO_Ag,
    FILTRO_Cu,
    FILTRO_Mo,
    FILTRO_ND
}_FilterCmd_Enum;

signals:
    

public:
    // Azionamento Specchio
    void setMirror(_MirrorCmd_Enum cmd);                    // Imposta lo stato dello Specchio
    void setLamp(_LampCmd_Enum param, unsigned char tmo);   // Attivazione Lampada centratore
    bool updateColli(void);                                 // Effettua l'update del collimatore
    bool manualColliUpdate(void);                           // Modalit√  di collimazione manuale

    bool setFiltro(void);                                   // Impostazione filtro predefinito a filter_Cmd
    bool setFiltro(_FilterCmd_Enum cmd, bool update);         // Impostazione del filtro richiesto
    bool manualSetFiltro(void);                             // Impostazione filtro in modo manuale
    bool manualSetFiltro(unsigned char index);

    void startColliTest(unsigned char num);                           // Attiva test ciclico sulle lame del collimatore

    QString getFiltroTag(unsigned char code);               // Restituisce il tag simbolico per il dato codice filtro
    _FilterCmd_Enum getFiltroStat(void){return filtroStat;} // Restituisce lo stato del filtro

    int getColli2DIndex(int pad);              // Restituisce l'indice della lista dei dati di collimazione 2D

    static void getCalibTomoCurve(bool R, int nRef,double footprint, double dispersione, unsigned char* dataP, unsigned char* dataN); // Restituisce la curva teorica

    unsigned char getDynamicColliTargetL(float angolo, float dispersione, unsigned char nRef, float footprint);
    unsigned char getDynamicColliTargetR(float angolo, float dispersione, unsigned char nRef, float footprint);
    void timerEvent(QTimerEvent* ev); // Override della classe QObject

public slots:
    void guiNotifySlot(unsigned char id, unsigned char mcccode, QByteArray buffer); // Notifica esecuzione azionamento specchio
    void changedPadNotify(void);
    void pcb249U1Notify(unsigned char id, unsigned char notifyCode, QByteArray buffer);

private:
    _MirrorCmd_Enum mirrorStat;   // 0=fuori campo, 1=in campo, 2=indeterminato    
    _FilterCmd_Enum filtroStat; // Stato corrente del filtro

public:
    unsigned char colli_model;
    bool     alrCuffia;     // Allarme temperatura cuffia
    bool     alrSensCuffia; // Allarme temperatura cuffia
    bool     lampStat;      // Stato Lampada centratore: TRUE = ACCESA

    unsigned char accessorio; // Accessorio riconosciuto

    // Gestione file di configurazione   
    #define COLLI_CNF_REV   3   // Revisione file di configurazione del collimatore

    bool colliConfUpdated;      // Configurazione aggiornata
    colliConf_Str colliConf;    // Configurazione collimatore
    void setTomoConfigDefault(QString mat);

    // Lettura/Scrittura files di configurazione: il file di configurazione dipente dal Gantry Type
    bool readConfigFile(void) ;
    bool storeConfigFile(void);



    // Ultimi comandi attivati
    _FilterCmd_Enum filtroCmd;  // Richiesta stato filtro durante updateColli

    // DATI PER COLLIMAZIONE MANUALE
    bool manualCollimation;     // Attiva l'impostazione manuale della collimazione
                                // sganciandola dalla selezione del PAD corrente
    bool manualFiltroCollimation;     // Attiva l'impostazione manuale della collimazione



    unsigned char manualFilter; // Impostazione manuale del filtro
    int manualL;
    int manualR;
    int manualT;
    int manualB;
    int manualF;

    // Collimazione Custom
    int customL;
    int customR;
    int customT;
    int customB;
    int customF;



    QString getFilterTag(unsigned char code)
    {
        if(code==FILTRO_Rh) return QString("Rh");
        else if(code==FILTRO_Al) return QString("Al");
        else if(code==FILTRO_Ag) return QString("Ag");
        else if(code==FILTRO_Cu) return QString("Cu");
        else if(code==FILTRO_Mo) return QString("Mo");
        return QString("UNDEFINED");
    }

    // Imposta il comando filtro utilizzando un TAG e ne verifica l'esattezza
    bool setFilterTag(QString tag)
    {
        if(getFilterTag(FILTRO_Rh) == tag) filtroCmd = FILTRO_Rh;
        else if(getFilterTag(FILTRO_Al) == tag) filtroCmd = FILTRO_Al;
        else if(getFilterTag(FILTRO_Ag) == tag) filtroCmd = FILTRO_Ag;
        else if(getFilterTag(FILTRO_Cu) == tag) filtroCmd = FILTRO_Cu;
        else if(getFilterTag(FILTRO_Mo) == tag) filtroCmd = FILTRO_Mo;
        else return FALSE;

        return TRUE;

    }


    unsigned char collimazione_frustoli;

    // Collimazione manuale in operativo
    void selectManualColliFormat(unsigned char pad); // Selezione di una particolare collimazione

private:
    int colliTestNumber;
    int colliTestTimer;
    bool colliTestStatus;
    bool filter_test_status;



};

#endif // COLLIMATORE_H
