//---------------------------------------------------------------------------

#ifndef DropDownH
#define DropDownH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

#include <map>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TFunctionEvent)();
//---------------------------------------------------------------------------

class TfrmDropDownFunc : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlCourses;
	TScrollBox *sbMessages;
	TTouchBtn *btnMsgUp;
	TTouchBtn *btnMsgDown;
	void __fastcall btnMsgDownMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall pnlCoursesResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall btnMsgUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void __fastcall BtnExeClick(TObject *Sender);

	__fastcall TfrmDropDownFunc(TComponent* Owner);
	bool CreateCancel;
	void CreateCancelBtn();
public:		// User declarations
	TFunctionEvent FunctionToCall;
	void Clear();
	void AddButton(AnsiString ButtonName,TFunctionEvent FunctionToCall);
	void RemoveCancelBtn(){CreateCancel = false;}
    std::map<AnsiString,TFunctionEvent> Buttons;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDropDownFunc *frmDropDownFunc;
//---------------------------------------------------------------------------

class TExeBtn : public TTouchBtn//TCustomStaticText
{
private:
public:
	__fastcall TExeBtn(Classes::TComponent* AOwner);
   AnsiString Title;
	AnsiString TextResult;
	TFunctionEvent FunctionToCall;
   bool Selected;
};

#endif
