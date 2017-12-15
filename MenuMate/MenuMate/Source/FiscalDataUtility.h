//---------------------------------------------------------------------------

#ifndef FiscalDataUtilityH
#define FiscalDataUtilityH
#include "PaymentTransaction.h"
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
    InternalError, InternalError2 = 16, InternalError3= 17, InternalError4 = 18, InternalError5 = 19, PowerFail = 20, WrongSalereturnRelation = 21,
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
class TFiscalDataUtility
{
    public:
        AnsiString CreateVersionRequest();
        AnsiString CreatePostRequest(PostType type);
        bool AnalyzeResponse(AnsiString inData, PostType type);
    private:
        bool AnalyzePostResponse(AnsiString inData, PostType type);
};
#endif
