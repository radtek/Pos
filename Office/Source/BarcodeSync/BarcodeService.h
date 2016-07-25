//---------------------------------------------------------------------------
#ifndef BarcodeServiceH
#define BarcodeServiceH
//---------------------------------------------------------------------------
#include "MM_Menu.h"
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
class BarcodeSynchronizationItem
{
public:

	BarcodeSynchronizationItem() { }

	int StockKey;
	AnsiString Description;
	AnsiString Barcode;
	AnsiString StockCode;
	AnsiString StockItem;

private:
};
//---------------------------------------------------------------------------
typedef std::vector<BarcodeSynchronizationItem> BarcodeSynchronizationItems;
//---------------------------------------------------------------------------
class IBarcodeService
{
public:

	virtual ~IBarcodeService() { }

	virtual Menu::TMenusInfo GetMenus() = 0;

	virtual BarcodeSynchronizationItems GetSyncItems(Menu::TMenusInfo& menus) = 0;

	virtual bool Synchronize(BarcodeSynchronizationItems& items) = 0;

};
//---------------------------------------------------------------------------
class IBarcodeServiceFactory
{
public:

	virtual std::auto_ptr<IBarcodeService> Build() = 0;
};
//---------------------------------------------------------------------------
class BarcodeService : public IBarcodeService
{
public:

	BarcodeService(AnsiString menuConnectionString, AnsiString stockConnectionString);
	virtual ~BarcodeService();

	virtual Menu::TMenusInfo GetMenus();

	virtual BarcodeSynchronizationItems GetSyncItems(Menu::TMenusInfo& menus);

	virtual bool Synchronize(BarcodeSynchronizationItems& items);

private:
	Database::TDBControl menuDatabse;
	Database::TDBControl stockDatabase;

	Database::TDBTransaction stockTransaction;
	Database::TcpIBSQL sqlStock;
	Database::TcpIBSQL sqlAddBarcode;
};
//---------------------------------------------------------------------------
class BarcodeServiceFactory : public IBarcodeServiceFactory
{
public:

	virtual std::auto_ptr<IBarcodeService> Build();
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
