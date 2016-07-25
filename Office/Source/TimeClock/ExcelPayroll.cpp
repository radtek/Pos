//---------------------------------------------------------------------------
#pragma hdrstop

#include "ExcelPayroll.h"
#include "OpExcel.hpp"
#include "OpOlkXP.hpp"
#include "OpOutlk.hpp"
#include <memory>
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Payroll
{
	class TXLRow
	{
	public:
		typedef std::map<int, std::pair<Variant, AnsiString> > TRow;

		TRow fRow;

		void AddCell(int Col, const Variant &Value)
		{
			fRow[Col] = std::make_pair(Value, "");
		}
		void AddCellHyperink(int Col, const Variant &Value, const AnsiString &Link)
		{
			fRow[Col] = std::make_pair(Value, Link);
		}
	};

	class ExcelTimeClockData
	{
		public: ExcelTimeClockData() : excel(new TOpExcel(0)) {}

		const std::auto_ptr<Opexcel::TOpExcel> excel;
		std::vector<TXLRow> rows;
	};
}
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
ExcelPayroll::ExcelPayroll() : data(new ExcelTimeClockData())
{
	TXLRow row;

	int col = 0;

	row.AddCell(col++, "Date");
	row.AddCell(col++, "Payroll ID");
	row.AddCell(col++, "Login");
	row.AddCell(col++, "Logout");
	row.AddCell(col++, "Break");
	row.AddCell(col++, "Hours Worked");
	row.AddCell(col++, "Department");

	data->rows.push_back(row);
}
//---------------------------------------------------------------------------
ExcelPayroll::~ExcelPayroll()
{
	delete data;
}
//---------------------------------------------------------------------------
void ExcelPayroll::Export()
{
	try
	{
		data->excel->Connected	= true;

		TOpExcelWorkbook *workBook		= data->excel->Workbooks->Add();
		TOpExcelWorksheet *workSheet	= workBook->Worksheets->Add();
		TOpExcelRange *range				= workSheet->Ranges->Add();

		range->HorizontalAlignment		= xlchaCenterAcrossSelection;

		range->Address						= "A1";
		range->HorizontalAlignment		= xlchaLeft;
		range->FontName					= "Tahoma";
		range->FontSize					= 17;
		range->SimpleText					= "Wages";

		range->Address						= "A2";
		range->HorizontalAlignment		= xlchaLeft;
		range->FontName					= "Tahoma";
		range->FontSize					= 14;
		range->Value						= "Printed " + Now().DateString();

		int rowNumber = 4;
		char maxCol = 0;
		for (unsigned int i=0; i<data->rows.size(); ++i)
		{
			const TXLRow &row = data->rows[i];

			TXLRow::TRow::const_iterator iCell = row.fRow.begin();
			for (; iCell != row.fRow.end(); ++iCell)
			{
				char col = iCell->first + 'A';
				range->Address	= AnsiString(col) + IntToStr(rowNumber);
				range->Value	= iCell->second.first;
				if (iCell->second.second != "")
				{
					TOpExcelHyperlink *link = workSheet->Hyperlinks->Add();
					link->Address = iCell->second.second;
					link->SubAddress = " ";
					link->AnchorCell = range->Address;
					link->Visible = true;
				}

				if (col > maxCol)
				{
					maxCol = col;
				}
			}
			++rowNumber;
		}
		range->Address = "A1:" + AnsiString(maxCol) + IntToStr(rowNumber);
		range->AutoFitColumns();

		data->excel->Visible = true;

		ShowMessage("Done");
	}
	__finally
	{
		data->excel->Connected	= false;
	}
}
//---------------------------------------------------------------------------
void ExcelPayroll::WorkPeriod(
	const Payroll::Worker& worker,
	const Payroll::WorkDay& workDay,
	const Payroll::WorkPeriod& workPeriod)
{
	TXLRow row;

	int col = 0;

	row.AddCell(col++, workDay.GetDateWorked().AsDateTime());
	row.AddCell(col++, worker.ToString());
	row.AddCell(col++, workPeriod.GetStart());
	row.AddCell(col++, workPeriod.GetEnd());
	row.AddCell(col++, workPeriod.GetBreak().FormatString("HH:mm:ss"));
	row.AddCell(col++, workPeriod.Duration().ToString());
	row.AddCell(col++, workPeriod.GetDepartment());

	data->rows.push_back(row);
}

//---------------------------------------------------------------------------

