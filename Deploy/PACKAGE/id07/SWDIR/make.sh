#!/bin/bash
# Questo bash fle consente di verificare se tutti i firmware sono presenti 

INPUT=$1
DIR=`dirname $0`/  

# LISTA FILES 
FILES="
CONFIG/collimazione.cnf 
CONFIG/armCfg.cnf
CONFIG/trxCfg.cnf
CONFIG/tomoCfg_1F.cnf
CONFIG/tomoCfg_2F.cnf
CONFIG/tomoCfg_3F.cnf
CONFIG/tomoCfg_4F.cnf
CONFIG/tomoCfg_5F.cnf
BATCH/monta.sh
TUBES/TEMPLATE_XM1016THI 
TUBES/TEMPLATE_35KV_XM1016T
TUBES/TEMPLATE_XK1016THI 
TUBES/TEMPLATE_35KV_XK1016T
BIN/DBTController 
BIN/m4_master.bin 
BIN/m4_slave.bin 
ROOTFS/mcc.ko 
ROOTFS/dbtm
ROOTFS/dbts
CONFIG/firmwares.cnf
BATCH/backup.sh
BATCH/restore.sh
install_master.sh
install_slave.sh"

EXIST_FILE()
{
FILE=$1
if [ !  -s ${FILE} ] ; then 
 	echo MISSING: $FILE
 	exit 0
fi
}


if [ "$INPUT" == "clean" ] ; then
	rm -f ${DIR}SWPackage.tar
	exit 1
fi

if [ "$INPUT" == "test" ] ; then
	for ff in $FILES ; do
		EXIST_FILE ${DIR}$ff	
	done
	exit 1
fi

if [ "$INPUT" == "build" ] ; then
	cd $DIR
	tar -cf SWPackage.tar $FILES 
	cd -
	exit 1
fi

echo MISSING COMMAND: clean, test, build
exit 0

