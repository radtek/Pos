//---------------------------------------------------------------------------

#ifndef ContainerOrdersH
#define ContainerOrdersH

#include "ItemComplete.h"
#include "ItemsCompleteCompressed.h"
#include "ItemRedirector.h"
#include "MMContactInfo.h"

//---------------------------------------------------------------------------
class TContainerOrders : public TObject
{
private:
    int GetCount(){return fItems->Count;}
    void SetCount(int in){fItems->Count = in;}
    int GetCompCount(){return fCompressedItemsList->Count;}
    void SetCompCount(int in){fCompressedItemsList->Count = in;}
    TItemComplete * Get(int Index){return (TItemComplete *)fItems->Items[Index];}
    TItemsCompleteCompressed * GetComp(int Index) {return (TItemsCompleteCompressed *)fCompressedItemsList->Items[Index];}
    void PutComp(int Index, TItemsCompleteCompressed *in) {fCompressedItemsList->Items[Index] = in;}
    TItemMinorComplete * GetPrev(int Index)	{return (TItemMinorComplete *)fPrevOrders->Items[Index];}
    int GetPrevCount() {return fPrevOrders->Count;}
    typedef int __fastcall (*TListSortCompare)(void * Item1, void * Item2);
    TListSortCompare ptrSortFunc;
    void __fastcall Sort();
    void __fastcall Compress();
    typedef void __fastcall (__closure *TCurrentItemRedirectorChangedEvent)(TItemRedirector *in);
    TItemRedirector * GetCurrentItemRedirector() {return fCurrentItemRedirector;}
    void PutCurrentItemRedirector(TItemRedirector *in);
    TCurrentItemRedirectorChangedEvent  getCurrentItemRedirectorChanged()      {return fOnCurrentItemRedirectorChanged;}
    void setCurrentItemRedirectorChanged(TCurrentItemRedirectorChangedEvent in)	{fOnCurrentItemRedirectorChanged = in;}
    TItemMinorComplete *getLastItemSelected()		   {return fLastItemSelected;}
    void setLastItemSelected(TItemMinorComplete *in)         {fLastItemSelected = in;}
protected:
    TCurrentItemRedirectorChangedEvent fOnCurrentItemRedirectorChanged;
    TItemRedirector *fCurrentItemRedirector;
    TItemMinorComplete * fLastItemSelected;
    TList *fItems;
    TList *fPrevOrders;
    TList *fCompressedItemsList;
public:
    Currency TotalDiscount;
    bool UnRavelDisplay;
    TItemComplete * LastItemAdded;
    int SelectedIndex;
    __property TCurrentItemRedirectorChangedEvent OnCurrentItemRedirectorChanged =  {read = getCurrentItemRedirectorChanged, write = setCurrentItemRedirectorChanged};
    __property TItemRedirector *CurrentItemRedirector =   {read = GetCurrentItemRedirector, write = PutCurrentItemRedirector};
    __property TItemMinorComplete * LastItemSelected = {read = getLastItemSelected, write = setLastItemSelected};
    __fastcall TContainerOrders();
    __fastcall virtual ~TContainerOrders();
    __property TItemComplete * Items[int Index] = {read=Get};
    __property TItemsCompleteCompressed * CompressedItems[int Index] =  {read=GetComp, write=PutComp};
    __property TList * List =	 {read=fItems};
    __property TItemMinorComplete * PrevItems[int Index] = {read=GetPrev};
    int Add(TItemComplete *Item, TItem *Det);
    int Remove(TItemMinorComplete *Item);
    int Delete(TItemComplete *Item);
    void Delete(int i);
    int AddPrev(TItemMinorComplete *Item) {return fPrevOrders->Add(Item);}
    int IndexPrev(TItemMinorComplete *Item)	 {return fPrevOrders->IndexOf(Item);}
    void DeletePrev(int i) {fPrevOrders->Delete(i);}
    __property int Count = {read=GetCount, write=SetCount};
    __property int CompressedCount = {read=GetCompCount, write=SetCompCount};
    __property int PrevCount = {read=GetPrevCount};
    void Clear() {fItems->Clear(); fPrevOrders->Clear();}
    void ClearPrev() {fPrevOrders->Clear();}
    void Select(TItemMinorComplete *Item);
    void RefreshDisplay();
    TMMContactInfo AppliedMembership;
    bool HasServingCourseHeader(TItemComplete *CurrentItem);
    bool HideServingCourseLabels;
};

//bool match(TItemComplete *PrevItem,TItemComplete *CurrentItem );
//bool match(TItemMinorComplete *PrevItem,TItemMinorComplete *CurrentItem );
#endif
