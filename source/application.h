#ifndef APPLICATION_H
#define APPLICATION_H


#define APPREVMAJ   1
#define APPREVMIN   4
#define BETAREV     0

// ___________________________________________________________________________________________
/*
    RELEASE 1.3.0: 26/02/2021

    - Calibrazione compressore: aggiunto controlli sul risultato della calibrazione
    - IRS: aggiunto test ciclico sulle lame del collimatore
    - IRS: aggiunto comandi per il Backup/Restore

    ___________________________________________________________________________________________
    RELEASE 1.4.0: 28/06/2021

    1) Ristrutturato files di calibrazione compressore:
       - Sostituiti i files compressore.cnf, calibpad.cnf con pad.cnf;
       - Nella fase di istallazione il nuovo software leggeraà il contenuto dei
         vecchi files e li sostituirà con il nuovo.

    2) Aggiunto il PAD PROSTHESIS che sostituisce il D75 a contatto
    3) Aggiunto compressore 9x9 per ingrandimento

    4) Modificato il formato del file di collimazione.
       Il file di confiogurazione passa a rev 3. Il file precedente viene letto
       e gli viene aggiunta la coppia di PAD nuovi.

    5) Aggiunta gestione piastrina di collimazione per Frustoli
       - Aggiunto sync SetSpecimen
       - Aggiunto Console: GetSpecimen
       - Gestione codice Piastrina di collimazione
       - Attivazione collimazione speciale se Biopsia inserita

       - debug -> piombo / frustoli
       - Manca aggiunta grafica su pagina biopsia
       - Manca comunicazione dello stato a AWS

    6)  Aggiunta collimazione Manuale:
        - Pannello Operativo
        - Pannello di service
        - Comandi IRS?

    7) Aggiunta gestione Parcheggio con rotazione Automatica o Manuale
       - Aggiunto un pannello di calibrazione del punto di parcheggio,
       - Aggiunti 6 messaggi di allarme/Warning relativi al parcheggio


*/
//////////////////////////////////////////////////////////////////////////////////////////////

#define _LOCAL_SERVICE_PORT  10006

//                              INDIRIZZI DI RETE DEL SISTEMA
#define _CONSOLE_IN_PORT        10002 // Ricezione comandi da Console
#define _CONSOLE_OUT_PORT       10003 // Invio Notifiche a console
#define _CONSOLE_ERROR_PORT     10004 // Invio messaggi Popup a Console
#define _CONSOLE_LOG_PORT       10005 // Invio messaggi di log a Console
#define _CONFIG_SLAVE_IN_PORT   10007 // POrta su GUI SLAVE di ricezione comandi da Master
#define _ECHO_PORT              10001 // Protocollo ECHO su entrambi Master/Slave
#define _AWS_OUT_PORT           10010 // Porta servizio a basso livello con il PC per Power On Off

// Definizione standard Unicode per i messaggi di protocollo esterni(da e per console)
// ATTENZIONE, EFFETTUARE UN CLEAN DEL PROGETTO SE SI CAMBIA QUESTA DEFINE
#define UNICODE_FORMAT 255 // 0xFF = true, 0 = non unicode
#define UNICODE_TYPE "UTF-16LE" // Tipo di unicode
//#define UNICODE_TYPE "UTF-8" // Tipo di unicode

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// STATO GENERALE DEL SISTEMA

#define HB(x)   (unsigned char)(((unsigned short)x)>>8)
#define LB(x)   (unsigned char)(((unsigned short)x) & 0x00FF)




/////////////////////////////////////////////////////////////////////////////////////////////
//                              GESTIONE DEI REGISTRI
/////////////////////////////////////////////////////////////////////////////////////////////
#define __REGID(x1,x2,x3,x4,x5,x6,x7)   x1
#define _REGID(x) __REGID(x)
#define __REGDEF(x1,x2,x3,x4,x5,x6,x7) x2,x3,x4,x5,x6,x7
#define _REGDEF(x) __REGDEF(x)
#define _IDREG(x1)  x1,0,0,0,0,0,0      // Crea un set di valori tipo registro

// Macro per testatre lo stato di un flag
#define __TEST_BIT(x,bit,ptr)     (ptr[x].val.bytes.lb & (1<<bit)) ? TRUE : FALSE
#define _TEST_BIT(x) __TEST_BIT(x)

// Macro per definire i campi di un registro
#define _BNK01 0       // Registro in banchi 0 / 1
#define _BNK23 1       // Registro in banchi 2 / 3

#define _RW     0       // Read write register
#define _RD     1       // Read only register

#define _8BIT   0       // Registro a 8 bit
#define _16BIT  1       // Registro a 16 bit

#define _VL     1        // Imposta il registro come Volatile
#define _NVL    0        // Imposta il registro come Non Volatile





#include "tcpipserver.h"
#include "tcpipclient.h"
#include "echodisplay.h"
#include <QWSServer>
#include <QObject>
#include <QFile>

extern "C" // Necessario per utilizzare simboli da libreria dinamica
{
#include <libmcc.h>
}
#include "../shared_a5_m4/shared.h"
#include "Configuration.h"
#include "resource.h"
#include "lib/dbase.h"
#include "lib/mcccom.h"
#include "lib/gwindow.h"
#include "lib/insertcalc.h"
#include "lib/msgbox.h"
#include "serverdebug.h"
#include "sysio.h"
#include "protoconsole.h"
#include "config.h"

#endif // APPLICATION_H
