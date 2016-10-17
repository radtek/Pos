
call "%BASEDIR%\Build\Scripts\BuildCode.bat"
echo ---- Build Complete ----
echo ---- Building Install Files ----

echo. ----------------------------------
echo == "%INNODIR%\iscc.exe %BASEDIR%\Build\Scripts\MMInstallScript.iss"
echo. ----------------------------------
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMInstallScript.iss"

echo. ----------------------------------
echo == "%INNODIR%\iscc.exe %BASEDIR%\Build\Scripts\MMUpdateScript.iss"
echo. ----------------------------------
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateScript.iss"

echo. ----------------------------------
echo == "%INNODIR%\iscc.exe %BASEDIR%\Build\Scripts\MMUpdateScriptMembership.iss"
echo. ----------------------------------
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateScriptMembership.iss"

echo. ----------------------------------
echo == "%INNODIR%\iscc.exe %BASEDIR%\Build\Scripts\MMUpdateIntermate.iss"
echo. ----------------------------------
"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateIntermate.iss"

cd %BASEDIR%
echo ---- Build Complete ----



::call "..\..\..\MachineEnviroment.bat"
::call "%BASEDIR%\Build\Scripts\BuildCode.bat"
::echo ---- Build Complete ----
::echo ---- Building Install Files ----
::"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMInstallScript.iss"
::"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateScript.iss"
::"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateScriptMembership.iss"
::"%INNODIR%\iscc.exe" "%BASEDIR%\Build\Scripts\MMUpdateIntermate.iss"
::cd %BASEDIR%
::echo ---- Build Complete ----
::pause
