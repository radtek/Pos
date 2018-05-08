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
};
#endif
