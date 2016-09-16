//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Discounts.h"
#include "DiscountEditor.h"
#include "MMMessageBox.h"
#include "ManagerDiscount.h"
#include "Main.h"
#include "CardSwipe.h"
#include "DiscountTimes.h"
#include "GUIScale.h"
#include "DiscountGroupsGUI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmDiscounts::TfrmDiscounts(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
   ListOrder = edloPriority;
}

//---------------------------------------------------------------------------
void __fastcall TfrmDiscounts::FormShow(TObject *Sender)
{
   tgListOrder->Buttons[0][ListOrder]->Latched = true;
	FormResize(Sender);
	LoadDiscountList();
	SetGridColors(tgridSelection);
	SetGridColors(tgListOrder);
	SetGridColors(tgMove);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscounts::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
	//	ScaleBy(Screen->Width, Temp);
	}
        this->Height = Screen->Height - 40;
        GUIScale::ChildrenTop(Panel2, 0.9);
	Panel2->Left = (BorderWidth) + Panel1->Width;
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::btnDeleteClick(TObject *Sender)
{

	if (CurrentDiscountKey != 0)
	{
		if(MessageBox("Are you sure you wish to remove this Discount?", "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
		{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			ManagerDiscount->DeleteDiscount(DBTransaction,CurrentDiscountKey);
			DBTransaction.Commit();
			LoadDiscountList();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmDiscounts::LoadDiscountList()
{
	std::auto_ptr<TStringList>List(new TStringList);
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
   if(ListOrder == edloPriority)
   {
      ManagerDiscount->GetDiscountListByPriority(DBTransaction,List.get());
   }
   else
   {
    std::auto_ptr<TStringList>DiscountList(new TStringList);
    std::vector<eDiscountFilter> discountFilter;
    ManagerDiscount->GetDiscountList(DBTransaction,List.get(),discountFilter);
   }
	DBTransaction.Commit();

   tgridSelection->ColCount = 0;
	tgridSelection->RowCount = 0;

	tgridSelection->VertScrollDistance = (tgridSelection->Height / 9) * 6;
	tgridSelection->DefaultRowHeight = tgridSelection->Height / 9;

	tgridSelection->ColCount = 0;
	tgridSelection->RowCount = List->Count;

   bool DiscountFound = false;
	for (int i = 0; i < List->Count; i++)
	{
      tgridSelection->Buttons[i][0]->Caption = List->Strings[i];
      tgridSelection->Buttons[i][0]->Tag = (int)List->Objects[i];
      if(CurrentDiscountKey == (int)List->Objects[i])
      {
         tgridSelection->Buttons[i][0]->Latched = true;
         DiscountFound = true;
      }
	}

   if(!DiscountFound)
   {
      CurrentDiscountKey = 0;
      CurrentDiscountName = "";
   }
}

void __fastcall TfrmDiscounts::btnAddDiscountClick(TObject *Sender)
{
	std::auto_ptr<TfrmDiscountEdit> frmDiscountEdit(TfrmDiscountEdit::Create<TfrmDiscountEdit>(this,DBControl));
	frmDiscountEdit->Editing = false;
	frmDiscountEdit->Caption = "New Discount";
	frmDiscountEdit->ShowModal();
	LoadDiscountList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tbtnEditDiscountClick(TObject *Sender)
{
	if (CurrentDiscountKey != 0)
	{
		std::auto_ptr<TfrmDiscountEdit> frmDiscountEdit(TfrmDiscountEdit::Create<TfrmDiscountEdit>(this,DBControl));
		frmDiscountEdit->DiscountKey = CurrentDiscountKey;
		frmDiscountEdit->Editing = true;
		frmDiscountEdit->Caption = "Edit Discount";
		frmDiscountEdit->ShowModal();
		LoadDiscountList();
	}
   else
   {
			MessageBox("Select a discount", "Select a discount", MB_ICONINFORMATION + MB_OK);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tbtnClearCardClick(TObject *Sender)
{
	if (CurrentDiscountKey != 0)
	{
		std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
		frmCardSwipe->tbOtherOpt->Visible = true;
      frmCardSwipe->tbOtherOpt->Caption = "Clear All Cards";
		frmCardSwipe->ShowModal();
		if (frmCardSwipe->ModalResult == mrOk)
		{
			AnsiString CardString = AnsiString(frmCardSwipe->SwipeString).SubString(1,80);
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			ManagerDiscount->DeleteCard(DBTransaction,CardString);
			DBTransaction.Commit();
			MessageBox("Card Cleared", "Card Cleared", MB_ICONINFORMATION + MB_OK);
		}
		else if(frmCardSwipe->ModalResult == mrAbort)
		{
			if(MessageBox("This will remove ALL cards associated with \r"+ CurrentDiscountName +".\rDo you wish to proceed?", "Warning",
				MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				ManagerDiscount->DeleteCard(DBTransaction,CurrentDiscountKey);
				DBTransaction.Commit();
				MessageBox("All Cards Cleared", "Card Cleared", MB_ICONINFORMATION + MB_OK);
         }
      }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tbtnAssignCardClick(TObject *Sender)
{
	if (CurrentDiscountKey)
	{
		std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
		frmCardSwipe->tbOtherOpt->Visible = false;
		frmCardSwipe->ShowModal();
		if (frmCardSwipe->ModalResult == mrOk)
		{
			AnsiString CardString = AnsiString(frmCardSwipe->SwipeString).SubString(1,80);
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			if(ManagerDiscount->GetDiscount(DBTransaction,CardString) == 0)
			{
				ManagerDiscount->SetCard(DBTransaction,CurrentDiscountKey, CardString);
				MessageBox("Card Added", "Card Added", MB_ICONINFORMATION + MB_OK);
			}
			else
			{
				MessageBox("This Card has already been assigned to a discount. Clear it first.", "Add Card Failed", MB_ICONWARNING + MB_OK);
			}
			DBTransaction.Commit();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::Panel19Click(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tgridSelectionMouseClick(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    CurrentDiscountKey = GridButton->Tag;
    CurrentDiscountName = GridButton->Caption;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    bool isCloudDiscount = ManagerDiscount->IsCloudDiscount(DBTransaction,CurrentDiscountKey);
    DBTransaction.Commit();
    tbtnEditDiscount->Enabled = !isCloudDiscount;
    btnDelete->Enabled = !isCloudDiscount;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tgListOrderMouseClick(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   ListOrder = (DiscountListOrder)GridButton->Tag;
   LoadDiscountList();
   if(ListOrder == edloPriority)
   {
      tgMove->Buttons[0][0]->Caption = "Increase Priorty";
      tgMove->Buttons[1][0]->Caption = "Decrease Priorty";
   }
   else
   {
      tgMove->Buttons[0][0]->Caption = "Move Up";
      tgMove->Buttons[1][0]->Caption = "Move Down";
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tgMoveMouseClick(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{

   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   if(ListOrder == edloPriority)
   {
      if(tgMove->Row(GridButton) == 0)
      {
         ManagerDiscount->IncreasePriority(DBTransaction,CurrentDiscountKey);

      }
      else
      {
         ManagerDiscount->DecreasePriority(DBTransaction,CurrentDiscountKey);
      }
   }
   else
   {
      if(tgMove->Row(GridButton) == 0)
      {
         ManagerDiscount->DecreaseAppearanceOrder(DBTransaction,CurrentDiscountKey);
      }
      else
      {
         ManagerDiscount->IncreaseAppearanceOrder(DBTransaction,CurrentDiscountKey);
      }
   }
   DBTransaction.Commit();
   LoadDiscountList();   
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::TouchBtn1MouseClick(TObject *Sender)
{
	if (CurrentDiscountKey != 0)
	{
      std::auto_ptr<TfrmDiscountTimes>(frmDiscountTimes)(TfrmDiscountTimes::Create<TfrmDiscountTimes>(this,DBControl));
      frmDiscountTimes->DiscountKey = CurrentDiscountKey;
      frmDiscountTimes->ShowModal();
   }
   else
   {
			MessageBox("Select a discount", "Select a discount", MB_ICONINFORMATION + MB_OK);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscounts::tbtnDiscountGroupMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmDiscountGroupsGUI> DiscountGroupsGUI(new TfrmDiscountGroupsGUI(this));
    DiscountGroupsGUI->Caption = "Add Discount Group";
    DiscountGroupsGUI->ShowModal();
}
//---------------------------------------------------------------------------

