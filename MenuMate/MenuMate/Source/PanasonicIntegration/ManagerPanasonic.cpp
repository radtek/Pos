//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerPanasonic.h"
#include "DBPanasonic.h"
#include "PanasonicModels.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerPanasonic* TManagerPanasonic::instance = NULL;

TManagerPanasonic::TManagerPanasonic()
    :panasonicThreadTerminated(true),
     panasonicThread(NULL)
{
    panasonicThreadTimer = new TTimer(NULL);
    panasonicThreadTimer->Enabled = false;
    panasonicThreadTimer->Interval = 1000;
    panasonicThreadTimer->OnTimer = OnPanasonicThreadTimerTick;

    StartPanasonicThreadTimer();
}
//-------------------------------------------------------------------------------------------
TManagerPanasonic::~TManagerPanasonic()
{
    delete panasonicThread;
    delete panasonicThreadTimer;
}
//---------------------------------------------------------------------------
TPanasonicThread::TPanasonicThread() : TThread(true)
{
    FreeOnTerminate = true;
}
//-------------------------------------------------------------------------------
void __fastcall TManagerPanasonic::PanasonicTheadTerminate( TObject* sender )
{
    panasonicThreadTerminated = true;
    StartPanasonicThreadTimer();
}
//---------------------------------------------------------------------------
void TManagerPanasonic::InitiatePanasonicThread()
{
    // initiate the thread if it is invalid
    if( panasonicThreadTerminated )
    {
        panasonicThread = new TPanasonicThread();
        panasonicThread->OnTerminate = PanasonicTheadTerminate;
    }
}
//-----------------------------------------------------------
void __fastcall TManagerPanasonic::OnPanasonicThreadTimerTick(TObject *Sender)
{
    if(panasonicThreadTerminated )
    {
        InitiatePanasonicThread();
        StartPanasonicThread();
    }
    else
    {
        StopPanasonicThreadTimer();
    }
}
//------------------------------------------------------------------------------------------------------
void TManagerPanasonic::StartPanasonicThread()
{
    if( panasonicThreadTerminated )
    {
        panasonicThreadTerminated = false;
        StopPanasonicThreadTimer();
        panasonicThread->Start();
    }
}
//---------------------------------------------------------------------------
void TManagerPanasonic::StartPanasonicThreadTimer()
{
    panasonicThreadTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void TManagerPanasonic::StopPanasonicThreadTimer()
{
    panasonicThreadTimer->Enabled = false;
}
//-------------------------------------------------------------------------------------
void  __fastcall TPanasonicThread::Execute()
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        ConvertTransactionInfoToPanasonicInfo(dbTransaction );
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
UnicodeString TPanasonicThread::GetMemberName(Database::TDBTransaction &dbTransaction, int contactKey)
{
    UnicodeString Retval = "";
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT a.SITE_ID, a.FIRST_NAME, a.LAST_NAME FROM CONTACTS a where a.CONTACTS_KEY = :CONTACTS_KEY ";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
        {
            Retval = IBInternalQuery->FieldByName("FIRST_NAME")->AsString;
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return Retval;
}
//----------------------------------------------------------------------------------------------------------------------
int TPanasonicThread::GetSiteId(Database::TDBTransaction &dbTransaction)
{
    int Retval;
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT INTEGER_VAL FROM VARSPROFILE where VARSPROFILE.VARIABLES_KEY = :VARIABLES_KEY";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 9;
        IBInternalQuery->ExecQuery();
        Retval = IBInternalQuery->RecordCount > 0 ? IBInternalQuery->Fields[0]->AsInteger : 0;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return Retval;
}
//--------------------------------------------------------------------------------------------------------------------------
void TPanasonicThread::ConvertTransactionInfoToPanasonicInfo(Database::TDBTransaction &dbTransaction )
{
    try
    {
        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *selectPaymentType = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();
        int arcBillKey;
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP, a.TOTAL, a.CONTACTS_KEY, a.INVOICE_NUMBER, "
                                     "a.RECEIPT, SECURITY.USER_KEY , ARC.LOYALTY_KEY "
                                    "FROM ARCBILL a "
                                    "INNER JOIN SECURITY ON A.SECURITY_REF = SECURITY.SECURITY_REF "
                                    "INNER JOIN (SELECT AB.ARCBILL_KEY, ARC.LOYALTY_KEY FROM ARCBILL AB INNER JOIN ARCHIVE ARC ON "
                                    "AB.ARCBILL_KEY = ARC.ARCBILL_KEY WHERE AB.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER GROUP BY 1,2)ARC ON "
                                        "A.ARCBILL_KEY = ARC.ARCBILL_KEY "
                                    "WHERE A.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER "
                                    "UNION ALL "
                                    "SELECT a.ARCBILL_KEY, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP, a.TOTAL, a.CONTACTS_KEY, a.INVOICE_NUMBER, "
                                    "a.RECEIPT, SECURITY.USER_KEY, ARC.LOYALTY_KEY  "
                                    "FROM DAYARCBILL a "
                                    "INNER JOIN SECURITY ON A.SECURITY_REF = SECURITY.SECURITY_REF "
                                    "INNER JOIN (SELECT AB.ARCBILL_KEY, ARC.LOYALTY_KEY FROM DAYARCBILL AB INNER JOIN DAYARCHIVE ARC ON "
                                    "AB.ARCBILL_KEY = ARC.ARCBILL_KEY WHERE AB.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER GROUP BY 1,2)ARC ON "
                                        "A.ARCBILL_KEY = ARC.ARCBILL_KEY "
                                    "WHERE A.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER ";
        IBInternalQuery->ParamByName("IS_POSTED_TO_PANASONIC_SERVER")->AsString = "F";
        IBInternalQuery->ExecQuery();

        TPanasonicModels* panasonicModel = new TPanasonicModels();
        TDBPanasonic* dbPanasonic = new TDBPanasonic();
        int contactKey = 0, loyaltyKey = 0;
        int siteId = GetSiteId(dbTransaction);

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            siteId = 0;
            contactKey = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
            arcBillKey =  IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;
            loyaltyKey = IBInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;
            panasonicModel->StoreId               = siteId;
            panasonicModel->Terminald             = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            panasonicModel->OperatorId            = contactKey;
            panasonicModel->OperatorName          = IBInternalQuery->FieldByName("STAFF_NAME")->AsString;
            panasonicModel->CustomerId            = loyaltyKey;
            panasonicModel->CustomerName          = GetMemberName(dbTransaction, loyaltyKey);
            panasonicModel->TransactionId         = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
            panasonicModel->TransactionType       = "Billed By";
            panasonicModel->ProductListId         = arcBillKey;
            panasonicModel->TransactionAmount     = IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
            panasonicModel->AgeRestricted         = false;
            panasonicModel->Suspended             = false;
            panasonicModel->CashOut               = false;
            panasonicModel->Cash                  = false;
            panasonicModel->TimeZoneOfET          = Now();
            panasonicModel->TimeZoneOfST          = Now();
            panasonicModel->DayLightTimeOfET      = Now();
            panasonicModel->DayLightTimeOfST      = Now();
            panasonicModel->StartTime             = Now();
            panasonicModel->EndTime               = Now();
            panasonicModel->CreditCard = false;
            panasonicModel->Cheque = false;
            panasonicModel->EFTPOS = false;
            //      Receipt->Clear();
            //      IBInternalQuery->FieldByName("RECEIPT")->SaveToStream(Receipt);
            panasonicModel->LastReceipt           = IBInternalQuery->FieldByName("RECEIPT")->AsString;

            if(IBInternalQuery->FieldByName("TOTAL")->AsCurrency < 0)
            {
                panasonicModel->RefundAmount = IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                panasonicModel->Purchase = false;
            }
            else
            {
                panasonicModel->RefundAmount = 0;
                panasonicModel->Purchase = true;
            }

            selectPaymentType->Close();
            selectPaymentType->SQL->Text = "SELECT a.ARCBILL_KEY, A.PAY_TYPE, A.SUBTOTAL, A.CASH_OUT, a.PROPERTIES "
                                     "FROM DAYARCBILLPAY a "
                                     "WHERE A.NOTE != :NOTE AND A.ARCBILL_KEY = :ARCBILL_KEY "
                                     "UNION ALL "
                                     "SELECT a.ARCBILL_KEY, A.PAY_TYPE, A.SUBTOTAL, A.CASH_OUT, a.PROPERTIES "
                                     "FROM ARCBILLPAY a "
                                     "WHERE A.NOTE != :NOTE AND A.ARCBILL_KEY = :ARCBILL_KEY ";

            selectPaymentType->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
            selectPaymentType->ParamByName("NOTE")->AsString = "Total Change.";
            selectPaymentType->ExecQuery();

             for(; !selectPaymentType->Eof; selectPaymentType->Next())
             {
                    if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 4097)
                    {
                        panasonicModel->TenderCash = selectPaymentType->FieldByName("SUBTOTAL")->AsCurrency;
                        panasonicModel->Cash = true;
                    }
                    else if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 13)
                    {
                        panasonicModel->CreditCard = true;
                    }
                    else if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 1)
                    {
                        panasonicModel->Cheque = true;
                    }
                    else if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 15)
                    {
                        panasonicModel->EFTPOS = true;
                        panasonicModel->CashOut = selectPaymentType->FieldByName("CASH_OUT")->AsString == "T" ? true : false;
                    }
              }

                dbPanasonic->SendDataToServer(*panasonicModel);

                //Convert TransactionInfo to panasonic Item info so that it can be directly posted to their TItemList Table.
                ConvertTransactionInfoToPanasonicItemList(dbTransaction, arcBillKey);

                //fill transaction db server info.
                ConverTransactionInfoToTransactionDBServerInfo(dbTransaction);

                //Update flag if data posted to panasonic server
                 UpdateArcBillAndDayArcBill(dbTransaction, arcBillKey);
        }
         delete dbPanasonic;
         delete panasonicModel;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

}
//----------------------------------------------------------------------------------------------------------------
void TPanasonicThread::ConvertTransactionInfoToPanasonicItemList(Database::TDBTransaction &dbTransaction, int arcBillKey)
{
    try
    {
        TDBPanasonic* dbPanasonic = new TDBPanasonic();
        TPanasonicItemList *itemList = new TPanasonicItemList();
        UnicodeString itemName = "" , sizename = "";

        TPanasonicProduct *productinfo = new TPanasonicProduct();

        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, a.ITEM_NAME, a.SIZE_NAME, a.PRICE, a.PRICE_LEVEL0, a.PRICE_LEVEL1, "
                                        "a.ORDER_TYPE, a.PRICE_ADJUST, a.HAPPY_HOUR, a.QTY, a.THIRDPARTYCODES_KEY "
                                     "FROM ARCHIVE a "
                                     "WHERE A.ARCBILL_KEY = :ARCBILL_KEY "
                                     "UNION ALL "
                                     "SELECT a.ARCBILL_KEY, a.ITEM_NAME, a.SIZE_NAME, a.PRICE, a.PRICE_LEVEL0, a.PRICE_LEVEL1, "
                                     "a.ORDER_TYPE, a.PRICE_ADJUST, a.HAPPY_HOUR, a.QTY, a.THIRDPARTYCODES_KEY "
                                     "FROM DAYARCHIVE a "
                                     "WHERE A.ARCBILL_KEY = :ARCBILL_KEY ";
        IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
        IBInternalQuery->ExecQuery();

        //For inserting items in titemlist Table
        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            itemName = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
            sizename = IBInternalQuery->FieldByName("SIZE_NAME")->AsString;
            itemList->ProductCode                = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;
            itemList->ProductDescription         = itemName + " " + sizename ;
            itemList->InternalTransactionId      = arcBillKey;
            itemList->UnitPrice                  = IBInternalQuery->FieldByName("HAPPY_HOUR")->AsString == "T" ? IBInternalQuery->FieldByName("PRICE_LEVEL1")->AsCurrency
                                                    : IBInternalQuery->FieldByName("PRICE_LEVEL0")->AsCurrency;
            itemList->Quantity                   = IBInternalQuery->FieldByName("QTY")->AsCurrency;
            itemList->Amount                     = itemList->Quantity*itemList->UnitPrice;
            itemList->Weight                     = 0;
            itemList->PriceInquiry               = false;
            itemList->Void                       = IBInternalQuery->FieldByName("ORDER_TYPE")->AsInteger == 2 ? true : false;
            itemList->StaffPurchase              = false;
            itemList->Refund                     = itemList->Quantity > 0 ? false : true;
            itemList->TrainingMode               = false;

            dbPanasonic->InsertItemsToTItemList(*itemList);

            productinfo->ProductCode                = itemList->ProductCode;
            productinfo->ProductDescription         = itemList->ProductDescription;

            dbPanasonic->InsertProductDetailsInToTProduct(*productinfo);
        }
        delete dbPanasonic;
        delete itemList;
        delete productinfo;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//----------------------------------------------------------------------------------------
void TPanasonicThread::ConverTransactionInfoToTransactionDBServerInfo(Database::TDBTransaction &dbTransaction)
{
    TPanasonicTransactionDBServerInformation *serverInfo = new TPanasonicTransactionDBServerInformation();
    serverInfo->PosSystemType              =   "Windows";
    serverInfo->PosSystemName              =   TDeviceRealTerminal::Instance().ID.Name;
    serverInfo->PosSystemVersion           =   GetPOSVersionInfo(dbTransaction);
    serverInfo->TransactioDBServerType     =   "TransactionDBServer";
    serverInfo->TransactionDBServerName    =   "TransactionDBServer For " + serverInfo->PosSystemName;
    serverInfo->TransactionDBServerVersion =   "1.00";
    TDBPanasonic* dbPanasonic = new TDBPanasonic();
    dbPanasonic->InsertTransactionDBServerInformation(*serverInfo);
    delete dbPanasonic;
    delete serverInfo;
}
//--------------------------------------------------------------------------------------------
UnicodeString TPanasonicThread::GetPOSVersionInfo(Database::TDBTransaction &dbTransaction)
{
    UnicodeString versionInfo = "";
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT   MAX(a.VERSION_NUMBER) FROM VERSIONHISTORY a ";
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
            versionInfo = IBInternalQuery->Fields[0]->AsString;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return versionInfo;
}
//---------------------------------------------------------------------------------------------------
void TPanasonicThread::UpdateArcBillAndDayArcBill(Database::TDBTransaction &dbTransaction, int arcBillKey)
{
    try
    {
        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "UPDATE ARCBILL SET IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER "
                                      "WHERE ARCBILL.ARCBILL_KEY = :ARCBILL_KEY AND ARCBILL.IS_POSTED_TO_PANASONIC_SERVER = 'F' ";

        IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
        IBInternalQuery->ParamByName("IS_POSTED_TO_PANASONIC_SERVER")->AsString = "T";
        IBInternalQuery->ExecQuery();

        if(!IBInternalQuery->RecordCount)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                                    "UPDATE DAYARCBILL SET IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER "
                                    "WHERE DAYARCBILL.ARCBILL_KEY = :ARCBILL_KEY AND DAYARCBILL.IS_POSTED_TO_PANASONIC_SERVER = 'F' ";
            IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
            IBInternalQuery->ParamByName("IS_POSTED_TO_PANASONIC_SERVER")->AsString = "T";
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

