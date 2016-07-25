               Welcome to the Zipbuilder v1.79 Release!
             This is the Borland C++ Builder Edition
                          December 12, 2005


This is version 1.79 of ZipBuilder. This release is comprehensive 
- it includes all the files you need. This should work on Borland 
C++ Builder, versions 3, 4, 5 and 6 and on the BDS4.0


This release contains:

ZipBuilder179xy.zip  - overall Freeware ZipBuilder v1.79 package.

x indicates update number
y indicates build number 

Files in Main directory:
	- Essential support files.  TZipBuilder VCL, and the source files for its
	      supporting classes that also needs to be included	into the ZipBuilder 
	      package, as well as the DLL import specifications and callback files.
	        Put these into a directory in your VCL search path.


                  Turbo C++ 2006 explorer version
 
Subdirectories:

  \DLL           - the DelZip179.dll
                           Put these into C:\WINDOWS\SYSTEM
                           or in your application directory

	\CBuilder			 - Contains packages that can be used to install the compmonent
									 in the different IDE's	
  \Help					 - the ZipHelp.hlp and ZipHelp.cnt to be include in the
                 		Borland Help system   
  \Docs          -  LICENSE.tXT	- File containing the full  GNU LESSER GENERAL
                                  PUBLIC LICENSE
                  README.TXT    - this file
                  INST-BCB3.TXT - info. about how to install the VCLs and
                           run the demo's in BCB version 3
                  INST-BCB4.TXT - info. about how to install the VCLs and
                           run the demo's in BCB version 4 
                  INST-BCB5.TXT - info about how to install the VCLs and 
                           run the demo's in BCB version 5
                  INST-BCB6.TXT - info about how to install the VCLs and 
                           run the demo's in BCB version 6 
                  BDS4_INST.TXT -	info about how to install the component in 
                           BDS4.0
                  Inst-TC2006.TXT - info about how to use TZipbuilder with the 
                           Turbo C++ 20006 Explorer version
                                    
  \CDemo1        - BCB demo program #1.
  \CDemo2        - BCB demo program #2.
  \CDemo3        - BCB demo program #3.
  \CDemo4        - BCB demo program #4.
  \CDemo5        - BCB demo program #5.
  \CDemo6        - BCB demo program #6.
  \CDemo7        - BCB demo program #7.
  \CDemo8        - BCB demo program #8. 
  \CDemo9        - BCB demo program #9.
  \CDemo10       - BCB demo program #10.

NOTE
----
For the ZBSFXxx.BIN and ZBSFXxx.RES files needed to make self-extracting
archives download the separate ZBSFXBinRes__ package


                            IMPORTANT!!! 
                            ************

All error message in English version are build-in, in this version.
This means including the ZipMsgUS.rc files is no longer needed.

To use another language there are two options.
Option 1: 
	Include the required languages into a resource file, this has to be done
	in the proper compressed format. A tool : 'ZipResMaker' has been made
	available to perform this task. For more info see the help file and the
	help file of the ZipResMaker tool.
Option 2:
	Include the appropriate ZipMsfxx.rc file into your project.


                     Installation Instructions
                                                  
See this files for more installation instructions:
Inst-BCB3.txt or
Inst-BCB4.txt or
Inst-BCB5.txt or
Inst-BCB6.txt or
bds4_inst.txt or
Inst-TC2006.txt


                      Licenses
                               
	This component is subject to the "GNU LESSER GENERAL PUBLIC LICENSE "
as explained in full in the License.txt file


                    DLL Source Code in C
       
   The DLL source code is distributed separately due to it's size, and the fact
that most users of this package probably don't want the C source for the DLL.  
The DLL source is also freeware, and will remain that way. 


                Problem Reports or Suggestions

We like you to report all problems and bugs to us.
To encourage these reporting Eric Engler has set-up a free mail list, all parties working on this project use this mail list for problem reporting and 
bug-solving.

You can subscribe to this mail list at:

  http://www.geocities.com/SiliconValley/Orchard/8607/


Roger Aelbrecht

Zipbuilder@drehoeksw.net