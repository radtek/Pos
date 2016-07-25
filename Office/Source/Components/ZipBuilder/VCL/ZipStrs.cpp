//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "CondDefines.h"
#include "ZipStrs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
 // required strings
const char* _CF_OverwriteYN = "Overwrite file '%s' in '%s' ?";
const char* _DS_AskDeleteFile = "There is already a file %s\nDo you want to overwrite this file";
const char* _DS_AskPrevFile = "ATTENTION: This is previous disk no %d!!!\n"
                               "Are you sure you want to overwrite the contents";
const char* _DS_CopyCentral = "Central directory";
const char* _DS_InDrive = "\nin drive: %s";
const char* _DS_InsertAVolume = "Please insert disk volume %.1d";
const char* _DS_InsertDisk = "Please insert last disk";
const char* _DS_InsertVolume = "Please insert disk volume %.1d of %.1d";
const char* _DS_NoDiskSpace = "This disk has not enough free space available";
const char* _DS_FirstInSet = "This is the first disk in a backup set,\n"
                             "please insert the last disk of this set";
const char* _DS_NoRenamePart = "Last part left as : %s";
const char* _LI_FileTooBig = "File larger than 4GB";
const char* _LI_MethodUnknown = "Compression method not known";
const char* _FM_Confirm = "Confirm";
const char* _FM_Erase   = "Erase ";
const char* _GE_CopyFile = "Copying: ";
const char* _GE_RangeError = "Index (%d) outside range 1..%d";
const char* _GE_TempZip = "Temporary zipfile: %s";
const char* _GE_WasBusy = "Busy + %s";
const char* _GE_Unknown = " Unknown error %d";            // new
const char* _PR_Loading = "*Loading Directory";
const char* _RN_ProcessFile = "Processing: ";
const char* _RN_RenameTo = " renamed to: ";
const char* _TM_Erasing = "EraseFloppy - Removing %s";    // new
const char* _TM_Deleting = "EraseFloppy - Deleting %s";   // new
const char* _TM_GetNewDisk = "Trace : GetNewDisk Opening: %s";
const char* _PW_Caption = "Password";
const char* _PW_MessageEnter = "Enter Password ";
const char* _PW_MessageConfirm = "Confirm Password ";
  // new strings
const char* _GE_Except  = "Exception in Event handler ";
const char* _GE_Reentered = "Was busy, Instructions may have been lost!";
const char* _GE_Busy    = " Busy, Please wait";
const char* _GE_Inactive = "not Active";
const char* _GE_EventEx = "Exception in Event ";                  // new
const char* _GE_DLLAbort = "DLL Error - Aborting";
const char* _GE_DLLBusy = "DLL Error - Busy";
const char* _GE_DLLCancel = "DLL Error - User Cancelled";
const char* _GE_DLLMem = "DLL Error - Not enough memory";
const char* _GE_DLLStruct = "DLL Error - Invalid structure";
const char* _GE_DLLEvent = "DLL Error - Exception in handler ";
const char* _GE_DLLCritical = "critical DLL Error %d";
const char* _SF_MsgTooLong   = "SFX Message string exceeds 255 characters!";
const char* _SF_DefPathTooLong = "SFX Default path exceeds 255 characters!";
const char* _SF_DlgTitleTooLong = "SFX Dialog title exceeds 255 characters!";
const char* _SF_CmdLineTooLong  = "SFX Command line exceeds 255 characters!";
const char* _SF_FBkPathTooLong  = "SFX Fallback path exceeds 255 characters!";
const char* _ZB_Yes = "&Yes";
const char* _ZB_No = "&No";
const char* _ZB_OK = "&OK";
const char* _ZB_Cancel = "&Cancel";
const char* _ZB_Abort =  "&Abort";
const char* _ZB_Retry = "&Retry";
const char* _ZB_Ignore = "&Ignore";
const char* _ZB_CancelAll = "CancelAll";
const char* _ZB_NoToAll = "NoToAll";
const char* _ZB_YesToAll = "YesToAll";

