@echo off
REM Standard Paths.
SET FIREBIRD_BIN_DIR=C:\Program Files\Firebird\Firebird_2_5\bin
SET BACKUP_DIR=\\Pos2\MenuMate
REM Old Files and Mothly Backup Day
set RemoveMonthlyFilesOlderThanDaysOnHDD=365
set RemoveMonthlyFilesOlderThanDaysOnStick=32
set RemoveWeeklyFilesOlderThanDays=30

REM Days Backups occur on ( other thanevery day )
set MonthlyBackupDay=30
set WeeklyBackupDay1=7
set WeeklyBackupDay2=14
set WeeklyBackupDay3=21
set WeeklyBackupDay4=28

REM ------ DO NOT TOUCH FROM HERE DOWN -------------

REM Sort out 64 or 32 bit Backup.
SET BASEDIR32=C:\Program Files\MenuMate
SET BASEDIR64=C:\Program Files (x86)\MenuMate
SET BASEDIR=%BASEDIR32%
IF EXIST "%BASEDIR64%" SET BASEDIR=%BASEDIR64%
IF NOT EXIST "%BASEDIR%" GOTO :EndNoBaseDir

REM BackupFile Names. Do NOT Touch.
set MMDailyfile=MMBackup.zip
set MMDailyStockfile=MMStockBackup.zip
set MMDailyMembershipfile=MMStockMasterBackup.zip

echo %MMfile%
for /f "tokens=1-4 delims=/ " %%i in ("%date%") do set Day=%%i
for /f "tokens=1-4 delims=/ " %%i in ("%date%") do set datestring=%%k-%%j-%%i

set MMMonthlyfile=%datestring%_MMMonthlyBackup.zip
set MMMonthlyStockfile=%datestring%_MMMonthlyStockBackup.zip
set MMMonthlyMembershipfile=%datestring%_MMMonthlyStockMasterBackup.zip
set MMWeeklyfile=%datestring%_MMWeeklyBackup.zip
set MMWeeklyStockfile=%datestring%_MMWeeklyStockBackup.zip
set MMWeeklyMembershipfile=%datestring%_MMWeeklyStockMasterBackup.zip

echo .........................
echo MenuMate Dir is %BASEDIR%
echo Backup   Dir is %BACKUP_DIR%
echo Firebird Dir is %FIREBIRD_BIN_DIR%
echo .........................

CALL :Clean
CALL :Backup 
CALL :Zip
CALL :Copy
GOTO :End 

