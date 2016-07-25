#include <vcl.h>
#pragma hdrstop

#include "quick_member_setup_options.h"

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#include "DeviceRealTerminal.h"
#include "ManagerVariable.h"
#include "GlobalSettings.h"

using Database::TDBTransaction;

__fastcall
TfrmQuickMemberSetupOptions::TfrmQuickMemberSetupOptions(TComponent *owner)
  : TForm(owner),
    gs(TGlobalSettings::Instance())
{
}

#pragma warn -par

void __fastcall
TfrmQuickMemberSetupOptions::initialize_checkboxes(TObject *sender)
{
	cbQMCIsEnabled->Checked = gs.QMSIsEnabled;

	/* For some reason, setting TCheckBox's "Enabled" property to "False" in
	 * the DFM doesn't take effect when the program's running. So... I'm
	 * disabling them myself... *sigh*
	 */
	cbDateOfBirthIsMandatory->Enabled  = false;
	cbEmailAddressIsMandatory->Enabled = false;
	cbPhoneNumberIsMandatory->Enabled  = false;

	/* We'll enable them if it makes sense to do so ... */
	if (gs.QMSIsEnabled)
		toggle_field_checkboxes_enabled_state();

   cbDateOfBirthIsMandatory->Checked  = gs.QMSDateOfBirthIsMandatory;
   cbEmailAddressIsMandatory->Checked = gs.QMSEmailAddressIsMandatory;
	cbPhoneNumberIsMandatory->Checked  = gs.QMSPhoneNumberIsMandatory;
}

void __fastcall
TfrmQuickMemberSetupOptions::enabled_clicked(TObject *sender)
{
	toggle_field_checkboxes_enabled_state();
}

#pragma warn .par

void __fastcall TfrmQuickMemberSetupOptions::button_clicked(TObject *sender)
{
	if (sender == btnSave) {
		TManagerVariable &mv = TManagerVariable::Instance();
		TDBTransaction   tr(TDeviceRealTerminal::Instance().DBControl);

		tr.StartTransaction();
		mv.SetDeviceBool(tr, vmQuickMemberSetupEnabled,
		                 gs.QMSIsEnabled = cbQMCIsEnabled->Checked);

		mv.SetDeviceBool(
		  tr,
        vmQuickMemberSetupDateOfBirthIsMandatory,
        gs.QMSDateOfBirthIsMandatory = cbDateOfBirthIsMandatory->Checked);

		mv.SetDeviceBool(
		  tr,
        vmQuickMemberSetupEmailIsMandatory,
        gs.QMSEmailAddressIsMandatory = cbEmailAddressIsMandatory->Checked);

		mv.SetDeviceBool(
		  tr,
        vmQuickMemberSetupPhoneIsMandatory,
        gs.QMSPhoneNumberIsMandatory = cbPhoneNumberIsMandatory->Checked);
		tr.Commit();

		ModalResult = mrOk;
		return;
	}

	ModalResult = mrCancel;
}

void
TfrmQuickMemberSetupOptions::toggle_field_checkboxes_enabled_state()
{
	cbDateOfBirthIsMandatory->Enabled  ^= true;
	cbEmailAddressIsMandatory->Enabled ^= true;
	cbPhoneNumberIsMandatory->Enabled  ^= true;
}

