
// ---------------------------------------------------------------------------

#pragma hdrstop

#include "PrintingSections.h"
#include "Printing.h"
#include "TableManager.h"
#include <DateUtils.hpp>
#include "OrderUtils.h"
#include <StrUtils.hpp>
#include <Classes.hpp>

#include "ListSecurityRefContainer.h"
#include "ReqPrintJob.h"
#include "PaymentTransaction.h"
#include "GlobalSettings.h"
#include "ItemCompleteSub.h"
#include "ManagerPatron.h"
#include "rounding.h"
#include "DBTierLevel.h"
#include <map>
#include <list>
#include <cmath>

#include "DiscountGroup.h"
#include "MMTouchKeyboard.h"
#include "ZForm.h"
#include "ReceiptUtility.h"
//#include <wchar.h>


// ---------------------------------------------------------------------------
#pragma package(smart_init)
#define check_space 0x20

extern int OptionsCompare(TListOptionContainer * Options1, TListOptionContainer * Options2);
extern int __fastcall SubOrdersCompare(TListSubOrderContainer * List1, TListSubOrderContainer * List2);

TPrintOutFormatInstructions::TPrintOutFormatInstructions()
{
	int i = 1;

	// Filters
	Instructions[i++] = InstructionPair(epofiFilterFood, "Filter Food");
	DefaultCaption[epofiFilterFood] = "Filter Food";

	Instructions[i++] = InstructionPair(epofiFilterBev, "Filter Beverage");
	DefaultCaption[epofiFilterBev] = "Filter Beverage";

	Instructions[i++] = InstructionPair(epofiFilterOutCancels, "Filter Out Cancels");
	DefaultCaption[epofiFilterOutCancels] = "Filter Out Cancels";

	Instructions[i++] = InstructionPair(epofiFilterOutFree, "Filter Out Free");
	DefaultCaption[epofiFilterOutFree] = "Filter Out Free";

	// Sorting
	Instructions[i++] = InstructionPair(epofiSortByDateOrdered, "Sort By Date");
	DefaultCaption[epofiSortByDateOrdered] = "Sort By Date";

	Instructions[i++] = InstructionPair(epofiSortByFoodDrink, "Sort By Food Drink");
	DefaultCaption[epofiSortByFoodDrink] = "Sort By Food Drink";

	// Formating
	Instructions[i++] = InstructionPair(epofiBreak, "-- Break --");
	DefaultCaption[epofiBreak] = "-- Break --";

	// Details
	Instructions[i++] = InstructionPair(epofiPrintKitchenInfo, "Kitchen Details");
	DefaultCaption[epofiPrintKitchenInfo] = "Kitchen Details";
	//	Instructions[i++] = InstructionPair(epofiPrintKitchenInfo, "Top Details Kitchen");
	//	DefaultCaption[epofiPrintKitchenInfo] = "Top Details Kitchen";

	Instructions[i++] = InstructionPair(epofiPrintReceiptInfo, "Receipt Details");
	DefaultCaption[epofiPrintReceiptInfo] = "Receipt Details";

	//Adding a new setting for receipt details with session date
	Instructions[i++] = InstructionPair(epofiSessionDateInfo, "Session Date");
	DefaultCaption[epofiSessionDateInfo] = "Session Date";

	//	Instructions[i++] = InstructionPair(epofiPrintReceiptInfo, "Top Details Receipt");
	//	DefaultCaption[epofiPrintReceiptInfo] = "Top Details Receipt";

	// Headers and Footer
	Instructions[i++] = InstructionPair(epofiPrintReceiptLogo, "Receipt Logo");
	DefaultCaption[epofiPrintReceiptLogo] = "Receipt Logo";

	Instructions[i++] = InstructionPair(epofiPrintSaleTypeDetail, "Sale Type");
	DefaultCaption[epofiPrintSaleTypeDetail] = "Sale Type";

	Instructions[i++] = InstructionPair(epofiPrintReceiptHeader, "Receipt Header");
	DefaultCaption[epofiPrintReceiptHeader] = "Receipt Header";

	Instructions[i++] = InstructionPair(epofiPrintReceiptFooter, "Receipt Footer");
	DefaultCaption[epofiPrintReceiptFooter] = "Receipt Footer";

	// Order Information
	Instructions[i++] = InstructionPair(epofiPrintChitNumber, "Chit Number");
	DefaultCaption[epofiPrintChitNumber] = "Chit Number";

	Instructions[i++] = InstructionPair(epofiPrintTable, "Tab/Table");
	DefaultCaption[epofiPrintTable] = "Tab/Table";

	Instructions[i++] = InstructionPair(epofiPrintSeat, "Seat/Guest");
	DefaultCaption[epofiPrintSeat] = "Seat/Guest";

	// Course and Serving Course Information
	Instructions[i++] = InstructionPair(epofiPrintServingCourse, "Serving Course");
	DefaultCaption[epofiPrintServingCourse] = "Serving Course";

	Instructions[i++] = InstructionPair(epofiPrintCourse, "Course");
	DefaultCaption[epofiPrintCourse] = "Course";

	// ???
	Instructions[i++] = InstructionPair(epofiPrintBarCode, "Barcode");
	DefaultCaption[epofiPrintBarCode] = "Barcode";

	// Members Information
	Instructions[i++] = InstructionPair(epofiPrintMembersName, "Members Name");
	DefaultCaption[epofiPrintMembersName] = "Members Name";

	Instructions[i++] = InstructionPair(epofiPrintMembersDetails, "Members Details");
	DefaultCaption[epofiPrintMembersDetails] = "Members Details";

	//MM2038
	Instructions[i++] = InstructionPair(epofiPickUpPrintCustomerName, "Customer Name");
	DefaultCaption[epofiPickUpPrintCustomerName] = "Customer Name";

	Instructions[i++] = InstructionPair(epofiPickUpPrintCustomerPhone, "Customer Phone");
	DefaultCaption[epofiPickUpPrintCustomerPhone] = "Customer Phone";

	Instructions[i++] = InstructionPair(epofiPrintPointsDetails, "Points Details");
	DefaultCaption[epofiPrintPointsDetails] = "Points Details";

	// Receipt Information
	Instructions[i++] = InstructionPair(epofiPrintPartyName, "Party Name");
	DefaultCaption[epofiPrintPartyName] = "Party Name";

	Instructions[i++] = InstructionPair(epofiPrintSoldBy, "Sold By");
	DefaultCaption[epofiPrintSoldBy] = "Sold By";

	Instructions[i++] = InstructionPair(epofiPrintWaitTimes, "Wait Times");
	DefaultCaption[epofiPrintWaitTimes] = "Wait Times";

	// Items Information
	Instructions[i++] = InstructionPair(epofiPrintItem, "Item");
	DefaultCaption[epofiPrintItem] = "Item";


	Instructions[i++] = InstructionPair(epofiPrintItemSides, "Item Sides");
	DefaultCaption[epofiPrintItemSides] = "Item Sides";

	Instructions[i++] = InstructionPair(epofiPrintItemOptions, "Item Options");
	DefaultCaption[epofiPrintItemOptions] = "Item Options";

	Instructions[i++] = InstructionPair(epofiPrintItemNote, "Item Note");
	DefaultCaption[epofiPrintItemNote] = "Item Note";

	Instructions[i++] = InstructionPair(epofiPrintItems, "Items");
	DefaultCaption[epofiPrintItems] = "Items";

	// Totals
	Instructions[i++] = InstructionPair(epofiPrintItemTotal, "Item Total");
	DefaultCaption[epofiPrintItemTotal] = "Item Total";

	Instructions[i++] = InstructionPair(epofiPrintItemsTotal, "Items Total");
	DefaultCaption[epofiPrintItemsTotal] = "Items Total";

	Instructions[i++] = InstructionPair(epofiPrintTotalEx, "Sub Total");
	DefaultCaption[epofiPrintTotalEx] = "Sub Total";

	Instructions[i++] = InstructionPair(epofiPrintTotal, "Total");
	DefaultCaption[epofiPrintTotal] = "Total";

	Instructions[i++] = InstructionPair(epofiPrintCombinedTaxTotal, "Combined Tax Totals");
	DefaultCaption[epofiPrintCombinedTaxTotal] = "Combined Tax Totals";

	Instructions[i++] = InstructionPair(epofiPrintSeparatedTaxTotals, "Seperated Tax Totals");
	DefaultCaption[epofiPrintSeparatedTaxTotals] = "Seperated Tax Totals";

	Instructions[i++] = InstructionPair(epofiPrintTaxInclServiceChargeTaxTotal, "Combined Tax and Service Charge Tax");
	DefaultCaption[epofiPrintTaxInclServiceChargeTaxTotal] = "Combined Tax and Service Charge Tax";

	Instructions[i++] = InstructionPair(epofiPrintServiceCharge, "Service Charge");
	DefaultCaption[epofiPrintServiceCharge] = "Service Charge";

	//	Instructions[i++] = InstructionPair(epofiPrintServiceChargeExclTax, "Service Charge Excl Tax");
	//	DefaultCaption[epofiPrintServiceChargeExclTax] = "Service Charge Excl Tax";

	Instructions[i++] = InstructionPair(epofiPrintPaymentTotals, "Payment Type Totals");
	DefaultCaption[epofiPrintPaymentTotals] = "Payment Totals Totals";

	// Discount and Surcharges
	Instructions[i++] = InstructionPair(epofiPrintPaymentSurcharges, "Payment Surcharges");
	DefaultCaption[epofiPrintPaymentSurcharges] = "Payment Surcharges";

	Instructions[i++] = InstructionPair(epofiPrintTotalDiscount, "Discount");
	DefaultCaption[epofiPrintTotalDiscount] = "Discount";

	Instructions[i++] = InstructionPair(epofiPrintTotalDiscountsDetails, "Discounts");
	DefaultCaption[epofiPrintTotalDiscountsDetails] = "Discounts";

	Instructions[i++] = InstructionPair(epofiPrintTotalDiscountsName, "Discounts by Name");
	DefaultCaption[epofiPrintTotalDiscountsName] = "Discounts by Name";

	Instructions[i++] = InstructionPair(epofiPrintAccCreditInfo, "Acc Credit Info");
	DefaultCaption[epofiPrintAccCreditInfo] = "Acc Credit Info";

	Instructions[i++] = InstructionPair(epofiPrintExtraInfo, "Extra Info");
	DefaultCaption[epofiPrintExtraInfo] = "Extra Info";

	Instructions[i++] = InstructionPair(epofiPrintPaymentInfo, "Web Order Info");
	DefaultCaption[epofiPrintPaymentInfo] = "Web Order Info";

	Instructions[i++] = InstructionPair(epofiPrintDeliveryInfo, "Web Delivery Info");
	DefaultCaption[epofiPrintDeliveryInfo] = "Web Delivery Info";

	Instructions[i++] = InstructionPair(epofiPrintWebOrderComment, "Web Orders Comments");
	DefaultCaption[epofiPrintWebOrderComment] = "Web Order Comments";

	Instructions[i++] = InstructionPair(epofiPrintMsg, "Message");
	DefaultCaption[epofiPrintMsg] = "Message";

	//Instructions[i++] = InstructionPair(epofiFormatChefMate, "Format For Chefmate");
	//DefaultCaption[epofiFormatChefMate] = "Format For Chefmate";

	Instructions[i++] = InstructionPair(epofiPrintHistory, "Payment History");
	DefaultCaption[epofiPrintHistory] = "Payment History";

	Instructions[i++] = InstructionPair(epofiPrintAccChargeInfo, "Account Charge Info");
	DefaultCaption[epofiPrintAccChargeInfo] = "Account Charge Info";

	Instructions[i++] = InstructionPair(epofiOpenCashDrawer, "Open Cash Drawer");
	DefaultCaption[epofiOpenCashDrawer] = "Open Cash Drawer";

	Instructions[i++] = InstructionPair(epofiPrintProductTotalsWithUnitPrice, "Item Unit Price");
	DefaultCaption[epofiPrintProductTotalsWithUnitPrice] = "Item Unit Price";

	//Not sure where exactly this should be added to the list so I have just added it to the end although I would like a better place for it - ChrisJ
  //	Instructions[i++] = InstructionPair(epofiPrintCustomerName, "Customer Name");
  //	DefaultCaption[epofiPrintCustomerName] = "Customer Name";

	Instructions[i++] = InstructionPair(epofiHotelCustomerName, "Hotel Customer Name");
	DefaultCaption[epofiHotelCustomerName] = "Hotel Customer Name";

	Instructions[i++] = InstructionPair(epofiHotelRoomNumber, "Hotel Room Number");
	DefaultCaption[epofiHotelRoomNumber] = "Hotel Room Number";

	// SM Accredidation
	Instructions[i++] = InstructionPair(epofiPrintSalesTaxExempt, "Sales Tax Exempt");
	DefaultCaption[epofiPrintSalesTaxExempt] = "Sales Tax Exempt";

	Instructions[i++] = InstructionPair(epofiPrintSalesTaxable, "Sales Taxable");
	DefaultCaption[epofiPrintSalesTaxable] = "Sales Taxable";

	Instructions[i++] = InstructionPair(epofiPrintSalesTax, "Sales Tax");
	DefaultCaption[epofiPrintSalesTax] = "Sales Tax";

    Instructions[i++] = InstructionPair(epofiPrintProfitTax, "Profit Tax");
	DefaultCaption[epofiPrintProfitTax] = "Profit Tax";

	Instructions[i++] = InstructionPair(epofiPrintLocalTax, "Local Tax");
	DefaultCaption[epofiPrintLocalTax] = "Local Tax";

	Instructions[i++] = InstructionPair(epofiPrintServiceChargeTax, "Service Charge Tax");
	DefaultCaption[epofiPrintServiceChargeTax] = "Service Charge Tax";

	Instructions[i++] = InstructionPair(epofiPrintZeroRated, "Zero Rated Sales");
	DefaultCaption[epofiPrintZeroRated] = "Zero Rated Sales";

	Instructions[i++] = InstructionPair(epofiPrintServiceChargePlusServiceChargeTax, "Service Charge & Tax");
	DefaultCaption[epofiPrintServiceChargePlusServiceChargeTax] = "Service Charge & Tax";

	Instructions[i++] = InstructionPair(epofiPrintItemScales, "Item Scales");
	DefaultCaption[epofiPrintItemScales] = "Item Scales";

	Instructions[i++] = InstructionPair(epofiPrintCurrencySymbol, "Currency Symbol");
	DefaultCaption[epofiPrintCurrencySymbol] = "Currency Symbol";

	Instructions[i++] = InstructionPair(epofiPrintRedeemableWeight, "Redeemable Weight");
	DefaultCaption[epofiPrintRedeemableWeight] = "Redeemable Weight";
	//add.... 3956
	Instructions[i++] = InstructionPair(epofiPrintTwoLinesItemsName, "Items Two Lines");
	DefaultCaption[epofiPrintTwoLinesItemsName] = "Items Two Lines";

	// add two line item Items Information
	Instructions[i++] = InstructionPair(epofiPrintTwoLinesItemName, "Item Two Lines");
	DefaultCaption[epofiPrintTwoLinesItemName] = "Item Two Lines";

	//add two lines Item totals..  2204
	Instructions[i++] = InstructionPair(epofiPrintItemTwoLinesTotal, "Item Two Lines Total");
	DefaultCaption[epofiPrintItemTwoLinesTotal] = "Item Two Lines Total";

	Instructions[i++] = InstructionPair(epofiPrintItemsTwoLinesTotal, "Items Two Lines Total");
	DefaultCaption[epofiPrintItemsTwoLinesTotal] = "Items Two Lines Total";

    Instructions[i++] = InstructionPair(epofiPrintDeliveryTime, "Delivery Time");
	DefaultCaption[epofiPrintDeliveryTime] = "Delivery Time";

    Instructions[i++] = InstructionPair(epofiGroupItems, "Group Items");
	DefaultCaption[epofiGroupItems] = "Group Items";

    Instructions[i++] = InstructionPair(epofiCurrentYearPts, "Current Year Points");
	DefaultCaption[epofiCurrentYearPts] = "Current Year Points";

    Instructions[i++] = InstructionPair(epofiPrintReceiptVoidFooter, "Receipt Void Footer");
	DefaultCaption[epofiPrintReceiptVoidFooter] = "Receipt Void Footer";
}


UnicodeString TPrintOutFormatInstructions::GetCaption(ePrintOutFormatInstructions Instruction)
{
	return DefaultCaption[Instruction];
}

UnicodeString TPrintOutFormatInstructions::GetInstructionCaption(ePrintOutFormatInstructions Instruction)
{
	typeInstructions::iterator itInstructions = Instructions.begin();
	for (; itInstructions != Instructions.end(); itInstructions++)
	{
		if (itInstructions->second.first == Instruction)
		{
			return itInstructions->second.second;
		}
	}
	return "";
}

TPrintSection::TPrintSection(TPrintFormat *inpPrinter,
TList *WorkingOrders,
TSectionInstructStorage &inMasterInstructionsList,
TPSectionInstruction *itInstruction) :
MasterInstructionsList	(inMasterInstructionsList),
OrdersList(new TList),
WorkingOrdersList(new TList),
ParentsOrdersList(new TList),
ReceiptLength(7)
{
	pPrinter = inpPrinter;
	ParentsOrdersList->Assign(WorkingOrders);
	OrdersList->Assign(WorkingOrders);
	ThisInstruction = itInstruction;

	// Suppresses Lines Above and Below.
	Empty = false;
}

TPrintSection::TPrintSection(TPrintFormat *inpPrinter,
TList *WorkingOrders,
TSectionInstructStorage &inMasterInstructionsList,
TPSectionInstruction *itInstruction,
TDocketFormat &inFormat)
:
MasterInstructionsList	(inMasterInstructionsList),
OrdersList(new TList),
WorkingOrdersList(new TList),
ParentsOrdersList(new TList),
ReceiptLength(7)
{
	pPrinter = inpPrinter;
	ParentsOrdersList->Assign(WorkingOrders);
	OrdersList->Assign(WorkingOrders);
	ThisInstruction = itInstruction;
	Format = inFormat;
	// Suppresses Lines Above and Below.
	Empty = false;
}


TPrintSection::~TPrintSection()
{
}

void TPrintSection::Clear()
{
	OrdersList->Clear();
}

