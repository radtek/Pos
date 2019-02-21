// ---------------------------------------------------------------------------
#ifndef SelectDishH
#define SelectDishH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchNumpad.h"
#include "TouchPages.h"
#include "ZForm.h"
#include "MM_DBCore.h"
#include "SystemEvents.h"
#include "SHDocVw_OCX.h"
#include <SHDocVw.hpp>
#include "SendForm.h"
#include "ConfirmOrd.h"
#include "Login.h"
#include "Discount.h"
#include "PatronType.h"
#include "Money.h"
#include "ItemRedirector.h"
#include "ItemComplete.h"
#include "WebMate.h"
#include "MMContactInfo.h"
#include "MMProxy.h"
#include "TcpStreamSender.h"
#include "TCPStreamPrepare.h"
#include "PatronCount.h"
#include "RunRateClient.h"
#include "Tables_ChitNumbers.h"
#include "DBHeldOrder.h"
#include "DBActiveChit.h"
#include "ProcessWebOrder.h"
#include "PaymentTransaction.h"
#include "ContactMemberApplied.h"
#include "MMTablePickerConnectorServer.h"
#include "ChefmateClientManager.h"
#include "ManagerChitNumber.h"
#include "ManagerParkedSales.h"
#include "ChitNumberController.h"
#include "DrinkCommandManager.h"
#include "MMCustomerDisplayManager.h"
#include <item_management_network_messaging_fwd.hh>
#include <vector>
#include <map>
#include <memory>
// ---------------------------------------------------------------------------
#ifndef OrderOnHold
#define OrderOnHold (OrderHeld && TGlobalSettings::Instance().EnableHoldSend)
#endif
// ---------------------------------------------------------------------------
class TDataBtn;
class TItem;
class TSeatOrders;
class TItemRecipe;
class TContainerOrders;
class TItemsCompleteCompressed;
class TItemRedirector;

class TMMCustomerDisplayManager;

enum eBtnToDisplay
{
   eBTDOverView, eBTDQty, eBTDOptionsPlus, eBTDOptionsMinus, eBTDSetMenu, eBTDSides, eBTDPrice, eBTDMembership, eBTDMemberPurchases,
   eBTDMemberFavourites, eBTDMemberPoints , eBTDDiscountDetails,eBTDRemove ,eBTDThorVouchers, eBTDChangeBarcode,eBTDNone,
};

typedef Set <eBtnToDisplay, eBTDOverView, eBTDNone> TBtnToDisplay;

struct TDiscountDetails
{
  public:
      TDiscount Discount;
      TList *OrderList;
      TDateTime DiscountTime;
};

struct TMembershipDiscountList
{
  public:
      TDiscount Discount;
      int DiscountKey;
      bool IsApplied;
};

class TfrmSelectDish : public TZForm
{
   friend TZForm;

