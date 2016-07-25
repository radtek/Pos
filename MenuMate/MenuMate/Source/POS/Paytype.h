//---------------------------------------------------------------------------
#ifndef PaytypeH
#define PaytypeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchNumpad.h"
#include "TouchGrid.h"
#include <set>
#include "Requests.h"
#include "ZForm.h"
#include "SystemEvents.h"
#include "DBTierLevel.h"
#include "PaymentTransaction.h"
 #include "ConfirmOrd.h"
 #include "Money.h"
#include "MallExportDefines.h"
#include "ComboForm.h"
//---------------------------------------------------------------------------
class TfrmPaymentType : public TZForm
{

	friend TZForm;
__published:	// IDE-managed Components
	TTimer *Timer1;
   TPanel *Panel2;
   TPanel *Panel5;
   TPanel *Panel12;
   TPanel *Panel7;
   TTouchBtn *btnPrelim;
   TPanel *Panel9;
   TLabel *lbeMembership;
   TMemo *Memo1;
   TTouchGrid *tgPayments;
	TPanel *Panel25;
	TPanel *Panel6;
	TPanel *Panel4;
	TPanel *Panel1;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *lbeDiscountSurcharge;
	TStaticText *stChange;
	TStaticText *stBalance;
	TStaticText *stDiscount;
	TPanel *Panel3;
	TTouchNumpad *tnWorkingAmount;
	TNumpadDisplay *ndWorkingAmount;
	TPanel *Panel8;
	TTouchBtn *tbCredit;
	TTouchBtn *tbPatronCount;
	TTouchBtn *tbtnDiscountSurcharge;
	TTouchBtn *tbtnRemoveTaxes;
	TTouchBtn *btnReceiptUp;
	TTouchBtn *btnReceiptDown;
	TTouchBtn *btnCancel;
   void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnPrelimClick(TObject *Sender);
	void __fastcall pnlCancelClick(TObject *Sender);
	void __fastcall stDoneClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnReceiptUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnReceiptUpMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnReceiptDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnReceiptDownMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall lbeCLRMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall lbeDiscountClick(TObject *Sender);
	void __fastcall lbeRemoveTaxClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tbPatronCountClick(TObject *Sender);
	void __fastcall tbCreditClick(TObject *Sender);
	void __fastcall tnWorkingAmountClick(TObject *Sender, TNumpadKey Key);
   void __fastcall tgPaymentsMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);

private:	// User declarations

	static TForm *WinOwner;
    bool isRefundButtonVisible;
    bool isRefundInProgress;
    bool index_Thor;
    bool IsWrkPayAmountChanged;
    bool IsVoucher;
	void __fastcall CreateParams(Controls::TCreateParams &params);
    void HideInvisibleRows();

   void __fastcall BtnPayment(TPayment *Payment);
   void __fastcall BtnPaymentAlt(TPayment *Payment);
    void RemoveTaxes(TStringList* removedTaxes);
    void GetOrderTaxes(TStringList* taxList);
    void GetRemovedTaxes(TStringList* removedTaxes);
    void PopulateReceipt(TReqPrintJob *Receipt);
	void ShowReceipt();
	Currency wrkPayAmount;
	void DisableOtherElectronicPayments(TPayment *inPayment);
	void DisableCashOutElectronicPayments();
	void EnableCashOutElectronicPayments();
	bool NoElectronicPayments();
	void GetPaymentNote(TPayment * Payment);
	void ShowPaymentTotals(bool MembersDiscount = false);
	void __fastcall ApplyDiscount(int DiscountKey,int ContactKey,TDiscountSource DiscountSource = dsMMUser);
	void RemoveMembershipDiscounts();
	// For scrolling the receipt
	TWinControl *ScrollingControl;
	TControl *ButtonClicked;
	bool ScrollUp;
	void Reset();
        Currency GetAvailableRedeemPoints(TPaymentTransaction PointsTransaction);
	Currency PointsKnapsack(TList *Orders, Currency AvailablePoints, Currency StepSize = MIN_CURRENCY_VALUE);
        void  ProcessPointPayment(TPayment *Payment);
        Currency GetTotalWeightInOrder(TList *Orders);
        Currency CalculatePointsForWeight(TList *Orders);
        void CalculateRedeemedPoints(TList *Orders,TRedeemPointsInformation *RedeemPointsInformation,
                                     Currency MaxAmountToRedeem, bool isPaybyWeight);
        void CalculatePointsWeighedItem(TItemComplete *Order,TTierLevel *TierLevel,TRedeemPointsInformation *RedeemPointsInformation,
                                         Currency MaxAmountToRedeem);
        void CalculatePointsNonWeighedItem(TItemComplete *Order,TTierLevel *TierLevel,TRedeemPointsInformation *RedeemPointsInformation,
                        Currency MaxAmountToRedeem);
        void CalculatePoints(Currency Qty, Currency Cost,TTierLevel *TierLevel, Currency MaxValue,
                TRedeemPointsInformation *RedeemPointsInformation,bool IsWeighted);
        Currency GetPointsForPrice(Currency value, Currency Cost,TTierLevel *TierLevel);
        Currency CalculateWeightForPoints(Currency Points);

    void disableButtonIfPermissionNotAllowed(
                            Database::TDBTransaction &DBTransaction,
                            TGridButton *button,
                            TMMContactInfo &userInfo,
                            SecurityCheck Permission );
    bool IsAllTabItemSelected();
    int purchaseButtonIndex;
    void CopyPaymentColor(int rowIndex);
    bool AllowRefund;
    bool ProcessMemberPointOnItemBasis(TPayment *Payment,Currency selectedPoints,Currency selectedPointsValue);
    void ApplyMemberPointsDiscount(Currency selectedPoints,Currency selectedPointsValue,bool includeAll = false);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall TfrmPaymentType::CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)

	TPaymentTransaction &CurrentTransaction;
