//---------------------------------------------------------------------------

#ifndef MemberCreationH
#define MemberCreationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ContactStaff.h"
#include "MMContactInfo.h"
#include "ManagerSyndCode.h"
#include "LogsDB.h"
#include "MM_DBCore.h"
#include <ExtCtrls.hpp>
#include "LoyaltyMateOperationDialogBox.h"
//---------------------------------------------------------------------------
using Database::TDBControl;
class TfrmMemberCreation : public TForm
{
__published:	// IDE-managed Components

    TPanel *Panel1;
    TTouchBtn *btnOk;
    TTouchBtn *TouchBtn1;
    TTouchBtn *btnEmail;
    TTouchBtn *btnLastName;
    TLabel *lbeEmail;
    TTouchBtn *btnContactPhone;
    TLabel *lbeContactPhone;
    TLabel *lbeLastName;
    TTouchBtn *TouchBtn2;
    TLabel *lbeName;
    TGroupBox *GroupBox1;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TTouchBtn *tbtnDay;
    TTouchBtn *tbtnMonth;
    TTouchBtn *tbtnYear;
    TTouchBtn *tbtnClearBirthday;
    TTouchBtn *tbtnSetBirthday;
    TTouchBtn *btnName;
    TGroupBox *gbLoyaltyMateActivation;
    TLabel *lbeLoyaltyMateActive;
    TTouchBtn *btnActivateLoyaltyMate;
    TLabel *lbeHeader;
    TCheckBox *cbNoEmail;
    void __fastcall EditCustomerBasicDetails(TObject *Sender);
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall TouchBtn1MouseClick(TObject *Sender);
    void __fastcall TouchBtn2MouseClick(TObject *Sender);
    void __fastcall tbtnDayMouseClick(TObject *Sender);
    void __fastcall tbtnMonthMouseClick(TObject *Sender);
    void __fastcall tbtnYearMouseClick(TObject *Sender);
    void __fastcall tbtnClearBirthdayMouseClick(TObject *Sender);
    void __fastcall tbtnSetBirthdayMouseClick(TObject *Sender);
    void __fastcall btnActivateLoyaltyMateMouseClick(TObject *Sender);
    void __fastcall cbNoEmailMouseClick(TObject *Sender);
    void SetupCustomerInfoPointers();
    void DisplayCustomerDataFromPointers();
    void getMemberDetailsFromActivationEmail();
    void toggleActivateAccountButton();
private:	// User declarations
        UnicodeString CustomerInfoPointers[4];
        TMMContactInfo &Info;
        int CheckEmailInDB(AnsiString email);
        void RefreshScreen();
public:		// User declarations
        __fastcall TfrmMemberCreation(TComponent* Owner, TMMContactInfo &info);
        bool Editing;
        TSyndCode CurrentSyndicateCode;
        	void DrawContactDetail();
            AnsiString GenMailId;


};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
