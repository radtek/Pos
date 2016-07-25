//---------------------------------------------------------------------------

#ifndef DBThirdPartyCodesH
#define DBThirdPartyCodesH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
#include "MM_DBCore.h"
enum TThirdPartyCodeType {tpItemSize,tpPaymentType };

class TDBThirdPartyCodes
{
   public:
	static UnicodeString GetThirdPartyDescriptionByCode(Database::TDBTransaction &DBTransaction,UnicodeString ThirdPartyCode);
	static UnicodeString GetThirdPartyDescriptionByKey(Database::TDBTransaction &DBTransaction,int ThirdPartyKey);
	static UnicodeString GetThirdPartyCodeByKey(Database::TDBTransaction &DBTransaction,int ThirdPartyKey);
   static TThirdPartyCodeType GetThirdPartyTypeByKey(Database::TDBTransaction &DBTransaction,int ThirdPartyKey);
	static int GetThirdPartyKeyByCode(Database::TDBTransaction &DBTransaction,UnicodeString ThirdPartyCode,TThirdPartyCodeType Type);
	static int SetThirdPartyCode(Database::TDBTransaction &DBTransaction,UnicodeString ThridPartyCode, UnicodeString Description,TThirdPartyCodeType Type);
   static int SetThirdPartyCodeByKey(Database::TDBTransaction &DBTransaction,int ThridPartyCodeKey,UnicodeString ThridPartyCode, UnicodeString Description,TThirdPartyCodeType Type);
};
#endif
