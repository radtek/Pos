//---------------------------------------------------------------------------
#ifndef MM_StockCoreH
#define MM_StockCoreH
//---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include <Set>
//---------------------------------------------------------------------------
namespace Stock
{
//---------------------------------------------------------------------------
typedef std::set<AnsiString>				TLocationSet;
typedef std::set<AnsiString>::iterator	TiLocationSet;
//---------------------------------------------------------------------------
class TStockControl
{
public:
										TStockControl(TIBDatabase *IBDatabase);
//	virtual							~TStockControl();		// Not Needed

	bool								CreateLocations(std::set<int> const& StockKeys, AnsiString Location);
	bool								GetLocations(TLocationSet& Locations);

protected:
	Database::TDBTransaction	fDBTransaction;
	void								fCreateStockLocation(int StockKey, AnsiString Location);

private:
	Database::TcpIBSQL			sqlFindLocation;
	Database::TcpIBSQL			sqlAddLocation;
	Database::TcpIBSQL			sqlGetStockLocations;

	Database::TcpIBSQL			sqlGetOrderItemDetails;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
