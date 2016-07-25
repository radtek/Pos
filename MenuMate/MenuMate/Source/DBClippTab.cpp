//---------------------------------------------------------------------------

#pragma hdrstop
#include "DBClippTab.h"
#include "MMLogging.h"
#include "DBOrder.h"
#include "DBTab.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

void TClippTab::SetTabKey(int tabKey)
{
    _tabKey = tabKey;
}

//---------------------------------------------------------------------------

void TClippTab::SetClippTabKey(int clippTabKey)
{
    _clippTabKey = clippTabKey;
}

//---------------------------------------------------------------------------

void TClippTab::SetTabCreatedDate(TDateTime tabCreatedDate)
{
    _tabCreatedDate = tabCreatedDate;
}

//---------------------------------------------------------------------------

void TClippTab::SetClippTabRef(UnicodeString clippTabRef)
{
    _clippTabRef = clippTabRef;
}

//---------------------------------------------------------------------------

void TClippTab::SetClippUserRef(UnicodeString clippUserRef)
{
    _clippUserRef = clippUserRef;
}

//---------------------------------------------------------------------------

void TClippTab::SetCustomerFirstName(UnicodeString customerFirstName)
{
    _customerFirstName = customerFirstName;
}

//---------------------------------------------------------------------------

void TClippTab::SetCustomerLastName(UnicodeString customerLastName)
{
    _customerLastName = customerLastName;
}

//---------------------------------------------------------------------------

void TClippTab::SetCustomerPhotoUrl(UnicodeString customerPhotoUrl)
{
    _customerPhotoUrl = customerPhotoUrl;
}

//---------------------------------------------------------------------------

void TClippTab::SetIsTabClosed(bool isTabClosed)
{
    _isTabClosed = isTabClosed;
}

//---------------------------------------------------------------------------

void TClippTab::SetTabName(UnicodeString tabName)
{
    _tabName = tabName;
}

//---------------------------------------------------------------------------

void TClippTab::SetTabType(TMMTabType tabType)
{
    _tabType = tabType;
}

//---------------------------------------------------------------------------

void TClippTab::SetTabLimit(Currency tabLimit)
{
    _tabLimit = tabLimit;
}

//---------------------------------------------------------------------------

void TClippTab::SetTabCredit(Currency tabCredit)
{
    _tabCredit = tabCredit;
}

//---------------------------------------------------------------------------

void TClippTab::SetClippPayments(CLIPP_PAYMENTS clippPayments)
{
    _clippPayments = clippPayments;
}

//---------------------------------------------------------------------------

void TClippPayment::SetClippTabPaymentKey(int clippTabPaymentKey)
{
    _clippTabPaymentKey = clippTabPaymentKey;
}

//---------------------------------------------------------------------------

void TClippPayment::SetClippTabKey(int clippTabKey)
{
    _clippTabKey = clippTabKey;
}

//---------------------------------------------------------------------------

void TClippPayment::SetClippPaymentRef(UnicodeString clippPaymentRef)
{
    _clippPaymentRef = clippPaymentRef;
}

//---------------------------------------------------------------------------

void TClippPayment::SetAmount(Currency amount)
{
    _amount = amount;
}

//---------------------------------------------------------------------------

void TClippPayment::SetClippPaymentType(TMMClippPaymentType clippPaymentType)
{
    _clippPaymentType = clippPaymentType;
}

//---------------------------------------------------------------------------

