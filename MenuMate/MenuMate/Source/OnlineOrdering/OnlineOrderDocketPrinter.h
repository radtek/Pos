//---------------------------------------------------------------------------

#ifndef OnlineOrderDocketPrinterH
#define OnlineOrderDocketPrinterH
#include <Classes.hpp>
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------
class TOnlineDocketPrinterThread : public TThread
{
    private:
        //TTimer* panasonicThreadTimer;
        void PrepareDataAndPrintDocket(Database::TDBTransaction &dbTransaction );
        void ProcessSecurity(TPaymentTransaction &PaymentTransaction);
        void PrintKitchenDockets(TPaymentTransaction &PaymentTransaction, int WebKey, UnicodeString JobName,UnicodeString DeviceName);
        void SendOnlineOrderToChefmate(TPaymentTransaction* inTransaction);
        void ProcessHappyHour(TPaymentTransaction &paymentTransaction, UnicodeString orderUniqueId);
        void PrintKitchenDocketsForWaiterApp(TPaymentTransaction &PaymentTransaction, int WebKey, UnicodeString JobName,UnicodeString DeviceName);
        TList* GetOrderItemList(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID);
        TItemComplete* LoadItemCompleteForWaiterAppOrder(Database::TDBTransaction &DBTransaction, TItem* Item, TItemSize* itemSize,
                            TItemComplete *itemComplete, UnicodeString orderGUID, int itemSizeIdentifier);
        void ManagePrintOperationsForWaiterApp(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID, bool IsDayArcBill);
        void PrintAndArchiveReceiptForWaiterAppOrders(TPaymentTransaction &PaymentTransaction, UnicodeString orderGUID);
        void MergePOSAndEFTPOSReceipt(std::auto_ptr<TStringList> &eftPosReceipt,TReqPrintJob *LastReceipt);
        void AddPaymentInfo(TPaymentTransaction &PaymentTransaction, UnicodeString orderGUID);
        void ManagePostingToMews(TPaymentTransaction &PaymentTransaction, UnicodeString orderGUID);
    protected:
        virtual void __fastcall Execute();
    public:
        void SendDataToPanasonicServer();
        // Constructor
        TOnlineDocketPrinterThread();
};

class TOnlineOrderDocketPrinter
{
    private:
        static TOnlineOrderDocketPrinter* instance;
        TOnlineDocketPrinterThread* docketPrinterThread;
        TTimer* docketPrinterThreadTimer;
        bool docketPrinterThreadTerminated;

        // Constructor and Destructor
        TOnlineOrderDocketPrinter();
        ~TOnlineOrderDocketPrinter();

        void __fastcall DocketPrinterTheadTerminate( TObject* sender );
        void InitiateDocketPrinterThread();
        void StartDocketPrinterThread();
        void StartDocketPrinterThreadTimer();
        void StopDocketPrinterThreadTimer();
        void __fastcall OnDocketPrinterThreadTimerTick(TObject *Sender);
        bool IsDocketPrintingPending();

    public:

        static TOnlineOrderDocketPrinter* Instance()
        {
            if(instance == NULL)
                instance = new TOnlineOrderDocketPrinter();

            return instance;
        }

        void TriggerTransactionSync();
};
#endif
