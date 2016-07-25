//---------------------------------------------------------------------------
#ifndef AuthenticationServiceH
#define AuthenticationServiceH
//---------------------------------------------------------------------------
#include <system.hpp>
#include "Permission.h"
#include "Role.h"
//---------------------------------------------------------------------------
namespace Ibsql { class TIBSQL; }
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
struct Identity
{
	AnsiString UserID;
	AnsiString UserName;
};
//---------------------------------------------------------------------------
class AuthenticationService
{
public:
	AuthenticationService();

	bool Authenticate(const AnsiString& username, const AnsiString& password, Identity& identity) const;
	bool HasPermission(const AnsiString& username, PermissionAccess role) const;

	Permissions GetPermissions() const;
	Roles GetRoles() const;

	Permissions GetRolePermissions(int roleId) const;

	Roles GetUserRoles(const AnsiString& userId) const;
	void SaveUserRoles(const AnsiString& userId, const Roles& roles) const;
	void SaveRolePermissions(int roleId, const Permissions& permissions) const;

	Role CreateRole(const AnsiString& roleName) const;
	bool DeleteRole(int roleId) const;

private:
	void SavePermissions() const;
	void InsertPermission(Ibsql::TIBSQL *insertQuery, PermissionAccess permission, AnsiString description) const;
};
//---------------------------------------------------------------------------
}
#endif
