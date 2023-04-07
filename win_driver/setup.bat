@ECHO OFF
ECHO The I2C Driver files will be be installed into the directory C:\USBI2C
PAUSE

XCOPY "*.*" "C:\USBI2C\*.*" /s /y /f

REM ECHO Files copied successfully.

notepad "C:\USBI2C\readme.txt"
 