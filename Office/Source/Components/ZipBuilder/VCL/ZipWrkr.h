/* TZipWorker part of TZipBuidler component v1.60 by Chris Vleghert
 * a C++ Builder 1, 3, 4 , 5 and 6 wrapper for the freeware ZIP and UNZIP DLL's
 * from Chris Vleghert and E.W. Engler.
 *	 e-mail: englere@abraxis.com
 *  www:    http://www.geocities.com/SiliconValley/Network/2114
 *  v1.78 by Roger Aelbrecht October 18, 2004.
 *          http://home.tiscali.be/driehoeksw
 */

#ifndef ZipMainUnitH
#define ZipMainUnitH

//---------------------------------------------------------------------------
//#include <vcl\Buttons.hpp>
// including ShellApi.hpp is not needed
// ZBuilder.cpp includes vcl.h that includes vcl0.h that includes dialogs.hpp and there ShlObj.hpp
// is included that has include ShellApi.hpp in it
//#include <vcl\ShellApi.hpp> // for TSHFileOpStruct definition

#include "CondDefines.h"
#include "CallBack.h"
#include "UnzipDLL.h"
#include "ZipDLL.h"
#include "ZipStructs.h"


typedef TMsgDlgBtn TPasswordButton;
typedef TMsgDlgButtons TPasswordButtons; //set of TPasswordButton;

//---------------------------------------------------------------------------
//----------------------BCB1, BCB3, BCB4 and BCB5  differences---------------
#if __BORLANDC__ < 1328		// BCB1=0x0520(1312)
#	// BCB_1 doesn't know 'PACKAGE'
#	define PACKAGE
namespace Sysutils
{
	extern bool __fastcall DeleteFile( const System::AnsiString aStr );
}
#	define PGBCONV unsigned short int
#   define mrNoToAll (Byte)(9)
#	// The old style Out of Memory exception.
#	include <except.h>
#	define MEMEXCEPT	xalloc
#else								// BCB3=0x0530(1328)
#	define PGBCONV unsigned int
#	// xalloc is obsolete and not used in BCB3.
#	include <new.h>
#	define MEMEXCEPT	std::bad_alloc
#endif

#if __BORLANDC__ > 1328		// BCB3=0x0530(1328)
#define BINT __int64
#else
#define BINT long
#endif


// We need a forward declaration.
namespace Zipworker
{
	class TZipWorker;
}

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
  // 1.75 18 Feb 2004 RP changed for >2G
struct MDZipData {                  // MyDirZipData
	unsigned short DiskStart;			// The number of the disk on which this file begins.
	unsigned __int64 RelOffLocal;		// This is the offset from the start of the first disk on.
	unsigned short FileNameLen;
	char          *FileName;			// The name of the file we have handled correctly.
	unsigned long	 CRC32;				// Used in the unzip code.
	unsigned long	 ComprSize;
	unsigned long	 UnComprSize;
	unsigned long	 DateTime;
};
#pragma option -a.

// Definition for if the datadescriptor record is present.
#define DescUsed( DirRec ) (DirRec.Flag & 0x0008)

