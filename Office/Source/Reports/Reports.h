//---------------------------------------------------------------------------

#ifndef ReportsH
#define ReportsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RPBase.hpp"
#include "RPDefine.hpp"
#include "RPRave.hpp"
#include "RPSystem.hpp"
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
#include <CheckLst.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "RpBase.hpp"
#include "RpDefine.hpp"
#include "RpRave.hpp"
#include "RpSystem.hpp"
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <IBQuery.hpp>
#include "NumericEdit.h"
#include "RpRender.hpp"
#include "RpRenderPDF.hpp"
#include "RpRenderHTML.hpp"
#include "RpRenderRTF.hpp"
#include "RpRenderText.hpp"
#include <DB.hpp>
#include <DBXpress.hpp>
#include <SqlExpr.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#define FAVORITES_INDEX	0
enum TTreeIndex {

			MENUS_INDEX													= 0,
					MENUS_GP_INDEX										,
					MENUS_RECIPES_INDEX								,
					STOCK_REDUCTION_ITEMS							,
					MENU_3RD_PARTY_INDEX								,
					MENU_ITEM_BARCODES								,
                    Tax_Profile_INDEX                                   ,
                    Breakdown_Category                               ,                                     
			FINANCIAL_INDEX											,
                    CASHUP_INDEX										,
                    SALES_SUMMARY_INDEX								,
                    HALF_HOULY_SALES_INDEX							,
                    TIPS_AND_VOUCHERS_INDEX							,
                    WAGES_INDEX											,
                    SAVED_SALES_INDEX									,
                    SALES_JOURNAL_INDEX									,
                    ACCUMULATED_ZED_INDEX                     ,
                    SALES_SUMMARY_B_INDEX                     ,
                    SALES_SUMMARY_C_INDEX                     ,
                    Finance_Daily_index         ,
                    CASHIER_INDEX                             ,
                    DEPT_SALES_SUMMARY                        ,
                    PROFIT_LOSS_REPORT                       ,
                    SALES_SUMMARY_D_INDEX                     ,

