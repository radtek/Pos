#include <vcl.h>
#pragma hdrstop

#include "SetButtonFontSize.h"

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//TfrmSetButtonFontSize *frmSetButtonFontSize;

#define custom_flag_mask    0x80
#define fnt_sz_is_custom(x) ((x) & (custom_flag_mask))

const int TfrmSetButtonFontSize::font_sizes[] = {
	4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36
};

const int TfrmSetButtonFontSize::sz_xlat[3][4] = {
		{6,  6, 6, 4}, {9, 9, 8, 8}, {10, 10, 10, 8}
};

TDeviceRealTerminal &TfrmSetButtonFontSize::ref_drt =
  TDeviceRealTerminal::Instance();
TGlobalSettings  &TfrmSetButtonFontSize::ref_gs = TGlobalSettings::Instance();
TManagerVariable &TfrmSetButtonFontSize::ref_mv = TManagerVariable::Instance();

__fastcall TfrmSetButtonFontSize::TfrmSetButtonFontSize(TComponent* Owner)
	: TForm  (Owner),
     transaction(ref_drt.DBControl)
{
	def_fnt_sz_for_curr_res = GetDefaultFontSize();

   tgridPreviewFontSz->ColCount = ref_gs.DefaultColumns;
	tgridPreviewFontSz->RowCount = ref_gs.DefaultRows;

	ref_drt.RegisterTransaction(transaction);

	transaction.StartTransaction();
	global_prof_key = ref_mv.GetProfile(transaction,
	                                    ProfileType::eSystemProfiles, "Globals");
	transaction.Commit();

	curr_fnt_sz = GetCurrentFontSize(transaction, global_prof_key);

	FillFontPreviewGrid();
	FillFontSelectionGrid();
}

void TfrmSetButtonFontSize::UpdateFontInfoText()
{
	lblDefFontSz->Caption =
	  "The default font size for "
	  + IntToStr(Screen->Width)
	  + "x" + IntToStr(Screen->Height)
	  + " is "
	  + IntToStr(def_fnt_sz_for_curr_res)
	  + "pts. The currently selected size is "
	  + IntToStr(curr_fnt_sz) + "pts.";
}

void TfrmSetButtonFontSize::SetFontSize(TDBTransaction &transaction,
                                        int profile_key, int size)
{
	transaction.StartTransaction();
   ref_mv.SetProfileInt(transaction, profile_key,
	                     vmVariables::vmPosGridFontSize, size);
	transaction.Commit();
	ref_gs.posButtonFontSize = size;
}

void TfrmSetButtonFontSize::FillFontPreviewGrid()
{
	AnsiString *generated_captions =
	  new AnsiString[ref_gs.DefaultColumns];

	try {
      int i = 0, j = 0, k = 0, m = ref_gs.DefaultColumns;

      UpdateFontInfoText();

      tgridPreviewFontSz->Font->Size = curr_fnt_sz;

		for (AnsiString base; i < m; k = 0, i++) {
         base = "Item #" + IntToStr(i + 1) + "\n";
			generated_captions[i] = base;
			while (k++ < i)
				generated_captions[i] = generated_captions[i] + base;
		}

      for (i = j = k = 0 ; k < m; k = i + j * ref_gs.DefaultColumns) {
         tgridPreviewFontSz->Buttons[j][i]->Caption = generated_captions[i++];
         j += (i %= ref_gs.DefaultColumns) == 0;
      }

      while (j < ref_gs.DefaultRows) {
         tgridPreviewFontSz->Buttons[j][i++]->Visible = false;
         j += (i %= ref_gs.DefaultColumns) == 0;
      }
	} __finally {
		delete[] generated_captions;
	}
}

void TfrmSetButtonFontSize::FillFontSelectionGrid()
{
	int i = 0, j = sizeof(font_sizes) / sizeof(int);

	tgridFontSizes->ColCount = j;
	tgridFontSizes->RowCount = 1;

	for ( ; i < j; i++) {
		tgridFontSizes->Buttons[0][i]->Caption = IntToStr(font_sizes[i]);
		tgridFontSizes->Buttons[0][i]->Latched = font_sizes[i] == curr_fnt_sz;
	}
}

void __fastcall TfrmSetButtonFontSize::SaveOrDiscard(TObject *Sender)
{
	if (reinterpret_cast<TTouchBtn *>(Sender)->Tag == 0)
		SetFontSize(transaction, global_prof_key, curr_fnt_sz);

	ModalResult = 0;
	Close();
}

void __fastcall TfrmSetButtonFontSize::btnResetToDefaultMouseClick(
  TObject *Sender)

{
	curr_fnt_sz = def_fnt_sz_for_curr_res;
	FillFontPreviewGrid();
	FillFontSelectionGrid();
}

void __fastcall TfrmSetButtonFontSize::NewFontSizeSelected(
  TObject *Sender, TMouseButton Button, TShiftState Shift,
  TGridButton *GridButtonSelected)
{
	curr_fnt_sz = StrToInt(GridButtonSelected->Caption);
	FillFontPreviewGrid();
}

int TfrmSetButtonFontSize::GetDefaultFontSize()
{
     return sz_xlat[(Screen->Height != 600)
                    + (Screen->Height != 768)][ref_gs.DefaultColumns - 4];
}

void TfrmSetButtonFontSize::ResetToDefaultFontSize()
{
	TDBTransaction transaction(ref_drt.DBControl);

	SetFontSize(transaction, GetProfileKey(transaction),
               custom_flag_mask | GetDefaultFontSize());
}

int TfrmSetButtonFontSize::GetProfileKey(TDBTransaction &transaction)
{
	int global_prof_key;

	transaction.StartTransaction();
	if (!(global_prof_key = ref_mv.GetProfile(transaction,
                                             ProfileType::eSystemProfiles,
                                             "Globals"))) {
		global_prof_key = ref_mv.SetProfile(transaction,
	                                       ProfileType::eSystemProfiles,
		                                    "Globals");
	}
	transaction.Commit();

	return global_prof_key;
}

bool TfrmSetButtonFontSize::IsCurrentSizeDefault()
{
	return fnt_sz_is_custom(ref_gs.posButtonFontSize);
}

int TfrmSetButtonFontSize::GetCurrentFontSize(TDBTransaction &transaction,
                                              int global_prof_key)
{
	int curr_fnt_sz;
	bool value_exists;

	transaction.StartTransaction();
	value_exists = ref_mv.GetProfileInt(transaction, global_prof_key,
	                                    vmVariables::vmPosGridFontSize,
	                                    curr_fnt_sz);
	transaction.Commit();
	if (!value_exists)
		SetFontSize(transaction, global_prof_key, curr_fnt_sz = 0x8a);

	return curr_fnt_sz & ~custom_flag_mask;
}

