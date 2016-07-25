//---------------------------------------------------------------------------


#pragma hdrstop

#include "Manager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManager::TManager()
{
	Enabled = false;
}

TManager::~TManager()
{
}

void __fastcall TManager::SetEnabled(bool value)
{
   if (fEnabled != value)
	{
		fEnabled = value;
	}
}

bool __fastcall TManager::GetEnabled()
{
   return fEnabled;
}
