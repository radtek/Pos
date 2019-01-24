//---------------------------------------------------------------------------

#ifndef AustriaFiscalInterfaceH
#define AustriaFiscalInterfaceH
#include "AustriaFiscalDataObjects.h"
#include "AustriaFiscalWSDL.h"
//---------------------------------------------------------------------------
class TAustriaFiscalInterface
{
    public:
        TAustriaFiscalInterface();
        TReceiptResponseAustriaFiscal PostDataToAustriaFiscal(TReceiptRequestAustriaFiscal receiptRequest,
                                                                UnicodeString url, UnicodeString cashBoxId,
                                                                UnicodeString accessToken);
        UnicodeString InitAustriaFiscal(UnicodeString url, UnicodeString cashBoxId,UnicodeString accessToken);
        bool CommissionAustriaFiscal(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId,UnicodeString accessToken);
        bool SendZeroReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId,UnicodeString accessToken);
        bool SendMonthlyReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId,UnicodeString accessToken);
        bool SendAnnualReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId,UnicodeString accessToken);
    private:
        void initAustriaClient();
        ReceiptRequestLocal* ApplyAdapterToRequest(TReceiptRequestAustriaFiscal receiptRequest);
        TReceiptResponseAustriaFiscal AppyAdapterToResponse(ReceiptReponseLocal* responseLocal);
        _di_IAustriaFiscalIntegrationWebService AustriaClient;
};
#endif
