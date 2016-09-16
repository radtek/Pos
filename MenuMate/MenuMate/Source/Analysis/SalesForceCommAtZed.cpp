//---------------------------------------------------------------------------


#pragma hdrstop

#include "SalesForceCommAtZed.h"
#include <Memory>
#include "PocketVoucherInterface.h"
#include "DeviceRealTerminal.h"
#include "SalesForceInterface.h"
#include "StockInterface.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
using namespace StockInterface;

TSalesForceCommAtZed::TSalesForceCommAtZed()
{
}

TSalesForceCommAtZed::~TSalesForceCommAtZed()
{
}

void TSalesForceCommAtZed::PVCommunication(AnsiString CompanyName)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        bool voucherCreated = CheckPocketVoucherPaymentType(DBTransaction,CompanyName);
        EnableOrDisablePV(CompanyName);
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}

void TSalesForceCommAtZed::SalesForceCommunication(AnsiString CompanyName)
{
    try
    {
        FindingValuesOfFields(CompanyName);
    }
    catch(Exception &E)
    {
    	TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}

bool TSalesForceCommAtZed::CheckPocketVoucherPaymentType(Database::TDBTransaction &DBTransaction,AnsiString CompanyName)
{
    bool pvCreated = false;
    bool doReturn = false;
    try
    {
        bool pocketVoucherExists = false;

        // Pocket Voucher needs to be established ; this needs communication with
        // Salesforce for details of Pocket Voucher
        std::auto_ptr<TPocketVoucherInterface> pvInterface (new TPocketVoucherInterface());
        PocketVoucherDetails pvDetailsLocal;
        pvDetailsLocal = pvInterface->GetPocketVoucherDetail(CompanyName);
        Username = pvDetailsLocal.Username;
        Password = pvDetailsLocal.Password;
        MerchantId = pvDetailsLocal.MerchantId;
        Url = pvDetailsLocal.Url;
        PVFieldSalesForce = pvDetailsLocal.PocketVoucherField;
        std::vector<TPayment> Payments;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(DBTransaction,Payments);
        for(std::vector<TPayment>::iterator i = Payments.begin() ; i != Payments.end() ; ++i)
        {
            // Identifies the Payment type is Pocket Voucher Type
            if(i->Properties & ePayTypePocketVoucher)
            {
                // Finds out if any property s different from what is present at SF
                if((i->UniVoucherPass != pvDetailsLocal.Password || i->VoucherMerchantID != pvDetailsLocal.MerchantId ||
                   !(i->Properties & ePayTypeDispPVMsg) || TDeviceRealTerminal::Instance().PocketVouchers->URL != pvDetailsLocal.Url || i->UniVoucherUser != pvDetailsLocal.Username
                     || i->DisplayOrder != 99) &&(pvDetailsLocal.MerchantId.Length()!= 0 && pvDetailsLocal.Password.Length()!= 0
                     && pvDetailsLocal.Url.Length()!= 0 && pvDetailsLocal.Username.Length()!= 0))
                 {
                     UpdatePVPaymentType(DBTransaction, i->Name);

                     TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPocketVoucherURL,pvDetailsLocal.Url);
                 }
                if(TDeviceRealTerminal::Instance().PocketVouchers->URL != pvDetailsLocal.Url)
                {
                   TDeviceRealTerminal::Instance().PocketVouchers->URL = pvDetailsLocal.Url;
                   TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPocketVoucherURL,Url);
                }
                 pocketVoucherExists = true;
                 break;
            }
        }
        // Pocket Voucher needs to be created as a payment type now...
        if(!pocketVoucherExists)
        {
            if(pvDetailsLocal.MerchantId.Length()!= 0 && pvDetailsLocal.Password.Length()!= 0
              && pvDetailsLocal.Url.Length()!= 0 && pvDetailsLocal.Username.Length()!= 0)
            {
                CreatePVAsPaymentType(DBTransaction);
                pvCreated = true;
            }

        }
        return pvCreated;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TSalesForceCommAtZed::UpdatePVPaymentType(Database::TDBTransaction &DBTransaction , AnsiString name)
{
    try
    {
        int Properties = 0;
        Properties |= ePayTypePocketVoucher;
        Properties |= ePayTypeDispPVMsg;
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE PAYMENTTYPES SET PROPERTIES = :PROPERTIES, "
        "VOUCHER_MERCHANT_ID = :VOUCHER_MERCHANT_ID, VOUCHER_URL = :VOUCHER_URL, "
        "VOUCHER_USER = :VOUCHER_USER, VOUCHER_PASS = :VOUCHER_PASS, DISPLAY_ORDER = :DISPLAY_ORDER "
        "WHERE "
        "PAYMENT_NAME	= :PAYMENT_NAME";
        IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
        IBInternalQuery->ParamByName("VOUCHER_MERCHANT_ID")->AsString = MerchantId;
        IBInternalQuery->ParamByName("VOUCHER_USER")->AsString = Username;
        IBInternalQuery->ParamByName("VOUCHER_PASS")->AsString = Password;
        IBInternalQuery->ParamByName("VOUCHER_URL")->AsString = Url;
        IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = name;
        IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsString = 99;
        IBInternalQuery->ExecQuery();
    }
   catch(Exception &E)
	{
		throw;
	}
}

void TSalesForceCommAtZed::CreatePVAsPaymentType(Database::TDBTransaction &DBTransaction)
{
        int a = 0;
        int Properties = 0;
        Properties |= ePayTypePocketVoucher;
        Properties |= ePayTypeDispPVMsg;
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
			"INSERT INTO PAYMENTTYPES (" "PAYMENT_KEY, " "PAYMENT_NAME, " "PROPERTIES, " "EXCHANGE_RATE, " "COLOUR, "
			"DISPLAY_ORDER, " "PERCENT_ADJUST, " "AMOUNT_ADJUST, " "ROUNDTO, " "ADJUST_REASON, " "GROUP_NUMBER, " "THIRDPARTYCODES_KEY, "
			"DEST_IP," "DEST_PORT," "TAX_RATE," "PRE_VOUCHER_CODE," "VOUCHER_MERCHANT_ID," "INVOICE_EXPORT,VOUCHER_URL,VOUCHER_USER, "
			"VOUCHER_PASS," "CSV_READ_LOCATION," "CSV_WRITE_LOCATION," "TABKEY," "GL_CODE " ") " "VALUES (" "(SELECT GEN_ID(GEN_PAYMENTTYPES, 1) FROM RDB$DATABASE), " ":PAYMENT_NAME, "
			":PROPERTIES, " ":EXCHANGE_RATE, " ":COLOUR, " ":DISPLAY_ORDER, " ":PERCENT_ADJUST, " ":AMOUNT_ADJUST, " ":ROUNDTO, "
			":ADJUST_REASON, " ":GROUP_NUMBER, " ":THIRDPARTYCODES_KEY, " ":DEST_IP," ":DEST_PORT," ":TAX_RATE," ":PRE_VOUCHER_CODE,"
			":VOUCHER_MERCHANT_ID, " ":INVOICE_EXPORT,:VOUCHER_URL,:VOUCHER_USER,:VOUCHER_PASS,"
			":CSV_READ_LOCATION,:CSV_WRITE_LOCATION,:TABKEY,:GL_CODE ) ";
        IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Pocket Voucher";
        IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Properties;
        IBInternalQuery->ParamByName("EXCHANGE_RATE")->AsCurrency = 0.0;
        IBInternalQuery->ParamByName("COLOUR")->AsInteger = clAqua;
        IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = 99;
        IBInternalQuery->ParamByName("PERCENT_ADJUST")->AsCurrency = 0.0;
        IBInternalQuery->ParamByName("AMOUNT_ADJUST")->AsCurrency = 0.0;
        IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = 0.0;
        IBInternalQuery->ParamByName("ADJUST_REASON")->AsString = "";
        IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = 0;
        IBInternalQuery->ParamByName("DEST_IP")->AsString = "";
        IBInternalQuery->ParamByName("DEST_PORT")->AsInteger = 0;
        IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = 0.0;
        IBInternalQuery->ParamByName("PRE_VOUCHER_CODE")->AsString = 639491;
        IBInternalQuery->ParamByName("VOUCHER_MERCHANT_ID")->AsString = MerchantId;
        IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = 0;
        IBInternalQuery->ParamByName("VOUCHER_URL")->AsString = Url;
        IBInternalQuery->ParamByName("VOUCHER_USER")->AsString = Username;
        IBInternalQuery->ParamByName("VOUCHER_PASS")->AsString = Password;
        IBInternalQuery->ParamByName("CSV_READ_LOCATION")->AsString = "";
        IBInternalQuery->ParamByName("CSV_WRITE_LOCATION")->AsString = "";
        IBInternalQuery->ParamByName("TABKEY")->AsInteger = 0;
        IBInternalQuery->ParamByName("GL_CODE")->AsString = 0;
        IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = 0;
        IBInternalQuery->ExecQuery();
        if(TDeviceRealTerminal::Instance().PocketVouchers->URL != Url)
        {
           TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPocketVoucherURL,Url);
           TDeviceRealTerminal::Instance().PocketVouchers->URL = Url;
        }
}

