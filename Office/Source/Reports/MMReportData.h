//---------------------------------------------------------------------------

#ifndef MMReportDataH
#define MMReportDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RpCon.hpp"
#include "RpConDS.hpp"
#include "RpDefine.hpp"
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBDatabase.hpp>
#include "PreviewData.h"
#include <IBSQL.hpp>
#include <DBClient.hpp>


//---------------------------------------------------------------------------
class TdmMMReportData : public TDataModule
{
__published:	// IDE-managed Components
	TIBQuery *qrMenu;
	TIBQuery *qrMenuStock;
	TRvDataSetConnection *ravMenu;
	TRvDataSetConnection *ravMenuStock;
	TIBTransaction *MMTrans;
	TIBTransaction *StockTrans;
	TIBQuery *qrCashupTotal;
	TRvDataSetConnection *ravCashupTotal;
	TIBQuery *qrCashupFloat;
	TRvDataSetConnection *ravCashupFloat;
	TIBQuery *qrCashup;
	TRvDataSetConnection *ravCashup;
	TIBQuery *qrSalesSummary;
	TRvDataSetConnection *ravSalesSummary;
	TIBQuery *qrTipsVouchers;
	TRvDataSetConnection *ravTipsVouchers;
	TIBQuery *qrHalfHoulrySummary;
	TRvDataSetConnection *ravHalfHoulrySummary;
	TIBQuery *qrWages;
	TRvDataSetConnection *ravWages;
	TIBQuery *qrPatronCount;
	TRvDataSetConnection *ravPatronCount;
	TIBQuery *qrAveSummary;
	TRvDataSetConnection *ravAveSummary;
	TIBQuery *qrAveCount;
	TRvDataSetConnection *ravAveCount;
	TIBQuery *qrDiscountSummary;
	TRvDataSetConnection *ravDiscountSummary;
	TIBQuery *qrAdjustSummary;
	TRvDataSetConnection *ravAdjustSummary;
	TIBQuery *qrConsumption;
	TRvDataSetConnection *ravConsumption;
	TIBQuery *qrUserSales;
	TRvDataSetConnection *ravUserSales;
	TIBQuery *qrChronological;
	TRvDataSetConnection *ravChronological;
	TIBQuery *qrTurnAround;
	TRvDataSetConnection *ravTurnAround;
	TIBQuery *qrBillPayments;
	TRvDataSetConnection *ravBillPayments;
	TIBQuery *qrBillDetails;
	TRvDataSetConnection *ravBillDetails;
	TIBQuery *qrTabSummary;
	TRvDataSetConnection *ravTabSummary;
	TIBQuery *qrTabDetails;
	TRvDataSetConnection *ravTabDetails;
	TIBQuery *qrDiscounts;
	TRvDataSetConnection *ravDiscounts;
	TIBQuery *qrCredits;
	TRvDataSetConnection *ravCredits;
	TIBQuery *qrLoyalty;
	TRvDataSetConnection *ravLoyalty;
	TIBQuery *qrCancelsBill;
	TRvDataSetConnection *ravCancelsBill;
	TIBQuery *qrAdjustments;
	TRvDataSetConnection *ravAdjustments;
	TIBQuery *qrCancelsDet;
	TRvDataSetConnection *ravCancelsDet;
	TIBQuery *qrPatronAverages;
	TRvDataSetConnection *ravPatronAverages;
	TRvDataSetConnection *ravDebtorInvoiceStatement;
	TIBQuery *qrDebtorInvoiceStatement;
	TIBQuery *qrDebtorInvoice;
	TRvDataSetConnection *ravDebtorInvoice;
	TRvDataSetConnection *ravDebtorSummary;
	TIBQuery *qrDebtorSummary;
	TIBQuery *qrSavedSales;
	TRvDataSetConnection *ravSavedSales;
	TIBQuery *qrCancels;
	TRvDataSetConnection *ravCancels;
	TIBQuery *qrLoyaltyHistory;
	TRvDataSetConnection *ravLoyaltyHistory;
	TIBQuery *qrPriceAdjust;
	TRvDataSetConnection *ravPriceAdjust;
	TIBQuery *qrCategoryAnalysis;
	TRvDataSetConnection *ravCategoryAnalysis;
	TIBQuery *qrCatLocTotal;
	TRvDataSetConnection *ravCatLocTotal;
	TIBQuery *qrManualCashDrawer;
	TRvDataSetConnection *ravManualCashDrawer;
	TIBQuery *qrCategoryBreakdown;
	TRvDataSetConnection *ravCategoryBreakdown;
	TIBQuery *qrReductionStock;
	TRvDataSetConnection *ravReductionStock;
	TIBQuery *qrReductionItems;
	TRvDataSetConnection *ravReductionItems;
	TRvDataSetConnection *ravLoyaltyDiscProducts;
	TIBQuery *qrLoyaltyDiscProducts;
	TRvDataSetConnection *ravLoyaltySales;
	TIBQuery *qrLoyaltySales;
	TRvDataSetConnection *ravHappyHour;
	TIBQuery *qrHappyHour;
	TRvDataSetConnection *ravLoyaltyPurchaseCount;
	TIBQuery *qrLoyaltyPurchaseCount;
	TIBQuery *qrBillTenders;
	TRvDataSetConnection *ravBillTenders;
	TIBSQL *sqlView;
	TClientDataSet *cdsHalfHourlySummary;
	TIBQuery *qrWeightedGP;
	TRvDataSetConnection *ravWeightedGP;
	TIBQuery *qrPatronTypes;
	TRvDataSetConnection *ravPatronTypes;
	TClientDataSet *cdsMenu;
	TIBQuery *qrMenuRecipe;
	TRvDataSetConnection *ravMenuRecipe;
	TIBQuery *qrLoyaltyAuditSummary;
	TRvDataSetConnection *ravLoyaltyAuditSummary;
	TIBQuery *qrMenu3rdParty;
	TRvDataSetConnection *ravMenu3rdParty;
	TIBQuery *qrCashupRecon;
	TRvDataSetConnection *ravCashupRecon;
        TRvDataSetConnection *ravMembershipAuditPointsBreakdown;
        TIBQuery *qrMembershipAuditPointsBreakdown;
    TRvDataSetConnection *ravExtendedMembershipAuditPointsBreakdown;
    TIBQuery *qrExtendedMembershipAuditPointsBreakdown;
    TIBQuery *qrInvoice;
    TRvDataSetConnection *ravInvoice;
    TIBQuery *qrSkimming;
    TRvDataSetConnection *ravSkimming;
    TIBQuery *qrRefloat;
    TRvDataSetConnection *ravRefloat;
    TIBQuery *qrInvoiceDetailed;
    TRvDataSetConnection *ravInvoiceDetailed;
    TIBQuery *qrLostRevenue;
    TRvDataSetConnection *ravLostRevenue;
    TIBQuery *qrSalesJournal1;
    TRvDataSetConnection *ravSalesJournal1;
    TIBQuery *qrSalesJournal2;
    TRvDataSetConnection *ravSalesJournal2;
    TIBQuery *qrSalesJournal3;
    TRvDataSetConnection *ravSalesJournal3;
    TIBQuery *qrSalesJournal4;
    TRvDataSetConnection *ravSalesJournal4;
    TIBQuery *qrDayConsumption;
    TRvDataSetConnection *ravDayConsumption;
    TIBTransaction *ChefMateTrans;
   TRvDataSetConnection *ravConsumptionExcSurcharge;
   TIBQuery *qrConsumptionExcSurcharge;
   TRvDataSetConnection *ravDSR;
   TIBQuery *qrDSR;
   TIBQuery *qrDSRTotal;
   TIBQuery *qrDSRInvoice;
   TRvDataSetConnection *ravDSRInvoice;
   TIBQuery *qrDSRConsolidated;
   TRvDataSetConnection *RAVDSRConsolidated;
   TIBQuery *qrDSRdeleteTable;
   TIBQuery *qrDSRTrans;
    TRvDataSetConnection *ravDSRMenuDay;
    TIBQuery *qrDSRMenuDay;
    TIBSQL *sqlAddLK;
    TRvDataSetConnection *ravTaxProfile;
    TIBQuery *qrTaxProfile;
   TIBQuery *qrDiscountsSurchargesTotals;
   TRvDataSetConnection *ravDiscountsSurchargesTotals;
   TRvDataSetConnection *ravSalesSummaryB;
   TIBQuery *qrSalesSummaryB;
   TRvDataSetConnection *ravAccumulatedTotals;
   TIBQuery *qrAccumulatedTotals;
   TRvDataSetConnection *ravAccumulatedZeds;
   TIBQuery *qrAccumulatedZeds;
   TRvDataSetConnection *ravCancelTotal;
   TIBQuery *qrCancelTotal;
   TRvDataSetConnection *ravPaymentTotal;
   TIBQuery *qrPaymentTotal;
   TRvDataSetConnection *ravDiscountTotal;
   TIBQuery *qrDiscountTotal;
   TRvDataSetConnection *ravSalesRevenue;
   TIBQuery *qrSalesRevenue;
   TIBQuery *qrSalesConsolidated;
   TRvDataSetConnection *ravSalesConsolidated;
   TRvDataSetConnection *ravSalesRevenueTotal;
   TIBQuery *qrSalesRevenueTotal;
   TRvDataSetConnection *ravSalesMenuSummary;
   TIBQuery *qrSalesMenuSummary;
   TIBQuery *qrDiscountTotalComplimentary;
   TRvDataSetConnection *ravDiscountTotalComplimentary;

