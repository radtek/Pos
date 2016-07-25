//---------------------------------------------------------------------------

#ifndef PhoenixHotelSystemH
#define PhoenixHotelSystemH
//---------------------------------------------------------------------------
#include "Money.h"
#include "PHSTCPIP.h"

class TPhoenixHM
{
	private:
	int FTCPPort;
	AnsiString FTCPIPAddress;
	int FPOSID;

	void __fastcall SetTCPPort(int value);
	int __fastcall GetTCPPort();
	void __fastcall SetTCPIPAddress(AnsiString value);
	AnsiString __fastcall GetTCPIPAddress();
	void __fastcall SetPOSID(int value);
	int __fastcall GetPOSID();

	void CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
	void ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);

	AnsiString LastTransactionRef;
	AnsiString GetRefNumber();

	AnsiString FDefaultPaymentCategory;
	AnsiString FDefaultItemCategory;
	AnsiString FCreditCategory;
	AnsiString FPointsCategory;
	AnsiString FRoundingCategory;
	AnsiString FDefaultTransactionAccount;
	AnsiString FDefaultSurchargeAccount;
	void __fastcall SetDefaultPaymentCategory(AnsiString value);
	AnsiString __fastcall GetDefaultPaymentCategory();
	void __fastcall SetDefaultItemCategory(AnsiString value);
	AnsiString __fastcall GetDefaultItemCategory();
	void __fastcall SetCreditCategory(AnsiString value);
	AnsiString __fastcall GetCreditCategory();
	void __fastcall SetPointsCategory(AnsiString value);
	AnsiString __fastcall GetPointsCategory();
	void __fastcall SetDefaultTransactionAccount(AnsiString value);
	AnsiString __fastcall GetDefaultTransactionAccount();
	void __fastcall SetDefaultSurchargeAccount(AnsiString value);
	AnsiString __fastcall GetDefaultSurchargeAccount();

	Database::TDBControl &DBControl;

	void __fastcall SetRoundingCategory(AnsiString value);
	AnsiString __fastcall GetRoundingCategory();
   std::set<AnsiString> CodesTestedOk;
	public :
	bool Registered;
	bool Enabled;
	TPhoenixHM(Database::TDBControl &inDBControl);
	void GetRoomStatus(TPhoenixRoomStatusExt &RoomStatus,AnsiString PMSIPAddress,int PMSPort);
	bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);

	AnsiString GetLastTransactionRef();

	void Initialise();

	__property int TCPPort  = { read=GetTCPPort, write=SetTCPPort};
	__property AnsiString TCPIPAddress  = { read=GetTCPIPAddress, write=SetTCPIPAddress };
	__property int POSID  = { read=GetPOSID, write=SetPOSID };

	std::auto_ptr<TPhoenixNetTCPManager>	fPhoenixNet;
	bool TestCode(AnsiString CodeToTest);
	void ClearCodesTestedOk();
	bool TestDefaultCodes();
   void FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode);
	__property AnsiString DefaultPaymentCategory  = { read=GetDefaultPaymentCategory, write=SetDefaultPaymentCategory };
	__property AnsiString DefaultItemCategory  = { read=GetDefaultItemCategory, write=SetDefaultItemCategory };
	__property AnsiString CreditCategory  = { read=GetCreditCategory, write=SetCreditCategory };
	__property AnsiString PointsCategory  = { read=GetPointsCategory, write=SetPointsCategory };
	__property AnsiString DefaultTransactionAccount  = { read=GetDefaultTransactionAccount, write=SetDefaultTransactionAccount };
	__property AnsiString DefaultSurchargeAccount  = { read=GetDefaultSurchargeAccount, write=SetDefaultSurchargeAccount };
	__property AnsiString RoundingCategory  = { read=GetRoundingCategory, write=SetRoundingCategory };

};

extern TPhoenixHM *PhoenixHM;

#endif