#ifdef USE_ALLZIPSTRINGS
const char* _GE_FatalZip = "Fatal Error in ZipDLL.DLL: abort exception";
const char* _GE_NoZipSpecified = "Error - no zip file specified!";
const char* _GE_NoMem   = "Requested memory not available";
const char* _GE_WrongPassword = "Error - passwords do NOT match\nPassword ignored";
const char* _RN_ZipSFXData = "Error while copying the SFX header";
const char* _RN_NoRenOnSpan = "Rename is not implemented for a spanned zip file";
const char* _RN_InvalidDateTime = "Invalid date/time argument for file: ";
const char* _PW_UnatAddPWMiss = "Error - no add password given";
const char* _PW_UnatExtPWMiss = "Error - no extract password given";
const char* _PW_ForFile = " for file: ";
const char* _CF_SourceIsDest = "Source archive is the same as the destination archive!";
const char* _CF_CopyFailed = "Copying a file from '%s' to '%s' failed";
const char* _CF_SourceNotFound = "File '%s' is not present in '%s'!";
const char* _CF_SFXCopyError = "Error while copying the SFX data";
const char* _CF_DestFileNoOpen = "Destination zip archive could not be opened!";
const char* _CF_NoCopyOnSpan = "CopyZippedFiles is not implemented for a spanned zip file";
const char* _LI_ReadZipError = "Seek error reading Zip archive!";
const char* _LI_ErrorUnknown = "Unknown error in List() function\n";
const char* _LI_WrongZipStruct = "Warning - Error in zip structure!";
const char* _LI_GarbageAtEOF = "Warning - Garbage at the end of the zipfile!";

const char* _AD_NothingToZip = "Error - no files to zip!";
const char* _AD_UnattPassword = "Unattended action not possible without a password";
const char* _AD_NoFreshenUpdate = "AddFreshen or AddUpdate not possible on a spanned archive";
const char* _AD_AutoSFXWrong = "Error %.1d occurred during Auto SFX creation.";
const char* _AD_InIsOutStream = "Input stream may not be set to the output stream";
const char* _AD_InvalidName = "Wildcards are not allowed in Filename or file specification";
const char* _AD_NoDestDir = "Destination directory '%s' must exist!";
const char* _DL_NothingToDel = "Error - no files selected for deletion";
const char* _DL_NoDelOnSpan = "Delete Files from archive is not implemented for a spanned zip file";
const char* _EX_FatalUnZip = "Fatal Error in UnzDLL.DLL: abort exception";
const char* _EX_UnAttPassword = "Warning - Unattended Extract: possible not all files extracted";
const char* _EX_NoStreamDLL = "Error - your UnzDLL.dll can not use streams,"
                              " please update to version >= 1.60";
const char* _EX_NoExtrDir = "Extract directory '%s' must exist";
const char* _LD_NoDll   = "Failed to load %s";
const char* _LD_BadDll  = "Unable to load %s - It is old or corrupt";
const char* _LD_DllLoaded = "Loaded ";
const char* _LD_DllUnloaded = "Unloaded ";
const char* _SF_NoZipSFXBin = "Error - ZIPSFX.BIN not found!";
const char* _SF_InputIsNoZip = "Error: input file is not a zip file";
const char* _SF_NoSFXSupport = "SFX Functions not supported";
const char* _CZ_NoExeSpecified = "Error - no .EXE file specified";
const char* _CZ_InputNotExe = "Error: input file is not an .EXE file";
const char* _CZ_SFXTypeUnknown = "Error determining the type of SFX archive";
const char* _DS_NoInFile = "Input file does not exist";
const char* _DS_NoOutFile = "Creation of output file failed";
const char* _DS_FileOpen = "Zip file could not be opened";
const char* _DS_NotaDrive = "Not a valid drive: %s";
const char* _DS_DriveNoMount = "Drive %s is NOT defined";
const char* _DS_NoVolume = "Volume label could not be set";
const char* _DS_NoMem   = "Not enough memory to display MsgBox";
const char* _DS_Canceled = "User canceled operation";
const char* _DS_FailedSeek = "Seek error in input file";
const char* _DS_NoWrite = "Write error in output file";
const char* _DS_EOCBadRead = "Error while reading the End Of Central Directory";
const char* _DS_LOHBadRead = "Error while reading a local header";
const char* _DS_CEHBadRead = "Error while reading a central header";
const char* _DS_LOHWrongSig = "A local header signature is wrong";
const char* _DS_CEHWrongSig = "A central header signature is wrong";
const char* _DS_LONameLen = "Error while reading a local file name";
const char* _DS_CENameLen = "Error while reading a central file name";
const char* _DS_LOExtraLen = "Error while reading a local extra field";
const char* _DS_CEExtraLen = "Error while reading a central extra field";
const char* _DS_DataDesc = "Error while reading/writing a data descriptor area";
const char* _DS_ZipData = "Error while reading zipped data";
const char* _DS_CECommentLen = "Error while reading a file comment";
const char* _DS_EOArchComLen = "Error while reading the archive comment";
const char* _DS_ErrorUnknown = "UnKnown error in function ReadSpan(), WriteSpan(),"
                                " ChangeFileDetails() or CopyZippedFiles()";