    TRvDataSetConnection *ravCatDiscount;
   TIBQuery *qrDiscountTotalCorporate;
 TIBQuery *qrCatDiscount;
   TIBQuery *qrCancelAdj;
   TRvDataSetConnection *RvDataSetConnection1;
   TIBQuery *qrCancelsAdj;
   TRvDataSetConnection *ravCancelsAdj;
   TIBQuery *qrRefunds;
   TRvDataSetConnection *ravRefunds;
   TRvDataSetConnection *ravDiscountTotalCorporate;
   TIBQuery *qrSalesCountByDayPart;
   TRvDataSetConnection *ravSalesCountByDayPart;
   TIBQuery *qrDiscountTotalNoGroup;
   TRvDataSetConnection *ravDiscountTotalNoGroup;
   TIBQuery *qrComplimentary;
   TRvDataSetConnection *ravComplimentary;
    TIBQuery *qrDSRConsolidatedByItem;
    TRvDataSetConnection *ravDSRConsolidatedByItem;
   TRvDataSetConnection *ravSalesDiscountTotalLocation;
   TIBQuery *qrSalesDiscountTotalLocation;
TRvDataSetConnection *ravCheckRemoval;      //MM-4327
    TIBQuery *qrCheckRemoval;
    TIBQuery *qrReprintReceipt;
    TRvDataSetConnection *ravReprintReceipt;    //MM-4327
    TIBQuery *qrDSRCat;
    TRvDataSetConnection *RvDataSetConnection2;
    TIBQuery *qrDSRCategory;
    TRvDataSetConnection *ravDSRCategory;
    TIBQuery *qrDSRTax;
    TRvDataSetConnection *ravDSRTax;
    TIBQuery *qrDSRDiscount;
    TRvDataSetConnection *ravDSRDiscount;
    TIBQuery *qrDSRrefcan;
    TRvDataSetConnection *ravDSRrefcan;
    TIBQuery *qrDSRSurcharge;
    TRvDataSetConnection *ravDSRSurcharge;
    TIBQuery *qrDSRPay;
    TRvDataSetConnection *ravDSRPay;
    TIBQuery *qrDSRSum;
    TRvDataSetConnection *ravDSRSum;
   TRvDataSetConnection *ravSalesConsolidatedCancel;
   TIBQuery *qrSalesConsolidatedCancel;                     //MM 4579
   TIBQuery *qrResetPoints;                  //MM 4579
   TRvDataSetConnection *ravResetPoint;                    //MM 4579
    TIBQuery *qrReprintOrder;
    TRvDataSetConnection *ravReprintOrder;
    TIBQuery *qrDiscountGrandTotal;
    TRvDataSetConnection *ravDiscountGrandTotal;
    TIBQuery *qrProfiltLoss;
    TRvDataSetConnection *RavProfiltLoss;
    TIBQuery *qrPointSpend;
    TRvDataSetConnection *ravPointSpend;
    TIBQuery *qrBreakdownCategory;
    TRvDataSetConnection *ravBreakdownCategory;

