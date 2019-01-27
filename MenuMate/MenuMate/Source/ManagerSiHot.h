//---------------------------------------------------------------------------

#ifndef ManagerSiHotH
#define ManagerSiHotH
#include "PaymentTransaction.h"
#include "DeviceRealterminal.h"
//---------------------------------------------------------------------------

class TManagerSiHot : public TBasePMS
{
	public :
       TManagerSiHot();
       ~TManagerSiHot();
       void GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString PMSIPAddress,int PMSPort);
       void Initialise();
       bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
       bool GetDefaultAccount(UnicodeString processMessage);
       void LogPMSEnabling(TriggerLocation triggerType);
       void UnsetPostingFlag();
       void StoreTicketPost(UnicodeString invoiceNumber, TMemoryStream *receiptData);
    private :
       bool RoomChargePost(TPaymentTransaction &_paymentTransaction);
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       bool SendRoomChargePost(TPaymentTransaction &PaymentTransaction);
       bool CheckIPAddressPort(AnsiString tcpIPAddress,int tcpPort);
       bool GetRoundingandDefaultAccount();
       bool RetryDefaultRoomPost(TPaymentTransaction &_paymentTransaction, TRoomCharge roomCharge);
       AnsiString GetLogFileName();
       void UpdateSiHotLogs(bool status);
       bool ExportData(TPaymentTransaction &paymentTransaction);
       void WaitOrProceedWithPost();
       void SetPostingFlag();
       void LogWaitStatus(std::auto_ptr<TStringList> waitLogs);
       void SavePMSTicketStatus(UnicodeString invoiceNumber, bool response);
       std::vector<UnicodeString> GetFailedPMSTicket();
       void ManageFailedStoreTicketPost(std::vector<UnicodeString> invoiceNumbers);
       void UpdatePMSTicketStatus(UnicodeString invoiceNumber, bool response);
       AnsiString GetReceiptForStoreTicket(UnicodeString invoiceNumber);
       UnicodeString FormatStoreTicket(TMemoryStream *receipt);
       bool CheckSihotPostingValidity(TPaymentTransaction paymentTransaction);
};
#endif