	void __fastcall SyncGridFontSizes();

__published: // IDE-managed Components
   TPanel *Panel1;
   TTimer *tmPosRefresh;
   TTimer *tiClock;
   TTimer *tiChitDelay;
   TTimer *btnTimer;
   TTimer *ListTimer;
   TTouchBtn *stHappyHour;
   TStaticText *stTime;
   TTimer *TimerTender;
   TPanel *pnlItemModify;
   TTouchBtn *tbtnToggleMenus;
   TPanel *pnlOrdering;
   TTouchGrid *tgridOrderCourse;
   TPanel *Panel5;
   TTouchGrid *tgridOrderItem;
   TTouchGrid *tgridServingCourse;
   TTouchBtn *tbtnUserName;
   TTouchBtn *tbtnChitNumber;
   TTouchBtn *tbtnFunctions;
   TTouchBtn *tbtnParkSales;
   TTouchBtn *tbtnReprintReceipts;
   TTouchBtn *tbtnOpenDrawer;
   TTouchBtn *tbtnSystem;
   TTouchBtn *tbtnSelectTable;
   TTouchBtn *tbtnSave;
   TTouchBtn *tbtnChangeTable;
   TTouchBtn *tbtnTender;
   TTouchBtn *tbtnCashSale;
   TTouchBtn *tbtnDollar1;
   TTouchBtn *tbtnDollar2;
   TTouchBtn *tbtnDollar3;
   TTouchBtn *tbtnDollar4;
   TTouchBtn *tbtnDollar5;
   TTouchGrid *tgridSeats;
   TPanel *pnlItemPickerBorder;
   TLabel *lbChange;
   TLabel *lbeTotal;
   TListBox *lbDisplay;
   TTouchBtn *btnUp;
   TTouchBtn *btnDown;
   TStaticText *lbeChange;
   TStaticText *lbeTotalCost;
   TTouchBtn *btnRemove;
   TTouchBtn *tbtnMembership;
   TPanel *pnlOptions;
   TTouchBtn *btnOk;
   TTouchGrid *btngridModify;
   TTouchBtn *tbtnMemberDisplayPageUp;
   TTouchBtn *tbtnMemberDisplayPageDown;
   TTouchPages *pcItemModify;
   TTouchSheet *tsOverview;
   TPanel *Panel12;
   TPanel *Panel7;
   TLabel *Label2;
   TMemo *memNote;
   TTouchGrid *tgridItemServingCourse;
   TPanel *Panel4;
   TLabel *Label3;
   TMemo *memOverview;
   TPanel *Panel6;
   TTouchSheet *tsQuantity;
   TPanel *pnlQuantity;
   TTouchNumpad *tnpQuantity;
   TNumpadDisplay *QtyDisplay;
   TTouchSheet *tsWebPage;
   TPanel *Panel9;
   TTouchSheet *tsSetMenu;
   TPanel *Panel3;
   TTouchGrid *tgridItemSetMenuItems;
   TTouchSheet *tsServingCourse;
   TPanel *Panel17;
   TTouchSheet *tsSides;
   TPanel *Panel14;
   TTouchGrid *tgridItemSideCourses;
   TTouchGrid *tgridItemSideItems;
   TTouchSheet *tsOptions;
   TPanel *pnlItemOptions;
   TTouchGrid *tgridItemOptions;
   TPanel *Panel18;
   TWebBrowser *webStockLevels;
   TWebBrowser *webDisplay;
   TTouchBtn *tbtnWebOrders;
   TTouchBtn *tbtnSearch;
   TEdit *tedtSearchItem;
   TTimer *tiPMSRoom;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall stCloseClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall tbtnCashSaleClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall tmPosRefreshTimer(TObject *Sender);
   void __fastcall tiClockTimer(TObject *Sender);
   void __fastcall lbDisplayDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
   void __fastcall tbtnTenderClick(TObject *Sender);
   void __fastcall FormHide(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall btnUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnTimerTimer(TObject *Sender);
   void __fastcall btnDollarMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnDollarMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall ListTimerTimer(TObject *Sender);
   void __fastcall lbDisplayMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall lbDisplayMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall lbDisplayClick(TObject *Sender);
   void __fastcall tbtnChangeTableClick(TObject *Sender);
   void __fastcall TimerTenderTimer(TObject *Sender);
   void __fastcall tbtnRunProgramClick();
   void __fastcall tbtnCallAwayClick();
   void __fastcall tbtnDiscountClick(bool combo = false);
   void __fastcall tbtnLuckyMemberClick();
   void __fastcall tbtnFlashReportsClick();
   void __fastcall btngridModifyMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall btnRemoveMouseClick(TObject *Sender);
   void __fastcall tgridItemServingCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgridItemSideCoursesMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgridItemSideItemsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tbtnToggleMenusMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnToggleMenusMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tgridItemSetMenuItemsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall btnOkMouseClick(TObject *Sender);
   void __fastcall tgridItemOptionsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgridOrderCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgridOrderItemMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgridServingCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tbtnUserNameMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnChitNumberMouseClick(TObject *Sender);
   void __fastcall tbtnFunctionsMouseClick(TObject *Sender);
   void __fastcall tbtnParkSalesMouseClick(TObject *Sender);
   void __fastcall tbtnReprintReceiptsMouseClick(TObject *Sender);
   void __fastcall tbtnOpenDrawerMouseClick(TObject *Sender);
   void __fastcall tbtnSystemMouseClick(TObject *Sender);
   void __fastcall tbtnMembershipMouseClick(TObject *Sender);
   void __fastcall tbtnSaveMouseClick(TObject *Sender);
   void __fastcall tgridSeatsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tbtnSelectTableMouseClick(TObject *Sender);
   void __fastcall memNoteClick(TObject *Sender);
   void __fastcall tbtnMemberDisplayPageUpMouseClick(TObject *Sender);
   void __fastcall tbtnMemberDisplayPageDownMouseClick(TObject *Sender);
   void __fastcall tnpQuantityClick(TObject *Sender, TNumpadKey Key);
   void __fastcall webDisplayBeforeNavigate2(TObject *ASender, const IDispatch *pDisp, OleVariant &URL, OleVariant &Flags,
	  OleVariant &TargetFrameName, OleVariant &PostData, OleVariant &Headers, WordBool &Cancel);
    void __fastcall tbtnWebOrdersMouseClick(TObject *Sender);
    void __fastcall tiChitDelayComplete(TObject *);
   void __fastcall tedtSearchItemChange(TObject *Sender);
   void __fastcall tedtSearchItemKeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall tbtnSearchMouseClick(TObject *Sender);
   void __fastcall tiPMSRoomInputTimer(TObject *Sender);
private: // User declarations
    AnsiString SouceTableForDelayedPayment;
    AnsiString CustName;
    AnsiString CustPhone;
    std::auto_ptr<TLabel> taxLabel;
    std::auto_ptr<TLabel> taxFigure;
    std::auto_ptr<TLabel> serviceChargeLabel;
    std::auto_ptr<TLabel> serviceChargeFigure;
    std::auto_ptr<TStringList> logList;
    bool isExtendedDisplayActive;
    int tbtnDollar1Width;
    int tbtnDollar2Width;
    int tbtnDollar3Width;
    int tbtnDollar4Width;
    int tbtnDollar5Width;
    int tbtnDollar1Left;
    int tbtnDollar2Left;
    int tbtnDollar3Left;
    int tbtnDollar4Left;
    int tbtnDollar5Left;
    bool isVoucherApplied;
    int priceValue;
    AnsiString priceValueS;
    AnsiString memberDetails;
    AnsiString memberName;
    AnsiString memberNo;
    TNotifyEvent OnCloseThreadTerminate;
    void AdjustScreenSize();
    TSaleType TypeOfSale;
    eLocation fLocation;
    TCustomerOrder CustomerOrder;
    long LastEnabledStateSync;
    unsigned char LastMenuSync1, LastMenuSync2;
    int LastTableNo, LastSeatNumber;
    TTouchBtn *LastCourseBtn, *LastClassBtn;
    TDateTime LastSale;
    int CurrentTimeKey;
    bool WaitingForSwipe;
    AnsiString SwipeString;
    bool Swiping;
    TDataBtn *CurrentButton;
    bool Always_Prompt;
    int SelectedTable;
    AnsiString SelectedTabContainerName;
    int SelectedSeat;
    AnsiString SelectedParty;
    TServingCourse CurrentServingCourse;
    TChitNumber ChitNumber;
    TContactMemberApplied Membership;
    unsigned long SetMenuMask;
    int SetMenuGroupNo;
    UnicodeString SelectedOrderMenuName;
    std::vector <TSeatOrders*> SeatOrders;
    Currency CurrentTender; // From pressing $5 - $100 buttons
    TTouchBtn *CurrentTenderButton;
    TMoney InitialMoney;
    bool ignore_preloaded_card;
    bool OrderHeld;
    bool IsPricedBarcode(AnsiString &data) const;
    bool IsWeighedBarcode(AnsiString &data) const;
    std::auto_ptr<TChefmateClientManager> cmClientManager;
    typedef std::vector<TCustomerDisp> TCustomerDisps;
    TCustomerDisp customerDisp;
    TStringList* removedTaxList;
    int sec_ref;
    AnsiString CustAddress;
    bool itemSearch;
    bool isWalkInUser;
    int selectedRoomNumber;
    UnicodeString selectedRoomNumberStr;
    TSiHotAccounts SiHotAccount;
    bool isRoomNoUiCalled;
    bool IsAutoLogOutInSelectDish;

    void YesGoForSessionWithDC(int memPoints, AnsiString memberPoints,AnsiString memNo,int contactKey);
    void StartThreadDC(int _memPoints, char* _memberDetails_char , AnsiString _memberPoints , AnsiString _memberDetails);
    void _fastcall CloseTerminateCallBack(TObject* sender);
    void __fastcall OnDCThreadTerminate(TObject* sender);
    bool CheckMenuExistsOrNot(UnicodeString default_menu_name_);
    void RedrawSetMenuItems();
    void AutoLogOut();
    void LockOutUser();
    double CountInstances(TItemComplete &archetype);
    void CheckAndSetItemQuantity();
    void MergeIdenticalOrders(TItemComplete &selected_item);
    void SearchItems();
    __fastcall TfrmSelectDish(TComponent* Owner);
    void __fastcall ForceHappyHourClick();
    TColor GetItemButtonFontColour(TColor inColor);
    void __fastcall HighlightSelectedItem();
    void __fastcall SetLocation(eLocation value);
    bool ApplyDiscount(Database::TDBTransaction &DBTransaction, int DiscountKey, TList *Orders,bool isInitiallyApplied = true,TDiscountSource DiscountSource = dsMMUser);
    bool checkQuantityAndSpliitItemsForCombo(TList *Orders, Database::TDBTransaction &DBTransaction);
    void RemoveMembershipDiscounts();
    void ProcessQuickPayment(TObject *Sender,AnsiString paymentName);
    bool IsProcessQuickPayment(TObject *Sender);
    AnsiString GetQuickPaymentName(TObject *Sender);
    void AddItemToSeat(Database::TDBTransaction& inDBTransaction,
                       TItem* inItem,
                       bool  inSetMenuItem,
                       TItemSize* inItemSize = NULL,
                       Currency  inPrice = 0,  bool IsItemSearchedOrScan = false);
    TItemComplete* createItemComplete(Database::TDBTransaction& DBTransaction,
                                      TItem* Item,
                                      bool  SetMenuItem,
                                      TItemSize* inItemSize , bool IsItemSeaarchedOrScan);
    bool DisplayHoldSend(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> &Patrons);
    void HoldButtonClick(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> Patrons);
    bool SendButtonClick(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> &Patrons);
    bool SendRelatedOrders(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> &Patrons);
    void RetrieveHeldCustomerName(std::auto_ptr <TList> &OrderObjects);
    void RefreshSeats();
    void SelectNewMenus();
    void SetSelectedSeat(bool selectGuest=false); //changes for 5900
    void RedrawSeatOrders();
    void __fastcall CurrentItemChange(TItemRedirector *);
    void RedrawModifyOptionsBtnGrid(bool Reset = false);
    void SetServingCourseBtnSizes();
    void pcItemModifyDisplayOptions(bool PlusOpt);
    void pcItemModifyDisplayOverview();
    void pcItemModifyDisplayQuantity();
    bool pcItemModifyDisplayPrice();
    bool pcItemModifyDisplaySetMenu();
    void pcItemModifyDisplayMember(eBtnToDisplay ButtonID);
    void pcItemModifyDisplayDiscountDetails();
    TItemMinorComplete * GetSelectedItem(TDisplayItemTypes Filter = TDisplayItemTypes());
    TItemMinor * GetSelectedSetMenuMaster();
    eBtnToDisplay GetModifyOptionSelected();
    void __fastcall CloseSidePanel();
    void __fastcall OpenSidePanel();
    void ApplyMembership(Database::TDBTransaction &DBTransaction, TMMContactInfo & Membership);
    bool StaffChanged(TMMContactInfo TempUserInfo);
    void RemoveMembership(Database::TDBTransaction &DBTransaction);
    bool TabPINOk(Database::TDBTransaction &DBTransaction, int inTabKey);
    void WriteOffBillAsWastage();
    void AlterFloat();
    void EnterPaxCount();
    void Navigate(TWebBrowser *WebBrowser, TStringList *Html);
    void SendOrderListToKitchen(Database::TDBTransaction &DBTransaction, TList *OrdersList, std::vector<TPatronType> Patrons);
    // gets patron count from user
    std::vector<TPatronType> QueryForPatronCount(TPaymentTransaction &PaymentTransaction);
    std::vector<TPatronType> GetPatronCount(Database::TDBTransaction &dbTransaction);
    void ReloadChitNumberStatistics();
    void CheckDeals(Database::TDBTransaction &DBTransaction);
    void MapItemsAndCategories(std::multimap<TItemMinorComplete*, int> &itemCatMap, Currency &totalItemCost);
    Currency GetPriceFromBarcode(AnsiString barcode);
    std::pair<TItem*, TItemSize*> GetLoadedItemFromBarcode(UnicodeString inBarcode);
    // auto ptr to hold process web order form
    std::auto_ptr<TfrmProcessWebOrder> frmProcessWebOrder;
    void CalculateAndDisplayTotalServiceCharge() const;
    void CalculateAndDisplayTotalTax() const;
    void UpdateTaxLabels();
    void PlaceLabels( TLabel &title_label, TLabel &value_label, unsigned short &y_position);
    void CreateTaxLabels();
    void SetupTaxLabels();
    unsigned short VertSpaceRequiredForLabels() const;
    unsigned short LabelsToShow() const;
    unsigned short VertSpaceSeparatingTotalAndChange() const;
    unsigned short VertSpaceSeparatingTitleAndValueLabels() const;
    void MakeRoomForLabels(unsigned short space_required);
    void InitializeLabel(TLabel &label, bool displays_value = false);
    void InitializeQuickPaymentOptions();
    void InitializeTablePatrons();
    void NagUserToSelectChit();
    int GetCount(std::vector<TPatronType> patronTypes);
    // MM-2063 : Creating tabs to auto save
    TModalResult setupAutoSaveTabContainer(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer);
    void ChangeCard();
    bool CheckDiscountPoints; // MM-3908
    void OpenTransactionAuditScreen();// Security Screen
    void CaptureDeliveryTime();
    int GetUpdatedPatronCount(Database::TDBTransaction &DBTransaction,int TableNumber,std::set<int> SeatCounter);
    void GetMemberByBarcode(Database::TDBTransaction &DBTransaction,AnsiString Barcode);
    void GetLoyaltyMember(Database::TDBTransaction &DBTransaction,TMMContactInfo & Info);
    void AssignBarcodeToMember();
    void ManageDiscounts();
    void HideSoldItems(Database::TDBTransaction &DBTransaction,TList *OrdersList);
    void ChangeMenuToChitDefault();
    ChitResult SetupChit(Database::TDBTransaction &tr);
    ChitResult InitializeChit();
    void ResetChit();
//    TPaymentTransaction PaymentTransaction(DBTransaction);
    void ApplyMemberDiscounts(Database::TDBTransaction &DBTransaction,bool isInitiallyApplied = true);
    void GetAllOrders(TList* outOrders);
    void AssignremovedTaxesList();
    void CancelItemsBill(TItemComplete *cancelitem, AnsiString& User, bool isFullItemRemove=false);//void CancelItemsBill(TItemComplete *item);
    bool CheckItemsPrintCancel();
    void SaveRemoveItems(AnsiString& User);
    bool ChangeItemQty;
    bool SaveTransactionDetails(UnicodeString &pay_type);
    void AssignDiscountLists();
    void ClearDiscountLists(std::vector<TDiscount> DiscountList);
    void GetChitDiscountList(Database::TDBTransaction &DBTransaction, std::vector<TDiscount> DiscountList);
    void RemoveChitDiscountList();
    void RemoveDiscountList(TStringList *List);
    bool CheckPromptForChit;
    void CheckMandatoryMembershipCardSetting(TObject * Sender);
    void GetItemsFromTable(int seatkey, TGridButton *GridButton, bool isCalledFromGuestSeat = false);
    //change here..
    UnicodeString SetPartyNameOnChitSettings(Database::TDBTransaction &DBTransaction, AnsiString PartyName, int tableNo, bool setPartyName = false);
    UnicodeString TfrmSelectDish::PrepareAddress(UnicodeString str_name);
    Currency  VerifyPriceLevelPrice(Database::TDBTransaction &DBTransaction,int itemsizeKey,Currency Price);
    void CheckLastAddedItem();
    bool NotifyLastWebOrder(Database::TDBTransaction &DBTransaction);
    bool GetValue(Database::TDBTransaction &DBTransaction);
    void UpdateValue();
    bool CheckCreditLimitExceeds(Database::TDBTransaction &dBTransaction,int tabKey);
    bool CheckTabIsInUse(Database::TDBTransaction &dBTransaction, int tabKey);
    bool ShowTabLockedMessage(AnsiString creditMessage);
    bool CheckCreditLimitForTable(Database::TDBTransaction &dBTransaction,int tabKey, Currency OrdersTotal);
    bool ShowTabCreditLimitExceedsMessage(Database::TDBTransaction &dBTransaction, Currency CreditLimit);
    void SendPointValueToRunRate( TPaymentTransaction &inTransaction );
    void DoCloundSync();
    void AddSearchedItemToSeat();
    void  OrderSearchedItem(std::pair<TItem*, TItemSize*> &itemAndSize);
    void SetPOSBackgroundColor();
	bool PromptForDiscountDescription(TDiscount &currentDiscount);
    bool PromptForDiscountAmount(TDiscount &currentDiscount);
    bool ApplyDiscount(Database::TDBTransaction &DBTransaction, TDiscount &CurrentDiscount, TList *Orders, bool isInitiallyApplied = true, TDiscountSource DiscountSource = dsMMUser);
    bool isChitDiscountExist;
    void RemoveChitDiscounts(TMMContactInfo Member);
    void CheckGiftCardBalance();
    bool CheckItemCanBeAddedToSeat(TItem *item);
    void LoadFoodAndBevList(TList *foodOrdersList, TList *bevOrdersList);
    bool CheckIfSubsidizedDiscountValid(int tabKey);
    void DisplayRoomNoUI();
    void GetRoomDetails();
    bool LoadRoomDetailsToPaymentTransaction(TPaymentTransaction &inTransaction);
    bool CloseActiveForm();
    std::vector<UnicodeString> LoadGuestDetails(UnicodeString defaultTransaction);
    std::vector<TPatronType> patronsStore;
    int storedPatronCountFromMenu;
    void StorePatronsInformation(TPaymentTransaction &PaymentTransaction);
    void InitializePatronForQuickSale(TPaymentTransaction &PaymentTransaction);
    void ExtractPatronInformation(TPaymentTransaction &PaymentTransaction);
    //void RestructureBillForPatrons(TPaymentTransaction &PaymentTransaction);
    void RestructureBillForSplit();
    void ApplyDiscountWithRestructure(TList *Orders, TDiscount CurrentDiscount);
    void MakeDummyPaymentTransaction(TList *Orders, TPaymentTransaction &paymentTransaction);
    void ExtractFromDummyPaymentTransaction(TPaymentTransaction &paymentTransaction, TList *Orders);
    bool ArePatronsChanged(std::vector<TPatronType> patronsOld,std::vector<TPatronType> patronsNew);
    void RestructureBillForPatrons(std::vector<TPatronType> patrons);
    bool LoadPMSGuestDetails(TPaymentTransaction &PaymentTransaction);
    void LoadDefaultGuestDetailsToSeatOrders(UnicodeString roomNo, UnicodeString firstName, UnicodeString accNo,UnicodeString lastName = "");
    void GetNextAvailableSeatAndLoadOrders(bool isCalledFromGuestSeat);
    void SyncWithCloud();
    //void SyncMenuAndTaxSettings(int syncType);
    //TSiteMenuInfo GetSiteMenus(Database::TDBTransaction &DBTransaction);
    TLoginSuccess GetStaffLoginAccess(Database::TDBTransaction &DBTransaction, int access);
    void  ShowErrorMessage(std::string message, TLoginSuccess Result);
    bool ShowMemberValidationMessage(int selectedTable);
    void CheckRegisteration();
    void SendFiscalPrint(TPaymentTransaction &paymentTransactionNew);
    void ClearCurrentTransactionDetails(TPaymentTransaction &paymentTransactionNew);
    void ClearSeatOrdersWithFiscalPrint();
    void RecordFiscalLogsSelectDish(TStringList* logList, AnsiString logValue);

protected:
   void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   void __fastcall CardSwipe(Messages::TMessage& Message);
   void __fastcall WebOrder(Messages::TMessage& Message);

   void __fastcall ItemAlteredRefresh(Messages::TMessage &Message);

   BEGIN_MESSAGE_MAP
   MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange);
   MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
   MESSAGE_HANDLER(WEBORDER_STATUSCHANGED, Messages::TMessage, WebOrder);
   MESSAGE_HANDLER(item_management::network_messaging::refresh_item_display,
				   Messages::TMessage,
				   ItemAlteredRefresh);
   END_MESSAGE_MAP(TForm)
   void InitXeroIntegration();
    TMMTablePickerConnectorServer *tpConnector;
    bool tpConnectorUp;
    bool tpOpen;
    void OpenTPConnector();  // Open ProxyMate's TablePicker connector
    void CloseTPConnector(); // Close ProxyMate's TablePicker connector
    void showTablePicker();
    void showNewTablePicker( TMMTablePickerConnectorServer* inConnector );
    void showOldTablePicker();
    void ChitCaptAndSaveName();
    void __fastcall mmAppConnectorOpenConnectorAck( System::TObject* Sender, int inACK, TMMConnectorTypes inConnectorType, TMMConnectorServer* inConnector  );
    void __fastcall tpClientUp(      TObject* Sender );
    void __fastcall tpClientDown(    TObject* Sender );
    void __fastcall tpSelectedTable( TObject* Sender, TFPTableData* inData );
    void __fastcall refreshSelectedSeat();
    UnicodeString GetTenderStrValue( vmVariables inVariable );
    UnicodeString CheckCurrencyStr( UnicodeString inCurrencyStr );
    Currency      BtnCaptionToCurrency( TTouchBtn* inBtn );
    void    setPatronCount( __int32 inPatronCount );
    __int32 getPatronCount();

