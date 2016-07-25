@echo off
if exist setenv.bat call setenv.bat
computil SetupC4
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\C4\*.* md ..\C4 >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
copy Rave50VCL.dpk Rave50VCLBEX40.dpk >nul
%NDC4%\bin\dcc32.exe Rave50VCLBEX40.dpk -B -H -W -Q -JPHN -N..\C4 -LE..\C4 -LN..\C4 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC4%\bin\dcc32.exe Rave50VCLBEX40.dpk -H -W -Q -LE..\C4 -LN..\C4 -N..\C4 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del Rave50VCLBEX40.dpk >nul
copy ..\C4\Rave50VCLBEX40.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDC4%\bin\dcc32.exe ..\Source\RpTChart.pas -M -H -W -Q -JPHN -N..\C4 -LE..\C4 -LN..\C4 -U..\C4 -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC4%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -JPHN -N..\C4 -LE..\C4 -LN..\C4 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Copy Files
REM ***************************************************
REM Fix Borland HPP Header Creation Bug
computil HppModify ..\Source\
copy ..\Source\*.obj ..\C4 >nul
copy ..\Source\*.hpp ..\C4 >nul
copy ..\Source\RpForm*.dfm ..\C4 >nul
copy Rave50VCLBEX40.bpi ..\C4 >nul
copy Rave50VCLBEX40.lsp ..\C4 >nul

REM ***************************************************
REM Create .LIB file
REM ***************************************************
echo Creating Rave50VCLBEX40.LIB file, please wait...
if exist ..\C4\Rave50VCLBEX40.lib del ..\C4\Rave50VCLBEX40.lib >nul
for %%9 in (..\C4\*.obj) do %NDC4%\bin\tlib.exe ..\C4\Rave50VCLBEX40.lib /P32 +%%9 >nul
del ..\C4\Rave50VCLBEX40.bak >nul
rem %NDC4%\bin\tlib.exe ..\C4\Rave50VCLBEX40.lib @..\C4\Rave50VCLBEX40.lsp /P32

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDC4%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\C4 -LE..\C4 -LN..\C4 -L..\C4\Rave50VCLBEX40.dcp -I..\Source -U..\C4;..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\C4\dclRave.dcu >nul
del ..\C4\dclRave.dcp >nul
del ..\C4\Rave50VCLBEX40.dcu >nul
del ..\C4\Rave50VCLBEX40.bpl >nul
del ..\C4\RPExeFrm.dcu >nul
del ..\C4\RaveReg.dcu >nul

del ..\Source\*.obj >nul
del ..\Source\*.hpp >nul
del Rave50VCLBEX40.bpi >nul
del Rave50VCLBEX40.lsp >nul

echo.
echo SUCCESS: C4 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: C4 Packages NOT Compiled
echo.
pause
:endok