//---------------------------------------------------------------------------
#ifndef NewUserH
#define NewUserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "MMContactInfo.h"
#include "Access.h"

//---------------------------------------------------------------------------
class TfrmNewUser : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlButtons;
	TPageControl *pgControl;
	TTabSheet *tsUser;
	TTabSheet *tsPayroll;
	TTabSheet *tsPalm;
	TPanel *Panel1;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TTouchBtn *btnName;
	TTouchBtn *btnInitials;
	TTouchBtn *btnAccess;
	TTouchBtn *btnPIN;
	TTouchBtn *btnSwipe;
	TTouchBtn *tbProximity;
	TPanel *Panel3;
	TPanel *Panel11;
	TPanel *Panel5;
	TTouchBtn *cbAdvancedUser;
	TTouchBtn *cbAutoPopup;
	TTouchBtn *cbScreenLock;
	TTouchBtn *cbGetAllTables;
	TTouchBtn *cbLargeFont;
	TLabel *Label8;
	TTouchBtn *btnLogoff;
	TRadioGroup *rgSoundLevel;
	TLabel *Label18;
	TLabel *PayrollID;
	TTouchBtn *btnPayrollID;
	TTouchBtn *btnAllpyPayrollTimesToAll;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label9;
	TLabel *Label10;
	TDateTimePicker *dtpStart6;
	TDateTimePicker *dtpStart5;
	TDateTimePicker *dtpStart4;
	TDateTimePicker *dtpStart3;
	TDateTimePicker *dtpStart2;
	TDateTimePicker *dtpStart1;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TPanel *Panel4;
	TTouchBtn *cbClassicMode;
	TTouchBtn *btnRemoveStaffCard;
	TTouchBtn *tbtnClearAllCards;
   TTouchBtn *btnClose;
   TTouchBtn *tbtnUserProfile;
   TTouchBtn *tbtnPalmProfile;
   TTouchBtn *tbtnPayroll;
	TTouchBtn *tbtnCancel;
	TTouchBtn *tbtnPointsPage;
   TTabSheet *tsPoints;
   TPanel *Panel2;
	TTouchBtn *tcbeprNoPointsRedemption;
	TTouchBtn *tcbeprNoPointsPurchases;
	TTouchBtn *tcbeprOnlyEarnsPointsWhileRedeemingPoints;
	TTouchBtn *tcbeprEarnsPointsWhileRedeemingPoints;
	TTouchBtn *tcbeprNeverEarnsPoints;
	TTouchBtn *tcbeprAllowedNegitive;
	TTouchBtn *btnAllowedTab;
	TLabel *Label19;
	TTouchBtn *btnHourRate;
	void __fastcall btnAcceptClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnNameClick(TObject *Sender);
	void __fastcall btnInitialsClick(TObject *Sender);
	void __fastcall btnAccessClick(TObject *Sender);
	void __fastcall btnPINClick(TObject *Sender);
	void __fastcall btnLogoffClick(TObject *Sender);
	void __fastcall btnSwipeClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
   void __fastcall btnAllpyPayrollTimesToAllClick(TObject *Sender);
   void __fastcall btnPayrollIDClick(TObject *Sender);
	void __fastcall tbProximityClick(TObject *Sender);
	void __fastcall btnAllowedTabClick(TObject *Sender);
	void __fastcall tbUserProfileClick(TObject *Sender);
	void __fastcall tbPalmProfileClick(TObject *Sender);
	void __fastcall tbAcePayrollClick(TObject *Sender);
   void __fastcall btnRemoveStaffCardClick(TObject *Sender);
   void __fastcall tbtnClearAllCardsClick(TObject *Sender);
   void __fastcall tbtnPointsPageMouseClick(TObject *Sender);
	void __fastcall btnHourRateMouseClick(TObject *Sender);

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmNewUser(TComponent* Owner,Database::TDBControl &inDBControl);
	
	TAccess AccessLevel;
	AnsiString PIN1, PIN2;
	AnsiString CardString,ProxString;
	bool ClearAllCards;
	void RedrawButtons(TObject * Sender);
	Database::TDBControl &DBControl;
	bool duplicate_prox_detected;


public:		// User declarations

	TMMContactInfo Info;
	bool Editing;
   long User_Key;
//   unsigned char NextUserID;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
