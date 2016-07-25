@echo off
if exist setenv.bat call setenv.bat
computil SetupC6
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\C6\*.* md ..\C6 >nul

REM ***************************************************
REM Compile Runtime Package Rave50CLX
REM ***************************************************
%NDC6%\bin\dcc32.exe Rave50CLX.dpk -B -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC6%\bin\dcc32.exe Rave50CLX.dpk -H -W -Q -LE..\C6 -LN..\C6 -N..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
copy ..\C6\Rave50CLXBEX60.bpl %NDWINSYS% >nul

REM ***************************************************
REM Compile Runtime Package Rave50VCL
REM ***************************************************
%NDC6%\bin\dcc32.exe Rave50VCL.dpk -B -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC6%\bin\dcc32.exe Rave50VCL.dpk -H -W -Q -LE..\C6 -LN..\C6 -N..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
copy ..\C6\Rave50VCLBEX60.bpl %NDWINSYS% >nul
copy Rave50CLX.bpi ..\C6 >nul

REM ***************************************************
REM Compile Misc Units
REM ***************************************************
%NDC6%\bin\dcc32.exe ..\Source\RpTChart.pas -M -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\C6 -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC6%\bin\dcc32.exe ..\Source\RvDLBDE.pas -M -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC6%\bin\dcc32.exe ..\Source\RvDLADO.pas -M -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC6%\bin\dcc32.exe ..\Source\RvDLDBX.pas -M -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
%NDC6%\bin\dcc32.exe ..\Source\QRvDLDBX.pas -M -H -W -Q -JPHN -N..\C6 -LE..\C6 -LN..\C6 -U..\Source -I..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

REM ***************************************************
REM Copy Files
REM ***************************************************
REM Fix Borland HPP Header Creation Bug
computil HppModify ..\Source\
copy ..\Source\*.obj ..\C6 >nul
copy ..\Source\*.hpp ..\C6 >nul
copy ..\Source\RpForm*.dfm ..\C6 >nul
copy ..\Source\QRpForm*.xfm ..\C6 >nul
copy Rave50VCL.bpi ..\C6 >nul
copy Rave50VCLBEX60.lsp ..\C6 >nul

REM ***************************************************
REM Create .LIB file for VCL
REM ***************************************************
echo Creating Rave50VCL.LIB file, please wait...
if exist ..\C6\Rave50VCL.lib del ..\C6\Rave50VCLlib >nul
for %%9 in (..\C6\R*.obj) do %NDC6%\bin\tlib.exe ..\C6\Rave50VCL.lib /P32 +%%9 >nul
del ..\C6\Rave50VCL.bak >nul
rem %NDC6%\bin\tlib.exe ..\C6\Rave50VCL.lib @..\C6\Rave50VCL.lsp /P32

REM ***************************************************
REM Create .LIB file for CLX
REM ***************************************************
echo Creating Rave50CLX.LIB file, please wait...
if exist ..\C6\Rave50CLX.lib del ..\C6\Rave50CLX.lib >nul
for %%9 in (..\C6\Q*.obj) do %NDC6%\bin\tlib.exe ..\C6\Rave50CLX.lib /P32 +%%9 >nul
del ..\C6\Rave50CLX.bak >nul
rem %NDC6%\bin\tlib.exe ..\C6\Rave50CLX.lib @..\C6\Rave50CLX.lsp /P32

REM ***************************************************
REM Compile Design-time Package dclRave
REM ***************************************************
%NDC6%\bin\dcc32.exe dclRave.dpk -B -H -W -Q -N..\C6 -LE..\C6 -LN..\C6 -L..\C6\Rave50VCLBEX60.dcp -I..\Source -U..\C6;..\Source -DBCB -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror

copy ..\C6\*.bpl ..\..\Libraries\.
copy ..\C6\*.lib ..\..\Libraries\.
copy ..\C6\*.bpi ..\..\Libraries\.

REM ***************************************************
REM Clean-up
REM ***************************************************
del ..\C6\dclRave.dcu
del ..\C6\dclRave.dcp
del ..\C6\Rave50VCLBEX60.bpl
del ..\C6\Rave50CLXBEX60.bpl
del ..\C6\RPExeFrm.dcu
del ..\C6\RaveReg.dcu

del ..\Source\*.obj
rem del ..\Source\*.hpp
del Rave50VCL.lsp
del Rave50CLX.lsp

echo.
echo SUCCESS: C6 Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: C6 Packages NOT Compiled
echo.
pause
:endok