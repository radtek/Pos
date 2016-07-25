//---------------------------------------------------------------------------
#ifndef ManagerThirdPartyH
#define ManagerThirdPartyH
//---------------------------------------------------------------------------
#include "Manager.h"
#define THRID_PARTY_VERSION "1,0,0"

class TManagerThirdParty : public TManager
{
	private:
	public :
	UnicodeString Version;
	UnicodeString Product;

	static TManagerThirdParty& Instance() {
        static TManagerThirdParty singleton;
        return singleton;
    }

	void Initialise(UnicodeString inProduct);
	TManagerThirdParty();
	UnicodeString GetCSVHeader();
};

#endif
