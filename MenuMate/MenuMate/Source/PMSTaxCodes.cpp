//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PMSTaxCodes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmTaxCodes *frmTaxCodes;
//---------------------------------------------------------------------------
__fastcall TfrmTaxCodes::TfrmTaxCodes(TComponent* Owner,Database::TDBControl &inDBControl)
    : TZForm(Owner), DBControl(inDBControl)
{
    Color = clBlack;
    //DrawGrid1->
}
//---------------------------------------------------------------------------
//void __fastcall TfrmTaxCodes::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
//          TRect &Rect, TGridDrawState State)
//{
//   if(ACol == 0 )
//   {
//           //
//   }
//}
//---------------------------------------------------------------------------
//void __fastcall TfrmTaxCodes::StringGrid1SelectCell(TObject *Sender, int ACol, int ARow,
//          bool &CanSelect)
//{
//  /*sgPay->Canvas->Font->Charset=DEFAULT_CHARSET;
//  sgPay->Canvas->Font->Color = clWindowText;
//  sgPay->Canvas->Font->Name="Tahoma";
//  sgPay->Canvas->Brush->Color = RGB(218,218,218);
//  UnicodeString CellContent = sgPay->Cells[ACol][ARow];
//  if(ARow == sgPay->RowCount-1)
//    {
//        sgPay->Canvas->Font->Size=17;
//        sgPay->Canvas->Font->Style= TFontStyles() << fsBold;
//    }
//  else
//    {
//        sgPay->Canvas->Font->Size=16;
//        sgPay->Canvas->Font->Style= TFontStyles();
//    }
//
//  sgPay->Canvas->FillRect(Rect);
//  if(ACol == 0)
//    {
//       sgPay->Canvas->TextRect(Rect, Rect.Left + 5 , Rect.Top, CellContent);
//    }
//   else
//    {
//         Word SavedAlign = SetTextAlign(sgPay->Canvas->Handle,TA_RIGHT);
//       sgPay->Canvas->TextRect(Rect, Rect.Right - 5, Rect.Top, CellContent);
//       SetTextAlign(sgPay->Canvas->Handle, SavedAlign);
//    }*/
//    ComboBox1->Left =  StringGrid1->Selection.TopLeft.X;
//    ComboBox1->Top =  StringGrid1->Selection.TopLeft.Y;
//
//
//
////  ComboBox1
//}
//---------------------------------------------------------------------------
