@echo off
if exist setenv.bat call setenv.bat
computil SetupC5
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\C5\*.* md ..\C5 >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
copy Rave50VCL.dpk Rave50VCLBEX50.dpk >nul
%NDC5%\bin\dcc32.exe Rave50VCLBEX50.dpk -B -H -W -Q -JPHN -N..\C5 -LE..\C5 -LN..\C5 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC5%\bin\dcc32.exe Rave50VCLBEX50.dpk -H -W -Q -LE..\C5 -LN..\C5 -N..\C5 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del Rave50VCLBEX50.dpk >nul
copy ..\C5\Rave50VCLBEX50.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDC5%\bin\dcc32.exe ..\Source\RpTChart.pas -M -H -W -Q -JPHN -N..\C5 -LE..\C5 -LN..\C5 -U..\C5 -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC5%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -JPHN -N..\C5 -LE..\C5 -LN..\C5 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC5%\bin\dcc32.exe ..\Source\RvDLADO.pas -M -H -W -Q -JPHN -N..\C5 -LE..\C5 -LN..\C5 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Copy Files
REM ***************************************************
REM Fix Borland HPP Header Creation Bug
computil HppModify ..\Source\
copy ..\Source\*.obj ..\C5 >nul
copy ..\Source\*.hpp ..\C5 >nul
copy ..\Source\RpForm*.dfm ..\C5 >nul
copy Rave50VCLBEX50.bpi ..\C5 >nul
copy Rave50VCLBEX50.lsp ..\C5 >nul

REM ***************************************************
REM Create .LIB file
REM ***************************************************
echo Creating Rave50VCLBEX50.LIB file, please wait...
if exist ..\C5\Rave50VCLBEX50.lib del ..\C5\Rave50VCLBEX50.lib >nul
for %%9 in (..\C5\*.obj) do %NDC5%\bin\tlib.exe ..\C5\Rave50VCLBEX50.lib /P32 +%%9 >nul
del ..\C5\Rave50VCLBEX50.bak >nul
rem %NDC5%\bin\tlib.exe ..\C5\Rave50VCLBEX50.lib @..\C5\Rave50VCLBEX50.lsp /P32

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDC5%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\C5 -LE..\C5 -LN..\C5 -L..\C5\Rave50VCLBEX50.dcp -I..\Source -U..\C5;..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\C5\dclRave.dcu >nul
del ..\C5\dclRave.dcp >nul
del ..\C5\Rave50VCLBEX50.dcu >nul
del ..\C5\Rave50VCLBEX50.bpl >nul
del ..\C5\RPExeFrm.dcu >nul
del ..\C5\RaveReg.dcu >nul

del ..\Source\*.obj >nul
del ..\Source\*.hpp >nul
del Rave50VCLBEX50.bpi >nul
del Rave50VCLBEX50.lsp >nul

echo.
echo SUCCESS: C5 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: C5 Packages NOT Compiled
echo.
pause
:endok