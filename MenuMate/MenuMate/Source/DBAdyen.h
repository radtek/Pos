//---------------------------------------------------------------------------

#ifndef DBAdyenH
#define DBAdyenH>
#include <Classes.hpp>
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
};
#endif
