//---------------------------------------------------------------------------

#ifndef SyncMenuToRecipeFormH
#define SyncMenuToRecipeFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include "IBarcodeSyncForm.h"
#include "BarcodeService.h"
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
class TBarcodeSyncForm : public TForm, public IBarcodeSyncForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TTabSheet *MenusTabSheet;
	TCheckListBox *Menus;
	TTabSheet *WaitingTabSheet;
	TLabel *PleaseWaitLabel;
	TLabel *Label1;
	TTabSheet *BarcodesTabSheet;
	TLabel *Label2;
	TButton *CancelButton1;
	TListView *SyncItems;
	TButton *CancelButton2;
	TButton *NextButton;
	TButton *DoneButton;
	void __fastcall NextButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CancelButtonClicked(TObject *Sender);
	void __fastcall DoneButtonClick(TObject *Sender);
private:	// User declarations
	IBarcodeSyncFormController* controller;

	Menu::TMenusInfo fMenus;
	BarcodeSynchronizationItems items;

	void Loading(AnsiString message); // Shows loading message while there is db activity etc.
	Menu::TMenusInfo GetCheckedMenus();
	BarcodeSynchronizationItems GetStockToSync();
public:		// User declarations
	__fastcall TBarcodeSyncForm();
	__fastcall virtual ~TBarcodeSyncForm();
	virtual void SetController(IBarcodeSyncFormController* controller);
	virtual void Show();
	virtual void Close();
	virtual void SelectMenus(const Menu::TMenusInfo& const menus);
	virtual void ShowBarcodes(BarcodeSynchronizationItems result);
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