void TPrintSection::ProcessSection(TReqPrintJob *PrintJob)
{
	/* Ok we gota loop though each section that matches the current group number.
	If the group number changes and is Lower then we need to return to back to the parent */
	switch(ThisInstruction->Instruction)
	{
	case epofiBreak:
		{
			FormatSectionData(PrintJob);
			if (ThisInstruction->Cut)
			{
				pPrinter->PartialCut();
			}
		}break;
	case epofiPrintKitchenInfo:
	case epofiPrintTable:
		{
			SortByTableTab();
			if (OrdersList->Count > 0)
			{ // For each Table.
				while (ExtractTableList())
				{
					FormatAndProcess(PrintJob);
				}
			}
			else
			{ // There are no Tables move to Children.
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintSeat:
		{
			SortBySeat();
			while (ExtractSeatList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintServingCourse:
		{
			SortByServingCourse();
			while (ExtractServingCourseList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintCourse:
		{
			SortByCourse();
			while (ExtractCourseList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
		// These Print options do not require
		// items on the receipt to print.
	case epofiPrintAccCreditInfo:
	case epofiPrintExtraInfo:
	case epofiPrintPaymentInfo:
	case epofiPrintDeliveryInfo:
	case epofiPrintWebOrderComment:
	case epofiPrintMsg:
	case epofiPrintReceiptLogo:
	case epofiPrintSaleTypeDetail:
	case epofiPrintReceiptHeader:
	case epofiPrintReceiptFooter:
	case epofiPrintWaitTimes:
	case epofiPrintChitNumber:
	case epofiPrintPaymentTotals:
	case epofiPrintPaymentSurcharges:
	case epofiPrintSeparatedTaxTotals:
	case epofiPrintMembersName:
	case epofiPrintMembersDetails:
		//MM2038
	case epofiPickUpPrintCustomerName:
	case epofiPickUpPrintCustomerPhone:
	case epofiPrintAccChargeInfo:
	case epofiPrintPartyName:
	case epofiPrintPointsDetails:
	case epofiPrintItemNote:
	case epofiPrintItemSides:
	case epofiPrintItemOptions:
	case epofiPrintReceiptInfo:
	case epofiSessionDateInfo:
	case epofiPrintHistory:
	case epofiOpenCashDrawer:
   //	case epofiPrintCustomerName:
	case epofiHotelCustomerName:
	case epofiHotelRoomNumber:
	case epofiPrintCombinedTaxTotal:
	case epofiPrintTaxInclServiceChargeTaxTotal:
	case epofiPrintServiceCharge:
		// SM Accreditation
		// ------------------------
	case epofiPrintSalesTaxExempt:
	case epofiPrintSalesTaxable:
	case epofiPrintSalesTax:
	case epofiPrintProfitTax:
	case epofiPrintLocalTax:
	case epofiPrintServiceChargeTax:
	case epofiPrintServiceChargePlusServiceChargeTax:
	case epofiPrintZeroRated:
	case epofiPrintRedeemableWeight:
    case epofiCurrentYearPts:
    case epofiPrintReceiptVoidFooter:
        case epofiPrintDeliveryTime:
		{
			SortByItems();
			ExtractItemsList();
			FormatAndProcess(PrintJob);
		}break;
		// ------------------------
	case epofiPrintProductTotalsWithUnitPrice:
		SortByItems();
		while (ExtractItemsList())
		FormatAndProcess(PrintJob);
		break;
	case epofiPrintItem:
		//case epofiPrintTwoLinesItemName: // add two line
		{
			SortByItems();
			while (ExtractItem())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintTotal:
		{
			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintItems:
		// case epofiPrintTwoLinesItemsName: // add two lines..

	case epofiPrintTotalDiscount:
	case epofiPrintTotalEx:
	case epofiPrintTotalDiscountsDetails:
	case epofiPrintTotalDiscountsName:
		{


			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintItemsTotal:
	case epofiPrintItemScales:
		{
			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcessNoChildren(PrintJob);
			}
		}break;
	case epofiPrintSoldBy:
		{
			SortBySoldBy();
			while (ExtractSoldByList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintItemTotal:
		{
			SortByItems();
			while (ExtractItem())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiSortByFoodDrink:
		{
			SortByFoodDrink();
			while (ExtractFoodDrinkList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintBarCode:
		{
			while (ExtractItemsList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiFilterFood:
		{
			while (ExtractItemsList(eFoodItem))
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiFilterBev:
		{
			while (ExtractItemsList(eDrinksItem))
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiFilterOutCancels:
		{
			if (OrdersList->Count > 0)
			{
				while (ExtractCancelsList())
				{
					FormatAndProcess(PrintJob);
				}
			}
			else
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiFilterOutFree:
		{
			while (ExtractFreeList(PrintJob->JobType))
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiFormatChefMate:
		{
			// Modify the Printer properties so that it prints really wide.
			if (pPrinter != NULL)
			{
				pPrinter->NormalWidth = 90;
				pPrinter->BoldWidth = 90;
				pPrinter->WordWrap = true;
			}
			// Sort the Items.
			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiSortByDateOrdered:
		{
			// Sort the Items.
			SortByDate();
			while (ExtractDateList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;
	case epofiPrintLoyaltyReceiptHeader:
	case epofiPrintLoyaltyReceiptMemberNumber:
	case epofiPrintLoyaltyReceiptMemberName:
	case epofiPrintLoyaltyReceiptActivationCode:
	case epofiPrintLoyaltyReceiptActivationInstructions:
	case epofiPrintCurrencySymbol:
		{
			FormatAndProcessNoChildren(PrintJob);
		}break;
	case epofiPrintTwoLinesItemName:
		{
			SortByItems();
			while (ExtractItem())
			{
				FormatAndProcess(PrintJob);
			}
		}break;

	case epofiPrintTwoLinesItemsName:
		{
			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcess(PrintJob);
			}
		}break;

	case epofiPrintItemTwoLinesTotal:
		{
			SortByItems();
			while (ExtractItem())
			{
				FormatAndProcess(PrintJob);
			}
		}break;

	case epofiPrintItemsTwoLinesTotal:
		{
			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcessNoChildren(PrintJob);
			}
		}break;
    case epofiGroupItems:
        {
			SortByItems();
			while (ExtractItemsList())
			{
				FormatAndProcessNoChildren(PrintJob);
			}
		}break;
	}
}

void TPrintSection::FormatAndProcess(TReqPrintJob *PrintJob)
{
	FormatSectionData(PrintJob);
	// Loop though all Sub Instructions
	ProcessChildren(PrintJob, pPrinter, WorkingOrdersList.get());
	WorkingOrdersList->Clear();
	if (ThisInstruction->Cut)
	{
		pPrinter->PartialCut();
	}
}

void TPrintSection::FormatAndProcessNoChildren(TReqPrintJob *PrintJob)
{
	FormatSectionData(PrintJob);
	WorkingOrdersList->Clear();
	if (ThisInstruction->Cut)
	{
		pPrinter->PartialCut();
	}
}

void TPrintSection::ProcessChildren(TReqPrintJob *PrintJob, TPrintFormat *inpPrinter, TList *WorkingOrders)
{
	// Container Iterator...
	// TSectionInstructStorage::iterator
	TPtrSectionInstructStorage::iterator ptrSubInstruction = ThisInstruction->SubInstructions.begin();
	while (ptrSubInstruction != ThisInstruction->SubInstructions.end())
	{
		TPSectionInstruction *itInstruction = *ptrSubInstruction;
		// Get The Iterator that iterator points too.
		TPrintSection NewSection(pPrinter, WorkingOrdersList.get(), MasterInstructionsList, itInstruction,Format);
		NewSection.ProcessSection(PrintJob);
		advance(ptrSubInstruction, 1);
	}
}

void TPrintSection::FormatSectionData(TReqPrintJob *PrintJob)
{
	// Used to Undo any Lines if the section is empty.
	pPrinter->BookMark();

	// Space out each section by a line.
	if (ThisInstruction->LineAbove)
	{
		pPrinter->NewLine();
	}

	if (ThisInstruction->DrawLineAbove)
	{
		pPrinter->DrawLine();
	}

	if (ThisInstruction->Visible)
	{
		switch(ThisInstruction->Instruction)
		{
		case epofiPrintTable:
			PrintTable(PrintJob);
			break;
		case epofiPrintKitchenInfo:
			PrintKitchenInfo(PrintJob);
			break;
		case epofiPrintReceiptInfo:
			PrintReceiptInfo(PrintJob);
			break;
		case epofiSessionDateInfo:
			PrintSessionDate(PrintJob);
			break;
		case epofiPrintSeat:
			PrintSeat(PrintJob);
			break;
		case epofiPrintServingCourse:
			PrintServingCourse(PrintJob);
			break;
		case epofiPrintCourse:
			PrintCourse(PrintJob);
			break;
		case epofiPrintItemNote:
			PrintItemNote(PrintJob);
			break;
		case epofiPrintItemSides:
			PrintItemSides(PrintJob);
			break;
		case epofiPrintItemOptions:
			PrintItemOptions(PrintJob);
			break;
		case epofiPrintItem:
			PrintItem(PrintJob);
			break;
		case epofiPrintItems:
			PrintItemRavelled(PrintJob);
			break;
		case epofiFilterFood:
		case epofiFilterBev:
		case epofiSortByFoodDrink:
			PrintFoodDrink(PrintJob);
			break;
		case epofiPrintBarCode:
			PrintBarCode(PrintJob);
			break;
		case epofiFormatChefMate:
			PrintChefMateHorizontal(PrintJob);
			break;
		case epofiPrintMembersName:
			PrintMemberNameInformation(PrintJob);
			break;
		case epofiPrintMembersDetails:
			PrintMemberDetailsInformation(PrintJob);
			break;
		case epofiPickUpPrintCustomerName:
			PrintCustomerNameInformation(PrintJob);
			break;
		case epofiPickUpPrintCustomerPhone:
			PrintCustomerPhoneInformation(PrintJob);
			break;
		case epofiPrintAccChargeInfo:
			PrintInvoiceAccountInformation(PrintJob);
			break;
		case epofiPrintPartyName:
			PrintPartyInformation(PrintJob);
			break;
		case epofiSortByDateOrdered:
			PrintDateOrdered(PrintJob);
			break;
		case epofiPrintPointsDetails:
			PrintPointsInformation(PrintJob);
			break;
		case epofiCurrentYearPts:
			PrintCurrentYearPts(PrintJob);
			break;
		case epofiPrintItemTotal:
			PrintItemTotal(PrintJob);
			break;
		case epofiPrintItemsTotal:
	  		PrintItemsTotal(PrintJob);
			break;
		case epofiPrintTotalDiscount:
			PrintTotalDicount(PrintJob);
			break;
		case epofiPrintTotalEx:
			PrintTotalEx(PrintJob);
			break;
		case epofiPrintTotal:
			PrintGrandTotal(PrintJob);
			break;
		case epofiPrintSeparatedTaxTotals:
			PrintSeparatedTaxTotals(PrintJob);
			//PrintGSTTotal(PrintJob);
			break;
		case epofiPrintTotalDiscountsDetails:
			PrintTotalDicountDetails(PrintJob);
			break;
		case epofiPrintTotalDiscountsName:
			if(PrintJob->Transaction->TypeOfSale!= NonChargableSale)
			{
				PrintTotalDicountsName(PrintJob);
			}
			break;
		case epofiPrintMsg:
			PrintMsg(PrintJob);
			break;
		case epofiPrintSoldBy:
			PrintSoldBy(PrintJob);
			break;
		case epofiPrintChitNumber:
			PrintChitNumber(PrintJob);
			break;
		case epofiPrintReceiptLogo:
			PrintReceiptLogo(PrintJob);
			break;
		case epofiPrintSaleTypeDetail:
			PrintSaleTypeDetails(PrintJob);
			break;
		case epofiPrintReceiptHeader:
			PrintReceiptHeader(PrintJob);
			break;
		case epofiPrintReceiptFooter:
			PrintReceiptFooter(PrintJob);
			break;
		case epofiPrintPaymentTotals:
			PrintPaymentTotals(PrintJob);
			break;
		case epofiPrintPaymentSurcharges:
			PrintPaymentSurcharges(PrintJob);
			break;
		case epofiPrintWaitTimes:
			PrintWaitTimes(PrintJob);
			break;
		case epofiPrintAccCreditInfo:
			PrintAccountCreditInfo(PrintJob);
			break;
		case epofiPrintExtraInfo:
			PrintExtraInfo(PrintJob);
			break;
		case epofiPrintPaymentInfo:
			PrintPaymentInfo(PrintJob);
			break;
		case epofiPrintDeliveryInfo:
			PrintDeliveryInfo(PrintJob);
			break;
		case epofiPrintHistory:
			PrintPaymentHistory(PrintJob);
			break;
		case epofiPrintWebOrderComment:
			PrintWebComments(PrintJob);
			break;
		case epofiOpenCashDrawer:
			OpenCashDrawer(PrintJob);
			break;
		case epofiPrintProductTotalsWithUnitPrice:
			PrintProductTotalsWithUnitPrice(PrintJob);
			break;
	   //	case epofiPrintCustomerName:
	   //		PrintCustomerName(PrintJob);
	   //		break;
		case epofiHotelCustomerName:
			PrintHotelCustomerName(PrintJob);
			break;
		case epofiHotelRoomNumber:
			PrintHotelRoomNumber(PrintJob);
			break;
		case epofiPrintCombinedTaxTotal:
			PrintCombinedTaxTotal(PrintJob);
			break;
		case epofiPrintTaxInclServiceChargeTaxTotal:
			PrintTaxInclServiceChargeTaxTotal(PrintJob);
			break;
		case epofiPrintServiceCharge:
			PrintServiceCharge(PrintJob);
			break;
		case epofiPrintLoyaltyReceiptHeader:
			PrintLoyaltyReceiptHeader(PrintJob);
			break;
		case epofiPrintLoyaltyReceiptMemberNumber:
			PrintLoyaltyReceiptMemberNumber(PrintJob);
			break;
		case epofiPrintLoyaltyReceiptMemberName:
			PrintLoyaltyReceiptMemberName(PrintJob);
			break;
		case epofiPrintLoyaltyReceiptActivationCode:
			PrintLoyaltyReceiptActivationCode(PrintJob);
			break;
		case epofiPrintLoyaltyReceiptActivationInstructions:
			PrintLoyaltyReceiptActivationInstructions(PrintJob);
			break;
		case epofiPrintSalesTaxExempt:
			PrintSalesTaxExempt(PrintJob);
			break;
		case epofiPrintSalesTaxable:
			PrintSalesTaxable(PrintJob);
			break;
		case epofiPrintSalesTax:
			PrintSalesTax(PrintJob);
			break;
		case epofiPrintProfitTax:
			PrintProfitTax(PrintJob);
			break;
		case epofiPrintLocalTax:
			PrintLocalTax(PrintJob);
			break;
		case epofiPrintServiceChargeTax:
			PrintServiceChargeTax(PrintJob);
			break;
		case epofiPrintServiceChargePlusServiceChargeTax:
			PrintServiceChargePlusServiceChargeTax(PrintJob);
			break;
		case epofiPrintZeroRated:
			PrintZeroRated(PrintJob);
			break;
		case epofiPrintItemScales:
			printItemScales( PrintJob );
			break;
		case epofiPrintCurrencySymbol:
			printCurrencySymbol( PrintJob );
			break;
		case epofiPrintRedeemableWeight:
			PrintAvailableWeight( PrintJob );
			break;
			//....                  3956
		case epofiPrintTwoLinesItemsName:
			PrintTwoLinesItemsName(PrintJob);
			break;
			//...
		case epofiPrintTwoLinesItemName:
			PrintTwoLinesItemName(PrintJob);
			break;
		case epofiPrintItemTwoLinesTotal: // 2024
			PrintTwoLinesItemTotal(PrintJob);
			break;

		case epofiPrintItemsTwoLinesTotal: // 2024
			PrintTwoLinesItemsTotal(PrintJob);
			break;
        case epofiPrintDeliveryTime:
            PrintDeliveryTime(PrintJob);
            break;
        case epofiGroupItems:
			PrintItemRavelled(PrintJob, true);
			break;
        case epofiPrintReceiptVoidFooter:
			PrintReceiptFooterSecond(PrintJob);
			break;
		default:
			break;
		}
	}

	// If the print Job is empty undo all the lines above and below.
	if (Empty)
	{
		pPrinter->Undo();
	}
	else
	{
		if (ThisInstruction->DrawLineBelow)
		{
			pPrinter->DrawLine();
		}

		if (ThisInstruction->LineBelow)
		{
			pPrinter->NewLine();
		}
	}
}

void
TPrintSection::PrintProductTotalsWithUnitPrice(TReqPrintJob *pj)
{
	/* This is used to translate between types without excessive casting. */
	union {
		void *v;
		TItemComplete *i;
	} order_un;

	/* We're collapsing all the items into a single "line item." That is to say
	* that four items of the same type become a single item with a quantity of
	* four. This has to be done (again...) because of how the Billing screen
	* lists orders within a tab.
	*
	* "items" stores a single instance of a given item; items with the same
	* PLU are considered to be identical and will be "collapsed" into a
	* single line-item.
	*
	* "item_qtys" is for convenience: it allows us to index into it by an
	* item's key and update the quantity stored. If the quantity stored is zero
	* we know that it's the first time that the given item has been seen and
	* that the TItemComplete structure describing that item must be inserted
	* into the "items" list.
	*/
	std::list<TItemComplete *>  items;
	std::map<unsigned int, Currency> item_qtys;
	int i = 0;
	int j = WorkingOrdersList->Count;
	std::list<TItemComplete *>::const_iterator m;
	std::list<TItemComplete *>::const_iterator n;

	for ( ; i < j; i++) {
		order_un.v = WorkingOrdersList->Items[i];
		if (!(double)item_qtys[order_un.i->PLU])
		items.push_back(order_un.i);
		item_qtys[order_un.i->PLU] +=
		order_un.i->GetQty();
	}

	for (m = items.begin(), n = items.end(); m != n; m++) {
		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
        int length = GetItemLength((*m)->ItemKitchenName, (*m)->ItemKitchenName.Length());
		pPrinter->Line->Columns[1]->Width = pPrinter->Width / 3;
        if(length > 0)
        {
           pPrinter->Line->Columns[0]->Width =
           pPrinter->Width - pPrinter->Line->Columns[1]->Width - length;
        }
        else
        {
            pPrinter->Line->Columns[0]->Width =
            pPrinter->Width - pPrinter->Line->Columns[1]->Width;
        }

        if((*m)->OrderType == CanceledOrder)
        {
            pPrinter->Line->Columns[0]->Text = "!!!! CANCEL ORDERS !!!!";
            pPrinter->Line->Columns[1]->Text = "";
            pPrinter->AddLine();
        }
		pPrinter->Line->Columns[0]->Text = (*m)->ItemKitchenName;
		pPrinter->Line->Columns[1]->Text =
		FormatFloat("$0.00", item_qtys[(*m)->PLU] * (*m)->PriceEach());
		pPrinter->AddLine();

		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Text =
		FormatFloat("    0.00 * ", item_qtys[(*m)->PLU])
		+ FormatFloat("0.00", (*m)->PriceEach());
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintHotelCustomerName(TReqPrintJob *PrintJob)
{   /************MM-5048*****Add line to get updated value**********************/
  TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.begin())
	{
		advance(i, -1);
		if (i->Instruction == epofiHotelCustomerName )
		{
			break;
		}
	}

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;
/************MM-5048***************************/


	AnsiString CustomerName = PrintJob->Transaction->Customer.Name;
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taCenter;
    pPrinter->Line->Columns[0]->Text =ItemName+ ": "  + CustomerName;
	pPrinter->AddLine();


}

void TPrintSection::PrintHotelRoomNumber(TReqPrintJob *PrintJob)
{
    /************MM-5048*****Add line to get updated value**********************/
  TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.begin())
	{
        advance(i, -1);
		if (i->Instruction == epofiHotelRoomNumber )
		{
			break;
		}
	}

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;
/************MM-5048***************************/

	if( PrintJob->Transaction->Customer.RoomNumber != 0 )
	{
		AnsiString RoomNumber= PrintJob->Transaction->Customer.RoomNumber;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taCenter;
	    pPrinter->Line->Columns[0]->Text =ItemName+ ": "  + RoomNumber;
		pPrinter->AddLine();
	}
}

int __fastcall SortTableTab(void *Item1, void *Item2) // TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->ContainerTabType == Order2->ContainerTabType)
	{
		if (Order1->ContainerTabType == TabTableSeat)
		{
			if (Order1->TableNo < Order2->TableNo)
			{
				return -1;
			}
			else if (Order1->TableNo == Order2->TableNo)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			if (Order1->TabKey < Order2->TabKey)
			{
				return -1;
			}
			else if (Order1->TabKey == Order2->TabKey)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else
	{
		if (Order1->ContainerTabType < Order2->ContainerTabType)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}

}

void TPrintSection::SortByTableTab()
{
	OrdersList->Sort(SortTableTab);
}

int __fastcall SortSeat(void *Item1, void *Item2) // TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->SeatNo < Order2->SeatNo)
	{
		return -1;
	}
	else if (Order1->SeatNo == Order2->SeatNo)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortBySeat()
{
	OrdersList->Sort(SortSeat);
}

int __fastcall SortServingCourse(void *Item1, void *Item2)
// TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->ServingCourse.SCOO < Order2->ServingCourse.SCOO)
	{
		return -1;
	}
	else if (Order1->ServingCourse.SCOO == Order2->ServingCourse.SCOO)
	{
		if (Order1->ServingCourse.ServingCourseKey < Order2->ServingCourse.ServingCourseKey)
		{
			return -1;
		}
		else if (Order1->ServingCourse.ServingCourseKey == Order2->ServingCourse.ServingCourseKey)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortByServingCourse()
{
	OrdersList->Sort(SortServingCourse);
}

int __fastcall SortCourse(void *Item1, void *Item2) // TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->CoursePrintingOrder < Order2->CoursePrintingOrder)
	{
		return -1;
	}
	else if (Order1->CoursePrintingOrder == Order2->CoursePrintingOrder)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortByCourse()
{
	OrdersList->Sort(SortCourse);
}

int __fastcall SortItems(void *Item1, void *Item2) // TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->ItemAppearanceOrder < Order2->ItemAppearanceOrder)
	{
		return -1;
	}
	else if (Order1->ItemAppearanceOrder == Order2->ItemAppearanceOrder)
	{
		if (Order1->ItemKitchenName < Order2->ItemKitchenName)
		{
			return -1;
		}
		else if (Order1->ItemKitchenName == Order2->ItemKitchenName)
		{
			if (Order1->Size < Order2->Size)
			{
				return -1;
			}
			else if (Order1->Size == Order2->Size)
			{
				// Match Options
				int OptCompVal = OptionsCompare(Order1->OptionsSelected, Order2->OptionsSelected);
				if (OptCompVal != 0)
				{
					return OptCompVal;
				}
				else
				{
					int SubOrdersRetval = SubOrdersCompare(Order1->SubOrders, Order2->SubOrders);
					if (SubOrdersRetval != 0)
					{
						return SubOrdersRetval;
					}
					else
					{
						// You need to sort by seat No here to stop duplicate seat no
						// appearing in the kitchen i.e [1,2,1].
						if (Order1->Note < Order2->Note)
						{
							return -1;
						}
						else if (Order1->Note == Order2->Note)
						{
							if (Order1->SeatNo < Order2->SeatNo)
							{
								return -1;
							}
							else if (Order1->SeatNo == Order2->SeatNo)
							{
								return 0;
							}
							else
							{
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortByItems()
{
	OrdersList->Sort(SortItems);
}

int __fastcall SortSoldBy(void *Item1, void *Item2)
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	TSecurityReference *Sec1 = Order1->Security->SecurityGetType(secOrderedBy);
	TSecurityReference *Sec2 = Order2->Security->SecurityGetType(secOrderedBy);

	if (Sec1 != NULL && Sec2 == NULL)
	{
		return -1;
	}
	if (Sec1 == NULL && Sec2 == NULL)
	{
		return 0;
	}
	else if (Sec1 != NULL && Sec2 != NULL)
	{
		if (Sec1->From < Sec2->From)
		{
			return -1;
		}
		else if (Sec1->From == Sec2->From)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortBySoldBy()
{
	OrdersList->Sort(SortSoldBy);
}

int __fastcall SortDate(void *Item1, void *Item2) // TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (DateOf(Order1->TimeStamp) < DateOf(Order2->TimeStamp))
	{
		return -1;
	}
	else if (DateOf(Order1->TimeStamp) == DateOf(Order2->TimeStamp))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortByDate()
{
	OrdersList->Sort(SortDate);
}

int __fastcall SortFoodDrink(void *Item1, void *Item2)
// TKitchens Sort Function.
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->ItemType < Order2->ItemType)
	{
		return -1;
	}
	else if (Order1->ItemType == Order2->ItemType)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void TPrintSection::SortByFoodDrink()
{
	OrdersList->Sort(SortFoodDrink);
}

void TPrintSection::PrintKitchenInfo(TReqPrintJob *PrintJob)
{
	std::set<long>SeatCounter;
	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[i];
		if (Order->SeatNo > 0)
		{
			SeatCounter.insert(Order->SeatNo);
		}
	}

	int SeatCount = SeatCounter.size();
	if (SeatCount == 0)
	SeatCount = 1;

	int PatronCount = 0;
	if (PrintJob->Transaction != NULL)
	{
		PatronCount = TManagerPatron::Instance().GetTotalPatrons(PrintJob->Transaction->Patrons);
	}

	UnicodeString CurrentSeatCount = "";
	if (PatronCount == 0)
	{
		CurrentSeatCount = TGlobalSettings::Instance().SeatLabel + " Count :" + IntToStr(SeatCount);
	}
	else
	{
		CurrentSeatCount = "Patron Count :" + IntToStr(PatronCount);
	}
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - (CurrentSeatCount.Length() + 1);
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Width = CurrentSeatCount.Length() + 1;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Text = PrintJob->Waiter;
	pPrinter->Line->Columns[1]->Text = CurrentSeatCount;
	pPrinter->AddLine();

	UnicodeString TimeString = PrintJob->TimeStamp.FormatString("DD/MM/YYYY hh:nn am/pm");
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = TimeString;
	pPrinter->AddLine();

	pPrinter->Line->ColCount = 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = "Docket No. " + PrintJob->DocketNumber;
	pPrinter->AddLine();
}

void TPrintSection::PrintReceiptInfo(TReqPrintJob *PrintJob)
{
	// this will cause the receipt details section to be printed only once.
	if( PrintJob->ReceiptDetails->Count > 0 )
	{
		Empty = true;
		return;
	}

	std::set<long>SeatCounter;
	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[i];
		if (Order->SeatNo > 0)
		{
			SeatCounter.insert(Order->SeatNo);
		}
	}

	int SeatCount = SeatCounter.size();
	if (SeatCount == 0)
	SeatCount = 1;

	int PatronCount = 0;
	if (PrintJob->Transaction != NULL)
	{
		PatronCount = TManagerPatron::Instance().GetTotalPatrons(PrintJob->Transaction->Patrons);
	}

	UnicodeString CurrentSeatCount = "";
	if (PatronCount == 0)
	{
		CurrentSeatCount = TGlobalSettings::Instance().SeatLabel + " Count :" + IntToStr(SeatCount);
	}
	else
	{
		CurrentSeatCount = "Patron Count :" + IntToStr(PatronCount);
	}

	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	AnsiString TerminalName = PrintJob->Header.RequestingDevice->ID.Name;
	pPrinter->Line->Columns[0]->Width = TerminalName.Length() + 1;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Width = pPrinter->Width - (TerminalName.Length() + 1);
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Text = TerminalName;
	pPrinter->Line->Columns[1]->Text = PrintJob->Header.RequestingDevice->ID.ComputerName;
	pPrinter->AddLine();

	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - (CurrentSeatCount.Length() + 1);
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Width = CurrentSeatCount.Length() + 1;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Text = PrintJob->Waiter;
	pPrinter->Line->Columns[1]->Text = CurrentSeatCount;
	pPrinter->AddLine();

	UnicodeString TimeString = PrintJob->TimeStamp.FormatString("DD/MM/YYYY hh:nn am/pm");
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = TimeString;
	pPrinter->AddLine();

    ShowRefundReference(PrintJob) ;

	// populate ReceiptDetails list
	PrintJob->ReceiptDetails->Add( TerminalName + " " + PrintJob->Header.RequestingDevice->ID.ComputerName );
	PrintJob->ReceiptDetails->Add( PrintJob->Waiter + " " + CurrentSeatCount );
	PrintJob->ReceiptDetails->Add( TimeString );
}
//---------------------------------------------------------------------------
void TPrintSection::ShowRefundReference(TReqPrintJob *PrintJob)
{
    if(TReceiptUtility::CheckRefundCancelTransaction(*PrintJob->Transaction)
          && TGlobalSettings::Instance().CaptureRefundRefNo)
    {
        pPrinter->Line->ColCount = 1;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->Columns[0]->Alignment = taCenter;
        pPrinter->Line->Columns[0]->Text = "OR No. ";
        pPrinter->Line->Columns[0]->Text += TReceiptUtility::ModifyInvoiceNumber(PrintJob->Transaction->RefundRefReceipt,
                                                   ReceiptLength);
        pPrinter->AddLine();
    }
}
//---------------------------------------------------------------------------
void TPrintSection::PrintSessionDate(TReqPrintJob *PrintJob)
{
	UnicodeString CurrentDate = Date();

	SYSTEMTIME SystemTime;
	::GetLocalTime(&SystemTime);

	if(TGlobalSettings::Instance().EndOfDay > 0 && SystemTime.wHour < TGlobalSettings::Instance().EndOfDay )
	{
		TDateTime SessionDate(SystemTime.wYear, SystemTime.wMonth, (SystemTime.wDay - 1));
		CurrentDate = SessionDate;
	}
	else
	{
		CurrentDate =  Date();
	}

	TDateTime SessionDate = CurrentDate;
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = "Session Date: " + SessionDate.FormatString("dd/mm/yyyy");;
	pPrinter->AddLine();
}

void TPrintSection::PrintTable(TReqPrintJob *PrintJob)
{
	AnsiString HeaderName;
    UnicodeString temp_data;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		HeaderName = GetHeaderName(Order);
	}

	pPrinter->Line->ColCount = 1;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taCenter;

	if (HeaderName != "CASH")
	{
        if(HeaderName.Length() > 12)
        {
            temp_data = HeaderName.SubString(0, 12);
            if(temp_data == "Online Order")
            {
                pPrinter->Line->Columns[0]->Text = temp_data;
                pPrinter->AddLine();
                temp_data = HeaderName.SubString(13, HeaderName.Length());
                pPrinter->Line->Columns[0]->Text = temp_data;
                pPrinter->AddLine();

            }
            else
            {
                pPrinter->Line->Columns[0]->Text = HeaderName;
                pPrinter->AddLine();
            }
        }
        else
        {
            pPrinter->Line->Columns[0]->Text = HeaderName;
            pPrinter->AddLine();
        }
	}
	else
	{

        if (WorkingOrdersList->Count > 0)
        {
            TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
            AnsiString receiptNo = Order->TabName;
            AnsiString receiptPartial = Order->IdName;
            if (receiptNo.SubString(1,7) == receiptPartial.SubString(1,7))
            {
               receiptNo = receiptPartial;
            }
            AnsiString tableNo = "";

            if (Order->TabType == TabDelayedPayment)
            {
                Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction.StartTransaction();
                TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
                IBInternalQuery->Close();

                IBInternalQuery->SQL->Text =
                    " SELECT "
                    " FROM_VAL "
                    " FROM "
                    "  SECURITY "
                    " WHERE "
                    "  TO_VAL = :TO_VAL ";
                IBInternalQuery->ParamByName("TO_VAL")->AsString = receiptNo;
                IBInternalQuery->ExecQuery();
                tableNo = IBInternalQuery->FieldByName("FROM_VAL")->AsString;
                IBInternalQuery->Close();
                DBTransaction.Commit();
                pPrinter->Line->Columns[0]->Text    =   tableNo;
                pPrinter->AddLine();
            }
            else
            {
                Empty = true;
            }
        }
	}
}

void TPrintSection::PrintSeat(TReqPrintJob *PrintJob)
{

	UnicodeString SeatName;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		if (Order->TabName != "")
		{
			SeatName = Order->TabName;
		}
		else if (Order->SeatNo > 0)
		{
			SeatName = TGlobalSettings::Instance().SeatLabel + " " + IntToStr(Order->SeatNo);
		}
    }

	if (SeatName == "")
	{
		Empty = true;
	}
	else
	{
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taCenter;

		pPrinter->Line->Columns[0]->Text = SeatName;
		pPrinter->AddLine();
    }
}

void TPrintSection::PrintServingCourse(TReqPrintJob *PrintJob)
{
	UnicodeString ServingCourseName;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		ServingCourseName = Order->ServingCourse.KitchenName;
	}

	if (ServingCourseName == "")
	{
		Empty = true;
	}
	else
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = ServingCourseName;
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintMemberDetailsInformation(TReqPrintJob *PrintJob)
{
	if (PrintJob->Transaction->Membership.Member.ContactKey != 0)
	{
		UnicodeString Member = PrintJob->Transaction->Membership.Member.Alias + " ( " + PrintJob->Transaction->Membership.Member.MembershipNumber + " ) ";
		switch(PrintJob->MembershipNameDisplay)
		{
		case 0: // Known As / Full Name
			if (PrintJob->Transaction->Membership.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			else
			{
				if (PrintJob->Transaction->Membership.Member.Title != "")
				{
					Member = PrintJob->Transaction->Membership.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
				}
				else
				{
					Member = PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
				}
			}
			break;
		case 1: // 1 Full Name / Known As
			if (PrintJob->Transaction->Membership.Member.Name != "")
			{
				if (PrintJob->Transaction->Membership.Member.Title != "")
				{
					Member = PrintJob->Transaction->Membership.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
				}
				else
				{
					Member = PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
				}
			}
			else
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			break;
		case 2: // Full Name
			if (PrintJob->Transaction->Membership.Member.Name != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
			}
			break;
		case 3: // Known As
			if (PrintJob->Transaction->Membership.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			break;
		default:
			if (PrintJob->Transaction->Membership.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			else
			{
				if (PrintJob->Transaction->Membership.Member.Title != "")
				{
					Member = PrintJob->Transaction->Membership.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
				}
				else
				{
					Member = PrintJob->Transaction->Membership.Member.Name+ " " + PrintJob->Transaction->Membership.Member.Surname;
				}
			}
		}

        pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - Member.Length();
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = "Membership";
		pPrinter->Line->Columns[1]->Width = Member.Length();
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
		pPrinter->Line->Columns[1]->Text = Member;
		pPrinter->AddLine();
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		if (PrintJob->Transaction->Membership.Member.Phone != "")
		{
			pPrinter->Line->Columns[0]->Text = PrintJob->Transaction->Membership.Member.Phone;
			pPrinter->AddLine();
		}

		if (PrintJob->Transaction->Membership.Member.Mobile != "")
		{
			pPrinter->Line->Columns[0]->Text = PrintJob->Transaction->Membership.Member.Mobile;
			pPrinter->AddLine();
		}

		if (PrintJob->Transaction->Membership.Member.LocationAddress != "")
		{
		   	std::auto_ptr<TStringList>TempList(new TStringList);
            TempList->Text = PrintJob->Transaction->Membership.Member.LocationAddress;

            pPrinter->Line->Columns[0]->Text = TempList->Text;
            if(TempList->Text.Length() > pPrinter->Width)
            {
                int start_Index, last_Index = pPrinter->Width - 1;

                for(start_Index = 0; start_Index < TempList->Text.Length()-1; start_Index +=pPrinter->Width, last_Index +=pPrinter->Width)
                {
                    pPrinter->Line->Columns[0]->Text = TempList->Text.SubString(start_Index,last_Index);
                    pPrinter->AddLine();
                }
            }
            else
            {
                pPrinter->AddLine();
            }
        }
	}
	else
	{
		Empty = true;
	}
}

//MM2038
void TPrintSection::PrintCustomerNameInformation(TReqPrintJob *PrintJob)
{

  TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.begin())
	{

		advance(i, -1);
		if (i->Instruction == epofiPickUpPrintCustomerName )
		{
			break;
		}
	}

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;


	UnicodeString SeatName,TempName;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		//TempName = "FadilTest";
		SeatName = Order->IdName;
	}

	if (SeatName == "")
	{
		Empty = true;
	}
	else
	{
    if(SeatName!=" " &&SeatName!=NULL)
    {
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;

	   //	pPrinter->Line->Columns[0]->Text = "Customer Name : " + SeatName;
     pPrinter->Line->Columns[0]->Text = ItemName+ ": " + SeatName;
		pPrinter->AddLine();
    }
	}
}

void TPrintSection::PrintCustomerPhoneInformation(TReqPrintJob *PrintJob)
{

      TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.begin())
	{

		advance(i, -1);
		if (i->Instruction == epofiPickUpPrintCustomerPhone )
		{
			break;
		}
	}

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;
	UnicodeString PhoneNumber,TempName;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		PhoneNumber = Order->IdNumber;
	}

	if (PhoneNumber == "")
	{
		Empty = true;
	}
	else
	{
      if(PhoneNumber!=" "&&PhoneNumber!=NULL)
    {
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;

	   //	pPrinter->Line->Columns[0]->Text = "Customer Phone Number : " + PhoneNumber;
       	pPrinter->Line->Columns[0]->Text = ItemName+" : " + PhoneNumber;

		pPrinter->AddLine();
    }
	}
}

void TPrintSection::PrintInvoiceAccountInformation(TReqPrintJob *PrintJob)
{
	if (PrintJob->Transaction->Account.Member.ContactKey != 0)
	{
		UnicodeString Member = PrintJob->Transaction->Account.Member.Alias + " ( " + PrintJob->Transaction->Account.Member.MembershipNumber + " ) ";
		switch(PrintJob->MembershipNameDisplay)
		{
		case 0: // Known As / Full Name
			if (PrintJob->Transaction->Account.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Account.Member.Alias;
			}
			else
			{
				if (PrintJob->Transaction->Account.Member.Title != "")
				{
					Member = PrintJob->Transaction->Account.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name;
				}
				else
				{
					Member = PrintJob->Transaction->Account.Member.Name;
				}
			}
			break;
		case 1: // 1 Full Name / Known As
			if (PrintJob->Transaction->Account.Member.Name != "")
			{
				if (PrintJob->Transaction->Account.Member.Title != "")
				{
					Member = PrintJob->Transaction->Account.Member.Title + " " + PrintJob->Transaction->Account.Member.Name;
				}
				else
				{
					Member = PrintJob->Transaction->Account.Member.Name;
				}
			}
			else
			{
				Member = PrintJob->Transaction->Account.Member.Alias;
			}
			break;
		case 2: // Full Name
			if (PrintJob->Transaction->Account.Member.Name != "")
			{
				Member = PrintJob->Transaction->Account.Member.Name;
			}
			break;
		case 3: // Known As
			if (PrintJob->Transaction->Account.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Account.Member.Alias;
			}
			break;
		default:
			if (PrintJob->Transaction->Account.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Account.Member.Alias;
			}
			else
			{
				if (PrintJob->Transaction->Account.Member.Title != "")
				{
					Member = PrintJob->Transaction->Account.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name;
				}
				else
				{
					Member = PrintJob->Transaction->Account.Member.Name;
				}
			}
		}

		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - Member.Length();
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;

		pPrinter->Line->Columns[0]->Width = ThisInstruction->Caption.Length() + 1;
		pPrinter->Line->Columns[1]->Width = pPrinter->Width - ThisInstruction->Caption.Length() - 1;
		pPrinter->Line->Columns[0]->Text = ThisInstruction->Caption;
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
		pPrinter->Line->Columns[1]->Text = Member;
		pPrinter->AddLine();

		if (PrintJob->Transaction->InvoiceNumber != "")
		{
			AnsiString InvCaption = "Invoice Number : ";
			pPrinter->Line->Columns[0]->Width = InvCaption.Length() + 1;
			pPrinter->Line->Columns[1]->Width = pPrinter->Width - InvCaption.Length() - 1;
			pPrinter->Line->Columns[0]->Text = InvCaption;
			pPrinter->Line->Columns[1]->Text = PrintJob->AccountInvoiceNumber;
			pPrinter->AddLine();
		}

		pPrinter->NewLine();
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;

		if (PrintJob->Transaction->Membership.Member.Phone != "")
		{
			pPrinter->Line->Columns[0]->Text = PrintJob->Transaction->Account.Member.Phone;
			pPrinter->AddLine();
		}

		if (PrintJob->Transaction->Account.Member.Mobile != "")
		{
			pPrinter->Line->Columns[0]->Text = PrintJob->Transaction->Account.Member.Mobile;
			pPrinter->AddLine();
		}

		if (PrintJob->Transaction->Account.Member.LocationAddress != "")
		{
			std::auto_ptr<TStringList>TempList(new TStringList);
			TempList->Text = PrintJob->Transaction->Account.Member.LocationAddress;
			for (int i = 0; i < TempList->Count; i++)
			{
				pPrinter->Line->Columns[0]->Text = TempList->Strings[i];
				pPrinter->AddLine();
			}
		}

		pPrinter->NewLine();
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 2;
		AnsiString Sign = "Signature : ";
		pPrinter->Line->Columns[0]->Width = Sign.Length() + 1;
		pPrinter->Line->Columns[1]->Width = pPrinter->Width - Sign.Length() - 1;
		pPrinter->Line->Columns[0]->Text = Sign;
		pPrinter->Line->Columns[1]->Text = "";
		pPrinter->AddLine();
		pPrinter->DrawLine();

	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintAccountCreditInfo(TReqPrintJob *PrintJob)
{
	if (PrintJob->Transaction->Money.CreditAvailable != 0)
	{
		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width * 2 / 3;
		pPrinter->Line->Columns[1]->Width = pPrinter->Width - pPrinter->Line->Columns[0]->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = "Acc Credit";
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
		pPrinter->Line->Columns[1]->Text = CurrToStrF(PrintJob->Transaction->Money.RoundedCreditAvailable, ffNumber, CurrencyDecimals);

		pPrinter->AddLine();

		pPrinter->Line->Columns[0]->Text = "Credit Used";
		pPrinter->Line->Columns[1]->Text = CurrToStrF(PrintJob->Transaction->Money.RoundedCreditRedeemed, ffNumber, CurrencyDecimals);
		pPrinter->AddLine();
		pPrinter->Line->Columns[0]->Text = "Credit Remaining";
		pPrinter->Line->Columns[1]->Text = CurrToStrF(PrintJob->Transaction->Money.RoundedCreditAvailable - PrintJob->Transaction->Money.RoundedCreditRedeemed, ffNumber, CurrencyDecimals);
		pPrinter->AddLine();
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintExtraInfo(TReqPrintJob *PrintJob)
{
	if (PrintJob->ExtraInfo->Count > 0)
	{
		pPrinter->NewLine();
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo.Bold = false;
		pPrinter->Line->FontInfo.Height = fsNormalSize;
		pPrinter->Line->FontInfo.Width = fsNormalSize;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Add(ThisInstruction->Caption);

		std::auto_ptr<TStringList>Coloums(new TStringList());
		WrapText(PrintJob->ExtraInfo, Coloums.get(), pPrinter->Line->Columns[0]->Width);
		for (int i = 0; i < Coloums->Count; i++)
		{
			pPrinter->Add(Coloums->Strings[i]);
		}

	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintDeliveryInfo(TReqPrintJob *PrintJob)
{
	if (PrintJob->DeliveryInfo->Count > 0)
	{
		pPrinter->NewLine();
        pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Add(ThisInstruction->Caption);

		std::auto_ptr<TStringList>Coloums(new TStringList());
		WrapText(PrintJob->DeliveryInfo, Coloums.get(), pPrinter->Line->Columns[0]->Width);
        bool underLine = pPrinter->Line->FontInfo.Underlined;
        pPrinter->Line->FontInfo.Underlined = false;
		for (int i = 0; i < Coloums->Count; i++)
		{
            if( i == Coloums->Count - 1)
               pPrinter->Line->FontInfo.Underlined = underLine;
            if(Coloums->Strings[i] != "") // changes here to check null values.
            {
			    pPrinter->Add(Coloums->Strings[i]);
            }
		}
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintWebComments(TReqPrintJob *PrintJob)
{
	if (PrintJob->OrderComments->Count > 0)
	{
		pPrinter->NewLine();
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo.Bold = false;
		pPrinter->Line->FontInfo.Height = fsNormalSize;
		pPrinter->Line->FontInfo.Width = fsNormalSize;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Add(ThisInstruction->Caption);

		std::auto_ptr<TStringList>Coloums(new TStringList());
		WrapText(PrintJob->OrderComments, Coloums.get(), pPrinter->Line->Columns[0]->Width);
		for (int i = 0; i < Coloums->Count; i++)
		{
			pPrinter->Add(Coloums->Strings[i]);
		}
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::WrapText(TStringList *TextIn, TStringList *TextOut, int Width)
{
	for (int i = 0; i < TextIn->Count; i++)
	{
		SplitString(TextIn->Strings[i], TextOut, Width);
	}
}

void TPrintSection::SplitString(UnicodeString Source, TStringList *TextOut, int Width)
{
	if (Source.Length() < Width)
	{
		TextOut->Add(Source);
	}
	else
	{
		int lines = Source.Length() / Width;
		int Extra = Source.Length() % Width;
		int offset = 1;
		for (int i = 0; i < lines; ++i, offset += Width)
		{
			TextOut->Add(Source.SubString(offset, Width));
		}
		if (Extra)
		{
			TextOut->Add(Source.SubString(offset, Extra));
		}
	}
}

void TPrintSection::PrintPaymentInfo(TReqPrintJob *PrintJob)
{
	if (PrintJob->PaymentInfo->Count > 0)
	{
		pPrinter->NewLine();
        pPrinter->Line->ColCount = 1;
        pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->Columns[0]->Alignment = taCenter;
		pPrinter->Add(ThisInstruction->Caption);

		std::auto_ptr<TStringList>Coloums(new TStringList());
		WrapText(PrintJob->PaymentInfo, Coloums.get(), pPrinter->Line->Columns[0]->Width);
		for (int i = 0; i < Coloums->Count; i++)
		{
		  	pPrinter->Add(Coloums->Strings[i]);
		}

	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::OpenCashDrawer(TReqPrintJob *PrintJob)
{
	pPrinter->OpenDrawer();
}

void TPrintSection::PrintPaymentHistory(TReqPrintJob *PrintJob)
{
	if (PrintJob->TabHistory->Count > 0)
	{
		pPrinter->NewLine();
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo.Bold = false;
		pPrinter->Line->FontInfo.Height = fsNormalSize;
		pPrinter->Line->FontInfo.Width = fsNormalSize;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Add(ThisInstruction->Caption);

		for (int i = 0; i < PrintJob->TabHistory->Count; i++)
		{
			pPrinter->Add(PrintJob->TabHistory->Strings[i]);
		}
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintMemberNameInformation(TReqPrintJob *PrintJob)
{
	if (PrintJob->Transaction->Membership.Member.ContactKey != 0)
	{
		UnicodeString Member = PrintJob->Transaction->Membership.Member.Alias + " ( " + PrintJob->Transaction->Membership.Member.MembershipNumber + " ) ";
		switch(PrintJob->MembershipNameDisplay)
		{
		case 0: // Known As / Full Name
			if (PrintJob->Transaction->Membership.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			else
			{
				if (PrintJob->Transaction->Membership.Member.Title != "")
				{
					Member = PrintJob->Transaction->Membership.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
				}
				else
				{
					Member = PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
				}
			}
			break;
		case 1: // 1 Full Name / Known As
			if (PrintJob->Transaction->Membership.Member.Name != "")
			{
				if (PrintJob->Transaction->Membership.Member.Title != "")
				{
					Member = PrintJob->Transaction->Membership.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
				}
				else
				{
					Member = PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
				}
			}
			else
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			break;
		case 2: // Full Name
			if (PrintJob->Transaction->Membership.Member.Name != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
			}
			break;
		case 3: // Known As
			if (PrintJob->Transaction->Membership.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			break;
		default:
			if (PrintJob->Transaction->Membership.Member.Alias != "")
			{
				Member = PrintJob->Transaction->Membership.Member.Alias;
			}
			else
			{
				if (PrintJob->Transaction->Membership.Member.Title != "")
				{
					Member = PrintJob->Transaction->Membership.Member.Title + " " + PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
				}
				else
				{
					Member = PrintJob->Transaction->Membership.Member.Name+" "+PrintJob->Transaction->Membership.Member.Surname;
				}
			}
		}

		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - Member.Length();
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[1]->Width = Member.Length();
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = "Membership";
		pPrinter->Line->Columns[1]->Text = Member;
		pPrinter->AddLine();
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintPartyInformation(TReqPrintJob *PrintJob)
{
	if (PrintJob->MiscData["PartyName"] != "")
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taCenter;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = PrintJob->MiscData["PartyName"];
		pPrinter->AddLine();
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintDateOrdered(TReqPrintJob *PrintJob)
{
	UnicodeString ServingCourseName;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = Order->TimeStamp.FormatString("-- dd/mm/yy --");
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintPointsInformation(TReqPrintJob *PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width * 2 / 3;
	pPrinter->Line->Columns[1]->Width = pPrinter->Width - pPrinter->Line->Columns[0]->Width;

	if (PrintJob->Transaction->Membership.Member.ContactKey != 0)
	{
		pPrinter->Add("Membership No." + PrintJob->Transaction->Membership.Member.MembershipNumber);

		if (PrintJob->Transaction->Membership.MemberSource == emsSmartCard || PrintJob->Transaction->Membership.MemberSource == emsManual
           ||PrintJob->Transaction->Membership.MemberSource ==  emsBarcode)
		{

             /*if(TGlobalSettings::Instance().LoyaltyMateEnabled &&
                !TGlobalSettings::Instance().IsPOSOffline)
                {
                    PrintJob->Transaction->Membership.Member.Points.ClearBySource(pasDatabase) ;
                      // Putting in the Points Earned.
                    TPointsTypePair typepair1( pttEarned,ptstLoyalty );
                    TPointsType type1( pasDatabase, typepair1, pesExported);
                    double pointsEarned = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableEarnedPoint -
                    TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableBDPoint -
                    TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableFVPoint;
                    PrintJob->Transaction->Membership.Member.Points.Load( type1,pointsEarned );

                    // Putting in the Points Loaded ( Purchased ).
                    TPointsTypePair typepair2( pttPurchased,ptstAccount );
                    TPointsType type2( pasDatabase, typepair2, pesExported );
                    PrintJob->Transaction->Membership.Member.Points.Load( type2, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableLoadedPoint );
                }*/
			pPrinter->Add("Current Points|" + FormatFloat("0.00",PrintJob->Transaction->Membership.Member.Points.getPointsBalance(pasDatabase)));

			if (PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRedeemed() != 0 || PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRefunded() != 0)
			{
				if(PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRefunded() != 0)
				{
					pPrinter->Add("Points Refunded|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRefunded()));
				}
				else
				{
					pPrinter->Add("Points Redeemed|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRedeemed()));
				}
			}
			if (PrintJob->Transaction->Membership.Member.Points.getCurrentPointsEarned() != 0)
			{
				pPrinter->Add("Points Earned|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getCurrentPointsEarned()));
			}
			if (PrintJob->Transaction->IsBirtdayProcessed)
			{
				pPrinter->Add("Birthday Reward|" + FormatFloat("0.00", PrintJob->Transaction->Birthdaypoints));
			}
			if (PrintJob->Transaction->IsFirstVisitProcessed)
			{
				pPrinter->Add("First Visit Reward|" + FormatFloat("0.00", TGlobalSettings::Instance().FirstVisitPoint));
			}

			if (TGlobalSettings::Instance().UseTierLevels && PrintJob->Transaction->Membership.Member.TierLevel > 0)
			{
				pPrinter->Add("Tier Level|" + TDBTierLevel::GetTierLevelName(PrintJob->Transaction->DBTransaction,PrintJob->Transaction->Membership.Member.TierLevel));
			}
			if (PrintJob->Transaction->Membership.Member.Points.getCurrentPointsPurchased() != 0)
			{
				if(PrintJob->Transaction->Membership.Member.Points.getCurrentPointsPurchased() > 0)
				{
					pPrinter->Add("Points Purchased|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getCurrentPointsPurchased()));
				}
				else if(PrintJob->Transaction->Membership.Member.Points.getCurrentPointsPurchased() < 0)
				{
					pPrinter->Add("Points Refunded|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getCurrentPointsPurchased()));
				}
			}
            if(PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRefunded() != 0)
            {
                double points_new_total = PrintJob->Transaction->Membership.Member.Points.getPointsBalance(pasDatabase) + PrintJob->Transaction->Membership.Member.Points.getCurrentPointsRefunded();
                pPrinter->Add("Points New Total|" + FormatFloat("0.00", points_new_total));
            }
            else
            {
                pPrinter->Add("Points New Total|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getPointsBalance()));
            }
			if(TGlobalSettings::Instance().EnableSeperateEarntPts)
			{
				pPrinter->Add("  - Earned Balance   " + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getPointsBalance(ptstLoyalty)) );
				pPrinter->Add("  - Loaded Balance  " + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getPointsBalance(ptstAccount)) );
			}
		}
		else if (PrintJob->Transaction->Membership.MemberSource == emsSmartCardOffline)
		{
			pPrinter->Add("No Smart Card presented.");
			pPrinter->Add("Points held in store.");
			pPrinter->Add("Points Earned & Held|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getPointsHeldEarned()));
			pPrinter->Add("Points Loaded & Held|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getPointsHeldPurchased()));
			pPrinter->Add("Points Total HELD at store|" + FormatFloat("0.00", PrintJob->Transaction->Membership.Member.Points.getPointsHeldTotal()));
		}
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintCurrentYearPts(TReqPrintJob *PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width * 2 / 3;
	pPrinter->Line->Columns[1]->Width = pPrinter->Width - pPrinter->Line->Columns[0]->Width;
	if (PrintJob->Transaction->Membership.Member.ContactKey != 0)
	{
        if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints > 0)
        {
            pPrinter->Add("Current Year Points|" + FormatFloat("0.00", TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints)); //GetEarnedPointsForCurrentYear(PrintJob->Transaction->DBTransaction, PrintJob->Transaction->Membership.Member)));
        }
        else
        {
            pPrinter->Add("Current Year Points|" + FormatFloat("0.00", TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetEarnedPointsForCurrentYear(PrintJob->Transaction->DBTransaction, PrintJob->Transaction->Membership.Member)));
        }

    }
}

void TPrintSection::PrintAvailableWeight(TReqPrintJob *PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width * 2 / 3;
	pPrinter->Line->Columns[1]->Width = pPrinter->Width - pPrinter->Line->Columns[0]->Width;
	Currency availablePoints = PrintJob->Transaction->Membership.Member.Points.getPointsBalance();
	Currency availableWeight = availablePoints;
	if(TGlobalSettings::Instance().UseTierLevels)
	{
		TTierLevel *TierLevel = new TTierLevel;
		TierLevel->Level = PrintJob->Transaction->Membership.Member.TierLevel;
		TDBTierLevel::GetTierLevelInformationByTierLevel(PrintJob->Transaction->DBTransaction,TierLevel);
		availableWeight = availablePoints * TierLevel->WeighedRedemWeight/TierLevel->WeighedRedemPoint;
	}
	pPrinter->Add("Redeemable Weight(Grams) |" + FormatFloat("0.00",availableWeight));

}

void TPrintSection::PrintCourse(TReqPrintJob *PrintJob)
{
	UnicodeString CourseName;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		CourseName = Order->CourseKitchenName;
	}

	if (CourseName != "")
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = CourseName;
		pPrinter->AddLine();
	}
	else
	{
		Empty = false;
	}
}

void TPrintSection::PrintItem(TReqPrintJob *PrintJob)
{
	if (WorkingOrdersList->Count == 0)
	{
		Empty = true;
	}
	else
	{
		bool IncludeNote;
		bool IncludeSides;
		bool IncludeOptions;

		TSectionInstructStorage::iterator i = ThisInstruction;
		for ( ; i != MasterInstructionsList.end(); i++) {
			IncludeNote = i->Instruction != epofiPrintItemNote;
			IncludeSides = i->Instruction != epofiPrintItemSides;
			IncludeOptions = i->Instruction != epofiPrintItemOptions;
		}

		WideString QtyMultiplier1 = WideString("x ");
		WideString QtyMultiplier10 = WideString("x");
		WideString Spacer1 = WideString(" ");
		WideString SeatStart = WideString("(");
		WideString SeatEnd = WideString(")");
		WideString SeatSpacer = WideString(", ");

		UnicodeString CancelLine = "";

		std::vector<TItemSubSection>SubItems;
		std::vector<TItemSubSection>Options;

		TDateTime TimeStamp = Now();

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *CurrentOrder =
			(TItemComplete *)WorkingOrdersList->Items[i];
			Currency CountQuantity = CurrentOrder->GetQty();

			for (int j = 0; j < CountQuantity; j++) {
				WideString ItemLine = "";
				WideString ItemLineSize = "";
				WideString SetMenuHeader = "";
				WideString MixLine = "";
				WideString SideLine = "";
				WideString NoteLine = "";
				WideString SetMenuItemSpacer = "";
				UnicodeString SizeName = "";
				int SetMenuGroup = 0;
				SubItems.clear();
				Options.clear();

				if (CurrentOrder->SetMenuMask != 0 && CurrentOrder->SetMenuGroup != 0)
				{
					if (Format.KeyItemHeader[CurrentOrder->SetMenuGroup] != WideString(""))
					{
						SetMenuHeader = "[" + Format.KeyItemHeader[CurrentOrder->SetMenuGroup] + "]";
					}
					SetMenuItemSpacer = Format.BulletSetMenuItem;
				}

				if (CurrentOrder->OrderType == CanceledOrder)
				{
					CancelLine = "!!!! CANCEL ORDERS !!!!";
				}
				else if (CurrentOrder->OrderType == CreditNonExistingOrder)
				{
					CancelLine = "!!!! REFUND ORDERS !!!!";
				}

				if (TimeStamp > CurrentOrder->TimeStamp)
				{
					TimeStamp = CurrentOrder->TimeStamp;
				}

				ItemLine = SetMenuItemSpacer;
				if (PrintJob->JobType == pjKitchen)
				{
					ItemLineSize = CurrentOrder->SizeKitchenName;
				}
				else
				{
					ItemLineSize = CurrentOrder->Size;
				}

				if (UpperCase(ItemLineSize) == "DEFAULT")
				ItemLineSize = "";

				ItemLine = ItemLine
				+ ItemLineSize
				+ Spacer1
				+ CurrentOrder->ItemKitchenName;

				int SeatsListWidth = 0;
				UnicodeString SeatsLine = ""; ;

				UnicodeString SeatTypeText = ""; // Cash Sale";
				UnicodeString Identifier = "";
				UnicodeString QtyText = "";

				switch(CurrentOrder->TabType)
				{
				case TabNormal:
				case TabStaff:
				case TabMember:
				case TabWeb:
					{
						SeatTypeText = ""; // Tab ";
						Identifier = ""; // PrevOrder->TabName;
						break;
					}
				case TabTableSeat:
					{
						// Cant use Tab name here it could be to long.
						SeatTypeText = "";
						Identifier = IntToStr(CurrentOrder->SeatNo);
						break;
					}
				case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
				default:
					// SeatTypeText = "Cash Sale";
					SeatTypeText = ""; // Cash Sale";
					Identifier = "";
					QtyText = "";
					break;
				}

				if (SeatTypeText != "" || Identifier != "")
				{
					SeatsLine = SeatStart + Identifier + SeatEnd;
				}

				if (CurrentOrder->SubOrders->Count > 0)
				{
					// Print Line
					// Only Show Mix if Items is Orders from the Drinks Menu.
					if (CurrentOrder->ItemType == eDrinksItem)
					{
						MixLine = "";
						if (UpperCase(CurrentOrder->Size) != "DEFAULT")
						MixLine = Format.MixHeader + CurrentOrder->MasterContainer;
					}
					else
					{
						SideLine = Format.SideHeader;
					}

					for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
					{
						if (UpperCase(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->Size) != "DEFAULT")
						{
							TItemCompleteSub *ItemCompleteSub = (TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]);
							if (PrintJob->JobType == pjKitchen)
							{
								SizeName =ItemCompleteSub->SizeKitchenName;
							}
							else
							{
								SizeName =ItemCompleteSub->Size;
							}
							TItemSubSection SubOrder;
							SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + SizeName + Spacer1 + ItemCompleteSub->ItemKitchenName;
							if (PrintJob->JobType == pjKitchen)
							{
								SubOrder.FontInfo = ItemCompleteSub->FontInfo;
							}
                            if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                            {
                                  SubOrder.isManuallyWeight=true;
                             }
							SubItems.push_back(SubOrder);
						}
						else
						{
							TItemCompleteSub *ItemCompleteSub = (TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]);
							TItemSubSection SubOrder;
							SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + ItemCompleteSub->ItemKitchenName;
							if (PrintJob->JobType == pjKitchen)
							{
								SubOrder.FontInfo = ItemCompleteSub->FontInfo;
							}

                            if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                            {
                                  SubOrder.isManuallyWeight=true;
                             }
							SubItems.push_back(SubOrder);
						}
					}
				}

				for (int i = 0; i < CurrentOrder->OptionsSelected->Count; i++)
				{
					TItemSubSection Option;
					UnicodeString BulletOption = CurrentOrder->OptionsSelected->OptionGet(i)->IsPlus ? "+ " : "- ";
					Option.Caption = SetMenuItemSpacer + Format.BulletOpt + BulletOption + CurrentOrder->OptionsSelected->OptionGet(i)->KitchenName;
					if (PrintJob->JobType == pjKitchen)
					{
						Option.FontInfo = CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo;
					}
					Options.push_back(Option);
				}

				if (CurrentOrder->Note != "")
				{
					NoteLine = Format.NoteHeader + CurrentOrder->Note;
				}

				TPrintInfo TotalFontInfo;
				if (PrintJob->JobType == pjKitchen)
				{
					TotalFontInfo.Bold = CurrentOrder->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
					TotalFontInfo.Underlined = CurrentOrder->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
					TotalFontInfo.Width = ((CurrentOrder->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
					TotalFontInfo.Height = ((CurrentOrder->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
					TotalFontInfo.Colour = ((CurrentOrder->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
					TotalFontInfo.Font = ((CurrentOrder->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
				}
				else
				{
					TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
					TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
					TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
					TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
					TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
					TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
				}

				pPrinter->Line->FontInfo = TotalFontInfo;
				// Print This Item.
				pPrinter->Line->ColCount = 1;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

				if (CancelLine != "")
				{
					pPrinter->Line->Columns[0]->Text = CancelLine;
					pPrinter->AddLine();
				}

				if (!SetMenuHeader.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = SetMenuHeader;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->AddLine();
				}

				pPrinter->Line->ColCount = 2;

				SeatsListWidth = SeatsLine.Length();
				if (SeatsListWidth > pPrinter->Width / 3)
				{
					SeatsListWidth = pPrinter->Width / 3;
				}

				pPrinter->Line->Columns[0]->Width = pPrinter->Width - SeatsListWidth;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				pPrinter->Line->Columns[1]->Width = SeatsListWidth;
				pPrinter->Line->Columns[1]->Alignment = taRightJustify;
				pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
				pPrinter->Line->Columns[0]->Text = ItemLine;
				pPrinter->Line->Columns[1]->Text = SeatsLine;
				pPrinter->AddLine();

              if(CurrentOrder->isManuallyEnteredWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
                        AddManuallyEnteredLineInReceipt(pPrinter);

				pPrinter->Line->FontInfo.Reset();
				pPrinter->Line->Columns[0]->Kanji = false;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->Columns[1]->Width = 0;

				if (IncludeOptions && Options.size() > 0)
				{
					for (int k = 0; k < Options.size(); k++)
					{
						pPrinter->Line->FontInfo = Options[k].FontInfo;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[0]->Text = Options[k].Caption;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
					}
				}

				if (IncludeNote && !NoteLine.IsEmpty())
				{
					pPrinter->WordWrap = true;
					pPrinter->Line->Columns[0]->Text = NoteLine;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}

				if (IncludeSides && SubItems.size() > 0)
				{
					if (!SideLine.IsEmpty())
					{
						pPrinter->Line->Columns[0]->Text = SideLine;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
					}

					for (int k = 0; k < SubItems.size(); k++)
					{
						pPrinter->Line->FontInfo = SubItems[k].FontInfo;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[0]->Text = SubItems[k].Caption;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();

                        if( SubItems[k].isManuallyWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)AddManuallyEnteredLineInReceipt(pPrinter);

         			}

					if (!MixLine.IsEmpty())
					{
						pPrinter->Line->Columns[0]->Text = MixLine;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
					}

				}
			}
		}
	}
}

void TPrintSection::PrintItemNote(TReqPrintJob *PrintJob)
{
	if (WorkingOrdersList->Count == 0)
	{
		Empty = true;
	}
	else
	{
		WideString QtyMultiplier1 = WideString("x ");
		WideString QtyMultiplier10 = WideString("x");
		WideString Spacer1 = WideString(" ");
		WideString SeatStart = WideString("(");
		WideString SeatEnd = WideString(")");
		WideString SeatSpacer = WideString(", ");
		UnicodeString CancelLine = "";

		TDateTime TimeStamp = Now();

		Empty = true;
		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			WideString NoteLine = "";
			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (CurrentOrder->Note != "")
			{
				NoteLine = Format.NoteHeader + CurrentOrder->Note;
			}

			TPrintInfo TotalFontInfo;
			if (PrintJob->JobType == pjKitchen)
			{
				TotalFontInfo.Bold = CurrentOrder->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrder->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrder->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrder->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrder->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrder->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
			}
			else
			{
				TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
				TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
				TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
				TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
			}

			pPrinter->Line->FontInfo = TotalFontInfo;
			pPrinter->Line->ColCount = 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

			if (!NoteLine.IsEmpty())
			{
				Empty = false;
				pPrinter->WordWrap = true;
				pPrinter->Line->Columns[0]->Text = NoteLine;
				pPrinter->AddLine();
			}
		}
	}
}

void TPrintSection::PrintItemSides(TReqPrintJob *PrintJob)
{
	if (WorkingOrdersList->Count == 0)
	{
		Empty = true;
	}
	else
	{
		WideString QtyMultiplier1 = WideString("x ");
		WideString QtyMultiplier10 = WideString("x");
		WideString Spacer1 = WideString(" ");
		WideString SeatStart = WideString("(");
		WideString SeatEnd = WideString(")");
		WideString SeatSpacer = WideString(", ");

		std::vector<TItemSubSection>SubItems;
		TDateTime TimeStamp = Now();

		Empty = true;
		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			WideString ItemLine = "";
			WideString ItemLineSize = "";
			WideString SetMenuHeader = "";
			WideString MixLine = "";
			WideString SideLine = "";
			WideString NoteLine = "";
			WideString SetMenuItemSpacer = "";
			UnicodeString SizeName = "";
			int SetMenuGroup = 0;
			SubItems.clear();

			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (CurrentOrder->SubOrders->Count > 0)
			{
				// Print Line
				// Only Show Mix if Items is Orders from the Drinks Menu.
				if (CurrentOrder->ItemType == eDrinksItem)
				{
					if (PrintJob->JobType == pjKitchen)
					{
						SizeName =CurrentOrder->SizeKitchenName;
					}
					else
					{
						SizeName =CurrentOrder->Size;
					}


					if (UpperCase(CurrentOrder->Size) != "DEFAULT")
					{
						MixLine = Format.MixHeader + CurrentOrder->MasterContainer;
					}
					else
					{
						MixLine = "";
					}
					// Right Justify these two lines
					if (UpperCase(CurrentOrder->Size) != "DEFAULT")
					{
						TItemSubSection SubOrder;
						SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + SizeName + Spacer1 + CurrentOrder->ItemKitchenName;
						if (PrintJob->JobType == pjKitchen)
						{
							SubOrder.FontInfo = CurrentOrder->FontInfo;
						}
					  if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                        {
                            SubOrder.isManuallyWeight=true;

                        }
                        SubItems.push_back(SubOrder);
					}
					else
					{
						TItemSubSection SubOrder;
						SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + CurrentOrder->ItemKitchenName;
						if (PrintJob->JobType == pjKitchen)
						{
							SubOrder.FontInfo = CurrentOrder->FontInfo;
						}
                       if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                        {
                            SubOrder.isManuallyWeight=true;

                        }
                        SubItems.push_back(SubOrder);
                    }


				}
				else
				{
					SideLine = Format.SideHeader;
				}

				for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
				{
					if (UpperCase(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->Size) != "DEFAULT")
					{
						TItemCompleteSub *ItemCompleteSub = (TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]);
						if (PrintJob->JobType == pjKitchen)
						{
							SizeName =ItemCompleteSub->SizeKitchenName;
						}
						else
						{
							SizeName =ItemCompleteSub->Size;
						}
						TItemSubSection SubOrder;
						SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + SizeName + Spacer1 + ItemCompleteSub->ItemKitchenName;
						if (PrintJob->JobType == pjKitchen)
						{
							SubOrder.FontInfo = ItemCompleteSub->FontInfo;
						}
					  if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                        {
                            SubOrder.isManuallyWeight=true;

                        }
                      	SubItems.push_back(SubOrder);
					}
					else
					{
						TItemCompleteSub *ItemCompleteSub = (TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]);
						TItemSubSection SubOrder;
						SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + ItemCompleteSub->ItemKitchenName;
						if (PrintJob->JobType == pjKitchen)
						{
							SubOrder.FontInfo = ItemCompleteSub->FontInfo;
						}
                       if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                        {
                            SubOrder.isManuallyWeight=true;

                        }
                      	SubItems.push_back(SubOrder);
					}
				}

				TPrintInfo TotalFontInfo;
				if (PrintJob->JobType == pjKitchen)
				{
					TotalFontInfo.Bold = CurrentOrder->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
					TotalFontInfo.Underlined = CurrentOrder->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
					TotalFontInfo.Width = ((CurrentOrder->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
					TotalFontInfo.Height = ((CurrentOrder->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
					TotalFontInfo.Colour = ((CurrentOrder->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
					TotalFontInfo.Font = ((CurrentOrder->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
				}
				else
				{
					TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
					TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
					TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
					TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
					TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
					TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
				}

				pPrinter->Line->FontInfo = TotalFontInfo;
				// Print This Item.
				pPrinter->Line->ColCount = 1;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

				if (SubItems.size() > 0)
				{
					Empty = false;
					if (!SideLine.IsEmpty())
					{
						pPrinter->Line->Columns[0]->Text = SideLine;
						pPrinter->AddLine();
					}

					for (int k = 0; k < SubItems.size(); k++)
					{
						pPrinter->Line->FontInfo = SubItems[k].FontInfo;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[0]->Text = SubItems[k].Caption;
						pPrinter->AddLine();

                        if( SubItems[k].isManuallyWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
                        {
                                UnicodeString str=" (Manually entered weight)";
                                pPrinter->Line->Columns[0]->Width = str.Length() ;
                                pPrinter->Line->Columns[0]->Text = str;
                                pPrinter->AddLine();
                         }

					}

					if (!MixLine.IsEmpty())
					{
						pPrinter->Line->Columns[0]->Text = MixLine;
						pPrinter->AddLine();
					}
				}
			}
		}
	}
}

void TPrintSection::PrintItemOptions(TReqPrintJob *PrintJob)
{
	if (WorkingOrdersList->Count == 0)
	{
		Empty = true;
	}
	else
	{
		WideString QtyMultiplier1 = WideString("x ");
		WideString QtyMultiplier10 = WideString("x");
		WideString Spacer1 = WideString(" ");
		WideString SeatStart = WideString("(");
		WideString SeatEnd = WideString(")");
		WideString SeatSpacer = WideString(", ");

		std::vector<TItemSubSection>Options;

		TDateTime TimeStamp = Now();
		Empty = true;
		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			WideString ItemLine = "";
			WideString ItemLineSize = "";
			WideString SetMenuHeader = "";
			WideString MixLine = "";
			WideString SideLine = "";
			WideString NoteLine = "";
			WideString SetMenuItemSpacer = "";
			int SetMenuGroup = 0;
			Options.clear();

			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (CurrentOrder->OptionsSelected->Count > 0)
			{
				for (int i = 0; i < CurrentOrder->OptionsSelected->Count; i++)
				{
					TItemSubSection Option;
					UnicodeString BulletOption = CurrentOrder->OptionsSelected->OptionGet(i)->IsPlus ? "+ " : "- ";
					Option.Caption = SetMenuItemSpacer + Format.BulletOpt + BulletOption + CurrentOrder->OptionsSelected->OptionGet(i)->KitchenName;

					TPrintInfo TotalFontInfo;
					if (PrintJob->JobType == pjKitchen)
					{
						TotalFontInfo.Bold = CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
						TotalFontInfo.Underlined = CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
						TotalFontInfo.Width = ((CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize))
						? fsDoubleSize : fsNormalSize;
						TotalFontInfo.Height = ((CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize))
						? fsDoubleSize : fsNormalSize;
						TotalFontInfo.Colour = ((CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
						TotalFontInfo.Font = ((CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
					}
					else
					{
						TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
						TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
						TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
						TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
						TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
						TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
					}

					Option.FontInfo = TotalFontInfo;
					Options.push_back(Option);
				}

				// Print This Item.
				pPrinter->Line->ColCount = 1;
				pPrinter->Line->FontInfo.Reset();
				pPrinter->Line->Columns[0]->Kanji = false;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				for (int k = 0; k < Options.size(); k++)
				{
					pPrinter->Line->FontInfo = Options[k].FontInfo;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->Line->Columns[0]->Text = Options[k].Caption;
					pPrinter->AddLine();
					Empty = false;
				}
			}
		}
	}
}

// 2024...
void TPrintSection::PrintTwoLinesItemTotal(TReqPrintJob *PrintJob)     // print two lines item total..
{
	bool IncludeDiscounts = true;
	TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.end())
	{
		if (i->Instruction == epofiPrintTotalDiscount || i->Instruction == epofiPrintTotalDiscountsDetails || i->Instruction == epofiPrintTotalDiscountsName)
		{
			IncludeDiscounts = false;
		}
		advance(i, 1);
	}

	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	WideString QtyMultiplier1 = WideString("x ");
	WideString QtyMultiplier10 = WideString("x ");
	WideString Spacer1 = WideString(" ");
	WideString SeatStart = WideString("(");
	WideString SeatEnd = WideString(")");
	WideString SeatSpacer = WideString(", ");
	WideString ReceiptItemSpacer = WideString("   ");
	WideString ReceiptItemSpacerNew = WideString("  "); //2024

	TDateTime TimeStamp = Now();
	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		Currency CountQuantity = 0;
		TItemComplete *InitialOrder = (TItemComplete*)WorkingOrdersList->Items[i];
		CountQuantity += InitialOrder->GetQty();
		UnicodeString Quantity = FormatFloat("0.###", CountQuantity);
		UnicodeString ItemName = InitialOrder->Item;
		UnicodeString ItemPrice = InitialOrder->Price();
		UnicodeString SizeName  = "";
		if (PrintJob->JobType == pjKitchen)
		{
			SizeName = InitialOrder->SizeKitchenName;
		}
		else
		{
			SizeName = InitialOrder->Size;
		}

		if (UpperCase(InitialOrder->Size) != "DEFAULT" && InitialOrder->Size != WideString(""))
		{
			if (CountQuantity != 1 && CountQuantity != 0 && CountQuantity < 10)
			{
				ItemName = Quantity + QtyMultiplier1 + InitialOrder->Item + Spacer1 + SizeName;
			}
			else if (CountQuantity >= 10)
			{
				ItemName = Quantity + QtyMultiplier10 + InitialOrder->Item + Spacer1 + SizeName;
			}
			else
			{
				ItemName = ReceiptItemSpacer + InitialOrder->Item + Spacer1 + SizeName;
			}
		}
		else
		{
			if (CountQuantity != 1 && CountQuantity != 0 && CountQuantity < 10)
			{
				ItemName = Quantity + QtyMultiplier1 + InitialOrder->Item;
			}
			else if (CountQuantity >= 10)
			{
				ItemName = Quantity + QtyMultiplier10 + InitialOrder->Item;
			}
			else
			{
				ItemName = ReceiptItemSpacer + InitialOrder->Item;
			}
		}

		if (IncludeDiscounts)
		{
			ItemPrice = CurrToStrF(
			RoundToNearest(InitialOrder->TotalPriceAdjustment(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals);

		}
		else
		{
			ItemPrice = CurrToStrF(
			RoundToNearest(InitialOrder->Price(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals);
		}

		// for weighted and price barcoded items, only show the name. without the quantity multiplier
		if (InitialOrder->WeightedPrice.WeightedItem
				|| (InitialOrder->ItemOrderedFrom &&  InitialOrder->ItemOrderedFrom->IsPriceBarcodedItem ))
		{
			ItemName = InitialOrder->Item;
		}

        
        if(InitialOrder->OrderType == CanceledOrder)
        {
            pPrinter->Line->Columns[0]->Text = "!!!! CANCEL ORDERS !!!!";
            pPrinter->Line->Columns[1]->Text = "";
            pPrinter->AddLine();
        }
        int length = GetItemLength(InitialOrder->Item, InitialOrder->Item.Length());

		pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
        if(length > 0)
        {
           pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width - length;
        }
        else
        {
           pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;
        }
		TStringList *NameList = new TStringList;
		PrepareName(pPrinter->Line->Columns[0]->Width,ItemName,NameList);

		for(int i = 0; i < NameList->Count; i++)
		{
			pPrinter->Line->Columns[0]->Text = ReceiptItemSpacerNew + NameList->Strings[i];
			pPrinter->Line->Columns[1]->Text = "";
			if(i == NameList->Count-1)
			{
				pPrinter->Line->Columns[1]->Text = ItemPrice;
			}
			pPrinter->AddLine();

            if(InitialOrder->isManuallyEnteredWeight==true && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
            {
              AddManuallyEnteredLineInReceipt(pPrinter);
            }
		}
		pPrinter->Line->Columns[1]->Text = "";
		pPrinter->Line->Columns[0]->Text = "";

	}
}
//2024
void TPrintSection::PrintTwoLinesItemsTotal(TReqPrintJob *PrintJob)     // print  two lines items total..
{
	WideString ReceiptItemSpacerNew = WideString("  "); //2024

	bool IncludeDiscounts = true;
	TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.end())
	{
		if (i->Instruction == epofiPrintTotalDiscount || i->Instruction == epofiPrintTotalDiscountsDetails || i->Instruction == epofiPrintTotalDiscountsName)
		{
			IncludeDiscounts = false;
		}
		advance(i, 1);
	}

	TList *OrderBundle;
	try
	{
		OrderBundle = new TList;
		TDateTime TimeStamp = Now();
		for (int i = 0; i < WorkingOrdersList->Count; )
		{
			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (TimeStamp > CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}

			TOrderBundle *TempBundle = new TOrderBundle();
			TempBundle->BundleOrders(PrintJob, WorkingOrdersList.get(), i,Format);

			OrderBundle->Add(TempBundle);
		}

		for (int j = 0; j < OrderBundle->Count; j++)
		{
			TOrderBundle *CurrentOrderBundle = (TOrderBundle*)OrderBundle->Items[j];
			TOrderBundle *NextOrderBundle = NULL;

			TPrintInfo TotalFontInfo;
			if (PrintJob->JobType == pjKitchen)
			{
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
			}
			else
			{
				TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
				TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
				TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
				TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
			}

			pPrinter->Line->FontInfo = TotalFontInfo;

			// Display the Items.
			pPrinter->Line->ColCount = 2;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			pPrinter->Line->FontInfo.Bold = false;
			pPrinter->Line->Columns[1]->Alignment = taRightJustify;
			pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

			UnicodeString ItemName = CurrentOrderBundle->ItemLine;
			UnicodeString ItemPrice = CurrToStrF(
			RoundToNearest(TOrderUtils::TotalPriceSides(CurrentOrderBundle->BundledOrders.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
			ffNumber,
			CurrencyDecimals);

			if (IncludeDiscounts)
			{
				ItemPrice = CurrToStrF(
				RoundToNearest(TOrderUtils::TotalPriceAdjustmentSides(CurrentOrderBundle->BundledOrders.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals);
			}

			if (!CurrentOrderBundle->CancelLine.IsEmpty())
			{
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->CancelLine;
                pPrinter->Line->Columns[1]->Text = "";
				pPrinter->AddLine();
			}

            int length = GetItemLength(CurrentOrderBundle->ItemLine, CurrentOrderBundle->ItemLine.Length());
            pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
            if(length > 0)
            {
               pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width - length;
            }
            else
            {
               pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;
            }

			//pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
			//pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

			TStringList *NameList = new TStringList;
			PrepareName(pPrinter->Line->Columns[0]->Width,ItemName,NameList);
			for(int i = 0; i < NameList->Count; i++)
			{
				pPrinter->Line->Columns[0]->Text = ReceiptItemSpacerNew + NameList->Strings[i];
				pPrinter->Line->Columns[1]->Text = "";
				if(i == NameList->Count-1)
				{
					pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;;;
				}
				pPrinter->AddLine();
			}
			pPrinter->Line->Columns[1]->Text = "";
			pPrinter->Line->Columns[0]->Text = "";

			if (CurrentOrderBundle->SubItems.size() > 0)
			{
				if (!CurrentOrderBundle->SideLine.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SideLine;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}

				for (int k = 0; k < CurrentOrderBundle->SubItems.size(); k++)
				{
					if (PrintJob->JobType == pjKitchen)
					{
						pPrinter->Line->FontInfo = CurrentOrderBundle->SubItems[k].FontInfo;
					}
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SubItems[k].Caption;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}

				if (!CurrentOrderBundle->MixLine.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->MixLine;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}
			}

			TPtrSectionInstructStorage::iterator ptrSubInstruction = ThisInstruction->SubInstructions.begin();
			while (ptrSubInstruction != ThisInstruction->SubInstructions.end())
			{
				TPSectionInstruction *itInstruction = *ptrSubInstruction;
				// Get The Iterator that iterator points too.
				// Process the Note & Options only once.
				if (((itInstruction->Instruction == epofiPrintItemNote) || (itInstruction->Instruction == epofiPrintItemOptions)) && CurrentOrderBundle->BundledOrders->Count > 0)
				{
					std::auto_ptr<TList>SingleOrders(new TList);
					SingleOrders->Add(CurrentOrderBundle->BundledOrders->Items[0]);
					TPrintSection NewSection(pPrinter, SingleOrders.get(), MasterInstructionsList, itInstruction,Format);
					NewSection.ProcessSection(PrintJob);
				}
				else
				{
					TPrintSection NewSection(pPrinter, CurrentOrderBundle->BundledOrders.get(), MasterInstructionsList, itInstruction,Format);
					NewSection.ProcessSection(PrintJob);
				}
				advance(ptrSubInstruction, 1);
			}
		}
	}
	__finally
	{
		while (OrderBundle->Count != 0)
		{
			delete(TOrderBundle*)OrderBundle->Items[0];
			OrderBundle->Delete(0);
		}
		delete OrderBundle;
	}
}

//2024

void TPrintSection::PrintItemTotal(TReqPrintJob *PrintJob)
{

	bool IncludeDiscounts = true;
	TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.end())
	{
		if (i->Instruction == epofiPrintTotalDiscount || i->Instruction == epofiPrintTotalDiscountsDetails || i->Instruction == epofiPrintTotalDiscountsName)
		{
			IncludeDiscounts = false;
		}
		advance(i, 1);
	}

	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	WideString QtyMultiplier1 = WideString("x ");
	WideString QtyMultiplier10 = WideString("x");
	WideString Spacer1 = WideString(" ");
	WideString SeatStart = WideString("(");
	WideString SeatEnd = WideString(")");
	WideString SeatSpacer = WideString(", ");
	WideString ReceiptItemSpacer = WideString("   ");

	TDateTime TimeStamp = Now();
	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		Currency CountQuantity = 0;
		TItemComplete *InitialOrder = (TItemComplete*)WorkingOrdersList->Items[i];
		CountQuantity += InitialOrder->GetQty();
		UnicodeString Quantity = FormatFloat("0.###", CountQuantity);
		UnicodeString ItemName = InitialOrder->Item;
		UnicodeString ItemPrice = InitialOrder->Price();
		UnicodeString SizeName  = "";
		if (PrintJob->JobType == pjKitchen)
		{
			SizeName = InitialOrder->SizeKitchenName;
		}
		else
		{
			SizeName = InitialOrder->Size;
		}

		if (UpperCase(InitialOrder->Size) != "DEFAULT" && InitialOrder->Size != WideString(""))
		{
			if (CountQuantity != 1 && CountQuantity != 0 && CountQuantity < 10)
			{
				ItemName = Quantity + QtyMultiplier1 + InitialOrder->Item + Spacer1 + SizeName;
			}
			else if (CountQuantity >= 10)
			{
				ItemName = Quantity + QtyMultiplier10 + InitialOrder->Item + Spacer1 + SizeName;
			}
			else
			{
				ItemName = ReceiptItemSpacer + InitialOrder->Item + Spacer1 + SizeName;
			}
		}
		else
		{
			if (CountQuantity != 1 && CountQuantity != 0 && CountQuantity < 10)
			{
				ItemName = Quantity + QtyMultiplier1 + InitialOrder->Item;
			}
			else if (CountQuantity >= 10)
			{
				ItemName = Quantity + QtyMultiplier10 + InitialOrder->Item;
			}
			else
			{
				ItemName = ReceiptItemSpacer + InitialOrder->Item;
			}
		}

		if (IncludeDiscounts)
		{
			ItemPrice = CurrToStrF(
			RoundToNearest(InitialOrder->TotalPriceAdjustment(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals);

		}
		else
		{
			ItemPrice = CurrToStrF(
			RoundToNearest(InitialOrder->Price(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals);
		}

		// for weighted and price barcoded items, only show the name. without the quantity multiplier
		if (InitialOrder->WeightedPrice.WeightedItem
				|| (InitialOrder->ItemOrderedFrom &&  InitialOrder->ItemOrderedFrom->IsPriceBarcodedItem ))
		{
			ItemName = InitialOrder->Item;
		}

		pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

        if(InitialOrder->OrderType == CanceledOrder)
        {
            pPrinter->Line->Columns[0]->Text = "!!!! CANCEL ORDERS !!!!";
            pPrinter->Line->Columns[1]->Text = "";
            pPrinter->AddLine();
        }
        pPrinter->Line->Columns[0]->Text = ItemName;
        pPrinter->Line->Columns[1]->Text = ItemPrice;
        pPrinter->AddLine();

        if(InitialOrder->isManuallyEnteredWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)AddManuallyEnteredLineInReceipt(pPrinter);


  		/*
		// ::::::::::::::::::::::::::::::::::::::::::::::::::::
		// This section has been commentted out to make weighted items
		// look the same as other items in the receipt

		// ::::::::::::::::::::::::::::::::::::::::::::::::::::
		// Weighted Size Info Here.
		if (InitialOrder->WeightedPrice.WeightedItem)
		{
			ItemName = Receipt->BulletOpt + InitialOrder->WeightedPrice.Weight.ToStringWithUnit()
							+ " @ "
							+ CurrToStrF(
									RoundToNearest(InitialOrder->WeightedPrice.UnitPrice, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
									ffNumber,
									CurrencyDecimals)
							+ "/"
							+ InitialOrder->WeightedPrice.Weight.GetUnitAsString();
			ItemPrice = "";
			pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

			pPrinter->Line->Columns[0]->Text = ItemName;
			pPrinter->Line->Columns[1]->Text = ItemPrice;
			pPrinter->AddLine();

			if (InitialOrder->WeightedPrice.TareWeight.Valid())
			{
				ItemName = Receipt->BulletOpt + "-" + InitialOrder->WeightedPrice.TareWeight.ToStringWithUnit() + " For Packaging";
				ItemPrice = "";
				pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

				pPrinter->Line->Columns[0]->Text = ItemName;
				pPrinter->Line->Columns[1]->Text = ItemPrice;
				pPrinter->AddLine();
			}
		}
*/
	}
}

void TPrintSection::PrintTotalDicountsName(TReqPrintJob *PrintJob)
{

	WideString ReceiptItemSpacer = PrintJob->Transaction->TypeOfSale == RegularSale ? WideString("   "):  WideString("") ;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	std::map<UnicodeString, Currency> DiscountTotals;
	bool Discounts = false;
	Currency SubTotal;
	Currency temp = 0;
	DiscountTotals.clear();

	bool scdApplied = false; // Senior Citizen Discount

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemComplete *Item = (TItemComplete*)WorkingOrdersList->Items[i];

		scdApplied = scdApplied | scdHasBeenApplied( Item->BillCalcResult.Discount );

		if (Item->BillCalcResult.Discount.size() != 0)
		{
			Discounts = true;
			std::vector<BillCalculator::TDiscountResult>::iterator itDiscounts;
			for (itDiscounts = Item->BillCalcResult.Discount.begin(); itDiscounts != Item->BillCalcResult.Discount.end(); itDiscounts++)
			{
				UnicodeString Discription = ReceiptItemSpacer + itDiscounts->Description;
				SubTotal = DiscountTotals.find(Discription) != DiscountTotals.end() ? DiscountTotals[Discription] : Currency(0);
				SubTotal += itDiscounts->DiscountType == BillCalculator::dtDiscount ? -itDiscounts->Content : itDiscounts->Content;
				DiscountTotals[Discription] = SubTotal;
			}
		}

		for (int i = 0; i < Item->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubItem = (TItemCompleteSub*)Item->SubOrders->SubOrderGet(i);
			if (SubItem->DiscountSize() != 0)
			{
				Discounts = true;
				std::vector<BillCalculator::TDiscountResult>::iterator itDiscounts;
				for (itDiscounts = SubItem->BillCalcResult.Discount.begin(); itDiscounts != SubItem->BillCalcResult.Discount.end(); itDiscounts++)
				{
					UnicodeString Discription = ReceiptItemSpacer + itDiscounts->Description;
					SubTotal = DiscountTotals.find(Discription) != DiscountTotals.end() ? DiscountTotals[Discription] : Currency(0);
					SubTotal += itDiscounts->DiscountType == BillCalculator::dtDiscount ? -itDiscounts->Content : itDiscounts->Content;
					DiscountTotals[Discription] = SubTotal;
				}
			}
		}
	}

	if (Discounts)
	{
		//Display Discount Header in case of regular sale not for complimentary and nonchargable sale
		if (ThisInstruction->Caption != "" && PrintJob->Transaction->TypeOfSale == RegularSale)
		{
			pPrinter->Line->Columns[0]->Width = ThisInstruction->Caption.Length() + 1;
			pPrinter->Line->Columns[1]->Width = pPrinter->Width - ThisInstruction->Caption.Length() - 1;
			pPrinter->Line->Columns[0]->Text = ThisInstruction->Caption;
			pPrinter->Line->Columns[1]->Text = "";
			pPrinter->AddLine();
		}

		std::map<UnicodeString, Currency>::iterator itDiscountTotals = DiscountTotals.begin();
		for (itDiscountTotals = DiscountTotals.begin(); itDiscountTotals != DiscountTotals.end(); itDiscountTotals++)
		{
			bool MidPointRoundsDown = TGlobalSettings::Instance().MidPointRoundsDown == (itDiscountTotals->second >= 0);
			AnsiString Adjustment = CurrToStrF(
			RoundToNearest(itDiscountTotals->second, 0.01, MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals);

            if(Adjustment != "0.00")
            {
                pPrinter->Line->Columns[1]->Width = Adjustment.Length() + 1;
                pPrinter->Line->Columns[0]->Width = pPrinter->Width - Adjustment.Length() - 1;

                pPrinter->Line->Columns[0]->Text = itDiscountTotals->first;
                pPrinter->Line->Columns[1]->Text = Adjustment;
                pPrinter->AddLine();
            }
		}

		// Senior Citizen Discount (SCD) applied
		if( scdApplied )
		{
			printSCDSummary();
		}
	}
	else
	{
		Empty = true;
	}
}

bool TPrintSection::scdHasBeenApplied( BillCalculator::DISCOUNT_RESULT_LIST inDiscount )
{
	BillCalculator::DISCOUNT_RESULT_LIST::iterator drIT = inDiscount.begin();

	for( ; drIT != inDiscount.end(); drIT++ )
	{
		std::vector<UnicodeString> discountGroupList = drIT->DiscountGroupList;
		std::vector<UnicodeString>::iterator gIT = discountGroupList.begin();

		for( ; gIT != discountGroupList.end(); gIT++ )
		{
			if( *gIT == SCD_DISCOUNT_GROUP )
			{
				return true;
			}
		}
	}
	return false;
}

void TPrintSection::printSCDSummary()
{
	std::vector<AnsiString> scdSummary;

	populateSCDSummary( scdSummary );
	printSCDSummary(    scdSummary );

	scdSummary.clear();
}

void TPrintSection::populateSCDSummary( std::vector<AnsiString>& inSCDSummary )
{
	inSCDSummary.push_back( "Name     : ________________________________________" );
	inSCDSummary.push_back( "ID No    : ________________________________________" );
	inSCDSummary.push_back( "Signature: ________________________________________" );
}

void TPrintSection::printSCDSummary( std::vector<AnsiString> inSCDSummary )
{
	pPrinter->Line->Columns[0]->Text = "";
	pPrinter->Line->Columns[1]->Text = "";
	pPrinter->AddLine();

	std::vector<AnsiString>::iterator scdIT = inSCDSummary.begin();

	for( ; scdIT != inSCDSummary.end(); scdIT++ )
	{
		pPrinter->Line->Columns[0]->Text = *scdIT;
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintTotalDicountDetails(TReqPrintJob *PrintJob)
{
	Empty = true;
	WideString ReceiptItemSpacer = WideString("   ");
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	if (ThisInstruction->Caption != "")
	{
		pPrinter->Line->Columns[0]->Width = ThisInstruction->Caption.Length() + 1;
		pPrinter->Line->Columns[1]->Width = pPrinter->Width - ThisInstruction->Caption.Length() - 1;
		pPrinter->Line->Columns[0]->Text = ThisInstruction->Caption;
		pPrinter->Line->Columns[1]->Text = "";
		pPrinter->AddLine();
	}

	std::map<int, std::map<AnsiString, Currency> >PriorityDiscounts;
	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemComplete *Item = (TItemComplete*)WorkingOrdersList->Items[i];
		if (Item->DiscountSize() != 0)
		{
			std::vector<BillCalculator::TDiscountResult>::iterator itDiscounts;
			for (itDiscounts = Item->BillCalcResult.Discount.begin(); itDiscounts != Item->BillCalcResult.Discount.end(); itDiscounts++)
			{
				int Priority = itDiscounts->Priority;
				AnsiString Discription = itDiscounts->Description;
				std::map<AnsiString, Currency> Discounts = PriorityDiscounts[Priority];
				Currency ThisDiscountAmount = Discounts[Discription];

				ThisDiscountAmount += itDiscounts->DiscountType == BillCalculator::dtDiscount ? -itDiscounts->Content : itDiscounts->Content;
				Discounts[Discription] = ThisDiscountAmount;

				PriorityDiscounts[Priority] = Discounts;
			}
		}
	}

	std::map<int, std::map<AnsiString, Currency> >::iterator itPriorityDiscounts;
	for (itPriorityDiscounts = PriorityDiscounts.begin(); itPriorityDiscounts != PriorityDiscounts.end(); itPriorityDiscounts++)
	{
		std::map<AnsiString, Currency>Discounts = itPriorityDiscounts->second;
		std::map<AnsiString, Currency>::iterator itDiscounts;
		for (itDiscounts = Discounts.begin(); itDiscounts != Discounts.end(); itDiscounts++)
		{
			AnsiString Discription = ReceiptItemSpacer + itDiscounts->first;
			bool MidPointRoundsDown = TGlobalSettings::Instance().MidPointRoundsDown == (itDiscounts->second >= 0);
			AnsiString Adjustment = CurrToStrF(
			RoundToNearest(itDiscounts->second, 0.01, MidPointRoundsDown ),
			ffNumber,
			CurrencyDecimals);
            if(Adjustment != "0.00")
            {
                pPrinter->Line->Columns[1]->Width = Adjustment.Length() + 1;
                pPrinter->Line->Columns[0]->Width = pPrinter->Width - Adjustment.Length() - 1;

                pPrinter->Line->Columns[0]->Text = Discription;
                pPrinter->Line->Columns[1]->Text = Adjustment;
                pPrinter->AddLine();
                Empty = false;
            }

		}
	}
}

void TPrintSection::PrintTotalDicount(TReqPrintJob *PrintJob)
{
	Empty = true;
	if (TOrderUtils::TotalAdjustment(WorkingOrdersList.get()) != 0)
	{
		Empty = false;
		WideString ReceiptItemSpacer = WideString("   ");
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

		// Display the Items.
		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->FontInfo.Bold = false;
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

		AnsiString ItemName = ReceiptItemSpacer + ThisInstruction->Caption;
		AnsiString ItemPrice = CurrToStrF(
		//RoundToNearest(TOrderUtils::TotalAdjustment(WorkingOrdersList.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
		PrintJob->Transaction->Money.RoundedTotalAdjustment,
		ffNumber,
		CurrencyDecimals);

		pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;
		pPrinter->Line->Columns[0]->Text = ItemName;
		pPrinter->Line->Columns[1]->Text = ItemPrice;
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintCombinedTaxTotal(TReqPrintJob* PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		SubTotal += CurrentOrder->TotalTaxSides_BillCalc() - CurrentOrder->ServiceChargeTaxSides_BillCalc();
	}

	UnicodeString ItemName = ThisInstruction->Caption;

	UnicodeString ItemPrice = CurrToStrF(RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown), ffNumber, CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintTaxInclServiceChargeTaxTotal(TReqPrintJob* PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		SubTotal += CurrentOrder->TotalTaxSides_BillCalc();
	}

	UnicodeString ItemName = ThisInstruction->Caption;

	UnicodeString ItemPrice = CurrToStrF(RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown), ffNumber, CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintServiceCharge(TReqPrintJob* PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		SubTotal += CurrentOrder->ServiceChargeSides_BillCalc();

		//  this method is only to print ServiceCharge value, it does not have to print service charge tax value
		//        SubTotal += CurrentOrder->ServiceChargeSides_BillCalc() + CurrentOrder->ServiceChargeTaxSides_BillCalc();
	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
}

// SM Accreditation
// -------------------------------------------------------------------------------------------

void TPrintSection::PrintSalesTaxExempt(TReqPrintJob* PrintJob)
{
	Currency productTax = 0;
	Currency serviceCharge = 0;
	Currency serviceChargeTax = 0;
	Currency localTax = 0;
	Currency profitTax = 0;
	Currency productTaxSides = 0;
	Currency serviceChargeSides = 0;
	Currency serviceChargeTaxSides = 0;
	Currency localTaxSides = 0;
    Currency profitTaxSides = 0;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		productTax = getProductTax(CurrentOrder);
		localTax = getLocalTax(CurrentOrder);
		profitTax = getProfitTax(CurrentOrder);
		serviceCharge = getServiceCharge(CurrentOrder);
		serviceChargeTax = getServiceChargeTax(CurrentOrder);


        if(CurrentOrder->BillCalcResult.PriceTaxExempt && productTax == 0)
        {
            SubTotal += CurrentOrder->BillCalcResult.FinalPrice - serviceCharge - serviceChargeTax - localTax - profitTax;
        }

		for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);
			productTaxSides = getProductTax((TItemMinorComplete*)SubOrderImage);
			serviceChargeSides = getServiceCharge((TItemMinorComplete*)SubOrderImage);
			serviceChargeTaxSides = getServiceChargeTax((TItemMinorComplete*)SubOrderImage);
			localTaxSides = getLocalTax((TItemMinorComplete*)SubOrderImage);
            profitTaxSides = getProfitTax((TItemMinorComplete*)SubOrderImage);

            if(SubOrderImage->BillCalcResult.PriceTaxExempt && productTaxSides == 0)
            {
                SubTotal +=  SubOrderImage->BillCalcResult.FinalPrice - serviceChargeSides - serviceChargeTaxSides - localTaxSides - profitTaxSides;
            }
	    }
    }
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintSalesTaxable(TReqPrintJob* PrintJob)
{
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localTax = 0;
    Currency profitTax = 0;
    Currency productTaxSides = 0;
    Currency serviceChargeSides = 0;
    Currency serviceChargeTaxSides = 0;
    Currency localTaxSides = 0;
    Currency profitTaxSides = 0;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;
	Currency TaxPercentage = 0;

    for (int i = 0; i < WorkingOrdersList->Count; i++)
    {
        TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
        productTax = getProductTax(CurrentOrder);
        localTax = getLocalTax(CurrentOrder);
        profitTax = getProfitTax(CurrentOrder);
        serviceCharge = getServiceCharge(CurrentOrder);
        serviceChargeTax = getServiceChargeTax(CurrentOrder);


		if(!CurrentOrder->BillCalcResult.PriceTaxExempt && productTax != 0)
		{
			if((TOrderUtils::TotalAdjustment(WorkingOrdersList.get()) != 0) && (TGlobalSettings::Instance().ReCalculateTaxPostDiscount == true))
			{
				SubTotal += CurrentOrder->BillCalcResult.FinalPrice -
				productTax - serviceCharge - serviceChargeTax - localTax - profitTax;
			}
			else
			{
				SubTotal += CurrentOrder->BillCalcResult.BasePrice * CurrentOrder->GetQty();
			}
		}

		for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);
			productTaxSides = getProductTax((TItemMinorComplete*)SubOrderImage);
			serviceChargeSides = getServiceCharge((TItemMinorComplete*)SubOrderImage);
			serviceChargeTaxSides = getServiceChargeTax((TItemMinorComplete*)SubOrderImage);
			localTaxSides = getLocalTax((TItemMinorComplete*)SubOrderImage);
            profitTaxSides = getProfitTax((TItemMinorComplete*)SubOrderImage);

			if(!SubOrderImage->BillCalcResult.PriceTaxExempt && productTaxSides != 0)
			{
				if((TOrderUtils::TotalAdjustment(WorkingOrdersList.get()) != 0) && (TGlobalSettings::Instance().ReCalculateTaxPostDiscount == true))
				{
					SubTotal += SubOrderImage->BillCalcResult.FinalPrice -
					productTaxSides - serviceChargeSides - serviceChargeTaxSides - localTaxSides - profitTaxSides;
				}
				else
				{
					SubTotal += SubOrderImage->BillCalcResult.BasePrice * SubOrderImage->GetQty();
				}
			}
		}
	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintSalesTax(TReqPrintJob* PrintJob)
{
	Currency productTax = 0;
	Currency productTaxSides = 0;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		productTax = getProductTax(CurrentOrder);

		if(!CurrentOrder->BillCalcResult.PriceTaxExempt)
		{
			SubTotal += productTax;
		}

		for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);
			productTaxSides = getProductTax((TItemMinorComplete*)SubOrderImage);

			if(!SubOrderImage->BillCalcResult.PriceTaxExempt)
			{
				SubTotal += productTaxSides;
			}
		}
	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;
	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
	/*if(TGlobalSettings::Instance().HideSalesTaxWhenValueIsZero) {
		if(productTax !=0) {
			pPrinter->Line->Columns[0]->Text = ItemName;
			pPrinter->Line->Columns[1]->Text = ItemPrice;
			pPrinter->AddLine();
		}
	}
	else
	{
		pPrinter->Line->Columns[0]->Text = ItemName;
		pPrinter->Line->Columns[1]->Text = ItemPrice;
		pPrinter->AddLine();
	}*/
}

void TPrintSection::PrintProfitTax(TReqPrintJob* PrintJob)
{

    std::map<UnicodeString, Currency> TaxesMap;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

    for (int i = 0; i < WorkingOrdersList->Count; i++)
    {
        TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];

      for (std::vector<BillCalculator::TTaxResult>::iterator taxIt = CurrentOrder->BillCalcResult.Tax.begin(); taxIt != CurrentOrder->BillCalcResult.Tax.end();taxIt++)
        {
            if (taxIt->Value != 0 &&  taxIt->TaxType == TTaxType::ttProfit)
            {
                if (TaxesMap.count(taxIt->Name) == 0 )
                {
                    TaxesMap[taxIt->Name] = taxIt->Value;
                }
                else
                {
                    TaxesMap[taxIt->Name] += taxIt->Value;
                }
            }
        }


     for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
    {
        TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);

        for (std::vector<BillCalculator::TTaxResult>::iterator taxIt = SubOrderImage->BillCalcResult.Tax.begin(); taxIt != SubOrderImage->BillCalcResult.Tax.end(); taxIt++)
        {
            if (taxIt->Value != 0  && taxIt->TaxType == TTaxType::ttProfit)
            {
                if (TaxesMap.count(taxIt->Name) == 0 )
                {
                    TaxesMap[taxIt->Name] = taxIt->Value;
                }
                else
                {
                    TaxesMap[taxIt->Name] += taxIt->Value;
                }
            }
        }

    }
}
    for (std::map<UnicodeString, Currency>::iterator taxesMapIt = TaxesMap.begin(); taxesMapIt != TaxesMap.end(); taxesMapIt++)
    {

    UnicodeString ItemName = taxesMapIt->first;

	UnicodeString ItemPrice = CurrToStrF(
                                    RoundToNearest(taxesMapIt->second, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
                                    ffNumber,
                                    CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;

	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

    pPrinter->Line->Columns[0]->Text = ItemName;

    pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() :ItemPrice;

    pPrinter->AddLine();

    }
}

void TPrintSection::PrintLocalTax(TReqPrintJob* PrintJob)

{
	Currency localTax = 0;
	Currency localTaxSides = 0;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		localTax = getLocalTax(CurrentOrder);

		if(!CurrentOrder->BillCalcResult.PriceTaxExempt)
		{
			SubTotal += localTax;
		}

		for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);
			localTaxSides = getLocalTax((TItemMinorComplete*)SubOrderImage);

			if(!SubOrderImage->BillCalcResult.PriceTaxExempt)
			{
				SubTotal += localTaxSides;
			}
		}
	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintServiceChargeTax(TReqPrintJob* PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		SubTotal += CurrentOrder->ServiceChargeTaxSides_BillCalc();
	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintServiceChargePlusServiceChargeTax(TReqPrintJob* PrintJob)
{
	Currency ServiceCharge = 0;
	Currency ServiceChargeTax = 0;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		ServiceCharge = CurrentOrder->ServiceChargeSides_BillCalc();
		ServiceChargeTax = CurrentOrder->ServiceChargeTaxSides_BillCalc();
		SubTotal += ServiceCharge + ServiceChargeTax;
	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintZeroRated(TReqPrintJob* PrintJob)
{
    Currency productTax = 0;
    Currency serviceCharge = 0;
    Currency serviceChargeTax = 0;
    Currency localTax = 0;
    Currency profitTax = 0;
    Currency productTaxSides = 0;
    Currency serviceChargeSides = 0;
    Currency serviceChargeTaxSides = 0;
    Currency localTaxSides = 0;
    Currency profitTaxSides = 0;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;

	Currency SubTotal = 0;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		productTax = getProductTax(CurrentOrder);
		localTax = getLocalTax(CurrentOrder);
		profitTax = getProfitTax(CurrentOrder);
		serviceCharge = getServiceCharge(CurrentOrder);
		serviceChargeTax = getServiceChargeTax(CurrentOrder);

		if(!CurrentOrder->BillCalcResult.PriceTaxExempt && productTax == 0)
		{
			SubTotal += CurrentOrder->BillCalcResult.FinalPrice - serviceCharge - serviceChargeTax - localTax - profitTax;
		}

		for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);
			productTaxSides = getProductTax((TItemMinorComplete*)SubOrderImage);
			serviceChargeSides = getServiceCharge((TItemMinorComplete*)SubOrderImage);
			serviceChargeTaxSides = getServiceChargeTax((TItemMinorComplete*)SubOrderImage);
			localTaxSides = getLocalTax((TItemMinorComplete*)SubOrderImage);
            profitTaxSides = getProfitTax((TItemMinorComplete*)SubOrderImage);

			if(!SubOrderImage->BillCalcResult.PriceTaxExempt && productTaxSides == 0)
			{
				SubTotal +=  SubOrderImage->BillCalcResult.FinalPrice - serviceChargeSides - serviceChargeTaxSides - localTaxSides - profitTaxSides;
			}
		}


	}
	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
	pPrinter->AddLine();
}

Currency TPrintSection::getProductTax(TItemMinorComplete* item)
{
	return item->TotalTaxType_BillCalc( TTaxType::ttSale );
}

Currency TPrintSection::getLocalTax(TItemMinorComplete* item)
{
	return item->TotalTaxType_BillCalc( TTaxType::ttLocal );
}

Currency TPrintSection::getProfitTax(TItemMinorComplete* item)
{
    return item->TotalTaxType_BillCalc( TTaxType::ttProfit );
}


Currency TPrintSection::getServiceCharge(TItemMinorComplete* item)
{
	return item->BillCalcResult.ServiceCharge.Value;
}

Currency TPrintSection::getServiceChargeTax(TItemMinorComplete* item)
{
	return item->BillCalcResult.ServiceCharge.TaxValue;
}


// -------------------------------------------------------------------------------------------

void TPrintSection::PrintTotalEx(TReqPrintJob *PrintJob)
{
	// Print Total Excluding Discounts.
	bool IncludeSurcharges = false;
	TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.begin())
	{
		if (i->Instruction == epofiPrintPaymentSurcharges || i->Instruction == epofiPrintTotalDiscountsDetails || i->Instruction == epofiPrintTotalDiscountsName)
		{
			IncludeSurcharges = true;
		}
		advance(i, -1);
		if (i->Instruction == epofiPrintTotalEx || i->Instruction == epofiPrintTotal)
		{
			break;
		}
	}

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;
	Currency SubTotal = 0;//TOrderUtils::TotalPriceSides(WorkingOrdersList.get());

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		SubTotal += RoundToNearest(CurrentOrder->TotalPriceSides(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	if (IncludeSurcharges)
	{
		SubTotal += PrintJob->Transaction->Money.PaymentSurcharges;
	}

	UnicodeString ItemPrice = CurrToStrF(
	RoundToNearest(SubTotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
	ffNumber,
	CurrencyDecimals);

	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;
	pPrinter->Line->Columns[0]->Text = ItemName;
	pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
	pPrinter->AddLine();
}

void TPrintSection::PrintGrandTotal(TReqPrintJob *pj)
{
	TSectionInstructStorage::const_iterator i = ThisInstruction;
	Currency subtotal = pj->Transaction->Money.RoundedGrandTotal;
	UnicodeString GrandTotal = CurrencyString + CurrToStrF(RoundToNearest( subtotal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
	                                                        ffNumber,CurrencyDecimals);
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[1]->Width = GrandTotal.Length() + 1;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width - GrandTotal.Length() - 1;
	pPrinter->Line->Columns[0]->Text = ThisInstruction->Caption;
	pPrinter->Line->Columns[1]->Text = (pj->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : GrandTotal;
	pPrinter->AddLine();
	//Currency adjustment = RoundToNearest(pj->Transaction->Money.RoundingAdjustment, 0.01, TGlobalSettings::Instance().MidPointRoundsDown );
        Currency adjustment = pj->Transaction->Money.RoundingAdjustment;
	if(adjustment !=0)
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->FontInfo.Bold = false;
		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
		pPrinter->Line->Columns[1]->Alignment = taRightJustify;
		pPrinter->Line->Columns[1]->Width = GrandTotal.Length() + 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - GrandTotal.Length() - 1;
		pPrinter->Line->Columns[0]->Text = "Price Adjustment";
		pPrinter->Line->Columns[1]->Text = (pj->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() :
		CurrencyString + CurrToStrF(adjustment,ffNumber,CurrencyDecimals);
		pPrinter->AddLine();
	}

}

void TPrintSection::PrintSeparatedTaxTotals(TReqPrintJob *PrintJob)
{
	std::map<UnicodeString, Currency> TaxesMap;

	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemMinorComplete *CurrentOrder = (TItemMinorComplete*)WorkingOrdersList->Items[i];
		for (std::vector<BillCalculator::TTaxResult>::iterator taxIt = CurrentOrder->BillCalcResult.Tax.begin(); taxIt != CurrentOrder->BillCalcResult.Tax.end(); taxIt++)
		{
			if (taxIt->Value != 0 || TGlobalSettings::Instance().PrintZeroTaxes)
			{
				if (TaxesMap.count(taxIt->Name) == 0)
				{
					TaxesMap[taxIt->Name] = taxIt->Value;
				}
				else
				{
					TaxesMap[taxIt->Name] += taxIt->Value;
				}
			}
		}
		if (CurrentOrder->BillCalcResult.ServiceCharge.TaxValue != 0 || TGlobalSettings::Instance().PrintZeroTaxes)
		{
			if (TaxesMap.count("Service Charge Tax") == 0)
			{
				TaxesMap["Service Charge Tax"] = CurrentOrder->BillCalcResult.ServiceCharge.TaxValue;
			}
			else
			{
				TaxesMap["Service Charge Tax"] += CurrentOrder->BillCalcResult.ServiceCharge.TaxValue;
			}
		}
		for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrderImage = CurrentOrder->SubOrders->SubOrderGet(i);
			for (std::vector<BillCalculator::TTaxResult>::iterator taxIt = SubOrderImage->BillCalcResult.Tax.begin(); taxIt != SubOrderImage->BillCalcResult.Tax.end(); taxIt++)
			{
				if (taxIt->Value != 0 || TGlobalSettings::Instance().PrintZeroTaxes)
				{
					if (TaxesMap.count(taxIt->Name) == 0)
					{
						TaxesMap[taxIt->Name] = taxIt->Value;
					}
					else
					{
						TaxesMap[taxIt->Name] += taxIt->Value;
					}
				}
			}
			if (SubOrderImage->BillCalcResult.ServiceCharge.TaxValue != 0 || TGlobalSettings::Instance().PrintZeroTaxes)
			{
				if (TaxesMap.count("Service Charge Tax") == 0)
				{
					TaxesMap["Service Charge Tax"] = SubOrderImage->BillCalcResult.ServiceCharge.TaxValue;
				}
				else
				{
					TaxesMap["Service Charge Tax"] += SubOrderImage->BillCalcResult.ServiceCharge.TaxValue;
				}
			}
		}
	}

	for (std::map<UnicodeString, Currency>::iterator taxesMapIt = TaxesMap.begin(); taxesMapIt != TaxesMap.end(); taxesMapIt++)
	{
		UnicodeString ItemName = taxesMapIt->first;

		UnicodeString ItemPrice = CurrToStrF(
		RoundToNearest(taxesMapIt->second, 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
		ffNumber,
		CurrencyDecimals);

		pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

		pPrinter->Line->Columns[0]->Text = ItemName;
		pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
		pPrinter->AddLine();
	}

	//	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	//
	//	// Display the Items.
	//	pPrinter->Line->ColCount = 2;
	//	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	//	pPrinter->Line->FontInfo.Bold = false;
	//	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	//	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	//
	//	UnicodeString ItemName = ThisInstruction->Caption;
	//
	//	Currency SubTotal = PrintJob->Transaction->Money.GrandTotalGSTContent;
	//
	//	UnicodeString ItemPrice = CurrToStrF(SubTotal, ffNumber, CurrencyDecimals);
	//
	//	pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
	//	pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;
	//
	//	pPrinter->Line->Columns[0]->Text = ItemName;
	//	pPrinter->Line->Columns[1]->Text = ItemPrice;
	//	pPrinter->AddLine();
}

void TPrintSection::PrintItemsTotal(TReqPrintJob *PrintJob)
{
	bool IncludeDiscounts = true;
	TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.end())
	{
		if (i->Instruction == epofiPrintTotalDiscount || i->Instruction == epofiPrintTotalDiscountsDetails || i->Instruction == epofiPrintTotalDiscountsName)
		{
			IncludeDiscounts = false;
		}
		advance(i, 1);
	}

	TList *OrderBundle;
	try
	{
		OrderBundle = new TList;
		TDateTime TimeStamp = Now();
		for (int i = 0; i < WorkingOrdersList->Count; )
		{
			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (TimeStamp > CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}

			TOrderBundle *TempBundle = new TOrderBundle();
			TempBundle->BundleOrders(PrintJob, WorkingOrdersList.get(), i,Format);

			OrderBundle->Add(TempBundle);
		}

		for (int j = 0; j < OrderBundle->Count; j++)
		{
			TOrderBundle *CurrentOrderBundle = (TOrderBundle*)OrderBundle->Items[j];
			TOrderBundle *NextOrderBundle = NULL;

			TPrintInfo TotalFontInfo;
			if (PrintJob->JobType == pjKitchen)
			{
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
			}
			else
			{
				TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
				TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
				TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
				TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
			}

			pPrinter->Line->FontInfo = TotalFontInfo;

			// Display the Items.
			pPrinter->Line->ColCount = 2;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			pPrinter->Line->FontInfo.Bold = false;
			pPrinter->Line->Columns[1]->Alignment = taRightJustify;
			pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

			UnicodeString ItemName = CurrentOrderBundle->ItemLine;
			UnicodeString ItemPrice = CurrToStrF(
			RoundToNearest(TOrderUtils::TotalPriceSides(CurrentOrderBundle->BundledOrders.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
			ffNumber,
			CurrencyDecimals);

			if (IncludeDiscounts)
			{
				ItemPrice = CurrToStrF(
				RoundToNearest(TOrderUtils::TotalPriceAdjustmentSides(CurrentOrderBundle->BundledOrders.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals);
			}

			if (!CurrentOrderBundle->CancelLine.IsEmpty())
			{
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->CancelLine;
                pPrinter->Line->Columns[1]->Text = "";
				pPrinter->AddLine();
			}
            int item_len = GetItemLength(CurrentOrderBundle->ItemLine, CurrentOrderBundle->ItemLine.Length());

			pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
            if(item_len > 0)
            {
               pPrinter->Line->Columns[0]->Width = pPrinter->Width -  pPrinter->Line->Columns[1]->Width - item_len;
            }
            else
            {
               pPrinter->Line->Columns[0]->Width = pPrinter->Width -  ItemPrice.Length() - 1;
            }
			pPrinter->Line->Columns[0]->Text = ItemName;
			pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
			pPrinter->AddLine();

            PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);
			// change.........
			/*TStringList *NameList = new TStringList;
						PrepareName(pPrinter->Line->Columns[0]->Width,ItemName,NameList);

						pPrinter->Line->Columns[0]->Text = NameList->Strings[0];
						pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
						pPrinter->AddLine();
						for(int i = 1; i < NameList->Count; i++)
						{
							pPrinter->Line->Columns[0]->Text = NameList->Strings[i];
							pPrinter->Line->Columns[1]->Text = "";
							pPrinter->AddLine();
						}*/
			//            if(CurrentOrderBundle->ItemWeight != "" )
			//              {
			//				 pPrinter->Line->ColCount = 1;
			//		 pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			//		 pPrinter->Line->FontInfo.Bold = false;
			//		 pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
			//                 pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemWeight;
			//                 pPrinter->AddLine();
			//              }

			//pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;

			if (CurrentOrderBundle->SubItems.size() > 0)
			{
				if (!CurrentOrderBundle->SideLine.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SideLine;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}

				for (int k = 0; k < CurrentOrderBundle->SubItems.size(); k++)
				{
					if (PrintJob->JobType == pjKitchen)
					{
						pPrinter->Line->FontInfo = CurrentOrderBundle->SubItems[k].FontInfo;
					}
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SubItems[k].Caption;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();

                  if( CurrentOrderBundle->SubItems[k].isManuallyWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
                    {
                        AddManuallyEnteredLineInReceipt(pPrinter);
                    }
				}

				if (!CurrentOrderBundle->MixLine.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->MixLine;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}
			}
			TPtrSectionInstructStorage::iterator ptrSubInstruction = ThisInstruction->SubInstructions.begin();
			while (ptrSubInstruction != ThisInstruction->SubInstructions.end())
			{
				TPSectionInstruction *itInstruction = *ptrSubInstruction;
				// Get The Iterator that iterator points too.
				// Process the Note & Options only once.
				if (((itInstruction->Instruction == epofiPrintItemNote) || (itInstruction->Instruction == epofiPrintItemOptions)) && CurrentOrderBundle->BundledOrders->Count > 0)
				{
					std::auto_ptr<TList>SingleOrders(new TList);
					SingleOrders->Add(CurrentOrderBundle->BundledOrders->Items[0]);
					TPrintSection NewSection(pPrinter, SingleOrders.get(), MasterInstructionsList, itInstruction,Format);
					NewSection.ProcessSection(PrintJob);
				}
				else
				{
					TPrintSection NewSection(pPrinter, CurrentOrderBundle->BundledOrders.get(), MasterInstructionsList, itInstruction,Format);
					NewSection.ProcessSection(PrintJob);
				}
				advance(ptrSubInstruction, 1);
			}
		}
	}
	__finally
	{
		while (OrderBundle->Count != 0)
		{
			delete(TOrderBundle*)OrderBundle->Items[0];
			OrderBundle->Delete(0);
		}
		delete OrderBundle;
	}
}


int TPrintSection::GetItemLength(UnicodeString itemname, int itemlength)
{
    int length = 0;
    std::string item = itemname.t_str();
    int len = item.length();

    if(len > itemlength)
    {
        length = len - itemlength;
    }
    return length;
}



void TPrintSection::PrepareName(int col_len, UnicodeString str_name, TStringList *name_lst)
{
	AnsiString strname = str_name, spaces = " ";
	UnicodeString temp_data , spacen;
	int name_len = str_name.Length();
	int data_strtidx = 0;
	int cal_len = col_len;
    int trk_spc = 0 , name_strt = 0, name_strt_aux=0;
    int row1 = 0, row2 = 0, space = 0;
    int iterator = 0;
    bool no_space = true;

    char* temp_char = new char[strname.Length()+1];
	strcpy(temp_char, strname.c_str());

	temp_data = str_name.SubString(data_strtidx, col_len);

	while(true)
	{
		if (name_len > col_len)
		{
			for ( trk_spc = 0 ; trk_spc < col_len ; trk_spc++)             // track of space between quantity and item-name
			{
				name_strt = trk_spc + 1;
				if ((temp_char[trk_spc] == check_space) && (temp_char[name_strt] != check_space))
				{
                    name_strt_aux = name_strt;
                    break;
				}
			}
			while(col_len > name_strt_aux)
            {
                if(temp_char[(data_strtidx + col_len)-1] == check_space)   // finding space within first line
                {
                    temp_data = str_name.SubString(data_strtidx, col_len) + "\n";
					data_strtidx = data_strtidx + col_len;
					name_len -= col_len;
                    name_strt = 1;
					col_len = cal_len;                                     // again add col value..
                    no_space = false;
                    if (row1 != 0)
                    {
                        for (iterator = 0 ; iterator < name_strt_aux-1 ; iterator++)  // name alignment
                        {
                            spacen = spacen + spaces;
                        }
                        temp_data = spacen + temp_data;
                        row2++ ;
                    }
					name_lst->Add(temp_data);
                    if (name_len > col_len - 2)
                    {
                        row1++;
                        no_space = true;
                        continue ;
                    }
                    else
                    {
                        break;
                    }
                }
                col_len = col_len - 1;
			}
            if (no_space)                                                   // String larger than column length and no space
            {
                col_len = cal_len;

                if (row1 != 0)                                             // One or More Substrings alraedy formed
                {
                    if (temp_char[data_strtidx-1] == check_space)          // Name Alignment
                    {
                        for (iterator = 0 ; iterator < name_strt_aux-1 ; iterator++)
                        {
                            space++;
                            spacen = spacen + spaces;
                        }
                    }
                    else
                    {
                        for (iterator = 0 ; iterator < name_strt_aux ; iterator++)
                        {
                            spacen = spacen + spaces;
                        }
                    }
                        row2++;
                        temp_data = str_name.SubString(data_strtidx, col_len);
                        temp_data = spacen + temp_data;
                        name_len -= col_len - name_strt_aux;
                        data_strtidx = data_strtidx + col_len-1-name_strt_aux;
                        name_lst->Add(temp_data);
                }
                else                                                       // first substring larger than column
                {
                    temp_data = str_name.SubString(data_strtidx, col_len);
                    temp_data = spacen + temp_data;
                    name_len -= col_len;
                    data_strtidx = data_strtidx + col_len  ;
                    name_lst->Add(temp_data);
                }
            }
		}
		else
		{
            if (row2 == 0)                                              // Name Alignment last row if one or zero rows above
            {
                for (iterator = 0 ; iterator < name_strt_aux ; iterator++)
                {
                    spacen = spacen + spaces;
                }
            }
            if (space != 0)                                               // Name Alignment last row if two rows above
            {
                spacen = spacen + spaces;
            }
            if ((temp_char[cal_len-2] == check_space) || (temp_char[cal_len-3] == check_space))
            {
              temp_data = str_name.SubString(data_strtidx+1 ,name_len+2);
            }
            if ((temp_char[cal_len-1] == check_space))
            {
              temp_data = str_name.SubString(data_strtidx-1 ,name_len+2);
            }
            else
            {
                if (row2 != 0)
                {
                    temp_data = str_name.SubString(data_strtidx ,name_len+2);
                }
                else
                {
                    if (temp_char[data_strtidx-1] == check_space)
                    {
                        temp_data = str_name.SubString(data_strtidx+1, name_len+2);
                    }
                    else
                    {
                        temp_data = str_name.SubString(data_strtidx-1, name_len+2);
                    }
                }
            }
            temp_data = spacen + temp_data;
			name_lst->Add(temp_data);
			break;
		}
	}
}



void TPrintSection::PrintOptionsSection(TOrderBundle *CurrentOrderBundle, TPrintFormat *pPrinter)
{
	for (int k = 0; k < CurrentOrderBundle->Options->Count; k++)
	{
		TItemOption *Option = CurrentOrderBundle->Options->OptionGet(k);
		UnicodeString BulletOption = Option->IsPlus ? "+ " : "- ";
		pPrinter->Line->FontInfo = Option->FontInfo;
		pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
		pPrinter->Line->Columns[0]->Text = Format.BulletOpt + BulletOption + Option->KitchenName;
		pPrinter->Line->Columns[1]->Text = "";
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintSubordersSection(TOrderBundle *CurrentOrderBundle, TPrintFormat *pPrinter, bool groupItemSides)
{
    UnicodeString SideCaption;
    UnicodeString SetMenuItemSpacer = " ";


	if (CurrentOrderBundle->SubItems.size() > 0)
	{
		if (!CurrentOrderBundle->SideLine.IsEmpty())
		{
			pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SideLine;
			pPrinter->Line->Columns[1]->Text = "";
			pPrinter->AddLine();
		}

        if(groupItemSides)
        {
            for (int k = 0; k < CurrentOrderBundle->SubItems.size(); k++)
            {
                pPrinter->Line->FontInfo = CurrentOrderBundle->SubItems[k].FontInfo;
                pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
                SideCaption = CurrentOrderBundle->SubItems[k].SideBullets + CurrentOrderBundle->SubItems[k].SideQuantity + SetMenuItemSpacer + CurrentOrderBundle->SubItems[k].Caption;
                pPrinter->Line->Columns[0]->Text =  SideCaption;
                pPrinter->Line->Columns[1]->Text = "";
                pPrinter->AddLine();
            }
        }
        else
        {
            for (int k = 0; k < CurrentOrderBundle->SubItems.size(); k++)
            {
                pPrinter->Line->FontInfo = CurrentOrderBundle->SubItems[k].FontInfo;
                pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
                pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SubItems[k].Caption;
                pPrinter->Line->Columns[1]->Text = "";
                pPrinter->AddLine();
            }
        }

		if (!CurrentOrderBundle->MixLine.IsEmpty())
		{
			pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->MixLine;
			pPrinter->Line->Columns[1]->Text = "";
			pPrinter->AddLine();
		}
	}
}

// write own functionality.........

// write own functionality.........
void TPrintSection::PrintTwoLinesItemName(TReqPrintJob *PrintJob)
{
	if (WorkingOrdersList->Count == 0)
	{
		Empty = true;
	}
	else
	{
		bool IncludeNote;
		bool IncludeSides;
		bool IncludeOptions;

		TSectionInstructStorage::iterator i = ThisInstruction;
		for ( ; i != MasterInstructionsList.end(); i++) {
			IncludeNote = i->Instruction != epofiPrintItemNote;
			IncludeSides = i->Instruction != epofiPrintItemSides;
			IncludeOptions = i->Instruction != epofiPrintItemOptions;
		}

		WideString QtyMultiplier1 = WideString("x ");
		WideString QtyMultiplier10 = WideString("x");
		WideString Spacer1 = WideString(" ");
		WideString SeatStart = WideString("(");
		WideString SeatEnd = WideString(")");
		WideString SeatSpacer = WideString(", ");

		UnicodeString CancelLine = "";

		std::vector<TItemSubSection>SubItems;
		std::vector<TItemSubSection>Options;

		TDateTime TimeStamp = Now();

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *CurrentOrder =
			(TItemComplete *)WorkingOrdersList->Items[i];
			Currency CountQuantity = CurrentOrder->GetQty();

			for (int j = 0; j < CountQuantity; j++) {
				WideString ItemLine = "";
				WideString ItemLineSize = "";
				WideString SetMenuHeader = "";
				WideString MixLine = "";
				WideString SideLine = "";
				WideString NoteLine = "";
				WideString SetMenuItemSpacer = "";
				UnicodeString SizeName = "";
				int SetMenuGroup = 0;
				SubItems.clear();
				Options.clear();

				if (CurrentOrder->SetMenuMask != 0 && CurrentOrder->SetMenuGroup != 0)
				{
					if (Format.KeyItemHeader[CurrentOrder->SetMenuGroup] != WideString(""))
					{
						SetMenuHeader = "[" + Format.KeyItemHeader[CurrentOrder->SetMenuGroup] + "]";
					}
					SetMenuItemSpacer = Format.BulletSetMenuItem;
				}

				if (CurrentOrder->OrderType == CanceledOrder)
				{
					CancelLine = "!!!! CANCEL ORDERS !!!!";
				}
				else if (CurrentOrder->OrderType == CreditNonExistingOrder)
				{
					CancelLine = "!!!! REFUND ORDERS !!!!";
				}

				if (TimeStamp > CurrentOrder->TimeStamp)
				{
					TimeStamp = CurrentOrder->TimeStamp;
				}

				ItemLine = SetMenuItemSpacer;
				if (PrintJob->JobType == pjKitchen)
				{
					ItemLineSize = CurrentOrder->SizeKitchenName;
				}
				else
				{
					ItemLineSize = CurrentOrder->Size;
				}
				if (UpperCase(ItemLineSize) == "DEFAULT")
				ItemLineSize = "";

				ItemLine = ItemLine
				+ ItemLineSize
				+ Spacer1
				+ CurrentOrder->ItemKitchenName;

				int SeatsListWidth = 0;
				UnicodeString SeatsLine = ""; ;

				UnicodeString SeatTypeText = ""; // Cash Sale";
				UnicodeString Identifier = "";
				UnicodeString QtyText = "";

				switch(CurrentOrder->TabType)
				{
				case TabNormal:
				case TabStaff:
				case TabMember:
				case TabWeb:
					{
						SeatTypeText = ""; // Tab ";
						Identifier = ""; // PrevOrder->TabName;
						break;
					}
				case TabTableSeat:
					{
						// Cant use Tab name here it could be to long.
						SeatTypeText = "";
						Identifier = IntToStr(CurrentOrder->SeatNo);
						break;
					}
				case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
				default:
					// SeatTypeText = "Cash Sale";
					SeatTypeText = ""; // Cash Sale";
					Identifier = "";
					QtyText = "";
					break;
				}

				if (SeatTypeText != "" || Identifier != "")
				{
					SeatsLine = SeatStart + Identifier + SeatEnd;
				}

				if (CurrentOrder->SubOrders->Count > 0)
				{
					// Print Line
					// Only Show Mix if Items is Orders from the Drinks Menu.
					if (CurrentOrder->ItemType == eDrinksItem)
					{
						MixLine = "";
						if (UpperCase(CurrentOrder->Size) != "DEFAULT")
						MixLine = Format.MixHeader + CurrentOrder->MasterContainer;
					}
					else
					{
						SideLine = Format.SideHeader;
					}

					for (int i = 0; i < CurrentOrder->SubOrders->Count; i++)
					{
						if (UpperCase(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->Size) != "DEFAULT")
						{
							TItemCompleteSub *ItemCompleteSub = (TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]);
							if (PrintJob->JobType == pjKitchen)
							{
								SizeName =ItemCompleteSub->SizeKitchenName;
							}
							else
							{
								SizeName =ItemCompleteSub->Size;
							}
							TItemSubSection SubOrder;
							SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + SizeName + Spacer1 + ItemCompleteSub->ItemKitchenName;
							if (PrintJob->JobType == pjKitchen)
							{
								SubOrder.FontInfo = ItemCompleteSub->FontInfo;
							}

                           if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                            {
                               SubOrder.isManuallyWeight=true;
                            }
                            SubItems.push_back(SubOrder);
						}
						else
						{
							TItemCompleteSub *ItemCompleteSub = (TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]);
							TItemSubSection SubOrder;
							SubOrder.Caption = SetMenuItemSpacer + Format.BulletSide + ItemCompleteSub->ItemKitchenName;
							if (PrintJob->JobType == pjKitchen)
							{
								SubOrder.FontInfo = ItemCompleteSub->FontInfo;
							}
						   if(((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                            {
                               SubOrder.isManuallyWeight=true;
                            }
                          	SubItems.push_back(SubOrder);
                       }
					}
				}

				for (int i = 0; i < CurrentOrder->OptionsSelected->Count; i++)
				{
					TItemSubSection Option;
					UnicodeString BulletOption = CurrentOrder->OptionsSelected->OptionGet(i)->IsPlus ? "+ " : "- ";
					Option.Caption = SetMenuItemSpacer + Format.BulletOpt + BulletOption + CurrentOrder->OptionsSelected->OptionGet(i)->KitchenName;
					if (PrintJob->JobType == pjKitchen)
					{
						Option.FontInfo = CurrentOrder->OptionsSelected->OptionGet(i)->FontInfo;
					}
					Options.push_back(Option);
				}

				if (CurrentOrder->Note != "")
				{
					NoteLine = Format.NoteHeader + CurrentOrder->Note;
				}

				TPrintInfo TotalFontInfo;
				if (PrintJob->JobType == pjKitchen)
				{
					TotalFontInfo.Bold = CurrentOrder->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
					TotalFontInfo.Underlined = CurrentOrder->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
					TotalFontInfo.Width = ((CurrentOrder->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
					TotalFontInfo.Height = ((CurrentOrder->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
					TotalFontInfo.Colour = ((CurrentOrder->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
					TotalFontInfo.Font = ((CurrentOrder->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
				}
				else
				{
					TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
					TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
					TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
					TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
					TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
					TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
				}

				pPrinter->Line->FontInfo = TotalFontInfo;
				// Print This Item.
				pPrinter->Line->ColCount = 1;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

				if (CancelLine != "")
				{
					pPrinter->Line->Columns[0]->Text = CancelLine;
					pPrinter->AddLine();
				}

				if (!SetMenuHeader.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = SetMenuHeader;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->AddLine();
				}

				pPrinter->Line->ColCount = 2;

				SeatsListWidth = SeatsLine.Length();
				if (SeatsListWidth > pPrinter->Width / 3)
				{
					SeatsListWidth = pPrinter->Width / 3;
				}

				pPrinter->Line->Columns[0]->Width = pPrinter->Width - SeatsListWidth;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				pPrinter->Line->Columns[1]->Width = SeatsListWidth;
				pPrinter->Line->Columns[1]->Alignment = taRightJustify;
				pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
				//pPrinter->Line->Columns[0]->Text = ItemLine;
				//pPrinter->Line->Columns[1]->Text = SeatsLine;

				TStringList *NameList = new TStringList;
				PrepareName(pPrinter->Line->Columns[0]->Width,ItemLine,NameList);
				pPrinter->Line->Columns[0]->Text = NameList->Strings[0];
				pPrinter->Line->Columns[1]->Text = SeatsLine;
				pPrinter->AddLine();

                if(CurrentOrder->isManuallyEnteredWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)AddManuallyEnteredLineInReceipt(pPrinter);

				for(int i = 1; i < NameList->Count; i++)
				{
					pPrinter->Line->Columns[0]->Text = NameList->Strings[i];
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
                    if(CurrentOrder->isManuallyEnteredWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)AddManuallyEnteredLineInReceipt(pPrinter);

				}
				//pPrinter->AddLine();

				pPrinter->Line->FontInfo.Reset();
				pPrinter->Line->Columns[0]->Kanji = false;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->Columns[1]->Width = 0;

				if (IncludeOptions && Options.size() > 0)
				{
					for (int k = 0; k < Options.size(); k++)
					{
						pPrinter->Line->FontInfo = Options[k].FontInfo;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[0]->Text = Options[k].Caption;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
					}
				}

				if (IncludeNote && !NoteLine.IsEmpty())
				{
					pPrinter->WordWrap = true;
					pPrinter->Line->Columns[0]->Text = NoteLine;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}

				if (IncludeSides && SubItems.size() > 0)
				{
					if (!SideLine.IsEmpty())
					{
						pPrinter->Line->Columns[0]->Text = SideLine;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
					}

					for (int k = 0; k < SubItems.size(); k++)
					{
						pPrinter->Line->FontInfo = SubItems[k].FontInfo;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[0]->Text = SubItems[k].Caption;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();

                       if( SubItems[k].isManuallyWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)AddManuallyEnteredLineInReceipt(pPrinter);
                	}

					if (!MixLine.IsEmpty())
					{
						pPrinter->Line->Columns[0]->Text = MixLine;
						pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
					}

				}
			}
		}
	}
}

void TPrintSection::PrintTwoLinesItemsName(TReqPrintJob *PrintJob)
{
	TList *OrderBundle;
	try
	{
		OrderBundle = new TList;
		TDateTime TimeStamp = Now();
		for (int i = 0; i < WorkingOrdersList->Count; )
		{
			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (TimeStamp > CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}

			TOrderBundle *TempBundle = new TOrderBundle();
			TempBundle->BundleOrders(PrintJob, WorkingOrdersList.get(), i, Format);

			OrderBundle->Add(TempBundle);
		}

		for (int j = 0; j < OrderBundle->Count; j++)
		{
			TOrderBundle *CurrentOrderBundle = (TOrderBundle*)OrderBundle->Items[j];
			TOrderBundle *NextOrderBundle = NULL;
			if (j + 1 < OrderBundle->Count)
			{
				NextOrderBundle = (TOrderBundle*)OrderBundle->Items[j + 1];
			}

			TPrintInfo TotalFontInfo;
			if (PrintJob->JobType == pjKitchen)
			{
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
			}
			else
			{
				TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
				TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
				TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
				TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
			}

			pPrinter->Line->FontInfo = TotalFontInfo;

			pPrinter->Line->ColCount = 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

			if (!CurrentOrderBundle->CancelLine.IsEmpty())
			{
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->CancelLine;
				pPrinter->AddLine();
			}

			if (!CurrentOrderBundle->SetMenuHeader.IsEmpty())
			{
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SetMenuHeader;
				pPrinter->AddLine();
			}

			int SeatsListWidth = 0;
			UnicodeString SeatsList;

			if (NextOrderBundle == NULL || (NextOrderBundle->SetMenuGroup != CurrentOrderBundle->SetMenuGroup) || (CurrentOrderBundle->SetMenuGroup == 0))
			{
				SeatsList = CurrentOrderBundle->SeatsLine;
				SeatsListWidth = SeatsList.Length();
				if ((SeatsListWidth > (pPrinter->Width / 5)) || (SeatsListWidth == 0))
				{
					pPrinter->Line->ColCount = 1;
					pPrinter->Line->Columns[0]->Width = pPrinter->Width;
					pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
					// add..
					TStringList *NameList = new TStringList;
					PrepareName(pPrinter->Line->Columns[0]->Width,CurrentOrderBundle->ItemLine,NameList);
					pPrinter->Line->Columns[0]->Text = NameList->Strings[0];
					//pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLine;    // add..
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->AddLine();
                    PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);


					for(int i = 1; i < NameList->Count; i++)
					{
						pPrinter->Line->Columns[0]->Text = NameList->Strings[i];
						//pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
                        PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

					}
					if (SeatsListWidth != 0)
					{
						SeatsList = TGlobalSettings::Instance().SeatLabel + " " + SeatsList;
						int StartPos = 0;
						// -4 to account for the Spaces add to Tab it in.
						int SeatsListChunkLength = pPrinter->Width - 4;
						do
						{
							UnicodeString Chunk = "    " + SeatsList.SubString(StartPos, SeatsListChunkLength);
							StartPos += SeatsList.SubString(StartPos, SeatsListChunkLength).Length() + 1;
							pPrinter->Line->Columns[0]->Text = Chunk;
							pPrinter->AddLine();
						}
						while (StartPos <= SeatsList.Length());

						pPrinter->Line->Columns[0]->Kanji = false;
					}
				}
				else
				{
					pPrinter->Line->ColCount = 2;
					SeatsListWidth = (pPrinter->Width / 5);
					pPrinter->Line->Columns[0]->Width = pPrinter->Width - SeatsListWidth;
					pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
					pPrinter->Line->Columns[1]->Width = SeatsListWidth;
					pPrinter->Line->Columns[1]->Alignment = taRightJustify;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					//// add.....
					TStringList *NameList = new TStringList;
					PrepareName(pPrinter->Line->Columns[0]->Width,CurrentOrderBundle->ItemLine,NameList);
					pPrinter->Line->Columns[0]->Text = NameList->Strings[0];
					pPrinter->Line->Columns[1]->Text = SeatsList;
					pPrinter->AddLine();
                    PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

					for(int i = 1; i < NameList->Count; i++)
					{
						pPrinter->Line->Columns[0]->Text = NameList->Strings[i];
						pPrinter->Line->Columns[1]->Text = "";
						pPrinter->AddLine();
                        PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

					}
					//pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLine;   // add..
					//pPrinter->Line->Columns[1]->Text = SeatsList;
					///pPrinter->AddLine();
					pPrinter->Line->Columns[0]->Kanji = false;
				}
			}
			else
			{
				pPrinter->Line->ColCount = 2;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width - SeatsListWidth;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				pPrinter->Line->Columns[1]->Width = SeatsListWidth;
				pPrinter->Line->Columns[1]->Alignment = taRightJustify;
				pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
				//add..........
				TStringList *NameList = new TStringList;
				PrepareName(pPrinter->Line->Columns[0]->Width,CurrentOrderBundle->ItemLine,NameList);
				pPrinter->Line->Columns[0]->Text = NameList->Strings[0];
				pPrinter->Line->Columns[1]->Text = SeatsList;
				pPrinter->AddLine();
                PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

				for(int i = 1; i < NameList->Count; i++)
				{
					pPrinter->Line->Columns[0]->Text = NameList->Strings[i];
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
                    PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

				}

				//pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLine;     // add..
				//pPrinter->Line->Columns[1]->Text = SeatsList;

				//pPrinter->AddLine();
				pPrinter->Line->Columns[0]->Kanji = false;
			}

			pPrinter->Line->ColCount = 2;
			pPrinter->Line->FontInfo.Reset();

			/* See Case 4028 You must always print options first.

			if(TGlobalSettings::Instance().ForcedSidesFirst)
			{
			PrintSubordersSection(CurrentOrderBundle,pPrinter);
			PrintOptionsSection(CurrentOrderBundle,pPrinter);
			}
			else
			{
			PrintOptionsSection(CurrentOrderBundle,pPrinter);
			PrintSubordersSection(CurrentOrderBundle,pPrinter);
			} */

			PrintOptionsSection(CurrentOrderBundle, pPrinter);
			PrintSubordersSection(CurrentOrderBundle, pPrinter);

			if (!CurrentOrderBundle->NoteLine.IsEmpty())
			{
				TPrintInfo TotalFontInfo;
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
				pPrinter->Line->FontInfo = TotalFontInfo;
				pPrinter->WordWrap = true;
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->NoteLine;
				pPrinter->Line->Columns[1]->Text = "";
				pPrinter->AddLine();
			}
		}
	}
	__finally
	{
		while (OrderBundle->Count != 0)
		{
			delete(TOrderBundle*)OrderBundle->Items[0];
			OrderBundle->Delete(0);
		}
		delete OrderBundle;
	}

}


void TPrintSection::PrintItemRavelled(TReqPrintJob *PrintJob, bool groupItemsTemplate)        // add..
{
	TList *OrderBundle;
	try
	{
		OrderBundle = new TList;
		TDateTime TimeStamp = Now();
		for (int i = 0; i < WorkingOrdersList->Count; )
		{
			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (TimeStamp > CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}

			TOrderBundle *TempBundle = new TOrderBundle();
            if(groupItemsTemplate)
            {
               TempBundle->BundleOrdersWithSides(PrintJob, WorkingOrdersList.get(), i, Format);
            }
            else
            {
			    TempBundle->BundleOrders(PrintJob, WorkingOrdersList.get(), i, Format);
            }

			OrderBundle->Add(TempBundle);
		}

		for (int j = 0; j < OrderBundle->Count; j++)
		{
			TOrderBundle *CurrentOrderBundle = (TOrderBundle*)OrderBundle->Items[j];
			TOrderBundle *NextOrderBundle = NULL;
			if (j + 1 < OrderBundle->Count)
			{
				NextOrderBundle = (TOrderBundle*)OrderBundle->Items[j + 1];
			}

			TPrintInfo TotalFontInfo;
			if (PrintJob->JobType == pjKitchen)
			{
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
			}
			else
			{
				TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
				TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
				TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
				TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
			}

			pPrinter->Line->FontInfo = TotalFontInfo;

			pPrinter->Line->ColCount = 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

			if (!CurrentOrderBundle->CancelLine.IsEmpty())
			{
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->CancelLine;
				pPrinter->AddLine();
			}

			if (!CurrentOrderBundle->SetMenuHeader.IsEmpty())
			{
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SetMenuHeader;
				pPrinter->AddLine();
			}

			int SeatsListWidth = 0;
			UnicodeString SeatsList;

			if (NextOrderBundle == NULL || (NextOrderBundle->SetMenuGroup != CurrentOrderBundle->SetMenuGroup) || (CurrentOrderBundle->SetMenuGroup == 0))
			{
				SeatsList = CurrentOrderBundle->SeatsLine;
				SeatsListWidth = SeatsList.Length();
				if ((SeatsListWidth > (pPrinter->Width / 5)) || (SeatsListWidth == 0))
				{
					pPrinter->Line->ColCount = 1;
					pPrinter->Line->Columns[0]->Width = pPrinter->Width;
					pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLine;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->AddLine();

                    PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

					if (SeatsListWidth != 0)
					{
						SeatsList = TGlobalSettings::Instance().SeatLabel + " " + SeatsList;
						int StartPos = 0;
						// -4 to account for the Spaces add to Tab it in.
						int SeatsListChunkLength = pPrinter->Width - 4;
						do
						{
							UnicodeString Chunk = "    " + SeatsList.SubString(StartPos, SeatsListChunkLength);
							StartPos += SeatsList.SubString(StartPos, SeatsListChunkLength).Length() + 1;
							pPrinter->Line->Columns[0]->Text = Chunk;
							pPrinter->AddLine();
						}
						while (StartPos <= SeatsList.Length());

						pPrinter->Line->Columns[0]->Kanji = false;
					}
				}
				else
				{
					pPrinter->Line->ColCount = 2;
					SeatsListWidth = (pPrinter->Width / 5);
					pPrinter->Line->Columns[0]->Width = pPrinter->Width - SeatsListWidth;
					pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
					pPrinter->Line->Columns[1]->Width = SeatsListWidth;
					pPrinter->Line->Columns[1]->Alignment = taRightJustify;
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					//// add.....

					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLine;   // add..
					pPrinter->Line->Columns[1]->Text = SeatsList;

                    pPrinter->AddLine();
					pPrinter->Line->Columns[0]->Kanji = false;

                    PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);

				}
			}
			else
			{
				pPrinter->Line->ColCount = 2;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width - SeatsListWidth;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				pPrinter->Line->Columns[1]->Width = SeatsListWidth;
				pPrinter->Line->Columns[1]->Alignment = taRightJustify;
				pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
				//add..........


				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLine;     // add..
				pPrinter->Line->Columns[1]->Text = SeatsList;

				pPrinter->AddLine();
				pPrinter->Line->Columns[0]->Kanji = false;

                PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);
			}

			pPrinter->Line->ColCount = 2;
			pPrinter->Line->FontInfo.Reset();


            if(groupItemsTemplate)
            {
                PrintOptionsWithQuantity(CurrentOrderBundle, pPrinter);
                PrintSubordersSection(CurrentOrderBundle, pPrinter, groupItemsTemplate);
            }
            else
            {
                PrintOptionsSection(CurrentOrderBundle, pPrinter);
                PrintSubordersSection(CurrentOrderBundle, pPrinter);
            }


			if (!CurrentOrderBundle->NoteLine.IsEmpty())
			{
				TPrintInfo TotalFontInfo;
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
				pPrinter->Line->FontInfo = TotalFontInfo;
				pPrinter->WordWrap = true;
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->NoteLine;
				pPrinter->Line->Columns[1]->Text = "";
				pPrinter->AddLine();
			}
		}
	}
	__finally
	{
		while (OrderBundle->Count != 0)
		{
			delete(TOrderBundle*)OrderBundle->Items[0];
			OrderBundle->Delete(0);
		}
		delete OrderBundle;
	}
}

void TPrintSection::PrintBarCode(TReqPrintJob *PrintJob)
{
	const int MAX_BC_LENGTH = 8;
	if (PrintJob->BarCodeData != 0)
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->BarcodeHRIPosition = BARCODE_HRI_POSITION_NONE;
		pPrinter->BarcodeHRIFont = BARCODE_HRI_FONT_B;
		pPrinter->BarcodeCheckChar = true;
		pPrinter->BarcodeRatio = BARCODE_RATION_5_2;
		pPrinter->BarcodeHeight = 80;
		pPrinter->BarcodeHorzExp = 2;
		pPrinter->BarcodeType = BARCODE_TYPE_CODE_128;
		pPrinter->AddBarcode("MMK" + IntToHex(__int64(PrintJob->BarCodeData), MAX_BC_LENGTH));
	}
	else
	{
		Empty = true;
	}
}

void TPrintSection::PrintChitNumber(TReqPrintJob *PrintJob)
{
	if (PrintJob->Transaction)
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->Columns[0]->Alignment = taCenter;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->Columns[0]->Text = "";
		if(PrintJob->Transaction->TypeOfSale == ComplimentarySale)
		{
			pPrinter->Line->Columns[0]->Text = "Complimentary";
		}
		else  if(PrintJob->Transaction->TypeOfSale == NonChargableSale)
		{
			pPrinter->Line->Columns[0]->Text = "Non-Chargeable";
		}
		pPrinter->AddLine();

		if (PrintJob->Transaction->ChitNumber.Valid())
		{
			UnicodeString ChitLabel = "";
			pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
			pPrinter->Line->Columns[0]->Alignment = taCenter;
			pPrinter->Line->ColCount = 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			ChitLabel = PrintJob->Transaction->ChitNumber.GetChitNumber();
			pPrinter->Line->Columns[0]->Text = ChitLabel;
			pPrinter->AddLine();
		}
		else
		{
			if(!((PrintJob->Transaction->TypeOfSale == ComplimentarySale) ||
						(PrintJob->Transaction->TypeOfSale == NonChargableSale)))
			{
				Empty = true;
			}
		}
	}
}

void TPrintSection::PrintReceiptLogo(TReqPrintJob *PrintJob)
{
	pPrinter->PrintGraphic();
}

void TPrintSection::PrintSaleTypeDetails(TReqPrintJob *PrintJob)
{
	if(PrintJob->Transaction->TypeOfSale != RegularSale)
	{
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo.Bold = false;
		pPrinter->Line->FontInfo.Height = fsNormalSize;
		pPrinter->Line->FontInfo.Width = fsNormalSize;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taCenter;
		pPrinter->Line->Columns[0]->Text =  PrintJob->Transaction->TypeOfSale == ComplimentarySale ?"** Complimentary Meal **": "** Non-Chargeable **" ;
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintReceiptHeader(TReqPrintJob *PrintJob)
{

	pPrinter->Line->ColCount = 1;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->FontInfo.Height = fsNormalSize;
	pPrinter->Line->FontInfo.Width = fsNormalSize;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taCenter;

    IsDBRegistered = false;
    TDeviceRealTerminal::Instance().Registered(&IsDBRegistered, NULL);
    if(!IsDBRegistered)
    {
        PrintTextForUnRegisteredDatabase();
    }
	else if (PrintJob->PaymentType == ptPreliminary && ( PrintJob->Transaction->InvoiceNumber == "" || PrintJob->Transaction->InvoiceNumber == "Undefined"))
	{
        if (PrintJob->ReceiptPHeader->Count == 0)
         {
           Empty = true;
         }
		else
		{
			for (int i = 0; i < PrintJob->ReceiptPHeader->Count; i++)
			{
				pPrinter->Line->Columns[0]->Text = PrintJob->ReceiptPHeader->Strings[i];
				pPrinter->AddLine();
			}
		}
      }

	else
	{
        if (PrintJob->ReceiptHeader->Count == 0)
		{
            Empty = true;
        }
		else
		{
			for (int i = 0; i < PrintJob->ReceiptHeader->Count; i++)
			{
				pPrinter->Line->Columns[0]->Text = PrintJob->ReceiptHeader->Strings[i];
				pPrinter->AddLine();
			}
            if(TReceiptUtility::CheckRefundCancelTransaction(*PrintJob->Transaction)&&
                 TGlobalSettings::Instance().ShowVoidOrRefund)
             {
                 pPrinter->Line->Columns[0]->Text = "VOID";//PrintVoidOnReceipt(PrintJob);
                 pPrinter->AddLine();
             }
            if(!TGlobalSettings::Instance().HideReceiptNumberForRefundItem ||
                !(PrintJob->Transaction->CreditTransaction))
            {
                PrintTaxInvoice(PrintJob);
                if(TReceiptUtility::CheckRefundCancelTransaction(*PrintJob->Transaction)
                    && TGlobalSettings::Instance().ShowVoidNumber)
                {
                    pPrinter->Line->Columns[0]->Text    =   "Void No.";
                }
                else
                    pPrinter->Line->Columns[0]->Text = TGlobalSettings::Instance().ReceiptNumberLabel.Trim();
                if(PrintJob->Transaction->TypeOfSale == RegularSale)
                {
                     AnsiString invoiceNumber = (AnsiString)PrintJob->Transaction->InvoiceNumber;
                     AnsiString prefix = TReceiptUtility::ExtractInvoiceNumber(invoiceNumber);
                     pPrinter->Line->Columns[0]->Text += prefix +
                               TReceiptUtility::ModifyInvoiceNumber(invoiceNumber, ReceiptLength);
                }
                else
                {
                      pPrinter->Line->Columns[0]->Text += PrintJob->Transaction->InvoiceNumber;
                }
                pPrinter->AddLine();
            }
		}
	}
}

//-----------------------------------------------------------------------------
void TPrintSection::PrintTaxInvoice(TReqPrintJob *PrintJob)
{
    if(!TGlobalSettings::Instance().HideTaxInvoice)
    {
        pPrinter->Line->Columns[0]->Text = "Tax Invoice";
        pPrinter->AddLine();
    }
}
//-----------------------------------------------------------------------------
void TPrintSection::PrintVoidOnReceipt(TReqPrintJob *PrintJob)
{
        pPrinter->Line->ColCount = 1;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->Columns[0]->Alignment = taCenter;
        pPrinter->Line->Columns[0]->Text = "VOID";
        pPrinter->AddLine();
}
//-----------------------------------------------------------------------------
void TPrintSection::PrintReceiptFooterSecond(TReqPrintJob *PrintJob)
{
    if(TReceiptUtility::CheckRefundCancelTransaction(*PrintJob->Transaction)
                && TGlobalSettings::Instance().SetVoidFooter)
    {
        if (PrintJob->ReceiptVoidFooter->Count == 0)
        {
            Empty = true;
        }
        else
        {
            pPrinter->Line->ColCount = 1;
            pPrinter->Line->FontInfo.Bold = false;
            pPrinter->Line->FontInfo.Height = fsNormalSize;
            pPrinter->Line->FontInfo.Width = fsNormalSize;
            pPrinter->Line->Columns[0]->Width = pPrinter->Width;
            pPrinter->Line->Columns[0]->Alignment = taCenter;
            for (int i = 0; i < PrintJob->ReceiptVoidFooter->Count; i++)
            {
                pPrinter->Line->Columns[0]->Text = PrintJob->ReceiptVoidFooter->Strings[i];
                pPrinter->AddLine();
            }
        }
    }
    else
    {
        Empty = true;
    }
}
//-----------------------------------------------------------------------------
void TPrintSection::PrintReceiptFooter(TReqPrintJob *PrintJob)
{

    if((!TReceiptUtility::CheckRefundCancelTransaction(*PrintJob->Transaction))
         || !TGlobalSettings::Instance().SetVoidFooter)
    {
        if (PrintJob->ReceiptFooter->Count == 0)
        {
            Empty = true;
        }
        else
        {
            pPrinter->Line->ColCount = 1;
            pPrinter->Line->FontInfo.Bold = false;
            pPrinter->Line->FontInfo.Height = fsNormalSize;
            pPrinter->Line->FontInfo.Width = fsNormalSize;
            pPrinter->Line->Columns[0]->Width = pPrinter->Width;
            pPrinter->Line->Columns[0]->Alignment = taCenter;

            for (int i = 0; i < PrintJob->ReceiptFooter->Count; i++)
            {
                pPrinter->Line->Columns[0]->Text = PrintJob->ReceiptFooter->Strings[i];
                pPrinter->AddLine();
            }
        }
    }
    else
    {
        Empty = true;
    }
}


void TPrintSection::PrintPaymentTotals(TReqPrintJob *PrintJob)
{
	Empty = true;

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	for (int i = 0; i < PrintJob->Transaction->PaymentsCount(); i++)
	{
		TPayment *SubPayment = PrintJob->Transaction->PaymentGet(i);

		if (SubPayment->GetCashOutTotal() != 0)
		{
			Empty = false;
			pPrinter->Line->Columns[1]->Width = CurrToStrF(
			RoundToNearest(SubPayment->GetCashOutTotal(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals).Length() + 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width;

			if (SubPayment->Properties & ePayTypeCSV)
			{
				pPrinter->Add(SubPayment->Name + " " + IntToStr(SubPayment->CSVNumber) + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetCashOutTotal(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals));
			}
			else if (SubPayment->Properties & ePayTypePocketVoucher)
			{
				pPrinter->Add(SubPayment->Name + " " + SubPayment->VoucherCode + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetCashOutTotal(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals));
			}
			else if (SubPayment->Properties & ePayTypePoints)
			{
				pPrinter->Add(SubPayment->Name + " Points Purchase" + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetCashOutTotal(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals));
			}
			else
			{
				pPrinter->Add(SubPayment->Name + " Cash Out" + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetCashOutTotal(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
				ffNumber,
				CurrencyDecimals));
			}
		}

		if (SubPayment->GetPayTendered() != 0)
		{
			Empty = false;
			pPrinter->Line->Columns[1]->Width = CurrToStrF(
			RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
			ffNumber,
			CurrencyDecimals).Length() + 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width;

			if (SubPayment->Properties & ePayTypeCSV)
			{
				pPrinter->Add(SubPayment->Name + " " + IntToStr(SubPayment->CSVNumber) + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals));
			}
			else if (SubPayment->Properties & ePayTypePocketVoucher)
			{
				pPrinter->Add(SubPayment->Name + " " + SubPayment->VoucherCode + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
				ffNumber,
				CurrencyDecimals));
			}
            else if(SubPayment->IsLoyaltyVoucher() && TGlobalSettings::Instance().LoyaltyMateEnabled)
            {
                pPrinter->Add(SubPayment->Name + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals));

                AnsiString vouchercode = "";
                bool isGiftCardUsed = false;
                if(SubPayment->Name == "Gift Card")
                  {
                    vouchercode = "Gift Card #" + PrintJob->Transaction->RedeemGiftVoucherInformation->VoucherNumber;
                    isGiftCardUsed = true;
                  }
                  else if(SubPayment->Name == "Voucher")
                  {
                    vouchercode = "Voucher #" + PrintJob->Transaction->RedeemPocketVoucherInformation->VoucherNumber;
                  }
                  pPrinter->Add(vouchercode);
                  if(isGiftCardUsed)
                     pPrinter->Add("Balance " + CurrToStr( PrintJob->Transaction->RedeemGiftVoucherInformation->GiftVoucherAmount -
                                                           PrintJob->Transaction->RedeemGiftVoucherInformation->RedeemedAmount));
            }
			else
			{
				pPrinter->Add(SubPayment->Name + "|" + CurrToStrF(
				RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
				ffNumber,
				CurrencyDecimals));
			}
		}

       if(SubPayment->TipAmount != 0)
        {
            pPrinter->Add(SubPayment->Name + " Tip " + "|" + CurrToStrF(
            RoundToNearest(SubPayment->TipAmount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown ),
            ffNumber,
            CurrencyDecimals));
        }
	}

	if (PrintJob->PaymentType != ptPreliminary)
	{
		pPrinter->Line->Columns[1]->Width = CurrToStrF(PrintJob->Transaction->Money.TotalRounding, ffNumber, CurrencyDecimals).Length() + 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width;
		pPrinter->Add("Rounding|" +((PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : CurrToStrF(PrintJob->Transaction->Money.TotalRounding, ffNumber, CurrencyDecimals)));
		Empty = false;
	}

	if (PrintJob->Transaction->Money.Change != 0)
	{
		pPrinter->NewLine();
		pPrinter->Line->Columns[1]->Width = CurrToStrF(PrintJob->Transaction->Money.RoundedChange, ffNumber, CurrencyDecimals).Length() + 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width;
		pPrinter->Add("Total Change|" + CurrToStrF(PrintJob->Transaction->Money.RoundedChange,ffNumber, CurrencyDecimals));
		Empty = false;
	}

}

void TPrintSection::PrintPaymentSurcharges(TReqPrintJob *PrintJob)
{
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	Empty = true;
	for (int i = 0; i < PrintJob->Transaction->PaymentsCount(); i++)
	{
		TPayment *SubPayment = PrintJob->Transaction->PaymentGet(i);

		if (SubPayment->GetAdjustment() != 0)
		{
            AnsiString text = SubPayment->AdjustmentReason;
            if(SubPayment->IsLoyaltyGiftCard())
            {
               text = "Purchased Gift Card #" + PrintJob->Transaction->PurchasedGiftVoucherInformation->VoucherNumber;
            }
            pPrinter->Line->Columns[1]->Width = CurrToStrF(RoundToNearest(SubPayment->GetAdjustment(), 0.01,
                                               TGlobalSettings::Instance().MidPointRoundsDown),ffNumber,CurrencyDecimals).Length() + 1;
            pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width;
            pPrinter->Line->Columns[0]->Text = text;
            pPrinter->Line->Columns[1]->Text = CurrToStrF(
            RoundToNearest(SubPayment->GetAdjustment(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),ffNumber,CurrencyDecimals);
            pPrinter->AddLine();

            if(SubPayment->IsLoyaltyGiftCard())
            {
                AnsiString balance = CurrToStr(RoundToNearest(PrintJob->Transaction->PurchasedGiftVoucherInformation->GiftVoucherAmount +
                                                           PrintJob->Transaction->PurchasedGiftVoucherInformation->RedeemedAmount,0.01,
                                                           TGlobalSettings::Instance().MidPointRoundsDown));
                pPrinter->Line->Columns[0]->Width = pPrinter->Width;
                pPrinter->Line->Columns[0]->Text = "Balance " + balance;
                pPrinter->Line->Columns[1]->Text = "";
                pPrinter->AddLine();
            }
            Empty = false;
		}
		if (SubPayment->GetRefundPointsValue() != 0)
		{
			pPrinter->Line->Columns[1]->Width = CurrToStrF(
			RoundToNearest(SubPayment->GetRefundPointsValue(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals).Length() + 1;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width - pPrinter->Line->Columns[1]->Width;
			pPrinter->Line->Columns[0]->Text = SubPayment->AdjustmentReason;
			pPrinter->Line->Columns[1]->Text = CurrToStrF(
			RoundToNearest(SubPayment->GetRefundPointsValue(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
			ffNumber,
			CurrencyDecimals);
			pPrinter->AddLine();
			Empty = false;
		}
	}
}

void TPrintSection::PrintWaitTimes(TReqPrintJob *PrintJob)
{
	if (PrintJob->WaitTime == 0.0)
	{
		Empty = true;
	}
	else
	{
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->FontInfo.Bold = true;
		pPrinter->Line->FontInfo.Width = fsNormalSize;
		pPrinter->Line->FontInfo.Height = fsNormalSize;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taCenter;
		pPrinter->Line->Columns[0]->Text = "Average Wait Time " + FloatToStrF(PrintJob->WaitTime, ffGeneral, 15, 2) + " Minutes";
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintSoldBy(TReqPrintJob *PrintJob)
{
	Empty = true;
	if (WorkingOrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
		TSecurityReference *Sec1 = Order->Security->SecurityGetType(secOrderedBy);
		if (Sec1 != NULL)
		{
			UnicodeString OrderedBy = "Ordered By : " + Sec1->From;
			pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
			pPrinter->Line->ColCount = 1;
			pPrinter->Line->Columns[0]->Width = OrderedBy.Length() + 1;
			pPrinter->Line->Columns[0]->Text = OrderedBy;
			pPrinter->AddLine();
			Empty = false;
		}
	}
}

void TPrintSection::PrintMsg(TReqPrintJob *PrintJob)
{
	if (ThisInstruction->Caption == "")
	{
		Empty = true;
	}
	else
	{
		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 2;
		pPrinter->Line->Columns[0]->Width = ThisInstruction->Caption.Length() + 1;
		pPrinter->Line->Columns[1]->Width = pPrinter->Width - ThisInstruction->Caption.Length() - 1;
		pPrinter->Line->Columns[0]->Text = ThisInstruction->Caption;
		pPrinter->Line->Columns[1]->Text = "";
		pPrinter->AddLine();
	}
}

void TPrintSection::PrintFoodDrink(TReqPrintJob *PrintJob)
{
	if (WorkingOrdersList->Count == 0)
	{
		Empty = true;
	}
	else
	{
		UnicodeString Label;
		if (WorkingOrdersList->Count > 0)
		{
			TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[0];
			if (Order->ItemType == eFoodItem)
			{
				Label = "Food";
			}
			else if (Order->ItemType == eDrinksItem)
			{
				Label = "Drinks";
			}
			else
			{
				Label = "Unknown";
			}
		}

		pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taCenter;
		pPrinter->Line->Columns[0]->Text = Label;
		pPrinter->AddLine();
	}
}

bool TPrintSection::ExtractTableList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
			if (NextOrder->ContainerTabType == TabTableSeat || NextOrder->TabType == TabTableSeat)
			{
				if (NextOrder->TableNo == Order->TableNo)
				{
					WorkingOrdersList->Add(NextOrder);
				}
				else
				{
					break; // Sorted WorkingOrdersList so we can Quit here.
				}
			}
			else
			{
				if (NextOrder->TabKey == Order->TabKey)
				{
					WorkingOrdersList->Add(NextOrder);
				}
				else
				{
					break; // Sorted WorkingOrdersList so we can Quit here.
				}
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractSeatList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
			if (NextOrder->SeatNo == Order->SeatNo)
			{
				WorkingOrdersList->Add(NextOrder);
			}
			else
			{
				break; // Sorted WorkingOrdersList so we can Quit here.
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractServingCourseList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
			if (NextOrder->ServingCourse.ServingCourseKey == Order->ServingCourse.ServingCourseKey)
			{
				WorkingOrdersList->Add(NextOrder);
			}
			else
			{
				break; // Sorted WorkingOrdersList so we can Quit here.
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractCourseList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
			// WorkingOrdersList->Add(NextOrder);
			if (NextOrder->CourseKitchenName == Order->CourseKitchenName)
			{
				WorkingOrdersList->Add(NextOrder);
			}
			else
			{
				break; // Sorted WorkingOrdersList so we can Quit here.
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractItemsList(TItemType inItemType)
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		for (int i = 0; i < OrdersList->Count; )
		{
			TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
			if (Order->ItemType == inItemType)
			{
				WorkingOrdersList->Add(Order);
				OrdersList->Remove(Order);
				DataExtracted = true;
			}
			else
			{
				i++;
			}
		}
	}
	return DataExtracted;
}

bool TPrintSection::ExtractItem()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		WorkingOrdersList->Add(Order);
		OrdersList->Remove(Order);
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractItemsList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
			 WorkingOrdersList->Add(Order);
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractDateList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
			// WorkingOrdersList->Add(NextOrder);
			if (DateOf(NextOrder->TimeStamp) == DateOf(Order->TimeStamp))
			{
				WorkingOrdersList->Add(NextOrder);
			}
			else
			{
				break; // Sorted WorkingOrdersList so we can Quit here.
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

bool TPrintSection::ExtractCancelsList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
			if (Order->OrderType != CanceledOrder)
			{
				WorkingOrdersList->Add(Order);
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
			DataExtracted = true;
		}
	}
	return DataExtracted;
}

bool TPrintSection::ExtractFreeList(int jobType)
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		for (int i = 0; i < OrdersList->Count; i++)
		{
            TItemComplete *newOrder = (TItemComplete *)OrdersList->Items[i];
			if (newOrder->TotalPriceAdjustment() != 0)
			{
                for (int index = 0; index < newOrder->SubOrders->Count; index++)
                {
                    TItemCompleteSub *suborder = newOrder->SubOrders->SubOrderGet(index);
                    if(suborder->TotalPriceAdjustment() == 0 &&  jobType == 1)
                    {
                        suborder->printFreeSideForReceipt = true;
                    }
                     else if (suborder->TotalPriceAdjustment() == 0 &&  jobType == 3)
			        {
                        suborder->printFreeSideForKitchen = true;
                    }

                }
			  	WorkingOrdersList->Add(newOrder);
			}

		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
			DataExtracted = true;
		}
	}
	return DataExtracted;
}

bool TPrintSection::ExtractSoldByList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		TSecurityReference *Sec1 = Order->Security->SecurityGetType(secOrderedBy);
		if (Sec1 != NULL)
		{
			for (int i = 0; i < OrdersList->Count; i++)
			{
				TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
				TSecurityReference *Sec2 = NextOrder->Security->SecurityGetType(secOrderedBy);
				if (Sec2 != NULL)
				{
					if (Sec1->From == Sec2->From)
					{
						WorkingOrdersList->Add(NextOrder);
					}
					else
					{
						break; // Sorted WorkingOrdersList so we can Quit here.
					}
				}
				else
				{
					break; // Sorted WorkingOrdersList so we can Quit here.
				}
			}

			for (int i = 0; i < WorkingOrdersList->Count; i++)
			{
				TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
				OrdersList->Remove(NextOrder);
				DataExtracted = true;
			}
		}
		else // There is no OrderedBy Infomation, Pass everything to the next ThisInstruction->
		{
			for (int i = 0; i < OrdersList->Count; i++)
			{
				TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
				WorkingOrdersList->Add(NextOrder);
				OrdersList->Remove(NextOrder);
				DataExtracted = true;
			}
		}
	}
	return DataExtracted;
}

bool TPrintSection::ExtractFoodDrinkList()
{
	bool DataExtracted = false;
	WorkingOrdersList->Clear();
	if (OrdersList->Count > 0)
	{
		TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)OrdersList->Items[i];
			if (NextOrder->ItemType == Order->ItemType)
			{
				WorkingOrdersList->Add(NextOrder);
			}
			else
			{
				break; // Sorted WorkingOrdersList so we can Quit here.
			}
		}

		for (int i = 0; i < WorkingOrdersList->Count; i++)
		{
			TItemComplete *NextOrder = (TItemComplete*)WorkingOrdersList->Items[i];
			OrdersList->Remove(NextOrder);
		}
		DataExtracted = true;
	}
	return DataExtracted;
}

UnicodeString TPrintSection::GetHeaderName(TItemComplete *Order)
{
	UnicodeString Header = "";
	switch(Order->TabType)
	{
	case TabNormal:
	case TabStaff:
	case TabMember:
		{
			Header = "Tab " + Order->TabName;
			break;
		}
	case TabTableSeat:
		{
			if (Order->TabContainerName == "")
			{
				Header = "Table " + IntToStr(Order->TableNo);
			}
			else
			{
				Header = Order->TabContainerName;
			}
			break;
		}
	case TabRoom:
		{
			if (Order->TabContainerName == "")
			{
				Header = "ROOM " + IntToStr(Order->RoomNo);
			}
			else
			{
				Header = Order->TabContainerName;
			}
			break;
		}
	case TabWeb:
		{
			Header = "Online Order " + Order->TabName;
			break;
		}
	case TabHeld:
		{
			Header = Order->TabName;
			break;
		}
	case TabHandheldCashAccount:
	case TabCashAccount:
	case TabNone:
	default:
		Header = "CASH";
	}
	return Header;
}

// ------------------------------------------------------------------------------
_fastcall TOrderBundle::TOrderBundle()
{
	MixLine 			= "";
	SideLine 			= "";
	ItemWeight 			= "";
	TimeStamp 			= Now();
	SetMenuGroup 		= 0;
	usingNMIStandardAus = false;

	BundledOrders.reset(new TList());
	Options.reset(new TListOptionContainer());
}
// ------------------------------------------------------------------------------

void TOrderBundle::BundleOrders(
TReqPrintJob *PrintJob,
TList *Orders,
int &CurrentIndex,
TDocketFormat &inFormat)
{
	// initializations
	UnicodeString QtyMultiplier1 	= UnicodeString("x ");
	UnicodeString QtyMultiplier10 	= UnicodeString("x");
	UnicodeString Spacer1 			= UnicodeString(" ");
	UnicodeString SeatStart 		= UnicodeString("(");
	UnicodeString SeatEnd 			= UnicodeString(")");
	UnicodeString SeatSpacer 		= UnicodeString(", ");
	UnicodeString SetMenuItemSpacer = "";
	SeatsLine 						= SeatStart;
	Format 							= inFormat;
	TItemComplete *InitialOrder 	= (TItemComplete*)(Orders->Items[CurrentIndex]);
	FontInfo 						= InitialOrder->FontInfo;

	if (CurrentIndex == 0)
	{
		if (InitialOrder->SetMenuMask != 0 && InitialOrder->SetMenuGroup != 0)
		{
			if (Format.KeyItemHeader[InitialOrder->SetMenuGroup] != WideString(""))
			{
				SetMenuHeader = "[" + Format.KeyItemHeader[InitialOrder->SetMenuGroup] + "]";
			}
			SetMenuGroup = InitialOrder->SetMenuGroup;
			SetMenuItemSpacer = Format.BulletSetMenuItem;
		}
	}
	else if (CurrentIndex > 0)
	{
		TItemComplete *PrevOrder = (TItemComplete*)(Orders->Items[CurrentIndex - 1]);
		if (InitialOrder->SetMenuMask != 0 && InitialOrder->SetMenuGroup != 0)
		{
			if ((InitialOrder->SetMenuGroup != PrevOrder->SetMenuGroup) || (InitialOrder->CourseKitchenName != PrevOrder->CourseKitchenName))
			{
				if (Format.KeyItemHeader[InitialOrder->SetMenuGroup] != WideString(""))
				{
					SetMenuHeader = "[" + Format.KeyItemHeader[InitialOrder->SetMenuGroup] + "]";
				}
				SetMenuGroup = InitialOrder->SetMenuGroup;
				SetMenuItemSpacer = Format.BulletSetMenuItem;
			}
			else
			{
				SetMenuGroup = InitialOrder->SetMenuGroup;
			}
		}
	}

	Currency CountQuantity = 0;
	int CountSeatQuantity = 0;
	UnicodeString LastItem = ((TItemComplete*)(Orders->Items[CurrentIndex]))->ItemKitchenName;
	// As long as it;s a normal order bundle it.
	if (InitialOrder->OrderType == NormalOrder)
	{
		while ((CurrentIndex < Orders->Count) && (LastItem == ((TItemComplete*)(Orders->Items[CurrentIndex]))->ItemKitchenName))
		{
			TItemComplete *CurrentOrder = (TItemComplete*)(Orders->Items[CurrentIndex]);

			if ( isDifferentItemFromInitial( InitialOrder, CurrentOrder ) )
			{
				// Ok the next order does not match this one so write out this ones
				// Seat Information, Yes its a real pain being retrospective....
				TItemComplete *PrevOrder = (TItemComplete*)(Orders->Items[CurrentIndex - 1]);
				// It may have been writtren out below if the next seat No if differnt
				// this traps for that.
				if (CountSeatQuantity > 0)
				{
					// Sort out the seat name.
					UnicodeString QtyText = "";
					UnicodeString SeatTypeText = "";
					UnicodeString Identifier = "";

					if (CountSeatQuantity > 1)
					{
						QtyText = IntToStr(CountSeatQuantity) + Format.DishesPerSeatMultiplerString;
					}

					switch(PrevOrder->TabType)
					{
					case TabNormal:
					case TabStaff:
					case TabMember:
					case TabWeb:
						{
							SeatTypeText = ""; // Tab ";
							Identifier = ""; // PrevOrder->TabName;
							break;
						}
					case TabTableSeat:
						{
							// Cant use Tab name here it could be to long.
							SeatTypeText = "";
							Identifier = IntToStr(PrevOrder->SeatNo);
							break;
						}
					case TabHandheldCashAccount:
					case TabCashAccount:
					case TabNone:
					default:
						// SeatTypeText = "Cash Sale";
						SeatTypeText = ""; // Cash Sale";
						Identifier = "";
						QtyText = "";
						break;
					}
					if (SeatTypeText != "" || Identifier != "")
					{
						SeatsLine += QtyText + SeatTypeText + Identifier + ", ";
					}
				}

				break; // break right out of the loop.
			}

            if(CurrentOrder->isManuallyEnteredWeight)
            {
                this->addManuallyEnteredWeightString=true;
            }
             else
            {
                this->addManuallyEnteredWeightString = false;
            }

			BundledOrders->Add(CurrentOrder);

			CountQuantity += CurrentOrder->GetQty();
			// Needed to sence seat name change.   Current Index is incremented so the
			// next reference to it is already pointing to the next seat.
			unsigned int OldTabKey = CurrentOrder->TabKey;
			CurrentIndex++;
			CountSeatQuantity++;
			unsigned int NewTabKey = (CurrentIndex < Orders->Count) ? ((TItemComplete*)(Orders->Items[CurrentIndex]))->TabKey : -1;
			UnicodeString NextItem = (CurrentIndex < Orders->Count) ? ((TItemComplete*)(Orders->Items[CurrentIndex]))->ItemKitchenName : UnicodeString("");
			// The Next seat is not the same as this one write out this seat name and
			// quantity of items assigned to it.
			// Or The Next Item is not the same and the While loop will exit.
			// Or this is the Last Order so write out seat name and quantity.
			if ((OldTabKey != NewTabKey) || (LastItem != NextItem) || (CurrentIndex == Orders->Count))
			{
				// Sort out the seat name.
				// Sort out the seat name.
				UnicodeString QtyText = "";
				UnicodeString SeatTypeText = "";
				UnicodeString Identifier = "";

				if (CountSeatQuantity > 1)
				{
					QtyText = IntToStr(CountSeatQuantity) + Format.DishesPerSeatMultiplerString;
				}

				switch(CurrentOrder->TabType)
				{
				case TabNormal:
				case TabStaff:
				case TabMember:
				case TabWeb:
					{
						SeatTypeText = ""; // Tab ";
						Identifier = ""; // CurrentOrder->TabName;
						break;
					}
				case TabTableSeat:
					{
						// Cant use Tab name here it could be to long.
						SeatTypeText = "";
						Identifier = IntToStr(CurrentOrder->SeatNo);
						break;
					}
				case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
				default:
					// SeatTypeText = "Cash Sale";
					SeatTypeText = "";
					Identifier = "";
					QtyText = "";
					break;
				}
				if (SeatTypeText != "" || Identifier != "")
				{
					SeatsLine += QtyText + SeatTypeText + Identifier + ", ";
				}
				CountSeatQuantity = 0; // reset seat qty.
			}

			// Update the timestamp to the oldest order.
			if (TimeStamp < CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}
		}
	}
	else
	{
		BundledOrders->Add(InitialOrder);
		CountQuantity += InitialOrder->GetQty();
		CurrentIndex++;
		UnicodeString QtyText = "";
		UnicodeString SeatTypeText = "";
		UnicodeString Identifier = "";

		QtyText = FormatFloat("0.##", CountQuantity) + Format.DishesPerSeatMultiplerString;

		switch(InitialOrder->TabType)
		{
		case TabNormal:
		case TabStaff:
		case TabMember:
		case TabWeb:
			{
				SeatTypeText = ""; // Tab ";
				Identifier = ""; // InitialOrder->TabName;
				break;
			}
		case TabTableSeat:
			{
				// Cant use Tab name here it could be to long.
				SeatTypeText = "";
				Identifier = IntToStr(InitialOrder->SeatNo);
				break;
			}
		case TabHandheldCashAccount:
		case TabCashAccount:
		case TabNone:
		default:
			// SeatTypeText = "Cash Sale";
			SeatTypeText = "";
			Identifier = "";
			QtyText = "";
			break;
		}
		if (SeatTypeText != "" || Identifier != "")
		{
			SeatsLine += QtyText + SeatTypeText + Identifier + ", ";
		}
	}

	if (InitialOrder->OrderType == CanceledOrder)
	{
		CancelLine = "!!!! CANCEL ORDERS !!!!";
	}
	else if (InitialOrder->OrderType == CreditNonExistingOrder)
	{
		CancelLine = "!!!! REFUND ORDERS !!!!";
	}

	// if (*SeatsLine.AnsiLastChar() == ',')
	// If the Per Seat Multipler is clear then remove the the seat information
	if (SeatsLine == SeatStart || Format.DishesPerSeatMultiplerString == "")
	{
		SeatsLine = "";
	}
	else
	{
		if (SeatsLine.SubString(SeatsLine.Length() - 1, SeatsLine.Length() - 1) == SeatSpacer)
		{
			SeatsLine = SeatsLine.SubString(1, SeatsLine.Length() - 2) + SeatEnd;
		}
		else
		{
			SeatsLine += SeatEnd;
		}
	}
	PrevPrinted = false;

	// -----------------------------------------------
	WideString Quantity = FormatFloat("0.###", CountQuantity);

	UnicodeString ItemName = "";
	UnicodeString SizeName = "";
	if (PrintJob->JobType == pjKitchen)
	{
		ItemName = InitialOrder->ItemKitchenName;
		SizeName =InitialOrder->SizeKitchenName;
	}
	else
	{
		ItemName = InitialOrder->Item;
		SizeName =InitialOrder->Size;
	}

	// format accordingly to the standards
	if (usingNMIStandardAus)
	{
		if(InitialOrder->IsPriceBarcodedItem)
		{
			// displaying price barcoded items in a special way
			ItemLine = ItemName;
			ItemWeight = FormatFloat("0.00", CountQuantity)
			+ " kg @ "
			+ CurrencyString
			+ CurrToStrF(InitialOrder->PriceEach(), ffNumber, CurrencyDecimals)
			+"/kg";
		}
		else if(InitialOrder->WeightedPrice.WeightedItem)
		{
			// weighted items are displayed in a special format. only for NMI
			ItemLine = ItemName;
			ItemWeight = FormatFloat("0.000", CountQuantity)
			+ " "
			+ InitialOrder->WeightedPrice.Weight.GetUnitAsString();

			bool hasTareWeight = InitialOrder->WeightedPrice.TareWeight.Valid()
			&& InitialOrder->WeightedPrice.TareWeight.ToDouble() > 0.00;

			ItemWeight += hasTareWeight ? " N" : "";

			ItemWeight += " @ "
			+ CurrencyString
			+ CurrToStrF(InitialOrder->PriceEach(), ffNumber, CurrencyDecimals)
			+"/"
			+  InitialOrder->WeightedPrice.Weight.GetUnitAsString();
		}
		else
		{
			// for non weighted items
			ItemLineSize = UpperCase(InitialOrder->Size) != "DEFAULT" ? SizeName : UnicodeString("");

			ItemLine 		= ItemName;
			ItemLineQty 	= "qty "
			+ Quantity
			+ " @ "
			+ CurrencyString
			+ InitialOrder->PriceEach()
			+ " each";
		}
	}
	else
	{
		// for general items
		if (UpperCase(InitialOrder->Size) != "DEFAULT")
		{
			ItemLine = Quantity + WideString(QtyMultiplier1) + SetMenuItemSpacer + SizeName + Spacer1 + ItemName;
			ItemLineSize = SizeName;
		}
		else
		{
			ItemLine = Quantity + WideString(QtyMultiplier1) + SetMenuItemSpacer + ItemName;
			ItemLineSize = "";
		}
	}

	Options->OptionCopyList(InitialOrder->OptionsSelected);

	if (InitialOrder->Note != "")
	{
		NoteLine = Format.NoteHeader + InitialOrder->Note;
	}

	if (InitialOrder->SubOrders->Count > 0)
	{
		// Print Line
		// Only Show Mix if Items is Orders from the Drinks Menu.
		/* Showing the Master container size was removed in 4-1-0-5 because
		1) No one used it.
		2) there is no pricing component accosiated with it.
		3) The Bar man will just pick a sensiable glass size based on the sizes of the
		mixes. */
		if (InitialOrder->ItemType == eDrinksItem)
		{
			/* if(UpperCase(InitialOrder->MasterContainer) != "DEFAULT")
			{
			MixLine = Format.MixHeader + InitialOrder->MasterContainer;
			}
			else
			{
			MixLine = "";
			} */
			MixLine = "";
		}
		else
		{
			SideLine = Format.SideHeader;
		}

		for (int i = 0; i < InitialOrder->SubOrders->Count; i++)
        {
            if(((!((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->printFreeSideForReceipt && PrintJob->JobType == pjReceiptReceipt)) || (!((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->printFreeSideForKitchen && PrintJob->JobType == pjKitchen))
             {
                UnicodeString ItemName = "";
                UnicodeString SizeName = "";
                if (PrintJob->JobType == pjKitchen)
                {
                    ItemName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->ItemKitchenName;
                    SizeName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->SizeKitchenName;
                }
                else
                {
                    ItemName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Item;
                    SizeName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Size;
                }

                if (UpperCase(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Size) != "DEFAULT")
                {
                    TItemSubSection SubItem;
                    SubItem.Caption = SetMenuItemSpacer + Format.BulletSide + SizeName + Spacer1 + ItemName;
                    SubItem.FontInfo = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->FontInfo;
                   if(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                   {
                       SubItem.isManuallyWeight=true;
                   }
                     else
                   {
                       SubItem.isManuallyWeight=false;
                   }
                   SubItems.push_back(SubItem);
                }
                else
                {
                    TItemSubSection SubItem;
                    SubItem.Caption = SetMenuItemSpacer + Format.BulletSide + ItemName;
                    SubItem.FontInfo = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->FontInfo;
                  if(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                   {
                       SubItem.isManuallyWeight=true;
                   }
                   else
                   {
                       SubItem.isManuallyWeight=false;
                   }
                   SubItems.push_back(SubItem);
                }
            }
		}
	}
}
// -----------------------------------------------------------------------------

_fastcall TOrderBundle::~TOrderBundle()
{
}
// -----------------------------------------------------------------------------

void TOrderBundle::SetNMIStandardAus( bool isNMIStandardAus )
{
	usingNMIStandardAus = isNMIStandardAus;
}
//------------------------------------------------------------------------------

bool TOrderBundle::isDifferentItemFromInitial( TItemComplete* InitialOrder, TItemComplete* CurrentOrder )
{
	bool result = false;

	result = (CurrentOrder->OrderType != NormalOrder)
 	|| (OptionsCompare(InitialOrder->OptionsSelected, CurrentOrder->OptionsSelected) != 0) 	// Options Differ
  	|| (SubOrdersCompare(InitialOrder->SubOrders, CurrentOrder->SubOrders) != 0) 			// SubOrders Differ
	|| (InitialOrder->Note != CurrentOrder->Note)
	|| (InitialOrder->SetMenuGroup != CurrentOrder->SetMenuGroup)
	|| (InitialOrder->MasterContainer != CurrentOrder->MasterContainer)
	|| (InitialOrder->Size != CurrentOrder->Size)
	|| ( seperatePriceChangedItemsNMI( InitialOrder, CurrentOrder )) 		// for NMI standards, tackles the same item, but with different prices
	|| ( seperateWeightedItemsNMI( CurrentOrder ) )							// for NMI standards, add each weighted item in its own bundle
	|| ( seperatePriceBarcodedItemsNMI( CurrentOrder) );					// for NMI standards, add price barcoded items in its own bundle

	return result;
}
//------------------------------------------------------------------------------

bool TOrderBundle::seperateWeightedItemsNMI( TItemComplete* currentOrder )
{
	return usingNMIStandardAus
	&& currentOrder->WeightedPrice.WeightedItem
	&& BundledOrders->Count > 0;
}
// -----------------------------------------------------------------------------

bool TOrderBundle::seperatePriceChangedItemsNMI ( TItemComplete* initialOrder, TItemComplete* currentOrder )
{
	return usingNMIStandardAus
	&& ( initialOrder->PriceEach() != currentOrder->PriceEach());
}
// -----------------------------------------------------------------------------

bool TOrderBundle::seperatePriceBarcodedItemsNMI ( TItemComplete* currentOrder )
{
	return usingNMIStandardAus
	&& currentOrder->IsPriceBarcodedItem
	&& BundledOrders->Count > 0;
}
// -----------------------------------------------------------------------------

void TPrintSection::PrintChefMateHorizontal(TReqPrintJob *PrintJob)
{
	WideString LastHeaderLine = "";
	TDateTime TimeStamp = Now();
	for (int i = 0; i < WorkingOrdersList->Count; )
	{
		TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

		if (TimeStamp > CurrentOrder->TimeStamp)
		{
			TimeStamp = CurrentOrder->TimeStamp;
		}

		TOrderBundle TempBundle;
		TempBundle.BundleOrders( PrintJob, WorkingOrdersList.get(), i, Format );

		TOrderBundle *CurrentOrderBundle = &TempBundle;

		std::auto_ptr<TStringList>ItemLine(new TStringList());
		WideString HeaderLine = "";

		TPrintInfo TotalFontInfo;
		TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
		TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
		TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
		TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
		TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
		TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;

		pPrinter->Line->FontInfo = TotalFontInfo;

		pPrinter->Line->ColCount = 1;
		pPrinter->Line->Columns[0]->Width = pPrinter->Width;
		pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

		HeaderLine = GetChefMateHeaderLine(PrintJob, CurrentOrder, ThisInstruction->FontInfo);
		if (HeaderLine == LastHeaderLine)
		{
			// Dont bother printing if it has already been printed.
			HeaderLine = "";
		}
		else
		{
			LastHeaderLine = HeaderLine;
		}

		GetChefMateItemLine(PrintJob, ThisInstruction->FontInfo, CurrentOrderBundle, ItemLine.get());

		if (HeaderLine != WideString(""))
		{
			pPrinter->Line->Columns[0]->Text = HeaderLine;
			pPrinter->AddLine();
		}

		for (int i = 0; i < ItemLine->Count; i++)
		{
			pPrinter->Line->Columns[0]->Text = ItemLine->Strings[i];
			pPrinter->AddLine();
		}
	}

}

WideString TPrintSection::GetChefMateHeaderLine(TReqPrintJob *PrintJob, TItemComplete *CurrentOrder, TPrintInfo inFontInfo)
{
	WideString HeaderLine = WideString("");
	std::set<long>SeatCounter;
	for (int i = 0; i < WorkingOrdersList->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)WorkingOrdersList->Items[i];
		if (Order->SeatNo > 0)
		{
			SeatCounter.insert(Order->SeatNo);
		}
	}

	UnicodeString HeaderName;
	if (CurrentOrder != NULL)
	{
		HeaderName = CurrentOrder->Terminal + " ";
		HeaderName += GetHeaderName(CurrentOrder);
	}

	int SeatCount = SeatCounter.size();
	if (SeatCount == 0)
	SeatCount = 1;

	int PatronCount = 0;
	if (PrintJob->Transaction != NULL)
	{
		PatronCount = TManagerPatron::Instance().GetTotalPatrons(PrintJob->Transaction->Patrons);
	}

	UnicodeString CurrentSeatCount = "";
	if (PatronCount == 0)
	{
		CurrentSeatCount = TGlobalSettings::Instance().SeatLabel + " Count :" + IntToStr(SeatCount);
	}
	else
	{
		CurrentSeatCount = "Patron Count :" + IntToStr(PatronCount);
	}

	UnicodeString TimeString = PrintJob->TimeStamp.FormatString("DD/MM hh:nn am/pm");
	UnicodeString ChitString = "";
	if (PrintJob->Transaction != NULL && PrintJob->Transaction->ChitNumber.Assigned())
	{
		ChitString = PrintJob->Transaction->ChitNumber.GetChitNumber();
	}

	UnicodeString Waiter = PrintJob->Waiter;

	HeaderLine = HeaderName + " " + ChitString + " " + CurrentSeatCount + " " + Waiter;
	return HeaderLine;
}

void TPrintSection::GetChefMateItemLine(TReqPrintJob *PrintJob, TPrintInfo inFontInfo, TOrderBundle *CurrentOrder, TStringList *Lines)
{
	WideString LastHeaderLine = "";
	UnicodeString CancelLine = "";

	WideString ItemLine = "";
	WideString SetMenuHeader = "";
	WideString SideLine = "";
	WideString OptionsLine = "";
	WideString SetMenuItemSpacer = "";
	WideString NotesLine = "";
	int SetMenuGroup = 0;

	if (CurrentOrder->SetMenuHeader != WideString(""))
	{
		SetMenuHeader = CurrentOrder->SetMenuHeader;
	}

	CancelLine = CurrentOrder->CancelLine;

	ItemLine = CurrentOrder->ItemLine;

	NotesLine = CurrentOrder->NoteLine;

	int SeatsListWidth = 0;

	for (int k = 0; k < CurrentOrder->SubItems.size(); k++)
	{
		SideLine += CurrentOrder->SubItems[k].Caption;
	}

	for (int k = 0; k < CurrentOrder->Options->Count; k++)
	{
		TItemOption *Option = CurrentOrder->Options->OptionGet(k);
		UnicodeString BulletOption = Option->IsPlus ? "+ " : "- ";
		OptionsLine += Format.BulletOpt + BulletOption + Option->KitchenName;
	}

	if (SetMenuHeader.Length() + CancelLine.Length() + ItemLine.Length() + SideLine.Length() + OptionsLine.Length() + NotesLine.Length() < pPrinter->NormalWidth)
	{
		Lines->Add(SetMenuHeader + CancelLine + ItemLine + SideLine + OptionsLine + NotesLine);
	}
	else if (SetMenuHeader.Length() + CancelLine.Length() + ItemLine.Length() + SideLine.Length() + OptionsLine.Length() < pPrinter->NormalWidth)
	{
		Lines->Add(SetMenuHeader + CancelLine + ItemLine + SideLine + OptionsLine);
		if (NotesLine != WideString(""))
		Lines->Add(NotesLine);
	}
	else if (SetMenuHeader.Length() + CancelLine.Length() + ItemLine.Length() + SideLine.Length() < pPrinter->NormalWidth)
	{
		Lines->Add(SetMenuHeader + CancelLine + ItemLine + SideLine);
		if (OptionsLine != WideString(""))
		Lines->Add(OptionsLine);
		if (NotesLine != WideString(""))
		Lines->Add(NotesLine);
	}
	else if (SetMenuHeader.Length() + CancelLine.Length() + ItemLine.Length() < pPrinter->NormalWidth)
	{
		Lines->Add(SetMenuHeader + CancelLine + ItemLine);
		if (SideLine != WideString(""))
		Lines->Add(SideLine);
		if (OptionsLine != WideString(""))
		Lines->Add(OptionsLine);
		if (NotesLine != WideString(""))
		Lines->Add(NotesLine);
	}
	else
	{
		Lines->Add(SetMenuHeader + CancelLine);
		Lines->Add(ItemLine);
		if (SideLine != WideString(""))
		Lines->Add(SideLine);
		if (OptionsLine != WideString(""))
		Lines->Add(OptionsLine);
		if (NotesLine != WideString(""))
		Lines->Add(NotesLine);
	}
}

void TPrintSection::GetChefMateMiscLine(TReqPrintJob *PrintJob, TPrintInfo inFontInfo, TOrderBundle *CurrentOrder, TStringList *Lines)
{

}
//----------------------------------------------------------------------------
void TPrintSection::PrintLoyaltyReceiptHeader( TReqPrintJob *PrintJob )
{
	AnsiString greeting = AnsiString(TGlobalSettings::Instance().ActivationTokenGreeting);
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->Columns[0]->Alignment = taCenter;
	pPrinter->Line->Columns[0]->Text = greeting;
	pPrinter->AddLine();
}
//----------------------------------------------------------------------------
void TPrintSection::PrintLoyaltyReceiptMemberNumber( TReqPrintJob *PrintJob )
{
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width / 2;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = "Member # : ";

	pPrinter->Line->Columns[1]->Width = pPrinter->Width / 2;
	pPrinter->Line->Columns[1]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Text = AnsiString(PrintJob->MiscData["memberNumber"]);

	pPrinter->AddLine();
}
//----------------------------------------------------------------------------
void TPrintSection::PrintLoyaltyReceiptMemberName( TReqPrintJob *PrintJob )
{
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width / 2;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = "Member Name : ";

	pPrinter->Line->Columns[1]->Width = pPrinter->Width / 2;
	pPrinter->Line->Columns[1]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Text = AnsiString(PrintJob->MiscData["memberName"]);

	pPrinter->AddLine();
}
//----------------------------------------------------------------------------
void TPrintSection::PrintLoyaltyReceiptActivationCode( TReqPrintJob *PrintJob )
{
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width / 2;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[0]->Text = "Activation code : ";

	pPrinter->Line->Columns[1]->Width = pPrinter->Width / 2;
	pPrinter->Line->Columns[1]->Alignment = taLeftJustify;
	pPrinter->Line->Columns[1]->Text = AnsiString(PrintJob->MiscData["activationToken"]);

	pPrinter->AddLine();
}
//----------------------------------------------------------------------------
void TPrintSection::PrintLoyaltyReceiptActivationInstructions( TReqPrintJob *PrintJob )
{
	printSingleLine("To activate your card, go to", pPrinter->Width,taCenter);
	printSingleLine("",pPrinter->Width, taCenter);

	printParagraph(TGlobalSettings::Instance().MemberActivationURL, pPrinter->Width);

	printSingleLine("",pPrinter->Width, taCenter);
	printParagraph("Click on 'Activate Online' button and enter the code above.", pPrinter->Width);
}
//----------------------------------------------------------------------------
void TPrintSection::printParagraph(UnicodeString text, int columnWidth)
{
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = columnWidth;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString remainder = text;
	int currentLineWidth = columnWidth;

	while(text.Length() >= columnWidth)
	{
		//TODO: check for \r character if it is there break the line from that position

		text = remainder.SubString(0,currentLineWidth);
		remainder = remainder.Delete(1,currentLineWidth);
		pPrinter->Line->Columns[0]->Text = AnsiString(text);
		pPrinter->AddLine();
	}

	pPrinter->Line->Columns[0]->Text = AnsiString(remainder);
	pPrinter->AddLine();
}
//---------------------------------------------------------------------------
void TPrintSection::printSingleLine(UnicodeString text, int columnWidth, TAlignment alignment)
{
	pPrinter->Line->ColCount = 1;
	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
	pPrinter->Line->Columns[0]->Width = columnWidth;
	pPrinter->Line->Columns[0]->Alignment = alignment;
	pPrinter->Line->Columns[0]->Text = AnsiString(text);
	pPrinter->AddLine();
}
//----------------------------------------------------------------------------

void TPrintSection::printItemScales( TReqPrintJob *PrintJob )
{
	bool IncludeDiscounts = shouldDisplayDiscountedPrice();

	TList *OrderBundle;
	try
	{
		OrderBundle = new TList;
		TDateTime TimeStamp = Now();
		for (int i = 0; i < WorkingOrdersList->Count; )
		{
			TItemComplete *CurrentOrder = (TItemComplete*)WorkingOrdersList->Items[i];

			if (TimeStamp > CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}

			TOrderBundle *TempBundle = new TOrderBundle();
			TempBundle->SetNMIStandardAus(true);
			TempBundle->BundleOrders(PrintJob, WorkingOrdersList.get(), i, Format);

			OrderBundle->Add(TempBundle);
		}

		for (int j = 0; j < OrderBundle->Count; j++)
		{
			TOrderBundle *CurrentOrderBundle = (TOrderBundle*)OrderBundle->Items[j];
			TOrderBundle *NextOrderBundle = NULL;

			TPrintInfo TotalFontInfo;
			if (PrintJob->JobType == pjKitchen)
			{
				TotalFontInfo.Bold = CurrentOrderBundle->FontInfo.Bold || ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = CurrentOrderBundle->FontInfo.Underlined || ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ((CurrentOrderBundle->FontInfo.Width == fsDoubleSize) || (ThisInstruction->FontInfo.Width == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Height = ((CurrentOrderBundle->FontInfo.Height == fsDoubleSize) || (ThisInstruction->FontInfo.Height == fsDoubleSize)) ? fsDoubleSize : fsNormalSize;
				TotalFontInfo.Colour = ((CurrentOrderBundle->FontInfo.Colour == fcRed) || (ThisInstruction->FontInfo.Colour == fcRed)) ? fcRed : fcBlack;
				TotalFontInfo.Font = ((CurrentOrderBundle->FontInfo.Font == ftFontB) || (ThisInstruction->FontInfo.Font == ftFontB)) ? ftFontB : ftFontA;
			}
			else
			{
				TotalFontInfo.Bold = ThisInstruction->FontInfo.Bold;
				TotalFontInfo.Underlined = ThisInstruction->FontInfo.Underlined;
				TotalFontInfo.Width = ThisInstruction->FontInfo.Width;
				TotalFontInfo.Height = ThisInstruction->FontInfo.Height;
				TotalFontInfo.Colour = ThisInstruction->FontInfo.Colour;
				TotalFontInfo.Font = ThisInstruction->FontInfo.Font;
			}

			pPrinter->Line->FontInfo = TotalFontInfo;

			// Display the Items.
			pPrinter->Line->ColCount = 2;
			pPrinter->Line->Columns[0]->Width = pPrinter->Width;
			pPrinter->Line->FontInfo.Bold = false;
			pPrinter->Line->Columns[1]->Alignment = taRightJustify;
			pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

			UnicodeString ItemName = CurrentOrderBundle->ItemLine;
			UnicodeString ItemPrice = CurrToStrF(RoundToNearest(TOrderUtils::TotalPriceSides(CurrentOrderBundle->BundledOrders.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown), ffNumber,
			CurrencyDecimals);

			if (IncludeDiscounts)
			{
				ItemPrice = CurrToStrF(RoundToNearest(TOrderUtils::TotalPriceAdjustmentSides(CurrentOrderBundle->BundledOrders.get()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown), ffNumber,
				CurrencyDecimals);
			}

            int item_len = GetItemLength(CurrentOrderBundle->ItemLine, CurrentOrderBundle->ItemLine.Length());

			pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
            if(item_len > 0)
            {
               pPrinter->Line->Columns[0]->Width = pPrinter->Width -  pPrinter->Line->Columns[1]->Width - item_len;
            }
            else
            {
               pPrinter->Line->Columns[0]->Width = pPrinter->Width -  ItemPrice.Length() - 1;
            }

			//pPrinter->Line->Columns[1]->Width = ItemPrice.Length() + 1;
			//pPrinter->Line->Columns[0]->Width = pPrinter->Width - ItemPrice.Length() - 1;

			pPrinter->Line->Columns[0]->Text = ItemName;
			pPrinter->Line->Columns[1]->Text = (PrintJob->Transaction->TypeOfSale == NonChargableSale) ? UnicodeString::UnicodeString() : ItemPrice;
			pPrinter->AddLine();

			if (CurrentOrderBundle->ItemWeight != "")
			{
				pPrinter->Line->ColCount = 1;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->FontInfo.Bold = false;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemWeight;
				pPrinter->AddLine();
              //Section start 8248
                PrintManuallyEnteredWeightString(CurrentOrderBundle,pPrinter);
              //Section end 8248
			}
			else
			{
				pPrinter->Line->ColCount = 1;
				pPrinter->Line->Columns[0]->Width = pPrinter->Width;
				pPrinter->Line->FontInfo.Bold = false;
				pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
				pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemLineQty;
				pPrinter->AddLine();
			}

			if (CurrentOrderBundle->SubItems.size() > 0)
			{
				pPrinter->Line->ColCount = 2;

				if (!CurrentOrderBundle->SideLine.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SideLine;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();

				}

				for (int k = 0; k < CurrentOrderBundle->SubItems.size(); k++)
				{
					if (PrintJob->JobType == pjKitchen)
					{
						pPrinter->Line->FontInfo = CurrentOrderBundle->SubItems[k].FontInfo;
					}
					pPrinter->Line->Columns[0]->Kanji = pPrinter->KanjiPrinter;
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->SubItems[k].Caption;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();


                   if( CurrentOrderBundle->SubItems[k].isManuallyWeight && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
                    {
                        AddManuallyEnteredLineInReceipt(pPrinter);
                    }

				}

				if (!CurrentOrderBundle->MixLine.IsEmpty())
				{
					pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->MixLine;
					pPrinter->Line->Columns[1]->Text = "";
					pPrinter->AddLine();
				}
			}

			TPtrSectionInstructStorage::iterator ptrSubInstruction = ThisInstruction->SubInstructions.begin();
			while (ptrSubInstruction != ThisInstruction->SubInstructions.end())
			{
				TPSectionInstruction *itInstruction = *ptrSubInstruction;
				// Get The Iterator that iterator points too.
				// Process the Note & Options only once.
				if (((itInstruction->Instruction == epofiPrintItemNote) || (itInstruction->Instruction == epofiPrintItemOptions)) && CurrentOrderBundle->BundledOrders->Count > 0)
				{
					std::auto_ptr<TList>SingleOrders(new TList);
					SingleOrders->Add(CurrentOrderBundle->BundledOrders->Items[0]);
					TPrintSection NewSection(pPrinter, SingleOrders.get(), MasterInstructionsList, itInstruction, Format);
					NewSection.ProcessSection(PrintJob);
				}
				else
				{
					TPrintSection NewSection(pPrinter, CurrentOrderBundle->BundledOrders.get(), MasterInstructionsList, itInstruction, Format);
					NewSection.ProcessSection(PrintJob);
				}
				advance(ptrSubInstruction, 1);
			}
		}
	}
	__finally
	{
		while (OrderBundle->Count != 0)
		{
			delete(TOrderBundle*)OrderBundle->Items[0];
			OrderBundle->Delete(0);
		}
		delete OrderBundle;
	}
}
//----------------------------------------------------------------------------

void TPrintSection::printCurrencySymbol( TReqPrintJob *PrintJob )
{
	printSingleLine( CurrencyString, pPrinter->Width, taRightJustify );

	//throw new Exception( "Not implemented !");
}
//----------------------------------------------------------------------------

bool TPrintSection::shouldDisplayDiscountedPrice()
{
	// displaying the discounted price by default
	bool result = true;

	TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.end())
	{
		if (i->Instruction == epofiPrintTotalDiscount || i->Instruction == epofiPrintTotalDiscountsDetails || i->Instruction == epofiPrintTotalDiscountsName)
		{
			result = false;
		}
		advance(i, 1);
	}

	return result;
}
//----------------------------------------------------------------------------

void TPrintSection::PrintDeliveryTime(TReqPrintJob *PrintJob)
{
 TSectionInstructStorage::iterator i = ThisInstruction;
	while (i != MasterInstructionsList.begin())
	{

		advance(i, -1);
		if (i->Instruction == epofiPrintDeliveryTime )
		{
			break;
		}
	}

	pPrinter->Line->FontInfo = ThisInstruction->FontInfo;

	// Display the Items.
	pPrinter->Line->ColCount = 2;
	pPrinter->Line->Columns[0]->Width = pPrinter->Width;
	pPrinter->Line->FontInfo.Bold = false;
	pPrinter->Line->Columns[1]->Alignment = taRightJustify;
	pPrinter->Line->Columns[0]->Alignment = taLeftJustify;

	UnicodeString ItemName = ThisInstruction->Caption;





  if((double)PrintJob->Transaction->ChitNumber.DeliveryTime > 0)
    {
        pPrinter->Line->FontInfo = ThisInstruction->FontInfo;
        // Display the Items.
        pPrinter->Line->ColCount = 2;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->FontInfo.Bold = false;
        pPrinter->Line->Columns[1]->Alignment = taRightJustify;
        pPrinter->Line->Columns[0]->Alignment = taLeftJustify;
        UnicodeString Caption =ItemName + " :"; //"Delivery Time :";
        AnsiString DeliveryTime = "";
        DeliveryTime = PrintJob->Transaction->ChitNumber.DeliveryTime.FormatString("DD/MM/YYYY hh:mm am/pm");
        pPrinter->Line->Columns[1]->Width = DeliveryTime.Length() + 1;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width - DeliveryTime.Length() - 1;
        pPrinter->Line->Columns[0]->Text = Caption;
        pPrinter->Line->Columns[1]->Text = DeliveryTime;
        pPrinter->AddLine();
   }
}

void TPrintSection::PrintTextForUnRegisteredDatabase()
{
    pPrinter->Line->Columns[0]->Text ="NOT FOR RESALE";
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text ="THIS SOFTWARE VERSION IS";
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text ="NOT REGISTERED CONTACT MENUMATE";
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text ="On SUPPORT@MENUMATE.COM TO BE";
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text = "REGISTERED CORRECTLY USE OF" ;
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text = "NON REGISTERED SOFTWARE MAY ";
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text = "RESULT IN ADDITIONAL LEGAL COSTS";
    pPrinter->AddLine();
    pPrinter->Line->Columns[0]->Text = "AND EXPENSES ";
    pPrinter->AddLine();
 }

//----------------------------------------------------------------------------------

void TOrderBundle::BundleOrdersWithSides(
TReqPrintJob *PrintJob,
TList *Orders,
int &CurrentIndex,
TDocketFormat &inFormat)
{
	// initializations
	UnicodeString QtyMultiplier1 	= UnicodeString("x ");
	UnicodeString QtyMultiplier10 	= UnicodeString("x");
	UnicodeString Spacer1 			= UnicodeString(" ");
	UnicodeString SeatStart 		= UnicodeString("(");
	UnicodeString SeatEnd 			= UnicodeString(")");
	UnicodeString SeatSpacer 		= UnicodeString(", ");
	UnicodeString SetMenuItemSpacer = "";
	SeatsLine 						= SeatStart;
	Format 							= inFormat;
	TItemComplete *InitialOrder 	= (TItemComplete*)(Orders->Items[CurrentIndex]);
	FontInfo 						= InitialOrder->FontInfo;

    WideString groupItemLine ;
    WideString ItemSideLine;

	if (CurrentIndex == 0)
	{
		if (InitialOrder->SetMenuMask != 0 && InitialOrder->SetMenuGroup != 0)
		{
			if (Format.KeyItemHeader[InitialOrder->SetMenuGroup] != WideString(""))
			{
				SetMenuHeader = "[" + Format.KeyItemHeader[InitialOrder->SetMenuGroup] + "]";
			}
			SetMenuGroup = InitialOrder->SetMenuGroup;
			SetMenuItemSpacer = Format.BulletSetMenuItem;
		}
	}
	else if (CurrentIndex > 0)
	{
		TItemComplete *PrevOrder = (TItemComplete*)(Orders->Items[CurrentIndex - 1]);
		if (InitialOrder->SetMenuMask != 0 && InitialOrder->SetMenuGroup != 0)
		{
			if ((InitialOrder->SetMenuGroup != PrevOrder->SetMenuGroup) || (InitialOrder->CourseKitchenName != PrevOrder->CourseKitchenName))
			{
				if (Format.KeyItemHeader[InitialOrder->SetMenuGroup] != WideString(""))
				{
					SetMenuHeader = "[" + Format.KeyItemHeader[InitialOrder->SetMenuGroup] + "]";
				}
				SetMenuGroup = InitialOrder->SetMenuGroup;
				SetMenuItemSpacer = Format.BulletSetMenuItem;
			}
			else
			{
				SetMenuGroup = InitialOrder->SetMenuGroup;
			}
		}
	}

	Currency CountQuantity = 0;
    Currency sideQuantity =0;
	int CountSeatQuantity = 0;
    Currency sideCount = 0;
    Currency optionQuantity = 0;
	UnicodeString LastItem = ((TItemComplete*)(Orders->Items[CurrentIndex]))->ItemKitchenName;

	// As long as it;s a normal order bundle it.
	if (InitialOrder->OrderType == NormalOrder)
	{
		while ((CurrentIndex < Orders->Count) && (LastItem == ((TItemComplete*)(Orders->Items[CurrentIndex]))->ItemKitchenName))
		{
			TItemComplete *CurrentOrder = (TItemComplete*)(Orders->Items[CurrentIndex]);
			if ( isDifferentItemFromInitial( InitialOrder, CurrentOrder ) )
			{
				// Ok the next order does not match this one so write out this ones
				// Seat Information, Yes its a real pain being retrospective....
				TItemComplete *PrevOrder = (TItemComplete*)(Orders->Items[CurrentIndex - 1]);
               
                if(((SubOrdersCompare(InitialOrder->SubOrders, CurrentOrder->SubOrders) != 0) ||
                            (OptionsCompare(InitialOrder->OptionsSelected, CurrentOrder->OptionsSelected) != 0))
                                && (InitialOrder->Size == CurrentOrder->Size))
                {
                        if(OptionsCompare(InitialOrder->OptionsSelected, CurrentOrder->OptionsSelected) != 0)
                        {

                         	Options->OptionCopyList(InitialOrder->OptionsSelected);

                            SaveOptions(InitialOrder->OptionsSelected, optionQuantity);



                        }

                        if((SubOrdersCompare(InitialOrder->SubOrders, CurrentOrder->SubOrders) != 0))
                        {

                            for (int i = 0; i < InitialOrder->SubOrders->Count; i++)
                            {
                                UnicodeString ItemName = "";
                                UnicodeString SizeName = "";
                                UnicodeString sideName = "";
                                bool isSameSide = false;

                                if (PrintJob->JobType == pjKitchen)
                                {
                                    ItemName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->ItemKitchenName;
                                    SizeName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->SizeKitchenName;
                                }
                                else
                                {
                                    ItemName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Item;
                                    SizeName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Size;
                                }

                                if (UpperCase(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Size) != "DEFAULT")
                                {
                                    TItemSubSection SubItem;
                                    sideName = WideString(QtyMultiplier1) + SizeName + Spacer1 + ItemName;

                                    //check if side already added than only increase it's quantity
                                    isSameSide = IsSideAlreadyExist(sideName, sideQuantity);

                                    if(!isSameSide)
                                    {
                                        SubItem.Caption = sideName;
                                        SubItem.SideQuantity = sideQuantity;
                                        SubItem.FontInfo = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->FontInfo;
                                        SubItem.SideBullets = Format.BulletSide;
                                        if(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                                        {
                                           SubItem.isManuallyWeight=true;
                                         }
                                           else
                                           {
                                               SubItem.isManuallyWeight=false;
                                           }
                                        SubItems.push_back(SubItem);
                                    }
                                }
                                else
                                {
                                    TItemSubSection SubItem;
                                    sideName = WideString(QtyMultiplier1) + ItemName;
                                    isSameSide = IsSideAlreadyExist(sideName, sideQuantity);

                                    if(!isSameSide)
                                    {
                                        SubItem.Caption = sideName;
                                        SubItem.FontInfo = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->FontInfo;
                                        SubItem.SideQuantity = sideQuantity;
                                        SubItem.SideBullets = Format.BulletSide;
                                        if(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                                        {
                                           SubItem.isManuallyWeight=true;
                                         }
                                           else
                                       {
                                           SubItem.isManuallyWeight=false;
                                       }
                                       SubItems.push_back(SubItem);
                                    }
                                }
                            }

                      }
                      sideQuantity = 0;
                      optionQuantity = 0;
                      InitialOrder 	= (TItemComplete *)(Orders->Items[CurrentIndex]);
                }

                else
                {
				// It may have been writtren out below if the next seat No if differnt
				// this traps for that.
				if (CountSeatQuantity > 0)
				{
					// Sort out the seat name.
					UnicodeString QtyText = "";
					UnicodeString SeatTypeText = "";
					UnicodeString Identifier = "";

					if (CountSeatQuantity > 1)
					{
						QtyText = IntToStr(CountSeatQuantity) + Format.DishesPerSeatMultiplerString;
					}

					switch(PrevOrder->TabType)
					{
					case TabNormal:
					case TabStaff:
					case TabMember:
					case TabWeb:
						{
							SeatTypeText = ""; // Tab ";
							Identifier = ""; // PrevOrder->TabName;
							break;
						}
					case TabTableSeat:
						{
							// Cant use Tab name here it could be to long.
							SeatTypeText = "";
							Identifier = IntToStr(PrevOrder->SeatNo);
							break;
						}
					case TabHandheldCashAccount:
					case TabCashAccount:
					case TabNone:
					default:
						// SeatTypeText = "Cash Sale";
						SeatTypeText = ""; // Cash Sale";
						Identifier = "";
						QtyText = "";
						break;
					}
					if (SeatTypeText != "" || Identifier != "")
					{
						SeatsLine += QtyText + SeatTypeText + Identifier + ", ";
					}
				}

				break; // break right out of the loop.
			}
            }

           if(CurrentOrder->isManuallyEnteredWeight)
            {
                this->addManuallyEnteredWeightString=true;
            }
            else
            {
                this->addManuallyEnteredWeightString = false;
            }

			BundledOrders->Add(CurrentOrder);

			CountQuantity += CurrentOrder->GetQty();
            if(InitialOrder->SubOrders->Count > 0)
            {
                sideQuantity +=  ((TItemCompleteSub*)(CurrentOrder->SubOrders->Items[0]))->GetQty();
            }
            if(InitialOrder->OptionsSelected->Count >0)
            {
                optionQuantity += CurrentOrder->GetQty();
            }
                              
			// Needed to sence seat name change.   Current Index is incremented so the
			// next reference to it is already pointing to the next seat.
			unsigned int OldTabKey = CurrentOrder->TabKey;
			CurrentIndex++;
			CountSeatQuantity++;

			unsigned int NewTabKey = (CurrentIndex < Orders->Count) ? ((TItemComplete*)(Orders->Items[CurrentIndex]))->TabKey : -1;
			UnicodeString NextItem = (CurrentIndex < Orders->Count) ? ((TItemComplete*)(Orders->Items[CurrentIndex]))->ItemKitchenName : UnicodeString("");
			// The Next seat is not the same as this one write out this seat name and
			// quantity of items assigned to it.
			// Or The Next Item is not the same and the While loop will exit.
			// Or this is the Last Order so write out seat name and quantity.
			if ((OldTabKey != NewTabKey) || (LastItem != NextItem) || (CurrentIndex == Orders->Count))
			{
				// Sort out the seat name.
				// Sort out the seat name.
				UnicodeString QtyText = "";
				UnicodeString SeatTypeText = "";
				UnicodeString Identifier = "";

				if (CountSeatQuantity > 1)
				{
					QtyText = IntToStr(CountSeatQuantity) + Format.DishesPerSeatMultiplerString;
				}

				switch(CurrentOrder->TabType)
				{
				case TabNormal:
				case TabStaff:
				case TabMember:
				case TabWeb:
					{
						SeatTypeText = ""; // Tab ";
						Identifier = ""; // CurrentOrder->TabName;
						break;
					}
				case TabTableSeat:
					{
						// Cant use Tab name here it could be to long.
						SeatTypeText = "";
						Identifier = IntToStr(CurrentOrder->SeatNo);
						break;
					}
				case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
				default:
					// SeatTypeText = "Cash Sale";
					SeatTypeText = "";
					Identifier = "";
					QtyText = "";
					break;
				}
				if (SeatTypeText != "" || Identifier != "")
				{
					SeatsLine += QtyText + SeatTypeText + Identifier + ", ";
				}
				CountSeatQuantity = 0; // reset seat qty.
			}

			// Update the timestamp to the oldest order.
			if (TimeStamp < CurrentOrder->TimeStamp)
			{
				TimeStamp = CurrentOrder->TimeStamp;
			}
		}
	}
	else
	{
		BundledOrders->Add(InitialOrder);
		CountQuantity += InitialOrder->GetQty();
        optionQuantity += InitialOrder->GetQty();

		CurrentIndex++;
		UnicodeString QtyText = "";
		UnicodeString SeatTypeText = "";
		UnicodeString Identifier = "";

		QtyText = FormatFloat("0.##", CountQuantity) + Format.DishesPerSeatMultiplerString;

		switch(InitialOrder->TabType)
		{
		case TabNormal:
		case TabStaff:
		case TabMember:
		case TabWeb:
			{
				SeatTypeText = ""; // Tab ";
				Identifier = ""; // InitialOrder->TabName;
				break;
			}
		case TabTableSeat:
			{
				// Cant use Tab name here it could be to long.
				SeatTypeText = "";
				Identifier = IntToStr(InitialOrder->SeatNo);
				break;
			}
		case TabHandheldCashAccount:
		case TabCashAccount:
		case TabNone:
		default:
			// SeatTypeText = "Cash Sale";
			SeatTypeText = "";
			Identifier = "";
			QtyText = "";
			break;
		}
		if (SeatTypeText != "" || Identifier != "")
		{
			SeatsLine += QtyText + SeatTypeText + Identifier + ", ";
		}
	}

	if (InitialOrder->OrderType == CanceledOrder)
	{
		CancelLine = "!!!! CANCEL ORDERS !!!!";
	}
	else if (InitialOrder->OrderType == CreditNonExistingOrder)
	{
		CancelLine = "!!!! REFUND ORDERS !!!!";
	}

	// if (*SeatsLine.AnsiLastChar() == ',')
	// If the Per Seat Multipler is clear then remove the the seat information
	if (SeatsLine == SeatStart || Format.DishesPerSeatMultiplerString == "")
	{
		SeatsLine = "";
	}
	else
	{
		if (SeatsLine.SubString(SeatsLine.Length() - 1, SeatsLine.Length() - 1) == SeatSpacer)
		{
			SeatsLine = SeatsLine.SubString(1, SeatsLine.Length() - 2) + SeatEnd;
		}
		else
		{
			SeatsLine += SeatEnd;
		}
	}
	PrevPrinted = false;

	// -----------------------------------------------
	WideString Quantity = FormatFloat("0.###", CountQuantity);
    Currency countSides = FormatFloat("0.###", sideQuantity);


	UnicodeString ItemName = "";
	UnicodeString SizeName = "";
	if (PrintJob->JobType == pjKitchen)
	{
		ItemName = InitialOrder->ItemKitchenName;
		SizeName =InitialOrder->SizeKitchenName;
	}
	else
	{
		ItemName = InitialOrder->Item;
		SizeName =InitialOrder->Size;
	}

	// format accordingly to the standards
	if (usingNMIStandardAus)
	{
		if(InitialOrder->IsPriceBarcodedItem)
		{
			// displaying price barcoded items in a special way
			ItemLine = ItemName;
			ItemWeight = FormatFloat("0.00", CountQuantity)
			+ " kg @ "
			+ CurrencyString
			+ CurrToStrF(InitialOrder->PriceEach(), ffNumber, CurrencyDecimals)
			+"/kg";
		}
		else if(InitialOrder->WeightedPrice.WeightedItem)
		{
			// weighted items are displayed in a special format. only for NMI
			ItemLine = ItemName;
			ItemWeight = FormatFloat("0.000", CountQuantity)
			+ " "
			+ InitialOrder->WeightedPrice.Weight.GetUnitAsString();

			bool hasTareWeight = InitialOrder->WeightedPrice.TareWeight.Valid()
			&& InitialOrder->WeightedPrice.TareWeight.ToDouble() > 0.00;

			ItemWeight += hasTareWeight ? " N" : "";

			ItemWeight += " @ "
			+ CurrencyString
			+ CurrToStrF(InitialOrder->PriceEach(), ffNumber, CurrencyDecimals)
			+"/"
			+  InitialOrder->WeightedPrice.Weight.GetUnitAsString();
		}
		else
		{
			// for non weighted items
			ItemLineSize = UpperCase(InitialOrder->Size) != "DEFAULT" ? SizeName : UnicodeString("");

			ItemLine 		= ItemName;
			ItemLineQty 	= "qty "
			+ Quantity
			+ " @ "
			+ CurrencyString
			+ InitialOrder->PriceEach()
			+ " each";
		}
	}
	else
	{
		// for general items
		if (UpperCase(InitialOrder->Size) != "DEFAULT")
		{
		    ItemLine = Quantity + Spacer1 + WideString(QtyMultiplier1) + SizeName + Spacer1 + ItemName;
			ItemLineSize = SizeName;

		}
		else
		{
			ItemLine = Quantity + Spacer1 + WideString(QtyMultiplier1)  + ItemName;
			ItemLineSize = "";
		}
	}
     	Options->OptionCopyList(InitialOrder->OptionsSelected);
        SaveOptions(InitialOrder->OptionsSelected, optionQuantity);

	if (InitialOrder->Note != "")
	{
		NoteLine = Format.NoteHeader + InitialOrder->Note;
	}

	if (InitialOrder->SubOrders->Count > 0)
	{
		if (InitialOrder->ItemType == eDrinksItem)
		{

			MixLine = "";
		}
		else
		{
			SideLine = Format.SideHeader;
		}

		for (int i = 0; i < InitialOrder->SubOrders->Count; i++)
        {

            if(((!((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->printFreeSideForReceipt && PrintJob->JobType == pjReceiptReceipt)) || (!((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->printFreeSideForKitchen && PrintJob->JobType == pjKitchen))
            {
                UnicodeString ItemName = "";
                UnicodeString SizeName = "";
                bool isSameSide = false;
                if (PrintJob->JobType == pjKitchen)
                {
                    ItemName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->ItemKitchenName;
                    SizeName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->SizeKitchenName;
                }
                else
                {
                    ItemName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Item;
                    SizeName = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Size;
                }

                if (UpperCase(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->Size) != "DEFAULT")
                {
                    UnicodeString sideName = WideString(QtyMultiplier1) + SizeName + Spacer1 + ItemName;
                    isSameSide = IsSideAlreadyExist(sideName, countSides);

                    if(!isSameSide)
                    {

                        TItemSubSection SubItem;
                        SubItem.SideQuantity = countSides;
                        SubItem.Caption = sideName;
                        SubItem.FontInfo = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->FontInfo;
                        SubItem.SideBullets = Format.BulletSide;
                        if(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                          {
                             SubItem.isManuallyWeight=true;
                         }
                           else
                           {
                               SubItem.isManuallyWeight=false;
                           }
                        SubItems.push_back(SubItem);
                    }
                }
                else
                {


                    UnicodeString sideName = WideString(QtyMultiplier1) + ItemName;
                    isSameSide = IsSideAlreadyExist(sideName, countSides);

                    if(!isSameSide)
                    {
                            TItemSubSection SubItem;
                            SubItem.SideQuantity = countSides;
                            SubItem.Caption = sideName;
                            SubItem.FontInfo = ((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->FontInfo;
                            SubItem.SideBullets = Format.BulletSide;
                            if(((TItemCompleteSub*)(InitialOrder->SubOrders->Items[i]))->GetIsManuallyEnteredWeight() )
                             {
                                    SubItem.isManuallyWeight=true;
                             }
                             else
                           {
                               SubItem.isManuallyWeight=false;
                           }
                            SubItems.push_back(SubItem);
                    }

                }
            }
		}
	}
}
// -----------------------------------------------------------------------------
//checking if side already added to vector than increase it's quantity only
bool TOrderBundle::IsSideAlreadyExist(UnicodeString sideCaption, Currency quantity)
{
    bool isSame = false;
    for(int j=0; j<SubItems.size(); j++)
    {
        if(SubItems[j].Caption == sideCaption)
        {
          isSame = true;
          SubItems[j].SideQuantity += quantity;
        }
    }
    return isSame;
}
// -----------------------------------------------------------------------------
//Printing options with quantity for groupitem template
void TPrintSection::PrintOptionsWithQuantity(TOrderBundle *CurrentOrderBundle, TPrintFormat *pPrinter)
{
    UnicodeString qtyMultiplier = " x ";
    UnicodeString optionQuantity;
    UnicodeString SetMenuItemSpacer = " ";


	if (CurrentOrderBundle->ItemOptions.size() > 0)
	{
        for (int k = 0; k < CurrentOrderBundle->ItemOptions.size(); k++)
        {
            optionQuantity =  CurrentOrderBundle->ItemOptions[k].OptionQuantity;
            pPrinter->Line->Columns[0]->Text = CurrentOrderBundle->ItemOptions[k].BulletOpt + CurrentOrderBundle->ItemOptions[k].BulletOption + optionQuantity + qtyMultiplier + CurrentOrderBundle->ItemOptions[k].Caption;
            pPrinter->Line->Columns[1]->Text = "";
            pPrinter->AddLine();
        }
	}
}
// -----------------------------------------------------------------------------
//checking if option already added to vector than increase it's quantity only
bool TOrderBundle::IsOptionAlreadyExist(UnicodeString optionName, Currency quantity)
{
    bool isSame = false;
    for(int j=0; j<ItemOptions.size(); j++)
    {
        if(ItemOptions[j].Caption == optionName)
        {
          isSame = true;
          ItemOptions[j].OptionQuantity += quantity;
        }
    }
    return isSame;
}
// -----------------------------------------------------------------------------
// save options to the vector
void TOrderBundle::SaveOptions(TListOptionContainer *Options1, Currency quantity)
{
    for (int i = 0; i < Options1->Count; i++)
    {
        UnicodeString optionName = "";
        bool isSameOption = false;

        TItemOption* newOption = Options1->OptionGet(i);

        TItemOptionSection option;

            optionName = newOption->KitchenName;

            //check if side already added than only increase it's quantity
            isSameOption = IsOptionAlreadyExist(optionName, quantity);

            if(!isSameOption)
            {
                option.Caption = newOption->KitchenName;
                option.OptionQuantity += quantity;
                option.BulletOption = newOption->IsPlus ? "+ " : "- ";
                option.BulletOpt = Format.BulletOpt;
                ItemOptions.push_back(option);
            }
    }
}



void TPrintSection::PrintManuallyEnteredWeightString(TOrderBundle* orderbundle, 	TPrintFormat* pPrinter )
 {
     if(orderbundle->addManuallyEnteredWeightString && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
        {
            AddManuallyEnteredLineInReceipt(pPrinter);
        }
 }

 void TPrintSection::AddManuallyEnteredLineInReceipt(TPrintFormat*  pPrinter)
 {
        UnicodeString str=" (Manually entered weight)";
     //   pPrinter->Line->Columns[1]->Width = str.Length() + 1;
        pPrinter->Line->Columns[0]->Width = str.Length() ;
        pPrinter->Line->Columns[0]->Text = str;

        if(pPrinter->Line->ColCount ==2)pPrinter->Line->Columns[1]->Text = "";

        pPrinter->AddLine();
 }
