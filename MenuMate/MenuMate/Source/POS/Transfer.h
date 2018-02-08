//---------------------------------------------------------------------------
#ifndef TransferH
#define TransferH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Grids.hpp>
#include "enum.h"
#include "ContactStaff.h"
#include "MM_DBCore.h"
#include "Modules.h"
#include "SystemEvents.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "DBContacts.h"
#include <vector>
#include "ZForm.h"
#include "ChefmateClientManager.h"
using Database::TDBTransaction;

//---------------------------------------------------------------------------
class TItemInfo
{
  public:
      int Tabkey;
      int OrderKey;
      AnsiString ItemName;
      double ItemQty;
      bool IsSideItem;
      AnsiString ItemSize;
      AnsiString ItemPrice;
      int SeatNo;
      AnsiString TabName;
      bool SetMenuMask;
      bool count;
      bool isWeighted;
      bool CheckHasSide;
      int Security_ref;
      Currency Item_Price;
      bool IsPriceAdjust;
};

class TTabDetails
{
  public:
      int Tabkey;
      bool IsTabKey;
      UnicodeString TabName;
      std::vector<TItemInfo> TabItems;
};

struct TTabColour
{
  public:
      int Tabkey;
      bool IsTabKey;
      int Orderkey;
      bool IsSideItem;
      double Qty;
      AnsiString ItemPrice;
      int RowCount;
      AnsiString ItemName;
      AnsiString ItemSize;
      std::vector<int> SaveOrderKey;
      std::vector<double> SaveItemQty;
      AnsiString TabName;
      bool SetMenuMask;
      bool count;
      bool isWeighted;
      bool CheckHasSide;
      int Security_ref;
      Currency Item_Price;
      bool IsPriceAdjust;
};

struct TTransferChefMateItem
{
  public:
      int DestinationType;
      UnicodeString CurrentSource;
      UnicodeString CurrentDest;
      AnsiString Username;
      int TimeKey;
      std::vector<TItemComplete *> Items;
};

struct TCheckTransferDetails
{
  public:
      int DestinationType;
      UnicodeString CurrentSource;
      UnicodeString CurrentDest;
      AnsiString Username;
};



class TfrmTransfer : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlSourceSeats;
	TScrollBox *sbSourceSeats;
	TTouchBtn *btnSeatsUp;
	TTouchBtn *btnSeatsDown;
    TPanel *PnlBorder;
	TPanel *PnlHeader;
    TPanel *PnlScreen;
	TStaticText *StHeader;
    TPanel *PnlOkCloseBtn;
	TTouchBtn *btnClose;
	TTouchBtn *tbNewTab;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);

    TTouchBtn *btnUp;
    TTouchBtn *btnDown;
    TTouchBtn *btnSourceUp;
    TTouchBtn *btnSourceDown;

	TTouchBtn *btnTransferFrom;
	TTouchBtn *btnTransferTo;
    TTouchBtn *btnOK;
    TGroupBox *gbTransferFrom;
    TGroupBox *gbTransferTo;
    TLabel *lbeTransferFrom;
    TLabel *lbeTransferTo;
    TListBox *lbDisplayTransferfrom;
    TListBox *lbDisplayTransferto;
    TTimer *TimerLongPress;
    TTimer *TimerDestLongPress;
    void __fastcall TimerLongPressTimer(TObject *Sender);
    void __fastcall TimerDestLongPressTimer(TObject *Sender);
    void __fastcall btnTransferFromClick(TObject *Sender);
    void __fastcall btnTransferToClick(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall lbDisplayTransferfromDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
    void __fastcall lbDisplayTransferfromMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall lbDisplayTransferfromClick(TObject *Sender);
    void __fastcall lbDisplayTransfertoDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
    void __fastcall lbDisplayTransfertoClick(TObject *Sender);
    void __fastcall btnUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnSourceUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnSourceDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //
    void __fastcall lbDisplayTransferfromMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall lbDisplayTransfertoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall lbDisplayTransfertoMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)

