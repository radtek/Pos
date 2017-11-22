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
    ServiceChargeAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSServiceChargeAccount);
    RoundingAccountSiHot = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSRoundingAccountSiHot);
    DefaultAccountNumber = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotDefaultTransaction);
    RoundingAccountNumber = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotRounding);

	DBTransaction.Commit();
	if(Registered && TCPIPAddress != "")
	{
		Enabled = true;
        Enabled = GetRoundingandDefaultAccount();
	}
	else
	{
		Enabled = false;
	}
}
//---------------------------------------------------------------------------
bool TManagerSiHot::GetRoundingandDefaultAccount()
{
    bool retValue = false;
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    retValue = siHotDataProcessor->GetDefaultAccount(TCPIPAddress,TCPPort);
    if(!retValue)
        MessageBox("SiHot could not get enabled.Please set correct URL and Default Transaction Account","Error", MB_OK + MB_ICONERROR);
    return retValue;
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
    Processing->Close();
    if(!roomResponse.IsSuccessful)
    {
        UnicodeString errorMessage = roomResponse.ResponseMessage;
        if(errorMessage == "")
           errorMessage = "Room Not Found";
        MessageBox(errorMessage,"PMS Error",MB_OK + MB_ICONERROR);
    }
    siHotDataProcessor->PrepareRoomStatus(siHotAccounts,roomResponse);

}
//---------------------------------------------------------------------------
bool TManagerSiHot::RoomChargePost(TPaymentTransaction &_paymentTransaction)
{
     // Call to SiHotDataProcessor for making Room Charge Post

    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Posting Data to SiHot , Please Wait...";
    Processing->Show();
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    TRoomCharge roomCharge;
    TRoomChargeResponse  roomResponse;
    siHotDataProcessor->CreateRoomChargePost(_paymentTransaction, roomCharge);
    std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
    roomResponse = siHotInterface->SendRoomChargePost(roomCharge);
    Processing->Close();
    if(roomResponse.IsSuccessful)
    {
        _paymentTransaction.Customer.Name = _paymentTransaction.Phoenix.AccountName;
        _paymentTransaction.Customer.RoomNumberStr = _paymentTransaction.Phoenix.RoomNumber;
      return true;
    }
    else
    {
        if(roomResponse.ResponseMessage == "")
			roomResponse.ResponseMessage = "Sale could not get processed.Press OK to  process sale again";
        if(MessageBox(roomResponse.ResponseMessage,"Error", MB_OK + MB_ICONERROR) == ID_OK);
            return false;
    }
}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::SendRoomRequest(TRoomRequest _roomRequest)
{
     // Call to SiHotInterface for sending Room Request
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     return siHotInterface->SendRoomRequest(_roomRequest);
}
//---------------------------------------------------------------------------
bool TManagerSiHot::ExportData(TPaymentTransaction &paymentTransaction, int StaffID)
{
    bool roomChargeSelected = false;
    for(int paymentIndex = 0 ; paymentIndex < paymentTransaction.PaymentsCount(); paymentIndex++)
    {
       TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
       if(payment->GetPaymentAttribute(ePayTypeRoomInterface) && payment->GetPayTendered() != 0)
       {
           roomChargeSelected = true;
           break;
       }
    }
    bool retValue = false;
    if(!paymentTransaction.WasSavedSales || !roomChargeSelected)
        retValue = RoomChargePost(paymentTransaction);
    else
    {
        // Make a room request and check credit limit
        std::vector<TSiHotAccounts> siHotAccounts;
        siHotAccounts.clear();
        TSiHotAccounts account;
        account.AccountNumber = ((TItemComplete*)paymentTransaction.Orders->Items[0])->RoomNoStr;  // enter room number
        siHotAccounts.push_back(account);
//        MessageBox("found true","Account number from vector",MB_OK);
        GetRoomStatus(siHotAccounts,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPPort);
        bool checkedCreditLimit = false;
        bool creditLimitViolated = false;
        Currency roomTender = 0;
        for(int i = 0; i < siHotAccounts.size(); i++)
        {
//            MessageBox(siHotAccounts[i].AccountNumber,"Account number from vector",MB_OK);
            if(siHotAccounts[i].AccountNumber == ((TItemComplete*)paymentTransaction.Orders->Items[0])->AccNo)//AccountNumber as per item)
            {
                for(int j = 0; j < siHotAccounts[i].AccountDetails.size(); j++)
                {
//                    MessageBox("inside second loop","123",MB_OK);
                    for(int paymentIndex = 0 ; paymentIndex < paymentTransaction.PaymentsCount(); paymentIndex++)
                    {
                        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
//                        MessageBox(payment->GetPayTendered(),"payment->GetPayTendered()",MB_OK);
//                        MessageBox(StrToCurr(siHotAccounts[i].AccountDetails[j].CreditLimit),"1",MB_OK);
                        if(payment->GetPaymentAttribute(ePayTypeRoomInterface) && (double)payment->GetPayTendered() > 0)
                        {
//                            MessageBox(StrToCurr(siHotAccounts[i].AccountDetails[j].CreditLimit),"StrToCurr(siHotAccounts[i].AccountDetails[j].CreditLimit)",MB_OK);
                            if((payment->GetPayTendered() > StrToCurr(siHotAccounts[i].AccountDetails[j].CreditLimit)) &&
                              StrToCurr(siHotAccounts[i].AccountDetails[j].CreditLimit) != 0)
                            {
                               creditLimitViolated = true;
                               checkedCreditLimit = true;
                               MessageBox("Credit Limit exceeded","Error",MB_OK + MB_ICONERROR);
                            }
                            else
                                checkedCreditLimit = true;
                            break;
                        }
                    }
                    if(checkedCreditLimit)
                        break;
                }
            }
            if(checkedCreditLimit)
                break;
        }
        if(checkedCreditLimit && !creditLimitViolated)
          retValue = RoomChargePost(paymentTransaction);
    }
    return retValue;
}
//---------------------------------------------------------------------------
