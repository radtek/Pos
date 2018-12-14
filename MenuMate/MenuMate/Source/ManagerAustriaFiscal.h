//---------------------------------------------------------------------------

#ifndef ManagerAustriaFiscalH
#define ManagerAustriaFiscalH
#include "AustriaFiscalDataObjects.h"
#include "PaymentTransaction.h"
//---------------------------------------------------------------------------
class TManagerAustriaFiscal
{
    public:
        TManagerAustriaFiscal();
        ~TManagerAustriaFiscal();
        bool GetEchoResponseFromConfig();
        bool GetEchoResponseFromMain();
        bool IsZeroReceiptSuccessful();
        bool CommissionAustriaFiscal();
        bool PostDataToAustriaFiscal();
        bool ExportData(TPaymentTransaction &paymentTransaction);
        void UnsetPostingFlag();
    private:
        bool WasSignatureReceivedInLastSale();
        void LogFiscalAustriaEnabling(std::auto_ptr<TStringList> list,bool isEnabled);
        bool IsEchoSuccessful();
        AnsiString GetLogFileName();
        TReceiptRequestAustriaFiscal GetAustriaReceiptDetails(TPaymentTransaction &paymentTransaction);
        UnicodeString GetInvoiceNumber(TPaymentTransaction paymentTransaction);
        std::vector<TChargeItemAustriaFiscal> GetChargeItemsAustria(TPaymentTransaction paymentTransaction);
        std::vector<TPayItemAustriaFiscal> GetPayItemsAustria(TPaymentTransaction paymentTransaction);
        TChargeItemAustriaFiscal GetBillItemAustria(TItemComplete* itemComplete, TPaymentTransaction paymentTransaction,bool &addDiscountItem);
        TChargeItemAustriaFiscal GetBillItemDiscountAustria(TItemComplete* itemComplete, TPaymentTransaction paymentTransaction);
        TChargeItemAustriaFiscal GetBillItemServiceCharge(TItemComplete* itemComplete, bool isRefund);
        double GetVatPercentage(TItemComplete * itemComplete);
        double GetItemAmount(TItemComplete * itemComplete);
        void WaitOrProceedWithPost();
        void LogWaitStatus(std::auto_ptr<TStringList> waitLogs);
        void StoreDataInDB(TReceiptResponseAustriaFiscal response, UnicodeString invoiceNumber,Database::TDBTransaction &DBTransaction);
        void StoreInvoiceDetailsinDB(TReceiptRequestAustriaFiscal receiptAustriaFiscal,Database::TDBTransaction &DBTransaction);
        void TakeCorrectiveMeasures(Database::TDBTransaction &DBTransaction);
        void SendOldInvoices(std::vector<TReceiptRequestAustriaFiscal> &receiptsPending,Database::TDBTransaction &DBTransaction);
        void GetOldInvoices(std::vector<TReceiptRequestAustriaFiscal> &receiptsPending,Database::TDBTransaction &DBTransaction);
        void GetInvoiceDetails(std::vector<TReceiptRequestAustriaFiscal> &receiptsPending,Database::TDBTransaction &DBTransaction);
        void UpdateInvoiceDetails(Database::TDBTransaction &DBTransaction,TReceiptRequestAustriaFiscal receipt,TReceiptResponseAustriaFiscal response);
        TDateTime GetMomentForReceipt(UnicodeString invoiceNumber,Database::TDBTransaction &DBTransaction);
};
#endif
