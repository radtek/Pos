//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerMews.h"
#include "MewsInterface.h"
#include "MewsDataProcessor.h"
#include "Generatormanager.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//public
TManagerMews::TManagerMews()
{
    TCPIPAddress                = "";
    ExpensesAccount             = "";
    RevenueCentre               = "";
	DefaultTransactionAccount   = "";
	DefaultPaymentCategory      = "";
	DefaultSurchargeAccount     = "";
	TipAccount                  = "";
	ServiceChargeAccount        = "";
    PointsCategory              = "";
    CreditCategory              = "";
	Enabled                     = false;
	Registered                  = false;
}
TManagerMews::~TManagerMews()
{
}
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
                  "      "+ "Menumate is trying to enable SiHot and then sale would be processed" +"\n");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
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
void TManagerMews::Initialise()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TCPIPAddress                = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress); // Ip Address
    ExpensesAccount             = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSExpensesAccount); // CLient Token
    RevenueCentre               = TManagerVariable::Instance().GetStr(DBTransaction,vmRevenueCentre);  // Access Token
	DefaultTransactionAccount   = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultAccount); // Outlet
//	DefaultPaymentCategory      = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPaymentCategory);
    PointsCategory              = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPointsCategory);  // Points
    PointsCategory              = CheckMewsCategoryExists(PointsCategory);
	DefaultSurchargeAccount     = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultSurchargeAccount); // Surcharge
    DefaultSurchargeAccount     = CheckMewsCategoryExists(DefaultSurchargeAccount);
    TipAccount                  = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSTipAccount); // Tip Account
    TipAccount                  = CheckMewsCategoryExists(TipAccount);
    ServiceChargeAccount        = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSServiceChargeAccount); // ServiceCharge
    ServiceChargeAccount        = CheckMewsCategoryExists(ServiceChargeAccount);
    CreditCategory              = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSCreditCategory); // Credit
    CreditCategory              = CheckMewsCategoryExists(CreditCategory);
    UnsetPostingFlag();
    Enabled                     = false;
    AnsiString errorMessage     = "";
    if(Registered && TCPIPAddress.Trim() != "" && TCPIPAddress.Trim() != 0)
        if(ExpensesAccount.Trim() != "" && ExpensesAccount.Trim() != 0)
            if(RevenueCentre.Trim() != "" && RevenueCentre.Trim() != 0)
                if(DefaultTransactionAccount.Trim() != "" && DefaultTransactionAccount.Trim() != 0)
                    if(PointsCategory.Trim() != "" && PointsCategory.Trim() != 0)
                        if(DefaultSurchargeAccount.Trim() != "" && DefaultSurchargeAccount.Trim() != 0)
                            if(ServiceChargeAccount.Trim() != "" && ServiceChargeAccount.Trim() != 0)
                                if(CreditCategory.Trim() != "" && CreditCategory.Trim() != 0)
                                    if(TGlobalSettings::Instance().OracleInterfaceIPAddress.Trim() != "" && TGlobalSettings::Instance().OracleInterfaceIPAddress.Trim() != 0)
                                        if(TipAccount.Trim() != "")
                                        {
                                            Enabled = GetSpaces(TCPIPAddress, ExpensesAccount, RevenueCentre,DBTransaction);
                                            if(!Enabled)
                                                errorMessage = "Mews interface is not enabled as Menumate could not communicate with Mews.";
//                                            Enabled = true;
                                        }
                                        else
                                            errorMessage = "Tip Account selection is Required for Mews Integration.\rPlease provide Tip Account.";
                                    else
                                        errorMessage = "Service selection is Required for Mews Integration.\rPlease provide Service.";
                                else
                                    errorMessage = "Credit Account selection is Required for Mews Integration.\rPlease provide Credit Account.";
                            else
                                errorMessage = "Service Charge selection is Required for Mews Integration.\rPlease provide Service Charge.";
                        else
                            errorMessage = "Surcharge selection is Required for Mews Integration.\rPlease provide Surcharge.";
                    else
                        errorMessage = "Points Account selection is Required for Mews Integration.\rPlease provide Points Account.";
                else
                    errorMessage = "Outlet selection is Required for Mews Integration.\rPlease provide Outlet.";
            else
                errorMessage = "Access Token is Required for Mews Integration.\rPlease provide Access Token.";
        else
            errorMessage = "Client Token is Required for Mews Integration.\rPlease provide Client Token.";
	else
		Enabled = false;
    if(Registered && TCPIPAddress.Trim() != "" && !Enabled)
        MessageBox(errorMessage,"Error",MB_OK+MB_ICONERROR);
    UpdateMewsLogs(Enabled);
    DBTransaction.Commit();
}
bool TManagerMews::SetUpMews(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    try
    {
        retValue     = GetOutlets(url,clientToken,accessToken,DBTransaction);
        if(retValue)
            retValue = GetServices(url,clientToken,accessToken,DBTransaction);
        if(retValue)
            retValue = GetSpaces(url,clientToken,accessToken,DBTransaction);
        if(retValue)
            retValue = GetCategories(url,clientToken,accessToken,DBTransaction);
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return retValue;
}
bool TManagerMews::GetOutlets(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    TMMProcessingState StateOutlets(Screen->ActiveForm, "Getting Outlets. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateOutlets);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TOutlet> outlets = siHotInterface->GetOutlets(url,clientToken, accessToken);
        if(outlets.size() > 0)
        {
            std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
            mewsDataProcessor->UpdateOutlets(outlets,DBTransaction);
            retValue = true;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }

    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
bool TManagerMews::GetServices(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    TMMProcessingState StateServices(Screen->ActiveForm, "Getting Services. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateServices);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TServiceMews> services = siHotInterface->GetServices(url,clientToken,accessToken);
        if(services.size() > 0)
        {
            std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
            mewsDataProcessor->UpdateServices(services,DBTransaction);
            retValue = true;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
bool TManagerMews::GetSpaces(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    TMMProcessingState StateSpaces(Screen->ActiveForm, "Getting Spaces. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateSpaces);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        TSpaceDetails spaces = siHotInterface->GetSpaces(url,clientToken,accessToken);
        if(spaces.Spaces.size() > 0)
        {
            std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
            mewsDataProcessor->UpdateSpaces(spaces,DBTransaction);
            retValue = true;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
bool TManagerMews::GetCategories(UnicodeString url, UnicodeString clientToken, UnicodeString accessToken,Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    TMMProcessingState StateCategories(Screen->ActiveForm, "Getting Categories. Please Wait...", "PMS Set Up");
    TDeviceRealTerminal::Instance().ProcessingController.Push(StateCategories);
    try
    {
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        std::vector<TAccountingCategory> categories = siHotInterface->GetMewsAccountingCategories(url,clientToken,accessToken);
        if(categories.size() > 0)
        {
            std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
            mewsDataProcessor->UpdateCategories(categories,DBTransaction);
            retValue = true;
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
bool TManagerMews::ExportData(TPaymentTransaction &_paymentTransaction, int StaffID)
{
     WaitOrProceedWithPost();
    if((_paymentTransaction.Money.PaymentAmount == 0) || _paymentTransaction.Orders->Count == 0)
    {
        UnsetPostingFlag();
        return true;
    }
    std::auto_ptr<TMewsDataProcessor> processor(new TMewsDataProcessor());
    double tip = 0;
    double tipEftPOS = 0;
    bool isSuccessful = false;
    bool isPostedRoomPost = false;
    bool hasRoomPost      = false;
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
        tip += tipEftPOS;
        double totalPayTendered = 0;
        double tipAux = 0;
        if(tip >= 0.0)
            tipAux = tip;
        double roundedPaymentAmount = (double)_paymentTransaction.Money.PaymentAmount -             //payment->GetPayTotal()
                                      (double)_paymentTransaction.Money.PaymentSurcharges
                                      + tipAux;
        double tipPortion = 0;
        double portion = 1;
        bool splitCase = false;
        TOrder mewsOrderBill;
        mewsOrderBill.ClientToken = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
        mewsOrderBill.AccessToken = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;;
        mewsOrderBill.Bills.clear();
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            tipPortion = RoundTo(tip * portion,-2);
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            double amount = (double)payment->GetPayTendered() - (double)payment->GetSurcharge();
            if((amount != 0)
                  && !payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
                if(payment->GetPaymentAttribute(ePayTypeRoomInterface))
                {
                    roundedPaymentAmount += (double)payment->GetPayRounding();
                    portion = (double)amount/roundedPaymentAmount ;
                    //tip += tipEftPOS;
                    tipPortion = RoundTo(tip * portion,-2);
                    if(portion != 1)
                    {
                        splitCase = true;
                        break;
                    }
                }
            }
        }
        std::auto_ptr<TMewsInterface> mewsInterface(new TMewsInterface());
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            double amount = (double)payment->GetPayTendered() - (double)payment->GetSurcharge();
            if((amount != 0)
                  && !payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
                TOrder mewsOrder;
                mewsOrder.ClientToken = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
                mewsOrder.AccessToken = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;;
                if(payment->GetPaymentAttribute(ePayTypeRoomInterface))
                {
                    mewsOrder.CustomerId  = _paymentTransaction.Phoenix.AccountNumber;
                    hasRoomPost      = true;
                    mewsOrder.ServiceId   = TGlobalSettings::Instance().OracleInterfaceIPAddress;
                    mewsOrder.ConsumptionUtc = Now();
                    mewsOrder.Items.clear();
                    GetDetailsForMewsOrderBill(_paymentTransaction, portion, i,tipPortion,mewsOrder,false);
                    UnicodeString value = "";
                    UnicodeString auxMessage = "";
                    value = mewsInterface->PostMewsOrder(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,mewsOrder);
                    if(value.Trim() == "Successful")
                        isSuccessful = true;
                    else if(value.Trim() != "Successful" && value.Trim() != "")
                    {
                        auxMessage = "Posting to Mews failed.\r";
                        auxMessage += value;
                        if(value.Pos("403") == 0)
                        {
                            TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
                            auxMessage += "\rMews interface is disabled now.";
                        }
                        MessageBox(auxMessage,"Info",MB_OK+MB_ICONINFORMATION);
                        isPostedRoomPost = isSuccessful;
                        break;
                    }
                    else
                    {
                        auxMessage = "Posting to Mews failed.\r";
                        if(value.Trim() != "")
                        {
                            auxMessage += value;
                            if(value.Pos("403") == 0)
                            {
                                TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
                                auxMessage += "\rMews interface is disabled now.";
                            }
                        }
                        else
                        {
                            auxMessage += "Menumate could not communicate with Mews.";
                            auxMessage += "\rMews interface is disabled now.";
                            TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
                        }
                        MessageBox(auxMessage,"Info",MB_OK+MB_ICONINFORMATION);
                        isSuccessful = false;
                    }
                    isPostedRoomPost = isSuccessful;
                }
                else
                {
                    if(mewsOrderBill.Bills.size() == 0)
                    {
                        double portionForBill = 1;
                        double tipPortionForBill = 0;
                        tipPortionForBill = tip;
                        if(splitCase)
                        {
                            if(portion != 0 && portion != 1)
                                portionForBill = 1-portion;
                            if(tipPortion != 0)
                                tipPortionForBill = RoundTo(tip * portionForBill,-2);
                        }
                        GetDetailsForMewsOrderBill(_paymentTransaction, portionForBill, i,tipPortionForBill,mewsOrderBill,true);
                    }
                    if(payment->GetPaymentAttribute(ePayTypeCredit) && (double)payment->GetPayTendered() < 0.0)
                       continue;
                    TItemMews itemMews;
                    itemMews.Type = "Payment";
                    if(!payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
                        itemMews.Name = payment->Name;
                    else
                        itemMews.Name = payment->CardType;
                    if(_paymentTransaction.CreditTransaction)
                        itemMews.UnitCount = -1;
                    else
                        itemMews.UnitCount = 1;
                    double paymentAmount = ((double)(payment->GetPayTendered() + payment->GetCashOut() - payment->GetChange()));
                    itemMews.UnitCost.Amount = fabs(RoundTo(paymentAmount,-2));
                    itemMews.UnitCost.Currency = CurrencyString;
                    itemMews.UnitCost.Tax = 0;
                    itemMews.Category.Name = "";
                    if(payment->GetPaymentAttribute(ePayTypePoints))
                    {
//                        std::auto_ptr<TMewsDataProcessor> mewsProcessor(new TMewsDataProcessor());
                        itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
//                        mewsProcessor->GetMewsName(_paymentTransaction.DBTransaction,
//                                                                            TDeviceRealTerminal::Instance().BasePMS->PointsCategory,3);
                    }
                    else if(payment->GetPaymentAttribute(ePayTypeCredit))
                    {
//                        std::auto_ptr<TMewsDataProcessor> mewsProcessor(new TMewsDataProcessor());
                        itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
//                        mewsProcessor->GetMewsName(_paymentTransaction.DBTransaction,
//                                                                            TDeviceRealTerminal::Instance().BasePMS->CreditCategory,3);
                    }
                    else
                    {
                        if(!payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
                            itemMews.Category.Name = GetMewsCategoryCodeForItem(NULL,true,payment->Name);
                        else
                        {
                            itemMews.Category.Name = GetMewsCategoryCodeForItem(NULL,false,payment->CardType);
                            if(itemMews.Category.Name.Trim() == "")
                            {
                                UnicodeString defaultCode = GetMewsCategoryCodeForItem(NULL,true,payment->CardType);
                                AddPaymentToPMSPaymentTypes(payment,defaultCode);
                                itemMews.Category.Name = defaultCode;
                            }
                        }
                    }
                    mewsOrderBill.Bills[0].Items.push_back(itemMews);
                    double surcharge = 0;
                    if(_paymentTransaction.CreditTransaction)
                        surcharge = payment->GetDiscount();
                    else
                        surcharge = payment->GetSurcharge();
                    if(surcharge != 0)
                    {
                        TItemMews itemMews;
                        itemMews.Type = "Revenue";
                        if(payment->GetPaymentAttribute(ePayTypePoints))
                            itemMews.Name = "Points Purchase";
                        else
                            itemMews.Name = "Surcharge";
                        if(_paymentTransaction.CreditTransaction)
                            itemMews.UnitCount = -1;
                        else
                            itemMews.UnitCount = 1;
                        itemMews.UnitCost.Amount = fabs(RoundTo(surcharge,-2));
                        itemMews.UnitCost.Currency = CurrencyString;
                        itemMews.UnitCost.Tax = 0;
                        //itemMews.Category.Code = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
//                        std::auto_ptr<TMewsDataProcessor> mewsProcessor(new TMewsDataProcessor());
                        itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
//                        mewsProcessor->GetMewsName(_paymentTransaction.DBTransaction,
//                                                                            TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount,3);
                        mewsOrderBill.Bills[0].Items.push_back(itemMews);
                    }
                }
            }
         }
         if(mewsOrderBill.Bills.size() > 0)
         {
            UnicodeString value = "";
            UnicodeString auxMessage = "";
            if((hasRoomPost && isPostedRoomPost) || (!hasRoomPost))
            {
                value = mewsInterface->PostMewsBill(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,mewsOrderBill);
                if(value.Trim() == "Successful")
                    isSuccessful = true;
                else if(value.Trim() != "Successful" && value.Trim() != "")
                {
                    auxMessage = "Posting to Mews failed.\r";
                    auxMessage += value;
                    if(value.Pos("403") == 0)
                    {
                        TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
                        auxMessage += "\rMews interface is disabled now.";
                    }
                    MessageBox(auxMessage,"Info",MB_OK+MB_ICONINFORMATION);
                    isSuccessful = false;
                }
                else
                {
                    auxMessage = "Posting to Mews failed.\r";
                    if(value.Trim() != "")
                    {
                        auxMessage += value;
                        if(value.Pos("403") == 0)
                        {
                            TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
                            auxMessage += "\rMews interface is disabled now.";
                        }
                    }
                    else
                    {
                        auxMessage += "Menumate could not communicate with Mews.";
                        auxMessage += "\rMews interface is disabled now.";
                        TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
                    }
                    MessageBox(auxMessage,"Info",MB_OK+MB_ICONINFORMATION);
                    isSuccessful = false;
                }
            }
         }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
//        DBTransaction.Rollback();
    }
    UnsetPostingFlag();
//    if(!isSuccessful)
//       TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
    return isSuccessful;
}
void TManagerMews::GetMewsCustomer(UnicodeString queryString, std::vector<TCustomerMews> &customerMews,bool isSpace)
{
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Getting Customer Details , Please Wait...";
    Processing->Show();
    try
    {
        customerMews.clear();
        std::auto_ptr<TMewsDataProcessor> mewsDataProcessor(new TMewsDataProcessor());
        TCustomerSearch customerSearch = mewsDataProcessor->CreateInquiryForRoom(queryString,isSpace);
        customerSearch.AccessToken = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        customerSearch.ClientToken = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
        std::auto_ptr<TMewsInterface> siHotInterface(new TMewsInterface());
        customerMews = siHotInterface->GetCustomers(TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,customerSearch);
    }
    catch(Exception &ex)
    {
        //MessageBox(ex.Message,"Exception",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    Processing->Close();
}
//private
AnsiString TManagerMews::GetLogFileName()
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "Menumate Services";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
//    directoryName = directoryName + "\\logs";
//    if (!DirectoryExists(directoryName))
//        CreateDir(directoryName);
    directoryName = directoryName + "\\Mews Post Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "MewsPosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "\\" + name;
    return fileName;
}
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
void TManagerMews::GetDetailsForMewsOrder(TPaymentTransaction &paymentTransaction, double portion, int paymentIndex,
                                            double tipPortion,TOrder &mewsOrder,bool isBill)
{
    try
    {
        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete* itemComplete = ((TItemComplete*)paymentTransaction.Orders->Items[i]);
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = itemComplete->Item;
            int qtyItem = 0;
            double qtyItemD = 0;
            double varianceAdditive = 0;
            qtyItemD = RoundTo((double)itemComplete->GetQty() * portion, -2);
            CalculateQtyAndvariance(qtyItemD, qtyItem, varianceAdditive);
            itemMews.UnitCount = qtyItem;
            bool seperateDiscount = false;
            double discountValue = 0;
            itemMews.UnitCost = GetUnitCost(itemComplete, portion,seperateDiscount,discountValue);
            varianceAdditive = varianceAdditive/qtyItem;
            itemMews.UnitCost.Amount = RoundTo((itemMews.UnitCost.Amount + (varianceAdditive*itemMews.UnitCost.Amount))/portion,-2);
            itemMews.Category.Name = "";
            itemMews.Category.Name = GetMewsCategoryCodeForItem(itemComplete,false);
            mewsOrder.Items.push_back(itemMews);
        }
        if(paymentTransaction.Money.ServiceCharge != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Service Charge";
            if(paymentTransaction.CreditTransaction)
                itemMews.UnitCount = -1;
            else
                itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = fabs(RoundTo((double)paymentTransaction.Money.ServiceCharge * portion,-2));
            itemMews.UnitCost.Currency = CurrencyString;
            if(TGlobalSettings::Instance().ApplyServiceChargeTax)
                itemMews.UnitCost.Tax = TGlobalSettings::Instance().ServiceChargeTaxRate;
            else
                itemMews.UnitCost.Tax = 0;
            itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
            // get code for items
            mewsOrder.Items.push_back(itemMews);
        }
        if(tipPortion != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Tips";
            if(paymentTransaction.CreditTransaction)
                itemMews.UnitCount = -1;
            else
                itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = fabs(RoundTo(tipPortion,-2));
            itemMews.UnitCost.Currency = CurrencyString;
            itemMews.UnitCost.Tax = 0;
            itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
            // get code for items
            mewsOrder.Items.push_back(itemMews);
        }
        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
        double surcharge = 0;
        if(paymentTransaction.CreditTransaction)
            surcharge = payment->GetDiscount();
        else
            surcharge = payment->GetSurcharge();
        if(surcharge != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Surcharge";
            if(paymentTransaction.CreditTransaction)
                itemMews.UnitCount = -1;
            else
                itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = fabs(RoundTo(surcharge,-2));
            itemMews.UnitCost.Currency = CurrencyString;
            itemMews.UnitCost.Tax = 0;
            itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
            // get code for items
            mewsOrder.Items.push_back(itemMews);
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
void TManagerMews::GetDetailsForMewsOrderBill(TPaymentTransaction &paymentTransaction, double portion, int paymentIndex,
                                            double tipPortion,TOrder &mewsOrder,bool isBill)
{
    std::vector<TBill> billList;
    billList.clear();
    try
    {
        TBill billMews;
        billMews.OutletId = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        billMews.Number   = GetInvoiceNumber(paymentTransaction);// Invoice Number
        billMews.Items.clear();
        mewsOrder.ConsumptionUtc = "";
        if(!isBill)
        {
            mewsOrder.ConsumptionUtc = GetInvoiceNumber(paymentTransaction);
        }

        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);

        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete* itemComplete = ((TItemComplete*)paymentTransaction.Orders->Items[i]);
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = itemComplete->Item + " (" + itemComplete->Size + ")";
            int qtyItem = 0;
            double qtyItemD = 0;
            double varianceAdditive = 0;
            qtyItemD = RoundTo((double)itemComplete->GetQty() * portion, -2);
            CalculateQtyAndvariance(qtyItemD, qtyItem, varianceAdditive);
            itemMews.UnitCount = qtyItem;
            bool seperateDiscount = false;
            double discountValue = 0;
            itemMews.UnitCost = GetUnitCost(itemComplete, portion,seperateDiscount,discountValue);
            varianceAdditive = varianceAdditive/qtyItem;
            itemMews.UnitCost.Amount = RoundTo((itemMews.UnitCost.Amount + (varianceAdditive*itemMews.UnitCost.Amount))/portion,-2);
            itemMews.Category.Name = "";
            itemMews.Category.Name = GetMewsCategoryCodeForItem(itemComplete,false);
            UnicodeString itemCatCode = itemMews.Category.Name;
            if(itemMews.UnitCost.Amount != 0)
            {
                if(isBill)
                    billMews.Items.push_back(itemMews);
                else
                    mewsOrder.Items.push_back(itemMews);
            }
            if(seperateDiscount)
            {
                TItemComplete* itemCompleteDiscount = ((TItemComplete*)paymentTransaction.Orders->Items[i]);
                TItemMews itemMewsDiscount;
                itemMewsDiscount.Type = "Revenue";
                itemMewsDiscount.Name = itemCompleteDiscount->Item;
                itemMewsDiscount.UnitCount = -1*1;
                itemMewsDiscount.UnitCost.Amount = RoundTo((discountValue + (varianceAdditive*discountValue))/portion,-2);
                itemMewsDiscount.UnitCost.Currency = CurrencyString;
                itemMewsDiscount.UnitCost.Tax = 0;
                itemMewsDiscount.Category.Name = "";
                itemMewsDiscount.Category.Name = itemCatCode;
                if(itemMewsDiscount.UnitCost.Amount != 0)
                {
                    if(isBill)
                        billMews.Items.push_back(itemMewsDiscount);
                    else
                        mewsOrder.Items.push_back(itemMewsDiscount);
                }
            }
            for(int j = 0; j < itemComplete->SubOrders->Count; j++)
            {
                TItemComplete* itemCompleteSub = ((TItemComplete*)itemComplete->SubOrders->Items[j]);
                TItemMews itemMewsSub;
                itemMewsSub.Type = "Revenue";
                itemMewsSub.Name = itemCompleteSub->Item;


                int qtyItemSub = 0;
                double qtyItemDSub = 0;
                double varianceAdditiveSub = 0;
                qtyItemDSub = RoundTo((double)itemCompleteSub->GetQty() * portion, -2);
                CalculateQtyAndvariance(qtyItemDSub, qtyItemSub, varianceAdditiveSub);
                itemMewsSub.UnitCount = qtyItemSub;
                bool seperateDiscountSub = false;
                double discountValueSub = 0;
                itemMewsSub.UnitCost = GetUnitCost(itemCompleteSub, portion,seperateDiscountSub,discountValueSub);
                varianceAdditiveSub = varianceAdditiveSub/qtyItemSub;
                itemMewsSub.UnitCost.Amount = RoundTo((itemMewsSub.UnitCost.Amount + (varianceAdditive*itemMewsSub.UnitCost.Amount))/portion,-2);
                itemMewsSub.Category.Name = "";
                itemMewsSub.Category.Name = GetMewsCategoryCodeForItem(itemCompleteSub,false);
                UnicodeString itemCatCode = itemMewsSub.Category.Name;
                if(itemMewsSub.UnitCost.Amount != 0)
                {
                    if(isBill)
                        billMews.Items.push_back(itemMewsSub);
                    else
                        mewsOrder.Items.push_back(itemMewsSub);
                }
                if(seperateDiscountSub)
                {
                    TItemComplete* itemCompleteDiscountSub = ((TItemComplete*)itemComplete->SubOrders->Items[j]);
                    TItemMews itemMewsDiscountSub;
                    itemMewsDiscountSub.Type = "Revenue";
                    itemMewsDiscountSub.Name = itemCompleteDiscountSub->Item;
                    itemMewsDiscountSub.UnitCount = -1*1;
                    itemMewsDiscountSub.UnitCost.Amount = RoundTo((discountValueSub + (varianceAdditiveSub*discountValueSub))/portion,-2);
                    itemMewsDiscountSub.UnitCost.Currency = CurrencyString;
                    itemMewsDiscountSub.UnitCost.Tax = 0;
                    itemMewsDiscountSub.Category.Name = "";
                    itemMewsDiscountSub.Category.Name = itemCatCode;
                    if(itemMewsDiscountSub.UnitCost.Amount != 0)
                    {
                        if(isBill)
                            billMews.Items.push_back(itemMewsDiscountSub);
                        else
                            mewsOrder.Items.push_back(itemMewsDiscountSub);
                    }
                }
            }
        }
        if(paymentTransaction.Money.ServiceCharge != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Service Charge";
            if(paymentTransaction.CreditTransaction)
                itemMews.UnitCount = -1;
            else
                itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = fabs(RoundTo((double)paymentTransaction.Money.ServiceCharge * portion,-2) +
                                            RoundTo((double)paymentTransaction.Money.ServiceChargeTax * portion,-2));
            itemMews.UnitCost.Currency = CurrencyString;
            if(TGlobalSettings::Instance().ApplyServiceChargeTax)
                itemMews.UnitCost.Tax = TGlobalSettings::Instance().ServiceChargeTaxRate/100;
            else
                itemMews.UnitCost.Tax = 0;
//            std::auto_ptr<TMewsDataProcessor> mewsProcessor(new TMewsDataProcessor());
            itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
//            mewsProcessor->GetMewsName(paymentTransaction.DBTransaction,
//                                                                TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount,3);
            //TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
            if(isBill)
                billMews.Items.push_back(itemMews);
            else
                mewsOrder.Items.push_back(itemMews);
        }
        if(tipPortion != 0)
        {
            TItemMews itemMews;
            itemMews.Type = "Revenue";
            itemMews.Name = "Tips";
            if(paymentTransaction.CreditTransaction)
                itemMews.UnitCount = -1;
            else
                itemMews.UnitCount = 1;
            itemMews.UnitCost.Amount = fabs(RoundTo(tipPortion,-2));
            itemMews.UnitCost.Currency = CurrencyString;
            itemMews.UnitCost.Tax = 0;
            //itemMews.Category.Code = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
//            std::auto_ptr<TMewsDataProcessor> mewsProcessor(new TMewsDataProcessor());
            itemMews.Category.Name = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
//            mewsProcessor->GetMewsName(paymentTransaction.DBTransaction,
//                                                                TDeviceRealTerminal::Instance().BasePMS->TipAccount,3);
            if(isBill)
                billMews.Items.push_back(itemMews);
            else
                mewsOrder.Items.push_back(itemMews);
        }
        if(isBill)
        {
            billList.push_back(billMews);
            mewsOrder.Bills = billList;
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        //MessageBox(E.Message,"",MB_OK);
	}

}
TUnitCost TManagerMews::GetUnitCost(TItemComplete* itemComplete, double portion,bool &seperateDiscount,double &discountValue)
{
    TUnitCost unitCost;
    try
    {
        double unitAmount = fabs((double)itemComplete->BillCalcResult.FinalPrice);
        unitAmount = unitAmount * portion;
        unitAmount -= fabs((double)itemComplete->BillCalcResult.ServiceCharge.Value * portion);
        unitAmount -= fabs((double)itemComplete->BillCalcResult.ServiceCharge.TaxValue * portion);
        if(TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount)
        {
            unitAmount += fabs((double)itemComplete->BillCalcResult.TotalDiscount);
            discountValue = fabs((double)itemComplete->BillCalcResult.TotalDiscount);
            discountValue = discountValue * portion;
            if(fabs((double)itemComplete->BillCalcResult.TotalDiscount) > 0)
                seperateDiscount = true;
        }
        unitAmount = unitAmount / fabs((double)itemComplete->GetQty());
        unitCost.Amount = unitAmount;
        unitCost.Currency = CurrencyString;
        unitCost.Tax = 0;
        for(int i = 0; i < itemComplete->BillCalcResult.Tax.size();i++)
        {
            if(itemComplete->BillCalcResult.Tax[i].TaxType == 0)
                unitCost.Tax += fabs((double)itemComplete->BillCalcResult.Tax[i].Percentage);
        }
        unitCost.Tax = unitCost.Tax/100;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return unitCost;
}
UnicodeString TManagerMews::GetMewsCategoryCodeForItem(TItemComplete *itemComplete,bool isDefaultRequired,UnicodeString name)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TAccountingCategoriesMapping> mewsAccountingCategoriesList;
    mewsAccountingCategoriesList.clear();
    if(name.Trim().Length() == 0)
    {
        std::auto_ptr<TMewsDataProcessor> processor (new TMewsDataProcessor());
        mewsAccountingCategoriesList = processor->GetMewsCategoriesList(DBTransaction);
    }
    UnicodeString code = "";
    try
    {
        if(name.Trim().Length() == 0)
        {
            IBInternalQuery->SQL->Text = "SELECT CATEGORY_KEY FROM ITEMSIZE WHERE ITEM_KEY = :ITEM_KEY "
                                        "AND SIZE_NAME = :SIZE_NAME";
            IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemComplete->ItemKey;
            IBInternalQuery->ParamByName("SIZE_NAME")->AsString = itemComplete->Size;
            IBInternalQuery->ExecQuery();
            int categoryKey =  IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
            std::vector<TAccountingCategoriesMapping> ::iterator it = mewsAccountingCategoriesList.begin();
            for(;it != mewsAccountingCategoriesList.end();advance(it,1))
            {
                if(it->CategoryKey == categoryKey)
                {
                    code = it->MewsCategoryCode;
                    break;
                }
            }
        }
        else
        {
            IBInternalQuery->SQL->Text = "SELECT * FROM PMSPAYMENTSCONFIG WHERE PMS_PAYTYPE_NAME = :PMS_PAYTYPE_NAME ";
            IBInternalQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString = name;
            IBInternalQuery->ExecQuery();
            if(IBInternalQuery->RecordCount > 0)
            {
                if(IBInternalQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString != NULL)
                    code =  IBInternalQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString;
            }
            if(isDefaultRequired)
            {
                if(code.Trim() == "")
                {
                    IBInternalQuery->Close();
                    IBInternalQuery->SQL->Text = "SELECT * FROM PMSPAYMENTSCONFIG WHERE PMS_PAYTYPE_CATEGORY = 0 ";
                    IBInternalQuery->ExecQuery();
                    if(IBInternalQuery->RecordCount > 0)
                    {
                        if(IBInternalQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString != NULL)
                            code =  IBInternalQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString;
                    }
                }
            }
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return code;
}
UnicodeString TManagerMews::GetInvoiceNumber(TPaymentTransaction _paymentTransaction)
{
    UnicodeString invoiceNumber = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQueryGenerator->Close();
        bool IsCompDiscountApplied      = false;
        bool IsNCDiscountApplied        = false;
        bool IsNormalDiscountApplied    = false;
        for(int index = 0; index < _paymentTransaction.Orders->Count; index++)
        {
            TItemComplete *itemcomplete = (TItemComplete*)_paymentTransaction.Orders->Items[index];
            for(int discountIndex = 0; discountIndex < itemcomplete->Discounts.size(); discountIndex++)
            {
                if(itemcomplete->Discounts[discountIndex].IsComplimentaryDiscount())
                {
                    IsCompDiscountApplied = true;
                    break;
                }
                if(itemcomplete->Discounts[discountIndex].IsNonChargableDiscount())
                {
                    IsNCDiscountApplied = true;
                    break;
                }
            }
            if(IsCompDiscountApplied || IsNCDiscountApplied)
            {
                break;
            }
        }
        if(IsCompDiscountApplied)
        {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERCOMP, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
            invoiceNumber = "Comp " + IntToStr(number);
        }
        else if(IsNCDiscountApplied)
        {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERNC, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
            invoiceNumber = "NC "+ IntToStr(number);
        }
        else
        {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
            invoiceNumber = IntToStr(number);
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
    return invoiceNumber;
}
void TManagerMews::CalculateQtyAndvariance(double &qtyItemD, int &qtyItem, double &varianceAdditive)
{
    if(qtyItemD >= 0)
    {
        double qtyAuxD = qtyItemD+ 0.5;  //2.8,3
        qtyItem = (int)qtyAuxD;
        if(qtyItem != 0)
        {
            varianceAdditive = qtyItemD - qtyItem;
        }
        else
        {
            qtyItem += 1;
            varianceAdditive = qtyItemD - qtyItem;
        }
    }
    else
    {
        double qtyAuxD = qtyItemD - 0.5;  //2.8,3
        qtyItem = (int)qtyAuxD;
        if(qtyItem != 0)
        {
            varianceAdditive = -(qtyItemD - qtyItem);
        }
        else
        {
            qtyItem -= 1;
            varianceAdditive = -(qtyItemD - qtyItem);
        }
    }
}
UnicodeString TManagerMews::CheckMewsCategoryExists(UnicodeString category)
{
    UnicodeString retValue = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM REVENUECODEDETAILS WHERE REVENUECODE_DESCRIPTION = :REVENUECODE_DESCRIPTION";
        IBInternalQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = category;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount > 0)
        {
            retValue = category;
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
       DBTransaction.Rollback();
    }
    return retValue;
}
void TManagerMews::AddPaymentToPMSPaymentTypes(TPayment *payment,AnsiString defaultCode)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery= DBTransaction.Query(DBTransaction.AddQuery());
        InsertQuery->Close();
        InsertQuery->SQL->Text = " INSERT INTO PMSPAYMENTSCONFIG "
                 " (PMS_PAYTYPE_ID, PMS_PAYTYPE_NAME, PMS_PAYTYPE_CODE,"
                 " PMS_PAYTYPE_CATEGORY ,IS_ELECTRONICPAYMENT) VALUES"
                 " (:PMS_PAYTYPE_ID, :PMS_PAYTYPE_NAME, :PMS_PAYTYPE_CODE,"
                 " :PMS_PAYTYPE_CATEGORY ,:IS_ELECTRONICPAYMENT)";
        InsertQuery->ParamByName("PMS_PAYTYPE_ID")->AsInteger =
                                 TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_PMSPAYTYPEID");
        InsertQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString = payment->CardType;
        InsertQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString = defaultCode;
        InsertQuery->ParamByName("PMS_PAYTYPE_CATEGORY")->AsInteger = 2;
        InsertQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "T";
        InsertQuery->ExecQuery();
        DBTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//----------------------------------------------//
