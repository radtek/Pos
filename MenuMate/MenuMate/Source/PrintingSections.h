// ---------------------------------------------------------------------------

#ifndef PrintingSectionsH
#define PrintingSectionsH
// ---------------------------------------------------------------------------
#include <vector>
#include <iterator>

#include "PrintFormat.h"
#include "PrintInfo.h"
#include "Item.h"
#include "ItemComplete.h"
#include "PSectionInstruction.h"
#include "enumPrintOutFormatInstructions.h"
#include "DocketFormat.h"

#include "IBillCalculator.h"
using BillCalculator::TBillCalcResult;
using BillCalculator::TAX_RESULT_LIST;
using BillCalculator::TTaxType;

class TPrintOutFormatInstructions
{
public:
	typeInstructions Instructions;

	std::map<ePrintOutFormatInstructions, UnicodeString>DefaultCaption;
	TPrintOutFormatInstructions();
	UnicodeString GetCaption(ePrintOutFormatInstructions Instruction);
	UnicodeString GetInstructionCaption(ePrintOutFormatInstructions Instruction);
};

class TItemSubSection
{
public:
	UnicodeString Caption;
	TPrintInfo FontInfo;
    Currency SideQuantity;
    UnicodeString SideBullets;
    bool isManuallyWeight;
};

class TItemOptionSection
{
public:
	UnicodeString Caption;
	TPrintInfo FontInfo;
    Currency OptionQuantity;
    UnicodeString BulletOption;
    UnicodeString BulletOpt;
};
int __fastcall SortTable(void*Item1, void*Item2);
int __fastcall SortSeat(void*Item1, void*Item2);
int __fastcall SortServingCourse(void*Item1, void*Item2);
int __fastcall SortItems(void*Item1, void*Item2);
int __fastcall SortFoodDrink(void*Item1, void*Item2);

class TReqPrintJob;

class TOrderBundle
{
public:
	__fastcall TOrderBundle();
	__fastcall ~TOrderBundle();

	void SetNMIStandardAus( bool isNMIStandardAus );
	void BundleOrders(
				TReqPrintJob *PrintJob,
				TList *Orders,
				int &CurrentIndex,
				TDocketFormat &inFormat);
    void BundleOrdersWithSides(
				TReqPrintJob *PrintJob,
				TList *Orders,
				int &CurrentIndex,
				TDocketFormat &inFormat);
    bool IsSideAlreadyExist(UnicodeString sideCaption, Currency quantity);
    bool IsOptionAlreadyExist(UnicodeString sideCaption, Currency quantity);
    void SaveOptions(TListOptionContainer *Options1, Currency quantity);

	bool PrevPrinted;

	std::vector<TItemSubSection>SubItems;
	std::auto_ptr<TList>BundledOrders;
	std::auto_ptr<TListOptionContainer> Options;
    std::vector<TItemOptionSection>ItemOptions;//TItemOptionSection

	TDocketFormat Format;
	WideString ItemWeight;
	WideString SeatsLine;
	WideString ItemLine;
	WideString ItemLineSize;
	WideString ItemLineQty;
	WideString SetMenuHeader;
	int SetMenuGroup;
	WideString MixLine; // For Drinks.
	WideString SideLine; // For Food.
	WideString NoteLine;
	WideString CancelLine;
	int ItemsCount;
	TDateTime TimeStamp; // oldest Time within the current bundle.
	TPrintInfo FontInfo;
    bool addManuallyEnteredWeightString;

private:
	bool isDifferentItemFromInitial( TItemComplete* InitialOrder, TItemComplete* CurrentOrder );
	bool seperateWeightedItemsNMI( TItemComplete* currentOrder );
	bool seperatePriceChangedItemsNMI ( TItemComplete* initialOrder, TItemComplete* currentOrder );
	bool seperatePriceBarcodedItemsNMI ( TItemComplete* currentOrder );

	bool usingNMIStandardAus;
};

