#define _BIOPSY_STARTUP_C
#include "dbt_m4.h"

static void biopsyUpdateConfig(void);

// Alla prima connessione determina quale BYM è connesso e
// conseguentemente attiva la thread relativa fino a spegnimento
void BIOPSY_driver(uint32_t taskRegisters)
{

    // Definisce se e quale modello è stato identificato
    generalConfiguration.biopsyCfg.biopsyModel = _BYM_NOT_DEFINED;

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

#ifdef __BIOPSY_SIMULATOR
    // Partenza processo gestione biopsia (opzionale)
    _task_create(0,_IDTASK(BIOPSYM),(uint32_t) NULL);

#endif

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


    // Aggiorna la configurazione in funzione del dispositivo identificato
    if(isStandard) generalConfiguration.biopsyCfg.biopsyModel = _BYM_STANDARD_MODEL;
    else generalConfiguration.biopsyCfg.biopsyModel = _BYM_EXTENDED_MODEL;
    biopsyUpdateConfig();

    // Attiva i loop relativi alle due torrette
    if(isStandard) biopsyStandardLoop();
    else biopsyExtendedLoop();
}

void biopsyUpdateConfig(void){
    // Aggiorna la configurazione in funzione del dispositivo identificato
    if(generalConfiguration.biopsyCfg.biopsyModel == _BYM_STANDARD_MODEL){
        printf("AGGIORNAMENTO CONFIGURAZIONE BIOPSIA STANDARD  \n");

        printf(" z-Home=%d\n",generalConfiguration.biopsyCfg.standardConf.conf.Z_homePosition);
        printf(" z-Base=%d\n",generalConfiguration.biopsyCfg.standardConf.conf.Z_basePosizionatore);
        printf(" offsetPad=%d\n",generalConfiguration.biopsyCfg.standardConf.conf.offsetPad);
        printf(" margineRisalita=%d\n",generalConfiguration.biopsyCfg.standardConf.conf.margineRisalita);
        printf(" marginePosizionamento=%d\n",generalConfiguration.biopsyCfg.standardConf.conf.marginePosizionamento);
        generalConfiguration.biopsyCfg.Z_basePosizionatore = generalConfiguration.biopsyCfg.standardConf.conf.Z_basePosizionatore;
        generalConfiguration.biopsyCfg.offsetPad = generalConfiguration.biopsyCfg.standardConf.conf.offsetPad;
        generalConfiguration.biopsyCfg.margineRisalita = generalConfiguration.biopsyCfg.standardConf.conf.margineRisalita;
    }else{
        printf("AGGIORNAMENTO CONFIGURAZIONE BIOPSIA ESTESA  \n");
        printf(" z-Base Estesa=%d\n",generalConfiguration.biopsyCfg.extendedConf.conf.Z_basePosizionatore);
        printf(" offsetPad Estesa=%d\n",generalConfiguration.biopsyCfg.extendedConf.conf.offsetPad);
        printf(" margineRisalita Estesa=%d\n",generalConfiguration.biopsyCfg.extendedConf.conf.margineRisalita);
        generalConfiguration.biopsyCfg.Z_basePosizionatore = generalConfiguration.biopsyCfg.extendedConf.conf.Z_basePosizionatore;
        generalConfiguration.biopsyCfg.offsetPad = generalConfiguration.biopsyCfg.extendedConf.conf.offsetPad;
        generalConfiguration.biopsyCfg.margineRisalita = generalConfiguration.biopsyCfg.extendedConf.conf.margineRisalita;
    }

    // Aggiorna il driver compressore sulla nuova posizione limite
    pcb215ForceUpdateData();

}

/*
  Funzione configuratrice biopsia
  buffer[0]: offsetFibra
  buffer[1]: offsetPad
  buffer[2]: margine risalita compressore
  buffer[3]: margine posizionamento
*/
bool config_biopsy(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len){

  generalConfiguration.biopsyCfg.standardConf.conf.Z_homePosition = buffer[0];
  generalConfiguration.biopsyCfg.standardConf.conf.Z_basePosizionatore = buffer[1];
  generalConfiguration.biopsyCfg.standardConf.conf.offsetPad = buffer[2];
  generalConfiguration.biopsyCfg.standardConf.conf.margineRisalita = buffer[3];
  generalConfiguration.biopsyCfg.standardConf.conf.marginePosizionamento = buffer[4];

  generalConfiguration.biopsyCfg.extendedConf.conf.Z_basePosizionatore = buffer[5];
  generalConfiguration.biopsyCfg.extendedConf.conf.offsetPad = buffer[6];
  generalConfiguration.biopsyCfg.extendedConf.conf.margineRisalita = buffer[7];

  if(generalConfiguration.biopsyCfg.biopsyModel == _BYM_NOT_DEFINED) return true;
  biopsyUpdateConfig();
  return true;
}

/* EOF */
