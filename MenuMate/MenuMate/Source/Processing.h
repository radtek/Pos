//---------------------------------------------------------------------------
#ifndef ProcessingH
#define ProcessingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

class TWaitForProcess : public TThread
{
private:
public:
};
//---------------------------------------------------------------------------
class TfrmProcessing : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TProgressBar *ProgressBar;
	TAnimate *Animate1;
	TLabel *Label2;
	TTouchBtn *btnCancel;
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
   __fastcall TfrmProcessing(TComponent* Owner);
	bool Closeable;
protected:
	__property Visible  = {write=SetVisible, default=0};
	void __fastcall Hide();
	void SetMessage(UnicodeString Message);
	UnicodeString GetMessage();
	void SetPosition(int iPos);
	void SetTitle(UnicodeString inTitle);
	UnicodeString GetTitle();
	void SetMax(int iMax);

public:		// User declarations
   void __fastcall Show();
	void __fastcall Close();
	__property UnicodeString Message = {write=SetMessage,read=GetMessage};
	__property UnicodeString Title = {write=SetTitle,read=GetTitle};
	__property int Position = {write=SetPosition};
   bool ShowProgress;
	__property int Max = {write=SetMax};
   bool Cancelled;
   bool CanCancel;
   bool Exclusive;
   __fastcall virtual ~TfrmProcessing();   // Used is other form Processes can steal focus of this one.
};
//---------------------------------------------------------------------------
#endif
