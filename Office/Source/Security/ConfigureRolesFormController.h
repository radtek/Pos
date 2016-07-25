//---------------------------------------------------------------------------
#ifndef ConfigureRolesFormControllerH
#define ConfigureRolesFormControllerH
//---------------------------------------------------------------------------
#include "BackgroundWorker.h"
#include "Role.h"
#include "Permission.h"
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
class ConfigureRolesFormController;
class IConfigureRolesForm
{
public:
	virtual void SetController(ConfigureRolesFormController* controller) = 0;
	virtual void Show() = 0;

	virtual void SetRoles(const Roles& roles) = 0;
	virtual void SetPermissions(const Permissions& permissions) = 0;
	virtual void SetRolePermissions(int roleId, const Permissions& permissions) = 0;
	virtual void SelectRole(const Role& selectedRole) = 0;
};
//---------------------------------------------------------------------------
class ConfigureRolesFormController
{
public:
	ConfigureRolesFormController(IConfigureRolesForm* form);
	virtual ~ConfigureRolesFormController();
	
	void BeginLoadRoles();
	void BeginLoadPermissions();
	void BeginLoadPermissions(const Security::Role& role);
	void BeginAddRole(const AnsiString name);
	void BeginDeleteRole(const Security::Role& role);
	void SaveRolePermissions(const Security::Role& role, const Security::Permissions& permissions);

private:
	IConfigureRolesForm* const form;

	Security::Roles __fastcall LoadRoles();
	void __fastcall OnRolesLoaded(Utils::IBackgroundWorker* worker, Security::Roles result);

	Security::Permissions __fastcall LoadPermissions();
	void __fastcall OnPermissionsLoaded(Utils::IBackgroundWorker* worker, Security::Permissions result);

	std::pair<int, Security::Permissions> __fastcall LoadRolePermissions(int roleId);
	void __fastcall OnRolePermissionsLoaded(Utils::IBackgroundWorker* worker, std::pair<int, Security::Permissions> permissions);

	Security::Role __fastcall CreateRole(const AnsiString name);
	void __fastcall OnRoleCreated(Utils::IBackgroundWorker* worker, Security::Role result);

	bool __fastcall DeleteRole(int roleId);
	void __fastcall OnRoleDeleted(Utils::IBackgroundWorker* worker, bool success);

	Utils::BackgroundWorkerFactory backgroundWorkerFactory;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
