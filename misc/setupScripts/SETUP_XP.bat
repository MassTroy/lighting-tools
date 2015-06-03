echo off
cls

echo Creating Directories
mkdir %SystemDrive%\bin
mkdir %SystemDrive%\bin\ltools\

echo Copying files
copy lightning-format.exe "%SystemDrive%\bin\lightning-format.exe"
copy lformat-prefs.txt "%SystemDrive%\bin\ltools\lformat-prefs.txt"
echo @lightning-format %* >> %SystemDrive%\bin\lformat.bat

echo Setting Path enviroment variable
echo SET Path=%PATH%;%SystemDrive%\bin >> %SystemDrive%\AUTOEXEC.bat
REM SETX PATH "%PATH%;%SYSTEMDRIVE\bin"

REM echo Creating registry keys
REM REG ADD HKEY_LOCAL_MACHINE\SOFTWARE\lightning-tools /v PrefPath /t REG_SZ /d "%SystemDrive%\bin\ltools\\"

REM echo "Selecting yes will allow you to open any folder in commmand prompt"
REM echo "This is not required, but is extremely helpful"
REM echo "Would you like to install this feature?"
REM OpenCommandPrompt.reg

echo DONE!
Pause


