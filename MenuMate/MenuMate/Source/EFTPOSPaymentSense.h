//---------------------------------------------------------------------------

#ifndef EFTPOSPaymentSenseH
#define EFTPOSPaymentSenseH
#include "Eftpos.h"
#include "DropDown.h"
#include "DropDownVar.h"
#include "PaymentSenseWSDL.h"
//---------------------------------------------------------------------------
class TEftPosPaymentSense : public TEftPos
{
 private:
        _di_IWCFServicePaymentSense paymentSenseClient;
        AuthorizationDetails *authorizationDetails;
        void InitPaymentSenseClient();
        void InitializeProperties();
        bool PingTerminal(eEFTTransactionType TxnType);
        TransactionDataResponse* DoTransaction(Currency amtPurchase, UnicodeString transactionType);
        bool GetResponseStatus(eEFTTransactionType TxnType, TransactionDataResponse* response);
        void LoadEftPosReceipt(ReceiptLines* receiptLines);
        void AddNewLine(AnsiString data);
        void PrintReports(UnicodeString reportType);
        void ShowPreviousZED();
        void SaveReportToDataBase(ReportResponseData* report, TMemoryStream *stream);
        bool IsTransfactionFinished(TransactionDataResponse* response );
        UnicodeString lastNotification;
        TransactionDataResponse*  WaitAndGetResponse(TransactionDataResponse *response);
public:
        TEftPosPaymentSense();
        ~TEftPosPaymentSense();
        void Initialise();
        void DoControlPannel();

        void ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,
                            Currency AmtCash, UnicodeString TxnRef,
                            ePANSource PANSource, UnicodeString CardSwipe,
                            int ExpiryMonth, int ExpiryYear);
        void ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
                            AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef = "");

        bool CheckChequeBranch(AnsiString Branch);
        bool CheckChequeSerial(AnsiString ChequeSerial);
        bool CheckChequeAccount(AnsiString ChequeAccount);

        void CancelTransaction(AnsiString inID){};
        void __fastcall DoSettlementCutover();
        void __fastcall DoSettlementEnquiry();
        void __fastcall DoLogon();
        void __fastcall ReprintReceipt();
        void __fastcall PrintZedReport();
        void __fastcall ReprintZedReport();
        AnsiString GetRefNumber();
        std::vector<AnsiString> GetAllTerminals();
        bool IsCashOutSupported();
};
#endif
