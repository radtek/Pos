set office_path=%BASEDIR%\Office
set office_obj_path=%office_path%\Obj
set office_lib_path=%office_path%\Libraries
set office_src_path=%office_path%\Source

::
:: We've got have the library path included in the system path
:: otherwise Borland doesn't always locate and load the necessary
:: packages.
::
set path=%path%;%sv_cbuilder6_path%;%office_lib_path%
