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
    void MoveOrderToTab(TPaymentTransaction &PaymentTransaction,bool IsTransferFromTable);
    bool IsDelayedPayment(TPaymentTransaction &PaymentTransaction );
};

#endif
