//---------------------------------------------------------------------------
#ifndef SelectRMSRoomH
#define SelectRMSRoomH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "enum.h"
#include <vector.h>
#include "ZForm.h"
#include "TouchControls.h"
#include "TouchKeyboard.h"
#include "TouchBtn.h"
#include "TouchNumpad.h"
#include "TouchGrid.h"
#include "TouchPages.h"
#include <vector>
#include "RMSRoom.h"
#include "NewBookRoom.h"
#include "MMContactInfo.h"
enum eRoomSearchType {eRoomSearchByName, eRoomSearchByNumber};

class TfrmRMSRoom : public TZForm
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
	TListBox *tlistRooms;
	TPanel *Panel16;
	TTouchBtn *tbtnCancel;
	TTouchBtn *tbtnOk;
	TTouchBtn *tbtnSearchByName;
	TTouchBtn *tbtnSearchByNumber;
	TTouchBtn *tbtnInterface;
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
	void __fastcall tbtnFullSearchClick(TObject *Sender);
	void __fastcall edSearchChange(TObject *Sender);
   void __fastcall TouchKeyboard1Click(TObject *Sender, char Char);
   void __fastcall btnSearchMouseClick(TObject *Sender);
   void __fastcall TouchNumpad2Click(TObject *Sender, TNumpadKey Key);
   void __fastcall tbtnInterfaceMouseClick(TObject *Sender);
   void __fastcall btnClearMouseClick(TObject *Sender);
   void __fastcall tgridHistoryMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tlistRoomsClick(TObject *Sender);
   void __fastcall TouchBtn2AutoRepeat(TObject *Sender);
   void __fastcall TouchBtn1AutoRepeat(TObject *Sender);
	void __fastcall Panel8Resize(TObject *Sender);
private:	// User declarations
	__fastcall TfrmRMSRoom(TComponent* Owner);

	void QuickSearch(AnsiString SearchString);
	void DisplayData();
	eRoomSearchType SearchType;
	bool SelectionVisible;
	int  SetSelectedRoom(int inAccountNumber, bool filter);
	bool Filterlist;
	int LastSearchIndex;
	eVisibleInterface DefaultInterfaceDisplay;
    int  SetSelectedNewBookRoom(int inRoomNumber, bool filter);

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
public:		// User declarations
	void RefreshDisplay();
	// Write to this string to set initial text.
	// Read from it on a ModalResult of mrOk for the result.
	AnsiString SearchString;
	bool MustHaveValidResult;
	bool StartWithShiftDown;
	bool IncrementalSearch;

   void SetFilterlist(bool temp);
   void UpdateInterface();
	std::vector<TRMSRoom> RoomsList;
	UnicodeString CsvFilePath;
	TRMSRoom SelectedRoom;
	SelectRoom(UnicodeString CsvFilePath);
	void LoadRooms(UnicodeString inCsvFilePath);
	TStringList *GetRoomsList();
    void LoadNewBookRooms(UnicodeString inCsvFilePath);
    std::vector<TNewBookRoom> NewBookRoomsList;
    SelectNewBookRoom(UnicodeString CsvFilePath);
    TNewBookRoom  NewBookSelectedRoom ;
    void DisplayNewBookRoomData();
     void RefreshNewBookDisplay();
};

#endif






