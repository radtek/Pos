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
	public :
	TPhoenixHM();
    ~TPhoenixHM();

   	std::auto_ptr<TPhoenixNetTCPManager>	fPhoenixNet;
    std::set<AnsiString> CodesTestedOk;

	void CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
	void ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
	void __fastcall SetRoundingCategory(AnsiString value);
	void GetRoomStatus(TPhoenixRoomStatusExt &RoomStatus,AnsiString PMSIPAddress,int PMSPort);
   	void Initialise();
	void ClearCodesTestedOk();
    void FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode);

	AnsiString LastTransactionRef;
	AnsiString GetRefNumber();
   	AnsiString GetLastTransactionRef();
	AnsiString __fastcall GetRoundingCategory();

	bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
	bool TestCode(AnsiString CodeToTest);
	bool TestDefaultCodes();
};
#endif

