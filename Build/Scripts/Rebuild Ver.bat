cd ..
call MachineEnviroment.bat
@echo off
cd %BASEDIR%
echo ---- Compression Complete ----
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMInstallScript.iss"
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateScript.iss"
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateScriptMembership.iss"
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateIntermate.iss"
cd %BASEDIR%
call \Build\Scripts\upload.bat
echo ---- Transfer Complete ----
pause