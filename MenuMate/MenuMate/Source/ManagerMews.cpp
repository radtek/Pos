//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerMews.h"
#include "MMMessageBox.h"
#include "Processing.h"
#include "SiHotInterface.h"
#include "MewsDataProcessor.h"
#include "MewsInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
TManagerMews::TManagerMews() : TBasePMS()
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
TManagerMews::~TManagerMews()
{

}
//---------------------------------------------------------------------------
void TManagerMews::Initialise()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
//	TCPPort                     = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSTCPPort);
	TCPIPAddress                = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress);
//	POSID                       = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSPOSID);
//
//	DefaultPaymentCategory      = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPaymentCategory);
//	DefaultItemCategory         = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSItemCategory);
//	CreditCategory              = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSCreditCategory);
//	PointsCategory              = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPointsCategory);
//	DefaultTransactionAccount   = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultAccount);
//	DefaultSurchargeAccount     = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultSurchargeAccount);
//	RoundingCategory            = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultRoundingAccount);
//    TipAccount                  = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSTipAccount);
//    ExpensesAccount             = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSExpensesAccount);
//    ServiceChargeAccount        = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSServiceChargeAccount);
//    RoundingAccountSiHot        = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSRoundingAccountSiHot);
//    DefaultAccountNumber        = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotDefaultTransaction);
//    RoundingAccountNumber       = TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotRounding);
//    ApiKey                      = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSAPIKey);
//    RevenueCodesMap.clear();
        SelectedMewsOutlet = TManagerVariable::Instance().GetStr(DBTransaction, vmOutletIdMewsSelected, "");
        SelectedMewsService = TManagerVariable::Instance().GetStr(DBTransaction, vmServiceMewsSelected, "");
        ClientToken = TManagerVariable::Instance().GetStr(DBTransaction, vmClientToken, "");
        AccessToken = TManagerVariable::Instance().GetStr(DBTransaction, vmAccessToken, "");
