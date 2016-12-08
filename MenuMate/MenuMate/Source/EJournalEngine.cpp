//---------------------------------------------------------------------------


#pragma hdrstop

#include "EJournalEngine.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "ReportManager.h"
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
       //ExteractZedReport(fromSessionDate,toSessionDate);
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
TMemoryStream* TEJournalEngine::ExtractZedReport(TDateTime fromSessionDate,TDateTime toSessionDate)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;;

    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        //IBInternalQuery->Close();
//        IBInternalQuery->SQL->Text="select a.REPORT from ZEDS a Where a.TRANS_DATE >= :From_DATE "
//                                    "and a.TRANS_DATE < :To_DATE " ;
//
//        IBInternalQuery->ParamByName("From_DATE")->AsDateTime = fromSessionDate;
//        IBInternalQuery->ParamByName("To_DATE")->AsDateTime = toSessionDate;
        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate);

        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);
           ManagerReceipt->Receipt->Position++;
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    //CategorizeEJournal(fromSessionDate,toSessionDate);
    return ZedReceipt;
}
//---------------------------------------------------------------------------

void TEJournalEngine::GetZReport(TIBSQL *IBInternalQuery,TDateTime fromSessionDate,TDateTime toSessionDate)
{
    //TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="select a.REPORT, a.Z_KEY from ZEDS a Where a.TRANS_DATE >= :From_DATE "
                                "and a.TRANS_DATE <= :To_DATE " ;
    IBInternalQuery->ParamByName("From_DATE")->AsDateTime = fromSessionDate;
    IBInternalQuery->ParamByName("To_DATE")->AsDateTime = toSessionDate;

    //return IBInternalQuery;
}

TMemoryStream* TEJournalEngine::ExtractZedReceiptReport(TDateTime fromSessionDate,TDateTime toSessionDate)
{
    TMemoryStream *ZedReceipt = new TMemoryStream;
    ZedReceipt->Clear();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBGetReciptQuery = DBTransaction.Query(DBTransaction.AddQuery());

        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate);
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            GetReceipt(IBGetReciptQuery, IBInternalQuery->FieldByName("Z_KEY")->AsInteger);
            IBGetReciptQuery->ExecQuery();
            for (; !IBGetReciptQuery->Eof; IBGetReciptQuery->Next())
            {
               IBGetReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);
               ZedReceipt->Position++;
            }
            IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);
            ZedReceipt->Position++;
        }

    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return ZedReceipt;
}

bool TEJournalEngine::IsXReportGenerate(TIBSQL *IBInternalQuery, int z_key)
{
    bool isXReportGenerate = false;
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text="SELECT a.Z_KEY FROM ZEDS a where a.Z_KEY > :Z_KEY ";
    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = z_key;
    IBInternalQuery->ExecQuery();
    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
       isXReportGenerate = true;
    }
    return isXReportGenerate;
}

TMemoryStream* TEJournalEngine::ExtractZedReceiptAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate)
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

        GetZReport(IBInternalQuery, fromSessionDate, toSessionDate);
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            GetReceipt(IBGetReciptQuery, IBInternalQuery->FieldByName("Z_KEY")->AsInteger);
            IBGetReciptQuery->ExecQuery();
            for (; !IBGetReciptQuery->Eof; IBGetReciptQuery->Next())
            {
               IBGetReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);
               ZedReceipt->Position++;
            }
            IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedReceipt);
            ZedReceipt->Position++;
        }
        if(IsXReportGenerate(IBCheckXReport, IBInternalQuery->FieldByName("Z_KEY")->AsInteger))
        {
           int k = 0;
           // no x - report will generate
        }
        else
        {
            int k = 0;
            GetCurrentRunningReceipt(IBGetCurrentRunningReciptQuery);
            IBGetCurrentRunningReciptQuery->ExecQuery();
            for (; !IBGetCurrentRunningReciptQuery->Eof; IBGetCurrentRunningReciptQuery->Next())
            {
               IBGetCurrentRunningReciptQuery->FieldByName("RECEIPT")->SaveToStream(ZedReceipt);
               ZedReceipt->Position++;
            }
            DisplayXReport(ZedReceipt);
           // x - report will generate
        }
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return ZedReceipt;
}
void TEJournalEngine::GetReceipt(TIBSQL *IBGetReciptQuery, int z_key)
{
    IBGetReciptQuery->Close();
    IBGetReciptQuery->SQL->Text=" SELECT a.RECEIPT, a.Z_KEY FROM ARCBILL a where a.Z_KEY =:Z_KEY" ;
    IBGetReciptQuery->ParamByName("Z_KEY")->AsInteger = z_key;
}

void TEJournalEngine::DisplayXReport(TMemoryStream* XReceipt)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    ReportManager reportManager;
    XReport* xReport = reportManager.GetXReport(&TGlobalSettings::Instance(), &DBTransaction);
    xReport->DisplayAndPrintForEJournal(XReceipt);
    DBTransaction.Commit();
}

void TEJournalEngine::GetCurrentRunningReceipt(TIBSQL *IBGetCurrentRunningReciptQuery)
{
    IBGetCurrentRunningReciptQuery->Close();
    IBGetCurrentRunningReciptQuery->SQL->Text=" SELECT a.ARCBILL_KEY, a.RECEIPT FROM DAYARCBILL a " ;
}



