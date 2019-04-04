ECHO OFF
ECHO Instalando los archivos de arduino due
set source=%CD%\variant.h
set destination=C:\Users\%USERNAME%\AppData\Local\Arduino15\packages\arduino\hardware\sam\1.6.12\variants\arduino_due_x
xcopy %source% %destination% /y /s /r
PAUSE