			SALES_INDEX													,
					CONSUMPTION_INDEX									,
					CONSUMPTION_BY_HALF_HOUR_INDEX				,
					SALES_BY_SALES_TYPE_INDEX						,
					CATEGORY_ANALYSIS_INDEX							,
					CATEGORY_BREAKDOWN_INDEX						,
					USER_SALES_INDEX									,
					PATRON_AVERAGES_INDEX							,
					PATRON_TYPES_INDEX	    						,
					TURN_AROUND_TIMES_INDEX							,
                    DAILY_SALES_REPORT,
                    Complimentary,
                    NonChargeable ,
			SECURITY_INDEX												,
					CHRONOLOGICAL_INDEX								,
					BILLPAYMENTS_INDEX								,
					BILLDETAILS_INDEX									,
					BILLTENDERS_INDEX                         ,
					TAB_SUMMARY_INDEX					  				,
					TAB_DETAILS_INDEX									,
					DISCOUNTS_INDEX									,
					CANCELS_INDEX										,
					CREDITS_INDEX										,
					PRICE_ADJUST_INDEX								,
					TRANSFERS_INDEX									,
					CASH_DRAWER_INDEX									,
					HAPPY_HOUR_INDEX									,
                    INVOICE_INDEX                                       ,
                    FLOATAUDIT_INDEX                                      ,
                    CHECK_REMOVAL_INDEX  ,
                    REPRINT_RECEIPT_INDEX   ,
                    REPRINT_ORDER_INDEX   ,
                    E_JOURNAL_INDEX,
			LOYALTY_INDEX												,
					LOYALTY_BIRTHDAYS_INDEX							,
					LOYALTY_DISC_PRODUCTS_INDEX	    			,
					LOYALTY_AUDIT_INDEX			      			,
                                        LOYALTY_DETAILS_INDEX							,
					LOYALTY_HISTORY_INDEX							,
                                        LOYALTY_MEMBERSHIP_AUDIT_INDEX,
                                        //LOYALTY_EXTENDED_MEMBERSHIP_AUDIT_INDEX,
					LOYALTY_PURCHASE_COUNT_INDEX      			,
					LOYALTY_SALES_INDEX	   	       			,
					LOYALTY_POINTS_INDEX								,
                    RESET_POINTS_INDEX                        , //MM-4579
                    POINT_SPEND_INDEX                        ,
			STOCK_INDEX													,
					STOCK_DETAILS_INDEX								,
					STOCK_VALUE_INDEX									,
					STOCK_BARCODES_INDEX								,
					STOCK_INQUIRY_INDEX								,
					STOCK_MOVEMENT_INDEX								,
					STOCK_TRANSFER_INDEX								,
                    STOCK_WRITEOFF_INDEX                            ,
                    STOCK_LOST_REVENUE_INDEX                        ,
					STOCK_REORDER_INDEX								,
					STOCKTAKE_VARIANCE_INDEX				  		,
					//STOCKTAKE_HISTORY_INDEX							,
					STOCKTAKE_VALUE_INDEX							,
                    STOCK_MANUFACTURE_INDEX                               ,
                    STOCK_RECONCIALATION_INDEX                      ,
			SUPPLIERS_INDEX											,
					SUPPLIER_DETAILS_INDEX							,
					SUPPLIER_PURCHASES_INDEX						,
					SUPPLIER_BACKORDER_INDEX						,
					SUPPLIER_REORDER_INDEX                     ,
                    SUPPLIER_INVOICE_INDEX
};
//---------------------------------------------------------------------------
class TfrmReports;               
namespace Reports
{
enum TReportFilterType { ftDate, ftCheckBox, ftMonthCheckBox, ftTree, ftNumber, ftString };
enum TReportType { rtPreview, rtExcel, rtGraph };

class TReportControl;
class TReportFilter;
typedef void (TfrmReports::*TPrintReport)	(TReportControl *ReportControl);
typedef void (TfrmReports::*TGetSQL)		(TReportFilter *ReportFilter);
//---------------------------------------------------------------------------
class TSubReport
{
private:
	TList					*Filters;
	int					CurrentFilterIndex;
public:
	TSubReport(AnsiString inCaption) : Caption(inCaption), CurrentFilterIndex(0)
	{
		Filters			= new TList;
	}
	void					AddFilterIndex(int Index)	{ Filters->Add((void *)Index); }
	int					FilterCount()					{ return Filters->Count; }
	void					FirstFilter()					{ CurrentFilterIndex = 0; }
	void					NextFilter()					{ CurrentFilterIndex++; }
	void					PrevFilter()					{ CurrentFilterIndex--; }
	int					CurrentFilter()				{ return int(Filters->Items[CurrentFilterIndex]); }
	bool					AllowPrevFilter()				{ return (CurrentFilterIndex > 0); }
	bool					AllowNextFilter()				{ return (CurrentFilterIndex < Filters->Count-1); }

	AnsiString			Caption;
};
//---------------------------------------------------------------------------
class TReportControl
{
private:
	TList				*ReportFilters;
	TList				*SubReports;
public:
	TReportControl() : CurrentSubReport(-1)
	{
		ReportFilters			= new TList;
		SubReports				= new TList;
	}
	~TReportControl()
	{
		for (int i=0; i<ReportFilters->Count; i++)
		{
			delete				(TReportFilter *)ReportFilters->Items[i];
		}
		delete					ReportFilters;
		for (int i=0; i<SubReports->Count; i++)
		{
			delete				(TSubReport *)SubReports->Items[i];
		}
		delete					SubReports;
	}

	int							AddFilter(TReportFilter *ReportFilter)		{ return ReportFilters->Add(ReportFilter); }
	TReportFilter				*ReportFilter(int Index) 						{ return (TReportFilter *)ReportFilters->Items[Index]; }
	int							FilterCount()										{ return ReportFilters->Count; }
//	int							CurrentFilter;

