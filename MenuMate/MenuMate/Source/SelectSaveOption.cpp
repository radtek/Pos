//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectSaveOption.h"
#include "DeviceRealTerminal.h"
#include "DBTab.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmSelectSaveOption::TfrmSelectSaveOption(TComponent* Owner)
: TZForm(Owner)
{
  IsBillingMode = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSaveOption::FormShow(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    if(TfrmSelectSaveOption::Caption != "Select Transfer From" && TfrmSelectSaveOption::Caption != "Select Transfer To")
    {
        ShowTransferSection();
        tbDelayedPayments->Visible = !IsBillingMode && TGlobalSettings::Instance().TransferTableOnPrintPrelim &&
                                     TDBTab::GetDelyedInvoiceNumberCount(DBTransaction) > 0;
        tbNewInvoice->Visible = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"] &&
                                 (IsBillingMode || !TGlobalSettings::Instance().DisableInternalInvoicing);
        tbWebOrders->Visible = IsBillingMode &&
                               TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Registered"];
        DBTransaction.Commit();
    }
    else
    {
       if(TfrmSelectSaveOption::Caption == "Select Transfer From")
       {
         ShowTransferSection();
         tbNewInvoice->Visible = false;
         HideForTransferSection();
       }
       else
       {
          ShowTransferSection();
          tbNewInvoice->Visible = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"] &&
                                 (IsBillingMode || !TGlobalSettings::Instance().DisableInternalInvoicing);
          HideForTransferSection();
       }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	ClientWidth = this->BorderWidth + Panel2->Width + Panel1->Width;
	ClientHeight = Panel2->Height + (Panel2->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbTabsMouseClick(TObject *Sender)
{
  SelectedTabType = TabNormal;
  DisplayMode = eTabs;
  Title = "Tabs";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbTablesMouseClick(TObject *Sender)
{
  SelectedTabType = TabTableSeat;
  DisplayMode = eTables;
  Title = "Tables";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbDelayedPaymentsMouseClick(TObject *Sender)
{
  SelectedTabType = TabDelayedPayment;
  DisplayMode = eTabs;
  Title = "Tabs";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbStafftabsMouseClick(TObject *Sender)
{
  SelectedTabType = TabStaff;
  DisplayMode = eTabs;
  Title = "Staff Tabs";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbRoomsMouseClick(TObject *Sender)
{
  SelectedTabType = TabRoom;
  DisplayMode = eRooms;
  Title = "Rooms";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbMembersTabMouseClick(TObject *Sender)
{
  SelectedTabType = TabMember;
  DisplayMode = eTabs;
  Title = "Members Tab";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbNewInvoiceMouseClick(TObject *Sender)
{
  SelectedTabType = TabInvoice;
  DisplayMode = eInvoices;
  Title = "Invoice";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::btnOkMouseClick(TObject *Sender)
{
  SelectedTabType = TabNone;
  Title = "";
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbWebOrdersMouseClick(TObject *Sender)
{
  SelectedTabType = TabWeb;
  DisplayMode = eTab;
  Title = "Web Orders";
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectSaveOption::tbClippTabsMouseClick(TObject *Sender)
{
  SelectedTabType = TabClipp;
  DisplayMode = eTab;
  Title = "Clipp Tabs";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void TfrmSelectSaveOption::ShowTransferSection()
{
   tbTabs->Visible = TGlobalSettings::Instance().TabsEnabled;
   tbTables->Visible = TGlobalSettings::Instance().TablesEnabled;
   tbStafftabs->Visible = TGlobalSettings::Instance().TabsEnabled;
   tbRooms->Visible = TDeviceRealTerminal::Instance().Modules.Status[eRegRooms]["Registered"];
   tbMembersTab->Visible = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"];
   tbClippTabs->Visible =  TGlobalSettings::Instance().IsClippIntegrationEnabled;
}

//---------------------------------------------------------------------------
void TfrmSelectSaveOption::HideForTransferSection()
{
   tbDelayedPayments->Visible = false;
   tbWebOrders->Visible = false;
}

//---------------------------------------------------------------------------

