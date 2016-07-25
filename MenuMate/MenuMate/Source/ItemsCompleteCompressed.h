//---------------------------------------------------------------------------

#ifndef ItemsCompleteCompressedH
#define ItemsCompleteCompressedH

#include "ItemMinorComplete.h"

class TContainerOrders;
class TItemComplete;
class TItemRedirector;

//---------------------------------------------------------------------------
enum TICType {icIncompleteItems,icCompleteItems,icMemberInfo,icDiscount,icServingCourse};

class TItemsCompleteCompressed
{
	public:

		TContainerOrders *Container;
		TICType Type;

		int ServingCourseKey; // Used to determine if this seting course already exists.
		bool Visible;
		__property TItemMinorComplete * ItemsList[int Index] = {read=Get};
		__property int Count = {read=GetCount};
		__property Currency Qty = {read=GetQty};
		TStringList *Display;
		__fastcall TItemsCompleteCompressed(TContainerOrders *inContainer);
		virtual __fastcall ~TItemsCompleteCompressed();
		void BuildSelf();
		bool Contains(TItemMinorComplete *);
		int Add(TItemMinorComplete * Item);
		int Remove(TItemMinorComplete * Item);
		int IndexOf(TItemMinorComplete * Item) {return fItemsList->IndexOf(Item);}

	private:

		TItemMinorComplete * Get(int Index)				{return (TItemMinorComplete *)fItemsList->Items[Index];}
		Currency GetQty();
		int GetCount(){return fItemsList->Count;}

		void clearDisplayFormat();
		void formatServingCourse();
		void formatCompleteItem();
		void formatOtherItem();
		void formatSetMenuMaster(TItemComplete* ItemComplete);
		void formatNormalItem(TItemComplete* ItemComplete);
		void formatNormalItemNMI( TItemComplete* ItemComplete, TItemRedirector* ItemRedirect );
		void formatOptionsForItem(TItemComplete * ItemComplete, TItemRedirector* ItemRedirect);
		void formatSidesForItem(TItemComplete* ItemComplete, TItemRedirector* ItemRedirect);

		UnicodeString getCurrencySymbol();

		TList *fItemsList;
        bool dc_item;
};

//---------------------------------------------------------------------------

#endif
