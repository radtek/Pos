//---------------------------------------------------------------------------


#pragma hdrstop

#include "EJournalEngine.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "ReportManager.h"
#include "ReportUtilities.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TEJournalEngine::TEJournalEngine()
{
}
//---------------------------------------------------------------------------

TEJournalEngine::~TEJournalEngine()
{
}
//---------------------------------------------------------------------------

EJournalType TEJournalEngine::CategorizeEJournal(TDateTime from,TDateTime to)
{
    // Receipts+X+Zed
    if(!TGlobalSettings::Instance().ExcludeReceipt && !TGlobalSettings::Instance().ExcludeXReport )
    {
       journalType = eZEDReceiptX;
    }
    // Receipts+Zed
    if(!TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
    {
       journalType = eZedReceipt;
    }
    //only Zed
    if(TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
    {
       journalType = eZed;
    }
    if(TGlobalSettings::Instance().ExcludeReceipt && !TGlobalSettings::Instance().ExcludeXReport)
    {
       journalType = eZedX;
    }
    return journalType;
}
//---------------------------------------------------------------------------
void TEJournalEngine::CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate)
{
    CategorizeEJournal(fromSessionDate,toSessionDate);
}
//---------------------------------------------------------------------------
TMemoryStream* TEJournalEngine::ExtractZedReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;
    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate, deviceName);

        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return ZedReceipt;
}
//---------------------------------------------------------------------------

void TEJournalEngine::GetZReport(TIBSQL *IBInternalQuery,TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName)
{
    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="select a.REPORT, a.Z_KEY from ZEDS a Where a.TRANS_DATE >= :From_DATE "
                                "and a.TRANS_DATE <= :To_DATE " + terminalNamePredicate +
                                " order by a.Z_KEY ";
    IBInternalQuery->ParamByName("From_DATE")->AsDateTime = fromSessionDate;
    IBInternalQuery->ParamByName("To_DATE")->AsDateTime = toSessionDate;

    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
}

TMemoryStream* TEJournalEngine::ExtractZedReceiptReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;
    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetCurrentRunningReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBCheckCurrentReceipt = DBTransaction.Query(DBTransaction.AddQuery());

        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate, deviceName);
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            GetReceipt(IBGetReciptQuery, IBInternalQuery->FieldByName("Z_KEY")->AsInteger, deviceName);
            IBGetReciptQuery->ExecQuery();
            for (; !IBGetReciptQuery->Eof; IBGetReciptQuery->Next())
            {
               IBGetReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);
            }
            IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);

        }
        if(!IsCurrentReceiptAvailable(IBCheckCurrentReceipt, toSessionDate, deviceName))
        {
            GetCurrentRunningReceipt(IBGetCurrentRunningReciptQuery, deviceName);
            IBGetCurrentRunningReciptQuery->ExecQuery();
            for (; !IBGetCurrentRunningReciptQuery->Eof; IBGetCurrentRunningReciptQuery->Next())
            {
               IBGetCurrentRunningReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);

            }
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return ZedReceipt;
}

bool TEJournalEngine::IsXReportAvailable(TIBSQL *IBInternalQuery, int z_key, AnsiString deviceName)
{
    bool isXReportGenerate = false;

    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    if(z_key > 0)
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text="SELECT a.Z_KEY FROM ZEDS a where a.Z_KEY > :Z_KEY and a.TIME_STAMP is not null " + terminalNamePredicate ;
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = z_key;
        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
        }
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           isXReportGenerate = true;
        }
    }
    if(!isXReportGenerate)
    {
        IBInternalQuery->Close();
        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            terminalNamePredicate = " where a.TERMINAL_NAME = :TERMINAL_NAME ";
        }
        IBInternalQuery->SQL->Text="SELECT a.ARCBILL_KEY FROM DAYARCBILL a " + terminalNamePredicate ;
        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
        }
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount == 0)
            isXReportGenerate = true;
    }
    return isXReportGenerate;
}

TMemoryStream* TEJournalEngine::ExtractZedReceiptAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;
    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBCheckXReport = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetCurrentRunningReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());

        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate, deviceName);
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            GetReceipt(IBGetReciptQuery, IBInternalQuery->FieldByName("Z_KEY")->AsInteger, deviceName);
            IBGetReciptQuery->ExecQuery();
            for (; !IBGetReciptQuery->Eof; IBGetReciptQuery->Next())
            {
               IBGetReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);

            }
            IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);

        }
        if(!IsXReportAvailable(IBCheckXReport, IBInternalQuery->FieldByName("Z_KEY")->AsInteger, deviceName))
        {
            // x - report will generate
            if(!IsCurrentReceiptAvailable(IBGetCurrentRunningReciptQuery, toSessionDate, deviceName))
            {
                GetCurrentRunningReceipt(IBGetCurrentRunningReciptQuery, deviceName);
                IBGetCurrentRunningReciptQuery->ExecQuery();
                for (; !IBGetCurrentRunningReciptQuery->Eof; IBGetCurrentRunningReciptQuery->Next())
                {
                   IBGetCurrentRunningReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);

                }
                DisplayXReport(ZedReceipt);
            }
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return ZedReceipt;
}
void TEJournalEngine::GetReceipt(TIBSQL *IBGetReciptQuery, int z_key, AnsiString deviceName)
{

    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    IBGetReciptQuery->Close();
    IBGetReciptQuery->SQL->Text=" SELECT a.RECEIPT, a.Z_KEY FROM ARCBILL a where a.Z_KEY =:Z_KEY " + terminalNamePredicate + " order by TIME_STAMP " ;
    IBGetReciptQuery->ParamByName("Z_KEY")->AsInteger = z_key;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBGetReciptQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
}

