//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBPanasonic.h"
#include "MMLogging.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "SQLServerUniProvider"
#pragma link "Uni"
#pragma link "UniProvider"
#pragma link "MemDS"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmDBPanasonic::TfrmDBPanasonic(TComponent* Owner)
    : TForm(Owner)
{

    if(!UniDataBaseConnection->Connected)
    {
        UniDataBaseConnection->ConnectString = "Provider Name=SQL Server;Data Source="+ TGlobalSettings::Instance().PanasonicServerIP + ";Initial Catalog=TRANSACTIONDB;User ID=TransactSrv;Password=Z7uQmcJJrXMBuFf9Fp7T85jAs7TWuyhf";
    }
}
void TfrmDBPanasonic::SendDataToServer(TPanasonicModels &panasonicModels)
{
    try
    {
        TUniQuery *UniInsertQuery = new TUniQuery(Owner);
        UniInsertQuery->Connection = UniDataBaseConnection;
        UniInsertQuery->Close();
        UniInsertQuery->SQL->Clear();
        UniInsertQuery->SQL->Text = "INSERT INTO TTRANSACTION "
                                        "(CompleteReceiptData, StoreId, TerminalId, OperatorId, OperatorName, CustomerId, "
                                        "CustomerName, TransactionId, TransactionType, TenderType, TransactionAmount, VoidAmount, RefundAmount, TenderCash, Suspended, "
                                        "CashOut, AgeRestricted, StartTime, TimeZoneOfST, DayLightTimeOfST, EndTime, TimeZoneOfET, DayLightTimeOfET , Cash, CreditCard, Cheque, "
                                        "EFTPOS, GiftCard, IncomeSupport, ClubCard, CanceledOrder, Purchase, OperatorSignOn, OperatorSignOff ) "
                                "VALUES (:CompleteReceiptData, :StoreId, :TerminalId, :OperatorId, :OperatorName, :CustomerId, :CustomerName, :TransactionId, "
                                        ":TransactionType, :TenderType, :TransactionAmount, :VoidAmount, :RefundAmount, :TenderCash, :Suspended, :CashOut, :AgeRestricted, "
                                        ":StartTime, :TimeZoneOfST, :DayLightTimeOfST, :EndTime, :TimeZoneOfET, :DayLightTimeOfET, :Cash, :CreditCard, :Cheque, :EFTPOS, :GiftCard, :IncomeSupport, "
                                        ":ClubCard, :CanceledOrder, :Purchase, :OperatorSignOn, :OperatorSignOff )";

        UniInsertQuery->ParamByName("CompleteReceiptData")->AsString    =  panasonicModels.LastReceipt;
        UniInsertQuery->ParamByName("StoreId")->AsAnsiString            =  (panasonicModels.StoreId).SubString (0,7);
        UniInsertQuery->ParamByName("TerminalId")->AsAnsiString         =  (panasonicModels.Terminald).SubString (0,7);
        UniInsertQuery->ParamByName("OperatorId")->AsAnsiString         =  (panasonicModels.OperatorId).SubString(0,7);
        UniInsertQuery->ParamByName("OperatorName")->AsAnsiString       =  panasonicModels.OperatorName != "" ? (panasonicModels.OperatorName).SubString (0,31):panasonicModels.OperatorName;
        UniInsertQuery->ParamByName("CustomerId")->AsAnsiString         =  panasonicModels.CustomerId != "" ? (panasonicModels.CustomerId).SubString (0,7):panasonicModels.CustomerId;
        UniInsertQuery->ParamByName("CustomerName")->AsAnsiString       =  panasonicModels.CustomerName != "" ? (panasonicModels.CustomerName).SubString (0,31) : panasonicModels.CustomerName;
        UniInsertQuery->ParamByName("TransactionId")->AsAnsiString      =  (panasonicModels.TransactionId).SubString (0,7);
        UniInsertQuery->ParamByName("TransactionType")->AsAnsiString    =  (panasonicModels.TransactionType).SubString (0,149);
        UniInsertQuery->ParamByName("TenderType")->AsAnsiString         =  panasonicModels.TenderType != "" ? (panasonicModels.TenderType).SubString (0,149) : panasonicModels.TenderType;
        UniInsertQuery->ParamByName("TransactionAmount")->AsCurrency    =  panasonicModels.TransactionAmount;
        UniInsertQuery->ParamByName("VoidAmount")->AsCurrency           =  panasonicModels.VoidAmount;
        UniInsertQuery->ParamByName("RefundAmount")->AsCurrency         =  panasonicModels.RefundAmount;
        UniInsertQuery->ParamByName("TenderCash")->AsCurrency           =  panasonicModels.TenderCash;
        UniInsertQuery->ParamByName("Suspended")->AsBoolean             =  panasonicModels.Suspended;
        UniInsertQuery->ParamByName("CashOut")->AsBoolean               =  panasonicModels.CashOut;
        UniInsertQuery->ParamByName("AgeRestricted")->AsBoolean         =  panasonicModels.AgeRestricted;
        UniInsertQuery->ParamByName("StartTime")->AsDateTime            =  panasonicModels.StartTime;
        UniInsertQuery->ParamByName("TimeZoneOfST")->AsDateTime         =  panasonicModels.TimeZoneOfST;
        UniInsertQuery->ParamByName("DayLightTimeOfST")->AsDateTime     =  panasonicModels.DayLightTimeOfST;
        UniInsertQuery->ParamByName("EndTime")->AsDateTime              =  panasonicModels.EndTime;
        UniInsertQuery->ParamByName("TimeZoneOfET")->AsDateTime         =  panasonicModels.TimeZoneOfET;
        UniInsertQuery->ParamByName("DayLightTimeOfET")->AsDateTime     =  panasonicModels.DayLightTimeOfET;
        UniInsertQuery->ParamByName("Cash")->AsBoolean                  =  panasonicModels.Cash;
        UniInsertQuery->ParamByName("CreditCard")->AsBoolean            =  panasonicModels.CreditCard;
        UniInsertQuery->ParamByName("Cheque")->AsBoolean                =  panasonicModels.Cheque;
        UniInsertQuery->ParamByName("EFTPOS")->AsBoolean                =  panasonicModels.EFTPOS;
        UniInsertQuery->ParamByName("GiftCard")->AsBoolean              =  panasonicModels.GiftCard;
        UniInsertQuery->ParamByName("IncomeSupport")->AsBoolean         =  panasonicModels.IncomeSupport;
        UniInsertQuery->ParamByName("ClubCard")->AsBoolean              =  panasonicModels.ClubCard;
        UniInsertQuery->ParamByName("CanceledOrder")->AsBoolean         =  panasonicModels.CanceledOrder;
        UniInsertQuery->ParamByName("Purchase")->AsBoolean              =  panasonicModels.Purchase;
        UniInsertQuery->ParamByName("OperatorSignOn")->AsBoolean        =  panasonicModels.OperatorSignOn;
        UniInsertQuery->ParamByName("OperatorSignOff")->AsBoolean       =  panasonicModels.OperatorSignOff;
        UniInsertQuery->Execute();
        delete UniInsertQuery;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TfrmDBPanasonic::InsertItemsToTItemList(TPanasonicItemList &itemList)
{
    try
    {
        TUniQuery *UniInsertQuery = new TUniQuery(Owner);
        UniInsertQuery->Connection = UniDataBaseConnection;
        UniInsertQuery->Close();
        UniInsertQuery->SQL->Clear();
        UniInsertQuery->SQL->Text = "INSERT INTO TITEMLIST "
                                        "(InternalTransactionID, ProductCode, ProductDescription, UnitPrice, Quantity, Amount, "
                                        "Weight, PriceInquiry, Void, StaffPurchase, Refund, TrainingMode) "
                                "VALUES (:InternalTransactionID, :ProductCode, :ProductDescription, :UnitPrice, :Quantity, :Amount, "
                                        " :Weight, :PriceInquiry, :Void, :StaffPurchase, :Refund, :TrainingMode)";

        UniInsertQuery->ParamByName("InternalTransactionID")->AsLargeInt     =  itemList.InternalTransactionId;
        UniInsertQuery->ParamByName("ProductCode")->AsAnsiString            =  (itemList.ProductCode).SubString (0,31);
        UniInsertQuery->ParamByName("ProductDescription")->AsAnsiString     =  (itemList.ProductDescription).SubString (0,31);
        UniInsertQuery->ParamByName("UnitPrice")->AsCurrency                =  itemList.UnitPrice;
        UniInsertQuery->ParamByName("Quantity")->AsCurrency                 =  itemList.Quantity;
        UniInsertQuery->ParamByName("Amount")->AsCurrency                   =  itemList.Amount;
        UniInsertQuery->ParamByName("Weight")->AsCurrency                   =  itemList.Weight;
        UniInsertQuery->ParamByName("PriceInquiry")->AsBoolean              =  itemList.PriceInquiry;
        UniInsertQuery->ParamByName("Void")->AsBoolean                      =  itemList.Void;
        UniInsertQuery->ParamByName("StaffPurchase")->AsBoolean             =  itemList.StaffPurchase;
        UniInsertQuery->ParamByName("Refund")->AsBoolean                    =  itemList.Refund;
        UniInsertQuery->ParamByName("TrainingMode")->AsBoolean              =  itemList.TrainingMode;
        UniInsertQuery->Execute();
        delete UniInsertQuery;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TfrmDBPanasonic::InsertProductDetailsInToTProduct(TPanasonicProduct &product)
{
    try
    {
        TUniQuery *UniInsertQuery = new TUniQuery(Owner);
        UniInsertQuery->Connection = UniDataBaseConnection;
        UniInsertQuery->Close();
        UniInsertQuery->SQL->Clear();
        UniInsertQuery->SQL->Text = "INSERT INTO TPRODUCT "
                                        "(ProductCode, ProductDescription) "
                                "VALUES (:ProductCode, :ProductDescription)";

        UniInsertQuery->ParamByName("ProductCode")->AsAnsiString            =  (product.ProductCode).SubString (0,31);
        UniInsertQuery->ParamByName("ProductDescription")->AsAnsiString     =  (product.ProductDescription).SubString (0,31);
        UniInsertQuery->Execute();
        delete UniInsertQuery;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TfrmDBPanasonic::InsertTransactionDBServerInformation(TPanasonicTransactionDBServerInformation &serverInfo)
{
    try
    {
        TUniQuery *UniInsertQuery = new TUniQuery(Owner);
        UniInsertQuery->Connection = UniDataBaseConnection;
        UniInsertQuery->Close();
        UniInsertQuery->SQL->Clear();
        UnicodeString posName = "";
        UnicodeString posVersion;
        bool isRowExist = false;

        UniInsertQuery->SQL->Text = "SELECT  PosSystemName, "
                                              "PosSystemVersion "
                                    "FROM tTransactionDBServerInformation ";
        UniInsertQuery->Execute();
        for(;!UniInsertQuery->Eof;UniInsertQuery->Next())
        {
            posName = UniInsertQuery->FieldByName("PosSystemName")->AsString;
            posVersion = UniInsertQuery->FieldByName("PosSystemVersion")->AsString;
            isRowExist = posName == (serverInfo.PosSystemName).SubString (0,49) && posVersion == (serverInfo.PosSystemVersion).SubString (0,11) ? true : false;
            if(isRowExist)
                break;
        }

        if(!isRowExist)
        {
            UniInsertQuery->Close();
            UniInsertQuery->SQL->Clear();
            UniInsertQuery->SQL->Text = "INSERT INTO TTRANSACTIONDBSERVERINFORMATION "
                                            "(PosSystemType, PosSystemName, PosSystemVersion, TransactionDBServerType, TransactionDBServerName, TransactionDBServerVersion) "
                                    "VALUES (:PosSystemType, :PosSystemName, :PosSystemVersion, :TransactionDBServerType, :TransactionDBServerName, :TransactionDBServerVersion)";

            UniInsertQuery->ParamByName("PosSystemType")->AsAnsiString              =  (serverInfo.PosSystemType).SubString (0,49);
            UniInsertQuery->ParamByName("PosSystemName")->AsAnsiString              =  (serverInfo.PosSystemName).SubString (0,49);
            UniInsertQuery->ParamByName("PosSystemVersion")->AsAnsiString           =  (serverInfo.PosSystemVersion).SubString (0,11);
            UniInsertQuery->ParamByName("TransactionDBServerType")->AsAnsiString    =  (serverInfo.TransactioDBServerType).SubString (0,49);
            UniInsertQuery->ParamByName("TransactionDBServerName")->AsAnsiString    =  (serverInfo.TransactionDBServerName).SubString (0,49);
            UniInsertQuery->ParamByName("TransactionDBServerVersion")->AsAnsiString =  (serverInfo.TransactionDBServerVersion).SubString (0,37);
            UniInsertQuery->Execute();
        }
        delete UniInsertQuery;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