int TDBClippTab::CreateAndSetClippTabDetails(Database::TDBTransaction &dbTransaction, TClippTab clippTab)
{
    int tabKey = TDBTab::SetTabDetails(dbTransaction, clippTab.TabKey, clippTab.TabName, clippTab.TabLimit, clippTab.TabType);

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    try
	{
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CLIPP_TAB_KEY, 1) FROM RDB$DATABASE";
        ibInternalQuery->ExecQuery();

        int clippTabKey = ibInternalQuery->Fields[0]->AsInteger;

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "INSERT INTO CLIPP_TAB "
                                            "("
                                                "CLIPP_TAB_KEY, "
                                                "CLIPP_TAB_TAB_KEY, "
                                                "TAB_CREATED_DATE, "
                                                "CLIPP_TAB_REF, "
                                                "CLIPP_USER_REF, "
                                                "CUSTOMER_FIRST_NAME, "
                                                "CUSTOMER_LAST_NAME, "
                                                "CUSTOMER_PHOTO_URL, "
                                                "IS_TAB_CLOSED "
                                            ") "
                                        "VALUES "
                                            "("
                                                ":CLIPP_TAB_KEY,"
                                                ":CLIPP_TAB_TAB_KEY,"
                                                ":TAB_CREATED_DATE,"
                                                ":CLIPP_TAB_REF, "
                                                ":CLIPP_USER_REF,"
                                                ":CUSTOMER_FIRST_NAME,"
                                                ":CUSTOMER_LAST_NAME,"
                                                ":CUSTOMER_PHOTO_URL, "
                                                ":IS_TAB_CLOSED "
                                            ");";

        ibInternalQuery->ParamByName("CLIPP_TAB_KEY")->AsInteger = clippTabKey;
        ibInternalQuery->ParamByName("CLIPP_TAB_TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("TAB_CREATED_DATE")->AsDateTime = clippTab.TabCreatedDate;
        ibInternalQuery->ParamByName("CLIPP_TAB_REF")->AsString = clippTab.ClippTabRef;
        ibInternalQuery->ParamByName("CLIPP_USER_REF")->AsString = clippTab.ClippUserRef;
        ibInternalQuery->ParamByName("CUSTOMER_FIRST_NAME")->AsString = clippTab.CustomerFirstName;
        ibInternalQuery->ParamByName("CUSTOMER_LAST_NAME")->AsString = clippTab.CustomerLastName;
        ibInternalQuery->ParamByName("CUSTOMER_PHOTO_URL")->AsString = clippTab.CustomerPhotoUrl;
        ibInternalQuery->ParamByName("IS_TAB_CLOSED")->AsString = "F";

        ibInternalQuery->ExecQuery();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return tabKey;
}

//---------------------------------------------------------------------------

int TDBClippTab::CreateClippTabPayment(Database::TDBTransaction &dbTransaction, int tabKey, UnicodeString clippPaymentRef, Currency clippPayment,
                            TMMClippPaymentType clippPaymentType)
{
    int clippTabPaymentKey = 0;
    int clippTabKey = GetClippTabKeyByTabKey(dbTransaction, tabKey);

    if(clippTabKey <= 0)
    {
        throw;
    }

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    try
	{
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CLIPP_TAB_PAYMENT_KEY, 1) FROM RDB$DATABASE";
        ibInternalQuery->ExecQuery();

        clippTabPaymentKey = ibInternalQuery->Fields[0]->AsInteger;

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "INSERT INTO CLIPP_TAB_PAYMENTS "
                                            "("
                                                "CLIPP_TAB_PAYMENT_KEY, "
                                                "CLIPP_TAB_KEY, "
                                                "CLIPP_PAYMENT_REF, "
                                                "CLIPP_PAYMENT, "
                                                "CLIPP_PAYMENT_TYPE "
                                            ") "
                                        "VALUES "
                                            "("
                                                ":CLIPP_TAB_PAYMENT_KEY,"
                                                ":CLIPP_TAB_KEY,"
                                                ":CLIPP_PAYMENT_REF,"
                                                ":CLIPP_PAYMENT, "
                                                ":CLIPP_PAYMENT_TYPE"
                                            ");";

        ibInternalQuery->ParamByName("CLIPP_TAB_KEY")->AsInteger = clippTabKey;
        ibInternalQuery->ParamByName("CLIPP_TAB_PAYMENT_KEY")->AsInteger = clippTabPaymentKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_REF")->AsString = clippPaymentRef;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT")->AsCurrency = clippPayment;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = clippPaymentType;

        ibInternalQuery->ExecQuery();

        //If there was an intermediate payment done on clipp than we need to consider that on POS tab also..
        //So we increase the tab credit limit..
        if(clippPaymentType == ClippPayment)
        {
            Currency currentTabCreditLimit = TDBTab::GetTabCreditLimit(dbTransaction, tabKey);

            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text = "UPDATE TAB "
                                            "SET "
                                                "CREDIT_LIMIT = :CREDIT_LIMIT "
                                            "WHERE TAB_KEY = :TAB_KEY";

            ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
            ibInternalQuery->ParamByName("CREDIT_LIMIT")->AsCurrency = (clippPayment + currentTabCreditLimit);

            ibInternalQuery->ExecQuery();
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return clippTabPaymentKey;
}

//---------------------------------------------------------------------------

bool TDBClippTab::CloseClippTab(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    int clippTabKey = GetClippTabKeyByTabKey(dbTransaction, tabKey);

    if(clippTabKey <= 0)
    {
        return false;
    }

    try
	{
        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "UPDATE CLIPP_TAB "
                                        "SET "
                                            "IS_TAB_CLOSED = :IS_TAB_CLOSED "
                                        "WHERE CLIPP_TAB_KEY = :CLIPP_TAB_KEY";

        ibInternalQuery->ParamByName("IS_TAB_CLOSED")->AsString = "T";
        ibInternalQuery->ParamByName("CLIPP_TAB_KEY")->AsInteger = clippTabKey;

        ibInternalQuery->ExecQuery();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return true;
}

//---------------------------------------------------------------------------

TClippTab* TDBClippTab::GetClippTabDetailsByTabKey(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TClippTab* clippTab;

	TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CT.CLIPP_TAB_KEY, "
                                        "CT.CLIPP_TAB_TAB_KEY, "
                                        "CT.TAB_CREATED_DATE, "
                                        "CT.CLIPP_TAB_REF, "
                                        "CT.CLIPP_USER_REF, "
                                        "CT.CUSTOMER_FIRST_NAME, "
                                        "CT.CUSTOMER_LAST_NAME, "
                                        "CT.CUSTOMER_PHOTO_URL, "
                                        "T.TAB_NAME, "
                                        "T.TAB_TYPE, "
                                        "T.CREDIT_LIMIT, "
                                        "T.CREDIT, "
                                        "CT.IS_TAB_CLOSED "
                                    "FROM CLIPP_TAB CT "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            clippTab = new TClippTab;

            clippTab->ClippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
            clippTab->TabKey = ibInternalQuery->FieldByName("CLIPP_TAB_TAB_KEY")->AsInteger;
            clippTab->TabCreatedDate = ibInternalQuery->FieldByName("TAB_CREATED_DATE")->AsDateTime;
            clippTab->ClippTabRef = ibInternalQuery->FieldByName("CLIPP_TAB_REF")->AsString;
            clippTab->ClippUserRef = ibInternalQuery->FieldByName("CLIPP_USER_REF")->AsString;
            clippTab->CustomerFirstName = ibInternalQuery->FieldByName("CUSTOMER_FIRST_NAME")->AsString;
            clippTab->CustomerLastName = ibInternalQuery->FieldByName("CUSTOMER_LAST_NAME")->AsString;
            clippTab->CustomerPhotoUrl = ibInternalQuery->FieldByName("CUSTOMER_PHOTO_URL")->AsString;
            clippTab->TabName = ibInternalQuery->FieldByName("TAB_NAME")->AsString;
            clippTab->TabType = (TMMTabType)ibInternalQuery->FieldByName("TAB_TYPE")->AsInteger;
            clippTab->TabLimit = ibInternalQuery->FieldByName("CREDIT_LIMIT")->AsCurrency;
            clippTab->TabCredit = ibInternalQuery->FieldByName("CREDIT")->AsCurrency;
            clippTab->IsTabClosed = ibInternalQuery->FieldByName("IS_TAB_CLOSED")->AsString == "F" ? false : true;
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippTab;
}

//---------------------------------------------------------------------------

TClippTab* TDBClippTab::GetClippTabDetailsByClippTabKey(Database::TDBTransaction &dbTransaction, int clippTabKey)
{
    TClippTab* clippTab;

	TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CT.CLIPP_TAB_KEY, "
                                        "CT.CLIPP_TAB_TAB_KEY, "
                                        "CT.TAB_CREATED_DATE, "
                                        "CT.CLIPP_TAB_REF, "
                                        "CT.CLIPP_USER_REF, "
                                        "CT.CUSTOMER_FIRST_NAME, "
                                        "CT.CUSTOMER_LAST_NAME, "
                                        "CT.CUSTOMER_PHOTO_URL, "
                                        "T.TAB_NAME, "
                                        "T.TAB_TYPE, "
                                        "T.CREDIT_LIMIT, "
                                        "T.CREDIT, "
                                        "CT.IS_TAB_CLOSED "
                                    "FROM CLIPP_TAB CT "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE CT.CLIPP_TAB_KEY = :CLIPP_TAB_KEY";

        ibInternalQuery->ParamByName("CLIPP_TAB_KEY")->AsInteger = clippTabKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            clippTab = new TClippTab;

            clippTab->ClippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
            clippTab->TabKey = ibInternalQuery->FieldByName("CLIPP_TAB_TAB_KEY")->AsInteger;
            clippTab->TabCreatedDate = ibInternalQuery->FieldByName("TAB_CREATED_DATE")->AsDateTime;
            clippTab->ClippTabRef = ibInternalQuery->FieldByName("CLIPP_TAB_REF")->AsString;
            clippTab->ClippUserRef = (TMMTabType)ibInternalQuery->FieldByName("CLIPP_USER_REF")->AsInteger;
            clippTab->CustomerFirstName = ibInternalQuery->FieldByName("CUSTOMER_FIRST_NAME")->AsString;
            clippTab->CustomerLastName = ibInternalQuery->FieldByName("CUSTOMER_LAST_NAME")->AsString;
            clippTab->CustomerPhotoUrl = ibInternalQuery->FieldByName("CUSTOMER_PHOTO_URL")->AsString;
            clippTab->TabName = ibInternalQuery->FieldByName("TAB_NAME")->AsString;
            clippTab->TabType = (TMMTabType)ibInternalQuery->FieldByName("TAB_TYPE")->AsInteger;
            clippTab->TabLimit = ibInternalQuery->FieldByName("CREDIT_LIMIT")->AsCurrency;
            clippTab->TabCredit = (TMMTabType)ibInternalQuery->FieldByName("CREDIT")->AsCurrency;
            clippTab->IsTabClosed = ibInternalQuery->FieldByName("IS_TAB_CLOSED")->AsString == "F" ? false : true;
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippTab;
}

//---------------------------------------------------------------------------

CLIPP_PAYMENTS TDBClippTab::GetAllClippPayments(Database::TDBTransaction &dbTransaction, int tabKey)
{
    CLIPP_PAYMENTS clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TClippPayment clippTabPayment;

            clippTabPayment.ClippTabPaymentKey = ibInternalQuery->FieldByName("CLIPP_TAB_PAYMENT_KEY")->AsInteger;
            clippTabPayment.ClippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
            clippTabPayment.ClippPaymentRef = ibInternalQuery->FieldByName("CLIPP_PAYMENT_REF")->AsString;
            clippTabPayment.Amount = ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            clippTabPayment.ClippPaymentType = (TMMClippPaymentType)ibInternalQuery->FieldByName("CLIPP_PAYMENT_TYPE")->AsInteger;

            ibInternalQuery->Next();

            clippPayments.push_back(clippTabPayment);
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

CLIPP_PAYMENTS TDBClippTab::GetTipPayments(Database::TDBTransaction &dbTransaction, int tabKey)
{
    CLIPP_PAYMENTS clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY AND CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = ClippTip;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TClippPayment clippTabPayment;

            clippTabPayment.ClippTabPaymentKey = ibInternalQuery->FieldByName("CLIPP_TAB_PAYMENT_KEY")->AsInteger;
            clippTabPayment.ClippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
            clippTabPayment.ClippPaymentRef = ibInternalQuery->FieldByName("CLIPP_PAYMENT_REF")->AsString;
            clippTabPayment.Amount = ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            clippTabPayment.ClippPaymentType = (TMMClippPaymentType)ibInternalQuery->FieldByName("CLIPP_PAYMENT_TYPE")->AsInteger;

            ibInternalQuery->Next();

            clippPayments.push_back(clippTabPayment);
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

CLIPP_PAYMENTS TDBClippTab::GetProcessingFeesPayments(Database::TDBTransaction &dbTransaction, int tabKey)
{
    CLIPP_PAYMENTS clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY AND CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = ClippProcessingFees;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TClippPayment clippTabPayment;

            clippTabPayment.ClippTabPaymentKey = ibInternalQuery->FieldByName("CLIPP_TAB_PAYMENT_KEY")->AsInteger;
            clippTabPayment.ClippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
            clippTabPayment.ClippPaymentRef = ibInternalQuery->FieldByName("CLIPP_PAYMENT_REF")->AsString;
            clippTabPayment.Amount = ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            clippTabPayment.ClippPaymentType = (TMMClippPaymentType)ibInternalQuery->FieldByName("CLIPP_PAYMENT_TYPE")->AsInteger;

            ibInternalQuery->Next();

            clippPayments.push_back(clippTabPayment);
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

CLIPP_PAYMENTS TDBClippTab::GetClippPayments(Database::TDBTransaction &dbTransaction, int tabKey)
{
    CLIPP_PAYMENTS clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY AND CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = ClippPayment;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            TClippPayment clippTabPayment;

            clippTabPayment.ClippTabPaymentKey = ibInternalQuery->FieldByName("CLIPP_TAB_PAYMENT_KEY")->AsInteger;
            clippTabPayment.ClippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
            clippTabPayment.ClippPaymentRef = ibInternalQuery->FieldByName("CLIPP_PAYMENT_REF")->AsString;
            clippTabPayment.Amount = ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            clippTabPayment.ClippPaymentType = (TMMClippPaymentType)ibInternalQuery->FieldByName("CLIPP_PAYMENT_TYPE")->AsInteger;

            ibInternalQuery->Next();

            clippPayments.push_back(clippTabPayment);
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

Currency TDBClippTab::GetAllClippPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey)
{
    Currency clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            clippPayments = clippPayments + ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            ibInternalQuery->Next();
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

Currency TDBClippTab::GetTipPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey)
{
    Currency clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY AND CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = ClippTip;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            clippPayments = clippPayments + ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            ibInternalQuery->Next();
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

Currency TDBClippTab::GetProcessingFeesPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey)
{
    Currency clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY AND CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = ClippProcessingFees;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            clippPayments = clippPayments + ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            ibInternalQuery->Next();
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

Currency TDBClippTab::GetClippPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey)
{
    Currency clippPayments;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
	ibInternalQuery->Close();

    try
	{
        ibInternalQuery->SQL->Text = "SELECT "
                                        "CTP.CLIPP_TAB_PAYMENT_KEY, "
                                        "CTP.CLIPP_TAB_KEY, "
                                        "CTP.CLIPP_PAYMENT_REF, "
                                        "CTP.CLIPP_PAYMENT, "
                                        "CTP.CLIPP_PAYMENT_TYPE "
                                    "FROM CLIPP_TAB_PAYMENTS CTP "
                                    "INNER JOIN CLIPP_TAB CT ON CTP.CLIPP_TAB_KEY = CT.CLIPP_TAB_KEY "
                                    "INNER JOIN TAB T ON CT.CLIPP_TAB_TAB_KEY = T.TAB_KEY "
                                    "WHERE T.TAB_KEY = :TAB_KEY AND (CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE OR "
                                    "CTP.CLIPP_PAYMENT_TYPE = :CLIPP_PROCESSING_FEE OR  CTP.CLIPP_PAYMENT_TYPE = :CLIPP_TIP_TYPE) ";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = ClippPayment;
        ibInternalQuery->ParamByName("CLIPP_PROCESSING_FEE")->AsInteger = ClippProcessingFees;
        ibInternalQuery->ParamByName("CLIPP_TIP_TYPE")->AsInteger = ClippTip;
        ibInternalQuery->ExecQuery();

        while (!ibInternalQuery->Eof)
        {
            clippPayments = clippPayments + ibInternalQuery->FieldByName("CLIPP_PAYMENT")->AsCurrency;
            ibInternalQuery->Next();
        }
    }
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

    return clippPayments;
}

//---------------------------------------------------------------------------

void TDBClippTab::GetOpenClippTabs(Database::TDBTransaction &dbTransaction, TStringList * tabList, TMMTabType tabType)
{
	try
	{
        //Clear out all the items if there in the list..
        tabList->Clear();

		TIBSQL *iBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

       iBInternalQuery->SQL->Text = "SELECT "
                                        "TAB.TAB_KEY, "
                                        "TAB.TAB_NAME NAME, "
                                        "TAB.ID_NUMBER "
                                    "FROM TAB "
                                    "INNER JOIN CLIPP_TAB on TAB.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY "
                                    "WHERE CLIPP_TAB.IS_TAB_CLOSED = 'F' "/* AND TAB.TAB_KEY "
                                                                "NOT IN "
                                                                    "(SELECT "
                                                                            "SEAT.TAB_KEY "
                                                                    "FROM SEAT WHERE SEAT.TAB_KEY IS NOT NULL) "
                                        " AND ( TAB.TAB_TYPE = " + IntToStr(tabType) + " OR TAB.TAB_TYPE = "+ IntToStr(TabCashAccount) + " ) " + */
                                    " ORDER BY " "LOWER(TAB.TAB_NAME)";

     	iBInternalQuery->ExecQuery();

		while (!iBInternalQuery->Eof)
		{
			int index = tabList->Add(iBInternalQuery->FieldByName("NAME")->AsString);
			tabList->Objects[index] = (TObject*)iBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
			iBInternalQuery->Next();
		}


	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

//---------------------------------------------------------------------------

int TDBClippTab::GetTabKeyByClippTabRef(Database::TDBTransaction &dbTransaction, UnicodeString clippTabRef)
{
    int tabKey = 0;
    try
	{
		TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT "
                                        "CLIPP_TAB.CLIPP_TAB_TAB_KEY "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_REF = :CLIPP_TAB_REF";

        ibInternalQuery->ParamByName("CLIPP_TAB_REF")->AsString = clippTabRef;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            tabKey = ibInternalQuery->FieldByName("CLIPP_TAB_TAB_KEY")->AsInteger;
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return tabKey;
}

//---------------------------------------------------------------------------

UnicodeString TDBClippTab::GetClippTabRefByTabKey(Database::TDBTransaction &dbTransaction, int tabKey)
{
    UnicodeString clippTabRef = "";
    try
	{
		TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT "
                                        "CLIPP_TAB.CLIPP_TAB_REF "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :CLIPP_TAB_TAB_KEY";

        ibInternalQuery->ParamByName("CLIPP_TAB_TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            clippTabRef = ibInternalQuery->FieldByName("CLIPP_TAB_REF")->AsString;
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return clippTabRef;
}

//---------------------------------------------------------------------------

int TDBClippTab::GetClippTabKeyByClippTabRef(Database::TDBTransaction &dbTransaction, UnicodeString clippTabRef)
{
    int clippTabKey = 0;
    try
	{
		TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = " SELECT "
                                        "CLIPP_TAB.CLIPP_TAB_KEY "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_REF = :CLIPP_TAB_REF";

        ibInternalQuery->ParamByName("CLIPP_TAB_REF")->AsString = clippTabRef;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            clippTabKey = ibInternalQuery->FieldByName("CLIPP_TAB_KEY")->AsInteger;
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return clippTabKey;
}

//---------------------------------------------------------------------------

int TDBClippTab::GetClippTabKeyByTabKey(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	int clippTabKey = 0;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
                                        "CLIPP_TAB.CLIPP_TAB_KEY "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :TAB_KEY";

        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
		IBInternalQuery->ExecQuery();

        clippTabKey = IBInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return clippTabKey;
}

//---------------------------------------------------------------------------

int TDBClippTab::GetTabKeyByClippTabKey(Database::TDBTransaction &dbTransaction, int clippTabKey)
{
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	int tabKey = 0;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
                                        "CLIPP_TAB.CLIPP_TAB_TAB_KEY "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_KEY = :CLIPP_TAB_KEY";

        IBInternalQuery->ParamByName("CLIPP_TAB_KEY")->AsInteger = clippTabKey;
		IBInternalQuery->ExecQuery();

        tabKey = IBInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return tabKey;
}

//---------------------------------------------------------------------------
int TDBClippTab::GetClippTabByTabKey(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	int tabType = 0;
	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "SELECT "
                                        "TAB.TAB_TYPE "
                                    "FROM TAB "
                                    "WHERE TAB.TAB_KEY = :TAB_KEY";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
		ibInternalQuery->ExecQuery();

        tabType = ibInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return tabType;
}

//---------------------------------------------------------------------------
bool TDBClippTab::IsTabClosed(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	bool isTabClosed = false;
	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "SELECT "
                                        "CLIPP_TAB.IS_TAB_CLOSED "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :TAB_KEY";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
		ibInternalQuery->ExecQuery();

        isTabClosed = ibInternalQuery->Fields[0]->AsString == "F" ? false : true;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return isTabClosed;
}

//---------------------------------------------------------------------------
bool TDBClippTab::IsTabExist(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	bool isTabExist = false;
	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "SELECT "
                                        "CLIPP_TAB.CLIPP_TAB_TAB_KEY "
                                    "FROM CLIPP_TAB "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :TAB_KEY";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
		ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            isTabExist = true;
        }

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return isTabExist;
}

//---------------------------------------------------------------------------
UnicodeString TDBClippTab::GetClippCustomerPhotoURl(Database::TDBTransaction &dbTransaction, int tabKey)
{
    UnicodeString customerPhotoUrl = "";
    try
	{
		TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT CLIPP_TAB.CUSTOMER_PHOTO_URL "
                                     "FROM CLIPP_TAB "
                                     "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :CLIPP_TAB_TAB_KEY ";

        ibInternalQuery->ParamByName("CLIPP_TAB_TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            customerPhotoUrl = ibInternalQuery->FieldByName("CUSTOMER_PHOTO_URL")->AsString;
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return customerPhotoUrl;
}

//---------------------------------------------------------------------------
UnicodeString TDBClippTab::GetCustomerName(Database::TDBTransaction &dbTransaction, int tabKey)
{
    UnicodeString customerName = "";
    try
	{
		TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

        ibInternalQuery->Close();
        ibInternalQuery->SQL->Text = "SELECT CLIPP_TAB.CUSTOMER_FIRST_NAME || '-' || CLIPP_TAB.CUSTOMER_LAST_NAME as CNAME "
                                     "FROM CLIPP_TAB "
                                     "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :CLIPP_TAB_TAB_KEY ";

        ibInternalQuery->ParamByName("CLIPP_TAB_TAB_KEY")->AsInteger = tabKey;
        ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            customerName = ibInternalQuery->FieldByName("CNAME")->AsString;
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return customerName;
}

//---------------------------------------------------------------------------
bool TDBClippTab::IsProcessingFeeAlreadyApplied(Database::TDBTransaction &dbTransaction, int tabKey, int PaymentType)
{
    int clippTabKey = GetClippTabKeyByTabKey(dbTransaction, tabKey);
    if(clippTabKey <= 0)
    {
        throw;
    }
    bool isFeeApplied = false;

    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "SELECT "
                                        "CLIPP_TAB_PAYMENTS.CLIPP_PAYMENT "
                                    "FROM CLIPP_TAB_PAYMENTS "
                                    "WHERE CLIPP_TAB_PAYMENTS.CLIPP_TAB_KEY = :TAB_KEY AND "
                                    "CLIPP_TAB_PAYMENTS.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE ";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = clippTabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = PaymentType;
		ibInternalQuery->ExecQuery();

        if(ibInternalQuery->RecordCount)
        {
            if(ibInternalQuery->Fields[0]->AsCurrency > 0)
            {
                isFeeApplied = true;
            }
        }

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return isFeeApplied;
}

//-------------------------------------------------------------------------------------------------
void TDBClippTab::UpdateProcessingFeeAmount(Database::TDBTransaction &dbTransaction, Currency processingFee, int tabKey, int PaymentType)
{
    int clippTabKey = GetClippTabKeyByTabKey(dbTransaction, tabKey);
    if(clippTabKey <= 0)
    {
        throw;
    }
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "UPDATE CLIPP_TAB_PAYMENTS "
                                        "SET CLIPP_TAB_PAYMENTS.CLIPP_PAYMENT = :CLIPP_PAYMENT "
                                    "WHERE CLIPP_TAB_PAYMENTS.CLIPP_TAB_KEY = :TAB_KEY AND "
                                    "CLIPP_TAB_PAYMENTS.CLIPP_PAYMENT_TYPE = :CLIPP_PAYMENT_TYPE ";

        ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = clippTabKey;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT_TYPE")->AsInteger = PaymentType;
        ibInternalQuery->ParamByName("CLIPP_PAYMENT")->AsCurrency = processingFee;
		ibInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

//-------------------------------------------------------------------------------------------------
void TDBClippTab::UpdateClippTabTabKey(Database::TDBTransaction &dbTransaction, int sourceTabKey, int destTabKey)
{
    if(destTabKey <= 0 || sourceTabKey <= 0)
    {
        throw;
    }
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

	try
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "UPDATE CLIPP_TAB "
                                        "SET CLIPP_TAB.CLIPP_TAB_TAB_KEY = :DEST_TAB_KEY "
                                    "WHERE CLIPP_TAB.CLIPP_TAB_TAB_KEY = :SOURCE_TAB_KEY  ";

        ibInternalQuery->ParamByName("SOURCE_TAB_KEY")->AsInteger = sourceTabKey;
        ibInternalQuery->ParamByName("DEST_TAB_KEY")->AsInteger = destTabKey;
		ibInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

