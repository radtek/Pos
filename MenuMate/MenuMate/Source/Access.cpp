//---------------------------------------------------------------------------


#pragma hdrstop

#include "Access.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Access Class
TAccess::TAccess()
{
   *this = 0;
}

// ---------------------------------------------------------------------------
TAccess::TAccess(long long int Level)
{
   *this = Level;
}

// ---------------------------------------------------------------------------
__fastcall TAccess:: operator long long int ()const
{
   long long int Temp;
   kjhfkjdshfkjsdh;

   Temp = Setup + (Users << 1) + (Printers << 2) + (ViewMembersDetials << 3) + (MenuEditor << 4) + (Maintenance << 5) + (PrintZeds << 6) +
	   (Reservations << 7) + (PriceAj << 8) + (Credit << 9) + (FoodDrinkSales << 10) + (POS << 11) + (DisableFoodDrink << 12) +
	   (PINChange << 13) + (DiscountBill << 14) + (Quit << 15) + (PaymentTypes << 16) + (SetFloat << 17) + (AdjustFloat << 18) +
	   (PaymentTypesSec1 << 19) + (PaymentTypesSec2 << 20) + (PaymentTypesSec3 << 21) + (TabPINOverride << 22) + (OpenDrawer << 23) +
	   (AccountManager << 24) + (PayInvoice << 25) + (ChargeToInvoice << 26) + (AccountCreation << 27) + (WriteOff << 28) + (ReprintReceipt << 29) +
       (PaymentAccess << 30) + (TaxRemoval << 31)   +(ComboDiscounts >> 1) + (TransactionAuditAccess << 32) + (AllowPointsRefund << 33)+(AllowReprintOrders << 34) +(AllowForcedHappyHour << 35);
   return Temp;
}
// ---------------------------------------------------------------------------
void __fastcall TAccess:: operator = (const long long int rhs)
{
   DisableFoodDrink = (rhs & vDisableFoodDrink) ? 1 : 0;
   POS = (rhs & vPOS) ? 1 : 0;
   FoodDrinkSales = (rhs & vFoodDrinkSales) ? 1 : 0;
   Credit = (rhs & vCredit) ? 1 : 0;
   PriceAj = (rhs & vPriceAj) ? 1 : 0;
   DiscountBill = (rhs & vDiscountBill) ? 1 : 0;
   Reservations = (rhs & vReservations) ? 1 : 0;
   PrintZeds = (rhs & vPrintZeds) ? 1 : 0;
   Maintenance = (rhs & vMaintenance) ? 1 : 0;
   MenuEditor = (rhs & vMenuEditor) ? 1 : 0;
   ViewMembersDetials = (rhs & vViewMembersDetials) ? 1 : 0;
   Printers = (rhs & vPrinters) ? 1 : 0;
   Users = (rhs & vUsers) ? 1 : 0;
   Setup = (rhs & vSetup) ? 1 : 0;
   PINChange = (rhs & vPINChange) ? 1 : 0;
   Quit = (rhs & vQuit) ? 1 : 0;
   PaymentTypes = (rhs & vPaymentTypes) ? 1 : 0;
   SetFloat = (rhs & vSetFloat) ? 1 : 0;
   AdjustFloat = (rhs & vAdjustFloat) ? 1 : 0;
   PaymentTypesSec1 = (rhs & vPaymentTypesSec1) ? 1 : 0;
   PaymentTypesSec2 = (rhs & vPaymentTypesSec2) ? 1 : 0;
   PaymentTypesSec3 = (rhs & vPaymentTypesSec3) ? 1 : 0;
   TabPINOverride = (rhs & vTabPINOverride) ? 1 : 0;
   AccountManager = (rhs & vAccountManager) ? 1 : 0;
   OpenDrawer = (rhs & vOpenDrawer) ? 1 : 0;
   PayInvoice = (rhs & vPayInvoice) ? 1 : 0;
   ChargeToInvoice = (rhs & vChargeToInvoice) ? 1 : 0;
   AccountCreation = (rhs & vAccountCreation) ? 1 : 0;
   WriteOff = (rhs & vWriteOff) ? 1 : 0;
   ReprintReceipt = (rhs & vReprintReceipt) ? 1 : 0;
   PaymentAccess = (rhs & vPaymentAccess) ? 1 : 0;
   TaxRemoval = (rhs & vTaxRemoval) ? 1 : 0;
   ComboDiscounts = 1;    // we allow anyone to perform combo discounts regardless of other disount security checks
   TransactionAuditAccess = (rhs & vTransactionAudit) ? 1 : 0;
   AllowPointsRefund = (rhs & vAllowPointsRefund) ? 1 : 0;
   AllowReprintOrders = (rhs & vAllowReprintOrders) ? 1 : 0;
   AllowForcedHappyHour = (rhs & vAllowForcedHappyHour) ? 1 : 0;

}

