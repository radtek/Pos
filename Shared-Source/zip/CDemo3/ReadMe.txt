TZipBuilder Version 1.79  DEMO 3 
v1.79 by Roger Aelbrecht September 2, 2005.  
     http://www30.websamba.com/driehoeksw
----------------------------------------------------------
TITLE:
	ZIP DEMO 3            
	
CHANGES:
	1.76: All ZipFilename renamed to ZipFileName	

DESCRIPTION :
	Is a type of program to Add/Extract files to an archive
	and shows the messages that are received when option
	Verbose is set to true, inclusive the message from the DLL

DEMONSTRATES :  

	Properties : 
		+ in Unit3.cpp : AddOptions + Count + ExtrOptions + 
		                FSpecArgs + Verbose + ZipFilename + Password
	Methods :
		+ in Unit3.cpp : Add + Delete + Extract + FullVersionString + 
										Load_Zip_Dll + Load_Unz_Dll + Unload_Zip_Dll + 
										Unload_Unz_Dll
	Events :
		+ in Unit3.cpp : OnMessage
	  	
BASIC OPERATIONS:
	All functions assume that an archive test1.zip exists
	
	Content of Archive:
		o Click on 'ListZip' and the content of the archive will be
			displayed in the TMemo
	
	Adding file to existing archive
		o Click on 'Add' and test*.txt will be added to the archive
		  This means that the files TestA.txt and TestB.txt will be added 
		  to the archive. All messages received during this operation are
		  shown in the Memo. Messages from the DLL are preceded by
		  'trace;'
	
	Add With Password
		o Click on 'Add with Pwd' and text*.txt will be added but before
			the new files are added to the archive a password is requested
			enter a password and confirm with entering the same
			All message received during this operation are shown in the Memo
		  Messages from the DLL are preceded by 'trace;'
	
	Extract file from archive
		o Click on 'Extract and all files in the archive will be extract
			to their original place with overwrite of existing files. 
			If during the	Add operation a password was given, this will be
			requested again during extract operation. All message received
			during this operation are shown in the Memo Messages from the
			DLL are preceded by 'trace;'                         
		
	Integrity Test of an Archive		
	  o Click on 'Test' and the integrity of the entire archive will be
	   	tested. If during the	Add operation a password was given, this
	   	will be requested again during extract operation. All message
	   	received during this operation are shown in the Memo Messages
	   	from the DLL are preceded by 'trace;'    
	 
	Deleting files from Archive
		o Click on 'Delete from Zip' and flies 'TestA.txt' and 'TestB.txt'
		 	will be deleted from the archive. All message received during
		 	this operation are shown in the Memo Messages from the DLL are
		 	preceded by 'trace;'     
		 	
	Versions
		o By clicking on 'Version' the version of the used TZipBuilder 
			component and of the DLL's are shown in a separate message box