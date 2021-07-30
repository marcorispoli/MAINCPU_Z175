#!/bin/bash
# Questo bash fle consente di verificare se tutti i firmware sono presenti 

INPUT=$1
DIR=`dirname $0`/  

# LISTA FILES 
FILES="
user.cnf 
compressore.cnf 
collimazione.cnf 
collimazione_analogica.cnf
armCfg.cnf
lenzeCfg.cnf 
trxCfg.cnf
tomoCfg_1F.cnf
tomoCfg_2F.cnf
tomoCfg_3F.cnf
tomoCfg_4F.cnf
cgs.cnf
monta.sh
TEMPLATE_XM1016THI 
TEMPLATE_35KV_XM1016T
TEMPLATE_XM12
TEMPLATE_XM1016
DBTController 
m4_master.bin 
mcc.ko 
dbtm
firmwares.cnf
backup.sh
restore.sh
install_master.sh"

EXIST_FILE()
{
FILE=$1
if [ !  -s ${FILE} ] ; then 
 	echo MISSING: $FILE
 	exit 0
fi
}


if [ "$INPUT" == "clean" ] ; then
	rm -f ${DIR}MasterPackage.tar
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
	tar -cf MasterPackage.tar $FILES 
	cd -
	exit 1
fi

echo MISSING COMMAND: clean, test, build
exit 0

