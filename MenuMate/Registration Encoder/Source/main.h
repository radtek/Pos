//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
enum TProduct { prMenuMate=1, prPalmMate=2, prOffice=3 };
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPageControl *pcLicences;
	TTabSheet *tsSingleLicence;
	TTabSheet *tsMultipleLicences;
	TPanel *Panel1;
	TLabel *Label3;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit7;
	TLabel *Label12;
	TEdit *edRegKey;
	TLabel *Label5;
	TMemo *memCustomerKeys;
	TLabel *Label4;
	TMemo *memRegistrationKeys;
	TBitBtn *BitBtn1;
	TCheckBox *chbAddToMulti;
	TPanel *Panel2;
	TLabel *Label2;
	TEdit *edPOSLicences;
	TEdit *edPalmLicences;
	TLabel *Label1;
	TCheckBox *chbKitchenModule;
   TCheckBox *cbRooms;
	TCheckBox *cbTurnAroundTimes;
	TLabel *Label9;
	TEdit *edOfficeLicences;
	TCheckBox *cbMembership;
	TCheckBox *cbMYOB;
	TCheckBox *cbMenuMateDirect;
	TBitBtn *btnClear;
	TCheckBox *cbIntergratedEftPos;
	TCheckBox *cbExpires;
	TDateTimePicker *dtpExpiry;
	TCheckBox *chPhoenixSystem;
	void __fastcall FormCreate(TObject *Sender);
   void __fastcall memCustomerKeysChange(TObject *Sender);
   void __fastcall edPOSLicencesChange(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
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
	void __fastcall FormShow(TObject *Sender);
	void __fastcall pcLicencesChange(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
private:	// User declarations
	AnsiString KeyToFriendlyKey(AnsiString Key, /*int ProductID, */int Index);
	AnsiString CheckSum(AnsiString Key);
	void EncodeKey();
	void EncodeKeys();
public:		// User declarations
  __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