//---------------------------------------------------------------------------
namespace Zipworker
{

typedef long __stdcall (*GetSHFormatAddress)( HWND hwnd, long drive, long Fid, long Opt );
typedef BOOL __stdcall (*GDiskFreeSpaceEx)( char *RootName, __int64 *FreeForCaller, __int64 *TotNoOfBytes, __int64 *TotNoOfFreeBytes );

#pragma option -b-
enum AddOptsEnum  { AddDirNames, AddRecurseDirs, AddMove, AddFreshen, AddUpdate,AddZipTime,
              AddForceDOS, AddHiddenFiles, AddArchiveOnly, AddResetArchive,
						  AddEncrypt,  AddSeparateDirs, AddVolume, AddFromDate, AddSafe, AddForceDest,
              AddDiskSpan, AddDiskSpanErase };
#pragma option -b.
typedef Set<AddOptsEnum, AddDirNames, AddDiskSpanErase>  AddOpts;
#pragma option -b-
enum AddStoreSuffix{ assGIF = 0, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ,
                     assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR, assEXE};
#pragma option -b.
typedef Set< AddStoreSuffix, assGIF, assEXE> AddStoreExts;
#pragma option -b-
enum SpanOptsEnum { spNoVolumeName, spCompatName, spWipeFiles, spTryFormat };
#pragma option -b.
typedef Set<SpanOptsEnum, spNoVolumeName, spTryFormat> SpanOpts;
#pragma option -b-
enum ExtrOptsEnum {ExtrDirNames, ExtrOverWrite, ExtrFreshen, ExtrUpdate, ExtrForceDirs, ExtrTest };
#pragma option -b.
typedef Set<ExtrOptsEnum, ExtrDirNames, ExtrTest>  ExtrOpts;
#pragma option -b-
enum OvrOpts {OvrConfirm, OvrAlways, OvrNever};
enum ReplaceOpts {rplConfirm, rplAlways, rplNewer, rplNever};
enum SFXOptsEnum  { SFXAskCmdLine, SFXAskFiles, SFXAutoRun, SFXHideOverWriteBox,
						  SFXCheckSize, SFXNoSuccessMsg };
#pragma option -b.
typedef Set<SFXOptsEnum, SFXAskCmdLine, SFXNoSuccessMsg> SfxOpts;
#pragma option -b-
enum CPOpts	{cpAuto, cpNone, cpOEM};
enum DeleteOpts	{htdFinal, htdAllowUndo};
enum UnZipSkipTypes	{stOnFreshen, stNoOverwrite, stFileExists, stBadPassword, stNoEncryptionDLL,
							 	  stCompressionUnknown, stUnknownZipHost, stZipFileFormatWrong, stGeneralExtractError };
enum ZipDiskStatusEnum {zdsEmpty, zdsHasFiles, zdsPreviousDisk, zdsSameFileName, zdsNotEnoughSpace };
enum TZipDiskAction	{zdaOk, zdaErase, zdaReject, zdaCancel, zdaYesToAll};
#pragma option -b.
typedef Set<ZipDiskStatusEnum, zdsEmpty, zdsNotEnoughSpace> TZipDiskStatus;
#pragma option -b-
enum CodePageDirection	{ cpdOEM2ISO, cpdISO2OEM };
enum TZipDelayedItems { zdiList, zdiComment };
#pragma option -b.
typedef Set<TZipDelayedItems, zdiList, zdiComment>  TZipDelays;


#ifndef NO_STREAM
class __declspec(delphiclass) TZipStream : public TMemoryStream
{
	friend class TZipWorker;

