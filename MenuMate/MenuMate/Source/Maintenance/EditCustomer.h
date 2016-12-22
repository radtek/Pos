// ---------------------------------------------------------------------------

#ifndef EditCustomerH
#define EditCustomerH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <DB.hpp>
#include <ComCtrls.hpp>
#include "ContactStaff.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "GroupGUI.h"
#include "ManagerDiscount.h"
#include "TouchKeyboard.h"

class TfrmLoyaltyMateOperationDialogBox;
class TLoyaltyMateDownloadMemberThread;

// ---------------------------------------------------------------------------
class TfrmEditCustomer : public TZForm
{
__published: // IDE-managed Components
	TPanel *pnlButtons;
	TTouchBtn *btnClose;
	TPageControl *pgControl;
	TTabSheet *tsUser;
	TPanel *Panel1;
	TLabel *lbeSmartCardSpace;
	TGroupBox *GroupBox2;
	TPanel *Panel3;
	TTouchBtn *tbtnUserProfile;
	TTouchBtn *tbtnPalmProfile;
	TTabSheet *tsDiscounts;
	TPanel *Panel2;
	TGroupBox *GroupBox6;
	TTouchGrid *tgDiscounts;
	TTouchBtn *tbtnPoints;
	TTabSheet *tsPoints;
	TPanel *Panel4;

	TTouchBtn *tcbeprNoPointsRedemption;
	TTouchBtn *tcbeprNoPointsPurchases;
	TTouchBtn *tcbeprEarnsPointsWhileRedeemingPoints;
	TTouchBtn *tcbeprOnlyEarnsPointsWhileRedeemingPoints;
	TTouchBtn *tcbeprNeverEarnsPoints;
	TTouchBtn *tcbeprAllowedNegitive;
	TTouchBtn *tbtnAllowedTab;
	TPanel *Panel5;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox5;
	TLabel *lbePoints;
	TGroupBox *GroupBox3;
	TLabel *Label7;
	TGroupBox *GroupBox4;
	TTouchBtn *btnSwipe;
	TTouchBtn *tbProximity;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TTouchBtn *tbtnCancel;
	TPanel *Panel6;
	TTabSheet *tsGroups;
	TPanel *Panel7;
	TPanel *Panel8;
	TPanel *Panel9;
	TTouchGrid *CurrentGroupsGrid;
	TTouchBtn *btnGroups;
	TTouchGrid *AvailableGroupsGrid;
	TTouchBtn *btnAccountProfile;
	TTabSheet *tsProfile;
	TPanel *Panel10;
	TCheckBox *CheckBoxCharges;
    TCheckBox *cbNoEmail;
	TPanel *Panel11;
	TPanel *Panel13;
    TTouchBtn *btnName;
    TTouchBtn *btnKnownAs;
    TTouchBtn *btnMemberNumber;
    TTouchBtn *btnTitle;
    TTouchBtn *btnSex;
    TTouchBtn *btnContactPhone;
	TTouchBtn *btnMobile;
	TTouchBtn *btnEmail;
    TTouchBtn *btnMailingAddress;
    TTouchBtn *btnLocationAddress;
    TTouchBtn *btnDisplayedNote;
    TLabel *lbeLocationAddress;
    TLabel *lbeMailingAddress;
    TLabel *lbeDisplayedNote;
	TTouchBtn *tcbeprFinancial;
    TTouchBtn *tcbeprAllowDiscounts;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnSwipeClick(TObject *Sender);
	void __fastcall btnRemoveCardClick(TObject *Sender);
	void __fastcall tbtnAllowedTabClick(TObject *Sender);
	void __fastcall tbtnClearAllCardsClick(TObject *Sender);
	void __fastcall tbtnUserProfileMouseClick(TObject *Sender);
	void __fastcall tbtnPalmProfileMouseClick(TObject *Sender);
	void __fastcall tgDiscountsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tcbeprNoPointsRedemptionClick(TObject *Sender);
	void __fastcall tcbeprNoPointsPurchasesClick(TObject *Sender);
	void __fastcall tcbeprEarnsPointsWhileRedeemingPointsClick(TObject *Sender);
	void __fastcall tcbeprOnlyEarnsPointsWhileRedeemingPointsClick(TObject *Sender);
	void __fastcall tcbeprNeverEarnsPointsClick(TObject *Sender);
	void __fastcall tcbeprAllowedNegitiveClick(TObject *Sender);
	void __fastcall tbtnPointsMouseClick(TObject *Sender);

