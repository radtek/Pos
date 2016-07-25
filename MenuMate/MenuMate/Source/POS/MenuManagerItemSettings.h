#ifndef MenuManagerItemSettingsH
#define MenuManagerItemSettingsH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <memory>

#include "MM_DBCore.h"
#include "MMTouchNumpad.h"

#include <key_types.hh>

using std::auto_ptr;

namespace item_management {

class i_item_manager;
class TSizeProperties;

class TfrmItemSettings : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlAll;
	TGroupBox *gbItemSettings;
	TLabel *lblPrice;
	TLabel *lblSpecialPrice;
	TEdit *edPrice;
	TEdit *edSpecialPrice;
	TGroupBox *gbProductCountSettings;
	TLabel *lblCurrentCount;
	TEdit *edCurrentCount;
	TTouchBtn *btnSave;
	TTouchBtn *btnCancel;
   TCheckBox *cbEnableCountingFunctionality;
   TLabel *lblWarnWhenBelow;
   TEdit *edWarnWhenBelow;
   TCheckBox *cbEnableWarnings;
   TCheckBox *cbResetToDefaultValue;
   TLabel *lblDefaultValue;
   TEdit *edDefaultValue;
    TPanel *Panel2;

	void __fastcall cancel_clicked(TObject *sender);
	void __fastcall save_clicked(TObject *sender);
	void __fastcall price_clicked(TObject *sender);
	void __fastcall special_price_clicked(TObject *sender);
	void __fastcall available_quantity_clicked(TObject *sender);
	void __fastcall warn_when_below_clicked(TObject *sender);
	void __fastcall default_clicked(TObject *sender);
   void __fastcall EnableCountingFunctionalityClicked(TObject *Sender);
   void __fastcall EnableWarningsClicked(TObject *Sender);
   void __fastcall reset_to_default_value_clicked(TObject *sender);

private:
	void get_input_currency(Currency &io);
	void get_input_double  (double &io);

	void update_display();

	TColor                    edit_bg_colors[2];
	int                       has_n_sizes;
	auto_ptr<TfrmTouchNumpad> numpad;

   TSizeProperties &properties_;

public:
	__fastcall TfrmItemSettings(TComponent *owner,
                               TSizeProperties &properties);
};

} /* item management */

#endif

