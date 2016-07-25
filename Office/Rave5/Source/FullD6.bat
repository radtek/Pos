@echo off
if exist setenv.bat call setenv.bat
computil SetupD6
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\D6\*.* md ..\D6 >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
copy ..\Source\RpForm*.dfm ..\D6 >nul
copy ..\Source\QRpForm*.xfm ..\D6 >nul
%NDD6%\bin\dcc32.exe Rave50VCL.dpk -B -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
copy ..\D6\Rave50VCLBEX60.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Runtime Package Rave50CLX
REM ***************************************************
%NDD6%\bin\dcc32.exe Rave50CLX.dpk -B -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
copy ..\D6\Rave50CLXBEX60.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDD6%\bin\dcc32.exe ..\source\RpTChart.pas -M -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD6%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD6%\bin\dcc32.exe ..\Source\RvDLADO.pas -M -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD6%\bin\dcc32.exe ..\Source\RvDLDBX.pas -M -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD6%\bin\dcc32.exe ..\Source\QRvDLDBX.pas -M -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDD6%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\D6 -LE..\D6 -LN..\D6 -L..\D6\Rave50VCL60.dcp -U..\D6;..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\D6\dclRave.dcu >nul
del ..\D6\dclRave.dcp >nul
del ..\D6\Rave50VCL.dcu >nul
del ..\D6\Rave50VCLBEX60.bpl >nul
del ..\D6\Rave50CLX.dcu >nul
del ..\D6\Rave50CLXBEX60.bpl >nul
del ..\D6\RPExeFrm.dcu >nul
del ..\D6\RaveReg.dcu >nul

echo.
echo SUCCESS: D6 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: D6 Packages NOT Compiled
echo.
pause
:endok