#include <vcl.h>

#pragma hdrstop

#pragma package(smart_init)

#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchTime"
#pragma link "TouchTime"
#pragma link "TouchBtn"
#pragma link "TouchBtn"
#pragma link "TouchControls"

#pragma resource "*.dfm"

#include "AutoZReportingSetup.h"
#include "Analysis.h"

using namespace Database;

TfrmAutoZReportingSetup *frmAutoZReportingSetup;

__fastcall TfrmAutoZReportingSetup::TfrmAutoZReportingSetup(
	TComponent *Owner, const TCheckTimeSettings *i, int *keyStore)
	: TForm(Owner)
{
	checkButtons[0] = btnCheck1, checkButtons[1] = btnCheck2;
	checkButtons[2] = btnCheck3, checkButtons[3] = btnCheck4;

	checkTimeSettings = new TCheckTimeSettings[4];

	memcpy(checkTimeSettings, i,
          sizeof(TCheckTimeSettings) * TAutoZVariable::NumberOfCheckTimes);
	for (TTouchBtn **m = checkButtons;
	     m < &checkButtons[TAutoZVariable::NumberOfCheckTimes]; i++, m++) {
		SetButtonColors(*m, CheckDeselectedBackgroundColor,
		                CheckDeselectedTextColor);
		SetButtonCaptionFromSettings(*m, i);
	}

	SetButtonColors(btnEmailZ, OptionUntouchedBackgroundColor,
	                OptionUntouchedBackgroundColor, OptionUntouchedTextColor);
	SetButtonColors(btnPrintZ, OptionUntouchedBackgroundColor,
	                OptionUntouchedBackgroundColor, OptionUntouchedTextColor);

	currentSaveState = SaveFlags::Fresh;
	this->keyStore = keyStore;
}

void __fastcall TfrmAutoZReportingSetup::CheckTimeSelected(TObject *Sender)
{
	if (lastCheckTime) {
		if (lastCheckTime == Sender)
			return;
      currentSaveState |= SaveFlags::HasModifiedACheckTime;
		DeselectCheckTime(lastCheckTime);
	} else {
		SetButtonColors(btnEmailZ, OptionDeselectedBackgroundColor,
		                OptionSelectedBackgroundColor, OptionTextColor);
		SetButtonColors(btnPrintZ, OptionDeselectedBackgroundColor,
		                OptionSelectedBackgroundColor, OptionTextColor);
		btnEmailZ->Enabled++;
		btnPrintZ->Enabled++;
	}

	btnResetCheckTime->Enabled = true;
   SelectCheckTime(lastCheckTime = static_cast<TTouchBtn *>(Sender));
}

void __fastcall TfrmAutoZReportingSetup::Close(TObject *Sender)
{
	if (!static_cast<TTouchBtn *>(Sender)->Tag) {
		if (!btnAuthenticateAs->Tag) {
			return;
		} else if (lastCheckTime) {
			DeselectCheckTime(lastCheckTime);
		}
		TAutoZReporting::Instance().UpdateSettings(checkTimeSettings);
	}

	delete[] checkTimeSettings;
	ModalResult = mrOk;
}

void TfrmAutoZReportingSetup::DeselectCheckTime(TTouchBtn *checkTime)
{
	TCheckTimeSettings *currSetting = checkTimeSettings + checkTime->Tag;

	SetButtonColors(
		checkTime, CheckDeselectedBackgroundColor, CheckDeselectedTextColor);

	currSetting->checkTime = ttCheckTime->Time;
	currSetting->options =
   	(currSetting->options & TCheckTimeOption::Enabled)
	   | (btnEmailZ->Latched << TCheckTimeOptionShift::EmailZReportShift)
	   | (btnPrintZ->Latched << TCheckTimeOptionShift::PrintZReportShift);

	SetButtonCaptionFromSettings(checkTime, currSetting);
}

void TfrmAutoZReportingSetup::SelectCheckTime(TTouchBtn *checkTime)
{
	TCheckTimeSettings *currSetting = checkTimeSettings + checkTime->Tag;

	btnEmailZ->Latched =
		currSetting->options & TCheckTimeOption::EmailZReport;
	btnPrintZ->Latched =
		currSetting->options & TCheckTimeOption::PrintZReport;

	currSetting->options |= TCheckTimeOption::Enabled;
	SetButtonColors(
		checkTime, CheckSelectedBackgroundColor,	CheckSelectedTextColor);

	ttCheckTime->Time = currSetting->checkTime;
}

void __fastcall TfrmAutoZReportingSetup::ResetCheckTimeSelected(
	TObject *Sender)
{
	btnEmailZ->Latched = false;
	btnPrintZ->Latched = false;

	if (static_cast<TTouchBtn *>(Sender)->Tag) {
		memset(checkTimeSettings, 0,
		       sizeof(TCheckTimeSettings) * TAutoZVariable::NumberOfCheckTimes);
		for (TTouchBtn **i = checkButtons;
		     i < &checkButtons[NumberOfCheckTimes]; i++) {
			DeselectCheckTime(*i);
		}
	} else {
		memset(checkTimeSettings + lastCheckTime->Tag,
		       0x0, sizeof(TCheckTimeSettings));
		DeselectCheckTime(lastCheckTime);
	}

	btnEmailZ->ButtonColor = OptionUntouchedBackgroundColor;
	btnPrintZ->ButtonColor = OptionUntouchedBackgroundColor;
	btnEmailZ->Font->Color = OptionUntouchedTextColor;
	btnPrintZ->Font->Color = OptionUntouchedTextColor;

	btnEmailZ->Enabled = false;
	btnPrintZ->Enabled = false;
	btnResetCheckTime->Enabled = false;

	lastCheckTime = NULL;
	ttCheckTime->Time = 0;
}

void TfrmAutoZReportingSetup::SetButtonCaptionFromSettings(
	TTouchBtn *checkTime, const TCheckTimeSettings *settings)
{
	checkTime->Caption =
		"Check " + UnicodeString(IntToStr(checkTime->Tag + 1));
	if (!(settings->options & TCheckTimeOption::Enabled)) {
		checkTime->Caption = checkTime->Caption + " is disabled";
	} else {
		checkTime->Caption =	checkTime->Caption + " at "
		                     + settings->checkTime.FormatString("hh:mm");
	}
}

void TfrmAutoZReportingSetup::SetButtonColors(TTouchBtn *checkTime,
	TColor background, TColor text)
{
   checkTime->Font->Color = text;
	checkTime->ButtonColor = background;
}

void TfrmAutoZReportingSetup::SetButtonColors(
	TTouchBtn *optionButton, TColor deselectedColor, TColor selectedColor,
	TColor textColor)
{
	optionButton->Font->Color  = textColor;
	optionButton->LatchedColor = selectedColor;
	optionButton->ButtonColor  = deselectedColor;
}

void __fastcall TfrmAutoZReportingSetup::SelectUserToAuthenticate(
	TObject *Sender)
{
	if (TfrmAnalysis::AuthenticateReportsAccess(TReportSource::PrintZed)) {
		btnAuthenticateAs->Caption = "Authenticate as...";
		btnAuthenticateAs->Tag = 0;
		currentSaveState &= ~SaveFlags::HasSelectedAUserToAuthenticateAs;
	} else {
		const TMMContactInfo &staff_member =
	      TfrmAnalysis::GetLastAuthenticatedUser();

		btnAuthenticateAs->Caption = "Authenticating as " + staff_member.Name;
		btnAuthenticateAs->Tag = staff_member.ContactKey;
		currentSaveState |= SaveFlags::HasSelectedAUserToAuthenticateAs;
	}
}

