#include "appinclude.h"
#include "globvar.h"


// Messaggi provenienti dallelibrerie Qt
void  qtOutput(QtMsgType type, const char *msg);
void  qtOutput(QtMsgType type, const char *msg)
{
    QTMSG(QString(msg));
    return;
}

//


infoClass::infoClass(QObject *parent) :
    QObject(0)
{
    qInstallMsgHandler(qtOutput);
}



