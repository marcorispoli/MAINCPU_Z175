#include "application.h"
#include "appinclude.h"
#include "globvar.h"

biopsyModule::biopsyModule()
{
    connected = false;
    model = BYM_UNDETERMINED_DEVICE;

    checksum_h=0;
    checksum_l=0;
    revisione=0;

    openCfg();


}


void biopsyModule::defaultConfigData(void){

    // Offset di puntamento
    config.Z_homePosition = 193;            // (mm) distanza zero torretta - fibra di carbonio
    config.Z_basePosizionatore = 189;       // Distanza base metallica - fibra di carbonio

    // Gestion Movimento Pad
    config.offsetPad = 59;              // Offset linea di calibrazione posizione - superficie staffe metalliche
    config.margineRisalita = 15;        // Margine di sicurezza per impatto con il compressore in risalita
    config.marginePosizionamento = 5;   // Margine di sicurezza impatto con il compressore in puntamento

}


bool biopsyModule::openCfg(void)
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
bool biopsyModule::storeConfig(void)
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

bool biopsyModule::updateConfig(void)
{
    // Invia la configurazione al driver per aggiornarlo in diretta
    return pConfig->sendMccConfigCommand(CONFIG_BIOPSY);

}