	TSubReport					*AddSubReport(AnsiString Caption)			{ return SubReport(SubReports->Add(new TSubReport(Caption))); }
	TSubReport					*SubReport(int Index) 							{ return (TSubReport *)SubReports->Items[Index]; }
	AnsiString					GetSubReport(int Index)  						{ return ((TSubReport *)SubReports->Items[Index])->Caption; }
	int							SubReportCount()									{ return SubReports->Count; }
	int							CurrentSubReport;

	TDateTime					Start;
	TDateTime					End;

	TPrintReport				PrintReport;
};
//---------------------------------------------------------------------------
class TReportFilter
{
protected:
	TReportControl				*Controller;
	TIBTransaction				*Transaction;
	TList							*PreviousFilters;
	void							fSetSQL(AnsiString SQL)
	{
		Query->Close();
		Query->SQL->Text		= SQL;
	}
	AnsiString					fGetSQL()
	{
		return					Query->SQL->Text;
	}
public:
	TReportFilter(TReportControl *inController, TIBTransaction *inTransaction)
	{
		Controller				= inController;
		Transaction				= inTransaction;
		Query						= new TIBSQL(NULL);
		Query->Database		= Transaction->DefaultDatabase;
		Query->Transaction	= Transaction;
		PreviousFilters		= new TList;
		GetSQL					= NULL;
		ShowGST					= false;
		GSTChecked				= true;
	}
	virtual ~TReportFilter()
	{
		delete					Query;
		delete					PreviousFilters;
	}
	TIBSQL						*Query;

	TReportFilterType			ReportFilterType;
	AnsiString					Caption;
	bool							ShowGST;
	bool							GSTChecked;

	int							AddPreviousFilters(TReportFilter *ReportFilter) { return PreviousFilters->Add(ReportFilter); }
	TReportFilter				*PreviousFilter(int Index) { return (TReportFilter *)PreviousFilters->Items[Index]; }
	__property AnsiString	SQL = {write=fSetSQL, read=fGetSQL};
	TGetSQL						GetSQL;
	AnsiString					ParameterName;

	virtual void				Refresh()=0;
};
//---------------------------------------------------------------------------
class TReportDateFilter : public TReportFilter
{
private:
public:
	TReportDateFilter(TReportControl *inController, TIBTransaction *inTransaction) : TReportFilter(inController, inTransaction)
	{
		ReportFilterType = ftDate;
	}
	~TReportDateFilter() {};

	void					Refresh() {}
};
//---------------------------------------------------------------------------
class TReportCheckboxFilter : public TReportFilter
{
private:
public:
	TReportCheckboxFilter(TReportControl *inController, TIBTransaction *inTransaction) : TReportFilter(inController, inTransaction)
	{
		ReportFilterType		= ftCheckBox;
		DisplayList				= new TStringList;
		SelectionList			= new TStringList;
		Selection				= new TStringList;
		SelectionDateRange	= false;
	}
	~TReportCheckboxFilter()
	{
		delete DisplayList;
		delete SelectionList;
		delete Selection;
	}

	void Refresh()
	{
		DisplayList->Clear();
		SelectionList->Clear();
		Transaction->StartTransaction();
		try
		{
			Query->Close();
			if (SelectionDateRange)
			{
				Query->ParamByName("StartTime")->AsDateTime	= Controller->Start;
				Query->ParamByName("EndTime")->AsDateTime		= Controller->End;
			}
			for (Query->ExecQuery(); !Query->Eof; Query->Next())
			{
				if (!Query->FieldByName(SelectionField)->IsNull)
				{
					if (DisplayList->IndexOf(Query->FieldByName(DisplayField)->AsString) == -1)
					{
						DisplayList->Add(Query->FieldByName(DisplayField)->AsString);
						SelectionList->Add(Query->FieldByName(SelectionField)->AsString);
					}
				}
			}

			Query->Close();
		}
		__finally
		{
			Transaction->Commit();
		}
	}

