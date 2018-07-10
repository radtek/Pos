//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MallExportRegenerateReport.h"
#include "MMTouchKeyboard.h"
#include "ManagerVariable.h"
#include "DeviceRealTerminal.h"
#include "MallExportManager.h"
#include "MallExportDataManager.h"
#include "MallExportInputFileDriver.h"
#include "MallExportOutputFileDriver.h"
#include "MallExportOutputDBDriver.h"
#include "MallExportDefines.h"
#include "Analysis.h"
#include "ManagerVariable.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmMallExportRegenerateReport *frmMallExportRegenerateReport;
//---------------------------------------------------------------------------
__fastcall TfrmMallExportRegenerateReport::TfrmMallExportRegenerateReport(TComponent* Owner)
    : TZForm(Owner)
{
    StartDate = Date();
    EndDate = Date() + 1.0;
    mcStartDate->Date = StartDate;
    mcEndDate->Date = EndDate;
    SetYear = Now().FormatString("yyyy");

    InitializeTimeOptions();
    cbStartHour->ItemIndex = 5;
    cbEndHour->ItemIndex = 5;
    cbStartMin->ItemIndex = 0;
    cbEndMin->ItemIndex = 0;
    ReportPath = "";
    OutputValue = "";

    StartHour = cbStartHour->ItemIndex;
    StartHour = "0" + StartHour;
    EndHour = cbEndHour->ItemIndex;
    EndHour = "0" + EndHour;
    StartMin = cbStartMin->ItemIndex;
    StartMin = "0" + StartMin;
    EndMin = cbEndMin->ItemIndex;
    EndMin = "0" + EndMin;
    InitializeTimeSet(SDate, EDate);

    edLocationPath->Text = "";
    DataQuery = "";

    InputManager = new TMallExportInputFileDriver();
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL ||
       TGlobalSettings::Instance().MallIndex == POWERPLANTMALL ||
       TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        OutputManager = new TMallExportOutputDBDriver();
    } else {
        OutputManager = new TMallExportOutputFileDriver();
    }

    ResetMallExportValues();

    edLocationPath->Enabled = true;
    edLocationPath->Color = clWindow;

    if(TGlobalSettings::Instance().MallIndex == CAPITALAND)
    {
        edLocationPath->Enabled = false;
        edLocationPath->Color = clInactiveCaptionText;
    }
    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        cbStartHour->ItemIndex = 0;
        cbEndHour->ItemIndex = 23;
        cbStartMin->ItemIndex = 0;
        cbEndMin->ItemIndex = 59;

        StartHour = cbStartHour->ItemIndex;
        StartHour = "0" + StartHour;
        EndHour = cbEndHour->ItemIndex;
        EndHour = "0" + EndHour;
        StartMin = cbStartMin->ItemIndex;
        StartMin = "0" + StartMin;
        EndMin = cbEndMin->ItemIndex;
        EndMin = "0" + EndMin;
        InitializeTimeSet(SDate, EDate);

       lbStartMin->Visible = false;
       lbStartHour->Visible = false;
       cbStartHour->Visible = false;
       cbStartMin->Visible = false;
       cbEndHour->Visible = false;
       cbEndMin->Visible = false;
       lbEndHour->Visible = false;
       lbEndMin->Visible = false;


    }

    sbAllTerminals->Visible = false;
    sbThisTerminal->Visible = false;
    ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportRegenerateReport::btnOkMouseClick(TObject *Sender)

