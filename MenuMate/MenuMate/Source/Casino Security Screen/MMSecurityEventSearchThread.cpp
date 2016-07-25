//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MMSecurityEventSearchThread.h"

#include "DBTransactionAudit.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMMSecurityEventSearchThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMMSecurityEventSearchThread::TMMSecurityEventSearchThread( TIBDatabase *inDatabase )
    : TThread( true )
{
    ibDatabase = inDatabase;
}
//---------------------------------------------------------------------------
void __fastcall TMMSecurityEventSearchThread::Execute()
{
    if( FOnSecurityTransactionFetch != NULL )
    {
        TDATA_LIST dataList = readDataListSince( DateTime );
        notifyTransactions( dataList );
    }
}
//---------------------------------------------------------------------------
TDATA_LIST TMMSecurityEventSearchThread::readDataListSince( TDateTime inDatetime )
{
    TDBTransactionAudit *transactionAudit = new TDBTransactionAudit();
    Database::TDBTransaction dbTransaction( ibDatabase );

    dbTransaction.StartTransaction();
       TDATA_LIST result = transactionAudit->GetEventsSinceTime( dbTransaction, inDatetime );
    dbTransaction.Commit();

    delete transactionAudit;

    return result;
}
//---------------------------------------------------------------------------
void TMMSecurityEventSearchThread::notifyTransactions( TDATA_LIST inDataList )
{
    TDATA_LIST::reverse_iterator dIT = inDataList.rbegin();

    for( ; dIT != inDataList.rend(); dIT++ )
    {
        transaction_data = &( *dIT );
        Synchronize( notifyRecordFetchedTransaction );

        DateTime = transaction_data->DateTime;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMMSecurityEventSearchThread::notifyRecordFetchedTransaction()
{
    FOnSecurityTransactionFetch( this, transaction_data );
}
//---------------------------------------------------------------------------