	void __fastcall qrMenuAfterScroll(TDataSet *DataSet);
	void __fastcall qrAveSummaryAfterScroll(TDataSet *DataSet);
	void __fastcall qrBillPaymentsAfterScroll(TDataSet *DataSet);
	void __fastcall qrPatronCountFilterRecord(TDataSet *DataSet,
          bool &Accept);
	void __fastcall qrCancelsBillAfterScroll(TDataSet *DataSet);
	void __fastcall qrReductionStockAfterScroll(TDataSet *DataSet);
	void __fastcall DataModuleDestroy(TObject *Sender);
	void __fastcall qrMenuRecipeAfterScroll(TDataSet *DataSet);

private:	// User declarations
	AnsiString ParamString(int Count, AnsiString Field, AnsiString ParamName="Param", AnsiString Operator="or");
	void SplitItemsAndSizes(TStrings *ItemList,TStringList *ItemNamesList,TStringList *SizeNamesList);
	void AddInZeroHalfHours(bool isConsumptionByHalfHour = false);
	TDateTime FixHalfHour(TDateTime HalfHour);
	void  GetRoundedTime(TDateTime theTime, Word &theHour, Word &theMin, Word &theSec,Word &theMSec );
	void  AdjustHalfHour( bool IncreaseHalfHour, Word &theHour, Word &theMin, bool &AfterMidnight );
    void ShowSql(AnsiString &s);
    AnsiString _taxJoins;
    AnsiString _selectSalesIncl;
    AnsiString _groupByClause;

public:		// User declarations
	__fastcall TdmMMReportData(TComponent* Owner);

