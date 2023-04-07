INTERSIL CORPORATION -- General Purpose - 64xx series LNB Controllers

This PC/i2c Interface Software is applicable to the ISL6405, ISL6424, ISL6421/A, ISL6425, ISL6423 & ISL6422
controllers

Written by: Theju Bernard
Revision: 1.2
Date: 01/25/06

*** This document will help with the installation of the
*** the necessary drivers and software to for the usb/i2c
*** interface to the ISL64xx evaluation boards.
*** You will need a windows 95/98, NT/2000, XP machine with
*** a standard USB port.
*** DO NOT CONNECT THE USB CABLE TILL THE DRIVER IS INSTALLED

*****************************************************************************************
*** You should refer to the ISL64xx datasheet and application note as well as
*** the documentation for the usbi2cio board from DeVaSys at www.devasya.com/i2ciob.htm
*****************************************************************************************

Print this document.

1.a Insert the CD/Floppy into the drive and run the file "setup.bat". This will copy 
    the required driver files into the directory C:\USBI2C. 

1.b If the zip file is downloaded from the Web, extract the files into the directory C:\USBI2C
    (use the make dir option if needed). The driver can be downloaded from a link on any of the 
    following LNB controllers : ISL6405/21/21A/24/25 on their respective webpages.

2.  Power up Devasys' usb/i2c board.  This board has the option of being supplied
    with 3.3V through the usb bus or externally with 5V.  With these options the
    i2c bus can operate at 3.3V or 5V.  This is controlled through a jumper 
    (J3, refer to the schematic of DeVaSys USB-i2cio board Rev B on the floppy).  
    For setup, powering up with external 5V is recommended

3.  After the usb/i2c board is powered up, connect the USB-2 cable to a standard
    USB port.

FOR USB to I2C Interface board RevA or RevB :

4.  Windows should detect the new USB device and the 'Found New Hardware Wizard' should begin.
    This will help you install the drivers. Follow the directions on the screen until you are 
    requested to search for the drivers.At this point, you should select the choose location 
    option and browse the directory C:\USBI2C\DRIVER\REVAB\

5.  Follow the remaining instructions to complete the driver installation.

6.  If this is successful another 'Found New Hardware Wizard' window will appear.
    This time you will repeat step 4 and 5.  Your usb/i2c board should be ready
    to go.

7.  Now make the neccessary connections to the ISL64xx evaluation board and power
    it up as required (Refer to the ISL64xx Datasheet and Application Note).

8.  Run the application program from the C:\USBI2C\REVAB :
	a) ISL6405&24_i2c_revc.exe for controlling the ISL6405 and ISL6424 devices
 	b) ISL6421&25_i2C_reva.exe for controlling the ISL6421 and ISL6425 devices
        c) ISL6423_i2c_RevD.exe for controlling the ISL6423 device
        d) ISL6422_i2c_RevC.exe for controlling the ISL6422 device
    and then select the open device command button.

9.  The program will initialize to the standard test values for each of the 
    system registers.  You can toggle the bits as needed and then click on 
    the 'write i2c' button to write to the system registers.  The lower portion 
    of the application window shows the current values of the system register bits.
    They will be updated continuously.

FOR USB to I2C Interface board RevB3

10.  Windows should detect the new USB device and the 'Found New Hardware Wizard' should begin.
    This will help you install the drivers. Follow the directions on the screen until you are 
    requested to search for the drivers.At this point, you should select the choose location 
    option and browse the directory C:\USBI2C\DRIVER\REVB3\

11.  Follow the remaining instructions to complete the driver installation.


12.  Now make the neccessary connections to the ISL64xx evaluation board and power
    it up as required (Refer to the ISL64xx Datasheet and Application Note).

13.  Run the application program from the C:\USBI2C\REVB3 :
	a) ISL6405&24_i2c_revc.exe for controlling the ISL6405 and ISL6424 devices
 	b) ISL6421&25_i2C_reva.exe for controlling the ISL6421 and ISL6425 devices
        c) ISL6423_i2c_RevD.exe for controlling the ISL6423 device
        d) ISL6422_i2c_RevC.exe for controlling the ISL6422 device
    and then select the open device command button.

14.  The program will initialize to the standard test values for each of the 
    system registers.  You can toggle the bits as needed and then click on 
    the 'write i2c' button to write to the system registers.  The lower portion 
    of the application window shows the current values of the system register bits.
    They will be updated continuously.

*** Fixes for the following errors :

1.  'No USB device detected.' 
	-- Make sure that the drivers were installed correctly.  If Windows did not
           detect your USB device, try running the Add/Remove Hardware Wizard in the
           control panal.
	-- Make sure the USB board is powered up (internally or externally not both)
	-- Did windows detect the USB device when plugged in?

2.  'Incorrect Return Value'
	-- The ISL64xx board may not be powered up.  Vcc=5V/3.3V and Vin = 12V
	-- Make sure SCL and SDA are connected correctly. Red=SDA & White=SCL.
	-- Refer the ISL64xx Application Note to make sure that the pins on the 
	   evaluation board are connected correctly.

If problems persist, contact:	Tim Klemann
				tklemann@intersil.com
				(321)724-7489