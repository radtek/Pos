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
                    if(DefaultPaymentCategory.Trim() != "" && DefaultPaymentCategory != NULL)
                    {
                        if(PointsCategory.Trim() != "" && PointsCategory != NULL)
                        {
                            if(CreditCategory.Trim() != "" && CreditCategory != NULL)
                            {
                                if(InitializeoracleTCP())
                                {
                                    Enabled = GetLinkStatus();
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
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            if(payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
               tip += (double)payment->GetAdjustment();
            }
        }
        double totalPayTendered = 0;
        double roundedPaymentAmount = (double)_paymentTransaction.Money.PaymentAmount -
                                      (double)_paymentTransaction.Money.PaymentSurcharges;
        for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = _paymentTransaction.PaymentGet(i);
            double amount = (double)payment->GetPayTendered() - (double)payment->GetSurcharge();
            totalPayTendered += amount;
            double portion = 0;
            tip += (double)payment->TipAmount;
            if((amount != 0)
                  && !payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
            {
                roundedPaymentAmount += (double)payment->GetPayRounding();
                portion = (double)amount/roundedPaymentAmount ;
                portion = RoundTo(portion,-2);
                double tipPortion = RoundTo(tip * portion,-2);
                postRequest = oracledata->CreatePost(_paymentTransaction,portion, i,tipPortion);
                if(payment->GetSurcharge() != 0)
                {
                   double surcharge = payment->GetSurcharge();
                   surcharge = RoundTo(surcharge * 100 , -2);
                   AnsiString strSurcharge = (AnsiString)surcharge;
                   if(strSurcharge.Pos(".") != 0)
                   {
                      strSurcharge = strSurcharge.SubString(1,strSurcharge.Pos(".")-1);
                   }
                   int size = postRequest.Discount.size();
                   if(size < 16)
                   {
                        int paymentSurcharge = 0;
                        paymentSurcharge += atoi(strSurcharge.c_str());
                        AnsiString str = paymentSurcharge;
                        postRequest.Discount.push_back(str);
                   }
                   else
                   {
                        int paymentSurcharge = atoi(postRequest.Discount[15].c_str());
                        paymentSurcharge += atoi(strSurcharge.c_str());
                        AnsiString str = paymentSurcharge;
                        postRequest.Discount[15] = str;
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
                    if(postRequest.Subtotal1.size() > 0)
                    {
                        int oldSubTotal = atoi(postRequest.Subtotal1[0].c_str());
                        oldSubTotal += roundingPortionInt;
                        postRequest.Subtotal1[0] = oldSubTotal;
                    }
                }
                postRequest.CheckNumber = checkNumber;
                TiXmlDocument doc = oracledata->CreatePostXML(postRequest);
                AnsiString resultData = "";
                AnsiString data = oracledata->SerializeOut(doc);
                resultData = TOracleTCPIP::Instance().SendAndFetch(data);
                retValue = oracledata->DeserializeData(resultData, _postResult);
            }
         }
         bool isNotCompleteCancel = false;
         for(int itemNumber = 0; itemNumber < _paymentTransaction.Orders->Count; itemNumber++)
         {
            TItemComplete *itemThis = (TItemComplete*)_paymentTransaction.Orders->Items[itemNumber];
            if(itemThis->OrderType != CanceledOrder)
            {
               isNotCompleteCancel = true;
               break;
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
            retValue = oracledata->DeserializeData(resultData, _postResult);

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
