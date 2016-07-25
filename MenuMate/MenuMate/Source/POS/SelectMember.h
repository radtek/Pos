//---------------------------------------------------------------------------
#ifndef SelectMemberH
#define SelectMemberH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "enum.h"
//#include "ContactStaff.h"
#include "Membership.h"
#include <vector.h>
#include "MM_DBCore.h"
#include "Modules.h"
#include "ZForm.h"
#include "ManagerMembership.h"
#include "GroupGUI.h"
#include "TouchControls.h"
#include "TouchKeyboard.h"
#include "TouchBtn.h"
#include "TouchNumpad.h"
#include "TouchGrid.h"
#include "TouchPages.h"

#include "DeviceRealTerminal.h"
#include "StringTools.h"


class TfrmSelectMember : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
   TPanel *pnlKeyboard;
   TMMTouchKeyboard *TouchKeyboard1;
   TTouchNumpad *TouchNumpad1;
   TPanel *Panel4;
   TPanel *Panel3;
   TTouchBtn *btnSearch;
   TTouchBtn *btnClear;
   TMemo *edSearch;
   TPanel *Panel5;
   TPanel *pnlList;
   TPanel *Panel8;
   TTouchBtn *TouchBtn1;
   TTouchBtn *TouchBtn2;
	TTouchPages *tpDetails;
	TTouchSheet *tsDetails;
	TListBox *lbMemberText;
	TTouchSheet *TouchSheet2;
	TTouchGrid *tgridHistory;
	TListBox *tlistMembers;
	TPanel *Panel16;
	TTouchBtn *tbtnCancel;
	TTouchBtn *tbtnOk;
	TTouchBtn *btnAddUser;
	TTouchBtn *btnEditUser;
	TTouchBtn *tbtnSearchByName;
	TTouchBtn *tbtnSearchByNumber;
	TTouchBtn *tbtnSearchByGuest;
	TTouchBtn *tbtnInterface;
	TTouchBtn *btnDelete;
	TPanel *Panel2;
	TPanel *Panel6;
	TPanel *Panel7;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall tbtnCancelClick(TObject *Sender);
	void __fastcall tbtnClearClick(TObject *Sender);
	void __fastcall edSearchKeyPress(TObject *Sender, char &Key);
	void __fastcall tbtnSearchByNameClick(TObject *Sender);
	void __fastcall tbtnSearchByNumberClick(TObject *Sender);
	void __fastcall tbtnSearchByGuestClick(TObject *Sender);
	void __fastcall tbtnFullSearchClick(TObject *Sender);
	void __fastcall btnAddUserClick(TObject *Sender);
	void __fastcall btnEditUserClick(TObject *Sender);
	void __fastcall edSearchChange(TObject *Sender);
   void __fastcall TouchKeyboard1Click(TObject *Sender, char Char);
   void __fastcall btnSearchMouseClick(TObject *Sender);
   void __fastcall TouchNumpad2Click(TObject *Sender, TNumpadKey Key);
   void __fastcall tbtnInterfaceMouseClick(TObject *Sender);
   void __fastcall btnClearMouseClick(TObject *Sender);
   void __fastcall tgridHistoryMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tlistMembersClick(TObject *Sender);
   void __fastcall TouchBtn2AutoRepeat(TObject *Sender);
   void __fastcall TouchBtn1AutoRepeat(TObject *Sender);
   void __fastcall btnDeleteMouseClick(TObject *Sender);
	void __fastcall Panel8Resize(TObject *Sender);
private:	// User declarations
	__fastcall TfrmSelectMember(TComponent* Owner);
	
	void QuickSearch(AnsiString SearchString);
	void DisplayData(TMMContactInfo &TempUserInfo);
	eMemberSearchType SearchType;
	bool SelectionVisible;
	int  SetSelectedMember(int SelectedMember, bool filter);
   bool Filterlist;
   TMMContactInfo MemberInfo;

	void OptimizedFetchFilteredMembers(TIBSQL *, TContactType);
	const char *OptimizedFormatMemberName(AnsiString);

    void CloseFormIfPhoneOrdersEnabled();
    void LoadSelectedMemberInfoFromDB();

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
public:		// User declarations
	int SelectMember(std::vector<TQuickMember> &LastAccessed,eMemberDisplayFilter inFilter = eMemberAll);
	void RefreshDisplay();
	// Write to this string to set initial text.
	// Read from it on a ModalResult of mrOk for the result.
	AnsiString SearchString;
	int LastMemberKey;
	int Last3rdPartyMemberKey;
	int LastSearchIndex;
	bool MustHaveValidMember;
	bool StartWithShiftDown;
	bool MemberIncrementalSearch;
	eMemberDisplayFilter Filter;

   void SetFilterlist(bool temp);

//	TContactMember *Contacts;
	TMMContactInfo SelectedUser;
//	int SelectedKey;
//	int SelectedName;
	AnsiString Password;
	void UpdateInterface();
        TMMContactInfo getMemberInfo(void);
};

//---------------------------------------------------------------------------
#endif
