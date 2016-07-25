//---------------------------------------------------------------------------
#pragma hdrstop

#include "Permission.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
Permission::Permission()
{
}
//---------------------------------------------------------------------------
Permission::Permission(int permissionId, Security::PermissionAccess access, AnsiString name)
{
	this->PermissionId = permissionId;
	this->PermissionAccess = access;
	this->PermissionName = name;
}
//---------------------------------------------------------------------------
Permission::Permission(const Permission& other)
{
	this->operator =(other);
}
//---------------------------------------------------------------------------
Permission::~Permission()
{
}
//---------------------------------------------------------------------------
Permission& Permission::operator = (const Permission& other)
{
	if (&other != this)
	{
		this->PermissionId = other.PermissionId;
		this->PermissionAccess = other.PermissionAccess;
		this->PermissionName = other.PermissionName;
	}
	return *this;
}
//---------------------------------------------------------------------------
bool Permission::operator < (const Permission& other) const
{
	return this->PermissionAccess < other.PermissionAccess;
}
//---------------------------------------------------------------------------
bool Permission::operator == (const Permission& other) const
{
	return this->PermissionAccess == other.PermissionAccess;
}
//---------------------------------------------------------------------------
bool Permission::operator != (const Permission& other) const
{
	return !this->operator ==(other);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

