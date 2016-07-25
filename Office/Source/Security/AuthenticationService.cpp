//---------------------------------------------------------------------------
#pragma hdrstop

#include "AuthenticationService.h"
#include "StockData.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
AuthenticationService::AuthenticationService()
{
}
//---------------------------------------------------------------------------
bool AuthenticationService::Authenticate(const AnsiString& username, const AnsiString& password, Identity& identity) const
{
	bool isAuthenticated = false;

	try
	{
		Database::TDBTransaction transaction(dmStockData->dbStock);
		TIBSQL *query = transaction.Query(transaction.AddQuery());
		transaction.Start();

		try
		{
			query->SQL->Text = "SELECT USER_ID, USER_NAME FROM USERS WHERE USERS.USER_ID = :USER_ID AND USERS.PASSWRD = :PASSWRD";
			query->ParamByName("USER_ID")->AsString = username.UpperCase();
			query->ParamByName("PASSWRD")->AsString = password;

			query->ExecQuery();
			isAuthenticated = (query->RecordCount > 0);
		}
		__finally
		{
			transaction.Commit();
		}

		identity.UserID =  query->FieldByName("USER_ID")->AsString;
		identity.UserName =  query->FieldByName("USER_NAME")->AsString;

		return isAuthenticated;
	}
	catch (...)
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool AuthenticationService::HasPermission(const AnsiString& username, PermissionAccess permission) const
{
	bool hasPermission = false;

	try
	{
		Roles roles = this->GetRoles();
		if (roles.size() == 0)
			return true;

		Database::TDBTransaction transaction(dmStockData->dbStock);
		TIBSQL *query = transaction.Query(transaction.AddQuery());
		transaction.Start();

		try
		{
			query->SQL->Text =
				"SELECT COUNT(*) AS HAS_PERMISSION FROM USERS "
					"INNER JOIN SECURITY_USER_ROLES ON SECURITY_USER_ROLES.USER_ID = USERS.USER_KEY "
					"INNER JOIN SECURITY_ROLE_PERMISSIONS ON SECURITY_ROLE_PERMISSIONS.ROLE_ID = SECURITY_USER_ROLES.ROLE_ID "
					"INNER JOIN SECURITY_PERMISSIONS ON SECURITY_PERMISSIONS.PERMISSION_ID = SECURITY_ROLE_PERMISSIONS.PERMISSION_ID "
				"WHERE USERS.USER_ID = :USER_ID AND SECURITY_PERMISSIONS.PERMISSION_KEY = :PERMISSION_KEY";
			query->ParamByName("USER_ID")->AsString = username.UpperCase();
			query->ParamByName("PERMISSION_KEY")->AsInteger = (int)permission;

			query->ExecQuery();
			hasPermission = query->FieldByName("HAS_PERMISSION")->AsInteger > 0;
		}
		__finally
		{
			transaction.Commit();
		}
		return hasPermission;
	}
	catch (...)
	{
		return false;
	}
}
//---------------------------------------------------------------------------
Permissions AuthenticationService::GetPermissions() const
{
	try
	{
		this->SavePermissions();

		Database::TDBTransaction transaction(dmStockData->dbStock);
		TIBSQL *query = transaction.Query(transaction.AddQuery());
		transaction.Start();

		Permissions permissions;

		try
		{
			query->SQL->Text = "SELECT PERMISSION_ID, PERMISSION_KEY, PERMISSION_NAME FROM SECURITY_PERMISSIONS ORDER BY PERMISSION_NAME";

			for (query->ExecQuery(); !query->Eof; query->Next())
			{
				permissions.insert(
					Permission(
						query->FieldByName("PERMISSION_ID")->AsInteger,
						(PermissionAccess)query->FieldByName("PERMISSION_KEY")->AsInteger,
						query->FieldByName("PERMISSION_NAME")->AsString));
			}
		}
		__finally
		{
			transaction.Commit();
		}

		return permissions;
	}
	catch (...)
	{
		return Permissions();
	}
}
//---------------------------------------------------------------------------
Roles AuthenticationService::GetRoles() const
{
	try
	{
		Database::TDBTransaction transaction(dmStockData->dbStock);
		TIBSQL *query = transaction.Query(transaction.AddQuery());
		transaction.Start();

		Roles roles;

		try
		{
			query->SQL->Text = "SELECT ROLE_ID, ROLE_NAME FROM SECURITY_ROLES ORDER BY ROLE_NAME";

			for (query->ExecQuery(); !query->Eof; query->Next())
			{
				roles.insert(
					Role(
						query->FieldByName("ROLE_ID")->AsInteger,
						query->FieldByName("ROLE_NAME")->AsString));
			}
		}
		__finally
		{
			transaction.Commit();
		}

		return roles;
	}
	catch (...)
	{
		return Roles();
	}
}
//---------------------------------------------------------------------------
Permissions AuthenticationService::GetRolePermissions(int roleId) const
{
	Database::TDBTransaction transaction(dmStockData->dbStock);
	TIBSQL *query = transaction.Query(transaction.AddQuery());
	transaction.Start();

	Permissions permissions;

	try
	{
		query->SQL->Text =
			"SELECT SECURITY_PERMISSIONS.PERMISSION_ID, SECURITY_PERMISSIONS.PERMISSION_KEY, SECURITY_PERMISSIONS.PERMISSION_NAME FROM SECURITY_PERMISSIONS "
			"INNER JOIN SECURITY_ROLE_PERMISSIONS ON  SECURITY_PERMISSIONS.PERMISSION_ID =  SECURITY_ROLE_PERMISSIONS.PERMISSION_ID "
			"WHERE SECURITY_ROLE_PERMISSIONS.ROLE_ID = :ROLE_ID "
			"ORDER BY SECURITY_PERMISSIONS.PERMISSION_NAME";
		query->ParamByName("ROLE_ID")->AsInteger = roleId;

		for (query->ExecQuery(); !query->Eof; query->Next())
		{
			permissions.insert(
				Permission(
					query->FieldByName("PERMISSION_ID")->AsInteger,
					(PermissionAccess)query->FieldByName("PERMISSION_KEY")->AsInteger,
					query->FieldByName("PERMISSION_NAME")->AsString));
		}
	}
	__finally
	{
		transaction.Commit();
	}

	return permissions;
}
//---------------------------------------------------------------------------
Roles AuthenticationService::GetUserRoles(const AnsiString& userId) const
{
	try
	{
		Database::TDBTransaction transaction(dmStockData->dbStock);
		TIBSQL *query = transaction.Query(transaction.AddQuery());
		transaction.Start();

		Roles roles;

		try
		{
			query->SQL->Text =
				"SELECT SECURITY_ROLES.ROLE_ID, SECURITY_ROLES.ROLE_NAME "
				"FROM SECURITY_ROLES "
					"INNER JOIN SECURITY_USER_ROLES ON SECURITY_ROLES.ROLE_ID = SECURITY_USER_ROLES.ROLE_ID "
					"INNER JOIN USERS ON SECURITY_USER_ROLES.USER_ID = USERS.USER_KEY "
				"WHERE USERS.USER_ID = :USER_ID";

			query->ParamByName("USER_ID")->AsString = userId;

			for (query->ExecQuery(); !query->Eof; query->Next())
			{
				roles.insert(
					Role(
						query->FieldByName("ROLE_ID")->AsInteger,
						query->FieldByName("ROLE_NAME")->AsString));
			}
		}
		__finally
		{
			transaction.Commit();
		}

		return roles;
	}
	catch (...)
	{
		return Roles();
	}
}
//---------------------------------------------------------------------------
void AuthenticationService::SaveUserRoles(const AnsiString& userId, const Roles& roles) const
{
	Database::TDBTransaction transaction(dmStockData->dbStock);
	TIBSQL *query = transaction.Query(transaction.AddQuery());
	TIBSQL *deleteQuery = transaction.Query(transaction.AddQuery());
	TIBSQL *insertQuery = transaction.Query(transaction.AddQuery());
	transaction.Start();

	try
	{
		query->SQL->Text = "SELECT USERS.USER_KEY FROM USERS WHERE USERS.USER_ID = :USER_ID";
		query->ParamByName("USER_ID")->AsString = userId;
		query->ExecQuery();

		int userPK = query->FieldByName("USER_KEY")->AsInteger;

		deleteQuery->SQL->Text = "DELETE FROM SECURITY_USER_ROLES WHERE SECURITY_USER_ROLES.USER_ID = :USER_ID";
		deleteQuery->ParamByName("USER_ID")->AsInteger = userPK;
		deleteQuery->ExecQuery();

		insertQuery->SQL->Text =
			"INSERT INTO SECURITY_USER_ROLES "
				"(USER_ROLE_ID, ROLE_ID, USER_ID)"
			"VALUES "
				"((SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE), :ROLE_ID, :USER_ID)";

		insertQuery->ParamByName("USER_ID")->AsInteger = userPK;

		for (Roles::const_iterator i = roles.begin(); i != roles.end(); ++i)
		{
			insertQuery->ParamByName("ROLE_ID")->AsString = i->RoleId;
			insertQuery->ExecQuery();
		}
	}
	__finally
	{
		transaction.Commit();
	}
}
//---------------------------------------------------------------------------
void AuthenticationService::SaveRolePermissions(int roleId, const Permissions& permissions) const
{
	Database::TDBTransaction transaction(dmStockData->dbStock);
	TIBSQL *deleteQuery = transaction.Query(transaction.AddQuery());
	TIBSQL *insertQuery = transaction.Query(transaction.AddQuery());
	transaction.Start();

	try
	{
		deleteQuery->SQL->Text = "DELETE FROM SECURITY_ROLE_PERMISSIONS WHERE ROLE_ID = :ROLE_ID";
		deleteQuery->ParamByName("ROLE_ID")->AsInteger = roleId;
		deleteQuery->ExecQuery();

		insertQuery->SQL->Text =
			"INSERT INTO SECURITY_ROLE_PERMISSIONS "
				"(ROLE_PERMISSIONS_ID, ROLE_ID, PERMISSION_ID)"
			"VALUES "
				"((SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE), :ROLE_ID, :PERMISSION_ID)";

		insertQuery->ParamByName("ROLE_ID")->AsInteger = roleId;

		for (Permissions::const_iterator i = permissions.begin(); i != permissions.end(); ++i)
		{
			insertQuery->ParamByName("PERMISSION_ID")->AsInteger = i->PermissionId;
			insertQuery->ExecQuery();
		}
	}
	__finally
	{
		transaction.Commit();
	}
}
//---------------------------------------------------------------------------
Role AuthenticationService::CreateRole(const AnsiString& roleName) const
{
	Database::TDBTransaction transaction(dmStockData->dbStock);
	TIBSQL *insertQuery = transaction.Query(transaction.AddQuery());
	TIBSQL *selectQuery = transaction.Query(transaction.AddQuery());
	transaction.Start();

	int newRoleId;
	AnsiString newRoleName;

	try
	{
		insertQuery->SQL->Text = "INSERT INTO SECURITY_ROLES (ROLE_ID, ROLE_NAME) VALUES ((SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE), :ROLE_NAME)";

		try
		{
			insertQuery->ParamByName("ROLE_NAME")->AsString = roleName;
			insertQuery->ExecQuery();
		}
		catch (Exception &e) {}

		selectQuery->SQL->Text = "SELECT ROLE_ID, ROLE_NAME FROM SECURITY_ROLES WHERE ROLE_NAME = :ROLE_NAME";
		selectQuery->ParamByName("ROLE_NAME")->AsString = roleName;
		selectQuery->ExecQuery();

		newRoleId = selectQuery->FieldByName("ROLE_ID")->AsInteger;
		newRoleName = selectQuery->FieldByName("ROLE_NAME")->AsString;
	}
	__finally
	{
		transaction.Commit();
	}

	return Role(newRoleId, newRoleName);
}
//---------------------------------------------------------------------------
bool AuthenticationService::DeleteRole(int roleId) const
{
	Database::TDBTransaction transaction(dmStockData->dbStock);
	TIBSQL *deleteQuery = transaction.Query(transaction.AddQuery());
	transaction.Start();

	bool success = false;

	try
	{
		deleteQuery->SQL->Text = "DELETE FROM SECURITY_ROLES WHERE ROLE_ID = :ROLE_ID";
		deleteQuery->ParamByName("ROLE_ID")->AsInteger = roleId;
		deleteQuery->ExecQuery();

		success = deleteQuery->RowsAffected > 0;
	}
	__finally
	{
		transaction.Commit();
	}

	return success;
}
//---------------------------------------------------------------------------
void AuthenticationService::SavePermissions() const
{
	Database::TDBTransaction transaction(dmStockData->dbStock);
	TIBSQL *insertQuery = transaction.Query(transaction.AddQuery());
	transaction.Start();

	try
	{
		insertQuery->SQL->Text =
			"INSERT INTO SECURITY_PERMISSIONS "
				"(PERMISSION_ID, PERMISSION_KEY, PERMISSION_NAME)"
			"VALUES "
				"((SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE), :PERMISSION_KEY, :PERMISSION_NAME)";

		this->InsertPermission(insertQuery, Security::MenuEditor, "Edit menus");
        this->InsertPermission(insertQuery, Security::DontAllowOfficeSetup, "Disallow Office configuration");
		this->InsertPermission(insertQuery, Security::StockAdmin, "Administer stock");
		this->InsertPermission(insertQuery, Security::StockReports, "View or print stock reports");
		this->InsertPermission(insertQuery, Security::Loyalty, "Loyalty system");
		this->InsertPermission(insertQuery, Security::LoyaltyReports, "View or print loyalty reports");
		this->InsertPermission(insertQuery, Security::Setup, "System properties and users");
		this->InsertPermission(insertQuery, Security::CashUp, "Access cashup facilities");
		this->InsertPermission(insertQuery, Security::AccountsExport, "Export accounting information");
		this->InsertPermission(insertQuery, Security::Reports, "View or print reports");
		this->InsertPermission(insertQuery, Security::MenuReports, "View or print menu reports");
		this->InsertPermission(insertQuery, Security::FinancialReports, "View or print financial reports");
		this->InsertPermission(insertQuery, Security::SalesReports, "View or print sales reports");
		this->InsertPermission(insertQuery, Security::SecurityReports, "View or print security reports");
        this->InsertPermission(insertQuery, Security::MYOB, "View stock MYOB exports");
        this->InsertPermission(insertQuery, Security::WriteOff, "Allow stock writeoffs");
        this->InsertPermission(insertQuery, Security::Transfer, "Allow transfers");
        this->InsertPermission(insertQuery, Security::Stocktake, "Allow stocktake");
        this->InsertPermission(insertQuery, Security::Manufacture, "Allow stock manufactures");

        this->InsertPermission(insertQuery, Security::DisallowEAStock,   "Disallow Edit/Add Stock");
        this->InsertPermission(insertQuery, Security::DisallowEASupplier, "Disallow Edit/Add Suppliers");

        this->InsertPermission(insertQuery, Security::DisallowCreatPOrder,   "Disallow Purchase Order Creation");
        this->InsertPermission(insertQuery, Security::DisallowCreatSOrder, "Disallow Supplier Order Creation");
        this->InsertPermission(insertQuery, Security::DisallowRecvPOrder,   "Disallow Receive Orders");
        this->InsertPermission(insertQuery, Security::DisallowRecvInv, "Disallow Recieve Invoices");
        this->InsertPermission(insertQuery, Security::DisallowUpdateSupplierCost,   "Disallow Update Supplier Cost");
        this->InsertPermission(insertQuery, Security::DisallowStockRequest, "Disallow Stock Request");

        this->InsertPermission(insertQuery, Security::DisallowStockSubstitute, "Disallow Stock Substitution");
	}
	__finally
	{
		transaction.Commit();
	}
}
//---------------------------------------------------------------------------
void AuthenticationService::InsertPermission(Ibsql::TIBSQL *insertQuery, PermissionAccess permission, AnsiString description) const
{
	try
	{
		insertQuery->ParamByName("PERMISSION_KEY")->AsInteger = permission;
		insertQuery->ParamByName("PERMISSION_NAME")->AsString = description;
		insertQuery->ExecQuery();
	}
	catch (EIBInterBaseError &e)
	{
	}
}
//---------------------------------------------------------------------------
}
