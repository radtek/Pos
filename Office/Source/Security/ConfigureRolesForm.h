//---------------------------------------------------------------------------
#ifndef ConfigureRolesFormH
#define ConfigureRolesFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
#include "ConfigureRolesFormController.h"
#include "Role.h"
#include "Permission.h"
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
class TfrmConfigureRoles : public TForm, public IConfigureRolesForm
{
__published:	// IDE-managed Components
	TListBox *Roles;
	TButton *btnNewRole;
	TEdit *edRoleName;
	TCheckListBox *Permissions;
	TLabel *Label1;
	TBitBtn *btnDeleteRole;
	TButton *btnSaveRole;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnNewRoleClick(TObject *Sender);
	void __fastcall btnDeleteRoleClick(TObject *Sender);
	void __fastcall RolesClick(TObject *Sender);
	void __fastcall btnSaveRoleClick(TObject *Sender);
public:		// User declarations
	__fastcall TfrmConfigureRoles();

	virtual void SetController(ConfigureRolesFormController* controller);
	virtual void Show();

	virtual void SetRoles(const Security::Roles& roles);
	virtual void SetPermissions(const Security::Permissions& permissions);
	virtual void SetRolePermissions(int roleId, const Security::Permissions& permissions);
	virtual void SelectRole(const Security::Role& selectedRole);

private:	// User declarations
	ConfigureRolesFormController* controller;
	Security::Roles roles;
	Security::Permissions permissions;
	AnsiString selectedRoleName;

	void OnSelectedRoleChanged(Security::Role* changedTo);

	bool CheckSelectRoleChanged();
	Security::Role* GetSelectedRole();
	void SelectNextAfterDelete(const Security::Role* const current);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
