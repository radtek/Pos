//---------------------------------------------------------------------------

#ifndef ChefmateClientManagerH
#define ChefmateClientManagerH

//---------------------------------------------------------------------------

#include "ChefmateClient.h"
#include "PaymentTransaction.h"
#include "ChitNumber.h"


class TChefmateClientManager
{
    public:
         TChefmateClientManager();
	    ~TChefmateClientManager();

        bool ChefMateEnabled();

        CMC_ERROR SendCompleteOrder( TPaymentTransaction* inTransaction );
        CMC_ERROR SendTransferOrder( TList* orderList,UnicodeString inSourceTableName );
        CMC_ERROR SendCreditOrder( TContainerOrders* inOrders );
        CMC_ERROR SendCancelOrder( TList* inElementsToCancel , AnsiString inTabTableName);
        CMC_ERROR SendCallAwayOrder( TCallAwayComplete* inCallAway );
        CMC_ERROR SendReplacementOrder( TPaymentTransaction* inTransaction );
        CMC_ERROR SendWebOrder( TPaymentTransaction* inTransaction , UnicodeString paymentStaus, TMMContactInfo customerInfo);

    private:
       bool TChefmateClientManager::chefMateEnabled();
       void initChefMate( TChefmateClient *inClient, TChitNumber* inChitNumber );
       void doneChefMate( TChefmateClient *inClient );
};

//---------------------------------------------------------------------------

#endif
