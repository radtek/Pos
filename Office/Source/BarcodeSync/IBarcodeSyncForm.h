//---------------------------------------------------------------------------
#ifndef IBarcodeSyncFormH
#define IBarcodeSyncFormH
//---------------------------------------------------------------------------
#include "MM_Menu.h"
#include "IBarcodeSyncForm.h"
#include "BarcodeService.h"
#include "IBarcodeSyncFormController.h"
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
class IBarcodeSyncForm
{
public:

	virtual void SetController(IBarcodeSyncFormController* controller) = 0;

	virtual void Show() = 0;

	virtual void Close() = 0;

	virtual void SelectMenus(const Menu::TMenusInfo& const menus) = 0;

	virtual void ShowBarcodes(BarcodeSynchronizationItems result) = 0;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
