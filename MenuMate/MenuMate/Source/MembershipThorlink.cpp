//---------------------------------------------------------------------------


#pragma hdrstop

#include "MembershipThorlink.h"
#include <set>
#include <Memory>
#include <Sysutils.hpp>
#include <DateUtils.hpp>
#include "Processing.h"
#include "MMContactInfo.h"
#include "DBContacts.h"
#include "GlobalSettings.h"
#include "ManagerThorVoucher.h"
#include "MMLogging.h"
#include "MMTouchKeyboard.h"


#ifdef MenuMate
#include "Message.h"
#endif

#pragma package(smart_init)

TMembershipThorlink::TMembershipThorlink(TModules &Modules,
	AnsiString AppKey, AnsiString SiteNo, AnsiString MerchantCode,AnsiString DeviceCode) : TMembership(Modules),
ThorClient(new TThorlinkClient()) {
	ContactType = eThorMember;
	ContactTabType = TabMember ;
	ReadOnlyInterface = true;
	Modules.Status[eRegMembers]["ReadOnly"] = ReadOnlyInterface;
	Enabled = Modules.Status[eRegMembers]["Enabled"];
	AppKey = AppKey;
	SiteNo = SiteNo;
	MerchantCode = MerchantCode;
    DeviceCode = DeviceCode;
    TGlobalSettings::Instance().IsThorlinkSelected = true;
}

TMembershipThorlink::~TMembershipThorlink() {
}

void TMembershipThorlink::Initialise
(Database::TDBTransaction &DBTransaction)
{
	TMembership::Initialise(DBTransaction);
}


/* you can only log in via Card this should have already happened beforethis function call.
 */
TLoginSuccess TMembershipThorlink::TestKeyLogin
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	if (!fEnabled)
		return lsUserNotFound;

	TLoginSuccess Success = lsUserNotFound;
	if (UserInfo.ContactKey != 0)
	{
		Success = lsAccepted;
	}
	else
	{
		Success = lsUserNotFound;
	}
	return Success;
}

bool TMembershipThorlink::GetExternalContactDetialsFromKey(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
    bool RetVal = false;
	if(Info.ContactKey != 0)
    {

            // Will load the Cards details.
            std::set<UnicodeString> Cards;
            TDBContacts::GetContactCards(DBTransaction,Info.ContactKey,Cards);
            if(Cards.size() > 0)
            {
				Info.CardStr = *Cards.begin();
                Info.ContactKey = GetContactByCard(DBTransaction, Info);
                if(Info.ContactKey != 0)
                {
					GetContactDetails(DBTransaction,Info.ContactKey,Info);
					RetVal = true;
                }
            }

    }
    return RetVal;
}

