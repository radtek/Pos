//---------------------------------------------------------------------------
#ifndef RoleH
#define RoleH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <set>
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
class Role
{
public:
	Role();
	Role(int id, AnsiString name);
	Role(const Role& other);

	virtual ~Role();

	Role& operator = (const Role& other);
	bool operator < (const Role& other) const;
	bool operator == (const Role& other) const;
	bool operator != (const Role& other) const;

	int RoleId;
	AnsiString RoleName;
};
//---------------------------------------------------------------------------
typedef std::set<Role> Roles;
}
//---------------------------------------------------------------------------
#endif
