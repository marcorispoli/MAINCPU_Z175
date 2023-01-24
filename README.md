# PROJECT Z175 MAIN CPU SOFTWARE SUITE

# Overview

This project provides the softwares to be installed into the Display terminals (Master and Slave) 
of the Gantry.

# Related Document

+ [Compilation Procedures][CP]: this is the document describing the compilation process;
+ [Deploy Procedures][DP]: this is the document describing the deploy process;
+ [Project Description Notes][PP]: this is the document describing the functional description;
+ [IRS Interface Commands][IC]: this is the document describing the IRS Interface commands;
+ [Not resolved issues][ISS]: this is the document describing the known issues;
+ [Release Note][RN]: this is the document describing the last release note;


[CP]:./DOC/Project%20Build.md
[DP]:./DOC/Project%20Deploy.md
[PP]:./DOC/SoftwareDetailedSpecification.md
[IC]:./DOC/IRS%20Command%20Description.md
[ISS]:./DOC/Issues.md
[RN]:./DOC/Release%20Note.md




# Directory Content

+ DOC: project documentation;
+ Deploy: This directory contains all the files and batch procedures to 
deploy the application;
+ languages: this directory contains the language translate source and compiled files;
+ lib: this directory contains library source code for the DBTController application;
+ M4: this directory contains the Project to compile the m4_master.bin and m4_slave.bin executables;
+ resource: this directory contains the assets files of the DBTController application;
+ shared_a5_m4: this directory contains common header files of the m4 project and DBTController project;
+ source: this directory contains the source code of theDBTController executable.
 
# Project modifcation and compilation

The project shall be compilaed into a dedicated Virtual Machine
with the proper compilers and IDE to cross compile the application.

# Deploy Process

In order to deploy the application it shall be executed the following steps:
- Change the directory Deploy/PACKAGE/ idxx name assigning to xx the value of the target package ID code (example id09); 
- Compile the DBTController application;
- Compile the m4_master application;
- Compile the m4_slave application;
- Copy the compiled DBTController, m4_master.bin, m4_slave.bin into the Deploy/PACKAGE/idxx/SWDIR/BIN directory;
- Copy any other rilevant file into the Deploy/PACKAGE/idxx/SWDIR/ in the proper destination directory;
- In case a new firmware release should be present, copy it into the Deploy/PACKAGE/idxx/FWDIR/;
- Update the file  Deploy/PACKAGE/idxx/SWDIR/CONFIG/firmwares.cnf with the proper revision codes;
- Modify the files:  
   + Deploy/PACKAGE/idxx/make_release.sh;
   + Deploy/PACKAGE/idxx/manual_install_master.sh;
   + Deploy/PACKAGE/idxx/manual_install_slave.sh;
- execute the batch file make.sh;
- execute the batch file make_release.sh;

If all the previous operations complete successfully, then the directory  Deploy/RELEASE
will contain all the files that shall be released to the Production.

