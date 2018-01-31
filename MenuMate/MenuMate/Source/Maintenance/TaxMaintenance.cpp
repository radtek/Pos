//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TaxMaintenance.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmTaxMaintenance::TfrmTaxMaintenance(TComponent* Owner, Database::TDBControl &inDBControl)
    : TZForm(Owner), DBControl(inDBControl)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxMaintenance::FormShow(TObject *Sender)
{
    //Populate Service Charge Combo Box
    loadTaxProfiles(cbTaxProfile->Items);

    bool usingServiceCharge = TGlobalSettings::Instance().UsingServiceCharge;

    //Fill all the form from global settings variables
    cbPriceIncludeTax->Checked                    = TGlobalSettings::Instance().ItemPriceIncludeTax;
    cbPriceIncludesServiceCharge->Checked         = TGlobalSettings::Instance().ItemPriceIncludeServiceCharge;
    cbCalculateTaxPreDiscount->Checked            = !TGlobalSettings::Instance().ReCalculateTaxPostDiscount;
    cbCalculateTaxPostDiscount->Checked           = TGlobalSettings::Instance().ReCalculateTaxPostDiscount;
    cbCalculateServiceChargePreDiscount->Checked  = !TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && usingServiceCharge;
    cbCalculateServiceChargePostDiscount->Checked = TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && usingServiceCharge;
    cbApplyTaxToServiceCharge->Checked            = TGlobalSettings::Instance().ApplyServiceChargeTax;

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

    cbUseItalyFiscalPrinter->Checked = TGlobalSettings::Instance().UseItalyFiscalPrinter;
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
        Close();
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
void __fastcall TfrmTaxMaintenance::cbUseItalyFiscalPrinterClick(TObject *Sender)
{
    TGlobalSettings::Instance().UseItalyFiscalPrinter = cbUseItalyFiscalPrinter->Checked;
    saveBoolSettingToDatabase(vmUseItalyFiscalPrinter, TGlobalSettings::Instance().UseItalyFiscalPrinter);
}
