/*

Aut: M. Rispoli
Data di Creazione: 19/06/2014
*/
#ifndef _BIOPSY_EXTENDED_DRIVER_H
#define _BIOPSY_EXTENDED_DRIVER_H

#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _BIOPSY_EXTENDED_DRIVER_C
  #define ext 
  #define extrd 
#else
  #define ext extern
  #define extrd extern const
#endif


// Funzioni di servizio
ext bool BiopsyExtendedIsPresent(void);
ext bool BiopsyDriverGetStat(unsigned char* statL, unsigned char* statH, bool reset);
ext bool BiopsyDriverGetRevision(unsigned char* val);
ext bool BiopsyDriverGetChecksum(unsigned char* chkl, unsigned char* chkh);
ext bool BiopsyDriverReset(void);


// Needle
ext bool BiopsyDriverGetNeedle(unsigned short* val);

// Position
ext bool BiopsyDriverGetX(unsigned short* val);
ext bool BiopsyDriverGetY(unsigned short* val);
ext bool BiopsyDriverGetZ(unsigned short* val);
ext bool BiopsyDriverGetSH(unsigned short* val);

// Targets
ext bool BiopsyDriverGetTGX(unsigned short* val);
ext bool BiopsyDriverGetTGY(unsigned short* val);
ext bool BiopsyDriverGetTGZ(unsigned short* val);
ext bool BiopsyDriverSetTGX(unsigned short val);
ext bool BiopsyDriverSetTGY(unsigned short val);
ext bool BiopsyDriverSetTGZ(unsigned short val);

// Activations
ext bool BiopsyDriverMoveXYZ(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveHOME(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveDecZ(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveIncZ(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveDecX(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveIncX(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveDecY(unsigned char* statusL, unsigned char* statusH);
ext bool BiopsyDriverMoveIncY(unsigned char* statusL, unsigned char* statusH);

// Registers
ext bool BiopsyDriverSetStepVal(unsigned char val, unsigned char* stepval);

// Tools
ext void BiopsyDriverSetBuzzer(void);

#endif
