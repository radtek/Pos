call Enviro.bat
echo ---- Compiling Doxygen ----
if not exist %BASEDIR%\doxygen md %BASEDIR%\doxygen
cd %BASEDIR%
call %DOXYGENDIR%\doxygen "MenuMate Doxygen"
call ftp -n -i -s:%BASEDIR%\FTPDoxygen.txt