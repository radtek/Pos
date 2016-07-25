//---------------------------------------------------------------------------
#ifndef PermissionH
#define PermissionH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <set>
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
enum PermissionAccess
{
	None = 0,
	MenuEditor = 100,
	StockAdmin = 200,
	Loyalty = 400,
	Setup = 600,

	CashUp = 700,
	AccountsExport = 800,

	Reports = 900,
	MenuReports = 1000,
	FinancialReports = 1100,
	SalesReports = 1200,
	SecurityReports = 1300,
	LoyaltyReports = 500,
	StockReports = 300,
    MYOB = 1500,
    WriteOff = 1600,
    Transfer = 1700,
    Stocktake = 1800,
    Manufacture = 1900,
    DontAllowOfficeSetup = 2000,
    DisallowEAStock    = 2100,
    DisallowEASupplier = 2200,
    DisallowCreatPOrder = 2300,
    DisallowCreatSOrder = 2400,
    DisallowRecvPOrder = 2500,
    DisallowRecvInv = 2600,
    DisallowStockSubstitute = 2700,
    DisallowUpdateSupplierCost = 2800,
    DisallowStockRequest = 2900
};
//---------------------------------------------------------------------------
class Permission
{
public:
	Permission();
	Permission(int permissionId, PermissionAccess access, AnsiString name);
	Permission(const Permission& other);

	virtual ~Permission();

	Permission& operator = (const Permission& other);
	bool operator < (const Permission& other) const;
	bool operator == (const Permission& other) const;
	bool operator != (const Permission& other) const;

	int PermissionId;
	PermissionAccess PermissionAccess;
	AnsiString PermissionName;
};
//---------------------------------------------------------------------------
typedef std::set<Permission> Permissions;
}
//---------------------------------------------------------------------------
#endif