public:		// User declarations
__fastcall TfrmPaymentType(TComponent* Owner,TPaymentTransaction &inCurrentTransaction);

	bool Recovering;
      int RedeamedPoints;
	bool SplitPayment;
	std::vector<TPatronType> PreCreditPatronCount;
	bool SecurePaymentAccess(TPayment * Payment);
	void EnableElectronicPayments();
	TModalResult Execute();
	bool QueryPatronCount;

	static TfrmPaymentType *Create(TForm* Owner,TPaymentTransaction &inCurrentTransaction);

   void OnSmartCardInserted(TSystemEvents *Sender);
   void OnSmartCardRemoved(TSystemEvents *Sender);   
   void ApplyMembership(TMMContactInfo &Member);
   void ApplyAccount(TMMContactInfo &Member);
   void ClearAccount();
   void RemoveMembership();
   void ProcessThorVouchers();

private:
  bool CheckDiscountPointsPaytype;
  int PrintCancelEvent;
  AnsiString memberNumber;
  void ShowFormToSelectItems(TPayment *Payment);
  bool CheckItemHasPoints();
  bool CheckOnlinePaidOrNot();
  void ShowWebOrderMembersPayment();
  bool IsMemberSelected;
  bool ThorMemberIsUnregistered();
};

class TPaymentTypeTouchButton : public TTouchBtn
{
public:
	TPayment *Payment;
	TPaymentTypeTouchButton *Sibling;
	__fastcall TPaymentTypeTouchButton(Classes::TComponent* AOwner);
};

//---------------------------------------------------------------------------


// Temporarily keeping a Matrix class here for use with the Knapsack Algorithm in Paytype.
// I want to move this out to it's own class but I'm not sure where exactly it should be kept. (Maybe a new folder?)
class CurrencyMatrix
{
private:
	unsigned NumRows, NumCols;
	Currency *DataArray;

public:
	CurrencyMatrix(unsigned Rows, unsigned Rols);	//Throws length_error exception
	~CurrencyMatrix();

	CurrencyMatrix(const CurrencyMatrix &OtherMatrix);
	CurrencyMatrix& operator= (const CurrencyMatrix &OtherMatrix);

	Currency& operator() (unsigned Row, unsigned Col);			//Throws out_of_range exception
	Currency  operator() (unsigned Row, unsigned Col) const;	//Throws out_of_range exception

	__property unsigned Rows =
	{
		read = NumRows
	};

	__property unsigned Cols =
	{
		read = NumCols
	};
};
extern bool PayNotCash;
#endif
