#define _BIOPSY_STARTUP_C
#include "dbt_m4.h"

// Alla prima connessione determina quale BYM è connesso e
// conseguentemente attiva la thread relativa fino a spegnimento
void BIOPSY_driver(uint32_t taskRegisters)
{
    // Inizializzazione di tutta la struttura dati relativa alla biopsia
    generalConfiguration.biopsyCfg.biopsyConnected = FALSE;
    generalConfiguration.biopsyCfg.biopsyArmEna = false;
    generalConfiguration.biopsyCfg.Z = 0;
    generalConfiguration.biopsyCfg.adapterId = 0;
    generalConfiguration.biopsyCfg.sbloccoReq=FALSE;
    generalConfiguration.biopsyCfg.checksum_h = 0;
    generalConfiguration.biopsyCfg.checksum_l = 0;
    generalConfiguration.biopsyCfg.revisione = 0;

    generalConfiguration.biopsyCfg.standardConf.needlePresent = FALSE;
    generalConfiguration.biopsyCfg.standardConf.movimento = FALSE;
    generalConfiguration.biopsyCfg.standardConf.movimento_x = FALSE;
    generalConfiguration.biopsyCfg.standardConf.movimento_y = FALSE;
    generalConfiguration.biopsyCfg.standardConf.movimento_z = FALSE;
    generalConfiguration.biopsyCfg.standardConf.stepUp_z = FALSE;
    generalConfiguration.biopsyCfg.standardConf.stepDwn_z = FALSE;

    // Configurazione Biopsia Estesa    
    generalConfiguration.biopsyCfg.extendedConf.stepVal = 10; // Default = 10

    // Attende la configurazione del sistema prima di procedere
    while(generalConfiguration.deviceConfigOk==FALSE) _time_delay(1000);

    // Inizio ciclo di interrogazione per identificare la torretta e il modello
    bool isStandard = false;

    while(1){
        _time_delay(1000);
        if(generalConfiguration.potterCfg.potId != POTTER_UNDEFINED) continue;

        // Prova a chiedere getStat per la standard
        if(biopsyStandardIsPresent()){
            isStandard = true;
            break;
        }

        // Prova a chiedere per la Extended
        if(BiopsyExtendedIsPresent()){
            isStandard = false;
            break;
        }
    }

    // Attiva i loop relativi alle due torrette
    if(isStandard) biopsyStandardLoop();
    else biopsyExtendedLoop();
}

/*
  Funzione configuratrice biopsia
  buffer[0]: offsetFibra
  buffer[1]: offsetPad
  buffer[2]: margine risalita compressore
  buffer[3]: margine posizionamento
*/
bool config_biopsy(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len){


  printf("AGGIORNAMENTO CONFIGURAZIONE BIOPSIA \n");

  printf(" z-Home=%d\n",buffer[0]);
  printf(" z-Base=%d\n",buffer[1]);
  printf(" offsetPad=%d\n",buffer[2]);
  printf(" margineRisalita=%d\n",buffer[3]);
  printf(" marginePosizionamento=%d\n",buffer[4]);

  generalConfiguration.biopsyCfg.conf.Z_homePosition = buffer[0];
  generalConfiguration.biopsyCfg.conf.Z_basePosizionatore = buffer[1];
  generalConfiguration.biopsyCfg.conf.offsetPad = buffer[2];
  generalConfiguration.biopsyCfg.conf.margineRisalita = buffer[3];
  generalConfiguration.biopsyCfg.conf.marginePosizionamento = buffer[4];

  // L'impostazione dei margini di posizionamento richiede un aggiornamento
  // dei parametri del compressore chge si deve adeguare a nuovi vincoli
  pcb215ForceUpdateData();

  return true;
}

/* EOF */
