#include <vcl.h>
#pragma hdrstop

#include <System.hpp>

#include "MMMessageBox.h"
#include "MenuManagerItemSettings.h"

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#include "MenuManagerItemSizeSelection.h"

namespace item_management {

__fastcall
TfrmItemSettings::TfrmItemSettings(
  TComponent* owner,
  TSizeProperties &properties)
    : TForm(owner),
      numpad(TZForm::Create<TfrmTouchNumpad>(this)),
      properties_(properties)
{
	edit_bg_colors[0] = clWindow;
	edit_bg_colors[1] = clBtnFace;

	numpad->btnCancel->Visible = true;
	numpad->btnOk->Visible     = true;

   edPrice->Enabled = true;
	edSpecialPrice->Enabled = true;

	update_display();
}

#pragma warn -par
void __fastcall
TfrmItemSettings::cancel_clicked(TObject *sender)
{
   properties_.RevertToOriginalState();

	ModalResult = mrCancel;
}

void __fastcall
TfrmItemSettings::available_quantity_clicked(TObject *sender)
{
   double number_available = properties_.GetAvailableQuantity();

	get_input_double(number_available);
   properties_.SetAvailableQuantity(number_available);

	update_display();
}

void __fastcall
TfrmItemSettings::default_clicked(TObject *sender)
{
   double default_value = properties_.GetDefaultQuantity();

	get_input_double(default_value);

   properties_.SetDefaultQuantity(default_value);
	update_display();
}

#pragma warn .par

void
TfrmItemSettings::get_input_currency(Currency &io)
{
	numpad->Caption    = "Enter a new price";
	numpad->CURInitial = io;
	numpad->Mode       = TPadMode::pmCurrency;

	if (numpad->ShowModal() == mrCancel)
		return;

	io = numpad->CURResult;
}

void
TfrmItemSettings::get_input_double(double &io)
{
	numpad->Caption    = "Enter a new value";
	numpad->CURInitial = io;
	numpad->Mode       = TPadMode::pmDecimal;

	if (numpad->ShowModal() == mrCancel)
		return;

	io = numpad->CURResult;
}

#pragma warn -par
void __fastcall
TfrmItemSettings::price_clicked(TObject *sender)
{
   Currency price;

	get_input_currency(price);

   properties_.SetPrice(price);
	update_display();
}

void __fastcall
TfrmItemSettings::save_clicked(TObject *sender)
{
	ModalResult = mrOk;
}
#pragma warn .par

#pragma warn -par
void __fastcall
TfrmItemSettings::special_price_clicked(TObject *sender)
{
   Currency special_price;

	get_input_currency(special_price);

   properties_.SetSpecialPrice(special_price);
	update_display();
}

void __fastcall
TfrmItemSettings::reset_to_default_value_clicked(TObject *sender)
{
   static const double default_quantity[2] = {-1, 0};

   properties_.SetDefaultQuantity(
     default_quantity[cbResetToDefaultValue->Checked]);

   update_display();
}

void __fastcall
TfrmItemSettings::warn_when_below_clicked(TObject *sender)
{
   double warning_value = properties_.GetWarningQuantity();

   get_input_double(warning_value);
   if (warning_value < 1) {
      MessageBox("The warning threshold must be larger than zero.",
                 "Invalid warning threshold.",
                 MB_ICONERROR | MB_OK);
      return;
   }

   properties_.SetWarningQuantity(warning_value);
   update_display();
}

#pragma warn .par

void
TfrmItemSettings::update_display()
{
   const TColor edit_background_color[2] = {clBtnFace, clWindow};
   const UnicodeString default_value_str[2] = {
     "",
     UnicodeString::FormatFloat("0.00", properties_.GetDefaultQuantity())
   };
   const UnicodeString n_available_str[2] = {
     "",
     UnicodeString::FormatFloat("0.00", properties_.GetAvailableQuantity())
   };
   const UnicodeString warning_str[2] = {
     "",
     UnicodeString::FormatFloat("0.00", properties_.GetWarningQuantity())
   };

   const double n_available = properties_.GetAvailableQuantity();
   const bool count_functionality_enabled = n_available != -1;
   const double warning_quantity = properties_.GetWarningQuantity();
   const bool warning_enabled =
     warning_quantity > 0 && count_functionality_enabled;
   const double default_quantity = properties_.GetDefaultQuantity();
   const bool defaulting_enabled =
     default_quantity != -1 && count_functionality_enabled;

	edPrice->Text =
	  UnicodeString::CurrToStrF(properties_.GetPrice(),
                               UnicodeString::sffCurrency, 2);
	edSpecialPrice->Text =
	  UnicodeString::CurrToStrF(properties_.GetSpecialPrice(),
                               UnicodeString::sffCurrency, 2);

   cbEnableCountingFunctionality->OnClick = 0x0;
   cbEnableWarnings->OnClick = 0x0;
   cbResetToDefaultValue->OnClick = 0x0;

   cbEnableCountingFunctionality->Checked = count_functionality_enabled;

   cbEnableWarnings->Enabled = count_functionality_enabled;
   cbResetToDefaultValue->Enabled = count_functionality_enabled;
   cbEnableWarnings->Checked = warning_enabled;
   cbResetToDefaultValue->Checked = defaulting_enabled;

   cbResetToDefaultValue->OnClick =
     &reset_to_default_value_clicked;
   cbEnableWarnings->OnClick = &EnableWarningsClicked;
   cbEnableCountingFunctionality->OnClick =
     &EnableCountingFunctionalityClicked;

	edCurrentCount->Enabled = count_functionality_enabled;
   edCurrentCount->Text = n_available_str[count_functionality_enabled];

   edDefaultValue->Enabled = defaulting_enabled;
   edDefaultValue->Text = default_value_str[defaulting_enabled];

   edWarnWhenBelow->Enabled = warning_enabled;
   edWarnWhenBelow->Text = warning_str[warning_enabled];

	edCurrentCount->Color = edit_background_color[count_functionality_enabled];
   edDefaultValue->Color = edit_background_color[defaulting_enabled];
   edWarnWhenBelow->Color =
     edit_background_color[warning_enabled];
}

#pragma warn -par
void __fastcall
TfrmItemSettings::EnableCountingFunctionalityClicked(TObject *sender)
{
   static const double available_quantity[2] = {-1, 1};

   properties_.SetAvailableQuantity(
     available_quantity[cbEnableCountingFunctionality->Checked]);
   properties_.SetDefaultQuantity(-1);
   properties_.SetWarningQuantity(0);

   update_display();
}

void __fastcall TfrmItemSettings::EnableWarningsClicked(TObject *sender)
{
   static const double warning_quantity[2] = {0, 1};

   properties_.SetWarningQuantity(warning_quantity[cbEnableWarnings->Checked]);
   update_display();
}

#pragma warn .par

} /* item_management */



