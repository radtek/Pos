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
       double QtyServiceCharge;
       UnicodeString GetInvoiceNumber(TPaymentTransaction _paymentTransaction);
       double GetVATpercentage(TItemComplete *itemComplete);
       void AddSurchargeAndTip( TRoomCharge &_roomCharge, double surcharge, TPaymentTransaction _paymentTransaction, UnicodeString _billNo);
       void AddDiscountPartToService(TItemComplete *itemComplete,TRoomCharge &_roomCharge, TPaymentTransaction _paymentTransaction,UnicodeString _billNo);
       void AddExpensesToSiHotService(TPayment* payment, TRoomCharge &_roomCharge, UnicodeString billno);
       std::map<UnicodeString,std::map<double,TSiHotService> > servicesSiHot;
       std::map<UnicodeString,TSiHotService> serviceSiHotDiscPart;
       std::map<UnicodeString,TSiHotPayments> paymentSiHot;
       double GetPriceTotal(TItemComplete* itemComplete ,bool recalculateTax);
       bool AddItemToSiHotService(TItemComplete *itemComplete,UnicodeString billNo,UnicodeString categoryCode);
       void AddServiceChargeAsService(TRoomCharge &_roomcharge, UnicodeString billNo, TPaymentTransaction _paymentTransaction);
       void AddRoundingAsService(TRoomCharge &_roomcharge, UnicodeString billNo, TPaymentTransaction _paymentTransaction);
       void AddPaymentMethods(TRoomCharge &_roomcharge, UnicodeString billNo, TPaymentTransaction _paymentTransaction);
};
#endif
