//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ManagerReports.h"
#include "MMLogging.h"
#include "Comms.h"
#include "ShowPrintout.h"
#include "Printout.h"
#include "Payment.h"
#include "GlobalSettings.h"
#include <vector>
#include <cctype>
//---------------------------------------------------------------------------
#pragma package(smart_init)


TSumPayments::TSumPayments()
{
 Name  = "" ;
 Total = 0 ;
 CashOut = 0;
 Rounding = 0;
 Surcharge = 0;
 Properties = 0;
 Qty = 0;
 TipAmount = 0;
}

bool TSumPayments::IsLoyaltyVoucher()
{
   return (Properties & ePayTypeGetVoucherDetails) && (Name == "Gift Card" || Name == "Voucher" )
          && TGlobalSettings::Instance().LoyaltyMateEnabled;
}

TManagerReports::TManagerReports(TForm * inOwner)
{
   Owner = inOwner;
   ZedToArchive = new TMemoryStream;
}

TManagerReports::~TManagerReports()
{
   delete ZedToArchive;
}

typedef std::pair<AnsiString, float> TTableTabSummaryData;

void
TManagerReports::PrintTableTabSummaryReport(
  Database::TDBTransaction &tr)
{
	static const AnsiString generateTableTabSummary(
		"select "
			"o.table_name, "
			"o.seatno, "
			"case when (t.tab_type in (1, 2)) then "
					  "case when (t.contacts_key is not null and "
								 "t.contacts_key <> 0) then c.name "
						   "else 'Unknown Member / Staff tab' "
						   "end "
				 "when (t.tab_name is null) then '' "
				 "else t.tab_name "
				 "end tab_name, "
			"case when (t.tab_type < 3) then 0 "
				 "when (t.tab_type in (3, 4)) then 1 "
				 "end tab_type, "
			"sum((o.price * o.qty) + o.discount) total_charge "
		"from tab t "
			 "inner join orders o "
				   "on o.tab_key = t.tab_key "
			 "left join contacts c "
				   "on c.contacts_key = t.contacts_key "
		"where t.tab_type < 5 "
			  "and o.time_stamp >= :base_date_time "
		"group by o.table_name, "
			 "o.seatno,"
			 "t.contacts_key,"
			 "t.tab_name, "
			 "c.name,"
			 "t.tab_type ");

	std::vector<TTableTabSummaryData> summaryData[2];
	TIBSQL *q = tr.Query(tr.AddQuery());
	unsigned short day;
	unsigned short dummy;
	unsigned short hour;
	unsigned short minute;
	unsigned short month;
	unsigned short year;

	static const AnsiString summaryStrtab[4] = {
		"Tabs",
		"Total Tabs",
		"Seated",
		"Total seated"
	};

	TTableTabSummaryData dataCache;
	bool printerExists;
    TPrintFormat *pfmt;
	std::auto_ptr<TPrintout> pout(new TPrintout);
	std::auto_ptr<TfrmShowPrintout>(showPrintWnd)(
	  TfrmShowPrintout::Create<TfrmShowPrintout>(Owner));
	AnsiString tab_name;
	float totals[2] = {0.0f, 0.0f};
	TDateTime base_date_time;

	Now().DecodeDate(&year, &month, &day);
	Now().DecodeTime(&hour, &minute, &dummy, &dummy);

   base_date_time = TDateTime(year, month, day, 5, 0, 0, 0);
	base_date_time -= ((hour << 6) | minute) < 0x140;

	q->SQL->Text = generateTableTabSummary;
	q->ParamByName("base_date_time")->AsDateTime = base_date_time;

	q->ExecQuery();
	for (int len, tab_type; !q->Eof; q->Next()) {
		if (!(tab_type = q->FieldByName("tab_type")->AsInteger))
			tab_name = q->FieldByName("tab_name")->AsString;

		else tab_name = q->FieldByName("table_name")->AsString + " " +
						q->FieldByName("tab_name")->AsString;

		summaryData[tab_type].push_back(
		  TTableTabSummaryData(tab_name,
		  q->FieldByName("total_charge")->AsFloat));
	}
	q->Close();

	if (!TComms::Instance().ReceiptPrinter.PhysicalPrinterKey) {
		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		pout->Printer = DefaultScreenPrinter;
	} else pout->Printer = TComms::Instance().ReceiptPrinter;

	pfmt = pout->PrintFormat;

	pfmt->DocumentName = "Table Tab Summary";
	pfmt->Line->ColCount = 1;
	pfmt->Line->FontInfo.Height	= fsDoubleSize;
	pfmt->Line->Columns[0]->Width = pfmt->Width;
	pfmt->Line->Columns[0]->Alignment = taCenter;
	pfmt->Line->Columns[0]->Text = pfmt->DocumentName = "Table Tab Summary";

	pfmt->Line->FontInfo.Height	= fsNormal;
	pfmt->AddLine();
	pfmt->Line->Columns[0]->Text =
	  Now().FormatString("d mmm yyyy h:nn:ss am/pm");
	pfmt->AddLine();
	pfmt->Line->Columns[0]->Text = TDeviceRealTerminal::Instance().ID.Name;
	pfmt->AddLine();
	pfmt->Line->Columns[0]->Text = TDeviceRealTerminal::Instance().User.Name;
	pfmt->AddLine();
	pfmt->NewLine();
	pfmt->Line->Columns[0]->DoubleLine();
	pfmt->AddLine();

	pfmt->Line->Columns[0]->Alignment = taLeftJustify;
	pfmt->Line->Columns[0]->Text = "Account purchases for zed period";
	pfmt->NewLine();

	for (int i = 0; i < 2; i++) {
		pfmt->Line->FontInfo.Height	= fsNormal;
		pfmt->Line->ColCount = 1;
		pfmt->Line->Columns[0]->Width = pfmt->Width;
		pfmt->Line->Columns[0]->Alignment = taLeftJustify;
		pfmt->Line->Columns[0]->Text = summaryStrtab[i << 1];
		pfmt->AddLine();

		pfmt->Line->FontInfo.Height = fsNormalSize;
		pfmt->Line->ColCount = 2;
		pfmt->Line->Columns[0]->Alignment = taRightJustify;
		pfmt->Line->Columns[0]->Width = pfmt->Width * 0.7;
		pfmt->Line->Columns[1]->Alignment = taRightJustify;
		pfmt->Line->Columns[1]->Width =
		  pfmt->Width - pfmt->Line->Columns[0]->Width;

		for (int j = 0; j < summaryData[i].size(); j++) {
			dataCache = summaryData[i][j];
			pfmt->Line->Columns[0]->Text = dataCache.first;
			pfmt->Line->Columns[1]->Text = FormatFloat("###0.00",
			                                           dataCache.second);
			totals[i] += dataCache.second;
			pfmt->AddLine();
		}

		pfmt->Line->Columns[0]->Text = "";
		pfmt->Line->Columns[1]->Line();
		pfmt->AddLine();
		pfmt->Line->Columns[0]->Text = summaryStrtab[(i << 1) + 1];
		pfmt->Line->Columns[1]->Text = FormatFloat("#####0.00", totals[i]);
		pfmt->AddLine();
	}

	pfmt->Line->FontInfo.Height = fsNormalSize;
	pfmt->Line->ColCount = 2;
	pfmt->Line->Columns[0]->Alignment = taRightJustify;
	pfmt->Line->Columns[0]->Width = pfmt->Width * 0.7;
	pfmt->Line->Columns[1]->Alignment = taRightJustify;
	pfmt->Line->Columns[1]->Width =
	  pfmt->Width - pfmt->Line->Columns[0]->Width;

	pfmt->Line->Columns[0]->Text = "";
	pfmt->Line->Columns[1]->DoubleLine();
	pfmt->AddLine();

	pfmt->Line->Columns[0]->Text = "Total saved sales for period";
	pfmt->Line->Columns[1]->Text = FormatFloat("#####0.00",
											   totals[0] + totals[1]);
	pfmt->AddLine();

	pfmt->PartialCut();

	pout->PrintToStream(showPrintWnd->CurrentPrintout.get());
	showPrintWnd->Execute();
}

