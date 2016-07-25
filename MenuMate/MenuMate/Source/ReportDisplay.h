//---------------------------------------------------------------------------

#ifndef ReportDisplayH
#define ReportDisplayH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <SHDocVw.hpp>
//---------------------------------------------------------------------------
class TfrmReportDisplay : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel3;
	TPanel *Panel1;
	TTouchBtn *TouchBtn1;
	TTouchBtn *tbtnPageUp;
	TTouchBtn *tbtnPageDown;
	TTouchBtn *tbtnLineUp;
	TTouchBtn *tbtnLineDown;
	TWebBrowser *webDisplay;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
	void __fastcall tbtnPageUpMouseClick(TObject *Sender);
	void __fastcall tbtnPageDownMouseClick(TObject *Sender);
	void __fastcall tbtnLineUpMouseClick(TObject *Sender);
	void __fastcall tbtnLineDownMouseClick(TObject *Sender);
private:	// User declarations
protected:
	__fastcall TfrmReportDisplay(TComponent* Owner);
public:		// User declarations
	void Navigate(TStringList *Html);
	void NavigateToURL(UnicodeString Url);
   void LoadHtml();
	TStringList *Source;
};
//---------------------------------------------------------------------------
#endif
