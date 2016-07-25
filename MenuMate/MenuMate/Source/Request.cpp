//---------------------------------------------------------------------------


#pragma hdrstop

#include "Request.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------

_fastcall TRequest::TRequest(TDeviceImage *Requester)
{
   Header.Error = proA_Ok;
   Header.AlterDB = false;
   Header.RequestingDevice = new TDeviceImage;
   Header.RequestingDevice->Copy(Requester);
}


_fastcall TRequest::~TRequest()
{
   delete Header.RequestingDevice;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
