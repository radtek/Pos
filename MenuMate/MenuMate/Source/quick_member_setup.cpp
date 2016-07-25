#include <cassert>
#include <vcl.h>

#pragma hdrstop

#include "GlobalSettings.h"
#include "MMMessageBox.h"

#include "quick_member_setup.h"
#include "SmartcardPreloader.h"

#pragma package(smart_init)

#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

const UnicodeString TfrmQuickMemberCreation::field_button_strings[5][2] = {
	{"Mobile Phone", "* Mobile Phone"},
	{"Email",        "* Email"},
	{"Day",          "* Day"},
	{"Month",        "* Month"},
	{"Year",         "* Year"},
};

const TColor TfrmQuickMemberCreation::field_colors[2] = {
	0x979492, clMaroon
};

__fastcall
TfrmQuickMemberCreation::TfrmQuickMemberCreation(
  TComponent *owner,
  TMMContactInfo &info)
  : TForm(owner),
    keyboard(TZForm::Create<TfrmTouchKeyboard>(this)),
    numpad(TZForm::Create<TfrmTouchNumpad>(this)),
    email(),
	 mobile_phone_number(),
    name(TSmartcard_Preloader::preload_member_name),
    day(1),
    month(1),
    year(1900),
    info(info),
    new_info(),
    gs(TGlobalSettings::Instance())
{
	keyboard->AllowCarriageReturn = false;
	keyboard->CloseOnDoubleCarriageReturn = false;
	keyboard->MaxLength = 50;
	keyboard->MustHaveValue = true;
	keyboard->StartWithShiftDown = false;

	new_info.Points = info.Points;

	numpad->btnCancel->Visible = true;
	numpad->btnOk->Visible = true;
	numpad->Mode = TPadMode::pmNumber;

	init_fields();
}

void
TfrmQuickMemberCreation::cancel()
{
	ModalResult = mrCancel;
}

void __fastcall
TfrmQuickMemberCreation::button_clicked(TObject *sender)
{
   static void (TfrmQuickMemberCreation::*button_handlers[8])() = {
		&TfrmQuickMemberCreation::set_name,
		&TfrmQuickMemberCreation::set_mobile_phone_number,
	   &TfrmQuickMemberCreation::set_email,
		&TfrmQuickMemberCreation::set_day,
		&TfrmQuickMemberCreation::set_month,
		&TfrmQuickMemberCreation::set_year,
		&TfrmQuickMemberCreation::save_member_details,
	   &TfrmQuickMemberCreation::cancel
	};

	assert(static_cast<TTouchBtn *>(sender)->Tag >= 0
	       && static_cast<TTouchBtn *>(sender)->Tag <= 8);

	((*this).*button_handlers[static_cast<TTouchBtn *>(sender)->Tag])();
}

void
TfrmQuickMemberCreation::save_member_details()
{
	if (has_filled_in_required_information()) {
		new_info.Alias = name;
      new_info.DateOfBirth = TDateTime(year, month, day);
      new_info.EMail = email;
      new_info.MembershipNumber = "";
      new_info.Mobile = mobile_phone_number;
      new_info.Name = name;
		new_info.Phone = mobile_phone_number;
		new_info.Points.Assign(info.Points);
		new_info.SiteID = info.SiteID;
      new_info.MemberType = 1;
      info = new_info;

      ModalResult = mrOk;
	}
}

void
TfrmQuickMemberCreation::set_email()
{
	if (get_string_input("To what address do we send emails?",
	                     email)) {
		btnEmail->ButtonColor = field_colors[0];
		btnEmail->Caption = email;
	}
}

void
TfrmQuickMemberCreation::set_mobile_phone_number()
{
	if (get_string_input("What number should we use to contact the member?",
	                     mobile_phone_number)) {
		btnMobilePhoneNumber->ButtonColor = field_colors[0];
		btnMobilePhoneNumber->Caption = mobile_phone_number;
	}
}

void
TfrmQuickMemberCreation::set_name()
{
	if (get_string_input("What's the member's name?", name)) {
		btnName->ButtonColor = field_colors[0];
		btnName->Caption = name;
	}
}

