@echo off
if exist setenv.bat call setenv.bat
computil SetupD4
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\D4\*.* md ..\D4 >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
copy ..\Source\RpForm*.dfm ..\D4 >nul
copy Rave50VCL.dpk Rave50VCLBEX40.dpk >nul
%NDD4%\bin\dcc32.exe Rave50VCLBEX40.dpk -B -H -W -Q -N..\D4 -LE..\D4 -LN..\D4 -U..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del Rave50VCLBEX40.dpk >nul
copy ..\D4\Rave50VCLBEX40.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDD4%\bin\dcc32.exe ..\Source\RpTChart.pas -M -H -W -Q -N..\D4 -LE..\D4 -LN..\D4 -U..\D4 -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD4%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -N..\D4 -LE..\D4 -LN..\D4 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDD4%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\D4 -LE..\D4 -LN..\D4 -L..\D4\Rave50VCLBEX40.dcp -I..\Source -U..\D4;..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\D4\dclRave.dcu >nul
del ..\D4\dclRave.dcp >nul
del ..\D4\Rave50VCLBEX40.dcu >nul
del ..\D4\Rave50VCLBEX40.bpl >nul
del ..\D4\RPExeFrm.dcu >nul
del ..\D4\RaveReg.dcu >nul

echo.
echo SUCCESS: D4 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: D4 Packages NOT Compiled
echo.
pause
:endok