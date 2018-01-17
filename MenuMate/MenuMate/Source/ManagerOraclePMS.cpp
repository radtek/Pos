//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerOraclePMS.h"
#include "DeviceRealterminal.h"
#include "MMMessageBox.h"
#include "OracleManagerDB.h"
#include "OracleTCPIP.h"
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
        if(Registered && TCPIPAddress != "")
        {
            if(LoadRevenueCodes(DBTransaction))
            {
                if(Slots.size() > 0)
                {
                    if(InitializeoracleTCP())
                    {
                        Enabled = GetLinkStatus();
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
bool TManagerOraclePMS::LoadRevenueCodes(Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    RevenueCodesMap.clear();
    std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
    TIBSQL* queryRevenue = managerDB->LoadRevenueCodes(DBTransaction);
    for(;!queryRevenue->Eof;queryRevenue->Next())
    {
        TRevenueCodeDetails codeDetails;
        codeDetails.IsDefault = queryRevenue->FieldByName("ISDEFAULT_REVENUECODE")->AsString == "T" ?
                                true : false;
        codeDetails.RevenueCodeDescription = queryRevenue->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
        RevenueCodesMap[queryRevenue->FieldByName("REVENUECODE")->AsInteger] =
            codeDetails;
        retValue = true;
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::InitializeoracleTCP()
{
//    MessageBox("Going to Initialize TCP","shivashu",MB_OK);
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
    retValue = oracledata->DeserializeGetLinkStatus(resultData);
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
        AnsiString checkNumber = managerDB->GetCheckNumber(DBTransaction);
        DBTransaction.Commit();

        double tip = 0;
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            if(payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
               tip += (double)payment->GetAdjustment();
            }
        }
        double totalPayTendered = 0;
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            double amount = (double)payment->GetPayTendered();
            totalPayTendered += amount;
            double portion = 0;
            tip += (double)payment->TipAmount;
            if((amount != 0)
                  && !payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
                portion = (double)amount/(double)_paymentTransaction.Money.PaymentAmount;
                double tipPortion = RoundTo(tip * portion,-2);
                postRequest = oracledata->CreatePost(_paymentTransaction,portion, i,tipPortion);
                postRequest.CheckNumber = checkNumber;
                TiXmlDocument doc = oracledata->CreatePostXML(postRequest);
                AnsiString resultData = "";
                AnsiString data = oracledata->SerializeOut(doc);
                resultData = TOracleTCPIP::Instance().SendAndFetch(data);
                retValue = oracledata->DeserializeData(resultData, _postResult);
            }
         }
         bool isCompleteCancel = false;
         for(int itemNumber = 0; itemNumber < _paymentTransaction.Orders->Count; itemNumber++)
         {
            TItemComplete *itemThis = (TItemComplete*)_paymentTransaction.Orders->Items[itemNumber];
            if(itemThis->OrderType != CanceledOrder)
            {
               isCompleteCancel = true;
               break;
            }
         }
         if(totalPayTendered == 0 && !isCompleteCancel) // case for cancelled,100% discount
         {
            double portion = 1;
            postRequest = oracledata->CreatePost(_paymentTransaction,portion, 0,0);
            postRequest.CheckNumber = checkNumber;
            TiXmlDocument doc = oracledata->CreatePostXML(postRequest);
            AnsiString resultData = "";
            AnsiString data = oracledata->SerializeOut(doc);
            resultData = TOracleTCPIP::Instance().SendAndFetch(data);
            retValue = oracledata->DeserializeData(resultData, _postResult);
         }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
    }
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
//        MessageBox(postRoomRequest.WorkstationId,"WORKSTATIONID12",MB_OK);
        TiXmlDocument doc = oracledata->CreateRoomInquiryXML(postRoomRequest);
        AnsiString resultData = "";
        AnsiString data = oracledata->SerializeOut(doc);
        resultData = TOracleTCPIP::Instance().SendAndFetch(data);

        oracledata->DeserializeInquiryData(resultData, _roomResult);
        if(!_roomResult.IsSuccessful)
        {
            MessageBox(_roomResult.resultText,"Warning",MB_OK);
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//---------------------------------------------------------------------------