const char* _DS_NoUnattSpan = "Unattended disk spanning not implemented";
const char* _DS_EntryLost = "A local header not found in internal structure";
const char* _DS_NoTempFile = "Temporary file could not be created";
const char* _DS_LOHBadWrite = "Error while writing a local header";
const char* _DS_CEHBadWrite = "Error while writing a central header";
const char* _DS_EOCBadWrite = "Error while writing the End Of Central Directory";
const char* _DS_ExtWrongSig = "Error while reading a Extended Local signature";
const char* _DS_NoValidZip = "This archive is not a valid Zip archive";
const char* _DS_NotLastInSet = "This is the %dth disk in a backup set,\n"
                               "please insert the last disk of this set";
const char* _DS_NoSFXSpan = "Error - Self extracting archives(.exe) can not be spanned";
const char* _DS_CEHBadCopy = "Error while copying a filename of a CEH structure";
const char* _DS_EOCBadSeek = "Seek error while skipping a EOC structure";
const char* _DS_EOCBadCopy = "Error while copying the zip archive comment";
const char* _DS_FirstFileOnHD = "This is the first file in a backup set,\n"
                                 "please choose the last file of this set";
const char* _DS_NoDiskSpan = "DiskSpanning not supported";
const char* _DS_UnknownError = "Unknown Error";
const char* _ED_SizeTooLarge = "Size of FileExtraData is larger than available array";
const char* _CD_NoCDOnSpan = "ChangeFileDetails is not implemented for a spanned zip file";
const char* _CD_NoEventHndlr = "No Event Handler found to Change File Details";
const char* _CD_LOExtraLen = "Error while writing a local extra field";
const char* _CD_CEExtraLen = "Error while writing a central extra field";
const char* _CD_CEComLen = "Error while writing a file comment";
const char* _CD_FileName = "No FileName in changed file details";
const char* _CD_CEHDataSize = "The combined length of CEH + FileName + FileComment +"
                               " ExtraData exceeds 65535";
const char* _CD_Changing = "Changing details of: ";
const char* _CD_DuplFileName = "Duplicate Filename: %s";
const char* _CD_NoProtected = "Cannot change details of Excrypted file";
const char* _CD_InvalidFileName = "Invalid Filename: '%s'";
const char* _CD_NoChangeDir = "Cannot change path";
const char* _CD_FileSpecSkip = "Filespec '%s' skipped";
const char* _PR_Archive = "*Resetting Archive bit";
const char* _PR_CopyZipFile = "*Copying Zip File";
const char* _PR_SFX     = "*SFX";
const char* _PR_Header  = "*??";
const char* _PR_Finish  = "*Finalising";
const char* _PR_Copying = "*Copying";
const char* _PR_CentrlDir = "*Central Directory";
const char* _PR_Checking = "*Checking";
const char* _PR_Joining = "*Joining";
const char* _PR_Splitting = "*Splitting";
const char* _WZ_DropDirOnly = "Dropping %d empty directory entries";
const char* _WZ_NothingToWrite = "Nothing to write";
#endif


#ifdef USE_ALLZIPSTRINGS
const int ResTableSize = 186; // 56 always + 131 rest
#else
const int ResTableSize = 56;  // 56 always only
#endif

