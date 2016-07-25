//---------------------------------------------------------------------------

#ifndef ProcessWebOrderH
#define ProcessWebOrderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "WebMate.h"
#include "WebOrderContainer.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include "PaymentTransaction.h"
#include "ReqPrintJob.h"

//---------------------------------------------------------------------------

typedef void (__closure *func_ptr)();

class TfrmProcessWebOrder : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTouchBtn *btnClose;
	TTouchBtn *btnPrev;
	TTouchBtn *tbtnAcceptOrder;
	TTouchBtn *btnNext;
	TTouchBtn *btnBillUp;
	TTouchBtn *btnBillDown;
	TMemo *memReceipt;
	TTouchBtn *TouchButton1;
	TPanel *pnltitle;
	TTouchBtn *tchbtnEditOrders;
	void __fastcall btnCloseMouseClick(TObject *Sender);
	void __fastcall btnPrevMouseClick(TObject *Sender);
	void __fastcall btnNextMouseClick(TObject *Sender);
	void __fastcall btnBillUpMouseClick(TObject *Sender);
	void __fastcall btnBillDownMouseClick(TObject *Sender);
	void __fastcall tbtnAcceptOrderMouseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    void acceptWebOrder();
    void autoAcceptAllWebOrders();
    void startAcceptWebOrdersThread(bool acceptAll=false);
    void __fastcall autoAcceptWebOrdersTheadTerminate( TObject* sender );

    bool autoAcceptingWebOrders;
    std::auto_ptr<TfrmProcessing> frmProcessing;

protected:
   void __fastcall WebOrder(Messages::TMessage& Message);
   BEGIN_MESSAGE_MAP
   MESSAGE_HANDLER(WEBORDER_STATUSCHANGED, Messages::TMessage, WebOrder);
   END_MESSAGE_MAP(TComponent)


public:		// User declarations
   TWebOrderContainer WebOrderContainer;
	__fastcall TfrmProcessWebOrder(TComponent* Owner);
	void __fastcall UpdateDisplay();
	void __fastcall UpdateButtons();
	void ShowReceipt();
	void Execute();
    TChitNumber WebOrderChitNumber;
    TMMContactInfo MemberInfo;
};
//---------------------------------------------------------------------------
class TAcceptWebOrdersThread: public TThread
{
	private:
        TWebOrderContainer* container;
        bool acceptAll;

        void acceptWebOrder();
        void acceptAllWebOrders();

	protected:
		void __fastcall Execute();

	public:
		TAcceptWebOrdersThread(TWebOrderContainer* webOrderContainer, bool acceptAllWebOrders=false);
        TChitNumber ChitNumber;
        int WebOrderTabKey;


};
//---------------------------------------------------------------------------
#endif
