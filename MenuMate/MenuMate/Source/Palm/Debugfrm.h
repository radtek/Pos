//---------------------------------------------------------------------------

#ifndef DebugfrmH
#define DebugfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmDebug : public TForm
{
__published:	// IDE-managed Components
	TListBox *lbBugs;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TComboBox *cbMessageType;
	TEdit *edTriggerCount;
	TEdit *edCount;
	TComboBox *cbErrorType;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TButton *Button4;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmDebug(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDebug *frmDebug;
//---------------------------------------------------------------------------
#endif