void TEJournalEngine::DisplayXReport(TMemoryStream* XReceipt)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    ReportManager reportManager;
    XReport* xReport = reportManager.GetXReport(&TGlobalSettings::Instance(), &DBTransaction);
    xReport->PopulateXReportForEJournal(XReceipt);
    DBTransaction.Commit();
}

void TEJournalEngine::GetCurrentRunningReceipt(TIBSQL *IBGetCurrentRunningReciptQuery, AnsiString deviceName)
{

    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = " Where a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    IBGetCurrentRunningReciptQuery->Close();
    IBGetCurrentRunningReciptQuery->SQL->Text=" SELECT a.ARCBILL_KEY, a.RECEIPT FROM DAYARCBILL a " + terminalNamePredicate + " order by TIME_STAMP " ;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBGetCurrentRunningReciptQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
}

TMemoryStream* TEJournalEngine::ExtractZedAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;
    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBCheckXReport = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetCurrentRunningReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());

        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate, deviceName);
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);

        }
        if(!IsXReportAvailable(IBCheckXReport, IBInternalQuery->FieldByName("Z_KEY")->AsInteger, deviceName))
        {
            // x - report will generate
            DisplayXReport(ZedReceipt);
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return ZedReceipt;
}

bool TEJournalEngine::IsCurrentReceiptAvailable(TIBSQL *IBInternalQuery, TDateTime toSessionDate, AnsiString deviceName)
{
    bool isCurrentReceiptGenerate = false;

    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT a.TRANS_DATE FROM ZEDS a where a.TRANS_DATE > :TRANS_DATE " + terminalNamePredicate ;
    IBInternalQuery->ParamByName("TRANS_DATE")->AsDateTime = toSessionDate;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
    IBInternalQuery->ExecQuery();
    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
       isCurrentReceiptGenerate = true;
    }

    if(!isCurrentReceiptGenerate)
    {
        IBInternalQuery->Close();
        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            terminalNamePredicate = " where a.TERMINAL_NAME = :TERMINAL_NAME ";
        }
        IBInternalQuery->SQL->Text="SELECT a.ARCBILL_KEY FROM DAYARCBILL a " + terminalNamePredicate ;
        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
        }
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount == 0)
            isCurrentReceiptGenerate = true;
    }
    return isCurrentReceiptGenerate;
}

TMemoryStream* TEJournalEngine::ExtractConsolidatedZedReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;
    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    fromSessionDate = GetStartTimeForConolidatedZed(fromSessionDate, deviceName);
    MessageBox(DateToStr(fromSessionDate), "StartTime", MB_OK + MB_ICONERROR);
    toSessionDate = GetEndTimeForConolidatedZed(toSessionDate, deviceName);
    MessageBox(DateToStr(toSessionDate), "EndTime", MB_OK + MB_ICONERROR);

    TTransactionInfo TransactionInfo;
    TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);
    TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfoForConsolidatedZed(DBTransaction, deviceName, fromSessionDate, toSessionDate);

    ReportManager reportManager;
    ConsolidatedZedReport* consolidatedzedReport = reportManager.GetConsolidatedZedReport(&TGlobalSettings::Instance(), &DBTransaction, &fromSessionDate, &toSessionDate);
    consolidatedzedReport->DisplayAndPrint(ZedReceipt);

    return ZedReceipt;
}

bool TEJournalEngine::CheckZedDataExistsForConolidatedZed(TDateTime from, TDateTime to, AnsiString deviceName)
{
    bool retval = false;
    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT a.Z_KEY FROM ZEDS a where a.TIME_STAMP >= :startTime  and a.TIME_STAMP <= :endTime " + terminalNamePredicate ;
    IBInternalQuery->ParamByName("startTime")->AsDateTime = from;
    IBInternalQuery->ParamByName("endTime")->AsDateTime = to;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
    IBInternalQuery->ExecQuery();
    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
       retval = true;
       break;
    }
    return retval;
}


TDateTime TEJournalEngine::GetStartTimeForConolidatedZed(TDateTime from, AnsiString deviceName)
{
    TDateTime retval = from;
    int z_key = 0;
    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT first(1) a.Z_KEY FROM ZEDS a where a.TIME_STAMP >= :startTime " + terminalNamePredicate ;
    IBInternalQuery->ParamByName("startTime")->AsDateTime = from;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
       z_key = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;
    }
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT first(1) a.TIME_STAMP FROM ARCBILL a where a.Z_KEY = :Z_KEY " + terminalNamePredicate +
                                " order by a.TIME_STAMP ";
    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = z_key;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
       retval = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
    }
    return retval;
}

TDateTime TEJournalEngine::GetEndTimeForConolidatedZed(TDateTime to, AnsiString deviceName)
{
    TDateTime retval = to;
    int z_key = 0;
    AnsiString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        terminalNamePredicate = "and a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT first(1) a.Z_KEY FROM ZEDS a where a.TIME_STAMP <= :endTime  " + terminalNamePredicate +
                                " order by a.TIME_STAMP desc ";
    IBInternalQuery->ParamByName("endTime")->AsDateTime = to;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
       z_key = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;
    }
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT first(1) a.TIME_STAMP FROM ARCBILL a where a.Z_KEY = :Z_KEY " + terminalNamePredicate +
                                " order by a.TIME_STAMP desc ";
    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = z_key;
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        IBInternalQuery->ParamByName("Terminal_Name")->AsString = deviceName;
    }
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
       retval = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
    }
    return retval;
}
