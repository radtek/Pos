@echo off
if exist setenv.bat call setenv.bat
computil SetupD7
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\D7\*.* md ..\D7 >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
copy ..\Source\RpForm*.dfm ..\D7 >nul
copy ..\Source\QRpForm*.xfm ..\D7 >nul
%NDD7%\bin\dcc32.exe Rave50VCL.dpk -B -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
copy ..\D7\Rave50VCLBEX70.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Runtime Package Rave50CLX
REM ***************************************************
%NDD7%\bin\dcc32.exe Rave50CLX.dpk -B -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
copy ..\D7\Rave50CLXBEX70.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDD7%\bin\dcc32.exe ..\Source\RpTChart.pas -M -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD7%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD7%\bin\dcc32.exe ..\Source\RvDLADO.pas -M -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD7%\bin\dcc32.exe ..\Source\RvDLDBX.pas -M -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDD7%\bin\dcc32.exe ..\Source\QRvDLDBX.pas -M -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -U..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDD7%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\D7 -LE..\D7 -LN..\D7 -L..\D7\Rave50VCL70.dcp -U..\D7;..\Source -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\D7\dclRave.dcu >nul
del ..\D7\dclRave.dcp >nul
del ..\D7\Rave50VCL.dcu >nul
del ..\D7\Rave50VCLBEX70.bpl >nul
del ..\D7\Rave50CLX.dcu >nul
del ..\D7\Rave50CLXBEX70.bpl >nul
del ..\D7\RPExeFrm.dcu >nul
del ..\D7\RaveReg.dcu >nul

echo.
echo SUCCESS: D7 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: D7 Packages NOT Compiled
echo.
pause
:endok