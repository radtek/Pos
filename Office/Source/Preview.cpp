//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Preview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma resource "*.dfm"
//TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
__fastcall TfrmPreview::TfrmPreview(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
   	ScaleBy(Screen->Width, Temp);
   }
   Panel2->Left = ClientWidth - Panel2->Width - pnlCourses->Left;
   sbDishes->Left = sbCourses->Width + (pnlCourses->Left * 2);
   sbDishes->Width = Panel2->Left - sbCourses->Width - (pnlCourses->Left * 3);

   Panel1->Width = sbDishes->Left + sbDishes->Width - Panel1->Left;

   Panel2->Height = ClientHeight - Panel2->Top - Panel1->Top;
   sbDishes->Height = ClientHeight - sbDishes->Top - stSelectTable->Height - (Panel1->Top * 2);
   ListBox1->Height = Panel2->ClientHeight - ListBox1->Top - btnUp->Height - (Panel1->Top * 2);
   btnUp->Top = Panel2->ClientHeight - btnUp->Height - Panel1->Top;
   btnDown->Top = btnUp->Top;

   stTender->Top = ClientHeight - stTender->Height - Panel1->Top;
   stSelectTable->Top = stTender->Top;

   pnlCourses->Height = ClientHeight - pnlCourses->Top - stSelectTable->Height - (Panel1->Top * 2);
   sbCourses->Height = pnlCourses->ClientHeight - btnCourseUp->Height - (Panel1->Top * 3);
   btnCourseUp->Top = pnlCourses->ClientHeight - btnCourseUp->Height - Panel1->Top;
   btnCourseDown->Top = btnCourseUp->Top;

   stCash->Top = stTender->Top;
   stSave->Top = stTender->Top;
   stLoyalty->Top = stTender->Top;
   stLoyalty->Left = stCash->Left + stCash->Width + Panel1->Left;;
   stLoyalty->Width = sbDishes->Left + sbDishes->Width - stLoyalty->Left;

   Panel3->Left = Panel1->Width + (Panel1->Left * 2);
   Panel3->Width = ClientWidth - Panel1->Width - (Panel1->Left * 3);
   stDrawer->Width = (Panel3->ClientWidth - (Panel1->Left * 3)) / 2;
   stClose->Width = stDrawer->Width;
   stClose->Left = Panel3->ClientWidth - stClose->Width - Panel1->Left;

   pnlChitNumber->Left = Panel1->ClientWidth - pnlChitNumber->Width - Panel1->Left;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::FormShow(TObject *Sender)
{
   FormResize(Sender);
//   btnCourseUp->Enabled = (sbCourses->VertScrollBar->Position > 0);
//   btnCourseDown->Enabled = (sbCourses->VertScrollBar->Range > sbCourses->ClientHeight);
	if (CurrentCourseBtn->Visible) StaticText1MouseDown(CurrentCourseBtn, mbLeft, TShiftState(), 0, 0);
   if (CurrentCourseBtn->Top + CurrentCourseBtn->Height >= sbCourses->Height)
   {
   	btnCourseDownClick(NULL);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::stCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::StaticText1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Button == mbLeft)
   {
   	CurrentCourseBtn->Color = clMaroon;
		((TTouchButton *)Sender)->Color = clGreen;
   	CurrentCourseBtn = (TTouchButton *)Sender;

      if (CurrentCourseBtn->Top + CurrentCourseBtn->Height >= sbCourses->Height)
      {
         btnCourseDownClick(NULL);
      }
      else if (CurrentCourseBtn->Top < 0)
      {
         btnCourseUpClick(NULL);
      }

   	for (int i=0; i<sbDishes->ControlCount; i++)
   	{
      	if (((String)sbDishes->Controls[i]->ClassName()) == "TTouchButton")
      	{
            if (CurrentCourseBtn->Tag == ((TTouchButton *)sbDishes->Controls[i])->Tag)
            {
               ((TTouchButton *)sbDishes->Controls[i])->Visible = true;
            }
            else
            {
               ((TTouchButton *)sbDishes->Controls[i])->Visible = false;
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   for (int i=0; i<sbDishes->ControlCount;)
   {
      if (((String)sbDishes->Controls[i]->ClassName()) == "TTouchButton")
      {
         delete sbDishes->Controls[i];
      }
      else i++;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::btnCourseDownClick(TObject *Sender)
{
   if (sbCourses->VertScrollBar->Position < (sbCourses->VertScrollBar->Range - sbCourses->ClientHeight))
	{
      int StopAt = (sbCourses->VertScrollBar->Position + sbCourses->Height) - TouchButton1->Height;
      if( StopAt > (sbCourses->VertScrollBar->Range - sbCourses->ClientHeight))
      {
         StopAt = sbCourses->VertScrollBar->Range - sbCourses->ClientHeight;
      }
 
      for (; sbCourses->VertScrollBar->Position < StopAt; sbCourses->VertScrollBar->Position += 3)
      {
         sbCourses->Update();
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::btnCourseUpClick(TObject *Sender)
{
	if (sbCourses->VertScrollBar->Position > 0)
   {
		int StopAt = (sbCourses->VertScrollBar->Position - sbCourses->Height) + TouchButton1->Height;
      if(StopAt < 30) StopAt = 0;
      
      for (int i=sbCourses->VertScrollBar->Position; i>= StopAt; i-=3)
      {
         sbCourses->VertScrollBar->Position = i;
         sbCourses->Update();
      }
   }
}
//---------------------------------------------------------------------------


