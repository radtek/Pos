
//---------------------------------------------------------------------------
#ifndef HoldSendH
#define HoldSendH
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
class TfrmHoldSend : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel2;
	TTouchBtn *tbtnHold;
	TTouchBtn *tbtnSend;
	void __fastcall tbtnHoldMouseClick(TObject *Sender);
	void __fastcall tbtnSendMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	//int fRetVal;
public:		// User declarations
	__fastcall TfrmHoldSend(TComponent* Owner);
	//__property int RetVal = { read = fRetVal };
	int RetVal;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHoldSend *frmHoldSend;
//---------------------------------------------------------------------------
#endif