private:	// User declarations
   TListSortCompare ptrSortFunc;
    void ClearListBox(TListBox *listBox);
    std::auto_ptr<TChefmateClientManager> cmClientManager; //For Chefmate Transfer request
    void transferOrderToChefMate(TList* orderList, TTransferComplete *Transfer);

    Database::TDBControl &DBControl;
	void UpdateSourceTableDetails(Database::TDBTransaction &DBTransaction);
	void UpdateSourceSeatDetails(Database::TDBTransaction &DBTransaction, bool isItemTransfered = false);
	void UpdateDestTableDetails(Database::TDBTransaction &DBTransaction);
	void UpdateDestSeatDetails(Database::TDBTransaction &DBTransaction);
	std::vector<long> SourceTabKeys;
	TTouchBtn *CurrentSourceSeatBtn;
	TTouchBtn *CurrentDestSeatBtn;
	void __fastcall SourceSeatClick(TObject *Sender);
	void __fastcall DestSeatClick(TObject *Sender);
	bool SourceSeatScrollUp;
	bool DestSeatScrollUp;
	TCustomer GetAndSaveCustomer( Database::TDBTransaction &DBTransaction, const int InvoiceKey );
	bool TabLockedOk(Database::TDBTransaction &DBTransaction, int inTabKey);
	bool TabPINOk(Database::TDBTransaction &DBTransaction,int inTabKey);
	bool TabStaffAccessOk(Database::TDBTransaction &DBTransaction,int inTabKey);

	AnsiString DisplayKeypad( AnsiString inCaption );
	int DisplayNumpad( AnsiString inCaption );
    void TransferTotal(int source_tabkey, int dest_tabkey, bool isReverse, bool isTabSelected);
    void UpdateTablePartyName(Database::TDBTransaction &DBTransaction, long DestTabKey, long SourceKey, bool isTabSelected);
    UnicodeString GetTabNameForTable(Database::TDBTransaction &DBTransaction, long source_key, bool isTabSelected = false);
    void PrepareItemList(Database::TDBTransaction &DBTransaction, int sourceKey, bool isTabSelected, TList *Orders);
    bool ShowSourceMembersTab(Database::TDBTransaction &DBTransaction);
    bool ShowDestMembersTab(Database::TDBTransaction &DBTransaction);
    TModalResult NewInvoiceItems(Database::TDBTransaction &DBTransaction);
    void ProcessInvoice(Database::TDBTransaction &DBTransaction, long key, bool isTabSelected,bool ReverseTransfer);
    bool CheckItemsOnTab(Database::TDBTransaction &DBTransaction,int inTabKey);
    TModalResult AddNewTab(Database::TDBTransaction &DBTransaction);
    void UpdateDestRooms(Database::TDBTransaction &DBTransaction);
    void UpdateSourceRooms(Database::TDBTransaction &DBTransaction);
    bool CheckStaffTabAccess(Database::TDBTransaction &DBTransaction); //
    //void PopulateSourceDestTabDetails(Database::TDBTransaction &DBTransaction, UnicodeString Name, int key, TListBox *listBox, bool isTabKeyEmpty = false);
    void PopulateSourceDestTabDetails(Database::TDBTransaction &DBTransaction, UnicodeString Name, int key, TListBox *listBox, bool isTabKeyEmpty = false);
    void DisplayItemsSourceList(TListBox *listBox, TTabColour *TabColor, UnicodeString Name);
    void UpdateListBox(TListBox *listBox);
    int GetTabKeyFromListBox(TListBox *listBox, int i);
    bool CheckTabOrItemSelected(TListBox *listBox, int i);
    TModalResult ShowTabDetails(Database::TDBTransaction &DBTransaction, int TabType, TListBox *listbox, TTouchBtn *btntransfer, AnsiString title, AnsiString Section);
    UnicodeString GetSourceTabName(Database::TDBTransaction &DBTransaction, long source_key);
    void TransferData(Database::TDBTransaction &DBTransaction);
    void ReverseData(Database::TDBTransaction &DBTransaction);
    bool CompareTabName(UnicodeString src_tab, UnicodeString dest_tab);
    void ReverseTransferTotal(int source_tabkey, int dest_tabkey, bool isReverse, bool isTabSelected);
    bool DestTabStaffAccessOk(Database::TDBTransaction &DBTransaction, int inTabKey);
    bool CheckDesttaffTabAccess(Database::TDBTransaction &DBTransaction);
    void UpdateChefmateTransferList(TList *OrderListItem, int source_key, int dest_key, bool isTabSelected);
    void TransferOrderListtoChefmate();
    void TotalTransferTableOrTab(Database::TDBTransaction &DBTransaction);
    void CollectDataForChefmateTransfer(int dest_key, TList *OrdersList, TListBox *listBox, bool isReverseTransfer = false);
    void ShowSelectScreen(Database::TDBTransaction &DBTransaction, AnsiString title);
    void DeleteEmptyTabs(Database::TDBTransaction &DBTransaction);
    void ShowProcessingBar();
    void SendClippTabDetails(int clippTabKey);
    void CloseClippTab(int clippTabKey);
    void SendTabDetails(AnsiString source, AnsiString dest, int sourceItemCount, int destItemCount);


    unsigned __int32 getMaxSeatCount();
    int GetSeatNumber(Database::TDBTransaction &DBTransaction, long source_key, bool isTabSelected, int table_no);
    bool CheckNameIsNullOrNot(Database::TDBTransaction &DBTransaction, int tableNumber, int key, int seatkey); //
    int Gettabkey(int table_no, int seat_key);
    UnicodeString TabName(int TabKey);


    AnsiString GetItemQtyWithName(double qty, AnsiString itemSize, UnicodeString itemName);
    bool CheckSideIsSelected(TListBox *listBox, int i);
    int GetOrderKey(Database::TDBTransaction &DBTransaction, int tab_key, UnicodeString itemName);
    int GetOrderKeyFromSavedList(Database::TDBTransaction &DBTransaction, int transfer_qty, TListBox *listBox, int i, bool isLongPressed = false);
    __int64 SplitOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey, int WaysToSplit);
    bool DeductOrder(TItemMinorComplete * const Order1, TItemMinorComplete * const Order2, int cnt);
    UnicodeString GetItemName(TListBox *listBox, int i, bool isTabSelected);
    double GetItemQty(TListBox *listBox, int i, bool isTabSelected);


    TMMContactInfo TempUserInfo;
    bool IsSourceDestinationSame();
    bool IsPartialTransfer;
    TMMContactInfo DestTempUserInfo;
    TMMContactInfo SourceTempUserInfo;
    bool CheckSecurityEvent(Database::TDBTransaction &DBTransaction, int sec_ref);
    bool IsSCDAppliedOnDest(TList* Orders);
    bool IsPWDAppliedOnDest(TList* Orders);

   ///clips functions
    void SaveClipItemsInStructure();
    void DoCliptabLinking(Database::TDBTransaction &DBTransaction,int sourceKey,int destKey);
    void UpdateTabNameAndKey(Database::TDBTransaction &DBTransaction, int source_key, int dest_key);
    bool CheckLinkedTabValidations(Database::TDBTransaction &DBTransaction,bool checkClipTabLimit);
    bool CheckIfTransferringLinkedTabItems(Database::TDBTransaction &DBTransaction,int sourcekey,bool isTabSelected);
    void DilinkingClipTab(Database::TDBTransaction &DBTransaction,long sourceTabKey,long destTabKey);
    bool CheckItemsInSeat(Database::TDBTransaction &DBTransaction, long tableNo,long seatNo);
    void UpdateGuestNameByClippName(Database::TDBTransaction &DBTransaction, long DestTabKey, long SourceKey, bool isTabSelected);
    bool CheckPartialTransferInClipCase(Database::TDBTransaction &DBTransaction);
    void UpdatePanelSourceKey(int PreviousValue , int NewValue);
    void ChangeItemTabKey(TListBox *listBox, int pos ,int value);
    void SaveClipItemsInStructure(Database::TDBTransaction &DBTransaction);
    void CheckClipAndTableCondition(Database::TDBTransaction &DBTransaction);

    bool CheckIfClipTransferringToAnotherLinkedGuest(Database::TDBTransaction &DBTransaction,int source_key, int DestTabKey, bool isTabSelected );
    void SetGuestNameForTable(Database::TDBTransaction &DBTransaction, long DestTabKey, long SourceKey, UnicodeString tabName);
    void SetPartyNameForDetinationTable(Database::TDBTransaction &DBTransaction, UnicodeString partyname);
    void SetPartyNameForSourceTable(Database::TDBTransaction &DBTransaction, UnicodeString partyname);
    bool CheckToOverwriteSourceStatus(Database::TDBTransaction &DBTransaction,bool checkSourceTable);
    void PrintTransferChefNotification(Database::TDBTransaction &DBTransaction ,bool IsPartialTransferForTable = false);

