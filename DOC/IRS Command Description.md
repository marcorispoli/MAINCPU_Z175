<!-- ------------------------Document header ------------------------------------>
 ![logo](./IMG/logo.png)
 ___

 <br/><br/>
 <br/><br/>

<!-- ------------------------Document TITLE PAGE --------------------------------->
 
<center><font size =6">Z175 MAIN CPU</font></center>
<center><font size =5">IRS Command Interface</font></center>
<center><font size =4">REV : ID9</font></center>

 <br/><br/>
 <br/><br/>
 <br/><br/>
 <br/><br/>

<center>
<font size =4">

|Date |Revision | Change log| Author|
|:---| :----: | :----: |:---|
|01/01/2023|ID9|First release| M. Rispoli|

</font>
</center>

<div style="page-break-after: always;"></div>

- [1. Document Overview](#1-document-overview)
- [2. IRS Overview](#2-irs-overview)
- [3. IRS activation](#3-irs-activation)
- [4. PC connection setup](#4-pc-connection-setup)
- [5. IRS connection establishment](#5-irs-connection-establishment)
- [6. Command syntax](#6-command-syntax)
- [7. Group command description](#7-group-command-description)
  - [7.1. SYSTEM (system:)](#71-system-system)
    - [7.1.1. ***setDATE***](#711-setdate)
    - [7.1.2. ***getRevisions***](#712-getrevisions)
    - [7.1.3. ***setShotMode***](#713-setshotmode)
    - [7.1.4. ***setShotNodetMode***](#714-setshotnodetmode)
    - [7.1.5. ***setCalibKv***](#715-setcalibkv)
    - [7.1.6. ***setCalibIa***](#716-setcalibia)
    - [7.1.7. ***setCalibTomo***](#717-setcalibtomo)
    - [7.1.8. ***setOperatingMode***](#718-setoperatingmode)
    - [7.1.9. ***reboot***](#719-reboot)
    - [7.1.10. ***setPowerOff***](#7110-setpoweroff)
    - [7.1.11. ***setUnpark***](#7111-setunpark)
    - [7.1.12. ***setPark***](#7112-setpark)
  - [7.2. CONFIG (config:)](#72-config-config)
    - [7.2.1. ***enableAccessory***](#721-enableaccessory)
    - [7.2.2. ***setSN***](#722-setsn)
    - [7.2.3. ***setPSW***](#723-setpsw)
    - [7.2.4. ***enableStarterKeep***](#724-enablestarterkeep)
    - [7.2.5. ***setTubeTemp***](#725-settubetemp)
    - [7.2.6. ***setLanguage***](#726-setlanguage)
    - [7.2.7. ***resetGantry***](#727-resetgantry)
    - [7.2.8. ***resetKvCalib***](#728-resetkvcalib)
    - [7.2.9. ***sysBackup***](#729-sysbackup)
    - [7.2.10. ***sysRestore***](#7210-sysrestore)
  - [7.3. DRIVERS (driver:)](#73-drivers-driver)
    - [7.3.1. ***freeze***](#731-freeze)
    - [7.3.2. ***run***](#732-run)
    - [7.3.3. ***read8***](#733-read8)
    - [7.3.4. ***read16***](#734-read16)
    - [7.3.5. ***write8***](#735-write8)
    - [7.3.6. ***write16***](#736-write16)
    - [7.3.7. ***command***](#737-command)
    - [7.3.8. ***special***](#738-special)



# 1. Document Overview

This document describes the functions of the DBTController IRS command interface.

# 2. IRS Overview

The IRS (Remote Service Interface) is basically a low level command "shell" providing textual functions that interact with the DBT with a very low level functions.

# 3. IRS activation

The IRS interface can be activated locally on the AWS station, launching the pre installed application called
putty.exe. Otherwise, the IRS interface can be activated using a Laptop PC that should be connected
through the Ethernet interface to the back service Ethernet slot of the DBT.

# 4. PC connection setup

In case it should be used a PC, after connecting it to the service socket (in the back of the DBT), it has to set
the IP address of the PC to : 192.168.30.10.
To connect the interface, the program putty.exe shall be used: the program has not to be installed, so it can
be also launched by the USB mass storage in case or copied to an PC internal directory.

# 5. IRS connection establishment

1. Launch putty.exe;
2. Select from the left panel (Category) the "Session" voice:
   
![](./IMG/irs_connection.png)

3. Set the "Basic options.." fields as follows:

![](./IMG/irs_connection_1.png)

4. Press the button OPEN to start the session: If the connection success the a shell pop-up displaying a welcome row reporting the current revision version
of the IRS interface.

# 6. Command syntax

The command syntax is easy:

**[group name:]** \<command name> [parameter1, parameter2,..,parameter-n]

***Group Name Description***

The **group_name** is a sub-set of commands grouped toghether

For example:
  
> system:
system > 


In the example above, the "system" sub group is entered, having access to a set of commands belonging to this group.

In order to get the command content of the group, it can be send the **?** command into the group:

> system: ?

    NOTE: when a group is entered it is not necessary to repeat the group name when a command is typed!

In order to get the whole group availables, the user can type the command **?** in the root of the Terminal:

Type the following commands:
> ..
> ?

The ".." command causes the Terminal enter the MAIN group (the root of all groups);

The command "?" request for the list of all available groups

# 7. Group command description

## 7.1. SYSTEM (system:)

### 7.1.1. ***setDATE***

|COMMAND|
|---|
setDATE Year Month Day hour minute second
|**DESCRIPTION**|
|Set the current display date|

### 7.1.2. ***getRevisions***

|COMMAND|
|---|
|getRevisions|
|**DESCRIPTION**|
|Get the list of the firmware revisions in the package|


### 7.1.3. ***setShotMode***

|COMMAND|
|---|
|setShotMode|
|**DESCRIPTION**|
|Rx Test mode activation (with detector)|

***NOTE: the study is automatically Opened***

### 7.1.4. ***setShotNodetMode***

|COMMAND|
|---|
|setShotMode|
|**DESCRIPTION**|
|Rx Test mode activation (without detector)|

***NOTE: the study is automatically Opened***

### 7.1.5. ***setCalibKv***

|COMMAND|
|---|
|setCalibKv|
|**DESCRIPTION**|
|Set the kV calibration Mode|

***NOTE: the study is automatically Opened***

### 7.1.6. ***setCalibIa***

|COMMAND|
|---|
|setCalibIa|
|**DESCRIPTION**|
|Set the Anodic Current calibration Mode|

***NOTE: the study is automatically Opened***

### 7.1.7. ***setCalibTomo***

|COMMAND|
|---|
|setCalibTomo num_samples|
|**DESCRIPTION**|
|Set the Tomo calibration Mode|

***NOTE: the study is automatically Opened***

### 7.1.8. ***setOperatingMode***

|COMMAND|
|---|
|setOperatingMode |
|**DESCRIPTION**|
|Set the Operating mode|

***NOTE: the study is NOT automatically Opened***

### 7.1.9. ***reboot***

|COMMAND|
|---|
|reboot |
|**DESCRIPTION**|
|The Master and Slave Terminals will reboot|

### 7.1.10. ***setPowerOff***

|COMMAND|
|---|
|setPowerOff |
|**DESCRIPTION**|
|Init the System power off|

***NOTE: this command works only with the Closed Study***

### 7.1.11. ***setUnpark***

|COMMAND|
|---|
|setUnpark |
|**DESCRIPTION**|
|Activate the ARM unparking procedure|

### 7.1.12. ***setPark***

|COMMAND|
|---|
|setPark |
|**DESCRIPTION**|
|Activate the ARM parking procedure|


## 7.2. CONFIG (config:)

### 7.2.1. ***enableAccessory***

|COMMAND|
|---|
|enableAccessory ON/OFF|
|**DESCRIPTION**|
|Activate/Deactivate the test on the Protection Patient|

### 7.2.2. ***setSN***

|COMMAND|
|---|
|setSN serial_number|
|**DESCRIPTION**|
|Set the Gantry serial number|

***NOTE: The serial_number shall be composed of only digits***

Usage:
> ***setSN 123456*** 

set the Gantry serial number to 123456

> ***setSN .*** 

erases the serial number!


### 7.2.3. ***setPSW***

|COMMAND|
|---|
|setPSW password_number|
|**DESCRIPTION**|
|Set the Gantry service password number|

***NOTE: The password_number shall be composed of only digits***

### 7.2.4. ***enableStarterKeep***

|COMMAND|
|---|
|enableStarterKeep ON/OFF|
|**DESCRIPTION**|
|Enables/Disables the Starter keeping mode|

***NOTE: For Low speed starter set this parameter to OFF!!***

### 7.2.5. ***setTubeTemp***

|COMMAND|
|---|
|setTubeTemp LT HT|
|**DESCRIPTION**|
|Set the Tube alarm threshold(HT) and the reset alarm threshold (LT)|

+ HT: is the Tube temperature in (°C) to trigger the Alarm;
+ LT: is the Tube temperature in (°C) to reset the Alarm; 

### 7.2.6. ***setLanguage***

|COMMAND|
|---|
|setLanguage [ITA\/ENG\/SPA\/FRA\/POR\/RUS]|
|**DESCRIPTION**|
|Set the GUI current language|


***NOTE: The Language is immediatelly changed!!***

### 7.2.7. ***resetGantry***

|COMMAND|
|---|
|resetGantry|
|**DESCRIPTION**|
|Clear the Gantry System Configuration file|

Use this command when the Gantry shall be reconfigured 
for the hardware setup:
- Motor/Manual rotation;
- HS or LS starter;
- Tilting presence;


### 7.2.8. ***resetKvCalib***

|COMMAND|
|---|
|resetKvCalib|
|**DESCRIPTION**|
|Clear the kV-sense calibration file|

Use this command when the kV read back calibration
shall be removed. 

***NOTE: In case of switch between 35kV generator to 49kV or viceversa it is recommended to clear the calibration file!!***

### 7.2.9. ***sysBackup***

|COMMAND|
|---|
|sysBackup backup_name|
|**DESCRIPTION**|
|Create a backup of both Master and Slave terminals|

The command creates the backup files of the Master terminal and the Slave terminal, stored in the HOME directories:
+ MASTER: /home/user/master_***backup_name***.tar
+ SLAVE: /home/user/slave_***backup_name***.tar

### 7.2.10. ***sysRestore***

|COMMAND|
|---|
|sysRestore backup_name|
|**DESCRIPTION**|
|Restore the backup file into Master and Slave terminals|

The file present into the HOME directory of the Master 
shall be: master_***backup_name***.tar;

The file present into the HOME directory of the Slave 
shall be: slave_***backup_name***.tar;

## 7.3. DRIVERS (driver:)

### 7.3.1. ***freeze***

|COMMAND|
|---|
|freeze|
|**DESCRIPTION**|
|Stop the execution of the Mater threads|

***NOTE: Only Expert. Dont't use this command!!***

### 7.3.2. ***run***

|COMMAND|
|---|
|run|
|**DESCRIPTION**|
|Restart the execution of the Mater threads|

***NOTE: Only Expert. Dont't use this command!!***

### 7.3.3. ***read8***

|COMMAND|
|---|
|read8 target address|
|**DESCRIPTION**|
|Read the ***address*** 8 bit register from the ***target*** |

the target can be:

+ PCB269: this is the Compressor board;
+ PCB190: this is the Generator board;
+ PCB244: this is the Potter board;
+ PCB249U1: this is the Collimator U1;
+ PCB249U2: this is the Collimator U2;

the address can be in hex format (0x00AB) or decimal format.

Example:

> read8  PCB269 0x80

This command reads the Bank-0 0x80 register of the PCB269 board.

### 7.3.4. ***read16***

|COMMAND|
|---|
|read16 target address|
|**DESCRIPTION**|
|Read the ***address*** 16 bit register from the ***target*** |

the target can be:

+ PCB269: this is the Compressor board;
+ PCB190: this is the Generator board;
+ PCB244: this is the Potter board;
+ PCB249U1: this is the Collimator U1;
+ PCB249U2: this is the Collimator U2;

This command reads the 16 bit register in little endian encoding where:
+ 0x80 is the address of the L-Byte;
+ 0x81 is the address of the H-Byte.

### 7.3.5. ***write8***

|COMMAND|
|---|
|write8 target address value|
|**DESCRIPTION**|
|Write the ***address*** 8 bit register of the ***target*** |

the target can be:

+ PCB269: this is the Compressor board;
+ PCB190: this is the Generator board;
+ PCB244: this is the Potter board;
+ PCB249U1: this is the Collimator U1;
+ PCB249U2: this is the Collimator U2;

The address can be in hex format (0x00AB) or decimal format.
The val can be in hex format (0x00AB) or decimal format.

***TargetRegister[address] = 8bit-value***

### 7.3.6. ***write16***

|COMMAND|
|---|
|write16 target address value|
|**DESCRIPTION**|
|Write the ***address*** 16 bit register of the ***target*** |

the target can be:

+ PCB269: this is the Compressor board;
+ PCB190: this is the Generator board;
+ PCB244: this is the Potter board;
+ PCB249U1: this is the Collimator U1;
+ PCB249U2: this is the Collimator U2;

The address can be in hex format (0x00AB) or decimal format.
The value can be in hex format (0x00AB) or decimal format.

***TargetRegister[address] = 16bit-value, little endian address***

### 7.3.7. ***command***

|COMMAND|
|---|
|**command** *target command_code command_param*|
|**DESCRIPTION**|
|Send a Command frame to the ***target*** |

the target can be:

+ PCB269: this is the Compressor board;
+ PCB190: this is the Generator board;
+ PCB244: this is the Potter board;
+ PCB249U1: this is the Collimator U1;
+ PCB249U2: this is the Collimator U2;

command_code and command_param can be in hex format or decimal format;

### 7.3.8. ***special***

|COMMAND|
|---|
|**special** *target b1 b2*|
|**DESCRIPTION**|
|Send a Special frame to the ***target*** |

the target can be:

+ PCB269: this is the Compressor board;
+ PCB190: this is the Generator board;
+ PCB244: this is the Potter board;
+ PCB249U1: this is the Collimator U1;
+ PCB249U2: this is the Collimator U2;

b1 and b2 can be in hex format or decimal format;


