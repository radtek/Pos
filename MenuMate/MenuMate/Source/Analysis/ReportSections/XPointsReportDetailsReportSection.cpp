#include "XPointsReportDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"

XPointsReportDetailsReportSection::XPointsReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmPointsReportDetailsSection, dbTransaction, globalSettings)
{
    _dataFormatUtilities = new DataFormatUtilities;
}


XPointsReportDetailsReportSection::~XPointsReportDetailsReportSection()
{
    delete _dataFormatUtilities;
    delete _memberShip;
}

void XPointsReportDetailsReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
        return;
    AddTitle(printOut, "Points Report");
	printOut->PrintFormat->NewLine();

    IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

    if (reportSectionDisplayStrategy)
	{
		//Call the strategy to build the section..
		reportSectionDisplayStrategy->BuildSection(printOut);
	}

    GetGiftCardOutput(printOut);
    GetFirstVisitOutput(printOut);
    GetBirthdayOutput(printOut);
}

void XPointsReportDetailsReportSection::GetGiftCardOutput(TPrintout* printOut)
{
     AnsiString masterSlaveCondition = "";
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    _memberShip = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();

	if (!_globalSettings->EnableDepositBagNum)
	{
		masterSlaveCondition = " AND  DAB.TERMINAL_NAME = :TERMINAL_NAME ";
	}

	TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
	ibInternalQuery->Close();
   /*	ibInternalQuery->SQL->Text = "SELECT "
                                        "COALESCE(SUM(PTrans.REDEEMED),0) REDEEMED, "
                                        "COALESCE( SUM(PTrans.PURCHASED),0) PURCHASED, "
                                        "PTrans.CONTACTS_KEY AS LOYALTY_KEY, "
                                        "CT.Name "
                                    "FROM DAYARCBILL DAB "
                                    "LEFT JOIN (SELECT "
                                                    "INVOICE_NUMBER, "
                                                    "POINTSTRANSACTIONS_KEY, "
                                                    "CONTACTS_KEY, "
                                                    "ADJUSTMENT_TYPE, "
                                                    "MIN(CASE WHEN (ADJUSTMENT_TYPE = 3 OR ADJUSTMENT_TYPE = 8 OR ADJUSTMENT_TYPE = 9) THEN ABS(COALESCE(ADJUSTMENT,0)) END) AS REDEEMED, "
                                                    "MIN(CASE WHEN ADJUSTMENT_TYPE = 1 THEN  COALESCE( ADJUSTMENT,0) END) AS PURCHASED "
                                                "FROM POINTSTRANSACTIONS "
                                                "GROUP BY POINTSTRANSACTIONS_KEY,INVOICE_NUMBER, CONTACTS_KEY,ADJUSTMENT_TYPE) PTrans on PTrans.INVOICE_NUMBER = DAB.INVOICE_NUMBER "
                                    "LEFT JOIN CONTACTS CT ON PTrans.CONTACTS_KEY = CT.CONTACTS_KEY "
                                    "WHERE CT.MEMBER_TYPE = 2 " + masterSlaveCondition +
                                    "GROUP BY PTrans.CONTACTS_KEY, CT.Name "
                                    "ORDER BY LOYALTY_KEY;";     */

  	ibInternalQuery->SQL->Text = 	"SELECT "
                                            "COALESCE(SUM(PTrans.REDEEMED),0) REDEEMED, "
                                            "COALESCE( SUM(PTrans.PURCHASED),0) PURCHASED, "
                                            "PTrans.CONTACTS_KEY AS LOYALTY_KEY,  "
                                            "PTrans.Name  "
                                        "FROM DAYARCBILL DAB  "
                                        "INNER JOIN (SELECT "
                                                "INVOICE_NUMBER,  "
                                                "POINTSTRANSACTIONS_KEY,  "
                                                "POINTSTRANSACTIONS.CONTACTS_KEY, "
                                                "ADJUSTMENT_TYPE, "
                                                "MIN(CASE WHEN (ADJUSTMENT_TYPE = 3 OR ADJUSTMENT_TYPE = 8 OR ADJUSTMENT_TYPE = 9) THEN ABS(COALESCE(ADJUSTMENT,0)) END) AS REDEEMED, "
                                                "MIN(CASE WHEN ADJUSTMENT_TYPE = 1 THEN  COALESCE( ADJUSTMENT,0) END) AS PURCHASED , "
                                                "C.NAME  "
                                        "FROM POINTSTRANSACTIONS   "
                                        "INNER JOIN ( SELECT CT.CONTACTS_KEY,CT.NAME  FROM CONTACTS CT WHERE CT.MEMBER_TYPE=2) C ON C.CONTACTS_KEY=POINTSTRANSACTIONS.CONTACTS_KEY "
                                                    "GROUP BY POINTSTRANSACTIONS_KEY,INVOICE_NUMBER, CONTACTS_KEY,ADJUSTMENT_TYPE,  C.NAME  "
                                                    ") PTrans on PTrans.INVOICE_NUMBER = DAB.INVOICE_NUMBER   "

                                        "WHERE DAB.TIME_STAMP >= (select coalesce(max(b.Time_Stamp),'28.12.1889, 19:39:25.000') from ZEDS b where b.TERMINAL_NAME = :ZTERMINAL_NAME)  "
                                         + masterSlaveCondition +
                                                "GROUP BY PTrans.CONTACTS_KEY, PTrans.Name  "
                                                "ORDER BY LOYALTY_KEY " ;

     ibInternalQuery->ParamByName("ZTERMINAL_NAME")->AsString = deviceName;
  	if (!_globalSettings->EnableDepositBagNum)
	{
		ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
	}
	ibInternalQuery->ExecQuery();

	if (ibInternalQuery->RecordCount)
	{
        AddSubTitle(printOut, "Gift Cards Points");

        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->AddLine();

	IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
        if(reportSectionDisplayTrait)
        {
            reportSectionDisplayTrait->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 3;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
        printOut->PrintFormat->Line->Columns[0]->Text = "Name";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
        printOut->PrintFormat->Line->Columns[1]->Text = "Sold";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width / 3;
        printOut->PrintFormat->Line->Columns[2]->Text = "Redeemed";
        printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->AddLine();

        Currency totalEarnedPoints    = 0;
        Currency totalRedeemedPoints = 0;

        for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
        {
                int loyaltyKey = ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;

                Currency earnedPoints    = ibInternalQuery->FieldByName("PURCHASED")->AsCurrency;
                Currency redeemedPoints = ibInternalQuery->FieldByName("REDEEMED")->AsCurrency;

                printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
                printOut->PrintFormat->Line->ColCount = 3;
                printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
                printOut->PrintFormat->Line->Columns[0]->Text = _memberShip->GetContactName(*_dbTransaction, loyaltyKey);
                printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
                printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(earnedPoints);
                printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
                printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width / 3;
                printOut->PrintFormat->Line->Columns[2]->Text = _dataFormatUtilities->FormatMMReportCurrency(redeemedPoints);
                printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
                printOut->PrintFormat->AddLine();

                totalEarnedPoints 	+= earnedPoints;
                totalRedeemedPoints += redeemedPoints;
        }

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->DoubleLine();
        printOut->PrintFormat->Line->Columns[2]->DoubleLine();
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Totals";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(totalEarnedPoints);
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printOut->PrintFormat->Line->Columns[2]->Text = _dataFormatUtilities->FormatMMReportCurrency(totalRedeemedPoints);
        printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();
	}
}

void XPointsReportDetailsReportSection::GetFirstVisitOutput(TPrintout* printOut)
{
    GetOutputForAdjustmentType(6, "First Visit Points", printOut);
}

void XPointsReportDetailsReportSection::GetBirthdayOutput(TPrintout* printOut)
{
    GetOutputForAdjustmentType(5, "Birthday Reward Points", printOut);
}

void XPointsReportDetailsReportSection::GetOutputForAdjustmentType(int adjustmentType, UnicodeString title, TPrintout* printOut)
{
    TIBSQL* query = GetPointsQuery(adjustmentType);

    query->ExecQuery();
    Currency totalEarnedPoints = 0;

    if (query->RecordCount)
    {
        AddSubTitle(printOut, title);

        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->AddLine();

        IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
        if(reportSectionDisplayTrait)
        {
            reportSectionDisplayTrait->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 2;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 2;
        printOut->PrintFormat->Line->Columns[0]->Text = "Name";
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 5 / 9);
        printOut->PrintFormat->Line->Columns[1]->Text = "Earned";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->AddLine();

        for (; !query->Eof; query->Next())
        {
                Currency earnedPoints    = query->FieldByName("Points")->AsCurrency;

                printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
                printOut->PrintFormat->Line->ColCount = 2;
                printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 2;
                printOut->PrintFormat->Line->Columns[0]->Text = query->FieldByName("NAME")->AsString;;
                printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 5 / 9);
                printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(earnedPoints);
                printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
                printOut->PrintFormat->AddLine();

                totalEarnedPoints 	+= earnedPoints;
        }

        printOut->PrintFormat->Line->ColCount = 2;
        printOut->PrintFormat->Line->Columns[0]->Text = "";
		printOut->PrintFormat->Line->Columns[1]->DoubleLine();
		printOut->PrintFormat->AddLine();

		printOut->PrintFormat->Line->Columns[0]->Text = "Totals";
		printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
		printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(totalEarnedPoints);
		printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
		printOut->PrintFormat->AddLine();
    }
}