//    UnsetPostingFlag();
//
    Enabled = false;
    if(Registered && TCPIPAddress != "")
	{
        Enabled = true;
        //pmsHelper->LoadPMSPaymentTypes(PMSPaymentTypeMap);
        if(ClientToken.Trim() != "")
        {
            if(AccessToken.Trim() != "")
            {
                if(SelectedMewsOutlet.Trim() != "")
                {
                    if(SelectedMewsService.Trim() != "")
                    {
                        Enabled = true;
                    }
                    else
                        MessageBox("Service is required to be selected./rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
                }
                else
                    MessageBox("Outlet is required to be selected./rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
            }
            else
                MessageBox("Client Token value is not provided./rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
        }
        else
            MessageBox("Client Token value is not provided./rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);

	}
    //Enabled = true;
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TManagerMews::LogPMSEnabling(TriggerLocation triggerType)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(triggerType == eUI)
        {
            List->Add("Note- "+ (AnsiString)"Enabling Mews as Selected from UI" +"\n");
        }
        else if(triggerType == eBoot)
        {
            List->Add("Note- "+ (AnsiString)"Enabling Mews at start of Menumate" +"\n");
        }
        else if(triggerType == eSelf)
        {
            List->Add("Note- "+ (AnsiString)"Found Mews disabled with necessary details present" +"\n" +
                  "      "+ "Menumate is trying to enable Mews and then sale would be processed" +"\n");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
AnsiString TManagerMews::GetLogFileName()
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "Menumate Services";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\Mews Post Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "Mews " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "\\" + name;
    return fileName;
}
//---------------------------------------------------------------------------
void TManagerMews::UpdateMewsLogs(bool status)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(status)
        {
            List->Insert((List->Count-1),"<<< Mews Interface Enabled >>>");
        }
        else
        {
            List->Insert((List->Count-1),"<<< Mews Interface Not Enabled >>>");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
void TManagerMews::WaitOrProceedWithPost()
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
void TManagerMews::LogWaitStatus(std::auto_ptr<TStringList> waitLogs)
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
void TManagerMews::SetPostingFlag()
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
void TManagerMews::UnsetPostingFlag()
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
bool TManagerMews::SetUpMews(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken)
{
    bool retValue = false;
    try
    {
        retValue     = GetOutlets(url,accessToken,clientToken);
        if(retValue)
            retValue = GetServices(url,accessToken,clientToken);
        if(retValue)
            retValue = GetSpaces(url,accessToken,clientToken);
        if(retValue)
            retValue = GetCategories(url,accessToken,clientToken);
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerMews::GetOutlets(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken)
{
    bool retValue = false;
    TMMProcessingState StateOutlets(Screen->ActiveForm, "Getting Outlet. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateOutlets);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TOutlet> outlets = siHotInterface->GetOutlets(url,accessToken,clientToken);
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        mewsDataProcessor->UpdateOutlets(outlets);
        retValue = true;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerMews::GetServices(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken)
{
    bool retValue = false;
    TMMProcessingState StateServices(Screen->ActiveForm, "Getting Services. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateServices);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TServiceMews> services = siHotInterface->GetServices(url,accessToken,clientToken);
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        mewsDataProcessor->UpdateServices(services);
        retValue = true;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerMews::GetSpaces(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken)
{
    bool retValue = false;
    TMMProcessingState StateSpaces(Screen->ActiveForm, "Getting Spaces. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateSpaces);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        TSpaceDetails spaces = siHotInterface->GetSpaces(url,accessToken,clientToken);
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        mewsDataProcessor->UpdateSpaces(spaces);
        retValue = true;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerMews::GetCategories(UnicodeString url, UnicodeString accessToken, UnicodeString clientToken)
{
    bool retValue = false;
    TMMProcessingState StateCategories(Screen->ActiveForm, "Getting Categories. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateCategories);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TAccountingCategory> categories = siHotInterface->GetMewsAccountingCategories(url,accessToken,clientToken);
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        mewsDataProcessor->UpdateCategories(categories);

        retValue = true;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerMews::ExportData(TPaymentTransaction &PaymentTransaction, int StaffID)
{
    return true;
}
//---------------------------------------------------------------------------
std::vector<TOutlet> TManagerMews::GetOutletsFromDB()
{
    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
    return processor->GetOutlets();
}
//---------------------------------------------------------------------------
std::vector<TServiceMews> TManagerMews::GetServicesFromDB()
{
    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
    return processor->GetServices();
}
//---------------------------------------------------------------------------
void TManagerMews::GetMewsCustomerByName(UnicodeString queryString, std::vector<TCustomerMews> &customerMews)
{
    try
    {
        customerMews.clear();
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        TCustomerSearch customerSearch = mewsDataProcessor->CreateInquiryForRoomByName(queryString);
        customerSearch.AccessToken = AccessToken;
        customerSearch.ClientToken = ClientToken;
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        customerMews = siHotInterface->GetCustomers(TCPIPAddress,customerSearch);
        MessageBox(customerMews.size(),"size for customers by name",MB_OK);
        if(customerMews.size() == 0)
            MessageBox("No customers found for selection criteria","Info",MB_OK+MB_ICONINFORMATION);
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception",MB_OK);
    }
}
//---------------------------------------------------------------------------
void TManagerMews::GetMewsCustomerBySpace(UnicodeString queryString, std::vector<TCustomerMews> &customerMews)
{
    try
    {
        customerMews.clear();
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        TCustomerSearch customerSearch = mewsDataProcessor->CreateInquiryForRoomBySpace(queryString);
        customerSearch.AccessToken = AccessToken;
        customerSearch.ClientToken = ClientToken;
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        customerMews = siHotInterface->GetCustomers(TCPIPAddress,customerSearch);
        MessageBox(customerMews.size(),"size for customers by space",MB_OK);
        if(customerMews.size() == 0)
            MessageBox("No customers found for selection criteria","Info",MB_OK+MB_ICONINFORMATION);
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception",MB_OK);
    }
}
//---------------------------------------------------------------------------
