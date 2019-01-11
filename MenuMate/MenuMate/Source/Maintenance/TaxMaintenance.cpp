//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TaxMaintenance.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "FiscalPrinterAdapter.h"
#include "SelectDish.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmTaxMaintenance::TfrmTaxMaintenance(TComponent* Owner, Database::TDBControl &inDBControl)
    : TZForm(Owner), DBControl(inDBControl)
{
    responseMessage = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::FormShow(TObject *Sender)
{
    //Populate Service Charge Combo Box
    ReadVariablesFromDatabase();
    loadTaxProfiles(cbTaxProfile->Items);
    loadTaxProfiles(cbTaxProfileRounding->Items);
    bool usingServiceCharge = TGlobalSettings::Instance().UsingServiceCharge;

    //Fill all the form from global settings variables
    cbPriceIncludeTax->Checked                    = TGlobalSettings::Instance().ItemPriceIncludeTax;
    cbPriceIncludesServiceCharge->Checked         = TGlobalSettings::Instance().ItemPriceIncludeServiceCharge;
    cbCalculateTaxPreDiscount->Checked            = !TGlobalSettings::Instance().ReCalculateTaxPostDiscount;
    cbCalculateTaxPostDiscount->Checked           = TGlobalSettings::Instance().ReCalculateTaxPostDiscount;
    cbCalculateServiceChargePreDiscount->Checked  = !TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && usingServiceCharge;
    cbCalculateServiceChargePostDiscount->Checked = TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && usingServiceCharge;
    cbApplyTaxToServiceCharge->Checked            = TGlobalSettings::Instance().ApplyServiceChargeTax;
    cbPostsaletofiscal->Checked                   = TGlobalSettings::Instance().IsFiscalPostingDisable;
    if(cbCalculateServiceChargePostDiscount->Checked)
    {
        cbCalculateServiceChargePreDiscount->Checked = false;
        cbCalculateServiceChargePreDiscount->Enabled = false;
    }
    else if(cbCalculateServiceChargePreDiscount->Checked)
    {
        cbCalculateServiceChargePostDiscount->Enabled = false;
    }

    if(cbApplyTaxToServiceCharge->Checked)
    {
        cbTaxProfile->Enabled = true;
        cbTaxProfile->ItemIndex = getTaxProfileIndexFromKey(TGlobalSettings::Instance().ServiceChargeTaxProfileKey);
    }
    cbApplyTaxToRounding->Checked            = TGlobalSettings::Instance().ApplyRoundingTax;
    if(cbApplyTaxToRounding->Checked)
    {
        cbTaxProfileRounding->Enabled = true;
        cbTaxProfileRounding->ItemIndex = getTaxProfileIndexFromKey(TGlobalSettings::Instance().RoundingTaxProfileKey);
    }
    cbUseItalyFiscalPrinter->OnClick = NULL;
    //edPrinterName->OnClick = NULL;
    cbUseItalyFiscalPrinter->Checked = TGlobalSettings::Instance().UseItalyFiscalPrinter;
    cbUseItalyFiscalPrinter->OnClick =  cbUseItalyFiscalPrinterClick;
    edPrinterName->Text = TGlobalSettings::Instance().PrinterType;
    edLogicalName->Text = TGlobalSettings::Instance().PrinterlogicalName;
    if(cbUseItalyFiscalPrinter->Checked)
    {
        edPrinterName->Enabled = true;
        edLogicalName->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::FormResize(TObject *Sender)
{
  //	if (Tag != Screen->Width)
  //	{
		int Temp = Tag;
		Tag = Screen->Width;
        if((double)Screen->Width/Screen->Height < 1.4)
        {
			ScaleBy(Screen->Width, Temp);
        }
   //	}
    /*
	pnlButtons->Top = Panel6->Left;
	pnlButtons->Left = ClientWidth - pnlButtons->Width - Panel6->Left;
	pnlButtons->Height = ClientHeight - (Panel6->Left*2);
	Panel6->Top = Panel6->Left;
	Panel6->Width = pnlButtons->Left - (Panel6->Left*2);
	Panel6->Height = ClientHeight - (Panel6->Left*2);
    */
    //add frn
    pnlButtons->Top = Pages->Left;
	pnlButtons->Left = ClientWidth - pnlButtons->Width - Pages->Left;
	pnlButtons->Height = ClientHeight - (Pages->Left*2);
	Pages->Top = Pages->Left;
	Pages->Width = pnlButtons->Left - (Pages->Left*2);
	Pages->Height = ClientHeight - (Pages->Left*2);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::tbtnOkMouseClick(TObject *Sender)
{
    if(cbApplyTaxToServiceCharge->Checked && cbTaxProfile->ItemIndex == -1)
    {
        MessageBox("Service charge tax must be selected.", "Error", MB_OK + MB_ICONERROR);
    }
    else
    {
        if(cbApplyTaxToRounding->Checked && cbTaxProfileRounding->ItemIndex == -1)
        {
            MessageBox("Rounding tax must be selected.", "Error", MB_OK + MB_ICONERROR);
        }
        else if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
        {
            std::auto_ptr<TFiscalPrinterAdapter> fiscalAdapter(new TFiscalPrinterAdapter());
            responseMessage = fiscalAdapter->GetFiscalPrinterStatus();
            if(responseMessage.Pos("OK") == 0)
            {
                MessageBox("Set The Fiscal Printer first","Error",MB_ICONERROR + MB_OK);
                TGlobalSettings::Instance().UseItalyFiscalPrinter = false;
                Database::TDBTransaction DBTransaction(DBControl);
                DBTransaction.StartTransaction();
                TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmUseItalyFiscalPrinter,TGlobalSettings::Instance().UseItalyFiscalPrinter);
                DBTransaction.Commit();
            }
            SyncTaxSettingWithWeb();
            Close();
        }
        else
        {
            SyncTaxSettingWithWeb();
            Close();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbPriceIncludeTaxClick(TObject *Sender)
{
    TGlobalSettings::Instance().ItemPriceIncludeTax = cbPriceIncludeTax->Checked;

    saveBoolSettingToDatabase(vmItemPriceIncludeTax, TGlobalSettings::Instance().ItemPriceIncludeTax);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbPriceIncludesServiceChargeClick(TObject *Sender)
{
    TGlobalSettings::Instance().ItemPriceIncludeServiceCharge = cbPriceIncludesServiceCharge->Checked;

    saveBoolSettingToDatabase(vmItemPriceIncludeServiceCharge, TGlobalSettings::Instance().ItemPriceIncludeServiceCharge);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbCalculateTaxPreDiscountClick(TObject *Sender)
{
    cbCalculateTaxPostDiscount->Checked = !cbCalculateTaxPreDiscount->Checked;

    TGlobalSettings::Instance().ReCalculateTaxPostDiscount = !cbCalculateTaxPreDiscount->Checked;

    saveBoolSettingToDatabase(vmCalculateTaxPostDiscount, TGlobalSettings::Instance().ReCalculateTaxPostDiscount);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbCalculateTaxPostDiscountClick(TObject *Sender)
{
    cbCalculateTaxPreDiscount->Checked = !cbCalculateTaxPostDiscount->Checked;

    TGlobalSettings::Instance().ReCalculateTaxPostDiscount = cbCalculateTaxPostDiscount->Checked;

    saveBoolSettingToDatabase(vmCalculateTaxPostDiscount, TGlobalSettings::Instance().ReCalculateTaxPostDiscount);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbCalculateServiceChargePreDiscountClick(TObject *Sender)

{
    cbCalculateServiceChargePostDiscount->Enabled = !cbCalculateServiceChargePreDiscount->Checked;

    TGlobalSettings::Instance().ReCalculateTaxPostDiscount = !cbCalculateServiceChargePreDiscount->Checked;
    TGlobalSettings::Instance().UsingServiceCharge = cbCalculateServiceChargePostDiscount->Checked || cbCalculateServiceChargePreDiscount->Checked;

    saveBoolSettingToDatabase(vmCalculateServiceChargePostDiscount, TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount);
    saveBoolSettingToDatabase(vmUsingServiceCharge,                 TGlobalSettings::Instance().UsingServiceCharge);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbCalculateServiceChargePostDiscountClick(TObject *Sender)
{
    cbCalculateServiceChargePreDiscount->Enabled = !cbCalculateServiceChargePostDiscount->Checked;

    TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount = cbCalculateServiceChargePostDiscount->Checked;
    TGlobalSettings::Instance().UsingServiceCharge = cbCalculateServiceChargePostDiscount->Checked || cbCalculateServiceChargePreDiscount->Checked;

    saveBoolSettingToDatabase(vmCalculateServiceChargePostDiscount, TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount);
    saveBoolSettingToDatabase(vmUsingServiceCharge,                 TGlobalSettings::Instance().UsingServiceCharge);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbApplyTaxToServiceChargeClick(TObject *Sender)
{
    cbTaxProfile->Enabled = cbApplyTaxToServiceCharge->Checked;
    TGlobalSettings::Instance().ApplyServiceChargeTax = cbApplyTaxToServiceCharge->Checked;

    saveBoolSettingToDatabase(vmApplyServiceChargeTax, TGlobalSettings::Instance().ApplyServiceChargeTax);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::cbTaxProfileChange(TObject *Sender)
{
    TGlobalSettings::Instance().ServiceChargeTaxProfileKey = taxProfiles[cbTaxProfile->ItemIndex].ProfileKey;
    TGlobalSettings::Instance().ServiceChargeTaxRate       = taxProfiles[cbTaxProfile->ItemIndex].Rate;

    saveIntSettingToDatabase(vmServiceChargeTaxProfileKey, TGlobalSettings::Instance().ServiceChargeTaxProfileKey);
    saveNumSettingToDatabase(vmServiceChargeTaxRate,       TGlobalSettings::Instance().ServiceChargeTaxRate);
}
//---------------------------------------------------------------------------

void TfrmTaxMaintenance::loadTaxProfiles(TStrings* serviceChargeTaxList)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    taxProfiles = TDBTax::GetAllProfilesOfType(DBTransaction, 0);

    DBTransaction.Commit();

    for(int i = 0; i < taxProfiles.size(); i++)
    {
        UnicodeString taxName = taxProfiles[i].Name + " (" + taxProfiles[i].Rate + "%)";
        serviceChargeTaxList->Add(taxName);
    }
}
//---------------------------------------------------------------------------

int TfrmTaxMaintenance::getTaxProfileIndexFromKey(int profileKey)
{
    for(int i = 0; i < taxProfiles.size(); i++)
    {
        if(taxProfiles[i].ProfileKey == profileKey)
        {
            return i;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
void TfrmTaxMaintenance::saveBoolSettingToDatabase(vmVariables vmVariable, bool value)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
	    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }

    TManagerVariable::Instance().SetProfileBool(DBTransaction, GlobalProfileKey, vmVariable, value);

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmTaxMaintenance::ReadVariablesFromDatabase()
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    try
    {
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
        if(GlobalProfileKey != 0)
        {
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmApplyRoundingTax,                   TGlobalSettings::Instance().ApplyRoundingTax);
            TManagerVariable::Instance().GetProfileInt( DBTransaction, GlobalProfileKey, vmRoundingTaxProfileKey,              TGlobalSettings::Instance().RoundingTaxProfileKey);
            TManagerVariable::Instance().GetProfileNum( DBTransaction, GlobalProfileKey, vmRoundingTaxRate,                    TGlobalSettings::Instance().RoundingTaxRate);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmApplyServiceChargeTax,              TGlobalSettings::Instance().ApplyServiceChargeTax);
            TManagerVariable::Instance().GetProfileInt( DBTransaction, GlobalProfileKey, vmServiceChargeTaxProfileKey,         TGlobalSettings::Instance().ServiceChargeTaxProfileKey);
            TManagerVariable::Instance().GetProfileNum( DBTransaction, GlobalProfileKey, vmServiceChargeTaxRate,               TGlobalSettings::Instance().ServiceChargeTaxRate);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmItemPriceIncludeTax,                TGlobalSettings::Instance().ItemPriceIncludeTax);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmItemPriceIncludeServiceCharge,      TGlobalSettings::Instance().ItemPriceIncludeServiceCharge);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmCalculateTaxPostDiscount,           TGlobalSettings::Instance().ReCalculateTaxPostDiscount);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmCalculateServiceChargePostDiscount, TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmUsingServiceCharge,                 TGlobalSettings::Instance().UsingServiceCharge );
        }
        DBTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TfrmTaxMaintenance::saveIntSettingToDatabase(vmVariables vmVariable, int value)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
	    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }

    TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVariable, value);

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmTaxMaintenance::saveNumSettingToDatabase(vmVariables vmVariable, double value)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
	    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }

    TManagerVariable::Instance().SetProfileNum(DBTransaction, GlobalProfileKey, vmVariable, value);

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTaxMaintenance::cbApplyTaxToRoundingClick(TObject *Sender)
{
    cbTaxProfileRounding->Enabled = cbApplyTaxToRounding->Checked;
    TGlobalSettings::Instance().ApplyRoundingTax = cbApplyTaxToRounding->Checked;

    saveBoolSettingToDatabase(vmApplyRoundingTax, TGlobalSettings::Instance().ApplyRoundingTax);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTaxMaintenance::cbRoundingTaxProfileChange(TObject *Sender)
{
    TGlobalSettings::Instance().RoundingTaxProfileKey = taxProfiles[cbTaxProfileRounding->ItemIndex].ProfileKey;
    TGlobalSettings::Instance().RoundingTaxRate       = taxProfiles[cbTaxProfileRounding->ItemIndex].Rate;

    saveIntSettingToDatabase(vmRoundingTaxProfileKey, TGlobalSettings::Instance().RoundingTaxProfileKey);
    saveNumSettingToDatabase(vmRoundingTaxRate,       TGlobalSettings::Instance().RoundingTaxRate);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTaxMaintenance::cbUseItalyFiscalPrinterClick(TObject *Sender)
{
    TGlobalSettings::Instance().UseItalyFiscalPrinter = cbUseItalyFiscalPrinter->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmUseItalyFiscalPrinter,TGlobalSettings::Instance().UseItalyFiscalPrinter);
    DBTransaction.Commit();
    if(cbUseItalyFiscalPrinter->Checked)
    {
        edPrinterName->Enabled = true;
        edLogicalName->Enabled = true;
    }
    else
    {
        edPrinterName->Enabled = false;
        edLogicalName->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmTaxMaintenance::edPrinterNameClick(TObject *Sender)
{
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
    frmTouchKeyboard->MaxLength = 50;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
    frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().PrinterType;
    frmTouchKeyboard->Caption = "Enter Printer Name";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
	    TGlobalSettings::Instance().PrinterType = frmTouchKeyboard->KeyboardText.Trim();
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmFPPrinterType,TGlobalSettings::Instance().PrinterType);
        edPrinterName->Text = TGlobalSettings::Instance().PrinterType;
        DBTransaction.Commit();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmTaxMaintenance::edLogicalNameClick(TObject *Sender)
{
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
    frmTouchKeyboard->MaxLength = 50;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
    frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().PrinterlogicalName;
    frmTouchKeyboard->Caption = "Enter Printer Logical Name";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
	    TGlobalSettings::Instance().PrinterlogicalName = frmTouchKeyboard->KeyboardText.Trim();
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmFPPrinterLogicalName,TGlobalSettings::Instance().PrinterlogicalName);
        edLogicalName->Text = TGlobalSettings::Instance().PrinterlogicalName;
        DBTransaction.Commit();
    }
}
//---------------------------------------------------------------------------
void TfrmTaxMaintenance::SyncTaxSettingWithWeb()
{
     //Sync Menu with web for online ordering..
     if(TGlobalSettings::Instance().EnableOnlineOrdering)
     {
        frmSelectDish->SyncTaxSetting();
     }
}
//-------------------------------------------------------------------------------
void __fastcall TfrmTaxMaintenance::cbPostRoomSaleToFiscalClick(TObject *Sender)
{
    TGlobalSettings::Instance().IsFiscalPostingDisable = cbPostsaletofiscal->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmIsFiscalPostingDisable,TGlobalSettings::Instance().IsFiscalPostingDisable);
    DBTransaction.Commit();
}

