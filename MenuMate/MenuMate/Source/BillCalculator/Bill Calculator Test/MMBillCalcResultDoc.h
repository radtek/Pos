//---------------------------------------------------------------------------

#ifndef MMBillCalcResultDocH
#define MMBillCalcResultDocH


//---------------------------------------------------------------------------

#include <vector>
#include "tinyxml.h"

#include "IBillCalculator.h"

//---------------------------------------------------------------------------

namespace BillCalculator
{

struct TBillCalcInfo
{
    AnsiString      Name;
    TPriceInfo      Input;
    TBillCalcResult Result;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TMMBillCalcResultDoc
{
public:
	//Singleton Instance Fetch Method
	static TMMBillCalcResultDoc* Instance();

	void Save(
				 AnsiString inBaseFileName,
		   const TPriceInfo*      const inInput,
		   const TBillCalcResult* const inResult );

    TBillCalcInfo BillCalcInfoWithURI( AnsString inURI );

private:
	//Singleton Object Instance
	static TMMBillCalcResultDoc* _billCalcResultDoc;

	TiXmlDocument *_xmlResultDoc;

	TiXmlElement *_rootElem;
	TiXmlElement *_inputElem;
	TiXmlElement *_outputElem;

	TMMBillCalcResultDoc();
	~TMMBillCalcResultDoc();

	TiXmlDocument* createResultXMLDoc();
	void addMainElements();

	void addInputElements(
			const BillCalculator::TPriceInfo* const inInput,
				  TiXmlElement* inInputElem );
	void addTaxElements(
			const TAX_INFO_LIST inTaxInfoList,
				  TiXmlElement* inTaxElems );
	void addDiscountElements(
			const DISCOUNT_INFO_LIST inDiscountInfoList,
				  TiXmlElement*      inDiscountElems );

	void addOutputElements(
			const BillCalculator::TBillCalcResult* const inResult,
				  TiXmlElement* inOutputElem );
	void addTaxResultElements(
			const TAX_RESULT_LIST inTaxList,
				  TiXmlElement* inTaxElem );
	void addDiscountResultElements(
			const DISCOUNT_RESULT_LIST inDiscountList,
			      TiXmlElement* inDiscountElem );

	void addElement(
			TiXmlElement* inParentElem,
			AnsiString inName,
			TiXmlElement* &inElem );
	void setNodeAttr(
			TiXmlElement* inElem,
			AnsiString inAttrName,
			AnsiString inAttrValue );

	AnsiString billCalcResultFileName( AnsiString inBaseName );
	AnsiString xmlDateToStr( TDateTime inDateTime );
};

}

#endif
