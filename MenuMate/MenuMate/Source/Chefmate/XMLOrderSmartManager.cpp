//---------------------------------------------------------------------------

#pragma hdrstop

#include "XMLOrderSmartManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TSmartOrderDocHeader
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TSmartOrderDocHeader::TSmartOrderDocHeader(
						AnsiString inAction,
						AnsiString inType,
						__int32    inDBKey,
						AnsiString inServerName,
						__int32    inOrderNumber,
						AnsiString inChitValue,
						AnsiString inTableName,
						AnsiString inOrderType,
						AnsiString inCustomerName,
						AnsiString inPartyName,
						AnsiString inPatronCount,
						AnsiString inSaleStartTime,
                        AnsiString inDeliveryTime,
                        AnsiString inPhone,
                        AnsiString inEmail,
                        AnsiString inAddress,
                        AnsiString inPaymentStatus,
                        AnsiString inSourceTableName )
{
	FAction       = inAction;
	FType         = inType,
	FDBKey        = inDBKey,
	FServerName   = inServerName,
	FOrderNumber  = inOrderNumber,
	FChitValue    = inChitValue,
	FTableName    = inTableName,
	FOrderType    = inOrderType,
	FCustomerName = inCustomerName,
	FPartyName    = inPartyName;
	FPatronCount  = inPatronCount;
	FSaleStartTime = inSaleStartTime;
    FDeliveryTime =  inDeliveryTime;
    FSourceTableName = inSourceTableName;
    FPhone           = inPhone;
    FEmail           = inEmail;
    FAddress       = inPaymentStatus;
    FPaymentStatus   = inPaymentStatus;
}
//......................................................

TSmartOrderDocHeader::~TSmartOrderDocHeader()
{
}
//......................................................

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TSmartOrderDoc
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TSmartOrderDoc::TSmartOrderDoc( TSmartOrderDocHeader* inHeader )
{
	FHeader = createHeader( inHeader );
}
//......................................................

TSmartOrderDoc::~TSmartOrderDoc()
{

}
//......................................................

TSaveXMLOrder* TSmartOrderDoc::XmlDocWithHostname( TCMHostname inHostName )
{
	TSaveXMLOrder* result;
	std::map<TCMHostname, TSaveXMLOrder>::const_iterator orderIT = 	_xmlOrderMap.find( inHostName );

	if( orderIT == _xmlOrderMap.end() )  // XML Doc not found!!!
	{
		result = _createOrderXMLDoc();
		_xmlOrderMap[inHostName] = *result;
	}
	else
	{
		result = &_xmlOrderMap[inHostName];
	}
	return result;
}
//......................................................

__int32 TSmartOrderDoc::GetOrderNumber()
{
	return FHeader->OrderNumber;
}
//......................................................

void TSmartOrderDoc::HostnameList( std::vector<TCMHostname>& outList )
{
	outList.clear();

	map<TCMHostname, TSaveXMLOrder>::const_iterator orderIT;

	for( orderIT = _xmlOrderMap.begin(); orderIT != _xmlOrderMap.end(); orderIT++ )
	{
		outList.push_back( orderIT->first );
	}
}
//......................................................

// PROTECTED METHODS
TSaveXMLOrder* TSmartOrderDoc::_createOrderXMLDoc()
{
	TSaveXMLOrder* result = TSaveXMLOrderBuilder::CreateSaveXMLOrderDoc();

	result->AllowServingCourseDuplicate = false;
	result->AllowCourseDuplicate        = false;

	openOrder( result );
	return result;
}
//......................................................

// PRIVATE METHODS
TSmartOrderDocHeader* TSmartOrderDoc::createHeader( TSmartOrderDocHeader* inHeader )
{
	 return new TSmartOrderDocHeader(
						inHeader->Action,
						inHeader->Type,
						inHeader->DBKey,
						inHeader->ServerName,
						inHeader->OrderNumber,
						inHeader->ChitValue,
						inHeader->TableName,
						inHeader->OrderType,
						inHeader->CustomerName,
						inHeader->PartyName,
						inHeader->PatronCount,
						inHeader->SaleStartTime,
                        inHeader->DeliveryTime,
                        inHeader->SourceTableName,
                        inHeader->email,
                        inHeader->phone,
                        inHeader->address,
                        inHeader->paymentStatus );
}
//......................................................

