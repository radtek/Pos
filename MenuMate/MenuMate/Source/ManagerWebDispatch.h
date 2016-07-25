//---------------------------------------------------------------------------

#ifndef ManagerWebDispatchH
#define ManagerWebDispatchH

#include <Forms.hpp>

//---------------------------------------------------------------------------
class TManagerWebDispatch
{
   bool Enabled;
   TForm *Owner;
   TWebBrowser *Browser;
   public :
   TManagerWebDispatch(TForm *inOwner,TWebBrowser *inBrowser);
   void Execute(UnicodeString CommandUrl,WordBool &Cancel);
};

#endif
