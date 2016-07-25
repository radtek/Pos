//---------------------------------------------------------------------------
#ifndef LoginH
#define LoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchNumpad.h"
#include <Graphics.hpp>
#include <jpeg.hpp>
#include <pngimage.hpp>
#include <vector>
#include "ContactStaff.h"
#include "enum.h"
#include "ZForm.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TfrmLogin : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *pnlLogin;
   TPanel *pnlKeyboard;
   TTouchGrid *tgUsers;
   TPanel *pnlCaption;
   TNumpadDisplay *NumpadDisplay;
   TTouchNumpad *TouchNumpad1;
   TTimer *tmFlashEnterPin;
   TLabel *lbeFlashPin;
   TPanel *pnlDisabled;
   TLabel *Label2;
   TTouchBtn *TouchBtn1;
   TPanel *pnlStaffMessage;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TMemo *memStaffMessage;
   TImage *Image1;
   TTimer *ClockTimer;
	TPanel *Panel1;
	TNumpadDisplay *ClockDisplay;
	TTouchBtn *tbtnCancel;
   void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCLRClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall tbtnReenableMouseClick(TObject *Sender);
   void __fastcall tgUsersMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);
   void __fastcall tbtnCancelMouseClick(TObject *Sender);
   void __fastcall tmFlashEnterPinTimer(TObject *Sender);
   void __fastcall TouchNumpad1Click(TObject *Sender, TNumpadKey Key);
   void __fastcall ClockTimerTimer(TObject *Sender);
   void __fastcall Image1Click(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange);
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmLogin(TComponent* Owner,Database::TDBTransaction &inDBTransaction);

	Database::TDBTransaction &DBTransaction;
    void GetApprovedMemberList(TStringList *List, bool isClockedInMember =false);
    bool CheckClockedInMember(Database::TDBTransaction &DBTransaction, int contact_key);
    void CheckPOSAccessFilter(TStringList *List);

public:		// User declarations
	static int LastUserKey,Last3rdPartyMemberKey;
 	UnicodeString LastUserName;
  	UnicodeString Password;

	TContact *Contacts;
	UnicodeString SearchString;
	std::vector<int> UserID;
	void RefreshDisplay();
	bool DisableManualLogin;
    bool AdminAttempt;
    int FlashTimer;
    SecurityCheck CheckRequiredAccess;
};
//---------------------------------------------------------------------------
int TfrmLogin::LastUserKey;
int TfrmLogin::Last3rdPartyMemberKey;


#endif
