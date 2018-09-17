//---------------------------------------------------------------------------

#ifndef ManagerMewsH
#define ManagerMewsH
#include "PaymentTransaction.h"
#include "DeviceRealterminal.h"
#include "MewsDataObjects.h"
//---------------------------------------------------------------------------
class TManagerMews : public TBasePMS
{
	public :
       TManagerMews();
       ~TManagerMews();
//       void GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString PMSIPAddress,int PMSPort);
       void Initialise();
       bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
       bool SetUpMews(UnicodeString url, UnicodeString accessToke, UnicodeString clientToken);
       void LogPMSEnabling(TriggerLocation triggerType);
       void UnsetPostingFlag();
       std::vector<TOutlet> GetOutletsFromDB();
       std::vector<TServiceMews> GetServicesFromDB();
    private :
       bool RoomChargePost(TPaymentTransaction &_paymentTransaction);
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       bool SendRoomChargePost(TPaymentTransaction &PaymentTransaction);
       bool CheckIPAddressPort(AnsiString tcpIPAddress,int tcpPort);
       bool GetRoundingandDefaultAccount();
       bool RetryDefaultRoomPost(TPaymentTransaction &_paymentTransaction, TRoomCharge roomCharge);
       AnsiString GetLogFileName();
       void UpdateMewsLogs(bool status);
       bool ExportData(TPaymentTransaction &paymentTransaction);
       void WaitOrProceedWithPost();
       void SetPostingFlag();
       void LogWaitStatus(std::auto_ptr<TStringList> waitLogs);
       bool GetOutlets(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken);
       bool GetServices(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken);
       bool GetSpaces(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken);
       bool GetCategories(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken);
       void GetMewsCustomerBySpace(UnicodeString queryString, std::vector<TCustomerMews> &customerMews);
       void GetMewsCustomerByName(UnicodeString queryString, std::vector<TCustomerMews> &customerMews);
};
#endif
