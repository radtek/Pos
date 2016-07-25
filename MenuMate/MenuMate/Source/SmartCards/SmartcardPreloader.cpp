#include <vcl.h>
#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "SmartCardAPI.h"
#include "SmartcardPreloader.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
#include "GlobalSettings.h"

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

enum evh_index {
	CALL_INSERT,
	CALL_REMOVE,
   CALL_DUMMY,
	__N_EVH_INDICES
};

const UnicodeString TSmartcard_Preloader::preload_member_name = "preloaded";
const UnicodeString TSmartcard_Preloader::preload_member_number = "preloaded";

__fastcall
TSmartcard_Preloader::TSmartcard_Preloader()
  : TForm(Screen->ActiveForm),
    amount_to_load(0.0),
    has_been_warned(false),
    mmsc(*TDeviceRealTerminal::Instance().ManagerMembership),
    msc(*mmsc.ManagerSmartCards),
    n_lines_status_can_hold(lbStatus->Height / lbStatus->ItemHeight - 1)
{
	fill_event_pointer_table();
	set_smartcard_event_states(&TSystemEvents::Disable);
	swap_existing_event_handlers();

	preload_member.Alias = preload_member_name;
	preload_member.Name = preload_member_name;
	preload_member.MembershipNumber = preload_member_number;
	preload_member.SiteID = TGlobalSettings::Instance().SiteID;
}

void
TSmartcard_Preloader::fill_event_pointer_table()
{
   smartcard_events[CARD_INSERTED]         = &msc.OnCardInserted;
   smartcard_events[CARD_REMOVED]          = &msc.OnCardRemoved;
	smartcard_events[NEW_CARD_INSERTED]     = &msc.OnNewCardInserted;
	smartcard_events[UNKNOWN_CARD_INSERTED] = &msc.OnUnknownCardInserted;
   smartcard_events[UNABLE_TO_DECODE_CARD] = &msc.OnUnableToDecodeCard;
   smartcard_events[CARD_UPDATED]          = &msc.OnCardUpdated;
   smartcard_events[CARD_CORRUPT]          = &msc.OnCardCorrupt;
   smartcard_events[CARD_UNREADABLE]       = &msc.OnCardUnreadable;
}

__fastcall
TSmartcard_Preloader::~TSmartcard_Preloader()
{
	restore_original_event_handlers();
	set_smartcard_event_states(&TSystemEvents::Enable);
}

void
TSmartcard_Preloader::swap_existing_event_handlers()
{
	static const unsigned int handler_selection[__N_EVPT_INDICES] = {
		CALL_INSERT,
		CALL_REMOVE,
		CALL_INSERT,
		CALL_INSERT,
		CALL_DUMMY,
		CALL_DUMMY,
		CALL_INSERT,
		CALL_DUMMY
	};

	std::vector<TMMEvent> new_handlers[__N_EVH_INDICES];

	new_handlers[CALL_INSERT].push_back(&smartcard_inserted);
	new_handlers[CALL_REMOVE].push_back(&smartcard_removed);

	for (int i = 0; i < __N_EVPT_INDICES; i++)
		original_event_handlers[i] =
		  smartcard_events[i]->swap_handlers(new_handlers[handler_selection[i]]);
}

void
TSmartcard_Preloader::set_smartcard_event_states(
  void (TSystemEvents::*alter_state_fun)())
{
	for (int i = 0; i < __N_EVPT_INDICES; i++)
		(*smartcard_events[i].*alter_state_fun)();
}

void
TSmartcard_Preloader::restore_original_event_handlers()
{
	for (int i = 0; i < __N_EVPT_INDICES; i++)
		smartcard_events[i]->set_handlers(original_event_handlers[i]);
}

#pragma warn -par

void
TSmartcard_Preloader::write_error_text(const UnicodeString header)
{
	write_status_text(header);
	write_status_text("Ensure that the card's contacts are clean");
	write_status_text("and that the card is firmly inserted into");
	write_status_text("the reader.");
	write_status_text("");
}

void
TSmartcard_Preloader::smartcard_inserted(TSystemEvents *event)
{
	btnTogglePreloader->Enabled = false;
	btnTogglePreloader->Refresh();

	write_status_text("Smartcard insertion detected.");
	write_status_text("Clearing data...");

   if (!msc.FormatCardToFactory()) {
		write_error_text("Card format failed!");
		return;
	}

	write_status_text("Card erased.");
	write_status_text("Writing preload data...");

	msc.BeginTransaction();
	try {
      	msc.SaveContactInfo(preload_member);
		msc.SaveContactPoints(preload_member.Points);
	} catch (...) {
		write_error_text("Failed to write member data.");
      msc.EndTransaction();
		throw;
	}

    msc.EndTransaction();
	write_status_text("Preload data written sucessfully.");
	write_status_text("Remove the smartcard.");
	write_status_text("");
}

