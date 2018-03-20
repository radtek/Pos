//---------------------------------------------------------------------------

#ifndef TaxMaintenanceH
#define TaxMaintenanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "ZForm.h"
#include "DBTax.h"
#include "ManagerVariable.h"
#include <vector>
#include <System.hpp>
//---------------------------------------------------------------------------
class TfrmTaxMaintenance : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *pnlButtons;
    TTouchBtn *tbtnOk;
    //TPanel *Panel6;
    TPanel *Panel11;
    TCheckBox *cbPriceIncludeTax;
    TCheckBox *cbPriceIncludesServiceCharge;
    TCheckBox *cbCalculateTaxPreDiscount;
    TCheckBox *cbCalculateTaxPostDiscount;
    TCheckBox *cbCalculateServiceChargePreDiscount;
    TCheckBox *cbCalculateServiceChargePostDiscount;
    TCheckBox *cbApplyTaxToServiceCharge;
    TComboBox *cbTaxProfile;
    TCheckBox *cbUseItalyFiscalPrinter;

    //add frn
    TPageControl *Pages;
    TTabSheet *tsTaxSettings;
    TCheckBox *cbApplyTaxToRounding;
    TComboBox *cbTaxProfileRounding;
    TEdit *edPrinterName;
    TEdit *edLogicalName;
    TLabel *lbPrinterName;
    TLabel *lbLogicalName;


    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall tbtnOkMouseClick(TObject *Sender);
    void __fastcall cbPriceIncludeTaxClick(TObject *Sender);
    void __fastcall cbPriceIncludesServiceChargeClick(TObject *Sender);
    void __fastcall cbCalculateTaxPreDiscountClick(TObject *Sender);
    void __fastcall cbCalculateTaxPostDiscountClick(TObject *Sender);
    void __fastcall cbCalculateServiceChargePreDiscountClick(TObject *Sender);
    void __fastcall cbCalculateServiceChargePostDiscountClick(TObject *Sender);
    void __fastcall cbApplyTaxToServiceChargeClick(TObject *Sender);
    void __fastcall cbTaxProfileChange(TObject *Sender);
    void __fastcall cbApplyTaxToRoundingClick(TObject *Sender);
    void __fastcall cbRoundingTaxProfileChange(TObject *Sender);

    void __fastcall cbUseItalyFiscalPrinterClick(TObject *Sender);
    void __fastcall edPrinterNameClick(TObject *Sender);
    void __fastcall edLogicalNameClick(TObject *Sender);
public:		// User declarations
    __fastcall TfrmTaxMaintenance(TComponent* Owner,Database::TDBControl &inDBControl);

private:	// User declarations

	Database::TDBControl &DBControl;
    std::vector<TTaxProfile> taxProfiles;

    void loadTaxProfiles(TStrings* serviceChargeTaxList);
    int getTaxProfileIndexFromKey(int profileKey);
    void saveBoolSettingToDatabase(vmVariables vmVariable, bool value);
    void saveIntSettingToDatabase(vmVariables vmVariable, int value);
    void saveNumSettingToDatabase(vmVariables vmVariable, double value);
    void ReadVariablesFromDatabase();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
