//---------------------------------------------------------------------------

#ifndef DCBillProcessingAtZedH
#define DCBillProcessingAtZedH
#include <vector>
#include <system.hpp>
#include <memory>
#include "DrinkCommandData.h"
#include "SeatOrders.h"
#include "TCPStreamPrepare.h"
//---------------------------------------------------------------------------
//class TSeatOrders;
class TDCBillProcessingAtZed
{
    public:
        TDCBillProcessingAtZed();
       ~TDCBillProcessingAtZed();
//
        void FetchAllDCOpenSessions();
        std::vector<TOpenSessionDetails> CollectedOpenSessionDetails;                       //DC
        void OnItemDetailsReceivedZED(TSystemEvents *Sender);
        void OnOrderComplete(TSystemEvents *Sender);
        void MakeItem(long itemKey, long sizesKey , AnsiString qtyValue , AnsiString billValue);
        bool ProcessOrders();
        void ApplyMembership();
        void PrepareOrderList();
        void ProceedForPayment();
        void TotalCosts(TPaymentTransaction PaymentTransaction);

    private:
        bool dc_item;
        UnicodeString SelectedOrderMenuName;
        int SelectedSeat;
        int SetMenuGroupNo;
        TPaymentTransaction PaymentTransaction(DBTransaction);
        int SelectedTable;
        AnsiString SelectedTabContainerName;
        TContactMemberApplied Membership;
        AnsiString TabContainerName;
        AnsiString card_no;
        AnsiString CardIdStored;
        TSaleType TypeOfSale;
        TList *OrdersList;
        std::vector<TItemComplete> vecTItemComplete;
        long SizesKey;
        std::auto_ptr<TList>OrdersList1;
        TMoney InitialMoney;
//        TItemComplete *SetMenuDish;
};
#endif
