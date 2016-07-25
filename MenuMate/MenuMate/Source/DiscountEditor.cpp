//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DiscountEditor.h"
#include "MMTouchKeyboard.h"
#include "GUIDiscount.h"
#include "DBOrder.h"
#include "DeviceRealTerminal.h"
#include <math.h>
#include "MMMessageBox.h"
#include "Money.h"
#include "MMTouchNumpad.h"
#include "VerticalSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchcontrols"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmDiscountEdit *frmDiscountEdit;

//---------------------------------------------------------------------------
__fastcall TfrmDiscountEdit::TfrmDiscountEdit(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
}

//---------------------------------------------------------------------------
void __fastcall TfrmDiscountEdit::FormShow(TObject *Sender)
{
	FormResize(Sender);

	if(!Editing)
	{
		CurrentDiscount.Clear();
		rgDiscountType->ItemIndex = 0;
	}
	else
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		ManagerDiscount->GetDiscount(DBTransaction,DiscountKey, CurrentDiscount);

		DBTransaction.Commit();

		rgDiscountType->ItemIndex = static_cast<int>(CurrentDiscount.Type);

       if(CurrentDiscount.CategoryFilterKeys.empty())
       {
          tbtnFilter->Caption = "Category Filter : None";
       }
       else
       {
          tbtnFilter->Caption = "Category Filtered";
       }
		btnName->Caption = "Name\r" + CurrentDiscount.Name;
      	tbtnDiscountID->Caption = "Discount ID\r" + IntToStr(CurrentDiscount.ID);
      	tbtnAppearanceOrder->Caption = "Appearance Order\r" + IntToStr(CurrentDiscount.AppearanceOrder);
      	tbtnPriority->Caption = "Discount Priority\r" + IntToStr(CurrentDiscount.Priority);
		tbtnDiscountGroup->Caption = "Discount Group\r" + IntToStr(CurrentDiscount.Group);

		if(CurrentDiscount.Mode == DiscModePercent)
		{
			if(CurrentDiscount.PercentAmount > 0)
			{
				btnAmount->Caption = "Discount\r" + FloatToStrF(fabs(CurrentDiscount.PercentAmount),ffGeneral, 15, 2) + "%";
			}
			else
			{
				btnAmount->Caption = "Surcharge\r" + FloatToStrF(fabs(CurrentDiscount.PercentAmount),ffGeneral, 15, 2) + "%";
			}
		}
		else if (CurrentDiscount.Mode == DiscModeCurrency)
		{
			if(CurrentDiscount.Amount > 0)
			{
				btnAmount->Caption = "Discount\r"+ FormatFloat("$0.00",fabs(CurrentDiscount.Amount));
			}
			else
			{
				btnAmount->Caption = "Surcharge\r"+ FormatFloat("$0.00",fabs(CurrentDiscount.Amount));
			}
		}
		else if (CurrentDiscount.Mode == DiscModeSetPrice)
		{
			btnAmount->Caption = "Set Price\r"+ FormatFloat("$0.00",fabs(CurrentDiscount.Amount));
		}
		else if (CurrentDiscount.Mode == DiscModeCombo)
		{
			btnAmount->Caption = "Combo\r"+ FormatFloat("$0.00",fabs(CurrentDiscount.Amount));
		}
        else if (CurrentDiscount.Mode == DiscModeItem)
		{
			btnAmount->Caption = "Set Price\r"+ FormatFloat("$0.00",fabs(CurrentDiscount.Amount));
		}
        else if (CurrentDiscount.Mode == DiscModePoints)
		{
			btnAmount->Caption = "Set Points\r"+ FormatFloat("0.00",fabs(CurrentDiscount.Amount));
		}
		memDescription->Lines->Text = CurrentDiscount.Description;
	}
	tbRounding->Caption = "Round To\r" + FormatFloat("0.00",CurrentDiscount.Rounding);
   tbMembersOnly->Latched = CurrentDiscount.MembersOnly;
   tbMembersExempt->Latched = CurrentDiscount.MembersExempt;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

	 //	ScaleBy(Screen->Width, Temp);
	}
	ClientWidth = Panel1->Left + Panel2->Left + Panel2->Width;
	ClientHeight = Panel1->Height + (Panel1->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::memDescriptionMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 40;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = memDescription->Lines->Text;
	frmTouchKeyboard->Caption = "Enter Discount Description";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		CurrentDiscount.Description = frmTouchKeyboard->KeyboardText;
		memDescription->Lines->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::Panel19Click(TObject *Sender)
{
   try
   {
      bool DiscountValid = true;
      if(CurrentDiscount.Name == "")
      {
      	MessageBox("Discount Invalid.\rPlease Enter a Button Name"  , "Warning", MB_ICONWARNING + MB_OK);
         DiscountValid = false;         
      }

      if(DiscountValid)
      {
         Database::TDBTransaction DBTransaction(DBControl);
         DBTransaction.StartTransaction();
         ManagerDiscount->SetDiscount(DBTransaction,DiscountKey, CurrentDiscount);
         DBTransaction.Commit();
         Close();
      }
   }
	catch(Exception &E)
	{
   	MessageBox("Discount not saved!.\r" + E.Message , "Warning", MB_ICONWARNING + MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::Panel20Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::btnNameClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 15;
	frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = CurrentDiscount.Name;
   frmTouchKeyboard->Caption = "Enter Discount Name";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
		CurrentDiscount.Name = frmTouchKeyboard->KeyboardText;
		btnName->Caption = "Name\r" + CurrentDiscount.Name;
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmDiscountEdit::btnAmountClick(TObject *Sender)
{
	std::auto_ptr<TfrmDiscount> frmDiscount(TfrmDiscount::Create<TfrmDiscount>(this));
	frmDiscount->CURInitial = 0;
	frmDiscount->PERCInitial = 0;
	frmDiscount->TotalValue = 0;
	frmDiscount->Mode = CurrentDiscount.Mode;

	if(CurrentDiscount.Mode == DiscModePercent)
	{
		frmDiscount->PERCInitial = CurrentDiscount.PercentAmount;
	}
	else if (CurrentDiscount.Mode == DiscModeCurrency)
	{
		frmDiscount->CURInitial = CurrentDiscount.Amount;
	}
	else if (CurrentDiscount.Mode == DiscModeSetPrice)
	{
		frmDiscount->CURInitial = CurrentDiscount.Amount;
	}
    else if (CurrentDiscount.Mode == DiscModeItem)
    {
        frmDiscount->CURInitial = CurrentDiscount.Amount;
    }
	else if (CurrentDiscount.Mode == DiscModeCombo)
	{
		frmDiscount->CURInitial = CurrentDiscount.Amount;
	}
    else if (CurrentDiscount.Mode == DiscModePoints)
	{
		frmDiscount->CURInitial = CurrentDiscount.Amount;
	}

	if (frmDiscount->ShowModal() == mrOk)
	{
		CurrentDiscount.Mode = frmDiscount->Mode;
		if (frmDiscount->Mode == DiscModeCurrency)
		{
			CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);

			if(CurrentDiscount.Amount != frmDiscount->CURResult)
			{
				MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
			}

			if(CurrentDiscount.Amount > 0)
			{
				btnAmount->Caption = "Discount\r"+ CurrToStrF(fabs(CurrentDiscount.Amount), ffCurrency, CurrencyDecimals);
			}
			else
			{
				btnAmount->Caption = "Surcharge\r"+ CurrToStrF(fabs(CurrentDiscount.Amount), ffCurrency, CurrencyDecimals);
			}
			CurrentDiscount.PercentAmount = 0;
		}
		else if (frmDiscount->Mode == DiscModePercent)
		{
			CurrentDiscount.PercentAmount = RoundToNearest(frmDiscount->PERCResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
			if(CurrentDiscount.PercentAmount > 0)
			{
				btnAmount->Caption = "Discount\r" + FloatToStrF(fabs(CurrentDiscount.PercentAmount),ffGeneral, 15, 2) + "%";
			}
			else
			{
				btnAmount->Caption = "Surcharge\r" + FloatToStrF(fabs(CurrentDiscount.PercentAmount),ffGeneral, 15, 2) + "%";
			}
			CurrentDiscount.Amount = 0;
		}
		else if (frmDiscount->Mode == DiscModeSetPrice)
		{
			CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
			btnAmount->Caption = "Set Price\r"+ CurrToStrF(fabs(CurrentDiscount.Amount), ffCurrency, CurrencyDecimals);
			CurrentDiscount.PercentAmount = 0;
		}

       	else if (frmDiscount->Mode == DiscModeItem)
		{
			CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
			btnAmount->Caption = "Set Price\r"+ CurrToStrF(fabs(CurrentDiscount.Amount), ffCurrency, CurrencyDecimals);
			CurrentDiscount.PercentAmount = 0;
		}

		else if (frmDiscount->Mode == DiscModeCombo)
		{
			CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
			btnAmount->Caption = "Combo\r"+ CurrToStrF(fabs(CurrentDiscount.Amount), ffCurrency, CurrencyDecimals);
			CurrentDiscount.ComboAmount = CurrentDiscount.Amount;
			CurrentDiscount.PercentAmount = 0;
		}
        else if (frmDiscount->Mode == DiscModePoints)
		{
			CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
			btnAmount->Caption = "Set Points\r"+ CurrToStrF(fabs(CurrentDiscount.Amount), ffNumber, CurrencyDecimals);
			CurrentDiscount.PercentAmount = 0;
		}
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbtnFilterClick(TObject *Sender)
{
    std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

    TVerticalSelection Item;
    Item.Title = "Close";
    Item.Properties["Color"] = IntToStr(clMaroon);
    Item.CloseSelection = true;
    SelectionForm->Items.push_back(Item);

    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    std::auto_ptr<TStringList>CatList(new TStringList());
    TDBOrder::GetCategoryList(DBTransaction,CatList.get());
    DBTransaction.Commit();

    for (int i = 0; i < CatList->Count; i++)
    {
      TVerticalSelection Item;
      Item.Title = CatList->Strings[i];
      Item.Properties["CATEGORY_KEY"] = IntToStr((int)CatList->Objects[i]);
      if(CurrentDiscount.CategoryFilterKeys.find((int)CatList->Objects[i]) != CurrentDiscount.CategoryFilterKeys.end())
      {
        Item.Properties["Latched"] = true;
        Item.Properties["Selected"] = true;
      }
      Item.CloseSelection = false;
      SelectionForm->Items.push_back(Item);
    }

	SelectionForm->ShowModal();
    CurrentDiscount.CategoryFilterKeys.clear();
    bool HasFilters = false;
    for (int i = 0; i < SelectionForm->Items.size(); i++)
	{
		TVerticalSelection Item = SelectionForm->Items[i];
		std::map<UnicodeString,Variant>::iterator Property;
		Property = Item.Properties.find("Selected");
		if(Property != Item.Properties.end() && static_cast<bool>(Item.Properties["Selected"]) == true && Item.Title != "Close")
		{
             int CatKey = (int)Item.Properties["CATEGORY_KEY"];
             if(CatKey != 0)
             {
         	 	CurrentDiscount.CategoryFilterKeys.insert((int)Item.Properties["CATEGORY_KEY"]);
             	HasFilters = true;
             }
        }
	}

   if(HasFilters)
   {
      tbtnFilter->Caption = "Category Filtered";
   }
   else
   {
      tbtnFilter->Caption = "Category Filter : None";
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::rgDiscountTypeClick(TObject *Sender)
{
	CurrentDiscount.Type = static_cast<TDiscountType>(rgDiscountType->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbRoundingMouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmDiscount> frmDiscount(TfrmDiscount::Create<TfrmDiscount>(this));

	frmDiscount->Mode = DiscModeCurrency;
	frmDiscount->PERCInitial = 0;
	frmDiscount->CURInitial = CurrentDiscount.Rounding;
	frmDiscount->tbToggleAmount->Visible = false;
	frmDiscount->pnlToggle->Visible = false;

	if (frmDiscount->ShowModal() == mrOk)
	{
		CurrentDiscount.Rounding = frmDiscount->CURResult;
		tbRounding->Caption = "Round To\r" + CurrToStrF(CurrentDiscount.Rounding, ffCurrency, CurrencyDecimals);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbtnDiscountIDMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
   frmTouchNumpad->Caption = "Enter Discount ID";
	frmTouchNumpad->btnOk->Visible = true;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = CurrentDiscount.ID;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
      if(frmTouchNumpad->INTResult < 1 || frmTouchNumpad->INTResult > 255)
      {
				MessageBox("The Discount ID must be between 1 and 255 inclusive.", "Error", MB_ICONWARNING + MB_OK);
      }
      else
      {
         CurrentDiscount.ID = frmTouchNumpad->INTResult;
         tbtnDiscountID->Caption = "Discount ID\r" + IntToStr(CurrentDiscount.ID);         
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbtnPriorityMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Discount Priority";
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = CurrentDiscount.Priority;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
      CurrentDiscount.Priority = frmTouchNumpad->INTResult;
      tbtnPriority->Caption = "Discount Priority\r" + IntToStr(CurrentDiscount.Priority);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbtnAppearanceOrderMouseClick(
      TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Appearance Order";
	frmTouchNumpad->btnOk->Visible = true;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = CurrentDiscount.AppearanceOrder;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
      CurrentDiscount.AppearanceOrder = frmTouchNumpad->INTResult;
      tbtnAppearanceOrder->Caption = "Appearance Order\r" + IntToStr(CurrentDiscount.AppearanceOrder);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbMembersOnlyMouseClick(TObject *Sender)
{
	CurrentDiscount.MembersOnly = tbMembersOnly->Latched;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbMembersExemptMouseClick(TObject *Sender)
{
	CurrentDiscount.MembersExempt = tbMembersExempt->Latched;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscountEdit::tbMaxValueMouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmDiscount> frmDiscount(TfrmDiscount::Create<TfrmDiscount>(this));
	frmDiscount->CURInitial = 0;
	frmDiscount->PERCInitial = 0;
	frmDiscount->TotalValue = 0;
	frmDiscount->pnlToggle->Enabled = false;
	frmDiscount->pnlToggle->Caption = "Maximum Value";
    frmDiscount->Mode = CurrentDiscount.Mode;

	if (CurrentDiscount.Mode == DiscModeCombo)
		frmDiscount->CURInitial = CurrentDiscount.Amount;

	if (frmDiscount->ShowModal() == mrOk)
	{
		CurrentDiscount.MaximumValue = RoundToNearest(frmDiscount->CURResult,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
      	tbMaxValue->Caption = "Maximum Value\r"+ CurrToStrF(fabs(CurrentDiscount.MaximumValue), ffCurrency, CurrencyDecimals);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscountEdit::tbtnDiscountGroupMouseClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
   frmTouchNumpad->Caption = "Enter Discount Group Number";
   frmTouchNumpad->btnOk->Visible = true;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = CurrentDiscount.Group;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
   		CurrentDiscount.Group = frmTouchNumpad->INTResult;
    	tbtnDiscountGroup->Caption = "Discount Group\r" + IntToStr(CurrentDiscount.Group);
   }
}
//---------------------------------------------------------------------------

