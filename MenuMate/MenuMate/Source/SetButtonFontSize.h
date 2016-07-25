#ifndef SetButtonFontSizeH
#define SetButtonFontSizeH

#include <Classes.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>

#include <Db.hpp>
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
#include "ManagerVariable.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>

#include <utility>

using Database::TDBTransaction;

class TfrmSetButtonFontSize : public TForm
{
typedef std::pair<int, int> TButtonDimensions;

__published:
	TTouchBtn  *btnCancelSelection;
	TTouchBtn  *btnSaveSelection;
	TTouchBtn  *btnResetToDefault;
	TGroupBox  *gbPreviewFontSz;
	TGroupBox  *gbSelectFontSz;
	TLabel     *lblDefFontSz;
	TTouchGrid *tgridFontSizes;
	TTouchGrid *tgridPreviewFontSz;
	void __fastcall btnResetToDefaultMouseClick(TObject *);
	void __fastcall NewFontSizeSelected(TObject *, TMouseButton, TShiftState,
                                       TGridButton *);
	void __fastcall SaveOrDiscard(TObject *);
private:
	static TDeviceRealTerminal &ref_drt;
	static TGlobalSettings     &ref_gs;
	static TManagerVariable    &ref_mv;

	static const int font_sizes[];
	static const int sz_xlat[3][4];

	int curr_fnt_sz;
	int def_fnt_sz_for_curr_res;
	int global_prof_key;
	TDBTransaction transaction;

	int  GetSelectedFontSize();
	void FillFontPreviewGrid();
	void FillFontSelectionGrid();
	void UpdateFontInfoText();

	static int  GetProfileKey(TDBTransaction &);
	static void SetFontSize(TDBTransaction &, int, int);

public:
	__fastcall TfrmSetButtonFontSize(TComponent* Owner);

	static int  GetCurrentFontSize(TDBTransaction &, int);
	static int  GetDefaultFontSize();
	static bool IsCurrentSizeDefault();
	static void ResetToDefaultFontSize();
};

extern PACKAGE TfrmSetButtonFontSize *frmSetButtonFontSize;

#endif
