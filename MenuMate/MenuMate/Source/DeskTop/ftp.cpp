//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ftp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmFTP::TfrmFTP(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTP::FormShow(TObject *Sender)
{
	Result = -1;
	FormResize(this);
	FormResize(NULL);
   Memo->Clear();
   SetWindowPos(Handle, HWND_TOPMOST, Left, Top, Width, Height, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTP::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTP::FormPaint(TObject *Sender)
{
   TRect RedBox = TRect(0,0,ClientWidth,ClientHeight);

   Canvas->Brush->Style = bsClear;
   Canvas->Pen->Style = psSolid;
   Canvas->Pen->Width = ClientWidth / 256;
   Canvas->Pen->Color = clRed;//RGB(142,88,66); // RGB(255,205,52)gold;//RGB(142,88,66); Copper
   Canvas->Rectangle(RedBox.left,
   						RedBox.top,
                     RedBox.right,
                     RedBox.bottom);
}
//---------------------------------------------------------------------------
void __fastcall TfrmFTP::lbeOpt1Click(TObject *Sender)
{
	Result = 1;
   Close();	
}
//---------------------------------------------------------------------------