class TPrintSection
{
public:
	TPrintSection(TPrintFormat *inpPrinter,
    			  TList *WorkingOrders,
                  TSectionInstructStorage &inMasterInstructionsList,
                  TPSectionInstruction *itInstruction);

	TPrintSection(TPrintFormat *inpPrinter,
    			  TList *WorkingOrders,
                  TSectionInstructStorage &inMasterInstructionsList,
                  TPSectionInstruction *itInstruction,
				  TDocketFormat &inFormat);

	~TPrintSection();

	TPrintFormat *pPrinter;

	// TList *Orders;
	// THe Master list of item this element can see which were orignally passed from the parent.
	std::auto_ptr<TList>ParentsOrdersList;

	// OrdersList Holds the Parents Order List passed down, and is used in the Extract
	// and filter process for those elements that loop.
	// i.e for This Iteration the extract Serving courses moves the orders in the Orders list for that
	// Serving course into the working orders list and them processes them, removing them from the
	// original orders list ready for he next iteration.
	std::auto_ptr<TList>OrdersList;
	std::auto_ptr<TList>WorkingOrdersList;

	/* This is a Flat list ( no sub instructions ) or the Print format Instruction.
	Some instructions look though this list and based on what below them format themselves
	accordingly. */
	TSectionInstructStorage &MasterInstructionsList;

	TPSectionInstruction *ThisInstruction;

	void ProcessSection(TReqPrintJob *PrintJob);
	void FormatAndProcess(TReqPrintJob *PrintJob);
	void FormatAndProcessNoChildren(TReqPrintJob *PrintJob);
	void ProcessChildren(TReqPrintJob *PrintJob, TPrintFormat *inpPrinter, TList *WorkingOrders);

	void Clear();

    TDocketFormat Format;


private:

	// Line Above and below the section are suppressed if the section is empty.
	bool Empty;
    const int ReceiptLength;

    void printParagraph(UnicodeString text, int columnWidth);
    void printSingleLine(UnicodeString text, int ColumnWidth, TAlignment alignment);

	void FormatSectionData(TReqPrintJob *PrintJob);

	// Sorts Each Section.
	void SortByTableTab();
	void SortBySeat();
	void SortByServingCourse();
	void SortByCourse();
	void SortByItems();
	void SortByFoodDrink();
	void SortByDate();
	void SortBySoldBy();

	// Prints each section to the TPrintFormat
	void PrintKitchenInfo(TReqPrintJob *PrintJob);
	void PrintTable(TReqPrintJob *PrintJob);
	void PrintSeat(TReqPrintJob *PrintJob);
	void PrintServingCourse(TReqPrintJob *PrintJob);
	void PrintCourse(TReqPrintJob *PrintJob);
	void PrintItem(TReqPrintJob *PrintJob);
	void PrintItemSides(TReqPrintJob *PrintJob);
	void PrintItemOptions(TReqPrintJob *PrintJob);
	void PrintItemNote(TReqPrintJob *PrintJob);
	void PrintItemRavelled(TReqPrintJob *PrintJob, bool groupItemsTemplate = false);
	void PrintItemTotal(TReqPrintJob *PrintJob);
	void PrintItemsTotal(TReqPrintJob *PrintJob);
    void PrintSaleTypeDetails(TReqPrintJob *PrintJob);
	void PrintTotalDicount(TReqPrintJob *PrintJob);
	void PrintTotalDicountDetails(TReqPrintJob *PrintJob);
	void PrintTotalDicountsName(TReqPrintJob *PrintJob);

    bool scdHasBeenApplied( BillCalculator::DISCOUNT_RESULT_LIST inDiscount );
    void printSCDSummary(TReqPrintJob *printJob);
    void populateSCDSummary(TReqPrintJob *printJob, std::vector<AnsiString>& inSCDSummary );
    void printSCDSummary( std::vector<AnsiString> inSCDSummary );

