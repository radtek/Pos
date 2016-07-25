//---------------------------------------------------------------------------

#pragma hdrstop

#include "XMLOrderManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TXMLOrderManager::TXMLOrderManager()
{
	_saveXMLOrderBuilder = TSaveXMLOrderBuilder::Instance();
	_lastOError = OERROR_NONE;
}
//......................................................

TXMLOrderManager::~TXMLOrderManager()
{
}
//......................................................

// Open a Complete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDERDOC TXMLOrderManager::OpenCompleteOrder( __int32    inDBKey,
											   AnsiString inServerName,
											   __int32    inOrderNumber,
											   AnsiString inChitValue,
											   AnsiString inTableTabName,
											   AnsiString inOrderType,
											   AnsiString inCustomerName,
											   AnsiString inPartyName )
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
			TSaveXMLOrder* xmlOrderDoc = createXMLOrderDoc();

			xmlOrderDoc->OpenCompleteOrder( inDBKey,
											inServerName,
											inOrderNumber,
											inChitValue,
											inTableTabName,
											inOrderType,
											inCustomerName,
											inPartyName );

			result      = addOrderXMLDoc( xmlOrderDoc );
			_lastOError = OERROR_SUCCESSFUL;
		}
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_COMPLETE_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens an Incomplete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDERDOC TXMLOrderManager::OpenIncompleteOrder( __int32    inDBKey,
												 AnsiString inServerName,
												 __int32    inOrderNumber,
												 AnsiString inChitValue,
												 AnsiString inTableTabName,
												 AnsiString inOrderType,
												 AnsiString inCustomerName,
												 AnsiString inPartyName )
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
			TSaveXMLOrder* xmlOrderDoc = createXMLOrderDoc();

			xmlOrderDoc->OpenIncompleteOrder( inDBKey,
											  inServerName,
											  inOrderNumber,
											  inChitValue,
											  inTableTabName,
											  inOrderType,
											  inCustomerName,
											  inPartyName );

			result      = addOrderXMLDoc( xmlOrderDoc );
			_lastOError = OERROR_SUCCESSFUL;
		}
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_INCOMPLETE_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens a Cancel Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
HORDERDOC TXMLOrderManager::OpenCancelOrder( __int32 inDBKey,
											 __int32 inOrderNumber )
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
			TSaveXMLOrder* xmlOrderDoc = createXMLOrderDoc();

			xmlOrderDoc->OpenCancelOrder( inDBKey, inOrderNumber );

			result      = addOrderXMLDoc( xmlOrderDoc );
			_lastOError = OERROR_SUCCESSFUL;
		}
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_CANCEL_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens a Credit Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
HORDERDOC TXMLOrderManager::OpenCreditOrder()
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = createXMLOrderDoc();

		xmlOrderDoc->OpenCreditOrder();

		result      = addOrderXMLDoc( xmlOrderDoc );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_CREDIT_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Opens a Transfer Order
