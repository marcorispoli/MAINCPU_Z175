/*

Aut: M. Rispoli
Data di Creazione: 30/09/2014

Data Ultima Modifica:22/09/2014
*/
#ifndef _GUI
#define _GUI

#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _GUI_C
  #define ext 
  #define extrd 

void  manageMccLoader(void);
void  digitalManageMccOperativo(void);
void  manageMccConfig(void);


void mcc_canopen(void);
void mcc_pcb244(void);
void mcc_raggi_std(void);
void mcc_raggi_tomo(void);
void readMasData(void);
void mcc_pcb215_get_trolley(void);
void readFilData(void);
void mccSetColli(unsigned char id, unsigned char mcccode);
void mccSetFuoco(unsigned char id, unsigned char mcccode);

void mccSetRotationToolConfig();

void mccGetGonio(unsigned char id, unsigned char mcccode);
void mccResetGonio(unsigned char id, unsigned char mcccode);

void mccSetDevCfg(void);

void mcc_cmd_trx(void);
void mcc_cmd_arm(void);

void mcc_set_mirror(unsigned char id, unsigned char mcccode,unsigned char cmd);
void mcc_set_lamp(unsigned char id, unsigned char mcccode);
void mcc_manual_rx_shot(void);
void mcc_test(void);
void mcc_exp_aec_std(void);
void mcc_raggi_aec_std(void);
void mcc_raggi_aec_tomo(void);
void mcc_raggi_ae(void);

void mcc_set_starter(void);

void mcc_get_io(void);
void mcc_get_revision(void);
void mcc_service_commands(int id, int subcmd, unsigned char* data,int len);
void mcc_loader_commands(int id, int subcmd, unsigned char* data,int len);
void mcc_pcb215_calibration(void);
void mccSetFiltro(void);
void mccBiopsyCmd(void);
void mccBiopsySimulator(void);
void mccBiopsyConfig(void);


void mccSetCalibFiltro(void);
void mcc_raggi_bassa_energia(bool);
void mcc_raggi_alta_energia(void);
void mcc_calib_zero(void);
void mcc_parking_mode(void);
void mcc_unparking_mode(void);



#else
  #define ext extern
  #define extrd extern const
#endif



/////////////////////////////////////////////////////////////////////////
  
  
// API ////////////////////////////////////////////////////////////////// 

ext void gui_interface_task(uint32_t initial_data); // Tash di gestione comandi da GUI

// MACRO ////////////////////////////////////////////////////////////////// 



#endif