void TManagerReports::PrintConsumption(Database::TDBTransaction &DBTransaction)
{
	AnsiString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

	bool PrinterExists = true;

	if(TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		PrinterExists = false;
	}

	std::auto_ptr<TPrintout> Printout(new TPrintout);

	if(!PrinterExists)
	{
		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		Printout->Printer	  = DefaultScreenPrinter;
	}
	else
	{
		Printout->Printer	  = TComms::Instance().ReceiptPrinter;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		TDateTime StartTime = Date() + EncodeTime(5,0,0,0); // Today 5am.
		TDateTime EndTime = (Date()+1) + EncodeTime(5,0,0,0); // Tommorow 5am.

		if(Now() < StartTime) // We are between Midnight and 5am
		{
			StartTime = (Date()-1) + EncodeTime(5,0,0,0); // Yesterday 5am.
			EndTime = Date() + EncodeTime(5,0,0,0); // Today 5am.
		}

		Printout->PrintFormat->DocumentName					= "Consumption Report";

		Printout->PrintFormat->Line->FontInfo.Height			= fsDoubleSize;
		Printout->PrintFormat->Line->ColCount					= 1;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
		Printout->PrintFormat->Line->Columns[0]->Text		= "Consumption Summary Report";
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->FontInfo.Height			= fsNormalSize;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->NewLine();

		Printout->PrintFormat->Line->Columns[0]->Text 		= "Report Covers ALL Tills for All Menus";
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->Columns[0]->Text			= "From " + StartTime.FormatString("dd/mmm h:nn:ss am/pm") + " To " + EndTime.FormatString("dd/mmm h:nn:ss am/pm");
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= "Printed At " + Now().FormatString("dd/mmm h:nn:ss am/pm");
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= DeviceName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= TDeviceRealTerminal::Instance().User.Name;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= "All Menus";
		Printout->PrintFormat->AddLine();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"Select "
				"ArcCategories.Category Group_Name, "
				"CategoryGroups.Name Course_Name, "
				"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
				"Archive.Size_Name, "
				"Sum(Archive.Qty) Item_Count, "
				"Cast(Sum( (Archive.Price * Archive.Qty )) as Numeric(15,4)) Price, "
				"Cast(Sum((Archive.Cost * (Archive.Cost_GST_Percent + 100) / 100)* Archive.Qty) as Numeric(15,4)) Cost "
			"From "
				"Security Left Join Archive on "
					"Security.Security_Ref = Archive.Security_Ref "
				"Left Join ArcCategories on "
					"Archive.Category_Key = ArcCategories.Category_Key "
				 "Left Join CategoryGroups on "
					"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"Where "
					"(Archive.Order_Type = 3 or "
					"Archive.Order_Type = 0) and "
					"Security.Time_Stamp >= :StartTime and "
					"Security.Time_Stamp <= :EndTime and "
					"Security.Security_Event = 'Ordered By' "
			"Group By "
				"ArcCategories.Category,"
				"CategoryGroups.Name,"
				"Archive.Item_Name,"
				"Archive.Size_Name "
			"Having "
				"Count(Archive.Archive_Key) > 0 "

			"Union All "

			"Select "
				"ArcCategories.Category Group_Name, "
				"CategoryGroups.Name Course_Name, "
				"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name, "
				"DayArchive.Size_Name, "
				"Sum(DayArchive.Qty) Item_Count, "
				"Cast(Sum((DayArchive.Price * DayArchive.Qty) ) as Numeric(15,4)) Price, "
				"Cast(Sum((DayArchive.Cost * (DayArchive.Cost_GST_Percent + 100) / 100) * DayArchive.Qty) as Numeric(15,4)) Cost "
			"From "
				"Security Left Join DayArchive on "
					"Security.Security_Ref = DayArchive.Security_Ref "
				 "Left Join ArcCategories on "
					"DayArchive.Category_Key = ArcCategories.Category_Key "
				 "Left Join CategoryGroups on "
					"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"Where "
				"(DayArchive.Order_Type = 3 or "
				"DayArchive.Order_Type = 0) and "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp <= :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"ArcCategories.Category, "
				"CategoryGroups.Name, "
				"DayArchive.Item_Name, "
				"DayArchive.Size_Name "
			"Having "
				"Count(DayArchive.Archive_Key) > 0 "

			"Union All "

			"Select "
				"ArcCategories.Category Group_Name, "
				"CategoryGroups.Name Course_Name, "
				"Orders.Item_Name, "
				"Orders.Size_Name, "
				"Sum(Orders.Qty) Item_Count, "
				"Cast(Sum((Orders.Price * Orders.Qty) )  as Numeric(15,4)) Price,"
				"Cast(Sum((Orders.Cost * (Orders.Cost_GST_Percent + 100) / 100) * Orders.Qty) as Numeric(15,4)) Cost "
			"From "
				"Security Left Join Orders on "
					"Security.Security_Ref = Orders.Security_Ref "
				 "Left Join ArcCategories on "
					"Orders.Category_Key = ArcCategories.Category_Key "
				 "Left Join CategoryGroups on "
					"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"Where "
				"(Orders.Order_Type = 3 or "
				"Orders.Order_Type = 0) and "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp <= :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"ArcCategories.Category, "
				"CategoryGroups.Name, "
				"Orders.Item_Name, "
				"Orders.Size_Name "
			"Having "
				"Count(Orders.Order_Key) > 0 "
			"Order By "
				"1,2,3,4";

		IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
		IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
		IBInternalQuery->ExecQuery();

		TCategoryGroups CategoryGroup;
      Currency GrandTotal = 0.00;
      Currency GrandQTY = 0.00;
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			TCategoryGroupDetails CategoryGroupDetails = CategoryGroup[IBInternalQuery->FieldByName("GROUP_NAME")->AsString];

			TCatTotal ItemTotals = CategoryGroupDetails.Details[IBInternalQuery->FieldByName("COURSE_NAME")->AsString];

			TBaseTotal Details;
			if(UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) == "DEFAULT")
			{
				Details = ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString];
			}
			else
			{
				Details = ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " +
															IBInternalQuery->FieldByName("SIZE_NAME")->AsString];
			}

			Details.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			Details.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			Details.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			if(UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) == "DEFAULT")
			{
				ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString] = Details;
			}
			else
			{
				ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " +
											  IBInternalQuery->FieldByName("SIZE_NAME")->AsString] = Details;
			}

			ItemTotals.Totals.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			ItemTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			ItemTotals.Totals.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			CategoryGroupDetails.Details[IBInternalQuery->FieldByName("COURSE_NAME")->AsString] = ItemTotals;
			CategoryGroupDetails.Totals.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			CategoryGroupDetails.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			CategoryGroupDetails.Totals.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;
			CategoryGroup[IBInternalQuery->FieldByName("GROUP_NAME")->AsString] = CategoryGroupDetails;
         GrandTotal  += IBInternalQuery->FieldByName("PRICE")->AsCurrency;
         GrandQTY    += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
		}

		if (CategoryGroup.size() > 0)
		{
         AddSectionTitle(Printout.get(),"Consumption Report for Zed Period");

		 Printout->PrintFormat->Line->ColCount = 3;
			Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
			Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
			Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
			Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
			Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;

			Printout->PrintFormat->Line->Columns[0]->Text = "Group / Category / Item";
			Printout->PrintFormat->Line->Columns[1]->Text = "Qty";
			Printout->PrintFormat->Line->Columns[2]->Text = "Total";
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

			TCategoryGroups::iterator itCatGroups;
			for (itCatGroups = CategoryGroup.begin(); itCatGroups != CategoryGroup.end() ;itCatGroups++)
			{
				if(itCatGroups != CategoryGroup.begin())
				{
					Printout->PrintFormat->Line->Columns[0]->Text = "";
					Printout->PrintFormat->Line->Columns[1]->Text = "";
					Printout->PrintFormat->Line->Columns[2]->Text = "";
					Printout->PrintFormat->AddLine();
				}
            /* ---- Draw a line ---- */
            Printout->PrintFormat->Line->ColCount					= 1;
            Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
            Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
            Printout->PrintFormat->Line->Columns[0]->Line();
            Printout->PrintFormat->AddLine();

            Printout->PrintFormat->Line->ColCount = 3;
            Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
            Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
            Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
            Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
            Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
            Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;
			/* ----            ---- */

				// Print the Cat Group.
				Printout->PrintFormat->Add(itCatGroups->first + "|" + FormatFloat("0.00",itCatGroups->second.Totals.Qty)  + "|" + FormatFloat("0.00",itCatGroups->second.Totals.Total) );

            /* ---- Draw a line ---- */
            Printout->PrintFormat->Line->ColCount					= 1;
            Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
            Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
            Printout->PrintFormat->Line->Columns[0]->Line();
            Printout->PrintFormat->AddLine();

            Printout->PrintFormat->Line->ColCount = 3;
            Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
            Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
            Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
            Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
			Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
            Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;
            /* ----            ---- */

				TCategoryGroupDetails CategoryGroupDetails = itCatGroups->second;
				TCategoryTotals::iterator itCategoryTotals;

				for (itCategoryTotals = CategoryGroupDetails.Details.begin(); itCategoryTotals != CategoryGroupDetails.Details.end();itCategoryTotals++)
				{
					Printout->PrintFormat->Add(" " + itCategoryTotals->first + "|" +
									  FormatFloat("0.00",itCategoryTotals->second.Totals.Qty) + "|" +
									  FormatFloat("0.00",itCategoryTotals->second.Totals.Total));

					TItemTotals::iterator itItemTotals;
					for (itItemTotals = itCategoryTotals->second.ItemDetails.begin(); itItemTotals != itCategoryTotals->second.ItemDetails.end(); itItemTotals++)
					{
						Printout->PrintFormat->Add("  " + itItemTotals->first + "|" +
										  FormatFloat("0.00",itItemTotals->second.Qty) + "|" +
										  FormatFloat("0.00",itItemTotals->second.Total));
					}
				}
			}

         // Print the Grand Total
         /* ---- Draw a line ---- */
         Printout->PrintFormat->Line->ColCount					= 1;
         Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
         Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
         Printout->PrintFormat->Line->Columns[0]->Line();
         Printout->PrintFormat->AddLine();

         Printout->PrintFormat->Line->ColCount = 3;
         Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
         Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
         Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
         Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
         Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
         Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;
         /* ----            ---- */

         Printout->PrintFormat->Add("Total |" + FormatFloat("0.00",GrandQTY) + "|" + FormatFloat("0.00",GrandTotal));

         /* ---- Draw a line ---- */
         Printout->PrintFormat->Line->ColCount					= 1;
         Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
         Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
         Printout->PrintFormat->Line->Columns[0]->Line();
         Printout->PrintFormat->AddLine();

         Printout->PrintFormat->Line->ColCount = 3;
         Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
         Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
         Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
         Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
         Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
         Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;
         /* ----            ---- */
		}

		Printout->PrintFormat->PartialCut();

		std::auto_ptr<TfrmShowPrintout>(frmShowPrintout)(TfrmShowPrintout::Create<TfrmShowPrintout>(Owner));
		Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());
		frmShowPrintout->Execute();
	}
	catch (Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerReports::PrintConsumptionByMenu(Database::TDBTransaction &DBTransaction, AnsiString SelectedMenu)
{
	AnsiString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

	bool PrinterExists = true;

	if(TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		PrinterExists = false;
	}

	std::auto_ptr<TPrintout> Printout(new TPrintout);

	if(!PrinterExists)
	{
		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		Printout->Printer	  = DefaultScreenPrinter;
	}
	else
	{
		Printout->Printer	  = TComms::Instance().ReceiptPrinter;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		TDateTime StartTime = Date() + EncodeTime(5,0,0,0); // Today 5am.
		TDateTime EndTime = (Date()+1) + EncodeTime(5,0,0,0); // Tommorow 5am.

		if(Now() < StartTime) // We are between Midnight and 5am
		{
			StartTime = (Date()-1) + EncodeTime(5,0,0,0); // Yesterday 5am.
			EndTime = Date() + EncodeTime(5,0,0,0); // Today 5am.
		}

		Printout->PrintFormat->DocumentName						= "Consumption Report";

		Printout->PrintFormat->Line->FontInfo.Height			= fsDoubleSize;
		Printout->PrintFormat->Line->ColCount					= 1;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
		Printout->PrintFormat->Line->Columns[0]->Text		= "Consumption Summary Report";
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->FontInfo.Height			= fsNormalSize;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->NewLine();

		Printout->PrintFormat->Line->Columns[0]->Text = "Report Covers ALL Tills for Select Menu";
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->Columns[0]->Text			= "From " + StartTime.FormatString("dd/mmm h:nn:ss am/pm") + " To " + EndTime.FormatString("dd/mmm h:nn:ss am/pm");
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= "Printed At " + Now().FormatString("dd/mmm h:nn:ss am/pm");
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= DeviceName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= TDeviceRealTerminal::Instance().User.Name;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text			= "Menu :" + SelectedMenu;
		Printout->PrintFormat->AddLine();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"Select "
				"ArcCategories.Category Group_Name, "
				"CategoryGroups.Name Course_Name, "
				"Cast(Archive.Item_Name as VarChar(50)) Item_Name, "
				"Archive.Size_Name, "
				"Sum(Archive.Qty) Item_Count, "
				"Cast(Sum((Archive.Price * Archive.Qty)  ) as Numeric(15,4)) Price, "
				"Cast(Sum((Archive.Cost * (Archive.Cost_GST_Percent + 100) / 100)*Archive.Qty) as Numeric(15,4)) Cost "
			"From "
				"Security Left Join Archive on "
					"Security.Security_Ref = Archive.Security_Ref "
				"Left Join ArcCategories on "
					"Archive.Category_Key = ArcCategories.Category_Key "
				 "Left Join CategoryGroups on "
					"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"Where "
					"(Archive.Order_Type = 3 or "
					"Archive.Order_Type = 0) and "
					"Archive.Menu_Name = :SelectedMenu and "
					"Security.Time_Stamp >= :StartTime and "
					"Security.Time_Stamp <= :EndTime and "
					"Security.Security_Event = 'Ordered By' "
			"Group By "
				"ArcCategories.Category,"
				"CategoryGroups.Name,"
				"Archive.Item_Name,"
				"Archive.Size_Name "
			"Having "
				"Count(Archive.Archive_Key) > 0 "

			"Union All "

			"Select "
				"ArcCategories.Category Group_Name, "
				"CategoryGroups.Name Course_Name, "
				"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name, "
				"DayArchive.Size_Name, "
				"Sum(DayArchive.Qty) Item_Count, "
				"Cast(Sum((DayArchive.Price *DayArchive.Qty)  ) as Numeric(15,4)) Price, "
				"Cast(Sum((DayArchive.Cost * (DayArchive.Cost_GST_Percent + 100) / 100)*DayArchive.Qty) as Numeric(15,4)) Cost "
			"From "
				"Security Left Join DayArchive on "
					"Security.Security_Ref = DayArchive.Security_Ref "
				 "Left Join ArcCategories on "
					"DayArchive.Category_Key = ArcCategories.Category_Key "
				 "Left Join CategoryGroups on "
					"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"Where "
				"(DayArchive.Order_Type = 3 or "
				"DayArchive.Order_Type = 0) and "
   			"DayArchive.Menu_Name = :SelectedMenu and "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp <= :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"ArcCategories.Category, "
				"CategoryGroups.Name, "
				"DayArchive.Item_Name, "
				"DayArchive.Size_Name "
			"Having "
				"Count(DayArchive.Archive_Key) > 0 "

			"Union All "

			"Select "
				"ArcCategories.Category Group_Name, "
				"CategoryGroups.Name Course_Name, "
				"Orders.Item_Name, "
				"Orders.Size_Name, "
				"Sum(Orders.Qty) Item_Count, "
				"Cast(Sum((Orders.Price* Orders.Qty)  ) as Numeric(15,4)) Price,"
				"Cast(Sum((Orders.Cost * (Orders.Cost_GST_Percent + 100) / 100)* Orders.Qty) as Numeric(15,4)) Cost "
			"From "
				"Security Left Join Orders on "
					"Security.Security_Ref = Orders.Security_Ref "
				 "Left Join ArcCategories on "
					"Orders.Category_Key = ArcCategories.Category_Key "
				 "Left Join CategoryGroups on "
					"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
			"Where "
				"(Orders.Order_Type = 3 or "
				"Orders.Order_Type = 0) and "
   			"Orders.Menu_Name = :SelectedMenu and "
				"Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp <= :EndTime and "
				"Security.Security_Event = 'Ordered By' "
			"Group By "
				"ArcCategories.Category, "
				"CategoryGroups.Name, "
				"Orders.Item_Name, "
				"Orders.Size_Name "
			"Having "
				"Count(Orders.Order_Key) > 0 "
			"Order By "
				"1,2,3,4";

		IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
		IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
		IBInternalQuery->ParamByName("SelectedMenu")->AsString = SelectedMenu;
		IBInternalQuery->ExecQuery();

		TCategoryGroups CategoryGroup;
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			TCategoryGroupDetails CategoryGroupDetails = CategoryGroup[IBInternalQuery->FieldByName("GROUP_NAME")->AsString];

			TCatTotal ItemTotals = CategoryGroupDetails.Details[IBInternalQuery->FieldByName("COURSE_NAME")->AsString];

			TBaseTotal Details;
			if(UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) == "DEFAULT")
			{
				Details = ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString];
			}
			else
			{
				Details = ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " +
															IBInternalQuery->FieldByName("SIZE_NAME")->AsString];
			}

			Details.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			Details.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			Details.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			if(UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) == "DEFAULT")
			{
				ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString] = Details;
			}
			else
			{
				ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " +
											  IBInternalQuery->FieldByName("SIZE_NAME")->AsString] = Details;
			}

			ItemTotals.Totals.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			ItemTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			ItemTotals.Totals.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			CategoryGroupDetails.Details[IBInternalQuery->FieldByName("COURSE_NAME")->AsString] = ItemTotals;
			CategoryGroupDetails.Totals.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			CategoryGroupDetails.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			CategoryGroupDetails.Totals.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			CategoryGroup[IBInternalQuery->FieldByName("GROUP_NAME")->AsString] = CategoryGroupDetails;
		}

		if (CategoryGroup.size() > 0)
		{
			Printout->PrintFormat->Line->FontInfo.Height			= fsNormalSize;
			Printout->PrintFormat->Line->ColCount					= 1;
			Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

         AddSectionTitle(Printout.get(),"Consumption Report for Zed Period");

			Printout->PrintFormat->Line->ColCount					= 3;
         Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
			Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
			Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
			Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
			Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;

			Printout->PrintFormat->Line->Columns[0]->Text = "Group / Category / Item";
			Printout->PrintFormat->Line->Columns[1]->Text = "Qty";
			Printout->PrintFormat->Line->Columns[2]->Text = "Total";
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

			TCategoryGroups::iterator itCatGroups;
         Currency GrandTotal = 0;
         float TotalQTY = 0;
			for (itCatGroups = CategoryGroup.begin(); itCatGroups != CategoryGroup.end() ;itCatGroups++)
			{
				if(itCatGroups != CategoryGroup.begin())
				{
					Printout->PrintFormat->Line->Columns[0]->Text = "";
					Printout->PrintFormat->Line->Columns[1]->Text = "";
					Printout->PrintFormat->Line->Columns[2]->Text = "";
					Printout->PrintFormat->AddLine();
				}
				// Print the Cat Group.
				Printout->PrintFormat->Add(itCatGroups->first + "|" + FormatFloat("0.00",itCatGroups->second.Totals.Qty)  + "|" + FormatFloat("0.00",itCatGroups->second.Totals.Total) );

				TCategoryGroupDetails CategoryGroupDetails = itCatGroups->second;
				TCategoryTotals::iterator itCategoryTotals;

				for (itCategoryTotals = CategoryGroupDetails.Details.begin(); itCategoryTotals != CategoryGroupDetails.Details.end();itCategoryTotals++)
				{
					Printout->PrintFormat->Add(" " + itCategoryTotals->first + "|" +
									  FormatFloat("0.00",itCategoryTotals->second.Totals.Qty) + "|" +
									  FormatFloat("0.00",itCategoryTotals->second.Totals.Total));

					TItemTotals::iterator itItemTotals;
					for (itItemTotals = itCategoryTotals->second.ItemDetails.begin(); itItemTotals != itCategoryTotals->second.ItemDetails.end(); itItemTotals++)
					{
						Printout->PrintFormat->Add("  " + itItemTotals->first + "|" +
										  FormatFloat("0.00",itItemTotals->second.Qty) + "|" +
										  FormatFloat("0.00",itItemTotals->second.Total));
					}
				}
            GrandTotal  += itCatGroups->second.Totals.Total;
            TotalQTY    +=  itCatGroups->second.Totals.Qty;
			}


			Printout->PrintFormat->Line->FontInfo.Height			= fsNormalSize;
			Printout->PrintFormat->Line->ColCount					= 1;
			Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			Printout->PrintFormat->Line->ColCount					= 3;
			Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width * 3/5;
			Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width		= Printout->PrintFormat->Width * 1/5;
			Printout->PrintFormat->Line->Columns[1]->Alignment	= taRightJustify;
			Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
			Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;
         
         Printout->PrintFormat->Add("Grand Total|" +
                       FormatFloat("0.00",TotalQTY) + "|" +
                       FormatFloat("0.00",GrandTotal));

		}

		Printout->PrintFormat->PartialCut();

		std::auto_ptr<TfrmShowPrintout>(frmShowPrintout)(TfrmShowPrintout::Create<TfrmShowPrintout>(Owner));
		Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());
		frmShowPrintout->Execute();
	}
	catch (Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		//throw;
	}
}

