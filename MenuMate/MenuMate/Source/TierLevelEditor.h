//---------------------------------------------------------------------------

#ifndef TierLevelEditorH
#define TierLevelEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "ZForm.h"
#include <Grids.hpp>
#include "DBTierLevel.h"


//---------------------------------------------------------------------------
class TfrmTierLevelEditor : public TZForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TTouchBtn *btnTierName;
        TTouchBtn *btnTierLevel;
        TTouchBtn *btnPointsRequired;
        TTouchBtn *btnBdayBonus;
        TGroupBox *GroupBox1;
        TTouchBtn *btnPricedPoints;
        TLabel *Label1;
        TLabel *Label2;
        TTouchBtn *btnWeighedGrams;
        TTouchBtn *btnWeighedPoints;
        TTouchBtn *btnOk;
        TTouchBtn *btnCancel;
        TLabel *Label3;
        TLabel *Label4;
        TCheckBox *cbEmailHeadOffice;
        TCheckBox *cbEmailMember;
        TCheckBox *cbChangeCard;
        TCheckBox *cbAllowEarntRedemption;
        void __fastcall btnCancelMouseClick(TObject *Sender);
        void __fastcall btnOkMouseClick(TObject *Sender);
        void __fastcall btnTierNameMouseClick(TObject *Sender);
        void __fastcall btnTierLevelMouseClick(TObject *Sender);
        void __fastcall btnPointsRequiredMouseClick(TObject *Sender);
        void __fastcall btnBdayBonusMouseClick(TObject *Sender);
        void __fastcall btnPricedPointsMouseClick(TObject *Sender);
        void __fastcall btnWeighedGramsMouseClick(TObject *Sender);
        void __fastcall btnWeighedPointsMouseClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	Database::TDBControl &DBControl;
        void Reset();
        int DisplayIntegerNumpad(int initialValue, AnsiString caption);
        Currency DisplayCurrencyNumpad(Currency initialValue, AnsiString caption);
        void AddTierLevel();
        void UpdateTierLevel();
        void GetTierLevelInformation();
        bool CheckTierLevelExist();
        bool CheckTierLevelData(AnsiString &message);
public:		// User declarations
        __fastcall TfrmTierLevelEditor(TComponent* Owner, Database::TDBControl &inDBControl);
        __fastcall ~TfrmTierLevelEditor();
        TTierLevel *TierLevel;
        bool IsEditMode;
        int TierId;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmTierLevelEditor *frmTierLevelEditor;
//---------------------------------------------------------------------------
#endif
