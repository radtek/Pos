@echo off
REM Sort out 64 or 32 bit Backup.
SET BASEDIR32=C:\Program Files\MenuMate
SET BASEDIR64=C:\Program Files (x86)\MenuMate
SET BASEDIR=%BASEDIR32%
IF EXIST "%BASEDIR64%" SET BASEDIR=%BASEDIR64%
IF NOT EXIST "%BASEDIR%" GOTO :EndNoBaseDir

CALL :CreateTask
CALL :UpdateBackupScript 
GOTO :End 

:CreateTask
echo.
schtasks /delete /tn "Backup Menumate"
schtasks /create /tn "Backup Menumate" /ru "System" /tr "\"%BASEDIR%\backup.cmd\"" /sc daily /st 03:00:00
GOTO :EOF

:UpdateBackupScript 
CALL :getcopypath
"%BASEDIR%\sed.exe" -e "s/SET BACKUP_DIR=.*/SET BACKUP_DIR=%copypath:\=\\%/" "%BASEDIR%\Backup.orig" > "%BASEDIR%\Backup.cmd"
GOTO :EOF

:getcopypath
echo.
echo.
set copypath=\\POS2\MenuMate
for /f "delims=" %%A in ('CALL "%BASEDIR%\sed.exe" -n -e "s/SET BACKUP_DIR=\(.*\)$/\1/p" "%BASEDIR%\Backup.cmd"') do set copypath=%%A
set /p copypath=Enter backup copy path (Default=%copypath%):
echo.
echo Checking path %copypath%... (This should be fast if it exists!)
if exist %copypath% goto :EOF
echo.
echo Path "%copypath%" not found!
goto getcopypath

:EndNoBaseDir
echo Unable to Locate MenuMate installation at %BASEDIR%
echo Task not created.

:end
pause