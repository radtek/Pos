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
    TPanel *Panel4;
    TPanel *Panel2;
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *pnlKeyboard;
    TPanel *pnlDateOfbirth;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel13;
    TPageControl *pgControl;
    TTabSheet *tsGroups;
    TTabSheet *tsUser;
    TTabSheet *tsDiscounts;
    TTabSheet *tsPoints;
    TTabSheet *tsProfile;
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox6;
    TGroupBox *GroupBox3;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox4;
    TTouchGrid *tgDiscounts;
    TTouchGrid *AvailableGroupsGrid;
    TTouchGrid *CurrentGroupsGrid;
    TTouchBtn *tbtnUserProfile;
    TTouchBtn *tbtnPalmProfile;
    TTouchBtn *tbtnPoints;
    TTouchBtn *btnClose;
    TTouchBtn *tcbeprNoPointsRedemption;
    TTouchBtn *tcbeprNoPointsPurchases;
    TTouchBtn *tcbeprEarnsPointsWhileRedeemingPoints;
    TTouchBtn *tcbeprOnlyEarnsPointsWhileRedeemingPoints;
    TTouchBtn *tcbeprNeverEarnsPoints;
    TTouchBtn *tcbeprAllowedNegitive;
    TTouchBtn *tbtnAllowedTab;
    TTouchBtn *tbProximity;
    TTouchBtn *tbtnCancel;
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
    TTouchBtn *tcbeprFinancial;
    TTouchBtn *tcbeprAllowDiscounts;
    TTouchBtn *btnGroups;
    TTouchBtn *btnAccountProfile;
    TTouchBtn *btnRemoveCard;
    TTouchBtn *tbtnClearAllCards;
    TLabel *lbeSmartCardSpace;
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
    TLabel *lbePoints;
    TLabel *Label7;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *lbeLocationAddress;
    TLabel *lbeMailingAddress;
    TLabel *lbeDisplayedNote;
    TEdit *edFirstName;
    TEdit *edLastName;
    TEdit *edPhone;
    TEdit *edMobile;
    TEdit *edAddress;
    TEdit *edEmail;
    TEdit *edMemberNumber;
    TEdit *edKnownas;
    TEdit *edSex;
    TEdit *edNote;
    TRichEdit *reAddress;
    TRichEdit *reLocationAddress;
    TMMTouchKeyboard *TouchKeyboard1;
//    TDateTimePicker *DateTimePicker1;
    TTouchBtn *tbDay;
    TTouchBtn *tbMonth;
    TTouchBtn *tbYear;
    TCheckBox *cbCopyAddress;
    TCheckBox *CheckBoxCharges;
    TCheckBox *cbNoEmail;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
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
    void __fastcall TouchKeyboard1Click(TObject *Sender, char Char);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
   // void __fastcall cbCopyAddressClick(TObject *Sender);
    void __fastcall tbDayMouseClick(TObject *Sender) ;
    void __fastcall tbMonthMouseClick(TObject *Sender) ;
    void __fastcall tbYearMouseClick(TObject *Sender) ;
    void RichTextBoxValue(char valueChar);
    void PopulateRichTextBox(char valueChar, TRichEdit *edRichTextbox);
    bool ValidateDate(TDateTime datetime);
//    void __fastcall DateTimePicker1OnChange(TObject *Sender);

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
    bool SetBirthday();
    void ClearBirthday();
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
    bool IsProxCardChange;
};
// ---------------------------------------------------------------------------
// extern PACKAGE TfrmEditCustomer *frmEditCustomer;
// ---------------------------------------------------------------------------
#endif
