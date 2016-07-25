               Welcome to the DelZip v1.78 Release!
             This is the Borland C++ Builder Edition
                          October 20, 2004


This is a beta release of DelZip version 1.78 This release
is comprehensive - it includes all the files you need. This should 
work on Borland C++ Builder, versions 3, 4, 5 and 6


This release contains:

ZipBuilder178xy.zip  - overall Freeware DelZip v1.78 package.

x indicates update number
y indicates build number 

Files in Main directory:

 README.TXT    - this file

 INST-BCB3.TXT - info. about how to install the VCLs and
                           run the demo's in BCB version 3

 INST-BCB4.TXT - info. about how to install the VCLs and
                           run the demo's in BCB version 4 
                           
 INST-BCB5.TXT - info about how to install the VCLs and 
                           run the demo's in BCB version 5
                            
 INST-BCB6.TXT - info about how to install the VCLs and 
                           run the demo's in BCB version 6 
Subdirectories:

  \DLL           - the ZIPDLL and UNZDLL DLL's (require Win32)
                           Put these into C:\WINDOWS\SYSTEM
                           or in your application directory

  \VCL           - Essential support files.  TZipBuilder VCL, 
  												 and the source files for its supporting 
  												 classes that also needs to be included 
  												 into the ZipBuilder package, 
                           as well as the DLL import specifications 
                           and callback files.  Put these into a 
                           directory in your VCL search path.

  \SFX           - Contains ZIPSFX.BIN, used to make self-extracting 
                           archives

  \SFXsource     - Contains the source files for ZipSFX.bin, used to
                           make a custom version, files are Delphi
                           Pascall files.

 \LangRes    - Contains the resource files for the different languages

  \CDemo1        - BCB demo program #1.
  \CDemo2        - BCB demo program #2.
  \CDemo3        - BCB demo program #3.
  \CDemo4        - BCB demo program #4.
  \CDemo5        - BCB demo program #5.
  \CDemo6        - BCB demo program #6.
  \CDemo7        - BCB demo program #7.
  \CDemo8        - BCB demo program #8. 
  \CDemo9        - BCB demo program #9.

                            IMPORTANT!!! 
                            ************

All error message in English version are build-in, in this version.
This means including the ZipMsgUS.rc files is no longer needed.
To use another language the user must specify which language to use 
by including the appropriate ZipMsfxx.rc file into his project.

Other languages are not guaranteed ready for this Beta release.
The file "ZipNew178.rc" contains the new messages used. these are
probably not translated.

                     Installation Instructions
                                                  
See this files for more installation instructions:
Inst-BCB3.txt or
Inst-BCB4.txt or
Inst-BCB5.txt or
Inst-BCB6.txt 


                      Licenses
                               
	This component is subject to the "GNU LESSER GENERAL PUBLIC LICENSE "
as explained in full in the Help file


                    DLL Source Code in C
       
   The DLL source code is distributed separately due to it's
size, and the fact that most Delphi users of this package
probably don't want the C source for the DLL's.  The DLL 
source is also freeware, and will remain that way. 
The filename of the DLL source code is ZipDll178xy.ZIP


                Problem Reports or Suggestions

We like you to report all problems and bugs to us.
To encourage these reporting Eric Engler has set-up a free
mail list, all parties working on this project use this mail list
for problem reporting and bug-solving

You can subscribe to this mail list at:

  http://www.geocities.com/SiliconValley/Orchard/8607/


Roger Aelbrecht

Roger.Aelbrecht@tiscali.be