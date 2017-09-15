::
::
set sv_version=6.42.0.4
SET TAGLABEL=%sv_version%
SET VERLABEL=%sv_version%
SET MMVERSION=%sv_version%
SET UPDATE=FALSE
REM SET BASEDIR=C:\MM_Cloud_Git\%BASEDIR%
call SetPath.local.bat

::
:: We build on x64 platforms sometime; we need to make sure that the variables
:: we use are standardized. On x64, 32bit programs are stored in ''Program Files (x86)``.
:: So, we'll want to use the ``%ProgramFiles(x86)%''. On x32, we'll set that to the
:: default Program files directory so we can use the same variables in the path
:: expansions below.
::
if "%PROCESSOR_ARCHITECTURE%"=="x86" set ProgramFiles(x86)=%ProgramFiles%

set sv_cbuilder6_path=%ProgramFiles(x86)%\Borland\CBuilder6\bin

::
:: We'll execute 'rsvars.bat' from the RAD Studio directory as that'll bring all of the
:: needed path elements into the environment.
::
set sv_cbuilder2010_path=%ProgramFiles(x86)%\Embarcadero\RAD Studio\7.0\bin

set sv_innosetup_version=5
set sv_innosetup_path=%ProgramFiles(x86)%\Inno Setup %sv_innosetup_version%

::
:: As with RAD Studio above, we'll execute 'vsvars32.bat' to bring all of the MSVC binary
:: paths into the environment.
::
set sv_visual_studio_version=10.0
set sv_visual_studio_path=%ProgramFiles(x86)%\Microsoft Visual Studio %sv_visual_studio_version%\Common7\Tools

::
:: Merge in the RAD Studio variables
::
call "%sv_cbuilder2010_path%\rsvars.bat"

::
:: Merge in the MSVC variables
::
call "%sv_visual_studio_path%\vsvars32.bat"

::
:: Ensure Inno Setup is in the path.
::
set path=%path%;%sv_innosetup_path%

:: Set so that we don't have to alter all of the existing scripts...
set INNODIR=%sv_innosetup_path%

::
:: Note that we don't merge in the C++Builder6 variables. This is because they conflict with the
:: variables of RAD Studio. There's a script specialized for building Office. That script ensures
:: the variables it requires are in place.
::