	void PrintFoodDrink(TReqPrintJob *PrintJob);
	void PrintBarCode(TReqPrintJob *PrintJob);
	void PrintChefMateHorizontal(TReqPrintJob *PrintJob);
	void PrintMemberNameInformation(TReqPrintJob *PrintJob);
	void PrintMemberDetailsInformation(TReqPrintJob *PrintJob);
    //MM2038
    void PrintCustomerNameInformation(TReqPrintJob *PrintJob);
    void PrintCustomerPhoneInformation(TReqPrintJob *PrintJob);
	void PrintInvoiceAccountInformation(TReqPrintJob *PrintJob);
	void PrintPartyInformation(TReqPrintJob *PrintJob);
	void PrintTotalEx(TReqPrintJob *PrintJob);
	void PrintGrandTotal(TReqPrintJob *PrintJob);
	void PrintDateOrdered(TReqPrintJob *PrintJob);
	void PrintPointsInformation(TReqPrintJob *PrintJob);
	void PrintMsg(TReqPrintJob *PrintJob);
	void PrintSoldBy(TReqPrintJob *PrintJob);
	void PrintChitNumber(TReqPrintJob *PrintJob);
	void PrintReceiptHeader(TReqPrintJob *PrintJob);
    void PrintReceiptHeaderSecond(TReqPrintJob *PrintJob);
	void PrintReceiptLogo(TReqPrintJob *PrintJob);
	void PrintReceiptFooter(TReqPrintJob *PrintJob);
	void PrintReceiptFooterSecond(TReqPrintJob *PrintJob);
	void PrintPaymentTotals(TReqPrintJob *PrintJob);
	void PrintPaymentSurcharges(TReqPrintJob *PrintJob);
	void PrintWaitTimes(TReqPrintJob *PrintJob);
	void PrintSeparatedTaxTotals(TReqPrintJob *PrintJob);
	void PrintAccountCreditInfo(TReqPrintJob *PrintJob);
	void PrintExtraInfo(TReqPrintJob *PrintJob);
	void PrintDeliveryInfo(TReqPrintJob *PrintJob);
	void PrintPaymentInfo(TReqPrintJob *PrintJob);
	void PrintPaymentHistory(TReqPrintJob *PrintJob);
	void PrintReceiptInfo(TReqPrintJob *PrintJob);
    void PrintSessionDate(TReqPrintJob *PrintJob);
    void PrintWebComments(TReqPrintJob *PrintJob);
	void OpenCashDrawer(TReqPrintJob *PrintJob);

    // actual printing methods for loyaltymate activation receipts
    void PrintLoyaltyReceiptHeader( TReqPrintJob *PrintJob );
    void PrintLoyaltyReceiptMemberNumber( TReqPrintJob *PrintJob );
    void PrintLoyaltyReceiptMemberName( TReqPrintJob *PrintJob );
    void PrintLoyaltyReceiptActivationCode( TReqPrintJob *PrintJob );
    void PrintLoyaltyReceiptActivationInstructions( TReqPrintJob *PrintJob );

	// Returns a List of Orders for the current Section.
	bool ExtractTableList();
	bool ExtractSeatList();
	bool ExtractServingCourseList();
	bool ExtractCourseList();
	bool ExtractItemsList();
	bool ExtractItem();
	bool ExtractDateList();
	bool ExtractItemsList(TItemType inItemType);
	bool ExtractFoodDrinkList();
	bool ExtractCancelsList();
	bool ExtractFreeList(int jobType  );
	bool ExtractSoldByList();

	void PrintOptionsSection(TOrderBundle *, TPrintFormat *);
	void PrintSubordersSection(TOrderBundle *, TPrintFormat *, bool groupItemSides = false);

	void PrintProductTotalsWithUnitPrice(TReqPrintJob *);