   //::::::::::::::::::::::::::
   // CHEFMATE
   //::::::::::::::::::::::::::
    __int32 _patronCount;
    void completeOrderToChefMate(      TPaymentTransaction* inTransaction );
    void completeHeldOrdersToChefMate( TPaymentTransaction* inTransaction );
    void creditOrderToChefMate( const std::vector<TSeatOrders *> &seat_orders );
    void __stdcall callAwayToChefMate( TCallAwayComplete* inCallAway );
   //::::::::::::::::::::::::::
   // Forced Options.
   //::::::::::::::::::::::::::
    void retrieveForcedOptionList(__int32 inIndex,TItem* inItem,
    TListOptionContainer* const outForcedOtionList );
    void purge_unsent_orders();
    unsigned __int32 getMaxTableCount();
    unsigned __int32 getMaxSeatCount();
   //::::::::::::::::::::::::::
   // Customer Display
   //::::::::::::::::::::::::::

   TMMCustomerDisplayManager *customer_display_manager;
                      TTimer *customer_display_timer;
                      TTimer *customer_display_finish_timer;

   Currency total_service_charge;
   Currency total_tax;
   void startCustomerDisplayServer();
   void stopCustomerDisplayServer();
   void customerDisplayResetPOSTotals();
   void customerDisplayReset();
   void customerDisplaySendOrder( std::vector <TSeatOrders*> inSeatOrder );
   void customerDisplayOrderPaid( Currency inLastTotal, Currency inLastChange );
   //:::::::::::::::::::::::::::::::::::::::::::::::
   // Parked Sales
   //:::::::::::::::::::::::::::::::::::::::::::::::

