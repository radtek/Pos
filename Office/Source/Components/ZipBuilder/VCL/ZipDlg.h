//---------------------------------------------------------------------------

#ifndef ZipDlgH
#define ZipDlgH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Dialogs.hpp>

namespace Zipdlg
{

const int PWLEN = 80;	// Maximum no. of characters in a password; Do not change!
const int zmtWarning  = 0x100000;
const int zmtError    = 0x110000;
const int zmtInformation = 0x120000;
const int zmtConfirmation = 0x130000;
const int zmtPassword = 0x140000;
const int NumBtns = 11;

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
class  TZipDialogBox : public TForm
{
private:
	TEdit* PwdEdit;
  TLabel* DxText;
  int ctx;
  SIZE AvDlgUnits;
  const char* IconID;
  int BeepID;
  int __fastcall GetDlgType(void);
  AnsiString __fastcall GetPWrd(void);
  void __fastcall SetPWrd(const AnsiString Value);
public:
	__fastcall TZipDialogBox( TComponent *Owner, int context);
	__fastcall virtual ~TZipDialogBox( void ) {};
		// And just to keep BCB1 happy...
	__fastcall virtual TZipDialogBox( TComponent *Owner ) : TForm( Owner ) { };
  void __fastcall Build(AnsiString Title, AnsiString msg,TMsgDlgButtons Btns);
  int __fastcall ShowModal(void);
  __property int DlgType={read= GetDlgType};
  __property AnsiString PWrd={read= GetPWrd, write= SetPWrd};



};

}/* namespace Zipdlg*/
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zipdlg;
#endif
#endif

