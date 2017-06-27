//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerPanasonic.h"
#include "PanasonicModels.h"
#include "StringTools.h"
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
    if(panasonicThreadTerminated && IsPendingRecordForPostingToServerExist())
    {
        InitiatePanasonicThread();
        StartPanasonicThread();
    }
    else
        StopPanasonicThreadTimer();
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
void TPanasonicThread::GetMemberNameAndCustomerID(Database::TDBTransaction &dbTransaction, UnicodeString invoiceNo, int &contactKey,
                                                    UnicodeString &memberName)
{
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT PT.CONTACTS_KEY,a.NAME, a.LAST_NAME FROM POINTSTRANSACTIONS PT "
                                        "INNER JOIN CONTACTS a  ON PT.CONTACTS_KEY = A.CONTACTS_KEY "
                                        "WHERE PT.INVOICE_NUMBER = :INVOICE_NUMBER ";
        IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = invoiceNo;
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
        {
            contactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
            memberName = IBInternalQuery->FieldByName("NAME")->AsString + " " + IBInternalQuery->FieldByName("LAST_NAME")->AsString;
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
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
//-------------------------------------------------------------------------------------------------------------------------
TDateTime TPanasonicThread::GetStartDateTime(Database::TDBTransaction &dbTransaction, int arcBillKey)
{
    TDateTime startDate;
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, MIN(a.TIME_STAMP) START_TIME  FROM DAYARCHIVE a  WHERE a.ARCBILL_KEY = :ARCBILL_KEY GROUP BY 1 ";
        IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
        IBInternalQuery->ExecQuery();
        startDate = IBInternalQuery->Fields[1]->AsDateTime;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return startDate;
}
//--------------------------------------------------------------------------------------------------------------------------
void TPanasonicThread::ConvertTransactionInfoToPanasonicInfo(Database::TDBTransaction &dbTransaction )
{
    TPanasonicModels* panasonicModel = new TPanasonicModels();
    TDBPanasonic* dbPanasonic = new TDBPanasonic();
    dbPanasonic->UniDataBaseConnection->Open();
    dbPanasonic->UniDataBaseConnection->StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        TIBSQL *selectPaymentType = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();
        int arcBillKey;
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP, a.TOTAL, a.CONTACTS_KEY, a.INVOICE_NUMBER, "
                                     "a.RECEIPT, SECURITY.USER_KEY "
                                    "FROM ARCBILL a "
                                    "INNER JOIN SECURITY ON A.SECURITY_REF = SECURITY.SECURITY_REF "
                                    "WHERE A.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER AND A.TERMINAL_NAME = :TERMINAL_NAME "
                                    "UNION ALL "
                                    "SELECT a.ARCBILL_KEY, a.TERMINAL_NAME, a.STAFF_NAME, a.TIME_STAMP, a.TOTAL, a.CONTACTS_KEY, a.INVOICE_NUMBER, "
                                    "a.RECEIPT, SECURITY.USER_KEY  "
                                    "FROM DAYARCBILL a "
                                    "INNER JOIN SECURITY ON A.SECURITY_REF = SECURITY.SECURITY_REF "
                                    "WHERE A.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER AND A.TERMINAL_NAME = :TERMINAL_NAME ";
        IBInternalQuery->ParamByName("IS_POSTED_TO_PANASONIC_SERVER")->AsString = "F";
        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
        IBInternalQuery->ExecQuery();

        int contactKey = 0, customerID = 0;
        UnicodeString memberName = "";
        UnicodeString appendString = "*";
        int siteId = GetSiteId(dbTransaction);

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            siteId = 0;
            contactKey = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
            arcBillKey =  IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;
            memberName = "";
            customerID = 0;

            TDateTime startDateTime = GetStartDateTime(dbTransaction, arcBillKey);
            panasonicModel->StoreId               = siteId;
            panasonicModel->Terminald             = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            panasonicModel->OperatorId            = contactKey;
            panasonicModel->OperatorName          = IBInternalQuery->FieldByName("STAFF_NAME")->AsString;
            panasonicModel->TransactionId         = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;

            //GetMemberNameAndCustomerID
            GetMemberNameAndCustomerID(dbTransaction, panasonicModel->TransactionId, customerID, memberName);
            panasonicModel->CustomerId            = customerID;
            panasonicModel->CustomerName          = memberName;
            panasonicModel->TransactionType       = appendString + panasonicModel->OperatorName + appendString;
            panasonicModel->ProductListId         = arcBillKey;
            panasonicModel->TransactionAmount     = IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
            panasonicModel->AgeRestricted         = false;
            panasonicModel->Suspended             = false;
            panasonicModel->CashOut               = false;
            panasonicModel->Cash                  = false;
            panasonicModel->VoidAmount            = 0;
            panasonicModel->TimeZoneOfET          = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            panasonicModel->TimeZoneOfST          = startDateTime;
            panasonicModel->DayLightTimeOfET      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            panasonicModel->DayLightTimeOfST      = startDateTime;
            panasonicModel->StartTime             = startDateTime;
            panasonicModel->EndTime               = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            panasonicModel->CreditCard = false;
            panasonicModel->Cheque = false;
            panasonicModel->EFTPOS = false;
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

            panasonicModel->TenderType = appendString + selectPaymentType->FieldByName("PAY_TYPE")->AsString + appendString;

             for(; !selectPaymentType->Eof; selectPaymentType->Next())
             {
                    //if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 4097)
                    if(HasAllProperties(selectPaymentType->FieldByName("PROPERTIES")->AsString,"1,13,"))
                    {
                        panasonicModel->TenderCash = selectPaymentType->FieldByName("SUBTOTAL")->AsCurrency;
                        panasonicModel->Cash = true;
                    }
                    else if(HasAllProperties(selectPaymentType->FieldByName("PROPERTIES")->AsString,"1,3,4,"))
                    //else if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 13)
                    {
                        panasonicModel->CreditCard = true;
                    }
                    else if(HasAllProperties(selectPaymentType->FieldByName("PROPERTIES")->AsString,"1,"))
                    //else if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 1)
                    {
                        panasonicModel->Cheque = true;
                    }
                    else if(HasAllProperties(selectPaymentType->FieldByName("PROPERTIES")->AsString,"1,2,3,4,"))
                    //else if(selectPaymentType->FieldByName("PROPERTIES")->AsCurrency == 15)
                    {
                        panasonicModel->EFTPOS = true;
                        panasonicModel->CashOut = selectPaymentType->FieldByName("CASH_OUT")->AsString == "T" ? true : false;
                    }
              }

                dbPanasonic->SendDataToServer(*panasonicModel);

                //Convert TransactionInfo to panasonic Item info so that it can be directly posted to their TItemList Table.
                ConvertTransactionInfoToPanasonicItemList(*dbPanasonic, dbTransaction, arcBillKey);

                //fill transaction db server info.
                ConverTransactionInfoToTransactionDBServerInfo(*dbPanasonic, dbTransaction);

                //Update flag if data posted to panasonic server
                 UpdateArcBillAndDayArcBill(dbTransaction, arcBillKey);
        }
        dbPanasonic->UniDataBaseConnection->Commit();
        dbPanasonic->UniDataBaseConnection->Close();

    }
    catch(Exception &E)
	{
        dbPanasonic->UniDataBaseConnection->Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    delete dbPanasonic;
    delete panasonicModel;
}
//----------------------------------------------------------------------------------------------------------------
bool TPanasonicThread::HasAllProperties(AnsiString propertyString,AnsiString allProperties)
{
  return TStringTools::Instance()->HasAllProperties(propertyString,allProperties);;
}
//----------------------------------------------------------------------------------------------------------------
void TPanasonicThread::ConvertTransactionInfoToPanasonicItemList(TDBPanasonic &dbPanasonic, Database::TDBTransaction &dbTransaction, int arcBillKey)
{
    try
    {
      //  TDBPanasonic* dbPanasonic = new TDBPanasonic();
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

            dbPanasonic.InsertItemsToTItemList(*itemList);

            productinfo->ProductCode                = itemList->ProductCode;
            productinfo->ProductDescription         = itemList->ProductDescription;

            dbPanasonic.InsertProductDetailsInToTProduct(*productinfo);
        }
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
void TPanasonicThread::ConverTransactionInfoToTransactionDBServerInfo(TDBPanasonic &dbPanasonic, Database::TDBTransaction &dbTransaction)
{
    TPanasonicTransactionDBServerInformation *serverInfo = new TPanasonicTransactionDBServerInformation();
    serverInfo->PosSystemType              =   "Windows";
    serverInfo->PosSystemName              =   TDeviceRealTerminal::Instance().ID.Name;
    serverInfo->PosSystemVersion           =   GetPOSVersionInfo(dbTransaction);
    serverInfo->TransactioDBServerType     =   "TransactionDBServer";
    serverInfo->TransactionDBServerName    =   "TransactionDBServer For " + serverInfo->PosSystemName;
    serverInfo->TransactionDBServerVersion =   "1.00";
   // TDBPanasonic* dbPanasonic = new TDBPanasonic();
    dbPanasonic.InsertTransactionDBServerInformation(*serverInfo);
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
//--------------------------------------------------------------------------------------------------------------------
bool TManagerPanasonic::IsPendingRecordForPostingToServerExist()
{
    bool isRecordExist = false;

    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY "
                                    "FROM ARCBILL a "
                                    "WHERE A.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER "
                                    "UNION ALL "
                                    "SELECT a.ARCBILL_KEY "
                                    "FROM DAYARCBILL a "
                                    "WHERE A.IS_POSTED_TO_PANASONIC_SERVER = :IS_POSTED_TO_PANASONIC_SERVER ";
        IBInternalQuery->ParamByName("IS_POSTED_TO_PANASONIC_SERVER")->AsString = "F";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            isRecordExist = true;

        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return isRecordExist;
}
//---------------------------------------------------------------------------------------------------------------------------
void TManagerPanasonic::TriggerTransactionSync()
{
    InitiatePanasonicThread();
    StartPanasonicThread();
}
//---------------------------------------------------------------------------------------------------------------------------
void TManagerPanasonic::PrepareTenderTypes()
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    TDBPanasonic* dbPanasonic = new TDBPanasonic();
    dbPanasonic->UniDataBaseConnection->Open();
    dbPanasonic->UniDataBaseConnection->StartTransaction();

    try
    {
        bool isRowExist = false;
        UnicodeString payType = "";
        std::vector <UnicodeString> PayTypes;
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Clear();
        IBInternalQuery->SQL->Text = "SELECT a.PAYMENT_NAME FROM PAYMENTTYPES a GROUP BY 1 ";
        IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            payType = "*" + IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString.SubString(0,47) + "*" ;
            PayTypes.push_back(payType);
        }
        dbPanasonic->InsertTenderTypes(PayTypes);

        dbPanasonic->UniDataBaseConnection->Commit();
        dbPanasonic->UniDataBaseConnection->Close();
    }
    catch(Exception &E)
	{
        dbPanasonic->UniDataBaseConnection->Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TManagerPanasonic::PrepareTransactionTypes()
{
    TDBPanasonic* dbPanasonic = new TDBPanasonic();
    dbPanasonic->UniDataBaseConnection->Open();
    dbPanasonic->UniDataBaseConnection->StartTransaction();

    try
    {
        dbPanasonic->PrepareTransactionTypes();
        dbPanasonic->UniDataBaseConnection->Commit();
        dbPanasonic->UniDataBaseConnection->Close();
    }
     catch(Exception &E)
	{
        dbPanasonic->UniDataBaseConnection->Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}


