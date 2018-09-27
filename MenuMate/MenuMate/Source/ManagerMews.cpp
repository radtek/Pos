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
    MewsAccountingCategoriesList.clear();
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
    MewsAccountingCategoriesList.clear();
    MewsAccountingMap.clear();
	TCPIPAddress                = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress);
    TipAccount                  = TManagerVariable::Instance().GetStr(DBTransaction,vmMewsTips);
    ServiceChargeAccount        = TManagerVariable::Instance().GetStr(DBTransaction,vmMewsServiceCharge);

    SelectedMewsOutlet = TManagerVariable::Instance().GetStr(DBTransaction, vmOutletIdMewsSelected, "");
    SelectedMewsService = TManagerVariable::Instance().GetStr(DBTransaction, vmServiceMewsSelected, "");
    ClientToken = TManagerVariable::Instance().GetStr(DBTransaction, vmClientToken, "");
    AccessToken = TManagerVariable::Instance().GetStr(DBTransaction, vmAccessToken, "");
    DefaultSurchargeAccount = TManagerVariable::Instance().GetStr(DBTransaction, vmPMSDefaultSurchargeAccount, "");
    UnsetPostingFlag();
    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
    processor->InitializeMewsCategories();
    MewsAccountingCategoriesList = processor->GetMewsCategoriesList();
    MewsAccountingMap = processor->GetMewsCategoriesMap();
    Enabled = false;
    if(Registered && TCPIPAddress != "")
	{
        if(ClientToken.Trim() != "")
        {
            if(AccessToken.Trim() != "")
            {
                if(SelectedMewsOutlet.Trim() != "")
                {
                    if(SelectedMewsService.Trim() != "")
                    {
                        if(TipAccount.Trim() != "")
                        {
                            if(ServiceChargeAccount.Trim() != "")
                                Enabled = true;
                            else
                                MessageBox("Service Charge account is required to be selected.\rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
                        }
                        else
                            MessageBox("Tip account is required to be selected.\rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);

                    }
                    else
                        MessageBox("Service is required to be selected.\rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
                }
                else
                    MessageBox("Outlet is required to be selected.\rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
            }
            else
                MessageBox("Client Token value is not provided.\rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);
        }
        else
            MessageBox("Client Token value is not provided.\rMews is not enabled.","Info",MB_OK+MB_ICONINFORMATION);

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
bool TManagerMews::SetUpMews(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken)
{
    bool retValue = false;
    try
    {
        retValue     = GetOutlets(url,clientToken,accessToken);
        if(retValue)
            retValue = GetServices(url,clientToken,accessToken);
        if(retValue)
            retValue = GetSpaces(url,clientToken,accessToken);
        if(retValue)
            retValue = GetCategories(url,clientToken,accessToken);
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerMews::GetOutlets(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken)
{
    bool retValue = false;
    TMMProcessingState StateOutlets(Screen->ActiveForm, "Getting Outlets. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateOutlets);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TOutlet> outlets = siHotInterface->GetOutlets(url,clientToken, accessToken);
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
bool TManagerMews::GetServices(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken)
{
    bool retValue = false;
    TMMProcessingState StateServices(Screen->ActiveForm, "Getting Services. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateServices);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TServiceMews> services = siHotInterface->GetServices(url,clientToken,accessToken);
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
bool TManagerMews::GetSpaces(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken)
{
    bool retValue = false;
    TMMProcessingState StateSpaces(Screen->ActiveForm, "Getting Spaces. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateSpaces);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        TSpaceDetails spaces = siHotInterface->GetSpaces(url,clientToken,accessToken);
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
bool TManagerMews::GetCategories(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken)
{
    bool retValue = false;
    TMMProcessingState StateCategories(Screen->ActiveForm, "Getting Categories. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateCategories);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TAccountingCategory> categories = siHotInterface->GetMewsAccountingCategories(url,clientToken,accessToken);
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
bool TManagerMews::ExportData(TPaymentTransaction &_paymentTransaction, int StaffID)
{
    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
    double tip = 0;
    double tipEftPOS = 0;
    bool isSuccessful = false;
    MessageBox("Export Data of Mews","",MB_OK);
    try
    {
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            if(payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
               tip += (double)payment->GetAdjustment();
            }
            tipEftPOS += (double)payment->TipAmount;
        }
        double totalPayTendered = 0;
        double tipAux = 0;
        if(tip >= 0.0)
            tipAux = tip;
        double roundedPaymentAmount = (double)_paymentTransaction.Money.PaymentAmount -             //payment->GetPayTotal()
                                      (double)_paymentTransaction.Money.PaymentSurcharges
                                      + tipAux;
//        MessageBox(roundedPaymentAmount,"roundedPaymentAmount",MB_OK);
//        MessageBox(_paymentTransaction.Money.Change,"Change",MB_OK);
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            double amount = (double)payment->GetPayTendered() - (double)payment->GetSurcharge();
//            MessageBox(amount,"amount",MB_OK);
//            MessageBox(payment->GetChange(),"payment change",MB_OK);
//            MessageBox(payment->GetPayTendered(),payment->GetSurcharge(),MB_OK);
            totalPayTendered += amount;
            double portion = 0;
            double portionOriginal = 0;
            if((amount != 0)
                  && !payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
                roundedPaymentAmount += (double)payment->GetPayRounding();
//                MessageBox(roundedPaymentAmount,"roundedPaymentAmount",MB_OK);
                portion = (double)amount/roundedPaymentAmount ;
//                MessageBox(portion,"portion",MB_OK);
                portionOriginal = portion;
                tip += tipEftPOS;
                double tipPortion = RoundTo(tip * portion,-2);
//                MessageBox(tipPortion,"tipPortion",MB_OK);
//                // Get Post Data
                TOrder mewsOrder;
                mewsOrder.ClientToken = ClientToken;
                mewsOrder.AccessToken = AccessToken;
                std::auto_ptr<TMewsInterface> mewsInterface(new TMewsInterface());
                if(payment->GetPaymentAttribute(ePayTypeRoomInterface))
                {
                    mewsOrder.CustomerId  = _paymentTransaction.Phoenix.AccountNumber;
                    mewsOrder.ServiceId   = SelectedMewsService;
                    mewsOrder.ConsumptionUtc = Now();
                    MessageBox("composing order for mews","",MB_OK);
                    mewsOrder.Items.clear();
                    mewsOrder.Items = processor->GetMewsOrder(_paymentTransaction,portion, i,tipPortion, MewsAccountingMap);
                    UnicodeString value = mewsInterface->PostMewsOrder(TCPIPAddress,mewsOrder);
                    if(value.Trim() != "Successful")
                        isSuccessful = true;
                }
                else
                {
                    MessageBox("composing Bill for mews","",MB_OK);
                    mewsOrder.Bills.clear();
                    mewsOrder.Bills = processor->GetMewsBill(_paymentTransaction,portion, i,tipPortion, MewsAccountingMap);
                }
                double surcharge = 0;
                if(_paymentTransaction.CreditTransaction)
                    surcharge = payment->GetDiscount();
                else
                    surcharge = payment->GetSurcharge();
//                if(surcharge != 0)
//                {
//                   surcharge = RoundTo(surcharge * 100 , -2);
//                   AnsiString strSurcharge = (AnsiString)surcharge;
//                   if(strSurcharge.Pos(".") != 0)
//                   {
//                      strSurcharge = strSurcharge.SubString(1,strSurcharge.Pos(".")-1);
//                   }
//                   for(int subtotalindex = 0; subtotalindex < 16; subtotalindex++)
//                   {
//                      if(postRequest.Subtotal1[subtotalindex] != "" && postRequest.Subtotal1[subtotalindex] != 0)
//                      {
//                        int paymentSurcharge = atoi(postRequest.ServiceCharge[subtotalindex].c_str());
//                        paymentSurcharge += atoi(strSurcharge.c_str());
//                        AnsiString str = paymentSurcharge;
//                        postRequest.ServiceCharge[subtotalindex] = str;
//                        break;
//                      }
//                   }
//                }
//
//                if(_paymentTransaction.Money.TotalRounding  != 0)
//                {
//                    double totalRounding = (double)_paymentTransaction.Money.TotalRounding;
//                    double roundingPortion = totalRounding * portion;
//                    roundingPortion = RoundTo(roundingPortion,-2);
//                    roundingPortion = roundingPortion * 100;
//                    AnsiString roundingPortionStr = (AnsiString)roundingPortion;
//                    if(roundingPortionStr.Pos(".") != 0)
//                    {
//                      roundingPortionStr = roundingPortionStr.SubString(1,roundingPortionStr.Pos(".")-1);
//                    }
//                    int roundingPortionInt = atoi(roundingPortionStr.c_str());
//                    int oldTotal = atoi(postRequest.TotalAmount.c_str());
//                    oldTotal += roundingPortionInt;
//                    postRequest.TotalAmount = oldTotal;
//                    for(int index = 0; index < postRequest.Subtotal1.size(); index++)
//                    {
//                       if(postRequest.Subtotal1[index].Trim() != "")
//                       {
//                           int oldSubTotal = atoi(postRequest.Subtotal1[index].c_str());
//                           oldSubTotal += roundingPortionInt;
//                           postRequest.Subtotal1[index] = oldSubTotal;
//                           break;
//                       }
//                    }
//                }
//                postRequest.CheckNumber = checkNumber;
                //  Send Post
                //  Capture result
            }
         }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
//        DBTransaction.Rollback();
    }
    return isSuccessful;
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
std::vector<TAccountingCategory> TManagerMews::GetCategoriesFromDB()
{
    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
    return processor->GetCategoriesFromDB();
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
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception",MB_OK);
    }
}
//---------------------------------------------------------------------------
