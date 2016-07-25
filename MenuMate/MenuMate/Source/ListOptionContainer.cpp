//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListOptionContainer.h"

#include "MM_DBCore.h"
#include "DeviceRealControl.h"
#include "MMLogging.h"

//#include <IBDatabase.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall OptionCompareItems(void *Item1, void *Item2)
{
   try
   {
       TItemOption* Option1 = (TItemOption*)Item1;
       TItemOption* Option2 = (TItemOption*)Item2;

       if (Option1->GroupNumber > Option2->GroupNumber)
       {
          return 1;
       }
       else if (Option1->GroupNumber == Option2->GroupNumber)
       {
          if (Option1->Name > Option2->Name)
          {
             return 1;
          }
          else if (Option1->Name == Option2->Name)
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
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	}
}

bool TListOptionContainer::operator==(TListOptionContainer &rhs)
{
	unsigned int j = Count;
	bool is_equivalent = j == rhs.Count;

	for (unsigned int i = 0; i < j && is_equivalent; i++)
		is_equivalent &= *OptionGet(i) == *rhs.OptionGet(i);

	return is_equivalent;
}

int OptionsCompare(TListOptionContainer *Options1, TListOptionContainer *Options2)
{
   	try
	{
       // May also have to sort based on master container.
       // Sort Options.
       Options1->OptionSort();

       // Same Object Check
       if(Options1 == Options2)
       		return 0;

       Options2->OptionSort();

       // begin list comparasion.
       if (Options1->Count > Options2->Count)
       {
          return 1;
       }
       else if (Options1->Count == Options2->Count)
       {
          for (int i = 0; i < Options1->Count; i++)
          {
             int RetVal = OptionCompareItems(Options1->OptionGet(i), Options2->OptionGet(i)); // Overloaded Function.
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

// ------------------------------------------------------------------------------

TListOptionContainer::TListOptionContainer()
{
   //OptComp = &OptionCompare;
}

__fastcall TListOptionContainer::~TListOptionContainer()
{
   while (Count > 0)
   {
	  OptionDelete(OptionGet(0));
   }
}

void TListOptionContainer::OptionDelete(TItemOption *in)
{
	try
	{
        if(in != NULL)
        {
            if (IndexOf(in) != -1)
            {
              if (in->Owner == this)
              {
                 Remove(in);
                 delete(TItemOption*)in;
                 in = NULL;
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
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	}
}

int TListOptionContainer::OptionAdd(TItemOption *inOption)
{
   return Add(inOption);
}

void TListOptionContainer::OptionSort()
{
    try
    {
	    Sort(OptionCompareItems);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
    }
}

void TListOptionContainer::OptionCopyList(TListOptionContainer *inOptionList)
{
   OptionClear();
   for (int i = 0; i < inOptionList->Count; i++)
   {
	  TItemOption *NewOption = inOptionList->OptionGet(i);
	  OptionAdd(NewOption);
   }
}

void TListOptionContainer::OptionAddList(TListOptionContainer *inOptionList)
{
   for (int i = 0; i < inOptionList->Count; i++)
   {
	  TItemOption *NewOption = inOptionList->OptionGet(i);
	  OptionAdd(NewOption);
   }
}

// When displaying the options loop though all groups 1 - 15 with each group
// check the options mask against the items postion to determine weather or not it's displayed.
// This function won't work from version 5.11.0.0. Use OptionRetrieveForcedList instead
void TListOptionContainer::OptionRetriveMaskedList(int Group, int ItemPos, TListOptionContainer *inOptionList)
{
   inOptionList->OptionClear();
   unsigned int mask = 1 << ItemPos;
   for (int i = 0; i < Count; i++)
   {
	  TItemOption *NewOption = OptionGet(i);
	  if (NewOption->GroupNumber == Group)
	  {
		 if (NewOption->ForcedMask & mask)
		 {
			inOptionList->OptionAdd(NewOption);
		 }
	  }
   }
}

// This function only work from version 5.18.0.0. Use OptionRetriveMaskedList for previous versions
void TListOptionContainer::OptionRetrieveForcedList( __int32 inGroup, __int32 inItemKey, TListOptionContainer* inForcedOptionList )
{
	Database::TDBTransaction dbTransaction( TDeviceRealControl::ActiveInstance().DBControl );
	dbTransaction.StartTransaction();

	//...........................................

	TIBSQL *qr    = dbTransaction.Query( dbTransaction.AddQuery() );
	qr->SQL->Text = "SELECT op.Options_Key as opKey, op.Flags as opFlags, op.OPTION_NAME as option_name,  op.OPTION_ORDER as option_order "
					"FROM OPTIONS op INNER JOIN FORCEDOPTIONS fop ON op.Options_Key = fop.Option "
					"WHERE fop.Item=:itemKey "
                    "Order by op.OPTION_ORDER asc;";

    qr->ParamByName( "itemKey" )->AsInteger = inItemKey;

	qr->ExecQuery();

	//...........................................

	inForcedOptionList->OptionClear();

	for( ; !qr->Eof; qr->Next() )
	{
		__int32 opKey = qr->FieldByName( "opKey" )->AsInteger;
        __int32 flag =  qr->FieldByName( "opFlags" )->AsInteger;
        UnicodeString option_name = qr->FieldByName( "option_name" )->AsString;
        int order = qr->FieldByName( "option_order" )->AsInteger;

		TItemOption *itemOption = OptionWithGrpNo( inGroup, opKey );

		if( itemOption != NULL )
		{
			inForcedOptionList->OptionAdd( itemOption );
		}
        else
        {
             TItemOption *itemOption = OptionWithflagNo( inGroup, flag , option_name, order);
             if(itemOption != NULL)
             {
                inForcedOptionList->OptionAdd( itemOption );
             }
        }
	}

	//...........................................

	dbTransaction.Commit();
}

TItemOption* TListOptionContainer::OptionWithGrpNo( __int32 inGrpNo, __int32 inOpKey )
{
   TItemOption* result = NULL;

   //::::::::::::::::::::::::::::::::::::::::::

   for( int i = 0; i < Count; i++ )
   {
	  TItemOption *option = OptionGet( i );

	  if( ( option->GroupNumber == inGrpNo ) && ( option->OptionKey == inOpKey ) )
	  {
		 result = option;
		 break;
	  }
   }

   //::::::::::::::::::::::::::::::::::::::::::

   return result;
}

void TListOptionContainer::OptionClear()
{
   while (Count > 0)
   {
	  OptionDelete(OptionGet(0));
   }
}

TItemOption *TListOptionContainer::OptionGet(int Index)
{
   if (Count > Index)
   {
	  return(TItemOption*)Items[Index];
   }
   return NULL;
}

int TListOptionContainer::OptionIndex(TItemOption *Opt)
{
   return IndexOf(Opt);
}

bool TListOptionContainer::Compare(TListOptionContainer *Options2)
{
   if (OptionsCompare(this, Options2) == 0)
   {
	  return true;
   }
   return false;
}
//----------------------------------------------------------------------------
TItemOption* TListOptionContainer::OptionWithflagNo( __int32 inGrpNo, __int32 flag, UnicodeString option_name, int order)
{
   TItemOption* result = NULL;

   //::::::::::::::::::::::::::::::::::::::::::

   for( int i = 0; i < Count; i++ )
   {
	  TItemOption *option = OptionGet( i );

	  if( ( option->GroupNumber == inGrpNo ) && ( option->Flags == flag ) && (option->Name == option_name) && (option->OptionOrder == order) )
	  {
		 result = option;
		 break;
	  }
   }

   //::::::::::::::::::::::::::::::::::::::::::

   return result;
}
