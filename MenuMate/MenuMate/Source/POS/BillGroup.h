//---------------------------------------------------------------------------
#ifndef BillGroupH
#define BillGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchBtn.h"
#include "touchbtn.h"
#include "touchcontrols.h"
#include "touchgrid.h"
#include <algorithm>
#include <set>
#include <iostream>
#include "enum.h"
#include "SelectTable.h"
#include "MM_DBCore.h"
#include "Membership.h"
#include "PnMOrder.h"
#include "SystemEvents.h"
#include "Modules.h"
#include "ZForm.h"
#include "PaymentTransaction.h"
#include "PaySubsUtility.h"

#include "ChefmateClientManager.h"
//---------------------------------------------------------------------------

class TfrmBillGroup : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlButtons;
	TIBQuery *IBQuery;
	TStaticText *lbePartyName;
	TPanel *Panel3;
	TPanel *Panel6;
	TTouchBtn *tbtnReprintReceipts;
	TTouchBtn *btnBillTable;
	TTouchBtn *btnBillSelected;
	TTouchBtn *btnPartialPayment;
	TTouchBtn *btnSplitPayment;
	TTouchBtn *btnTransfer;
	TTouchBtn *btnApplyMembership;
	TTouchBtn *btnClose;
	TTouchBtn *tbtnDiscount;
	TPanel *Panel7;
	TPanel *Panel9;
	TLabel *lbeMembership;
	TPanel *Panel2;
	TTouchBtn *btnScrollBillUp;
	TTouchBtn *btnScrollBillDown;
	TMemo *Memo1;
	TPanel *Panel1;
	TPanel *Panel4;
	TPanel *pnlgridItemListBoarder;
	TTouchGrid *tgridItemList;
	TPanel *pnlgridContainerListBoarder;
	TTouchGrid *tgridContainerList;
	TPanel *Panel5;
	TTouchBtn *tbtnSelectAll;
	TTouchBtn *tbtnClearAll;
	TPanel *Panel8;
	TTouchBtn *tbtnCancel;
	TTouchBtn *tbtnSplit;
	TTouchBtn *tbtnMove;
	TTouchBtn *tbtnSelectZone;
    TTimer *SplitTimer;
    TTouchBtn *tbtnToggleGST;

    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tbtnReprintReceiptsMouseClick(TObject *Sender);
    void __fastcall tgridContainerListMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
    void __fastcall tbtnShowItemsMouseClick(TObject *Sender);
    void __fastcall tgridItemListMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
    void __fastcall tbtnDiscountMouseClick(TObject *Sender);
    void __fastcall btnBillTableMouseClick(TObject *Sender);
    void __fastcall btnBillSelectedMouseClick(TObject *Sender);
    void __fastcall btnPartialPaymentMouseClick(TObject *Sender);
    void __fastcall btnSplitPaymentMouseClick(TObject *Sender);
    void __fastcall btnTransferMouseClick(TObject *Sender);
    void __fastcall btnApplyMembershipMouseClick(TObject *Sender);
    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall tbtnClearAllMouseClick(TObject *Sender);
    void __fastcall tbtnSelectAllMouseClick(TObject *Sender);
    void __fastcall btnScrollBillUpAutoRepeat(TObject *Sender);
    void __fastcall btnScrollBillDownAutoRepeat(TObject *Sender);
    void __fastcall tbtnMoveMouseClick(TObject *Sender);
    void __fastcall tbtnSplitMouseClick(TObject *Sender);
    void __fastcall tbtnCancelMouseClick(TObject *Sender);
    void __fastcall tbtnSelectZoneMouseClick(TObject *Sender);
    void __fastcall ProcessBillThorVouchers(Database::TDBTransaction &DBTransaction);
    void __fastcall SplitTimerTick(TObject *Sender);
    void __fastcall tgridItemListMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton, int X, int Y);
    void __fastcall tgridItemListMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton, int X, int Y);
    void __fastcall tbtnToggleGSTMouseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	void __fastcall SelectZone(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange);
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	  MESSAGE_HANDLER(UWM_SHOWZONESELECT, Messages::TMessage, SelectZone);
	END_MESSAGE_MAP(TComponent)