{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::btnCancelMouseClick(TObject *Sender)

{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::mcStartDateClick(TObject *Sender)
{
   StartDate = mcStartDate->Date;
   SetYear = StartDate.FormatString("yyyy");
   InitializeTimeSet(SDate, EDate);
   ShowDateTimes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::mcEndDateClick(TObject *Sender)
{
   if(int(mcStartDate->Date) > int(mcEndDate->Date))
   {
        mcEndDate->Date = mcStartDate->Date;
   }
   EndDate = mcEndDate->Date;
   InitializeTimeSet(SDate, EDate);
   ShowDateTimes();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::btnGenerateMouseClick(TObject *Sender)

{
    switch(TGlobalSettings::Instance().MallIndex)
    {
        case SMMALL:
        RegenerateSMExport();
        break;

        case ROBINSONMALL:
        RegenerateRobinsonExport();
        break;

        case AYALAMALL:
        RegenerateAyalaExport();
        break;

        case POWERPLANTMALL:
        RegeneratePowerPlantExport();
        break;

        case CAPITALAND:
        RegenerateCapitalandExport();
        break;

        case ALPHALANDMALL:
        RegenerateAlphalandExport();
        break;

        case MEGAWORLDMALL:
        RegenerateMegaworldExport();
        break;

        case SHANGRILAMALL:
        RegenerateShangrilaExport();
        break;

        case DLFMALL:
        RegenerateDLFMallExport();
        break;

        case FEDERALLANDMALL:
        RegenerateFederalLandExport();
        break;

        default:
        break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::cbStartHourChange(TObject *Sender)

{
    StartHour = cbStartHour->ItemIndex;
    StartHour = FixTime(StartHour);
    InitializeTimeSet(SDate, EDate);
    ShowDateTimes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::cbStartMinChange(TObject *Sender)
{
    StartMin = cbStartMin->ItemIndex;
    InitializeTimeSet(SDate, EDate);
    ShowDateTimes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::cbEndHourChange(TObject *Sender)
{
    EndHour = cbEndHour->ItemIndex;
    InitializeTimeSet(SDate, EDate);
    ShowDateTimes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::cbEndMinChange(TObject *Sender)
{
    EndMin = cbEndMin->ItemIndex;
    InitializeTimeSet(SDate, EDate);
    ShowDateTimes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::edLocationPathMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 300;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edLocationPath->Text;
	frmTouchKeyboard->Caption = "Enter File Location";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edLocationPath->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMallExportRegenerateReport::btnLoadPathMouseClick(TObject *Sender)
{
    edLocationPath->Text = TGlobalSettings::Instance().MallPath;
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::InitializeTimeOptions()
{
    // Initialize StartTime and End Time
    for(int i = 0; i <= 23; i++)
    {
        UnicodeString HourValue = "0";
        if(i < 10)
        {
            HourValue += i;
        }
        else
        {
            HourValue = i;
        }

        cbStartHour->AddItem(HourValue.t_str(),NULL);
        cbEndHour->AddItem(HourValue.t_str(),NULL);
    }

    // Initialize Start Minutes and End Minutes
    for(int i = 0; i <= 59; i++)
    {
        UnicodeString MinutesValue = "0";
        if(i < 10)
        {
            MinutesValue += i;
        }
        else
        {
            MinutesValue = i;
        }

        cbStartMin->AddItem(MinutesValue.t_str(),NULL);
        cbEndMin->AddItem(MinutesValue.t_str(),NULL);
    }
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::InitializeTimeSet(TDateTime &SDate, TDateTime &EDate)
{
   if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
   {
     StartHour ="00";
     StartMin = "01";
     EndHour = "23";
     EndMin = "59";
     UnicodeString StartHM = StartHour + ":" + StartMin + ":00";
     UnicodeString EndHM = EndHour + ":" + EndMin + ":00";
     SDate = StartDate + StrToTime(StartHM);
     EDate = EndDate + StrToTime(EndHM);

   }
   else
   {
    UnicodeString StartHM = StartHour + ":" + StartMin + ":00";
    UnicodeString EndHM = EndHour + ":" + EndMin + ":00";

    SDate = StartDate + StrToTime(StartHM);
    EDate = EndDate + StrToTime(EndHM);

   }
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateSMExport()
{
    UnicodeString MallPath = edLocationPath->Text;
    UnicodeString FileName = MallPath + SDate.FormatString("mmddyyyy") + ".txt";
    UnicodeString Format = ",";
    UnicodeString FlagFormat = "";
    std::fstream OutputFlag;

    DataRead.clear();
    DataToWrite.clear();

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();

    query->SQL->Text = "SELECT FIRST 1 * FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE ORDER BY AM.ENDING_INVOICE DESC";

    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ExecQuery();

    if(!query->Eof)
    {
        MALLCODE = query->FieldByName("MALLCODE")->AsString;
        OutputValue = MALLCODE + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TENANT_NAME = query->FieldByName("TENANT_NAME")->AsString;
        OutputValue = TENANT_NAME + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CLASSCODE = query->FieldByName("CLASSCODE")->AsString;
        OutputValue = CLASSCODE + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRADECODE = query->FieldByName("TRADECODE")->AsString;
        OutputValue = TRADECODE + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OUTLETNUMBER = query->FieldByName("OUTLETNUMBER")->AsString;
        OutputValue = OUTLETNUMBER + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NEW_GRANDTOTAL = query->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(NEW_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OLD_GRANDTOTAL = query->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(OLD_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SALESTYPE = query->FieldByName("SALESTYPE")->AsString;
        OutputValue = SALESTYPE + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DEPARTMENTSUM = query->FieldByName("DEPARTMENTSUM")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DEPARTMENTSUM, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REGDISCOUNT_AMOUNT = query->FieldByName("REGDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(REGDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        EMPDISCOUNT_AMOUNT = query->FieldByName("EMPDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(EMPDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_AMOUNT = query->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(SCDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VIPDISCOUNT_AMOUNT = query->FieldByName("VIPDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VIPDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        PWDDISCOUNT_AMOUNT = query->FieldByName("PWDDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(PWDDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GPCDISCOUNT_AMOUNT = query->FieldByName("GPCDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(GPCDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNTG1_AMOUNT = query->FieldByName("DISCOUNTG1_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DISCOUNTG1_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNTG2_AMOUNT = query->FieldByName("DISCOUNTG2_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DISCOUNTG2_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNTG3_AMOUNT = query->FieldByName("DISCOUNTG3_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DISCOUNTG3_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNTG4_AMOUNT = query->FieldByName("DISCOUNTG4_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DISCOUNTG4_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNTG5_AMOUNT = query->FieldByName("DISCOUNTG5_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DISCOUNTG5_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNTG6_AMOUNT = query->FieldByName("DISCOUNTG6_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DISCOUNTG6_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_SALES = query->FieldByName("VAT_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VAT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHERTAX = query->FieldByName("OTHERTAX")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(OTHERTAX, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        ADJUSTMENTS = query->FieldByName("ADJUSTMENTS")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(ADJUSTMENTS, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        POS_ADJ = query->FieldByName("POS_ADJ")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(POS_ADJ, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NEG_ADJ = query->FieldByName("NEG_ADJ")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(NEG_ADJ, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        POS_ADJ_NONTAX = query->FieldByName("POS_ADJ_NONTAX")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(POS_ADJ_NONTAX, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NEG_ADJ_NONTAX = query->FieldByName("NEG_ADJ_NONTAX")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(NEG_ADJ_NONTAX, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DAILY_SALES = query->FieldByName("DAILY_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DAILY_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_AMOUNT = query->FieldByName("VOID_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VOID_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_AMOUNT = query->FieldByName("REFUND_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(REFUND_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TAX_INCLUSIVE_SALES = query->FieldByName("TAX_INCLUSIVE_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(TAX_INCLUSIVE_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VATEXEMPT_SALES = query->FieldByName("VATEXEMPT_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VATEXEMPT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CHARGECRED_SALES = query->FieldByName("CHARGECRED_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(CHARGECRED_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CHARGECRED_SALES_TAX = query->FieldByName("CHARGECRED_SALES_TAX")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(CHARGECRED_SALES_TAX, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GIFT_CHEQUE_SALES = query->FieldByName("GIFT_CHEQUE_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(GIFT_CHEQUE_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DEBIT_CARD_SALES = query->FieldByName("DEBIT_CARD_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DEBIT_CARD_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_TENDER_SALES = query->FieldByName("OTHER_TENDER_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(OTHER_TENDER_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        MASTERCARD_SALES = query->FieldByName("MASTERCARD_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(MASTERCARD_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VISA_SALES = query->FieldByName("VISA_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VISA_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        AMEX_SALES = query->FieldByName("AMEX_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(AMEX_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DINERS_SALES = query->FieldByName("DINERS_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(DINERS_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        JCB_SALES = query->FieldByName("JCB_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(JCB_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_CARD_SALES = query->FieldByName("OTHER_CARD_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(OTHER_CARD_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCHARGE_AMOUNT = query->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(SCHARGE_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_CHARGES = query->FieldByName("OTHER_CHARGES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(OTHER_CHARGES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        FIRST_TRANSACTION = query->FieldByName("FIRST_TRANSACTION")->AsInteger;
        OutputValue = IntToStr(FIRST_TRANSACTION) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        LAST_TRANSACTION = query->FieldByName("LAST_TRANSACTION")->AsInteger;
        OutputValue = IntToStr(LAST_TRANSACTION) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTION_COUNT = query->FieldByName("TRANSACTION_COUNT")->AsInteger;
        OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        BEGINNING_INVOICE = query->FieldByName("BEGINNING_INVOICE")->AsString;
        OutputValue = BEGINNING_INVOICE + Format;
        DataToWrite.push_back(OutputValue.t_str());

        ENDING_INVOICE = query->FieldByName("ENDING_INVOICE")->AsString;
        OutputValue = ENDING_INVOICE + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CASH_COUNT = query->FieldByName("CASH_COUNT")->AsInteger;
        OutputValue = IntToStr(CASH_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GC_COUNT = query->FieldByName("GC_COUNT")->AsInteger;
        OutputValue = IntToStr(GC_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DEBIT_COUNT = query->FieldByName("DEBIT_COUNT")->AsInteger;
        OutputValue = IntToStr(DEBIT_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_TENDER_COUNT = query->FieldByName("OTHER_TENDER_COUNT")->AsInteger;
        OutputValue = IntToStr(OTHER_TENDER_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        MASTERCARD_COUNT = query->FieldByName("MASTERCARD_COUNT")->AsInteger;
        OutputValue = IntToStr(MASTERCARD_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VISA_COUNT = query->FieldByName("VISA_COUNT")->AsInteger;
        OutputValue = IntToStr(VISA_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        AMEX_COUNT = query->FieldByName("AMEX_COUNT")->AsInteger;
        OutputValue = IntToStr(AMEX_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DINERS_COUNT = query->FieldByName("DINERS_COUNT")->AsInteger;
        OutputValue = IntToStr(DINERS_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        JCB_COUNT = query->FieldByName("JCB_COUNT")->AsInteger;
        OutputValue = IntToStr(JCB_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHERCARD_COUNT = query->FieldByName("OTHERCARD_COUNT")->AsInteger;
        OutputValue = IntToStr(OTHERCARD_COUNT);
        OutputValue = (OutputValue == "0" || OutputValue == "") ? "0000" + Format : OutputValue + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TERMINAL_NAME = query->FieldByName("TERMINAL_NAME")->AsString;
        OutputValue = TERMINAL_NAME + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SERIALNUMBER = query->FieldByName("SERIALNUMBER")->AsString;
        OutputValue = SERIALNUMBER + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CURRENTEODCOUNTER = query->FieldByName("CURRENTEODCOUNTER")->AsInteger;
        OutputValue = IntToStr(CURRENTEODCOUNTER + 1) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTIONTIME = query->FieldByName("TRANSACTIONTIME")->AsDateTime;
        OutputValue = TRANSACTIONTIME.FormatString("hhnnss") + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTIONDATE = query->FieldByName("TRANSACTIONDATE")->AsDateTime;
        OutputValue = TRANSACTIONDATE.FormatString("mmddyyyy");
        DataToWrite.push_back(OutputValue.t_str());
    }

    if(query->RecordCount == 0)
    {
        MessageBox( "Generating Export File for dates the POS was not used is not Allowed!", "Cannot Generate File", MB_OK );
    }
    else
    {
        dataManager->ExportFile(OutputManager, DataToWrite, FileName);
        FlagFormat = MallPath + "Z" + SDate.FormatString("yymmdd") + ".flg";
        OutputFlag.open(FlagFormat.c_str(), std::ios_base::out);
        OutputFlag.close();
        MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
    }
    ResetMallExportValues();
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateRobinsonExport()
{
    int i = 0;
    int RowNum = 0;
    UnicodeString MallPath = edLocationPath->Text;
    UnicodeString FileName = "";
    UnicodeString ItemNo = "";
    UnicodeString Format = "\n";
    UnicodeString Zeroes = "";
    UnicodeString StrCount = "";
    std::ostringstream StreamIntToStr;
    std::string StringIntVal = "";

    UnicodeString TenantID = "";
    UnicodeString TerminalNo = "";
    UnicodeString TenantName = "";
    std::string TenantNameSTR = "";
    UnicodeString FileDate = "";
    UnicodeString Mall = "ROBINSON";
    bool Pastdate = false;

    DataRead.clear();
    DataToWrite.clear();

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();

    query->SQL->Text = "SELECT FIRST 1 * FROM ARCMALLEXPORT AS AM WHERE AM.HOUR_VALUE != ' ' AND AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE "
                       "ORDER BY AM.ENDING_INVOICE DESC";

    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ExecQuery();

    if(!query->Eof)
    {
        TENANT_NAME = query->FieldByName("TENANT_NAME")->AsString;
        DataRead[0] = TENANT_NAME;

        TERMINAL_NAME = query->FieldByName("TERMINAL_NAME")->AsString;
        DataRead[1] = TERMINAL_NAME;

        GROSS_SALES = query->FieldByName("GROSS_SALES")->AsCurrency;
        DataRead[2] = CurrToStrF((RoundToNearest(GROSS_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        VAT_SALES = query->FieldByName("VAT_SALES")->AsCurrency;
        DataRead[3] = CurrToStrF((RoundToNearest(VAT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        VOID_AMOUNT = query->FieldByName("VOID_AMOUNT")->AsCurrency;
        DataRead[4] = CurrToStrF((RoundToNearest(VOID_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        VOID_COUNT = query->FieldByName("VOID_COUNT")->AsInteger;
        DataRead[5] = IntToStr(VOID_COUNT);

        REGDISCOUNT_AMOUNT = query->FieldByName("REGDISCOUNT_AMOUNT")->AsCurrency;
        DataRead[6] = CurrToStrF((RoundToNearest(REGDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        REGDISCOUNT_COUNT = query->FieldByName("REGDISCOUNT_COUNT")->AsInteger;
        DataRead[7] = IntToStr(REGDISCOUNT_COUNT);

        REFUND_AMOUNT = query->FieldByName("REFUND_AMOUNT")->AsCurrency;
        DataRead[8] = CurrToStrF((RoundToNearest(REFUND_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        REFUND_COUNT = query->FieldByName("REFUND_COUNT")->AsInteger;
        DataRead[9] = IntToStr(REFUND_COUNT);

        SCDISCOUNT_AMOUNT = query->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        DataRead[10] = CurrToStrF((RoundToNearest(SCDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        SCDISCOUNT_COUNT = query->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
        DataRead[11] = IntToStr(SCDISCOUNT_COUNT);

        SCHARGE_AMOUNT = query->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
        DataRead[12] = CurrToStrF((RoundToNearest(SCHARGE_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        EODOLD = query->FieldByName("EODOLD")->AsCurrency;
        DataRead[13] = CurrToStr(EODOLD);

        OLD_GRANDTOTAL = query->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
        DataRead[14] = CurrToStrF((RoundToNearest(OLD_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        EODNEW = query->FieldByName("EODNEW")->AsCurrency;
        DataRead[15] = CurrToStr(EODNEW);

        NEW_GRANDTOTAL = query->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
        DataRead[16] = CurrToStrF((RoundToNearest(NEW_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

//        DATE_VALUE = query->FieldByName("DATE_VALUE")->AsDateTime;
        DataRead[17] = SDate.FormatString("mm/dd/yyyy");

        NOVELTY_SALES = query->FieldByName("NOVELTY_SALES")->AsCurrency;
        DataRead[18] = CurrToStrF((RoundToNearest(NOVELTY_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        MISC_SALES = query->FieldByName("MISC_SALES")->AsCurrency;
        DataRead[19] = CurrToStrF((RoundToNearest(MISC_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        LOCALTAXES = query->FieldByName("LOCALTAXES")->AsCurrency;
        DataRead[20] = CurrToStrF((RoundToNearest(LOCALTAXES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        CHARGECRED_SALES = query->FieldByName("CHARGECRED_SALES")->AsCurrency;
        DataRead[21] = CurrToStrF((RoundToNearest(CHARGECRED_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        CHARGECRED_SALES_TAX = query->FieldByName("CHARGECRED_SALES_TAX")->AsCurrency;
        DataRead[22] = CurrToStrF((RoundToNearest(CHARGECRED_SALES_TAX, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        VATEXEMPT_SALES = query->FieldByName("VATEXEMPT_SALES")->AsCurrency;
        DataRead[23] = CurrToStrF((RoundToNearest(VATEXEMPT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        PHARMA_SALES = query->FieldByName("PHARMA_SALES")->AsCurrency;
        DataRead[24] = CurrToStrF((RoundToNearest(PHARMA_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        NONPHARMA_SALES = query->FieldByName("NONPHARMA_SALES")->AsCurrency;
        DataRead[25] = CurrToStrF((RoundToNearest(NONPHARMA_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        PWDDISCOUNT_AMOUNT = query->FieldByName("PWDDISCOUNT_AMOUNT")->AsCurrency;
        DataRead[26] = CurrToStrF((RoundToNearest(PWDDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        SALESNOTSUBTORENT = query->FieldByName("SALESNOTSUBTORENT")->AsCurrency;
        DataRead[27] = CurrToStrF((RoundToNearest(SALESNOTSUBTORENT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        REPTRANSACTION_AMOUNT = query->FieldByName("REPTRANSACTION_AMOUNT")->AsCurrency;
        DataRead[28] = CurrToStrF((RoundToNearest(REPTRANSACTION_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        REPTRANSACTION_COUNT = query->FieldByName("REPTRANSACTION_COUNT")->AsInteger;
        DataRead[29] = IntToStr(REPTRANSACTION_COUNT);

        Pastdate = false;
    }

    if(query->RecordCount == 0)
    {
        GenerateNoExportDays(TERMINAL_NAME, MALLCODE, TRANSACTIONDATE, BEGINNING_INVOICE, ENDING_INVOICE, OLD_GRANDTOTAL, NEW_GRANDTOTAL, BEGINNING_OR, ENDING_OR, Mall, Pastdate);
    }

    for(i=0;i<DataRead.size();i++)
    {
        StrCount = DataRead[i];
        if(i==0)
        {
            TenantName = DataRead[i];
            TenantNameSTR = TenantName.t_str();
            for(int i=0;i<TenantName.Length();i++)
            {
                if(i>=TenantName.Length() - 4)
                {
                    TenantID = TenantID + TenantNameSTR[i];
                }
            }
        }

        if(i==1)
        {
            TerminalNo = DataRead[i];
            if (TerminalNo.Length() == 1)
            {
                TerminalNo = "0" + TerminalNo;
            }
        }

        if(i==17)
        {
            FileDate = SDate.FormatString("mmdd");
        }

        RowNum = i+1;
        StreamIntToStr << RowNum;
        StringIntVal = StreamIntToStr.str();
        StreamIntToStr.str("");
        StreamIntToStr.clear();
        ItemNo = StringIntVal.c_str();
        if(ItemNo.Length() == 1)
        {
            ItemNo = "0" + ItemNo;
        }

        Zeroes = "";
        for(int j=0;j<(16-StrCount.Length());j++)
        {
            Zeroes += "0";
        }

        if (i==29)
        {
            OutputValue = ItemNo + Zeroes + DataRead[i];
        }
        else
        {
            OutputValue = ItemNo + Zeroes + DataRead[i] + Format;
        }
        DataToWrite.push_back(OutputValue.t_str());
        ItemNo = "";
    }

    FileName = MallPath + TenantID + FileDate + "." + TerminalNo + "1" + ".txt";

    if(!Pastdate)
    {
        dataManager->ExportFile(OutputManager, DataToWrite, FileName);
        MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
    }

    ResetMallExportValues();
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateAyalaExport()
{
    UnicodeString MallPath = edLocationPath->Text + SetYear + "\\";
    UnicodeString MallPathFileName = "";
    UnicodeString TenantName = TGlobalSettings::Instance().BranchCode;
    UnicodeString ContractNum = TGlobalSettings::Instance().SerialNo;
    UnicodeString TenantID = "";
    UnicodeString DateFormat = StartDate.FormatString("mmdd");
    UnicodeString FileExt = "";
    UnicodeString Format = "";
    UnicodeString FileInitName = "";
    UnicodeString Mall = "AYALA";
    UnicodeString NewLine = "";
    UnicodeString ZHeader = "";
    UnicodeString ZData = "";
    UnicodeString HHeader = "";
    UnicodeString HData = "";
    std::string TenantNameSTR = TenantName.t_str();
    bool Pastdate = false;

    InitializeTimeSet(SDate, EDate);

    for(int i=0;i<TenantName.Length();i++)
    {
        if(i<3)
        {
            TenantID = TenantID + TenantNameSTR[i];
        }
    }

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    for(int j=0;j<2;j++)
    {
        FileExt = (j==0) ? ".dbf" : ".txt";
        Format = (j==0) ? "                    " : ",";
        FileInitName = (j==0) ? TenantID : ContractNum;
        NewLine = (j==0) ? "" : "\n";
        ZHeader = (j==0) ? "ZHEADER" : "ZTXTHEADER";
        ZData = (j==0) ? "ZDATA" : "ZTXTDATA";
        HHeader = (j==0) ? "HHEADER" : "HTXTHEADER";
        HData = (j==0) ? "HDATA" : "HTXTDATA";


        MallPathFileName = MallPath + FileInitName + DateFormat + FileExt;

        // ayalaExport->CreateHeaderFormatZed(MallPathFileName,DataToWrite);
        // dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZHEADER");
        // DataToWrite.clear();

        TIBSQL *query1 = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *query2 = DBTransaction.Query(DBTransaction.AddQuery());
        query1->Close();

        query1->SQL->Text = "SELECT FIRST 1 A.TERMINAL_NAME, A.MALLCODE, A.TRANSACTIONDATE, A.BEGINNING_INVOICE, A.ENDING_INVOICE, A.OLD_GRANDTOTAL, A.NEW_GRANDTOTAL "
                            "FROM (SELECT AM.TERMINAL_NAME, AM.MALLCODE, AM.TRANSACTIONDATE, (SELECT FIRST 1 AM.BEGINNING_INVOICE FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE) BEGINNING_INVOICE , AM.ENDING_INVOICE, "
                            "(SELECT FIRST 1 AM.OLD_GRANDTOTAL FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE) OLD_GRANDTOTAL, AM.NEW_GRANDTOTAL "
                            "FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE ORDER BY AM.ENDING_INVOICE DESC) A";

        query1->ParamByName("STARTDATE")->AsDateTime = SDate;
        query1->ParamByName("ENDDATE")->AsDateTime = EDate;
        query1->ExecQuery();

        if(query1->RecordCount == 0)
        {
            GenerateNoExportDays(TERMINAL_NAME, MALLCODE, TRANSACTIONDATE, BEGINNING_INVOICE, ENDING_INVOICE, OLD_GRANDTOTAL, NEW_GRANDTOTAL, BEGINNING_OR, ENDING_OR, Mall, Pastdate);
        }

        if(!Pastdate)
        {
        //    ayalaExport->CreateHeaderFormatZed(MallPathFileName,DataToWrite);
            UnicodeString Store = "";
            if(j==0)
            {
                std::string InsertColumn;
                std::string Column[21] = { "TRANDATE", "OLDGT", "NEWGT", "DLYSALE", "TOTDISC",
                                           "TOTREF", "TOTCAN", "VAT", "TENTNAME", "BEGINV",
                                           "ENDINV", "BEGOR", "ENDOR", "TRANCNT", "LOCALTX",
                                           "SERVCHARGE", "NOTAXSALE", "RAWGROSS", "DLYLOCTAX",
                                           "OTHERS", "TERMNUM" };
                for(int i=0;i<21;i++)
                {
                    Store = Column[i].c_str();
                    DataToWrite.push_back(Store.t_str());
                }
            }
            else
            {
                Store = "TRANDATE" + Format + "OLDGT" + Format + "NEWGT" + Format + "DLYSALE" + Format +
                        "TOTDISC" + Format + "TOTREF" + Format + "TOTCAN" + Format + "VAT" + Format +
                        "TENTNAME" + Format + "BEGINV" + Format + "ENDINV" + Format + "BEGOR" + Format +
                        "ENDOR" + Format + "TRANCNT" + Format + "LOCALTX" + Format + "SERVCHARGE" + Format +
                        "NOTAXSALE" + Format + "RAWGROSS" + Format + "DLYLOCTAX" + Format + "OTHERS" + Format + "TERMNUM" + "\n";
                DataToWrite.push_back(Store.t_str());
            }

            dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, ZHeader);
        }
        DataToWrite.clear();

        while(!query1->Eof)
        {
            TERMINAL_NAME = query1->FieldByName("TERMINAL_NAME")->AsString;
            MALLCODE = query1->FieldByName("MALLCODE")->AsString;
            TRANSACTIONDATE = SDate;
            BEGINNING_INVOICE = query1->FieldByName("BEGINNING_INVOICE")->AsString;
            ENDING_INVOICE = query1->FieldByName("ENDING_INVOICE")->AsString;
            OLD_GRANDTOTAL = query1->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
            NEW_GRANDTOTAL = query1->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
            BEGINNING_OR = BEGINNING_INVOICE;
            ENDING_OR = ENDING_INVOICE;
            query1->Next();
        }

        query2->Close();
        query2->SQL->Text = "SELECT SUM(AM.GROSS_SALES) GROSS_SALES, SUM(AM.VAT_SALES) VAT_SALES, SUM(AM.VOID_AMOUNT) VOID_AMOUNT, SUM(AM.REFUND_AMOUNT) REFUND_AMOUNT, "
                            "SUM(AM.SCHARGE_AMOUNT) SCHARGE_AMOUNT, SUM(AM.LOCALTAXES) LOCALTAXES, SUM(AM.VATEXEMPT_SALES) VATEXEMPT_SALES, SUM(AM.DAILY_SALES) DAILY_SALES, "
                            "SUM(AM.TRANSACTION_COUNT) TRANSACTION_COUNT, SUM(AM.TOTALDISCOUNT) TOTALDISCOUNT, SUM(AM.LOCTAXEXEMPTDLY_SALES) LOCTAXEXEMPTDLY_SALES, "
                            "SUM(AM.FINEDINECUST_COUNT) FINEDINECUST_COUNT FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE";

        query2->ParamByName("STARTDATE")->AsDateTime = SDate;
        query2->ParamByName("ENDDATE")->AsDateTime = EDate;
        query2->ExecQuery();

        if(query1->RecordCount != 0)
        {
            while(!query2->Eof)
            {
                OutputValue = (j==0) ? TRANSACTIONDATE.FormatString("yyyymmdd") + Format : TRANSACTIONDATE.FormatString("mm/dd/yyyy") + Format;
                DataToWrite.push_back(OutputValue.t_str());

                OutputValue = ayalaExport->FixDecimalPlaces(OLD_GRANDTOTAL) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                OutputValue = ayalaExport->FixDecimalPlaces(NEW_GRANDTOTAL) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                DAILY_SALES = query2->FieldByName("DAILY_SALES")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(DAILY_SALES) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                TOTALDISCOUNT = query2->FieldByName("TOTALDISCOUNT")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(TOTALDISCOUNT) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                REFUND_AMOUNT = query2->FieldByName("REFUND_AMOUNT")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(REFUND_AMOUNT) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                VOID_AMOUNT = query2->FieldByName("VOID_AMOUNT")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(VOID_AMOUNT) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                VAT_SALES = query2->FieldByName("VAT_SALES")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(VAT_SALES) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                if(j==0)
                {
                    OutputValue = MALLCODE + Format;
                }
                else
                {
//                    UnicodeString MallCodeTemp = MALLCODE;
//                    UnicodeString MallCodeVal = "";
//                    std::string MallCodeSTR = MallCodeTemp.t_str();
//                    for(int i=0;i<MallCodeTemp.Length();i++)
//                    {
//                        if(i>3)
//                        {
//                            MallCodeVal = MallCodeVal + MallCodeSTR[i];
//                        }
//                    }
//                    OutputValue = MallCodeVal + Format;
                    OutputValue = TGlobalSettings::Instance().TenantNo + Format;
                }

                DataToWrite.push_back(OutputValue.t_str());

                OutputValue = BEGINNING_INVOICE + Format;
                DataToWrite.push_back(OutputValue.t_str());

                OutputValue = ENDING_INVOICE + Format;
                DataToWrite.push_back(OutputValue.t_str());

                OutputValue = BEGINNING_OR + Format;
                DataToWrite.push_back(OutputValue.t_str());

                OutputValue = ENDING_OR + Format;
                DataToWrite.push_back(OutputValue.t_str());

                TRANSACTION_COUNT = query2->FieldByName("TRANSACTION_COUNT")->AsInteger;
                OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                LOCALTAXES = query2->FieldByName("LOCALTAXES")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(LOCALTAXES) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                SCHARGE_AMOUNT = query2->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(SCHARGE_AMOUNT) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                VATEXEMPT_SALES = query2->FieldByName("VATEXEMPT_SALES")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(VATEXEMPT_SALES) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                GROSS_SALES = query2->FieldByName("GROSS_SALES")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(GROSS_SALES) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                LOCTAXEXEMPTDLY_SALES = query2->FieldByName("LOCTAXEXEMPTDLY_SALES")->AsCurrency;
                OutputValue = ayalaExport->FixDecimalPlaces(LOCTAXEXEMPTDLY_SALES) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                FINEDINECUST_COUNT = query2->FieldByName("FINEDINECUST_COUNT")->AsInteger;
                OutputValue = IntToStr(FINEDINECUST_COUNT) + Format;
                DataToWrite.push_back(OutputValue.t_str());

                if(j==0)
                {
                    OutputValue = TERMINAL_NAME + Format;
                }
                else
                {
                    OutputValue = TERMINAL_NAME + NewLine;
                }
                DataToWrite.push_back(OutputValue.t_str());
                query2->Next();
            }

            Pastdate = false;
        }
        else
        {
            for(int i=0;i<21;i++)
            {
                OutputValue = DataRead[i];
                DataToWrite.push_back(OutputValue.t_str());
            }
        }

        if(!Pastdate)
        {
            dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, ZData);
            if(j==1)
            {
                MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
            }
        }
        else
        {
            return;
        }

        DataToWrite.clear();

// FOR HOURLY

        UnicodeString Terminal = "";
        UnicodeString Tenant = "";
        UnicodeString TXTTenant = "";
        UnicodeString Date = "";
        UnicodeString DBTime = "";
        UnicodeString Amount = "";
        int TransactionTotal = 0;
        UnicodeString TotAmount = "";

        std::map<UnicodeString, UnicodeString>::iterator it;

        DataRead.clear();
        MallPathFileName = MallPath + FileInitName + DateFormat + "H" + FileExt;

        // ayalaExport->CreateHeaderFormatHourly(MallPathFileName,DataToWrite);
        UnicodeString Store = "";
        if(j==0)
        {
            std::string InsertColumn;
            std::string Column[6] = { "TRANDATE", "HOUR", "SALES", "TRANCNT", "TENTNAME", "TERMNUM"};

            for(int i=0;i<6;i++)
            {
                Store = Column[i].c_str();
                DataToWrite.push_back(Store.t_str());
            }
        }
        else
        {
            std::fstream OutputFile;
            OutputFile.open(MallPathFileName.c_str());
            if(!OutputFile.is_open())
            {
                Store = "TRANDATE" + Format + "HOUR" + Format + "SALES" + Format + "TRANCNT" + Format +
                        "TENTNAME" + Format + "TERMNUM" + "\n";
                DataToWrite.push_back(Store.t_str());
            }
            else
            {
                DataToWrite.clear();
            }
            OutputFile.close();
        }

        dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, HHeader);
        DataToWrite.clear();

        TIBSQL *query3 = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *query4 = DBTransaction.Query(DBTransaction.AddQuery());
        query3->Close();

        query3->SQL->Text = "SELECT * FROM ARCMALLEXPORTHOURLY AS AMH WHERE AMH.DATE_VALUE >= :STARTDATE "
                            "AND AMH.DATE_VALUE < :ENDDATE ORDER BY AMH.DATE_VALUE, AMH.TIME_VALUE ASC";


        query3->ParamByName("STARTDATE")->AsDateTime = SDate;
        query3->ParamByName("ENDDATE")->AsDateTime = EDate;
        query3->ExecQuery();

        while(!query3->Eof)
        {
            TDateTime DBDate;
            Terminal = query3->FieldByName("TERMINAL_NAME")->AsString;
            Tenant = query3->FieldByName("TENANT_NAME")->AsString;
            DBDate = query3->FieldByName("DATE_VALUE")->AsDateTime;
            Date = DBDate.FormatString("yyyymmdd");
            DBTime = query3->FieldByName("TIME_VALUE")->AsString;
            Amount = query3->FieldByName("AMOUNT_VALUE")->AsCurrency;

            Tenant = (j==1) ? TGlobalSettings::Instance().TenantNo : Tenant;

            DataRead[DBTime] = GetHourlyFormat(Terminal, Tenant, Date, DBTime);

            query3->Next();
        }

        for(int i=0;i<24;i++)
        {
            UnicodeString Time = "";
            UnicodeString Data = "";
            switch(i)
            {
                case 0:
                    Time = "06";
                    break;
                case 1:
                    Time = "07";
                    break;
                case 2:
                    Time = "08";
                    break;
                case 3:
                    Time = "09";
                    break;
                case 4:
                    Time = "10";
                    break;
                case 5:
                    Time = "11";
                    break;
                case 6:
                    Time = "12";
                    break;
                case 7:
                    Time = "13";
                    break;
                case 8:
                    Time = "14";
                    break;
                case 9:
                    Time = "15";
                    break;
                case 10:
                    Time = "16";
                    break;
                case 11:
                    Time = "17";
                    break;
                case 12:
                    Time = "18";
                    break;
                case 13:
                    Time = "19";
                    break;
                case 14:
                    Time = "20";
                    break;
                case 15:
                    Time = "21";
                    break;
                case 16:
                    Time = "22";
                    break;
                case 17:
                    Time = "23";
                    break;
                case 18:
                    Time = "00";
                    break;
                case 19:
                    Time = "01";
                    break;
                case 20:
                    Time = "02";
                    break;
                case 21:
                    Time = "03";
                    break;
                case 22:
                    Time = "04";
                    break;
                case 23:
                    Time = "05";
                    break;
                default:
                    break;
            }

            if(query3->RecordCount != 0)
            {
                it = DataRead.find(Time);
                OutputValue = (it->second);
            }
            else
            {
                OutputValue = "";
            }

            if((i>=18) && (i<=23))
            {
                Date = (j==0) ? EDate.FormatString("yyyymmdd") : EDate.FormatString("mm/dd/yyyy");
            }
            else
            {
                Date = (j==0) ? SDate.FormatString("yyyymmdd") : SDate.FormatString("mm/dd/yyyy");
            }

            GetHourlyData(Terminal, Tenant, Date, Time, TransactionTotal, TotAmount);

            if(j==1)
            {
//                UnicodeString MallCodeTemp = Tenant;
//                UnicodeString MallCodeVal = "";
//                std::string MallCodeSTR = MallCodeTemp.t_str();
//                for(int i=0;i<MallCodeTemp.Length();i++)
//                {
//                    if(i>3)
//                    {
//                        MallCodeVal = MallCodeVal + MallCodeSTR[i];
//                    }
//                }
//                TXTTenant = MallCodeVal;
                TXTTenant = TGlobalSettings::Instance().TenantNo;
            }

            for(int i=0;i<6;i++)
            {
                if(i==0)
                {
                    OutputValue = Date + Format;
                }
                else if(i==1)
                {
                    OutputValue = Time + ":00" + Format;
                }
                else if(i==2)
                {
                    if(TransactionTotal != 0)
                    {
                        OutputValue = TotAmount + Format;
                    }
                    else
                    {
                        OutputValue = "0.00" + Format;
                    }
                }
                else if(i==3)
                {
                    if(j==0)
                    {
                        if(TransactionTotal != 0)
                        {
                            OutputValue = TransactionTotal;
                        }
                        else
                        {
                            OutputValue = "0";
                        }
                    }
                    else
                    {
                        if(TransactionTotal != 0)
                        {
                            OutputValue = IntToStr(TransactionTotal) + Format;
                        }
                        else
                        {
                            OutputValue = "0" + Format;
                        }
                    }
                }
                else if(i==4)
                {
                    OutputValue = (j==0) ? Tenant + Format : TXTTenant + Format;
                }
                else
                {
                    OutputValue = (j==0) ? Terminal + Format : Terminal + NewLine;
                }
                DataToWrite.push_back(OutputValue.t_str());
            }
            dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, HData);
            DataToWrite.clear();
        }
        ResetMallExportValues();
    }
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegeneratePowerPlantExport()
{
    UnicodeString MallPath = edLocationPath->Text + StartDate.FormatString("mmddyy") + "01" + ".sal";
    UnicodeString FileName = StartDate.FormatString("mmddyy") + "01" + ".sal";
    UnicodeString RIdentifier = "";
    UnicodeString Format = " ";

    DataRead.clear();
    DataToWrite.clear();

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();

    query->SQL->Text = "SELECT FIRST 1 * FROM ARCMALLEXPORTHOURLY AS AMH WHERE AMH.DATE_VALUE >= :STARTDATE AND AMH.DATE_VALUE < :ENDDATE";

    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ExecQuery();

    if(query->RecordCount == 0)
    {
        MessageBox( "Generating Export File for dates the POS was not used is not Allowed!", "Cannot Generate File", MB_OK );
        return;
    }

    if(!query->Eof)
    {
        TENANT_NAME = query->FieldByName("TENANT_NAME")->AsString;
        MALLCODE = query->FieldByName("MALLCODE")->AsString;
    }

    RIdentifier = "01";

    TIBSQL *query1 = DBTransaction.Query(DBTransaction.AddQuery());

    query1->Close();

    query1->SQL->Text = "SELECT TIME_VALUE, SUM(AMOUNT_VALUE) AMOUNT_VALUE, SUM(TRANSACTION_COUNT) TRANSACTION_COUNT, SUM(VAT_SALES) VAT_SALES, SUM(TOTALDISCOUNT) TOTALDISCOUNT, "
                        "SUM(SCHARGE_AMOUNT) SCHARGE_AMOUNT, SUM(REFUND_COUNT) REFUND_COUNT, SUM(REFUND_AMOUNT) REFUND_AMOUNT, SUM(VOID_COUNT) VOID_COUNT, SUM(VOID_AMOUNT) VOID_AMOUNT, "
                        "SUM(SCDISCOUNT_AMOUNT) SCDISCOUNT_AMOUNT FROM ARCMALLEXPORTHOURLY AS AMH WHERE AMH.DATE_VALUE >= :STARTDATE AND AMH.DATE_VALUE < :ENDDATE "
                        "GROUP BY TIME_VALUE ORDER BY TIME_VALUE ASC ";

    query1->ParamByName("STARTDATE")->AsDateTime = SDate;
    query1->ParamByName("ENDDATE")->AsDateTime = EDate;
    query1->ExecQuery();

    while(!query1->Eof)
    {
        OutputValue = RIdentifier + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = "\"" + MALLCODE + "\"" + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = StartDate.FormatString("mm/dd/yyyy") + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = query1->FieldByName("TIME_VALUE")->AsString + ":00" + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GROSS_SALES = query1->FieldByName("AMOUNT_VALUE")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(GROSS_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_SALES = query1->FieldByName("VAT_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VAT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TOTALDISCOUNT = query1->FieldByName("TOTALDISCOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(TOTALDISCOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCHARGE_AMOUNT = query1->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(SCHARGE_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTION_COUNT = query1->FieldByName("TRANSACTION_COUNT")->AsInteger;
        OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = "\"" + TENANT_NAME + "\"" + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_COUNT = query1->FieldByName("REFUND_COUNT")->AsInteger;
        OutputValue = IntToStr(REFUND_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_AMOUNT = query1->FieldByName("REFUND_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(REFUND_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_COUNT = query1->FieldByName("VOID_COUNT")->AsInteger;
        OutputValue = IntToStr(VOID_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_AMOUNT = query1->FieldByName("VOID_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VOID_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_AMOUNT = query1->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(SCDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + "\n";
        DataToWrite.push_back(OutputValue.t_str());

        dataManager->ExportFile(OutputManager, DataToWrite, MallPath);
        DataToWrite.clear();
        query1->Next();
    }

    RIdentifier = "99";

    TIBSQL *query2 = DBTransaction.Query(DBTransaction.AddQuery());

    query2->Close();

    query2->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AMOUNT_VALUE, SUM(TRANSACTION_COUNT) TRANSACTION_COUNT, SUM(VAT_SALES) VAT_SALES, SUM(TOTALDISCOUNT) TOTALDISCOUNT, "
                        "SUM(SCHARGE_AMOUNT) SCHARGE_AMOUNT, SUM(REFUND_COUNT) REFUND_COUNT, SUM(REFUND_AMOUNT) REFUND_AMOUNT, SUM(VOID_COUNT) VOID_COUNT, SUM(VOID_AMOUNT) VOID_AMOUNT, "
                        "SUM(SCDISCOUNT_AMOUNT) SCDISCOUNT_AMOUNT FROM ARCMALLEXPORTHOURLY AS AMH WHERE AMH.DATE_VALUE >= :STARTDATE AND AMH.DATE_VALUE < :ENDDATE ";

    query2->ParamByName("STARTDATE")->AsDateTime = SDate;
    query2->ParamByName("ENDDATE")->AsDateTime = EDate;
    query2->ExecQuery();

    if(!query2->Eof)
    {
        OutputValue = RIdentifier + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = "\"" + MALLCODE + "\"" + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = StartDate.FormatString("mm/dd/yyyy") + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GROSS_SALES = query2->FieldByName("AMOUNT_VALUE")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(GROSS_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_SALES = query2->FieldByName("VAT_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VAT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TOTALDISCOUNT = query2->FieldByName("TOTALDISCOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(TOTALDISCOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCHARGE_AMOUNT = query2->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(SCHARGE_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTION_COUNT = query2->FieldByName("TRANSACTION_COUNT")->AsInteger;
        OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = "\"" + TENANT_NAME + "\"" + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_COUNT = query2->FieldByName("REFUND_COUNT")->AsInteger;
        OutputValue = IntToStr(REFUND_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_AMOUNT = query2->FieldByName("REFUND_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(REFUND_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_COUNT = query2->FieldByName("VOID_COUNT")->AsInteger;
        OutputValue = IntToStr(VOID_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_AMOUNT = query2->FieldByName("VOID_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(VOID_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_AMOUNT = query2->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(SCDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + "\n";
        DataToWrite.push_back(OutputValue.t_str());

        dataManager->ExportFile(OutputManager, DataToWrite, MallPath);
        DataToWrite.clear();
    }

    RIdentifier = "95";

    TIBSQL *query3 = DBTransaction.Query(DBTransaction.AddQuery());
    query3->Close();

    query3->SQL->Text = "SELECT FIRST 1 (SELECT FIRST 1 AM.OLD_GRANDTOTAL FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE) OLD_GRANDTOTAL, AM.NEW_GRANDTOTAL "
                        "FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE ORDER BY AM.TRANSACTIONDATE DESC";

    query3->ParamByName("STARTDATE")->AsDateTime = SDate;
    query3->ParamByName("ENDDATE")->AsDateTime = EDate;
    query3->ExecQuery();

    if(!query3->Eof)
    {
        OutputValue = RIdentifier + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = "\"" + MALLCODE + "\"" + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = StartDate.FormatString("mm/dd/yyyy") + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OLD_GRANDTOTAL = query3->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(OLD_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NEW_GRANDTOTAL = query3->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(NEW_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        dataManager->ExportFile(OutputManager, DataToWrite, MallPath);
        DataToWrite.clear();
    }

    MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );

    DataRead.clear();
    ResetMallExportValues();
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateCapitalandExport()
{
    UnicodeString HostName = TGlobalSettings::Instance().FTPServer;
    UnicodeString UserPath = TGlobalSettings::Instance().FTPPath;
    UnicodeString UserName = TGlobalSettings::Instance().FTPUserName;
    UnicodeString UserPassword = TGlobalSettings::Instance().FTPPassword;
    UnicodeString BasePath = ExtractFilePath(Application->ExeName) + MALLEXPORTFOLDERPATH;
    UnicodeString Zeroes = "";
    UnicodeString DocType = "D";
    UnicodeString LocalPath = BasePath + "\\Capitaland\\";
    UnicodeString LocalPathFileName = "";
    UnicodeString LocalFileName = "";
    int FileSerialNumber = 0;
    int FCount = 0;

    if(TGlobalSettings::Instance().FTPPath == "")
    {
        UserPath = "/";
    }

    const char * hostName = HostName.t_str();
    const char * userPath = UserPath.t_str();
    const char * userName = UserName.t_str();
    const char * userPassword = UserPassword.t_str();

    DataRead.clear();
    DataToWrite.clear();

    OutputValue = DocType;
    DataToWrite.push_back(OutputValue.t_str());

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TIBSQL *query0 = DBTransaction.Query(DBTransaction.AddQuery());

    query0->Close();

    query0->SQL->Text = "SELECT COUNT(A.ARCMALLEXPORT_KEY) AS ZCOUNT FROM (SELECT * FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE <= :ENDDATE) AS A";

    query0->ParamByName("ENDDATE")->AsDateTime = EDate;
    query0->ExecQuery();

    if(!query0->Eof)
    {
        FileSerialNumber = query0->FieldByName("ZCOUNT")->AsInteger;
        if(FileSerialNumber != 0)
        {
            FileSerialNumber = FileSerialNumber - 1;

            if(FileSerialNumber >= 999)
            {
                for(int i=999; i<=FileSerialNumber; i+0)
                {
                    FileSerialNumber = FileSerialNumber - i;
                }
            }
        }
    }

    TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();

    query->SQL->Text = "SELECT FIRST 1 * FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE ORDER BY AM.ENDING_INVOICE DESC";

    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ExecQuery();

    if(query->RecordCount == 0)
    {
        MessageBox( "Generating Export File for dates the POS was not used is not Allowed!", "Cannot Generate File", MB_OK );
        return;
    }

    if(!query->Eof)
    {
        MALLCODE = query->FieldByName("MALLCODE")->AsString;
        TRANSACTIONDATE = SDate;
        GROSS_SALES = query->FieldByName("GROSS_SALES")->AsCurrency;
        OutputValue = CurrToStrF((RoundToNearest(GROSS_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);

        capitalandExport->CreateFileName(LocalPath, LocalPathFileName, LocalFileName, DocType, MALLCODE, FileSerialNumber);

        Zeroes = "";
        for(int j=0;j<(11-OutputValue.Length());j++)
        {
            Zeroes += "0";
        }
        OutputValue = Zeroes + OutputValue;

        OutputValue = MALLCODE + TRANSACTIONDATE.FormatString("yyyymmdd") + OutputValue;
        DataToWrite.push_back(OutputValue.t_str());
    }

    dataManager->ExportFile(OutputManager, DataToWrite, LocalPathFileName);
    capitalandExport->FileSubmit(hostName, userName, userPassword, userPath, LocalPathFileName, LocalFileName, FCount);

    MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );

    ResetMallExportValues();
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateAlphalandExport()
{
    UnicodeString MallPath = edLocationPath->Text;
    UnicodeString AlphaCode = "TMS";
    UnicodeString FileName = MallPath + AlphaCode;
    UnicodeString Format = "\t";

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();

    query->SQL->Text = "SELECT * FROM ARCMALLEXPORTTRANSACTION AS AMT WHERE AMT.CURRENT_DATE_VALUE >= :STARTDATE AND AMT.CURRENT_DATE_VALUE < :ENDDATE "
                       "ORDER BY AMT.CURRENT_DATE_VALUE ASC";

    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ExecQuery();

    if(query->RecordCount == 0)
    {
        MessageBox( "Generating Export File for dates the POS was not used is not Allowed!", "Cannot Generate File", MB_OK );
        return;
    }

    while(!query->Eof)
    {
        UnicodeString Name = "";
        std::ostringstream StreamIntToStr;
        std::string StringIntVal = "";
        UnicodeString ItemNo = "";

        OutputValue = query->FieldByName("DATE_VALUE")->AsString + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TENANT_NAME = query->FieldByName("TENANT_NAME")->AsString;
        OutputValue = TENANT_NAME + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CURRENT_DATE_VALUE = query->FieldByName("CURRENT_DATE_VALUE")->AsDateTime;
        OutputValue = CURRENT_DATE_VALUE.FormatString("yyyy-mm-dd") + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = query->FieldByName("TIME_VALUE")->AsString + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TERMINAL_NAME = query->FieldByName("TERMINAL_NAME")->AsString;
        StreamIntToStr << StrToInt(TERMINAL_NAME);
        StringIntVal = StreamIntToStr.str();
        ItemNo = StringIntVal.c_str();
        TERMINAL_NAME = ItemNo;
        OutputValue = AddZeroesToValue(3,StrToInt(TERMINAL_NAME)) + TERMINAL_NAME + Format;
        TERMINAL_NAME = AddZeroesToValue(3,StrToInt(TERMINAL_NAME)) + TERMINAL_NAME;
        DataToWrite.push_back(OutputValue.t_str());

        INVOICE_NUMBER = query->FieldByName("INVOICE_NUMBER")->AsInteger;
        OutputValue = AddZeroesToValue(8,StrToInt(INVOICE_NUMBER)) + INVOICE_NUMBER + Format;
        INVOICE_NUMBER = AddZeroesToValue(8,StrToInt(INVOICE_NUMBER)) + INVOICE_NUMBER;
        DataToWrite.push_back(OutputValue.t_str());

        GROSS_SALES = query->FieldByName("GROSS_SALES")->AsCurrency;
        OutputValue = CheckValue(GROSS_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_SALES = query->FieldByName("VAT_SALES")->AsCurrency;
        OutputValue = CheckValue(VAT_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NONVAT_SALES = query->FieldByName("NONVAT_SALES")->AsCurrency;
        OutputValue = CheckValue(NONVAT_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VATEXEMPT_SALES = query->FieldByName("VATEXEMPT_SALES")->AsCurrency;
        OutputValue = CheckValue(VATEXEMPT_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_AMOUNT = query->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(SCDISCOUNT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        EMPDISCOUNT_AMOUNT = query->FieldByName("EMPDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(EMPDISCOUNT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        PRODISCOUNT_AMOUNT = query->FieldByName("PRODISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(PRODISCOUNT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHERDISCOUNT_AMOUNT = query->FieldByName("OTHERDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(OTHERDISCOUNT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_AMOUNT = query->FieldByName("REFUND_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(REFUND_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCHARGE_AMOUNT = StrToCurr(query->FieldByName("SCHARGE_AMOUNT")->AsString);
        OutputValue = CheckValue(SCHARGE_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_SURCHARGE = query->FieldByName("OTHER_SURCHARGE")->AsCurrency;
        OutputValue = CheckValue(OTHER_SURCHARGE) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NET_SALES = query->FieldByName("NET_SALES")->AsCurrency;
        OutputValue = CheckValue(NET_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OLD_GRANDTOTAL = query->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
        OutputValue = CheckValue(OLD_GRANDTOTAL) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NEW_GRANDTOTAL = query->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
        OutputValue = CheckValue(NEW_GRANDTOTAL) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        ZCOUNT = query->FieldByName("ZCOUNT")->AsInteger;
        OutputValue = IntToStr(ZCOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CASH_SALES = query->FieldByName("CASH_SALES_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(CASH_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CARD_SALES = query->FieldByName("CARD_SALES_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(CARD_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CHECK_SALES = query->FieldByName("CHECK_SALES_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(CHECK_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GC_SALES_AMOUNT = query->FieldByName("GC_SALES_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(GC_SALES_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        EPAY_SALES = query->FieldByName("EPAY_SALES_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(EPAY_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_SALES = query->FieldByName("OTHER_SALES_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(OTHER_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_AMOUNT = query->FieldByName("VOID_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(VOID_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DEPOSIT_AMOUNT = query->FieldByName("DEPOSIT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(DEPOSIT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        APPLIED_DEPOSIT_AMOUNT = query->FieldByName("APPLIED_DEPOSIT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(APPLIED_DEPOSIT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_SALES_COUNT = query->FieldByName("VAT_SALES_COUNT")->AsInteger;
        OutputValue = IntToStr(VAT_SALES_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NONVAT_SALES_COUNT = query->FieldByName("NONVAT_SALES_COUNT")->AsInteger;
        OutputValue = IntToStr(NONVAT_SALES_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VATEXEMPT_SALES_COUNT = query->FieldByName("VATEXEMPT_SALES_COUNT")->AsInteger;
        OutputValue = IntToStr(VATEXEMPT_SALES_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_COUNT = query->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
        OutputValue = IntToStr(SCDISCOUNT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        EMPDISCOUNT_COUNT = query->FieldByName("EMPDISCOUNT_COUNT")->AsInteger;
        OutputValue = IntToStr(EMPDISCOUNT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        PRODISCOUNT_COUNT = query->FieldByName("PRODISCOUNT_COUNT")->AsInteger;
        OutputValue = IntToStr(PRODISCOUNT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHERDISCOUNT_COUNT = query->FieldByName("OTHERDISCOUNT_COUNT")->AsInteger;
        OutputValue = IntToStr(OTHERDISCOUNT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_COUNT = query->FieldByName("REFUND_COUNT")->AsInteger;
        OutputValue = IntToStr(REFUND_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCHARGE_COUNT = query->FieldByName("SCHARGE_COUNT")->AsInteger;
        OutputValue = IntToStr(SCHARGE_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_SURCHARGE_COUNT = query->FieldByName("OTHER_SURCHARGE_COUNT")->AsInteger;
        OutputValue = IntToStr(OTHER_SURCHARGE_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CASH_COUNT = query->FieldByName("CASH_COUNT")->AsInteger;
        OutputValue = IntToStr(CASH_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CARD_COUNT = query->FieldByName("CARD_COUNT")->AsInteger;
        OutputValue = IntToStr(CARD_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CHECK_COUNT = query->FieldByName("CHECK_COUNT")->AsInteger;
        OutputValue = IntToStr(CHECK_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GC_COUNT = query->FieldByName("GC_COUNT")->AsInteger;
        OutputValue = IntToStr(GC_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        EPAY_COUNT = query->FieldByName("EPAY_COUNT")->AsInteger;
        OutputValue = IntToStr(EPAY_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_COUNT = query->FieldByName("OTHER_COUNT")->AsInteger;
        OutputValue = IntToStr(OTHER_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_COUNT = query->FieldByName("VOID_COUNT")->AsInteger;
        OutputValue = IntToStr(VOID_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANS_VOID_COUNT = query->FieldByName("TRANS_VOID_COUNT")->AsInteger;
        OutputValue = IntToStr(TRANS_VOID_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DEPOSIT_COUNT = query->FieldByName("DEPOSIT_COUNT")->AsInteger;
        OutputValue = IntToStr(DEPOSIT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        APPLIED_DEPOSIT_COUNT = query->FieldByName("APPLIED_DEPOSIT_COUNT")->AsInteger;
        OutputValue = IntToStr(APPLIED_DEPOSIT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        PATRON_COUNT = query->FieldByName("PATRON_COUNT")->AsInteger;
        OutputValue = IntToStr(PATRON_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTION_COUNT = query->FieldByName("TRANSACTION_COUNT")->AsInteger;
        OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NOSALE_COUNT = query->FieldByName("NOSALE_COUNT")->AsInteger;
        OutputValue = IntToStr(NOSALE_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SALES_FLAG = query->FieldByName("SALES_FLAG")->AsString;
        OutputValue = SALES_FLAG + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_PERCENTAGE = query->FieldByName("VAT_PERCENTAGE")->AsCurrency;
        OutputValue = CheckValue(VAT_PERCENTAGE);
        DataToWrite.push_back(OutputValue.t_str());

        Name = FileName + StartDate.FormatString("yyddmm") + INVOICE_NUMBER + "." + TERMINAL_NAME;
        dataManager->ExportFile(OutputManager, DataToWrite, Name);

        TERMINAL_NAME = "";
        DataToWrite.clear();
        query->Next();
    }

    TIBSQL *query1 = DBTransaction.Query(DBTransaction.AddQuery());

    query1->Close();

    query1->SQL->Text = "SELECT FIRST 1 A.TERMINAL_NAME, A.TENANT_NAME, A.TRANSACTIONDATE, A.BEGINNING_INVOICE, A.ENDING_INVOICE, A.OLD_GRANDTOTAL, A.NEW_GRANDTOTAL , A.EODOLD, A.EODNEW "
                        "FROM (SELECT AM.TERMINAL_NAME, AM.TENANT_NAME, AM.TRANSACTIONDATE, (SELECT FIRST 1 AM.BEGINNING_INVOICE FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE) BEGINNING_INVOICE , AM.ENDING_INVOICE, "
                        "(SELECT FIRST 1 AM.OLD_GRANDTOTAL FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE) OLD_GRANDTOTAL, AM.NEW_GRANDTOTAL, "
                        "(SELECT FIRST 1 AM.EODOLD FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE) EODOLD, AM.EODNEW "
                        "FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE ORDER BY AM.ENDING_INVOICE DESC) A";

    query1->ParamByName("STARTDATE")->AsDateTime = SDate;
    query1->ParamByName("ENDDATE")->AsDateTime = EDate;
    query1->ExecQuery();

    if(!query1->Eof)
    {
        TERMINAL_NAME = query1->FieldByName("TERMINAL_NAME")->AsString;
        TENANT_NAME = query1->FieldByName("TENANT_NAME")->AsString;
        DATE_VALUE = SDate;
        BEGINNING_INVOICE = query1->FieldByName("BEGINNING_INVOICE")->AsString;
        ENDING_INVOICE = query1->FieldByName("ENDING_INVOICE")->AsString;
        OLD_GRANDTOTAL = query1->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
        NEW_GRANDTOTAL = query1->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
        EODOLD = query1->FieldByName("EODOLD")->AsInteger;
        EODNEW = query1->FieldByName("EODNEW")->AsInteger;
        query1->Next();
    }

    TIBSQL *query2 = DBTransaction.Query(DBTransaction.AddQuery());
    query2->Close();
    query2->SQL->Text = "SELECT SUM(AM.GROSS_SALES) GROSS_SALES, SUM(AM.VAT_SALES) VAT_SALES, SUM(AM.VATEXEMPT_SALES) VATEXEMPT_SALES, SUM(AM.NONVAT_SALES) NONVAT_SALES, "
                        "SUM(AM.VOID_AMOUNT) VOID_AMOUNT, SUM(AM.VOID_COUNT) VOID_COUNT, SUM(AM.TOTALDISCOUNT) TOTALDISCOUNT, SUM(AM.DISCOUNT_COUNT) DISCOUNT_COUNT, SUM(AM.REFUND_AMOUNT) REFUND_AMOUNT, "
                        "SUM(AM.REFUND_COUNT) REFUND_COUNT, SUM(AM.SCDISCOUNT_AMOUNT) SCDISCOUNT_AMOUNT, SUM(AM.SCDISCOUNT_COUNT) SCDISCOUNT_COUNT, SUM(AM.SCHARGE_AMOUNT) SCHARGE_AMOUNT, "
                        "SUM(AM.CARD_SALES) CARD_SALES, SUM(AM.CASH_SALES) CASH_SALES, SUM(AM.CHECK_SALES) CHECK_SALES, SUM(AM.GIFT_CHEQUE_SALES) GIFT_CHEQUE_SALES, SUM(AM.EPAY_SALES) EPAY_SALES, SUM(AM.OTHER_SALES) OTHER_SALES, "
                        "SUM(AM.NO_SALES) NO_SALES, SUM(AM.TRANSACTION_COUNT) TRANSACTION_COUNT FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE";


    query2->ParamByName("STARTDATE")->AsDateTime = SDate;
    query2->ParamByName("ENDDATE")->AsDateTime = EDate;
    query2->ExecQuery();

    if(!query2->Eof)
    {
        UnicodeString Name = "";
        std::ostringstream StreamIntToStr;
        std::string StringIntVal = "";
        UnicodeString ItemNo = "";

        OutputValue = TENANT_NAME + Format;
        DataToWrite.push_back(OutputValue.t_str());

        StreamIntToStr << StrToInt(TERMINAL_NAME);
        StringIntVal = StreamIntToStr.str();
        ItemNo = StringIntVal.c_str();
        TERMINAL_NAME = ItemNo;
        OutputValue = AddZeroesToValue(3,StrToInt(TERMINAL_NAME)) + TERMINAL_NAME + Format;
        TERMINAL_NAME = AddZeroesToValue(3,StrToInt(TERMINAL_NAME)) + TERMINAL_NAME;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = DATE_VALUE.FormatString("yyyy-mm-dd") + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = AddZeroesToValue(8,StrToInt(BEGINNING_INVOICE)) + BEGINNING_INVOICE + Format;
        BEGINNING_INVOICE = AddZeroesToValue(8,StrToInt(BEGINNING_INVOICE)) + BEGINNING_INVOICE;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = AddZeroesToValue(8,StrToInt(ENDING_INVOICE)) + ENDING_INVOICE + Format;
        ENDING_INVOICE = AddZeroesToValue(8,StrToInt(ENDING_INVOICE)) + ENDING_INVOICE;
        DataToWrite.push_back(OutputValue.t_str());

        GROSS_SALES = query2->FieldByName("GROSS_SALES")->AsCurrency;
        OutputValue = CheckValue(GROSS_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VAT_SALES = query2->FieldByName("VAT_SALES")->AsCurrency;
        OutputValue = CheckValue(VAT_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NONVAT_SALES = query2->FieldByName("NONVAT_SALES")->AsCurrency;
        OutputValue = CheckValue(NONVAT_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VATEXEMPT_SALES = query2->FieldByName("VATEXEMPT_SALES")->AsCurrency;
        OutputValue = CheckValue(VATEXEMPT_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_AMOUNT = query2->FieldByName("VOID_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(VOID_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        VOID_COUNT = query2->FieldByName("VOID_COUNT")->AsInteger;
        OutputValue = IntToStr(VOID_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TOTALDISCOUNT = query2->FieldByName("TOTALDISCOUNT")->AsCurrency;
        OutputValue = CheckValue(TOTALDISCOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        DISCOUNT_COUNT = query2->FieldByName("DISCOUNT_COUNT")->AsInteger;
        OutputValue = IntToStr(DISCOUNT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_AMOUNT = query2->FieldByName("REFUND_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(REFUND_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        REFUND_COUNT = query2->FieldByName("REFUND_COUNT")->AsInteger;
        OutputValue = IntToStr(REFUND_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_AMOUNT = query2->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        OutputValue = CheckValue(SCDISCOUNT_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCDISCOUNT_COUNT = query2->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
        OutputValue = IntToStr(SCDISCOUNT_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        SCHARGE_AMOUNT = StrToCurr(query2->FieldByName("SCHARGE_AMOUNT")->AsString);
        OutputValue = CheckValue(SCHARGE_AMOUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CASH_SALES = query2->FieldByName("CASH_SALES")->AsCurrency;
        OutputValue = CheckValue(CASH_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CARD_SALES = query2->FieldByName("CARD_SALES")->AsCurrency;
        OutputValue = CheckValue(CARD_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        CHECK_SALES = query2->FieldByName("CHECK_SALES")->AsCurrency;
        OutputValue = CheckValue(CHECK_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        GIFT_CHEQUE_SALES = query2->FieldByName("GIFT_CHEQUE_SALES")->AsCurrency;
        OutputValue = CheckValue(GIFT_CHEQUE_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        EPAY_SALES = query2->FieldByName("EPAY_SALES")->AsCurrency;
        OutputValue = CheckValue(EPAY_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OTHER_SALES = query2->FieldByName("OTHER_SALES")->AsCurrency;
        OutputValue = CheckValue(OTHER_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        NO_SALES = query2->FieldByName("NO_SALES")->AsCurrency;
        OutputValue = CheckValue(NO_SALES) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = IntToStr(EODOLD) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = CheckValue(OLD_GRANDTOTAL) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = IntToStr(EODNEW) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        OutputValue = CheckValue(NEW_GRANDTOTAL) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        TRANSACTION_COUNT = query2->FieldByName("TRANSACTION_COUNT")->AsInteger;
        OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
        DataToWrite.push_back(OutputValue.t_str());

        Name = FileName + StartDate.FormatString("yyddmm") + TERMINAL_NAME + "." + "EOD";
        dataManager->ExportFile(OutputManager, DataToWrite, Name);

        TERMINAL_NAME = "";
        DataToWrite.clear();
    }

    MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );

    ResetMallExportValues();
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateShangrilaExport()
{
    UnicodeString Format = " ";
    UnicodeString OutputValue = "";

    UnicodeString FileName = edLocationPath->Text + StartDate.FormatString("mmddyyyy") + ".txt";
    UnicodeString MallCode = TGlobalSettings::Instance().TenantNo;
    UnicodeString SalesDept = TGlobalSettings::Instance().BranchCode;
    UnicodeString TenantName = "";
    UnicodeString DateValue = "";
    UnicodeString TimeValue = "";
    UnicodeString AmountValue = 0;
    UnicodeString TaxValue = 0;
    UnicodeString AdjValue = 0;
    UnicodeString SChargeValue = 0;
    UnicodeString RIdentifier = "01";
    UnicodeString Amount = "";

    int TransactionTotal = 0;
    int val = 0;

    std::string TimeCount[12];
    bool flag = false;

    unsigned short Hour = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

    DataToWrite.clear();

    std::map<UnicodeString, UnicodeString>::iterator it;

    // ####################
    // Hourly Export File #
    // ####################

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL* query0 = DBTransaction.Query(DBTransaction.AddQuery());

    query->SQL->Text = "SELECT MALLCODE, DATE_VALUE, TIME_VALUE, SUM(AMOUNT_VALUE) AMOUNT_VALUE, "
                       "SUM(VAT_SALES) VAT_SALES, SUM(TOTALDISCOUNT) TOTALDISCOUNT, "
                       "SUM(SCHARGE_AMOUNT) SCHARGE_AMOUNT, SUM(TRANSACTION_COUNT) TRANSACTION_COUNT "
                       "FROM ARCMALLEXPORTHOURLY WHERE MALLCODE=:MALLCODE AND DATE_VALUE >= :STARTDATE "
                       "AND DATE_VALUE < :ENDDATE GROUP BY MALLCODE, DATE_VALUE, TIME_VALUE";
    DBTransaction.StartTransaction();
    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ParamByName("MALLCODE")->AsString = MallCode;
    query->ExecQuery();

    while(!query->Eof)
    {
        TenantName = query->FieldByName("MALLCODE")->AsString;
        DateValue = StartDate.FormatString("mm/dd/yyyy");
        TimeValue = query->FieldByName("TIME_VALUE")->AsString;
        AmountValue = shangrilaExport->FixDecimalPlaces(RoundToNearest(query->FieldByName("AMOUNT_VALUE")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        TaxValue = shangrilaExport->FixDecimalPlaces(RoundToNearest(query->FieldByName("VAT_SALES")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        AdjValue = shangrilaExport->FixDecimalPlaces(RoundToNearest(query->FieldByName("TOTALDISCOUNT")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        SChargeValue = shangrilaExport->FixDecimalPlaces(RoundToNearest(query->FieldByName("SCHARGE_AMOUNT")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        TransactionTotal = query->FieldByName("TRANSACTION_COUNT")->AsCurrency;

        TimeCount[val] = TimeValue.t_str();
        val++;

        DataRead[TimeValue] = GetHourlyFormat(TenantName, DateValue, TimeValue, AmountValue, TaxValue, AdjValue, SChargeValue, TransactionTotal, SalesDept);

        query->Next();
    }
    query->Close();

    for(int i=0;i<12;i++)
    {
        UnicodeString Time = "";
        UnicodeString Data = "";

        switch(i)
        {
            case 0:
                Time = "10";
                break;
            case 1:
                Time = "11";
                break;
            case 2:
                Time = "12";
                break;
            case 3:
                Time = "13";
                break;
            case 4:
                Time = "14";
                break;
            case 5:
                Time = "15";
                break;
            case 6:
                Time = "16";
                break;
            case 7:
                Time = "17";
                break;
            case 8:
                Time = "18";
                break;
            case 9:
                Time = "19";
                break;
            case 10:
                Time = "20";
                break;
            case 11:
                Time = "21";
                break;
            default:
                break;
        }

        for(int i=0;i<val;i++)
        {
            if(TimeCount[i].c_str() == Time)
            {
                it = DataRead.find(Time);
                OutputValue = (it->second);
                flag = true;
                break;
            }
            else
            {
                flag = false;
            }
        }

        if(!flag)
        {
            TransactionTotal = 0;
            OutputValue = GetHourlyFormat(MallCode, DateValue, Time, AmountValue, TaxValue, AdjValue, SChargeValue, TransactionTotal, SalesDept);
        }

        OutputValue = RIdentifier + Format + OutputValue;

        DataToWrite.push_back(OutputValue.t_str());

        dataManager->ExportFile(OutputManager, DataToWrite, FileName);
    }

    // #################
    // EOD Export File #
    // #################

    RIdentifier = "99";
    UnicodeString Gross = "";
    UnicodeString Vat = "";
    UnicodeString Discount = "";
    UnicodeString SCharge = "";

    query0->Close();

    query0->SQL->Text = "SELECT FIRST 1 * FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE AND AM.TRANSACTIONDATE < :ENDDATE ORDER BY AM.ENDING_INVOICE DESC";

    query0->ParamByName("STARTDATE")->AsDateTime = SDate;
    query0->ParamByName("ENDDATE")->AsDateTime = EDate;
    query0->ExecQuery();

    if(query0->RecordCount == 0)
    {
        OutputValue = RIdentifier + Format + "\"" + MallCode + "\"" + Format + StartDate.FormatString("mm/dd/yyyy") + Format +
                      "09:00" + Format + "0.00" + Format + "0.00" + Format + "0.00" + Format + "0.00" + "0" + Format + "\"" + SalesDept + "\"";
        DataToWrite.push_back(OutputValue.t_str());
        return;
    }
    else
    {
        TENANT_NAME = query0->FieldByName("TENANT_NAME")->AsString;
        TRANSACTIONDATE = query0->FieldByName("TRANSACTIONDATE")->AsDateTime;
        Gross = shangrilaExport->FixDecimalPlaces(RoundToNearest(query0->FieldByName("GROSS_SALES")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        Vat = shangrilaExport->FixDecimalPlaces(RoundToNearest(query0->FieldByName("VAT_SALES")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        Discount = shangrilaExport->FixDecimalPlaces(RoundToNearest(query0->FieldByName("TOTALDISCOUNT")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        SCharge = shangrilaExport->FixDecimalPlaces(RoundToNearest(query0->FieldByName("SCHARGE_AMOUNT")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        TRANSACTION_COUNT = query0->FieldByName("TRANSACTION_COUNT")->AsInteger;
        MALLCODE = query0->FieldByName("MALLCODE")->AsString;
        OutputValue = RIdentifier + Format + "\"" + TENANT_NAME + "\"" + Format + TRANSACTIONDATE.FormatString("mm/dd/yyyy") + Format +
                      "09:00" + Format + Gross + Format + Vat + Format + Discount + Format + SCharge + Format +
                      TRANSACTION_COUNT + Format + "\"" + MALLCODE + "\"";
        DataToWrite.push_back(OutputValue.t_str());
    }
    dataManager->ExportFile(OutputManager, DataToWrite, FileName);

    MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );

    DataRead.clear();
    ResetMallExportValues();
}
//---------------------------------------------------------------------------

// Ayala Methods
UnicodeString TfrmMallExportRegenerateReport::FixTime(UnicodeString Time)
{
    UnicodeString result = "";

    if(Time.Length() == 1)
    {
        result = "0" + Time;
    }
    else
    {
        result = Time;
    }
    return result;
}
//---------------------------------------------------------------------------

// Ayala Methods
// This method will generate the output format for hourly export file
UnicodeString TfrmMallExportRegenerateReport::GetHourlyFormat(UnicodeString TerminalName, UnicodeString TenantName,
                                                    UnicodeString DateValue, UnicodeString TimeValue)
{
    int TransactionTotal = 0;
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";
    UnicodeString Amount = "";

    GetHourlyData(TerminalName, TenantName, DateValue, TimeValue, TransactionTotal, Amount);

    if(TransactionTotal != 0)
    {
        result = DateValue + TimeValue + ":00" + Amount + TransactionTotal + TenantName + TerminalName;
    }
    else
    {
        result = DateValue + TimeValue + ":00" + "0.00" + "0" + TenantName + TerminalName;
    }

    return result;
}
//---------------------------------------------------------------------------

// Ayala Methods
// This method will generate the data for hourly export file
UnicodeString TfrmMallExportRegenerateReport::GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                  UnicodeString &DateValue, UnicodeString &TimeValue,
                                                  int &TransactionTotal, UnicodeString &Amount)
{
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";
    TDateTime PrevSDate = SDate - 1.0;
    TDateTime PrevEDate = EDate - 1.0;
    UnicodeString CheckPrevSDate = PrevSDate.FormatString("mm/dd/yy");
    UnicodeString CheckPrevEDate = PrevEDate.FormatString("mm/dd/yy");
    UnicodeString SelectedDate = SDate.FormatString("mm/dd/yy");
    int DayCount = 1;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());

    query1->Close();
    query1->SQL->Text = "SELECT * FROM ARCMALLEXPORTHOURLY AS AMH WHERE AMH.DATE_VALUE >= :STARTDATE "
                        "AND AMH.DATE_VALUE < :ENDDATE ORDER BY AMH.DATE_VALUE, AMH.TIME_VALUE ASC";

    DBTransaction.StartTransaction();
    query1->ParamByName("STARTDATE")->AsDateTime = SDate;
    query1->ParamByName("ENDDATE")->AsDateTime = EDate;
    query1->ExecQuery();

    if(query1->RecordCount == 0)
    {
        while(SelectedDate.Compare(CheckPrevSDate) !=0)
        {
            TIBSQL* query2 = DBTransaction.Query(DBTransaction.AddQuery());
            query2->Close();
            query2->SQL->Text = "SELECT * FROM ARCMALLEXPORTHOURLY AS AMH WHERE AMH.DATE_VALUE >= :STARTDATE "
                                "AND AMH.DATE_VALUE < :ENDDATE ORDER BY AMH.DATE_VALUE, AMH.TIME_VALUE ASC";

            DBTransaction.StartTransaction();
            query2->ParamByName("STARTDATE")->AsDateTime = PrevSDate;
            query2->ParamByName("ENDDATE")->AsDateTime = PrevEDate;
            query2->ExecQuery();

            if(query2->RecordCount == 0)
            {
                PrevSDate = SDate - DayCount;
                PrevEDate = EDate - DayCount;
                DayCount++;
            }
            else
            {
                TerminalName = query2->FieldByName("TERMINAL_NAME")->AsString;
                TenantName = query2->FieldByName("TENANT_NAME")->AsString;
                CheckPrevSDate = SDate.FormatString("mm/dd/yy");
            }
        }
    }
    else
    {
        query->Close();
        query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM FROM ARCMALLEXPORTHOURLY "
                           "WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND "
                           "TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME";
        DBTransaction.StartTransaction();
        query->ParamByName("TIMEVALUE")->AsString = TimeValue;
        query->ParamByName("TERMINALNAME")->AsString = TerminalName;
        query->ParamByName("STARTDATE")->AsDateTime = SDate;
        query->ParamByName("ENDDATE")->AsDateTime = EDate;
        query->ExecQuery();

        AmountTotal = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
        TransactionTotal = RoundToNearest(query->FieldByName("TRANSSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);

        DBTransaction.Commit();
        query->Close();
    }

    Amount = ayalaExport->FixDecimalPlaces(AmountTotal);

    return result;
}

// Ayala Methods
void TfrmMallExportRegenerateReport::GenerateNoExportDays(UnicodeString &TerminalName, UnicodeString &MallCode, TDateTime &TransactionDate, UnicodeString &BegInvoice,
                                                          UnicodeString &EndInvoice, Currency &OldGT, Currency &NewGT, UnicodeString &BegOR, UnicodeString &EndOR, UnicodeString Mall, bool &Pastdate)
{
    TDateTime PrevSDate = SDate - 1.0;
    TDateTime PrevEDate = EDate - 1.0;
    UnicodeString CheckPrevSDate = PrevSDate.FormatString("mm/dd/yy");
    UnicodeString CheckPrevEDate = PrevEDate.FormatString("mm/dd/yy");
    UnicodeString SelectedDate = SDate.FormatString("mm/dd/yy");
    UnicodeString Format = "                    ";
    int DayCount = 1;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

    while(SelectedDate.Compare(CheckPrevSDate) !=0)
    {
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        query->Close();
        query->SQL->Text = "SELECT * FROM ARCMALLEXPORT AS AM WHERE AM.TRANSACTIONDATE >= :STARTDATE "
                           "AND AM.TRANSACTIONDATE < :ENDDATE";

        DBTransaction.StartTransaction();
        query->ParamByName("STARTDATE")->AsDateTime = PrevSDate;
        query->ParamByName("ENDDATE")->AsDateTime = PrevEDate;
        query->ExecQuery();

        if(query->RecordCount == 0)
        {
            PrevSDate = SDate - DayCount;
            PrevEDate = EDate - DayCount;
            DayCount++;
        }
        else
        {
            if(Mall == "AYALA")
            {
                TerminalName = query->FieldByName("TERMINAL_NAME")->AsString;
                MallCode = query->FieldByName("MALLCODE")->AsString;
                TransactionDate = SDate;
                OldGT = query->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
                NewGT = OldGT;
                BegInvoice = "0";
                EndInvoice = "0";
                BegOR = "0";
                EndOR = "0";

                for(int i=0;i<21;i++)
                {
                    if(i==0)
                    {
                        OutputValue = TransactionDate.FormatString("yyyymmdd");
                        DataRead[i] = OutputValue;
                    }
                    else if(i==1)
                    {
                        OutputValue = CurrToStr(OldGT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==2)
                    {
                        OutputValue = CurrToStr(NewGT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==3)
                    {
                        OutputValue = CurrToStr(DAILY_SALES) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==4)
                    {
                        OutputValue = CurrToStr(TOTALDISCOUNT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==5)
                    {
                        OutputValue = CurrToStr(REFUND_AMOUNT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==6)
                    {
                        OutputValue = CurrToStr(VOID_AMOUNT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==7)
                    {
                        OutputValue = CurrToStr(VAT_SALES) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==8)
                    {
                        OutputValue = MallCode + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==9)
                    {
                        OutputValue = BegInvoice + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==10)
                    {
                        OutputValue = EndInvoice + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==11)
                    {
                        OutputValue = BegOR + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==12)
                    {
                        OutputValue = EndOR + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==13)
                    {
                        OutputValue = IntToStr(TRANSACTION_COUNT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==14)
                    {
                        OutputValue = CurrToStr(LOCALTAXES) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==15)
                    {
                        OutputValue = CurrToStr(SCHARGE_AMOUNT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==16)
                    {
                        OutputValue = CurrToStr(VATEXEMPT_SALES) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==17)
                    {
                        OutputValue = CurrToStr(GROSS_SALES) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==18)
                    {
                        OutputValue = CurrToStr(LOCTAXEXEMPTDLY_SALES) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==19)
                    {
                        OutputValue = IntToStr(FINEDINECUST_COUNT) + Format;
                        DataRead[i] = OutputValue;
                    }
                    else if(i==20)
                    {
                        OutputValue = TerminalName + Format;
                        DataRead[i] = OutputValue;
                    }
                }

                CheckPrevSDate = SDate.FormatString("mm/dd/yy");
            }
            else
            {
                for(int i=0;i<30;i++)
                {
                    if(i==0)
                    {
                        DataRead[i] = query->FieldByName("TENANT_NAME")->AsString;
                    }
                    else if(i==1)
                    {
                        DataRead[i] = query->FieldByName("TERMINAL_NAME")->AsString;
                    }
                    else if(i==13)
                    {
                        DataRead[i] = CurrToStr(query->FieldByName("EODOLD")->AsCurrency);
                    }
                    else if(i==14)
                    {
                        OLD_GRANDTOTAL = query->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
                        DataRead[i] = CurrToStrF((RoundToNearest(OLD_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);
                    }
                    else if(i==15)
                    {
                        DataRead[i] = CurrToStr(query->FieldByName("EODNEW")->AsCurrency);
                    }
                    else if(i==16)
                    {
                        NEW_GRANDTOTAL = query->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
                        DataRead[i] = CurrToStrF((RoundToNearest(NEW_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);
                    }
                    else if(i==17)
                    {
                        DataRead[i] = SDate.FormatString("mm/dd/yyyy");
                        DATE_VALUE  = SDate;
                    }
                    else if(i==5 || i==7 || i==9 || i==11 || i==29)
                    {
                        DataRead[i] = IntToStr(0);
                    }
                    else
                    {
                        DataRead[i] = CurrToStrF((RoundToNearest(0, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);
                    }
                }
                CheckPrevSDate = SDate.FormatString("mm/dd/yy");
            }
        }

        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());
        query1->Close();
        query1->SQL->Text = "SELECT FIRST 1 AM.TRANSACTIONDATE FROM ARCMALLEXPORT AS AM ORDER BY AM.TRANSACTIONDATE ASC ";

        DBTransaction.StartTransaction();
        query1->ExecQuery();

        TDateTime DATEVAL;
        UnicodeString CompareVal = "";
        UnicodeString StartingDate = "";
        DATEVAL = query1->FieldByName("TRANSACTIONDATE")->AsDateTime;

        Pastdate = false;

        if(SDate < DATEVAL)
        {
            Pastdate = true;
            MessageBox( "Cannot Generate Export File for dates prior to the first Mall Export date! " "\""+ DATEVAL.FormatString("mm-dd-yyyy") + "\"", "Cannot Generate File", MB_OK );
            break;
        }
    }
}
//---------------------------------------------------------------------------

// Ayala Method
void TfrmMallExportRegenerateReport::ResetMallExportValues()
{
    MALLCODE = "";
    TENANT_NAME = "";
    CLASSCODE = "";
    TRADECODE = "";
    OUTLETNUMBER = "";
    NEW_GRANDTOTAL = 0;
    OLD_GRANDTOTAL = 0;
    SALESTYPE = "";
    DEPARTMENTSUM = 0;
    REGDISCOUNT_AMOUNT = 0;
    EMPDISCOUNT_AMOUNT = 0;
    SCDISCOUNT_AMOUNT = 0;
    VIPDISCOUNT_AMOUNT = 0;
    PWDDISCOUNT_AMOUNT = 0;
    GPCDISCOUNT_AMOUNT = 0;
    DISCOUNTG1_AMOUNT = 0;
    DISCOUNTG2_AMOUNT = 0;
    DISCOUNTG3_AMOUNT = 0;
    DISCOUNTG4_AMOUNT = 0;
    DISCOUNTG5_AMOUNT = 0;
    DISCOUNTG6_AMOUNT = 0;
    VAT_SALES = 0;
    OTHERTAX = 0;
    ADJUSTMENTS = 0;
    POS_ADJ = 0;
    NEG_ADJ = 0;
    POS_ADJ_NONTAX = 0;
    NEG_ADJ_NONTAX = 0;
    DAILY_SALES = 0;
    VOID_AMOUNT = 0;
    REFUND_AMOUNT = 0;
    TAX_INCLUSIVE_SALES = 0;
    VATEXEMPT_SALES = 0;
    CHARGECRED_SALES = 0;
    CHARGECRED_SALES_TAX = 0;
    GIFT_CHEQUE_SALES = 0;
    DEBIT_CARD_SALES = 0;
    OTHER_TENDER_SALES = 0;
    MASTERCARD_SALES = 0;
    VISA_SALES = 0;
    AMEX_SALES = 0;
    DINERS_SALES = 0;
    JCB_SALES = 0;
    OTHER_CARD_SALES = 0;
    SCHARGE_AMOUNT = 0;
    OTHER_CHARGES = 0;
    FIRST_TRANSACTION = 0;
    LAST_TRANSACTION = 0;
    TRANSACTION_COUNT = 0;
    BEGINNING_INVOICE = "";
    ENDING_INVOICE = "";
    CASH_COUNT = 0;
    GC_COUNT = 0;
    DEBIT_COUNT = 0;
    OTHER_TENDER_COUNT = 0;
    MASTERCARD_COUNT = 0;
    VISA_COUNT = 0;
    AMEX_COUNT = 0;
    DINERS_COUNT = 0;
    JCB_COUNT = 0;
    OTHERCARD_COUNT = 0;
    TERMINAL_NAME = "";
    SERIALNUMBER = "";
    TRANSACTIONTIME;
    TRANSACTIONDATE;
    GROSS_SALES = 0;
    VOID_COUNT = 0;
    REGDISCOUNT_COUNT = 0;
    REFUND_COUNT = 0;
    SCDISCOUNT_COUNT = 0;
    EODOLD = 0;
    EODNEW = 0;
    DATE_VALUE;
    NOVELTY_SALES = 0;
    MISC_SALES = 0;
    LOCALTAXES = 0;
    PHARMA_SALES = 0;
    NONPHARMA_SALES = 0;
    SALESNOTSUBTORENT = 0;
    REPTRANSACTION_AMOUNT = 0;
    REPTRANSACTION_COUNT = 0;
    CASH_SALES = 0;
    HOUR_VALUE = "";
    TOTALDISCOUNT = 0;
    BEGINNING_OR = "";
    ENDING_OR = "";
    LOCTAXEXEMPTDLY_SALES = 0;
    FINEDINECUST_COUNT = 0;
    TENDERSURCHARGES = 0;
    NONVAT_SALES = 0;
    CHECK_SALES = 0;
    EPAY_SALES = 0;
    NO_SALES = 0;
    PREVEODCOUNTER = 0;
    CURRENTEODCOUNTER = 0;
    DISCOUNT_COUNT = 0;
    CARD_SALES = 0;
    OTHER_SALES = 0;
    INVOICE_NUMBER = "";
    PRODISCOUNT_AMOUNT = 0;
    OTHERDISCOUNT_AMOUNT = 0;
    OTHER_SURCHARGE = 0;
    NET_SALES = 0;
    GC_SALES_AMOUNT = 0;
    DEPOSIT_AMOUNT = 0;
    APPLIED_DEPOSIT_AMOUNT = 0;
    ZCOUNT = 0;
    VAT_SALES_COUNT = 0;
    NONVAT_SALES_COUNT = 0;
    VATEXEMPT_SALES_COUNT = 0;
    EMPDISCOUNT_COUNT = 0;
    PRODISCOUNT_COUNT = 0;
    OTHERDISCOUNT_COUNT = 0;
    SCHARGE_COUNT = 0;
    OTHER_SURCHARGE_COUNT = 0;
    CARD_COUNT = 0;
    CHECK_COUNT = 0;
    EPAY_COUNT = 0;
    OTHER_COUNT = 0;
    TRANS_VOID_COUNT = 0;
    DEPOSIT_COUNT = 0;
    APPLIED_DEPOSIT_COUNT = 0;
    PATRON_COUNT = 0;
    NOSALE_COUNT = 0;
    SALES_FLAG = "";
    VAT_PERCENTAGE = 0;
    SALESTYPE_FOOD = 0;
    SALESTYPE_NONFOOD = 0;
    SALESTYPE_GROCERIES = 0;
    SALESTYPE_MEDICINES = 0;
    SALESTYPE_OTHERS = 0;
}
//---------------------------------------------------------------------------

// Alphaland Method
UnicodeString TfrmMallExportRegenerateReport::AddZeroesToValue(int ZeroCount, int Value)
{
    UnicodeString Zeroes = "";
    UnicodeString ItemNo = "";
    UnicodeString StrCount = "";

    std::ostringstream StreamIntToStr;
    std::string StringIntVal = "";

    StreamIntToStr << Value;
    StringIntVal = StreamIntToStr.str();
    ItemNo = StringIntVal.c_str();

    Zeroes = "";
    for(int j=0;j<(ZeroCount-ItemNo.Length());j++)
    {
        Zeroes += "0";
    }

    return Zeroes;
}
//---------------------------------------------------------------------------

// Alphaland Method
UnicodeString TfrmMallExportRegenerateReport::CheckValue(Currency Amount)
{
    UnicodeString result = "";
    std::string StrValue = "";
    bool Flag = false;


    if(Amount == 0)
    {
        result = "0.00";
    }
    else
    {
        result = RoundToNearest(Amount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
        StrValue = result.t_str();
        for(int i=0; i<StrValue.length();i++)
        {
            if(StrValue[i] == '.')
            {
                Flag = true;
            }
        }

        if(!Flag)
        {
            result = result + ".00";
        }
    }

    return result;
}
//---------------------------------------------------------------------------

// Megaworld Methods
UnicodeString TfrmMallExportRegenerateReport::GetHourlyFormat(UnicodeString TenantName, UnicodeString DateValue, UnicodeString TimeValue,
                                                        UnicodeString AmountValue, UnicodeString TaxValue, UnicodeString AdjValue, UnicodeString SChargeValue,
                                                        int TransactionTotal, UnicodeString SalesDept)
{
    UnicodeString result = "";
    UnicodeString Format = " ";
    UnicodeString TimeData = "";

    TimeData = shangrilaExport->GetRegTime(TimeValue);

    if(TransactionTotal != 0)
    {
        result = "\"" + TenantName + "\"" + Format + DateValue + Format + TimeData + Format +
                 AmountValue + Format + TaxValue + Format + AdjValue + Format + SChargeValue + Format +
                 TransactionTotal + Format + "\"" + SalesDept + "\"" + "\n";
    }
    else
    {
        result = "\"" + TenantName + "\"" + Format + DateValue + Format + TimeData + Format +
                 "0.00" + Format + "0.00" + Format + "0.00" + Format + "0.00" + Format +
                 "0" + Format + "\"" + SalesDept + "\"" + "\n";
    }

    return result;
}
//---------------------------------------------------------------------------
//DLF Mall export
void TfrmMallExportRegenerateReport::RegenerateDLFMallExport()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TIBSQL* TimeQuery = DBTransaction.Query(DBTransaction.AddQuery());
    TimeQuery->Close();
    TimeQuery->SQL->Text =
                            "SELECT "
                            "    Extract (Day From a.TIME_STAMP) Bill_Day,     "
                            "    Extract (month From a.TIME_STAMP) Bill_Month, "
                            "    Extract (year From a.TIME_STAMP) Bill_Year    "
                            "FROM ARCBILL a "
                            "WHERE "
                            "       a.TIME_STAMP >= :START_TIME AND "
                            "       a.TIME_STAMP < :END_TIME "
                            "GROUP BY 3,2,1 "
                            "ORDER BY 3,2,1 ";
    TimeQuery->ParamByName("START_TIME")->AsDateTime = SDate;
    TimeQuery->ParamByName("END_TIME")->AsDateTime = EDate;
    TimeQuery->ExecQuery();

    for (; !TimeQuery->Eof; TimeQuery->Next())
    {
        day =  TimeQuery->FieldByName("Bill_Day")->AsInteger;
        month =  TimeQuery->FieldByName("Bill_Month")->AsInteger;
        year =  TimeQuery->FieldByName("Bill_Year")->AsInteger;
        TrasactionDate = EncodeDateTime(year, month, day, 0, 0, 0, 0);
        TIBSQL* IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        // for opened section code 1
        WriteInToFileForFirstCode(&DBTransaction, day, month, year, TrasactionDate);

        // Get all bills for that time period
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                    "SELECT "
                                        "a.ARCBILL_KEY, "
                                        "a.TIME_STAMP "
                                    "FROM ARCBILL a "
                                    "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                    "GROUP BY "
                                        "a.ARCBILL_KEY,a.TIME_STAMP "
                                    "Order By "
                                        "a.TIME_STAMP asc ";
        IBInternalQuery->ParamByName("Day")->AsInteger = day;
        IBInternalQuery->ParamByName("Month")->AsInteger = month;
        IBInternalQuery->ParamByName("Year")->AsInteger = year;
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            int ArcBillKey = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;

            //section 101 starts here
            WriteInToFileForSecondCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);
            //section 101 ends here

            //section 111 starts here
            WriteInToFileForThirdCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);

            //121 section starts from here
            WriteInToFileForFourthCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);

            //section 131 starts from here
            WriteInToFileForFifthCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);
    }
     //for code 1 closed section starts from here
    WriteInToFileForSixthCode(&DBTransaction, day, month, year);

    //load file to ftp
    LoadFileToFTP();

    //change file name to null
    TGlobalSettings::Instance().DLFMallFileName = "";
    std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
    frmAnalysis->SaveCompValueinDBStrUnique(vmDLFMallFileName, TGlobalSettings::Instance().DLFMallFileName);

    //increase file_number
    TGlobalSettings::Instance().BatchNo = TGlobalSettings::Instance().BatchNo+1;;
    TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmBatchNo, TGlobalSettings::Instance().BatchNo);

    }

    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::WriteInToFileForFirstCode(Database::TDBTransaction *dbTransaction, int day, int month, int year, TDateTime cdate)
{
    TIBSQL* IBInternalQuery = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = " SELECT "
                                        "FIRST 1 a.ARCBILL_KEY, "
                                        "a.INVOICE_NUMBER, "
                                        "a.TIME_STAMP, "
                                        "a.STAFF_NAME, "
                                        "a.TERMINAL_NAME "
                                "FROM ARCBILL a "
                                 "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                 "ORDER BY "
                                        "a.TIME_STAMP asc ";
      IBInternalQuery->ParamByName("Day")->AsInteger = day;
      IBInternalQuery->ParamByName("Month")->AsInteger = month;
      IBInternalQuery->ParamByName("Year")->AsInteger = year;

    IBInternalQuery->ExecQuery();


        TIBSQL* IBInternalQueryInvNumber = dbTransaction->Query(dbTransaction->AddQuery());
        IBInternalQueryInvNumber->Close();
        IBInternalQueryInvNumber->SQL->Text =  "select INVOICE_NUMBER from ARCBILL where ARCBILL.ARCBILL_KEY= :ARCBILL_KEY "  ;
        IBInternalQueryInvNumber->ParamByName("ARCBILL_KEY")->AsString =IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger-1 ;
        IBInternalQueryInvNumber->ExecQuery();
        AnsiString Receipt_No = IBInternalQueryInvNumber->FieldByName("INVOICE_NUMBER")->AsString == "" ?
                                        IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString : IBInternalQueryInvNumber->FieldByName("INVOICE_NUMBER")->AsString;

    AnsiString invoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
    AnsiString staffName = IBInternalQuery->FieldByName("STAFF_NAME")->AsString;
    terminal_Name = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
    TDateTime date_Time = IBInternalQuery->FieldByName("TIME_STAMP")->AsString;

    AnsiString cmd_code = "1";
    AnsiString file_stat = "OPENED";
    AnsiString tenant_No = TGlobalSettings::Instance().TenantNo;
    AnsiString pos_No = terminal_Name;
  //  AnsiString Receipt_No = invoiceNumber-1;
    int Tran_File_No =TGlobalSettings::Instance().BatchNo;
    AnsiString date = date_Time.FormatString("yyyymmdd");
    AnsiString time = date_Time.FormatString("hh:mm:ss");
    AnsiString user_ID = staffName;
    AnsiString Sale_date = date_Time.FormatString("yyyymmdd");

    AnsiString  finalValue = cmd_code+"|" +file_stat+"|"+tenant_No+"|"+pos_No+"|"+Receipt_No+"|"+ Tran_File_No+ "|" +
    date  +"|" +time+"|" +user_ID+"|" +Sale_date;

    //creating instance of TListPaymentSystem
    TListPaymentSystem *PaymentSystem;
    if(TGlobalSettings::Instance().DLFMallFileName==""||TGlobalSettings::Instance().DLFMallFileName== "[null]" )
    {
        AnsiString fileName = PaymentSystem->CreateFilename(cdate);
        AnsiString filePath = PaymentSystem->CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
    }
}
//---------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::WriteInToFileForSecondCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery0 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery0->Close();
    IBInternalQuery0->SQL->Text =
                                "SELECT "
                                    "a.INVOICE_NUMBER, "
                                    "cast((CASE WHEN coalesce(a.TOTAL,0) < 0  THEN  a.INVOICE_NUMBER  "
                                                             "END) as numeric(17, 4)) AS Refund_Receipt, "
                                    "a.TIME_STAMP, "
                                    "a.STAFF_NAME, "
                                    "a.TERMINAL_NAME, "
                                    "a.PATRON_COUNT "
                                "FROM ARCBILL a "
                                "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                        "AND a.ARCBILL_KEY = :ARCBILL_KEY "
                                "GROUP BY "
                                    "a.INVOICE_NUMBER, "
                                    "a.TIME_STAMP,  "
                                    " a.STAFF_NAME, "
                                    "a.TERMINAL_NAME, "
                                    "a.PATRON_COUNT, "
                                    "a.TOTAL ";
    IBInternalQuery0->ParamByName("Day")->AsInteger = day;
    IBInternalQuery0->ParamByName("Month")->AsInteger = month;
    IBInternalQuery0->ParamByName("Year")->AsInteger = year;
    IBInternalQuery0->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery0->ExecQuery();

    AnsiString cmd_code= "101";
    AnsiString receipt_No   = IBInternalQuery0->FieldByName("INVOICE_NUMBER")->AsString;
    AnsiString shift_No = "1";
    TDateTime invoice_Date_Time =  IBInternalQuery0->FieldByName("TIME_STAMP")->AsDateTime;
    AnsiString Date = invoice_Date_Time.FormatString("yyyymmdd");
    AnsiString Time = invoice_Date_Time.FormatString("hh:mm:ss");
    AnsiString user_id = IBInternalQuery0->FieldByName("STAFF_NAME")->AsString;
    AnsiString manual_Receipt = "";
    UnicodeString refund_Receipt = IBInternalQuery0->FieldByName("Refund_Receipt")->AsString;
    AnsiString reason_Code = "";
    AnsiString salesMan_Code = IBInternalQuery0->FieldByName("STAFF_NAME")->AsString;
    AnsiString table_No = "";//IBInternalQuery0->FieldByName("TERMINAL_NAME")->AsString;
    int cust_Count = IBInternalQuery0->FieldByName("PATRON_COUNT")->AsInteger;
    AnsiString training = "N";
    AnsiString tran_status = "SALE";

    AnsiString  finalValue= cmd_code+"|" +receipt_No+"|"+shift_No+"|"+Date+"|"+Time+"|"+ user_id+ "|" +
    manual_Receipt  +"|" +refund_Receipt+"|" +reason_Code+"|" +salesMan_Code
    +"|" +table_No+"|" +cust_Count+"|" +training+"|" +tran_status;

    TListPaymentSystem *PaymentSystem;
    AnsiString fileName = PaymentSystem->CreateFilename(date);
    AnsiString filePath = PaymentSystem->CreateTextFile(fileName,finalValue);
}
//------------------------------------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::WriteInToFileForThirdCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery1 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery1->Close();
    IBInternalQuery1->SQL->Text =

                                "  SELECT "
                                            "    ARC.ARCBILL_KEY,  "
                                            "    ARC.ITEM_ID,      "
                                            "    SUM(ARC.QTY) QTY, "
                                            "    ARC.PRICE_LEVEL0, "
                                            "    cast((CASE WHEN coalesce(ARC.PRICE_ADJUST,0) <> 0  THEN  coalesce(ARC.PRICE_ADJUST,0)                                                             "
                                            "                               WHEN ARC.HAPPY_HOUR = 'F' AND coalesce(ARC.PRICE_ADJUST,0) = 0 AND ARC.PRICE <> 0 THEN  coalesce(ARC.PRICE_LEVEL0,0)   "
                                            "                               WHEN ARC.HAPPY_HOUR = 'T' AND coalesce(ARC.PRICE_ADJUST,0) = 0 AND ARC.PRICE <> 0 THEN  coalesce(ARC.PRICE_LEVEL1,0)   "
                                            "                               WHEN coalesce(ARC.PRICE_ADJUST,0) = 0 AND ARC.PRICE = 0 THEN  coalesce(ARC.PRICE,0)                                    "
                                            "                         END) as numeric(17, 4)) AS New_Price,                                                                                        "
                                            "    ARC.DISCOUNT,          "
                                            "    ARC.MENU_NAME,         "
                                            "    ARC.CATEGORY_KEY,      "
                                            "    CAST(SUM(ARC.QTY*ARC.PRICE)+ ARC.DISCOUNT - Sum(  COALESCE(AOT.VAT,0) ) - Sum( COALESCE( AOT.ServiceCharge,0)) - Sum( COALESCE( AOT.OtherServiceCharge,0))AS NUMERIC(17,4))ITEM_NSALES, "
                                            "    CAST(SUM(ARC.DISCOUNT) AS NUMERIC(17,4)) TOTAL_DISCOUNT, "
                                            "    CAST(Sum(COALESCE(AOT.VAT,0) ) + Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0)) AS NUMERIC(17,4)) TOTAL_TAX, "
                                            "    ARC.PLU "
                                "FROM ARCHIVE ARC "
                                    "LEFT JOIN (  "
                                            "	SELECT    "
                                            "	ARCORDERTAXES.ARCHIVE_KEY, "
                                            "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,                "
                                            "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,      "
                                            "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge  "
                                            "	FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,            "
                                            "	Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                                            "	FROM ARCORDERTAXES a                               "
                                            "	group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                                            "	order by 1 )  ARCORDERTAXES                        "
                                            "	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                                    "	AOT ON ARC.ARCHIVE_KEY = AOT.ARCHIVE_KEY           "
                                "WHERE "
                                        "Extract (Day From ARC.TIME_STAMP_BILLED) = :Day and "
                                        "Extract (Month From ARC.TIME_STAMP_BILLED) = :Month and "
                                        "Extract (Year From ARC.TIME_STAMP_BILLED) = :Year and ARC.ARCBILL_KEY = :ARCBILL_KEY  "
                               "GROUP BY "
                                        " ARC.ITEM_ID,       "
                                        "    ARC.QTY,                "
                                        "    ARC.PRICE_LEVEL0,       "
                                        "    ARC.PRICE_LEVEL1,       "
                                        "    ARC.PRICE_ADJUST,       "
                                        "    ARC.DISCOUNT,           "
                                        "    ARC.MENU_NAME,          "
                                        "    ARC.CATEGORY_KEY,       "
                                        "    AOT.VAT,                "
                                        "    AOT.ServiceCharge,      "
                                        "    AOT.OtherServiceCharge, "
                                        "    ARC.PLU,                "
                                        "    ARC.ARCBILL_KEY,        "
                                        "    ARC.HAPPY_HOUR,         "
                                        "    ARC.PRICE               "
                                "ORDER BY "
                                        "ARC.ARCBILL_KEY    ";
    IBInternalQuery1->ParamByName("Day")->AsInteger = day;
    IBInternalQuery1->ParamByName("Month")->AsInteger = month;
    IBInternalQuery1->ParamByName("Year")->AsInteger = year;
    IBInternalQuery1->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery1->ExecQuery();

   for (; !IBInternalQuery1->Eof; IBInternalQuery1->Next())
    {
      	int qty = IBInternalQuery1->FieldByName("QTY")->AsCurrency;
		AnsiString cmd_code= "111";
		AnsiString  item_Code = IBInternalQuery1->FieldByName("ITEM_ID")->AsInteger;
		AnsiString  item_Qty =  IBInternalQuery1->FieldByName("QTY")->AsCurrency;
		AnsiString  org_Price = IBInternalQuery1->FieldByName("PRICE_LEVEL0")->AsCurrency;
		AnsiString  new_Price = IBInternalQuery1->FieldByName("New_Price")->AsCurrency;
		AnsiString  item_Flag              ="";
		AnsiString  tax_Code               ="Tax";
		AnsiString  discount_Code          ="";
		AnsiString  discount_Amt   = IBInternalQuery1->FieldByName("DISCOUNT")->AsCurrency;
		AnsiString  item_Dept              = IBInternalQuery1->FieldByName("MENU_NAME")->AsString;//ItemComplete->MenuName;              	//length 8
		AnsiString  item_Catg              = IBInternalQuery1->FieldByName("CATEGORY_KEY")->AsInteger;//catkey ;          			  	//length 8
		AnsiString  label_Keys             = "";              				//length 8
		AnsiString  item_Comm              = "0";             				 //length 1
		AnsiString  item_Nsales            = IBInternalQuery1->FieldByName("ITEM_NSALES")->AsCurrency;

		AnsiString  discount_By            = IBInternalQuery1->FieldByName("TOTAL_DISCOUNT")->AsCurrency;
		AnsiString  discount_Sign          ="$";              				//length 1
		AnsiString  item_Stax              = IBInternalQuery1->FieldByName("TOTAL_TAX")->AsCurrency;//CurrToStr(ItemComplete->BillCalcResult.TotalTax);              //length 10
		AnsiString  plu_Code               = IBInternalQuery1->FieldByName("PLU")->AsInteger;


		AnsiString  finalValue= cmd_code+"|" +item_Code+"|"+item_Qty+"|"+org_Price+"|"+new_Price+"|"+ item_Flag+ "|" +
		tax_Code  +"|" +discount_Code+"|" +discount_Amt+"|" +item_Dept
		+"|" +item_Catg+"|" +label_Keys+"|" +item_Comm+"|" +item_Nsales+"|" +discount_By+"|" +discount_Sign+"|" +item_Stax+"|" +plu_Code;

        TListPaymentSystem *PaymentSystem;
		AnsiString fileName = PaymentSystem->CreateFilename(date);
		AnsiString filePath = PaymentSystem->CreateTextFile(fileName,finalValue);
    }
}
//------------------------------------------------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::WriteInToFileForFourthCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery2 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery2->Close();
    IBInternalQuery2->SQL->Text =
                                    "SELECT AB.ARCBILL_KEY,                                                   "
                                        "     CAST (AB.TOTAL-AB.DISCOUNT-(Sum(COALESCE(AOT.VAT,0) ) + Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0))) AS NUMERIC (17,4)) SALES,                 "
                                        "    CAST (AB.TOTAL AS NUMERIC (17,4)) TOTAL_SALES,                       "
                                        "   -1*AB.DISCOUNT DISCOUNT,                                                         "
                                        "    CAST(Sum( COALESCE( AOT.ServiceCharge,0))AS NUMERIC(17,4)) CHARGES,  "
                                        "    CAST(Sum(COALESCE(AOT.VAT,0) ) + Sum( COALESCE( AOT.OtherServiceCharge,0)) AS NUMERIC(17,4)) TAX, "
                                        "    AB.ROUNDING_ADJUSTMENT ROUNDING_AMT  "
                                    "FROM ARCBILL AB                          "
                                    "LEFT JOIN (                              "
                                    "                SELECT                   "
                                    "                    AOT.ARCBILL_KEY,     "
                                    "                    CAST(Sum(COALESCE(AOT.VAT,0) ) AS NUMERIC(17,4)) VAT, "
                                    "                    CAST(Sum( COALESCE( AOT.ServiceCharge,0)) AS NUMERIC(17,4)) ServiceCharge,           "
                                    "                    CAST(Sum( COALESCE( AOT.OtherServiceCharge,0)) AS NUMERIC(17,4)) OtherServiceCharge  "
                                    "                FROM                                                                                     "
                                    "                (                                                                                        "
                                    "                    SELECT                                                                               "
                                    "                            ARCORDERTAXES.ARCHIVE_KEY,                                                   "
                                    "                            MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,                "
                                    "                            MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,      "
                                    "                            MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge ,"
                                    "                            ARCHIVE.ARCBILL_KEY                                                                               "
                                    "                            FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                                                           "
                                    "                            Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE                                                "
                                    "                            FROM ARCORDERTAXES a                                                                              "
                                    "                            group by  a.TAX_TYPE  , a.ARCHIVE_KEY                                                             "
                                    "                            order by 1 )  ARCORDERTAXES                                           "
                                    "                            LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY  "
                                    "                            GROUP BY ARCORDERTAXES.ARCHIVE_KEY, ARCHIVE.ARCBILL_KEY)AOT           "
                                    "                GROUP BY         "
                                    "                AOT.ARCBILL_KEY  "
                                    "            )                    "
                                    "        AOT ON  AB.ARCBILL_KEY = AOT.ARCBILL_KEY  "
                                    "WHERE "
                                        "Extract (Day From AB.TIME_STAMP) = :Day and "
                                        "Extract (Month From AB.TIME_STAMP) = :Month and "
                                        "Extract (Year From AB.TIME_STAMP) = :Year and AB.ARCBILL_KEY = :ARCBILL_KEY "
                                    "GROUP BY                    "
                                    "    AB.ARCBILL_KEY,         "
                                    "    AB.TOTAL,               "
                                    "    AB.DISCOUNT,            "
                                    "    AOT.VAT,                "
                                    "    AOT.ServiceCharge,      "
                                    "    AOT.OtherServiceCharge, "
                                    "    AB.ROUNDING_ADJUSTMENT  "
                                    "ORDER BY                    "
                                    "    AB.ARCBILL_KEY          " ;
    IBInternalQuery2->ParamByName("Day")->AsInteger = day;
    IBInternalQuery2->ParamByName("Month")->AsInteger = month;
    IBInternalQuery2->ParamByName("Year")->AsInteger = year;
    IBInternalQuery2->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery2->ExecQuery();

	AnsiString cmd_code= "121";
	AnsiString  sales         = IBInternalQuery2->FieldByName("SALES")->AsCurrency;
	AnsiString  discount      = IBInternalQuery2->FieldByName("DISCOUNT")->AsCurrency;

	AnsiString  cess          ="";              //length 11
	AnsiString  charges       = IBInternalQuery2->FieldByName("CHARGES")->AsCurrency;
	AnsiString  tax           = IBInternalQuery2->FieldByName("TAX")->AsCurrency;          //length 11
	AnsiString  tax_Type      = "";
    AnsiString grandTotal =  IBInternalQuery2->FieldByName("TOTAL_SALES")->AsCurrency;
	tax_Type="E";
	AnsiString  exempt_Gst    ="Y";
	AnsiString discount_Code = "";
	AnsiString  other_chg     ="";
 AnsiString  discount_Per  = IBInternalQuery2->FieldByName("SALES")->AsCurrency != 0 ? RoundAt(((IBInternalQuery2->FieldByName("DISCOUNT")->AsCurrency*100)/(IBInternalQuery2->FieldByName("SALES")->AsCurrency)),1)
                                : RoundAt(IBInternalQuery2->FieldByName("DISCOUNT")->AsCurrency, 1);

	AnsiString  rounding_Amt  = IBInternalQuery2->FieldByName("ROUNDING_AMT")->AsCurrency;

	AnsiString finalValue= cmd_code+"|" +sales+"|"+discount+"|"+cess+"|"+charges+"|"+ tax+ "|" +
	tax_Type  +"|" +exempt_Gst+"|" +discount_Code+"|" +other_chg
	+"|" +discount_Per+"|" +rounding_Amt;

    TListPaymentSystem *PaymentSystem;
	AnsiString fileName = PaymentSystem->CreateFilename(date);
	AnsiString filePath =     PaymentSystem->CreateTextFile(fileName,finalValue);
}
//----------------------------------------------------------------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::WriteInToFileForFifthCode(Database::TDBTransaction *dbTransaction, int arcBillKey,int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery3 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery3->Close();
    IBInternalQuery3->SQL->Text =
                                    "SELECT   "
                                        "a.ARCBILL_KEY, "
                                        "a.PAY_TYPE,   "
                                        "a.SUBTOTAL,   "
                                        "a.ROUNDING,   "
                                        "AB.TOTAL,      "
                                        "a.NOTE "
                                    "FROM ARCBILLPAY a "
                                    "INNER JOIN ARCBILL AB on a.ARCBILL_KEY = AB.ARCBILL_KEY "
                                    "WHERE "
                                        "a.SUBTOTAL <> 0 AND  "
                                        " a.ARCBILL_KEY = :ARCBILL_KEY AND  "
                                        "Extract (Day From AB.TIME_STAMP) = :Day and "
                                        "Extract (Month From AB.TIME_STAMP) = :Month and "
                                        "Extract (Year From AB.TIME_STAMP) = :Year "
                                    "ORDER BY a.ARCBILL_KEY ";
    IBInternalQuery3->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery3->ParamByName("Day")->AsInteger = day;
    IBInternalQuery3->ParamByName("Month")->AsInteger = month;
    IBInternalQuery3->ParamByName("Year")->AsInteger = year;
    IBInternalQuery3->ExecQuery();


    for (; !IBInternalQuery3->Eof; IBInternalQuery3->Next())
    {
        AnsiString cmd_code= "131";
        AnsiString  type;
        if(IBInternalQuery3->FieldByName("NOTE")->AsString != "")
        {
            type          = "C";
        }
        else
        {
            type          = "T";
        }
        AnsiString  payment_Name  = IBInternalQuery3->FieldByName("PAY_TYPE")->AsString;//SubPayment->Name;              //length 20
        AnsiString  curr_Code     = "$";
        AnsiString  buy_Rate      = "1";
        AnsiString  amount        =  IBInternalQuery3->FieldByName("SUBTOTAL")->AsCurrency;
        AnsiString  remarks1      = "";
        AnsiString  remarks2      = "";
        AnsiString  base_Amount   = IBInternalQuery3->FieldByName("TOTAL")->AsCurrency;//PaymentTransaction.Money.GrandTotal;              //length 10

        AnsiString  finalValue= cmd_code+"|" +type+"|"+payment_Name+"|"+curr_Code+"|"+buy_Rate+"|"+ amount+ "|" +
        remarks1  +"|" +remarks2+"|" +base_Amount;

         TListPaymentSystem *PaymentSystem;
        AnsiString fileName = PaymentSystem->CreateFilename(date);
        AnsiString filePath =  PaymentSystem->CreateTextFile(fileName,finalValue);
   }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::WriteInToFileForSixthCode(Database::TDBTransaction *dbTransaction,int day , int month , int year)
{
    TIBSQL* IBInternalQuery4 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery4->Close();
    IBInternalQuery4->SQL->Text = " SELECT "
                                        "FIRST 1 a.ARCBILL_KEY, "
                                        "a.INVOICE_NUMBER, "
                                        "a.TIME_STAMP, "
                                        "a.STAFF_NAME, "
                                        "a.TERMINAL_NAME "
                                "FROM ARCBILL a "
                                 "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                 "ORDER BY "
                                        "a.ARCBILL_KEY DESC ";
    IBInternalQuery4->ParamByName("Day")->AsInteger = day;
    IBInternalQuery4->ParamByName("Month")->AsInteger = month;
    IBInternalQuery4->ParamByName("Year")->AsInteger = year;

    IBInternalQuery4->ExecQuery();

    TDateTime End_Date_Time = IBInternalQuery4->FieldByName("TIME_STAMP")->AsString;

    AnsiString cmd_code= "1";
    AnsiString file_stat ="Closed";
    AnsiString tenant_No = TGlobalSettings::Instance().TenantNo;
    AnsiString pos_No = terminal_Name;//IBInternalQuery4->FieldByName("TERMINAL_NAME")->AsString;
    AnsiString Receipt_No = IBInternalQuery4->FieldByName("INVOICE_NUMBER")->AsString;
    AnsiString Tran_File_No = TGlobalSettings::Instance().BatchNo;
    AnsiString date = End_Date_Time.FormatString("yyyymmdd");
    AnsiString time = End_Date_Time.FormatString("hh:mm:ss");
    AnsiString user_ID = IBInternalQuery4->FieldByName("STAFF_NAME")->AsString;
    AnsiString Sale_date = End_Date_Time.FormatString("yyyymmdd");

    AnsiString finalValue= cmd_code+"|" +file_stat+"|"+tenant_No+"|"+pos_No+"|"+Receipt_No+"|"+ Tran_File_No+ "|" +
    date  +"|" +time+"|" +user_ID+"|" +Sale_date;

    if(TGlobalSettings::Instance().DLFMallFileName !="" && TGlobalSettings::Instance().DLFMallFileName!= "[null]" )
    {
        AnsiString mallPath = TGlobalSettings::Instance().MallPath;
        AnsiString LocalPath = mallPath + "\\DLFMall\\" +  TGlobalSettings::Instance().DLFMallFileName;

        TStrings * List = new TStringList();
        if (FileExists(LocalPath ))
        {
           List->LoadFromFile(LocalPath );
        }
        List->Add(finalValue);
        List->SaveToFile(LocalPath );
        delete List;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::LoadFileToFTP()
{
    UnicodeString HostName = TGlobalSettings::Instance().FTPServer;
    UnicodeString UserPath = TGlobalSettings::Instance().FTPPath;
    UnicodeString UserName = TGlobalSettings::Instance().FTPUserName;
    UnicodeString UserPassword = TGlobalSettings::Instance().FTPPassword;

    UnicodeString LocalPathFileName= TGlobalSettings::Instance().MallPath+ "\\DLFMall\\"+TGlobalSettings::Instance().DLFMallFileName; //"C:/trip.txt";//
    UnicodeString LocalFileName = TGlobalSettings::Instance().DLFMallFileName ;
    int FCount=0;
    const char * hostName = HostName.t_str();
    const char * userPath = UserPath.t_str();
    const char * userName = UserName.t_str();
    const char * userPassword = UserPassword.t_str();

    std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
    frmAnalysis->FileSubmit(hostName, userName, userPassword, userPath, LocalPathFileName, LocalFileName, FCount);
}
//-------------------------------------------------------------------------------------------------------------------------------------

// Initialize Start and End Time for each mall
void TfrmMallExportRegenerateReport::SetSpecificMallTimes(int &StartH, int &EndH, int &StartM, int &EndM)
{
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        StartH = 6;
        EndH = 6;
        StartM = 0;
        EndM = 0;
    }
    else
    {
        StartH = 5;
        EndH = 5;
        StartM = 0;
        EndM = 0;
    }
}
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------
// For Federal Land
UnicodeString TfrmMallExportRegenerateReport::GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                  UnicodeString &DateValue, UnicodeString &TimeValue,
                                                  int &TransactionTotal, UnicodeString &Amount, UnicodeString &MinuteValue,
                                                  int &SCDiscountCount, int &PatronCount)
{
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());

    query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM, SUM(SCDISCOUNT_COUNT) as SCCOUNT, SUM(PATRON_COUNT) as PATRON_COUNT "
                       "FROM ARCMALLEXPORTHOURLY WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME";
    DBTransaction.StartTransaction();
    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query->ParamByName("TERMINALNAME")->AsString = TerminalName;
    query->ExecQuery();

    AmountTotal = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    TransactionTotal = RoundToNearest(query->FieldByName("TRANSSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    SCDiscountCount = query->FieldByName("SCCOUNT")->AsInteger;
    PatronCount = query->FieldByName("PATRON_COUNT")->AsInteger;

    Amount = federallandExport->FixDecimalPlaces(AmountTotal);

    DBTransaction.Commit();
    query->Close();

    query1->SQL->Text = "SELECT MINUTE_VALUE FROM ARCMALLEXPORTHOURLY "
                       "WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME ORDER BY MINUTE_VALUE DESC";
    DBTransaction.StartTransaction();
    query1->ParamByName("STARTDATE")->AsDateTime = SDate;
    query1->ParamByName("ENDDATE")->AsDateTime = EDate;
    query1->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query1->ParamByName("TERMINALNAME")->AsString = TerminalName;
    query1->ExecQuery();

    MinuteValue = query1->FieldByName("MINUTE_VALUE")->AsString;

    DBTransaction.Commit();
    query1->Close();

    return result;
}
//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateFederalLandExport()
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";
    UnicodeString MallPath = edLocationPath->Text;
    UnicodeString MallPathFileName = "";
    UnicodeString DateVal = "";
    UnicodeString Query = "";

    // For Daily Sales
    federallandExport->CreateFilename("S", MallPath, MallPathFileName);
    federallandExport->CreateHeaderFormat(MallPathFileName,DataToWrite, "S");
    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
    DataToWrite.clear();

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *query1 = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *query2 = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *query3 = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *query4 = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *query5 = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *query6 = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();

    query->SQL->Text = "SELECT FIRST 1 OLD_GRANDTOTAL, BEGINNING_INVOICE, TERMINAL_NAME, TENANT_NAME, MALLCODE FROM ARCMALLEXPORT "
                       "WHERE TRANSACTIONDATE >= :STARTDATE AND TRANSACTIONDATE < :ENDDATE ORDER BY BEGINNING_INVOICE ASC";

    query->ParamByName("STARTDATE")->AsDateTime = SDate;
    query->ParamByName("ENDDATE")->AsDateTime = EDate;
    query->ExecQuery();

    if(!query->Eof)
    {
        OLD_GRANDTOTAL = query->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
        BEGINNING_INVOICE = query->FieldByName("BEGINNING_INVOICE")->AsString;
        TERMINAL_NAME = query->FieldByName("TERMINAL_NAME")->AsString;
        TENANT_NAME = query->FieldByName("TENANT_NAME")->AsString;
        MALLCODE = query->FieldByName("MALLCODE")->AsString;
    }

    query1->Close();

    query1->SQL->Text = "SELECT FIRST 1 NEW_GRANDTOTAL, ENDING_INVOICE FROM ARCMALLEXPORT "
                       "WHERE TRANSACTIONDATE >= :STARTDATE AND TRANSACTIONDATE < :ENDDATE ORDER BY BEGINNING_INVOICE DESC";

    query1->ParamByName("STARTDATE")->AsDateTime = SDate;
    query1->ParamByName("ENDDATE")->AsDateTime = EDate;
    query1->ExecQuery();

    if(!query1->Eof)
    {
        NEW_GRANDTOTAL = query1->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
        ENDING_INVOICE = query1->FieldByName("ENDING_INVOICE")->AsString;
    }

    query2->Close();

    query2->SQL->Text = "SELECT SUM(DAILY_SALES) AS DAILY_SALES, SUM(TOTALDISCOUNT) AS TOTALDISCOUNT, SUM(SCDISCOUNT_AMOUNT) AS SCDISCOUNT_AMOUNT, "
                        "SUM(PWDDISCOUNT_AMOUNT) PWDDISCOUNT_AMOUNT, SUM(GPCDISCOUNT_AMOUNT) AS GPCDISCOUNT_AMOUNT, SUM(VIPDISCOUNT_AMOUNT) AS VIPDISCOUNT_AMOUNT, "
                        "SUM(EMPDISCOUNT_AMOUNT) AS EMPDISCOUNT_AMOUNT, SUM(REGDISCOUNT_AMOUNT) AS REGDISCOUNT_AMOUNT, "
                        "SUM(DISCOUNTG1_AMOUNT) AS DISCOUNTG1_AMOUNT, SUM(REFUND_AMOUNT) AS REFUND_AMOUNT, SUM(VOID_AMOUNT) AS VOID_AMOUNT, "
                        "SUM(VAT_SALES) AS VAT_SALES, SUM(TAX_INCLUSIVE_SALES) AS TAX_INCLUSIVE_SALES, SUM(VATEXEMPT_SALES) AS VATEXEMPT_SALES, "
                        "SUM(TRANSACTION_COUNT) AS TRANSACTION_COUNT, SUM(FINEDINECUST_COUNT) AS FINEDINECUST_COUNT, "
                        "SUM(SCDISCOUNT_COUNT) AS SCDISCOUNT_COUNT, SUM(LOCALTAXES) AS LOCALTAXES, SUM(SCHARGE_AMOUNT) AS SCHARGE_AMOUNT, "
                        "SUM(NONVAT_SALES) AS NONVAT_SALES, SUM(GROSS_SALES) AS GROSS_SALES, SUM(LOCTAXEXEMPTDLY_SALES) AS LOCTAXEXEMPTDLY_SALES, "
                        "SUM(CASH_SALES) AS CASH_SALES, SUM(CARD_SALES) AS CARD_SALES, SUM(OTHER_SALES) AS OTHER_SALES FROM ARCMALLEXPORT "
                        "WHERE TRANSACTIONDATE >= :STARTDATE AND TRANSACTIONDATE < :ENDDATE";

    query2->ParamByName("STARTDATE")->AsDateTime = SDate;
    query2->ParamByName("ENDDATE")->AsDateTime = EDate;
    query2->ExecQuery();

    if(!query2->Eof)
    {
        DAILY_SALES = query2->FieldByName("DAILY_SALES")->AsCurrency;
        TOTALDISCOUNT = query2->FieldByName("TOTALDISCOUNT")->AsCurrency;
        SCDISCOUNT_AMOUNT = query2->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
        PWDDISCOUNT_AMOUNT = query2->FieldByName("PWDDISCOUNT_AMOUNT")->AsCurrency;
        GPCDISCOUNT_AMOUNT = query2->FieldByName("GPCDISCOUNT_AMOUNT")->AsCurrency;
        VIPDISCOUNT_AMOUNT = query2->FieldByName("VIPDISCOUNT_AMOUNT")->AsCurrency;
        EMPDISCOUNT_AMOUNT = query2->FieldByName("EMPDISCOUNT_AMOUNT")->AsCurrency;
        REGDISCOUNT_AMOUNT = query2->FieldByName("REGDISCOUNT_AMOUNT")->AsCurrency;
        DISCOUNTG1_AMOUNT = query2->FieldByName("DISCOUNTG1_AMOUNT")->AsCurrency;
        REFUND_AMOUNT = query2->FieldByName("REFUND_AMOUNT")->AsCurrency;
        VOID_AMOUNT = query2->FieldByName("VOID_AMOUNT")->AsCurrency;
        VAT_SALES = query2->FieldByName("VAT_SALES")->AsCurrency;
        TAX_INCLUSIVE_SALES = query2->FieldByName("TAX_INCLUSIVE_SALES")->AsCurrency;
        VATEXEMPT_SALES = query2->FieldByName("VATEXEMPT_SALES")->AsCurrency;
        TRANSACTION_COUNT = query2->FieldByName("TRANSACTION_COUNT")->AsInteger;
        FINEDINECUST_COUNT = query2->FieldByName("FINEDINECUST_COUNT")->AsInteger;
        SCDISCOUNT_COUNT = query2->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
        LOCALTAXES = query2->FieldByName("LOCALTAXES")->AsCurrency;
        SCHARGE_AMOUNT = query2->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
        NONVAT_SALES = query2->FieldByName("NONVAT_SALES")->AsCurrency;
        GROSS_SALES = query2->FieldByName("GROSS_SALES")->AsCurrency;
        LOCTAXEXEMPTDLY_SALES = query2->FieldByName("LOCTAXEXEMPTDLY_SALES")->AsCurrency;
        CASH_SALES = query2->FieldByName("CASH_SALES")->AsCurrency;
        CARD_SALES = query2->FieldByName("CARD_SALES")->AsCurrency;
        OTHER_SALES = query2->FieldByName("OTHER_SALES")->AsCurrency;
    }

    DateVal = SDate.FormatString("mm/dd/yyyy");

    OutputValue = DateVal + Format + TENANT_NAME + Format + MALLCODE + Format + "=\"" + federallandExport->FixDecimalPlaces(OLD_GRANDTOTAL) + "\"" + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(NEW_GRANDTOTAL) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(DAILY_SALES) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(TOTALDISCOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(SCDISCOUNT_AMOUNT) + "\"" + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(PWDDISCOUNT_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(GPCDISCOUNT_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(VIPDISCOUNT_AMOUNT) + "\"" + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(EMPDISCOUNT_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(REGDISCOUNT_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(DISCOUNTG1_AMOUNT) + "\"" + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(REFUND_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(VOID_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(VAT_SALES) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(TAX_INCLUSIVE_SALES) + "\"" + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(VATEXEMPT_SALES) + "\"" + Format + BEGINNING_INVOICE + Format + ENDING_INVOICE + Format +
                  TRANSACTION_COUNT + Format + FINEDINECUST_COUNT + Format + SCDISCOUNT_COUNT + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(LOCALTAXES) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(SCHARGE_AMOUNT) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(NONVAT_SALES) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(GROSS_SALES) + "\"" + Format +
                  "=\"" + federallandExport->FixDecimalPlaces(LOCTAXEXEMPTDLY_SALES) + "\"" + Format + TERMINAL_NAME + Format + "=\"" + federallandExport->FixDecimalPlaces(CASH_SALES) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(CARD_SALES) + "\"" + Format + "=\"" + federallandExport->FixDecimalPlaces(OTHER_SALES) + "\"" + "\n";

    DataToWrite.push_back(OutputValue.t_str());
    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
    DataToWrite.clear();

    // For Daily Sales Discount
    UnicodeString DISCOUNTTYPECODE = "";
    Currency DISCOUNTPERC = 0;
    Currency DISCOUNTAMOUNT = 0;

    federallandExport->CreateFilename("D", MallPath, MallPathFileName);
    federallandExport->CreateHeaderFormat(MallPathFileName,DataToWrite, "D");
    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
    DataToWrite.clear();

    query3->Close();

    query3->SQL->Text = "SELECT DATE_VALUE, TENANT_NAME, DISCOUNT_TYPE, DISCOUNT_PERC, DISCOUNT_AMOUNT, TRANSACTION_COUNT, "
                        "FINEDINECUST_COUNT, SCDISCOUNT_COUNT FROM ARCMALLEXPORTOTHERDETAILS "
                        "WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND TRANSACTION_CODE = 'DISC'";

    DBTransaction.StartTransaction();
    query3->ParamByName("STARTDATE")->AsDateTime = SDate;
    query3->ParamByName("ENDDATE")->AsDateTime = EDate;
    query3->ExecQuery();

    DateVal = SDate.FormatString("mm/dd/yyyy");

    while(!query3->Eof)
    {
        TENANT_NAME = query3->FieldByName("TENANT_NAME")->AsString;
        DISCOUNTTYPECODE = query3->FieldByName("DISCOUNT_TYPE")->AsString;
        DISCOUNTPERC = query3->FieldByName("DISCOUNT_PERC")->AsCurrency;
        DISCOUNTAMOUNT = query3->FieldByName("DISCOUNT_AMOUNT")->AsCurrency;
        TRANSACTION_COUNT = query3->FieldByName("TRANSACTION_COUNT")->AsInteger;
        FINEDINECUST_COUNT = query3->FieldByName("FINEDINECUST_COUNT")->AsInteger;
        SCDISCOUNT_COUNT = query3->FieldByName("SCDISCOUNT_COUNT")->AsInteger;

        OutputValue = DateVal + Format + TENANT_NAME + Format + DISCOUNTTYPECODE + Format +
                      DISCOUNTPERC + Format + "=\"" + federallandExport->FixDecimalPlaces(DISCOUNTAMOUNT) + "\"" + Format + TRANSACTION_COUNT + Format +
                      FINEDINECUST_COUNT + Format + SCDISCOUNT_COUNT + "\n";
        DataToWrite.push_back(OutputValue.t_str());
        query3->Next();
    }
    DBTransaction.Commit();

    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
    DataToWrite.clear();

    // For Daily Sales Payment
    UnicodeString PAYMENTCODE = "";
    UnicodeString PAYMENTCODEDESC = "";
    UnicodeString PAYMENTCLASS = "";
    UnicodeString PAYMENTCLASSDESC = "";
    Currency PAYMENTAMOUNT = 0;

    federallandExport->CreateFilename("P", MallPath, MallPathFileName);
    federallandExport->CreateHeaderFormat(MallPathFileName,DataToWrite, "P");
    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
    DataToWrite.clear();

    query4->Close();

    query4->SQL->Text = "SELECT DATE_VALUE, TENANT_NAME, PAYMENT_CODE, PAYMENT_CODE_DESC, "
                        "PAYMENT_CLASS, PAYMENT_CLASS_DESC, PAYMENT_AMOUNT FROM ARCMALLEXPORTOTHERDETAILS "
                        "WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND TRANSACTION_CODE = 'PAYMENT' AND REFUND_CANCEL_REASON = ''";

    DBTransaction.StartTransaction();
    query4->ParamByName("STARTDATE")->AsDateTime = SDate;
    query4->ParamByName("ENDDATE")->AsDateTime = EDate;
    query4->ExecQuery();

    DateVal = SDate.FormatString("mm/dd/yyyy");

    while(!query4->Eof)
    {
        TENANT_NAME = query4->FieldByName("TENANT_NAME")->AsString;
        PAYMENTCODE = query4->FieldByName("PAYMENT_CODE")->AsString;
        PAYMENTCODEDESC = query4->FieldByName("PAYMENT_CODE_DESC")->AsString;
        PAYMENTCLASS = query4->FieldByName("PAYMENT_CLASS")->AsString;
        PAYMENTCLASSDESC = query4->FieldByName("PAYMENT_CLASS_DESC")->AsString;
        PAYMENTAMOUNT = query4->FieldByName("PAYMENT_AMOUNT")->AsCurrency;

        OutputValue = DateVal + Format + TENANT_NAME + Format + PAYMENTCODE + Format +
                      PAYMENTCODEDESC + Format + PAYMENTCLASS + Format + PAYMENTCLASSDESC +
                      Format + "=\"" + federallandExport->FixDecimalPlaces(PAYMENTAMOUNT) + "\"" + "\n";
        DataToWrite.push_back(OutputValue.t_str());
        query4->Next();
    }
    DBTransaction.Commit();

    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
    DataToWrite.clear();

    // For Daily Sales Refund Cancel
    UnicodeString RCCODE = "";
    UnicodeString RCREASON = "";
    Currency RCAMOUNT = 0;

    federallandExport->CreateFilename("R", MallPath, MallPathFileName);
    federallandExport->CreateHeaderFormat(MallPathFileName,DataToWrite, "R");
    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
    DataToWrite.clear();

    query5->Close();

    query5->SQL->Text = "SELECT DATE_VALUE, TENANT_NAME, TRANSACTION_CODE, REFUND_CANCEL_REASON, "
                        "REFUND_CANCEL_AMOUNT, TRANSACTION_COUNT, FINEDINECUST_COUNT, SCDISCOUNT_COUNT "
                        "FROM ARCMALLEXPORTOTHERDETAILS WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND TRANSACTION_CODE = 'CNCLD' OR TRANSACTION_CODE = 'RFND'";

    DBTransaction.StartTransaction();
    query5->ParamByName("STARTDATE")->AsDateTime = SDate;
    query5->ParamByName("ENDDATE")->AsDateTime = EDate;
    query5->ExecQuery();

    DateVal = SDate.FormatString("mm/dd/yyyy");

    while(!query5->Eof)
    {
        TENANT_NAME = query5->FieldByName("TENANT_NAME")->AsString;
        RCCODE = query5->FieldByName("TRANSACTION_CODE")->AsString;
        RCREASON = query5->FieldByName("REFUND_CANCEL_REASON")->AsString;
        RCAMOUNT = query5->FieldByName("REFUND_CANCEL_AMOUNT")->AsCurrency;
        TRANSACTION_COUNT = query5->FieldByName("TRANSACTION_COUNT")->AsInteger;
        FINEDINECUST_COUNT = query5->FieldByName("FINEDINECUST_COUNT")->AsInteger;
        SCDISCOUNT_COUNT = query5->FieldByName("SCDISCOUNT_COUNT")->AsInteger;

        OutputValue = DateVal + Format + TENANT_NAME + Format + RCCODE + Format +
                      RCREASON + Format + "=\"" + federallandExport->FixDecimalPlaces(RCAMOUNT) + "\"" + Format + TRANSACTION_COUNT + Format +
                      FINEDINECUST_COUNT + Format + SCDISCOUNT_COUNT + "\n";
        DataToWrite.push_back(OutputValue.t_str());
        query5->Next();
    }
    DBTransaction.Commit();

    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
    DataToWrite.clear();

    // For Hourly Sales
    UnicodeString TERMINALNUM = TGlobalSettings::Instance().TerminalNo;
    UnicodeString DATEVALUE = "";
    UnicodeString TIMEVALUE = "";
    UnicodeString MINUTEVALUE = "";
    Currency AMOUNTVALUE = 0;
    int TransactionTotal = 0;
    UnicodeString Amount = "";
    UnicodeString TempVal = "";
    TENANT_NAME = "";
    OutputValue = "";

    std::map<UnicodeString, UnicodeString>::iterator it;

    federallandExport->CreateFilename("H", MallPath, MallPathFileName);
    federallandExport->CreateHeaderFormat(MallPathFileName,DataToWrite, "H");
    dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
    DataToWrite.clear();
    DataRead.clear();

    query6->Close();

    query6->SQL->Text = "SELECT * FROM ARCMALLEXPORTHOURLY WHERE DATE_VALUE >= :STARTDATE AND DATE_VALUE < :ENDDATE AND TERMINAL_NAME=:TERMINALNAME "
                        "ORDER BY DATE_VALUE, TIME_VALUE ASC";

    DBTransaction.StartTransaction();
    query6->ParamByName("STARTDATE")->AsDateTime = SDate;
    query6->ParamByName("ENDDATE")->AsDateTime = EDate;
    query6->ParamByName("TERMINALNAME")->AsString = TERMINALNUM;
    query6->ExecQuery();

    while(!query6->Eof)
    {
        TENANT_NAME = query6->FieldByName("MALLCODE")->AsString;
        DATEVALUE = SDate.FormatString("mm/dd/yyyy");
        TIMEVALUE = query6->FieldByName("TIME_VALUE")->AsString;
        AMOUNTVALUE = query6->FieldByName("AMOUNT_VALUE")->AsCurrency;
        MINUTEVALUE = query6->FieldByName("MINUTE_VALUE")->AsString;
        SCDISCOUNT_COUNT = query6->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
        FINEDINECUST_COUNT = query6->FieldByName("PATRON_COUNT")->AsInteger;

        DataRead[TIMEVALUE] = federallandExport->GetHourlyFormat(TERMINALNUM, TENANT_NAME, DATEVALUE, TIMEVALUE, FINEDINECUST_COUNT, MINUTEVALUE, SCDISCOUNT_COUNT);

        query6->Next();
    }
    DBTransaction.Commit();

    for(int i=0;i<24;i++)
    {
        UnicodeString Time = "";
        UnicodeString Data = "";
        TDateTime Tomorrow = Now() + 1.0;
        TDateTime Yesterday = Now() - 1.0;

        switch(i)
        {
            case 0:
                Time = "00";
                break;
            case 1:
                Time = "01";
                break;
            case 2:
                Time = "02";
                break;
            case 3:
                Time = "03";
                break;
            case 4:
                Time = "04";
                break;
            case 5:
                Time = "05";
                break;
            case 6:
                Time = "06";
                break;
            case 7:
                Time = "07";
                break;
            case 8:
                Time = "08";
                break;
            case 9:
                Time = "09";
                break;
            case 10:
                Time = "10";
                break;
            case 11:
                Time = "11";
                break;
            case 12:
                Time = "12";
                break;
            case 13:
                Time = "13";
                break;
            case 14:
                Time = "14";
                break;
            case 15:
                Time = "15";
                break;
            case 16:
                Time = "16";
                break;
            case 17:
                Time = "17";
                break;
            case 18:
                Time = "18";
                break;
            case 19:
                Time = "19";
                break;
            case 20:
                Time = "20";
                break;
            case 21:
                Time = "21";
                break;
            case 22:
                Time = "22";
                break;
            case 23:
                Time = "23";
                break;
            default:
                break;
        }

        if(query6->RecordCount == 0)
        {
            DataRead[Time] = federallandExport->GetHourlyFormat(TERMINALNUM, TENANT_NAME, DATEVALUE, TIMEVALUE, FINEDINECUST_COUNT, MINUTEVALUE, SCDISCOUNT_COUNT);
        }

        it = DataRead.find(Time);
        TempVal = (it->second);


        if(TENANT_NAME == "")
        {
            TENANT_NAME = TGlobalSettings::Instance().BranchCode;
        }

        GetHourlyData(TERMINALNUM, TENANT_NAME, DATEVALUE, Time, TransactionTotal, Amount, MINUTEVALUE, SCDISCOUNT_COUNT, FINEDINECUST_COUNT);

        for(int i=0;i<7;i++)
        {
            if(i==0)
            {
                TempVal = DATEVALUE + Format;
            }
            else if(i==1)
            {
                TempVal = TENANT_NAME + Format;
            }
            else if(i==2)
            {
                TempVal = Time + ":" + MINUTEVALUE + Format;
            }
            else if(i==3)
            {
                TempVal = "=\"" + Amount + "\"" + Format;
            }
            else if(i==4)
            {
                TempVal = IntToStr(TransactionTotal) + Format;
            }
            else if(i==5)
            {
                TempVal = IntToStr(FINEDINECUST_COUNT) + Format;
            }
            else
            {
                TempVal = IntToStr(SCDISCOUNT_COUNT) + "\n";
            }
            OutputValue += TempVal;
        }
        DataToWrite.push_back(OutputValue.t_str());
        dataManager->ExportFile(OutputManager, DataToWrite, MallPathFileName, "HTXTDATA");
        DataToWrite.clear();
        OutputValue = "";
    }

    DataToWrite.clear();

    MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );

    DataRead.clear();
    ResetMallExportValues();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportRegenerateReport::sbAllClick(TObject *Sender)
{
    isAllTerminalsSelected = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMallExportRegenerateReport::sbThisTerminalClick(TObject *Sender)
{
    isAllTerminalsSelected = false;
}
//-----------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::ShowDateTimes()
{
	lbFrom->Caption					= "From: " + mcStartDate->Date.FormatString("ddddd") + " at " + SDate.FormatString("HH:nn");
	lbTo->Caption					= "To: " + mcEndDate->Date.FormatString("ddddd") + " at " + EDate.FormatString("HH:nn");
}

//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::RegenerateMegaworldExport()
{

    UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
    UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
    GetListOfDatesBetwSdateEndDate(SDate,EDate);
    ResetMallExportValues();

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void TfrmMallExportRegenerateReport::GetListOfDatesBetwSdateEndDate(TDateTime Startdate,TDateTime EndDate)
{

   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    UnicodeString DateValue = "";
    TDateTime tempdatevalue;
     bool IsBreakConsolidated = false;
     ResetValues();
   UnicodeString Day_StartDate = Startdate.FormatString("dd");

   UnicodeString Month_StartDate = Startdate.FormatString("mm");


   UnicodeString Year_StartDate = Startdate.FormatString("yyyy");


   UnicodeString Day_EndDate = EndDate.FormatString("dd");


   UnicodeString Month_EndDate = EndDate.FormatString("mm");


   UnicodeString Year_EndDate = EndDate.FormatString("yyyy");

   query->Close();


          query->SQL->Text = "SELECT CAST(a.DATE_VALUE AS date)Date_value "
                             "FROM ARCMALLEXPORT a "
                             "WHERE extract(DAY from (a.DATE_VALUE)) >= :day_startday "
                             "AND extract(MONTH from (a.DATE_VALUE)) >= :month_startday "
                             "AND extract(YEAR from (a.DATE_VALUE)) >= :year_startday "
                             "And extract(DAY from (a.DATE_VALUE)) <= :day_endDay "
                             "AND extract(MONTH from (a.DATE_VALUE)) <= :month_endDay "
                             "AND extract(YEAR from (a.DATE_VALUE)) <= :year_endyear "
                             "GROUP BY 1 ";


           query->ParamByName("day_startday")->AsString = Day_StartDate;

           query->ParamByName("month_startday")->AsString =Month_StartDate;

           query->ParamByName("year_startday")->AsString =Year_StartDate;


           query->ParamByName("day_endDay")->AsString =Day_EndDate;

           query->ParamByName("month_endDay")->AsString =Month_EndDate;

           query->ParamByName("year_endyear")->AsString =Year_EndDate;


          query->ExecQuery();
          if(query->RecordCount == 0)
          {
            MessageBox( "Generating Export File for dates the POS was not used is not Allowed!", "Cannot Generate File", MB_OK );
            return;
          }

          for( ; !query->Eof; query->Next())
          {
            tempdatevalue = query->Fields[0]->AsDate;
            DateValue = tempdatevalue.FormatString("mm/dd/yyyy");
            GetTotalZedCorrespondingDate(SDate,EDate,DateValue,IsBreakConsolidated);
            ResetValues();
          }
           if(query->RecordCount != 0)
           {
             MessageBox( "Generation of file Successful", "Gernerating File", MB_OK );
           }
}

void TfrmMallExportRegenerateReport::GetTotalZedCorrespondingDate(TDateTime Startdate,TDateTime EndDate,UnicodeString Datevalue,bool &isBreakConsolidated)
{
    int Count = 0;
    int Zedkey ;

    SaveHourlyData.Time_Value = "";
    SaveHourlyData.Amount_Value = 0;
    SaveHourlyData.Transaction_Count = 0;
    SaveHourlyData.Patron_Count = 0;

    for(int i =0;i<24;i++)
    {

      HourlyDataStorage.push_back(SaveHourlyData) ;

    }
    std::vector<TMegaworldRegenerateHourlyData>::iterator itrHourlyData;
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString Day_StartDate = Startdate.FormatString("dd");

   UnicodeString Month_StartDate = Startdate.FormatString("mm");


   UnicodeString Year_StartDate = Startdate.FormatString("yyyy");


   UnicodeString Day_EndDate = EndDate.FormatString("dd");


   UnicodeString Month_EndDate = EndDate.FormatString("mm");


   UnicodeString Year_EndDate = EndDate.FormatString("yyyy");


   query->Close();


          query->SQL->Text = "SELECT a.Z_KEY "
                             "FROM ARCMALLEXPORT a "
                             "WHERE extract(DAY from (a.DATE_VALUE)) >= :day_startday "
                             "AND extract(MONTH from (a.DATE_VALUE)) >= :month_startday "
                             "AND extract(YEAR from (a.DATE_VALUE)) >= :year_startday "
                             "And extract(DAY from (a.DATE_VALUE)) <= :day_endDay "
                             "AND extract(MONTH from (a.DATE_VALUE)) <= :month_endDay "
                             "AND extract(YEAR from (a.DATE_VALUE)) <= :year_endyear "
                             "GROUP BY 1 ";


           query->ParamByName("day_startday")->AsString = Day_StartDate;

           query->ParamByName("month_startday")->AsString =Month_StartDate;

           query->ParamByName("year_startday")->AsString =Year_StartDate;


           query->ParamByName("day_endDay")->AsString =Day_EndDate;

           query->ParamByName("month_endDay")->AsString =Month_EndDate;

           query->ParamByName("year_endyear")->AsString =Year_EndDate;


          query->ExecQuery();

    for( ; !query->Eof; query->Next())
    {
        Zedkey = query->Fields[0]->AsInteger;
        CheckFirstSaleOfEachZed(Zedkey,Startdate,EndDate,Datevalue,isBreakConsolidated) ;

    }

}

void TfrmMallExportRegenerateReport::CheckFirstSaleOfEachZed(int Zedkey ,TDateTime Startdate,TDateTime EndDate, UnicodeString datevalue,bool isBreakConsolidated)
{
   UnicodeString MallPath = edLocationPath->Text;
   UnicodeString LocalPathFileName = "";
   UnicodeString LocalPath = "";
   UnicodeString MallPathFileName = "";
   UnicodeString FileName = "";
   UnicodeString DateForDailyFile = "";
   UnicodeString Format = "\n";
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
   UnicodeString StartDateFormat = Startdate.FormatString("mm/dd/yyyy");
   UnicodeString EndDateFormat = EndDate.FormatString("mm/dd/yyyy");
   UnicodeString QueryDate;
   TDateTime ValidateQueryDate;
   query->Close();
          query->SQL->Text = "SELECT FIRST 1 a.DATE_VALUE "
                             "FROM ARCMALLEXPORTHOURLY a "
                             "WHERE a.Z_KEY =:Zedkey "
                             "ORDER BY a.AME_HOURLY_KEY ";

          query->ParamByName("Zedkey")->AsInteger = Zedkey;

          query->ExecQuery();

         ValidateQueryDate = query->Fields[0]->AsDate;
         QueryDate = ValidateQueryDate.FormatString("mm/dd/yyyy");
         DateForDailyFile = ValidateQueryDate.FormatString("mmddyyyy");
      
        if(QueryDate == datevalue )
        {
          batchId = batchId+1;

          CreateFilename("H", MallPath, LocalPath, LocalPathFileName, MallPathFileName,ValidateQueryDate);
          PrepareDateForHourly( Zedkey, Startdate);
          TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);

          CreateFilename("S", MallPath, LocalPath, LocalPathFileName, MallPathFileName,ValidateQueryDate);
          PrepareDateForDaily( Zedkey, DateForDailyFile);
          TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);

          CreateFilename("D", MallPath, LocalPath, LocalPathFileName, MallPathFileName,ValidateQueryDate);
          PrepareDataForDiscount( Zedkey, DateForDailyFile);
          TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        }
        else
        {
           return;
        }

}

//---------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::PrepareDataForDiscount(int Zedkey,UnicodeString DateForFile)
{
        TMegaworldRegenerateDiscount Discounted_data;
        int MaxZedKey;
        int SecondMaxZedKey;
        TDateTime DateValueForSecondMaxZed ;
        UnicodeString DiscountName = "";
        UnicodeString DiscountDesc = "";
        Currency DiscountTotal = 0;
        UnicodeString discountname = "";
        UnicodeString discountdesc = "";
        Currency discounttotal = 0;
        UnicodeString Format = ",";
        Currency DiscountValue = 0;
        UnicodeString OutputValue = "";
        TExportResponse result;

        DataToWrite.clear();
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());


                   query->Close();

                   query->SQL->Text =  "SELECT A.NAME,A.DESCRIPTION,SUM(A.DISCOUNTED_VALUE) AS DISC_VAL "
                                        "FROM ARCORDERDISCOUNTS A inner join ARCHIVE B on "
                                        "A.ARCHIVE_KEY = B.ARCHIVE_KEY inner join ARCBILL C on "
                                        "C.ARCBILL_KEY = B.ARCBILL_KEY "
                                        "WHERE C.Z_KEY > :MIN_ZED And C.Z_KEY <= :MAX_ZED "
                                        "GROUP BY A.NAME,A.DESCRIPTION ";

               query->ParamByName("MIN_ZED")->AsInteger = Zedkey-1;
               query->ParamByName("MAX_ZED")->AsInteger = Zedkey;



                   query->ExecQuery();


                   while(!query->Eof)
                   {

                       DiscountName = query->FieldByName("NAME")->AsString;
                       DiscountDesc = query->FieldByName("DESCRIPTION")->AsString;
                       DiscountTotal = fabs(query->FieldByName("DISC_VAL")->AsCurrency);

                       Discounted_data.Discount_Name = query->FieldByName("NAME")->AsString;
                       Discounted_data.Description = query->FieldByName("DESCRIPTION")->AsString;
                       Discounted_data.Discount_Value = fabs(query->FieldByName("DISC_VAL")->AsCurrency);


                       if(AvailableDiscountName.size() == 0)
                       {

                         AvailableDiscountName.push_back(Discounted_data);
                       }
                       else
                       {
                         bool isAdded = false;
                          for (itrRegenerate = AvailableDiscountName.begin(); itrRegenerate!=AvailableDiscountName.end(); itrRegenerate++)
                          {
                             if(itrRegenerate->Discount_Name == DiscountName && itrRegenerate->Description == DiscountDesc)
                             {


                                itrRegenerate->Discount_Value += DiscountTotal;
                                DiscountTotal = itrRegenerate->Discount_Value;
                               isAdded = true;
                               break;


                             }
                          }
                          if(!isAdded)
                          {
                                 AvailableDiscountName.push_back(Discounted_data);
                          }


                       }
                       query->Next();

                     StoreAllValue[make_pair(DiscountName,DiscountDesc)] = DiscountTotal;

                   }

             for (itrmapRegenerate = StoreAllValue.begin(); itrmapRegenerate!=StoreAllValue.end(); itrmapRegenerate++)
             {
                DiscountName = (itrmapRegenerate->first).first;
                DiscountDesc = (itrmapRegenerate->first).second;
                DiscountTotal = itrmapRegenerate->second;

                OutputValue= DiscountName + Format + DiscountDesc + Format + DiscountTotal + "\n";
                DataToWrite.push_back(OutputValue.t_str());

             }
}
void TfrmMallExportRegenerateReport::PrepareDateForHourly(int Zedkey,TDateTime Startdate)
{
    UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
    UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
    UnicodeString DateValue = Startdate.FormatString("mmddyyyy");
    std::vector<TMegaworldRegenerateHourlyData>::iterator itrHourlyData;
    Currency AmountSum = 0;
    int TransactionTotal = 0;
    int TransactionSum = 0;
    int PatronCount = 0;
    int PatronSum = 0;
    UnicodeString Amount = "";
    UnicodeString TotalValue = "";
    UnicodeString OutputValue = "";
    TDateTime CurrentdateTime = Now();
    UnicodeString CurrentdateTimeFormat = CurrentdateTime.FormatString("mm/dd/yyyy");
    DataToWrite.clear();

    UnicodeString IndexOne ="01";
    UnicodeString IndexTwo ="02";
    UnicodeString IndexThree ="03";

    HourlyoneStageData.insert(pair <UnicodeString, UnicodeString> (IndexOne, TenantName));
    HourlyoneStageData.insert(pair <UnicodeString, UnicodeString> (IndexTwo, TerminalNum));
    HourlyoneStageData.insert(pair <UnicodeString, UnicodeString> (IndexThree, DateValue));

    for (itrHourlyoneStageData = HourlyoneStageData.begin(); itrHourlyoneStageData != HourlyoneStageData.end(); ++itrHourlyoneStageData)
    {
        OutputValue = itrHourlyoneStageData->first  + itrHourlyoneStageData->second + "\n";
        DataToWrite.push_back(OutputValue.t_str());

    }
        for(int i=0;i<24;i++)
        {
            UnicodeString Time = i;
            Time = (Time.Length() == 1) ? "0" + Time : Time;

            GetHourlyDataMegaWorld(TerminalNum, TenantName, DateValue, Time, TransactionTotal, Amount, PatronCount,Zedkey);

            if(TransactionTotal == 0)
            {

                continue;
            }
        }
             for (itrHourlyData = HourlyDataStorage.begin(); itrHourlyData!=HourlyDataStorage.end(); itrHourlyData++)
             {

                UnicodeString TempAmount = "";

                TempAmount = RemoveDecimalValue(Amount);


                if(itrHourlyData->Time_Value == "")
                {
                     continue;
                }

                else
                {
                OutputValue = "04" + itrHourlyData->Time_Value + "\n" +
                              "05" + itrHourlyData->Amount_Value + "\n" +
                              "06" + itrHourlyData->Transaction_Count + "\n" +
                              "07" + itrHourlyData->Patron_Count + "\n";

                DataToWrite.push_back(OutputValue.t_str());
               }
                AmountSum += itrHourlyData->Amount_Value;
                TransactionSum += itrHourlyData->Transaction_Count;
                PatronSum += itrHourlyData->Patron_Count;
             }

        TotalValue = AmountSum;
        OutputValue = "08" + TotalValue + "\n" +
                      "09" + TransactionSum + "\n" +
                      "10" + PatronSum;
        DataToWrite.push_back(OutputValue.t_str());
}
//---------------------------------------------------------------------------
void TfrmMallExportRegenerateReport::PrepareDateForDaily(int Zedkey,UnicodeString DateForFile)
{
    UnicodeString Format = "\n";
    UnicodeString SalesTypeID = "";
    UnicodeString TenantID = "";
    UnicodeString FOOD = "SalesTypeFood";
    UnicodeString NONFOOD = "SalesTypeNon-Food";
    UnicodeString GROCERIES = "SalesTypeGroceries";
    UnicodeString MEDICINES = "SalesTypeMedicines";
    UnicodeString OTHERS = "SalesTypeOthers";
    UnicodeString tempTenant = "";
    UnicodeString tempTerminal = "";
    std::string TenantNameSTR;
    TExportResponse result;
    std::vector<UnicodeString> AvailableSalesType;
    DataToWrite.clear();

        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query0 = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());


        UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
        UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;

        OutputValue = "01" + TenantName + "\n" +
                      "02" + TerminalNum + "\n" +
                      "03" + DateForFile + "\n";
        DataToWrite.push_back(OutputValue.t_str());
               query->Close();

               query->SQL->Text = "SELECT  *  FROM ARCMALLEXPORT A "
                                  "WHERE A.Z_KEY > :MIN_ZED And A.Z_KEY <= :MAX_ZED ";

               query->ParamByName("MIN_ZED")->AsInteger = Zedkey-1;
               query->ParamByName("MAX_ZED")->AsInteger = Zedkey;

               query->ExecQuery();

            if(!query->Eof)
            {

                OLD_GRANDTOTAL = query->FieldByName("OLD_GRANDTOTAL")->AsCurrency;
                oldgrandtotal =OLD_GRANDTOTAL;
                OLD_GRANDTOTAL = oldgrandtotal;

                NEW_GRANDTOTAL = query->FieldByName("NEW_GRANDTOTAL")->AsCurrency;
                Newgrandtotal =NEW_GRANDTOTAL;
                NEW_GRANDTOTAL = Newgrandtotal;


                GROSS_SALES = query->FieldByName("GROSS_SALES")->AsCurrency;
                oldgrossSale +=GROSS_SALES;
                GROSS_SALES = oldgrossSale;


                VATEXEMPT_SALES = query->FieldByName("VATEXEMPT_SALES")->AsCurrency;
                vatexemptSales +=VATEXEMPT_SALES;
                VATEXEMPT_SALES = vatexemptSales;

                SCDISCOUNT_AMOUNT = query->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
                ScdAmount +=SCDISCOUNT_AMOUNT;
                SCDISCOUNT_AMOUNT = ScdAmount;

                REGDISCOUNT_AMOUNT = query->FieldByName("REGDISCOUNT_AMOUNT")->AsCurrency;
                RegdiscountAmount +=REGDISCOUNT_AMOUNT;
                REGDISCOUNT_AMOUNT = RegdiscountAmount;

                REFUND_AMOUNT = query->FieldByName("REFUND_AMOUNT")->AsCurrency;
                RefundAmount +=REFUND_AMOUNT;
                REFUND_AMOUNT = RefundAmount;

                VAT_SALES = query->FieldByName("VAT_SALES")->AsCurrency;
                VatSales +=VAT_SALES;
                VAT_SALES = VatSales;

                SCHARGE_AMOUNT = query->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
                SchargeAmount +=SCHARGE_AMOUNT;
                SCHARGE_AMOUNT = SchargeAmount;

                DailySales = query->FieldByName("DAILY_SALES")->AsCurrency;
                DailySales +=DAILY_SALES;
                DAILY_SALES = DailySales;

                CASH_SALES = query->FieldByName("CASH_SALES")->AsCurrency;
                CashSales +=CASH_SALES;
                CASH_SALES = CashSales;


                CARD_SALES = query->FieldByName("CARD_SALES")->AsCurrency;
                CardSales +=CARD_SALES;
                CARD_SALES = CardSales;

                OTHER_SALES = query->FieldByName("OTHER_SALES")->AsCurrency;
                OtherSales +=OTHER_SALES;
                OTHER_SALES = OtherSales;


                VOID_AMOUNT = query->FieldByName("VOID_AMOUNT")->AsCurrency;
                VoidAmount +=VOID_AMOUNT;
                VOID_AMOUNT = VoidAmount;

                FineDineCustCount = query->FieldByName("FINEDINECUST_COUNT")->AsInteger;
                FineDineCustCount +=FINEDINECUST_COUNT;
                FINEDINECUST_COUNT = FineDineCustCount;


                CURRENTEODCOUNTER = query->FieldByName("CURRENTEODCOUNTER")->AsInteger;
                EodCounter +=CURRENTEODCOUNTER;
                CURRENTEODCOUNTER = EodCounter;


                TRANSACTION_COUNT = query->FieldByName("TRANSACTION_COUNT")->AsInteger;
                TransactionCount +=TRANSACTION_COUNT;
                TRANSACTION_COUNT = TransactionCount;



                SALESTYPE_FOOD = query->FieldByName("SALESTYPE_FOOD")->AsCurrency;
                salestype_food +=SALESTYPE_FOOD;
                SALESTYPE_FOOD = salestype_food;


                SALESTYPE_NONFOOD = query->FieldByName("SALESTYPE_NONFOOD")->AsCurrency;
                salestype_Nonfood +=SALESTYPE_NONFOOD;
                SALESTYPE_NONFOOD = salestype_Nonfood;


                SALESTYPE_GROCERIES = query->FieldByName("SALESTYPE_GROCERIES")->AsCurrency;
                salestype_Groceries +=SALESTYPE_GROCERIES;
                SALESTYPE_GROCERIES = salestype_Groceries;

                SALESTYPE_MEDICINES = query->FieldByName("SALESTYPE_MEDICINES")->AsCurrency;
                salestype_Medicines +=SALESTYPE_MEDICINES;
                SALESTYPE_MEDICINES = salestype_Medicines;

                SALESTYPE_OTHERS = query->FieldByName("SALESTYPE_OTHERS")->AsCurrency;
                salestype_others +=SALESTYPE_OTHERS;
                SALESTYPE_OTHERS = salestype_others;

               }

              OutputValue = "04" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(OLD_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "05" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(NEW_GRANDTOTAL, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "06" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(GROSS_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "07" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(VATEXEMPT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());

              OutputValue = "08" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SCDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "09" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(REGDISCOUNT_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "10" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(REFUND_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "11" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(VAT_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "12" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SCHARGE_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "13" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(DAILY_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "14" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(CASH_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "15" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(CARD_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "16" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(OTHER_SALES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "17" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(VOID_AMOUNT, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "18" + IntToStr(FINEDINECUST_COUNT) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "19" + IntToStr(CURRENTEODCOUNTER) + Format;
              DataToWrite.push_back(OutputValue.t_str());


              OutputValue = "20" + IntToStr(TRANSACTION_COUNT) + Format;
              DataToWrite.push_back(OutputValue.t_str());


               query0->Close();
               query0->SQL->Text = "SELECT STIR.SALES_TYPE_KEY FROM SALES_TYPE_ITEMS_RELATION STIR "
                                    "GROUP BY STIR.SALES_TYPE_KEY";
               query0->ExecQuery();

                while(!query0->Eof)
                {
                    int SalesTypeDB = query0->FieldByName("SALES_TYPE_KEY")->AsInteger;
                    query1->Close();
                    query1->SQL->Text = "SELECT ST.SALES_TYPE_NAME FROM SALES_TYPE ST WHERE ST.SALES_TYPE_KEY = :SALES_KEY";
                    query1->ParamByName("SALES_KEY")->AsInteger = SalesTypeDB;
                    query1->ExecQuery();
                    UnicodeString SalesTypeValue = "SalesType" + query1->FieldByName("SALES_TYPE_NAME")->AsString;
                    AvailableSalesType.push_back(SalesTypeValue.t_str());
                    query0->Next();
                }
                DBTransaction.Commit();

                for(int j=0; j<AvailableSalesType.size();j++)
                {
                    if(AvailableSalesType.at(j) == FOOD)
                    {
                        OutputValue = "2101" + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                        OutputValue = "22" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SALESTYPE_FOOD, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2));
                        OutputValue = (AvailableSalesType.size() == (j+1)) ? OutputValue : OutputValue + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                    }
                    else if(AvailableSalesType.at(j) == NONFOOD)
                    {
                        OutputValue = "2102" + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                        OutputValue = "22" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SALESTYPE_NONFOOD, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2));
                        OutputValue = (AvailableSalesType.size() == (j+1)) ? OutputValue : OutputValue + Format;
                        DataToWrite.push_back(OutputValue.t_str());

                    }
                    else if(AvailableSalesType.at(j) == GROCERIES)
                    {
                        OutputValue = "2103" + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                        OutputValue = "22" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SALESTYPE_GROCERIES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2));
                        OutputValue = (AvailableSalesType.size() == (j+1)) ? OutputValue : OutputValue + Format;
                        DataToWrite.push_back(OutputValue.t_str());

                    }
                    else if(AvailableSalesType.at(j) == MEDICINES)
                    {
                        OutputValue = "2104" + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                        OutputValue = "22" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SALESTYPE_MEDICINES, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2));
                        OutputValue = (AvailableSalesType.size() == (j+1)) ? OutputValue : OutputValue + Format;
                        DataToWrite.push_back(OutputValue.t_str());

                    }
                    else if(AvailableSalesType.at(j) == OTHERS)
                    {
                        OutputValue = "2105" + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                        OutputValue = "22" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SALESTYPE_OTHERS, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2));
                        OutputValue = (AvailableSalesType.size() == (j+1)) ? OutputValue : OutputValue + Format;
                        DataToWrite.push_back(OutputValue.t_str());

                    }
                }
}

int TfrmMallExportRegenerateReport::GetHourlyDataMegaWorld(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                      UnicodeString &DateValue, UnicodeString &TimeValue,
                                                      int &TransactionTotal, UnicodeString &Amount, int &PatronCount,int Zedkey)
{

    TDateTime Currentime = Now();
    UnicodeString PresentDateTime = Currentime.FormatString("mmddyyyy");
    bool BreakConSolidateDateForCurrentDate = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    TDateTime DateValueForSecondMaxZed ;

    query->Close();
    query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM, SUM(PATRON_COUNT) AS PATRONSUM FROM ARCMALLEXPORTHOURLY A "
                       "WHERE A.TIME_VALUE=:TIMEVALUE AND A.Z_KEY > :MIN_ZED And A.Z_KEY <= :MAX_ZED ";

    query->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query->ParamByName("MIN_ZED")->AsInteger = Zedkey-1;
    query->ParamByName("MAX_ZED")->AsInteger = Zedkey;

    query->ExecQuery();

    TransactionTotal = query->FieldByName("TRANSSUM")->AsInteger;

    if(TransactionTotal == 0)
    {
      return 0;
    }
    else
    {
        int index = atoi(TimeValue.t_str());
        TransactionTotal = query->FieldByName("TRANSSUM")->AsInteger;
        PatronCount = query->FieldByName("PATRONSUM")->AsInteger;
        Amount = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
        Amount = FixDecimalPlaces(Amount);
        Amount = RemoveDecimalValue(Amount);
        HourlyDataStorage[index].Time_Value = TimeValue;
        HourlyDataStorage[index].Amount_Value += Amount;
        HourlyDataStorage[index].Transaction_Count += TransactionTotal;
        HourlyDataStorage[index].Patron_Count += PatronCount;

    }

    DBTransaction.Commit();
    query->Close();

    return TransactionTotal;
}

void TfrmMallExportRegenerateReport::CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                                         UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName,TDateTime &DateValue)
{

        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString TenantID = "";
        UnicodeString MonthValue = "";
        UnicodeString DayValue = "";
        GetExportInfo(MallPath, TenantID, TerminalNo, MonthValue, DayValue, DateValue);
        MallPath = CheckDir(MallPath);
        MallPathFileName = MallPath + mode + TenantID + TerminalNo + batchId + "." + MonthValue + DayValue;

}

void TfrmMallExportRegenerateReport::GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                                        UnicodeString &TerminalNo,
                                                        UnicodeString &MonthValue, UnicodeString &DayValue,
                                                        TDateTime DateValue)
{
        UnicodeString TenantNo = TGlobalSettings::Instance().TenantNo;
        UnicodeString CheckMonth = DateValue.FormatString("mm");
        UnicodeString CheckDay = DateValue.FormatString("dd");

        int Month = StrToInt(CheckMonth);
        std::string TenantNameSTR = TenantNo.t_str();

        Month = StrToInt(CheckMonth);

        // TenantName format is the first 4 Character of the Tenant Name
        for(int i=0;i<TenantNo.Length();i++)
        {
            if(i<4)
            {
                TenantID = TenantID + TenantNameSTR[i];
            }
        }

        // Teminal number must be at least 2 digit
        if (TerminalNo.Length() == 1)
        {
            TerminalNo = "0" + TerminalNo;
        }

        // Month value must be 1-9 and A-C(10,11,12)
        for(int i=0;i<12;i++)
        {
            if(Month == 10)
            {
                MonthValue = "A";
                break;
            }
            else if(Month == 11)
            {
                MonthValue = "B";
                break;
            }
            else if(Month == 12)
            {
                MonthValue = "C";
                break;
            }
            else
            {
                MonthValue = IntToStr(Month);
            }
        }

        // Day should have at least 2 digit in format
        DayValue = (CheckDay.Length() == 1) ? "0" + CheckDay : CheckDay;
}
UnicodeString TfrmMallExportRegenerateReport::RemoveDecimalValue(UnicodeString amountValue)
{
    UnicodeString result = "";
    std::string DataAmountSTR;
    DataAmountSTR = amountValue.t_str();

    for(int i=0;i<amountValue.Length();i++)
    {
        if(DataAmountSTR[i] != '.')
        {
            result = result + DataAmountSTR[i];
        }
    }
    return result;
}
UnicodeString TfrmMallExportRegenerateReport::FixDecimalPlaces(Currency AmountTotal)
{
    UnicodeString result = "";
    UnicodeString GetData = AmountTotal;
    std::string TempData = "";
    std::string value;
    std::string adecimal;
    bool decimal = false;

    TempData = GetData.t_str();

    for(int i=0;i<TempData.length();i++)
    {
        if((TempData.at(i) != '.') && (!decimal))
        {
            value = value + TempData.at(i);
        }
        else
        {
            decimal = true;
            if(TempData.at(i) != '.')
            {
                adecimal = adecimal + TempData.at(i);
            }
        }
    }

    if(adecimal.length() == 1)
    {
        adecimal = adecimal + "0";
    }
    GetData = value.c_str();

    if(!decimal)
    {
        result = GetData + ".00";
    }
    else
    {
        result = GetData + "." + adecimal.c_str();
    }

    return result;
}
UnicodeString TfrmMallExportRegenerateReport::CheckDir(UnicodeString Path)
{
    if(!DirectoryExists(Path))
    {
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
    }
    return Path;
}

void TfrmMallExportRegenerateReport::ResetValues()
{

    oldgrandtotal = 0;
    Newgrandtotal = 0;
    oldgrossSale = 0;
    vatexemptSales=0;
    ScdAmount = 0;
    RegdiscountAmount = 0;
    RefundAmount = 0;
    VatSales = 0;
    SchargeAmount = 0;
    DailySales = 0;
    CashSales = 0;
    CardSales = 0;
    OtherSales = 0;
    VoidAmount = 0;
    salestype_food = 0;
    salestype_Nonfood = 0;
    salestype_Groceries = 0;
    salestype_Medicines = 0;
    salestype_others = 0;

    FineDineCustCount = 0;
    EodCounter = 0;
    TransactionCount = 0;
    batchId =0;

    AmountValue = 0 ;
    transactioncount=0;
    Patroncountvalue=0;
    StoreAllValue.clear();
    AvailableDiscountName.clear();
    HourlyDataStorage.clear();
}


