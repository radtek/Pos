/*
  ZBDeLZip.h - port of DelZip.h (dll interface)
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2005  Roger Aelbrecht

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License (licence.txt) for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: Zipbuilder AT drehoeksw DOT net
  DelphiZip maillist subscribe at http://www.geocities.com/SiliconValley/Orchard/8607/ 
  updates: http://www.drehoeksw.net

  last modified 2005-11-20
---------------------------------------------------------------------------*/
#ifndef DelZip_H
#define DelZip_H

const int Callback_Except_No = 10106;
//Callbackstructs-------------------------------------------------------------
// Maximum no. of files in a single ZIP file (do NOT change):
//const int FilesMax = 4096;

/* All the items in the CallBackStruct are passed to the ZB
 * program from the DLL.  Note that the "Caller" value returned
 * here is the same one specified earlier in ZipParms by the
 * BCB program. */
#pragma option -a1
struct ZCallBackStruct
{
  void  *Caller;                // 'this' reference of the ZipBuilder class
  long   Version;               // version no. of DLL.
  BOOL   IsOperationZip;        // true=zip, false=unzip
  int    ActionCode;            // returns <0 if result valid
  long   Arg1;                  // ErrorCode;
  const char *MsgP;             // pointer to text
  union
  {
    char Msg[512];              // storage for returning text
    struct
    {
      __int64 FileSize;
      __int64 Written;             // number of bytes written
      const char *MsgP2;        // additional text
      unsigned Arg2;            // additional arg
      unsigned Arg3;
    };
  };
};
#pragma option -a.
typedef struct ZCallBackStruct *PZCallBackStruct;

#define CB_USEMSG ((const char *)-1)

/* Declare a function pointer type for the BCB/Delphi callback function, to
 * be called by the DLL to pass updated status info back to BCB/Delphi.*/

typedef int __stdcall (*ZFunctionPtrType)(void *, PZCallBackStruct ZCallbackRec);

#ifndef NO_UNZIP_STRUCTS
//Unzip structs-------------------------------------------------------------------
#pragma option -a1

/* These records are very critical.  Any changes in the order of items, the
   size of items, or modifying the number of items, may have disasterous
   results.  You have been warned! */

typedef struct
{
  char          *fFileSpec;
  char          *fFileAltName;
  char          *fPassword;
  long          fNotUsed[4];
  long          fMatch;                   // used by extract for searches
}UnzFileData;

struct UnzExFileData
{
  char      *fFileSpec;
  long      fNotUsed[3];
};

typedef struct
{
  HWND                          fHandle;
  void                          *fCaller;
  long                          fVersion;
  ZFunctionPtrType              ZCallbackFunc;
  BOOL                          fTraceEnabled;
  unsigned                 fWantedCodePage;
  unsigned                 fPromptToOverwrite;
  char                          *pZipPassword;
  BOOL                          fTest;
  BOOL                          fComments;
  BOOL                          fConvert;
  BOOL                          fQuiet;
  BOOL                          fVerboseEnabled;
  BOOL                          fUpdate;
  BOOL                          fFreshen;
  BOOL                          fDirectories;
  BOOL                          fOverwrite;
  long                          fArgc;
  char                          *pZipFN;
  UnzFileData                   *fUFDS;
  struct UnzExFileData          *fXUDFS;
  BOOL                          fUseOutStream;      // Use Memory stream as output.
  void                          *fOutStream;          // Pointer to the start of streaam data.
  unsigned long                 fOutStreamSize; // Size of the output data.
  BOOL                          fUseInStream;       // Use memory stream as input.
  void                          *fInStream;           // Pointer to the start of the input stream data.
  unsigned long                 fInStreamSize;      // Size of the input data.
  unsigned long                 fPwdReqCount;       // PasswordRequestCount, How many times a password will be asked per file
  char                          *fExtractDir;
  long                          fNotUsed[8];
  long                          fSeven;
}UnZipParms;
#pragma option -a.

