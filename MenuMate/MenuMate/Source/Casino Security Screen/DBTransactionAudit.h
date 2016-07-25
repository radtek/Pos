//---------------------------------------------------------------------------

#ifndef DBTransactionAuditH
#define DBTransactionAuditH

#include "MM_DBCore.h"
#include "MMLogging.h"
#include "MMSecurityEventSearchThread.h"
#include <list>
#include "Analysis.h"

//---------------------------------------------------------------------------
struct TDelayedPaymentData
{
    TDateTime     DateTime;
    UnicodeString TransactionNo;
    UnicodeString Location;
    UnicodeString Terminal;
    UnicodeString Cashier;
    UnicodeString FromTable;
    Currency      Amount;
};
class TDBTransactionAudit
{
public:
    static std::list<TSecurityTransactionData> GetEventsFromTheLastHour(Database::TDBTransaction &DBTransaction);
    static std::list<TSecurityTransactionData> GetEventsSinceTime(Database::TDBTransaction &DBTransaction, TDateTime inSinceTime);
    static std::list<TSecurityTransactionData> GetEventsBetweenTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime);
    static std::list<TSecurityTransactionData> GetEventsSinceTheLastZed(Database::TDBTransaction &DBTransaction);
    static std::list<TDelayedPaymentData> GetAllDelayedPayments(Database::TDBTransaction &DBTransaction);
private:
    static void GetTransactionsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList);
    static void GetFloatAdjustmentsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList);
    static void GetOpenDrawsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList);

    static void GetTransactionsSinceTheLastZed(Database::TDBTransaction &DBTransaction, std::list<TSecurityTransactionData> &outTransactionList);
    static void GetFloatAdjustmentsSinceTheLastZed(Database::TDBTransaction &DBTransaction, std::list<TSecurityTransactionData> &outTransactionList);
    static void GetOpenDrawsSinceTheLastZed(Database::TDBTransaction &DBTransaction, std::list<TSecurityTransactionData> &outTransactionList);
    static void GetCancelItemsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList);

    static void GetDelayedPaymentsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList);
    static void GetDelayedPaymentsSecurityInfo(Database::TDBTransaction &DBTransaction,
                                                         AnsiString InvoiceNumber,
                                                         TDateTime &timestamp,
                                                         AnsiString &userName,
                                                         AnsiString &FromTable);
};
#endif