void TManagerReports::AddSectionTitle(TPrintout *Printout,AnsiString Title)
{
   Printout->PrintFormat->NewLine();
   Printout->PrintFormat->Line->FontInfo.Height			= fsNormalSize;
   Printout->PrintFormat->Line->ColCount					= 1;
   Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
   Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
   Printout->PrintFormat->Line->Columns[0]->Text = "";
   Printout->PrintFormat->Line->Columns[0]->DoubleLine();
   Printout->PrintFormat->AddLine();

   Printout->PrintFormat->Line->FontInfo.Height			= fsDoubleSize;
   Printout->PrintFormat->Line->ColCount = 3;
   Printout->PrintFormat->Line->Columns[0]->Width		= (Printout->PrintFormat->Width - Title.Length()) / 2;
   Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
   Printout->PrintFormat->Line->Columns[1]->Width		= Title.Length();
   Printout->PrintFormat->Line->Columns[1]->Alignment	= taCenter;
   Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Title.Length() - (Printout->PrintFormat->Width - Title.Length()) / 2;
   Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;
   Printout->PrintFormat->Line->Columns[0]->Text = "";
   Printout->PrintFormat->Line->Columns[1]->Text = Title;
   Printout->PrintFormat->Line->Columns[2]->Text = "";
   Printout->PrintFormat->AddLine();
}

