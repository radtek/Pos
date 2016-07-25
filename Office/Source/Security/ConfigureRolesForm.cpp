//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ConfigureRolesForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
namespace Security
{
//---------------------------------------------------------------------------
__fastcall TfrmConfigureRoles::TfrmConfigureRoles()
	: TForm((TComponent*)NULL), selectedRoleName("")
{
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::Show()
{
	this->ShowModal();
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::SetController(ConfigureRolesFormController* controller)
{
	this->controller = controller;
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::SetRoles(const Security::Roles& roles)
{
	this->Roles->Items->Clear();
	this->roles = roles;

	int selectedIndex = -1;
	Security::Roles::iterator role = this->roles.begin();
	while (role != this->roles.end())
	{
		int index = this->Roles->Items->AddObject(role->RoleName, (TObject*)&(*role));

		if (role->RoleName == this->selectedRoleName)
			selectedIndex = index;
		else if (selectedIndex < 0)
			selectedIndex = 0;

		role++;
	}

	if (selectedIndex > -1)
		this->Roles->Selected[selectedIndex] = true;

	edRoleName->SetFocus();

	Security::Role* selected = this->GetSelectedRole();
	this->OnSelectedRoleChanged(selected);
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::SetPermissions(const Security::Permissions& permissions)
{
	this->Permissions->Items->Clear();
	this->permissions = permissions;

	Security::Permissions::iterator permission = this->permissions.begin();
	while (permission != this->permissions.end())
	{
		this->Permissions->Items->AddObject(permission->PermissionName, (TObject*)&(*permission));
		permission++;
	}

	this->controller->BeginLoadRoles();
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::SetRolePermissions(int roleId, const Security::Permissions& permissions)
{
	Security::Role* selected = this->GetSelectedRole();
	if (selected->RoleId != roleId)
		return;

	for (int i = 0; i < this->Permissions->Items->Count; i++)
	{
		Security::Permission* current = (Security::Permission*)this->Permissions->Items->Objects[i];
		this->Permissions->Checked[i] = permissions.find(*current) != permissions.end();
	}
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::SelectRole(const Security::Role& selectedRole)
{
	this->selectedRoleName = selectedRole.RoleName;
	this->controller->BeginLoadRoles();
}
//---------------------------------------------------------------------------
// Events
//---------------------------------------------------------------------------
void __fastcall TfrmConfigureRoles::FormShow(TObject *Sender)
{
	this->controller->BeginLoadPermissions();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfigureRoles::btnNewRoleClick(TObject *Sender)
{
	if (this->edRoleName->Text != NULL && this->edRoleName->Text != "")
	{
		this->controller->BeginAddRole(this->edRoleName->Text);
		this->edRoleName->Text = "";
	}
	else
	{
		this->edRoleName->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfigureRoles::btnDeleteRoleClick(TObject *Sender)
{
	Security::Role* selected = this->GetSelectedRole();
	if (selected == NULL)
	{
		this->edRoleName->SetFocus();
		return;
	}

	if (Application->MessageBox(
		"Are you sure you want to delete this role?"
		"\r\n\r\nThis action is permenant and will make this role unavailable for all users."
		"\r\nMake sure there is at least 1 user with permission to configure users.",
		"Delete?",
		MB_YESNO + MB_ICONQUESTION) == ID_YES)
	{
		this->SelectNextAfterDelete(selected);
		this->controller->BeginDeleteRole(*selected);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfigureRoles::RolesClick(TObject *Sender)
{
	this->CheckSelectRoleChanged();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfigureRoles::btnSaveRoleClick(TObject *Sender)
{
	Security::Role* selected = this->GetSelectedRole();
	if (selected == NULL)
		return;

	Security::Permissions rolePermissions;
	for (int i = 0; i<this->Permissions->Items->Count; i++)
	{
		if (this->Permissions->Checked[i])
		{
			Security::Permission* current = (Security::Permission*)this->Permissions->Items->Objects[i];
			rolePermissions.insert(*current);
		}
	}

	this->controller->SaveRolePermissions(*selected, rolePermissions);
	this->controller->BeginLoadRoles();
}
//---------------------------------------------------------------------------
// Private Helpers
//---------------------------------------------------------------------------
bool TfrmConfigureRoles::CheckSelectRoleChanged()
{
	Security::Role* selected = this->GetSelectedRole();
	if (selected == NULL)
	{
		if (this->selectedRoleName != "" || this->selectedRoleName != NULL)
		{
			this->selectedRoleName = "";
		}
		else
		{
			return false;
		}
	}
	else if (this->selectedRoleName != selected->RoleName)
	{
		this->selectedRoleName = selected->RoleName;
	}
	else
	{
		return false;
	}

	this->OnSelectedRoleChanged(selected);
	return true;
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::OnSelectedRoleChanged(Security::Role* changedTo)
{
	if (changedTo == NULL)
	{
		this->Permissions->Enabled = false;
		return;
	}
	else
	{
		this->Permissions->Enabled = true;
		for (int i = 0; i < this->Permissions->Items->Count; i++)
		{
			this->Permissions->Checked[i] = false;
		}

		this->controller->BeginLoadPermissions(*changedTo);
	}
}
//---------------------------------------------------------------------------
Security::Role* TfrmConfigureRoles::GetSelectedRole()
{
	int selIndex = -1;
	for (int i = 0; i < this->Roles->Items->Count; i++)
	{
		if (this->Roles->Selected[i])
		{
			selIndex = i;
			break;
		}
	}

	if (selIndex < 0)
	{
		return NULL;
	}

	return (Security::Role*)this->Roles->Items->Objects[selIndex];
}
//---------------------------------------------------------------------------
void TfrmConfigureRoles::SelectNextAfterDelete(const Security::Role* const current)
{
	int selIndex = this->Roles->Items->IndexOfObject((TObject*)current);

	int newIndex = selIndex + 1;
	if (newIndex >= (int)this->roles.size()) newIndex = selIndex - 1;
	if (newIndex > -1)
	{
		Security::Role* selectedRole = (Security::Role*)this->Roles->Items->Objects[newIndex];
		this->selectedRoleName = selectedRole->RoleName;
	}
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

