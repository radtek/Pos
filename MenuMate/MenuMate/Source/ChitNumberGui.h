#ifndef ChitNumberGuiH
#define ChitNumberGuiH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>

#include "ManagerChitNumber.h"
#include "DBChit.h"
#include "TaxMaintenance.h"

class TfrmTouchKeyboard;
class TfrmVerticalSelect;

class chit_options_prompt_t;

class TfrmChitNumberGui : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlMajor;
    TPanel *pnlLabel;
    TTouchBtn *tbtnName;
    TTouchBtn *tbtnType;
    TTouchBtn *tbtnMask;
    TPanel *pnlMinor;
    TTouchBtn *btnOk;
    TTouchBtn *btnCancel;
    TTouchBtn *tbtnFormat;
    TTouchBtn *tbtnDefaultMenu;
    TTouchBtn *tbtnMaxChitNo;
        TGroupBox *GroupBox1;
        TCheckBox *cbDisableHoldAndSend;
        TCheckBox *cbEnableTableDisplay;
        TCheckBox *cbNonChargeable;
        TCheckBox *cbCaptureCustomerNameAndPhone;
        TCheckBox *cbCaptureCustomerDetails;
        TCheckBox *cbAutoSaveOnTab;
        TCheckBox *cbPromptDeliveryTime;
    void __fastcall OkClicked(TObject *Sender);
    void __fastcall CancelClicked(TObject *Sender);
    void __fastcall NameClicked(TObject *Sender);
    void __fastcall MaskClicked(TObject *Sender);
    void __fastcall TypeClicked(TObject *Sender);
    void __fastcall FormatClicked(TObject *Sender);
    void __fastcall DefaultMenuClicked(TObject *Sender);
	void __fastcall ToggleHoldAndSendForThisChit(TObject *Sender);
    void __fastcall cbEnableTableDisplayClick(TObject *Sender);
        void __fastcall cbNonChargeableClick(TObject *Sender);
    //MM2035
    void __fastcall cbCaptureCustomerNameAndPhoneClick(TObject *Sender);
    //MM2036
    void __fastcall cbAutoSaveOnTabClick(TObject *Sender);
    //MM2063
    void __fastcall cbCaptureCustomerDetailsClick(TObject *Sender);
    void __fastcall tbtnMaxChitNoMouseClick(TObject *Sender);
        void __fastcall cbPromptDeliveryTimeClick(TObject *Sender);

    TTouchBtn *tbtnRemoveTax;
    void __fastcall tbtnRemoveTaxMouseClick(TObject *Sender);
    TTouchBtn *tbtnRemoveServiceCharge;
    void __fastcall tbtnRemoveServiceChargeMouseClick(TObject *Sender);
    TTouchBtn *tbtnSelectDiscount;
    void __fastcall tbtnSelectDiscountMouseClick(TObject *Sender);

    TTouchBtn *tbtnAddMinutes;
    void __fastcall tbtnAddMinutesMouseClick(TObject *Sender);

    TCheckBox *cbSaveCustomerName;
    TCheckBox *cbSaveMemberName;
    TCheckBox *cbSaveMemberAddress;
    void __fastcall cbSaveCustomerNameClick(TObject *Sender);
    void __fastcall cbSaveMemberNameClick(TObject *Sender);
    void __fastcall cbSaveMemberAddressClick(TObject *Sender);

    TCheckBox *cbOnlinePickupOrder;
    TCheckBox *cbOnlineDeliveryOrder;
    void __fastcall cbOnlinePickupOrderClick(TObject *Sender);
    void __fastcall cbOnlineDeliveryOrderClick(TObject *Sender);


private:	// User declarations
    void CreateFormatPopupMenu();
    void CreateMenuPopupMenu();
    void CreatePopupMenus();
    void CreateTypePopupMenu();
    void GetInput(const UnicodeString &prompt, UnicodeString &destination);
    void Redraw();

    std::auto_ptr<chit_options_prompt_t> chit_options_prompt_;
    std::auto_ptr<TfrmTouchKeyboard> keyboard_;
    std::auto_ptr<TfrmVerticalSelect> format_popup_menu_;
    std::auto_ptr<TfrmVerticalSelect> type_popup_menu_;
    std::auto_ptr<TfrmVerticalSelect> menu_popup_menu_;

    TChitNumber chit_;
    UnicodeString default_menu_name_;


    void  GetTaxes(TStringList* taxList, int type);
    bool ShowTaxRemovalForm(AnsiString caption, TStringList* taxList, TStringList* selectTaxList, TStringList* lastTaxList);
    void ShowNumPad(AnsiString caption);
    bool ValidateMinutes(int min);
    void ProcessData(int min);

    void TfrmChitNumberGui::GetDiscounts(TStringList* discountsList);
    bool ShowPointsAsDiscount;
    bool CheckPickupOrDelivery(AnsiString caption);
    bool CheckMenuExistsOrNot(UnicodeString default_menu_name_);



public:		// User declarations
   __fastcall TfrmChitNumberGui(TComponent *owner, const TChitNumber &chit);

    const TChitNumber &ChitNumber() const;
    TChitNumber &ChitNumber();

    const std::set<chit_name_t> &OptionsToBeAdded() const;
    const std::set<chit_name_t> &OptionsToBeDeleted() const;
    const std::map<chit_name_t, chit_name_t> &OptionsToBeRenamed() const;
};

#endif

