//---------------------------------------------------------------------------

#ifndef ManagerOraclePMSH
#define ManagerOraclePMSH
#include "PaymentTransaction.h"
#include "BasePMS.h"
//---------------------------------------------------------------------------

class TManagerOraclePMS : public TBasePMS
{
	public :
       TManagerOraclePMS();
       ~TManagerOraclePMS();
       void GetRoomStatus();
       void Initialise();
       bool ExportData(TPaymentTransaction &_paymentTransaction,int StaffId);
       void LoadMeals(Database::TDBTransaction &DBTransaction);
};
#endif
