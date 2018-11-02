//---------------------------------------------------------------------------

#ifndef SelectSaveOptionH
#define SelectSaveOptionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include "IdBaseComponent.hpp"
#include "ZForm.h"
#include "enumTab.h"
#include "enum.h"
#include <set>
//---------------------------------------------------------------------------
class TfrmSelectSaveOption : public TZForm
{
  friend TZForm;
__published:	// IDE-managed Components
  TPanel *Panel1;
  TPanel *Panel2;
    TTouchBtn *btnOk;
    TTouchBtn *tbTabs;
    TTouchBtn *tbTables;
    TTouchBtn *tbDelayedPayments;
    TTouchBtn *tbStafftabs;
    TTouchBtn *tbRooms;
    TTouchBtn *tbMembersTab;
    TTouchBtn *tbNewInvoice;
    TTouchBtn *tbWebOrders;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall tbTabsMouseClick(TObject *Sender);
    void __fastcall tbTablesMouseClick(TObject *Sender);
    void __fastcall tbDelayedPaymentsMouseClick(TObject *Sender);
    void __fastcall tbStafftabsMouseClick(TObject *Sender);
    void __fastcall tbRoomsMouseClick(TObject *Sender);
    void __fastcall tbMembersTabMouseClick(TObject *Sender);
    void __fastcall tbNewInvoiceMouseClick(TObject *Sender);
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall tbWebOrdersMouseClick(TObject *Sender);
private:	// User declarations
      void ShowTransferSection();
      void HideForTransferSection();
public:		// User declarations
    __fastcall TfrmSelectSaveOption(TComponent* Owner);
    TMMTabType SelectedTabType;
    TMMDisplayMode DisplayMode;
    AnsiString Title;
    bool IsBillingMode;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectSaveOption *frmSelectSaveOption;
//---------------------------------------------------------------------------
#endif