TIBSQL* XPointsReportDetailsReportSection::GetPointsQuery(int adjustmentType)
{
    AnsiString masterSlaveCondition = "";
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    _memberShip = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();

	if (!_globalSettings->EnableDepositBagNum)
	{
		masterSlaveCondition = " AND DAB.TERMINAL_NAME = :TERMINAL_NAME ";
	}

	TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
	ibInternalQuery->Close();
 /*	ibInternalQuery->SQL->Text = "SELECT "
                                    "COALESCE(SUM(PT.ADJUSTMENT), 0) Points, "
                                    "PT.CONTACTS_KEY AS LOYALTY_KEY, "
                                    "CT.NAME "
                                "FROM DAYARCBILL DAB "
                                "LEFT JOIN POINTSTRANSACTIONS PT ON DAB.INVOICE_NUMBER = PT.INVOICE_NUMBER "
                                "LEFT JOIN CONTACTS CT ON PT.CONTACTS_KEY = CT.CONTACTS_KEY "
                                "WHERE PT.ADJUSTMENT_TYPE = :Adjustment_Type " + masterSlaveCondition +
                                "GROUP BY PT.CONTACTS_KEY, CT.NAME "
                                "ORDER BY PT.CONTACTS_KEY; ";     */

    	ibInternalQuery->SQL->Text =
                                    "SELECT "
                                        "COALESCE(SUM(PT1.ADJUSTMENT), 0) Points, "
                                        "PT1.CONTACTS_KEY AS LOYALTY_KEY, "
                                        "PT1.NAME  "
                                    "FROM DAYARCBILL DAB "
                                    "INNER JOIN ( SELECT PT.ADJUSTMENT_TYPE,PT.CONTACTS_KEY,PT.ADJUSTMENT,CT.NAME,PT.INVOICE_NUMBER FROM  POINTSTRANSACTIONS  PT "
                                                "INNER JOIN CONTACTS CT ON PT.CONTACTS_KEY = CT.CONTACTS_KEY "
                                                "WHERE PT.ADJUSTMENT_TYPE =:Adjustment_Type   "
                                                ")PT1 ON PT1.INVOICE_NUMBER = DAB.INVOICE_NUMBER  "
                                   "WHERE DAB.TIME_STAMP >= (select coalesce(max(b.Time_Stamp),'28.12.1889, 19:39:25.000') from ZEDS b where b.TERMINAL_NAME = :ZTERMINAL_NAME)  "
                                     + masterSlaveCondition +
                                   "GROUP BY PT1.CONTACTS_KEY, PT1.NAME   "
                                    "ORDER BY PT1.CONTACTS_KEY;  ";


    ibInternalQuery->ParamByName("ZTERMINAL_NAME")->AsString = deviceName;
    ibInternalQuery->ParamByName("Adjustment_Type")->AsInteger = adjustmentType;

	if (!_globalSettings->EnableDepositBagNum)
	{
		ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
	}

    return ibInternalQuery;
}
