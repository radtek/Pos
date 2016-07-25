//---------------------------------------------------------------------------

#ifndef LoyaltyMateOperationDialogBoxH
#define LoyaltyMateOperationDialogBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "ManagerLoyaltyMate.h"
//---------------------------------------------------------------------------
class TfrmLoyaltyMateOperationDialogBox : public TZForm
{
__published:	// IDE-managed Components
    TLabel *lblDescription;
    TGroupBox *grpMessageContent;
    TTouchBtn *btnCancelOperation;
    void __fastcall btnCancelOperationMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	void setDownloadThread(TLoyaltyMateDownloadMemberThread *inDownloadThread);
	void __fastcall loyaltyMateDownloadMemberThreadCompleted(TObject* sender);

	TLoyaltyMateDownloadMemberThread* loyaltyMemberDownloadThread;
public:		// User declarations
    __fastcall TfrmLoyaltyMateOperationDialogBox(TComponent* Owner);
    UnicodeString OperationDescription;
    UnicodeString OperationTitle;
    bool PreventCancelOperation;
    bool BarcodeMemberNotExist;
	TMMContactInfo Info;
	__property TLoyaltyMateDownloadMemberThread* DownloadThread =
	{
		write = setDownloadThread
    };
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLoyaltyMateOperationDialogBox *frmLoyaltyMateOperationDialogBox;
//---------------------------------------------------------------------------
#endif
