//---------------------------------------------------------------------------

#ifndef SiHotDataProcessorH
#define SiHotDataProcessorH
#include "PaymentTransaction.h"
#include "DeviceRealTerminal.h"
#include "DBOrder.h"
#include "SiHotDataObjects.h"
//---------------------------------------------------------------------------

class TSiHotDataProcessor
{
    public:
       TRoomRequest CreateRoomRequest(std::vector<TSiHotAccounts> &siHotAccounts ,AnsiString pmsIPAddress, int pmsPort);
       void CreateRoomChargePost(TPaymentTransaction _paymentTransaction, TRoomCharge &_roomCharge);
       void PrepareRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,TRoomResponse);
       int GetTransNumber();
    private:
       UnicodeString GetInvoiceNumber(TPaymentTransaction _paymentTransaction);
       double GetVATpercentage(TItemComplete *itemComplete);
       UnicodeString GetGLCode(int key);
       void AddSurchargeAndTip( TRoomCharge _roomCharge, double surcharge, TPaymentTransaction _paymentTransaction, UnicodeString _billNo);
       void AddDiscountPartToService(TItemComplete *itemComplete,TRoomCharge &_roomCharge, TPaymentTransaction _paymentTransaction,UnicodeString _billNo);
       void AddExpensesToSiHotService(TPayment* payment, TRoomCharge &_roomCharge, UnicodeString billno);
};
#endif
