//---------------------------------------------------------------------------

#ifndef DBActiveChitH
#define DBActiveChitH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>

#include "MM_DBCore.h"
#include "PnMOrder.h"
#include "SeatOrders.h"
#include "ChitNumber.h"

class TDBActiveChit
{
private:
	TDBActiveChit();
	~TDBActiveChit();

public:
	static int GetOrCreateActiveChit(Database::TDBTransaction &DBTransaction, TChitNumber ActiveChitNumber);
	static TChitNumber LoadChitNumberFromKey(Database::TDBTransaction &DBTransaction, int ActiveChitNumberKey);
	static bool DeleteActiveChit(Database::TDBTransaction &DBTransaction, int ActiveChitKey);
	static bool IsActiveChitInUse(Database::TDBTransaction &DBTransaction, int ActiveChitKey);

};
//---------------------------------------------------------------------------
#endif
