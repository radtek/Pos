//---------------------------------------------------------------------------
#ifndef ManagerThorVoucherH
#define ManagerThorVoucherH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include <set>
#include "MM_DBCore.h"

//---------------------------------------------------------------------------
class TManagerThorVoucher
{
   private :
      bool fEnabled;
      void __fastcall SetEnabled(bool value);
      bool __fastcall GetEnabled();

	public:
    bool isThorIndex;
	void Initialise();
   __property bool Enabled  = { read=GetEnabled, write=SetEnabled };
	TManagerThorVoucher();
	~TManagerThorVoucher();

    int SetThorVoucherCategories(Database::TDBTransaction &DBTransaction);
    std::set<int> ThorVoucherKeys(Database::TDBTransaction &DBTransaction);
    void UpdateThorVoucherCategories(Database::TDBTransaction &DBTransaction);
    void InsertThorVoucherCategories(Database::TDBTransaction &DBTransaction,std::set<int> ThorKeys,int categoryKey);

};

extern TManagerThorVoucher *ManagerThorVoucher;

#endif