void TMembershipThorlink::GetContactDetails(Database::TDBTransaction &DBTransaction,
	int InternalKey,
	TMMContactInfo &Info)
{
	if (!fEnabled)
		return;
	try
    {
		TMembership::GetContactDetails(DBTransaction, Info.ContactKey, Info);
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipThorlink::UpdateThorlinkVouchers(Database::TDBTransaction &DBTransaction)
{
  try
    {

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "DELETE FROM THORLINK_VOUCHERS ";
		IBInternalQuery->ExecQuery();

        if(!ThorClient->thorclientvouchers.empty())
        {
            for(TDETAILS_LIST::iterator itTabs = ThorClient->thorclientvouchers.begin(); itTabs != ThorClient->thorclientvouchers.end(); itTabs++)
            {
                UnicodeString ThorName = "";

                IBInternalQuery1->Close();
                IBInternalQuery1->SQL->Text = "INSERT INTO THORLINK_VOUCHERS "
                "( "
                "   THOR_VOUCHERS_KEY,"
                "   NAME,"
                "   PROPERTIES,"
                "   DESCRIPTION,"
                "   PERCENTAGE,"
                "   AMOUNT,"
                "   VOUCHER_TYPE,"
                "   VOUCHER_MODE,"
                "   ROUNDING,"
                "   PRIORITY,"
                "   APPEARANCE_ORDER,"
                "   MEMBERS_ONLY,"
                "   MEMBERS_EXEMPT,"
                "   APPLYTOMEMBERS,"
                "   MAX_VALUE,"
                "   VOUCHER_GROUP,"
                "   VOUCHERS_CODE,"
                "   ISTHORGROUP"
                ") "
                "VALUES "
                "( "
                "   :THOR_VOUCHERS_KEY,"
                "   :NAME,"
                "   :PROPERTIES,"
                "   :DESCRIPTION,"
                "   :PERCENTAGE,"
                "   :AMOUNT,"
                "   :VOUCHER_TYPE,"
                "   :VOUCHER_MODE,"
                "   :ROUNDING,"
                "   :PRIORITY,"
                "   :APPEARANCE_ORDER,"
                "   :MEMBERS_ONLY,"
                "   :MEMBERS_EXEMPT,"
                "   :APPLYTOMEMBERS,"
                "   :MAX_VALUE,"
                "   :VOUCHER_GROUP,"
                "   :VOUCHERS_CODE,"
                "   :ISTHORGROUP"
                ");";
                if((itTabs->PluCode.Length() != 0) /*&& (itTabs->Name.Pos(":") == 0)*/)
                {
                    IBInternalQuery1->ParamByName("THOR_VOUCHERS_KEY")->AsInteger = atoi(((AnsiString)itTabs->Id).c_str());
                    IBInternalQuery1->ParamByName("NAME")->AsString  = itTabs->PluCode + ": " +itTabs->Name;
                    IBInternalQuery1->ParamByName("PROPERTIES")->AsInteger  = atoi(itTabs->PluCode.c_str());
                }
                else
                {
                    IBInternalQuery1->ParamByName("THOR_VOUCHERS_KEY")->AsInteger = atoi(((AnsiString)itTabs->Id).c_str());
                    IBInternalQuery1->ParamByName("NAME")->AsString  = itTabs->Name;
                    IBInternalQuery1->ParamByName("PROPERTIES")->AsInteger  = atoi(((AnsiString)itTabs->Name.SubString(0,itTabs->Name.Pos(":")-1)).c_str());
                }

                IBInternalQuery1->ParamByName("DESCRIPTION")->AsString  = itTabs->Name;
                IBInternalQuery1->ParamByName("PERCENTAGE")->AsDouble  = 0.0;
                IBInternalQuery1->ParamByName("AMOUNT")->AsDouble = 0.0;
                IBInternalQuery1->ParamByName("VOUCHER_TYPE")->AsInteger = 0;
                IBInternalQuery1->ParamByName("VOUCHER_MODE")->AsInteger = 1;
                IBInternalQuery1->ParamByName("ROUNDING")->AsDouble = 0.0100;
                IBInternalQuery1->ParamByName("PRIORITY")->AsInteger = 0;
                IBInternalQuery1->ParamByName("APPEARANCE_ORDER")->AsInteger = 0;
                IBInternalQuery1->ParamByName("MEMBERS_ONLY")->AsString = "F";
                IBInternalQuery1->ParamByName("MEMBERS_EXEMPT")->AsString = "F";
                IBInternalQuery1->ParamByName("APPLYTOMEMBERS")->AsString = "F";
                IBInternalQuery1->ParamByName("MAX_VALUE")->AsDouble = 0;
                IBInternalQuery1->ParamByName("VOUCHER_GROUP")->AsInteger = 1;
                IBInternalQuery1->ParamByName("VOUCHERS_CODE")->AsString = itTabs->Code;
                IBInternalQuery1->ParamByName("ISTHORGROUP")->AsString = "T";
                IBInternalQuery1->ExecQuery();
            }
            ThorClient->thorclientvouchers.clear();
        }

    }
  catch(Exception & E)
	{
      TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
	}

}

int TMembershipThorlink::GenerateThorVoucherKey(Database::TDBTransaction &DBTransaction)
{
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_THORVOUCHER_KEY, 1) FROM RDB$DATABASE ";
    IBInternalQuery->ExecQuery();
    int val = IBInternalQuery->Fields[0]->AsInteger;
    return val;
}

int TMembershipThorlink::GetContactByCard
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
	if (!fEnabled)
		return 0;

	try {
		GetExternalContactDetails(DBTransaction, Info);
		Info.ContactKey = GetInternalKey(DBTransaction, Info.ExternalKey);
		// Get or create the member locally / update there info and return the lcoal key.
	    }
	catch(Exception &E)
        {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
    	}
	return Info.ContactKey;
}

