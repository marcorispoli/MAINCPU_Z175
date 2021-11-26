
QT += network



TRANSLATIONS += languages/traduzione_ita.ts \
                languages/traduzione_spa.ts \
                languages/traduzione_eng.ts \
                languages/traduzione_ger.ts \
                languages/traduzione_rus.ts \
                languages/traduzione_tur.ts \
                languages/traduzione_por.ts \
                languages/traduzione_pol.ts \
                languages/traduzione_chn.ts \
                languages/traduzione_fra.ts \
                languages/traduzione_ltu.ts \



HEADERS += \
    source/tcpipserver.h \
    source/tcpipclient.h \
    source/echodisplay.h \
    source/resource.h \
    lib/gwindow.h \
    source/mainpage.h \
    lib/dbase.h \
    source/pagelanguages.h \
    source/pageacr.h \
    source/pagealarms.h \
    source/application.h \
    source/console.h \
    source/generatore.h \
    lib/mcccom.h \
    source/sysio.h \
    source/prototoconsole.h \
    source/protoconsole.h \
    source/serverdebug.h \
    source/appinclude.h \
    source/globvar.h \
    lib/insertcalc.h \
    source/Configuration.h \
    source/config.h \
    source/collimatore.h \
    source/compressor.h \
    source/potter.h \
    lib/msgbox.h \
    source/loader.h \
    shared_a5_m4/shared.h \
    source/Service/servicepanelmenu.h \
    source/Service/Calib/calibmenu.h \
    source/startuppage.h \
    source/Service/Calib/calibcompressorposition.h \
    lib/numericpad.h \
    source/Service/Calib/calibcompressorforce.h \
    source/ImagePage.h \
    source/openStudyPage.h \
    source/projectionPage.h \
    source/Service/Calib/calibzerosetting.h \
    source/Service/Setup/system.h \
    source/Service/Calib/calibfilter.h \
    source/Service/Calib/calibpower.h \
    source/Service/Calib/calibpot.h \
    source/Service/Calib/calibstarter.h \
    source/Service/Tools/toolsmenu.h \
    source/Service/Tools/tiltingtool.h \
    source/Service/Tools/armtool.h \
    source/Service/Tools/lenzetool.h \
    shared_a5_m4/mcc.h \
    shared_a5_m4/errors.h \
    shared_a5_m4/defines.h \
    source/Service/Tools/invertertool.h \
    source/print.h \
    source/Service/Calib/calibColli.h \
    source/Service/Calib/calibParking.h \
    source/BiopsyExtended/biopsyExtendedPage.h \
    source/BiopsyExtended/biopsyExtendedDevice.h \
    source/BiopsyStandard/biopsyStandardPage.h \
    source/BiopsyStandard/biopsyStandardDevice.h \
    source/biopsymodule.h

SOURCES += \
    source/main.cpp \
    source/tcpipserver.cpp \
    source/tcpipclient.cpp \
    source/echodisplay.cpp \
    source/resource.cpp \
    lib/gwindow.cpp \
    source/mainpage.cpp \
    lib/dbase.cpp \
    source/pagelanguages.cpp \
    source/pageacr.cpp \
    source/pagealarms.cpp \
    source/console.cpp \
    source/generatore.cpp \
    lib/mcccom.cpp \
    source/sysio.cpp \
    source/prototoconsole.cpp \
    source/protoconsole.cpp \
    source/serverdebug.cpp \
    lib/insertcalc.cpp \
    source/config.cpp \
    source/collimatore.cpp \
    source/compressor.cpp \
    source/potter.cpp \
    lib/msgbox.cpp \
    source/loader.cpp \
    source/Service/servicepanelmenu.cpp \
    source/Service/Calib/calibmenu.cpp \
    source/startuppage.cpp \
    source/Service/Calib/calibcompressorposition.cpp \
    lib/numericpad.cpp \
    source/Service/Calib/calibcompressorforce.cpp \
    source/ImagePage.cpp \
    source/openStudyPage.cpp \
    source/projectionPage.cpp \
    source/Service/Calib/calibzerosetting.cpp \
    source/Service/Setup/system.cpp \
    source/Service/Calib/calibfilter.cpp \
    source/Service/Calib/calibpower.cpp \
    source/Service/Calib/calibpot.cpp \
    source/Service/Calib/calibstarter.cpp \
    source/Service/Tools/toolsmenu.cpp \
    source/Service/Tools/tiltingtool.cpp \
    source/Service/Tools/armtool.cpp \
    source/Service/Tools/lenzetool.cpp \
    source/Service/Tools/invertertool.cpp \
    source/print.cpp \
    source/Service/Calib/calibColli.cpp \
    source/Service/Calib/calibParking.cpp \
    source/BiopsyExtended/biopsyExtendedPage.cpp \
    source/BiopsyExtended/biopsyExtendedDevice.cpp \
    source/BiopsyStandard/biopsyStandardPage.cpp \
    source/BiopsyStandard/biopsyStandardDevice.cpp \
    source/biopsymodule.cpp


FORMS += \
    lib/insertcalc.ui \
    lib/msgbox.ui \
    source/Service/servicepanelmenu.ui \
    source/Service/Calib/calibmenu.ui \
    source/startuppage.ui \
    source/Service/Calib/calibcompressorposition.ui \
    lib/numericpad.ui \
    source/Service/Calib/calibcompressorforce.ui \
    source/Service/Calib/calibzerosetting.ui \
    source/Service/Setup/system.ui \
    source/Service/Calib/calibFilter.ui \
    source/Service/Calib/calibPower.ui \
    source/Service/Calib/calibpot.ui \
    source/Service/Calib/calibStarter.ui \
    source/Service/Tools/toolsmenu.ui \
    source/Service/Tools/tilting.ui \
    source/Service/Tools/arm.ui \
    source/Service/Tools/lenze.ui \
    source/Service/Tools/inverter.ui \
    source/Service/Calib/calibColli.ui \
    source/Service/Calib/calibParking.ui \
    source/BiopsyExtended/biopsy.ui



unix:!macx: LIBS += -L$$PWD/../../../../home/user/Desktop/CHIMERA/ltib/rootfs/usr/lib/ -lmcc

INCLUDEPATH += $$PWD/../../../../home/user/Desktop/CHIMERA/ltib/rootfs/usr/include
DEPENDPATH += $$PWD/../../../../home/user/Desktop/CHIMERA/ltib/rootfs/usr/include

RESOURCES += \
    resource/ui/grafica.qrc

OTHER_FILES += \
    Deploy/RELEASE/Release_Note \
    Deploy/PACKAGE/Release_note \
    Deploy/PACKAGE/Release_note.txt

