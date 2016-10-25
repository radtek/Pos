//---------------------------------------------------------------------------

#ifndef PaymentMaintenanceH
#define PaymentMaintenanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "MM_DBCore.h"
#include "ListPaymentSystem.h"
#include "ZForm.h"
#include "touchbtn.h"
#include "touchcontrols.h"
#include "touchgrid.h"
//---------------------------------------------------------------------------
class TfrmPaymentMaintenance : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
        TPanel *Panel44;
        TPanel *Panel45;
	TTouchBtn *tbAddPaymentType;
	TTouchBtn *tbEditPaymentType;
	TTouchBtn *TouchBtn3;
	TTouchBtn *tbDeletePaymentType;
	TTouchBtn *tbDefaultPaymentType;
	TStaticText *lbePartyName;
	TTouchGrid *tgridContainerList;
    TTouchBtn *tbPaymentGroups;
        void __fastcall FormResize(TObject *Sender);
   void __fastcall imgExitClick(TObject *Sender);
   void __fastcall pnlAddClick(TObject *Sender);
   void __fastcall tbtnEditClick(TObject *Sender);
   void __fastcall tbtnPaymentGroupClick(TObject *Sender);
   void __fastcall tbtnDeleteClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall pnlDefaultsClick(TObject *Sender);
	void __fastcall tgridContainerListMouseClick(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
private:	// User declarations
	__fastcall TfrmPaymentMaintenance(TComponent* Owner,Database::TDBControl &inDBControl,TListPaymentSystem *inPaymentSystem);
	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);
	void __fastcall UpdateList();
	Database::TDBControl &DBControl;
	TListPaymentSystem *PaymentSystem;
	int PaymentKey;
public:		// User declarations
	TfrmPaymentMaintenance static *Create(TForm* Owner,Database::TDBControl &inDBControl,TListPaymentSystem *inPaymentSystem);
    bool IsPaymentExist(Database::TDBTransaction &DBTransaction,AnsiString PaymentName);

};
//---------------------------------------------------------------------------
#endif