:Clean
echo Cleaning up old FBK files. 
echo Renaming the old set to .Last.fbk
IF EXIST "%BASEDIR%\MenuMate.Last.fbk" DEL "%BASEDIR%\MenuMate.Last.fbk"
IF EXIST "%BASEDIR%\Stock.Last.fbk" DEL "%BASEDIR%\Stock.Last.fbk"
IF EXIST "%BASEDIR%\StockMaster.Last.fbk" DEL "%BASEDIR%\StockMaster.Last.fbk"
echo Copying Current fbk's to .Last.fbk
IF EXIST "%BASEDIR%\MenuMate.fbk" copy /Y "%BASEDIR%\MenuMate.fbk" "%BASEDIR%\MenuMate.Last.fbk"
IF EXIST "%BASEDIR%\Stock.fbk" copy /Y "%BASEDIR%\Stock.fbk" "%BASEDIR%\Stock.Last.fbk"
IF EXIST "%BASEDIR%\StockMaster.fbk" copy /Y "%BASEDIR%\StockMaster.fbk" "%BASEDIR%\StockMaster.Last.fbk"
echo Cleaning up Backup Dir %BACKUP_DIR%
IF NOT EXIST "%BASEDIR%\forfiles.exe" GOTO NO_FORFILES_EXE
IF NOT EXIST "%BACKUP_DIR%" GOTO NO_CLEANBACKUP_DIR
echo Removing Monthly Backups older than %RemoveMonthlyFilesOlderThanDaysOnHDD% from %BASEDIR%
"%BASEDIR%\forfiles.exe" -p%BASEDIR% -m*MMMonthly*.zip -d-%RemoveMonthlyFilesOlderThanDaysOnHDD% -c"cmd /c del @PATH\@FILE"
echo Removing Monthly Backups older than %RemoveMonthlyFilesOlderThanDaysOnStick% from %BACKUP_DIR%
"%BASEDIR%\forfiles.exe" -p%BACKUP_DIR% -m*MMMonthly*.zip -d-%RemoveMonthlyFilesOlderThanDaysOnStick% -c"cmd /c del @PATH\@FILE"
echo Removing Weekly Backups older than %RemoveWeeklyFilesOlderThanDays% from %BASEDIR%
"%BASEDIR%\forfiles.exe" -p%BASEDIR% -m*MMWeekly*.zip -d-%RemoveWeeklyFilesOlderThanDays% -c"cmd /c del @PATH\@FILE"
echo Removing Weekly Backups older than %RemoveWeeklyFilesOlderThanDays% from %BACKUP_DIR%
"%BASEDIR%\forfiles.exe" -p%BACKUP_DIR% -m*MMWeekly*.zip -d-%RemoveWeeklyFilesOlderThanDays% -c"cmd /c del @PATH\@FILE"
GOTO :EOF
:NO_FORFILES_EXE
echo The Program "%BASEDIR%\forfiles.exe" was not found.
echo Skipping removal of Old files from Backup Dir.
echo you can download forfiles.exe from ftp://ftp.microsoft.com/ResKit/y2kfix/x86/
echo However it should be in your MenuMate directory, Have you run the MMUpdate-X-X-X-X.exe?
GOTO :EOF
:NO_CLEANBACKUP_DIR
echo Backup Directory "%BACKUP_DIR%%" not found.
echo Skipping removal of Old files from Backup Dir.
GOTO :EOF

:Backup
IF NOT EXIST "%FIREBIRD_BIN_DIR%\gbak.exe" GOTO NO_GBAK_EXE
echo Backing up. Check backup.log for Details.
"%FIREBIRD_BIN_%\gbak.exe" -b  -ig -v -user "SYSDBA" -pas "masterkey" "localhost:%BASEDIR%\MenuMate.fdb" "%BASEDIR%\MenuMate.fbk" > backup.log 2>&1 || "%FIREBIRD_BIN_DIR%\gbak.exe" -b -ig -v -user "SYSDBA" -pas "masterkey" "localhost:%BASEDIR%\MenuMate.fdb" "%BASEDIR%\MenuMate.fbk" > backup.log 2>&1
"%FIREBIRD_BIN_DIR%\gbak.exe" -b  -ig -v -user "SYSDBA" -pas "masterkey" "localhost:%BASEDIR%\Stock.fdb" "%BASEDIR%\Stock.fbk" >> backup.log 2>&1 || "%FIREBIRD_BIN_DIR%\gbak.exe" -b -ig -v -user "SYSDBA" -pas "masterkey" "localhost:%BASEDIR%\Stock.fdb" "%BASEDIR%\Stock.fbk" >> backup.log 2>&1 
"%FIREBIRD_BIN_DIR%\gbak.exe" -b  -ig -v -user "SYSDBA" -pas "masterkey" "localhost:%BASEDIR%\StockMaster.fdb" "%BASEDIR%\StockMaster.fbk" >> backup.log 2>&1 || "%FIREBIRD_BIN_DIR%\gbak.exe" -b -ig -v -user "SYSDBA" -pas "masterkey" "localhost:%BASEDIR%\StockMaster.fdb" "%BASEDIR%\StockMaster.fbk" >> backup.log 2>&1
GOTO :EOF
:NO_GBAK_EXE
echo Aborting Backup. No gbak.exe here "%FIREBIRD_BIN_DIR%\gbak.exe"
echo Aborting Backup. No gbak.exe here "%FIREBIRD_BIN_DIR%\gbak.exe" >> backup.log
GOTO :End