	void PrintCustomerName(TReqPrintJob *PrintJob);
	void PrintHotelCustomerName(TReqPrintJob *PrintJob);
	void PrintHotelRoomNumber(TReqPrintJob *PrintJob);
    void PrintCombinedTaxTotal(TReqPrintJob* PrintJob);
    void PrintTaxInclServiceChargeTaxTotal(TReqPrintJob* PrintJob);
    void PrintServiceCharge(TReqPrintJob* PrintJob);
    void PrintAvailableWeight(TReqPrintJob *PrintJob);
// SM Accreditation
// ---------------------------------------------------
    void PrintSalesTaxExempt(TReqPrintJob* PrintJob);
    void PrintSalesTaxable(TReqPrintJob* PrintJob);
    void PrintSalesTax(TReqPrintJob* PrintJob);
    void PrintLocalTax(TReqPrintJob* PrintJob);
    void PrintProfitTax(TReqPrintJob* PrintJob);
    void PrintServiceChargeTax(TReqPrintJob* PrintJob);
    void PrintZeroRated(TReqPrintJob* PrintJob);
    void PrintServiceChargePlusServiceChargeTax(TReqPrintJob* PrintJob);
    void PrintCurrentYearPts(TReqPrintJob *PrintJob);

    Currency getProductTax(TItemMinorComplete* item);
    Currency getLocalTax(TItemMinorComplete* item);
    Currency getProfitTax(TItemMinorComplete* item);
    Currency getServiceCharge(TItemMinorComplete* item);
    Currency getServiceChargeTax(TItemMinorComplete* item);

// ---------------------------------------------------


    static UnicodeString LeftPadString(UnicodeString inString, UnicodeString inChar, int strLen);
	UnicodeString GetHeaderName(TItemComplete *Order);
	WideString GetChefMateHeaderLine(TReqPrintJob *PrintJob, TItemComplete *CurrentOrder, TPrintInfo inFontInfo);
	void GetChefMateItemLine(TReqPrintJob *PrintJob, TPrintInfo inFontInfo, TOrderBundle *CurrentOrder, TStringList *Lines);
	void GetChefMateMiscLine(TReqPrintJob *PrintJob, TPrintInfo inFontInfo, TOrderBundle *CurrentOrder, TStringList *Lines);
    void WrapText(TStringList *TextIn , TStringList *TextOut,int Width);
    void SplitString(UnicodeString Source, TStringList *TextOut, int Width);
    //define by me;
    ///UnicodeString TPrintSection::PrepareName(int col_len, UnicodeString str_name);
    //TStringList TPrintSection::PrepareName(int col_len, UnicodeString str_name, TStringList* name_lst);
    void TPrintSection::PrepareName(int col_len, UnicodeString str_name, TStringList *name_lst);
    void TPrintSection::PrintTwoLinesItemTotal(TReqPrintJob *PrintJob); //2024
    void TPrintSection::PrintTwoLinesItemsTotal(TReqPrintJob *PrintJob); //2024
    // add new functions...
    void PrintTwoLinesItemsName(TReqPrintJob *PrintJob);
    void PrintTwoLinesItemName(TReqPrintJob *PrintJob);
// ---------------------------------------------------
//	NMI standard compliants
// ---------------------------------------------------
	void printItemScales(       TReqPrintJob* PrintJob );
	void printCurrencySymbol(   TReqPrintJob* PrintJob );
// ---------------------------------------------------

// ---------------------------------------------------
//	Utility functions
// ---------------------------------------------------
	bool shouldDisplayDiscountedPrice();
    void PrintDeliveryTime(TReqPrintJob *PrintJob);
    void PrintTextForUnRegisteredDatabase();
    bool IsDBRegistered;

    void PrintOptionsWithQuantity(TOrderBundle *, TPrintFormat *);

    void PrintManuallyEnteredWeightString(TOrderBundle* , 	TPrintFormat* );
    void AddManuallyEnteredLineInReceipt(TPrintFormat*);
    void PrintNonFreeItemsOnly(TReqPrintJob *PrintJob);
    int GetItemLength(UnicodeString itemname, int length);
    void PrintTaxInvoice(TReqPrintJob *PrintJob) ;
    void PrintVoidOnReceipt(TReqPrintJob *PrintJob);
    void ShowRefundReference(TReqPrintJob *PrintJob);
    void PrintBIRSalesTax(TReqPrintJob* PrintJob);
};

// ------------------------------------------------------------------------------
#endif

