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
       void LogPMSEnabling(TriggerLocation triggerType);
       AnsiString GetFileName();
       bool EnableOraclePMSSilently();
       bool FindAndTerminateProcess();
    private:
       bool GetLinkStatus();
       bool InitializeoracleTCP();
       bool LoadMealTimings(Database::TDBTransaction &DBTransaction);
       bool TriggerApplication();
       void CloseExistingApplication();
       void MakeOracleSeedFile();
};
#endif