#define inclusive_is_between(x, m, n) ((x) >= (m) && (x) <= (n))

void
TfrmQuickMemberCreation::set_month()
{
	unsigned short _month = 0;

	while (get_numeric_input("What month was the member born in?",
	                         _month) && !inclusive_is_between(_month, 1, 12)) {
		MessageBox("The valid range for months is 1..12.",
	              "You've entered an invalid month.", MB_OK | MB_ICONERROR);
		_month = 0;
	}

	btnMonth->Caption = !_month ? btnMonth->Caption : IntToStr(month = _month);
	btnMonth->ButtonColor = field_colors[!month];
}


void
TfrmQuickMemberCreation::set_day()
{
	unsigned short _day = 0;

	while (get_numeric_input("What day was the member born on?",
	                         _day) && !inclusive_is_between(_day, 1, 31)) {
		MessageBox("The valid range for days is 1..31.",
	              "You've entered an invalid day!", MB_OK | MB_ICONERROR);
		_day = 0;
	}

	btnDay->Caption = !_day ? btnYear->Caption : IntToStr(day = _day);
	btnDay->ButtonColor = field_colors[!day];
}

void
TfrmQuickMemberCreation::set_year()
{
	unsigned short _year = 0;

	while (get_numeric_input("What year was the member born in?", _year)
	       && !inclusive_is_between(_year, 1900, CurrentYear())) {
		MessageBox("The year entered must fall between 1900 and "
		           + IntToStr(CurrentYear())
		           + ".",
		           "You've entered an invalid year of birth!",
		           MB_OK | MB_ICONERROR);
		_year = 0;
	}

	btnYear->Caption = !_year ? btnYear->Caption : IntToStr(year = _year);
	btnYear->ButtonColor = field_colors[!year];
}

#undef inclusive_is_between

bool
TfrmQuickMemberCreation::get_numeric_input(
  const UnicodeString &title,
  unsigned short &destination)
{
	numpad->Caption = title;
	numpad->INTInitial = destination;

	if (numpad->ShowModal() == mrOk)
		return destination = numpad->INTResult, true;

	return false;
}

bool
TfrmQuickMemberCreation::get_string_input(
  const UnicodeString &title,
  UnicodeString &destination)
{
	UnicodeString trimmed;

	keyboard->Caption = title;
	keyboard->KeyboardText = destination;

	if (keyboard->ShowModal() == mrOk
	    && (trimmed = keyboard->KeyboardText.Trim()).Length())
		return destination = trimmed, true;

	return false;
}

void
TfrmQuickMemberCreation::init_fields()
{
	fields[MOBILE_PHONE] = btnMobilePhoneNumber;
	fields[EMAIL] = btnEmail;
	fields[DAY] = btnDay;
	fields[MONTH] = btnMonth;
	fields[YEAR] = btnYear;

	/*
	 * Initialize the required_fields array; this has to be done before
	 * we call init_fields().
	 */
	required_fields[MOBILE_PHONE] = gs.QMSPhoneNumberIsMandatory;
	required_fields[EMAIL] = gs.QMSEmailAddressIsMandatory;
	required_fields[DAY] = required_fields[MONTH] =
	  required_fields[YEAR] = gs.QMSDateOfBirthIsMandatory;

	for (int i = 0; i < __N_FIELDS; i++)
		fields[i]->Caption = field_button_strings[i][required_fields[i]];
}

bool
TfrmQuickMemberCreation::has_filled_in_required_information()
{
	static const TColor field_colors[2] = {
		clMaroon, 0x979492
	};

	bool all_good = btnName->Caption != "* Name";

	btnName->ButtonColor = field_colors[all_good];
	btnName->Font->Color = clWhite;

   /*
	 * We're taking advantage of the fact that "true" and "false" are equivalent
	 * to "1" and "0".
	 */
	for (int i = 0, j, k; i < __N_FIELDS; all_good &= k, i++) {
		j = required_fields[i];
		k = (fields[i]->Caption != field_button_strings[i][j]) >= j;
      fields[i]->ButtonColor = field_colors[k];
		fields[i]->Font->Color = clWhite;
	}

	return all_good;
}

