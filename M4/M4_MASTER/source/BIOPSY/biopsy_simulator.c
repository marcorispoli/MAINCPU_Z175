#define _BIOPSY_SIMULATOR_C
#include "dbt_m4.h" 

#ifdef __BIOPSY_SIMULATOR


#define BYM_REVISION 1
#define BYM_CHKL     0xAA
#define BYM_CHKH     0xBB

#define _SIM_RESET          1
#define _SIM_MOVE_XYZ       2
#define _SIM_MOVE_STPUP     3
#define _SIM_MOVE_STPDWN    4


// Registri Simulatore
static unsigned char sim_COMMANDS;  // Codice comando in corso
static bool sim_movimento;          // Comando in corso
static bool sim_connected;          // Torretta inserita / disinserita
static bool sim_sblocco;            // Simulazione stato del pulsante di sblocco
static int  sim_timer_bottoni;      // Tempo di attivazione bottoni

// Registri Torretta
static unsigned char bym_StatusH;   // Status register
static unsigned char bym_StatusL;

static unsigned int  bym_X;       // Posizione corrente
static unsigned int  bym_Y;
static unsigned int  bym_Z;
static unsigned int  bym_SH;

static unsigned int  bym_TGX;      // Target corrente
static unsigned int  bym_TGY;
static unsigned int  bym_TGZ;

static int           bym_Needle;    // Analogico Needle

