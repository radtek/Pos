// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PaymentMaintenance.h"
#include "NewPaymentType.h"
#include "MMMessageBox.h"
#include "ListPaymentSystem.h"
#include "MMLogging.h"
#include "PaymentTypeGroupsGUI.h"
#include "GlobalSettings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "touchgrid"
#pragma resource "*.dfm"

TForm * TfrmPaymentMaintenance::WinOwner = NULL;

// ---------------------------------------------------------------------------
__fastcall TfrmPaymentMaintenance::TfrmPaymentMaintenance(TComponent* Owner, Database::TDBControl &inDBControl,
   TListPaymentSystem *inPaymentSystem) : TZForm(Owner), DBControl(inDBControl), PaymentSystem(inPaymentSystem)
{

   Color = clBlack;
   PaymentKey = 0;
   UpdateList();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::CreateParams(Controls::TCreateParams &params)
{
   TForm::CreateParams(params);
   if (WinOwner)
   {
      params.WndParent = WinOwner->Handle;
      WinOwner = NULL;
   }
}
// ---------------------------------------------------------------------------
TfrmPaymentMaintenance *TfrmPaymentMaintenance::Create(TForm* Owner, Database::TDBControl &inDBControl, TListPaymentSystem *inPaymentSystem)
{
   WinOwner = Owner;
   return new TfrmPaymentMaintenance(Owner, inDBControl, inPaymentSystem);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::FormResize(TObject *Sender)
{
  // if (Tag != Screen->Width)
  // {
      int Temp = Tag;
      Tag = Screen->Width;
      if((double)Screen->Width / Screen->Height < 1.4)
      {
      	ScaleBy(Screen->Width, Temp);
      }
  // }
   ClientWidth = Panel45->Left + Panel45->Width + Panel44->Left;
   ClientHeight = Panel45->Height + (Panel45->Top * 2);
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::imgExitClick(TObject *Sender)
{
   Close();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::pnlAddClick(TObject *Sender)
{
   std::auto_ptr <TfrmNewPaymentType> frmNewPaymentType(TfrmNewPaymentType::Create(this, DBControl, PaymentSystem, 0));
   frmNewPaymentType->Caption = "New Payment Type";
   frmNewPaymentType->ShowModal();
   UpdateList();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::tbtnEditClick(TObject *Sender)
{
   if (PaymentKey != 0)
   {
      std::auto_ptr <TfrmNewPaymentType> frmNewPaymentType(TfrmNewPaymentType::Create(this, DBControl, PaymentSystem, PaymentKey));
      frmNewPaymentType->Caption = "Editing Payment Type";
      frmNewPaymentType->ShowModal();
      UpdateList();
   }
   else
   {
      MessageBox("Please Select a Payment Type to work with.", "Error", MB_ICONWARNING + MB_OK);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::tbtnDeleteClick(TObject *Sender)
{
   if (PaymentKey != 0)
   {
      if (MessageBox("Are you sure you wish to remove this Payment Type?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
      {
	 Database::TDBTransaction DBTransaction(DBControl);
	 DBTransaction.StartTransaction();
	 PaymentSystem->PaymentDelete(DBTransaction, PaymentKey);
	 DBTransaction.Commit();
	 UpdateList();
      }
   }
   else
   {
      MessageBox("Please Select a Payment Type to work with.", "Error", MB_ICONWARNING + MB_OK);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::FormShow(TObject *Sender)
{
   FormResize(Sender);
   SetGridColors(tgridContainerList);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::pnlDefaultsClick(TObject *Sender)
{
   try
   {
      std::auto_ptr <TStringList> PaymentList(new TStringList);
      Database::TDBTransaction DBTransaction(DBControl);
      DBTransaction.StartTransaction();

      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
      DBTransaction.RegisterQuery(IBInternalQuery);

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
	  "INSERT INTO PAYMENTTYPES (" "PAYMENT_KEY, " "PAYMENT_NAME, " "PROPERTIES, " "COLOUR, " "ROUNDTO," "TAX_RATE,"
	  "DISPLAY_ORDER, INVOICE_EXPORT) " "VALUES (" "(SELECT GEN_ID(GEN_PAYMENTTYPES, 1) FROM RDB$DATABASE), " ":PAYMENT_NAME, " ":PROPERTIES, "
	  ":COLOUR, " ":ROUNDTO," ":TAX_RATE," ":DISPLAY_ORDER, :INVOICE_EXPORT)";

      int Properties = 0;
      if(!IsPaymentExist(DBTransaction,CASH))
      {
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = CASH;
          Properties |= ePayTypeOpensCashDrawer;
          Properties |= ePayTypeCash;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clGreen;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 0;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = 0.10;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }

      if(!IsPaymentExist(DBTransaction,"Cheque"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Cheque";
          Properties = 0;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clBlue;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 1;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }

      if(!IsPaymentExist(DBTransaction,"Eftpos"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Eftpos";
          Properties = 0;
          Properties |= ePayTypeAllowCashOut;
          Properties |= ePayTypeElectronicTransaction;
          Properties |= ePayTypeCheckAccepted;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clTeal;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 2;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }
      if(!IsPaymentExist(DBTransaction,"Amex"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Amex";
          Properties = 0;
          Properties |= ePayTypeElectronicTransaction;
          Properties |= ePayTypeCheckAccepted;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clAqua;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 3;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }

      if(!IsPaymentExist(DBTransaction,"Diners"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Diners";
          Properties = 0;
          Properties |= ePayTypeElectronicTransaction;
          Properties |= ePayTypeCheckAccepted;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clCream;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 4;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }

      if(!IsPaymentExist(DBTransaction,"Visa"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Visa";
          Properties = 0;
          Properties |= ePayTypeElectronicTransaction;
          Properties |= ePayTypeCheckAccepted;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clWhite;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 5;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }

      if(!IsPaymentExist(DBTransaction,"Master Card"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Master Card";
          Properties = 0;
          Properties |= ePayTypeElectronicTransaction;
          Properties |= ePayTypeCheckAccepted;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clOlive;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 6;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }

      if(!IsPaymentExist(DBTransaction,"Tips"))
      {
          IBInternalQuery->Close();
          IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Tips";
          Properties = 0;
          Properties |= ePayTypeCustomSurcharge;
          IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
          IBInternalQuery->ParamByName("COLOUR")->AsInteger = clGray;
          IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 7;
          IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
          IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 15;
          IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
          IBInternalQuery->ExecQuery();
      }
      DBTransaction.Commit();
      UpdateList();
   }
   catch(Exception & E)
   {
      MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
      TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmPaymentMaintenance::UpdateList()
{
   std::auto_ptr <TStringList> PaymentList(new TStringList);
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   PaymentSystem->PaymentLoad(DBTransaction, PaymentList.get());
   DBTransaction.Commit();
   tgridContainerList->RowCount = PaymentList->Count;

   for (int i = 0; i < PaymentList->Count; i++)
   {
      tgridContainerList->Buttons[i][0]->Caption = PaymentList->Strings[i];
      tgridContainerList->Buttons[i][0]->Tag = (int)PaymentList->Objects[i];
      if (tgridContainerList->Buttons[i][0]->Tag == PaymentKey)
      {
	 tgridContainerList->Buttons[i][0]->Latched = true;
      }
      else
      {
	 tgridContainerList->Buttons[i][0]->Latched = false;
      }
   }
}

void __fastcall TfrmPaymentMaintenance::tgridContainerListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
   PaymentKey = GridButton->Tag;
   tbEditPaymentType->Enabled = !(TGlobalSettings::Instance().LoyaltyMateEnabled  && (GridButton->Caption == "Gift Card" || GridButton->Caption == "Voucher" ));
   tbDeletePaymentType->Enabled = !(TGlobalSettings::Instance().LoyaltyMateEnabled  && (GridButton->Caption == "Gift Card" || GridButton->Caption == "Voucher" ));
}

void __fastcall TfrmPaymentMaintenance::tbtnPaymentGroupClick(TObject *Sender)
{
    std::auto_ptr <TfrmPaymentTypesGroupsGUI> PaymentTypesGroupsGUI(new TfrmPaymentTypesGroupsGUI(this));
    PaymentTypesGroupsGUI->Caption = "Add Payment Group";
    PaymentTypesGroupsGUI->ShowModal();
}
// ---------------------------------------------------------------------------
bool TfrmPaymentMaintenance::IsPaymentExist(Database::TDBTransaction &DBTransaction,AnsiString PaymentName)
{
    bool retVal = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
    IBInternalQuery->SQL->Text =  "SELECT PAYMENT_KEY FROM PAYMENTTYPES WHERE PAYMENT_NAME = :PAYMENT_NAME";
    IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = PaymentName;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
      retVal = true;
    }
    return retVal;
}
