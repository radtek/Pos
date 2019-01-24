//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerOraclePMS.h"
#include "MMMessageBox.h"
#include "OracleManagerDB.h"
#include "OracleTCPIP.h"
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include "PMSHelper.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TManagerOraclePMS::TManagerOraclePMS() : TBasePMS()
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
TManagerOraclePMS::~TManagerOraclePMS()
{
}
//---------------------------------------------------------------------------
void TManagerOraclePMS::Initialise()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        MakeOracleSeedFile();
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
        RevenueCentre = TManagerVariable::Instance().GetStr(DBTransaction,vmRevenueCentre);
        LoadMeals(DBTransaction);
        TOracleTCPIP::Instance().UnsetPostingFlag();
        if(Registered && TCPIPAddress != "")
        {
            std::auto_ptr<TPMSHelper> pmsHelper(new TPMSHelper());
            //pmsHelper->LoadPMSPaymentTypes(PMSPaymentTypeMap);
            if(pmsHelper->LoadRevenueCodes(RevenueCodesMap, DBTransaction))
            {
                if(RevenueCodesMap.size() < 17)
                {
                    if(Slots.size() > 0)
                    {
                        if(DefaultPaymentCategory.Trim() != "" && DefaultPaymentCategory != NULL)
                        {
                            if(PointsCategory.Trim() != "" && PointsCategory != NULL)
                            {
                                if(CreditCategory.Trim() != "" && CreditCategory != NULL)
                                {
                                    if(TGlobalSettings::Instance().IsOraclePOSServer)
                                    {
                                       if(TGlobalSettings::Instance().OracleInterfacePortNumber != 0 && TGlobalSettings::Instance().OracleInterfaceIPAddress.Trim() != "")
                                       {
                                            if(TriggerApplication())
                                            {
                                                Enabled = GetLinkStatus();
                                            }
                                       }
                                       else
                                       {
                                            MessageBox("Oracle Interface IP Address and Oracle Port Number are must","Information",MB_OK);
                                            Enabled = false;
                                       }
                                    }
                                    else
                                    {
                                        FindAndTerminateProcess();
                                        Sleep(1000);
                                        if(InitializeoracleTCP())
                                        {
                                           Enabled = true;
                                           TOracleTCPIP::Instance().Disconnect();
                                        }
                                        else
                                        {
                                            Enabled = false;
                                        }
                                    }
                                }
                                else
                                {
                                    MessageBox("Credit Category is incorrect.\nIt is required for set up of Oracle PMS.", "Warning", MB_OK + MB_ICONINFORMATION);
                                    Enabled = false;
                                }
                            }
                            else
                            {
                                MessageBox("Points Category is incorrect.\nIt is required for set up of Oracle PMS.", "Warning", MB_OK + MB_ICONINFORMATION);
                                Enabled = false;
                            }
                        }
                        else
                        {
                            MessageBox("Default Payment Category is incorrect.\nIt is required for set up of Oracle PMS.", "Warning", MB_OK + MB_ICONINFORMATION);
                            Enabled = false;
                        }
                    }
                    else
                    {
                        MessageBox("Serving Times are required for set up of Oracle.", "Warning", MB_OK + MB_ICONINFORMATION);
                        Enabled = false;
                    }
                }
                else
                {
                    MessageBox("Number of Revenue codes can be 1 to 16 only.","WARNING",MB_ICONWARNING + MB_OK);
                    Enabled = false;
                }
            }
            else
            {
                MessageBox("Revenue codes are required for set up of Oracle.", "Warning", MB_OK + MB_ICONINFORMATION);
                Enabled = false;
            }
        }
        else
        {
            Enabled = false;
        }
        DBTransaction.Commit();
    }
    catch (Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
    }
}

