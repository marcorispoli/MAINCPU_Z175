#!/bin/bash
# NESSUN PARAMETRO: ISTALLA TUTTO IL CONTENUTO 
ROOT=$1

#VERIFICA ESISTENZA DELLA STRUTTURA DATI
if [ -d "$ROOT/resource" ] ; then
        echo
else
        echo "CREAZIONE STRUTTURA DATI"
        mkdir $ROOT/resource
        mkdir $ROOT/resource/config
fi
                                        
#AGGIORNAMENTO FILE DI SISTEMA
cp DBTController $ROOT/
cp m4_slave.bin $ROOT/
cp mcc.ko $ROOT/lib/modules/3.0.15-ts-armv7l/extra/
cp dbts $ROOT/
chmod 777 $ROOT/DBTController

cp monta.sh $ROOT/
chmod 777 $ROOT/monta.sh

cp backup.sh $ROOT/
chmod 777 $ROOT/backup.sh


cp restore.sh $ROOT/
chmod 777 $ROOT/restore.sh

sync
