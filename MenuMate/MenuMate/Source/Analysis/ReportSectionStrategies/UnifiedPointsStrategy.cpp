#include "UnifiedPointsStrategy.h"
#include "DeviceRealTerminal.h"

UnifiedPointsStrategy::UnifiedPointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
    _pointsReportHeaderTraits = pointsReportHeaderTraits;
    _dataFormatUtilities = new DataFormatUtilities;
}

void UnifiedPointsStrategy::BuildSection(TPrintout* printOut)
{
    AnsiString masterSlaveCondition = "WHERE DAB.TIME_STAMP >= (select coalesce(max(b.Time_Stamp),'28.12.1889, 19:39:25.000') from ZEDS b where b.TERMINAL_NAME = :ZTERMINAL_NAME) AND ";

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    _memberShip = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();

	if (!_globalSettings->EnableDepositBagNum)
	{
		masterSlaveCondition = masterSlaveCondition + " DAB.TERMINAL_NAME = :TERMINAL_NAME AND ";
	}

	TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
	ibInternalQuery->Close();

    ibInternalQuery->SQL->Text = "SELECT  "
                                    "SUM(DAYARCHIVE.POINTS_EARNED) EARNED,  "
                                    "SUM(DAYARCHIVE.REDEEMED) REDEEMED,  "
                                    "DAYARCHIVE.LOYALTY_KEY  "
                            "FROM DAYARCBILL DAB "
                            "LEFT JOIN DAYARCHIVE ON DAB.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
                            "inner join (select PT.INVOICE_NUMBER from POINTSTRANSACTIONS   PT "
                                        "inner join CONTACTS  CT ON CT.CONTACTS_KEY = PT.CONTACTS_KEY  "
                                        "where CT.MEMBER_TYPE != 2  "
                                        "AND  PT.ADJUSTMENT_TYPE not in (5,6) GROUP BY INVOICE_NUMBER "
                            " ) pts on  DAB.INVOICE_NUMBER = pts.INVOICE_NUMBER " + masterSlaveCondition + "(DAYARCHIVE.POINTS_EARNED != 0 OR DAYARCHIVE.REDEEMED != 0) "
                                    "AND DAYARCHIVE.LOYALTY_KEY != 0 AND DAYARCHIVE.LOYALTY_KEY IS NOT NULL "
                            "GROUP BY LOYALTY_KEY; ";


    ibInternalQuery->ParamByName("ZTERMINAL_NAME")->AsString = deviceName;
	if (!_globalSettings->EnableDepositBagNum)
	{
		ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
	}
	ibInternalQuery->ExecQuery();

	if (ibInternalQuery->RecordCount)
	{
		_pointsReportHeaderTraits->ApplyTraits(printOut);

		Currency totalEarnedPoints    = 0;
		Currency totalRedeemedPoints = 0;

		for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
		{
			int loyaltyKey = ibInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;

			Currency earnedPoints    = ibInternalQuery->FieldByName("EARNED")->AsCurrency;
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

