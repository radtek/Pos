@echo off
if exist setenv.bat call setenv.bat
computil SetupD5
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\D5\*.* md ..\D5 >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
copy ..\Source\RpForm*.dfm ..\D5 >nul
copy Rave50VCL.dpk Rave50VCLBEX50.dpk >nul
%NDD5%\bin\dcc32.exe Rave50VCLBEX50.dpk -B -H -W -Q -N..\D5 -LE..\D5 -LN..\D5 -U..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del Rave50VCLBEX50.dpk >nul
copy ..\D5\Rave50VCLBEX50.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDD5%\bin\dcc32.exe ..\Source\RpTChart.pas -M -H -W -Q -N..\D5 -LE..\D5 -LN..\D5 -U..\D5 -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD5%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -N..\D5 -LE..\D5 -LN..\D5 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD5%\bin\dcc32.exe ..\Source\RvDLADO.pas -M -H -W -Q -N..\D5 -LE..\D5 -LN..\D5 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDD5%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\D5 -LE..\D5 -LN..\D5 -L..\D5\Rave50VCLBEX50.dcp -I..\Source -U..\D5;..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\D5\dclRave.dcu >nul
del ..\D5\dclRave.dcp >nul
del ..\D5\Rave50VCLBEX50.dcu >nul
del ..\D5\Rave50VCLBEX50.bpl >nul
del ..\D5\RPExeFrm.dcu >nul
del ..\D5\RaveReg.dcu >nul

echo.
echo SUCCESS: D5 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: D5 Packages NOT Compiled
echo.
pause
:endok