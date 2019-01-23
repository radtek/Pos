//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSiHot.h"
#include "SiHotDataProcessor.h"
#include "MMMessageBox.h"
//#include "SiHotInterface.h"
#include "Processing.h"
#include "PMSHelper.h"
#include "GeneratorManager.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

const char CR = 0x0D; // Carriage Return
const char LF = 0x0A; // Line Feed
const char FF = 0x0C; // Form Feed
//---------------------------------------------------------------------------
TManagerSiHot::TManagerSiHot() : TBasePMS()
{
	TCPPort                 = 0;
	TCPIPAddress            = "";
	POSID                   = 0;
	DefaultPaymentCategory  = "";
	DefaultItemCategory     = "";
	PointsCategory          = "";
	CreditCategory          = "";
	LastTransactionRef      = "";
	RoundingCategory        = "";
    ApiKey                  = "";
	Enabled                 = false;
	Registered              = false;
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
	TCPPort                     = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSTCPPort);
	TCPIPAddress                = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress);
	POSID                       = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSPOSID);

	DefaultPaymentCategory      = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPaymentCategory);
	DefaultItemCategory         = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSItemCategory);
	CreditCategory              = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSCreditCategory);
	PointsCategory              = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPointsCategory);
	DefaultTransactionAccount   = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultAccount);
	DefaultSurchargeAccount     = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultSurchargeAccount);
	RoundingCategory            = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultRoundingAccount);
    TipAccount                  = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSTipAccount);
    ExpensesAccount             = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSExpensesAccount);
    ServiceChargeAccount        = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSServiceChargeAccount);
    RoundingAccountSiHot        = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSRoundingAccountSiHot);
    DefaultAccountNumber        = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotDefaultTransaction);
    RoundingAccountNumber       = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotRounding);
    ApiKey                      = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSAPIKey);
    RevenueCodesMap.clear();
    UnsetPostingFlag();

    if(Registered && TCPIPAddress != "")
	{
		Enabled = true;
        std::auto_ptr<TPMSHelper> pmsHelper(new TPMSHelper());
        //pmsHelper->LoadPMSPaymentTypes(PMSPaymentTypeMap);
        if(pmsHelper->LoadRevenueCodes(RevenueCodesMap, DBTransaction))
        {
            Enabled = GetRoundingandDefaultAccount();
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
    retValue = siHotDataProcessor->GetDefaultAccount(TCPIPAddress,TCPPort, TDeviceRealTerminal::Instance().BasePMS->ApiKey);
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
    roomResponse = siHotInterface->SendRoomChargePost(roomCharge,TGlobalSettings::Instance().PMSTimeOut,TDeviceRealTerminal::Instance().BasePMS->ApiKey,
                                                    TGlobalSettings::Instance().EnableItemDetailsPosting);
    Processing->Close();
    if(roomResponse.IsSuccessful)
    {
        _paymentTransaction.Customer.Name = _paymentTransaction.Phoenix.AccountName;
        _paymentTransaction.Customer.RoomNumberStr = _paymentTransaction.Phoenix.RoomNumber;
        retValue = true;
    }
    else
    {
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
            roomResponse = siHotInterface->SendRoomChargePost(roomCharge,TGlobalSettings::Instance().PMSTimeOut,TDeviceRealTerminal::Instance().BasePMS->ApiKey,TGlobalSettings::Instance().EnableItemDetailsPosting);
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
    retValue = siHotDataProcessor->GetDefaultAccount(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,0,TDeviceRealTerminal::Instance().BasePMS->ApiKey);
    UpdateSiHotLogs(retValue);
    return retValue;
}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::SendRoomRequest(TRoomRequest _roomRequest)
{
     // Call to SiHotInterface for sending Room Request
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     return siHotInterface->SendRoomRequest(_roomRequest,TGlobalSettings::Instance().PMSTimeOut,TDeviceRealTerminal::Instance().BasePMS->ApiKey);
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

    if(!CheckSihotPostingValidity(paymentTransaction))
        return true;

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
    TGlobalSettings::Instance().PMSPostSuccessful = retValue;
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
    directoryName = directoryName + "\\logs";
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
void TManagerSiHot::StoreTicketPost(UnicodeString invoiceNumber, TMemoryStream *receiptStream)
{
    bool response = false;
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Posting Tickets to SiHot , Please Wait...";
    Processing->Show();
    try 
    {
         // Call to SiHotDataProcessor for making Store Ticket Post
        std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
        TStoreTicket storeTicket;
        TRoomChargeResponse  roomResponse;
        receiptStream->Position = 0;
        AnsiString receiptData = FormatStoreTicket(receiptStream);
        siHotDataProcessor->CreateStoreTicketPost(invoiceNumber, storeTicket, receiptData);
        std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
        roomResponse = siHotInterface->SendStoreTicketPost(storeTicket,TGlobalSettings::Instance().PMSTimeOut,TDeviceRealTerminal::Instance().BasePMS->ApiKey);
        
        if(roomResponse.IsSuccessful)
            response = true;
        else
            response = false;

        //Saving response in PMSTicket table
        SavePMSTicketStatus(invoiceNumber, response);
    
        //Getting List of Invoice no. having failed status
        std::vector<UnicodeString> invoiceNumbers;
        if(response)
            invoiceNumbers = GetFailedPMSTicket();
    
        //Error handling (Posting of Old failed Store Tickets)
        if(invoiceNumbers.size() > 0)
            ManageFailedStoreTicketPost(invoiceNumbers);
    }
    catch(Exception &Exc) 
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
    Processing->Close();
}
//---------------------------------------------------------------------------
UnicodeString TManagerSiHot::FormatStoreTicket(TMemoryStream *receipt)
{
    UnicodeString receiptData = "";
    try
    {
        char *Line = (char*)receipt->Memory;
        for (int j = 0; j < receipt->Size; j++)
        {
            if (Line[j] == 0x1B)
            {
                if (Line[j + 1] == 0x1C)
                   j += 2;
                else if (Line[j + 1] == 0x72)
                   j += 2;
                else if (Line[j + 1] == 0x07)
                   j += 4;
                else if (Line[j + 1] == 0x64)
                   j += 2;
                else if (Line[j + 1] == 0x2D)
                   j += 2;
                else if (Line[j + 1] == 0x14)
                   j += 1;
                else if (Line[j + 1] == 0x0E)
                   j += 1;
                else if (Line[j + 1] == 'P')
                   j += 1;
                else if (Line[j + 1] == 'M')
                   j += 1;
                else if (Line[j + 1] == 'F')
                   j += 1;
                else if (Line[j + 1] == 'E')
                   j += 1;
            }
            else if (Line[j] == 0x14 || Line[j] == 0x0E)
            {
            // Single Character command just step over it.
            }
            else if (Line[j] == 0x1D)
            {
                if (Line[j + 1] == 0x56)
                   j += 2;
                else if (Line[j + 1] == 0x2F)
                   j += 2;
            }
            else if (Line[j] == ::CR)
            {
                if (Line[j + 1] == ::LF)
                   j += 1;
                receiptData += "\r";
            }
            else if (Line[j] == ::LF)
            {
                if (Line[j + 1] == ::CR)
                   j += 1;
                receiptData += "\r";
            }
            else
            {
                if (Line[j] == char(196))
                {
                   receiptData += "-";
                }
                else
                {
                   AnsiString check = "";
                   if(Line[j] == 0x21 && Line[j+1] == 0x00)
                   {
                        j += 1;
//                        receiptData += "";
                   }
                   else if(Line[j] == 0x00 && Line[j+1] != 0x21)
                   {
                        receiptData += "";
                   }
                   else
                        if((Line[j] >= 0x20 &&  Line[j] <= 0x7F))
                            receiptData += Line[j];
                }
            }
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return receiptData;
}
//---------------------------------------------------------------------------
void TManagerSiHot::SavePMSTicketStatus(UnicodeString invoiceNumber, bool response)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        int pmsTicketId = TGeneratorManager::GetNextGeneratorKey(DBTransaction, "GEN_PMSTICKETS_ID");
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "INSERT INTO PMSTICKETS (PMSTICKETS_ID, INVOICE_NUMBER, IS_TICKET_POSTED) "
                                     "VALUES (:PMS_TICKET_ID, :INVOICE_NUMBER, :RESPONSE ) ";

        IBInternalQuery->ParamByName("PMS_TICKET_ID")->AsInteger = pmsTicketId;
		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = invoiceNumber;
        if(response)
		    IBInternalQuery->ParamByName("RESPONSE")->AsString = "T";
        else
            IBInternalQuery->ParamByName("RESPONSE")->AsString = "F";

        IBInternalQuery->ExecQuery();

        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
std::vector<UnicodeString> TManagerSiHot::GetFailedPMSTicket()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    std::vector<UnicodeString> invoiceNumbers;
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT INVOICE_NUMBER FROM PMSTICKETS WHERE IS_TICKET_POSTED = :STATUS ";

        IBInternalQuery->ParamByName("STATUS")->AsString = "F";

        IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
        {
            invoiceNumbers.push_back(IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString);
        }

        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }

    return invoiceNumbers;
}
//---------------------------------------------------------------------------
void TManagerSiHot::ManageFailedStoreTicketPost(std::vector<UnicodeString> invoiceNumbers)
{
    bool response = false;
    TStoreTicket storeTicket;
    TRoomChargeResponse  roomResponse;
    AnsiString receiptData;
    for(int i = 0; i < invoiceNumbers.size() ; ++i)
    {
        receiptData = GetReceiptForStoreTicket(invoiceNumbers[i]);
        std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
        siHotDataProcessor->CreateStoreTicketPost(invoiceNumbers[i], storeTicket, receiptData);
        std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
        roomResponse = siHotInterface->SendStoreTicketPost(storeTicket,TGlobalSettings::Instance().PMSTimeOut,TDeviceRealTerminal::Instance().BasePMS->ApiKey);
        if(roomResponse.IsSuccessful)
        {
            response = true;
            UpdatePMSTicketStatus(invoiceNumbers[i],response);
        }
        else
        {
            break;
        }
    }
}
//--------------------------------------------------------------------------
void TManagerSiHot::UpdatePMSTicketStatus(UnicodeString invoiceNumber, bool response)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "UPDATE PMSTICKETS SET IS_TICKET_POSTED = 'T' WHERE INVOICE_NUMBER = :INVOICE_NUMBER ";

		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = invoiceNumber;

        IBInternalQuery->ExecQuery();
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
AnsiString TManagerSiHot::GetReceiptForStoreTicket(UnicodeString invoiceNumber)
{
    AnsiString receiptData;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text =  "SELECT RECEIPT FROM DAYARCBILL WHERE INVOICE_NUMBER = :INVOICE_NUMBER "
                                      "UNION ALL "
                                      "SELECT RECEIPT FROM ARCBILL WHERE INVOICE_NUMBER = :INVOICE_NUMBER ";

        IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = invoiceNumber;

        IBInternalQuery->ExecQuery();
        TMemoryStream *Receipt = new TMemoryStream;
		Receipt->Clear();
		IBInternalQuery->FieldByName("RECEIPT")->SaveToStream(Receipt);
        Receipt->Position = 0;
        receiptData = FormatStoreTicket(Receipt);
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }

    return receiptData;
}
bool TManagerSiHot::CheckSihotPostingValidity(TPaymentTransaction paymentTransaction)
{
    bool isNotCompleteCancel = false;
    bool isOnlyPointsTransaction = false;
    try
    {
        if(paymentTransaction.Orders->Count == 0 &&
           (paymentTransaction.Membership.Member.Points.getCurrentPointsPurchased() != 0 || paymentTransaction.Membership.Member.Points.getCurrentPointsRefunded() != 0))
        {
            isOnlyPointsTransaction = true;
        }
        if(!isOnlyPointsTransaction)
        {
            for(int indexOrder = 0; indexOrder < paymentTransaction.Orders->Count; indexOrder++)
            {
                TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[indexOrder];
                if(itemComplete->OrderType != CanceledOrder)
                {
                   isNotCompleteCancel = true;
                   break;
                }
            }
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return  !isOnlyPointsTransaction && isNotCompleteCancel;
}