private:	// User declarations
    __int64 SelectedItemKey;
    void SplitItem(Database::TDBTransaction &transaction, int orderKey,double quantityToSplit);
    void __fastcall CloseTerminateCallBack(TObject* sender);
	void UpdateTableDetails(Database::TDBTransaction &DBTransaction);
	void UpdateSeatDetails(Database::TDBTransaction &DBTransaction,TMembership *Membership);
	void ShowReceipt();
    TDiscount SelectedDiscount;
	bool InCheckFunc;
	int BillItems(Database::TDBTransaction &DBTransaction,const std::set<__int64> &ItemsToBill,TPaymentTransactionType TransType);
	Database::TDBControl &DBControl;
	bool Scaled;
	eDisplayMode SelectedZone();
    void RemoveMembership(Database::TDBTransaction &DBTransaction);
    void RemoveMembershipDiscounts(Database::TDBTransaction &DBTransaction);
   	void LoadCustNameAndOrderType(TPaymentTransaction &PaymentTransaction);
   	void _onItemsCanceled( TList* inList,AnsiString inTabTableName );
    void applyWaiterStationSettingsIfEnabled();
    void ClearTabOnDelayedPaymentSelection(Database::TDBTransaction &DBTransaction,int inSelectedTab);
    void UpdateBilledPatronCount(Database::TDBTransaction &DBTransaction,int tableNo);
    bool discountFormProcessed;
    void RemoveThorMembership();
    std::pair<int, int> CalculatePatronCount();
    int DeterminePatronCount();
    std::auto_ptr<TChefmateClientManager> cmClientManager;
    void _displayLastReceipt( Database::TDBTransaction &DBTransaction, TReqPrintJob *LastReceipt );
    bool CheckDiscountPointsBillGroup; // MM-3908
    void _displayWebOrderDetailsReceipt( Database::TDBTransaction &DBTransaction, std::set<__int64> WebOrderKeys);
    void _getWebOrderMemberDetails(Database::TDBTransaction &DBTransaction, TPaymentTransaction &PaymentTransaction, TStringList *webDeliverydetails, TStringList *webdetails, int web_key, TDateTime delivery_time);
    bool IsClipTabPresent;
    void DisableButtons();
    void EnableButtons();
    bool ClipOptionSelected;
    bool ClipTabInTable;
    bool DoSplit(Database::TDBTransaction &transaction,std::map<__int64, TPnMOrder> orders);
    void SplitItemOnClick(int itemSelected);
    void RefreshItemStatus(Currency splitValue,int itemSelected,Database::TDBTransaction &DBTransaction);
    void ToggleItemState(TGridButton *GridButton);
    void SelectItem(TGridButton *GridButton);
    void DeselectItem(TGridButton *GridButton);
    void GetMemberByBarcode(Database::TDBTransaction &DBTransaction,AnsiString Barcode);
    void GetLoyaltyMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
    Currency GetAvailableRedeemPoints(TMMContactInfo &Member);
    void DisableBillEntireTable(Database::TDBTransaction &DBTransaction);
    void ChangeBillEntireTableState();
    void DisableToggleGSTButton(Database::TDBTransaction &DBTransaction);
    void RemoveLoyaltymateMembership(std::set <__int64> SelectedItemKeys);
    AnsiString VoucherCode;
    bool CanMoveItemToOtherTab(int sourceAccNumber, int TabTransferTo);
    bool CheckBillEntireWithCustomerJourney();
    void CustomizeForSiHot(TPaymentTransaction &PaymentTransaction);
