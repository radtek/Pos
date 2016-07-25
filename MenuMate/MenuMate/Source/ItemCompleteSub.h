//---------------------------------------------------------------------------

#ifndef ItemCompleteSubH
#define ItemCompleteSubH

#include "ItemMinorComplete.h"
//---------------------------------------------------------------------------
class TItemCompleteSub : public TItemMinorComplete
{
	public:
   TListSubOrderContainer *Owner;
	long MasterOrderKey;            // Orders Key is returned in this feild.
	int GetMasterOrderKey(){return MasterOrderKey;}

	/* If you are using Assign to create a COPY for a new
   order i.e a Split you will need to reassign the Security Ref
   with TDBSecurity::GetNextSecurityRef() */
	void Assign(TItemCompleteSub *RetItem);

   TItemCompleteSub();
	virtual __fastcall ~TItemCompleteSub();
   friend bool operator==(const TItemCompleteSub &f1, const TItemCompleteSub &f2);

        int DefaultPatronCount() const;
        int PatronCount() const;
        int PatronCount(const int, const bool);
};

bool operator == (TItemCompleteSub &x,TItemCompleteSub &y)
{
   if (  (x.MasterOrderKey == y.MasterOrderKey) &&
		   (x.OrderKey == y.OrderKey) &&
         (x.Item_ID == y.Item_ID) &&
         (x.Item == y.Item) &&
         (x.Size == y.Size) &&
         (x.Course == y.Course) &&
		   (x.PriceEach() == y.PriceEach()) &&
         (x.Cost == y.Cost)
      )
      {
         return true;
      }
   return false;
};

#endif
