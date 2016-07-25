::
:: Setup the base environment.
::
call MachineEnviroment.bat

::
:: Setup the variables C++Builder needs to build Office.
::
call Scripts\setup_office_environment.bat

::
:: Launch C++Builder 6.
::
start bcb