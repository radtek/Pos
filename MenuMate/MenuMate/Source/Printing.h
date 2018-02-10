//---------------------------------------------------------------------------
#ifndef PrintingH
#define PrintingH

#include <set>
#include <map>
#include "Request.h"
#include "MMLogging.h"
#include "PrintFormat.h"
#include "Money.h"
#include "ManagerPhysicalPrinter.h"
#include "VirtualPrinterManager.h"
#include "MM_DBCore.h"
#include "ManagerChitNumber.h"
#include "ReqPrintJob.h"

#include "enumPrinting.h"
#include "PrintingSections.h"
#include "LoyaltyPrinting.h"
#include "TransferComplete.h"
//------------------------------------------------------------------------------
//------------------------Print Job class for TPrinterConnects Printer Ques ----
//------------------------------------------------------------------------------
class TPrintJob;
class TPrintout;
class TConnectPrinter;
class TKitchenCourseTable;
class TCallAwayComplete;
//---------------------------------------------------------------------------
//enum TPrinterModel { pmUnknown, pmAxiohm_A715, pmAxiohm_A714, pmEpson_TM_T88, pmEpson_U210B };


//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Helper Class for generating Print Jobs for the Kitchen.
class TKitchen
{
private:
	TList *Courses;
	TList *OrdersList;
   int SeatCount; 												// A list of TKitchenCourses. The Courses Present in the current Request.
	void BundleCallAwayCoursesTables(TCallAwayComplete *CallAway,TReqPrintJob *Request,TStringList *PrinterCallAwaysList);	// Bundles stuff into Courses above.

	TPrintout* GetPrintout(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request, TPrinterVirtual *VirtualPrinter, bool CallAway = false);	// Finds the appriate printout for the
																										// printer or creates one
	void AddCallAwayToPrintout(TPrintFormat *PrintFormat);								// Adds pre-sorted courses to the above printout.
    void AddTransferToPrintout(TPrintFormat *pPrinter,TTransferComplete *Transfer);     // Add Print Transfer for the Table Transfer to Kitchen
	void PrintOrderHeader(TPrintFormat *pPrinter,										// Header for AddOrdersToPrintout()
		TKitchenCourseTable *CourseTable);
	void PrintCallAwayHeader(TPrintFormat *pPrinter,										// Header for AddOrdersToPrintout()
		TKitchenCourseTable *CourseTable);

	public:
	bool GetPrintouts(Database::TDBTransaction &DBTransaction,TCallAwayComplete *CallAway,TReqPrintJob *Request);
	bool GetPrintouts(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request,TPrinterTypeFilter PrinterTypeFilter = pfAll_Printers,TSectionInstructStorage *Template = NULL,int VirtualPrinterKeyFilter = 0,bool isChefmate=false);
	bool GetPrintoutsPreview(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request,TPrinterVirtual *CurrentPrinter, TSectionInstructStorage *Template);

   void GetChildren(TSectionInstructStorage &Template,
                    TPSectionInstruction *ParentNodeInstruction,
                    TSectionInstructStorage::iterator &itInstruction);

   // Add Print Transfer for the Table Transfer to Kitchen
   bool GetPrintouts(Database::TDBTransaction &DBTransaction,TTransferComplete *Transfer,TReqPrintJob *Request);

   // Returns the Print job stored in the PrintJobs TList so we can add to it.
   __fastcall TKitchen();
   __fastcall ~TKitchen();
   bool PrintMenuKeyItems;
   void Initialise(Database::TDBTransaction &DBTransaction);

   TDocketFormat Format;

   void GetSetMenuHeaders(Database::TDBTransaction &DBTransaction,TList *OrdersList);

	bool GetOrdersForThisPrinter(Database::TDBTransaction &DBTransaction,TPrinterVirtual *CurrentPrinter, TList * PrinterOrderList);
	bool GetCallAwaysForThisPrinter(Database::TDBTransaction &DBTransaction,TPrinterVirtual *CurrentPrinter, TStringList * PrinterOrderList);
	static void LoadKitchenTemplate(Database::TDBTransaction &DBTransaction,int PrinterProfileKey,TSectionInstructStorage &inKitchenTemplate);
	static void SaveKitchenTemplate(Database::TDBTransaction &DBTransaction,int PrinterProfileKey,TSectionInstructStorage &inKitchenTemplate);
};

