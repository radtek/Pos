//---------------------------------------------------------------------------

#ifndef MMSecurityEventSearchThreadH
#define MMSecurityEventSearchThreadH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <list.h>

#include "MM_DBCore.h"

//---------------------------------------------------------------------------

struct TSecurityTransactionData
{
    TDateTime     DateTime;
    UnicodeString TransactionNo;
    UnicodeString Location;
    UnicodeString Terminal;
    UnicodeString Cashier;
    UnicodeString PaymentType;
    Currency      Amount;
    Currency      Cashout;
    Currency      Adjustment;

    bool operator <(const TSecurityTransactionData& a) const
    {
        return (DateTime < a.DateTime);
    }

    bool operator >(const TSecurityTransactionData& a) const
    {
        return (DateTime > a.DateTime);
    }

    static bool comparator(const TSecurityTransactionData& a, const TSecurityTransactionData& b)
    {
        return a > b;
    }
};
//.........................................

typedef void __fastcall ( __closure *TNotifySecurityTransaction )
                        ( System::TObject *inSender, TSecurityTransactionData* inData );

//---------------------------------------------------------------------------

typedef std::list<TSecurityTransactionData> TDATA_LIST;

class TMMSecurityEventSearchThread : public TThread
{
private:
protected:
    TIBDatabase *ibDatabase;
    TSecurityTransactionData   *transaction_data;
    TNotifySecurityTransaction  FOnSecurityTransactionFetch;

    //.............................................

    void __fastcall Execute();

    //.............................................

    TDATA_LIST readDataListSince( TDateTime inDatetime );
    void notifyTransactions( TDATA_LIST inDataList );

    void __fastcall notifyRecordFetchedTransaction();

public:
    __fastcall TMMSecurityEventSearchThread( TIBDatabase *inDatabase );

    TDateTime    DateTime;

	__property TNotifySecurityTransaction OnSecurityTransactionFetch = { read=FOnSecurityTransactionFetch, write=FOnSecurityTransactionFetch };
};
//---------------------------------------------------------------------------
#endif
