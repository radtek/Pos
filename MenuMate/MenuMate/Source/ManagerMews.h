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
       bool SetUpMews(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken);
       void LogPMSEnabling(TriggerLocation triggerType);
       void UnsetPostingFlag();
       std::vector<TOutlet> GetOutletsFromDB();
       std::vector<TServiceMews> GetServicesFromDB();
       bool GetSpaces(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken);
       std::vector<TAccountingCategory> GetCategoriesFromDB();
    private :
       bool RoomChargePost(TPaymentTransaction &_paymentTransaction);
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       bool SendRoomChargePost(TPaymentTransaction &PaymentTransaction);
       bool CheckIPAddressPort(AnsiString tcpIPAddress,int tcpPort);
       bool GetRoundingandDefaultAccount();
       bool RetryDefaultRoomPost(TPaymentTransaction &_paymentTransaction, TRoomCharge roomCharge);
       AnsiString GetLogFileName();
       bool ExportData(TPaymentTransaction &paymentTransaction);
       void WaitOrProceedWithPost();
       void SetPostingFlag();
       void LogWaitStatus(std::auto_ptr<TStringList> waitLogs);
       bool GetOutlets(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken);
       bool GetServices(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken);
       bool GetCategories(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken);
       void GetMewsCustomerBySpace(UnicodeString queryString, std::vector<TCustomerMews> &customerMews);
       void GetMewsCustomerByName(UnicodeString queryString, std::vector<TCustomerMews> &customerMews);
       void UpdateMewsLogs(bool status);
//       bool CategoriesAreSetUp();
};
#endif