void TManagerReports::AddSubSectionTitle(TPrintout *Printout,AnsiString Title)
{
   Printout->PrintFormat->NewLine();
   Printout->PrintFormat->Line->FontInfo.Height			= fsNormalSize;
   Printout->PrintFormat->Line->ColCount = 3;
   Printout->PrintFormat->Line->Columns[0]->Width		= (Printout->PrintFormat->Width - Title.Length()) / 2;
   Printout->PrintFormat->Line->Columns[0]->Alignment	= taLeftJustify;
   Printout->PrintFormat->Line->Columns[1]->Width		= Title.Length();
   Printout->PrintFormat->Line->Columns[1]->Alignment	= taCenter;
   Printout->PrintFormat->Line->Columns[2]->Width		= Printout->PrintFormat->Width - Title.Length() - (Printout->PrintFormat->Width - Title.Length()) / 2;
   Printout->PrintFormat->Line->Columns[2]->Alignment	= taRightJustify;

   Printout->PrintFormat->Line->Columns[0]->Line(Printout->PrintFormat->Line->Columns[0]->Width - 1);
   Printout->PrintFormat->Line->Columns[1]->Text = Title;
   Printout->PrintFormat->Line->Columns[2]->Line(Printout->PrintFormat->Line->Columns[2]->Width - 1);
   Printout->PrintFormat->AddLine();
}

