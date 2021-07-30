#include "pageOpenAnalogic.h"
#include "analog.h"
#include "ui_analog.h"
#include "../application.h"
#include "../appinclude.h"
#include "../globvar.h"

void AnalogPageOpen::xrayReleasePushButton(void){
    // In ogni caso
}

/*
 */
// FUNZIONE DI CHIAMATA ESPOSIZIONE
void AnalogPageOpen::startXraySequence(void){

    ApplicationDatabase.setData(_DB_XDMAS,(int) 0);
    ApplicationDatabase.setData(_DB_XDUDOSE,(int) 0);
    saveOptions();

    if(ApplicationDatabase.getDataI(_DB_TECH_MODE) == ANALOG_TECH_MODE_MANUAL) xrayManualSequence();
    else if(ApplicationDatabase.getDataI(_DB_TECH_MODE) == ANALOG_TECH_MODE_SEMI) xraySemiAutoSequence();
    else xrayFullAutoSequence();

}

void AnalogPageOpen::xrayErrorInCommand(unsigned char code){
    if(!code) return;

    // Spegne il simbolo raggi in corso
    ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 0, DBase::_DB_FORCE_SGN);

    // Attiva codice di errore auto ripristinante
    PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI,code,true);

    return;
}

void AnalogPageOpen::xrayManualSequence(void){
    unsigned char data[18];

    // Impostazione kV
    pGeneratore->setkV((float) ApplicationDatabase.getDataI(_DB_DKV)/10);
    pGeneratore->setmAs((float) ApplicationDatabase.getDataI(_DB_DMAS)/10);

    XspessoreSeno = ApplicationDatabase.getDataI(_DB_SPESSORE);
    XselectedkV = pGeneratore->selectedKv;
    XselectedDmAs = pGeneratore->selectedDmAs;
    XselectedIa = pGeneratore->selectedIn;
    XselectedFuoco = pGeneratore->selectedFSize;
    XselectedFiltro = pCollimatore->getFiltroStat();

    // Impostazione dati di esposizione
    unsigned char errcode = pGeneratore->validateAnalogData(ANALOG_TECH_MODE_MANUAL);
    if(errcode){
        xrayErrorInCommand(errcode);
        return;
    }

    // Comunque effettua il refresh dello starter
    pGeneratore->refreshStarter();

    // Aggiorna il filtro se necessario
    if(pCollimatore->getFiltroStat() != (Collimatore::_FilterCmd_Enum) pConfig->analogCnf.selected_filtro){
        pCollimatore->setFiltro((Collimatore::_FilterCmd_Enum) pConfig->analogCnf.selected_filtro, true);
    }

    // Update del fuoco per sicurezza
    setCurrentFuoco();

    data[0] =  (unsigned char) (pGeneratore->selectedVdac&0x00FF);
    data[1] =  (unsigned char) (pGeneratore->selectedVdac>>8);
    data[2] =  (unsigned char) (pGeneratore->selectedIdac&0x00FF);
    data[3] =  (unsigned char) (pGeneratore->selectedIdac>>8);
    data[4] =  (unsigned char) (pGeneratore->selectedmAsDac&0x00FF);
    data[5] =  (unsigned char) (pGeneratore->selectedmAsDac>>8);
    data[6] =  pGeneratore->timeoutExp;
    data[7]=0;
    if(pGeneratore->SWA) data[7]|=1;
    if(pGeneratore->SWB) data[7]|=2;

    // Gestione dello Starter:
    if(pGeneratore->starterHS) data[7]|=4;        // Alta VelocitÃ

    data[8] =  0; // Tensione Griglia da aggiungere
    data[9] =  pGeneratore->maxV;
    data[10] = pGeneratore->minV;
    data[11] = pGeneratore->maxI;
    data[12] = pGeneratore->minI;

    // Gestione dello sblocco del compressore: NON IN BIOPSIA e non in combo
    if(pBiopsy->connected) data[13] = 0;
    else if(pConfig->userCnf.enableSblocco) data[13] = 1;
    else data[13]=0;

    // Aggiungo i valori nominali inviati al driver
    data[14] = (unsigned char) ((unsigned int) (pGeneratore->selectedKv * 10) & 0x00FF);
    data[15] = (unsigned char) ((unsigned int) (pGeneratore->selectedKv * 10) >> 8);
    data[16] = (unsigned char) ((unsigned int) (pGeneratore->selectedIn * 10) & 0x00FF);
    data[17] = (unsigned char) ((unsigned int) (pGeneratore->selectedIn * 10) >> 8);

    // Prova ad inviare il comando
    if(pConsole->pGuiMcc->sendFrame(MCC_XRAY_ANALOG_MANUAL,1,data,sizeof(data))==FALSE)
    {
        xrayErrorInCommand(ERROR_MCC_COMMAND);
        return;
    }

    ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 1, DBase::_DB_FORCE_SGN);
    io->setXrayLamp(true);


}

