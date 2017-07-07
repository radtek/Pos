//---------------------------------------------------------------------------

#ifndef ManagerDelayedPaymentH
#define ManagerDelayedPaymentH
//---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include "PaymentTransaction.h"
#include "ConfirmOrd.h"

class TManagerDelayedPayment
{
public :
    static TManagerDelayedPayment& Instance()
        {
            static TManagerDelayedPayment singleton;
            return singleton;
        }

    TManagerDelayedPayment();
    void MoveOrderToTab(Database::TDBTransaction &DBTransaction,TSaveOrdersTo &inOrderContainer);
    void MoveOrderToTab(TPaymentTransaction &PaymentTransaction,bool IsTransferFromTable, bool isMixedMenuOrder = true);
    bool IsDelayedPayment(TPaymentTransaction &PaymentTransaction );
    void SplitDelayedPaymentOrderByMenuType(TList *orderList, TList *foodOrdersList, TList *bevOrdersList);
};

#endif
