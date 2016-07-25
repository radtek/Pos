//---------------------------------------------------------------------------


#pragma hdrstop

#include "WebOrderContainer.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TWebOrderContainer::first()
{
   if(Current.Valid())
   {
   	Next.push(Current);
   }

	while(!Next.empty())
   {
   	Prev.push(Next.top());
      Next.pop();
   }

   if(!Prev.empty())
   {
   	Current = Prev.top();
   	Prev.pop();
   }
}

void TWebOrderContainer::last()
{
   if(Current.Valid())
   {
	   Prev.push(Current);
   }

	while(!Prev.empty())
   {
   	Next.push(Prev.top());
      Prev.pop();
   }

   if(!Next.empty())
   {
      Current = Next.top();
      Next.pop();
   }

}

bool TWebOrderContainer::prev()
{
   if(hasprev())
   {
      Next.push(Current);
      Current = Prev.top();
      Prev.pop();
      return true;
   }
   return false;
}

bool TWebOrderContainer::hasprev()
{
   return !Prev.empty();
}

bool TWebOrderContainer::next()
{
   if(hasnext())
   {
      Prev.push(Current);
      Current = Next.top();
      Next.pop();
      return true;
   }
   return false;
}

bool TWebOrderContainer::hasnext()
{
   return !Next.empty();
}

bool TWebOrderContainer::empty()
{
   return Current.GUID == "";
}

void TWebOrderContainer::clear()
{
	while (!Next.empty())
		Next.pop();

	while (!Prev.empty())
		Prev.pop();

   Current.Clear();
}

bool TWebOrderContainer::find(UnicodeString GUID)
{
   first();
   bool found = false;
   do
   {
		if(Current.GUID == GUID)
      {
         found = true;
      }
   } while(!found && prev());
}