	AnsiString			DisplayField;
	AnsiString			SelectionField;
	bool					SelectionDateRange;

	TStringList			*DisplayList;
	TStringList			*SelectionList;

	TStringList			*Selection;
};
//---------------------------------------------------------------------------
class TReportMonthFilter : public TReportCheckboxFilter
{
private:
public:
	TReportMonthFilter(TReportControl *inController, TIBTransaction *inTransaction) : TReportCheckboxFilter(inController, inTransaction)
	{
		ReportFilterType		= ftMonthCheckBox;
	}

	void Refresh()
	{
		DisplayList->Clear();
		SelectionList->Clear();

		DisplayList->Add("January");
		DisplayList->Add("February");
		DisplayList->Add("March");
		DisplayList->Add("April");
		DisplayList->Add("May");
		DisplayList->Add("June");
		DisplayList->Add("July");
		DisplayList->Add("August");
		DisplayList->Add("September");
		DisplayList->Add("October");
		DisplayList->Add("November");
		DisplayList->Add("December");
		for (int i=1; i<=12; i++)
		{
			SelectionList->Add(IntToStr(i));
		}
	}

/*	AnsiString			DisplayField;
	AnsiString			SelectionField;
	bool					SelectionDateRange;

	TStringList			*DisplayList;
	TStringList			*SelectionList;

	TStringList			*Selection;*/
};
//---------------------------------------------------------------------------
class TFilterTreeNode
{
private:
	TList						*Nodes;
	void						ClearNodes()
	{
		for (int i=0; i<Nodes->Count; i++)
		{
			TFilterTreeNode *Node = (TFilterTreeNode *)Nodes->Items[i];
			delete Node;
			Node = NULL;
		}
		Nodes->Clear();
	}
public:
	TFilterTreeNode(AnsiString inCaption, AnsiString inData) : Caption(inCaption), Data(inData)
	{
		Nodes					= new TList;
	}
	~TFilterTreeNode()
	{
		ClearNodes();
		delete				Nodes;
	}
	AnsiString				Caption;
	AnsiString				Data;

	int						NodeCount()										  		{ return Nodes->Count; }
	TFilterTreeNode		*AddChild(AnsiString iCap, AnsiString iData)	{ return Node(Nodes->Add(new TFilterTreeNode(iCap, iData))); }

	TFilterTreeNode		*Node(int Index)										{ return (TFilterTreeNode *)Nodes->Items[Index]; }
};
//---------------------------------------------------------------------------
class TReportTreeFilter : public TReportFilter
{
private:
	TList							*Nodes;
	void							ClearNodes()
	{
		for (int i=0; i<Nodes->Count; i++)
		{
			TFilterTreeNode *Node = (TFilterTreeNode *)Nodes->Items[i];
			delete Node;
			Node = NULL;
		}
		Nodes->Clear();
	}
public:
	TReportTreeFilter(TReportControl *inController, TIBTransaction *inTransaction) : TReportFilter(inController, inTransaction)
	{
		ReportFilterType		= ftTree;
		Nodes						= new TList;
		Selection				= new TStringList;
		SelectionDateRange	= false;
	}
	~TReportTreeFilter()
	{
		ClearNodes();
		delete Nodes;
		delete Selection;
	}

	void Refresh()
	{
		ClearNodes();
		Transaction->StartTransaction();
		try
		{
			Query->Close();
			if (SelectionDateRange)
			{
				Query->ParamByName("StartTime")->AsDateTime	= Controller->Start;
				Query->ParamByName("EndTime")->AsDateTime		= Controller->End;
			}
			AnsiString LastRootKey = "";
			TFilterTreeNode *Root = NULL;
			for (Query->ExecQuery(); !Query->Eof; Query->Next())
			{
				if (!Query->FieldByName(SelectionField)->IsNull)
				{
					if (Query->FieldByName(RootKeyField)->AsString != LastRootKey || !Root)
					{
						Root				= AddRoot(Query->FieldByName(DisplayRootField)->AsString, Query->FieldByName(RootKeyField)->AsString);
						LastRootKey		= Query->FieldByName(RootKeyField)->AsString;
					}
					Root->AddChild(Query->FieldByName(DisplayChildField)->AsString, Query->FieldByName(ChildKeyField)->AsString);
				}
			}
			Query->Close();
		}
		__finally
		{
			Transaction->Commit();
		}
	}

