//---------------------------------------------------------------------------

#ifndef AccessH
#define AccessH
//---------------------------------------------------------------------------

// Security Defines.
#define vAllowForcedHappyHour   0x800000000
#define vAllowReprintOrders     0x400000000
#define vAllowPointsRefund      0x200000000
#define vTransactionAudit       0x100000000
#define vTaxRemoval             0x080000000
#define vPaymentAccess          0x040000000
#define vReprintReceipt 	    0x020000000
#define vWriteOff 			    0x010000000
#define vAccountCreation	    0x008000000
#define vChargeToInvoice	    0x004000000
#define vPayInvoice			    0x002000000
#define vAccountManager		    0x001000000
#define vOpenDrawer			    0x000800000
#define vTabPINOverride		    0x000400000
#define vPaymentTypesSec3	    0x000200000
#define vPaymentTypesSec2	    0x000100000
#define vPaymentTypesSec1	    0x000080000
#define vAdjustFloat		    0x000040000
#define vSetFloat			    0x000020000
#define vPaymentTypes		    0x000010000
#define vQuit				    0x000008000
#define vDiscountBill		    0x000004000
#define vPINChange			    0x000002000
#define vDisableFoodDrink	    0x000001000
#define vPOS				    0x000000800
#define vFoodDrinkSales		    0x000000400
#define vCredit				    0x000000200
#define vPriceAj			    0x000000100
#define vReservations		    0x000000080
#define vPrintZeds			    0x000000040
#define vMaintenance		    0x000000020
#define vMenuEditor			    0x000000010
#define vViewMembersDetials	    0x000000008
#define vPrinters				0x000000004
#define vUsers					0x000000002
#define vSetup					0x000000001

enum SecurityCheck {CheckNone,CheckPOS,
                    CheckFoodDrinkSales,
                    CheckCredit,
                    CheckPriceAj,
                    CheckReservations,
                    CheckZeds,
                    CheckMaintenance,
                    CheckMenuEditor,
                    CheckViewMembersDetials,
                    CheckPrinters,
                    CheckUsers,
                    CheckSetup,
                    CheckDisable,
					CheckPINChange,
                    CheckDiscountBill,
                    CheckQuit,
                    CheckPaymentTypes,
					CheckSetFloat,
                    CheckAdjustFloat,
                    CheckPaymentTypesSec1,
					CheckPaymentTypesSec2,
                    CheckPaymentTypesSec3,
                    CheckTabPINOverride,
					CheckOpenDrawer,
                    CheckAccountManager,
                    CheckPayInvoice,
                    CheckChargeToInvoice,
					CheckAccountCreation,
                    CheckWriteOff,
                    CheckComboDiscounts,
                    CheckReprintReceipt,
                    CheckPaymentAccess,
                    CheckTaxRemoval,
                    CheckTransactionAuditAccess,
                    CheckAllowPointsRefund,
                    CheckAllowReprintOrders,
                    CheckAllowForcedHappyHour};


class TAccess // User declarations
{
public:
   __fastcall operator long long()const;
   void __fastcall operator = (long long rhs);
   bool AccessCheck(SecurityCheck Destination);
   TAccess();
   TAccess(long long Level);
   unsigned long long int POS : 1;
   unsigned long long int DisableFoodDrink : 1;
   unsigned long long int FoodDrinkSales : 1;
   unsigned long long int Credit : 1;
   unsigned long long int PriceAj : 1;
   unsigned long long int DiscountBill : 1;
   unsigned long long int Reservations : 1;
   unsigned long long int PrintZeds : 1;
   unsigned long long int Maintenance : 1;
   unsigned long long int MenuEditor : 1;
   unsigned long long int ViewMembersDetials : 1;
   unsigned long long int Printers : 1;
   unsigned long long int Users : 1;
   unsigned long long int Setup : 1;
   unsigned long long int PINChange : 1;
   unsigned long long int Quit : 1;
   unsigned long long int PaymentTypes : 1;
   unsigned long long int SetFloat : 1;
   unsigned long long int AdjustFloat : 1;
   unsigned long long int PaymentTypesSec1 : 1;
   unsigned long long int PaymentTypesSec2 : 1;
   unsigned long long int PaymentTypesSec3 : 1;
   unsigned long long int TabPINOverride : 1;
   unsigned long long int AccountManager : 1;
   unsigned long long int OpenDrawer : 1;
   unsigned long long int PayInvoice : 1;
   unsigned long long int ChargeToInvoice : 1;
   unsigned long long int AccountCreation : 1;
   unsigned long long int WriteOff : 1;
   unsigned long long int ComboDiscounts : 1;
   unsigned long long int TransactionAuditAccess : 1;
   unsigned long long int ReprintReceipt : 1;
   unsigned long long int PaymentAccess : 1;
   unsigned long long int TaxRemoval : 1;
   unsigned long long int AllowPointsRefund : 1;
   unsigned long long int AllowReprintOrders : 1;
   unsigned long long int AllowForcedHappyHour : 1;


};

#endif