void sim_serialCommand(unsigned char* rxdata, unsigned char* txdata){

    if(rxdata==0) return ;
    if(txdata==0) return ;

    if((sim_movimento)||(!sim_connected)){
        _time_delay(50);
        txdata[0]=0;
        txdata[1]=0;
        txdata[2]=0;
        txdata[3]=0;
        return ;
    }

    // COMANDO GET STATUS
    if((rxdata[0]==0x8D) && (rxdata[1]==0)){
        txdata[0]=0x8D;
        txdata[1]=bym_StatusL;
        txdata[2]=bym_StatusH;
        if(rxdata[2]==1){
            // Reset bit di stato
            bym_StatusL &=0x3F;
            bym_StatusH &=0x7F;
        }
    }else if((rxdata[0]==0x8D) && (rxdata[1]==3)){
        txdata[0]=0x8D; // GET_REVISION
        txdata[1]=BYM_REVISION;
        txdata[2]=0;
    }else if((rxdata[0]==0x8D) && (rxdata[1]==4)){
        txdata[0]=0x8D; // GET_CHECKSUM
        txdata[1]=BYM_CHKL;
        txdata[2]=BYM_CHKH;
    }else if((rxdata[0]==0x8D) && (rxdata[1]==5)){
        txdata[0]=0x8D; // GET_NEEDLE
        txdata[1]=(unsigned char) (bym_Needle & 0xFF);
        txdata[2]=(unsigned char) ((bym_Needle >> 8) & 0xFF);
    }else if((rxdata[0]==0x8D) && (rxdata[1]==6)){
        txdata[0]=0x8D; // RESET BYM
        txdata[1]=bym_StatusL;
        txdata[2]=bym_StatusH;
    }else if((rxdata[0]==0x8E) && (rxdata[1]==0)){
        txdata[0]=0x8E; // GET_X
        txdata[1]=(unsigned char) (bym_X & 0xFF);
        txdata[2]=(unsigned char) ((bym_X >> 8) & 0xFF);
    }else if((rxdata[0]==0x8E) && (rxdata[1]==0)){
        txdata[0]=0x8E; // GET_X
        txdata[1]=(unsigned char) (bym_X & 0xFF);
        txdata[2]=(unsigned char) ((bym_X >> 8) & 0xFF);
    }else if((rxdata[0]==0x8E) && (rxdata[1]==1)){
        txdata[0]=0x8E; // GET_TGX
        txdata[1]=(unsigned char) (bym_TGX & 0xFF);
        txdata[2]=(unsigned char) ((bym_TGX >> 8) & 0xFF);
    }else if(rxdata[0]==0xCE) {
        txdata[0]=0xCE; // SET_TGX
        bym_TGX = rxdata[1] + 256 * rxdata[2];
        txdata[1]=(unsigned char) (bym_TGX & 0xFF);
        txdata[2]=(unsigned char) ((bym_TGX >> 8) & 0xFF);
    }else if((rxdata[0]==0x8F) && (rxdata[1]==0)){
        txdata[0]=0x8F; // GET_Y
        txdata[1]=(unsigned char) (bym_Y & 0xFF);
        txdata[2]=(unsigned char) ((bym_Y >> 8) & 0xFF);
    }else if((rxdata[0]==0x8F) && (rxdata[1]==1)){
        txdata[0]=0x8F; // GET_TGY
        txdata[1]=(unsigned char) (bym_TGY & 0xFF);
        txdata[2]=(unsigned char) ((bym_TGY >> 8) & 0xFF);
    }else if(rxdata[0]==0xCF) {
        txdata[0]=0xCF; // SET_TGY
        bym_TGY = rxdata[1] + 256 * rxdata[2];
        txdata[1]=(unsigned char) (bym_TGY & 0xFF);
        txdata[2]=(unsigned char) ((bym_TGY >> 8) & 0xFF);
    }else if((rxdata[0]==0x90) && (rxdata[1]==0)){
        txdata[0]=0x90; // GET_Z
        txdata[1]=(unsigned char) (bym_Z & 0xFF);
        txdata[2]=(unsigned char) ((bym_Z >> 8) & 0xFF);
    }else if((rxdata[0]==0x90) && (rxdata[1]==1)){
        txdata[0]=0x90; // GET_TGZ
        txdata[1]=(unsigned char) (bym_TGZ & 0xFF);
        txdata[2]=(unsigned char) ((bym_TGZ >> 8) & 0xFF);
    }else if((rxdata[0]==0x90) && (rxdata[1]==2)){
        txdata[0]=0x90; // GET_SH
        txdata[1]=(unsigned char) (bym_SH & 0xFF);
        txdata[2]=(unsigned char) ((bym_SH >> 8) & 0xFF);
    }else if(rxdata[0]==0xD0) {
        txdata[0]=0xD0; // SET_TGZ
        bym_TGZ = rxdata[1] + 256 * rxdata[2];
        txdata[1]=(unsigned char) (bym_TGZ & 0xFF);
        txdata[2]=(unsigned char) ((bym_TGZ >> 8) & 0xFF);
    }else if((rxdata[0]==0x10) && (rxdata[1]==0)) {
        txdata[0]=0x10; // MOVE_XYZ()
        bym_StatusL = 0x1;  // MOTOR ON STATUS
        sim_COMMANDS = _SIM_MOVE_XYZ;
        sim_movimento = true;

        txdata[1]=bym_StatusL;
        txdata[2]=bym_StatusH;
    }else if((rxdata[0]==0x10) && (rxdata[1]==2)) {
        txdata[0]=0x10; // MOVE_STPDWN()
        bym_StatusL = 0x1;  // MOTOR ON STATUS
        sim_COMMANDS = _SIM_MOVE_STPDWN;
        sim_movimento = true;

        txdata[1]=bym_StatusL;
        txdata[2]=bym_StatusH;
    }

    // Fine comando in maniera corretta
    txdata[3] = txdata[0] ^ txdata[1] ^ txdata[2];
    return ;
}

void sim_BymReset(void){

    bym_StatusL=0x42; // HOME, COMMAND COMPLETES
    bym_StatusH=0x80; // RESET BIT

    bym_X = 0;
    bym_Y = 0;
    bym_Z = 0;
    bym_SH = 0;

}

void sim_DisconnectedStatus(void){
    while(!sim_connected) _time_delay(1000);
    sim_sblocco = false;
    sim_movimento = false;
    sim_timer_bottoni = 0;

    sim_BymReset();
    return;
}

