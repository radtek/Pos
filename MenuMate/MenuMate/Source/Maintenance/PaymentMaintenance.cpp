// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PaymentMaintenance.h"
#include "NewPaymentType.h"
#include "MMMessageBox.h"
#include "ListPaymentSystem.h"
#include "MMLogging.h"
#include "PaymentTypeGroupsGUI.h"
#include "MMTouchKeyboard.h"
#include "GlobalSettings.h"
#include "StringTools.h"
//#include "ManagerPanasonic.h"
#include "ManagerPMSCodes.h"
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
      int PayKey = 0;
      AnsiString Properties = "";
      std::auto_ptr <TStringList> PaymentList(new TStringList);
      std::vector <UnicodeString> PayTypes;
      Database::TDBTransaction DBTransaction(DBControl);
      DBTransaction.StartTransaction();

      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
      DBTransaction.RegisterQuery(IBInternalQuery);

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
	  "INSERT INTO PAYMENTTYPES (" "PAYMENT_KEY, " "PAYMENT_NAME, " "PROPERTIES, " "COLOUR, " "ROUNDTO," "TAX_RATE,"
	  "DISPLAY_ORDER, INVOICE_EXPORT) " "VALUES (" ":PAYMENT_KEY, " ":PAYMENT_NAME, " ":PROPERTIES, "
	  ":COLOUR, " ":ROUNDTO," ":TAX_RATE," ":DISPLAY_ORDER, :INVOICE_EXPORT)";

      if(!IsPaymentExist(DBTransaction,CASH))
      {
          if(!PaymentExistsInPMS(DBTransaction, CASH))
          {
              PayKey = GeneratePaymentKey(DBTransaction);
              Properties = "-" + IntToStr(ePayTypeOpensCashDrawer) + "-" + IntToStr(ePayTypeCash)  + "-";
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = CASH;
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clGreen;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 0;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = 0.10;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeOpensCashDrawer);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCash);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");

              MakePaymentinPMS(DBTransaction,CASH, false,PayKey);
          }
          else
          {
              MessageBox("Cash Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }

      if(!IsPaymentExist(DBTransaction,"Cheque"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "Cheque"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Cheque";
              Properties = "-0-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clBlue;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 1;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              MakePaymentinPMS(DBTransaction,"Cheque", false,PayKey);
          }
          else
          {
              MessageBox("Cheque Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }

      if(!IsPaymentExist(DBTransaction,"EFTPOS"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "EFTPOS"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "EFTPOS";
              Properties = "-" + IntToStr(ePayTypeAllowCashOut) + "-" + IntToStr(ePayTypeElectronicTransaction)  + "-" + IntToStr(ePayTypeCheckAccepted) +"-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clTeal;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 2;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeAllowCashOut);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeElectronicTransaction);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCheckAccepted);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              MakePaymentinPMS(DBTransaction,"EFTPOS", true,PayKey);
          }
          else
          {
              MessageBox("EFTPOS Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }
      if(!IsPaymentExist(DBTransaction,"AMEX"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "AMEX"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "AMEX";
              Properties = "-" + IntToStr(ePayTypeElectronicTransaction) + "-" + IntToStr(ePayTypeCheckAccepted)  + "-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clAqua;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 3;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeElectronicTransaction);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCheckAccepted);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              MakePaymentinPMS(DBTransaction,"AMEX", true,PayKey);
          }
          else
          {
              MessageBox("AMEX Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }

      if(!IsPaymentExist(DBTransaction,"DINERS"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "DINERS"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "DINERS";
              Properties = "-" + IntToStr(ePayTypeElectronicTransaction) + "-" + IntToStr(ePayTypeCheckAccepted)  + "-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clCream;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 4;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeElectronicTransaction);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCheckAccepted);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              MakePaymentinPMS(DBTransaction,"DINERS", true,PayKey);
          }
          else
          {
              MessageBox("DINERS Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }

      if(!IsPaymentExist(DBTransaction,"VISA"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "VISA"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "VISA";
              Properties = "-" + IntToStr(ePayTypeElectronicTransaction) + "-" + IntToStr(ePayTypeCheckAccepted)  + "-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clWhite;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 5;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeElectronicTransaction);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCheckAccepted);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              MakePaymentinPMS(DBTransaction,"VISA", true,PayKey);
          }
          else
          {
              MessageBox("VISA Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }

      if(!IsPaymentExist(DBTransaction,"MASTERCARD"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "MASTERCARD"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "MASTERCARD";
              Properties = "-" + IntToStr(ePayTypeElectronicTransaction) + "-" + IntToStr(ePayTypeCheckAccepted)  + "-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clOlive;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 6;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeElectronicTransaction);
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCheckAccepted);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              MakePaymentinPMS(DBTransaction,"MASTERCARD", true,PayKey);
          }
          else
          {
              MessageBox("MASTERCARD Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }

      if(!IsPaymentExist(DBTransaction,"Tips"))
      {
          if(!PaymentExistsInPMS(DBTransaction, "Tips"))
          {
              IBInternalQuery->Close();
              PayKey = GeneratePaymentKey(DBTransaction);
              IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PayKey;
              IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Tips";
              Properties = "-" + IntToStr(ePayTypeCustomSurcharge) + "-";
              IBInternalQuery->ParamByName("PROPERTIES")->AsString = Properties;
              IBInternalQuery->ParamByName("COLOUR")->AsInteger = clGray;
              IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 7;
              IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = MIN_CURRENCY_VALUE;
              IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 15;
              IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
              IBInternalQuery->ExecQuery();
              PaymentSystem->SetPaymentAttribute(DBTransaction,PayKey,ePayTypeCustomSurcharge);

//              if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//                PayTypes.push_back("*" + IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString + "*");
              // Not required for Tips as they already are present as separate enitity under PMS
              //MakePaymentinPMS(DBTransaction,"Tips", false,PayKey);
          }
          else
          {
              MessageBox("Tips Payment Name already exists under PMS Payment Types.", "Error", MB_OK);
          }
      }
      DBTransaction.Commit();
      UpdateList();

//     if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//     {
//        PaymentSystem->InsertPaymentTypeInPanasonicDB(PayTypes);
//     }
   }
   catch(Exception & E)
   {
      MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
      TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}
// ---------------------------------------------------------------------------
int TfrmPaymentMaintenance::GeneratePaymentKey(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *IBInternalQuery  = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PAYMENTTYPES, 1) FROM RDB$DATABASE";
    IBInternalQuery->ExecQuery();
    return IBInternalQuery->Fields[0]->AsInteger;
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
   btnGlCode->Visible = ( (TGlobalSettings::Instance().LoyaltyMateEnabled  && (GridButton->Caption == "Gift Card" || GridButton->Caption == "Voucher" ))
                        ||
                        (TGlobalSettings::Instance().EnableOnlineOrdering  && (GridButton->Caption == "WAITERAPP" )) );

   btnGlCode->Caption = "GL Code\r" + GetGlCode(PaymentKey);;

     tbEditPaymentType->Enabled = !((TGlobalSettings::Instance().LoyaltyMateEnabled  && (GridButton->Caption == "Gift Card" || GridButton->Caption == "Voucher" ))
     ||
     (TGlobalSettings::Instance().EnableOnlineOrdering  && (GridButton->Caption == "WAITERAPP" )));
     tbDeletePaymentType->Enabled = !((TGlobalSettings::Instance().LoyaltyMateEnabled  && (GridButton->Caption == "Gift Card" || GridButton->Caption == "Voucher" ))
      ||
     (TGlobalSettings::Instance().EnableOnlineOrdering  && (GridButton->Caption == "WAITERAPP" )));
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
    IBInternalQuery->SQL->Text =  "SELECT PAYMENT_KEY FROM PAYMENTTYPES WHERE UPPER(PAYMENT_NAME) = :PAYMENT_NAME"
                                  " OR PAYMENT_NAME = :MODIFIEDPAYMENTNAME ";
    IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = PaymentName.UpperCase();
    IBInternalQuery->ParamByName("MODIFIEDPAYMENTNAME")->AsString = TStringTools::Instance()->UpperCaseWithNoSpace(PaymentName);
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
      retVal = true;
    }
    return retVal;
}
//-----------------------------------------------------------------------------
bool TfrmPaymentMaintenance::PaymentExistsInPMS(Database::TDBTransaction &DBTransaction,AnsiString PaymentName)
{
    bool retVal = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
    IBInternalQuery->SQL->Text =  "SELECT PMS_PAYTYPE_ID FROM PMSPAYMENTSCONFIG WHERE UPPER(PMS_PAYTYPE_NAME) = :PMS_PAYTYPE_NAME"
                                  " OR PMS_PAYTYPE_NAME = :MODIFIEDPAYMENTNAME ";
    IBInternalQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString = PaymentName.UpperCase();
    IBInternalQuery->ParamByName("MODIFIEDPAYMENTNAME")->AsString = TStringTools::Instance()->UpperCaseWithNoSpace(PaymentName);
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
      retVal = true;
    }
    return retVal;
}
//-----------------------------------------------------------------------------
AnsiString TfrmPaymentMaintenance::GetGlCode(int paymentKey)
{
    AnsiString retVal = "";
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
    IBInternalQuery->SQL->Text =  "SELECT GL_CODE FROM PAYMENTTYPES WHERE PAYMENT_KEY = :PAYMENT_KEY ";
    IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = paymentKey;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
      retVal = IBInternalQuery->FieldByName("GL_CODE")->AsString;
    }
    DBTransaction.Commit();
    return retVal;
}

void TfrmPaymentMaintenance::UpdateGlCode(AnsiString GlCode,int paymentKey)
{
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
        IBInternalQuery->SQL->Text =  "UPDATE PAYMENTTYPES SET GL_CODE = :GL_CODE WHERE PAYMENT_KEY = :PAYMENT_KEY ";
        IBInternalQuery->ParamByName("GL_CODE")->AsString = GlCode;
        IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = paymentKey;
        IBInternalQuery->ExecQuery();
        DBTransaction.Commit();
   }
   catch(Exception & E)
   {
        DBTransaction.Rollback();
   }
}

void __fastcall TfrmPaymentMaintenance::btnGlCodeMouseClick(TObject *Sender)
{
   if (PaymentKey != 0)
   {

        std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
        frmTouchKeyboard->MaxLength = 30;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = false;
        frmTouchKeyboard->KeyboardText = GetGlCode(PaymentKey);
        frmTouchKeyboard->Caption = "Enter GL Code";
        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
           UpdateGlCode(frmTouchKeyboard->KeyboardText.Trim(),PaymentKey);
           btnGlCode->Caption = "GL Code\r" + frmTouchKeyboard->KeyboardText.Trim();
        }
        UpdateList();
   }
   else
   {
      MessageBox("Please Select a Payment Type to work with.", "Error", MB_ICONWARNING + MB_OK);
   }
}
//---------------------------------------------------------------------------
void TfrmPaymentMaintenance::MakePaymentinPMS(Database::TDBTransaction &DBTransaction,AnsiString name, bool isElectronicPayment,int key)
{
    TPMSPaymentType pmsPaymentType;
    pmsPaymentType.PMSPayTypeName = name;
    pmsPaymentType.PMSPayTypeCode = "";
    pmsPaymentType.PMSPayTypeCategory = eMMCategory;
    pmsPaymentType.PMSMMPayTypeLink = key;
    pmsPaymentType.isElectronicPayment = isElectronicPayment;
    std::auto_ptr<TManagerPMSCodes> managerPMSCodes(new TManagerPMSCodes());
    managerPMSCodes->SetPMSPaymentType(DBTransaction,pmsPaymentType,true, true);
}
//---------------------------------------------------------------------------

