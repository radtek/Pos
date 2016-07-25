//---------------------------------------------------------------------------

#ifndef WebOrderContainerH
#define WebOrderContainerH
//---------------------------------------------------------------------------
#include <stack>
#include "WebOrder.h"

class TWebOrderContainer
{
	public:
	std::stack<TWebOrder> Prev;
   TWebOrder Current;
	std::stack<TWebOrder> Next;
   void clear();
   void first();
	void last();
   bool prev();
   bool next();
   bool empty();
   bool hasnext();
   bool hasprev();
   bool find(UnicodeString GUID);
};
#endif
