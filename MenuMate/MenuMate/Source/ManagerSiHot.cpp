//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSiHot.h"
#include "SiHotDataProcessor.h"
#include "MMMessageBox.h"
//#include "SiHotInterface.h"
#include "Processing.h"
#include "PMSHelper.h"
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
void TManagerSiHot::LogPMSEnabling(TriggerLocation triggerType)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(triggerType == eUI)
        {
            List->Add("Note- "+ (AnsiString)"Enabling SiHot as Selected from UI" +"\n");
        }
        else if(triggerType == eBoot)
        {
            List->Add("Note- "+ (AnsiString)"Enabling SiHot at start of Menumate" +"\n");
        }
        else if(triggerType == eSelf)
        {
            List->Add("Note- "+ (AnsiString)"Found SiHot disabled with necessary details present" +"\n" +
                  "      "+ "Menumate is trying to enable SiHot and then sale would be processed" +"\n");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
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
    RevenueCodesMap.clear();
    UnsetPostingFlag();

    if(Registered && TCPIPAddress != "")
	{
		Enabled = true;
        std::auto_ptr<TPMSHelper> pmsHelper(new TPMSHelper());
        //pmsHelper->LoadPMSPaymentTypes(PMSPaymentTypeMap);
        if(pmsHelper->LoadRevenueCodes(RevenueCodesMap, DBTransaction))
        {
            if(DefaultItemCategory.Trim() != "")
                Enabled = GetRoundingandDefaultAccount();
            else
            {
                MessageBox("Api-Key is required for set up of SiHot.","Warning",MB_OK + MB_ICONINFORMATION);
                Enabled = false;
            }
        }
        else
        {
            MessageBox("Revenue codes are required for set up of SiHot.", "Warning", MB_OK + MB_ICONINFORMATION);
            Enabled = false;
        }
        if(Enabled)
        {
            DefaultAccountNumber = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotDefaultTransaction);
        }
	}
	else
	{
		Enabled = false;
	}
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
bool TManagerSiHot::GetRoundingandDefaultAccount()
{
    bool retValue = false;
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    retValue = siHotDataProcessor->GetDefaultAccount(TCPIPAddress,TCPPort, DefaultItemCategory);
    UpdateSiHotLogs(retValue);
    if(!retValue)
        MessageBox("SiHot could not get enabled.Please set correct URL and Default Transaction Account","Error", MB_OK + MB_ICONERROR);
    return retValue;
}
//---------------------------------------------------------------------------
void TManagerSiHot::GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString pmsIPAddress,int pmsPort)
{
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Syncing in progress , Please Wait...";
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
        if(errorMessage.Pos("roomnotfound") || errorMessage == "")
            errorMessage = "Room Not Found";
        else if(errorMessage.Pos("roomnotoccupied"))
           errorMessage = "Room Not Occupied";
        MessageBox(errorMessage,"PMS Error",MB_OK + MB_ICONERROR);
    }
    siHotDataProcessor->PrepareRoomStatus(siHotAccounts,roomResponse);

}
//---------------------------------------------------------------------------
bool TManagerSiHot::RoomChargePost(TPaymentTransaction &_paymentTransaction)
{
     // Call to SiHotDataProcessor for making Room Charge Post
    bool retValue = false;
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Posting Data to SiHot , Please Wait...";
    Processing->Show();
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    TRoomCharge roomCharge;
    TRoomChargeResponse  roomResponse;
    siHotDataProcessor->CreateRoomChargePost(_paymentTransaction, roomCharge);
    std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
    roomResponse = siHotInterface->SendRoomChargePost(roomCharge,TGlobalSettings::Instance().PMSTimeOut,DefaultItemCategory);
    Processing->Close();
    if(roomResponse.IsSuccessful)
    {
        _paymentTransaction.Customer.Name = _paymentTransaction.Phoenix.AccountName;
        _paymentTransaction.Customer.RoomNumberStr = _paymentTransaction.Phoenix.RoomNumber;
        retValue = true;
    }
    else
    {
//        if(roomCharge.AccountNumber.Trim() != TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber.Trim())
//        {
            AnsiString responseString = "";
            responseString = roomResponse.ResponseMessage;
            if(roomResponse.ResponseMessage == "")
                responseString = "Sale could not get processed.Press OK to  process sale again";
            if(roomCharge.AccountNumber.Trim() == TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber.Trim() &&
               responseString.Pos("accountclosed") != 0)
            {
                responseString += "\rPlease try again or check your Default Room configuration.";
                TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
            }
            if(MessageBox(responseString,"Error", MB_OK + MB_ICONERROR) == ID_OK);
                retValue = false;
//        }
//        else
//        {
//            retValue = RetryDefaultRoomPost(_paymentTransaction,roomCharge);
//        }
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerSiHot::RetryDefaultRoomPost(TPaymentTransaction &_paymentTransaction, TRoomCharge roomCharge)
{
    bool retValue = false;
    try
    {
        UnicodeString processMessage =
                              "Sync in progress for fresh details of Default Transaction Account...";
        bool isInquirySuccessful = false;
        isInquirySuccessful = GetDefaultAccount(processMessage);
        if(isInquirySuccessful)
        {
            std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
            TRoomChargeResponse  roomResponse;
            roomCharge.AccountNumber = (UnicodeString)TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber;
            _paymentTransaction.Phoenix.AccountName = TManagerVariable::Instance().GetStr(_paymentTransaction.DBTransaction,vmSiHotDefaultTransactionName);
            _paymentTransaction.Phoenix.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
            _paymentTransaction.Phoenix.AccountNumber = roomCharge.AccountNumber;
            _paymentTransaction.SalesType = eRoomSale;
            _paymentTransaction.Customer.RoomNumberStr = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;

            for (int i = 0; i < _paymentTransaction.Orders->Count; i++)
            {
                TItemComplete *Order = (TItemComplete*)_paymentTransaction.Orders->Items[i];
                if(Order->TabType != TabNone && Order->TabType != TabCashAccount)
                    break;
                if(Order->TabType != TabCashAccount)
                {
                    Order->TabType          = TabRoom;
                    Order->TabContainerName = _paymentTransaction.Phoenix.RoomNumber;
                    Order->TabName = _paymentTransaction.Phoenix.RoomNumber;
                }
                Order->RoomNoStr = _paymentTransaction.Phoenix.AccountNumber;
            }
            roomResponse = siHotInterface->SendRoomChargePost(roomCharge,TGlobalSettings::Instance().PMSTimeOut,DefaultItemCategory);
            if(roomResponse.IsSuccessful)
                retValue = true;
            else
            {
                if(roomResponse.ResponseMessage == "")
                    roomResponse.ResponseMessage = "Sale could not get processed.Press OK to  process sale again";
                if(MessageBox(roomResponse.ResponseMessage,"Error", MB_OK + MB_ICONERROR) == ID_OK)
                    retValue = false;
            }
        }
        else
            MessageBox("Default Room is not checked in.","Error",MB_OK);
    }
    catch(Exception &Exc)
    {
        retValue = false;
    }
    return retValue;
}
//---------------------------------------------------------------------------
/*
        Added code to check the account details for default room number
        (stored in DefaultTransactionAccount)
        The change is required to mitigate the risk if the default room number is
        having a new check in with different account number(stored in )
*/
bool TManagerSiHot::GetDefaultAccount(UnicodeString processMessage)
{
    bool retValue = false;
    std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
    retValue = siHotDataProcessor->GetDefaultAccount(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,0,DefaultItemCategory);
    UpdateSiHotLogs(retValue);
    return retValue;
}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::SendRoomRequest(TRoomRequest _roomRequest)
{
     // Call to SiHotInterface for sending Room Request
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     return siHotInterface->SendRoomRequest(_roomRequest,TGlobalSettings::Instance().PMSTimeOut,DefaultItemCategory);
}
//---------------------------------------------------------------------------
bool TManagerSiHot::ExportData(TPaymentTransaction &paymentTransaction, int StaffID)
{
   WaitOrProceedWithPost();
   return ExportData(paymentTransaction);
}
//---------------------------------------------------------------------------
bool TManagerSiHot::ExportData(TPaymentTransaction &paymentTransaction)
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
        GetRoomStatus(siHotAccounts,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPPort);
        bool checkedCreditLimit = false;
        bool creditLimitViolated = false;
        Currency roomTender = 0;
        for(int i = 0; i < siHotAccounts.size(); i++)
        {
            if(siHotAccounts[i].AccountNumber == ((TItemComplete*)paymentTransaction.Orders->Items[0])->AccNo)//AccountNumber as per item)
            {
                for(int j = 0; j < siHotAccounts[i].AccountDetails.size(); j++)
                {
                    for(int paymentIndex = 0 ; paymentIndex < paymentTransaction.PaymentsCount(); paymentIndex++)
                    {
                        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
                        if(payment->GetPaymentAttribute(ePayTypeRoomInterface) && (double)payment->GetPayTendered() > 0)
                        {
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
    if(!retValue)
        UnsetPostingFlag();
    return retValue;
}
//---------------------------------------------------------------------------
void TManagerSiHot::UpdateSiHotLogs(bool status)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(status)
        {
            List->Insert((List->Count-1),"<<< SiHot Interface Enabled >>>");
        }
        else
        {
            List->Insert((List->Count-1),"<<< SiHot Interface Not Enabled >>>");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
AnsiString TManagerSiHot::GetLogFileName()
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "Menumate Services";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\Sihot Post Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "SiHotPosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "\\" + name;
    return fileName;
}
//---------------------------------------------------------------------------
void TManagerSiHot::WaitOrProceedWithPost()
{
    bool isPosting = false;
    int global_profile_key;
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    std::auto_ptr<TStringList> waitLogs(new TStringList);
    try
    {
        Sleep(Random(21));
        Sleep(Random(30));
        Sleep(Random(50));
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        tr.StartTransaction();
        // This is used to retain the state of the checkbox if the POS is exited
        #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
        #pragma warn .pia
        TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsSiHotPostInProgress, isPosting);
        if(isPosting)
            waitLogs->Add("Entered queue at                          " + Now().FormatString("hh:mm:ss tt"));
        while(isPosting)
        {
            Sleep(400);
            TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsSiHotPostInProgress, isPosting);
        }
        mv.SetProfileBool(tr,global_profile_key,
        vmIsSiHotPostInProgress,true);
        tr.Commit();
        //SetPostingFlag();
        if(waitLogs->Count > 0)
        {
            waitLogs->Add("Wait Over at                              " + Now().FormatString("hh:mm:ss tt"));
            waitLogs->Add("=================================================================================");
            LogWaitStatus(waitLogs);
        }
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TManagerSiHot::LogWaitStatus(std::auto_ptr<TStringList> waitLogs)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        for(int index = 0; index < waitLogs->Count; index++)
        {
            AnsiString value = waitLogs->operator [](index);
            List->Add(value);
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TManagerSiHot::SetPostingFlag()
{
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();
    try
    {
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        int global_profile_key;

        // This is used to retain the state of the checkbox if the POS is exited
    #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
    #pragma warn .pia

        mv.SetProfileBool(tr,global_profile_key,
        vmIsSiHotPostInProgress,true);

        tr.Commit();
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TManagerSiHot::UnsetPostingFlag()
{
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();
    try
    {
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        int global_profile_key;

        // This is used to retain the state of the checkbox if the POS is exited
    #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
    #pragma warn .pia

        mv.SetProfileBool(tr,global_profile_key,
        vmIsSiHotPostInProgress,false);

        tr.Commit();
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
