//---------------------------------------------------------------------------
#ifndef pauseH
#define pauseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmPause : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
  TProgressBar *Progress;
	TBitBtn *btnExit;
  void __fastcall btnExitClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
   void __fastcall FormHide(TObject *Sender);
private:	// User declarations
  __fastcall TfrmPause(TComponent* Owner);
public:		// User declarations
  bool Cancel;
  void incbar();
  void Reset();


};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPause *frmPause;
//---------------------------------------------------------------------------
#endif
