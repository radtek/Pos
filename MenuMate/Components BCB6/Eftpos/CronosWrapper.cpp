//---------------------------------------------------------------------------
#pragma hdrstop

#include "CronosWrapper.h"
#include <safearry.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ChronosEFTPOS::ChronosEFTPOS() : cronosInterface(new TCronosInterface(0))
{
	cronosInterface->OnEFT2POSMessage = OnMessage;;
}
//---------------------------------------------------------------------------
ChronosEFTPOS::~ChronosEFTPOS()
{
	delete cronosInterface;
}
//---------------------------------------------------------------------------
void __fastcall ChronosEFTPOS::OnMessage(TObject * Sender, OleVariant* psaMsg/*[in]*/)
{
	if (FOnCronosEFTPOSMessage)
	{
		PSafeArray p = PSafeArray(psaMsg->VArray);

		// extract start and end
		long LBound, UBound;
		SafeArrayGetLBound(p,1,&LBound);
		SafeArrayGetUBound(p,1,&UBound);

		// extract the elements
		AnsiString text;
		char datab;

		for (long i=LBound; i<=UBound; ++i)
		{
			SafeArrayGetElement( p, &i, &datab);
			text += datab;
		}

		FOnCronosEFTPOSMessage(this, text);
	}
}
//---------------------------------------------------------------------------
void ChronosEFTPOS::Write(const AnsiString& text)
{
   VARIANT vOut;
   VariantInit(&vOut); // extra call

   SAFEARRAY *pSA;
   SAFEARRAYBOUND aDim[1];    // one dimensional array
   aDim[0].lLbound= 0;
   aDim[0].cElements= text.Length();
   char * pActualData= NULL;
   HRESULT hres = S_OK;


   // Create SafeArray
   pSA= SafeArrayCreate(VT_UI1,1,aDim);

       VariantInit(&vOut); // may not be necessary
       vOut.vt= (VT_ARRAY|VT_UI1); // V_VT(&vOut)= (VT_ARRAY|VT_UI1);
       vOut.parray= pSA; // V_ARRAY(&vOut)= pSA;
       if (hres=SafeArrayAccessData(pSA,(void **)&pActualData)) throw hres;
       for (int i=0; i<text.Length(); i++)
       {
           pActualData[i]= text[i+1];
       }
       if (hres=SafeArrayUnaccessData(pSA)) throw hres;

		cronosInterface->POS2EFTMessage( &pSA );


   // Clean Up
   VariantClear(&vOut); // calls SafeArrayDestroy!
}
//---------------------------------------------------------------------------

