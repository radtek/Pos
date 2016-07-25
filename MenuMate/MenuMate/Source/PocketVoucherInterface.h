#ifndef PocketVoucherInterfaceH
#define PocketVoucherInterfaceH
#include "PocketVoucherWSDL.h"

//---------------------------------------------------------------------------
struct PocketVoucherDetails
{
    TDateTime Date_Updated;
    UnicodeString MerchantId;
    UnicodeString Password;
    bool PocketVoucherField;
    UnicodeString Url;
    UnicodeString Username;
};
struct Request
{
    UnicodeString voucherCode;
    UnicodeString voucherAmount;
    UnicodeString transactionAmount;
    UnicodeString merchantId;
    UnicodeString merchantReference;
    UnicodeString terminalId;
    UnicodeString User;
    UnicodeString Password;
};
struct RedemptionResponse
{
    UnicodeString status;
    UnicodeString responseCode;
    UnicodeString responseMessage;
};
class TPocketVoucherInterface
{
	private:
    _di_IPocketVoucherWebService pVClient;

   public :
     void EnablePocketVoucher(UnicodeString accName);
     PocketVoucherDetails GetPocketVoucherDetail(UnicodeString accName);
     TPocketVoucherInterface();
     void initPVClient();
     void DisablePocketVoucher(UnicodeString accName);
     RedemptionResponse PrepareRedeemRequest(Request request);
};
#endif
