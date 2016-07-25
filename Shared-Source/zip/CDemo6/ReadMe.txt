TZipBuilder Version 1.76  DEMO 5 
v1.76 by Roger Aelbrecht June 20, 2004.  
     http://home.tiscali.be/driehoeksw
----------------------------------------------------------
CHANGES:

1.73 12 August 2003 AppendSlash replaced by DelimitPath in AddFiles  
1.76 20 June 2004 - Replaced All ZipFilename by ZipFileName 
                  - DelimithPath is no longer member of TZipBuilder need to include ZipUtils.h
                  to use 'global' function DelimithPath
                  - Parameters for OnProgress adapted
                  - Loading of Dll's moved to OnActivate Event
TITLE:
	SPAN DISK DEMO

DESCRIPTION :
	Is a complete File Manager type of program that lets you create 
	archives, view archives, extract files, etc.But all files will 
	be spanned, this means it will be split in smaller parts so that
	they fit on an floppy disk.
	Spanning can be used to created smaller archives parts on hard 
	disk to be able to send smaller parts over networks. 
	
	For This demo;
	- Spanned archive on floppy disk:
		o file name on all disk is archive.zip
		o volume of disks is set to PKBACKUP #xx were xx is a sequence
		 number 01, 02, 03 etc...
	- Spanned archive on hard disk:
	 	o files are of type Archivexxx.zip were xxx is a sequence number
	 		001, 002, 003 etc..
  
DEMONSTRATES :  

	Properties : 
		+ in Main.cpp : AddOptions + Count + ErrCode + ExtrBaseDir + 
		                ExtrOptions + FSpecArgs + SuccessCnt +
		                KeepFreeOnDisk1 + MaxVolumeSize +
		                MinFreeVolumeSize + ZipFilename
    + in Message.cpp  : Cancel  + ZipBusy + UnzBusy
 
	
	Methods :
		+ in Main.cpp : Add + Extract + FullVersionString + Load_Zip_Dll +
		                Load_Unz_Dll + Unload_Zip_Dll + Unload_Unz_Dll +
		                ReadSpan + Writespan
	  + in AddFiles.Cpp :	AppendSlash
	  
	Events :
		+ in Main.cpp : OnDirUpdate + OnMessage + OnProgress
	  
BASIC OPERATIONS:

	New Spanned Archive:
	+ On floppy disks
	 	o before starting set Max.volume size to 0 to use full disk
	 	  capacity
	 	o set keep free on first disk to any value you require
	 	o check or uncheck Format each disk as required
	 	o Click 'New,Add and span' in the open dialog, browse to a floppy
	 	  disk drive and enter the name of the archive.
	 	  In de file selection dialog select all files to be added to the
	 	  archive and click on 'Start Now', When a new disk is needed the
	 	  application will ask for it
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  string grid on the main form 
	 	o the archive's file name is the one you entered and the volume
	 	  name of the diskettes is changed to PKBACK #01, PKBACK #02 ...
	+ On hard disk  
	  o before starting set Max.volume size to any size you want the
	    parts to be 1457664 is the equivalent size of a &.44 MB diskette
	 	o set keep free on first disk to any value you require
	 	o check or uncheck Format each disk as no use we don't format the
	 	  hard disk
	 	o Click 'New,Add and span' in the opendialog, browse to the
	 	  directory you want the archive on and enter the name of the
	 	  archive.
	 	  In de file selection dialog select all files to be added to the
	 	  archive and click on 'Start Now', When a new disk is needed the
	 	  application will ask for it
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form
		o the archive's file names are of the type Archive001.zip,
		  Archive002.zip and so on.
		  
	Open Existing and Add Files and Span:
	- On floppy disks
	 	o before starting set Max.volume size to 0 to use full disk
	 	  capacity
	 	o set keep free on first disk to any value you require
	 	o check or uncheck Format each disk as required     
	 	o Insert the last disk of the set for the archive you want to add
	 	  to in the floppy disk drive, if you inserted another disk of the
	 	  set the application will ask you to insert the last disk.
	 	o Click 'Open,Add and span' in the opendialog, browse to the
	 	  floppy disk drive and select the name of the archive you want to
	 	  add to.
	 	  In de file selection dialog select all files to be added to the
	 	  archive and click on 'Start Now'. When a new disk is needed the
	 	  application will ask for it
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 
	
	- On hard disk  
	  o before starting set Max.volume size to any size you want the
	    parts to be 1457664 is the equivalent size of a &.44 MB diskette
	 	o set keep free on first disk to any value you require
	 	o check or uncheck Format each disk as no use we don't format the
	 	  hard disk
	 	o Click 'Open, Add and span' in the opendialog, browse to the
	 	  directory with the archive you want to add to on and enter the
	 	  name of the archive without the sequence numbers. The files on
	 	  the disk have names such as Archive001.zip, Archive002.zip, use
	 	  only the name Archive.zip
	 	  In de file selection dialog select all files to be added to the
	 	  archive and click on 'Start Now', When a new disk is needed the
	 	  application will ask for it
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form	
	
	Open Existing and Span:
		With Open and span we open an existing normal archive and span it
		to floppies or split it to different files              
	- To floppy disks
		o before starting set Max.volume size to 0 to use full disk
	 	  capacity
	 	o set keep free on first disk to any value you require
	 	o You need empty formatted disks for this operation. The format
	 	  function is not called 
	 	o Click 'Open and Span' in the Open dialog select the archive you
	 	  want to span to floppies, next in the dir selection dialog
	 	  select the floppy drive. The spanning operation starts
	 	  immediately
	 	o  When a new disk is needed the application will ask for it
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 	
	- On hard disk  
	  o before starting set Max.volume size to any size you want the
	    parts to be 1457664 is the equivalent size of a 1.44 MB diskette
	 	o set keep free on first disk to any value you require
	 	o Click 'Open and Span' in the Open dialog select the archive you
	 	  want to span to floppies, next in the dir selection dialog
	 	  select the destination directory. The spanning operation starts
	 	  immediately
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 	

