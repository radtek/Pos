//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Filectrl.hpp>
#include "PayrollExport.h"
#include "MMData.h"
#include "Connections.h"
#include "CSV.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmPayrollExport *frmPayrollExport;
//---------------------------------------------------------------------------
__fastcall TfrmPayrollExport::TfrmPayrollExport(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPayrollExport::BitBtn1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

