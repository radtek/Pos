//---------------------------------------------------------------------------

#ifndef ItemCompleteH
#define ItemCompleteH

#include "ChitNumber.h"
#include "ItemMinorComplete.h"
#include "Item.h"

#include "enumTab.h"

//---------------------------------------------------------------------------
class TItemComplete : public TItemMinorComplete
{
	private :
		int Course_Key;           // Class/Course Table
		long Course_ID;            // Class/Course Table
		long MasterContainer_ID;
		int Customer_Key;
		int _activeChitNumberKey;	// active chit number key generated when using hold and send feature
		TChitNumber fChitNumber;


	bool discounts_compare(TItemComplete &other_item);
	bool options_compare(TItemComplete &other_item);

	public :
   int OriginalItemKey;
	TItem *ItemOrderedFrom;

	long CoursePrintingOrder;

	__property TChitNumber ChitNumber = { read = fChitNumber, write = fChitNumber };


	// Req Top Level Info.
   long Palm_ID;
   int TableNo;
   int SeatNo;
   int RoomNo;
   int TabKey;
   int TimeKey;
   int MenuKey;
   int WebKey;
   void SetActiveChitNumberKey(int activeChitKey){ _activeChitNumberKey = activeChitKey;}
   int GetActiveChitNumberKey(void){ return _activeChitNumberKey; }

	UnicodeString InvoiceNumber;
	UnicodeString OrderedLocation;  	// Order Appears on Palm. (also Bar / Restaurant)
	TMMTabType TabType;					// The Type the order was when it was ordered (Could have been transfered).
	TMMTabType ContainerTabType; 		// The Current Containers Type.

   bool DuplicateDetected;	 	// Already in the DB.
	bool PrintChitNumber;

	TItemType       ItemType;

	UnicodeString Terminal;
	UnicodeString TabName;
	UnicodeString WebID;
	UnicodeString TabContainerName;
	UnicodeString PartyName;
	UnicodeString Note;
	UnicodeString DiscountedBy;
	UnicodeString CreditedBy;
	UnicodeString MasterContainer;
    //MM2038
    UnicodeString IdName;
    //the variable here is following the name in database itself (get permission to use this one not creating the new one)
    UnicodeString IdNumber;
	//---------TCPIP------------
	TListOptionContainer *OptionsSelected;

	TItemComplete();
	virtual __fastcall ~TItemComplete();

	TItemComplete(const TItemComplete &OtherItem);
	TItemComplete& operator= (const TItemComplete &OtherItem);

	/* If you are using Assign to create a COPY for a new
   order i.e a Split you will need to reassign the Security Ref
   with TDBSecurity::GetNextSecurityRef() */
   void virtual Assign(TItemMinor *RetItem);
	TItemComplete *Clone();


	/*
    * This is a "shallow" comparison. It tests the minimum needed to determine
    * whether an item is different from another. This function is only really
	 * used by the AdjustQty(...) function in SelectDish.(cpp|h).
	 *
    * The function compares:
    * - Item keys.
	 * - Price
	 * - Discounts
	 * - Options
	 * - Whether the item has suborders are not
	 *
	 * The suborders themselves are not tested.
	 */
	bool shallow_compare(TItemComplete &against_this);



};

#endif
