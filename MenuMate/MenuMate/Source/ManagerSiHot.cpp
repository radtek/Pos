//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSiHot.h"
#include "SiHotDataProcessor.h"
#include "MMMessageBox.h"
#include "DeviceRealterminal.h"
#include "SiHotInterface.h"
#include "Processing.h"
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
	DefaultItemCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSItemCategory);
	CreditCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSCreditCategory);
	PointsCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPointsCategory);
	DefaultTransactionAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultAccount);
	DefaultSurchargeAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultSurchargeAccount);
	RoundingCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultRoundingAccount);
    TipAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSTipAccount);
    ExpensesAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSExpensesAccount);

	DBTransaction.Commit();
	if(Registered && TCPIPAddress != "")
	{
		Enabled = true;
        Enabled = CheckIPAddressPort(TCPIPAddress,TCPPort);
	}
	else
	{
		Enabled = false;
	}
}
//---------------------------------------------------------------------------
bool TManagerSiHot::CheckIPAddressPort(AnsiString tcpIPAddress,int tcpPort)
{
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    int transno = siHotDataProcessor->GetTransNumber();
    std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
    bool value = siHotInterface->ValidateIPAddressPort(tcpIPAddress,tcpPort,transno);
    if(!value)
       MessageBox("SiHot could not get enabled.Please set correct SiHot IP Address and Port Number","Error", MB_OK + MB_ICONERROR);
    return value;
}
//---------------------------------------------------------------------------
void TManagerSiHot::GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString pmsIPAddress,int pmsPort)
{
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Getting Room Details , Please Wait...";
    Processing->Show();
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    TRoomRequest roomRequest;
    roomRequest = siHotDataProcessor->CreateRoomRequest(siHotAccounts,pmsIPAddress,pmsPort);
    TRoomResponse roomResponse;
    roomResponse = SendRoomRequest(roomRequest);
    siHotDataProcessor->PrepareRoomStatus(siHotAccounts,roomResponse);
    Processing->Close();
}
//---------------------------------------------------------------------------
bool TManagerSiHot::RoomChargePost(TPaymentTransaction _paymentTransaction)
{
     // Call to SiHotDataProcessor for making Room Charge Post
     if(_paymentTransaction.Phoenix.AccountNumber == "")
         return true;
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Getting Room Details , Please Wait...";
    Processing->Show();
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    TRoomCharge roomCharge;
    TRoomChargeResponse  roomResponse;
    siHotDataProcessor->CreateRoomChargePost(_paymentTransaction, roomCharge);
    std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
    roomResponse = siHotInterface->SendRoomChargePost(roomCharge);
    Processing->Close();
    if(roomResponse.IsSuccessful)
      return true;
    else
    {
      MessageBox(roomResponse.ResponseMessage,"Error", MB_OK + MB_ICONERROR);
      return false;
    }
}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::SendRoomRequest(TRoomRequest _roomRequest)
{
     // Call to SiHotInterface for sending Room Request
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     return siHotInterface->SendRoomRequest(_roomRequest);
}//---------------------------------------------------------------------------
bool TManagerSiHot::ExportData(TPaymentTransaction &PaymentTransaction, int StaffID)
{
    return RoomChargePost(PaymentTransaction) ;
}
//---------------------------------------------------------------------------
