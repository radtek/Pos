::
:: Set variables needed to use C++Builder 6 with Office.
::
call setup_office_environment.bat

::
:: Clear the object files.
::

:: Generate a list of the object files that we need to remove...
svn st %office_obj_path% | sed -ne "s!\?[[:space:]]\+\(.*\)!\1!p" > kill_list

:: Nuke 'em
for /f %%i in (kill_list) do rm -f %%i

:: Clean up
rm -f kill_list

cd %office_src_path%

::
:: No stale makefiles, thanks.
::
rm -f Office.mak

::
:: Convert the project file to a standard UNIX-style Makefile. This
:: allows us to build Office without invoking C++Builder 6's GUI.
::
bpr2mak Office.bpr

::
:: Go!
::
call make -f Office.mak

cd %BASEDIR%

cp "%BASEDIR%\Runtime\Office.exe" "%BASEDIR%\Static Files\."