void TSalesForceCommAtZed::EnableOrDisablePV(AnsiString CompanyName)
{
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    try
    {
        bool pocketVoucherExists = false;

        std::vector<TPayment> Payments;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(DBTransaction1,Payments);
        for(std::vector<TPayment>::iterator i = Payments.begin() ; i != Payments.end() ; ++i)
        {
            if(i->Properties & ePayTypePocketVoucher)
             {
                 pocketVoucherExists = true;
             }
        }
        // check for PV needs to be updated
        if(pocketVoucherExists != PVFieldSalesForce)
        {
           std::auto_ptr<TPocketVoucherInterface> pvInterface (new TPocketVoucherInterface());
           if(pocketVoucherExists)
           {
              pvInterface->EnablePocketVoucher(CompanyName);
           }
           else
           {
              pvInterface->DisablePocketVoucher(CompanyName);
           }
        }
        DBTransaction1.Commit();
    }
    catch(Exception &E)
    {
       DBTransaction1.Rollback();
       throw;
    }
}

void TSalesForceCommAtZed::FindingValuesOfFields(AnsiString accountName)
{
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    try
    {
        // First Get all values of required fields for all the Pos if it is the case of multiple
        // Pos connected to single database
        SalesForceFields sfFields;
        sfFields.AccountName = accountName;
        sfFields.Xero = IsXeroActivated(DBTransaction1) ;
        sfFields.WebMate = IsWebMateActivated(DBTransaction1);
        sfFields.Mobi2Go = sfFields.WebMate;
        sfFields.Thorlink = IsThorActivated(DBTransaction1);
        sfFields.VersionNumber = GetversionNumber(DBTransaction1);
        sfFields.PalmMate = IsPalmMateActivated(DBTransaction1);
        sfFields.ChefMate = IsChefMateActivated(DBTransaction1);
        sfFields.Loyalty = IsLoyaltyActivated(DBTransaction1);
        sfFields.Stock = IsStockActivated(DBTransaction1);
        DBTransaction1.Commit();

        std::auto_ptr<TSalesForceInterface> sfInterface (new TSalesForceInterface());
        sfInterface->UpdateSalesForceFields(sfFields);
    }
    catch(Exception &E)
    {
      DBTransaction1.Rollback();
      throw;
    }
}

