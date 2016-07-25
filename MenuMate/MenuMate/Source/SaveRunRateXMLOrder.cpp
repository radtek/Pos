//---------------------------------------------------------------------------

#pragma hdrstop

#include "SaveRunRateXMLOrder.h"

#include "vector.h"

#include "TcpStreamSender.h"
#include "TcpStreamSenderSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

ESaveXMLOrder::ESaveXMLOrder( UnicodeString inMsg, OERROR inError ) : Exception( inMsg )
{
	FError = inError;
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

ECommitOrderInvalidResponseFormat::ECommitOrderInvalidResponseFormat( UnicodeString inMsg ) :
								   Exception( inMsg )
{
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TSaveRunRateXMLOrder::TSaveRunRateXMLOrder()
{
	FAllowServingCourseDuplicate = false;
	FAllowCourseDuplicate        = false;

	_xmlOrderDoc = createXMLOrderDoc();
	addMainElements();
}
//......................................................

TSaveRunRateXMLOrder::~TSaveRunRateXMLOrder()
{
}
//......................................................

// Open a Complete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
//......................................................

// Opens an Incomplete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
//......................................................

// Opens a Cancel Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
//......................................................

// Opens a Credit Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
//......................................................

// Open a Replacement Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
//......................................................

// Open a Call Away Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
//......................................................

// Opens a Transfer Order
// SaveOrder() must be called straight away
//......................................................

// Adds a ServingCourse to an Order
//......................................................

// Adds a Course to an Order
//......................................................

// Adds a Item to an Order
//......................................................

// Adds a Item to cancel to an Order.
// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
// If inCaceled is false then inQty is ignored
//......................................................

// Adds a Item's Print Format to an Order
//......................................................

// Adds a Item's Option to an Order
//......................................................

// Adds a Item's Option Print Format to an Order
//......................................................

// Adds a Item's Side to an Order

// Adds a Item's Side to an Order
//......................................................

// Adds a Option's Print Format to an Order
//......................................................

// Adds a Side's Option to an Order
//......................................................

// Adds a Option's Print Format to an Order
//......................................................

// Saves an Order
//......................................................

//__int32 TSaveXMLOrder::GetOrderNumber()
//{
//	return StrToInt( _rootElem->Attribute( "orderNumber" ) );
//}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::

void TSaveRunRateXMLOrder::_setAllowServingCourseDuplicate( bool inAllow )
{
	FAllowServingCourseDuplicate = inAllow;
}
//......................................................

void TSaveRunRateXMLOrder::_setAllowCourseDuplicate( bool inAllow )
{
	FAllowCourseDuplicate = inAllow;
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::

TiXmlDocument* TSaveRunRateXMLOrder::createXMLOrderDoc()
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
void TSaveRunRateXMLOrder::addMainElements()
{
	addElement( ( TiXmlElement* )_xmlOrderDoc, "Order",          _rootElem );
	addElement( _rootElem,                    "ServingCourses", _servingCoursesElem );
	addElement( _rootElem,                    "Courses",        _coursesElem );
	addElement( _rootElem,                    "Items",          _itemsElem );
}
//---------------------------------------------------------------------------
void TSaveRunRateXMLOrder::addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------

void TSaveRunRateXMLOrder::setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue )
{
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

// Adds a Print Format to an Order
// Adds an Option to an Order
HOPTION TSaveRunRateXMLOrder::addOption( HPARENT    inParentHandle,
								  __int32    inDBKey,
								  AnsiString inName,
								  AnsiString inKitchenName,
								  bool	     inIsPlus )
{
	HOPTION result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	TiXmlElement *parentElem  = ( TiXmlElement* )inParentHandle;
	TiXmlElement *optionsElem = parentElem->FirstChildElement()->NextSiblingElement();
	TiXmlElement *optionElem;

	//::::::::::::::::::::::::::::

	addElement(  optionsElem, "Option",      optionElem );
	setNodeAttr( optionElem,  "key",         IntToStr( inDBKey ) );
	setNodeAttr( optionElem,  "name",        inName );
	setNodeAttr( optionElem,  "kitchenName", inKitchenName );
	setNodeAttr( optionElem,  "isPlus",      inIsPlus ? "true" : "false" );

	TiXmlElement *tmpElem;
	addElement(  optionElem, "PrintFormat", tmpElem );

	//::::::::::::::::::::::::::::

	result = ( HOPTION )optionElem;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Sends an Order to ChefMate
OERROR TSaveRunRateXMLOrder::sendOrderToCheftMate(
							TiXmlDocument* inXMLOrderDoc,
							HostnameLIST   inHostnameList )
{
	OERROR result = OERROR_SUCCESSFUL;
	try
	{
		HostnameLIST_Iterator cmIT;

		for( cmIT = inHostnameList.begin(); cmIT != inHostnameList.end(); cmIT++ )
		{
			std::auto_ptr<TTcpStreamSender> streamSender(
												new TTcpStreamSender(
													( AnsiString )*cmIT,
													TTcpStreamSenderSettings::Instance().IPPort ) );

			streamSender->Send( inXMLOrderDoc );
		}
		result = OERROR_FAILED_CLASS_COMMITTING_ORDER;
	}
	catch( ETcpStreamSenderInvalidIPPort &exc )
	{
		result = OERROR_FAILED_CLASS_COMMIT_ORDER_INVALID_IPPORT;
	}
	catch( ECommitOrderInvalidResponseFormat &exc )
	{
		result = OERROR_FAILED_CLASS_COMMIT_ORDER_INVALID_RESPONSE_FORMAT;
	}
	return result;
}
//......................................................

OERROR TSaveRunRateXMLOrder::responseToErrorResult( TiXmlDocument *inXMLResponseDoc )
{
	OERROR result = OERROR_SUCCESSFUL;

	//:::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement* rootElem = ( TiXmlElement* )inXMLResponseDoc->RootElement();

		AnsiString orderAction    = AnsiString( rootElem->Attribute( "action" ) );
		AnsiString responseResult = AnsiString( rootElem->Attribute( "result" ) );

		if( responseResult.UpperCase() == "FAILED" )
		{
			result = OERROR_FAILED_CLASS_COMMIT_ORDER;
		}
	}
	catch( ... )
	{
		throw new ECommitOrderInvalidResponseFormat( "Invalid XML response format" );
	}

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

AnsiString TSaveRunRateXMLOrder::orderFileName( HORDER inOrderHandle )
{
	AnsiString cmFolder = ".\\chefmate";

	if( !DirectoryExists( cmFolder ) )
	{
		ForceDirectories( cmFolder );
	}

	AnsiString result = cmFolder + "\\" +
						Now().FormatString( "yyyy-mm-dd - hh-nn-ss " ) + " " +
						IntToStr( ( int )inOrderHandle ) + ".xml";

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

AnsiString TSaveRunRateXMLOrder::xmlDateToStr( TDateTime inDateTime )
{
	return inDateTime.FormatString( "dd/mm/yyyy hh:nn:ss " );
}
//......................................................