public:		// User declarations
	__fastcall TfrmBillGroup(TComponent* Owner,Database::TDBControl &inDBControl);
	void SplitItemsInSet(Database::TDBTransaction &, int);
	void __fastcall UpdateRightButtonDisplay(TObject *Sender);
	int CurrentTable, CurrentRoom,CurrentMember,CurrentInvoiceKey;
    TMMContactInfo TempUserInfo;
    TMMTabType CurrentTabType;
	TMMDisplayMode CurrentDisplayMode;
	std::set<__int64> SelectedTabs;
	std::map<__int64,TPnMOrder> VisibleItems;
	std::map<__int64,TPnMOrder> SelectedItems;
	TStringList* TabList;
    std::vector<TPatronType> PatronTypes;
    void CheckLoyalty();
    void CheckLoyalty(std::set <__int64> ReceiptItemKeys);
    bool IsWaiterLogged;
    bool MembershipConfirmed; // Membership has been confimed by user.
    TContactMemberApplied Membership;
	int CurrentSelectedTab;
	int PatronCount;
	void UpdateSplitButtonState();
	bool IsSelectionComposedOnlyOfSides() const;
	bool IgnoreItemThreshhold;
	void TabStateChanged(Database::TDBTransaction &DBTransaction,TMembership *Membership);
	void OnTabChange(TSystemEvents *Sender);
	bool TabInUseOk(int inTabKey);
	bool TabsInUseOk(Database::TDBTransaction &DBTransaction,std::set<__int64> TabsToLock);
	bool TabStaffAccessOk(Database::TDBTransaction &DBTransaction);
    bool TabPINOk(Database::TDBTransaction &DBTransaction,int inTabKey);
	void UpdateItemListDisplay(Database::TDBTransaction &DBTransaction);
	void ResetForm();
	void ResetSelection();
	void ItemSetRemoveItems(Database::TDBTransaction &DBTransaction,int TabKey);
	void ItemSetAddItems(Database::TDBTransaction &DBTransaction,int TabKey);
	void TransferItem(Database::TDBTransaction &DBTransaction,int ItemKey);
	void CancelItems(Database::TDBTransaction &DBTransaction,std::set<__int64> ItemsToBeCanceled,TMMContactInfo &CancelUserInfo);
	void UpdateContainerListColourDisplay();
	void UpdateItemListColourDisplay();
	void __fastcall ApplyDiscount(Database::TDBTransaction &DBTransaction,int DiscountKey,TDiscountSource DiscountSource = dsMMUser);
    void __fastcall ApplyThorDiscount(Database::TDBTransaction &DBTransaction,int DiscountKey,TDiscountSource DiscountSource = dsMMUser);
	bool AddToSelectedTabs(Database::TDBTransaction &DBTransaction,long TabKey);
	void ApplyMembership(Database::TDBTransaction &DBTransaction,TMMContactInfo &MembershipInfo);
   void OnSmartCardInserted(TSystemEvents *Sender);
   void OnSmartCardRemoved(TSystemEvents *Sender);
   void RemoveMembershipFreeItems(Database::TDBTransaction &DBTransaction);
   TDateTime delivery_time;
   TChitNumber WebOrderChitNumber;
   TMMContactInfo MemberInfo;
   void UpdateBillGroupBtnVisibilityInClipTabCase(Database::TDBTransaction &DBTransaction,std::map<__int64,TPnMOrder> &SelectedItems);
   void CheckingClipItemsInSelectedList(Database::TDBTransaction &DBTransaction);
   int CheckIfListContainOnlyClipItems(Database::TDBTransaction &DBTransaction,std::map<__int64,TPnMOrder> &SelectedItems);
   void CheckLinkedTable(int TableNumber);
   int PatronCountForMallExport;
private:
    void SendPointValueToRunRate( TPaymentTransaction &inTransaction );
    int extractPatronCountForMallExport(TPaymentTransaction &paymentTransaction);
    void UpdateContainerList();
    void ClearLoyaltyVoucher();

};
#endif

