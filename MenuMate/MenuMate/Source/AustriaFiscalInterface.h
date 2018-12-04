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
        TReceiptResponseAustriaFiscal PostDataToAustriaFiscal(TReceiptRequestAustriaFiscal receiptRequest);
        UnicodeString InitAustriaFiscal(UnicodeString url, UnicodeString cashBoxId);
        bool CommissionAustriaFiscal(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId);
        bool SendZeroReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId);
        bool SendMonthlyReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId);
        bool SendAnnualReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId);
    private:
        void initAustriaClient();
        ReceiptRequestLocal* ApplyAdapterToRequest(TReceiptRequestAustriaFiscal receiptRequest);
        TReceiptResponseAustriaFiscal AppyAdapterToResponse(ReceiptReponseLocal* responseLocal);
        _di_IAustriaFiscalIntegrationWebService AustriaClient;
};
#endif