	void SetupMenuProfit(TDateTime StartTime, TDateTime EndTime, TStrings *Menus/*, bool IncGST*/);
	void SetupMenuRecipes(TDateTime StartTime, TDateTime EndTime, TStrings *Menus, bool IncGST);
	void SetupStockReductionItems(TStrings *Locations, TStrings *Groups);
	void SetupMenu3rdPartyCodes(TStrings *Menus);

	void SetupCashup(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals);
	void SetupCashupReconciliation(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals);
	void SetupCategoryAnalysis(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, bool GroupByLocation);
	void SetupCategoryBreakdown(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, bool GroupByLocation);
	void SetupHalfHourlyDaily(TDateTime StartTime, TDateTime EndTime,TStrings *Terminals);
	void SetupHalfHourlyConsolidated(TDateTime StartTime, TDateTime EndTime,TStrings *Terminals);
	void SetupTipsAndVouchers(TDateTime StartTime, TDateTime EndTime, TStrings *PayTypes);
	void SetupWagesByDepatment(TDateTime StartTime, TDateTime EndTime, TStrings *Names);
	void SetupWagesByStaff(TDateTime StartTime, TDateTime EndTime, TStrings *Names);
	void SetupSavedSales(TStrings *TabTypes);

//	void SetupSalesSummary(TDateTime StartTime, TDateTime EndTime);
   void SetupCategoryConsumptionExcSurcharge(TDateTime StartTime, TDateTime EndTime, TStrings *Categories);
	void SetupSalesSummaryByLocation(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
	void SetupCategoryConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Categories);
	void SetupSalesTypeConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Categories, TStrings *SalesTypes);
	void SetupCategoryConsumptionByHalfHour(TDateTime StartTime, TDateTime EndTime, TStrings *Categories) ;
	void SetupDayConsumption(TDateTime StartTime, TDateTime EndTime);
	void SetupHalfHourlyDailyByConsumption(TDateTime StartTime, TDateTime EndTime,TStrings *Terminals);

	void SetupMenuConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Menus, bool IncGST);
	void SetupLocationConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Menus, bool IncGST);
	void SetupTabConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Tabs, bool IncGST);
	void Setup3rdPartyConsumption(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Menus);
	void SetupUserSales(TDateTime StartTime, TDateTime EndTime, TStrings *Names, TStrings *Groups);
   void SetupUserSalesByCategory(TDateTime StartTime, TDateTime EndTime, TStrings *Names);
   void SetupUserSalesSummary(TDateTime StartTime, TDateTime EndTime, TStrings *Names);
	void SetupPatronAverages(TDateTime StartTime, TDateTime EndTime);
	void SetupPatronTypes(TDateTime StartTime, TDateTime EndTime,TStrings *Types);
	void SetupTurnAround(TDateTime StartTime, TDateTime EndTime);

	void SetupChronological(TDateTime StartTime, TDateTime EndTime, TStrings *Tables, TStrings *Tabs, TStrings *Terminals);
	void SetupBillPayments(AnsiString InvoiceNumber);
	void SetupBillPayments(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices, TStrings *Terminals);
	void SetupBillDetails(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices, TStrings *Terminals);
	void SetupBillDetails(AnsiString InvoiceNumber);
   void SetupBillTenders( TDateTime StartTime, TDateTime EndTime, TStrings *Tenders, TStrings *Terminals);
	void SetupTabSummary(TStrings *TabTypes,TStrings *Tabs);
   void SetupSalesJournal(bool includeSCTaxInTax, TDateTime StartTime, TDateTime EndTime, TStrings *Terminals);
	void SetupTabDetails(TStrings *TabTypes,TStrings *Tabs);
	void SetupDiscounts(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts,TStrings *Locations);
	void SetupDiscountsAndSurchargesTotals(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts,TStrings *Locations);
	void SetupDiscountedItemsDetails(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
	void SetupDiscountedItemsSummary(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts,TStrings *Locations);
	void SetupCancelsBill(TDateTime StartTime, TDateTime EndTime);
	void SetupCancels(TDateTime StartTime, TDateTime EndTime, TStrings *Names);
	void SetupCredits(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
	void SetupPriceAdjustments(TDateTime StartTime, TDateTime EndTime, TStrings *Locations,TStrings *Names,TStrings *Invoices);
	void SetupAdjustments(TDateTime StartTime, TDateTime EndTime, AnsiString AdjustType);
	void SetupManualCashDrawer(TDateTime StartTime, TDateTime EndTime, TStrings *Users);
	void SetupHappyHour(TDateTime StartTime, TDateTime EndTime, TStrings *Locations,int CurrentSubReport);
    void SetupSkimming( TDateTime StartTime, TDateTime EndTime);
    void SetupRefloat( TDateTime StartTime, TDateTime EndTime);

   void SetupLoyaltyDetails(TStrings *Customers);
	void SetupNewLoyaltyDetails(TDateTime StartTime, TDateTime EndTime);
	void SetupAlteredLoyaltyDetails(TDateTime StartTime, TDateTime EndTime);
	void SetupLoyaltyPoints(TStrings *Customers);
	void SetupLoyaltyHistoryVIPCredit(TDateTime StartTime, TDateTime EndTime, TStrings *Customers,TStrings *Locations);
	void SetupLoyaltyHistoryLocation(TDateTime StartTime, TDateTime EndTime, TStrings *Customers,TStrings *Locations);
	void SetupLoyaltyHistoryCustomer(TDateTime StartTime, TDateTime EndTime, TStrings *Customers,TStrings *Locations);
	void SetupLoyaltyHistoryCustomer(TDateTime StartTime, TDateTime EndTime, TStrings *Customers,TStrings *Locations, bool t);
	void SetupLoyaltyHistoryDates(TDateTime StartTime, TDateTime EndTime,TStrings *Locations);

//	void SetupLoyaltyHistory(TDateTime StartTime, TDateTime EndTime,TStrings *Locations);
	void SetupLoyaltyHistoryVIPCredit(TDateTime StartTime, TDateTime EndTime,TStrings *Locations);
	void SetupLoyaltyBirthdays(TStrings *Months);
	void SetupLoyaltyDiscountedProducts(TDateTime StartTime, TDateTime EndTime,TStrings *Discounts,TStrings *Customers);
	void SetupLoyaltySalesSummaryCategory(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *Groups);
	void SetupLoyaltySalesSummaryItem(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *ItemList);
	void SetupLoyaltySalesDetailedCategory(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *Groups);
	void SetupLoyaltySalesDetailedItem(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *ItemList);
	void SetupLoyaltySalesDetailedCategoryExternalMember(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *Groups);
	void SetupLoyaltySalesDetailedItemExternalMember(TDateTime StartTime, TDateTime EndTime, TStrings *Names,TStrings *ItemList);
	void SetupLoyaltyPurchaseCountByLocation(TStrings *Locations);
	void SetupLoyaltyPurchaseCountByContact(TStrings *Names);
	void SetupLoyaltyAuditSummary(TDateTime StartTime, TDateTime EndTime, TStrings *Names);
	void SetupLoyaltyAudit(TDateTime StartTime, TDateTime EndTime, TStrings *Names ,TStrings *Locations);

	void SetupDebtorInvoice(AnsiString InvoiceNumber);
	void SetupDebtorInvoiceStatement(int DebtorKey, TDateTime PrintDate);
	void SetupDebtorSummary();
	void TestData(TIBQuery *theQuery,bool SaveToClipBrd);
    void SetupInvoice( TDateTime StartTime, TDateTime EndTime, TStrings *Members, TStrings *Groups);
    void SetupInvoiceDetailed( TDateTime StartTime, TDateTime EndTime, TStrings *Members);
    void SetupStockLostRevenue(TDateTime StartTime, TDateTime EndTime);
    void SetupDailySales(TDateTime StartTime, TDateTime EndTime);
    void SetupDailySalesByMenu(TDateTime StartTime, TDateTime EndTime, TStrings *Menus, bool IncGST);
    void SetupDailySalesByCategories(TDateTime StartTime, TDateTime EndTime, TStrings *Categories);
    void SetupDailySalesByInvoice(AnsiString InvoiceNumber);
    void SetupDailySalesByInvoice(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices, bool IncGST);
    void SetupConsolidatedSales(TDateTime StartTime, TDateTime EndTime,TStrings *Locations);
    void  SetupComplimentary(TDateTime StartTime, TDateTime EndTime, TStrings *Invoices);  //MM-3908
    void  SetupNonChargeable(TDateTime StartTime, TDateTime EndTime, TStrings *Categories);
    void SetupDailySalesByCategoriesDate(TDateTime StartTime, TDateTime EndTime, TStrings *Categories);
    void SetupTaxProfile(TStrings *Categories);
    void SetupAccumulatedZeds(TDateTime StartTime, TDateTime EndTime);
    void SetupSalesSummaryB(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
    void SetupSalesSummaryC(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
    void SetupFinanceDaily(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals, TStrings *Locations);
    void  SalesSummaryData(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations)   ;
    void BillTendersByTerminal(TDateTime StartTime, TDateTime EndTime,
                               TStrings *Terminals,  TStrings *Locations);
    void  SalesSummaryDataLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations,TStrings *Terminals) ;
    void  SetupCashupLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations,TStrings *Terminals) ;
    void SetupCashier(TDateTime StartTime, TDateTime EndTime, TStrings *Terminals, TStrings *Locations);
    void SalesSummaryLT(TDateTime StartTime, TDateTime EndTime,  TStrings *Locations, TStrings *Terminals);
    //MM-2966
    void SetupConsolidatedSalesByItem(TDateTime StartTime, TDateTime EndTime);
    //MM-4327
    void SetupCheckRemoval(TDateTime StartTime, TDateTime EndTime);
    //MM-4101
    void SetupDSRSummary(TDateTime StartTime, TDateTime EndTime,TStrings *Menus);
    void SetupDSRSum(TDateTime StartTime, TDateTime EndTime);
    void SetupReprintReceipt(TDateTime StartTime, TDateTime EndTime);
    //MM-4579
    void ResetPoints(TDateTime StartTime, TDateTime EndTime, int i);
    void SetupReprintOrder(TDateTime StartTime, TDateTime EndTime);

    void SetupProfiltLoss(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
 void SetupPointSpend(TDateTime StartTime, TDateTime EndTime, TStrings *Categories, TStrings *payments);
  void SetupLoyaltyMembershipAuditItem1(TDateTime StartTime, TDateTime EndDate, TStrings *Names) ;
  void SetupBreakdownCategory(TStrings *Menus);
    void SetupSalesSummaryD(TDateTime StartTime, TDateTime EndDate);
};


  
//---------------------------------------------------------------------------
extern PACKAGE TdmMMReportData *dmMMReportData;
//---------------------------------------------------------------------------
#endif
