//---------------------------------------------------------------------------

#ifndef EftChequeVerifyH
#define EftChequeVerifyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmChequeVerfiy : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel27;
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *edChqAmount;
	TEdit *edChqSerial;
	TEdit *edChqBranch;
	TEdit *edChqAccount;
	TTouchBtn *TouchBtn1;
	TTouchBtn *TouchBtn2;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall Panel1Click(TObject *Sender);
	void __fastcall edChqSerialMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edChqBranchMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edChqAccountMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations

public:		// User declarations
	Currency ChequeAmount;
    __fastcall TfrmChequeVerfiy(TComponent* Owner);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
