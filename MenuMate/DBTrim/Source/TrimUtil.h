//---------------------------------------------------------------------------
#ifndef TrimUtilH
#define TrimUtilH

#include <Classes.hpp>
#include "MM_DBCore.h"


#define STATUSCHANGED (WM_APP + 451)
#define STATUSCOMPLETE 1
#define STATUSBEGIN 2
#define STATUSWORK 3
#define STATUSERROR 4
//---------------------------------------------------------------------------
class TDBTrimUtil : public TThread
{
    	Database::TDBControl DBControl;
      TDateTime TrimDate;
   public:
      AnsiString LastError;
      int MaxCount;
      int Position;      
      TDBTrimUtil();
      void Start(TDateTime TrimDate);
      void __fastcall Execute();
 		// void __fastcall SetStatus();
      void Cancel();
};

#endif
 