// TODO : Use Singleton pattern here.
//------------------------------------------------------------------------------

enum DisplayBarCodeOverride {eDispBCNormal,eDispBCOff,eDispBCOn};

class TReceipt
{
private:
	void AddOrdersToPrintout(TReqPrintJob* Request);
   void GetChildren( TSectionInstructStorage &Template,
                     TPSectionInstruction *ParentNodeInstruction,
                     TSectionInstructStorage::iterator &itInstruction);
public:
   void LoadTemplate(Database::TDBTransaction &DBTransaction,int PrinterKey,TSectionInstructStorage &inTemplate);
   void SaveTemplate(Database::TDBTransaction &DBTransaction,int PrinterKey,TSectionInstructStorage &inTemplate);
	void _fastcall GetPrintouts(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request, TPrinterPhysical Printer,DisplayBarCodeOverride BCOverRide = eDispBCNormal );
   bool GetPrintoutsPreview(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request,TPrinterPhysical Printer,TSectionInstructStorage *inTemplate);

	__fastcall TReceipt::TReceipt();
	__fastcall TReceipt::~TReceipt();
	TStringList *Header;
	TStringList *PHeader;
	TStringList *Footer;
	void SetHeaderFooter(TStrings *inHeader ,TStrings *inPHeader,TStrings *inFooter, TStrings *inVoidFooter,TStrings * inSubHeader);
	// Printing a list of selected Seats.
	Currency GrandTotal;
	Currency TotalChange;
	// Stores all the Seat orders for this Table in a printable format.
    bool PrintingEnabled;
    bool PrintNoteWithDiscount;
    bool TipOnRecipt;
    bool OptionOnRecipt;
    bool SignatureOnRecipt;
    bool SumPriceAdjusts;
    bool SumDiscounts;
    bool DisplayBarCodes;
    bool DisplayChitNumber;
    bool AlwaysPrintReceiptTenderedSales;
    bool AlwaysPrintReceiptCashSales;
    bool ConsolidateReceipt;
    bool HideFreeItemsOnReceipt;
    bool HideZeroDollarsOnReceipt;
    void Initialise(Database::TDBTransaction &DBTransaction);
    bool RavellItemsWithPriceAdjusts;
    int  MembershipNameDisplay;
    bool AlwaysPrintReceiptDiscountSales;

	UnicodeString BulletOpt;
	UnicodeString NoteHeader;
    TStringList *VoidFooter;
    TStringList *SubHeader;
};

//---------------------------------------------------------------------------
class TKitchenCourseTable
{
public:
	int PrintingOrder;
	WideString Course;
	WideString CourseKitchenName;
	int TableNo;
/* Required in case we ever want to sort courses based on menu and in order to
   Filter the Printing order so we can have a different order for different menus.*/
	UnicodeString Menu;
	TChitNumber ChitNumber;
	UnicodeString DocketNumber;
   bool ThisCourseWasPrinted; // We will use this to track weather a course was ever printed.
	UnicodeString TableName;
	bool PrintHeader;
   UnicodeString Waiter;
   TDateTime TimeStamp;
	TKitchenCourseTable(UnicodeString inMenuFK,int inTable,WideString inCourse,WideString inCourseKitchenName,UnicodeString inTableName);
	virtual ~TKitchenCourseTable();
};

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------




class TCallAwayComplete
{
public:
	long TableNo;
    UnicodeString TableName;
    UnicodeString PartyName;
    TStringList *Courses;	// Serving course names
    TCallAwayComplete();

	virtual __fastcall ~TCallAwayComplete();

    void CopyFrom( const TCallAwayComplete* source );
};

//------------------------------------------------------------------------------
extern TReceipt *Receipt;
//------------------------------------------------------------------------------


#endif
