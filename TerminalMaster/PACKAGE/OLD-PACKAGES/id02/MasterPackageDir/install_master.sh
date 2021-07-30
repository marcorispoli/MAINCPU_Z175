#!/bin/bash
# NESSUN PARAMETRO: ISTALLA TUTTO IL CONTENUTO 
ROOT=$1


#Copia solo se il file non esiste
COPY_IF_NOT_EXIST()
{
FILE=$1 
DIR=$2
if [ -s ${DIR}${FILE} ] ; then
	#SE IL FILE ESISTE
	echo "FILE $FILE GIA ESISTENTE"
else
	#SE IL FILE  NON ESISTE
	echo "COPY FILE $FILE TO $DIR"
	cp $FILE $DIR
fi
}

#VERIFICA ESISTENZA DELLA STRUTTURA DATI
if [ -d "$ROOT/resource" ] ; then
	echo
else
	echo "CREAZIONE STRUTTURA DATI"
	mkdir $ROOT/resource
	mkdir $ROOT/resource/config
	mkdir $ROOT/resource/config/Tubes
fi

#FILES DI CONFIGURAZIONE
COPY_IF_NOT_EXIST user.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST compressore.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST collimazione.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST collimazione_analogica.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST tomoCfg_1F.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST tomoCfg_2F.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST tomoCfg_3F.cnf $ROOT/resource/config/
COPY_IF_NOT_EXIST tomoCfg_4F.cnf $ROOT/resource/config/
cp armCfg.cnf $ROOT/resource/config/
cp trxCfg.cnf $ROOT/resource/config/
cp lenzeCfg.cnf $ROOT/resource/config/
cp firmwares.cnf $ROOT/resource/config/
cp cgs.cnf $ROOT/resource/config/

#AGGIORNAMENTO TEMPLATES TUBI
rm -r $ROOT/resource/config/Tubes/TEMPLATE_*
cp -r TEMPLATE_* $ROOT/resource/config/Tubes/

#AGGIORNAMENTO FILE DI SISTEMA
cp DBTController $ROOT/
cp m4_master.bin $ROOT/
cp mcc.ko $ROOT/lib/modules/3.0.15-ts-armv7l/extra/
cp dbtm $ROOT/
chmod 777 $ROOT/DBTController
cp backup.sh $ROOT/
cp restore.sh $ROOT/
cp monta.sh $ROOT/
chmod 777 $ROOT/backup.sh
chmod 777 $ROOT/restore.sh
chmod 777 $ROOT/monta.sh


sync


