TZipBuilder Version 1.79  DEMO 8 
v1.79 by Roger Aelbrecht September 6, 2005.  
     http://www30.websamba.com/driehoeksw
----------------------------------------------------------
TITLE:
	VERSDEMO 
	Based on Original Delphi published code by 
	Russell Peters and Alistair George 
	
DESCRIPTION :
	Sometimes users want to keep different versions of the same 
	file in one archive. This example gives a possible solution.
	It uses the OnSetNewName event in TZipBuilder to check if the
	added file already exists and if it does adds a directory name
	to the file name. This new directory consists of a string
	presentation of the actual DateTime. 
                                                           
DEMONSTRATES :  
  
  Events:
	- OnSetNewName event during Add operation in ZipBuilder
	- OnMessage in case of errors in add or Extract operations
	- OnExtractOverwrite event in Extract operation
	
	Properties:
		ZipFileName + AddOptions + Count + DirEntry +ErrCode + 
		ExtrBaseDir + ExtrOptions + FSpecArgs + SuccessCnt + 
		ZipFileSize                                                          
                                                           
                                                           
BASIC OPERATIONS:            

	+ To start you need to create some sample files and a zip archive
	  containing these sample files. You start the demo by clicking
	  "Create Sample"
	+ To view the content of the archive click "Show ZipContent"
	  The first time only one version of each file is present.
	+ Next make some modifications to the samples by clicking
	  "Change Files"
	+ Update the zip archive by clicking "Update Zip". This will add 
	  the modified files to the archive
	+ By clicking "Show ZipContent" again you will see the new contents
	  the changed files are added prefixed by a directory in the format
	  \#mmddhhnn#\ where mm = month; dd = day; hh = hour; nn is minutes
	  
	+ You can delete all created sample files by clicking "Delete
	  Samples". By clicking "Delete All" both sample files and archive
	  will be deleted
	  
	+ Clicking "Restore Latest" will Restore the latest version of each
	  file in the archive to its original location
	+ "Restore Sel Vers" will restore files you selected in the TreeView
	  Selecting files can be done by clicking the check boxes in front
	  of the file date and time presentations of each file.
	
	 