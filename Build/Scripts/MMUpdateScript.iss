; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
PrivilegesRequired=admin

AppName=MenuMate Update
AppVerName=MenuMate Update {#GetEnv('MMVERSION')}
AppPublisher=MenuMate Ltd
AppPublisherURL=http://www.menumate.com
AppSupportURL=http://www.iqworks.co.nz
AppUpdatesURL=ftp://ftp.menumate.com
DefaultDirName={pf}\MenuMate
DisableDirPage=true
DefaultGroupName=MenuMate
DisableProgramGroupPage=true
OutputDir={#GetEnv('BASEDIR')}\Installers
OutputBaseFilename=MMUpdate ALL {#GetEnv('MMVERSION')}
AppCopyright=IQWorks Ltd
DirExistsWarning=no
AppVersion={#GetEnv('MMVERSION')}
UninstallDisplayIcon={app}\MenuMate.exe

; uncomment the following line if you want your installation to run on NT 3.51 too.
; MinVersion=4,3.51
InternalCompressLevel=ultra
;VersionInfoVersion={#GetEnv('MMVER')}
VersionInfoCompany=IQWorks Ltd
VersionInfoDescription=MenuMate P.O.S System
ShowLanguageDialog=yes
WizardImageFile={#GetEnv('BASEDIR')}\Build\Tungsten.BMP
DisableFinishedPage=true
AlwaysShowComponentsList=false
ShowComponentSizes=false
FlatComponentsList=false
DisableReadyPage=true

[Tasks]

#include "chefmate_script.iss"

[Files]
; backup some importtant files.
;Source: {app}\PalmMateIR.exe; DestDir: {app}\backup\Pre3.0; Flags: external skipifsourcedoesntexist uninsneveruninstall; CopyMode: alwaysoverwrite

; Docs
Source: {#GetEnv('BASEDIR')}\Static Files\Documentation\LOYALTY - 6 Steps to Menumate Loyalty.pdf; DestDir: {app}\Documentation; BeforeInstall: XeroIntegration_BeforeInstall;
Source: {#GetEnv('BASEDIR')}\Static Files\Documentation\MYOB - Premier Integration (Australian Version).pdf; DestDir: {app}\Documentation; BeforeInstall: FloorPlan_BeforeInstall;
Source: {#GetEnv('BASEDIR')}\Static Files\Documentation\POS101.pdf; DestDir: {app}\Documentation
Source: {#GetEnv('BASEDIR')}\Static Files\Documentation\POS201.pdf; DestDir: {app}\Documentation
Source: {#GetEnv('BASEDIR')}\Static Files\Documentation\STOCK - 10 Steps to Total Stock Control.pdf; DestDir: {app}\Documentation
Source: {#GetEnv('BASEDIR')}\Static Files\Documentation\v5.5 PosDroid Training Book.pdf; DestDir: {app}\Documentation


; Server Files
Source: {#GetEnv('BASEDIR')}\Runtime\IBTools.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Runtime\Parser.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\Repair.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\Archive.exe; DestDir: {app}; Flags: ignoreversion

; MenuMate Files
;Source: {#GetEnv('BASEDIR')}\Runtime\DeskMate.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\DeskMate.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Runtime\MenuMate.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Runtime\Kiosk.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\Submitfile.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Runtime\ChefMate.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\Office.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\ZSummaryReports.txt; DestDir: {app}; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\Owner Details.txt; DestDir: {app}; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Runtime\JoinMate.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Runtime\DBTrim.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\help\DB_Trim\DB_Trim_Help_Index.html; DestDir: {app}\help\DB_Trim\; Flags: ignoreversion

Source: {#GetEnv('BASEDIR')}\Static Files\GSTMate.exe; DestDir: {app}; Flags: ignoreversion

Source: {#GetEnv('BASEDIR')}\Static Files\Printers.dat; DestDir: {app}; Flags: ignoreversion

;Backup Files.
Source: {#GetEnv('BASEDIR')}\Static Files\Backup.cmd; DestDir: {app}; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\Backup.orig; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\BackupMakeTask.bat; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\sed.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\forfiles.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\zip.exe; DestDir: {app}; Flags: ignoreversion

Source: {#GetEnv('BASEDIR')}\Static Files\Palm\MenuMate Bluetooth.prc; DestDir: {app}\Palm; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\CASMCARD.DLL; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\BORLNDMM.DLL; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\CC32100MT.DLL; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\windowsxp-kb4019276-x86-embedded-enu_3822fc1692076429a7dc051b00213d5e1240ce3d.exe; DestDir: {app}; Flags: ignoreversion
 Source: {#GetEnv('BASEDIR')}\Static Files\Microsoft.PointOfService.dll; DestDir: {app}; Flags: ignoreversion
 Source: {#GetEnv('BASEDIR')}\Static Files\FiscalIntegration.dll; DestDir: {app}; Flags: ignoreversion
 Source: {#GetEnv('BASEDIR')}\Static Files\FiscalIntegration.tlb; DestDir: {app}; Flags: ignoreversion
 Source: {#GetEnv('BASEDIR')}\Static Files\FiscalIntegration.pdb; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\FirebirdSql.Data.FirebirdClient.dll; DestDir: {app}; Flags: ignoreversion 
Source: {#GetEnv('BASEDIR')}\Static Files\dac140.bpl; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\unidac140.bpl; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\msprovider140.bpl; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\tdsprovider140.bpl; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\porttalk.sys; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\help\vncviewer.exe; DestDir: {app}\help; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\help\winvnc.exe; DestDir: {app}\help; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\help\zipdll.dll; DestDir: {app}\help; Flags: ignoreversion

Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\index.html; DestDir: {app}\kiosk; Flags: ignoreversion 
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\logo.jpg; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\docket.txt; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\DocketMember.txt; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\DocketBirthday.txt; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\DocketDraw.txt; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\DocketVisit.txt; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\printer.gif; DestDir: {app}\kiosk; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\kiosk\indexFinancial.html; DestDir: {app}\kiosk; Flags: ignoreversion 
Source: {#GetEnv('BASEDIR')}\Static Files\OrderLogo.bmp; DestDir: {app}; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\OracleTCPServer.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\OnlineOrderingApp.exe; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\OnlineOrderingApp.exe.config; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\Microsoft.AspNet.SignalR.Client.dll; DestDir: {app}; Flags: ignoreversion
Source: {#GetEnv('BASEDIR')}\Static Files\NewtonSoft.Json.dll; DestDir: {app}; Flags: ignoreversion;

;OpenSSL for Email - MM POS
Source: {#GetEnv('BASEDIR')}\Static Files\email\openssl-0.9.8h-1-setup.exe; DestDir: {app}\email; Flags: ignoreversion onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\Static Files\email\READ-ME.txt; DestDir: {app}\email; Flags: ignoreversion onlyifdoesntexist

; Summa Files
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\ConfigFileManager.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\FileMate.Core.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\FileMate.Core.Xml.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Assemblies\3rdParty\Ionic\32\Ionic.Zip.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\LogFileManager.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Assemblies\3rdParty\Nlog\32\NLog.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\NLog.config"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\Resources\go snow80 tangerine80.png"; DestDir: "{app}\Summa Pro\Resources"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\Resources\pause snow80 tangerine80.png"; DestDir: "{app}\Summa Pro\Resources"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\Summa.exe"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\SummaCL.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion
Source: "{#GetEnv('BASEDIR')}\Runtime\Summa Pro\XMLManager.dll"; DestDir: "{app}\Summa Pro"; Flags: ignoreversion

;GSTMate Help.
Source: ..\..\Static Files\HTML\blank.gif; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\bullet_topic.gif; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\bullet_topic_opened.gif; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\bullet_window.gif; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\bullet_window_opened.gif; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\contextmenu.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\de_style.css; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_GST Upgrade for New Zealand_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_GST Upgrade for New Zealand_image_1.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_Introduction_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_3_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_1_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_1_image_1.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_1_image_2.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_2_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_2_image_1.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_2_image_2.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_2_image_3.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_3_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_3_image_1.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_3_image_2.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_4_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_4_image_1.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_4_image_2.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_4_image_3.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_5_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_5_image_1.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_4_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_module_5_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\drex_When to Run GSTMate_image_0.png; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\GST Upgrade for New Zealand.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\hidemenu.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\index.xml; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\Introduction.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\javascript_browser_detection.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\lzw.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_3.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_4.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_4_1.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_4_2.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_4_3.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_4_4.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_4_5.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\module_5.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\resize.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\search.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\style.css; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\tabber.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\tabberoptions.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\Tooltips.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\When to Run GSTMate.htm; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\xmlengine.js; DestDir: {app}\Html
Source: ..\..\Static Files\HTML\de_search\0xu004c.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu004d.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu004e.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu004f.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu005a.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0030.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0031.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0032.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0033.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0036.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0041.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0042.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0043.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0044.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0045.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0046.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0047.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0048.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0049.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0050.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0051.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0052.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0053.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0054.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0055.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0056.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0057.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\0xu0059.txt; DestDir: {app}\Html\de_search
Source: ..\..\Static Files\HTML\de_search\ids.txt; DestDir: {app}\Html\de_search;
Source: ..\..\Static Files\HTML\de_search\prefixes.txt; DestDir: {app}\Html\de_search;

Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.Winservices.Client.Console\app.config; DestDir: {app}\Floorplan\ConsoleClient ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.Winservices.Client.Console\bin\Release\Menumate.Winservices.Client.Console.exe ; DestDir: {app}\Floorplan\ConsoleClient ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.Winservices.Client.Console\bin\Release\Menumate.Winservices.Client.Console.pdb ; DestDir: {app}\Floorplan\ConsoleClient ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.Winservices.Client.Console\bin\Release\Menumate.Winservices.Client.Console.exe.config; DestDir: {app}\Floorplan\ConsoleClient ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\Config\NLog.config; DestDir: {app}\Floorplan\ConsoleClient ; Flags:ignoreversion;

;Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\app.config ; DestDir: {app}\Floorplan\GuiClient ; Flags:ignoreversion;
;Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\bin\Release\Menumate.WinServices.Client.FloorPlan.exe.config ; DestDir: {app}\Floorplan\GuiClient ; Flags:ignoreversion;
;Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\bin\Release\Menumate.WinServices.Client.FloorPlan.exe ; DestDir: {app}\Floorplan\GuiClient ; Flags:ignoreversion;
;Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\bin\Release\Menumate.WinServices.Client.FloorPlan.pdb ; DestDir: {app}\Floorplan\GuiClient ; Flags:ignoreversion;
;Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\client\Menumate.WinServices.Client.FloorPlan\bin\Release\NLog.dll ; DestDir: {app}\Floorplan\GuiClient ; Flags:ignoreversion;
;Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\Config\NLog.config; DestDir: {app}\Floorplan\GuiClient ; Flags:ignoreversion;

; FloorPlan Service
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Menumate.WinServices.Contracts.pdb ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Menumate.WinServices.pdb ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Antlr3.Runtime.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Castle.ActiveRecord.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Castle.Components.Validator.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Castle.Core.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Castle.DynamicProxy2.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\FirebirdSql.Data.FirebirdClient.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Iesi.Collections.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\log4net.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Lucene.Net.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Menumate.WinServices.Contracts.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\NHibernate.ByteCode.Castle.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\NHibernate.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\NHibernate.Search.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\NLog.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\StringProcessing.dll ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\Config\NLog.config; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\Reservations.fdb; DestDir: {app}\Floorplan\Service; Flags: onlyifdoesntexist
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\Config\MMWebServicesConfig.xml; DestDir: {app}\Floorplan\Service\Config ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Menumate.WinServices.exe.config ; DestDir: {app}\Floorplan\Service ; Flags:ignoreversion;
Source: {#GetEnv('BASEDIR')}\MenuMate\Services\Menumate.Services\bin\Release\Menumate.WinServices.exe ; DestDir: {app}\Floorplan\Service ; Flags: ignoreversion;
Source: {#GetEnv('BASEDIR')}\Static Files\Floorplan Backgrounds\*; DestDir: "{app}\Floorplan\Demo Images"; 

; FloorPlan Editor
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\FloorPlanClient.dll";        DestDir: "{app}\Floorplan\GuiClient" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\FloorPlanEditor.exe";        DestDir: "{app}\Floorplan\GuiClient" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\FloorPlanEditor.exe.config"; DestDir: "{app}\Floorplan\GuiClient" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\XMLManager.dll";             DestDir: "{app}\Floorplan\GuiClient" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\Texts.xml";                  DestDir: "{app}\Floorplan\GuiClient" ; Flags:ignoreversion;

; Menumate Services Application's Configuration: Webmate and Chefmate databases
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\MenumateServices Config.exe";    DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\Menumate Services Utilities.dll"; DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\XMLManager.dll";                 DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\WinOSInfo.dll";                  DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NLog.xml";                       DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NLog.dll";                       DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NLog.config";                    DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;

; Menumate Services Application: Webmate, Chefmate, Loyaltymate and Accounting Integration
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\XMLManager.dll";              DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\Xero.Api.dll";                 DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\WinOSInfo.dll";               DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\publickey.cer";               DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\public_privatekey.pfx";       DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NLog.xml";                    DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NLog.dll";                    DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NLog.config";                 DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\AccountingConfig.exe";              DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\Loyaltymate.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\SplitButton.dll";             DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\FirebirdSql.Data.FirebirdClient.dll"; DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\MenumateUtils.dll";           DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\MenumateServices.exe";        DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\AccountingIntegration.dll";         DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\MenumateServices.exe.config"; DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; AfterInstall: FloorPlan_AfterInstall;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\LoyaltyMateCredentials.info";  DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; AfterInstall: XeroIntegration_AfterInstall;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.IO.dll";               DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.Net.Http.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.Net.Http.Extensions.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.Net.Http.Primitives.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.Net.Http.WebRequest.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.RunTime.dll";                    DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\System.Threading.Tasks.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\NewtonSoft.Json.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\Smartpay.Eftpos.PaymentProvider.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion; 
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\SalesForceCommunication.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\PocketVoucherIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\MYOB.AccountRight.SDK.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\SiHotIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\AdyenIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\WalletPaymets.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\SmartConnectIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\PaymentSenseIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\MewsIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\AustriaFiscalIntegration.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\fiskaltrust.ifPOS.net40.dll";             DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\fiskaltrust.ifPOS.Utilities.net40.dll";             DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\RegistrationIntegration.dll";             DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Menumate\Services\ServicesApp\Ready for Distribution\OnlineOrdering.dll";            DestDir: "{app}\Menumate Services\"; Flags: ignoreversion;
; Menumate Customer Display
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\Menumate Second Display.exe";   DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\MMChangingImageController.dll"; DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\MMChangingImageManager.dll";    DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\MMExtensions.dll";              DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\MMOrderItemsController.dll";    DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\OrderItemsManager.dll";         DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\SharedMemory.dll";              DestDir: "{app}\Customer Display" ; Flags:ignoreversion;
Source: "{#GetEnv('BASEDIR')}\Second Display\Client\Menumate Second Display\Ready for Distribution\XMLManager.dll";                DestDir: "{app}\Customer Display" ; Flags:ignoreversion;

[Registry]
Root: HKLM; Subkey: Software\IQWORKS\MenuMate; Flags: createvalueifdoesntexist uninsdeletekeyifempty
Root: HKLM; Subkey: Software\IQWORKS\MenuMate\; Flags: createvalueifdoesntexist; ValueType: string; ValueName: InstallDir; ValueData: {app}
                                
[Icons]
Name: {group}\Office; Filename: {app}\Office.exe; WorkingDir: {app}; IconIndex: 0; Flags: createonlyiffileexists
Name: {userdesktop}\Office; Filename: {app}\Office.exe; WorkingDir: {app}; IconIndex: 0; Flags: createonlyiffileexists
Name: {group}\Floorplan Editor; Filename: {app}\Floorplan\GuiClient\Menumate.WinServices.Client.FloorPlan.exe; WorkingDir: {app}; IconIndex: 0; Flags: createonlyiffileexists

[InstallDelete]
Type: files; Name: {app}\Documentation\*;
Type: files; Name: {app}\ChefmateV2\*;
Type: files; Name:  {app}\Menumate Services\AccountingConfig.exe;
Type: files; Name:  {app}\Menumate Services\XeroIntegration.xml

[Types]

[Components]
; This script was first created by ISTool
; http://www.lerstad.com/istool/

[Dirs]
Name: "{app}\Floorplan\Demo Images"
Name: {app}\Properties
Name: {app}\Html
Name: {app}\Html\de_search
Name: "{app}\Summa Pro"
Name: "{app}\Summa Pro\Resources"

[Messages]
WelcomeLabel2=This will install [name/ver] on your computer.%n%nYou may need to run this 'MenuMateSetup.exe' file more than once on each terminal. Run this Setup file once for each MenuMate module you wish to install e.g Server, POS, Office.%n%nFirst you need to install/upgrade the Server module on the machine which is to be your Interbase Server. This machine can be the same machine as a Point of Sale terminal.%n%nIt is strongly recommended that you close all other applications you have running before continuing. This will help prevent any conflicts during the installation process.
WizardLicense=Installer Notes
LicenseLabel=Please read the following critical information before upgrading.
LicenseLabel3=You must accept the responsibility for these site changes before continuing.
LicenseAccepted=I &accept
LicenseNotAccepted=I &do not accept

[Code]
procedure FloorPlan_BeforeInstall();
var
  ServicePath: String;
  ServiceResultCode: Integer;
begin
  ServicePath := ExpandConstant('{app}')
                 + '\\Floorplan\\Service\\Menumate.WinServices.exe';

  if FileExists(ServicePath) then
  begin
    Exec(ServicePath, '--stop', '', SW_HIDE,
         ewWaitUntilTerminated, ServiceResultCode);
    Exec(ServicePath, '--remove', '', SW_HIDE,
         ewWaitUntilTerminated, ServiceResultCode);
  end;
end;

procedure FloorPlan_AfterInstall();
var
  ResultCode: Integer;
  ServicePath: String;
begin
  ServicePath := ExpandConstant('{app}')
	         + '\\Floorplan\\Service\\Menumate.WinServices.exe';

  Exec(ServicePath, '--install', '', SW_HIDE,
       ewWaitUntilTerminated, ResultCode);
  Exec(ServicePath, '--start', '', SW_HIDE,
       ewWaitUntilTerminated, ResultCode);
end;

function InitializeUninstall(): Boolean;
var
  ResultCode: Integer;
begin
  Exec(ExpandConstant('{app}') + '\Floorplan\Service\Menumate.WinServices.exe',
                      '--remove', '', SW_SHOW,
                      ewWaitUntilTerminated, ResultCode);
  Result := true;
end;

procedure XeroIntegration_BeforeInstall();
var
  ServicePath : String;
  ServiceResultCode: Integer;
begin
  ServicePath := ExpandConstant('{app}')
                 + '\Menumate Services\MenumateServices.exe';

  if FileExists(ServicePath) then
  begin
    Exec(ServicePath, '--stop',   '', SW_HIDE, ewWaitUntilTerminated, ServiceResultCode);
    Exec(ServicePath, '--remove', '', SW_HIDE, ewWaitUntilTerminated, ServiceResultCode);
  end;
end;

procedure XeroIntegration_AfterInstall();
var
  ServicePath : String;
  ServiceResultCode: Integer;
begin
  ServicePath := ExpandConstant('{app}')+ '\Menumate Services\MenumateServices.exe';

  Exec(ServicePath, '--install', '', SW_HIDE, ewWaitUntilTerminated, ServiceResultCode);
  Exec(ServicePath, '--start',   '', SW_HIDE, ewWaitUntilTerminated, ServiceResultCode);
end;



