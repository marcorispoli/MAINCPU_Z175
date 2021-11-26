#ifndef _BIOPSY_STARTUP_H
#define _BIOPSY_STARTUP_H

#ifdef ext
#undef ext
#undef extrd
#endif
#ifdef _BIOPSY_STARTUP_C
  #define ext
  #define extrd
#else
  #define ext extern
  #define extrd extern const
#endif

typedef struct
{
  // OBBLIGATORIO
  unsigned char fault:1;        // Fault condition
  unsigned char updconf:1;      // Richiesta aggiornamento configurazione
  unsigned char ready:1;        // Driver Operativo
  unsigned char freeze:1;       // Driver Momentaneamente bloccato

  // SPECIFICO PER DRIVER
  unsigned char spare:4;
  //--------------------------

  // OBBLIGATORIO
  unsigned char maj_code;        // Codice Firmware letto da periferica
  unsigned char min_code;        // Codice Firmware letto da periferica
  unsigned char error;           // Fault code
}_BIOPSY_Stat_Str;



ext void    BIOPSY_driver(uint32_t initial_data);  // Driver BIOPSY
ext bool    config_biopsy(bool setmem, unsigned char blocco, unsigned char* buffer, unsigned char len);
#endif
