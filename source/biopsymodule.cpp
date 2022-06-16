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

    openCfgStandard();
    openCfgExtended();


}


void biopsyModule::defaultConfigDataStandard(void){

    // Offset di puntamento
    configStd.Z_homePosition = 193;        // (mm) distanza zero torretta - fibra di carbonio
    configStd.Z_basePosizionatore = 189;   // Distanza base metallica - fibra di carbonio

    // Gestion Movimento Pad
    configStd.offsetPad = 50;              // Offset linea di calibrazione posizione - superficie staffe metalliche
    configStd.margineRisalita = 15;        // Margine di sicurezza per impatto con il compressore in risalita
    configStd.marginePosizionamento = 5;   // Margine di sicurezza impatto con il compressore in puntamento

}

void biopsyModule::defaultConfigDataExtended(void){


    configExt.offsetPad = 59;              // Offset linea di calibrazione posizione - superficie staffe metalliche
    configExt.Z_basePosizionatore = 230;   // Margine di sicurezza per impatto con il compressore in risalita
    configExt.margineRisalita = 15;        // Margine di sicurezza impatto con il compressore in puntamento

    configExt.sh_zero_level = 390;
    configExt.sh_150_level = 680;
    configExt.sh_m150_level = 0;

}


bool biopsyModule::openCfgStandard(void)
{
    QString filename;
    QList<QString> dati;

    // Default File di configurazione
    defaultConfigDataStandard();

    filename =  QString("/resource/config/biopsy_std.cnf");

    // Se nn esiste lo crea con i default
    QFile file(filename.toAscii());
    if(!file.exists()){
        return storeConfigStandard();
    }

    // Se è corrotto lo crea di default
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return storeConfigStandard();
    }

    // Procede con la lettura del formato corrente
    while(1){

        dati = Config::getNextArrayFields(&file);
        if(dati.isEmpty()) break;

        if(dati.at(0)=="Z_HOME"){
            configStd.Z_homePosition = dati.at(1).toInt();

        }else if(dati.at(0)=="Z_BASE"){
             configStd.Z_basePosizionatore = dati.at(1).toInt();

        }else if(dati.at(0)=="OFFSET_PAD"){
            configStd.offsetPad = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_RISALITA"){
            configStd.margineRisalita = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_POSIZIONAMENTO"){
            configStd.marginePosizionamento = dati.at(1).toInt();
        }

    }

    file.close();
    return true;
}

bool biopsyModule::openCfgExtended(void)
{
    QString filename;
    QList<QString> dati;

    // Default File di configurazione
    defaultConfigDataExtended();

    filename =  QString("/resource/config/biopsy_extended.cnf");

    // Se nn esiste lo crea con i default
    QFile file(filename.toAscii());
    if(!file.exists()){
        return storeConfigExtended();
    }

    // Se è corrotto lo crea di default
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return storeConfigExtended();
    }

    // Procede con la lettura del formato corrente
    while(1){

        dati = Config::getNextArrayFields(&file);
        if(dati.isEmpty()) break;

        if(dati.at(0)=="Z_BASE"){
             configExt.Z_basePosizionatore = dati.at(1).toInt();

        }else if(dati.at(0)=="OFFSET_PAD"){
            configExt.offsetPad = dati.at(1).toInt();

        }else  if(dati.at(0)=="MARGINE_RISALITA"){
            configExt.margineRisalita = dati.at(1).toInt();

        }else  if(dati.at(0)=="SH_ZERO"){
            configExt.sh_zero_level = dati.at(1).toInt();

        }else  if(dati.at(0)=="SH_150"){
            configExt.sh_150_level = dati.at(1).toInt();

        }else  if(dati.at(0)=="SH_M150"){
            configExt.sh_m150_level = dati.at(1).toInt();

        }
    }

    file.close();
    return true;
}


bool biopsyModule::storeConfigStandard(void)
{
    QString filename;

    filename =  QString("/resource/config/biopsy_standard.cnf");
    QFile file(filename.toAscii());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return FALSE;

    file.write( QString("<Z_HOME, %1>\n").arg((int) configStd.Z_homePosition).toAscii());
    file.write( QString("<Z_BASE, %1>\n").arg((int) configStd.Z_basePosizionatore).toAscii());

    file.write( QString("<OFFSET_PAD,%1>\n").arg((int) configStd.offsetPad).toAscii());
    file.write( QString("<MARGINE_RISALITA,%1>\n").arg((int) configStd.margineRisalita).toAscii());
    file.write( QString("<MARGINE_POSIZIONAMENTO,%1>\n").arg((int) configStd.marginePosizionamento).toAscii());



    file.flush();
    file.close();

    return true;
}


bool biopsyModule::storeConfigExtended(void)
{
    QString filename;

    filename =  QString("/resource/config/biopsy_extended.cnf");
    QFile file(filename.toAscii());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return FALSE;

    file.write( QString("<Z_BASE, %1>\n").arg((int) configExt.Z_basePosizionatore).toAscii());
    file.write( QString("<OFFSET_PAD,%1>\n").arg((int) configExt.offsetPad).toAscii());
    file.write( QString("<MARGINE_RISALITA,%1>\n").arg((int) configExt.margineRisalita).toAscii());

    file.write( QString("<SH_ZERO, %1>\n").arg((int) configExt.sh_zero_level).toAscii());
    file.write( QString("<SH_150,%1>\n").arg((int) configExt.sh_150_level).toAscii());
    file.write( QString("<SH_M150,%1>\n").arg((int) configExt.sh_m150_level).toAscii());


    file.flush();
    file.close();

    return true;
}

bool biopsyModule::updateConfig(void)
{
    // Invia la configurazione al driver per aggiornarlo in diretta
    return pConfig->sendMccConfigCommand(CONFIG_BIOPSY);

}