void TSmartOrderDoc::openOrder( TSaveXMLOrder* inOrderDoc )
{
	if( FHeader->Type.UpperCase() == "INCOMPLETE" )
	{
		inOrderDoc->OpenIncompleteOrder(
						FHeader->DBKey,
						FHeader->ServerName,
						FHeader->OrderNumber,
						FHeader->ChitValue,
						FHeader->TableName,
						FHeader->OrderType,
						FHeader->CustomerName,
						FHeader->PartyName );
	}
	else
	{
		openCompleteOrder( inOrderDoc );
	}
}
//......................................................

void TSmartOrderDoc::openCompleteOrder( TSaveXMLOrder* inOrderDoc )
{
	if( FHeader->Action.UpperCase() == "ORDER" )
	{
		inOrderDoc->OpenCompleteOrder(
						FHeader->DBKey,
						FHeader->ServerName,
						FHeader->OrderNumber,
						FHeader->ChitValue,
						FHeader->TableName,
						FHeader->OrderType,
						FHeader->CustomerName,
						FHeader->PartyName,
						FHeader->PatronCount,
						FHeader->SaleStartTime,
                        FHeader->DeliveryTime,
                        FHeader->phone,
                        FHeader->email,
                        FHeader->address,
                        FHeader->paymentStatus
                        );
	}
	else if( FHeader->Action.UpperCase() == "CANCEL" )
	{
		inOrderDoc->OpenCancelOrder(
						FHeader->DBKey,
						FHeader->OrderNumber,
                        FHeader->TableName );
	}
	else if( FHeader->Action.UpperCase() == "CREDIT" )
	{
		inOrderDoc->OpenCreditOrder();
	}
	else if( FHeader->Action.UpperCase() == "REPLACE" )
	{
		inOrderDoc->OpenReplacementOrder(
						FHeader->DBKey,
						FHeader->OrderNumber,
						FHeader->TableName );
	}
	else if( FHeader->Action.UpperCase() == "CALLAWAY" )
	{
		inOrderDoc->OpenCallAwayOrder(
						FHeader->OrderNumber,
						FHeader->TableName );
	}
	else if( FHeader->Action.UpperCase() == "TRANSFER" )
	{
		inOrderDoc->OpenTransferOrder(
						FHeader->DBKey,
						FHeader->OrderNumber,
						FHeader->TableName,
                        FHeader->ServerName,
						FHeader->PatronCount,
						FHeader->SaleStartTime,
                        FHeader->SourceTableName );
	}
}
//......................................................

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TXMLOrderSmartManager
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TXMLOrderSmartManager::TXMLOrderSmartManager()
{
	_lastOError = OERROR_NONE;
}
//......................................................

TXMLOrderSmartManager::~TXMLOrderSmartManager()
{
}
//......................................................

// Open a Complete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDERDOC TXMLOrderSmartManager::OpenCompleteOrder(
									__int32    inDBKey,
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
                                    AnsiString inPaymentStatus )
{
	HORDERDOC result = 0;

	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
            if(inPaymentStatus == "")
            {
                inPaymentStatus =  "order";
            }
                TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
                                                        new TSmartOrderDocHeader(
                                                                "Order",
                                                                "complete",   //todo
                                                                inDBKey,
                                                                inServerName,
                                                                inOrderNumber,
                                                                inChitValue,
                                                                inTableTabName,
                                                                inOrderType,
                                                                "Rupendra",
                                                                inPartyName,
                                                                inPatronCount,
                                                                inSaleStartTime,
                                                                inDeliveryTime,
                                                                "05661694658",         //todo
                                                                "rupendrapanday@gmail.com",
                                                                "B-228 Sector-92 Noida",
                                                                "Paid" ) );

			result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
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

	return result;
}
//......................................................

