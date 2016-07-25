                         //---------------------------------------------------------------------------

#ifndef BuildIntSfxH
#define BuildIntSfxH

#include "CondDefines.h"
#include "ZipStructs.h"

#if __BORLANDC__ > 1343		// BCB4=0x540(1344), BCB5=0x550(1360)
typedef __int64 SeekInt;
#else
typedef int SeekInt;
#endif
#ifdef INTERNAL_SFX
//---------------------------------------------------------------------------
namespace BuildInternalSfx
{

// note IconResource Info is moved to Zipstructs.h
// class to hold older internal sfx code from ZipBuilder
// uses properties of ZipBuilder
// setting of properties to be done inside zipbuilder

class TInternalSFX
{
private:
  TObject* FOwner;
  int FInFileHandle;
  int	FOutFileHandle;

  DWord  __fastcall SearchResDirEntry(pIRD ResStart, pIRDirE entry, int Depth);
  DWord  __fastcall BrowseResDir( pIRD ResStart, pIRD dir, int Depth );
	DWord  __fastcall LookForDirs( const void *SectionData, const DWord SectionVirtualStart,
															const int SectionLen, pIDD directories );
	int    __fastcall ReplaceIcon( int SFXFile, int SFXSize );
	int    __fastcall RWCentralDir( int &OutFile, struct ZipEndOfCentral &EOC, int OffsetChange );
public:
  int			 FSFXOffset; // must accessible from TZipBuilder
  TInternalSFX(TObject* owner);
  ~TInternalSFX(void){};
//  virtual ~TInternalSFX(void){};
  int    __fastcall ConvertSFX( bool AutoExeViaAdd );
	int    __fastcall ConvertZIP( void );

};

} //namespace BuildInternalSfx
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace BuildInternalSfx;
#endif

#endif // INTERNAL_SFX


#endif