:Zip
IF NOT EXIST "%BASEDIR%\zip.exe" GOTO NO_ZIP_EXE
echo Zipping up files. Check backup.log for Details.
"%BASEDIR%\zip.exe" -9 "%BASEDIR%\%MMDailyfile%" "%BASEDIR%\MenuMate.fbk" >> backup.log 2>&1
"%BASEDIR%\zip.exe" -9 "%BASEDIR%\%MMDailyStockfile%" "%BASEDIR%\Stock.fbk" >> backup.log 2>&1
"%BASEDIR%\zip.exe" -9 "%BASEDIR%\%MMDailyMembershipfile%" "%BASEDIR%\StockMaster.fbk" >> backup.log 2>&1
GOTO :EOF
:NO_ZIP_EXE
echo Skipping Zipping. No Zip.exe here "%BASEDIR%\zip.exe"
echo Skipping Zipping. No Zip.exe here "%BASEDIR%\zip.exe" >> backup.log
GOTO :EOF

:EndNoBaseDir
echo Unable to Locate MenuMate installation.
echo Aborting Backup the Base Directory %BASEDIR% dose not exist.
echo Aborting Backup the Base Directory %BASEDIR% dose not exist. >> backup.log

:End
echo Finished.
exit


:Copy
IF NOT EXIST "%BACKUP_DIR%" GOTO NO_BACKUP_DIR
echo Copying daily files. Check backup.log for Details.
copy /Y "%BASEDIR%\%MMDailyfile%" "%BACKUP_DIR%\*.*" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyStockfile%" "%BACKUP_DIR%\*.*" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyMembershipfile%" "%BACKUP_DIR%\*.*" >> backup.log 2>&1
IF %Day% == %WeeklyBackupDay1% CALL :CopyToWeeklyTarget
IF %Day% == %WeeklyBackupDay2% CALL :CopyToWeeklyTarget
IF %Day% == %WeeklyBackupDay3% CALL :CopyToWeeklyTarget
IF %Day% == %WeeklyBackupDay4% CALL :CopyToWeeklyTarget
IF %Day% == %MonthlyBackupDay% CALL :CopyToMonthlyTarget
GOTO :EOF
:CopyToWeeklyTarget
echo Copying Weekly files to %BACKUP_DIR%. Check backup.log for Details.
copy /Y "%BASEDIR%\%MMDailyfile%" "%BACKUP_DIR%\%MMWeeklyfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyStockfile%" "%BACKUP_DIR%\%MMWeeklyStockfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyMembershipfile%" "%BACKUP_DIR%\%MMWeeklyMembershipfile%" >> backup.log 2>&1
echo Also Copying Weekly files to %BASEDIR%. Check backup.log for Details.
copy /Y "%BASEDIR%\%MMDailyfile%" "%BASEDIR%\%MMWeeklyfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyStockfile%" "%BASEDIR%\%MMWeeklyStockfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyMembershipfile%" "%BASEDIR%\%MMWeeklyMembershipfile%" >> backup.log 2>&1
GOTO :EOF
:CopyToMonthlyTarget
echo Copying Monthly files to %BACKUP_DIR%. Check backup.log for Details.
copy /Y "%BASEDIR%\%MMDailyfile%" "%BACKUP_DIR%\%MMMonthlyfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyStockfile%" "%BACKUP_DIR%\%MMMonthlyStockfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyMembershipfile%" "%BACKUP_DIR%\%MMMonthlyMembershipfile%" >> backup.log 2>&1
echo Also Copying Monthly files to %BASEDIR%. Check backup.log for Details.
copy /Y "%BASEDIR%\%MMDailyfile%" "%BASEDIR%\%MMMonthlyfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyStockfile%" "%BASEDIR%\%MMMonthlyStockfile%" >> backup.log 2>&1
copy /Y "%BASEDIR%\%MMDailyMembershipfile%" "%BASEDIR%\%MMMonthlyMembershipfile%" >> backup.log 2>&1
GOTO :EOF
:NO_BACKUP_DIR
echo Skipping Backup Copy. Backup Dir dose not exist "%BACKUP_DIR%"
echo Skipping Backup Copy. Backup Dir dose not exist "%BACKUP_DIR%" >> backup.log
GOTO :EOF