   void setParkedSaleBtnStatus();
   void setParkedSalesBtnColor();
   void setParkedSale( TParkedSale* inSale );

    //:::::::::::::::::::::::::::::::::::::::::::::::
   // Transaction Audit Screen
   //:::::::::::::::::::::::::::::::::::::::::::::::
   void createTransactionAuditScreen();
   void resetTransactionAuditScreen();
   void AddSecurityOnMoveOrderToTab(Database::TDBTransaction &DBTransaction,TList *OrderObjects);
public: // User declarations
   // Was Private ItemMod Changes...
   void YesGoForSessionWithDC();
   void StartProcessForDC(int _memPoints, char* _memberDetails_char , AnsiString _memberPoints , AnsiString _memberDetails);
   bool CallDCFromSelectDish;
   void ResetDCCallFromOutSide();
   void TotalCosts();
   void UpdateExternalDevices(bool UpdateTopPoleDisplay = true, bool UpdateBottomPoleDisplay = true);
   // ------------------

   __property TNotifyEvent OnDCTerminate = {read = OnCloseThreadTerminate, write = OnCloseThreadTerminate};

   std::auto_ptr<TManagerParkedSales> ParkedSales;
   bool DisplayingPlusOptions;
   bool IsOrderinProgress;
   bool IsOrders;
   bool IsOrdersSome;
   int  ClippTabKey;
   typedef std::vector<TPatronType> PATRON_TYPE_VECTOR;
   TMMContactInfo PreUserInfo;
   TSystemEvents BeforeItemOrdered;
   TSystemEvents AfterItemOrdered;
   TSystemEvents BeforeSaleProcessed;
   TSystemEvents AfterSaleProcessed;
   TSystemEvents AfterSelectedItemChanged;
   void OnItemDetailsReceived(TSystemEvents *Sender);
   TItemCompleteSub * AddSubItemToItem(Database::TDBTransaction &DBTransaction, TItem *Item, TItemComplete *MasterOrder);
   void RemoveSideItemFromItem(TItemCompleteSub *side_item, TItemComplete *master_item);
   bool PosRefresh;
   bool IsWaiterLogged;
//   int Thor_Index;
   void RedrawMenu();
   void RedrawCourses();
   void RedrawItems();
   bool OrdersPending();
   bool OrdersParked(Database::TDBTransaction &DBTransaction);
   bool LoyaltyPending();
   bool DeleteAllUnsentAndProceed(Database::TDBTransaction &DBTransaction);
   bool DeleteUnsentAndProceed(Database::TDBTransaction &DBTransaction);
   bool DeleteUnsentDCAndProceed(Database::TDBTransaction &DBTransaction);
   __property eLocation CurrentLocation =
   {
	  read = fLocation, write = SetLocation
   };
   void SetItemNote(AnsiString Note);
   bool ProcessOrders(TPaymentTransaction &PaymentTransaction, TObject *Sender, Database::TDBTransaction &DBTransaction, int SelectedTab, TMMTabType TabType,
	  AnsiString TabContainerName, AnsiString TabName, AnsiString PartyName = "", bool PrintPrelim = false, int TableNo = 0,
	  int SeatNo = 0, int RoomNo = 0, bool BillOff = false, AnsiString DelayedInvoiceNumber = "");