typedef UnZipParms *pUnZipParms;
#endif

#ifndef NO_ZIP_STRUCTS
//Zip structs---------------------------------------------------------------------
#pragma option -a1

typedef struct
{
  char* fFileSpec;
  char* fFileComment;     //  z->comment and z->com
  char* fFileAltName;
  char* fPassword;
  BOOL                        fEncrypt;
  unsigned   fRecurse;
  unsigned fFromDate;
  int fLevel;
  int fnotused2;
  char* fRootDir;         //  RootDir support for relative paths
  long    fNotUsed[6];
}FileData;

struct ExcludedFileSpec
{
  char      *fFileSpec;
};

typedef struct
{
  HWND                          fHandle;
  void                          *fCaller;
  long                          fVersion;
  ZFunctionPtrType              ZCallbackFunc;
  BOOL                          fTraceEnabled;
  char                          *pZipPassword;        // password pointer
  char                          *pSuffix;
  BOOL                          fEncrypt;           // General encrypt, if not superseded by FileData.fEncrypt
  BOOL                          fSystem;
  BOOL                          fVolume;
  BOOL                          fExtra;
  BOOL                          fNoDirEntries;
  BOOL                          fUseDate;           // General DateUsed, if not superseded by FileData.fDateUsed
  BOOL                          fVerboseEnabled;
  BOOL                          fQuiet;
  int                           fLevel;
  BOOL                          fComprSpecial;      // Not used any more. (v1.6) now fSuffix takes care of this.
  BOOL                          fCRLF_LF;
  BOOL                          fJunkDir;
  unsigned             fRecurse;
  unsigned             fNoRecurseFiles;
  BOOL                          fGrow;
  BOOL                          fForce;
  BOOL                          fMove;
  BOOL                          fDeleteEntries;
  BOOL                          fUpdate;
  BOOL                          fFreshen;
  BOOL                          fJunkSFX;
  BOOL                          fLatestTime;
        // General Date, if not superseded by FileData.fDate
  unsigned              fDate;
  long                          fArgc;                  // Changed, Number of FileData structures.
  char                          *pZipFN;                // Ptr to name of zip file.
  char                          *fTempPath;             //  b option
  char                          *fArchComment;          //  zcomment and zcomlen
  int                     fArchiveFilesOnly;      //  when != 0 only zip when archive bit set
  int                     fResetArchiveBit;       //  when != 0 reset the archive bit after a successfull zip
  FileData                      *fFDS;                  //  (Actually an array of FileData's)
  BOOL                          fForceWin;              //
  int                           fTotExFileSpecs;        //  Number of ExcludedFileSpec structures.
  struct ExcludedFileSpec       *fExFiles;              //  Array of file specs to exclude from zipping.
  BOOL                          fUseOutStream;          //  Use memory stream as output.
  void                          *fOutStream;            //  Pointer to the start of the output stream data.
  unsigned long                 fOutStreamSize;         // Size of the Output data.
  BOOL                          fUseInStream;           // Use memory stream as input.
  void                          *fInStream;             // Pointer to the start of the input stream data.
  unsigned long                 fInStreamSize;          // Size of the input data.
  DWORD                         fStrFileAttr;           // File attributes of the file stream.
  DWORD                         fStrFileDate;           // File date/time to set for the streamed file.
  BOOL                          fHowToMove;
  unsigned                fWantedCodePage;
  unsigned                fWantedOS;
  unsigned                      fVCLVer;
  char*                         fGRootDir;
  unsigned                      fNotUsed[2];
  int                           fSeven;                 // End of structure (eg. 7)
}ZipParms;
#pragma option -a.

typedef ZipParms *PZipParms;
#endif

const int CRITICAL_CANCELLED = -1;
const int CRITICAL_ABORT     = -2;
const int CRITICAL_CALLBACK  = -3;
const int CRITICAL_MEMORY    = -4;
const int CRITICAL_STRUCT    = -5;
const int CRITICAL_ERROR     = -6;


#endif
