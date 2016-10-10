//---------------------------------------------------------------------------

#pragma hdrstop

#include "ChefmateClient.h"

#include <map>

#include "SelectDish.h"

#include "DBOrder.h"
#include "ItemSize.h"
#include "DeviceRealTerminal.h"
#include "ReqPrintJob.h"
#include "ChefmateClient.h"
#include "DBSaleTimes.h"
#include "DocketManager.h"
#include "VirtualPrinterManager.h"
#include "ListSecurityRefContainer.h"
#include "MMContactInfo.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TChefmateClient::~TChefmateClient()
{
	Close();
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CMC_ERROR TChefmateClient::Open(TIBDatabase* inIBDatabase,TChitNumber* inChitNumber )
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	chefmateSetDB( inIBDatabase );
	if( chefmateAvailable())
	{
		if( chefmateInit() )
		{
			chefmateClear();
			chefmateInitOrderNumberGenerator();
			clearCustomerNameAndOrderType();
			_chitNumber = inChitNumber;
			result = CMC_ERROR_SUCCESSFUL;
		}
	}
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::Close()
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( chefmateOpen() )
	{
		ChefmateInterface.Done();
		result = CMC_ERROR_SUCCESSFUL;
	}
	else
	{
    	result = CMC_ERROR_CLIENT_ALREADY_CLOSED;
    }

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCompleteOrder( TPaymentTransaction* inTransaction)
{
	CMC_ERROR result = CMC_ERROR_FAILED;

	if( inTransaction->Orders->Count == 0 )
	{
		return SendCompleteEmptyOrder();
	}

    TItemComplete *order = ( TItemComplete* )inTransaction->Orders->Items[0];
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	if(ChefmateInterface.OpenCompleteOrder(
                            cmOrderDBKey( order ),              // Order Key
                            cmStaffName(order),                     // Server Name
                            cmOrderNumber(),                    // Order Number
                            cmChitValue(     inTransaction ),   // Chit Value
                            cmTableTabName(  order ),           // Table/Tab Name
                            cmOrderType(     inTransaction ),   // Order Type
                            cmCustomerName(  inTransaction ),   // Customer Name
                            cmPartyName(     order ),           // Party Name
                            cmPatronCount(   inTransaction ),	// Patron Count
                            cmSaleStartTime( order ),           // Sale Start Time
                            cmDeliveryTime(inTransaction),      // Delivery Time
                            cmCustomerPhone(  inTransaction ),   // Phone no
                            "",  //Email //if we want to send email then  send  cmCustomerEmail(  inTransaction )
                            cmCustomerAddress(  inTransaction )  )) // Customer Address
	{
    	for( int i = 0; i < inTransaction->Orders->Count; i++ )
		{
			TItemComplete *order1 = ( TItemComplete* )inTransaction->Orders->Items[i];
			orderItemToChefmate( order1 );
		}
    	commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCreditOrder( TContainerOrders* inOrders )
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( inOrders->Count == 0 )
	{
		return SendCompleteEmptyOrder();
	}
	if( ChefmateInterface.OpenCreditOrder() )
	{
		for (int i = 0; i < inOrders->Count; ++i)
		{
			TItemComplete *item =  reinterpret_cast<TItemComplete *>( inOrders->Items[i] );
			orderItemToChefmate(item);
		}
		commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCreditOrder( TList* inOrdersList )
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( inOrdersList->Count == 0 )
	{
		return SendCompleteEmptyOrder();
	}
	if( ChefmateInterface.OpenCreditOrder() )
	{
		for (int i = 0; i < inOrdersList->Count; ++i)
		{
			TItemComplete *item =  reinterpret_cast<TItemComplete *>( inOrdersList->Items[i] );
			orderItemToChefmate(item);
		}
		commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendReplacementOrder( TPaymentTransaction* inTransaction )
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( inTransaction->Orders->Count == 0 )
	{
		return CMC_ERROR_NO_ITEMS_TO_REPLACE;
	}
	TItemComplete *order = ( TItemComplete* )inTransaction->Orders->Items[0];

	if( ChefmateInterface.OpenReplacementOrder(cmOrderDBKey( order ),cmOrderNumber(),cmTableTabName(order)))
	{
		for( int i = 0; i < inTransaction->Orders->Count; i++ )
		{
			order = ( TItemComplete* )inTransaction->Orders->Items[i];
			orderItemToChefmate( order );
		}
		commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCallAwayOrder( TCallAwayComplete* inCallAway )
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( ChefmateInterface.OpenCallAwayOrder(cmOrderNumber(),inCallAway->TableName ) )
	{
		for( int i = 0; i < inCallAway->Courses->Count; i++ )
		{
			UnicodeString servingCourseName = inCallAway->Courses->Strings[i];
			callAwayServingCourseToChefmate( servingCourseName );
		}
		commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCompleteEmptyOrder()
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( ChefmateInterface.OpenCompleteOrder(
			0,
			cmServerName(),
			cmOrderNumber(),
			cmChitValue(),
			"",                // Table/Tab Name
			cmOrderType(),
			cmCustomerName(),
			"",                // Party Name
			"",                // Patron Count
			"",                // Sale Start Time
            cmDeliveryTime() ,  //Delivery Time
            "",   // Phone no
            "", //Email
            "" ))  //Customer Address
	{
		commitOrder();
		result = CMC_ERROR_EMPTY_ORDER_SENT;
	}
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCancelOrder( TList* inElementsToCancel,AnsiString inTabTableName )
{
	CMC_ERROR result = CMC_ERROR_FAILED;
	if( inElementsToCancel->Count == 0 )
	{
		return CMC_ERROR_NO_ITEMS_TO_CANCEL;
	}
	TPnMOrder* element = ( TPnMOrder* )inElementsToCancel->Items[0];
    if( ChefmateInterface.OpenCancelOrder(element->TabKey, cmOrderNumber(),inTabTableName) )
        {
            ITEM_TO_CANCEL_MAP oc;  // Orders to Cancel
            ITEM_TO_CANCEL_MAP::const_iterator ocIT;
            populateOrdersToCancel( inElementsToCancel, oc );
            for( ocIT = oc.begin(); ocIT != oc.end(); ocIT++ )
            {
               orderItemToCancelToChefmate( ocIT->first, ocIT->second );
            }
            result = CMC_ERROR_SUCCESSFUL;
        }
	commitOrder();
	TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendTransferOrder( TList* orderList ,UnicodeString SourceTableName )
{
    CMC_ERROR result = CMC_ERROR_FAILED;
	if( orderList->Count == 0 )
	{
		return CMC_ERROR_NO_ORDER_TO_TRANSFER;
	}
	TItemComplete *order = ( TItemComplete* )orderList->Items[0];

	if( ChefmateInterface.OpenTransferOrder(
			                                cmOrderDBKey( order ),              // Order Key
                                            cmOrderNumber(),                    // Order Number
                                            cmTableTabName(  order ),           // Table/Tab Name
                                            cmServerName(),                     // Server Name
                                            cmPatronCount(   orderList ),	    // Patron Count
                                            cmSaleStartTime( order ),
                                            SourceTableName ) )        // Sale Start Time
	{
		for( int i = 0; i < orderList->Count; i++ )
		{
			order = ( TItemComplete* )orderList->Items[i];
			orderItemToChefmate( order );
		}
		commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}
	return result;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TChefmateClient::chefmateSetDB( TIBDatabase *inIBDatabase )
{
	ChefmateInterface.SetDatabase( inIBDatabase );
}
//---------------------------------------------------------------------------
bool TChefmateClient::chefmateInit()
{
	return ChefmateInterface.Init();
}
//---------------------------------------------------------------------------
void TChefmateClient::chefmateClear()
{
	ChefmateInterface.Clear();
}
//---------------------------------------------------------------------------
void TChefmateClient::chefmateInitOrderNumberGenerator()
{
	// This is a temporary solution for generating Order Numbers.
	UnicodeString terminalName = TDeviceRealTerminal::Instance().ID.Name;

	TChefmateOrderNumberGenerator::Instance().InitOrderNumber( terminalName );
}
//---------------------------------------------------------------------------
bool TChefmateClient::chefmateAvailable()
{
	return ChefmateInterface.Available();
}
//---------------------------------------------------------------------------
bool TChefmateClient::chefmateOpen()
{
	return ChefmateInterface.Open;
}
//---------------------------------------------------------------------------
void TChefmateClient::populateOrdersToCancel(
						TList*              inElementsToCancel,
						ITEM_TO_CANCEL_MAP& outOrdersToCancel )
{
	outOrdersToCancel.clear();

	if( inElementsToCancel->Count == 0 )
		return;

	try
	{
		//..................................................................

		TList *savedOrderList = new TList();
		readSavedOrdersFromElements( inElementsToCancel, savedOrderList );

		//..................................................................

		for( int i = 0; i < inElementsToCancel->Count; i++ )
		{
			TPnMOrder* element = ( TPnMOrder* )inElementsToCancel->Items[i];
            for(int i = 0; i < element->Qty ; i++)
            {
                TItemComplete    *order;
                TItemCompleteSub *side;

                if( elementIsItem( element ) )
                {
                    if( orderFromElement( element, savedOrderList, order ) )
                    {
                        addToOrdersToCancelMap( order, outOrdersToCancel );
                    }
                }
                else
                {
                    if( orderAndSideFromElement( element, savedOrderList, order, side ) )
                    {
                        addToSidesToCancelMap( order, side, outOrdersToCancel );
                    }
                }
            }
		}
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::readSavedOrdersFromElements(
						TList* inElementsToCancel,
						TList* outSavedOrderList )
{
	// Set of Orders Key: No duplication;
	std::set<__int64> setOfOrderKeys;

	for( int i = 0; i < inElementsToCancel->Count; i++ )
	{
	   setOfOrderKeys.insert( ( ( TPnMOrder* )inElementsToCancel->Items[i] )->TabKey );
	}

	//.......................................................................

	Database::TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl );

	transaction.StartTransaction();

	TDBOrder::GetOrdersIncludingSidesFromTabKeys(
					transaction,
					outSavedOrderList,
					setOfOrderKeys );

	transaction.Commit();
}
//---------------------------------------------------------------------------
bool TChefmateClient::elementIsItem( TPnMOrder* element )
{
	return !element->IsSide;
}
//---------------------------------------------------------------------------
bool TChefmateClient::orderFromElement(
						TPnMOrder*      inElement,
						TList*			inSavedOrderList,
						TItemComplete* &outOrder )
{
	bool result = false;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	for( int i = 0; i < inSavedOrderList->Count; i++ )
	{
		TItemComplete* savedOrder = ( TItemComplete* )inSavedOrderList->Items[i];

		if( savedOrder->OrderKey == inElement->Key )
		{
			outOrder = savedOrder;
			result   = true;

			break;
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TChefmateClient::orderAndSideFromElement(
						TPnMOrder*         inElement,
						TList*			   inSavedOrderList,
						TItemComplete*    &outOrder,
						TItemCompleteSub* &outSide )
{
	bool result = false;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	for( int i = 0; i < inSavedOrderList->Count; i++ )
	{
		TItemComplete*    savedOrder = ( TItemComplete* )inSavedOrderList->Items[i];
		TItemCompleteSub* saveSide;

		if( elementIsSideInThisOrder( inElement, savedOrder, saveSide ) )
		{
			outOrder = savedOrder;
			outSide  = saveSide;
			result   = true;

			break;
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TChefmateClient::elementIsSideInThisOrder(
						TPnMOrder*         inElement,
						TItemComplete*     inOrder,
						TItemCompleteSub* &outSide )
{
	 if( inElement->TabKey != ( ( TItemComplete* )inOrder )->TabKey )
	 {
		 return false;
	 }

	//......................................................

	bool result = false;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::

	for( int i = 0; i < inOrder->SubOrders->Count; i++ )
	{
		TItemCompleteSub *side = ( TItemCompleteSub* )inOrder->SubOrders->Items[i];

		if( side->OrderKey == inElement->Key )
		{
			outSide = side;
			result  = true;

			break;
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TChefmateClient::addToSidesToCancelMap(
						TItemComplete*      inOrder,
						TItemCompleteSub*   inSide,
						ITEM_TO_CANCEL_MAP& outOrdersToCancel )
{
	// In this case HITEM is used to cast TItemComplete objects.
	// It's not the same as HITEM in ChefmateManager

	// In this case HSIDE is used to cast TItemCompleteSub objects.
	// It's not the same as HSIDE in ChefmateManager

	ITEM_TO_CANCEL_MAP::const_iterator mapIT =
		outOrdersToCancel.find( ( HITEM )inOrder );

	if( mapIT == outOrdersToCancel.end() ) // inOrder needs to be added to the MAP
	{
		HITEM_TO_CANCEL_INFO *info = new HITEM_TO_CANCEL_INFO();
		info->Qty      = 0;
		info->Canceled = false;
		info->HSideList.push_back( ( HSIDE )inSide );

		outOrdersToCancel[( HITEM )inOrder] = info;
	}
	else
	{
		HITEM_TO_CANCEL_INFO *info = mapIT->second;

		info->HSideList.push_back( ( HSIDE )inSide );
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::addToOrdersToCancelMap(
						TItemComplete*      inOrder,
						ITEM_TO_CANCEL_MAP& outOrdersToCancel )
{
	// In this case HITEM is used to cast TItemComplete objects.
	// It's not the same as HITEM in ChefmateManager

	// In this case HSIDE is used to cast TItemCompleteSub objects.
	// It's not the same as HSIDE in ChefmateManager

	ITEM_TO_CANCEL_MAP::const_iterator mapIT =  outOrdersToCancel.find( ( HITEM )inOrder );

	if( mapIT == outOrdersToCancel.end() ) // inOrder needs to be added to the MAP
	{
		HITEM_TO_CANCEL_INFO *info = new HITEM_TO_CANCEL_INFO();
		info->Qty      = 1;
		info->Canceled = true;

		outOrdersToCancel[( HITEM )inOrder] = info;
	}
	else
	{
		HITEM_TO_CANCEL_INFO *info = mapIT->second;
		info->Qty      += 1;
		info->Canceled =  true;
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemToChefmate( TItemComplete *inOrderItem )
{
	if ( inOrderItem->OriginalItemKey == NULL && inOrderItem->ItemOrderedFrom != NULL )
	{
		inOrderItem->OriginalItemKey = inOrderItem->ItemOrderedFrom->ItemKey;

    }
    else
    {
       inOrderItem->OriginalItemKey = inOrderItem->ItemKey;

    }

	if ( inOrderItem->OriginalItemKey != NULL )
	{

		__int32 itemSizeKey =
			TDBChefmate::GetItemSizeKey( inOrderItem->OriginalItemKey, inOrderItem->Size );
		__int32 itemOrderKey = inOrderItem->OrderKey;

		//.........................................................

		UnicodeString itemName =
						TDBChefmate::GetItemName( inOrderItem->OriginalItemKey ) + " " +
						inOrderItem->Size;

		UnicodeString itemKitchenName =
						TDBChefmate::GetItemKitchenName( inOrderItem->OriginalItemKey ) + " " +
						inOrderItem->SizeKitchenName;
		//.........................................................

		int itemCourseKey = TDBChefmate::GetItemCourseKey( inOrderItem->OriginalItemKey );

		//.........................................................
		// Chefmate is meant for restaurants and cafes where quantities are integer
		unsigned int qty = std::fabs(inOrderItem->GetQty());

   		for( int i = 0; i < qty; i++ )
		{
			HostnameLIST          hList;
			HostnameLIST_Iterator hIT;
			getChefMateHostNameList( inOrderItem, hList );
    		for( hIT = hList.begin(); hIT != hList.end(); hIT++ )
			{
            	TCMHostname cmHostname = ( AnsiString )( *hIT );
				HITEM itemHandle = ChefmateInterface.AddItem(
					cmHostname,
					itemSizeKey,                                   // ItemSize's Key
					itemOrderKey,          					       // Order Item's Key
					itemName,              						   // Item's Name
					itemKitchenName,                           	   // Item's Kitchen Name
					itemCourseKey, 								   // Item's Course Key
					inOrderItem->ServingCourse.ServingCourseKey,   // Item's Serving Course Key
					inOrderItem->Note                           ); // Item's Note

				// Add Item's Options
				orderItemOptionsToChefmate( cmHostname, itemHandle, inOrderItem );

				// Add Item's Sides
				orderItemSidesToChefmate(   cmHostname, itemHandle, inOrderItem );
			}
		}
	}
    else
    {
         //
    }
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemToCancelToChefmate(
						HITEM                 inItem,
						HITEM_TO_CANCEL_INFO* inInfo )
{
	TItemComplete* orderItem = ( TItemComplete* )inItem;
    if ( orderItem->OriginalItemKey == NULL && orderItem->ItemOrderedFrom != NULL )
	{
		orderItem->OriginalItemKey = orderItem->ItemOrderedFrom->ItemKey;

    }
    else
    {
       orderItem->OriginalItemKey = orderItem->ItemKey;

    }
	__int32       itemOrderKey = orderItem->OrderKey;

    UnicodeString itemName1 =  TDBChefmate::GetItemName( orderItem->OriginalItemKey ) + " " +  orderItem->Size;
    UnicodeString itemKitchenName =  TDBChefmate::GetItemKitchenName( orderItem->OriginalItemKey) + " " +	orderItem->SizeKitchenName;
	UnicodeString itemName =  orderItem->Item + "  " + orderItem->Size;

	HostnameLIST          hList;
	HostnameLIST_Iterator hIT;

	getChefMateHostNameList( ( TItemComplete* )orderItem, hList );

	for( hIT = hList.begin(); hIT != hList.end(); hIT++ )
	{
		TCMHostname cmHostname = ( AnsiString )( *hIT );

		HITEM itemHandle = ChefmateInterface.AddItemToCancel(
								cmHostname,
								itemOrderKey,     // Order Item's Key
								itemName,         // Order Item's Name
								inInfo->Canceled, // If FALSE Qty will be ignored
								inInfo->Qty );    // amount of items to cancel

		// Add Item's Sides to cancel
		orderItemSidesToCancelToChefmate( cmHostname, itemHandle, inInfo->HSideList );
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::callAwayServingCourseToChefmate( UnicodeString inServingCourseName )
{
    HostnameLIST          hList;
	HostnameLIST_Iterator hIT;

	getChefMateHostNameList( hList );

	for( hIT = hList.begin(); hIT != hList.end(); hIT++ )
	{
		TCMHostname cmHostname = ( AnsiString )( *hIT );

		ChefmateInterface.AddCallAwayServingCourse( cmHostname, inServingCourseName );
	}
}
//---------------------------------------------------------------------------
bool TChefmateClient::getChefMateHost(
						TItemComplete* inOrderItem,
						TCMHostname&   outHostname )
{
	bool result = false;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		std::auto_ptr<TPaymentTransaction>transaction( new TPaymentTransaction( DBTransaction ) );

		transaction->Orders->Add( inOrderItem );

		if( transaction->Orders->Count > 0 )
		{
			std::auto_ptr<TReqPrintJob>request( new TReqPrintJob( &TDeviceRealTerminal::Instance() ) );
			request->Transaction = transaction.get();

            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
            Kitchen->Initialise(DBTransaction);
			Kitchen->GetPrintouts(DBTransaction, request.get(), pfChefMate_Printer,NULL,0,true );

		 //	request->Printouts->FilterForChefMate();
			if( request->Printouts->Count > 0 )
			{
				TPrintout *printOut = ( TPrintout* )request->Printouts->Items[0];
				outHostname         = printOut->Printer.ServerName;
				result              = true;
			}
		}

		DBTransaction.Commit();
	}
	catch( ... )
	{
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TChefmateClient::getChefMateHostNameList( HostnameLIST&  outList )
{
	try
	{
		outList.clear();

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		TManagerPhysicalPrinter printerManager;
		std::auto_ptr<TStringList>serverNameList(new TStringList);
		printerManager.GetPrinterServerList(DBTransaction, serverNameList.get(), ptChefMate_Printer);

		if( serverNameList->Count > 0 )
		{
			for( int i = 0; i < serverNameList->Count; i++ )
			{
				outList.push_back( serverNameList->Strings[i] );

			}
		}

		DBTransaction.Commit();
	}
	catch( ... )
	{

	}
}
//---------------------------------------------------------------------------
void TChefmateClient::getChefMateHostNameList(
						TItemComplete* inOrderItem,
						HostnameLIST&  outList )
{
	try
	{
		outList.clear();
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
        TStringList* chefmatePrinters = TManagerVirtualPrinter::LoadChefmatePrinter(DBTransaction, inOrderItem);
        for (int i = 0; i < chefmatePrinters->Count; i++)
        {
           outList.push_back( chefmatePrinters->Strings[i]);
        }
        DBTransaction.Commit();
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemOptionsToChefmate(
							TCMHostname    inCMHostname,
							HITEM          inItemHandle,
							TItemComplete *inOrderItem )
{
	for( int i = 0; i < inOrderItem->OptionsSelected->Count; i++ )
	{
		TItemOption *option = inOrderItem->OptionsSelected->OptionGet( i );
		orderItemOptionToChefmate( inCMHostname, inItemHandle, option );
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemSidesToChefmate(
							TCMHostname    inCMHostname,
							HITEM          inItemHandle,
							TItemComplete *inOrderItem )
{
	for( int i = 0; i < inOrderItem->SubOrders->Count; i++ )
	{
		TItemCompleteSub *side = inOrderItem->SubOrders->SubOrderGet( i );
		orderItemSideToChefmate( inCMHostname, inItemHandle, side );
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemSidesToCancelToChefmate(
							TCMHostname inCMHostname,
							HITEM       inItemHandle,
							HSIDE_LIST  inHSideList )
{
	HSIDE_LIST::const_iterator slIT;
	for( slIT = inHSideList.begin(); slIT != inHSideList.end(); slIT++ )
	{
		orderItemSideToCancelToChefmate(
					inCMHostname,
					inItemHandle,
					( TItemCompleteSub* )*slIT );
	}
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemOptionToChefmate(
							TCMHostname cmHostname,
							HITEM inItemHandle,
							TItemOption* inItemOption )
{


      	ChefmateInterface.AddItemOption(
					cmHostname,
					inItemHandle,             // Item's Handle
					inItemOption->OptionKey,   // Item's Option Key
					inItemOption->Name,        // Item's Option Name
					inItemOption->KitchenName, // Item's Option Kitchen Name
					inItemOption->IsPlus    ); // Item's Option Is Plus
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemSideToChefmate(
							TCMHostname       cmHostname,
							HITEM             inItemHandle,
							TItemCompleteSub* inItemSide )
{
	HSIDE sideHandle = ChefmateInterface.AddItemSide(
							cmHostname,
							inItemHandle,                   // Item's Handle
							inItemSide->OrderKey,           // Order Item's Side Key
							inItemSide->Item,               // Item's Side Name
							inItemSide->ItemKitchenName );  // Item's Side Kitchen Name
}
//---------------------------------------------------------------------------
void TChefmateClient::orderItemSideToCancelToChefmate(
							TCMHostname       cmHostname,
							HITEM             inItemHandle,
							TItemCompleteSub* inItemSide )
{
	HSIDE sideHandle = ChefmateInterface.AddItemSideToCancel(
							cmHostname,
							inItemHandle,         // Item's Handle
							inItemSide->OrderKey, // Order Item's Side Key
							inItemSide->Item );   // Order Item's Side Name
}
//---------------------------------------------------------------------------
void TChefmateClient::orderSideOptionToChefmate(
						TCMHostname  cmHostname,
						HSIDE        inSideHandle,
						TItemOption* inItemOption )
{
	ChefmateInterface.AddSideOption(
					cmHostname,
					inSideHandle,              // Item's Handle
					inItemOption->OptionKey,   // Item's Option Key
					inItemOption->Name,        // Item's Option Name
					inItemOption->KitchenName, // Item's Option Kitchen Name
					inItemOption->IsPlus    ); // Item's Option Is Plus
}
//---------------------------------------------------------------------------
// Commit order in a separate thread
OERROR TChefmateClient::commitOrder()
{
	OERROR result = OERROR_SUCCESSFUL;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		ChefmateInterface.CommitOrder();
		clearCustomerNameAndOrderType();
	}
	catch( ... )
	{
		result = OERROR_FAILED_COMMIT_ORDER;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TChefmateClient::clearCustomerNameAndOrderType()
{
	TCustNameAndOrderType::Instance()->ClearNameCaught();
}
//---------------------------------------------------------------------------
__int32 TChefmateClient::cmOrderDBKey( TItemComplete *inOrder )
{
	try
	{
		// TimeKey: Field in the ORDERS table.
		// It's the same for all the items in the same order
		// It's used as an OrderKey since there is no OrderKey in the DB.
		return inOrder->TimeKey;
	}
	catch( ... )
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmServerName()
{
	try
	{
		return TDeviceRealTerminal::Instance().User.Name;
	}
	catch( ... )
	{
		return "Server Name";
	}
}

//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmStaffName(TItemComplete* inOrder)
{
    try
	{
		TSecurityReference *Sec1 = inOrder->Security->SecurityGetType(secOrderedBy);
		if (Sec1 != NULL)
		{
		  return  Sec1->From;
		}
        return TDeviceRealTerminal::Instance().User.Name;
	}
	catch( ... )
	{
		return "Server Name";
	}


}
//---------------------------------------------------------------------------
__int32 TChefmateClient::cmOrderNumber()
{
	try
	{
		return TChefmateOrderNumberGenerator::Instance().CurrentOrderNumber();
	}
	catch( ... )
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmChitValue()
{
	return cmChitValue( _chitNumber );
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmChitValue( TChitNumber *inChitNumber )
{
	try
	{
		return inChitNumber->GetChitNumber();
	}
	catch( ... )
	{
		return UnicodeString( L"Chit" );
    }
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmChitValue( TPaymentTransaction* inPaymentTransaction )
{
	return cmChitValue( &inPaymentTransaction->ChitNumber );
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmTableTabName( TItemComplete *inOrder )
{
	try
	{
		return 	inOrder->TabContainerName + " : " + inOrder->TabName;
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmOrderType()
{
	try
	{

		TCustNameAndOrderType *cusNameAndOrderType = TCustNameAndOrderType::Instance();

		if( cusNameAndOrderType->NameAndOrderTypeLoaded() )
		{
			return TCustNameAndOrderType::Instance()->GetStringPair().second;
		}
		else
		{
			return "";
		}
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmOrderType( TPaymentTransaction* inTransaction )
{
	try
	{
		TCustNameAndOrderType *cusNameAndOrderType = TCustNameAndOrderType::Instance();

		if( cusNameAndOrderType->NameAndOrderTypeLoaded() )
		{
			return TCustNameAndOrderType::Instance()->GetStringPair().second;
		}
		else
		{
			return "";
		}
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmCustomerName()
{
	try
	{
		TCustNameAndOrderType *cusNameAndOrderType = TCustNameAndOrderType::Instance();

		if( cusNameAndOrderType->NameAndOrderTypeLoaded() )
		{
			return cusNameAndOrderType->GetStringPair().first;
		}
		else
		{
			return "";
		}
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmCustomerName( TPaymentTransaction* inTransaction )
{
	try
	{
         UnicodeString customerName = "";
         TItemComplete *order = ( TItemComplete* )inTransaction->Orders->Items[0];
         if(inTransaction->ChitNumber.IsCaptureCustomerDetails)
         {
            customerName =  inTransaction->Membership.Member.Name + " "+ inTransaction->Membership.Member.Surname;
         }
         else if(inTransaction->ChitNumber.IsCaptureCustomerNameAndPhone)
         {
            customerName =  order->IdName;
         }
         else
         {

            TCustNameAndOrderType *cusNameAndOrderType = TCustNameAndOrderType::Instance();

            if( cusNameAndOrderType->NameAndOrderTypeLoaded() )
            {
                customerName = cusNameAndOrderType->GetStringPair().first;
            }
        }
        return customerName;
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmPartyName( TItemComplete *inOrder )
{
	try
	{
		return inOrder->PartyName;
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
// NOT USED .. SO FAR.
UnicodeString TChefmateClient::cmPatronCount( ORDER_VECTOR inOrderVector )
{
	__int32 count = 0;

	//::::::::::::::::::::::::::::::::::::::::::::

	// A map to get only one order by seat.
	std::map<__int32, TItemComplete*> distinctOrders;

	ORDER_VECTOR::const_iterator oIT;

	for( oIT = inOrderVector.begin(); oIT != inOrderVector.end(); oIT++ )
	{
		TItemComplete *order = ( TItemComplete* )( *oIT );

		distinctOrders[order->SeatNo] = order;
	}

	count = distinctOrders.size();

	//::::::::::::::::::::::::::::::::::::::::::::

	return IntToStr( count );
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmPatronCount( TList* inOrderList )
{
	__int32 count = 0;

	//::::::::::::::::::::::::::::::::::::::::::::

	// A map to get only one order by seat.
	std::map<__int32, TItemComplete*> distinctOrders;

	for( int i = 0; i < inOrderList->Count; i++ )
	{
		TItemComplete *order = ( TItemComplete* )inOrderList->Items[i];

		distinctOrders[order->SeatNo] = order;
	}

	count = distinctOrders.size();

	//::::::::::::::::::::::::::::::::::::::::::::

	return IntToStr( count );
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmPatronCount( TPaymentTransaction* inTransaction )
{
	__int32 count = 0;

	//::::::::::::::::::::::::::::::::::::::::::::

	std::vector<TPatronType>::const_iterator ptIT;

	for( ptIT = inTransaction->Patrons.begin(); ptIT != inTransaction->Patrons.end(); ptIT++ )
	{
		count += ptIT->Count;
	}

	//::::::::::::::::::::::::::::::::::::::::::::

	if( count == 0 )
	{
		return cmPatronCount( inTransaction->Orders );
	}
	else
	{
		return IntToStr( count );
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmSaleStartTime( TItemComplete* inOrder )
{
	Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );

	DBTransaction.StartTransaction();

	TDateTime saleStartTime = TDBSaleTimes::GetSaleStartTimeFromTimeKey( DBTransaction, inOrder->TimeKey );

	DBTransaction.Commit();

	return saleStartTime.FormatString( "dd/mm/yyyy hh:nn:ss " );
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmDeliveryTime( TPaymentTransaction* inTransaction )
{
    try
    {
       if(((double)inTransaction->ChitNumber.DeliveryTime <= 0) )
       {
         return "";
       }
       else
       {

          return inTransaction->ChitNumber.DeliveryTime.FormatString("dd/mm/yyyy hh:nn:ss ");
       }
    }
    catch( ... )
    {
       return "";
    }
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmDeliveryTime()
{
       if((double)_chitNumber->DeliveryTime <= 0)
       {
         return "";
       }
       else
       {
          return _chitNumber->DeliveryTime.FormatString("dd/mm/yyyy hh:nn:ss ");
       }
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClient::SendCompleteWebOrder( TPaymentTransaction* inTransaction, UnicodeString paymentStatus, TMMContactInfo customerDetails )
{
	CMC_ERROR result = CMC_ERROR_FAILED;

	if( inTransaction->Orders->Count == 0 )
	{
		return SendCompleteEmptyOrder();
	}

    TItemComplete *order = ( TItemComplete* )inTransaction->Orders->Items[0];
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(ChefmateInterface.OpenWebOrder(
                            cmOrderDBKey( order ),              // Order Key
                            cmStaffName(order),                     // Server Name
                            cmOrderNumber(),                    // Order Number
                            cmChitValue(     inTransaction ),   // Chit Value
                            cmTableTabName(  order ),           // Table/Tab Name
                            cmOrderType(     inTransaction ),   // Order Type
                            customerDetails,   // Customer Details
                            cmPartyName(     order ),           // Party Name
                            cmPatronCount(   inTransaction ),	// Patron Count
                            cmSaleStartTime( order ),           // Sale Start Time
                            cmDeliveryTime(inTransaction),       // Delivery Time
							paymentStatus ))
	{
    	for( int i = 0; i < inTransaction->Orders->Count; i++ )
		{
			TItemComplete *order1 = ( TItemComplete* )inTransaction->Orders->Items[i];
			orderItemToChefmate( order1 );
		}
    	commitOrder();
		TChefmateOrderNumberGenerator::Instance().NextOrderNumber();
		result = CMC_ERROR_SUCCESSFUL;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmCustomerPhone( TPaymentTransaction* inTransaction )
{
	try
	{
         TItemComplete *order = ( TItemComplete* )inTransaction->Orders->Items[0];
         UnicodeString customerPhone = "";
         if(inTransaction->ChitNumber.IsCaptureCustomerDetails)
         {
            customerPhone =  inTransaction->Membership.Member.Phone;
         }
         else if(inTransaction->ChitNumber.IsCaptureCustomerNameAndPhone)
         {
            customerPhone = order->IdNumber;
         }
         return customerPhone;
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmCustomerEmail( TPaymentTransaction* inTransaction )
{
	try
	{
         UnicodeString customerEmail = "";
         if(inTransaction->ChitNumber.IsCaptureCustomerDetails)
         {
            customerEmail =  inTransaction->Membership.Member.EMail;
         }
         return customerEmail;
	}
	catch( ... )
	{
		return "";
	}
}
//---------------------------------------------------------------------------
UnicodeString TChefmateClient::cmCustomerAddress( TPaymentTransaction* inTransaction )
{
	try
	{
         UnicodeString customerAddress = "";
         if(inTransaction->ChitNumber.IsCaptureCustomerDetails)
         {
            customerAddress =  inTransaction->Membership.Member.LocationAddress;
         }
         return customerAddress;
	}
	catch( ... )
	{
		return "";
	}
}
