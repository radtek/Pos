//---------------------------------------------------------------------------

#ifndef EZipBuilderH
#define EZipBuilderH
//#include <vcl\vcl.h>
//---------------------------------------------------------------------------

namespace Ezipbuilder
{
 // new version 1.76
class PACKAGE EZipBuilder : public Sysutils::Exception
{
public:
  bool FDisplayMsg;	// We do not always want to see a message after an exception.
	int  FResIdent;	// We also save the Resource ID in case ZipMsg is not linked in the application.

	__fastcall EZipBuilder(int Ident, bool Display = true);
	__fastcall EZipBuilder(int Ident, const int DiskNo );
	__fastcall EZipBuilder(int Ident, const AnsiString Drive );
	__fastcall EZipBuilder(int Ident, const AnsiString File1, const AnsiString File2 );
  __fastcall EZipBuilder(int Ident, const TVarRec* Args, const int Args_Size);
  __fastcall EZipBuilder(const AnsiString Msg);
  __fastcall EZipBuilder(const AnsiString Msg, const bool Display);
	__fastcall virtual ~EZipBuilder(void) {}

	__property int Ident = {read=FResIdent, nodefault};

};

// new 1.73 handling of abort exceptions
class PACKAGE EZipAbort : public EZipBuilder
{
  public:
     __fastcall EZipAbort(void);
};

}	/* namespace Ezipbuilder */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Ezipbuilder;
#endif

#endif

