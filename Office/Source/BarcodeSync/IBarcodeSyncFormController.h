//---------------------------------------------------------------------------
#ifndef IBarcodeSyncFormControllerH
#define IBarcodeSyncFormControllerH
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
class IBarcodeSyncFormController
{
public:

	virtual ~IBarcodeSyncFormController() { }
	virtual void BeginLoadMenus() = 0;
	virtual void MenusSelected(Menu::TMenusInfo& menus) = 0;
	virtual void Synchronize(BarcodeSynchronizationItems items) = 0;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
