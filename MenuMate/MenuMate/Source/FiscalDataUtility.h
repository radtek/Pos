//---------------------------------------------------------------------------

#ifndef FiscalDataUtilityH
#define FiscalDataUtilityH
#include "PaymentTransaction.h"
#include "DeviceRealTerminal.h"
#include "Stream.h"
//---------------------------------------------------------------------------
enum ReturnCodesVersion
{
    OK, WrongLength,CRCError,UnknownCommand
};
enum ReturnCodesPosts
{
    UnknownCommandPost = -3, WrongLengthPost = -2, CRCErrorPost = -1, OKPost = 0, WrongArgsNumber = 1, WrongDateTimeRange = 2,
    WrongOrgNumberFormat = 3, WrongCashRegIDFormat = 4, WrongSerialNumFormat = 5, UndefinedReceiptFormat = 6, UndefinedReceiptFormat2 = 7,
    WrongReturnAmount = 8, WrongSaleAmount = 9, WrongVatFormat = 10, WrongVatFormat2 = 11, WrongVatFormat3 = 12, WrongVatFormat4 = 13,
    InternalError = 15, InternalError2 = 16, InternalError3= 17, InternalError4 = 18, InternalError5 = 19, PowerFail = 20, WrongSalereturnRelation = 21,
    FieldPresentAfterCRC = 22, InternalErrorCounter = 23, InternalLogFull = 24
};
enum CommResult
{
    IsPosted = 0, IsSaved = 1
};
enum PostType
{
    eFiscalVerNumber = 1, eFiscalNormalReceipt, eFiscalCopyReceipt, eFiscalPracticeReceipt, eFiscalProofReceipt
};
class TFiscalTax
{
    public:
        int Type;
        AnsiString Name;
        Currency Percentage;
        double Value;
        ~TFiscalTax();
        TFiscalTax();
};
class TFiscalData
{
    public:
       TFiscalData();
       ~TFiscalData();
       int FiscalDataKey;
       AnsiString FiscalInvoiceNumber;
       AnsiString FiscalDateTime;
       AnsiString FiscalRegisterID;
       AnsiString FiscalSerialNumber;
       AnsiString FiscalReceiptType;
       AnsiString FiscalAmount;
       AnsiString FiscalVatRate1;
       AnsiString FiscalVat1;
       AnsiString FiscalVatRate2;
       AnsiString FiscalVat2;
       AnsiString FiscalVatRate3;
       AnsiString FiscalVat3;
       AnsiString FiscalVatRate4;
       AnsiString FiscalVat4;
       AnsiString FiscalCRCValue;
       AnsiString FIscalResponse;
       AnsiString IsPostedToPosPlus;
};
class TFiscalDataUtility
{
    public:
        AnsiString CreateVersionRequest();
        AnsiString CreatePostRequest(PostType type);
        bool AnalyzeResponse(AnsiString inData, PostType type);
        AnsiString GetPOSPlusData(TPaymentTransaction &paymentTransaction);
        AnsiString GetPOSPlusData(AnsiString invoiceNumber);
    private:
        bool AnalyzePostResponse(AnsiString inData, PostType type);
        AnsiString PrepareDataForPOSPlus(TPaymentTransaction &paymentTransaction);
        AnsiString GetInvoiceNumber(TPaymentTransaction paymentTransaction);
        void ExtractVatDetails(TPaymentTransaction paymentTransaction, std::auto_ptr<TFiscalData> &inFiscalData);
        void ExtractVatDetailsOnItemBasis(TItemComplete *itemComplete);
        std::vector<TFiscalTax> TaxVector;
        AnsiString ConvertDataToString(std::auto_ptr<TFiscalData> &fiscalData, TPaymentTransaction &paymentTransaction);
        int StreamCheckCRC(AnsiString value);
        AnsiString GetBillData(AnsiString invoiceNumber);
        TBytes CreateByteArray( const char* inBuffer, __int32 inBufferSize );
        void GetTaxPercentageFromDB(Database::TDBTransaction &DBTransaction);
        void PopulateTaxDetails(std::auto_ptr<TFiscalData> &inFiscalData);
        AnsiString ConvertDataToString(std::auto_ptr<TFiscalData> fiscalData, bool isRefundReceipt);
};
#endif
