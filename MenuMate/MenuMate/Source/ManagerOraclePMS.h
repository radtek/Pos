//---------------------------------------------------------------------------

#ifndef ManagerOraclePMSH
#define ManagerOraclePMSH
#include "BasePMS.h"
#include "OracleDataBuilder.h"
//---------------------------------------------------------------------------

class TManagerOraclePMS : public TBasePMS
{
	public :
       TManagerOraclePMS();
       ~TManagerOraclePMS();
       void Initialise();
       bool ExportData(TPaymentTransaction &_paymentTransaction,int StaffId);
       void LoadMeals(Database::TDBTransaction &DBTransaction);
       void GetRoomStatus(AnsiString _roomNumber,  TRoomInquiryResult &_roomResult);//std::auto_ptr<TRoomInquiryResult> &roomResult);
    private:
       bool GetLinkStatus();
       bool InitializeoracleTCP();
};
#endif