TLoginSuccess TMembershipThorlink::FindMember
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo) {
	if (!fEnabled)
		return lsUserNotFound;
	TLoginSuccess LoginSuccess = lsUserNotFound;

	if (UserInfo.ContactKey == 0)
    {
		if (UserInfo.CardStr != "")
        {
			UserInfo.ContactKey = GetContactByCard(DBTransaction, UserInfo);
		}
	}

	if (UserInfo.ContactKey != 0) {
		LoginSuccess = TestKeyLogin(DBTransaction, UserInfo);
		GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
	}

	return LoginSuccess;
}

TLoginSuccess TMembershipThorlink::IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{

	if(UserInfo.CardStr != "")
    {
        return lsAccepted;
    }
    else
    {
    	return lsDenied;
    }
}


TContactType TMembershipThorlink::GetContactType(int ContactKey) {
	if (!fEnabled)
		return eContactTypeUnknown;
	TContactType RetVal = eThorMember;
	return RetVal;
}

void TMembershipThorlink::GetKioskDisplayMsg
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info,
		 std::map<UnicodeString, UnicodeString> &Data)
{
	if (!fEnabled)
		return;
	try {
		Data["%TITLE%"] = "Thor";
		Data["%MSG%"] = "Welcome";
		Data["%MSG1%"] = Info.Name;
		Data["%VISIT_MSG%"] = "";
		Data["%BIRTHDAY_MSG%"] = "";
		// Data["%POINTS%"] = FormatFloat("0.00",Info.Points.CurrentPoints);
		Data["BIRTHDAY_PRINT"] = "F";
		Data["DOCKET_PRINT"] = "F";
		Data["DOCKET_LUCKYDRAW_PRINT"] = "F";
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

TLoginSuccess TMembershipThorlink::GetSubsPaid
		(Database::TDBTransaction &DBTransaction, int InternalKey) {
	return lsAccepted;
}

void TMembershipThorlink::SetContactDetails
		(Database::TDBTransaction &DBTransaction, int InternalKey,
	TMMContactInfo &Info) {
	if (!fEnabled)
		return;
	try {
		TContact::SetContactDetails(DBTransaction, InternalKey, Info);
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

TLoginSuccess TMembershipThorlink::TestCardLogin
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	if (!fEnabled)
		return lsUserNotFound;

	TLoginSuccess Success = lsUserNotFound;
	try {
		GetExternalContactDetails(DBTransaction, UserInfo);
		if (UserInfo.ExternalKey != 0)
		{
			Success = GetSubsPaid(DBTransaction, UserInfo.ContactKey);
		}
		else
		{
			Success = lsUserNotFound;
		}
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
 		throw;
	}
	return Success;
}
//
void TMembershipThorlink::GetExternalContactDetails
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info) {
	if (!fEnabled)
		return;
	bool UpdateLocalInfo = false;

        if(!(TGlobalSettings::Instance().IsThorlinkEnabled))
        {
           throw Exception("Please Enable the Thorlink Membership First\r");
        }
        else
        {
            if (Info.CardStr != "")
            {
//                Info.CardStr = "6276046917719848";
                std::auto_ptr<TfrmProcessing>
                (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
                Processing->Message = "Processing Transaction With Thorlink , Please Wait...";
                Processing->Show();
            try
            {
                bool autorizationSuccessful = CallInitMethod();
//                ThorClient->SetAuthorizationParameters(TGlobalSettings::Instance().ThorlinkAppKey, TGlobalSettings::Instance().ThorlinkSiteNo, TGlobalSettings::Instance().ThorlinkMerchantCode, TGlobalSettings::Instance().ThorlinkDeviceCode);
                if(!autorizationSuccessful)
                {
                   Processing->Close();
                   throw Exception("Could not Communicate with Thor , Please Check Services.");
                }

                ThorClient->GetMemberInformation(Info.CardStr);
                Processing->Close();
                AnsiString message = ThorClient->ResponseDescription;
                if (ThorClient->ResponseDescription.SubString(1,2) == "OK")
                {
                    Info.Name = ThorClient->FirstName +" "+ ThorClient->LastName;
                    Info.ExternalKey = StrToIntDef(ThorClient->TraderId,0);

                    Info.CardsToAdd.insert(Info.CardStr);
                    Info.Points.Clear();
                    double int64Points = ThorClient->LoyaltyBalance;

                    TPointsTypePair typepair(pttEarned,ptstLoyalty);
                    TPointsType type(pasDatabase, typepair,pesExported);
                    Info.Points.Load(type,Currency(int64Points));
                    Info.TabEnabled = false;
                    UpdateLocalInfo = true;
                }
                else
                {
                    if(message.Length() == 0)
                    {
                       message = "Could Not Get Response From Thor." ;
                    }
                    throw Exception(message);
                }

               }
              catch(Exception & E)
               {
                    Processing->Close();
                    TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
                    throw;
                }
            }
            else
            {
                throw Exception("Unable to lookup member, please use there card. ");
            }
            // Update the Member in our Database.
            if (UpdateLocalInfo)
            {
                Info.ContactKey = GetInternalKey(DBTransaction, Info.ExternalKey);
                // Preserve the Contacts PIN for Pre Auth.
                Info.PIN = TDBContacts::GetPIN(DBTransaction, Info.ContactKey);
                SetContactDetails(DBTransaction, Info.ContactKey, Info);
                UpdateThorlinkVouchers(DBTransaction);
                ThorVoucherKeys = ManagerThorVoucher->ThorVoucherKeys(DBTransaction) ;
                int Category = ManagerThorVoucher->SetThorVoucherCategories(DBTransaction) ;
                ManagerThorVoucher->UpdateThorVoucherCategories(DBTransaction);
                ManagerThorVoucher->InsertThorVoucherCategories(DBTransaction,ThorVoucherKeys,Category) ;
                DBTransaction.Commit();
                DBTransaction.StartTransaction();
            }
        }

}

bool TMembershipThorlink::CallInitMethod()
{
    return ThorClient->SetAuthorizationParameters(TGlobalSettings::Instance().ThorlinkAppKey, TGlobalSettings::Instance().ThorlinkSiteNo, TGlobalSettings::Instance().ThorlinkMerchantCode, TGlobalSettings::Instance().ThorlinkDeviceCode);
}
AnsiString TMembershipThorlink::SendRequestForPay(std::vector<TTenderDetails> tenderDetailsList,std::vector<TItemDetailsThor> itemsList)
{
    AnsiString responsePurchase = "";
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Processing Transaction With Thorlink , Please Wait...";
    Processing->Show();
    TPurchaseResponse response = ThorClient->GetTenderInformation(tenderDetailsList,itemsList);
    if(response.code != eTransactionSuccessful && response.code != eTransactionInSAF)
    {
        responsePurchase = response.responseMessage;
    }
    Processing->Close();
    return responsePurchase;
}AnsiString TMembershipThorlink::SendRequestForRefund(TRefundTransaction refundTransaction){
    AnsiString responsePurchase = "";
    std::auto_ptr<TfrmProcessing>
    (Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Processing Transaction With Thorlink , Please Wait...";
    Processing->Show();
    TPurchaseResponse response = ThorClient->GetRefundInformation(refundTransaction);
    if(response.code != eTransactionSuccessful && response.code != eTransactionInSAF)
    {
        responsePurchase = response.responseMessage;
    }
    Processing->Close();
    return responsePurchase;
}