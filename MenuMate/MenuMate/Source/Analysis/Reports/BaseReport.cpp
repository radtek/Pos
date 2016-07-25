#include "BaseReport.h"
#include "Comms.h"
#include "PrinterPhysical.h"
#include "ShowPrintout.h"
#include <vector>
#include <algorithm>
#include <memory>


BaseReport::BaseReport()
{
	_sections.clear();
}

//.....................................

BaseReport::~BaseReport()
{
    std::for_each(_sections.begin(), _sections.end(), delete_ptr());
}

//.....................................

unsigned int BaseReport::AddSection(IReportSection* reportSection)
{
	_sections.push_back(reportSection);
	return _sections.size() - 1;
}
//.....................................

IReportSection* BaseReport::InsertSectionAtIndex(unsigned int index, IReportSection* reportSection)
{
    std::vector<IReportSection*>::iterator itReportSectionIterator;
    itReportSectionIterator = _sections.begin();

    _sections.insert (itReportSectionIterator,index,reportSection);
}
//.....................................

IReportSection* BaseReport::RelocateSection(unsigned int sourceIndex, unsigned int targetIndex)
{
	//todo: move section at inSourceIndex to inTargetIndex
	// return the moved section

    //_sections.erase(_sections.begin())

	return NULL;
}
//.....................................

IReportSection* BaseReport::SectionAtIndex(unsigned int index)
{
    return _sections.at(index);
}
//.....................................

void BaseReport::DeleteSectionAtIndex(unsigned int index)
{
	_sections.erase(_sections.begin() + index);
}
//.....................................
void BaseReport::DeleteSection(IReportSection* reportSection)
{
	// todo: delete a given section from the list
}
//.....................................

void BaseReport::EnableSectionAtIndex(unsigned int index)
{
	// todo: enable the section at inIndex to be shown
}
//.....................................
void BaseReport::EnableSection(IReportSection* reportSection)
{
	// todo: enable the section inSection
}
//.....................................

void BaseReport::DisableSectionAtIndex(unsigned int index)
{
	// todo: disable the section at inIndex
}
//.....................................

void BaseReport::DisableSection(IReportSection* reportSection)
{
	// todo: disable the section inSection
}
//.....................................

unsigned int BaseReport::SectionCount()
{
	return _sections.size();
}
//.....................................

std::vector<IReportSection*> BaseReport::Sections()
{
	return _sections;
}
//.....................................

std::vector<IReportSection*> BaseReport::EnabledSections()
{
	return _sections;
}
//.....................................

std::vector<IReportSection*> BaseReport::DisabledSections()
{
	return _sections;
}

TPrintout* BaseReport::SetupPrintOutInstance()
{
    bool printerExists = true;

    if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
    {
        printerExists = false;
    }

    TPrintout* printOut = new TPrintout;

    if (!printerExists)
    {
        TPrinterPhysical defaultScreenPrinter;
        defaultScreenPrinter.NormalCharPerLine = 40;
        defaultScreenPrinter.BoldCharPerLine = 40;

        printOut->Printer = defaultScreenPrinter;
    }
    else
    {
        printOut->Printer = TComms::Instance().ReceiptPrinter;
    }

    printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;

    printOut->PrintFormat->DocumentName = GetReportName();

    ReportSections functor(printOut);
    std::for_each(_sections.begin(), _sections.end(), functor);

    //std::bind2nd(std::mem_fun(&IReportSection::GetOutput), printOut));
    return printOut;
}

