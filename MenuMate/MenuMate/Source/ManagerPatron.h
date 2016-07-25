//---------------------------------------------------------------------------

#ifndef ManagerPatronH
#define ManagerPatronH


#include <Grids.hpp>
#include <system.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <memory>
#include <vector>

#include "MM_DBCore.h"
#include "POS_XMLBase.h"
#include "PatronType.h"
//---------------------------------------------------------------------------

class TManagerPatron
{
	private :
	public:
	static TManagerPatron& Instance() {
        static TManagerPatron singleton;
        return singleton;
    }

	int GetCount(Database::TDBTransaction &DBTransaction);
	TPatronType GetPatronTypes(Database::TDBTransaction &DBTransaction, int Key);
	TPatronType GetDefaultPatron(Database::TDBTransaction &DBTransaction);
	void GetPatronTypes(Database::TDBTransaction &DBTransaction,TStringList *List);
	void GetPatronTypes(Database::TDBTransaction &DBTransaction,std::vector<TPatronType> &PatronTypes);
	void GetPatronTypes(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid);
	void SetPatronType(Database::TDBTransaction &DBTransaction,long Key,TPatronType PatronType);
	void SetDefaultPatrons(Database::TDBTransaction &DBTransaction, std::vector<TPatronType> &PatronTypes, int Count);
	int GetTotalPatrons(std::vector<TPatronType> &PatronTypes);	
	void Delete(Database::TDBTransaction &DBTransaction,int Key);
	void ClearDefault(Database::TDBTransaction &DBTransaction);
   void BuildXMLListPatronCounts(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data);
};

#endif
