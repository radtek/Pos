TZipBuilder Version 1.79  DEMO 5 
v1.79 by Roger Aelbrecht September 3, 2005.  
     http://www30.websamba.com/driehoeksw
----------------------------------------------------------
CHANGES:

1.73 12 August 2003 AppendSlash replaced by DelimitPath in AddFiles  
1.76 20 June 2004 Replaced All ZipFilename by ZipFileName 
                  DelimithPath is no longer member of TZipBuilder need to include ZipUtils.h
                  to use 'global' function DelimithPath

TITLE:
	Create self extracting archive

DESCRIPTION :
	This is a simple demo program that shows
	how users can directly create .EXE archives,
	without first making a .ZIP file.
  
DEMONSTRATES :  

	Properties : 
		+ in Main5.cpp : AddOptions + Count + ErrCode +
		                FSpecArgs + SuccessCnt + Trace +
			 							Verbose + ZipFilename + Password +
			 							SFXDefaultDir
    + in AddFiles.cpp : KeepFreeOnDisk1 + MaxVolumeSize
      
	Methods :
		+ in Main5.cpp : Add + Delete + Load_Zip_Dll + Unload_Zip_Dll +
		                 GetAddPassword
	  + in AddFiles.Cpp :	AppendSlash
	 
	Events :
		+ in Main5.cpp : OnDirUpdate + OnMessage
	  	
BASIC OPERATIONS:

	Create SFX:
		o Click on 'Create New Archive'. An open file dialog box will open 
			you can browse to the directory of your choice and enter the
			filename, before creating the archive the program asks if this 
			archive needs to be protected by password, depending on your
			answer a password dialog box will open. Entering a filename
			will create the basic SFX file with default settings.
		o Now add files to your archive by clicking 'Add Files',
		  An drive/dir/file selection box will open, you can select the
		  files that should be in the archive. Clicking 'StartNow' will add
		  all files to your archive
    o You can delete files from the archive by selecting the files to
    	delete and click on 'Delete file'
    
  Destroy Current archive
  	o Clicking this button will destroy the currently opened archive
  		As usual you need to confirm this destruction. 














































