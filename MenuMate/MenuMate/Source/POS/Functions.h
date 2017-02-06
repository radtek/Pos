//---------------------------------------------------------------------------

#ifndef FunctionsH
#define FunctionsH
//---------------------------------------------------------------------------


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdIPWatch.hpp"
#include <set>
#include <Graphics.hpp>
#include "ZForm.h"
#include "SystemEvents.h"
#include "Enum.h"
//---------------------------------------------------------------------------
class TfrmFunctions : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TTouchBtn *BtnOk;
	TTouchBtn *BtnCallAway;
	TTouchBtn *BtnDiscountAll;
	TTouchBtn *BtnHappyHour;
	TTouchBtn *BtnLuckyMember;
	TTouchBtn *TouchBtn7;
	TTouchBtn *TouchBtn8;
	TTouchBtn *TouchBtn9;
	TTouchBtn *TouchBtn10;
	TTouchBtn *btnPaxCount;
	TTouchBtn *TouchBtn1;
    TTouchBtn *tbtnSearchItem;
        TTouchBtn *btnChangeCard;
    TTouchBtn *btnLiveTransaction;
    TTouchBtn *btnSync;
    TTouchBtn *btnGiftCard;

	void __fastcall btnOkMouseClick(TObject *Sender);
	void __fastcall CallAwayMouseClick(TObject *Sender);
	void __fastcall btnDiscountAllMouseClick(TObject *Sender);
	void __fastcall btnHappyHourMouseClick(TObject *Sender);
	void __fastcall btnProgramsMouseClick(TObject *Sender);
	void __fastcall btnLuckyMembersMouseClick(TObject *Sender);
	void __fastcall btnFloatManagementMouseClick(TObject *Sender);
	void __fastcall btnReportsMouseClick(TObject *Sender);
	void __fastcall btnWastageMouseClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnPaxCountMouseClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
    void __fastcall tbtnSearchItemMouseClick(TObject *Sender);
    void __fastcall btnChangeCardMouseClick(TObject *Sender);
    void __fastcall btnLiveTransactionMouseClick(TObject *Sender);
    void __fastcall btnSyncMouseClick(TObject *Sender);
    void __fastcall btnGiftCardMouseClick(TObject *Sender);

private:	// User declarations
	int BtnSelection;
public:		// User declarations
	__fastcall TfrmFunctions(TComponent* Owner);
	int GetBtnSelection(void);

};
//---------------------------------------------------------------------------
#endif
