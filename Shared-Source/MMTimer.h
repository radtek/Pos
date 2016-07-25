//---------------------------------------------------------------------------

#ifndef MMTimerH
#define MMTimerH

#include <vcl.h>

//---------------------------------------------------------------------------

class TMMTimer : public TTimer
{
    public:
      __fastcall virtual TMMTimer(Classes::TComponent* AOwner);
      __fastcall virtual ~TMMTimer(void);

    protected:
      HWND FWindowHandle;

      void __fastcall WndMethod( Messages::TMessage& Msg );
      bool processMessage( Messages::TMessage& Msg );
};

#endif
