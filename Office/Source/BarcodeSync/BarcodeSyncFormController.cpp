//---------------------------------------------------------------------------
#pragma hdrstop

#include "BarcodeSyncFormController.h"
#include "BarcodeService.h"
#include "MMData.h"
#include "MM_Menu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
BarcodeSyncFormController::BarcodeSyncFormController(IBarcodeSyncForm* form, IBarcodeServiceFactory& barcodeServiceFactory)
	: barcodeServiceFactory(barcodeServiceFactory), form(form)
{
	this->form->SetController(this);
}
//---------------------------------------------------------------------------
BarcodeSyncFormController::~BarcodeSyncFormController()
{
}
//---------------------------------------------------------------------------
void BarcodeSyncFormController::BeginLoadMenus()
{
	this->backgroundWorkerFactory.RunWorker<Menu::TMenusInfo>(&this->LoadMenus, &this->OnMenusLoaded);
}
//---------------------------------------------------------------------------
void BarcodeSyncFormController::MenusSelected(Menu::TMenusInfo& menus)
{
	this->backgroundWorkerFactory.RunWorker<BarcodeSynchronizationItems, Menu::TMenusInfo>(&this->LoadBarcodes, &this->OnBarcodesLoaded, menus);
}
//---------------------------------------------------------------------------
void BarcodeSyncFormController::Synchronize(BarcodeSynchronizationItems items)
{
	this->backgroundWorkerFactory.RunWorker<bool, BarcodeSynchronizationItems>(&this->SaveBarcodes, &this->OnBarcodesSaved, items);
}
//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
Menu::TMenusInfo __fastcall BarcodeSyncFormController::LoadMenus()
{
	return this->barcodeServiceFactory.Build()->GetMenus();
}
//---------------------------------------------------------------------------
void __fastcall BarcodeSyncFormController::OnMenusLoaded(Utils::IBackgroundWorker* worker, Menu::TMenusInfo menus)
{
	this->form->SelectMenus(menus);
}
//---------------------------------------------------------------------------
BarcodeSynchronizationItems __fastcall BarcodeSyncFormController::LoadBarcodes(Menu::TMenusInfo menus)
{
	return this->barcodeServiceFactory.Build()->GetSyncItems(menus);
}
//---------------------------------------------------------------------------
void __fastcall BarcodeSyncFormController::OnBarcodesLoaded(Utils::IBackgroundWorker* worker, BarcodeSynchronizationItems result)
{
	this->form->ShowBarcodes(result);
}
//---------------------------------------------------------------------------
bool __fastcall BarcodeSyncFormController::SaveBarcodes(BarcodeSynchronizationItems result)
{
	return this->barcodeServiceFactory.Build()->Synchronize(result);
}
//---------------------------------------------------------------------------
void __fastcall BarcodeSyncFormController::OnBarcodesSaved(Utils::IBackgroundWorker* worker, bool success)
{
	if (success)
		this->form->Close();
}
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------

