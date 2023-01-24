<!-- ------------------------Document header ------------------------------------>
 ![logo](./IMG/logo.png)
 ___

 <br/><br/>
 <br/><br/>

<!-- ------------------------Document TITLE PAGE --------------------------------->
 
<center><font size =6">Z175 MAIN CPU</font></center>
<center><font size =5">Issues</font></center>
<center><font size =4">REV : 1.0</font></center>

 <br/><br/>
 <br/><br/>
 <br/><br/>
 <br/><br/>

<center>
<font size =4">

|Date |Revision | Change log| Author|
|:---| :----: | :----: |:---|
|19/01/2023|1.0|First release| M. Rispoli|

</font>
</center>

<div style="page-break-after: always;"></div>

# Document Overview

This document describes the known not yet fixed issues in the current release.

# Issues Description

## Open Door Error at the Startup

It has been reported an occasional error after the Power Up that require to restart again the Mammo Unit.  
At the moment the reason is unknown due to the difficult in reproducing the event. 
However it has been introduced into this version a special debug command in the case of the event to try to reset the error. 

## Tube activation timeout

This issue happen when the AWS requests for a Tube positioning 
with the Tube already positioned.
The Gantry never signals the AWS with the ACK command and the AWS 
goes into timeout waiting the event. 
The AWS can repeat the command and the problem goes eway.

