//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#ifdef PalmMate
	#include "Palm.h"
#endif

#include "Secure.h"
#include "Login.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#define Security 1
TfrmSecurity *frmSecurity;
//---------------------------------------------------------------------------
__fastcall TfrmSecurity::TfrmSecurity(TComponent* Owner)
   : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::FormCreate(TObject *Sender)
{
   CurrentUserAccess = 0;
   TempAccess = 0;
//   CurrentUserID = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::cbPOSClick(TObject *Sender)
{
   if (cbPOS->Checked == true)
   {
   	cbDisableFD->Enabled = true;
	cdCredit->Enabled = true;
   	cbPriceAj->Enabled = true;
	  cbDiscount->Enabled = true;
	  cbWriteOff->Enabled = true;
   }
   else
   {
   	cbDisableFD->Checked = false;
	cdCredit->Checked = false;
   	cbPriceAj->Checked = false;
	  cbDiscount->Checked = false;
	  cbWriteOff->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::cbMaintainClick(TObject *Sender)
{
   if (cbMaintain->Checked == true)
   {
   	cbMenus->Enabled = true;
   	cbUsers->Enabled = true;
   	cbPrinters->Enabled = true;
   }
   else
   {
   	cbMenus->Checked = false;
   	cbUsers->Checked = false;
   	cbPrinters->Checked = false;

   	cbMenus->Enabled = false;
   	cbUsers->Enabled = false;
   	cbPrinters->Enabled = false;
   }
}

//---------------------------------------------------------------------------

void __fastcall TfrmSecurity::FormShow(TObject *Sender)
{
	FormResize(NULL);
   cbPOS->Checked					= TempAccess.POS;
  	cbDisableFD->Checked		   = TempAccess.DisableFoodDrink;
	cdCredit->Checked			   = TempAccess.Credit;
	cbPriceAj->Checked		   = TempAccess.PriceAj;
  	cbDiscount->Checked		   = TempAccess.DiscountBill;
   cbAccessZeds->Checked		= TempAccess.PrintZeds;
   cbSetFloat->Checked			= TempAccess.SetFloat;
   cbAdjustFloat->Checked		= TempAccess.AdjustFloat;
   cbMaintain->Checked			= TempAccess.Maintenance;
	cbMenus->Checked				= TempAccess.MenuEditor;
	cbUsers->Checked				= TempAccess.Users;
	cbPrinters->Checked			= TempAccess.Printers;
	cbSetup->Checked				= TempAccess.Setup;
//   cbReservations->Checked		= TempAccess.Reservations;
   cbPINChange->Checked			= TempAccess.PINChange;
   cbQuit->Checked				= TempAccess.Quit;
   cbPaymentTypes->Checked    = TempAccess.PaymentTypes;
   cbSec1->Checked            = TempAccess.PaymentTypesSec1;
   cbSec2->Checked            = TempAccess.PaymentTypesSec2;
   cbSec3->Checked            = TempAccess.PaymentTypesSec3;
   cbTabPINOverride->Checked  = TempAccess.TabPINOverride;
   cbAccountManager->Checked  = TempAccess.AccountManager;
   cbViewMembersDetials->Checked = TempAccess.ViewMembersDetials;
	tbOpenDrawer->Checked		= TempAccess.OpenDrawer;
	tcbPayInvoice->Checked		= TempAccess.PayInvoice;
	tcbTransToInvoice->Checked = TempAccess.ChargeToInvoice;
	tcbAccountCreation->Checked = TempAccess.AccountCreation;
	cbWriteOff->Checked			= TempAccess.WriteOff;
    cbReprintReceipt->Checked =   TempAccess.ReprintReceipt;
    cbPaymentAccess->Checked =   TempAccess.PaymentAccess;
    cbTaxRemoval->Checked =   TempAccess.TaxRemoval;
    tcbViewTransAudit->Checked    = TempAccess.TransactionAuditAccess;
    cbAllowPointsRefund->Checked = TempAccess.AllowPointsRefund;
    cbAllowReprintOrders->Checked = TempAccess.AllowReprintOrders;
    cbAllowForcedHappyHour->Checked = TempAccess.AllowForcedHappyHour;

}
//---------------------------------------------------------------------------
void TfrmSecurity::AccessDenied()
{
#ifdef Security
	TDeviceRealTerminal::Instance().User.Clear();
	#ifndef PalmMate
		MessageBox("Access Denied.", "Access Denied.", MB_OK + MB_ICONERROR);
   #endif
#endif
}
//---------------------------------------------------------------------------
void TfrmSecurity::LogOut()
{
   TDeviceRealTerminal::Instance().User.Clear();
   CurrentUserAccess = 0;
   TempAccess = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::btnTemplateAdminClick(TObject *Sender)
{
	TempAccess = mmAdmin;
   FormShow(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::btnTemplateSuperClick(TObject *Sender)
{
	TempAccess = mmPowerUser;
   FormShow(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::btnTemplateSalesClick(TObject *Sender)
{
	TempAccess = mmSales;
   FormShow(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSecurity::btnTemplateWaiterClick(TObject *Sender)
{
   TempAccess = mmWaiter;
   FormShow(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::btnTemplateNoneClick(TObject *Sender)
{
	TempAccess = 0;
   FormShow(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::btnAcceptClick(TObject *Sender)
{
   TempAccess.POS						= cbPOS->Checked;
  	TempAccess.DisableFoodDrink	= cbDisableFD->Checked;
	TempAccess.Credit					= cdCredit->Checked;
  	TempAccess.PriceAj				= cbPriceAj->Checked;
  	TempAccess.DiscountBill			= cbDiscount->Checked;
   TempAccess.AdjustFloat        = cbAdjustFloat->Checked;
   TempAccess.SetFloat           = cbSetFloat->Checked;
   TempAccess.PrintZeds				= cbAccessZeds->Checked;
   TempAccess.Maintenance			= cbMaintain->Checked;
   TempAccess.MenuEditor			= cbMenus->Checked;
   TempAccess.Users					= cbUsers->Checked;
   TempAccess.Printers				= cbPrinters->Checked;
   TempAccess.Setup					= cbSetup->Checked;
//   TempAccess.Reservations		= cbReservations->Checked;
   TempAccess.PINChange				= cbPINChange->Checked;
   TempAccess.Quit					= cbQuit->Checked;
   TempAccess.PaymentTypes			= cbPaymentTypes->Checked;
   TempAccess.PaymentTypesSec1	= cbSec1->Checked;
   TempAccess.PaymentTypesSec2	= cbSec2->Checked;
   TempAccess.PaymentTypesSec3	= cbSec3->Checked;
   TempAccess.TabPINOverride		= cbTabPINOverride->Checked;
   TempAccess.AccountManager		= cbAccountManager->Checked;
    TempAccess.ViewMembersDetials       = cbViewMembersDetials->Checked;
	TempAccess.OpenDrawer			= tbOpenDrawer->Checked;
	TempAccess.PayInvoice			= tcbPayInvoice->Checked;
	TempAccess.ChargeToInvoice		= tcbTransToInvoice->Checked;
	TempAccess.AccountCreation		= tcbAccountCreation->Checked;
	TempAccess.WriteOff				= cbWriteOff->Checked;
    TempAccess.ReprintReceipt = cbReprintReceipt->Checked;
    TempAccess.PaymentAccess = cbPaymentAccess->Checked;
    TempAccess.TaxRemoval = cbTaxRemoval->Checked;
    TempAccess.TransactionAuditAccess   = tcbViewTransAudit->Checked;
    TempAccess.AllowPointsRefund    = cbAllowPointsRefund->Checked;
    TempAccess.AllowReprintOrders   =cbAllowReprintOrders->Checked;
     TempAccess.AllowForcedHappyHour   =cbAllowForcedHappyHour->Checked;

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSecurity::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
        if((double)Screen->Width / Screen->Height < 1.4)
        {
   		ScaleBy(Screen->Width, Temp);
        }
   }
   ClientWidth = Panel1->Width + Panel13->Width + BorderWidth;
   ClientHeight = Panel1->Height + Panel14->Height + BorderWidth;
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}

