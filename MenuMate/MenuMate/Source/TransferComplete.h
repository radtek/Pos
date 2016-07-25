//---------------------------------------------------------------------------

#ifndef TransferCompleteH
#define TransferCompleteH
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TTransferComplete
{
  public:
  AnsiString TableTransferedFrom;
  AnsiString TableTransferedTo;
  AnsiString UserName;
   __fastcall TTransferComplete();
   __fastcall ~TTransferComplete();
};

#endif
