//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListSubOrderContainer.h"
#include "ItemCompleteSub.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall SubOrderCompare(void *Item1, void *Item2)
{
   try
   {
       TItemCompleteSub* SubOrder1 = (TItemCompleteSub*)Item1;
       TItemCompleteSub* SubOrder2 = (TItemCompleteSub*)Item2;

       if (SubOrder1->Course < SubOrder2->Course)
       {
          return 1;
       }
       else if (SubOrder1->Course == SubOrder2->Course)
       {
          if (SubOrder1->Item < SubOrder2->Item)
          {
             return 1;
          }
          else if (SubOrder1->Item == SubOrder2->Item)
          {
             if (SubOrder1->Item_ID < SubOrder2->Item_ID)
             {
                return 1;
             }
             else if (SubOrder1->Item_ID == SubOrder2->Item_ID)
             {
                if (SubOrder1->PriceEach() < SubOrder2->PriceEach())
                {
                   return 1;
                }
                else if (SubOrder1->PriceEach() == SubOrder2->PriceEach())
                {
                   if (SubOrder1->Size < SubOrder2->Size)
                   {
                      return 1;
                   }
                   else if (SubOrder1->Size == SubOrder2->Size)
                   {
                      return 0;
                   }
                   else
                   {
                      return -1;
                   }
                }
                else
                {
                   return -1;
                }
             }
             else
             {
                return -1;
             }
          }
          else
          {
             return -1;
          }
       }
       else
       {
          return -1;
       }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	}
}

int __fastcall SubOrdersCompare(TListSubOrderContainer *List1, TListSubOrderContainer *List2)
{
    try
    {

        if (List1->Count < List2->Count)
        {
          return 1;
        }
        else if (List1->Count == List2->Count)
        {
          for (int i = 0; i < List1->Count; i++)
          {
             int RetVal = SubOrderCompare(List1->SubOrderGet(i), List2->SubOrderGet(i)); // Overloaded Function.
             if (RetVal != 0)
             {
                return RetVal;
             }
          }
          return 0;
        }
        else
        {
          return -1;
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	}
}

__fastcall TListSubOrderContainer::TListSubOrderContainer()
{
   SubComp = &SubOrderCompare;
}

__fastcall TListSubOrderContainer::~TListSubOrderContainer()
{
   SubOrderClear();
}

void TListSubOrderContainer::SubOrderClear()
{
   while (Count > 0)
   {
	  SubOrderDelete(SubOrderGet(0));
   }
}

TItemCompleteSub *TListSubOrderContainer::SubOrderGet(int Index)
{
   if (Count > Index)
   {
	  return(TItemCompleteSub*)Items[Index];
   }
   return NULL;
}

int TListSubOrderContainer::SubOrderGetIndex(TItemCompleteSub *SubOrder)
{
   return IndexOf(SubOrder);
}

void TListSubOrderContainer::SubOrderCopyList(TListSubOrderContainer *inSubOrderList)
{
   while (Count > 0)
   {
	  SubOrderDelete(0);
   }
   for (int i = 0; i < inSubOrderList->Count; i++)
   {
	  TItemCompleteSub *NewSubOrder = new TItemCompleteSub();
     inSubOrderList->SubOrderGet(i)->Assign(NewSubOrder);
	  SubOrderAdd(NewSubOrder);
   }
}

int TListSubOrderContainer::SubOrderAdd(TItemCompleteSub *in)
{
   in->Owner = this;
   return Add(in);
}

void TListSubOrderContainer::SubOrderDelete(TItemCompleteSub *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TItemCompleteSub*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}



bool TListSubOrderContainer::Compare(TListSubOrderContainer *in)
{
   if (SubOrdersCompare(this, in) == 0)
   {
	  return true;
   }
   return false;
}