//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListRoomContainer.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall RoomsSortNumeric(void *Item1,void *Item2)
{
	TRoomData *Room1 = (TRoomData *)Item1;
	TRoomData *Room2 = (TRoomData *)Item2;
	if (Room1->RoomNo < Room2->RoomNo)
	{
		return -1;
	}
	else if (Room1->RoomNo == Room2->RoomNo)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
//------------------------------------------------------------------------------

int __fastcall RoomsSortRoomName(void *Item1,void *Item2)
{
	TRoomData *Room1 = (TRoomData *)Item1;
	TRoomData *Room2 = (TRoomData *)Item2;
	if (Room1->Name.UpperCase() < Room2->Name.UpperCase())
	{
		return -1;
	}
	else if (Room1->Name.UpperCase() == Room2->Name.UpperCase())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
//------------------------------------------------------------------------------

int __fastcall RoomsSortRoomGuest(void *Item1,void *Item2)
{
	TRoomData *Room1 = (TRoomData *)Item1;
	TRoomData *Room2 = (TRoomData *)Item2;
	if (Room1->PartyName.UpperCase() < Room2->PartyName.UpperCase())
	{
		return -1;
	}
	else if (Room1->PartyName.UpperCase() == Room2->PartyName.UpperCase())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}



TListRoomContainer::TListRoomContainer()
{
}

__fastcall TListRoomContainer::~TListRoomContainer()
{
	Clear();
}

void __fastcall TListRoomContainer::Clear()
{
   while(Count > 0)
   {
      Delete(Get(0));
   }
   TList::Clear();
}

TRoomData *TListRoomContainer::Get(int Index)
{
   if(Count > Index)
   {
      return (TRoomData *)Items[Index];
	}
   return NULL;
}

TRoomData *TListRoomContainer::GetByRoomNo(int inRoomNo)
{
	try
	{
		for(int i = 0; i < Count; i++ )
		{
			TRoomData *ItemRoom = (TRoomData *)Items[i];
			if(ItemRoom->RoomNo == inRoomNo)
			{
				return ItemRoom;
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
	return NULL;
}

TRoomData *TListRoomContainer::GetByRoomKey(int inRoomKey)
{
	try
	{
		for(int i = 0; i < Count; i++ )
		{
			TRoomData *ItemRoom = (TRoomData *)Items[i];
			if(ItemRoom->RoomKey == inRoomKey)
			{
				return ItemRoom;
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
	return NULL;
}


int TListRoomContainer::Add(TRoomData *in)
{
   in->Owner = this;
   return TList::Add(in);
}

void TListRoomContainer::Delete(TRoomData *in)
{
	try
	{
		if(in != NULL)
		{
			if(IndexOf(in) != -1)
			{
				if(in->Owner == this)
				{
					Remove(in);
					delete (TRoomData *)in;
				}
				else
				{
					Remove(in);
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TListRoomContainer::SortByRoomNumber()
{
	try
	{
		Sort(RoomsSortNumeric);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TListRoomContainer::SortByRoomName()
{
	try
	{
		Sort(RoomsSortRoomName);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TListRoomContainer::SortByGuest()
{
	try
	{
		Sort(RoomsSortRoomGuest);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//------------------------------------------------------------------------------
