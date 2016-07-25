call MachineEnviroment.bat
cd %BASEDIR%
cd "%BASEDIR%\Build\Scripts"
call "%BASEDIR%\Build\Scripts\Build All.bat"
echo ---- Build Complete ----
pause