// SaveOrder() must be called straight away
HORDERDOC TXMLOrderManager::OpenTransferOrder( __int32    inDBKey,
											   __int32    inOrderNumber,
											   AnsiString inTableTabName )
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
			TSaveXMLOrder* xmlOrderDoc = createXMLOrderDoc();

			xmlOrderDoc->OpenTransferOrder( inDBKey,
											inOrderNumber,
											inTableTabName );

			result      = addOrderXMLDoc( xmlOrderDoc );
			_lastOError = OERROR_SUCCESSFUL;
		}
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_TRANSFER_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a ServingCourse to an Order
HSCOURSE TXMLOrderManager::AddServingCourse( HORDERDOC  inOrderDocHandle,
											 __int32    inDBKey,
											 AnsiString inName,
											 AnsiString inKitchenName )
{
	HSCOURSE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddServingCourse( inDBKey,
												inName,
												inKitchenName );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_SERVING_COURSE;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Course to an Order
HCOURSE TXMLOrderManager::AddCourse( HORDERDOC  inOrderDocHandle,
									 __int32    inDBKey,
									 AnsiString inName,
									 AnsiString inKitchenName,
									 __int32    inServingCourseKey )
{
	HCOURSE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddCourse( inDBKey,
										 inName,
										 inKitchenName,
									 	 inServingCourseKey );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_COURSE;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item to an Order
HITEM TXMLOrderManager::AddItem( HORDERDOC  inOrderDocHandle,
								 __int32    inDBKey,
								 __int32    inOrderItemDBKey,
								 AnsiString inName,
								 AnsiString inKitchenName,
								 __int32    inServingCourseKey,
								 __int32    inCourseKey,
								 bool       inCanceled )
{
	HITEM result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddItem( inDBKey,
									   inOrderItemDBKey,
									   inName,
									   inKitchenName,
									   inServingCourseKey,
									   inCourseKey );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_ITEM;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item to cancel to an Order
HITEM TXMLOrderManager::AddItemToCancel( HORDERDOC   inOrderDocHandle,
										 TCMHostname inCMHostname,
										 __int32     inDBKey,
										 bool        inCanceled,
										 __int32     inQty )
{
	HITEM result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddItemToCancel(
								inDBKey,
								inCanceled,
								inQty );

		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_ITEM;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Print Format to an Order
HPFORMAT TXMLOrderManager::AddItemPrintingFormat( HORDERDOC inOrderDocHandle,
												  HITEM     inItemHandle,
												  __int32   inDBKey,
												  bool	    inUnderline,
												  bool	    inBold,
												  TColor	inColor,
												  bool	    inLargeFont,
												  bool	    inDoubleWidth,
												  bool	    inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddItemPrintingFormat( inItemHandle,
													 inDBKey,
													 inUnderline,
													 inBold,
													 inColor,
													 inLargeFont,
													 inDoubleWidth,
													 inDoubleHeight );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_ITEM_PFORMAT;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Option to an Order
HOPTION TXMLOrderManager::AddItemOption( HORDERDOC  inOrderDocHandle,
										 HITEM      inItemHandle,
										 __int32    inDBKey,
										 AnsiString inName,
										 AnsiString inKitchenName,
					   					 bool	    inIsPlus )
{
	HOPTION result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddItemOption( inItemHandle,
											 inDBKey,
											 inName,
											 inKitchenName,
										     inIsPlus );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_ITEM_OPTION;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Option Print Format to an Order
HPFORMAT TXMLOrderManager::AddItemOptionPrintingFormat( HORDERDOC inOrderDocHandle,
														HOPTION   inOptionHandle,
														__int32   inDBKey,
														bool	  inUnderline,
														bool	  inBold,
														TColor	  inColor,
														bool	  inLargeFont,
														bool	  inDoubleWidth,
													    bool	  inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddItemOptionPrintingFormat( inOptionHandle,
														   inDBKey,
														   inUnderline,
														   inBold,
														   inColor,
														   inLargeFont,
														   inDoubleWidth,
														   inDoubleHeight );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_ITEM_OPTION_PFORMAT;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Item's Side to an Order
HSIDE TXMLOrderManager::AddItemSide( HORDERDOC  inOrderDocHandle,
						   			 HITEM      inItemHandle,
									 __int32    inDBKey,
									 AnsiString inName,
				   					 AnsiString inKitchenName )
{
	HSIDE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddItemSide( inItemHandle,
										   inDBKey,
										   inName,
										   inKitchenName );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_SIDE;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Option's Print Format to an Order
HPFORMAT TXMLOrderManager::AddSidePrintingFormat( HORDERDOC inOrderDocHandle,
												  HSIDE     inSideHandle,
												  __int32   inDBKey,
												  bool	    inUnderline,
												  bool	    inBold,
												  TColor	inColor,
												  bool	    inLargeFont,
												  bool	    inDoubleWidth,
												  bool	    inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddSidePrintingFormat( inSideHandle,
													 inDBKey,
													 inUnderline,
													 inBold,
													 inColor,
													 inLargeFont,
													 inDoubleWidth,
													 inDoubleHeight );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_SIDE_PFORMAT;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Side's Option to an Order
HOPTION TXMLOrderManager::AddSideOption( HORDERDOC  inOrderDocHandle,
										 HSIDE      inSideHandle,
										 __int32    inDBKey,
										 AnsiString inName,
										 AnsiString inKitchenName,
									     bool	      inIsPlus )
{
	HOPTION result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddSideOption( inSideHandle,
											 inDBKey,
											 inName,
											 inKitchenName,
										     inIsPlus );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_SIDE_OPTION;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Adds a Option's Print Format to an Order
HPFORMAT TXMLOrderManager::AddSideOptionPrintingFormat( HORDERDOC inOrderDocHandle,
														HOPTION   inOptionHandle,
														__int32   inDBKey,
														bool	  inUnderline,
														bool	  inBold,
														TColor	  inColor,
														bool	  inLargeFont,
														bool	  inDoubleWidth,
														bool	  inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->AddSideOptionPrintingFormat( inOptionHandle,
														   inDBKey,
														   inUnderline,
														   inBold,
														   inColor,
														   inLargeFont,
														   inDoubleWidth,
														   inDoubleHeight );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_ADD_SIDE_OPTION_PFORMAT;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Commits an Order
OERROR TXMLOrderManager::CommitOrder(
							HORDERDOC    inOrderDocHandle,
							HostnameLIST inHostnameList )
{
	OERROR result = OERROR_SUCCESSFUL;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc( inOrderDocHandle );

		result = xmlOrderDoc->CommitOrder( inHostnameList );
		deleteOrderDoc( inOrderDocHandle );

		_lastOError = result;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_COMMIT_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Deletes an Order
OERROR TXMLOrderManager::DeleteOrder( HORDERDOC inOrderDocHandle )
{
	OERROR result = OERROR_SUCCESSFUL;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		result = deleteOrderDoc( inOrderDocHandle );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_DELETE_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

OERROR TXMLOrderManager::GetLastOError()
{
	return _lastOError;
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TXMLOrderManager::orderOpen( __int32 inOrderNumber )
{
	return findOrder( inOrderNumber ) != 0;
}
//---------------------------------------------------------------------------

TSaveXMLOrder* TXMLOrderManager::createXMLOrderDoc()
{
	return _saveXMLOrderBuilder.CreateSaveXMLOrderDoc();
}
//---------------------------------------------------------------------------

HORDERDOC TXMLOrderManager::addOrderXMLDoc( TSaveXMLOrder* inSaveXMLOrderDoc )
{
	_xmlOrderDocs.push_back( *inSaveXMLOrderDoc );

	return ( HORDERDOC )( &_xmlOrderDocs[_xmlOrderDocs.size() - 1] );
}
//---------------------------------------------------------------------------

HORDERDOC TXMLOrderManager::findOrder( __int32 inOrderNumber )
{
	std::vector<TSaveXMLOrder>::iterator oIT;

	for( oIT = _xmlOrderDocs.begin(); oIT != _xmlOrderDocs.end(); oIT++ )
	{
		AnsiString orderNo = ( *oIT ).GetOrderNumber();
		if( orderNo == inOrderNumber )
		{
			return ( HORDERDOC )( &( ( TSaveXMLOrder )( *oIT ) ) );
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

 TSaveXMLOrder* TXMLOrderManager::getXMLOrderDoc( HORDERDOC inOrderDocHandle )
{
	return ( TSaveXMLOrder* )inOrderDocHandle;
}
//---------------------------------------------------------------------------

OERROR TXMLOrderManager::deleteOrderDoc( HORDERDOC inOrderDocHandle )
{
	OERROR result = OERROR_FAILED_MANAGER_UKNOWN_ORDER;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	std::vector<TSaveXMLOrder>::iterator oIT;

	for( oIT = _xmlOrderDocs.begin(); oIT != _xmlOrderDocs.end(); oIT++ )
	{
		if( ( HORDERDOC )( &( ( TSaveXMLOrder )( *oIT ) ) ) == inOrderDocHandle )
		{
			_xmlOrderDocs.erase( oIT );

			result = OERROR_SUCCESSFUL;

			break;
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