void TManagerReports::PrintFloatAdjustments(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName)
{
	try
	{

	  TDateTime PrevZedTime = Now();

	  bool PrinterExists = true;

	  if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	  {
		 PrinterExists = false;
	  }

		std::auto_ptr <TPrintout> Printout(new TPrintout);

	  if (!PrinterExists)
	  {
		 TPrinterPhysical DefaultScreenPrinter;
		 DefaultScreenPrinter.NormalCharPerLine = 40;
		 DefaultScreenPrinter.BoldCharPerLine = 40;
		 Printout->Printer = DefaultScreenPrinter;
	  }
	  else
	  {
		 Printout->Printer = TComms::Instance().ReceiptPrinter;
	  }

	  Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;



		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "select zeds.z_Key, zeds.initial_float, refloat_skim.amount, REFLOAT_SKIM.REASONS, REFLOAT_SKIM.TRANSACTION_TYPE, refloat_skim.staff, refloat_skim.time_stamp "
									 "from zeds left join refloat_skim on zeds.z_key = REFLOAT_SKIM.Z_KEY "
									 "where zeds.TIME_STAMP is null and zeds.TERMINAL_NAME = :TERMINAL_NAME "
								  //	 "and (refloat_skim.transaction_type = 'Refloat' or refloat_skim.transaction_type = 'Skim') "
									 "order by 6, 7; ";

		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();




		 if (IBInternalQuery->RecordCount)
		 {

			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			AddSectionTitle(Printout.get(), "Float Audit (by Staff member)");
			Printout->PrintFormat->Line->ColCount = 4;

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->Line->Columns[2]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			Printout->PrintFormat->Line->ColCount = 4;
			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 4;
			Printout->PrintFormat->Line->Columns[0]->Text = "Operator";
			Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 4;
			Printout->PrintFormat->Line->Columns[1]->Text = "Time";
			Printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width * 1 / 4;
			Printout->PrintFormat->Line->Columns[2]->Text = "Reason";
			Printout->PrintFormat->Line->Columns[2]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[3]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[2]->Width - Printout->PrintFormat->Line->Columns[1]->Width - Printout->PrintFormat->Line->Columns[0]->Width;
			Printout->PrintFormat->Line->Columns[3]->Text = "Total";
			Printout->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
			Printout->PrintFormat->AddLine();



			Float total = 0;




			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				if(IBInternalQuery->FieldByName("TRANSACTION_TYPE")->AsString == "No Change")
					continue;

				Printout->PrintFormat->Line->Columns[0]->Text = IBInternalQuery->FieldByName("STAFF")->AsString;
				Printout->PrintFormat->Line->Columns[1]->Text = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("HH:MM:SS ");
				Printout->PrintFormat->Line->Columns[2]->Text = IBInternalQuery->FieldByName("REASONS")->AsString;
				Printout->PrintFormat->Line->Columns[3]->Text = FloatToStrF(IBInternalQuery->FieldByName("AMOUNT")->AsFloat, ffCurrency,
				18, CurrencyDecimals);
				total += IBInternalQuery->FieldByName("AMOUNT")->AsFloat;


				if(IBInternalQuery->FieldByName("TRANSACTION_TYPE")->AsString == "Initial")
				{
					Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[3]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[2]->Width - Printout->PrintFormat->Line->Columns[1]->Width - Printout->PrintFormat->Line->Columns[0]->Width;
					Printout->PrintFormat->Line->Columns[2]->Text = "Set Float";
				}
				else
				{
					Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[3]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[2]->Width - Printout->PrintFormat->Line->Columns[1]->Width - Printout->PrintFormat->Line->Columns[0]->Width;
				}


				Printout->PrintFormat->AddLine();
			}
					Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 2;
					Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 4;
					Printout->PrintFormat->Line->Columns[2]->Width = 0;
					Printout->PrintFormat->Line->Columns[3]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[2]->Width - Printout->PrintFormat->Line->Columns[1]->Width - Printout->PrintFormat->Line->Columns[0]->Width;

			Printout->PrintFormat->Line->Columns[0]->Text = "Float Balance:";
			Printout->PrintFormat->Line->Columns[1]->Text = "";
			Printout->PrintFormat->Line->Columns[2]->Text = "";
			Printout->PrintFormat->Line->Columns[3]->Text = FloatToStrF(total, ffCurrency, 18, CurrencyDecimals);

			Printout->PrintFormat->AddLine();
		}





		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "select zeds.z_Key, zeds.initial_float, refloat_skim.amount, REFLOAT_SKIM.REASONS, REFLOAT_SKIM.TRANSACTION_TYPE, refloat_skim.staff, refloat_skim.time_stamp "
									 "from zeds left join refloat_skim on zeds.z_key = REFLOAT_SKIM.Z_KEY "
									 "where zeds.TIME_STAMP is null and zeds.TERMINAL_NAME = :TERMINAL_NAME "
								  //	 "and (refloat_skim.transaction_type = 'Refloat' or refloat_skim.transaction_type = 'Skim') "
									 "order by  7; ";

		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();


		if (IBInternalQuery->RecordCount)
		 {

			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			AddSectionTitle(Printout.get(), "Float Audit (by Time)");
			Printout->PrintFormat->Line->ColCount = 4;

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->Line->Columns[2]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			Printout->PrintFormat->Line->ColCount = 2;
			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 2;
			Printout->PrintFormat->Line->Columns[0]->Text = "Time";
			Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width;
			Printout->PrintFormat->Line->Columns[1]->Text = "Amount";
			Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
			Printout->PrintFormat->AddLine();



			Float total = 0;





			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				if(IBInternalQuery->FieldByName("TRANSACTION_TYPE")->AsString == "No Change")
					continue;

				Printout->PrintFormat->Line->Columns[0]->Text = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("HH:MM:SS, DD/MM/YYYY");
				Printout->PrintFormat->Line->Columns[1]->Text = FloatToStrF(IBInternalQuery->FieldByName("AMOUNT")->AsFloat, ffCurrency,
				18, CurrencyDecimals);
				total += IBInternalQuery->FieldByName("AMOUNT")->AsFloat;

				Printout->PrintFormat->AddLine();
			}

			Printout->PrintFormat->Line->Columns[0]->Text = "Float Balance: ";
			Printout->PrintFormat->Line->Columns[1]->Text = FloatToStrF(total, ffCurrency, 18, CurrencyDecimals);
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->Columns[0]->Text = "";
			Printout->PrintFormat->Line->Columns[1]->Text = "";
			Printout->PrintFormat->AddLine();
		   }

		 Printout->PrintFormat->PartialCut();

		 std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (Owner));
		 Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

		 if (TGlobalSettings::Instance().EnableBlindBalances)
		 {
			frmShowPrintout->btnCancel->Visible = false;
		 }
		 else
		 {
			frmShowPrintout->btnCancel->Visible = true;
			frmShowPrintout->btnCancel->Caption = "Cancel Zed";
		 }
		 frmShowPrintout->btnClose->Caption = "Close Till";
		 frmShowPrintout->btnClosePrint->Caption = "Close Till and Print Zed";

	  {
		 frmShowPrintout->btnCancel->Caption = "Close";
		 frmShowPrintout->btnClose->Visible = false;
		 frmShowPrintout->btnClosePrint->Caption = "Print";
	  }
	  frmShowPrintout->Execute();
	  ExitCode = frmShowPrintout->ExitCode;

		 UnicodeString Dir = ExtractFilePath(Application->ExeName) + ZDIR;
		 if (!DirectoryExists(Dir))
		 {
			CreateDir(Dir);
		 }
		 Printout->PrintToFile(Dir + "\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + DeviceName + LOGFILE);
		 Printout->PrintToStream(ZedToArchive);

	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
	}
}




