//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MallExportResendReport.h"
#include "MallExportManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
TfrmMallExportResendReport *frmMallExportResendReport;
//---------------------------------------------------------------------------
__fastcall TfrmMallExportResendReport::TfrmMallExportResendReport(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportResendReport::btnOkMouseClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportResendReport::btnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportResendReport::FormShow(TObject *Sender)
{
    tgReportList->ColCount = 1;
    tgReportList->RowCount = ReportList->Count;

    for(int i=0; i< ReportList->Count;i++)
    {
        tgReportList->Buttons[i][0]->Caption = ReportList->Strings[i];
        tgReportList->Buttons[i][0]->Tag = (int)ReportList->Objects[i];
        tgReportList->Buttons[i][0]->Color = clHotLight;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportResendReport::tgReportListMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    if(GridButton->Color == clHotLight)
    {
        GridButton->Color = clGreen;
        if(((GridButton->Caption).Length() > 2) && ((GridButton->Caption).Length() < 5))
        {
            YearFolder = GridButton->Caption;
            status = true;
        }
        else
        {
            allReports->Add(GridButton->Caption);
            status = false;
        }
    }
    else
    {
        GridButton->Color = clHotLight;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportResendReport::btnResendReportMouseClick(TObject *Sender)

{
    std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());
    MEM->IMallManager->ResendReports(allReports, YearFolder, status);
    if(status)
    {
        ModalResult = mrOk;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportResendReport::btnSelectAllMouseClick(TObject *Sender)

{
    tgReportList->ColCount = 1;
    tgReportList->RowCount = ReportList->Count;
    status = false;

    for(int i=0; i< ReportList->Count;i++)
    {
        tgReportList->Buttons[i][0]->Caption = ReportList->Strings[i];
        allReports->Add(tgReportList->Buttons[i][0]->Caption);
        tgReportList->Buttons[i][0]->Color = clGreen;
    }

}
//---------------------------------------------------------------------------

