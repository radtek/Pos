//---------------------------------------------------------------------------

#ifndef PhoenixHotelSystemH
#define PhoenixHotelSystemH
//---------------------------------------------------------------------------
#include "ManagerPMS.h"
#include "Money.h"
#include "PHSTCPIP.h"
//--------------------------------------------------------------------------
class TPhoenixHM : public TBasePMS
{
	public:
	void CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
	void ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);

	AnsiString LastTransactionRef;
	AnsiString GetRefNumber();
	void __fastcall SetRoundingCategory(AnsiString value);
	AnsiString __fastcall GetRoundingCategory();
    std::set<AnsiString> CodesTestedOk;
	public :
	TPhoenixHM();
    ~TPhoenixHM();
	void GetRoomStatus(TPhoenixRoomStatusExt &RoomStatus,AnsiString PMSIPAddress,int PMSPort);
	bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
   	AnsiString GetLastTransactionRef();
   	void Initialise();
   	std::auto_ptr<TPhoenixNetTCPManager>	fPhoenixNet;
	bool TestCode(AnsiString CodeToTest);
	void ClearCodesTestedOk();
	bool TestDefaultCodes();
    void FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode);

};
#endif

