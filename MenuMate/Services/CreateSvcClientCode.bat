@ECHO OFF

call "C:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\svcutil.exe" http://localhost:8732/MenumateWinServices/service?wsdl >> CreateSvcClientCode.LOG
IF NOT EXIST SvcPOSIntegrationClientCode ( mkdir SvcPOSIntegrationClientCode ) ELSE ( del SvcPOSIntegrationClientCode\*.* ) >> CreateSvcClientCode.LOG
move output.config SvcPOSIntegrationClientCode\app.config >> CreateSvcClientCode.LOG
move PosIntegrationService.cs SvcPOSIntegrationClientCode\PosIntegrationService.cs >> CreateSvcClientCode.LOG

ECHO PosIntegration Client has been succesfully created (PosIntegrationService.cs and app.config)

@ECHO OFF

call "C:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\svcutil.exe" http://localhost:8000/MenumateWinServices/service?wsdl >> CreateSvcClientCode.LOG
IF NOT EXIST SvcReservationsClientCode ( mkdir SvcReservationsClientCode ) ELSE ( del SvcReservationsClientCode\*.* ) >> CreateSvcClientCode.LOG
move output.config SvcReservationsClientCode\app.config >> CreateSvcClientCode.LOG
move ReservationsService.cs SvcReservationsClientCode\ReservationsService.cs >> CreateSvcClientCode.LOG

ECHO Reservations Client has been succesfully created (ReservationsService.cs and app.config)