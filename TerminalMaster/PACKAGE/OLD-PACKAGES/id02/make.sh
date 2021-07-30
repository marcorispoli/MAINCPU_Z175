#!/bin/bash
# Questo bash fle consente di creare automaticamente tutto 
# il package

# RECUPERA IL CODICE NUMERICO DEL PACKAGE
DIR=${PWD##*/}
ID=${DIR##id}

INPUT=$1
if [ "$INPUT" == "?" ] ; then
	echo "make.sh xx: compila tutto il package e produce PACKAGE_IDxx.tar"
	echo "make.sh clean: cancella tutti i files temporanei"
	echo "make.sh test: effettua il test di completezza contenuto"
	
	exit 0
fi


if [ "$INPUT" == "clean" ] ; then
	echo PACKAGE CLEANING ..
	FWDIR/make.sh clean
	MasterPackageDir/make.sh clean
	SlavePackageDir/make.sh clean
	rm PACKAGE_ID*.tar
	exit 1
fi

if [ "$INPUT" == "test" ] ; then
	echo PACKAGE  TESTING ..
	FWDIR/make.sh test
	MasterPackageDir/make.sh test
	SlavePackageDir/make.sh test
	exit 1
fi


#####################################################
# Verifica preliminare che tutto sia in ordine
FWDIR/make.sh test
if [ $? == 0 ] ; then
	echo Archivio Firmwares non corretto
	exit 0
fi 

MasterPackageDir/make.sh test
if [ $? == 0 ] ; then
	echo Archivio Master Package non corretto 
	exit 0
fi 

SlavePackageDir/make.sh test
if [ $? == 0 ] ; then
	echo Archivio Slave Package non corretto 
	exit 0
fi 
####################################################

#Nome del file archivio 
PACKAGE_FILE="PACKAGE_ID${ID}.tar"
echo CREAZIONE DEL PACKAGE: $PACKAGE_FILE


# cancella gli archivi già realizzati
rm -f *.tar

FWDIR/make.sh clean
MasterPackageDir/make.sh clean
SlavePackageDir/make.sh clean

# Creazione del FirmwarePackage.tar
FWDIR/make.sh build 
MasterPackageDir/make.sh build 
SlavePackageDir/make.sh build 

#Creazione del PACKAGE FILE
mv FWDIR/FirmwarePackage.tar ./
mv MasterPackageDir/MasterPackage.tar ./
mv SlavePackageDir/SlavePackage.tar ./
cp MasterPackageDir/firmwares.cnf ./
tar -cf $PACKAGE_FILE FirmwarePackage.tar firmwares.cnf MasterPackage.tar SlavePackage.tar
rm -f FirmwarePackage.tar
rm -f MasterPackage.tar
rm -f SlavePackage.tar
rm -f firmwares.cnf

#################################################################
if [ ! -d /mnt/nfs/PACKAGE ] ; then
	echo "monta il disco remoto Linux"
	/monta.sh
fi

#Se c'è connessione con Linux allora copia anche il contenuto
#nella directory relativa
if [ -d /mnt/nfs/PACKAGE ] ; then
	echo "copia archivio su Linux..."
	cp -r /home/user/id${ID} /mnt/nfs/PACKAGE
	cp MasterPackageDir/firmwares.cnf /mnt/nfs/PACKAGE
	cp PACKAGE_ID${ID}.tar /mnt/nfs/PACKAGE
	sync
else
	echo "Impossibile copiare copiare archivio su Linux"
fi