	protected:
		inline __fastcall virtual ~TZipStream( void ) { }
		inline __fastcall virtual TZipStream( void ) {
			Clear();
		}
	public:
		void __fastcall virtual SetPointer( void *Ptr, int Size );
};
#endif

#pragma option -a1
struct ZipDirEntry
{		// Data from the Central header
	unsigned char	MadeByVersion;
	unsigned char	HostVersionNo;
	Word				Version;
	Word				Flag;
	Word				CompressionMethod;
	long				DateTime;
	long				CRC32;
	long				CompressedSize;
	long				UncompressedSize;
	Word				FileNameLength;
	Word				ExtraFieldLength;
	unsigned short	FileCommentLen;
	unsigned short	StartOnDisk;
	unsigned short	IntFileAttrib;
	unsigned long	ExtFileAttrib;
	unsigned long	RelOffLocalHdr;
	String			FileName;
	String			FileComment;
	bool				Encrypted;
	String      ExtraData;
};
#pragma option -a.

class TMZipDataList : public TList
{
	private:
		MDZipData* GetItems(int Index);
	public:
		__fastcall TMZipDataList(int TotalEntries);
		__fastcall ~TMZipDataList(void);
		__property MDZipData* Items[int Index] =  {read=GetItems};
		int IndexOf(const String& fname);
};
#pragma option -a.
//------------------------------------------------------------------------
typedef void __fastcall (__closure *TCopyZipOverwriteEvent)(System::TObject *Sender, String ForFile,
                                                             bool &DoOverwrite );
typedef void __fastcall (__closure *TCRC32ErrorEvent)(System::TObject *Sender, String ForFile,
                                                       unsigned FoundCRC, unsigned ExpectedCRC,
                                                       bool &DoExtract );
typedef void __fastcall (__closure *TExtractOverwriteEvent)(System::TObject *Sender, String ForFile,
                                                            bool IsOlder, bool &DoOverwrite, int DirIndex );
typedef void __fastcall (__closure *TExtractSkippedEvent)(System::TObject *Sender, String ForFile,
                                                          UnZipSkipTypes SkipType, int ExtError );
typedef void __fastcall (__closure *TFileCommentEvent)(System::TObject *Sender, String ForFile,
                                                      String &FileComment, bool &IsChanged );
typedef void __fastcall (__closure *TFileExtraEvent)(TObject *Sender,String ForFile, String& Data,
                                                     bool &IsChanged);
typedef void __fastcall (__closure *TGetNextDiskEvent)( System::TObject *Sender, int DiskSeqNo, int DiskTotal,
                                                       String Drive, bool &Cancel );

typedef void __fastcall (__closure *TNewNameEvent)( System::TObject *Sender, int SeqNo,const ZipDirEntry* ZipEntry );
typedef void __fastcall (__closure *TPasswordErrorEvent)( System::TObject *Sender, bool IsZipAction, String &NewPassword,
                                                         String ForFile, unsigned &RepeatCount, TMsgDlgBtn &Action );
typedef void __fastcall (__closure *TSetAddNameEvent)(TObject* Sender, String& FileName,
                                                       const String ExtName, bool& IsChanged);
typedef void __fastcall (__closure *TSetExtNameEvent)(TObject* Sender, String& Filename, bool& IsChanged);
typedef void __fastcall (__closure *TSetNewNameEvent)( System::TObject *Sender, String &OldFileName, bool &IsChanged );
typedef void __fastcall (__closure *TStatusDiskEvent)( System::TObject *Sender, int PreviousDisk, String PreviousFile, TZipDiskStatus Status, TZipDiskAction &Action );


#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
class __declspec(delphiclass) TZipWorker : public TZipBase//Classes::TComponent
{
	friend class TSHFormatThread;
#ifdef INTERNAL_SFX
  friend class TInternalSFX;
#endif

typedef Zipbase::TZipBase inherited;
	private:
	// fields of published properties
    TZipDelays FDelaying;
    int	FAddCompLevel;
    AddOpts FAddOptions;
    AddStoreExts FAddStoreSuffixes;
    AnsiString FExtAddStoreSuffixes;
    // private versions of property variables
    int	FDirOnlyCount;
    HWND FHandle;
    bool FIsSpanned;
    Classes::TList* FZipContents;
    String FExtrBaseDir;
    bool FZipBusy;
		bool FUnzBusy;
    ExtrOpts FExtrOptions;
    TStrings* FFSpecArgs;
    TStrings* FFSpecArgsExcl;
    String FZipFileName;
    int FSuccessCnt;
		String FPassword;
		bool FEncrypt;
    unsigned FSFXOffset;
    SpanOpts FSpanOptions;
    int FVer;
    AnsiString FVersionInfo;
    bool AutoExeViaAdd;
		String FVolumeName;
    int FBufSize;
		BINT FSizeOfDisk;
    BINT FDiskFree;
    BINT FFreeOnDisk;
		String FDrive;
    bool FDriveFixed;
    DeleteOpts FHowToDelete;
    String FStoredExtraData;
    int	FDiskNr;
		unsigned short FTotalDisks;
		unsigned FFileSize;
		unsigned FRealFileSize;
		bool FWrongZipStruct;
		String FInFileName;
		int	FInFileHandle;
		int	FOutFileHandle;
    unsigned char	FHostNum;
		unsigned char	FHostVer;
    int	FDateStamp;
    TDateTime FFromDate;
   	TZipShowProgress FShowProgress;
    unsigned FFreeOnDisk1;
    unsigned FFreeOnAllDisks;
    int FMaxVolumeSize;
		int	FMinFreeVolSize;
    CPOpts FCodePage;
    unsigned FZipSOC;
		unsigned FZipEOC;
		String FZipComment;
    String FEOCComment;
#ifndef NO_STREAM
    class TZipStream	*FZipStream;
#endif
    unsigned FPasswordReqCount;
    bool FUseDirOnlyEntries;
    String FRootDir;
		// Dll related variables
    TZipDll	*fZipDll;
		TUnzDll *fUnzDll;
		ZipDLL::ZipParms	  	*ZipParms;
		UnzipDLL::UnZipParms 	*UnZipParms;
#ifndef NO_SPAN
		bool FConfirmErase;
		int	 FDiskWritten;
		int	 FDriveNr;
		bool FNewDisk;
		String FOutFileName;
		TZipDiskStatus	FZipDiskStatus;
		TZipDiskAction	FZipDiskAction;
    TGetNextDiskEvent FOnGetNextDisk;
    TStatusDiskEvent FOnStatusDisk;
#endif
#ifdef INTERNAL_SFX
    // internal sfx class
    TInternalSFX* FIntSFX;
#endif
#ifndef NO_SFX
		String					FSFXCaption;				// dflt = "Self-extracting Archive"
		String						FSFXCommandLine;	// dflt = ""
		String						FSFXDefaultDir;		// dflt = ""
		TIcon*						FSFXIcon;
		String						FSFXMessage;
		SfxOpts						FSFXOptions;
		OvrOpts						FSFXOverWriteMode;	// OvrConfirm  (others: OvrAlways, OvrNever)
		String						FSFXPath;
#endif
		//Property get/set functions
    void __fastcall SetActive(bool Value);
		int    __fastcall GetCount( void );
		void   __fastcall SetFileName( String Value );
		int    __fastcall GetZipVers( void );
		int    __fastcall GetUnzVers( void );
		String __fastcall GetZipComment( void );
		void	 __fastcall SetZipComment( String Value );
    void __fastcall SetExtAddStoreSuffixes(String Value);
    void __fastcall SetVersionInfo(AnsiString Value);
    void __fastcall SetPasswordReqCount(unsigned Value);

