//---------------------------------------------------------------------------
#ifndef SelectPhoenixRoomH
#define SelectPhoenixRoomH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "enum.h"
#include "ContactStaff.h"
#include <vector.h>
#include "PhoenixHotelSystem.h"
#include "SiHotDataObjects.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------

class TfrmPhoenixRoom : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlNumPad;
	TPanel *Panel3;
	TLabel *Label2;
	TPanel *Panel4;
	TLabel *Label4;
	TPanel *Panel5;
	TLabel *Label6;
	TPanel *Panel6;
	TLabel *Label8;
	TPanel *Panel7;
	TLabel *Label10;
	TPanel *Panel9;
	TLabel *Label14;
	TPanel *Panel10;
	TLabel *Label16;
	TPanel *Panel11;
	TLabel *Label18;
	TPanel *Panel12;
	TLabel *Label20;
	TPanel *Panel13;
	TLabel *Label22;
	TPanel *pnlBackSpace;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel16;
	TPanel *Panel19;
	TPanel *Panel21;
	TMemo *memText;
	TPanel *Panel22;
	TStringGrid *List;
	TTouchBtn *btnUserUp;
	TTouchBtn *btnUserDown;
	TEdit *edSearch;
	TLabel *Label32;
	TLabel *lbeCaption;
	TPanel *pnlScroll;
	TPanel *pnlLastPicked;
	TTouchBtn *btnRoomsLeft;
	TTouchBtn *btnRoomsRight;
	TTouchBtn *tbtnSearchByName;
	TTouchBtn *tbtnSearchByNumber;
	TTouchBtn *tbtnSearchByGuest;
	TTouchBtn *tbtnOk;
	TTouchBtn *tbtnCancel;
	TTouchBtn *pnlEnter;
	TPanel *pnlKeys;
	TPanel *Panel18;
	TLabel *Label27;
	TLabel *Label28;
	TPanel *Panel20;
	TLabel *Label29;
	TLabel *Label30;
	TPanel *Panel23;
	TLabel *Label31;
	TLabel *Label33;
	TPanel *Panel24;
	TLabel *Label34;
	TLabel *Label35;
	TPanel *Panel25;
	TLabel *Label36;
	TLabel *Label37;
	TPanel *Panel26;
	TLabel *Label38;
	TLabel *Label45;
	TPanel *Panel27;
	TLabel *Label46;
	TLabel *Label47;
	TPanel *Panel31;
	TLabel *Label48;
	TLabel *Label49;
	TPanel *Panel32;
	TLabel *Label50;
	TLabel *Label51;
	TPanel *Panel33;
	TLabel *Label52;
	TLabel *Label53;
	TPanel *Panel34;
	TLabel *Label54;
	TLabel *Label57;
	TPanel *Panel35;
	TLabel *Label60;
	TLabel *Label61;
	TPanel *Panel36;
	TLabel *Label62;
	TLabel *Label63;
	TPanel *Panel37;
	TPanel *Panel38;
	TPanel *Panel39;
	TPanel *Panel40;
	TPanel *Panel43;
	TPanel *Panel44;
	TPanel *Panel45;
	TPanel *Panel46;
	TPanel *Panel47;
	TPanel *Panel48;
	TPanel *Panel49;
	TPanel *Panel50;
	TPanel *Panel51;
	TLabel *Label64;
	TLabel *Label65;
	TPanel *Panel52;
	TLabel *Label66;
	TLabel *Label67;
	TPanel *Panel56;
	TLabel *Label68;
	TLabel *Label75;
	TPanel *pnlCapsLock;
	TPanel *Panel58;
	TPanel *Panel59;
	TPanel *Panel60;
	TPanel *Panel61;
	TPanel *Panel62;
	TPanel *Panel63;
	TPanel *Panel64;
	TPanel *Panel65;
	TPanel *Panel66;
	TPanel *Panel67;
	TLabel *Label76;
	TLabel *Label77;
	TPanel *Panel68;
	TLabel *Label78;
	TLabel *Label79;
	TPanel *pnlShift1;
	TPanel *Panel70;
	TPanel *Panel71;
	TPanel *Panel72;
	TPanel *Panel73;
	TPanel *Panel74;
	TPanel *Panel75;
	TPanel *Panel76;
	TPanel *Panel77;
	TLabel *Label80;
	TLabel *Label81;
	TPanel *Panel78;
	TLabel *Label82;
	TLabel *Label83;
	TPanel *Panel79;
	TLabel *Label84;
	TLabel *Label85;
	TPanel *Panel80;
	TTouchBtn *TouchButton1;
	TPanel *Panel8;
	TLabel *Label1;
	TPanel *Panel14;
	TLabel *Label3;
	TTouchBtn *tbtnKeyboard;
	TTouchBtn *btnNumPad;
	TPanel *Panel15;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ShiftClick(TObject *Sender);
	void __fastcall ButtonMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CapsLockClick(TObject *Sender);
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
	void __fastcall ListDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnUserUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnUserDownMouseDown(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, int X, int Y);
	void __fastcall ListClick(TObject *Sender);
	void __fastcall btnRoomsRightMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Panel1Resize(TObject *Sender);
	void __fastcall btnRoomsLeftMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnNumPadClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmPhoenixRoom(TComponent* Owner);
	
	int MaxRemembered;
	bool ShiftDown;
	bool CapsDown;
	void DrawUpDownPanel(bool Down, TPanel *Panel);
	void DrawLetters();
	void QuickSearch();
	void DisplayData();
	void __fastcall QuickClick(TObject *Sender);
	void SlideLeft();
	void SlideRight();
	void ShowAccount();
	int  SetSelectedFolder(int FolderNumber);
	bool SelectionVisible;
	void RefreshList();
    void SearchPhoenix(TPhoenixRoomStatusExt SelectedRoom);
    void SearchSiHot(TPhoenixRoomStatusExt SelectedRoom);
    void SearchOracle(TPhoenixRoomStatusExt SelectedRoom);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
	std::vector<AnsiString> LastAccessed;
public:		// User declarations
   AnsiString PMSIPAddress;
   int PMSPort;

	SelectRoom(AnsiString inPMSIPAddress,int inPMSPort);
	// Write to this string to set initial text.
	// Read from it on a ModalResult of mrOk for the result.
	bool MustHaveValidRoom;
	bool StartWithShiftDown;
	eVisibleInterface	DefaultInterfaceDisplay;
	TPhoenixRoomStatusExt SelectedRoom;
	void UpdateInterface();
    std::vector<TSiHotAccounts> SiHotAccounts;
    double LimitSiHot;
};
//---------------------------------------------------------------------------

#endif