// Opens an Incomplete Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDERDOC TXMLOrderSmartManager::OpenIncompleteOrder(
									__int32    inDBKey,
									AnsiString inServerName,
									__int32    inOrderNumber,
									AnsiString inChitValue,
									AnsiString inTableTabName,
									AnsiString inOrderType,
									AnsiString inCustomerName,
									AnsiString inPartyName )
{
	HORDERDOC result = 0;
	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
			TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
													new TSmartOrderDocHeader(
															"order",
															"incomplete",
															inDBKey,
															inServerName,
															inOrderNumber,
															inChitValue,
															inTableTabName,
															inOrderType,
															inCustomerName,
															inPartyName,
															"",
															"",
                                                            "",
                                                            "",
                                                            "",
                                                            "",
                                                            "" ) );

			result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
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
HORDERDOC TXMLOrderSmartManager::OpenCancelOrder(
									__int32 inDBKey,
									__int32 inOrderNumber,
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
			TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
													new TSmartOrderDocHeader(
															"cancel",
															"complete",
															inDBKey,       // DB Key
															"",      // Server Name
															inOrderNumber,
															"",      // Chit Value
															inTableTabName,      // TableName,
															"",      // Order Type
															"",      // Customer Name
															"",      // Party Name
															"",      // Patron Count
															"",      // Sale Start Time
                                                            "",      //Delivery Time
                                                            "",      //phone
															"",      //eMail
															"",      // Address
                                                            ""  ) ); // paid status

			result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
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

	return result;
}
//......................................................

// Opens a Credit Order
// The items to be canceled must be added afterwards
// Once all the items are added SaveOrder() must be called
HORDERDOC TXMLOrderSmartManager::OpenCreditOrder()
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
												new TSmartOrderDocHeader(
														"credit",
														"complete",
														0,       // DB Key
														"",      // Server Name
														0,       // Order Number
														"",      // Chit Value
														"",      // TableName,
														"",      // Order Type
														"",      // Customer Name
														"",      // Party Name
														"",      // Patron Count
														"",      // Sale Start Time
                                                        "",
                                                        "",
                                                        "",
                                                        "",
                                                        ""  ) ); //Delivery Time

		result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
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
	return result;
}
//......................................................

// Open a Replacement Order
// Order's components must be added afterwards
// Once all the components SaveOrder() must be called
HORDER TXMLOrderSmartManager::OpenReplacementOrder(
									__int32    inDBKey,
									__int32    inOrderNumber,
									AnsiString inTableTabName )
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
												new TSmartOrderDocHeader(
														"replace",
														"complete",
														inDBKey,       		 // DB Key
														"",      	   		 // Server Name
														inOrderNumber,       // Order Number
														"",      			 // Chit Value
														inTableTabName,      // TableName,
														"",      			 // Order Type
														"",      			 // Customer Name
														"",      			 // Party Name
														"",                  // Patron Count
														"",      // Sale Start Time
                                                        "",
                                                        "",
                                                        "",
                                                        "",
                                                        ""  ) ); //Delivery Time

		result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_REPLACEMENT_ORDER;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//......................................................

// Open a Call Away Order
// Order's serving courses must be added afterwards
// Once all the serving courses are added SaveOrder() must be called
HORDER TXMLOrderSmartManager::OpenCallAwayOrder(
									__int32    inOrderNumber,
									AnsiString inTableTabName )
{
	HORDERDOC result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
												new TSmartOrderDocHeader(
														"callaway",
														"complete",
														0,       		 	 // DB Key
														"",      	   		 // Server Name
														inOrderNumber,       // Order Number
														"",      			 // Chit Value
														inTableTabName,      // TableName,
														"",      			 // Order Type
														"",      			 // Customer Name
														"",      			 // Party Name
														"",		 			 // Patron Count
														"",      // Sale Start Time
                                                        "",
                                                        "",
                                                        "",
                                                        "",
                                                        ""  ) ); //Delivery Time

		result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
		_lastOError = OERROR_SUCCESSFUL;
	}
	catch( ESaveXMLOrder &eOrder )
	{
		_lastOError = eOrder.Error;
	}
	catch( ... )
	{
		_lastOError = OERROR_FAILED_MANAGER_OPEN_REPLACEMENT_ORDER;
	}
	return result;
}
//......................................................

