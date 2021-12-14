#ifndef BIOPSYMODULE_H
#define BIOPSYMODULE_H
#include "application.h"

class biopsyModule
{

public:
    biopsyModule();
    biopsyConfStd_Str configStd;      // Dati di configurazionebiopsia standard
    biopsyConfExt_Str configExt;      // Dati di configurazionebiopsia estesa

    bool connected;             // Stato della connessione
    unsigned char model;        // Modello identificato

    // Dati perifierica collegata
    unsigned char checksum_h;
    unsigned char checksum_l;
    unsigned char revisione;

    // ID comando in corso
    int activationId;


    void defaultConfigDataStandard(void);
    void defaultConfigDataExtended(void);
    bool openCfgStandard(void);         // Funzione per l'apertura del file di configurazione
    bool openCfgExtended(void);         // Funzione per l'apertura del file di configurazione
    bool storeConfigStandard(void);     // Salva la configurazione
    bool storeConfigExtended(void);     // Salva la configurazione
    bool updateConfig(void);            // Aggioirna M4 con i valori correnti

};

#endif // BIOPSYMODULE_H
