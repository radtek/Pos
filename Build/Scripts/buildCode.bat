*-@echo off
@echo
@echo
@echo ----- Building Project Group ------
@echo

cd  %BASEDIR%\
MSBuild Project-MenuMate.groupproj /t:Clean
del /S /Q VersionRes.res

MSBuild Project-MenuMate.groupproj /t:Build /p:config=Release;nowarn=8057

:
: Build Office.
:
call "%BASEDIR%\Build\Scripts\build_office.bat"

echo ----- Building SVU -----
cd "%BASEDIR%\Svu"
call devenv.com /build Release "%BASEDIR%\Svu\Svu.sln" > Svu.mak.log 2>&1

echo ----- Building FileMate ------
cd "%BASEDIR%\FileMate"
call devenv.com /build Release %BASEDIR%\FileMate\FileMate.sln > FileMate.mak.log 2>&1

echo ----- Building IntaMate ------
cd "%BASEDIR%\MenuMate\IntaMate"
call devenv.com /build Release %BASEDIR%\MenuMate\IntaMate\IntaMate.sln > IntaMate.mak.log 2>&1

echo ----- Building Summa Pro ------
cd "%BASEDIR%\MenuMate\Summa Pro"
call %BASEDIR%\Runtime\Svu.exe %BASEDIR%\Menumate\Version\VersionRes.h "%BASEDIR%\MenuMate\Summa Pro\Summa\MainForm.cs"
call devenv.com /build Release "%BASEDIR%\MenuMate\Summa Pro\Summa\Summa Pro.sln" > Summa.mak.log 2>&1
call move /y "%BASEDIR%\MenuMate\Summa Pro\Summa\MainForm.cs.orig" "%BASEDIR%\MenuMate\Summa Pro\Summa\MainForm.cs"

echo ----- Building Floorplan ------
call %BASEDIR%\Runtime\Svu.exe %BASEDIR%\Menumate\Version\VersionRes.h "%BASEDIR%\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\EditorForm.cs"
call devenv.com /build Release "%BASEDIR%\Menumate\Services\Menumate.Services\Menumate.Services.sln" > %BASEDIR%\Menumate\Services\Menumate.Services\MMServices.mak.log 2>&1
call move /y "%BASEDIR%\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\EditorForm.cs.orig" "%BASEDIR%\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\EditorForm.cs"

echo ----- Building LoyaltyMate dll ------
call devenv.com /build Release "%BASEDIR%\Menumate\Services\Loyalty_mate\Loyalty_mate.sln" > %BASEDIR%\Menumate\Services\Loyalty_mate\Loyalty_mate.mak.log 2>&1

: echo ----- Building Menumate Services Config App ------
: call "%BASEDIR%\Build\Scripts\build_menumate_services_config.bat"

echo ----- Building Menumate Services: ServiceApp.exe ------
echo ----- Services: Loyaltymate, Chefmate, Webmate, Xero Integration ------
call %BASEDIR%\Runtime\Svu.exe %BASEDIR%\Menumate\Version\VersionRes.h "%BASEDIR%\MenuMate\Services\ServicesApp\XeroConfig\MainForm.cs"
call %BASEDIR%\Runtime\Svu.exe %BASEDIR%\Menumate\Version\VersionRes.h "%BASEDIR%\MenuMate\Services\ServicesApp\FloorPlanEditor\MainForm.cs"
call %BASEDIR%\Runtime\Svu.exe %BASEDIR%\Menumate\Version\VersionRes.h "%BASEDIR%\MenuMate\Services\ServicesApp\MenumateServices Config\MainForm.cs"
call devenv.com /build Release "%BASEDIR%\MenuMate\Services\ServicesApp\ServicesApp.sln" > %BASEDIR%\MenuMate\Services\ServicesApp\ServicesApp.mak.log 2>&1
call move /y "%BASEDIR%\MenuMate\Services\ServicesApp\XeroConfig\MainForm.cs.orig" "%BASEDIR%\MenuMate\Services\ServicesApp\XeroConfig\MainForm.cs"
call move /y "%BASEDIR%\MenuMate\Services\ServicesApp\FloorPlanEditor\MainForm.cs.orig" "%BASEDIR%\MenuMate\Services\ServicesApp\FloorPlanEditor\MainForm.cs"
call move /y "%BASEDIR%\MenuMate\Services\ServicesApp\MenumateServices Config\MainForm.cs.orig" "%BASEDIR%\MenuMate\Services\ServicesApp\MenumateServices Config\MainForm.cs"

echo ----- Building ChefMate ------
call "%BASEDIR%\Build\Scripts\build_chefmate.bat"

echo ----- Building Customer Display ------
call "%BASEDIR%\Build\Scripts\build_customer_display.bat"

echo ----- Build Complete -----
cd %BASEDIR%