//---------------------------------------------------------------------------
bool TManagerOraclePMS::TriggerApplication()
{
    bool isProcOpen = false;
    try
    {
      // start the program up
        CloseExistingApplication();
        Sleep(1000);
        UnicodeString strPath = ExtractFilePath(Application->ExeName);
        strPath +="OracleTCPServer.exe";
        isProcOpen = CreateProcess( strPath.t_str(),   // the path
        NULL,
        //argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &TGlobalSettings::Instance().siOracleApp,            // Pointer to STARTUPINFO structure
        &TGlobalSettings::Instance().piOracleApp             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        );
        Sleep(2000);
    }
    catch(Exception &E)
    {
        MessageBox(E.Message,"Exception",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        TerminateProcess(TGlobalSettings::Instance().piOracleApp.hProcess,0);
    }
      return isProcOpen;
}
//---------------------------------------------------------------------------
void TManagerOraclePMS::CloseExistingApplication()
{
    //TerminateProcess(TGlobalSettings::Instance().piOracleApp.hProcess,0);
    bool isTerminated = false;
    isTerminated = FindAndTerminateProcess();
    if(isTerminated)
    {
        Sleep(16000);
    }
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::FindAndTerminateProcess()
{
    bool retValue = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, "OracleTCPServer.exe") == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                TerminateProcess(hProcess,0);
                CloseHandle(hProcess);
                retValue = true;
                break;
            }
        }
    }

    CloseHandle(snapshot);
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::InitializeoracleTCP()
{
    TOracleTCPIP::Instance().IsSilentConnect = false;
    bool retValue = TOracleTCPIP::Instance().Connect();
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::GetLinkStatus()
{
    bool retValue = false;
    std::auto_ptr<TOracleDataBuilder> oracledata(new TOracleDataBuilder());
    TLinkDescription linkDescription = oracledata->CreateLinkDescription();
    TiXmlDocument doc = oracledata->CreateLinkDescriptionXML(linkDescription);
    AnsiString data = oracledata->SerializeOut(doc);
    AnsiString resultData = "";
    resultData = TOracleTCPIP::Instance().SendAndFetch(data);
    // deserialize the resposne
    if(resultData.Pos("Connection Failed") == 0)
        retValue = oracledata->DeserializeGetLinkStatus(resultData);
    else
        retValue = false;
    return retValue;
}
//---------------------------------------------------------------------------
void TManagerOraclePMS::LoadMeals(Database::TDBTransaction &DBTransaction)
{
    Slots.clear();
    std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
    TIBSQL* queryMeals = managerDB->LoadMeals(DBTransaction);
    for(;!queryMeals->Eof;queryMeals->Next())
    {
        TTimeSlots slots;
        slots.MealName = queryMeals->FieldByName("MEALIDENTIFIER")->AsInteger;
        slots.StartTime = queryMeals->FieldByName("STARTTIME")->AsTime;
        slots.EndTime   = queryMeals->FieldByName("ENDTIME")->AsDateTime;
        slots.IsDefault = queryMeals->FieldByName("ISDEFAULT_SERVINGTIME")->AsString == "T" ?
                          true : false;
        Slots.push_back(slots);
    }
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::ExportData(TPaymentTransaction &_paymentTransaction,
                                    int StaffId)
{
    AnsiString checkNumber = "";
    if(_paymentTransaction.Orders->Count == 0)
    {
       return true;
    }
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    bool retValue = false;
    try
    {
        std::auto_ptr<TOracleDataBuilder> oracledata(new TOracleDataBuilder());
        TPostRequest postRequest;
        TPostRequestAnswer _postResult;
        std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
        checkNumber = managerDB->GetCheckNumber(DBTransaction);
        DBTransaction.Commit();

        double tip = 0;
        double tipEftPOS = 0;
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
        double roundedPaymentAmount = (double)_paymentTransaction.Money.PaymentAmount -
                                      (double)_paymentTransaction.Money.PaymentSurcharges
                                      + tipAux;
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            double amount = (double)payment->GetPayTendered() - (double)payment->GetSurcharge();
            totalPayTendered += amount;
            double portion = 0;
            double portionOriginal = 0;
            if((amount != 0)
                  && !payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
                roundedPaymentAmount += (double)payment->GetPayRounding();
                portion = (double)amount/roundedPaymentAmount ;
                portionOriginal = portion;
                tip += tipEftPOS;
                double tipPortion = RoundTo(tip * portion,-2);
                postRequest = oracledata->CreatePost(_paymentTransaction,portion, i,tipPortion);
                double surcharge = 0;
                if(_paymentTransaction.CreditTransaction)
                    surcharge = payment->GetDiscount();
                else
                    surcharge = payment->GetSurcharge();
                if(surcharge != 0)
                {
                   surcharge = RoundTo(surcharge * 100 , -2);
                   AnsiString strSurcharge = (AnsiString)surcharge;
                   if(strSurcharge.Pos(".") != 0)
                   {
                      strSurcharge = strSurcharge.SubString(1,strSurcharge.Pos(".")-1);
                   }
                   for(int subtotalindex = 0; subtotalindex < 16; subtotalindex++)
                   {
                      if(postRequest.Subtotal1[subtotalindex] != "" && postRequest.Subtotal1[subtotalindex] != 0)
                      {
                        int paymentSurcharge = atoi(postRequest.ServiceCharge[subtotalindex].c_str());
                        paymentSurcharge += atoi(strSurcharge.c_str());
                        AnsiString str = paymentSurcharge;
                        postRequest.ServiceCharge[subtotalindex] = str;
                        break;
                      }
                   }
                }
                if(_paymentTransaction.Money.TotalRounding  != 0)
                {
                    double totalRounding = (double)_paymentTransaction.Money.TotalRounding;
                    double roundingPortion = totalRounding * portion;
                    roundingPortion = RoundTo(roundingPortion,-2);
                    roundingPortion = roundingPortion * 100;
                    AnsiString roundingPortionStr = (AnsiString)roundingPortion;
                    if(roundingPortionStr.Pos(".") != 0)
                    {
                      roundingPortionStr = roundingPortionStr.SubString(1,roundingPortionStr.Pos(".")-1);
                    }
                    int roundingPortionInt = atoi(roundingPortionStr.c_str());
                    int oldTotal = atoi(postRequest.TotalAmount.c_str());
                    oldTotal += roundingPortionInt;
                    postRequest.TotalAmount = oldTotal;
                    for(int index = 0; index < postRequest.Subtotal1.size(); index++)
                    {
                       if(postRequest.Subtotal1[index].Trim() != "")
                       {
                           int oldSubTotal = atoi(postRequest.Subtotal1[index].c_str());
                           oldSubTotal += roundingPortionInt;
                           postRequest.Subtotal1[index] = oldSubTotal;
                           break;
                       }
                    }
                }
                int totalInt = atoi(postRequest.TotalAmount.c_str());
                double paymentD = RoundToNearest(
                    (double)payment->GetPayTendered() -(double)payment->GetSurcharge(),
                    0.01,TGlobalSettings::Instance().MidPointRoundsDown);
                paymentD = paymentD * 100;
                int paymentInt = paymentD;
                if(paymentInt - totalInt != 0)
                {
                    int adjustmentInt = paymentInt - totalInt;
                    int oldTotal = atoi(postRequest.TotalAmount.c_str());
                    oldTotal += adjustmentInt;
                    postRequest.TotalAmount = oldTotal;
                    for(int index = 0; index < postRequest.Subtotal1.size(); index++)
                    {
                       if(postRequest.Subtotal1[index].Trim() != "")
                       {
                           int oldSubTotal = atoi(postRequest.Subtotal1[index].c_str());
                           oldSubTotal += adjustmentInt;
                           postRequest.Subtotal1[index] = oldSubTotal;
                           break;
                       }
                    }
                }
                //Removing values present after decimal point
                int finalTotal = atoi(postRequest.TotalAmount.c_str());
                postRequest.TotalAmount = finalTotal;
                postRequest.CheckNumber = checkNumber;
                TiXmlDocument doc = oracledata->CreatePostXML(postRequest);
                AnsiString resultData = "";
                AnsiString data = oracledata->SerializeOut(doc);
                resultData = TOracleTCPIP::Instance().SendAndFetch(data);
                if(resultData.Pos("Connection Failed") == 0)
                {
                    retValue = oracledata->DeserializeData(resultData, _postResult);
                }
                else
                {
                    retValue = false;
                }
            }
         }
         bool isNotCompleteCancel = false;
         bool hasCancelledItems = false;
         for(int itemNumber = 0; itemNumber < _paymentTransaction.Orders->Count; itemNumber++)
         {
            TItemComplete *itemThis = (TItemComplete*)_paymentTransaction.Orders->Items[itemNumber];
            if(itemThis->OrderType != CanceledOrder)
            {
               isNotCompleteCancel = true;
               hasCancelledItems = false;
               break;
            }
            else if(itemThis->OrderType == CanceledOrder)
            {
                hasCancelledItems = true;
            }
         }
         if(totalPayTendered == 0 && isNotCompleteCancel) // case for cancelled,100% discount
         {
            double portion = 1;
            postRequest = oracledata->CreatePost(_paymentTransaction,portion, 0,0);
            postRequest.CheckNumber = checkNumber;
            TiXmlDocument doc = oracledata->CreatePostXML(postRequest);
            AnsiString resultData = "";
            AnsiString data = oracledata->SerializeOut(doc);
            resultData = TOracleTCPIP::Instance().SendAndFetch(data);
            if(resultData.Pos("Connection Failed") == 0)
                retValue = oracledata->DeserializeData(resultData, _postResult);
            else
                retValue = false;
         }
         else if(totalPayTendered == 0 && hasCancelledItems)
         {
            retValue = true;
         }

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
    }
    if(retValue == true)
        TGlobalSettings::Instance().OracleCheckNumber = checkNumber;
    return retValue;
}
//---------------------------------------------------------------------------
void TManagerOraclePMS::GetRoomStatus(AnsiString _roomNumber, TRoomInquiryResult &_roomResult)//std::auto_ptr<TRoomInquiryResult> &_roomResult)
{
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Getting Room Details , Please Wait...";
    Processing->Show();
    try
    {
        std::auto_ptr<TOracleDataBuilder> oracledata(new TOracleDataBuilder());
        TPostRoomInquiry postRoomRequest;
        postRoomRequest.InquiryInformation = _roomNumber;
        oracledata->CreatePostRoomInquiry(postRoomRequest);
        TiXmlDocument doc = oracledata->CreateRoomInquiryXML(postRoomRequest);
        AnsiString resultData = "";
        AnsiString data = oracledata->SerializeOut(doc);
        resultData = TOracleTCPIP::Instance().SendAndFetch(data);
        if(resultData.Pos("Connection Failed") == 0)
        {
            oracledata->DeserializeInquiryData(resultData, _roomResult);
            if(!_roomResult.IsSuccessful)
            {
                MessageBox(_roomResult.resultText,"Warning",MB_OK);
            }
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//---------------------------------------------------------------------------
void TManagerOraclePMS::LogPMSEnabling(TriggerLocation triggerType)
{
    try
    {
        AnsiString fileName = GetFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(triggerType == eUI)
        {
            List->Add("Note- "+ (AnsiString)"Enabling Oracle as Selected from UI" +"\n");
        }
        else if(triggerType == eBoot)
        {
            List->Add("Note- "+ (AnsiString)"Enabling Oracle at start of Menumate" +"\n");
        }
        else if(triggerType == eSelf)
        {
            List->Add("Note- "+ (AnsiString)"Found Oracle disabled with necessary details present" +"\n" +
                  "      "+ "Menumate is trying to enable Oracle and then sale would be processed" +"\n");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
AnsiString TManagerOraclePMS::GetFileName()
{
    AnsiString directoryName = "";
    AnsiString fileName = "";
    directoryName = ExtractFilePath(Application->ExeName) + "/Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "/Oracle Posts Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "OraclePosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    fileName = directoryName + "/" + name;
    return fileName;
}
//----------------------------------------------------------------------------
bool TManagerOraclePMS::EnableOraclePMSSilently()
{
    bool retValue = false;
    try
    {
        TOracleTCPIP::Instance().IsSilentConnect = true;
        if(TOracleTCPIP::Instance().Connect())
        {
            retValue = GetLinkStatus();
        }
    }
    catch(Exception &Ex)
    {
        retValue = false;
    }
    return retValue;
}
//----------------------------------------------------------------------------
void TManagerOraclePMS::MakeOracleSeedFile()
{
   AnsiString fileName = ExtractFilePath(Application->ExeName) + "OracleSeed.txt" ;

    std::auto_ptr<TStringList> List(new TStringList);
    if (FileExists(fileName) )
    {
      List->LoadFromFile(fileName);
      List->Clear();
    }
    List->Add(TManagerVariable::Instance().DeviceProfileKey);
    List->Add(TGlobalSettings::Instance().InterbaseIP);
    List->Add(TGlobalSettings::Instance().DatabasePath);

    List->SaveToFile(fileName );
}
//----------------------------------------------------------------------------
