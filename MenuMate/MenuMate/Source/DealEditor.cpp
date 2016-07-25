//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DealEditor.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
#include "VerticalSelect.h"
#include "DBOrder.h"
#include "DealLevelsEditor.h"
#include "Deal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmDealEditorForm::TfrmDealEditorForm(TComponent* Owner, Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
	dealName = "";
	priority = 0;
	dealCategoryKey = 0;
	dealCategoryName = "";
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::SetDeal(IStoredDeal *inDeal)
{
	deal = inDeal;
	dealName = deal->GetName();
	priority = deal->GetPriority();
	dealCategoryKey = deal->GetCategoryKey();

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	dealCategoryName = TDBOrder::GetArchiveCategory(DBTransaction, dealCategoryKey);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::ok()
{
	if (dealName == "")
	{
		MessageBox("A deal must have a name.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (dealCategoryKey == 0)
	{
		MessageBox("A deal must have a category.", "Error", MB_OK + MB_ICONERROR);
	}
	else
	{
		deal->SetName(dealName);
		deal->SetPriority(priority);
		deal->SetCategoryKey(dealCategoryKey);
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::cancel()
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::setName()
{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 15;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = dealName;
	frmTouchKeyboard->Caption = "Enter Deal Name";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		dealName = frmTouchKeyboard->KeyboardText;
		if (dealName != "")
		{
			btnName->Caption = "Name: " + dealName;
		}
		else
		{
            btnName->Caption = "Deal Name";
        }
	}
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::setPriority()
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Deal Priority";
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = priority;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		priority = frmTouchNumpad->INTResult;
		btnPriority->Caption = "Priority: " + IntToStr(priority);
	}
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::setCategory()
{
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = "$000098F5";
	Item.Properties["FontColor"] = IntToStr(clWhite);
	Item.Properties["Selected"] = false;
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
		Item.Properties["Color"] = "$00004A00";
		Item.Properties["Selected"] = false;
		Item.Properties["CATEGORY_KEY"] = IntToStr((int)CatList->Objects[i]);
		Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);
	}

	SelectionForm->ShowModal();
	for (int i = 0; i < SelectionForm->Items.size(); i++)
	{
		TVerticalSelection Item = SelectionForm->Items[i];
		std::map<UnicodeString,Variant>::iterator Property;
		Property = Item.Properties.find("Selected");
		if(Property != Item.Properties.end() && static_cast<bool>(Item.Properties["Selected"]) && Item.Title != "Cancel")
		{
			dealCategoryKey = (int)Item.Properties["CATEGORY_KEY"];
			dealCategoryName = Item.Title;
		}
	}

	if(dealCategoryKey != 0)
	{
		btnCategory->Caption = "Category: " + dealCategoryName;
	}
	else
	{
		btnCategory->Caption = "Category";
	}
}
//---------------------------------------------------------------------------

void TfrmDealEditorForm::setPriceLevels()
{
	std::auto_ptr<TfrmDealLevels> frmDealLevels(TfrmDealLevels::Create<TfrmDealLevels>(this, DBControl));
	frmDealLevels->SetDealLevels(deal->GetStoredDealLevels());
	frmDealLevels->ShowModal();
	deal->SetStoredDealLevels(frmDealLevels->GetDealLevels());
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::btnOkMouseClick(TObject *Sender)
{
	ok();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::btnCancelMouseClick(TObject *Sender)
{
	cancel();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::btnNameMouseClick(TObject *Sender)
{
	setName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::btnPriorityMouseClick(TObject *Sender)
{
	setPriority();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::btnCategoryMouseClick(TObject *Sender)
{
 	setCategory();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::btnPriceLevelsMouseClick(TObject *Sender)
{
    setPriceLevels();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealEditorForm::FormShow(TObject *Sender)
{
    if (dealName != "")
	{
		btnName->Caption = "Name: " + dealName;
	}

	if (priority != 0)
	{
		btnPriority->Caption = "Priority: " + IntToStr(priority);
	}

	if(dealCategoryKey != 0)
	{
		btnCategory->Caption = "Category: " + dealCategoryName;
	}
}
//---------------------------------------------------------------------------

