#ifndef SmartcardPreloaderH
#define SmartcardPreloaderH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "MMContactInfo.h"
#include "SystemEvents.h"

#include <map>
#include <vector>

class TManagerMembershipSmartCards;
class TManagerSmartCard;

class TSmartcard_Preloader : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlContents;
	TTouchBtn *btnNumberOfPointsToLoad;
	TTouchBtn *btnTogglePreloader;
	TListBox *lbStatus;
	TTouchBtn *btnClose;
	void __fastcall number_to_load_clicked(TObject *sender);
	void __fastcall preloader_start(TObject *sender);
	void __fastcall preloader_stop(TObject *sender);
	void __fastcall close_clicked(TObject *sender);

private:	// User declarations
   enum evpt_index {
      CARD_INSERTED,
      CARD_REMOVED,
      NEW_CARD_INSERTED,
      UNKNOWN_CARD_INSERTED,
      UNABLE_TO_DECODE_CARD,
      CARD_UPDATED,
      CARD_CORRUPT,
      CARD_UNREADABLE,
		__N_EVPT_INDICES
   };

	void smartcard_inserted(TSystemEvents *event);
	void smartcard_removed(TSystemEvents *event);

	void set_smartcard_event_states(void (TSystemEvents::*alter_state_fun)());
	void fill_event_pointer_table();
	void swap_existing_event_handlers();
	void restore_original_event_handlers();
	void write_status_text(const UnicodeString text);
	void write_error_text(const UnicodeString header);

	std::vector<TMMEvent> original_event_handlers[__N_EVPT_INDICES];
	TSystemEvents *smartcard_events[__N_EVPT_INDICES];

	Currency amount_to_load;
	bool has_been_warned;
	TManagerMembershipSmartCards &mmsc;
	TManagerSmartCard &msc;
	TMMContactInfo preload_member;
	int n_lines_status_can_hold;
	int status_line_item_height;

public:		// User declarations
	__fastcall TSmartcard_Preloader();
	__fastcall ~TSmartcard_Preloader();

	static bool is_preloaded_card(const TMMContactInfo &info);

	static const UnicodeString preload_member_name;
	static const UnicodeString preload_member_number;
};

#endif

