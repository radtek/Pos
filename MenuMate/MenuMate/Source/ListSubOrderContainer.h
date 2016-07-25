//---------------------------------------------------------------------------

#ifndef ListSubOrderContainerH
#define ListSubOrderContainerH

#include <Generics.Collections.hpp>

class TItemCompleteSub;

class TListSubOrderContainer : public TList
{
public:
   typedef int __fastcall (*TListSortCompare)(void * Item1, void * Item2);
   TListSortCompare SubComp;

   int SubOrderAdd(TItemCompleteSub *in);
   void SubOrderDelete(TItemCompleteSub *in);
   void SubOrderClear();
   bool Compare(TListSubOrderContainer *in);
   void SubOrderCopyList(TListSubOrderContainer *inSubOrderList);
	TItemCompleteSub *SubOrderGet(int Index);
	int SubOrderGetIndex(TItemCompleteSub *SubOrder);
   virtual __fastcall ~TListSubOrderContainer();
   virtual __fastcall TListSubOrderContainer();
};
#endif