// Opens a Transfer Order
// SaveOrder() must be called straight away
HORDERDOC TXMLOrderSmartManager::OpenTransferOrder(
									__int32    inDBKey,
									__int32    inOrderNumber,
									AnsiString inTableTabName,
                                    AnsiString inServerName,
                                    AnsiString inPatronCount,
                                    AnsiString inSaleStartTime,
                                    AnsiString inSourceTableName )
{
	HORDERDOC result = 0;

	try
	{
		if( orderOpen( inOrderNumber) )
		{
			_lastOError = OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN;
		}
		else
		{
			TSmartOrderDoc* xmlSmartOrderDoc = createXMLSmartOrderDoc(
													new TSmartOrderDocHeader(
															"transfer",
															"complete",
															inDBKey,       // DB Key
															inServerName,      // Server Name
															inOrderNumber,       // Order Number
															"",      // Chit Value
															inTableTabName,      // TableName,
															"",      // Order Type
															"",      // Customer Name
															"",      // Party Name
															inPatronCount,      // Patron Count
															inSaleStartTime, // Sale Start Time
                                                            "" ,
                                                            "",
                                                            "",
                                                            "",
                                                            "",
                                                            inSourceTableName ) );//Delivery Time

			result      = addSmartOrderXMLDoc( xmlSmartOrderDoc );
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
	return result;
}
//......................................................

// Adds a ServingCourse to an Order
HSCOURSE TXMLOrderSmartManager::AddServingCourse(
									HORDERDOC   inOrderDocHandle,
									TCMHostname inHostname,
									__int32     inDBKey,
									AnsiString  inName,
									AnsiString  inKitchenName,
									__int32 	inDisplayOrder )
{
	HSCOURSE result = 0;
	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

		result = xmlOrderDoc->AddServingCourse( inDBKey,
												inName,
												inKitchenName,
												inDisplayOrder );
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
	return result;
}
//......................................................

// Adds a Course to an Order
HCOURSE TXMLOrderSmartManager::AddCourse(
								HORDERDOC   inOrderDocHandle,
								TCMHostname inHostname,
								__int32     inDBKey,
								AnsiString  inName,
								AnsiString  inKitchenName,
								__int32     inServingCourseKey,
								__int32 	inDisplayOrder )
{
	HCOURSE result = 0;
	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

		result = xmlOrderDoc->AddCourse( inDBKey,
										 inName,
										 inKitchenName,
									 	 inServingCourseKey,
										 inDisplayOrder );
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
	return result;
}
//......................................................

// Adds a Item to an Order
HITEM TXMLOrderSmartManager::AddItem(
								HORDERDOC   inOrderDocHandle,
								TCMHostname inHostname,
								__int32     inDBKey,
								__int32     inOrderItemDBKey,
                              AnsiString    inName,
							  AnsiString    inKitchenName,
								__int32     inServingCourseKey,
								__int32     inCourseKey,
							  AnsiString    inNote )
{
	HITEM result = 0;
  	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

       	result = xmlOrderDoc->AddItem( inDBKey,
									   inOrderItemDBKey,
									   inName,
									   inKitchenName,
									   inServingCourseKey,
									   inCourseKey,
									   inNote );

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

	return result;
}
//......................................................

// Adds a Item to cancel to an Order
// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
// If inCaceled is false then inQty is ignored
HITEM TXMLOrderSmartManager::AddItemToCancel(
								  HORDERDOC   inOrderDocHandle,
								  TCMHostname inHostname,
								  __int32     inOrderItemKey,
								  AnsiString  inItemName,
								  bool        inCanceled,
								  __int32     inQty )
{
	HITEM result = 0;

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(inOrderDocHandle, inHostname );

		result = xmlOrderDoc->AddItemToCancel(
								inOrderItemKey,
								inItemName,
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
	return result;
}
//......................................................

// Adds a Item's Print Format to an Order
HPFORMAT TXMLOrderSmartManager::AddItemPrintingFormat(
									HORDERDOC   inOrderDocHandle,
									TCMHostname inHostname,
									HITEM       inItemHandle,
									__int32     inDBKey,
									bool	    inUnderline,
									bool	    inBold,
									TColor	    inColor,
									bool	    inLargeFont,
									bool	    inDoubleWidth,
									bool	    inDoubleHeight )
{
	HPFORMAT result = 0;
	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

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
	return result;
}
//......................................................

// Adds a Item's Option to an Order
HOPTION TXMLOrderSmartManager::AddItemOption(
									HORDERDOC   inOrderDocHandle,
									TCMHostname inHostname,
									HITEM       inItemHandle,
									__int32     inDBKey,
									AnsiString  inName,
									AnsiString  inKitchenName,
									bool	    inIsPlus )
{
	HOPTION result = 0;
	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

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

	return result;
}
//......................................................

// Adds a Item's Option Print Format to an Order
HPFORMAT TXMLOrderSmartManager::AddItemOptionPrintingFormat(
									HORDERDOC   inOrderDocHandle,
									TCMHostname inHostname,
									HOPTION     inOptionHandle,
									__int32     inDBKey,
									bool	    inUnderline,
									bool	    inBold,
									TColor	    inColor,
									bool	    inLargeFont,
									bool	    inDoubleWidth,
									bool	    inDoubleHeight )
{
	HPFORMAT result = 0;

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

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

	return result;
}
//......................................................

// Adds a Item's Side to an Order
HSIDE TXMLOrderSmartManager::AddItemSide(
								HORDERDOC   inOrderDocHandle,
								TCMHostname inHostname,
								HITEM       inItemHandle,
								__int32     inOrderSideKey,
								AnsiString  inName,
								AnsiString  inKitchenName )
{
	HSIDE result = 0;
	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

		result = xmlOrderDoc->AddItemSide( inItemHandle,
										   inOrderSideKey,
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
	return result;
}
//......................................................

// Adds a Item's Side to an Order
HSIDE TXMLOrderSmartManager::AddItemSideToCancel(
								HORDERDOC   inOrderDocHandle,
								TCMHostname inHostname,
								HITEM       inItemHandle,
								__int32     inOrderSideKey,
								AnsiString  inSideName )
{
	HSIDE result = 0;
	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

		result = xmlOrderDoc->AddItemSideToCancel(
								inItemHandle,
								inOrderSideKey,
								inSideName );

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
HPFORMAT TXMLOrderSmartManager::AddSidePrintingFormat(
									HORDERDOC   inOrderDocHandle,
									TCMHostname inHostname,
									HSIDE       inSideHandle,
									__int32     inDBKey,
									bool	    inUnderline,
									bool	    inBold,
									TColor	    inColor,
									bool	    inLargeFont,
									bool	    inDoubleWidth,
									bool	    inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

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
HOPTION TXMLOrderSmartManager::AddSideOption(
								HORDERDOC   inOrderDocHandle,
								TCMHostname inHostname,
								HSIDE       inSideHandle,
								__int32     inDBKey,
								AnsiString  inName,
								AnsiString  inKitchenName,
								bool	    inIsPlus )
{
	HOPTION result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

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
HPFORMAT TXMLOrderSmartManager::AddSideOptionPrintingFormat(
									HORDERDOC   inOrderDocHandle,
									TCMHostname inHostname,
									HOPTION     inOptionHandle,
									__int32     inDBKey,
									bool	    inUnderline,
									bool	    inBold,
									TColor	    inColor,
									bool	    inLargeFont,
									bool	    inDoubleWidth,
									bool	    inDoubleHeight )
{
	HPFORMAT result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		TSaveXMLOrder* xmlOrderDoc = getXMLOrderDoc(
										inOrderDocHandle,
										inHostname );

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
OERROR TXMLOrderSmartManager::CommitOrder( HORDERDOC inOrderDocHandle )
{
	OERROR result = OERROR_SUCCESSFUL;
	try
	{
		std::vector<TCMHostname> hostnameList;
		std::vector<TCMHostname>::const_iterator hlIT;

		TSmartOrderDoc *smartOrderDoc = ( TSmartOrderDoc* )inOrderDocHandle;
		smartOrderDoc->HostnameList( hostnameList ) ;

		for( hlIT = hostnameList.begin(); hlIT != hostnameList.end(); hlIT++ )
		{
			TCMHostname hostname = ( TCMHostname )( *hlIT );
			TSaveXMLOrder* xmlOrderDoc = smartOrderDoc->XmlDocWithHostname( hostname );
			std::vector<TCMHostname> hostnameLIST;
			hostnameLIST.push_back( hostname );
			result = xmlOrderDoc->CommitOrder( hostnameLIST );
		}

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
	return result;
}
//......................................................

// Deletes an Order
OERROR TXMLOrderSmartManager::DeleteOrder( HORDERDOC inOrderDocHandle )
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

OERROR TXMLOrderSmartManager::GetLastOError()
{
	return _lastOError;
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TXMLOrderSmartManager::orderOpen( __int32 inOrderNumber )
{
	return findOrder( inOrderNumber ) != 0;
}
//---------------------------------------------------------------------------

HORDERDOC TXMLOrderSmartManager::findOrder( __int32 inOrderNumber )
{
	std::vector<TSmartOrderDoc>::iterator oIT;

	for( oIT = _xmlSmartOrderList.begin(); oIT != _xmlSmartOrderList.end(); oIT++ )
	{
		AnsiString orderNo = ( *oIT ).GetOrderNumber();
		if( orderNo == inOrderNumber )
		{
			return ( HORDERDOC )( &( ( TSmartOrderDoc )( *oIT ) ) );
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

TSmartOrderDoc* TXMLOrderSmartManager::createXMLSmartOrderDoc( TSmartOrderDocHeader* inHeader )
{
	return new TSmartOrderDoc( inHeader );
}
//---------------------------------------------------------------------------

HORDERDOC TXMLOrderSmartManager::addSmartOrderXMLDoc( TSmartOrderDoc* inSmartOrderDoc )
{
	_xmlSmartOrderList.push_back( *inSmartOrderDoc );
	return ( HORDERDOC )( &_xmlSmartOrderList[_xmlSmartOrderList.size() - 1] );
}
//---------------------------------------------------------------------------

 TSaveXMLOrder* TXMLOrderSmartManager::getXMLOrderDoc(
											HORDERDOC   inOrderDocHandle,
											TCMHostname inHostname )
{
	TSmartOrderDoc* smartOrderDoc = ( TSmartOrderDoc* )inOrderDocHandle;
	return smartOrderDoc->XmlDocWithHostname( inHostname );
}
//---------------------------------------------------------------------------

OERROR TXMLOrderSmartManager::deleteOrderDoc( HORDERDOC inOrderDocHandle )
{
	OERROR result = OERROR_FAILED_MANAGER_UKNOWN_ORDER;
	std::vector<TSmartOrderDoc>::iterator oIT;

	for( oIT = _xmlSmartOrderList.begin(); oIT != _xmlSmartOrderList.end(); oIT++ )
	{
		if( ( HORDERDOC )( &( ( TSmartOrderDoc )( *oIT ) ) ) == inOrderDocHandle )
		{
			_xmlSmartOrderList.erase( oIT );
			result = OERROR_SUCCESSFUL;
			break;
		}
	}
	return result;
}
//---------------------------------------------------------------------------

