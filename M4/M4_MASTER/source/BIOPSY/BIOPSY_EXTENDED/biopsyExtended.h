/*

Aut: M. Rispoli
Data di Creazione: 19/06/2014
*/
#ifndef _BIOPSY_EXTENDED_H
#define _BIOPSY_EXTENDED_H

#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _BIOPSY_EXTENDED_C
  #define ext 
  #define extrd 
#else
  #define ext extern
  #define extrd extern const
#endif

#define  _ADAPTER_A_LEVEL_H         1100
#define  _ADAPTER_A_LEVEL_L         950

#define  _ADAPTER_B_LEVEL_H         750
#define  _ADAPTER_B_LEVEL_L         550

#define  _ADAPTER_C_LEVEL_H         400
#define  _ADAPTER_C_LEVEL_L         200

#define  _ADAPTER_SHORT_LEVEL_H     100
#define  _ADAPTER_SHORT_LEVEL_L     0

#define  _ADAPTER_OPEN_LEVEL_H      2100
#define  _ADAPTER_OPEN_LEVEL_L      1950

  // API //////////////////////////////////////////////////////////////////
  ext bool  biopsyExtendedMoveXYZ(unsigned short X, unsigned short Y, unsigned short Z);
  ext bool  biopsyExtendedIsPresent(void);
  ext void  biopsyExtendedLoop(void);

  /*
  ext bool  biopsyStepIncZ(void);
  ext bool  biopsyStepDecZ(void);
  ext bool  biopsyStepIncX(void);
  ext bool  biopsyStepDecX(void);
  ext bool  biopsyStepIncY(void);
  ext bool  biopsyStepDecY(void);
*/

#endif

