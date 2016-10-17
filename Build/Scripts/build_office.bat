:
: We don't want to pollute the general build environment with C++Builder 6's variables.
:
if "%sv_running_in_build_vm%"=="yes" "cmd /C %BASEDIR%\Build\Scripts\build_office_helper.bat"