Extract files from spanned Archive;
	- Spanned archive on floppy disks
	 	o Insert the last disk of the set for the archive you want to add
	 	  to in the floppy disk drive, if you inserted another disk of the
	 	  set the application will ask you to insert the last disk.
	 	o Click 'Extract from spanned zip' and Browse to the archive on
	 	  floppy disk in the open file dialog.
	 	o in the Directory selection dialog select the extract directory,
	 	  that is the directory where all files will be written to. Check
	 	  options you need or want and click 'OK'
	 	o Follow the instructions about inserting disk in the program
	 	o All files in the archive will be extracted to the chosen
	 	  directory
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 		
	- Spanned archive on hard disk
		o Click 'Extract from spanned zip' and Browse to the directory
		  with the spanned archive and enter the name of the archive
		  without the sequence numbers. The files on the disk have names
		  such as Archive001.zip, Archive002.zip, use only the name
		  Archive.zip	 	  
	 	o in the Directory selection dialog select the extract directory,
	 	  that is the directory where all files will be written to. Check
	 	  options you need or want and click 'OK'
	 	o All files in the archive will be extracted to the chosen
	 	  directory
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 			  
	
	Create Normal archive from spanned archive  
	This function will create a 'normal' archive out of the parts of a
	spanned archive	
	- Spanned archive on floppy disks
		o Insert the last disk of the set for the archive you want to add
	 	  to in the floppy disk drive, if you inserted another disk of the
	 	  set the application will ask you to insert the last disk.
	 	o Click 'Make Standard zip from a spanned one' and Browse to the
	 	  archive on floppy disk in the open file dialog.
	 	o in the Directory selection dialog select the directory where the
	 	  new standard zip will be written to	 	  
	 	o Follow the instructions about inserting disk in the program
	 	o All files in the archive will be read and copied to the new
	 	  standard zip.
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 			
	- Spanned archive on hard disk  
		o Click 'Make Standard zip from a spanned one' and Browse to the
		  directory with the spanned archive and enter the name of the
		  archive without the sequence numbers. The files on the disk have
		  names such as Archive001.zip, Archive002.zip, use only the name
		  Archive.zip	 	  
	 	o in the Directory selection dialog select the directory where the
	 	  new standard zip will be written to	 	  
	 	o All files in the archive will be extracted to the chosen
	 	  directory
	 	o The messages received during the operation are shown in the
	 	  messages form
	 	o On completion the content of the archive is shown in the
	 	  stringgrid on the main form 			  