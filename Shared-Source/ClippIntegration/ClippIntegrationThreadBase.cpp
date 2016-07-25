//---------------------------------------------------------------------------

#pragma hdrstop

#include "ClippIntegrationThreadBase.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationThreadBase::TClippIntegrationThreadBase() : TThread(true)
{
    FreeOnTerminate = true;
    _clippIntegrationInterface = new TClippIntegrationInterface;
}

//---------------------------------------------------------------------------

void __fastcall TClippIntegrationThreadBase::Execute()
{
    StartServiceCommunication();
}

//---------------------------------------------------------------------------
