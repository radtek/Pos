set ServiceURL=%1

IF [%ServiceURL%]==[] GOTO DEFAULT_URL

echo. ----------------------------------
echo == Binding a Web Service Client from URL: %ServiceURL% ...
echo. ----------------------------------

GOTO START

:DEFAULT_URL
set ServiceURL="http://localhost:8733/MenumateServices/EFTPOS/?wsdl"

:START
echo. ----------------------------------
echo == Binding a Web Service Client from the default URL: %ServiceURL% ...
echo. ----------------------------------

"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Bin\NETFX 4.0 Tools\x64\svcutil.exe" %ServiceURL%

