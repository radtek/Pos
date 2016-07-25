//---------------------------------------------------------------------------

#ifndef ReportDisplayNavH
#define ReportDisplayNavH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ReportDisplay.h"
#include "SHDocVw_OCX.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
//---------------------------------------------------------------------------
class TfrmReportDisplayNav : public TfrmReportDisplay
{
__published:	// IDE-managed Components
	void __fastcall webDisplayBeforeNavigate2(TObject *ASender, const IDispatch *pDisp,
          OleVariant &URL, OleVariant &Flags, OleVariant &TargetFrameName,
          OleVariant &PostData, OleVariant &Headers, WordBool &Cancel);

private:	// User declarations
public:		// User declarations
   __fastcall TfrmReportDisplayNav(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReportDisplayNav *frmReportDisplayNav;
//---------------------------------------------------------------------------
#endif
