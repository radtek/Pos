TZipBuilder Version 1.79  DEMO 10 
v1.79 by Roger Aelbrecht September 7, 2005.  
     http://www30.websamba.com/driehoeksw
----------------------------------------------------------
TITLE:
	STORE DRIVE LETTER DEMO 
	
DESCRIPTION :
	Is an example how the drive letter for a file can be stored in the
	filename in the archive
	Drive letters are not stored in the archive in the normal way.
	By using the OnSetAddName and OnSetExtName events we can add that
	information to the filename and restore the drive in the path 
	during extraction
                                                           
DEMONSTRATES :  
  
  Events:
	
	- OnSetAddName for changing the name of the file added to the archive
	- OnsetExtName for changing the file name and path of the file written
	  during extraction
	- OnExtractOverWrite to ask if an existing file may be overwritten.
	
	Properties:
		ZipFileName + AddOptions + FSpecArgs + SuccessCnt                                                         
                                                           
                                                           
BASIC OPERATIONS:            

	+ To start click the Add button, an archive will be created with the 
	  files of this demo, the Memo shows how the filenames are changed
	+ To know the content of the archive click on Content and the name
	  of the files int he archive will be shown
	+ To extract the fiels to their original places click on Extract,
	  the MEmo will show how the filenames and path are changed.
	
	
	 