void sim_ActivationStatus(void){
    sim_movimento = true;
    _time_delay((absI(bym_TGX - bym_X) + absI(bym_TGY - bym_Y) + absI(bym_TGZ - bym_Z))/2);
    bym_StatusL = 0x40; // Completed flag
    bym_X = bym_TGX;
    bym_Y = bym_TGY;
    bym_Z = bym_TGZ;

    if(((bym_X==0)||(bym_X==2580)||(bym_X==1290)) && (bym_Y==0) && (bym_Z==0)) bym_StatusL |= 04; // Home Flag
    else bym_StatusL |= 02; // Target flag
    sim_movimento = false;
    return;
}



void BIOPSY_simdriver(uint32_t taskRegisters)
{
    // Init driver
    sim_connected = false;
    sim_movimento = false;
    sim_COMMANDS = 0;


    while(1){
        if(!sim_connected) sim_DisconnectedStatus();
        _time_delay(100);


        switch(sim_COMMANDS){
        case _SIM_RESET:
            _time_delay(500);
            sim_BymReset();
            break;

        case _SIM_MOVE_XYZ:
            sim_ActivationStatus();
            break;

        case _SIM_MOVE_STPUP:
            if(bym_TGZ<11) bym_TGZ=0;
            else bym_TGZ -=10;
            sim_ActivationStatus();
            break;

        case _SIM_MOVE_STPDWN:
            if(bym_TGZ > 1300) bym_TGZ=1300;
            else bym_TGZ +=10;
            sim_ActivationStatus();
            break;

        }

    }
}




/*__________________________________________________________________________
 * API DI INTERFACCIA PER LA GUI
 *__________________________________________________________________________*/
void SimSetPush(bool stat){
    sim_sblocco = stat;
    printf("SIMULATORE BYM:PULSANTE SBLOCCO >%d\n", sim_sblocco);
}


void SimSetAdapter(int id){

    if(id == _BP_ADAPTER_OPEN) bym_Needle = (_ADAPTER_OPEN_LEVEL_L + _ADAPTER_OPEN_LEVEL_H)/2;
    else if(id == _BP_ADAPTER_SHORT) bym_Needle = (_ADAPTER_SHORT_LEVEL_L + _ADAPTER_SHORT_LEVEL_H)/2;
    else if(id == _BP_ADAPTER_A) bym_Needle = (_ADAPTER_A_LEVEL_L + _ADAPTER_A_LEVEL_H)/2;
    else if(id == _BP_ADAPTER_B) bym_Needle = (_ADAPTER_B_LEVEL_L + _ADAPTER_B_LEVEL_H)/2;

    printf("SIMULATORE BYM:NEEDLE >%d\n", bym_Needle);
}

void SimConnessione(bool stat){
    printf("SIMULATORE BYM:CONNESSIONE >%d\n", stat);
    sim_connected = stat;

}
void SimSetLatX(unsigned char lat){

    bym_StatusH &= 0xFC;
    bym_StatusH |= (lat & 0x3);

    if((bym_StatusH &0x3) == _BP_ASSEX_POSITION_LEFT)   printf("SIMULATORE BYM:latX > LEFT\n");
    else if((bym_StatusH &0x3) == _BP_ASSEX_POSITION_CENTER)   printf("SIMULATORE BYM:latX > CENTER\n");
    else if((bym_StatusH &0x3) == _BP_ASSEX_POSITION_RIGHT)   printf("SIMULATORE BYM:latX > RIGHT\n");
    else printf("SIMULATORE BYM:latX > ND\n");
}

void SimSetSH(int sh){
    bym_SH = sh;
    printf("SIMULATORE BYM:SH >%d\n", bym_SH);
}

void SimSetXYZ(unsigned short X, unsigned short Y, unsigned short Z){
    bym_X = bym_TGX = X;
    bym_Y = bym_TGY = Y;
    bym_Z = bym_TGZ = Z;
    printf("SIMULATORE BYM:X,Y,Z >[%d,%d,%d]\n", bym_X, bym_Y, bym_Z);
}

#endif



