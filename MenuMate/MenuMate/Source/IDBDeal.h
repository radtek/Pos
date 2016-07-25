//---------------------------------------------------------------------------

#ifndef IDBDealH
#define IDBDealH
//---------------------------------------------------------------------------

#include "IDealStorage.h"

class IDBDeal: public IDealStorage
{
	public:
		virtual ~IDBDeal() {}

		virtual bool Save(
                    Database::TDBTransaction &DBTransaction,
                    IStoredDeal *inItem) = 0;

		virtual bool Update(
                    Database::TDBTransaction &DBTransaction,
                    IStoredDeal *inItem) = 0;

		virtual bool Delete(
                    Database::TDBTransaction &DBTransaction,
                    IStoredDeal *inItem) = 0;
};

#endif
