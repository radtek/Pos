#include "XBreakdownCategoriesDetailsReportSection.h"
#include "GlobalSettings.h"



XBreakdownCategoriesDetailsReportSection::XBreakdownCategoriesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmBreakdownCategoriesDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XBreakdownCategoriesDetailsReportSection::XBreakdownCategoriesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmXReport, mmBreakdownCategoriesDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XBreakdownCategoriesDetailsReportSection::~XBreakdownCategoriesDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XBreakdownCategoriesDetailsReportSection::GetOutput(TPrintout* printout)
{
    AddTitle(printout, "Breakdown Categories");
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    TIBSQL *qrBreakdownCategories = _dbTransaction->Query(_dbTransaction->AddQuery());
    TStringList *CatList = new TStringList;
    qrBreakdownCategories->Close();
    qrBreakdownCategories->SQL->Text = "SELECT " " COUNT (DAYARCCATEGORY.CATEGORY_KEY)," " CATEGORY,"
    " SUM ((PRICE * QTY)) TOTAL," " SUM (QTY) TOTALQTY" " FROM" " DAYARCCATEGORY"
    " INNER JOIN ARCCATEGORIES ON DAYARCCATEGORY.CATEGORY_KEY = ARCCATEGORIES.CATEGORY_KEY"
    " INNER JOIN DAYARCHIVE ON DAYARCCATEGORY.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY WHERE"
    " ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder)
    + ") ";
    if(!_globalSettings->EnableDepositBagNum)
    {
        qrBreakdownCategories->SQL->Text = qrBreakdownCategories->SQL->Text +
        " AND TERMINAL_NAME = :TERMINAL_NAME GROUP BY CATEGORY;";
        qrBreakdownCategories->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    }
    else
    {
        qrBreakdownCategories->SQL->Text = qrBreakdownCategories->SQL->Text +
        " GROUP BY CATEGORY;";
    }
    qrBreakdownCategories->ExecQuery();

    for (; !qrBreakdownCategories->Eof; qrBreakdownCategories->Next())
    {
        CatList->Add(qrBreakdownCategories->FieldByName("CATEGORY")->AsString + "|" + qrBreakdownCategories->FieldByName("TOTALQTY")
        ->AsString + " | " + dataFormatUtilities->FormatMMReportCurrency( qrBreakdownCategories->FieldByName("TOTAL")->AsCurrency ) );
    }

    if (CatList->Count > 0)
		{
		      //	AddSubSectionTitle(printout.get(), "Billed Sales Totals");
			printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			printout->PrintFormat->Line->ColCount = 3;
			printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width / 2;
			printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 6;
			printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
			printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width - (printout->PrintFormat->Width * 2 / 3);
			printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
			Currency TotalCat = 0;
			for (int i = 0; i < CatList->Count; i++)
			{
				printout->PrintFormat->Add(CatList->Strings[i]);
			}
            IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
            if(reportSectionDisplayTraits)
            {
            reportSectionDisplayTraits->ApplyTraits(printout);
            }
			printout->PrintFormat->NewLine();
			printout->PrintFormat->NewLine();
		}
}

void XBreakdownCategoriesDetailsReportSection::GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime)
{
    AddTitle(printout, "Breakdown Categories");
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    TIBSQL *qrBreakdownCategories = _dbTransaction->Query(_dbTransaction->AddQuery());
    TStringList *CatList = new TStringList;
    qrBreakdownCategories->Close();
    qrBreakdownCategories->SQL->Text = "SELECT " " COUNT (DAYARCCATEGORY.CATEGORY_KEY)," " CATEGORY,"
    " SUM ((PRICE * QTY)) TOTAL," " SUM (QTY) TOTALQTY" " FROM" " DAYARCCATEGORY"
    " INNER JOIN ARCCATEGORIES ON DAYARCCATEGORY.CATEGORY_KEY = ARCCATEGORIES.CATEGORY_KEY"
    " INNER JOIN DAYARCHIVE ON DAYARCCATEGORY.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY WHERE"
    " ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder)
    + ") ";
    if(!_globalSettings->EnableDepositBagNum)
    {
        qrBreakdownCategories->SQL->Text = qrBreakdownCategories->SQL->Text +
        " AND TERMINAL_NAME = :TERMINAL_NAME GROUP BY CATEGORY;";
        qrBreakdownCategories->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    }
    else
    {
        qrBreakdownCategories->SQL->Text = qrBreakdownCategories->SQL->Text +
        " GROUP BY CATEGORY;";
    }
    qrBreakdownCategories->ExecQuery();

    for (; !qrBreakdownCategories->Eof; qrBreakdownCategories->Next())
    {
        CatList->Add(qrBreakdownCategories->FieldByName("CATEGORY")->AsString + "|" + qrBreakdownCategories->FieldByName("TOTALQTY")
        ->AsString + " | " + dataFormatUtilities->FormatMMReportCurrency( qrBreakdownCategories->FieldByName("TOTAL")->AsCurrency ) );
    }

    if (CatList->Count > 0)
		{
		      //	AddSubSectionTitle(printout.get(), "Billed Sales Totals");
			printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			printout->PrintFormat->Line->ColCount = 3;
			printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width / 2;
			printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 6;
			printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
			printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width - (printout->PrintFormat->Width * 2 / 3);
			printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
			Currency TotalCat = 0;
			for (int i = 0; i < CatList->Count; i++)
			{
				printout->PrintFormat->Add(CatList->Strings[i]);
			}
            IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
            if(reportSectionDisplayTraits)
            {
            reportSectionDisplayTraits->ApplyTraits(printout);
            }
			printout->PrintFormat->NewLine();
			printout->PrintFormat->NewLine();
		}
}