	AnsiString			DisplayRootField;
	AnsiString			RootKeyField;
	AnsiString			DisplayChildField;
	AnsiString			ChildKeyField;

	AnsiString			SelectionField;
	bool					SelectionDateRange;

	TFilterTreeNode	*AddRoot(AnsiString iCap, AnsiString iData)	{ return Node(Nodes->Add(new TFilterTreeNode(iCap, iData))); }
	TFilterTreeNode	*Node(int Index)										{ return (TFilterTreeNode *)Nodes->Items[Index]; }
	int					NodeCount()										  		{ return Nodes->Count; }
//	TStringList			*DisplayList;
//	TStringList			*SelectionList;

	TStringList			*Selection;
};
//---------------------------------------------------------------------------
/*class TReportNumberFilter : public TReportFilter
{
private:
public:
	TReportNumberFilter(TReportControl *inController, TIBTransaction *inTransaction) : TReportFilter(inController, inTransaction)
	{
		ReportFilterType		= ftNumber;
	}
	~TReportNumberFilter() {};

	void					Refresh() {}

	AnsiString			SelectionField;
	bool					SelectionDateRange;

	int					Selection;
};*/
//---------------------------------------------------------------------------
class TReportStringFilter : public TReportFilter
{
private:
public:
	TReportStringFilter(TReportControl *inController, TIBTransaction *inTransaction) : TReportFilter(inController, inTransaction)
	{
		ReportFilterType		= ftString;
	}
	~TReportStringFilter() {};

	void					Refresh() {}

	AnsiString			SelectionField;
	bool					SelectionDateRange;

