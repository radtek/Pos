#ifndef ZipDLLH
#define ZipDLLH
// ZIPDLL.H   - C++ Builder translation of file "wizzip.h" by Eric W. Engler
/* Header file for ZIPDLL - put this into the "include" statement of any
   other unit that wants to access the DLL. */

//---------------------------------------------------------------------------
#include "CallBack.h"

#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include "ZipLdr.h"
namespace ZipDLL {
//-- type declarations -------------------------------------------------------

#pragma option -a1

struct FileData 
{
	char					  *fFileSpec;			// Replaces pZipFN
	char					  *fFileComment;		//  z->comment and z->com
	char					  *fFileAltName;	
	char					  *fPassword;			//  Override in v1.60L
	BOOL						fEncrypt;			//  Override in v1.60L
	unsigned short int	fRecurse;			//  Override in v1.60L
	unsigned short int	fNoRecurseFiles;	//  Override
	BOOL						fDateUsed;			//  Override
	char						fDate[8];			//  Override
	char					  *fRootDir;			//  RootDir support for relative paths
	long						fNotUsed[16];	  
};

struct ExcludedFileSpec 
{
	char	  *fFileSpec;
};


struct ZipParms 
{
	HWND								fHandle;
	void							  *fCaller;
	long								fVersion;
	CallBack::ZFunctionPtrType ZCallbackFunc;
	BOOL								fTraceEnabled;
	char							  *pZipPassword;		// password pointer
	char							  *pSuffix;
	BOOL								fEncrypt;			// General encrypt, if not superseded by FileData.fEncrypt
	BOOL								fSystem;
	BOOL								fVolume;
	BOOL								fExtra;
	BOOL								fNoDirEntries;
	BOOL								fUseDate;			// General DateUsed, if not superseded by FileData.fDateUsed
	BOOL								fVerboseEnabled;
	BOOL								fQuiet;
	int								fLevel;
	BOOL								fComprSpecial;		// Not used any more. (v1.6) now fSuffix takes care of this.
	BOOL								fCRLF_LF;
	BOOL								fJunkDir;
	unsigned short int			fRecurse;
	unsigned short int			fNoRecurseFiles;
	BOOL								fGrow;
	BOOL								fForce;
	BOOL								fMove;
	BOOL								fDeleteEntries;
	BOOL								fUpdate;
	BOOL								fFreshen;
	BOOL								fJunkSFX;
	BOOL								fLatestTime;
	char								fDate[8];			// General Date, if not superseded by FileData.fDate
	long								fArgc;				// Changed, Number of FileData structures.
	char							  *pZipFN;				// Ptr to name of zip file.
	char							  *fTempPath;			//  b option
	char							  *fArchComment;		//  zcomment and zcomlen
	short int						fArchiveFilesOnly;//  when != 0 only zip when archive bit set
	short int						fResetArchiveBit;	//  when != 0 reset the archive bit after a successfull zip
	struct FileData			  *fFDS;					//  (Actually an array of FileData's)
	BOOL								fForceWin;			// 
	int								fTotExFileSpecs;	//  Number of ExcludedFileSpec structures.
	struct ExcludedFileSpec	  *fExFiles;	//  Array of file specs to exclude from zipping.
	BOOL								fUseOutStream; 	//  Use memory stream as output.
	void							  *fOutStream;	  		//  Pointer to the start of the output stream data.
	unsigned long					fOutStreamSize;	// Size of the Output data.
	BOOL								fUseInStream; 		// Use memory stream as input.
	void							  *fInStream;	  		// Pointer to the start of the input stream data.
	unsigned long					fInStreamSize;		// Size of the input data.
	DWORD								fStrFileAttr;		// File attributes of the file stream.
	DWORD								fStrFileDate;		// File date/time to set for the streamed file.
	BOOL								fHowToMove;			
	unsigned short		 	fWantedCodePage;
  unsigned short      fWantedOS;
  unsigned 	fVCLVer;		// new v1.74
  char*    fGRootDir; // new 1.76 global root directory
//	unsigned short					fNotUsed0;
	unsigned 						 fNotUsed[2];
	int								fSeven;				// End of structure (eg. 7)
};
#pragma option -a.

typedef ZipParms *PZipParms;


// 1.73 (2 August 2003 RA) added minver parameter in DoExec
class TZipDll : public TZiplibloader::TZipLibLoader
{
  public:
    TZipDll(TObject *owner) :TZipLibLoader(owner, true){};
    int Exec(PZipParms info, int minver){return DoExec(info, minver);}
};


}	/* namespace ZipDLL */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace ZipDLL;
#endif
#endif
