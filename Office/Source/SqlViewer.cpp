//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SqlViewer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSqlView *SqlView;
//---------------------------------------------------------------------------
__fastcall TSqlView::TSqlView(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TSqlView::SetSqlText(AnsiString &s) {
    memoSqlText->Lines->Clear();
    memoSqlText->Lines->Append(s);
}
