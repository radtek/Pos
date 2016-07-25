//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ErrorWarningSettings.h"
#include "MMRegistry.h"
#include "Consts.h"
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmErrorWarningSettings *frmErrorWarningSettings;
//---------------------------------------------------------------------------
__fastcall TfrmErrorWarningSettings::TfrmErrorWarningSettings(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmErrorWarningSettings::FormShow(TObject *Sender)
{
	AnsiString Key = OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Editor Checks\\";
	int Result;

	Result = 0;
	RegistryRead(Key + "Food", "Recipes", Result);
	chbFoodRecipes->Checked = Result;

	Result = 1;
	RegistryRead(Key + "Food", "NoPrice", Result);
	chbFoodNoPrice->Checked = Result;

	Result = 1;
	RegistryRead(Key + "Food", "NoSpecialPrice", Result);
	chbFoodNoSpecialPrice->Checked = Result;

	Result = 1;
	RegistryRead(Key + "Food", "Recipes", Result);
	chbFoodRecipes->Checked = Result;

	Result = 0;
	RegistryRead(Key + "Food", "3rdPartyCode", Result);
	chbFood3rdPartyError->Checked = Result;

	Result = 1;
	RegistryRead(Key + "Drinks", "NoPrice", Result);
	chbDrinkNoPrice->Checked = Result;

	Result = 1;
	RegistryRead(Key + "Drinks", "NoSpecialPrice", Result);
	chbDrinkNoSpecialPrice->Checked = Result;

	Result = 1;
	RegistryRead(Key + "Drinks", "Recipes", Result);
	chbDrinkRecipes->Checked = Result;

	Result = 0;
	RegistryRead(Key + "Drinks", "3rdPartyCode", Result);
	chbDrink3rdPartyError->Checked = Result;
}
//---------------------------------------------------------------------------
void __fastcall TfrmErrorWarningSettings::btnCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmErrorWarningSettings::Button2Click(TObject *Sender)
{
	AnsiString Key = OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Editor Checks\\";

	RegistryWrite(Key + "Food", "Recipes",				chbFoodRecipes->Checked?1:0);
	RegistryWrite(Key + "Food", "NoPrice",				chbFoodNoPrice->Checked?1:0);
	RegistryWrite(Key + "Food", "NoSpecialPrice",	chbFoodNoSpecialPrice->Checked?1:0);
	RegistryWrite(Key + "Food", "3rdPartyCode",		chbFood3rdPartyError->Checked?1:0);

	RegistryWrite(Key + "Drinks", "Recipes",			chbDrinkRecipes->Checked?1:0);
	RegistryWrite(Key + "Drinks", "NoPrice",			chbDrinkNoPrice->Checked?1:0);
	RegistryWrite(Key + "Drinks", "NoSpecialPrice",	chbDrinkNoSpecialPrice->Checked?1:0);
	RegistryWrite(Key + "Drinks", "3rdPartyCode",	chbDrink3rdPartyError->Checked?1:0);

	Close();
}
//---------------------------------------------------------------------------