   void UpdateTableButton();
   void RedrawServingCourses();
   void RedrawServingCoursesOverview();
   void GetServingCouseSelected();
   bool CheckAdJustItemQty();
   void AdjustItemQty(Currency Count = 0);
   TModalResult GetTabContainer(Database::TDBTransaction &DBTransaction, TMMTabType TabType, TSaveOrdersTo &OrderContainer);
   TModalResult GetOrderContainer(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer);
   TModalResult GetTableContainer(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer);
   void OnMenuChange(TSystemEvents *Sender);
   void OnTillsMenusAltered(TSystemEvents *Sender);
   void OnBeforeItemOrdered(TSystemEvents *Sender);
   void OnAfterItemOrdered(TSystemEvents *Sender);
   void OnAfterSaleProcessed(TSystemEvents *Sender);
   void OnLockOutTimer(TSystemEvents *Sender);
   void RefreshModifyGui(TSystemEvents *Sender);
   void OnSmartCardInserted(TSystemEvents *Sender);
   void OnSmartCardRemoved(TSystemEvents *Sender);
   void TransactionComplete(TSystemEvents *Sender);
   void RemoveItem(Database::TDBTransaction &DBTransaction);
   void SetSelectedServingCourse(int SelectedServingCourse);
   int GetServingCourse();
   void ChangeItemsServingCourse();
   void SetReceiptPreview(Database::TDBTransaction &DBTransaction, TStringList * ReceiptDisplay, TMMTabType TabType,
	  AnsiString TabContainerName, AnsiString TabName, AnsiString PartyName = "", int SelectedTab = 0, int TableNo = 0,
	  int SeatNo = 0, int RoomNo = 0);
    void RedrawItemSideCourses();
    void RedrawItemSideItems(TListCourse *);
    void ResetPOS();
    void SetSelectedItem(TItemMinorComplete *Order);
    void __fastcall ProcessWebOrders(bool Prompt = false);
    AnsiString DisplayCustomerTypeReasons();
    int DisplayNameKeypad();
    void SaveTabData(TSaveOrdersTo &OrderContainer);
    void PaintItemToDisplay(long itemKey, int sizesKey);
    void PaintItemToDisplay(long itemKey, long sizesKey , AnsiString qtyValue , AnsiString billValue);
    bool CloseChitForm();
    bool CanClose;
    bool IsChitPromptFormActive;