//---------------------------------------/------------------------------------
TZipResRec ResTable[ResTableSize]= {
    {CF_OverwriteYN, _CF_OverwriteYN},
    {DS_AskDeleteFile, _DS_AskDeleteFile},
    {DS_AskPrevFile, _DS_AskPrevFile},
    {DS_CopyCentral, _DS_CopyCentral},
    {DS_InDrive, _DS_InDrive},
    {DS_InsertDisk, _DS_InsertDisk},
    {DS_InsertVolume, _DS_InsertVolume},
    {DS_InsertAVolume, _DS_InsertAVolume},
    {DS_NoDiskSpace, _DS_NoDiskSpace},
    {DS_FirstInSet, _DS_FirstInSet},
    {DS_NoRenamePart, _DS_NoRenamePart},
    {FM_Confirm, _FM_Confirm},
    {FM_Erase, _FM_Erase},
    {GE_CopyFile, _GE_CopyFile},
    {GE_RangeError, _GE_RangeError},
    {GE_TempZip, _GE_TempZip},
    {GE_WasBusy, _GE_WasBusy},
    {GE_Unknown, _GE_Unknown},
    {LI_FileTooBig, _LI_FileTooBig},
    {PR_Loading, _PR_Loading},
    {RN_ProcessFile, _RN_ProcessFile},
    {RN_RenameTo, _RN_RenameTo},
    {TM_Erasing, _TM_Erasing},
    {TM_Deleting, _TM_Deleting},
    {TM_GetNewDisk, _TM_GetNewDisk},
    {GE_Except, _GE_Except},
    {GE_Reentered, _GE_Reentered},
    {GE_Busy, _GE_Busy},
    {GE_Inactive, _GE_Inactive},
    {GE_EventEx, _GE_EventEx},
    {PW_Caption, _PW_Caption},
    {PW_MessageEnter, _PW_MessageEnter},
    {PW_MessageConfirm, _PW_MessageConfirm},
  // new strings
    {GE_DLLAbort, _GE_DLLAbort},
    {GE_DLLBusy, _GE_DLLBusy},
    {GE_DLLCancel, _GE_DLLCancel},
    {GE_DLLMem, _GE_DLLMem},
    {GE_DLLStruct, _GE_DLLStruct},
    {GE_DLLEvent, _GE_DLLEvent},
    {GE_DLLCritical, _GE_DLLCritical},
    {SF_MsgTooLong, _SF_MsgTooLong},
    {SF_DefPathTooLong, _SF_DefPathTooLong},
    {SF_DlgTitleTooLong, _SF_DlgTitleTooLong},
    {SF_CmdLineTooLong, _SF_CmdLineTooLong},
    {SF_FBkPathTooLong, _SF_FBkPathTooLong},
    {ZB_Yes, _ZB_Yes},
    {ZB_No, _ZB_No},
    {ZB_OK,_ZB_OK},
    {ZB_Cancel, _ZB_Cancel},
    {ZB_Abort, _ZB_Abort},
    {ZB_Retry, _ZB_Retry},
    {ZB_Ignore, _ZB_Ignore},
    {ZB_CancelAll, _ZB_CancelAll},
    {ZB_NoToAll, _ZB_NoToAll},
    {ZB_YesToAll, _ZB_YesToAll}
#ifdef USE_ALLZIPSTRINGS
    ,{GE_FatalZip, _GE_FatalZip},
    {GE_NoZipSpecified, _GE_NoZipSpecified},
    {GE_NoMem, _GE_NoMem},
    {GE_WrongPassword, _GE_WrongPassword},
    {GE_CopyFile, _GE_CopyFile},
    {GE_Except, _GE_Except},
    {GE_Reentered, _GE_Reentered},
    {GE_Busy, _GE_Busy},
    {GE_Inactive, _GE_Inactive},
    {GE_RangeError, _GE_RangeError},
    {GE_TempZip, _GE_TempZip},
    {RN_ZipSFXData, _RN_ZipSFXData},
    {RN_NoRenOnSpan, _RN_NoRenOnSpan},
    {RN_ProcessFile, _RN_ProcessFile},
    {RN_RenameTo, _RN_RenameTo},
    {RN_InvalidDateTime, _RN_InvalidDateTime},
    {PW_UnatAddPWMiss, _PW_UnatAddPWMiss},
    {PW_UnatExtPWMiss, _PW_UnatExtPWMiss},
    {PW_ForFile, _PW_ForFile},
    {CF_SourceIsDest, _CF_SourceIsDest},
    {CF_CopyFailed, _CF_CopyFailed},
    {CF_SourceNotFound, _CF_SourceNotFound},
    {CF_SFXCopyError, _CF_SFXCopyError},
    {CF_DestFileNoOpen, _CF_DestFileNoOpen},
    {CF_NoCopyOnSpan, _CF_NoCopyOnSpan},
    {LI_ReadZipError, _LI_ReadZipError},
    {LI_ErrorUnknown, _LI_ErrorUnknown},
    {LI_WrongZipStruct, _LI_WrongZipStruct},
    {LI_GarbageAtEOF, _LI_GarbageAtEOF},
    {LI_MethodUnknown, _LI_MethodUnknown},
    {AD_NothingToZip, _AD_NothingToZip},
    {AD_UnattPassword, _AD_UnattPassword},
    {AD_NoFreshenUpdate, _AD_NoFreshenUpdate},
    {AD_AutoSFXWrong, _AD_AutoSFXWrong},
    {AD_InIsOutStream, _AD_InIsOutStream},
    {AD_InvalidName, _AD_InvalidName},
    {AD_NoDestDir, _AD_NoDestDir},
    {DL_NothingToDel, _DL_NothingToDel},
    {DL_NoDelOnSpan, _DL_NoDelOnSpan},
    {EX_FatalUnZip, _EX_FatalUnZip},
    {EX_UnAttPassword, _EX_UnAttPassword},
    {EX_NoStreamDLL, _EX_NoStreamDLL},
    {EX_NoExtrDir, _EX_NoExtrDir},
    {LD_NoDll, _LD_NoDll},
    {LD_BadDll, _LD_BadDll},
    {LD_DllLoaded, _LD_DllLoaded},
    {LD_DllUnloaded, _LD_DllUnloaded},
    {SF_NoZipSFXBin, _SF_NoZipSFXBin},
    {SF_InputIsNoZip, _SF_InputIsNoZip},
    {SF_NoSFXSupport, _SF_NoSFXSupport},
    {SF_MsgTooLong, _SF_MsgTooLong},
    {SF_DefPathTooLong, _SF_DefPathTooLong},
    {SF_DlgTitleTooLong, _SF_DlgTitleTooLong},
    {SF_CmdLineTooLong, _SF_CmdLineTooLong},
    {SF_FBkPathTooLong, _SF_FBkPathTooLong},
    {CZ_NoExeSpecified, _CZ_NoExeSpecified},
    {CZ_InputNotExe, _CZ_InputNotExe},
    {CZ_SFXTypeUnknown, _CZ_SFXTypeUnknown},
    {DS_NoInFile, _DS_NoInFile},
    {DS_NoOutFile, _DS_NoOutFile},
    {DS_NoRenamePart, _DS_NoRenamePart},
    {DS_FileOpen, _DS_FileOpen},
    {DS_NotaDrive, _DS_NotaDrive},
    {DS_DriveNoMount, _DS_DriveNoMount},
    {DS_NoVolume, _DS_NoVolume},
    {DS_NoMem, _DS_NoMem},
    {DS_Canceled, _DS_Canceled},
    {DS_FailedSeek, _DS_FailedSeek},
    {DS_NoWrite, _DS_NoWrite},
    {DS_EOCBadRead, _DS_EOCBadRead},
    {DS_LOHBadRead, _DS_LOHBadRead},
    {DS_CEHBadRead, _DS_CEHBadRead},
    {DS_LOHWrongSig, _DS_LOHWrongSig},
    {DS_CEHWrongSig, _DS_CEHWrongSig},
    {DS_LONameLen, _DS_LONameLen},
    {DS_CENameLen, _DS_CENameLen},
    {DS_LOExtraLen, _DS_LOExtraLen},
    {DS_CEExtraLen, _DS_CEExtraLen},
    {DS_DataDesc, _DS_DataDesc},
    {DS_ZipData, _DS_ZipData},
    {DS_CECommentLen, _DS_CECommentLen},
    {DS_EOArchComLen, _DS_EOArchComLen},
    {DS_ErrorUnknown, _DS_ErrorUnknown},
    {DS_NoUnattSpan, _DS_NoUnattSpan},
    {DS_EntryLost, _DS_EntryLost},
    {DS_NoTempFile, _DS_NoTempFile},
    {DS_LOHBadWrite, _DS_LOHBadWrite},
    {DS_CEHBadWrite, _DS_CEHBadWrite},
    {DS_EOCBadWrite, _DS_EOCBadWrite},
    {DS_ExtWrongSig, _DS_ExtWrongSig},
    {DS_NoValidZip, _DS_NoValidZip},
    {DS_NotLastInSet, _DS_NotLastInSet},
    {DS_NoSFXSpan, _DS_NoSFXSpan},
    {DS_CEHBadCopy, _DS_CEHBadCopy},
    {DS_EOCBadSeek, _DS_EOCBadSeek},
    {DS_EOCBadCopy, _DS_EOCBadCopy},
    {DS_FirstFileOnHD, _DS_FirstFileOnHD},
    {DS_NoDiskSpan, _DS_NoDiskSpan},
    {DS_UnknownError, _DS_UnknownError},
    {FM_Erase, _FM_Erase},
    {FM_Confirm, _FM_Confirm},
    {ED_SizeTooLarge, _ED_SizeTooLarge},
    {CD_NoCDOnSpan, _CD_NoCDOnSpan},
    {CD_NoEventHndlr, _CD_NoEventHndlr},
    {CD_LOExtraLen, _CD_LOExtraLen},
    {CD_CEExtraLen, _CD_CEExtraLen},
    {CD_CEComLen, _CD_CEComLen},
    {CD_FileName, _CD_FileName},
    {CD_CEHDataSize, _CD_CEHDataSize},
    {CD_Changing, _CD_Changing},
    {CD_DuplFileName, _CD_DuplFileName},
    {CD_NoProtected, _CD_NoProtected},
    {CD_InvalidFileName, _CD_InvalidFileName},
    {CD_NoChangeDir, _CD_NoChangeDir},
    {CD_FileSpecSkip, _CD_FileSpecSkip},
    {PR_Archive, _PR_Archive},
    {PR_CopyZipFile, _PR_CopyZipFile},
    {PR_SFX, _PR_SFX},
    {PR_Header, _PR_Header},
    {PR_Finish, _PR_Finish},
    {PR_Copying, _PR_Copying},
    {PR_CentrlDir, _PR_CentrlDir},
    {PR_Checking, _PR_Checking},
    {PR_Loading, _PR_Loading},
    {PR_Joining, _PR_Joining},
    {PR_Splitting, _PR_Splitting},
    {WZ_DropDirOnly, _WZ_DropDirOnly},
    {WZ_NothingToWrite, _WZ_NothingToWrite},
    {TM_Erasing, _TM_Erasing},
    {TM_Deleting, _TM_Deleting},
    {TM_GetNewDisk, _TM_GetNewDisk}
#endif
};