bool TSalesForceCommAtZed::IsXeroActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        if(TGlobalSettings::Instance().IsXeroEnabled || TGlobalSettings::Instance().IsMYOBEnabled)
            isActivated = true;
        return isActivated;
}

bool TSalesForceCommAtZed::IsWebMateActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        TIBSQL *IBInternalQuery = DBTransaction1.Query(DBTransaction1.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " VARSPROFILE_KEY "
                            " FROM "
                            "  VARSPROFILE "
                            " WHERE "
                            " VARIABLES_KEY  = :VARIABLES_KEY  AND INTEGER_VAL <> :INTEGER_VAL";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 2117;
        IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = 0;
        IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
            isActivated = true;
		}
        return isActivated;
}

bool TSalesForceCommAtZed::IsThorActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        TIBSQL *IBInternalQuery = DBTransaction1.Query(DBTransaction1.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " VARSPROFILE_KEY "
                            " FROM "
                            "  VARSPROFILE "
                            " WHERE "
                            " VARIABLES_KEY  = :VARIABLES_KEY  AND INTEGER_VAL <> :INTEGER_VAL";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 9049;
        IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = 0;
        IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
            isActivated = true;
		}
        return isActivated;
}

bool TSalesForceCommAtZed::IsStockActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        UnicodeString key = "";
        TIBSQL *IBInternalQuery = DBTransaction1.Query(DBTransaction1.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " REGKEY "
                            " FROM "
                            "  REGISTRATION "
                            " WHERE "
                            " PRODUCT  = :PRODUCT ";
        IBInternalQuery->ParamByName("PRODUCT")->AsString = "Office";
        IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
            key = IBInternalQuery->FieldByName("REGKEY")->AsString;
            if(key.Length() > 0)
            {
                isActivated = true;
            }
		}
