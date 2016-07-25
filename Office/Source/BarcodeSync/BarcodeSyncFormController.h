//---------------------------------------------------------------------------
#ifndef BarcodeSyncFormControllerH
#define BarcodeSyncFormControllerH
//---------------------------------------------------------------------------
#include "IBarcodeSyncForm.h"
#include "BarcodeService.h"
#include "BackgroundWorker.h"
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
class BarcodeSyncFormController : private IBarcodeSyncFormController
{
public:

	BarcodeSyncFormController(IBarcodeSyncForm* form, IBarcodeServiceFactory& barcodeServiceFactory);
	virtual ~BarcodeSyncFormController();

	virtual void BeginLoadMenus();

	virtual void MenusSelected(Menu::TMenusInfo& menus);

	virtual void Synchronize(BarcodeSynchronizationItems items);

private:

	IBarcodeSyncForm* const form;
	IBarcodeServiceFactory& const barcodeServiceFactory;

	Menu::TMenusInfo __fastcall LoadMenus();
	void __fastcall OnMenusLoaded(Utils::IBackgroundWorker* worker, Menu::TMenusInfo result);

	BarcodeSynchronizationItems __fastcall LoadBarcodes(Menu::TMenusInfo menus);
	void __fastcall OnBarcodesLoaded(Utils::IBackgroundWorker* worker, BarcodeSynchronizationItems result);

	bool __fastcall SaveBarcodes(BarcodeSynchronizationItems result);
	void __fastcall OnBarcodesSaved(Utils::IBackgroundWorker* worker, bool success);

	Utils::BackgroundWorkerFactory backgroundWorkerFactory;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
