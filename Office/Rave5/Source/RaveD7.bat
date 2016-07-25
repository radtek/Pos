@echo off
if exist setenv.bat call setenv.bat
computil SetupD7
if exist setenv.bat call setenv.bat
call clean.bat
if not exist ..\RvLib\*.* md ..\RvLib >nul

REM ***************************************************
REM Compile Rave Packages
REM ***************************************************
del ..\Editors\*.dcu
copy ..\RVCS\Rv*.res ..\D7 >nul
%NDD7%\bin\dcc32.exe ..\RVCL\RvCore.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvCore.dcu

%NDD7%\bin\dcc32.exe ..\RVCL\RvBarcode.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvBarcode.dcu

%NDD7%\bin\dcc32.exe ..\RVCL\RvGraphics.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvGraphics.dcu

%NDD7%\bin\dcc32.exe ..\RVCL\RvStandard.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvStandard.dcu

%NDD7%\bin\dcc32.exe ..\RVCL\RvReport.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvReport.dcu

..\RVS\RVS2INC ..\RVS\SYSFUNC.RVS ..\RVS\Rv*.rvs >nul
%NDD7%\bin\dcc32.exe ..\RVCL\RvEngine.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvEngine.dcu

%NDD7%\bin\dcc32.exe ..\RVCL\RvCompiler.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvCompiler.dcu

%NDD7%\bin\dcc32.exe ..\RVCL\RvDesign.dpk -M -H -W -Q -N..\D7 -LE.. -LN..\D7 -U..\D7;..\Editors;..\RVCL -I..\Source -$D-,L- -V- %1 %2 %3
if errorlevel 1 goto enderror
del ..\D7\RvDesign.dcu

echo.
echo SUCCESS: Rave Packages Compiled
echo.
goto endok
:enderror
call clean
echo.
echo FAILURE: Rave Packages NOT Compiled
echo.
pause
:endok