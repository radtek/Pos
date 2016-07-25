//---------------------------------------------------------------------------
#ifndef GetMoneyH
#define GetMoneyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <set>
#include <iterator>
#include "PaymentTransaction.h"
#include <memory>
#include "DrinkCommandManager.h"
#include "ZForm.h"
//---------------------------------------------------------------------------
enum eTransactionStages {eSTAGE0,eSTAGE1,eSTAGE2,eSTAGE3};
class TPayment;
//enum eUserOptions{eNone,eClose,eCloseandPrint,eBack};


class TfrmControlTransaction : public TZForm
{
   friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlPayment;
	TPanel *Panel1;
	TPanel *pnlCashOutTitle;
   TPanel *pnlPaymentAmount;
	TPanel *Panel3;
	TTouchBtn *btnCloseBill;
	TTouchBtn *btnPrintBill;
   TStringGrid *sgPay;
	TTouchBtn *btnBack;
	TPanel *Panel25;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCloseBillClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall stPrintBillClick(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnBackClick(TObject *Sender);
    void __fastcall sgPayDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
private:	// User declarations
	TPaymentTransaction &CurrentTransaction;
	//__fastcall TfrmControlTransaction(TComponent* Owner,TPaymentTransaction &inCurrentTransaction);

	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);

    Currency getTotalTaxFromTransaction( TPaymentTransaction &inCurrentTransaction );
    Currency getServiceChargeFromTransaction( TPaymentTransaction &inCurrentTransaction );
    Currency Pts;
    Currency BillPts;
    UnicodeString MemberNo;
    Currency RefundPts;
    Currency Points;
    double PtsRefund , PtsBuy;
    AnsiString PointsBuy;

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)

public:		// User declarations
    __fastcall TfrmControlTransaction(TComponent* Owner,TPaymentTransaction &inCurrentTransaction);
	static TfrmControlTransaction *Create(TForm* Owner,TPaymentTransaction &inCurrentTransaction);

   void Reset();

   eUserOptions RetrieveUserOptions();
   eUserOptions CloseBill();
   eUserOptions UserOption;
};
//---------------------------------------------------------------------------
#endif
