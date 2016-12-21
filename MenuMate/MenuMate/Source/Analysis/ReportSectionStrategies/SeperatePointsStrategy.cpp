#include "SeperatePointsStrategy.h"
#include "DeviceRealTerminal.h"

SeperatePointsStrategy::SeperatePointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
    _pointsReportHeaderTraits = pointsReportHeaderTraits;
    _dataFormatUtilities = new DataFormatUtilities;
    IsConsolidatedStartegy = false;
}

SeperatePointsStrategy::SeperatePointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits, TDateTime* startTime, TDateTime* endTime)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings, startTime, endTime)
{
    _pointsReportHeaderTraits = pointsReportHeaderTraits;
    _dataFormatUtilities = new DataFormatUtilities;
    IsConsolidatedStartegy = true;
}

void SeperatePointsStrategy::BuildSection(TPrintout* printOut)
{
    AnsiString masterSlaveCondition = "";
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    _memberShip = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();

	if (!_globalSettings->EnableDepositBagNum)
	{
		masterSlaveCondition = "DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME AND ";
	}

	TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
	ibInternalQuery->Close();
    if(IsConsolidatedStartegy)
    {
       GetPointDetailsConsolidatedZed(ibInternalQuery, masterSlaveCondition);
       ibInternalQuery->ParamByName("startTime")->AsDateTime = *_startTime;
       ibInternalQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
       GetPointDetailsNormalZed(ibInternalQuery, masterSlaveCondition);
    }

    if (!_globalSettings->EnableDepositBagNum)
	{
		ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
	}
	ibInternalQuery->ExecQuery();

	if (ibInternalQuery->RecordCount)
	{
		_pointsReportHeaderTraits->ApplyTraits(printOut);

		int loyaltyKey = ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;

		Currency earnedPoints 	= 0;
		Currency redeemedPoints = 0;

		Currency totalEarnedPoints    = 0;
		Currency totalRedeemedPoints = 0;

		for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
		{
			if (loyaltyKey != ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger)
			{
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

				loyaltyKey = ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;

				earnedPoints    = 0;
				redeemedPoints = 0;
			}

			if (ibInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger == 2)
			{
				earnedPoints = ibInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
				totalEarnedPoints += earnedPoints;
			}
			else if (ibInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger == 3)
			{
				redeemedPoints = -ibInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
				totalRedeemedPoints += redeemedPoints;
			}
		}

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

	ibInternalQuery->Close();

    if(IsConsolidatedStartegy)
    {
        GetEarnedPointDetailsForConsolidatedZed(ibInternalQuery, masterSlaveCondition);
        ibInternalQuery->ParamByName("startTime")->AsDateTime = *_startTime;
        ibInternalQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    }
    else
    {
       GetEarnedPointDetailsForNormalZed(ibInternalQuery, masterSlaveCondition);
    }



	/*ibInternalQuery->SQL->Text = "SELECT "
                                        "SUM(ADJUSTMENT) ADJUSTMENT, "
                                        "ADJUSTMENT_TYPE, "
                                        "POINTSTRANSACTIONS.CONTACTS_KEY LOYALTY_KEY "
                                    "FROM POINTSTRANSACTIONS "
                                    "LEFT JOIN DAYARCBILL ON POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
                                    "LEFT JOIN CONTACTS ON POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
                                    "WHERE " + masterSlaveCondition + "ADJUSTMENT_SUBTYPE = 2 "
                                    "GROUP BY POINTSTRANSACTIONS.CONTACTS_KEY, ADJUSTMENT_TYPE "
                                    "ORDER BY LOYALTY_KEY;";*/

	if (!_globalSettings->EnableDepositBagNum)
	{
		ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
	}
	ibInternalQuery->ExecQuery();

	if (ibInternalQuery->RecordCount)
	{
        printOut->PrintFormat->NewLine();
        printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->SolidLineChar = '-';
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
        printOut->PrintFormat->Line->FontInfo.Bold = true;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[0]->Text = "Pre Loaded Report";
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[0]->Line();
        printOut->PrintFormat->AddLine();

		printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 3;
		printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[0]->Text = "Name";
		printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
		printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[1]->Text = "Loaded";
		printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
		printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[2]->Text = "Spent";
		printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
		printOut->PrintFormat->AddLine();

		printOut->PrintFormat->Line->ColCount = 1;
		printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
		printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
		printOut->PrintFormat->Line->Columns[0]->Line();
		printOut->PrintFormat->AddLine();

		int loyaltyKey = ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;

		Currency loadedPoints = 0;
		Currency spentPoints  = 0;

		Currency totalLoadedPoints = 0;
		Currency totalSpentPoints  = 0;

		for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
		{
			if (loyaltyKey != ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger)
			{
				printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				printOut->PrintFormat->Line->ColCount = 3;
				printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
				printOut->PrintFormat->Line->Columns[0]->Text = _memberShip->GetContactName(*_dbTransaction, loyaltyKey);
				printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
				printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(loadedPoints);
				printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
				printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width / 3;
				printOut->PrintFormat->Line->Columns[2]->Text = _dataFormatUtilities->FormatMMReportCurrency(spentPoints);
				printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
				printOut->PrintFormat->AddLine();

				loyaltyKey = ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;

				loadedPoints = 0;
				spentPoints  = 0;
			}

			if (ibInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger == 1)
			{
				loadedPoints = ibInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
				totalLoadedPoints += loadedPoints;
			}
			else if (ibInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger == 3)
			{
				spentPoints = -ibInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
				totalSpentPoints  += spentPoints;
			}
		}

		printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		printOut->PrintFormat->Line->ColCount = 3;
		printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[0]->Text = _memberShip->GetContactName(*_dbTransaction, loyaltyKey);
		printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
		printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(loadedPoints);
		printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
		printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[2]->Text = _dataFormatUtilities->FormatMMReportCurrency(spentPoints);
		printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
		printOut->PrintFormat->AddLine();

		printOut->PrintFormat->Line->Columns[0]->Text = "";
		printOut->PrintFormat->Line->Columns[1]->DoubleLine();
		printOut->PrintFormat->Line->Columns[2]->DoubleLine();
		printOut->PrintFormat->AddLine();

		printOut->PrintFormat->Line->Columns[0]->Text = "Totals";
		printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
		printOut->PrintFormat->Line->Columns[1]->Text = _dataFormatUtilities->FormatMMReportCurrency(totalLoadedPoints);
		printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
		printOut->PrintFormat->Line->Columns[2]->Text = _dataFormatUtilities->FormatMMReportCurrency(totalSpentPoints);
		printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
		printOut->PrintFormat->AddLine();
	}
}

void SeperatePointsStrategy::GetPointDetailsNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition)
{
    ibInternalQuery->SQL->Text = "SELECT "
    "SUM(ADJUSTMENT) ADJUSTMENT, "
    "ADJUSTMENT_TYPE, "
    "POINTSTRANSACTIONS.CONTACTS_KEY LOYALTY_KEY "
    "FROM "
    "POINTSTRANSACTIONS LEFT JOIN DAYARCBILL "
    "ON POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
    "LEFT JOIN CONTACTS ON "
    "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
    "WHERE " + masterSlaveCondition + "ADJUSTMENT_SUBTYPE = 1 AND CONTACTS.MEMBER_TYPE != 2"
    "GROUP BY POINTSTRANSACTIONS.CONTACTS_KEY, ADJUSTMENT_TYPE "
    "ORDER BY LOYALTY_KEY;";
}

