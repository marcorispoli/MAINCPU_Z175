/*

Aut: M. Rispoli
Data di Creazione: 19/06/2014
*/
#ifndef _BIOPSY_STANDARD_H
#define _BIOPSY_STANDARD_H

#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _BIOPSY_STANDARD_C
  #define ext 
  #define extrd 
#else
  #define ext extern
  #define extrd extern const
#endif


  // API ////////////////////////////////////////////////////////////////// 
  ext bool  biopsyStandardIsPresent(void);
  ext void  biopsyStandardLoop(void);


  ext bool  biopsyStandardSetXYZ(unsigned short X, bool XGO, unsigned short Y, bool YGO, unsigned short Z, bool ZGO);
  ext bool  biopsyStandardSetZLimit(unsigned char zlimit);
  ext bool  biopsyStandardSetZLesione(unsigned char zlesione);
  ext bool  biopsyStandardSetLago(unsigned char lago);
  ext void  biopsyStandardReset(void);
  ext void  biopsyStandardStepZ(unsigned char dir, unsigned char val);

  
#endif
