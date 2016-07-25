//---------------------------------------------------------------------------

#ifndef InitializeDCSessionH
#define InitializeDCSessionH
#include <system.hpp>
#include "DrinkCommandData.h"
#include "DrinkCommandManager.h"
#include <memory>

//---------------------------------------------------------------------------
class TInitializeDCSession
{
    public:
         TInitializeDCSession();
	    ~TInitializeDCSession();
        void StartSession(int memPoints,AnsiString memberPoints,AnsiString memNo,int contactKey);
};
#endif