struct RenIds
{
	Word newId;
	Word oldId;
};

const RenIds SubstIds[4] = { {ZB_OK, PW_Ok},{ZB_Cancel, PW_Cancel},
                             {ZB_CancelAll, PW_CancelAll}, {ZB_Abort, PW_Abort}};

const char* Find(int id)
{
  const char* Result = NULL;
  Word wi = static_cast<Word>(id);
  for(int i = 0; i < ResTableSize; ++i)
  {
    if(ResTable[i].i == wi)
    {
      Result = ResTable[i].s;
      break;
    }
  }
  return Result;
}

AnsiString FindOld(int id)
{
  AnsiString Result = "";
  for(int i = 0; i < 4; ++i)
  {
    if(static_cast<Word>(id) == SubstIds[i].newId)
    {
      Result = ZipLoadStr(SubstIds[i].oldId);
      break;
    }
  }
  return Result;
}

//---------------------------------------------------------------------------
AnsiString ZipLookup(int ident)
{
  AnsiString Result = "";
  if(ident < 10000) return Result;
  const char* p = Find(ident);
  if(!p) Result = FindOld(ident);
  else Result = AnsiString(p);
  return Result;
}
//---------------------------------------------------------------------------
TZipStrEvent GOnZipStr = NULL;
//---------------------------------------------------------------------------
// Global function to load resource string - new 1.76
AnsiString __export ZipLoadStr(int ident)
{
  AnsiString Result = "";
  if(GOnZipStr) GOnZipStr(ident, Result);
  if(Result.IsEmpty()) Result = LoadStr(ident);
  if(Result.IsEmpty()) Result = ZipLookup(ident);
  return Result;
}
//----------------------------------------------------------------------------
AnsiString __export ZipLoadStr(int ident, const AnsiString DefStr)
{
  AnsiString Result = ZipLoadStr(ident);
  if(Result == "") Result = DefStr;
  return Result;
}
//----------------------------------------------------------------------------
TZipStrEvent SetZipStr(TZipStrEvent handler, bool set)
{
  TZipStrEvent Result = GOnZipStr;
  if(set) GOnZipStr = handler;
  return Result;
}
//----------------------------------------------------------------------------
AnsiString __export ZipFmtLoadStr(int Ident, const System::TVarRec* Args, const int Args_Size)
{
  AnsiString Result = ZipLoadStr(Ident);
  if(Result != "") Result = Format(Result, Args, Args_Size);
  return Result;
}