void SeperatePointsStrategy::GetPointDetailsConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition)
{
	if (!_globalSettings->EnableDepositBagNum)
	{
		masterSlaveCondition = " ARCBILL.TERMINAL_NAME = :TERMINAL_NAME AND ";
	}

    ibInternalQuery->SQL->Text = "SELECT "
    "SUM(ADJUSTMENT) ADJUSTMENT, "
    "ADJUSTMENT_TYPE, "
    "POINTSTRANSACTIONS.CONTACTS_KEY LOYALTY_KEY "
    "FROM "
    "POINTSTRANSACTIONS LEFT JOIN ARCBILL "
    "ON POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER "
    "LEFT JOIN CONTACTS ON "
    "POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
    "WHERE " + masterSlaveCondition + "ADJUSTMENT_SUBTYPE = 1 AND CONTACTS.MEMBER_TYPE != 2"
    " and ARCBILL.TIME_STAMP >= :startTime  and ARCBILL.TIME_STAMP < :endTime  "
    "GROUP BY POINTSTRANSACTIONS.CONTACTS_KEY, ADJUSTMENT_TYPE "
    "ORDER BY LOYALTY_KEY;";
}

void SeperatePointsStrategy::GetEarnedPointDetailsForNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition)
{
    ibInternalQuery->SQL->Text = "SELECT "
            "SUM(ADJUSTMENT) ADJUSTMENT, "
            "ADJUSTMENT_TYPE, "
            "POINTSTRANSACTIONS.CONTACTS_KEY LOYALTY_KEY "
        "FROM POINTSTRANSACTIONS "
        "LEFT JOIN DAYARCBILL ON POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
        "LEFT JOIN CONTACTS ON POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
        "WHERE " + masterSlaveCondition + "ADJUSTMENT_SUBTYPE = 2 "
        "GROUP BY POINTSTRANSACTIONS.CONTACTS_KEY, ADJUSTMENT_TYPE "
        "ORDER BY LOYALTY_KEY;";
}

void SeperatePointsStrategy::GetEarnedPointDetailsForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition)
{
	if (!_globalSettings->EnableDepositBagNum)
	{
		masterSlaveCondition = " ARCBILL.TERMINAL_NAME = :TERMINAL_NAME AND ";
	}

    ibInternalQuery->SQL->Text = "SELECT "
            "SUM(ADJUSTMENT) ADJUSTMENT, "
            "ADJUSTMENT_TYPE, "
            "POINTSTRANSACTIONS.CONTACTS_KEY LOYALTY_KEY "
        "FROM POINTSTRANSACTIONS "
        "LEFT JOIN ARCBILL ON POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER "
        "LEFT JOIN CONTACTS ON POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY "
        "WHERE " + masterSlaveCondition + "ADJUSTMENT_SUBTYPE = 2 "
        " and ARCBILL.TIME_STAMP >= :startTime  and ARCBILL.TIME_STAMP < :endTime "
        "GROUP BY POINTSTRANSACTIONS.CONTACTS_KEY, ADJUSTMENT_TYPE "
        "ORDER BY LOYALTY_KEY;";
}


