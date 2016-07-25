#include "XDiscountReportDetailsReportSection.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"
#include "ManagerReports.h"
XDiscountReportDetailsReportSection::XDiscountReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmDiscountReportDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XDiscountReportDetailsReportSection::~XDiscountReportDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XDiscountReportDetailsReportSection::GetOutput(TPrintout* printout)
{
        AddTitle(printout, "Discount Report");
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        TIBSQL *qrDiscount = _dbTransaction->Query(_dbTransaction->AddQuery());
        TTransactionInfo transactionInfo;
        TStringList *DiscountServerList = new TStringList;
		UnicodeString DiscountSQL = "";
		if (_globalSettings->SummariseDiscountOnZed)
		{
			DiscountSQL =
			"select '' NAME,SUM(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) DISCOUNT, sum(DAYARCHIVE.QTY) QTY,DAYARCORDERDISCOUNTS.DESCRIPTION "
			"from " "DAYARCBILL LEFT JOIN SECURITY ON DAYARCBILL.SECURITY_REF = SECURITY.SECURITY_REF "
			"LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "
			"LEFT JOIN DAYARCHIVE ON DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
			"LEFT JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
			"where "
			"DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' AND DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Complimentary' AND "
			"DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME " "AND DAYARCHIVE.DISCOUNT != 0 " "AND SECURITY.SECURITY_EVENT = '" + UnicodeString
			(SecurityTypes[secDiscountedBy]) + "' " "group by NAME,DAYARCORDERDISCOUNTS.DESCRIPTION; ";
		}
		else
		{
			DiscountSQL = "select SUM(DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) DISCOUNT, "
			"DAYARCBILL.ARCBILL_KEY,CONTACTS.NAME,DAYARCORDERDISCOUNTS.DESCRIPTION, DAYARCHIVE.TIME_STAMP_BILLED " "from "
			"DAYARCBILL LEFT JOIN SECURITY ON DAYARCBILL.SECURITY_REF = SECURITY.SECURITY_REF "
			"LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "
			"LEFT JOIN DAYARCHIVE ON DAYARCBILL.ARCBILL_KEY = DAYARCHIVE.ARCBILL_KEY "
			"LEFT JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
			"where "
			"DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' AND DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Complimentary' AND "
			"DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME " "AND DAYARCHIVE.DISCOUNT != 0 " "AND SECURITY.SECURITY_EVENT = '" + UnicodeString
			(SecurityTypes[secDiscountedBy]) + "' "
			"group by DAYARCBILL.ARCBILL_KEY,CONTACTS.NAME,DAYARCORDERDISCOUNTS.DESCRIPTION, DAYARCHIVE.TIME_STAMP_BILLED;";
		}

		qrDiscount->Close();
		qrDiscount->SQL->Text = DiscountSQL;
		qrDiscount->ParamByName("TERMINAL_NAME")->AsString = deviceName;
		qrDiscount->ExecQuery();

		for (; !qrDiscount->Eof; qrDiscount->Next())
		{
			if (qrDiscount->FieldByName("DISCOUNT")->AsCurrency != 0)
			{
				int Index = DiscountServerList->IndexOf(qrDiscount->FieldByName("NAME")->AsString);
				if (Index == -1)
				{
					TStringList *ItemsList = new TStringList;
					Index = DiscountServerList->AddObject(qrDiscount->FieldByName("NAME")->AsString, ItemsList);
					TCurrencyTotal *Item = new TCurrencyTotal;
					Item->Total = qrDiscount->FieldByName("DISCOUNT")->AsCurrency;
					if (TGlobalSettings::Instance().SummariseDiscountOnZed)
					{
						((TStringList*)DiscountServerList->Objects[Index])->AddObject
						(qrDiscount->FieldByName("DESCRIPTION")->AsString + " |x" + qrDiscount->FieldByName("QTY")->AsString,
						Item);
					}
					else
					{
						((TStringList*)DiscountServerList->Objects[Index])->AddObject
						(qrDiscount->FieldByName("TIME_STAMP_BILLED")->AsDateTime.FormatString("hh:nn ") + qrDiscount->FieldByName
						("DESCRIPTION")->AsString, Item);
					}
				}
				else
				{
					TCurrencyTotal *Item = new TCurrencyTotal;
					Item->Total = qrDiscount->FieldByName("DISCOUNT")->AsCurrency;
					if (TGlobalSettings::Instance().SummariseDiscountOnZed)
					{
						((TStringList*)DiscountServerList->Objects[Index])->AddObject
						(qrDiscount->FieldByName("DESCRIPTION")->AsString + " |x" + qrDiscount->FieldByName("QTY")->AsString,
						Item);
					}
					else
					{
						((TStringList*)DiscountServerList->Objects[Index])->AddObject
						(qrDiscount->FieldByName("TIME_STAMP_BILLED")->AsDateTime.FormatString("hh:nn ") + qrDiscount->FieldByName
						("DESCRIPTION")->AsString, Item);
					}
				}

			}
		}

           //	BuildXMLTotalsDiscount(DBTransaction);

		if (DiscountServerList->Count > 0)
		{
			Currency DiscountTotal = 0;

			printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			printout->PrintFormat->Line->ColCount = 1;
			printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width;
			printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			printout->PrintFormat->Line->Columns[0]->DoubleLine();
			printout->PrintFormat->AddLine();
			printout->PrintFormat->NewLine();

			//AddSectionTitle(printout.get(), "Discount Report");

			printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width / 3;
			printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
//			printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
		  //	printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
		//	printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width - printout->PrintFormat->Line->Columns[0]
		//	->Width - printout->PrintFormat->Line->Columns[1]->Width;
		//	printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

			if (TGlobalSettings::Instance().SummariseDiscountOnZed)
			{
				printout->PrintFormat->Add("Name | Items | Total");
			}
			else
			{
				printout->PrintFormat->Add("Name | | Total");
			}
			printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

			// Currency TotalDiscount = 0;
			for (int i = 0; i < DiscountServerList->Count; i++)
			{
				// Add User name...
				printout->PrintFormat->Line->ColCount = 1;
				printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width;
				printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				printout->PrintFormat->Add(DiscountServerList->Strings[i]);

				if (TGlobalSettings::Instance().SummariseDiscountOnZed)
				{
					// ...followed by the items discounted by that user.
					printout->PrintFormat->Line->ColCount = 3;
					printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width / 3;
					printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
					printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
					printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
					printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width - printout->PrintFormat->Line->Columns[0]
					->Width - printout->PrintFormat->Line->Columns[1]->Width;
					printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
					for (int j = 0; j < ((TStringList*)DiscountServerList->Objects[i])->Count; j++)
					{
						printout->PrintFormat->Add(((TStringList*)DiscountServerList->Objects[i])->Strings[j] + "|" + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)((TStringList*)DiscountServerList->Objects[i])->Objects[j])->Total ) );
						DiscountTotal += ((TCurrencyTotal*)((TStringList*)DiscountServerList->Objects[i])->Objects[j])->Total;
						delete((TStringList*)DiscountServerList->Objects[i])->Objects[j];
					}
				}
				else
				{
					 //printoutFormatForTxtValue(printout.get());

                        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

                        if(reportSectionDisplayTraits)
                        {
                        reportSectionDisplayTraits->ApplyTraits(printout);
                        }
					for (int j = 0; j < ((TStringList*)DiscountServerList->Objects[i])->Count; j++)
					{
						printout->PrintFormat->Add(((TStringList*)DiscountServerList->Objects[i])->Strings[j] + "|" + dataFormatUtilities->FormatMMReportCurrency( ( (TCurrencyTotal*)((TStringList*)DiscountServerList->Objects[i])->Objects[j])->Total ) );
						DiscountTotal += ((TCurrencyTotal*)((TStringList*)DiscountServerList->Objects[i])->Objects[j])->Total;
						delete((TStringList*)DiscountServerList->Objects[i])->Objects[j];
					}
				}
				delete DiscountServerList->Objects[i];
			}
		   //	printoutFormatForTxtValue(printout.get());
            IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
            if(reportSectionDisplayTraits)
            {
               reportSectionDisplayTraits->ApplyTraits(printout);
            }
			printout->PrintFormat->Line->Columns[0]->Text = "";
			printout->PrintFormat->Line->Columns[1]->DoubleLine();
			printout->PrintFormat->AddLine();
			printout->PrintFormat->Add("Total Discounts|" + dataFormatUtilities->FormatMMReportCurrency( DiscountTotal ) );
			TCalculatedTotals DiscountsTotal(etcTotalDiscounts, DiscountTotal,DiscountTotal,0, 0);
			transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalDiscounts]] = DiscountsTotal;
		}


 }
