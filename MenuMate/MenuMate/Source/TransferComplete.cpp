//---------------------------------------------------------------------------


#pragma hdrstop

#include "TransferComplete.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TTransferComplete::TTransferComplete()
{
  TableTransferedFrom = "";
  TableTransferedTo = "";
  UserName = "";
}

__fastcall TTransferComplete::~TTransferComplete()
{
    //
}