   //:::::::::::::::::::::::::::::::::::::::::::::::
   // Parked Sales
   //:::::::::::::::::::::::::::::::::::::::::::::::

    void ClearAllParkedSales( Database::TDBTransaction &DBTransaction );
    TModalResult GetOrderContainerForTab(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer);
    TModalResult GetTabContainerForTab(Database::TDBTransaction &DBTransaction, TMMTabType TabType, TSaveOrdersTo &OrderContainer);
    bool SubsidizedDiscountApply(int tabkey);
    bool ParkSaletemp(int tabkey);
    void settempParkedSale( TParkedSale* inSale );
    void SubsidizedQuantityApply(bool IsBilledQty);
    int tabKey;
    TMMContactInfo Info;
    bool IsSubSidizeProfileExist;
    bool IsSubSidizeOrderCancil;
    int temptabkey;
    bool IsSubSidizeProcessed ;
    bool IsTabBillProcessed;
    bool UserForceHappyHourRight;
    bool IsParkSalesEnable;
    bool dc_item_show;
    int DisplayRowCount;
    int LastIndexDisplay;
    eTabType CurrentTabType;
    void RefreshMenu();
    bool check;
    bool IsTextBoxFocused();
    void UpdateMenuItemsAfterLoginScreen();
    int GetDefaultServingCourse(int item_key);
    bool CheckForServingCoursePrompt(int item_key);
    void SyncSiteMenus();
    void SyncTaxSetting();
 };
