//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SyncMenuToRecipeForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
__fastcall TBarcodeSyncForm::TBarcodeSyncForm()
	: TForm((TComponent*)NULL)
{
}
//---------------------------------------------------------------------------
__fastcall TBarcodeSyncForm::~TBarcodeSyncForm()
{
	//this->controller->Terminate();
}
//---------------------------------------------------------------------------
void TBarcodeSyncForm::SetController(IBarcodeSyncFormController* controller)
{
	this->controller = controller;
}
//---------------------------------------------------------------------------
void TBarcodeSyncForm::Show()
{
	this->Loading("Please wait. Loading menus...");
	this->ShowModal();
}
//---------------------------------------------------------------------------
void TBarcodeSyncForm::Close()
{
	TForm::Close();
}
//---------------------------------------------------------------------------
void TBarcodeSyncForm::SelectMenus(const Menu::TMenusInfo& const menus)
{
	this->Menus->Items->Clear();
	this->fMenus = menus;

	Menu::TMenusInfo::iterator menu = this->fMenus.begin();
	while (menu != this->fMenus.end())
	{
		int index = this->Menus->Items->AddObject(menu->Menu_Name, (TObject*)&(*menu));
		this->Menus->Checked[index] = true;
		menu++;
	}

	this->PageControl->ActivePage = this->MenusTabSheet;
}
//---------------------------------------------------------------------------
void TBarcodeSyncForm::ShowBarcodes(BarcodeSynchronizationItems items)
{
	this->items = items;

	this->SyncItems->Items->Clear();

	BarcodeSynchronizationItems::iterator item = this->items.begin();
	while (item != this->items.end())
	{
		TListItem* listItem = this->SyncItems->Items->Add();
		listItem->Data = &(*item);
		listItem->Caption = item->Description;
		listItem->SubItems->Add(item->StockItem + " (" + item->StockCode + ")");
		listItem->SubItems->Add(item->Barcode);

		item++;
	}

	this->PageControl->ActivePage = this->BarcodesTabSheet;

}
//---------------------------------------------------------------------------
// Event Handlers
//---------------------------------------------------------------------------
void __fastcall TBarcodeSyncForm::FormShow(TObject *Sender)
{
	this->controller->BeginLoadMenus();
}
//---------------------------------------------------------------------------
void __fastcall TBarcodeSyncForm::NextButtonClick(TObject *Sender)
{
	this->Loading("Please wait. Loading barcodes...");
	Menu::TMenusInfo selected = this->GetCheckedMenus();
	this->controller->MenusSelected(selected);
}
//---------------------------------------------------------------------------
void __fastcall TBarcodeSyncForm::DoneButtonClick(TObject *Sender)
{
	this->Loading("Please wait. Saving...");
	BarcodeSynchronizationItems selected = this->GetStockToSync();
	this->controller->Synchronize(selected);
}
//---------------------------------------------------------------------------
void __fastcall TBarcodeSyncForm::CancelButtonClicked(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
void TBarcodeSyncForm::Loading(AnsiString message)
{
	this->PleaseWaitLabel->Visible = true;
	this->PleaseWaitLabel->Caption = message;
	this->PageControl->ActivePage = this->WaitingTabSheet;
}
//---------------------------------------------------------------------------
Menu::TMenusInfo TBarcodeSyncForm::GetCheckedMenus()
{
	Menu::TMenusInfo selected;

	for (int i=0; i<this->Menus->Items->Count; i++)
	{
		if (this->Menus->Checked[i])
		{
			Menu::TMenuInfo* menuPtr = (Menu::TMenuInfo*)this->Menus->Items->Objects[i];
			selected.push_back(*menuPtr);
		}
	}

	return selected;
}
//---------------------------------------------------------------------------
BarcodeSynchronizationItems TBarcodeSyncForm::GetStockToSync()
{
	BarcodeSynchronizationItems items;

	for (int i=0; i<this->SyncItems->Items->Count; i++)
	{
		BarcodeSynchronizationItem* item = (BarcodeSynchronizationItem*)this->SyncItems->Items->Item[i]->Data;
		items.push_back(*item);
	}

	return items;
}
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------

