#define POTTER_C
#include "application.h"
#include "appinclude.h"
#include "globvar.h"

void Potter::activateConnections(void){
    connect(pConsole,SIGNAL(mccGuiNotify(unsigned char,unsigned char,QByteArray)),this,SLOT(guiNotify(unsigned char,unsigned char,QByteArray)),Qt::UniqueConnection);

}

Potter::Potter(QObject *parent) :
    QObject(parent)
{
    potterId = POTTER_UNDEFINED;
    potterFactor=255;
    potterValidFactor = FALSE;

}

void Potter::guiNotify(unsigned char id, unsigned char mcccode, QByteArray data)
{
    unsigned char i,ii;
    unsigned char potter;
    static unsigned char flag_potter = POTTER_UNDEFINED;
    static unsigned char flag_potterFactor = 0;

#ifdef __APPLICATION_DISABLE_POTTER
return;
#endif

    if(mcccode==MCC_POTTER_ID)
    {
        potterId = data.at(0);
        potterValidFactor = FALSE;

        // Se la Biopsia � presente allora il codice accessorio � impostato dalla biopsia
        if((data.at(0)==POTTER_UNDEFINED)&&(!pBiopsy->connected)) {
            ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) data.at(0),0);
            ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("POTTER","ACCESSORIO NON DEFINITO", 0, QApplication::UnicodeUTF8)),0);
            potter = POTTER_UNDEFINED;
        }else if(data.at(0)==POTTER_2D){
            ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) data.at(0),0);
            ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("POTTER","POTTER_2D", 0, QApplication::UnicodeUTF8)),0);
            potter = POTTER_2D;
        } else if(data.at(0)==POTTER_TOMO){
            ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) data.at(0),0);
            ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("POTTER","POTTER_3D", 0, QApplication::UnicodeUTF8)),0);
            potter = POTTER_TOMO;
        }
        else if(data.at(0)==POTTER_MAGNIFIER)
        {
            ApplicationDatabase.setData(_DB_ACCESSORIO, (unsigned char) data.at(0),0);
            // Verifica se l'ingranditore è definito
            if(data.at(1)==255){
                // Non calcolato. Non produce errore e imposta 1x
                ApplicationDatabase.setData(_DB_MAG_FACTOR,(int) 10,0);
                ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("POTTER","FATTORE_INGRANDIMENTO", 0, QApplication::UnicodeUTF8)).arg(1).arg(0),0);
                PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_POTTER, 0, FALSE);
            }else if((data.at(1)>7)||(pCompressore->config.sbalzoIngranditore[data.at(1)] == 0))
            {
                potterFactor = 255;
                ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("POTTER","INGRANDITORE_ND", 0, QApplication::UnicodeUTF8)),0);

                // Attiva allarme che non si resetta
                if(data.at(1)>7) PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_POTTER, ERROR_MAG_READ, FALSE);
                else PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_POTTER, ERROR_MAG_CONF, FALSE);
            }else
            {
                // Nel caso di cambiamento di stato alloora aggiorna il database
                if((potterFactor!=data.at(1))||(potterValidFactor==FALSE))
                {
                    potterFactor = data.at(1);
                    i = pCompressore->config.fattoreIngranditore[data.at(1)]/10;
                    ii = pCompressore->config.fattoreIngranditore[data.at(1)]%10;

                    ApplicationDatabase.setData(_DB_MAG_FACTOR,(int) pCompressore->config.fattoreIngranditore[data.at(1)],0);
                    ApplicationDatabase.setData(_DB_ACCESSORY_NAME,QString(QApplication::translate("POTTER","FATTORE_INGRANDIMENTO", 0, QApplication::UnicodeUTF8)).arg(i).arg(ii),0);
                }
                potterValidFactor = TRUE;
                PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_POTTER, 0, FALSE);
            }


        }

        if(data.size()>3){
            cassette = data[2];
            cassetteExposed = data[3];
        }

        // Notifies the AWS with Async message
        if((flag_potter!=potter)||(potterFactor!=flag_potterFactor)){
            pToConsole->setConfigChanged(pToConsole->ACCESSORIO);
            flag_potter = potter;
            flag_potterFactor = potterFactor;
        }

    }

}

// Impostazione campo dell'esposimetro
// Valori ammessi: 0,1,2
bool Potter::setDetectorField(unsigned char val){
    unsigned char data[1];
    data[0] = val;
    return pConsole->pGuiMcc->sendFrame(MCC_244_A_DETECTOR_FIELD,1,data, sizeof(data));
}
