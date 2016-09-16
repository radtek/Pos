#include "XPriceAdjustmentReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"
#include "Analysis.h"

XPriceAdjustmentReportSection::XPriceAdjustmentReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmShowPriceAdjustmentSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XPriceAdjustmentReportSection::~XPriceAdjustmentReportSection()
{
    delete dataFormatUtilities;
}

void XPriceAdjustmentReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
      return;
	std::auto_ptr<TStringList> AdjustmentsUserList (new TStringList);
	std::auto_ptr<TStringList> AdjustmentsSQL (new TStringList);
	if (_globalSettings->ShowPriceAdjustment)
	{
		AdjustmentsSQL->Add("SELECT SUM(r.BASE_PRICE*r.QTY - r.PRICE_LEVEL0*r.QTY) Adjustment,  c.NAME "
		"FROM DAYARCHIVE r "
		"left join SECURITY s on r.SECURITY_REF = s.SECURITY_REF "
		"left join CONTACTS c on s.USER_KEY = c.CONTACTS_KEY "
		"where "
		"(r.BASE_PRICE > r.PRICE_LEVEL0 and r.HAPPY_HOUR = 'F') "
		"and r.TERMINAL_NAME = :TERMINAL_NAME "
		"and s.SECURITY_EVENT = 'Price Adjust' "
		"group by c.NAME ");

		AdjustmentsSQL->Add("SELECT SUM(r.BASE_PRICE*r.QTY - r.PRICE_LEVEL1*r.QTY) Adjustment,  c.NAME "
		"FROM DAYARCHIVE r "
		"left join SECURITY s on r.SECURITY_REF = s.SECURITY_REF "
		"left join CONTACTS c on s.USER_KEY = c.CONTACTS_KEY "
		"where "
		"(r.BASE_PRICE > r.PRICE_LEVEL1 and r.HAPPY_HOUR = 'T') "
		"and r.TERMINAL_NAME = :TERMINAL_NAME "
		"and s.SECURITY_EVENT = 'Price Adjust' "
		"group by c.NAME ");

		AdjustmentsSQL->Add("SELECT SUM(r.BASE_PRICE*r.QTY - r.PRICE_LEVEL0*r.QTY) Adjustment,  c.NAME "
		"FROM DAYARCHIVE r "
		"left join SECURITY s on r.SECURITY_REF = s.SECURITY_REF "
		"left join CONTACTS c on s.USER_KEY = c.CONTACTS_KEY "
		"where "
		"(r.BASE_PRICE < r.PRICE_LEVEL0 and r.HAPPY_HOUR = 'F') "
		"and r.TERMINAL_NAME = :TERMINAL_NAME "
		"and s.SECURITY_EVENT = 'Price Adjust' "
		"group by c.NAME ");

		AdjustmentsSQL->Add("SELECT SUM(r.BASE_PRICE*r.QTY - r.PRICE_LEVEL1*r.QTY) Adjustment,  c.NAME "
		"FROM DAYARCHIVE r "
		"left join SECURITY s on r.SECURITY_REF = s.SECURITY_REF "
		"left join CONTACTS c on s.USER_KEY = c.CONTACTS_KEY "
		"where "
		"(r.BASE_PRICE < r.PRICE_LEVEL1 and r.HAPPY_HOUR = 'T') "
		"and r.TERMINAL_NAME = :TERMINAL_NAME "
		"and s.SECURITY_EVENT = 'Price Adjust' "
		"group by c.NAME ");
     }
     TIBSQL* IBInternalQuery = _dbTransaction->Query( _dbTransaction->AddQuery());
	 for( int i = 0; i < AdjustmentsSQL->Count; i ++ )
	 {
		AnsiString SQL = AdjustmentsSQL->Strings[i];

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = SQL;
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
		IBInternalQuery->ExecQuery();

		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			if (IBInternalQuery->FieldByName("Adjustment")->AsCurrency != 0)
			{
				AnsiString Name = IBInternalQuery->FieldByName("NAME")->AsString;
				if(Name == "")
				{
					TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "No SecurityRef for Price Adjsut on Zed");
					Name = "Unknown";
				}

				int Index = AdjustmentsUserList->IndexOf(Name);
				if (Index == -1)
				{
					TCurPriceAdj *Item = new TCurPriceAdj;
					Index = AdjustmentsUserList->AddObject(Name, Item);
					if(i < 2)
					{
						Item->TotalUp += IBInternalQuery->FieldByName("Adjustment")->AsCurrency;
					}
					else
					{
						Item->TotalDown += IBInternalQuery->FieldByName("Adjustment")->AsCurrency;
					}
				}
				else
				{
					TCurPriceAdj *Item = ((TCurPriceAdj*)AdjustmentsUserList->Objects[Index]);
					if(i < 2)
					{
						Item->TotalUp += IBInternalQuery->FieldByName("Adjustment")->AsCurrency;
					}
					else
					{
						Item->TotalDown += IBInternalQuery->FieldByName("Adjustment")->AsCurrency;
					}
				}
			}
		}
  }

	if (AdjustmentsUserList->Count > 0)
	{
		AddTitle(printOut, "Price Adjusments Report");
		printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		printOut->PrintFormat->Line->ColCount = 3;

		printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
		printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
		printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width /3;
		printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
		printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]
		->Width - printOut->PrintFormat->Line->Columns[1]->Width;
		printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

		printOut->PrintFormat->Add("Name | Priced Up | Priced Down");
		printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

		Currency TotalUp = 0;
		Currency TotalDown = 0;
		for (int i = 0; i < AdjustmentsUserList->Count; i++)
		{
			printOut->PrintFormat->Line->ColCount = 3;
			printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width / 3;
			printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
			printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
			printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]
			->Width - printOut->PrintFormat->Line->Columns[1]->Width;
			printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

			Currency Up   = ((TCurPriceAdj*)AdjustmentsUserList->Objects[i])->TotalUp;
			Currency Down = ((TCurPriceAdj*)AdjustmentsUserList->Objects[i])->TotalDown;

			TotalUp += Up;
			TotalDown += Down;
			printOut->PrintFormat->Add(AdjustmentsUserList->Strings[i] + "|" + dataFormatUtilities->FormatMMReportCurrency( Up ) + "|" +  dataFormatUtilities->FormatMMReportCurrency( Down ) );
			delete((TCurPriceAdj*)AdjustmentsUserList->Objects[i]);
		}

			printOut->PrintFormat->Line->Columns[0]->Text = "";
			printOut->PrintFormat->Line->Columns[1]->Line();
			printOut->PrintFormat->Line->Columns[2]->Line();
	 		printOut->PrintFormat->AddLine();
	    	printOut->PrintFormat->Add("Total |" + dataFormatUtilities->FormatMMReportCurrency( TotalUp ) + "|" + dataFormatUtilities->FormatMMReportCurrency( TotalDown ) );

	}
}
