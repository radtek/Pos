//---------------------------------------------------------------------------

#ifndef TaxRemovalH
#define TaxRemovalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "ZForm.h"
#include "SystemEvents.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include <CheckLst.hpp>
#include "TouchGrid.h"
#include <vector>
#include "DBTax.h"
//---------------------------------------------------------------------------
class  TfrmTaxRemoval : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    	TPanel *Panel1;
    	TPanel *Panel2;
	    TTouchBtn *BtnOk;
    TTouchBtn *btnCancel;
    TTouchGrid *tgTaxList;
	void __fastcall btnOkMouseClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall tgTaxListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
private:	// User declarations
	__fastcall TfrmTaxRemoval(TComponent* Owner,Database::TDBControl &inDBControl);
	Database::TDBControl &DBControl;
    std::vector<TTaxProfile> taxProfiles;
    void LoadSalesTaxes();
    void LoadServiceCharges();
    void LoadServiceChargeTaxes();
    void LoadLocalTaxes();

protected:

public:		// User declarations
      TStringList *RemovedTaxes;
      TStringList *TaxList;

};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
