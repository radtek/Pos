//---------------------------------------------------------------------------
#ifndef PosMainH
#define PosMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <Graphics.hpp>

#include <ComCtrls.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdIPWatch.hpp"
#include <set>
#include <Graphics.hpp>
#include "ZForm.h"
#include "SystemEvents.h"
#include "Enum.h"
#include "DeviceRealTerminal.h"
#include "ChitNumberController.h"




//---------------------------------------------------------------------------
class TfrmPOSMain : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TTimer *tiLockout;
	TPanel *Panel2;
	TTouchBtn *tbMenuManager;
	TTouchBtn *tbAccountManager;
	TTouchBtn *tbtnBilling;
	TTouchBtn *tbtnTransfer;
	TTouchBtn *tbtnReprintOrders;
	TTouchBtn *tbtnReprintReceipt;
	TTouchBtn *btnMessage;
	TTouchBtn *tbEftPosManager;
	TPanel *Panel1;
	TTouchBtn *TouchBtn14;
	TTouchBtn *TouchBtn12;
	TTouchBtn *tbTableManager;
	TTouchBtn *tbSendHeldOrders;
        TTouchBtn *btnDelayedPayment;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbtnCloseClick(TObject *Sender);
	void __fastcall tbtnBillingClick(TObject *Sender);
	void __fastcall tbtnReprintOrdersClick(TObject *Sender);
	void __fastcall tbtnReprintReceiptClick(TObject *Sender);
	void __fastcall tbMenuManagerClick(TObject *Sender);
	void __fastcall tbtnTransferClick(TObject *Sender);
	void __fastcall btnMessageClick(TObject *Sender);
   void __fastcall tbtnExitPOSClick(TObject *Sender);
   void __fastcall tbAccountManagerClick(TObject *Sender);
	void __fastcall tbEftPosManagerClick(TObject *Sender);
	void __fastcall tbTableManagerMouseClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall tbtnSendHeldOrdersClick(TObject *Sender);
        void __fastcall btnDelayedPaymentMouseClick(TObject *Sender);

private:	// User declarations
	__fastcall TfrmPOSMain(TComponent* Owner,Database::TDBControl &inIBDatabase);
	
	AnsiString Strip(AnsiString inStr, AnsiString inStripStr);
	Database::TDBControl &DBControl;
    void ShowTableScreen();
    void ShowReprintReceipt();

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
public:		// User declarations

	float CurrStrToFloat(AnsiString inStr);
	bool ExitPOS;
	bool RedrawMenus;
	bool SendHeldOrders;
    bool ShowTablePicker;
    bool TransferSelected;
	void OnLockOutTimer(TSystemEvents *Sender);
    TChitNumber WebOrderChitNumber;
    TMMContactInfo MemberInfo;
    bool MenuEdited;
//   void CheckThatTheOrdersWorked(TList *Orders, AnsiString Msg);
};
//---------------------------------------------------------------------------
#endif
