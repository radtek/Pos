#ifndef __h_quick_member_setup__
#define __h_quick_member_setup__

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "MMContactInfo.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "VerticalSelect.h"

#include <memory>

class TGlobalSettings;

class TfrmQuickMemberCreation : public TForm
{
__published:
	TPanel *pnlMemberDetails;
	TTouchBtn *btnName;
	TTouchBtn *btnMobilePhoneNumber;
	TTouchBtn *btnEmail;
	TGroupBox *gbDateOfBirth;
	TTouchBtn *btnYear;
	TTouchBtn *btnMonth;
	TTouchBtn *btnDay;
	TLabel *lblMemberDetails;
	TTouchBtn *btnSave;
	TTouchBtn *btnCancel;

	void __fastcall button_clicked(TObject *Sender);

private:
	enum fields {
		MOBILE_PHONE,
		EMAIL,
		DAY,
		MONTH,
		YEAR,
		__N_FIELDS
	};

	void cancel();

   void save_member_details();

	void set_email();
	void set_mobile_phone_number();
	void set_name();

	void set_day();
	void set_month();
	void set_year();

	bool get_numeric_input(const UnicodeString &title,
	                       unsigned short &destination);
	bool get_string_input(const UnicodeString &title,
	                      UnicodeString &destination);
	void init_fields();

	bool has_filled_in_required_information();

	std::auto_ptr<TfrmTouchKeyboard> keyboard;
	std::auto_ptr<TfrmTouchNumpad> numpad;

	UnicodeString email;
	UnicodeString mobile_phone_number;
	UnicodeString name;

	unsigned short day;
	unsigned short month;
	unsigned short year;

	TMMContactInfo &info;
	TMMContactInfo new_info;

	/*
    * An array of flags representing whether a field is mandatory or not.
	 * The elements are set when the form is created. The flags are stored for
	 * only the Phone, Email, Day, Month and Year fields. Name is always needed.
	 */
	bool required_fields[5];

	/*
	 * References to btnMobilePhoneNumber, btnEmail, btnDay, btnMonth, btnYear.
	 * Initialized on form creation; we don't know at compile-time where the
	 * buttons will be in memory.
	 *
	 * Indexed by button tag.
	 */
	TTouchBtn *fields[5];

	TGlobalSettings &gs;

	/*
	 * As the name says, this LUT stores the captions of the field buttons.
	 * The thing that has to be noted here is that 1) "Name" is not stored in
	 * the LUT; it's always mandatory. 2) The second dimension is indexed by
	 * whether the field is mandatory or not; it stores the same string data,
	 * just prefixed with an asterisk.
	 */
	static const UnicodeString field_button_strings[5][2];
	static const TColor field_colors[2];

public:
	__fastcall TfrmQuickMemberCreation(TComponent *owner, TMMContactInfo &info);
};

#endif