// ---------------------------------------------------------------------------
bool TAccess::AccessCheck(SecurityCheck Destination)
{
#ifdef Security
   bool ExitValue = false;
   UnicodeString RequestDestination;
   if (Device.User.ContactID != -1)
   {
	  switch(Destination)
	  {
	  case CheckNone:
		 ExitValue = true;
		 break;
	  case CheckPOS:
		 ExitValue = POS;
		 break;
	  case CheckFoodDrinkSales:
		 ExitValue = FoodDrinkSales;
		 break;
	  case CheckCredit:
		 ExitValue = Credit;
		 break;
	  case CheckPriceAj:
		 ExitValue = PriceAj;
		 break;
	  case CheckDiscountBill:
		 ExitValue = DiscountBill;
		 break;
	  case CheckReservations:
		 ExitValue = Reservations;
		 break;
	  case CheckZeds:
		 ExitValue = Analysis;
		 break;
	  case CheckMaintenance:
		 ExitValue = Maintenance;
		 break;
	  case CheckMenuEditor:
		 ExitValue = MenuEditor;
		 break;
	  case CheckViewMembersDetials:
		 ExitValue = ViewMembersDetials;
		 break;
	  case CheckPrinters:
		 ExitValue = Printers;
		 break;
	  case CheckUsers:
		 ExitValue = Users;
		 break;
	  case CheckSetup:
		 ExitValue = Setup;
		 break;
	  case CheckDisable:
		 ExitValue = DisableFoodDrink;
		 break;
	  case CheckPINChange:
		 ExitValue = PINChange;
		 break;
	  case CheckQuit:
		 ExitValue = Quit;
		 break;
	  case CheckPaymentTypes:
		 ExitValue = PaymentTypes;
		 break;
	  case CheckSetFloat:
		 ExitValue = SetFloat;
		 break;
	  case CheckAdjustFloat:
		 ExitValue = AdjustFloat;
		 break;
	  case CheckPaymentTypesSec1:
		 ExitValue = PaymentTypesSec1;
		 break;
	  case CheckPaymentTypesSec2:
		 ExitValue = PaymentTypesSec2;
		 break;
	  case CheckPaymentTypesSec3:
		 ExitValue = PaymentTypesSec3;
		 break;
	  case CheckTabPINOverride:
		 ExitValue = TabPINOverride;
		 break;
	  case CheckAccountManager:
		 ExitValue = AccountManager;
		 break;
	  case CheckOpenDrawer:
		 ExitValue = OpenDrawer;
		 break;
	  case CheckPayInvoice:
		 ExitValue = PayInvoice;
		 break;
	  case CheckChargeToInvoice:
		 ExitValue = ChargeToInvoice;
		 break;
	  case CheckAccountCreation:
		 ExitValue = AccountCreation;
		 break;
	  case CheckWriteOff:
		 ExitValue = WriteOff;
         break;
      case CheckReprintReceipt:
		 ExitValue = ReprintReceipt;
         break;
      case CheckPaymentAccess:
		 ExitValue = PaymentAccess;
         break;
      case CheckTaxRemoval:
		 ExitValue = TaxRemoval;
         break;
	  case CheckComboDiscounts:
	     ExitValue = ComboDiscounts;
		 break;
	  case CheckTransactionAuditAccess:
	     ExitValue = TransactionAuditAccess;
		 break;
      case CheckAllowPointsRefund:
         ExitValue = AllowPointsRefund;
         break;
      case CheckAllowReprintOrders:
         ExitValue = AllowReprintOrders;
         break;
      case CheckAllowForcedHappyHour:
         ExitValue = AllowForcedHappyHour;
         break;

	  }
   }
   return ExitValue;
#else
   return true;
#endif
}