    TCheckTerminateEvent FOnCheckTerminate;
    TCopyZipOverwriteEvent FOnCopyZipOverwrite;
    TCRC32ErrorEvent FOnCRC32Error;
    Classes::TNotifyEvent		FOnDirUpdate;
    TExtractOverwriteEvent FOnExtractOverwrite;
    TExtractSkippedEvent FOnExtractSkipped;
    TFileCommentEvent FOnFileComment;
    TFileExtraEvent FOnFileExtra;
    TNewNameEvent FOnNewName;
    TPasswordErrorEvent FOnPasswordError;
    TSetAddNameEvent FOnSetAddName;
    TSetExtNameEvent FOnSetExtName;
    TSetNewNameEvent FOnSetNewName;
			// Private "helper" functions
    void __fastcall SetCancel(bool Value);
    bool __fastcall IsFixedDrive(AnsiString drv);
		ZipDirEntry* __fastcall GetDirEntry(int idx);
    void __fastcall FreeZipDirEntryRecords( void );
    void __fastcall SetZipSwitches( String &NameOfZipFile );
    void __fastcall SetUnZipSwitches( String &NameOfZipFile );
    String __fastcall ConvertOEM(const String Source, CodePageDirection Direction);
    bool __fastcall GetDriveProps(void);
    bool   __fastcall OpenEOC( struct ZipEndOfCentral &EOC, bool DoExcept );
    int    __fastcall CopyBuffer( int &InFile, int &OutFile, int ReadLen );
		void   __fastcall WriteJoin( void *Buffer, int BufSize, int DSErrIdent );
    void   __fastcall ReadJoin( void *Buffer, int BufSize, int DSErrIdent);
    void   __fastcall GetNewDisk( int DiskSeq );
    void __fastcall DllCallBack(CallBack::PZCallBackStruct cbr);
    void   __fastcall DiskFreeAndSize( int Action );
		void   __fastcall AddSuffix( const AddStoreSuffix SufOption, String &sStr, int sPos );
    void   __fastcall ExtExtract( int UseStream, TMemoryStream *MemStream );
		void   __fastcall ExtAdd(int UseStream, DWORD StrFileDate, DWORD StrFileAttr,
                                TMemoryStream *MemStream);
    void __fastcall SetDeleteSwitches(void);
    void __fastcall RemoveDriveSpec(AnsiString& FullPathName);
    bool __fastcall HasExtendedChars(const AnsiString FileName);
    void __fastcall Setup_extend(void);
    void __fastcall CloseFileHandle(int &FileHandle);
    bool __fastcall IsPathOnly(const String f);
    AnsiString __fastcall ExtMsg(AnsiString CMsg, CallBack::PZCallBackStruct cbr,
                                  unsigned XIdent, int XPreamble);

#ifndef NO_SPAN
    int __fastcall GetLastVolume(String FInFileName,ZipEndOfCentral &EOC, bool AllowNotExists);
    void __fastcall CreateMVFileName(String& FileName,bool StripPartNbr);
		void __fastcall CheckForDisk(bool Writing);
		void __fastcall ClearFloppy(String dir);
    bool   __fastcall IsRightDisk(void);
    AnsiString __fastcall MakeString(char* Buffer, int Size);
		void   __fastcall RWSplitData( char *Buffer, int ReadLen, int DSErrIdent );
		void   __fastcall RWJoinData( char *Buffer, int ReadLen, int DSErrIdent );
		void   __fastcall WriteSplit( const void *Buffer, int Len, int MinSize );
		int  __fastcall ZipFormat( void );
    AnsiString __fastcall NameOfPart(AnsiString fn, bool compat);
#endif

#ifdef INTERNAL_SFX
    int    __fastcall ConvertSFX( bool AutoExeViaAdd );
#endif
  protected:
    static int __stdcall ZCallback( CallBack::PZCallBackStruct cbr );

