//---------------------------------------------------------------------------

#ifndef DBAdyenH
#define DBAdyenH>
#include <Classes.hpp>
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TDBAdyen
{
private :

public:
	TDBAdyen();
	~TDBAdyen();
    static UnicodeString GetServiceID();
    static UnicodeString GetTransactionID();
    static UnicodeString GetCompanyName();
    static void SetRefundTransaction(bool isRefund);
    static UnicodeString GetInvoiceNumber(int arcBillKey);
    static UnicodeString GetMerchantAccount(UnicodeString invoiceNumber);
    static void UpdateEFTPOSSettleField(Database::TDBTransaction &DBTransaction, UnicodeString invoiceNumber);
    static bool IsTipFromReceiptAlreadyAdded(int arcBillKey);
    static void UpdateEFTPOSReference(UnicodeString originalReference, UnicodeString modifiedReference, UnicodeString pspReference);
};
#endif
