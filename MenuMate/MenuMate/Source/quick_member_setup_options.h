#ifndef __h_quick_member_setup_options__
#define __h_quick_member_setup_options__

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>

class TGlobalSettings;

class TfrmQuickMemberSetupOptions : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gbFields;
	TCheckBox *cbPhoneNumberIsMandatory;
	TCheckBox *cbEmailAddressIsMandatory;
	TCheckBox *cbDateOfBirthIsMandatory;
	TCheckBox *cbQMCIsEnabled;
	TTouchBtn *btnSave;
	TTouchBtn *btnCancel;
	TPanel *pnlContents;
	void __fastcall initialize_checkboxes(TObject *sender);
	void __fastcall button_clicked(TObject *sender);
	void __fastcall enabled_clicked(TObject *sender);
private:	// User declarations
	TGlobalSettings     &gs;

	void toggle_field_checkboxes_enabled_state();

public:		// User declarations
	__fastcall TfrmQuickMemberSetupOptions(TComponent* Owner);
};

#endif
