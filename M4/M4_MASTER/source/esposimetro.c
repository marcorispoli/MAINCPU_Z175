#define _ESPOSIMETRO_C
#include "dbt_m4.h" 

// Restituisce il valore PLOG letto dall'esposimetro durante il pre-impulso
int getPlog(unsigned short rad){
    if(rad>sizeof(rToPlog)) return 1024;
    return (int) rToPlog[rad];
}

// ______________________________________________________________________________________________

/* EOF */
   


