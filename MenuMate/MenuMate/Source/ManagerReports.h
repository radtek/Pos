//---------------------------------------------------------------------------

#ifndef ManagerReportsH
#define ManagerReportsH
//---------------------------------------------------------------------------
#include "Forms.hpp"
#include <vector>
#include <map>
#include "MM_DBCore.h"
#include "Printing.h"

//---------------------------------------------------------------------------
class TBaseTotal
{
public:
	TBaseTotal() :  RawTotal(0),Total(0),Qty(0),Cost(0) {};
	Currency RawTotal;
    Currency SalesTaxContent;
    Currency ServiceChargeTaxContent;
    Currency ServiceChargeContent;
    Currency LocalTaxContent;
    Currency ProfitTaxContent;
    Currency TaxContent;
	Currency Total;
	Currency Cost;
	double Qty;
};

class TSumPayments
{
	public:
	TSumPayments() : Name(""), Total(0),CashOut(0),Rounding(0),Surcharge(0),Properties(0),Qty(0){}
	int Properties;
   int Qty;
	AnsiString Name;
	Currency Total;
	Currency CashOut;
	Currency Surcharge;
	Currency Rounding;
	std::map<int, Currency> Points;
};

//<Cobb Loaf (small)> <Totals for this Item>
typedef std::map<AnsiString , TBaseTotal > TItemTotals;

class TCatTotal
{
public:
   int Category_Key;
	TBaseTotal Totals;
	TItemTotals ItemDetails;
};

//<MM_ALL> <Totals for this Category>
typedef std::map<AnsiString , TCatTotal > TCategoryTotals;

class TCategoryGroupDetails
{
public:
   int Category_Group_Key;
   AnsiString Category_Group_Name;   
	TCategoryTotals Details; // Finanical Categorys MM_ALL etc.
	TBaseTotal Totals; // Total for list list of Finanical Categories.
};

// List of Groups <Group Name MM_TOTAL> <List of Finanical Categorys MM_ALL>
typedef std::map<AnsiString , TCategoryGroupDetails > TCategoryGroups;

class TCategoryGroupsSection
{
public:
	TCategoryGroups Details; // List of CategoryGroups.
	TBaseTotal Totals;	// Total for this list of groups. i.e Billed Sales.
        TBaseTotal SummaTotals;
};

class TFinancialDetails
{
public:
	TCategoryGroupsSection BilledSales;
        TCategoryGroupsSection ComplimentarySales;
	TCategoryGroupsSection SavedSales;
	TCategoryGroupsSection TotalSales;
};

class TCalcTotal
{
   public :
   TCalcTotal(): Name(""), Value(0),Oprand('_'),DisplayOrder(0) {};
 	TCalcTotal(AnsiString inName,Currency inValue, char inOprand,int inDisplayOrder)
      : Name(inName), Value(inValue),Oprand(inOprand),DisplayOrder(inDisplayOrder) {}
   AnsiString Name;
   Currency Value;
   char Oprand;
   int DisplayOrder;
};

class TManagerReports
{
   private :
      TForm *Owner;
      void AddSectionTitle(TPrintout *Printout,AnsiString Title);
	  void AddSubSectionTitle(TPrintout *Printout,AnsiString Title);
	TMemoryStream * ZedToArchive;
   public :
      TManagerReports(TForm * inOwner);
	  void PrintConsumption(Database::TDBTransaction &DBTransaction);
	  void PrintTableTabSummaryReport(Database::TDBTransaction &);
   ~TManagerReports();
	  void PrintConsumptionByMenu(Database::TDBTransaction &DBTransaction, AnsiString SelectedMenu);
	  void PrintFloatAdjustments(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName);
};

#endif
 