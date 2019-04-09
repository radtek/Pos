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
       void Initialise();
       bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
       void LogPMSEnabling(TriggerLocation triggerType);
       void UnsetPostingFlag();
       bool SetUpMews(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction);
       bool GetSpaces(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction);
       void GetMewsCustomer(UnicodeString queryString, std::vector<TCustomerMews> &customerMews,bool isSpace);
    private :
       AnsiString GetLogFileName();
       void WaitOrProceedWithPost();
       void SetPostingFlag();
       void LogWaitStatus(std::auto_ptr<TStringList> waitLogs);
       bool GetCategories(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction);
       bool GetServices(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction);
       bool GetOutlets(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction);
       void GetDetailsForMewsOrderBill(TPaymentTransaction &paymentTransaction, double portion, int paymentIndex,
                                            double tipPortion,TOrder &mewsOrder,bool isBill);
       TUnitCost GetUnitCost(TItemComplete* itemComplete, double portion,bool &seperateDiscount,double &value);
       UnicodeString GetMewsCategoryCodeForItem(TItemComplete *itemComplete,bool isDefaultRequired,UnicodeString name = "");
       UnicodeString GetInvoiceNumber(TPaymentTransaction _paymentTransaction);
       void CalculateQtyAndvariance(double &qtyItemD, int &qtyItem, double &varianceAdditive);
       void UpdateMewsLogs(bool status);
       UnicodeString CheckMewsCategoryExists(UnicodeString category);
       void AddPaymentToPMSPaymentTypes(TPayment *payment,AnsiString defaultCode);
       int GetCategoryKeyFromItemValues(TItemComplete *itemComplete,Database::TDBTransaction &DBTransaction);
};
#endif