public:		// User declarations
    __fastcall TfrmTransfer(TComponent* Owner,Database::TDBControl &inDBControl);
	int CurrentSourceTable,CurrentDestTable, CurrentSourceRoom,CurrentDestRoom;
	TMMTabType CurrentSourceTabType,CurrentDestTabType;
	TMMDisplayMode CurrentSourceDisplayMode,CurrentDestDisplayMode;
	TMMContactInfo TempSourceUserInfo;
	TMMContactInfo TempDestUserInfo;
    __int64 PreSelectedTab;
    int NewInvoiceKey;
    TDBTransaction* DBTransaction;

    std::vector<TTabDetails> Tabs;

    int SelectedTabKey;
    int InvoiceKey;
    int InvoiceTabKey;

    std::vector<TTransferChefMateItem> ChefMateOrderList;
    bool isTabOrItemSelected;
    bool isTotalTableTransfered;
    std::map<int, TTransferChefMateItem> CHEFMATE_ITEM_GROUP_MAP;
    int row_count;
    std::vector<TTabColour> CollectItemDetails;
    std::vector<int> GetSavedOrderKey;
    bool IsBalAvailable;
    bool SourceAllowStaffTab;
    bool DestAllowStaffTab;
    bool IsPartialTransferDone;
   //clip objects
    bool ClipTabSelected;
    std::map<int ,double > ClipItemsContainer;
    bool ClipPresentInFromPanel;
    bool ClipPresentInToPanel;
    bool  linkCliptabToTable;
    int  NewClipTabKey;
    bool CloseClipTab;
    bool ItemTransferredFromClip;
    bool isClipLongPress;
    bool IsTableTransferfrom;
    bool IsTableTransferTO;
    std::map<UnicodeString,UnicodeString> Partialtransfer;

};
//---------------------------------------------------------------------------
#endif


