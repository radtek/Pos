//---------------------------------------------------------------------------

#ifndef ListSecurityRefContainerH
#define ListSecurityRefContainerH

#include "SecurityReference.h"
#include "MM_DBCore.h"
#include <Generics.Collections.hpp>

//---------------------------------------------------------------------------
class TListSecurityRefContainer : public TList
{
	int SecurityRef;
	public:
	void SecurityClear();
	void SetSecurityRefNumber(int inSecurityRef);
	int  GetSecurityRefNumber();
	void SecurityDelete(TSecurityReference *in);
	int SecurityAdd(TSecurityReference *in);
	int SecurityUpdate(TSecurityTypes Type,TSecurityReference in);
	void SecurityCopyList(TListSecurityRefContainer *inSecList);
	TSecurityReference *SecurityGet(int Index);
	TSecurityReference *SecurityGetType(TSecurityTypes Type);
	Variant SecurityGetField(TSecurityTypes Type,TSecurityFields Field);
	int SecurityAddUniqueRef(Database::TDBTransaction &DBTransaction,int OrderKey,TSecurityReference *in, bool isTransactioncommit = false);
	void SecurityLoad(Database::TDBTransaction &DBTransaction,int SecRef);
	int SecurityGetOrderCount(Database::TDBTransaction &DBTransaction,int SecRef);

	TListSecurityRefContainer();
	__fastcall virtual ~TListSecurityRefContainer();
};

#endif
