//---------------------------------------------------------------------------

#ifndef EnterKeyH
#define EnterKeyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmEnterKey : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *Edit2;
	TLabel *Label3;
	TEdit *Edit3;
	TLabel *Label4;
	TEdit *Edit4;
	TLabel *Label5;
	TEdit *Edit5;
	TLabel *Label6;
	TEdit *Edit6;
	TEdit *Edit7;
	TLabel *Label7;
	TBevel *Bevel1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TEdit *edRegKey;
	TBitBtn *BitBtn3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall Edit2KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall Edit3KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall Edit4KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall Edit5KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall Edit6KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall Edit7KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmEnterKey(TComponent* Owner);
	AnsiString CustomerKey;
	AnsiString RegistrationKey;
//	int ProductID;
	int Index;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEnterKey *frmEnterKey;
//---------------------------------------------------------------------------
#endif
