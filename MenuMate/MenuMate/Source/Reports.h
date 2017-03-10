//---------------------------------------------------------------------------

#ifndef ReportsH
#define ReportsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdIPWatch.hpp"
#include <set>
#include <Graphics.hpp>
#include "ZForm.h"
#include "SystemEvents.h"
#include "Enum.h"
//---------------------------------------------------------------------------
class TfrmReports : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TTouchBtn *btnStock;
	TTouchBtn *btnTransaction;
	TTouchBtn *btnTransferred;
	TTouchBtn *btnConsumption;
	TTouchBtn *btnConsumpByMenu;
	TTouchBtn *btnMemberPoints;
	TTouchBtn *btnCancel;
	TTouchBtn *btnTabsTables;
	TTouchBtn *BtnFloatAdjustments;
	TTouchBtn *btnSiteSummary;
	TTouchBtn *btnXReport;
    TTouchBtn *btnEJournal;
    TTouchBtn *btnConsolidatedZed;
    TTouchBtn *btnHavanaReport;
	void __fastcall btnStockMouseClick(TObject *Sender);
	void __fastcall btnTransactionMouseClick(TObject *Sender);
	void __fastcall btnTransferredMouseClick(TObject *Sender);
	void __fastcall btnConsumptionMouseClick(TObject *Sender);
	void __fastcall btnConsumpByMenuMouseClick(TObject *Sender);
	void __fastcall btnMemberPointsMouseClick(TObject *Sender);
	void __fastcall btnCancelMouseClick(TObject *Sender);
	void __fastcall btnTabsTablesMouseClick(TObject *Sender);
	void __fastcall BtnFloatAdjustmentsMouseClick(TObject *Sender);
	void __fastcall btnSiteSummaryMouseClick(TObject *Sender);
	void __fastcall btnXReportMouseClick(TObject *Sender);
    void __fastcall btnEJournalMouseClick(TObject *Sender);
    void __fastcall btnConsolidatedZedMouseClick(TObject *Sender);
    void __fastcall btnHavanaReportMouseClick(TObject *Sender);

private:	// User declarations
	int BtnSelection;
public:		// User declarations
	__fastcall TfrmReports(TComponent* Owner);
	int GetBtnSelection(void);
};
//---------------------------------------------------------------------------
#endif