	AnsiString			Selection;
};
//---------------------------------------------------------------------------
} /* namespace Reports */
using namespace Reports;
class TfrmReports : public TForm
{
__published:	// IDE-managed Components
	TRvSystem *ReportSystem;
	TPanel *Panel5;
	TTreeView *TreeView1;
	TPanel *Panel6;
	TPanel *Panel10;
	TBitBtn *BitBtn8;
	TPageControl *pcReportProperties;
	TTabSheet *tsDateTime;
	TPanel *Panel7;
	TTabSheet *tsCheckFilter;
	TLabel *Label1;
	TLabel *Label2;
	TMonthCalendar *mcStartDate;
	TMonthCalendar *mcEndDate;
	TPanel *pnlStartTime;
	TCSpinButton *csStartHour;
	TCSpinButton *csStartMinute;
	TCSpinButton *csEndHour;
	TPanel *pnlEndTime;
	TCSpinButton *csEndMinute;
	TLabel *lbeTo;
	TLabel *lbeFrom;
	TBevel *Bevel1;
	TBitBtn *btnThisDay;
	TBitBtn *btnWeek;
	TBitBtn *btnThisMonth;
	TBitBtn *btnDateNext;
	TLabel *lbeDateTitle;
	TLabel *lbeCheckFilterRange;
	TRadioButton *rbFilterAll;
	TRadioButton *rbFilterSelection;
	TCheckBox *chbCheckFilterGST;
	TBevel *Bevel3;
	TCheckListBox *chlbFilterList;
	TPanel *Panel8;
	TLabel *lbeCheckFilterTitle;
	TTabSheet *tsTreeFilter;
	TTreeView *tvFilterTree;
	TLabel *lbeTreeFilterRange;
	TRadioButton *rbTreeAll;
	TRadioButton *rbTreeSelection;
	TCheckBox *chbTreeFilterGST;
	TBevel *Bevel2;
	TPanel *Panel9;
	TLabel *lbeTreeFilterTitle;
	TBitBtn *btnTreeFilterBack;
	TBitBtn *btnTreeFilterPreview;
	TBitBtn *btnCheckFilterNext;
	TBitBtn *btnTreeFilterNext;
	TBitBtn *btnCheckFilterBack;
	TBitBtn *btnCheckFilterPreview;
	TBitBtn *btnDatePreview;
	TCheckBox *chbDateGST;
	TIBTransaction *MMFilterTransaction;
	TTabSheet *tsReportType;
	TPanel *Panel11;
	TLabel *lbeTypeTitle;
	TRadioGroup *rgReports;
	TBitBtn *btnTypePreview;
	TBitBtn *btnTypeNext;
	TBevel *Bevel4;
	TBitBtn *btnBackFromDate;
	TTabSheet *tsString;
	TPanel *Panel12;
	TLabel *lbeStringTitle;
	TBitBtn *btnStringPreview;
	TBitBtn *btnStringNext;
	TBitBtn *btnStringBack;
	TBevel *Bevel5;
	TCheckBox *chbStringGST;
	TLabel *lbeStringCaption;
	TRvRenderPDF *RavePDF;
	TRvRenderHTML *RaveHTML;
	TRvRenderRTF *RaveRTF;
	TRvRenderText *RaveText;
    TRvProject *rvMenuMate;
	TIBTransaction *StockFilterTransaction;
	TTabSheet *tsFolderDescription;
	TListView *ListView1;
	TLabel *lbeSelectReport;
	TLabel *lbeFolderDescription;
	TPanel *Panel2;
	TLabel *Label3;
	TRvProject *rvStock;
	TEdit *edString;
	TFindDialog *FindDialog;
	TBitBtn *btnFind;
	TBitBtn *btnCheckFilterExcel;
	TBitBtn *btnTreeFilterExcel;
	TBitBtn *btnStringExcel;
	TBitBtn *btnDateExcel;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall tvReportsChange(TObject *Sender, TTreeNode *Node);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall mcStartDateClick(TObject *Sender);
	void __fastcall csStartHourDownClick(TObject *Sender);
	void __fastcall csStartHourUpClick(TObject *Sender);
	void __fastcall csStartMinuteDownClick(TObject *Sender);
	void __fastcall csStartMinuteUpClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnThisDayClick(TObject *Sender);
	void __fastcall btnThisMonthClick(TObject *Sender);
	void __fastcall csEndHourDownClick(TObject *Sender);
	void __fastcall csEndHourUpClick(TObject *Sender);
	void __fastcall csEndMinuteDownClick(TObject *Sender);
	void __fastcall csEndMinuteUpClick(TObject *Sender);
	void __fastcall btnBackFromFilterClick(TObject *Sender);
	void __fastcall ListView1DblClick(TObject *Sender);
	void __fastcall btnPreviewClick(TObject *Sender);
	void __fastcall rbFilterAllClick(TObject *Sender);
	void __fastcall rbFilterSelectionClick(TObject *Sender);
	void __fastcall tsEndTimeShow(TObject *Sender);
	void __fastcall tvFilterTreeMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tvFilterTreeKeyPress(TObject *Sender, char &Key);
	void __fastcall btnWeekClick(TObject *Sender);
	void __fastcall TreeView1Change(TObject *Sender, TTreeNode *Node);
	void __fastcall btnFilterNextClick(TObject *Sender);
	void __fastcall btnTypeNextClick(TObject *Sender);
	void __fastcall rgReportsClick(TObject *Sender);
	void __fastcall rbTreeAllClick(TObject *Sender);
	void __fastcall rbTreeSelectionClick(TObject *Sender);
	void __fastcall btnTreeFilterNextClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall tsCheckFilterExit(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	TDateTime StartTime, EndTime;
	TTreeNode *LastNode;
	TReportType ReportType;
	bool fRunningReport;
	int CheckListBoxCtr;
	void ShowDateTimes();
	void SetupFilterDisplay();

	TReportControl *ReportControl;
	void ShowCurrentFilter(bool _disableBackAndExcelButton = false);
	void SaveCurrentFilter();
	void AddFilterStringParams(TStrings *SQL, TStrings *Values, AnsiString FieldName, AnsiString Operator="And");
	void AddFilterNumericParams(TStrings *SQL, TStrings *Values, AnsiString FieldName);

	void PrintMenuProfit(TReportControl *ReportControl);
	void PrintMenuRecipes(TReportControl *ReportControl);
	void PrintStockReductionItems(TReportControl *ReportControl);
	void PrintMenu3rdPartyCodes(TReportControl *ReportControl);
	void PrintMenuItemBarcodes(TReportControl *ReportControl);
        void PrintMembershipAudit(TReportControl *);
        void PrintExtendedMembershipAudit(TReportControl *);

	void PrintCashup(TReportControl *ReportControl);
	void PrintCategoryAnalysis(TReportControl *ReportControl);
	void PrintCategoryBreakdown(TReportControl *ReportControl);
	void PrintHalfHourlySales(TReportControl *ReportControl);
	void PrintTipsAndVouchers(TReportControl *ReportControl);
	void PrintWages(TReportControl *ReportControl);
	void PrintSavedSales(TReportControl *ReportControl);

	void PrintSalesSummary(TReportControl *ReportControl);
	void PrintSalesJournal(TReportControl *ReportControl);

	void PrintConsumption(TReportControl *ReportControl);
	void PrintConsumptionBySalesType(TReportControl *ReportControl);
	void PrintConsumptionByHalfHour(TReportControl *ReportControl);
	void PrintUserSales(TReportControl *ReportControl);
	void PrintPatronAverages(TReportControl *ReportControl);
	void PrintPatronTypes(TReportControl *ReportControl);
	void PrintTurnaroundTimes(TReportControl *ReportControl);

	void GetChronologicalTableFilter(TReportFilter *ReportFilter);
	void GetChronologicalTabFilter(TReportFilter *ReportFilter);
	void PrintChronological(TReportControl *ReportControl);

	void GetBillPaymentsReceiptFilter(TReportFilter *ReportFilter);
	void GetBillDetailsReceiptFilter(TReportFilter *ReportFilter);
	void GetPriceAdjustmentsReceiptFilter(TReportFilter *ReportFilter);
	void GetPriceAdjustmentsStaffFilter(TReportFilter *ReportFilter);
	void GetDiscountsFilter(TReportFilter *ReportFilter);
   void GetDiscountsSurchargesNamesFilter(TReportFilter *ReportFilter);
	void GetBillTendersFilter(TReportFilter *ReportFilter);
	void PrintBillPayments(TReportControl *ReportControl);
	void PrintBillDetails(TReportControl *ReportControl);
	void PrintBillTenders(TReportControl *ReportControl);
	void PrintTabSummary(TReportControl *ReportControl);
	void PrintTabDetails(TReportControl *ReportControl);
	void PrintDiscounts(TReportControl *ReportControl);
	void PrintCancels(TReportControl *ReportControl);
	void PrintCredits(TReportControl *ReportControl);
	void PrintPriceAdjustments(TReportControl *ReportControl);
	void PrintTransfers(TReportControl *ReportControl);
	void PrintManualCashDrawer(TReportControl *ReportControl);
	void PrintHappyHour(TReportControl *ReportControl);
    void PrintSkimming(TReportControl *ReportControl);
    void PrintRefloat(TReportControl *ReportControl);

	void GetCurrentTabList(TReportFilter *ReportFilter);
   void GetDetailTabList(TReportFilter *ReportFilter);

	void PrintLoyaltyCustomerDetails(TReportControl *ReportControl);
	void PrintPointsEarned(TReportControl *ReportControl);
	void PrintLoyaltyHistory(TReportControl *ReportControl);
	void PrintLoyaltyBirthdays(TReportControl *ReportControl);
	void PrintLoyaltyDiscountedProducts(TReportControl *ReportControl);
	void PrintLoyaltySales(TReportControl *ReportControl);
   void PrintLoyaltyPurchaseCount(TReportControl *ReportControl);
	void PrintLoyaltyAudit(TReportControl *ReportControl);
   void PrintResetPoint(TReportControl *ReportControl); //MM-4579


	void GetStockLocationItemFilter(TReportFilter *ReportFilter);

	void PrintStockDetails(TReportControl *ReportControl);
	void PrintStockValue(TReportControl *ReportControl);
	void PrintStockBarcodes(TReportControl *ReportControl);
	void PrintStockMovement(TReportControl *ReportControl);
	void PrintStockTransfer(TReportControl *ReportControl);
	void PrintStockInquiry(TReportControl *ReportControl);
	void PrintStockReorder(TReportControl *ReportControl);
	void PrintStocktakeVariance(TReportControl *ReportControl);
	void PrintStocktakeValue(TReportControl *ReportControl);
    void PrintStockWriteOff(TReportControl *ReportControl);
    void PrintStockLostRevenue(TReportControl *ReportControl);
    void PrintStockManufacture(TReportControl *ReportControl);


	void PrintPurchaseInvoices(TReportControl *ReportControl);
	void GetStockTransSupplierList(TReportFilter *ReportFilter);
	void GetStockInvoiceList(TReportFilter *ReportFilter);

    void GenerateMembershipQuery(TIBQuery *, TReportControl *,
        AnsiString &, AnsiString &);
    void GenerateExtendedMembershipQuery(TIBQuery *, TReportControl *,
        AnsiString &, AnsiString &);

	void PrintSupplierDetails(TReportControl *ReportControl);
	void PrintSupplierBackOrders(TReportControl *ReportControl);
	void PrintSupplierReorder(TReportControl *ReportControl);
	void ExportToExcel( TStringList *ExcelDataSets,AnsiString ACaption );
   void DailySalesReport(TReportControl *ReportControl);
void PrintNonChargeable(TReportControl *ReportControl)  ;
void PrintComplimentary(TReportControl *ReportControl);
   void PrintTaxProfile(TReportControl *ReportControl) ;
      void PrintSupplierInvoice(TReportControl *ReportControl);
   void PrintAccumulatedZeds(TReportControl *ReportControl);
	void PrintSalesSummaryB(TReportControl *ReportControl);
	void PrintSalesSummaryC(TReportControl *ReportControl);
    	void PrintFinanceDaily(TReportControl *ReportControl);

   void PrintCashier(TReportControl *ReportControl);
   void PrintDeptSalesSummary(TReportControl *ReportControl);
   void GetTerminalFilter(TReportFilter *ReportFilter) ;
   void PrintCheckRemoval(TReportControl *ReportControl);     //MM-4327
     void PrintReprintReceipt(TReportControl *ReportControl);
   void PrintReprintOrder(TReportControl *ReportControl);

   	void GetStockReceiptList(TReportFilter *ReportFilter);

    void PrintProfitLossSummary(TReportControl *ReportControl);
public:		// User declarations
	__fastcall TfrmReports(TComponent* Owner);
    bool SaveReportToFile(AnsiString &EmailFileName );
    void SetToPreview( );
    void SendEmail( AnsiString EmailFileName, AnsiString EmailAddress, AnsiString EmailSubject, AnsiString ReceiverName );
    void PrintInvoice(TReportControl *ReportControl);
    void GetInvoiceFilter(TReportFilter *ReportFilter);
    void PrintPointSpend(TReportControl *ReportControl);
    void PrintBreakdownCategory(TReportControl *ReportControl);
    void PrintStockReconcialation(TReportControl *ReportControl);
    void PrintSalesSummaryD(TReportControl *ReportControl);
    void PrintEJournalReport(TReportControl *ReportControl);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReports *frmReports;
//---------------------------------------------------------------------------
#endif
