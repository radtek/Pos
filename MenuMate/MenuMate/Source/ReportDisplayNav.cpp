//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReportDisplayNav.h"
#include "ManagerWebDispatch.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ReportDisplay"
#pragma link "SHDocVw_OCX"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmReportDisplayNav *frmReportDisplayNav;
//---------------------------------------------------------------------------
__fastcall TfrmReportDisplayNav::TfrmReportDisplayNav(TComponent* Owner)
   : TfrmReportDisplay(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportDisplayNav::webDisplayBeforeNavigate2(TObject *ASender,
          const IDispatch *pDisp, OleVariant &URL, OleVariant &Flags, OleVariant &TargetFrameName,
          OleVariant &PostData, OleVariant &Headers, WordBool &Cancel)

{
   TWebBrowser *Browser;
   if ((Browser = dynamic_cast<TWebBrowser *>(ASender)) != NULL)
	{
		std::auto_ptr<TManagerWebDispatch> Dispatch (new TManagerWebDispatch(this,Browser));
	   UnicodeString Data = URL;
		Dispatch->Execute(Data,Cancel);
	}
}
//---------------------------------------------------------------------------

