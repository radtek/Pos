//---------------------------------------------------------------------------

#ifndef SCDPatronUtilityH
#define SCDPatronUtilityH
#include "PaymentTransaction.h"
//---------------------------------------------------------------------------
class TSCDPatronUtility
{
    public:
    TSCDPatronUtility();
    ~TSCDPatronUtility();
    bool RestructureBill(TPaymentTransaction &paymentTransaction);
    bool CheckDiscountsAvailable(TPaymentTransaction paymentTransaction);
    void GetPatronDistribution(std::vector<TPatronType> patrons, double &nonSCD, double &SCD);
    bool CheckPatronsAvailable(TPaymentTransaction paymentTransaction);
    void DivideBill(TPaymentTransaction &paymentTransaction,std::auto_ptr <TList> &SCDOrders,std::auto_ptr <TList> &NormalOrders);
    bool CanByPassSCDValidity(TPaymentTransaction paymentTransaction);
    bool CanByPassSCDValidity(TPaymentTransaction paymentTransaction, TDiscount CurrentDiscount);
    bool IsCounterDiscountAvailable(TPaymentTransaction paymentTransaction, TDiscount CurrentDiscount);

    private:
    bool CheckBillRestructureRequiredAfterPatron(TPaymentTransaction paymentTransaction);
    Currency GetAmountForSCD(TPaymentTransaction paymentTransaction);
    void MergeItems(TPaymentTransaction &paymentTransaction);
    void AddIfPossible(TItemComplete *oldOrder,TPaymentTransaction &paymentTransaction,std::vector<int> &indexToBeExcluded,int oldIndex);
    bool isSameItemFromInitial(TItemComplete *oldOrder,TItemComplete *newOrder);
    void ApplyDiscountsOnWholeBill(TPaymentTransaction &paymentTransaction);
};
#endif
