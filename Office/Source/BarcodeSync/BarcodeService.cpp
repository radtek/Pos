//---------------------------------------------------------------------------
#pragma hdrstop

#include "BarcodeService.h"
#include "MMData.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace BarcodeSync
{
//---------------------------------------------------------------------------
std::auto_ptr<IBarcodeService> BarcodeServiceFactory::Build()
{
	return std::auto_ptr<IBarcodeService>(new BarcodeService(
		dmMMData->dbMenuMate->DatabaseName,
		dmStockData->dbStock->DatabaseName));
}
//---------------------------------------------------------------------------
BarcodeService::BarcodeService(
	AnsiString connectionString,
	AnsiString stockConnectionString)
	:
	menuDatabse(Database::TDBSettings("", connectionString, true)),
	stockDatabase(Database::TDBSettings("", stockConnectionString, true)),
	stockTransaction(stockDatabase),
	sqlStock(new TIBSQL(NULL)),
	sqlAddBarcode(new TIBSQL(NULL))
{
	this->menuDatabse.Connect();
	this->stockDatabase.Connect();

	this->stockTransaction.RegisterQuery(this->sqlStock);
	this->stockTransaction.RegisterQuery(this->sqlAddBarcode);

	this->sqlStock->SQL->Text =
		"select "
			"Stock.Description,"
			"Stock.Stock_Key,"
			"Barcode.Barcode "
		"from "
			"Stock left join Barcode on Barcode.Stock_Key = Stock.Stock_Key "
		"where "
			"Stock.Code = :Code";

	this->sqlAddBarcode->SQL->Text =
		"insert into Barcode "
			"(Barcode, Stock_Key, Supplier_Stock_Key) "
		"values "
			"(:Barcode, :Stock_Key, -1)";

	this->stockTransaction.StartTransaction();
}
//---------------------------------------------------------------------------
BarcodeService::~BarcodeService()
{
	this->stockTransaction.CommitTransaction();
}
//---------------------------------------------------------------------------
Menu::TMenusInfo BarcodeService::GetMenus()
{
	Menu::TMenuLoadDB menuLoader(this->menuDatabse, 0);
	Menu::TMenusInfo menus;
	menuLoader.EnumMenus(menus);

	return menus;
}
//---------------------------------------------------------------------------
BarcodeSynchronizationItems BarcodeService::GetSyncItems(Menu::TMenusInfo& menus)
{
	BarcodeSynchronizationItems syncItems;
	Menu::TMenusInfo::iterator selected = menus.begin();

	while (selected != menus.end())
	{
		Menu::TMenuLoadDB menuLoader(this->menuDatabse, selected->Key);

		Menu::TMenuInfo menuInfo;
		Menu::TSizesInfo sizesInfo;
		Menu::TCategoriesInfo categoriesInfo;
		Menu::TLocationsInfo locationsInfo;
		Menu::TServingCoursesInfo servingCoursesInfo;

		if (menuLoader.GetMenu(&menuInfo, &sizesInfo, &categoriesInfo, &locationsInfo, &servingCoursesInfo))
		{
			Menu::TCourseInfo courseInfo;
			while (menuLoader.GetNextCourse(&courseInfo))
			{
				Menu::TItemInfo itemInfo;
				while (menuLoader.GetNextItem(&itemInfo))
				{
					Menu::TItemSizeInfo itemSizeInfo;
					while (menuLoader.GetNextItemSize(&itemSizeInfo))
					{
                        	if (itemSizeInfo.Barcode != NULL && itemSizeInfo.Barcode != WideString("") && itemSizeInfo.Recipes.size() == 1)
						{
							Menu::TSizeRecipeInfo& recipe = *itemSizeInfo.Recipes.begin();

							BarcodeSynchronizationItem item;

							this->sqlStock->Close();
							this->sqlStock->ParamByName("Code")->AsString = recipe.Stock_Code;

							bool alreadyExists = false;
							for (this->sqlStock->ExecQuery(); !this->sqlStock->Eof; this->sqlStock->Next())
							{
								if (this->sqlStock->FieldByName("Barcode")->AsString == itemSizeInfo.Barcode)
								{
									alreadyExists = true;
									break;
								}

								item.StockItem = this->sqlStock->FieldByName("Description")->AsString;
							}

							if (false == alreadyExists)
							{
								item.StockKey = this->sqlStock->FieldByName("Stock_Key")->AsInteger;
								item.Description = itemInfo.Item_Name + " - " + itemSizeInfo.Size_Name;
								item.StockCode = recipe.Stock_Code;
								item.Barcode = itemSizeInfo.Barcode;

								syncItems.push_back(item);
							}
						}
					}
				}
			}
		}

		selected++;
	}

	return syncItems;
}
//---------------------------------------------------------------------------
bool BarcodeService::Synchronize(BarcodeSynchronizationItems& items)
{
	try
	{
		BarcodeSynchronizationItems::iterator item = items.begin();
		while (item != items.end())
		{
			this->sqlAddBarcode->ParamByName("Stock_Key")->AsString = item->StockKey;
			this->sqlAddBarcode->ParamByName("Barcode")->AsString = item->Barcode;
			this->sqlAddBarcode->ExecQuery();

			item++;
		}

		return true;
	}
	catch (Exception &e)
	{
		this->stockTransaction.Rollback();
		Application->MessageBox(AnsiString("Failed to save barcodes: " + e.Message).c_str(), "Error", MB_OK + MB_ICONERROR);
		return false;
	}
}
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------

