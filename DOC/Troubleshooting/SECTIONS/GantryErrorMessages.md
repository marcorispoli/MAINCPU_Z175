<!-- ------------------------Document header ------------------------------------>
 ![logo](./IMG/logo.png)
 ___

 <br/><br/>
 <br/><br/>

<!-- ------------------------Document TITLE PAGE --------------------------------->
 
<center><font size =6">Z175 Troubleshooting</font></center>
<center><font size =5">Error Message Handling</font></center>
<center><font size =4">REV : 1.0</font></center>

 <br/><br/>
 <br/><br/>
 <br/><br/>
 <br/><br/>

<center>
<font size =4">

|Date |Revision | Change log| Author|
|:---| :----: | :----: |:---|
|25/01/2023|1.0|First release| M. Rispoli|

</font>
</center>

<div style="page-break-after: always;"></div>

# 1. TABLE OF CONTENT

- [1. TABLE OF CONTENT](#1-table-of-content)
- [2. SCOPE](#2-scope)
- [3. Gantry Error Message Overview](#3-gantry-error-message-overview)
- [4. Gantry Error Families](#4-gantry-error-families)
- [5. Biopsy related errors: \[00000\] to \[00099\]](#5-biopsy-related-errors-00000-to-00099)
  - [5.1. \[00001\] Biopsy Error in positioning](#51-00001-biopsy-error-in-positioning)
  - [5.2. \[00002\] Biopsy Command Timeout](#52-00002-biopsy-command-timeout)
  - [5.3. \[00003\] Biopsy Activation Busy](#53-00003-biopsy-activation-busy)
  - [5.3. \[00004\] Biopsy AL Undefined Error](#53-00004-biopsy-al-undefined-error)
- [6. C-ARM Rotation errors: \[00100\] to \[00199\]](#6-c-arm-rotation-errors-00100-to-00199)
- [7. Collimator related errors: \[00300\] to \[00399\]](#7-collimator-related-errors-00300-to-00399)
- [8. TILT Rotation errors: \[00400\] to \[00499\]](#8-tilt-rotation-errors-00400-to-00499)
- [9. Generator related errors: \[00500\] to \[00599\]](#9-generator-related-errors-00500-to-00599)
- [10. Gantry Software errors: \[00600\] to \[00699\]](#10-gantry-software-errors-00600-to-00699)
- [11. Paddle detection errors: \[00700\] to \[00799\]](#11-paddle-detection-errors-00700-to-00799)
- [12. Exposure related errors: \[00900\] to \[00999\]](#12-exposure-related-errors-00900-to-00999)
- [13. Compressor device errors: \[01000\] to \[01099\]](#13-compressor-device-errors-01000-to-01099)
- [14. Power Service device errors: \[01100\] to \[01199\]](#14-power-service-device-errors-01100-to-01199)
- [15. Lenze Driver errors: \[01200\] to \[01299\]](#15-lenze-driver-errors-01200-to-01299)
- [16. X-RAY Push Button errors: \[01300\] to \[01399\]](#16-x-ray-push-button-errors-01300-to-01399)
- [17. Compression Pedals errors: \[01400\] to \[01499\]](#17-compression-pedals-errors-01400-to-01499)
- [18. C-ARM Up/Down Pedals errors: \[01500\] to \[01599\]](#18-c-arm-updown-pedals-errors-01500-to-01599)
- [19. C-ARM Rotation Buttons errors: \[01600\] to \[01699\]](#19-c-arm-rotation-buttons-errors-01600-to-01699)
- [20. System Configuration errors: \[01800\] to \[01899\]](#20-system-configuration-errors-01800-to-01899)
- [21. Parking activation errors: \[01900\] to \[01999\]](#21-parking-activation-errors-01900-to-01999)

<div style="page-break-after: always;"></div>

# 2. SCOPE 

This document provides the description of the error signaled by the Z175 Gantry 
and possible solutions.

# 3. Gantry Error Message Overview

The Gantry rise an error message whenever a possible anomaly behavior is detected.

The Message can be momentary if the event can be for its nature momentary or it can be 
persistent as long as the error event is detected.

If the error is momentary then it is signaled by touch screens with a Panel Error visible 
only for 20 secondes. When the panel disappears then the error as well is cleared.

If the error instead should be persistent, then the Error Panel will appear for 20 seconds then 
will hide, but in this case the error condition is not cleared and the Touch Screens will display a proper symbol that the operator can press to reopen the Error Panel.

  >NOTE: more than one error can be active at the same time.

  When the error message is generated, the Acquisition Software (if it is running) will save it into the Acquisition Log File.

The error message is coded as follows:
+ **[M:XXXXX]**
+ The five digits following the [M:] provides the full unique error code;

# 4. Gantry Error Families

The error code is divided into error families:

+ 00000 to 00099: Biopsy related errors;
+ 00100 to 00199: C-ARM Rotation errors;
+ 00300 to 00399: Collimator related errors;
+ 00400 to 00499: TILT Rotation errors;
+ 00500 to 00599: Generator related errors;
+ 00600 to 00699: Gantry Software errors;
+ 00700 to 00799: Paddle detection errors;
+ 00900 to 00999: Exposure related errors;
+ 01000 to 01099: Compressor device errors;
+ 01100 to 01199: Power Service device errors;
+ 01200 to 01299: Lenze Driver errors;
+ 01300 to 01399: X-RAY Push Button errors;
+ 01400 to 01499: Compression Pedals errors;
+ 01500 to 01599: C-ARM Up/Down Pedals errors;
+ 01600 to 01699: C-ARM Rotation Buttons errors;
+ 01700 to 01799: Not Assigned;
+ 01800 to 01899: System Configuration errors;
+ 01900 to 01999: Parking activation errors;

<div style="page-break-after: always;"></div>

# 5. Biopsy related errors: [00000] to [00099]

## 5.1. [00001] Biopsy Error in positioning

This error is generated from the Biopsy device when one of the moving axes 
cannot reach the target. 

There are several possible scenario for this error:

+ The target is closed to one of the mechanical limits: the pointer strikes against the limit.
  + Verify if the axes are properly calibrated for the activation range;

+ The target strikes against some object along the travel; 
  + checks the coordinates received from the acquisition software;

+ The travel distance is very short (less than few millimeters) :
  + the activation can be inaccurate or even could hang until a timeout in the case it should be shorter than few millimeters. This may suggest an excess in the mechanical friction that could be investigate (mechanical regulations).

## 5.2. [00002] Biopsy Command Timeout

This error is generated from the Biopsy device when the activation takes too mutch 
time to complete.

This event can be related to a Biopsy Device Firmware bug 
due to an unknown scenario. Most probably the Biopsy Device 
is hung-up and needs to be reset. If the error persist 
it is necessary to request the R&D assistence.

## 5.3. [00003] Biopsy Activation Busy

This error is generated by Gantry when tries to execute a Biopsy 
activation and the Biopsy device is still completing a positionning.

This scenario should be related to a wrong workflow of the acquisition software
that requested a new activation before to wait the completion of a pending one.

Investigate into the Acquisition software Log to verify if the Software 
sent more activation command to the Gantry. In the case the Log shouldn't evidence such event,
then a possible alternative cause could be an Hung condition in the Biopsy Device Firmware.


## 5.3. [00004] Biopsy AL Undefined Error

This error is generated by Gantry when an internal software procedure 
controlling the Biopsy activation loose the control. 

This is a Gantry Software BUG.
Contact the R&D.


<div style="page-break-after: always;"></div>

# 6. C-ARM Rotation errors: [00100] to [00199]

<div style="page-break-after: always;"></div>

# 7. Collimator related errors: [00300] to [00399]

<div style="page-break-after: always;"></div>

# 8. TILT Rotation errors: [00400] to [00499]

<div style="page-break-after: always;"></div>

# 9. Generator related errors: [00500] to [00599]

<div style="page-break-after: always;"></div>

# 10. Gantry Software errors: [00600] to [00699]

<div style="page-break-after: always;"></div>

# 11. Paddle detection errors: [00700] to [00799]

<div style="page-break-after: always;"></div>

# 12. Exposure related errors: [00900] to [00999]

<div style="page-break-after: always;"></div>

# 13. Compressor device errors: [01000] to [01099]

<div style="page-break-after: always;"></div>

# 14. Power Service device errors: [01100] to [01199]

<div style="page-break-after: always;"></div>

# 15. Lenze Driver errors: [01200] to [01299]

<div style="page-break-after: always;"></div>

# 16. X-RAY Push Button errors: [01300] to [01399]

<div style="page-break-after: always;"></div>

# 17. Compression Pedals errors: [01400] to [01499]

<div style="page-break-after: always;"></div>

# 18. C-ARM Up/Down Pedals errors: [01500] to [01599]

<div style="page-break-after: always;"></div>

# 19. C-ARM Rotation Buttons errors: [01600] to [01699]

<div style="page-break-after: always;"></div>

# 20. System Configuration errors: [01800] to [01899]

<div style="page-break-after: always;"></div>

# 21. Parking activation errors: [01900] to [01999]

<div style="page-break-after: always;"></div>