TZipBuilder Version 1.79  DEMO 9 
v1.79 by Roger Aelbrecht September 6, 2005.  
     http://www30.websamba.com/driehoeksw
----------------------------------------------------------
TITLE:
	ADD IN THREAD DEMO 
	Based on Original Delphi published code by 
	Russell Peters for TZipMaster Demo9
	
DESCRIPTION :
	Is an example how TZipBuilder can be used in a Multi Threaded
	application.
	It starts a thread and then executes the Add method in this thread.
	After completion the results are reported to the main thread and the 
	thread is ended.
                                                           
DEMONSTRATES :  
  
  Events:
	
	- OnMessage for Error messages during Add operation
	- OnCheckTerminate event in TZipBuilder to check if the operations in
		TZipBuilder (especially DLL's) need to be stopped because the thread
		is terminated
	
	Properties:
		ZipFileName + AddOptions + FSpecArgs + SuccessCnt                                                         
                                                           
                                                           
BASIC OPERATIONS:            

	+ To start click the Add button, the thread will be created and the
		Add operation started.
	+ To prematurely end the operation click the Cancel button
	
	
	 