#ifndef AutoZReportingSetupH
#define AutoZReportingSetupH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <TouchBtn.h>
#include <TouchControls.h>
#include <TouchTime.h>

#include <memory>

#include "AutoZReporting.h"

using std::auto_ptr;

#define CheckDeselectedBackgroundColor  clBtnFace
#define CheckDeselectedTextColor        clBlack
#define CheckSelectedBackgroundColor    clBlack
#define CheckSelectedTextColor          clWhite

#define OptionDeselectedBackgroundColor clRed
#define OptionSelectedBackgroundColor   clGreen
#define OptionTextColor                 clWhite
#define OptionUntouchedBackgroundColor  CheckDeselectedBackgroundColor
#define OptionUntouchedTextColor        CheckDeselectedTextColor

class TfrmAutoZReportingSetup : public TForm
{
public:
	__fastcall TfrmAutoZReportingSetup(TComponent *, const TCheckTimeSettings *,
	                                   int *);
__published:
	TTouchBtn  *btnCheck1;
	TTouchBtn  *btnCheck2;
	TTouchBtn  *btnCheck3;
	TTouchBtn  *btnCheck4;
	TTouchBtn  *btnDiscardAndClose;
	TTouchBtn  *btnEmailZ;
	TTouchBtn  *btnPrintZ;
	TTouchBtn  *btnResetAllCheckTimes;
	TTouchBtn  *btnResetCheckTime;
	TTouchBtn  *btnSaveAndClose;
	TTouchTime *ttCheckTime;
	TTouchBtn *btnAuthenticateAs;

	void __fastcall CheckTimeSelected(TObject *Sender);
	void __fastcall Close(TObject *Sender);
	void __fastcall ResetCheckTimeSelected(TObject *Sender);
	void __fastcall SelectUserToAuthenticate(TObject *Sender);
private:
	enum SaveFlags {
		Fresh = 1,
		HasModifiedACheckTime = 2,
		HasSelectedAUserToAuthenticateAs = 4
	};

	void DeselectCheckTime(TTouchBtn *);
	void SelectCheckTime(TTouchBtn *);
	void SetButtonCaptionFromSettings(TTouchBtn *, const TCheckTimeSettings *);
	void SetButtonColors(TTouchBtn *, TColor, TColor);
	void SetButtonColors(TTouchBtn *, TColor, TColor, TColor);

	TTouchBtn *checkButtons[TAutoZVariable::NumberOfCheckTimes];
	TCheckTimeSettings *checkTimeSettings;
	unsigned int currentSaveState;
	int *keyStore;
  	TTouchBtn *lastCheckTime;
};

extern PACKAGE TfrmAutoZReportingSetup *frmAutoZReportingSetup;

#endif

