#!/bin/bash
# Questo bash fle consente di verificare se tutti i firmware sono presenti 

INPUT=$1
DIR=`dirname $0`/  

# LISTA FILES 
FILES="
DBTController
m4_slave.bin
mcc.ko
dbts
install_slave.sh
monta.sh
backup.sh
restore.sh
"

EXIST_FILE()
{
FILE=$1
if [ !  -s ${FILE} ] ; then 
 	echo MISSING: $FILE
 	exit 0
fi
}


if [ "$INPUT" == "clean" ] ; then
	rm -f ${DIR}SlavePackage.tar
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
	tar -cf SlavePackage.tar $FILES 
	cd -
	exit 1
fi

echo MISSING COMMAND: clean, test, build
exit 0

