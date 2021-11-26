#ifndef BIOPSYMODULE_H
#define BIOPSYMODULE_H
#include "application.h"

class biopsyModule
{

public:
    biopsyModule();
    biopsyConf_Str config;      // Dati di configurazione

    bool connected;             // Stato della connessione
    unsigned char model;        // Modello identificato

    // Dati perifierica collegata
    unsigned char checksum_h;
    unsigned char checksum_l;
    unsigned char revisione;

    // ID comando in corso
    int activationId;


    void defaultConfigData(void);
    bool openCfg(void);         // Funzione per l'apertura del file di configurazione
    bool storeConfig(void);     // Salva la configurazione
    bool updateConfig(void);    // Aggioirna M4 con i valori correnti

};

#endif // BIOPSYMODULE_H
