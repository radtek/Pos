//---------------------------------------------------------------------------

#ifndef DropDownVarH
#define DropDownVarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

#include <vector>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TddvBtn
{
	public :
	TddvBtn();
	TddvBtn(AnsiString BtnName, int RetVal);

	AnsiString ButtonCaption;
	int ReturnVal;
};

class TfrmDropDownVar : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlCourses;
	TScrollBox *sbMessages;
	TTouchBtn *btnMsgUp;
	TTouchBtn *btnMsgDown;
	TPanel *Panel1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnMsgUpMouseClick(TObject *Sender);
	void __fastcall btnMsgDownMouseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void __fastcall BtnExeClick(TObject *Sender);
	std::vector<TddvBtn> Buttons;
	__fastcall TfrmDropDownVar(TComponent* Owner);
public:		// User declarations

	int Result;
	bool ShowCloseButton;
	void Clear();
	void AddButton(TddvBtn Btn);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDropDownVar *frmDropDownVar;
//---------------------------------------------------------------------------

class TExeBtnVar : public TTouchBtn//TCustomStaticText
{
private:
public:
   __fastcall TExeBtnVar(Classes::TComponent* AOwner);
   AnsiString Title;
   AnsiString TextResult;
   int Result;
   bool Selected;
};

#endif
