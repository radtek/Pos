//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TMemo *Memo1;
  TPanel *Panel1;
  TButton *Button1;
  TButton *Button2;
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
  TThread* Fzip;
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
  __property TThread* ZipThread={read=Fzip, write=Fzip};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

