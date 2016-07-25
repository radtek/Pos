@ECHO OFF

call "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Bin\svcutil.exe" "http://localhost:8733/MenumateServices/EFTPOS/?wsdl" >> CreateEFTPOSClientCode.LOG

IF NOT EXIST SvcEFTPOSClientCode ( mkdir SvcEFTPOSClientCode ) ELSE ( del /Q SvcEFTPOSClientCode\*.* ) >> CreateEFTPOSClientCode.LOG

move output.config SvcEFTPOSClientCode\App.config >> CreateEFTPOSClientCode.LOG
move WCFServiceEFTPOS.cs SvcEFTPOSClientCode\WCFServiceEFTPOSClient.cs >> CreateEFTPOSClientCode.LOG

ECHO WCFServiceEFTPOS Client has been succesfully created (WCFServiceEFTPOSClient .cs and app.config)
