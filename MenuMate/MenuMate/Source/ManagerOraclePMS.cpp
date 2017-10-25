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
        bool connectionMade = InitializeoracleTCP();

        if(Registered && TCPIPAddress != "")
        {
            if(connectionMade)
                Enabled = GetLinkStatus();
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
bool TManagerOraclePMS::InitializeoracleTCP()
{
    return TOracleTCPIP::Instance().Connect();
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::GetLinkStatus()
{
    bool retValue = false;
    std::auto_ptr<TOracleDataBuilder> oracledata(new TOracleDataBuilder());
    TLinkDescription linkDescription = oracledata->CreateLinkDescription();
    // Call for sending Link Description and receiving Link Alive
    retValue = true;
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
        Slots.push_back(slots);
    }
}
//---------------------------------------------------------------------------
bool TManagerOraclePMS::ExportData(TPaymentTransaction &_paymentTransaction,
                                    int StaffId)
{
    TOracleDataBuilder oracledata;
    TPostRequest postRequest;
    oracledata.CreatePost(_paymentTransaction,postRequest);
    TiXmlDocument doc = oracledata.CreatePostXML(postRequest);

    // Post Sales Data
    return true;
}
//---------------------------------------------------------------------------
void TManagerOraclePMS::GetRoomStatus(AnsiString _roomNumber, TRoomInquiryResult &_roomResult)//std::auto_ptr<TRoomInquiryResult> &_roomResult)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    std::auto_ptr<TOracleDataBuilder> oracledata(new TOracleDataBuilder());
    TPostRoomInquiry postRoomRequest;
    postRoomRequest.InquiryInformation = _roomNumber;
    oracledata->CreatePostRoomInquiry(postRoomRequest);
    oracledata->CreateRoomInquiryXML(postRoomRequest);
    _roomResult = oracledata->createXMLInquiryDoc();
    if(!_roomResult.IsSuccessful)
    {
        MessageBox(_roomResult.resultText,"Warning",MB_OK);
    }
    // Post room Inquiry
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
