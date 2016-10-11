//---------------------------------------------------------------------------

#pragma hdrstop

#include "SaveXMLOrder.h"

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

TSaveXMLOrder::TSaveXMLOrder()
{
	FAllowServingCourseDuplicate = false;
	FAllowCourseDuplicate        = false;

	_xmlOrderDoc = createXMLOrderDoc();
	addMainElements();
}
//......................................................

TSaveXMLOrder::~TSaveXMLOrder()
{
}
//......................................................

// Open a Complete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDER TSaveXMLOrder::OpenCompleteOrder( __int32    inDBKey,
										 AnsiString inServerName,
										 __int32    inOrderNumber,
										 AnsiString inChitValue,
										 AnsiString inTableTabName,
										 AnsiString inOrderType,
										 AnsiString inCustomerName,
										 AnsiString inPartyName,
										 AnsiString inPatronCount,
										 AnsiString inSaleStartTime,
                                         AnsiString inDeliveryTime,
                                         AnsiString inPhone,
                                         AnsiString inEmail,
                                         AnsiString inAddress,
                                         AnsiString inPaymentStatus
									   )
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::
        AnsiString actionType = "order";
        if(inPaymentStatus != "")
            actionType = "webOrder";

		setNodeAttr( orderElem, "action",         actionType );
		setNodeAttr( orderElem, "type",           "complete" );
		setNodeAttr( orderElem, "dbKey",          IntToStr( inDBKey ) );
		setNodeAttr( orderElem, "serverName",     inServerName );
		setNodeAttr( orderElem, "orderNumber",    IntToStr( inOrderNumber ) );
		setNodeAttr( orderElem, "chitValue",      inChitValue );
		setNodeAttr( orderElem, "tableTabName",   inTableTabName );
		setNodeAttr( orderElem, "orderType",      inOrderType );
		setNodeAttr( orderElem, "customerName",   inCustomerName );
		setNodeAttr( orderElem, "partyName",      inPartyName );
		setNodeAttr( orderElem, "patronCount",    inPatronCount );
		setNodeAttr( orderElem, "saleStartTime",  inSaleStartTime );
		setNodeAttr( orderElem, "saleFinishTime", xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",   inDeliveryTime );
        setNodeAttr( orderElem, "customerPhone",   inPhone );
        setNodeAttr( orderElem, "customerEmail",  inEmail);
        setNodeAttr( orderElem, "customerAddress", inAddress );
        if(actionType == "webOrder")
        {
            setNodeAttr( orderElem, "paymentStatus", inPaymentStatus );
        }
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_COMPLETE_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens an Incomplete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDER TSaveXMLOrder::OpenIncompleteOrder( __int32    inDBKey,
										   AnsiString inServerName,
										   __int32    inOrderNumber,
										   AnsiString inChitValue,
										   AnsiString inTableTabName,
										   AnsiString inOrderType,
										   AnsiString inCustomerName,
										   AnsiString inPartyName
										 )
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::

		setNodeAttr( orderElem, "action",       "order" );
		setNodeAttr( orderElem, "type",         "incomplete" );
		setNodeAttr( orderElem, "dbKey",        IntToStr( inDBKey ) );
		setNodeAttr( orderElem, "serverName",   inServerName );
		setNodeAttr( orderElem, "orderNumber",  IntToStr( inOrderNumber ) );
		setNodeAttr( orderElem, "chitValue",    inChitValue );
		setNodeAttr( orderElem, "tableTabName", inTableTabName );
		setNodeAttr( orderElem, "orderType",    inOrderType );
		setNodeAttr( orderElem, "customerName", inCustomerName );
		setNodeAttr( orderElem, "partyName",    inPartyName );
		setNodeAttr( orderElem, "saleFinishTime", xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",  xmlDateToStr( Now() ) );
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_INCOMPLETE_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens a Cancel Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
HORDER TSaveXMLOrder::OpenCancelOrder( __int32 inDBKey,
									   __int32 inOrderNumber,
                                       AnsiString inTableTabName )
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::

		setNodeAttr( orderElem, "action",       "cancel" );
		setNodeAttr( orderElem, "type",         "complete" );
		setNodeAttr( orderElem, "dbKey",        IntToStr( inDBKey ) );
		setNodeAttr( orderElem, "serverName",   "" );
		setNodeAttr( orderElem, "orderNumber",  IntToStr( inOrderNumber ) );
		setNodeAttr( orderElem, "chitValue",    "" );
		setNodeAttr( orderElem, "tableTabName", inTableTabName );
		setNodeAttr( orderElem, "orderType",    "" );
		setNodeAttr( orderElem, "customerName", "" );
		setNodeAttr( orderElem, "partyName",    "" );
		setNodeAttr( orderElem, "patronCount",    "0" );
		setNodeAttr( orderElem, "saleStartTime",  xmlDateToStr( Now() ) );
		setNodeAttr( orderElem, "saleFinishTime", xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",  xmlDateToStr( Now() ) );
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_CANCEL_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens a Credit Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
HORDER TSaveXMLOrder::OpenCreditOrder()
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::

		setNodeAttr( orderElem, "action",       "credit" );
		setNodeAttr( orderElem, "type",         "complete" );
		setNodeAttr( orderElem, "dbKey",        "0" );
		setNodeAttr( orderElem, "serverName",   "" );
		setNodeAttr( orderElem, "orderNumber",  "0" );
		setNodeAttr( orderElem, "chitValue",    "" );
		setNodeAttr( orderElem, "tableTabName", "" );
		setNodeAttr( orderElem, "orderType",    "" );
		setNodeAttr( orderElem, "customerName", "" );
		setNodeAttr( orderElem, "partyName",    "" );
		setNodeAttr( orderElem, "saleFinishTime",     xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",  xmlDateToStr( Now() ) );
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_CREDIT_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Open a Replacement Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDER TSaveXMLOrder::OpenReplacementOrder( __int32    inDBKey,
											__int32    inOrderNumber,
											AnsiString inTableTabName )
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::

		setNodeAttr( orderElem, "action",       "replace" );
		setNodeAttr( orderElem, "type",         "complete" );
		setNodeAttr( orderElem, "dbKey",        IntToStr( inDBKey ) );
		setNodeAttr( orderElem, "serverName",   "" );
		setNodeAttr( orderElem, "orderNumber",  IntToStr( inOrderNumber ) );
		setNodeAttr( orderElem, "chitValue",    "" );
		setNodeAttr( orderElem, "tableTabName", inTableTabName );
		setNodeAttr( orderElem, "orderType",    "" );
		setNodeAttr( orderElem, "customerName", "" );
		setNodeAttr( orderElem, "partyName",    "" );
		setNodeAttr( orderElem, "patronCount",  "0" );
		setNodeAttr( orderElem, "saleFinishTime", xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",  xmlDateToStr( Now() ) );
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_COMPLETE_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Open a Call Away Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDER TSaveXMLOrder::OpenCallAwayOrder( __int32    inOrderNumber,
										 AnsiString inTableTabName )
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::

		setNodeAttr( orderElem, "action",       "callaway" );
		setNodeAttr( orderElem, "type",         "complete" );
		setNodeAttr( orderElem, "dbKey",        "" );
		setNodeAttr( orderElem, "serverName",   "" );
		setNodeAttr( orderElem, "orderNumber",  IntToStr( inOrderNumber ) );
		setNodeAttr( orderElem, "chitValue",    "" );
		setNodeAttr( orderElem, "tableTabName", inTableTabName );
		setNodeAttr( orderElem, "orderType",    "" );
		setNodeAttr( orderElem, "customerName", "" );
		setNodeAttr( orderElem, "partyName",    "" );
		setNodeAttr( orderElem, "patronCount",  "0" );
		setNodeAttr( orderElem, "saleFinishTime", xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",  xmlDateToStr( Now() ) );
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_COMPLETE_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens a Transfer Order
// SaveOrder() must be called straight away
HORDER TSaveXMLOrder::OpenTransferOrder( __int32    inDBKey,
										 __int32    inOrderNumber,
										 AnsiString inTableTabName,
                                         AnsiString inServerName,
                                         AnsiString inPatronCount,
                                         AnsiString inSaleStartTime,
                                         AnsiString inSourceTableName)
{
	HORDER result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *orderElem = _rootElem;

		//::::::::::::::::::::::::::::

		setNodeAttr( orderElem, "action",       "transfer" );
		setNodeAttr( orderElem, "type",         "complete" );
		setNodeAttr( orderElem, "dbKey",        IntToStr( inDBKey ) );
		setNodeAttr( orderElem, "serverName",   inServerName );
		setNodeAttr( orderElem, "orderNumber",  IntToStr( inOrderNumber ) );
		setNodeAttr( orderElem, "chitValue",    "" );
		setNodeAttr( orderElem, "tableTabName", inTableTabName );
		setNodeAttr( orderElem, "orderType",    "" );
		setNodeAttr( orderElem, "customerName", "" );
        setNodeAttr( orderElem, "patronCount",    inPatronCount );
		setNodeAttr( orderElem, "partyName",    "" );
        setNodeAttr( orderElem, "saleStartTime",  inSaleStartTime);
		setNodeAttr( orderElem, "saleFinishTime", xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "deliveryTime",  xmlDateToStr( Now() ) );
        setNodeAttr( orderElem, "sourceTableName",  inSourceTableName  );
		//::::::::::::::::::::::::::::

		result = ( HORDER )orderElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_OPEN_TRANSFER_ORDER );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a ServingCourse to an Order
HSCOURSE TSaveXMLOrder::AddServingCourse( __int32    inDBKey,
										  AnsiString inName,
										  AnsiString inKitchenName,
										  __int32 	 inDisplayOrder )
{
	HSCOURSE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	TiXmlElement *scourseElem = servingCourseWithKey( inDBKey );

	// To avoid Serving Course duplication in the current order
	if(    FAllowServingCourseDuplicate ||
		( !FAllowServingCourseDuplicate && ( scourseElem == NULL ) ) )
	{
		try
		{
			TiXmlElement *scourseElem;

			//::::::::::::::::::::::::::::

			addElement( _servingCoursesElem, "ServingCourse", scourseElem );
			setNodeAttr( scourseElem, 		 "key",  IntToStr( inDBKey ) );
			setNodeAttr( scourseElem, 		 "name", inName );
			setNodeAttr( scourseElem, 		 "kitchenName", inKitchenName );
			setNodeAttr( scourseElem, 		 "displayOrder",  IntToStr( inDisplayOrder ) );

			//::::::::::::::::::::::::::::

			result = ( HSCOURSE )scourseElem;
		}
		catch( Exception &e  )
		{
			throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_SERVING_COURSE );
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Course to an Order
HCOURSE TSaveXMLOrder::AddCourse( __int32    inDBKey,
								  AnsiString inName,
								  AnsiString inKitchenName,
								  __int32    inServingCourseKey,
								  __int32 	 inDisplayOrder )
{
	HCOURSE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	TiXmlElement *courseElem = courseWithKey( inDBKey );

	// To avoid Course duplication in the current order
	if(    FAllowCourseDuplicate ||
		( !FAllowCourseDuplicate && ( courseElem == NULL ) ) )
	{
		try
		{
			TiXmlElement *courseElem;

			//::::::::::::::::::::::::::::

			addElement( _coursesElem, "Course",             courseElem );
			setNodeAttr( courseElem,  "key",                IntToStr( inDBKey ) );
			setNodeAttr( courseElem,  "name",               inName );
			setNodeAttr( courseElem,  "kitchenName",        inKitchenName );
			setNodeAttr( courseElem,  "servingCourseDBKey", inServingCourseKey );
			setNodeAttr( courseElem,  "displayOrder", 		IntToStr( inDisplayOrder ) );

			//::::::::::::::::::::::::::::

			result = ( HCOURSE )courseElem;
		}
		catch( Exception &e  )
		{
			throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_COURSE );
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item to an Order
HITEM TSaveXMLOrder::AddItem( __int32    inDBKey,
							  __int32    inOrderItemDBKey,
                           AnsiString    inName,
						   AnsiString    inKitchenName,
							  __int32    inServingCourseKey,
							  __int32    inCourseKey,
                           AnsiString    inNote )
{
	HITEM result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *itemElem;

		//::::::::::::::::::::::::::::
		addElement( _itemsElem, "Item",               itemElem );
		setNodeAttr( itemElem,  "key",                IntToStr( inDBKey ) );
		setNodeAttr( itemElem,  "name",               inName  );
		setNodeAttr( itemElem,  "kitchenName",        inKitchenName );
		setNodeAttr( itemElem,  "orderItemKey",       IntToStr( inOrderItemDBKey ) );
		setNodeAttr( itemElem,  "servingCourseDBKey", IntToStr( inServingCourseKey ) );
		setNodeAttr( itemElem,  "courseDBKey",        IntToStr( inCourseKey ) );
		setNodeAttr( itemElem,  "note",               inNote );

		TiXmlElement *tmpElem;
		addElement(  itemElem, "PrintFormat", tmpElem );
		addElement(  itemElem, "Options",     tmpElem );
		addElement(  itemElem, "Sides",       tmpElem );

		//::::::::::::::::::::::::::::

		result = ( HITEM )itemElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item to cancel to an Order.
// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
// If inCaceled is false then inQty is ignored
HITEM TSaveXMLOrder::AddItemToCancel( __int32    inOrderItemKey,
									  AnsiString inItemName,
									  bool       inCanceled,
									  __int32    inQty )
{
	HITEM result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *itemElem;

		//::::::::::::::::::::::::::::

		AnsiString canceledStr = inCanceled ? "TRUE" : "FALSE";

		addElement( _itemsElem, "Item",         itemElem );
		setNodeAttr( itemElem,  "orderItemKey", IntToStr( inOrderItemKey ) );
		setNodeAttr( itemElem,  "name",         inItemName );
		setNodeAttr( itemElem,  "canceled",     canceledStr );
		setNodeAttr( itemElem,  "quantity",     IntToStr( inQty )  );

		TiXmlElement *tmpElem;
		addElement(  itemElem, "Sides",       tmpElem );

		//::::::::::::::::::::::::::::

		result = ( HITEM )itemElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Print Format to an Order
HPFORMAT TSaveXMLOrder::AddItemPrintingFormat( HITEM      inItemHandle,
											   __int32    inDBKey,
											   bool	      inUnderline,
											   bool	      inBold,
											   TColor	  inColor,
											   bool	      inLargeFont,
											   bool	      inDoubleWidth,
											   bool	      inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
	HPARENT parentHandle = ( HPARENT )inItemHandle;
	result = addPrintingFormat( parentHandle,
								inDBKey,
								inUnderline,
								inBold,
								inColor,
								inLargeFont,
								inDoubleWidth,
							    inDoubleHeight );
	}
	catch( Exception &e )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM_PFORMAT );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Option to an Order
HOPTION TSaveXMLOrder::AddItemOption( HITEM      inItemHandle,
									  __int32    inDBKey,
									  AnsiString inName,
									  AnsiString inKitchenName,
									  bool	     inIsPlus )
{
	HOPTION result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		HPARENT itemParent = ( HPARENT )inItemHandle;
		result = addOption( itemParent,
							inDBKey,
							inName,
							inKitchenName,
							inIsPlus );
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM_OPTION );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Option Print Format to an Order
HPFORMAT TSaveXMLOrder::AddItemOptionPrintingFormat( HOPTION inOptionHandle,
													 __int32 inDBKey,
													 bool	  inUnderline,
													 bool	  inBold,
													 TColor  inColor,
													 bool	  inLargeFont,
													 bool	  inDoubleWidth,
												     bool	  inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
	HPARENT parentHandle = ( HPARENT )inOptionHandle;
	result = addPrintingFormat( parentHandle,
								inDBKey,
								inUnderline,
								inBold,
								inColor,
								inLargeFont,
								inDoubleWidth,
							    inDoubleHeight );
	}
	catch( Exception &e )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM_OPTION_PFORMAT );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Side to an Order
HSIDE TSaveXMLOrder::AddItemSide( HITEM      inItemHandle,
								  __int32    inOrderSideKey,
								  AnsiString inName,
								  AnsiString inKitchenName )
{
	HSIDE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *itemElem  = ( TiXmlElement* )inItemHandle;
		TiXmlElement *sidesElem = itemElem->FirstChildElement()->NextSiblingElement()->NextSiblingElement();
		TiXmlElement *sideElem;

		//::::::::::::::::::::::::::::

		addElement(  sidesElem, "Side",        sideElem );
		setNodeAttr( sideElem,  "key",         IntToStr( inOrderSideKey ) );
		setNodeAttr( sideElem,  "name",        inName );
		setNodeAttr( sideElem,  "kitchenName", inKitchenName );

		TiXmlElement *tmpElem;
		addElement(  sideElem, "PrintFormat", tmpElem );
		addElement(  sideElem, "Options",     tmpElem );

		//::::::::::::::::::::::::::::

		result = ( HSIDE )sideElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM_SIDE );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Side to an Order
HSIDE TSaveXMLOrder::AddItemSideToCancel( HITEM      inItemHandle,
										  __int32    inOrderSideKey,
										  AnsiString inSideName )
{
	HSIDE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TiXmlElement *itemElem  = ( TiXmlElement* )inItemHandle;
		TiXmlElement *sidesElem = itemElem->FirstChildElement();
		TiXmlElement *sideElem;

		//::::::::::::::::::::::::::::

		addElement(  sidesElem, "Side", sideElem );
		setNodeAttr( sideElem,  "key",  IntToStr( inOrderSideKey ) );
		setNodeAttr( sideElem,  "name", inSideName );

		//::::::::::::::::::::::::::::

		result = ( HSIDE )sideElem;
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_ITEM_SIDE );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Option's Print Format to an Order
HPFORMAT TSaveXMLOrder::AddSidePrintingFormat( HSIDE      inSideHandle,
											   __int32    inDBKey,
											   bool		  inUnderline,
											   bool	      inBold,
											   TColor	  inColor,
											   bool		  inLargeFont,
											   bool		  inDoubleWidth,
											   bool		  inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
	HPARENT parentHandle = ( HPARENT )inSideHandle;
	result = addPrintingFormat( parentHandle,
								inDBKey,
								inUnderline,
								inBold,
								inColor,
								inLargeFont,
								inDoubleWidth,
							    inDoubleHeight );
	}
	catch( Exception &e )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_SIDE_PFORMAT );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Side's Option to an Order
HOPTION TSaveXMLOrder::AddSideOption( HSIDE      inSideHandle,
									  __int32    inDBKey,
									  AnsiString inName,
									  AnsiString inKitchenName,
									  bool	     inIsPlus )
{
	HOPTION result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		HPARENT itemParent = ( HPARENT )inSideHandle;
		result = addOption( itemParent,
							inDBKey,
							inName,
							inKitchenName,
							inIsPlus );
	}
	catch( Exception &e  )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_SIDE_OPTION );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Option's Print Format to an Order
HPFORMAT TSaveXMLOrder::AddSideOptionPrintingFormat( HOPTION    inOptionHandle,
													 __int32    inDBKey,
									  				 bool		inUnderline,
													 bool		inBold,
													 TColor	    inColor,
													 bool		inLargeFont,
													 bool		inDoubleWidth,
												 	 bool		inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
	HPARENT parentHandle = ( HPARENT )inOptionHandle;
	result = addPrintingFormat( parentHandle,
							   inDBKey,
							   inUnderline,
							   inBold,
							   inColor,
							   inLargeFont,
							   inDoubleWidth,
							   inDoubleHeight );
	}
	catch( Exception &e )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_ADD_SIDE_OPTION_PFORMAT );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Saves an Order
OERROR TSaveXMLOrder::CommitOrder( HostnameLIST inHostnameList )
{
	OERROR result = OERROR_SUCCESSFUL;

	//:::::::::::::::::::::::::::::::::::::::::

	try
	{
		result = sendOrderToCheftMate(
					_xmlOrderDoc,
					inHostnameList );
	}
	catch( Exception &e )
	{
		throw new ESaveXMLOrder( e.Message, OERROR_FAILED_CLASS_COMMIT_ORDER );
	}

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

__int32 TSaveXMLOrder::GetOrderNumber()
{
	return StrToInt( _rootElem->Attribute( "orderNumber" ) );
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::

void TSaveXMLOrder::_setAllowServingCourseDuplicate( bool inAllow )
{
	FAllowServingCourseDuplicate = inAllow;
}
//......................................................

void TSaveXMLOrder::_setAllowCourseDuplicate( bool inAllow )
{
	FAllowCourseDuplicate = inAllow;
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::

TiXmlDocument* TSaveXMLOrder::createXMLOrderDoc()
{
    TiXmlDocument* result = new TiXmlDocument();

	//::::::::::::::::::::::::::::::

    // add declaration
	TiXmlDeclaration *decl = new TiXmlDeclaration(_T("1.0"), _T("ANSI_CHARSET"), _T("")); //new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	result->LinkEndChild( decl );

    //::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
void TSaveXMLOrder::addMainElements()
{
	addElement( ( TiXmlElement* )_xmlOrderDoc, "Order",          _rootElem );
	addElement( _rootElem,                    "ServingCourses", _servingCoursesElem );
	addElement( _rootElem,                    "Courses",        _coursesElem );
	addElement( _rootElem,                    "Items",          _itemsElem );
}
//---------------------------------------------------------------------------
void TSaveXMLOrder::addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------

void TSaveXMLOrder::setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue )
{
    //MessageBox( inAttrName, "attribute name", MB_OK + MB_ICONWARNING);
    //MessageBox( inAttrValue, "value", MB_OK + MB_ICONWARNING);
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------

TiXmlElement* TSaveXMLOrder::servingCourseWithKey( __int32 inDBKey )
{
	TiXmlElement* result = NULL;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	TiXmlElement* scElement = _servingCoursesElem->FirstChildElement();

	while( scElement != NULL )
	{
	   if( StrToInt( scElement->Attribute( "key" ) )  == inDBKey )
	   {
		   result =  scElement;
		   break;
	   }

	   scElement = scElement->NextSiblingElement();
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//----------------------------------------------------------------------------

TiXmlElement* TSaveXMLOrder::courseWithKey( __int32 inDBKey )
{
	TiXmlElement* result = NULL;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	TiXmlElement* cElement = _coursesElem->FirstChildElement();

	while( cElement != NULL )
	{
	   if( StrToInt( cElement->Attribute( "key" ) ) == inDBKey )
	   {
		   result =  cElement;
		   break;
	   }

	   cElement = cElement->NextSiblingElement();
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//----------------------------------------------------------------------------

// Adds a Print Format to an Order
HPFORMAT TSaveXMLOrder::addPrintingFormat( HPARENT inParentmHandle,
										   __int32 inDBKey,
										   bool	   inUnderline,
										   bool	   inBold,
										   TColor  inColor,
										   bool	   inLargeFont,
										   bool	   inDoubleWidth,
										   bool	   inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	TiXmlElement *parentElem  = ( TiXmlElement* )inParentmHandle;
	TiXmlElement *pFormatElem = parentElem->FirstChildElement();

	//::::::::::::::::::::::::::::

	setNodeAttr( pFormatElem, "key",          IntToStr( inDBKey ) );
	setNodeAttr( pFormatElem, "underline",    inUnderline    ? "true" : "false" );
	setNodeAttr( pFormatElem, "bold",         inBold         ? "true" : "false" );
	setNodeAttr( pFormatElem, "color",        IntToStr( ( int )inColor ) );
	setNodeAttr( pFormatElem, "largeFont",    inLargeFont    ? "true" : "false" );
	setNodeAttr( pFormatElem, "doubleWidth",  inDoubleWidth  ? "true" : "false" );
	setNodeAttr( pFormatElem, "doubleHeight", inDoubleHeight ? "true" : "false" );

	//::::::::::::::::::::::::::::

	result = ( HPFORMAT )pFormatElem;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds an Option to an Order
HOPTION TSaveXMLOrder::addOption( HPARENT    inParentHandle,
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
OERROR TSaveXMLOrder::sendOrderToCheftMate(
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
													( UnicodeString )*cmIT,
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

OERROR TSaveXMLOrder::responseToErrorResult( TiXmlDocument *inXMLResponseDoc )
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

AnsiString TSaveXMLOrder::orderFileName( HORDER inOrderHandle )
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

AnsiString TSaveXMLOrder::xmlDateToStr( TDateTime inDateTime )
{
	return inDateTime.FormatString( "dd/mm/yyyy hh:nn:ss " );
}
//......................................................