void AnalogPageOpen::xraySemiAutoSequence(void){

    ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 1, DBase::_DB_FORCE_SGN);
}

void AnalogPageOpen::xrayFullAutoSequence(void){

    ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 1, DBase::_DB_FORCE_SGN);
}

/*
 *
    data[0]=RXOK;
    data[1]=(unsigned char) ((Param->dmAs_released)&0xFF);
    data[2]=(unsigned char) ((Param->dmAs_released>>8)&0xFF);
    data[3]=(unsigned char) ((Param->pulses_released)&0xFF);
    data[4]=(unsigned char) ((Param->pulses_released>>8)&0xFF);
    data[5]=(unsigned char) ((plog)&0xFF);
    data[6]=(unsigned char) ((plog>>8)&0xFF);
    data[7]=(unsigned char) ((rad)&0xFF);
    data[8]=(unsigned char) ((rad>>8)&0xFF);
    data[9]=(unsigned char) ((rad5)&0xFF);
    data[10]=(unsigned char) ((rad5>>8)&0xFF);
    data[11]=(unsigned char) ((rad25)&0xFF);
    data[12]=(unsigned char) ((rad25>>8)&0xFF);

    int val;
    val = (int) (KV * (float) 10);
    data[13]=(unsigned char) ((val)&0xFF);
    data[14]=(unsigned char) (((val)>>8)&0xFF);
    val = (int) (IMED * (float) 10);
    data[15]=(unsigned char) ((val)&0xFF);
    data[16]=(unsigned char) (((val)>>8)&0xFF);
    data[17]=(unsigned char) ((int)(TIME)&0xFF);
    data[18]=(unsigned char) (((int)(TIME)>>8)&0xFF);
 *
 */
void AnalogPageOpen::guiNotify(unsigned char id, unsigned char mcccode, QByteArray data)
{
    float ldmas;
    float ldose;

    switch(mcccode)
    {

    case MCC_XRAY_ANALOG_MANUAL:
        stopAttesaDati();

        ldmas = ((float) (data.at(1)+256*data.at(2)));
        ldose = pGeneratore->getDoseUg(XspessoreSeno,ldmas,XselectedkV*10,XselectedFiltro);
        ApplicationDatabase.setData(_DB_XDMAS,(int) ldmas);
        ApplicationDatabase.setData(_DB_XDUDOSE,(int) ldose);

        // Aggiornamento delle statistiche solo con generazione
        if((data.at(0)==RXOK)||(data.at(0)<LAST_ERROR_WITH_PREP)){
            pGeneratore->notifyStatisticData(pGeneratore->selectedKv, ldmas/10, true);
        }


        if(data.at(0)){
            PageAlarms::activateNewAlarm(_DB_ALLARMI_ALR_RAGGI, data.at(0),TRUE); // Self resetting
        }

        // Rilascio Pulsante raggi
        ApplicationDatabase.setData(_DB_XRAY_SYM,(unsigned char) 0, DBase::_DB_FORCE_SGN);
        io->setXrayLamp(false);
        break;


    default:
            return;
        break;
    }
}