//        DBTransaction1.Commit();
        // Query for Stock
        UnicodeString StockDB =TGlobalSettings::Instance().StockInterbaseIP + ":" +TGlobalSettings::Instance().StockDatabasePath;
        bool exists = false;
        try
        {
            TStockInterface StockInterface(StockDB);
            StockInterface.Initialise();
            exists = StockInterface.CheckStockItem();
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message + " Full DB Path " + StockDB);
			throw;
		}
        if(exists)
        {
            isActivated = true;
        }
        else
        {
            isActivated = false;
        }
        return isActivated;
}

UnicodeString TSalesForceCommAtZed::GetversionNumber(Database::TDBTransaction &DBTransaction1)
{
        return GetFileVersion();
}

bool TSalesForceCommAtZed::IsPalmMateActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        TIBSQL *IBInternalQuery = DBTransaction1.Query(DBTransaction1.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " DEVICE_NAME "
                            " FROM "
                            "  DEVICES "
                            " WHERE "
                            " PRODUCT  = :PRODUCT";
        IBInternalQuery->ParamByName("PRODUCT")->AsString = "PalmMate";
        IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
            isActivated = true;
		}
        return isActivated;
}

bool TSalesForceCommAtZed::IsChefMateActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        TIBSQL *IBInternalQuery = DBTransaction1.Query(DBTransaction1.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " PHYSICALPRINTER_KEY "
                            " FROM "
                            "  PHYSICALPRINTER "
                            " WHERE "
                            " SHARE_NAME  = :SHARE_NAME";
        IBInternalQuery->ParamByName("SHARE_NAME")->AsString = "ChefMate Display";
        IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
            isActivated = true;
		}
        return isActivated;
}

bool TSalesForceCommAtZed::IsLoyaltyActivated(Database::TDBTransaction &DBTransaction1)
{
        bool isActivated = false;
        TIBSQL *IBInternalQuery = DBTransaction1.Query(DBTransaction1.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " VARSPROFILE_KEY "
                            " FROM "
                            "  VARSPROFILE "
                            " WHERE "
                            " VARIABLES_KEY  = :VARIABLES_KEY  AND INTEGER_VAL <> :INTEGER_VAL";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 7038;
        IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = 0;
        IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
            isActivated = true;
		}
        return isActivated;
}
