//---------------------------------------------------------------------------

#ifndef EftPosDialogsH
#define EftPosDialogsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ListPaymentSystem.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmEftPos : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlSuccess;
	TLabel *lbeSuccess;
	TLabel *lbeElectronicTransType;
	TPanel *Panel7;
	TStaticText *stEftCashOut;
	TStaticText *stEFTMoney;
	TPanel *stEftType;
	TPanel *pnlSignature;
	TLabel *Label1;
	TTouchBtn *tbSigYes;
	TTouchBtn *tbSigNo;
	TTouchBtn *tbTranYes;
	TTouchBtn *tbTransNo;
   TPanel *Panel1;
   TStaticText *stEftTotal;
	void __fastcall tbSigYesClick(TObject *Sender);
	void __fastcall tbSigNoClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmEftPos(TComponent* Owner);
   TModalResult SignatureOk();
	TModalResult TransactionOk(TPayment *Payment);
	TModalResult TransactionOk(TPayment *Payment,AnsiString Caption);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
