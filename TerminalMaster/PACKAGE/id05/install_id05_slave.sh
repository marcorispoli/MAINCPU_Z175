##############################################################
# Definizione del Package di riferimento
##############################################################
PKGCODE=ID05

# Definizione delle directory
HOME=/home/user
CONFIG=/resource/config
INSTALL=/INSTALL


##############################################################
# Sezione standard per tutte le istallazioni manuali e hotfix:
# Viene inserito il nuovo package e copiato il file 
# firmwares.cnf nella directory config. 
# Se file sono cambiati allora il sistema automaticamente
# istallerà tutto al riavvio.
##############################################################
# Pulitura home directory
echo CLEANING UP HOME DIRECTORY ...
mv $HOME/PACKAGE_$PKGCODE.tar /
rm -f $HOME/*
rm -r $HOME/*
mv /PACKAGE_$PKGCODE.tar $HOME/

# Estrazione contenuto in home/user  
echo PACKAGE EXTRACTING ..
cd $HOME
tar -xf PACKAGE_$PKGCODE.tar SWPackage.tar

##############################################################
# Questa sezione è dedicata all'hotfix che non preveda
# una modifica ai files sotto controllo di revisione
##############################################################
 
# Aggiunge la GUI di recovery
tar -xf SWPackage.tar BIN/DBTController ROOTFS/dbts
mv BIN/DBTController  /
mv BIN/m4_slave.bin  /
mv ROOTFS/dbts /
chmod 777 /DBTController


##############################################################
# Pulitura home directory
echo CLEANING UP HOME DIRECTORY ...
rm -f $HOME/*
rm -r $HOME/*

sync

echo INSTALLATION COMPLETED. 










