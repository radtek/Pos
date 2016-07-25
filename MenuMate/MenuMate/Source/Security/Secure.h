//---------------------------------------------------------------------------
#ifndef SecureH
#define SecureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Contact.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
enum AccessResponse {arDenied,arGranted,arCanceled};

class TfrmSecurity : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel14;
	TPanel *btnTemplateAdmin;
	TPanel *btnTemplateSuper;
	TPanel *btnTemplateSales;
	TPanel *btnTemplateNone;
	TPanel *Panel2;
	TPanel *Panel13;
	TPanel *Panel1;
	TCheckBox *cbPOS;
	TCheckBox *cbSetup;
	TCheckBox *cbMaintain;
	TCheckBox *cbAccessZeds;
	TCheckBox *cbPINChange;
	TCheckBox *cbQuit;
	TCheckBox *cbMenus;
	TCheckBox *cbUsers;
	TCheckBox *cbPrinters;
	TCheckBox *cbDisableFD;
	TCheckBox *cdCredit;
	TCheckBox *cbPriceAj;
	TCheckBox *cbDiscount;
	TCheckBox *cbPaymentTypes;
	TCheckBox *cbSetFloat;
	TCheckBox *cbAdjustFloat;
	TCheckBox *cbSec1;
	TCheckBox *cbSec2;
	TCheckBox *cbSec3;
	TCheckBox *cbTabPINOverride;
	TCheckBox *cbAccountManager;
	TCheckBox *tbOpenDrawer;
	TCheckBox *tcbTransToInvoice;
	TCheckBox *tcbPayInvoice;
   TCheckBox *cbViewMembersDetials;
	TTouchBtn *TouchBtn1;
	TTouchBtn *btnMinimise;
	TCheckBox *tcbAccountCreation;
	TCheckBox *cbWriteOff;
    TCheckBox *cbReprintReceipt;
    TCheckBox *cbPaymentAccess;
    TCheckBox *cbTaxRemoval;
    TPanel *btnTemplateWaiter;
    TCheckBox *tcbViewTransAudit;
    TCheckBox *cbAllowPointsRefund;
    TCheckBox *cbAllowReprintOrders;
    TCheckBox *cbAllowForcedHappyHour;
   void __fastcall cbPOSClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
	void __fastcall cbMaintainClick(TObject *Sender);
	void __fastcall btnTemplateAdminClick(TObject *Sender);
	void __fastcall btnTemplateSuperClick(TObject *Sender);
	void __fastcall btnTemplateSalesClick(TObject *Sender);
	void __fastcall btnTemplateNoneClick(TObject *Sender);
	void __fastcall btnAcceptClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
    void __fastcall btnTemplateWaiterClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
public:
	TAccess CurrentUserAccess;
   TAccess TempAccess;
   __fastcall TfrmSecurity(TComponent* Owner);
    void AccessDenied();
    void LogOut();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSecurity *frmSecurity;
//---------------------------------------------------------------------------
#endif

