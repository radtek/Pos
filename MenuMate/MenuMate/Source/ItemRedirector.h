//---------------------------------------------------------------------------

#ifndef ItemRedirectorH
#define ItemRedirectorH

#include <System.hpp>

#include "ItemsCompleteCompressed.h"
#include "ListOptionContainer.h"

//---------------------------------------------------------------------------
enum TDisplayItemType
{
	itNormalItem, itPromoItem, itSetMenuMasterItem, itSubItem, itPrevItem, itOption, itMembershipDisplay, itMembershipDisplayNote,
	itEarntPts, itLoadedPts, itServingCourseDisplay, itDiscountDisplay
};

typedef Set <TDisplayItemType, itNormalItem, itDiscountDisplay> TDisplayItemTypes;

class TItemRedirector : public TObject
{
public:
	TDisplayItemTypes	ItemType;
   TItemRedirector *ParentRedirector;
	TItemsCompleteCompressed *CompressedContainer;
   void *ItemObject;
	bool Selected;
   TListOptionContainer *OptList;
   TList *SubOrdersList;
   TItemRedirector(TItemsCompleteCompressed *inCompressed);
};
#endif
