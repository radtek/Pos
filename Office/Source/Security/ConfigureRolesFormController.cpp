//---------------------------------------------------------------------------
#pragma hdrstop

#include "ConfigureRolesFormController.h"
#include "AuthenticationService.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
namespace Security
{
//---------------------------------------------------------------------------
ConfigureRolesFormController::ConfigureRolesFormController(IConfigureRolesForm* form)
	: form(form)
{
	this->form->SetController(this);
}
//---------------------------------------------------------------------------
ConfigureRolesFormController::~ConfigureRolesFormController()
{
}
//---------------------------------------------------------------------------
void ConfigureRolesFormController::BeginLoadRoles()
{
	this->backgroundWorkerFactory.RunWorker<Security::Roles>(
		&this->LoadRoles,
		&this->OnRolesLoaded);
}
//---------------------------------------------------------------------------
void ConfigureRolesFormController::BeginLoadPermissions()
{
	this->backgroundWorkerFactory.RunWorker<Security::Permissions>(
		&this->LoadPermissions,
		&this->OnPermissionsLoaded);
}
//---------------------------------------------------------------------------
void ConfigureRolesFormController::BeginLoadPermissions(const Security::Role& role)
{
	this->backgroundWorkerFactory.RunWorker<std::pair<int, Security::Permissions>, int>(
		&this->LoadRolePermissions,
		&this->OnRolePermissionsLoaded,
		role.RoleId);
}
//---------------------------------------------------------------------------
void ConfigureRolesFormController::BeginAddRole(const AnsiString name)
{
	this->backgroundWorkerFactory.RunWorker<Security::Role, AnsiString>(
		&this->CreateRole,
		&this->OnRoleCreated,
		name);
}
//---------------------------------------------------------------------------
void ConfigureRolesFormController::BeginDeleteRole(const Security::Role& role)
{
	this->backgroundWorkerFactory.RunWorker<bool, int>(
		&this->DeleteRole,
		&this->OnRoleDeleted,
		role.RoleId);
}
//---------------------------------------------------------------------------
void ConfigureRolesFormController::SaveRolePermissions(const Security::Role& role, const Security::Permissions& permissions)
{
	Security::AuthenticationService auth;
	auth.SaveRolePermissions(role.RoleId, permissions);
}
//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------
Security::Roles __fastcall ConfigureRolesFormController::LoadRoles()
{
	Security::AuthenticationService auth;
	return auth.GetRoles();
}
//---------------------------------------------------------------------------
void __fastcall ConfigureRolesFormController::OnRolesLoaded(Utils::IBackgroundWorker* worker, Security::Roles roles)
{
	this->form->SetRoles(roles);
}
//---------------------------------------------------------------------------
Security::Permissions __fastcall ConfigureRolesFormController::LoadPermissions()
{
	Security::AuthenticationService auth;
	return auth.GetPermissions();
}
//---------------------------------------------------------------------------
void __fastcall ConfigureRolesFormController::OnPermissionsLoaded(Utils::IBackgroundWorker* worker, Security::Permissions permissions)
{
	this->form->SetPermissions(permissions);
}
//---------------------------------------------------------------------------
std::pair<int, Security::Permissions> __fastcall ConfigureRolesFormController::LoadRolePermissions(int roleId)
{
	Security::AuthenticationService auth;
	return std::pair<int, Security::Permissions>(
		roleId,
		auth.GetRolePermissions(roleId));
}
//---------------------------------------------------------------------------
void __fastcall ConfigureRolesFormController::OnRolePermissionsLoaded(Utils::IBackgroundWorker* worker, std::pair<int, Security::Permissions> permissions)
{
	this->form->SetRolePermissions(permissions.first, permissions.second);
}
//---------------------------------------------------------------------------
Security::Role __fastcall ConfigureRolesFormController::CreateRole(const AnsiString roleName)
{
	Security::AuthenticationService auth;
	return auth.CreateRole(roleName);
}
//---------------------------------------------------------------------------
void __fastcall ConfigureRolesFormController::OnRoleCreated(Utils::IBackgroundWorker* worker, Security::Role role)
{
	this->form->SelectRole(role);
}
//---------------------------------------------------------------------------
bool __fastcall ConfigureRolesFormController::DeleteRole(int roleId)
{
	Security::AuthenticationService auth;
	return auth.DeleteRole(roleId);
}
//---------------------------------------------------------------------------
void __fastcall ConfigureRolesFormController::OnRoleDeleted(Utils::IBackgroundWorker* worker, bool success)
{
	if (success)
		this->BeginLoadRoles();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
