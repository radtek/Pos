//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMBillCalcResultDoc.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

namespace BillCalculator
{

TMMBillCalcResultDoc* TMMBillCalcResultDoc::_billCalcResultDoc = NULL;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
TMMBillCalcResultDoc::~TMMBillCalcResultDoc()
{
}
//---------------------------------------------------------------------------
TMMBillCalcResultDoc* TMMBillCalcResultDoc::Instance()
{
	if( !_billCalcResultDoc )
	{
		_billCalcResultDoc = new TMMBillCalcResultDoc();
	}
	return _billCalcResultDoc;
}
//---------------------------------------------------------------------------
void TMMBillCalcResultDoc::Save(
				 AnsiString inBaseFileName,
		   const TPriceInfo*      const inInput,
		   const TBillCalcResult* const inResult )
{
	_xmlResultDoc = createResultXMLDoc();
	addMainElements();

    setNodeAttr( _rootElem, "name", inBaseFileName );

	addInputElements(  inInput,  _inputElem );
	addOutputElements( inResult, _outputElem );

	_xmlResultDoc->SaveFile( billCalcResultFileName( inBaseFileName ).c_str() );

    delete _xmlResultDoc;
}
//---------------------------------------------------------------------------
TBillCalcInfo TMMBillCalcResultDoc::BillCalcInfoWithURI( AnsString inURI )
{
	_xmlResultDoc = createResultXMLDoc( inURI );

    TBillCalcInfo result;

    result.Name = AnsiString( _rootElem->Attribute( "name" ) );

    readInputSection(  result.Input  );
    readResultSection( result.Result );

    return result;
}
//...........................................................................

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
TMMBillCalcResultDoc::TMMBillCalcResultDoc()
{
}
//...........................................................................
TiXmlDocument* TMMBillCalcResultDoc::createResultXMLDoc()
{
    TiXmlDocument* result = new TiXmlDocument();

	//::::::::::::::::::::::::::::::

    // add declaration
	TiXmlDeclaration *decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	result->LinkEndChild( decl );

    //::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
void TMMBillCalcResultDoc::addMainElements()
{
	addElement( ( TiXmlElement* )_xmlResultDoc, "BillCalcResult", _rootElem );
	addElement( _rootElem,                      "Input",  		 _inputElem );
	addElement( _rootElem,                      "Output",        _outputElem );
}
//---------------------------------------------------------------------------
void TMMBillCalcResultDoc::addInputElements(
							  const BillCalculator::TPriceInfo* const inInput,
									TiXmlElement* inInputElem )
{
	AnsiString multiDiscStr =
		inInput->MultiDiscountType == mdtSimple ? "simple" : "compound";

	AnsiString multiTaxStr =
		inInput->MultiTaxType == mttSimple ? "simple" : "compound";

	setNodeAttr( inInputElem, "value", FormatCurr( "#,#.00", inInput->Value ) );
	setNodeAttr( inInputElem, "taxInclusive", inInput->TaxInclusive ? "true" : "false" );
	setNodeAttr( inInputElem, "taxExclusiveDiscount", inInput->TaxExclusiveDiscount ? "true" : "false" );
    setNodeAttr( inInputElem, "taxExclusiveServiceCharge", inInput->TaxExclusiveServiceCharge ? "true" : "false" );
	setNodeAttr( inInputElem, "multiDiscountType", multiDiscStr );
	setNodeAttr( inInputElem, "multiTaxType", multiTaxStr );
    setNodeAttr( inInputElem, "taxBeforeDiscount", inInput->TaxBeforeDiscount ? "true" : "false" );
    setNodeAttr( inInputElem, "serviceChargeBeforeDiscount", inInput->ServiceChargeBeforeDiscount ? "true" : "false" );
	setNodeAttr( inInputElem, "taxExempt", inInput->FinalPriceTaxExempt ? "true" : "false" );

	TiXmlElement *taxElems;
	TiXmlElement *discountElems;
	TiXmlElement *serviceChargeElem;

	addElement( inInputElem, "TaxInfoList",      taxElems );
	addElement( inInputElem, "DiscountInfoList", discountElems );
	addElement( inInputElem, "ServiceCharge",    serviceChargeElem );

	addTaxElements( inInput->TaxInfoList, taxElems );
	addDiscountElements( inInput->DiscountInfoList, discountElems );

	setNodeAttr( serviceChargeElem, "percentage",     FormatCurr( "#,#.00", inInput->ServiceChargeInfo.Percentage ) );
	setNodeAttr( serviceChargeElem, "taxPercentage",  FormatCurr( "#,#.00", inInput->ServiceChargeInfo.TaxPercentage ) );
}
//...........................................................................
void TMMBillCalcResultDoc::addTaxElements(
							  const TAX_INFO_LIST inTaxInfoList,
									TiXmlElement* inTaxElems )
{
	TAX_INFO_LIST::const_iterator taxInfoIT;

	for( taxInfoIT  = inTaxInfoList.begin();
		 taxInfoIT != inTaxInfoList.end(); taxInfoIT++ )
	{
		TTaxInfo taxInfo = *taxInfoIT;

        TiXmlElement *taxElem;

        addElement( inTaxElems, "TaxInfo", taxElem );

		setNodeAttr( taxElem, "name", taxInfo.Name );
		setNodeAttr( taxElem, "percentage",     FormatCurr( "#,#.00", taxInfo.Percentage ) );
	}
}
//...........................................................................
void TMMBillCalcResultDoc::addDiscountElements(
							  const DISCOUNT_INFO_LIST inDiscountInfoList,
							        TiXmlElement* inDiscountElems )
{
	DISCOUNT_INFO_LIST::const_iterator discountInfoIT;

	for( discountInfoIT  = inDiscountInfoList.begin();
		 discountInfoIT != inDiscountInfoList.end(); discountInfoIT++ )
	{
		TDiscountInfo discountInfo = *discountInfoIT;

		AnsiString discountTypeStr =
			discountInfo.DiscountType == dtDiscount ? "discount" : "surcharge";

		AnsiString discountWayStr =
			discountInfo.DiscountWay == dwPercentage ? "percentage" : "money";

        TiXmlElement* discountElem;

        addElement( inDiscountElems, "DiscountInfo", discountElem );

		setNodeAttr( discountElem, "name", discountInfo.Name );
		setNodeAttr( discountElem, "percentage",     FormatCurr( "#,#.00", discountInfo.Percentage ) );
		setNodeAttr( discountElem, "value",          FormatCurr( "#,#.00", discountInfo.Value ) );
		setNodeAttr( discountElem, "priority",       IntToStr( discountInfo.Priority ) );
		setNodeAttr( discountElem, "discountType",   discountTypeStr );
		setNodeAttr( discountElem, "discountWay",    discountWayStr );
	}
}
//...........................................................................
void TMMBillCalcResultDoc::addOutputElements(
							  const BillCalculator::TBillCalcResult* const inResult,
							        TiXmlElement* inOutputElem )
{
	Currency              BasePrice;

    TAX_RESULT_LIST       Tax;
	DISCOUNT_RESULT_LIST  Discount;

	setNodeAttr( inOutputElem, "basePrice",     FormatCurr( "#,#.00", inResult->BasePrice ) );
	setNodeAttr( inOutputElem, "finalPrice",    FormatCurr( "#,#.00", inResult->FinalPrice ) );
	setNodeAttr( inOutputElem, "totalTax",      FormatCurr( "#,#.00", inResult->TotalTax ) );
	setNodeAttr( inOutputElem, "totalDiscount", FormatCurr( "#,#.00", inResult->TotalDiscount ) );

	TiXmlElement *taxElem;
	TiXmlElement *discountElem;
	TiXmlElement *serviceChargeElem;

	addElement( inOutputElem, "TaxResult",      taxElem );
	addElement( inOutputElem, "DiscountResult", discountElem );
	addElement( inOutputElem, "ServiceCharge",  serviceChargeElem );

	addTaxResultElements( inResult->Tax, taxElem );
	addDiscountResultElements( inResult->Discount, discountElem );

	setNodeAttr( serviceChargeElem, "percentage",    FormatCurr( "#,#.00", inResult->ServiceCharge.Percentage ) );
	setNodeAttr( serviceChargeElem, "Content",       FormatCurr( "#,#.00", inResult->ServiceCharge.Value ) );
	setNodeAttr( serviceChargeElem, "taxPercentage", FormatCurr( "#,#.00", inResult->ServiceCharge.TaxPercentage ) );
	setNodeAttr( serviceChargeElem, "taxContent",    FormatCurr( "#,#.00", inResult->ServiceCharge.TaxValue ) );
}
//...........................................................................
void TMMBillCalcResultDoc::addTaxResultElements(
							  const TAX_RESULT_LIST inTaxList,
									TiXmlElement* inTaxElem )
{
	TAX_RESULT_LIST::const_iterator taxIT;

	for( taxIT  = inTaxList.begin();
		 taxIT != inTaxList.end(); taxIT++ )
	{
		TTaxResult taxResult = *taxIT;

		setNodeAttr( inTaxElem, "name", taxResult.Name );
		setNodeAttr( inTaxElem, "percentage", FormatCurr( "#,#.00", taxResult.Percentage ) );
		setNodeAttr( inTaxElem, "content",    FormatCurr( "#,#.00", taxResult.Value ) );
	}
}
//...........................................................................
void TMMBillCalcResultDoc::addDiscountResultElements(
							  const DISCOUNT_RESULT_LIST inDiscountList,
									TiXmlElement* inDiscountElem )
{
	DISCOUNT_RESULT_LIST::const_iterator discountIT;

	for( discountIT  = inDiscountList.begin();
		 discountIT != inDiscountList.end(); discountIT++ )
	{
		TDiscountResult discountResult = *discountIT;

		AnsiString discountTypeStr =
			discountResult.DiscountType == dtDiscount ? "discount" : "surcharge";

		AnsiString discountWayStr =
			discountResult.DiscountWay == dwPercentage ? "percentage" : "money";

		setNodeAttr( inDiscountElem, "name", discountResult.Name );
		setNodeAttr( inDiscountElem, "percentage",     FormatCurr( "#,#.00", discountResult.Percentage ) );
		setNodeAttr( inDiscountElem, "value",          FormatCurr( "#,#.00", discountResult.Value ) );
		setNodeAttr( inDiscountElem, "content",        FormatCurr( "#,#.00", discountResult.Content ) );
		setNodeAttr( inDiscountElem, "priority",       IntToStr( discountResult.Priority ) );
		setNodeAttr( inDiscountElem, "discountType",   discountTypeStr );
		setNodeAttr( inDiscountElem, "discountWay",    discountWayStr );
	}
}
//...........................................................................
void TMMBillCalcResultDoc::addElement(
			TiXmlElement* inParentElem,
			AnsiString inName,
			TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------
void TMMBillCalcResultDoc::setNodeAttr(
			      TiXmlElement* inElem,
				  AnsiString inAttrName,
				  AnsiString inAttrValue )
{
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
AnsiString TMMBillCalcResultDoc::billCalcResultFileName( AnsiString inBaseName )
{
	AnsiString bcFolder = ".\\Results";

	if( !DirectoryExists( bcFolder ) )
	{
		ForceDirectories( bcFolder );
	}

	AnsiString result = bcFolder + "\\" +
						Now().FormatString( "yyyy-mm-dd - hh-nn-ss " ) + " " +
						inBaseName + ".xml";

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
AnsiString TMMBillCalcResultDoc::xmlDateToStr( TDateTime inDateTime )
{
	return inDateTime.FormatString( "dd/mm/yyyy hh:nn:ss " );
}
//---------------------------------------------------------------------------
void TMMBillCalcResultDoc::readInputSection( TPriceInfo& outInput )
{
    outInput.Value = StrToCurrency( AnsiString( _inputElem->Attribute( "value" ) ) );
    outInput.TaxInclusive = readInputBoolAttr( "taxInclusive" );


    // Only apply if the Price is Tax Inclusive
	outInput.TaxExclusiveDiscount      = readInputBoolAttr( "taxExclusiveDiscount" );
    outInput.TaxExclusiveServiceCharge = readInputBoolAttr( "taxExclusiveServiceCharge" );

    outInput.TaxBeforeDiscount = readInputBoolAttr( "taxBeforeDiscount" );
    outInput.TaxBeforeDiscount = readInputBoolAttr( "serviceChargeBeforeDiscount" );

    outInput.FinalPriceTaxExempt = readInputBoolAttr( "taxExempt" );

    outInput.MultiDiscountType = readInputMultiDiscountTypeAttr( "multiDiscountType" );
    outInput.MultiTaxType      = readInputMultiTaxTypeAttr( "multiTaxType" );

    TAX_INFO_LIST      TaxInfoList;
    DISCOUNT_INFO_LIST DiscountInfoList;
    TServiceChargeInfo ServiceChargeInfo;
}
//---------------------------------------------------------------------------
void TMMBillCalcResultDoc::readResultSection( TBillCalcResult& outResult )
{
}
//---------------------------------------------------------------------------
bool TMMBillCalcResultDoc::readInputBoolAttr( AnsiString inAttrName )
{
    AnsiString tmp = AnsiString( _inputElem->Attribute( inAttrName.c_str() ) ) ).LowerCase();

    return tmp == "true"
}
//---------------------------------------------------------------------------
TMultiDiscountType TMMBillCalcResultDoc::readInputMultiDiscountTypeAttr( AnsiString inAttrName )
{
    AnsiString tmp = AnsiString( _inputElem->Attribute( inAttrName.c_str() ) ) ).LowerCase();

    return ( tmp == "simple" ) ?  mdtSimple : mdtCompound;
}
//---------------------------------------------------------------------------
TMultiTaxType TMMBillCalcResultDoc::readInputMultiTaxTypeAttr( AnsiString inAttrName )
{
    AnsiString tmp = AnsiString( _inputElem->Attribute( inAttrName.c_str() ) ) ).LowerCase();

    return ( tmp == "simple" ) ?  mttSimple : mttCompound;
}
//---------------------------------------------------------------------------