	public:
		__fastcall TZipWorker(void);
		__fastcall virtual ~TZipWorker( void );
    virtual void __fastcall BeforeDestruction(void);
    virtual void __fastcall Starting(void);
    virtual void __fastcall Done(void);
		// public properties (run-time only)
    __property bool Active = {read=FActive, write=SetActive, nodefault};
		__property HWND Handle = {read=FHandle, write=FHandle, nodefault};
		__property ErrCode;
		__property Message;
		__property Classes::TList *ZipContents = {read=FZipContents};
		__property Cancel;
		__property bool ZipBusy = {read=FZipBusy, nodefault};
		__property bool UnzBusy = {read=FUnzBusy, nodefault};
		__property int Count = {read=GetCount, nodefault};
		__property int SuccessCnt = {read=FSuccessCnt, nodefault};
		__property int ZipVers = {read=GetZipVers, nodefault};
		__property int UnzVers = {read=GetUnzVers, nodefault};
		__property int Ver={read=FVer};
		__property unsigned SFXOffset	= {read=FSFXOffset, nodefault};
		__property unsigned	ZipSOC = {read=FZipSOC,	nodefault};
		__property unsigned	ZipEOC = {read=FZipEOC,	nodefault};
		__property bool	IsSpanned	= {read=FIsSpanned, nodefault};
		__property unsigned	ZipFileSize	= {read=FRealFileSize,	nodefault};
		__property int FullErrCode = {read=FFullErrCode, write=FFullErrCode, nodefault};
		__property BINT  TotalSizeToProcess = {read=GetTotalSizeToProcess,    nodefault};
		__property String ZipComment = {read=GetZipComment,   write=SetZipComment, nodefault};
		__property int DirOnlyCount	= {read=FDirOnlyCount,   default=0};
		__property ZipDirEntry* DirEntry[int idx] = {read=GetDirEntry};
#ifndef NO_STREAM
		__property TZipStream	*ZipStream = {read=FZipStream,		  nodefault};
#endif
    		//Public Methods
		int __fastcall Add(void);
		int __fastcall Delete(void);
		int __fastcall Extract(void);
		int  __fastcall List( void );
     // load dll - return version
		int __fastcall Load_Zip_Dll(void );
		int __fastcall Load_Unz_Dll(void );
		void __fastcall Unload_Zip_Dll(void );
		void __fastcall Unload_Unz_Dll(void );
    String __fastcall ZipDllPath(void);
    String __fastcall UnzDllPath(void);
    void   __fastcall AbortDlls(void);
		int    __fastcall Copy_File( const String src, const String dest );
    String __fastcall GetAddPassword(TPasswordButton& Response);
		String __fastcall GetExtrPassword( TPasswordButton& Response );
    AnsiString __fastcall AppendSlash(AnsiString sDir);
		int    __fastcall Rename( TList &RenameList, unsigned long DateTime );
		TPasswordButton __fastcall GetPassword( String DialogCaption, String MsgTxt, TPasswordButtons pwButtons,
		                                        String &ResultStr );
    TPasswordButton __fastcall GetPassword(String DialogCaption, String MsgTxt, int ctx,
                                           TPasswordButtons pwb, String& ResultStr);
		int    __fastcall CopyZippedFiles( TZipWorker *DestZipBuilder, bool DeleteFromSource, OvrOpts OverwriteDest );
		String __fastcall FullVersionString(void);
    void __fastcall Clear(void);
    ZipDirEntry* __fastcall Find(const AnsiString fspec, int& idx);
    int __fastcall IndexOf(const AnsiString fname);
    	//Events
		__property Classes::TNotifyEvent	OnDirUpdate	= { read=FOnDirUpdate, write=FOnDirUpdate};
		__property TSetNewNameEvent OnSetNewName = { read=FOnSetNewName, write=FOnSetNewName};
		__property TNewNameEvent OnNewName = { read=FOnNewName, write=FOnNewName};
		__property TCRC32ErrorEvent OnCRC32Error = { read=FOnCRC32Error, write=FOnCRC32Error};
		__property TPasswordErrorEvent OnPasswordError = { read=FOnPasswordError, write=FOnPasswordError};
		__property TExtractOverwriteEvent OnExtractOverwrite = {read=FOnExtractOverwrite, write=FOnExtractOverwrite};
		__property TExtractSkippedEvent	 OnExtractSkipped	 = {read=FOnExtractSkipped,	 write=FOnExtractSkipped};
		__property TCopyZipOverwriteEvent OnCopyZipOverwrite = {read=FOnCopyZipOverwrite, write=FOnCopyZipOverwrite};
		__property TFileCommentEvent OnFileComment = {read=FOnFileComment, write=FOnFileComment};
    __property TFileExtraEvent OnFileExtra={read=FOnFileExtra, write=FOnFileExtra};
    __property TSetAddNameEvent OnSetAddName={read=FOnSetAddName, write=FOnSetAddName};
    __property TSetExtNameEvent OnSetExtName={read=FOnSetExtName, write=FOnSetExtName};
#ifndef NO_SPAN
		int    __fastcall WriteSpan( const String InFileName, const String OutFileName, bool UseXprogress);
		int    __fastcall ReadSpan( const String InFileName, String &OutFilePath, bool UseXprogress );
    __property TGetNextDiskEvent OnGetNextDisk = {read=FOnGetNextDisk, write=FOnGetNextDisk};
	  __property TStatusDiskEvent OnStatusDisk = {read=FOnStatusDisk, write=FOnStatusDisk};
#endif
#ifdef INTERNAL_SFX
		int    __fastcall ConvertSFX( void );
		int    __fastcall ConvertZIP( void );
#endif
#ifndef NO_SFX
    int    __fastcall IsZipSFX( const String SFXExeName );
		__property SfxOpts SFXOptions = {read=FSFXOptions, write=FSFXOptions, default=SFXCheckSize};
		__property String	 SFXPath	= {read=FSFXPath,	write=FSFXPath,	nodefault};
		__property OvrOpts SFXOverWriteMode = {read=FSFXOverWriteMode, write=FSFXOverWriteMode, default=OvrConfirm};
		__property String	 SFXCaption = {read=FSFXCaption, write=FSFXCaption, nodefault};
		__property String	 SFXDefaultDir = {read=FSFXDefaultDir, write=FSFXDefaultDir,  nodefault};
		__property String	 SFXCommandLine = {read=FSFXCommandLine, write=FSFXCommandLine, nodefault};
		__property String  SFXMessage = {read=FSFXMessage, write=FSFXMessage, nodefault};
		__property TIcon  *SFXIcon = {read=FSFXIcon, write=FSFXIcon, nodefault};
#endif
#ifndef NO_STREAM
    TZipStream * __fastcall ExtractFileToStream(String FileName);
    int   __fastcall AddStreamToFile(String FileName, DWORD FileDate, DWORD FileAttr);
		TZipStream * __fastcall ExtractStreamToStream( TMemoryStream *InStream, unsigned long OutSize = 32768 );
		TZipStream * __fastcall AddStreamToStream( TMemoryStream *InStream );
#endif
 		__property int AddCompLevel={read=FAddCompLevel, write= FAddCompLevel};
    __property AddOpts AddOptions={ read= FAddOptions,write=FAddOptions};
    __property TDate AddFrom={read=FFromDate,write=FFromDate};
    __property int BufSize={read=FBufSize, write=FBufSize, default=65535};
    __property AnsiString ExtrBaseDir={read=FExtrBaseDir,write=FExtrBaseDir};
    __property ExtrOpts ExtrOptions={read=FExtrOptions,write=FExtrOptions};
		__property TStrings *FSpecArgs ={read=FFSpecArgs,write=FFSpecArgs};
    __property TStrings *FSpecArgsExcl ={read=FFSpecArgsExcl, write=FFSpecArgsExcl};
		__property String ZipFileName = {read=FZipFileName, write=SetFileName, nodefault};
		__property String	Password = {read=FPassword, write=FPassword, nodefault};
		__property AddStoreExts AddStoreSuffixes={read=FAddStoreSuffixes,write=FAddStoreSuffixes};
    __property String ExtAddStoreSuffixes = {read=FExtAddStoreSuffixes,
                                        write=SetExtAddStoreSuffixes, nodefault};
    __property CPOpts CodePage={read=FCodePage,write=FCodePage}; // default cpAuto;
		__property DeleteOpts HowToDelete = {read=FHowToDelete, write=FHowToDelete, default=htdAllowUndo};
    __property AnsiString VersionInfo={read=FVersionInfo,write=SetVersionInfo};
    __property unsigned PasswordReqCount={read=FPasswordReqCount,write=SetPasswordReqCount};
    __property bool UseDirOnlyEntries={read=FUseDirOnlyEntries,write=FUseDirOnlyEntries, default=false};
    __property AnsiString RootDir={read=FRootDir,write=FRootDir};
#ifndef NO_SPAN
    __property SpanOpts SpanOptions={ read=FSpanOptions,write=FSpanOptions};
    __property bool ConfirmErase={read=FConfirmErase,write=FConfirmErase};
    __property unsigned KeepFreeOnDisk1={read=FFreeOnDisk1,write=FFreeOnDisk1};
    __property unsigned KeepFreeOnAllDisks={read=FFreeOnAllDisks,write=FFreeOnAllDisks};
    __property int MaxVolumeSize={ read= FMaxVolumeSize, write= FMaxVolumeSize};
    __property int MinFreeVolumeSize={read=FMinFreeVolSize,write=FMinFreeVolSize,default=65536};
#endif

};

#pragma option -a.

#if __BORLANDC__ < 1344		// BCB4=0x540(1344)
 //-- template instantiations -------------------------------------------------
 template class ExtrOpts;
 template class AddOpts;
 template class SfxOpts;
 template class AddStoreExts;
#endif
}	/* EOF namespace Zipworker */

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace Zipworker;
#endif


/*
Changes for 1.73
  - From this release part of the history can be found at the top of each changed/new function
    Version, date and author and reason forchange are indicated for each function
	For changes for previous version see 'History.txt'
*/
#endif


