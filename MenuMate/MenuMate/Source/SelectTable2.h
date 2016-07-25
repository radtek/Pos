//---------------------------------------------------------------------------

#ifndef SelectTable2H
#define SelectTable2H
//---------------------------------------------------------------------------


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "ZForm.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchBtn.h"

#include "PlanController.h"
#include <Graphics.hpp>
#include <jpeg.hpp>

//---------------------------------------------------------------------------
class TFrmSelectTable2 : public TZForm
{
__published:	// IDE-managed Components
	TPanel *panelTables;
	TImage *imgTables;
	TPanel *Panel1;
	TTouchBtn *TouchBtn2;
	TTouchGrid *tgridLocations;
	TPanel *PnlLocation;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall imgTablesMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall imgTablesClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TouchBtn2MouseClick(TObject *Sender);
	void __fastcall tgridLocationsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
    TTimer *tiUpdateFloorPlanReq;
    void __fastcall tiUpdateFloorPlanReqTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender);

	private:	// User declarations
	TablePlan::PlanController *_controller;
	Database::TDBControl &_iBDatabase;
    void UpdateTableFloorPlan(TablePlan::PlanController *c);
    TablePlan::PlanController *get_plan;
    void UpdateTableOnFormShow();
    void UpdateTableOnFormPaint();
public:		// User declarations
	__fastcall TFrmSelectTable2(TComponent* Owner, Database::TDBControl &IBDatabase);

	void AssociateWithController(TablePlan::PlanController *c);
   UnicodeString SelectedTabContainerName;
   UnicodeString SelectedPartyName;
   int SelectedTabContainerNumber;
};
//---------------------------------------------------------------------------
//extern PACKAGE TFrmSelectTable2 *frmSelectTable2;
//---------------------------------------------------------------------------
#endif
