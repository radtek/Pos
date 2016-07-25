//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"

#include <memory>

#include "MMBillCalcResultDoc.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::

bool TMainForm::checkBeforeClosing()
{
    bool result;

    int mbResult = Application->MessageBox(
                        L"Save current project before close the application.",
                        L"Closing",
                        MB_YESNOCANCEL );

    switch( mbResult )
    {
        case IDYES:
        {
            saveProject();
            result = true;
        }

        case IDNO:
        {
            result = true;
        }

        case IDCANCEL:
        {
            result = false;
        }

        default: result = false;
    }

    return result;
}
//---------------------------------------------------------------------------
void TMainForm::saveProject()
{
}
//---------------------------------------------------------------------------
void TMainForm::commitDefault()
{
	using namespace BillCalculator;

	std::auto_ptr<TMMBillCalculator> billCalculator( new TMMBillCalculator );
	std::auto_ptr<TPriceInfo> priceInfo( new TPriceInfo );

	initDefaultPriceInfo( priceInfo.get() );

	TBillCalcResponse* response = billCalculator.get()->Execute( priceInfo.get() );

	showBillCalcResult( &response->Result );
	saveBillCalcResult( "Default", priceInfo.get(), &response->Result );

	delete response;
}
//---------------------------------------------------------------------------
void TMainForm::initDefaultPriceInfo( BillCalculator::TPriceInfo* const outPrinceInfo )
{
	using namespace BillCalculator;

	outPrinceInfo->Value = 100.00; // $100.00

	outPrinceInfo->FinalPriceTaxExempt = false;

	outPrinceInfo->TaxInclusive = false;

	outPrinceInfo->TaxExclusiveDiscount = false;
    outPrinceInfo->TaxExclusiveServiceCharge = false;

    outPrinceInfo->TaxBeforeDiscount = false;
	outPrinceInfo->ServiceChargeBeforeDiscount = false;

	outPrinceInfo->MultiDiscountType = mdtSimple;
	outPrinceInfo->MultiTaxType      = mttSimple;

	initDefaultTaxInfoList( outPrinceInfo->TaxInfoList );
	initDefaultDiscountInfoList( outPrinceInfo->DiscountInfoList );

	outPrinceInfo->ServiceChargeInfo.Percentage     = 10.00; // 10.0% Service Charge
	outPrinceInfo->ServiceChargeInfo.TaxPercentage  = 15.00; // 15.0% Service Charge Tax
}
//---------------------------------------------------------------------------
void TMainForm::initDefaultTaxInfoList( BillCalculator::TAX_INFO_LIST& outTaxInfoList )
{
	using namespace BillCalculator;

	TTaxInfo taxInfo;

	taxInfo.Name = "GST";
	taxInfo.Percentage = 15.00; // GST = 15.0%

	outTaxInfoList.push_back( taxInfo );
}
//---------------------------------------------------------------------------
void TMainForm::initDefaultDiscountInfoList( BillCalculator::DISCOUNT_INFO_LIST& outDiscountInfoList )
{
	using namespace BillCalculator;

	TDiscountInfo discountInfo;

	discountInfo.Name = "GST";
	discountInfo.Percentage = 15.00; // GST = 15.0%
	discountInfo.Value = 0;
	//discountInfo.TaxExclusive = true;
	discountInfo.Priority = 1;
	discountInfo.DiscountType = dtDiscount;
	discountInfo.DiscountWay  = dwPercentage;

	//outDiscountInfoList.push_back( discountInfo );
}
//---------------------------------------------------------------------------
void TMainForm::showBillCalcResult( const BillCalculator::TBillCalcResult* const inResult )
{
}
//---------------------------------------------------------------------------
void TMainForm::saveBillCalcResult(
						 UnicodeString inFileName,
				   const BillCalculator::TPriceInfo*      const inInput,
				   const BillCalculator::TBillCalcResult* const inResult )
{
	using namespace BillCalculator;

	TMMBillCalcResultDoc::Instance()->Save( inFileName, inInput, inResult );
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::
// PUBLISHED SECTION
//::::::::::::::::::::::::::::::::::::::::::::::

void __fastcall TMainForm::OpenItemClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewItemClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveItemClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveAsItemClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExitItemClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    if( checkBeforeClosing() )
    {
        Action = caNone;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::btnCommitInputClick(TObject *Sender)
{
	commitDefault();
}
//---------------------------------------------------------------------------


