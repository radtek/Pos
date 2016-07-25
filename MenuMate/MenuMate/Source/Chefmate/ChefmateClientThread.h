//---------------------------------------------------------------------------

#ifndef ChefmateClientThreadH
#define ChefmateClientThreadH
//---------------------------------------------------------------------------

#include <Classes.hpp>

#include "PaymentTransaction.h"
#include "ChitNumber.h"
#include "ChefmateClient.h"

//---------------------------------------------------------------------------

enum TExecuteType { etSendCompleteOrder, etSendIncompleteOrder, etSendCreditOrder, etSendCancelOrder, etSendTransferOrder, etSendCallAwayOrder, etSendReplacementOrder };

class TChefmateClient;

class TChefmateClientThread
{
private:
protected:
    TExecuteType executeType;

    void sendCompleteOrderWithTransaction( TPaymentTransaction* inTransaction );
     void sendCreditOrder(TList* creditOrderList);
    void sendCancelOrder(TList* cancelOrderList);
    void sendTransferOrder(TList* orderList);
    void sendCallAwayOrder(TCallAwayComplete* callAwayOrder);
    void sendReplacementOrderWithTransaction( TPaymentTransaction* inTransaction );

    void initChefMate( TChefmateClient *inClient, TChitNumber* inChitNumber );
    void doneChefMate( TChefmateClient *inClient );
    void ExecuteProcess();

public:
    TPaymentTransaction* CompleteOrderTransaction;
    TList* CreditOrdersList;
    TList* CancelOrdersList;
    TList* TransferOrdersList;
    TList* OrderList;
    TCallAwayComplete* CallAwayOrder;
    TPaymentTransaction* ReplacementOrderTransaction;
    UnicodeString SourceTableName;
    __fastcall TChefmateClientThread();

    void SendCompleteOrder();
    void SendCreditOrder();
    void SendCancelOrder();
    void SendCallAwayOrder();
    void SendReplacementOrder();
    void SendTransferOrder();
};
//---------------------------------------------------------------------------
#endif

