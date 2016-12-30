#include "BaseReportSection.h"
#include "ReportSectionDisplayStrategyProvider.h"
#include "ReportSectionDisplayTraitsProvider.h"

BaseReportSection::BaseReportSection(ReportType reportType, ReportSectionType reportSectionType, Database::TDBTransaction* dbTransaction,
            TGlobalSettings* globalSettings) : _reportType(reportType), _dbTransaction(dbTransaction), _globalSettings(globalSettings),
            _reportSectionType(reportSectionType)
{
	//We need to insert the required strategy to build the section..
	ReportSectionDisplayStrategyProvider* reportSectionDisplayStrategyProvider = new ReportSectionDisplayStrategyProvider(dbTransaction, globalSettings);
	_reportSectionDisplayStrategy = reportSectionDisplayStrategyProvider->CreateSectionStrategy(reportType, reportSectionType);

    //We need to inject the traits for the section, these traits will add some additional properties to sections..
    ReportSectionDisplayTraitsProvider* reportSectionDisplayTraitsProvider = new ReportSectionDisplayTraitsProvider(globalSettings);
    _reportSectionDisplayTraits = reportSectionDisplayTraitsProvider->CreateSectionTraits(reportType, reportSectionType);
    IsConsolidatedZed = false;
}


BaseReportSection::BaseReportSection(ReportType reportType, ReportSectionType reportSectionType, Database::TDBTransaction* dbTransaction,
            TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime) : _reportType(reportType), _dbTransaction(dbTransaction), _globalSettings(globalSettings),
            _reportSectionType(reportSectionType), _startTime(startTime), _endTime(endTime)
{
	//We need to insert the required strategy to build the section..
	ReportSectionDisplayStrategyProvider* reportSectionDisplayStrategyProvider = new ReportSectionDisplayStrategyProvider(dbTransaction, globalSettings, startTime, endTime);
	_reportSectionDisplayStrategy = reportSectionDisplayStrategyProvider->CreateSectionStrategy(reportType, reportSectionType);

    //We need to inject the traits for the section, these traits will add some additional properties to sections..
    ReportSectionDisplayTraitsProvider* reportSectionDisplayTraitsProvider = new ReportSectionDisplayTraitsProvider(globalSettings);
    _reportSectionDisplayTraits = reportSectionDisplayTraitsProvider->CreateSectionTraits(reportType, reportSectionType);
    IsConsolidatedZed = true;
}

IReportSectionDisplayStrategy* BaseReportSection::GetReportSectionStrategy()
{
    return _reportSectionDisplayStrategy;
}

std::vector<IReportSectionDisplayTraits*> BaseReportSection::GetReportSectionTraits()
{
    return _reportSectionDisplayTraits;
}

void BaseReportSection::SetReportSectionStrategy(IReportSectionDisplayStrategy* reportSectionDisplayStrategy)
{
    _reportSectionDisplayStrategy = reportSectionDisplayStrategy;
}

void BaseReportSection::SetReportSectionTraits(std::vector<IReportSectionDisplayTraits*> reportSectionDisplayTraits)
{
    _reportSectionDisplayTraits = reportSectionDisplayTraits;
}

bool BaseReportSection::GetIsEnabled()
{
    return _isEnabled;
}

void BaseReportSection::SetIsEnabled(bool isEnabled)
{
    _isEnabled =  isEnabled;
}

void BaseReportSection::SetStartTime(TDateTime* startTime)
{
    _startTime =  startTime;
}

void BaseReportSection::SetEndTime(TDateTime* endTime)
{
    _endTime =  endTime;
}

TDateTime* BaseReportSection::GetStartTime()
{
    return _startTime;
}

TDateTime* BaseReportSection::GetEndTime()
{
    return _endTime;
}

void BaseReportSection::AddTitle(TPrintout* printOut, AnsiString title)
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
	printOut->PrintFormat->Line->Columns[0]->Text = title;
	printOut->PrintFormat->AddLine();

    printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	printOut->PrintFormat->Line->ColCount = 1;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
	printOut->PrintFormat->Line->Columns[0]->Text = "";
	printOut->PrintFormat->Line->Columns[0]->Line();
	printOut->PrintFormat->AddLine();
}

void BaseReportSection::AddSubTitle(TPrintout *printOut, UnicodeString title)
{
	printOut->PrintFormat->NewLine();
	printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	printOut->PrintFormat->Line->ColCount = 1;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
    printOut->PrintFormat->Line->Columns[0]->Text = title;
	printOut->PrintFormat->AddLine();
}

IReportSectionDisplayTraits* BaseReportSection::GetTextFormatDisplayTrait()
{
    std::vector<IReportSectionDisplayTraits*> reportSectionDisplayTraits = GetReportSectionTraits();
    if(!reportSectionDisplayTraits.empty())
    {
        for (std::vector<IReportSectionDisplayTraits*>::iterator itReportSectionDisplayTraits = reportSectionDisplayTraits.begin(); itReportSectionDisplayTraits != reportSectionDisplayTraits.end(); itReportSectionDisplayTraits++)
        {
            IReportSectionDisplayTraits* reportSectionDisplayTraits = *itReportSectionDisplayTraits;
            if(reportSectionDisplayTraits->GetTraitType() == mmTextFormatDisplayTrait)
            {
                return reportSectionDisplayTraits;
                break;
            }
        }
    }
}




