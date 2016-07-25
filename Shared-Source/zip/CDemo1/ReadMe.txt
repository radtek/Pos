TZipBuilder Version 1.79  DEMO 1 
v1.79 by Roger Aelbrecht September 2, 2005.  
     http://www30.websamba.com/driehoeksw
---------------------------------------------------------- 
CHANGES:
1.73 12 August 2003 in AddFiles.cpp replaced AppendSlash by DelimitPath
1.76 19 June 2004   in Main.cpp - All ZipFilename replaced by ZipFileName
                                - ProgressEvent signature changed
                                - Loading of Dll's moved to OnActivate Event
1.78 19 October 2004 in Main.cpp - Use of ProgressDetails events (replaces older Progress evnets)
                                 
TITLE:
	CDEMO1

DESCRIPTION :
	Is a complete File Manager type of program that lets you create 
	archives, view archives, extract files, etc.  It can
  also turn .zip files into .exe files (and vice-versa).
  
DEMONSTRATES :  

	Properties : 
		+ in Main.cpp : AddOptions + Count + ErrCode +
		                ExtrBaseDir + ExtrOptions + 
		                FSpecArgs + SuccessCnt + Trace +
			 							Unattended + Verbose + ZipComment +
			              ZipFilename
    + in AddFiles.cpp : KeepFreeOnDisk1 + MaxVolumeSize
    + in Message.cpp  : Cancel  + ZipBusy
    + in Rename.cpp   : ZipContents	
    + in SFX.cpp     : SFXCaption + SFXCommandLine + SFXDefaultDir + 
    									 SFXIcon + SFXMessage + SFXOptions + 
    									 SFXOverWriteMode    		
	
	Methods :
		+ in Main.cpp : Add + ConvertSFX + ConvertZIP + Delete + 
		                Extract + FullVersionString + Load_Zip_Dll +
		                Load_Unz_Dll + Unload_Zip_Dll + Unload_Unz_Dll
	  + in AddFiles.Cpp :	AppendSlash
	  + in Rename.cpp   :Rename	
	
	Events :
		+ in Main.cpp : OnDirUpdate + OnMessage + OnProgressDetails
	  	
BASIC OPERATIONS:

	Create Archive:
		o in Main form click 'New ZipFile' and select the directory and
		  the filename for the new archive.
		o Click 'Add to Zip' and in the 'Add Files' selection box select
		  the files you want to include in the archive by selecting the
		  drive, directory and files in the left part of the form, confirm
		  your selection in the middle part and select the options you want 
		  in this part as well. You selection will be shown in the right
		  hand part of the form
		o Instead of selecting all files you can select drive + directory
		  and fill in the	Wildcard edit box with the your choice and enter
		  them with either the Add button or the Add+ path button, the
		  former will only add the wildcards, the latter the selected 
		  directory + the wildcards 
		o When your selection is completed push the 'Start now' button and
		  the archive will be made
		o All message received during the Add operation are shown in the
		  Message window
		o At the end of the operation the number of added files is shown	
		o when the message window is dismissed you will see the content of
		  the archive in the main form
	
	Adding to Existing Archive
		o To do anything to an existing archive you have to open it first,
		  click 'OpenZip' and browse to the file you want to open
		o Next click the Add burton and add the files you want as above
		
	Extracting files from archive
		o Start with opening an existing archive by clicking 'Open Zip' 
		  and browse to the archive you want
		o Next click 'Extract from Zip' and a directory selection form
		  will appear, select the directory were the files should be 
		  extracted to and indicate the options you want
		o Click 'OK' tho start the operations.
		o All message received during the Add operation are shown in the
		  Message window
		o At the end of the operation the number of extracted files is
		  shown	appear
	 
	Delete files from an archive
		o Start with opening an existing archive by clicking 'Open Zip' 
		  and browse to the archive you want
		o select the files you want to delete from the archive in the
		  string grid on the mainform
		o Click 'Delete from zip' and confirm
		o All message received during the Add operation are shown in the
		  Message window
		o At the end of the operation the number of deleted files is
		  shown	appear			            

	Rename a file in an archive
		o Start with opening an existing archive by clicking 'Open Zip' 
		  and browse to the archive you want
		o Click 'Rename' and a rename dialog will open
		o Select the file to rename in the 'Chose old file specification'
		  drop down selection box
		o Enter the new name for this file in the 'Type new file
		  specification' edit box
		o If you want a new dat/time set for this file (or all files) 
		  enter the date/time you want in the 'Date/time spec4 edit fields
		  usually in the MM/DD/YY HH:MM:SS format, but this can change
		  depending on the local settings of your computer
		o If the selection is correct push the 'Add' button and your
		  selection will appear in the upper part of these form   
		o If you need to add more files repeat the above
		o If you want to remove a file from the list select it in the
		  top part and click 'Remove Selected'
		o To rename all indicated files click 'OK' to start the operation
		o All message received during the Add operation are shown in the
		  Message window
		o At the end of the operation the number of renamed files is
		  shown	appear     
		  
	Creating an SFX file : SFX is a self-extracting archive
	  o Start with opening an existing archive by clicking 'Open Zip' 
		  and browse to the archive you want	
		o Click 'Convert to EXE' a new form with several tab sheets is
		  opened that allows selection of the options for the SFX file
		o Select each of the tab sheets and insert or select the options
		  and or messages that you want to appear in the SFX file
		o When all is selected click 'DoItnow' and the conversion will
		  be done, if successful it will return the new file name 
	
	Converting a SFX file to a zip file
	  o Start with opening an existing SFX by clicking 'Open Zip' 
		  and browse to the archive you want this has to be an .exe file
	  o The button 'Convert to EXE' will now read 'Convert To ZIP'
	  o By clicking this button the conversion will be executed.
	