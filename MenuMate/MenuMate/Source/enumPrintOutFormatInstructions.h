#ifndef enumPrintOutFormatInstructionsH
#define enumPrintOutFormatInstructionsH

#include <Classes.hpp>
#include <utility>
#include <map>

enum ePrintOutFormatInstructions{
                                epofiPrintKitchenInfo,
								 epofiPrintSeat,
								 epofiPrintServingCourse,
								 epofiPrintCourse,
								 epofiPrintItem,
								 epofiPrintItems,
								 epofiSortByFoodDrink,
								 epofiPrintBarCode,
								 epofiBreak,
								 epofiFilterFood,
								 epofiFilterBev,
								 epofiFormatChefMate,
								 epofiPrintMembersName,
								 epofiPrintMembersDetails,
								 epofiPrintPartyName,
								 epofiFilterOutCancels,
								 epofiFilterOutFree,
								 epofiSortByDateOrdered,
								 epofiPrintPointsDetails,
								 epofiPrintItemTotal,
								 epofiPrintItemsTotal,
								 epofiPrintItemNote,
								 epofiPrintTotalEx,
								 epofiPrintTotal,
								 epofiPrintTotalDiscount,
								 epofiPrintTotalDiscountsDetails,
								 epofiPrintTotalDiscountsName,
								 epofiPrintAccCreditInfo,
								 epofiPrintMsg,
								 epofiPrintSoldBy,
								 epofiPrintChitNumber,
								 epofiPrintReceiptLogo,
								 epofiPrintReceiptHeader,
								 epofiPrintReceiptFooter,
								 epofiPrintPaymentTotals,
								 epofiPrintWaitTimes,
								 epofiPrintPaymentSurcharges,
								 epofiPrintSeparatedTaxTotals,
								 epofiPrintItemSides,
								 epofiPrintItemOptions,
								 epofiPrintExtraInfo,
								 epofiPrintReceiptInfo,
								 epofiPrintTable,
								 epofiPrintHistory,
								 epofiPrintAccChargeInfo,
								 epofiOpenCashDrawer,
								 epofiPrintPaymentInfo,
								 epofiPrintDeliveryInfo,
								 epofiPrintWebOrderComment,
								 epofiPrintProductTotalsWithUnitPrice,
								 epofiPrintCustomerName,
								 epofiHotelCustomerName,
								 epofiHotelRoomNumber,
                                 epofiPrintCombinedTaxTotal,
                                 epofiPrintServiceCharge,
								 epofiPrintLoyaltyReceiptHeader,
                                 epofiPrintLoyaltyReceiptMemberNumber,
                                 epofiPrintLoyaltyReceiptMemberName,
                                 epofiPrintLoyaltyReceiptActivationCode,
                                 epofiPrintLoyaltyReceiptActivationInstructions,
                                 epofiSessionDateInfo,
                                 epofiPrintSalesTaxExempt,
                                 epofiPrintSalesTaxable,
                                 epofiPrintSalesTax,
                                 epofiPrintLocalTax,
                                 epofiPrintServiceChargeTax,
                                 epofiPrintZeroRated,
                                 epofiPrintSaleTypeDetail,
                                 epofiPrintServiceChargePlusServiceChargeTax,
                                 epofiPrintItemScales,
                                 epofiPrintCurrencySymbol,
                                 epofiPrintTaxInclServiceChargeTaxTotal,
                                 epofiPickUpPrintCustomerName,
                                 epofiPickUpPrintCustomerPhone,
                                 epofiPrintRedeemableWeight,
                                 epofiPrintTwoLinesItemsName,
                                 epofiPrintTwoLinesItemName,
                                 epofiPrintItemTwoLinesTotal,
                                 epofiPrintItemsTwoLinesTotal,
                                 epofiPrintDeliveryTime,
                                 epofiPrintProfitTax,
                                 epofiGroupItems,
                                 epofiCurrentYearPts,
                                 epofiPrintReceiptVoidFooter,
                                 epofiPrintBIRSalesTax,
                                 epofiPrintPOSPlusSerialNumber,
                                 epofiPrintOrganizationNumber,
                                 epofiPrintOracleCheckNumber,
                                 epofiPrintSignatureSection,
                                 epofiPrintEFTPOSReceipt,
								 };

typedef std::pair<ePrintOutFormatInstructions,UnicodeString> InstructionPair;
typedef std::map<int,InstructionPair> typeInstructions;
#endif