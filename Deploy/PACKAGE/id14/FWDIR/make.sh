#!/bin/bash
# Questo bash fle consente di verificare se tutti i firmware sono presenti 

INPUT=$1
DIR=`dirname $0`/  

# LISTA FILES 
FILES="
FW190.hex
FW240.hex
FW244.hex
FW249U1.hex
FW249U2.hex
FW249U1A.hex
FW249U2A.hex
FW269.hex
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
	rm -f ${DIR}*.tar
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
	tar -cf FirmwarePackage.tar $FILES
	cd - 
	exit 1
fi

echo MISSING COMMAND: clean, test, build
exit 0

