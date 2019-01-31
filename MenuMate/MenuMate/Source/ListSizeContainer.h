//---------------------------------------------------------------------------

#ifndef ListSizeContainerH
#define ListSizeContainerH

#include <Classes.hpp>
//---------------------------------------------------------------------------
class TItemSize;

class TListSizeContainer : public TList
{
public:
	void SizeClear();
	void SizeDelete(TItemSize *in);
	int SizeAdd(TItemSize *inSize);
	void SizeCopyList(TListSizeContainer *inSizeList);
	TItemSize *SizeGet(int Index);
	TItemSize *SizeGet(UnicodeString Name);
    TItemSize *SizeGetByItemSizeKey(long ItemSizeKey) ;
	TItemSize *SizeGetByPalmID(int PalmID);
    TItemSize *SizeGetByBarcode(const UnicodeString);
	TListSizeContainer();
	__fastcall virtual ~TListSizeContainer();
};

#endif
