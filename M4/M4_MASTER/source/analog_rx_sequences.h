/*

Aut: M. Rispoli
Data di Creazione: 22/09/2014

Data Ultima Modifica:22/09/2014
*/
#ifndef _ANALOG_RX_SEQUENCES_H
#define _ANALOG_RX_SEQUENCES_H

#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _ANALOG_RX_SEQUENCES_C
  #define ext 
  #define extrd 
#else
  #define ext extern
  #define extrd extern const
#endif


// CODICI SEQUENZE
#define EXPOSIMETER_CALIBRATION_PULSE   1
#define EXPOSIMETER_CALIBRATION_PROFILE 2
#define TUBE_CALIBRATION_PROFILE        3
#define MANUAL_MODE_EXPOSURE            4


void tune_esposimeter_rx(_RxStdSeq_Str* Param); // Funzione di esposizione per calibrazione hw esposimetro
int AnalogManualModeExposure(void);

#endif