	void __fastcall btnGroupsMouseClick(TObject *Sender);
	void __fastcall AvailableGroupsGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall CurrentGroupsGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall btnAccountProfileMouseClick(TObject *Sender);
	void __fastcall CheckBoxChargesMouseClick(TObject *Sender);
	void __fastcall tbProximityMouseClick(TObject *Sender);
	void __fastcall cbNoEmailMouseClick(TObject *Sender);
    void __fastcall tcbeprFinancialClick(TObject *Sender);
    void __fastcall tcbeprAllowDiscountsClick(TObject *Sender);


    TLabel *lblFirstname;
    TLabel *lblLasttname;
    TLabel *lblPhone;
    TLabel *lblMobile;
    TLabel *lblAddress;
    TLabel *lblEmail;
    TLabel *lblMemberNumber;
    TLabel *lblKnownas;
    TLabel *lblSex;
    TLabel *lblNote;
    TLabel *lblLocation;

    TEdit *edFirstName;
    TEdit *edLastName;
    TEdit *edPhone;
    TEdit *edMobile;
    TEdit *edAddress;
    TRichEdit *reAddress;
    TRichEdit *reLocationAddress;
    TEdit *edEmail;
    TEdit *edMemberNumber;
    TEdit *edKnownas;
    TEdit *edSex;
    TEdit *edNote;
    TDateTimePicker *DateTimePicker1;
    TPanel *pnlKeyboard;
    TMMTouchKeyboard *TouchKeyboard1;
    TPanel *pnlDateOfbirth;

    TCheckBox *cbCopyAddress;
    void __fastcall TouchKeyboard1Click(TObject *Sender, char Char);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall cbCopyAddressClick(TObject *Sender);
    void RichTextBoxValue(char valueChar);
    void PopulateRichTextBox(char valueChar, TRichEdit *edRichTextbox);
    bool ValidateDate(TDateTime datetime);
    void __fastcall DateTimePicker1OnChange(TObject *Sender);


protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)END_MESSAGE_MAP(TForm)

private: // User declarations
	__fastcall TfrmEditCustomer(TComponent* Owner);

	static TForm *WinOwner;
    UnicodeString *CustomerInfoPointers[11];

	void __fastcall CreateParams(Controls::TCreateParams &params);
	void RedrawButtons(TObject * Sender);
	void RedrawDiscounts();
	void DisplayPage();
	void DisplayAvailableGroups(void);
	void DisplayCurrentGroups(void);
	void UpdateDataBase(void);
	std::vector<ContactGroup> GroupInfo;
	ContactGroup FindGroupData(UnicodeString groupname);
	ContactGroup FindGroupData(int groupkey);
    void SetupCustomerInfoPointers();
    void DisplayCustomerDataFromPointers();
    UnicodeString RemoveLineBreaks(UnicodeString line);

    void ChangeTextBoxFocus();
    void TextBoxValue(char valueChar);
    void PopulateTextBox(char valueChar, TEdit *edTextbox);
    void ValidateTextBox();
    void SaveData();
    bool ValidateEmailId();
    bool SeparateName(UnicodeString name);
    void ProcessData();
    void PopulateDateTimePicker(char valueChar, TDateTimePicker *dateTime);
    bool CheckMobileNumber(UnicodeString mobile);
    void UpdatePointsRuleOldMember();
    void UpdatePointsRuleNewMember();
    void LoadPointsRuleToContact();
public: // User declarations
	TMMContactInfo Info;
	std::auto_ptr <TStringList> DiscountList;

	static TfrmEditCustomer *Create(TForm* Owner);
	__fastcall virtual ~TfrmEditCustomer();
	void __fastcall UpdateSmartCardSpace();
	void DrawContactDetails();

	bool Editing;
	bool ClearAllCards;
    int MemberType;
};
// ---------------------------------------------------------------------------
// extern PACKAGE TfrmEditCustomer *frmEditCustomer;
// ---------------------------------------------------------------------------
#endif
