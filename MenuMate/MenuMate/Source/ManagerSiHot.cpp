//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSiHot.h"
#include "SiHotDataProcessor.h"
#include "MMMessageBox.h"
#include "DeviceRealterminal.h"
#include "SiHotInterface.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
TManagerSiHot::TManagerSiHot() : TBasePMS()
{
	TCPPort = 0;
	TCPIPAddress = "";
	POSID = 0;
	DefaultPaymentCategory = "";
	DefaultItemCategory = "";
	PointsCategory = "";
	CreditCategory = "";
	LastTransactionRef = "";
	RoundingCategory = "";
	Enabled = false;
	Registered = false;
    MessageBox("Inside Constructor of SiHot","Notify",MB_OK);
}
//---------------------------------------------------------------------------
TManagerSiHot::~TManagerSiHot()
{

}
//---------------------------------------------------------------------------
void TManagerSiHot::Initialise()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TCPPort = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSTCPPort);
	TCPIPAddress = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress);
	POSID = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSPOSID);

	DefaultPaymentCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPaymentCategory);
    MessageBox(DefaultPaymentCategory,"DefaultPaymentCategory",MB_OK);
	DefaultItemCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSItemCategory);
	CreditCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSCreditCategory);
	PointsCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPointsCategory);
	DefaultTransactionAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultAccount);
	DefaultSurchargeAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultSurchargeAccount);
	RoundingCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultRoundingAccount);

	DBTransaction.Commit();
	if(Registered && TCPIPAddress != "")
	{
		Enabled = true;
		Enabled = TestDefaultCodes();
	}
	else
	{
		Enabled = false;
	}
}
//---------------------------------------------------------------------------
void TManagerSiHot::GetRoomStatus(TPhoenixRoomStatusExt &status,AnsiString pmsIPAddress,int pmsPort)
{
    if(status.AccountNumber != "")
    {
        std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
        TRoomRequest roomRequest;
        roomRequest = siHotDataProcessor->CreateRoomRequest(status,pmsIPAddress,pmsPort);
        TRoomResponse roomResponse;
        roomResponse = SendRoomRequest(roomRequest);
        status = siHotDataProcessor->PrepareRoomStatus(roomResponse);
    }
}
//---------------------------------------------------------------------------
bool TManagerSiHot::RoomChargePost(TPaymentTransaction _paymentTransaction)
{
     // Call to SiHotDataProcessor for making Room Charge Post
      std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
      TRoomCharge roomCharge;
      siHotDataProcessor->CreateRoomChargePost(_paymentTransaction, roomCharge);

      return SendRoomChargePost(roomCharge);
}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::SendRoomRequest(TRoomRequest _roomRequest)
{
//     // Call to SiHotInterface for sending Room Request
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     return siHotInterface->SendRoomRequest(_roomRequest);
}//---------------------------------------------------------------------------
bool TManagerSiHot::SendRoomChargePost(TRoomCharge _roomCharge)
{
     // Call to SiHotInterface for sending Room Charge Post
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     TRoomChargeResponse roomResponse = siHotInterface->SendRoomChargePost(_roomCharge);
     return true;
}
//---------------------------------------------------------------------------
bool TManagerSiHot::ExportData(TPaymentTransaction &PaymentTransaction, int StaffID)
{
    return true;
}
//---------------------------------------------------------------------------
