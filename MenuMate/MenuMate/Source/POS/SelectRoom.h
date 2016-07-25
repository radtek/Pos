//---------------------------------------------------------------------------
#ifndef SelectRoomH
#define SelectRoomH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include <Grids.hpp>
#include "enum.h"
#include "Rooms.h"
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
enum eSearchType {eSeachRoomByName, eSeachRoomByNumber, eSeachRoomByGuest};

class TfrmSelectRoom : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlKeys;
	TPanel *Panel3;
	TLabel *Label1;
	TLabel *Label2;
	TPanel *Panel4;
	TLabel *Label3;
	TLabel *Label4;
	TPanel *Panel5;
	TLabel *Label5;
	TLabel *Label6;
	TPanel *Panel6;
	TLabel *Label7;
	TLabel *Label8;
	TPanel *Panel7;
	TLabel *Label9;
	TLabel *Label10;
	TPanel *Panel8;
	TLabel *Label11;
	TLabel *Label12;
	TPanel *Panel9;
	TLabel *Label13;
	TLabel *Label14;
	TPanel *Panel10;
	TLabel *Label15;
	TLabel *Label16;
	TPanel *Panel11;
	TLabel *Label17;
	TLabel *Label18;
	TPanel *Panel12;
	TLabel *Label19;
	TLabel *Label20;
	TPanel *Panel13;
	TLabel *Label21;
	TLabel *Label22;
	TPanel *Panel14;
	TLabel *Label23;
	TLabel *Label24;
	TPanel *Panel15;
	TLabel *Label25;
	TLabel *Label26;
	TPanel *pnlBackSpace;
	TPanel *pnlTab;
	TPanel *pnlQ;
	TPanel *pnlW;
	TPanel *pnlE;
	TPanel *pnlR;
	TPanel *pnlT;
	TPanel *pnlY;
	TPanel *pnlU;
	TPanel *pnlI;
	TPanel *pnlO;
	TPanel *pnlP;
	TPanel *Panel28;
	TLabel *Label39;
	TPanel *Panel29;
	TLabel *Label40;
	TPanel *Panel30;
	TLabel *Label41;
	TLabel *Label42;
	TLabel *Label43;
	TLabel *Label44;
	TPanel *pnlCapsLock;
	TPanel *pnlA;
	TPanel *pnlS;
	TPanel *pnlD;
	TPanel *pnlF;
	TPanel *pnlG;
	TPanel *pnlH;
	TPanel *pnlJ;
	TPanel *pnlK;
	TPanel *pnlL;
	TPanel *Panel41;
	TPanel *Panel42;
	TLabel *Label55;
	TLabel *Label56;
	TLabel *Label58;
	TLabel *Label59;
	TPanel *pnlShift1;
	TPanel *pnlZ;
	TPanel *pnlX;
	TPanel *pnlC;
	TPanel *pnlV;
	TPanel *pnlB;
	TPanel *pnlN;
	TPanel *pnlM;
	TPanel *Panel53;
	TPanel *Panel54;
	TPanel *Panel55;
	TLabel *Label69;
	TLabel *Label70;
	TLabel *Label71;
	TLabel *Label72;
	TLabel *Label73;
	TLabel *Label74;
	TPanel *pnlSpace;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel16;
	TPanel *Panel19;
	TPanel *Panel21;
	TMemo *memText;
	TPanel *Panel22;
	TStringGrid *StringGrid1;
	TTouchBtn *btnUserUp;
	TTouchBtn *btnUserDown;
	TEdit *edSearch;
	TLabel *Label32;
	TLabel *lbeCaption;
	TPanel *pnlScrollRooms;
	TPanel *pnlRooms;
	TTouchBtn *btnRoomsLeft;
	TTouchBtn *btnRoomsRight;
	TTouchBtn *tbtnSearchByName;
	TTouchBtn *tbtnSearchByNumber;
	TTouchBtn *tbtnSearchByGuest;
	TTouchBtn *tbtnOk;
	TTouchBtn *tbtnCancel;
	TTouchBtn *pnlEnter;
	TTouchBtn *tbtnFullSearch;
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
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall tbtnFullSearchClick(TObject *Sender);
	void __fastcall StringGrid1Click(TObject *Sender);
	void __fastcall btnRoomsRightMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Panel1Resize(TObject *Sender);
	void __fastcall btnRoomsLeftMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnUserUpAutoRepeat(TObject *Sender);
	void __fastcall btnUserDownAutoRepeat(TObject *Sender);
private:	// User declarations
	__fastcall TfrmSelectRoom(TComponent* Owner,Database::TDBTransaction &DBTransaction);
	
	Database::TDBTransaction &DBTransaction;
	bool ShiftDown;
	bool CapsDown;
	void DrawUpDownPanel(bool Down, TPanel *Panel);
	void DrawLetters();
	int CurrentRoomKey;
	void QuickSearch(AnsiString SearchString);
	void DisplayRoomData();
	eSearchType SearchType;
	void __fastcall QuickRoomClick(TObject *Sender);
	TListRoomContainer *RoomsList;
	void SlideRoomsLeft();
	void SlideRoomsRight();
	TRoomData * SetSelectedRoom(TRoomData * inRoomSelected);
	bool SelectionVisible;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
//	  MESSAGE_HANDLER(UWM_CARDSWIPE, TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)

public:		// User declarations
	void RefreshDisplay();
	// Write to this string to set initial text.
	// Read from it on a ModalResult of mrOk for the result.
	AnsiString SearchString;
	int LastRoomKey;
	int LastSearchIndex;
	bool MustHaveValidRoom;
	bool StartWithShiftDown;
	eRoomDisplayFilter Filter;
	eSearchType DefaultSeatchType;

	TRoomData *SelectedRoom;
};
//---------------------------------------------------------------------------
#endif