// ---------------------------------------------------------------------------

class TDataBtn : public TTouchBtn // TCustomStaticText
{
private:
public:
   __fastcall TDataBtn(Classes::TComponent * AOwner);
   __fastcall ~TDataBtn();
   TItem *Item;
};
// ---------------------------------------------------------------------------

class TCustNameAndOrderType
{
private:
    //Singleton Object Instance
    static TCustNameAndOrderType* CustNameAndOrderType;

    TCustNameAndOrderType();
    ~TCustNameAndOrderType();

    TCustNameAndOrderType(TCustNameAndOrderType const&){};
    TCustNameAndOrderType& operator=(TCustNameAndOrderType const&){};

    int CustNameKey;
    AnsiString OrderTypeMessage;
    bool NameCaught;

    AnsiString GetCustNameFromKey(int Key);

protected:

public:
    //Singleton Instance Fetch Method
    static TCustNameAndOrderType* Instance();

    __property bool IsNameCaught =
    {
        read = NameCaught
    };

    void CatchCustNameAndOrderType(int CustKey, AnsiString OrderType, std::vector <TSeatOrders*>& SeatOrders = std::vector <TSeatOrders*>());
    void LoadFromOrdersDatabase(int TableNumber);
    void ClearNameCaught();
    std::pair<AnsiString, AnsiString> GetStringPair();
    bool Valid();
	bool NameAndOrderTypeLoaded();
};

extern PACKAGE TfrmSelectDish *frmSelectDish;

#endif

