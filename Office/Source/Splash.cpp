//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Filectrl.hpp>

#include "Splash.h"
#include "Version.h"
#include "..\..\Menumate\Version\VersionRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "pngimage"
#pragma resource "*.dfm"
TfrmSplash *frmSplash;
//---------------------------------------------------------------------------
__fastcall TfrmSplash::TfrmSplash(TComponent* Owner)
	: TForm(Owner)
{
    AnsiString ver = STRPRODUCTVER;
    char *m = ver.c_str();
    char *strpos = ver.c_str();
    char *strend = &strpos[ver.Length() - 1];

    for ( ; strpos < strend; strpos++)
        if (*strpos == ',')
            *strpos = '.';

	lbeVersion->Caption = "Version " + ver;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplash::FormCreate(TObject *Sender)
{
	/*ClientWidth = imgSplash->Width;
	ClientHeight = imgSplash->Height;
	Left = (Screen->Width / 2) - (imgSplash->Width/2);
	Top = (Screen->Height / 2) - (imgSplash->Height/2);

	HRGN R, R1, R2;
	R  = CreateRectRgn(0, 0, ClientWidth, ClientHeight);

	TPoint P[5];
	P[0] = Point(5, 6);
	P[1] = Point(488, 6);
	P[2] = Point(416, 171);
	P[3] = Point(388, 193);
	P[4] = Point(5, 193);
	R1 = CreatePolygonRgn(P, 5, ALTERNATE);
	R2 = CreateEllipticRgn(366, 141, 418, 193);
	CombineRgn(R, R1, R2, RGN_OR);

	SetWindowRgn(Handle, R, true);*/
}
//---------------------------------------------------------------------------