void
TSmartcard_Preloader::smartcard_removed(TSystemEvents *event)
{
	btnTogglePreloader->Enabled = true;
	write_status_text("Insert a smartcard to stamp.");
}

void __fastcall
TSmartcard_Preloader::number_to_load_clicked(TObject *sender)
{
   std::auto_ptr<TfrmTouchNumpad> numpad(
	  TZForm::Create<TfrmTouchNumpad>(this));

	numpad->Mode = pmCurrency;
	numpad->CURInitial = amount_to_load;
	numpad->btnCancel->Visible = true;
	numpad->btnOk->Visible = true;

	if (numpad->ShowModal() == mrOk)
    {
   		amount_to_load = numpad->CURResult;
      	btnNumberOfPointsToLoad->Caption =
		UnicodeString::CurrToStrF(amount_to_load, UnicodeString::sffCurrency, 2);

        if( TGlobalSettings::Instance().EnableSeperateEarntPts )
        {
            // preload into loaded points
            TPointsTypePair typepair1(pttPurchased,ptstAccount);
            TPointsType type1(pasDatabase, typepair1,pesNone);
            preload_member.Points.Load(type1,amount_to_load);
        }
        else
        {
            // preload into earned points
            TPointsTypePair typepair2(pttEarned,ptstLoyalty);
            TPointsType type2(pasDatabase, typepair2,pesNone);
            preload_member.Points.Load(type2,amount_to_load);
        }

        /* older code - this has to be changed depending on which version of loyalty we use. if we use nz we should load to earnt points
            and if we use aussi, we should use loaded points
        TPointsTypePair typepair(pttEarned,ptstLoyalty);
        TPointsType type(pasDatabase, typepair,pesNone);
        preload_member.Points.Load(type,amount_to_load); */

        /* This was the old Code, Looks incorrect to me. Is the preloader really
        supposed to set the Current points and the point purchased to the same amount?

		preload_member.Points.CurrentPoints = amount_to_load;
		preload_member.Points.CurrentPointsHeldTotal = amount_to_load;
		preload_member.Points.PointsPurchased = amount_to_load;*/

		btnNumberOfPointsToLoad->Tag = amount_to_load != 0;
		btnTogglePreloader->Enabled = true;
	}
}

void __fastcall
TSmartcard_Preloader::close_clicked(TObject *sender)
{
   ModalResult = mrOk;
}

void __fastcall
TSmartcard_Preloader::preloader_start(TObject *sender)
{
	if (!has_been_warned
	    && MessageBox("Any card you insert while the preload process is active "
                     "will be erased. All data on the card, such as points, "
	                  "will be lost. Are you sure you want to continue?",
		               "Warning", MB_ICONEXCLAMATION | MB_YESNO) != IDYES) {
		return;
	}

	has_been_warned = true;

	btnTogglePreloader->Caption = "Stop Batch Preloader";
	btnTogglePreloader->OnMouseClick = &preloader_stop;
	btnTogglePreloader->ButtonColor = clMaroon;
	btnNumberOfPointsToLoad->Enabled = false;
	btnClose->Enabled = false;

	set_smartcard_event_states(&TSystemEvents::Enable);
	smartcard_removed(NULL);
}

void __fastcall
TSmartcard_Preloader::preloader_stop(TObject *sender)
{
	set_smartcard_event_states(&TSystemEvents::Disable);

	btnTogglePreloader->Caption = "Start Batch Preloader";
	btnTogglePreloader->OnMouseClick = &preloader_start;
	btnTogglePreloader->ButtonColor = 0x2193F6;

	btnNumberOfPointsToLoad->Enabled = true;
	btnClose->Enabled = true;
}

#pragma warn .par

void
TSmartcard_Preloader::write_status_text(const UnicodeString text)
{
	lbStatus->Items->Add(text);
	lbStatus->TopIndex =
	  lbStatus->TopIndex + (lbStatus->Count > n_lines_status_can_hold);
	lbStatus->Update();
}

bool
TSmartcard_Preloader::is_preloaded_card(const TMMContactInfo &info)
{
	return info.Name.Length() == preload_member_name.Length()
	       && info.Alias.Length() == preload_member_name.Length()
           && info.MembershipNumber.Length() == preload_member_number.Length()
	       && info.Name == preload_member_name
	       && info.MembershipNumber == preload_member_number;
}

