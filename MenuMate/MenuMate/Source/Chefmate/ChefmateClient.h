//---------------------------------------------------------------------------

#ifndef ChefmateClientH
#define ChefmateClientH

//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>

#include "ChefmateOrderGenerator.h"

#include "ChefmateInterface.h"
#include "MM_DBCore.h"
#include "SeatOrders.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "ContainerOrders.h"
#include "PnMOrder.h"
#include "ChitNumber.h"
#include "PaymentTransaction.h"
#include "Printing.h"

//---------------------------------------------------------------------------

typedef __int32 CMC_ERROR;

const CMC_ERROR_SUCCESSFUL                = 0x0002;
const CMC_ERROR_FAILED                    = 0x0003;
const CMC_ERROR_NO_DYNAMIC_ORDERS_ALLOWED = 0x0004;
const CMC_ERROR_EMPTY_ORDER_SENT          = 0x0005;
const CMC_ERROR_NO_ITEMS_TO_CANCEL        = 0x0006;
const CMC_ERROR_CLIENT_ALREADY_CLOSED     = 0x0007;
const CMC_ERROR_NO_ITEMS_TO_REPLACE		  = 0x0008;
const CMC_ERROR_NO_ORDER_TO_TRANSFER      = 0x0009;
const CMC_ERROR_NOT_AVAILABLE             = 0xFFFF;

typedef std::vector<TSeatOrders*>   SEAT_ORDER_VECTOR;
typedef std::vector<TItemComplete*> ORDER_VECTOR;

typedef void __stdcall ( __closure * _OnItemsCanceled )( void* );

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  NOT FOR PUBLIC ACCESSS.  It is only internally used
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// In this case HITEM is used to cast TItemComplete objects.
// It's not the same as HITEM in ChefmateInterface

// In this case HSIDE is used to cast TItemCompleteSub objects.
// It's not the same as HSIDE in ChefmateInterface
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef std::vector<HSIDE> HSIDE_LIST;
struct HITEM_TO_CANCEL_INFO
{
	__int32    Qty;
	bool       Canceled;
	HSIDE_LIST HSideList;
};
typedef std::map<HITEM, HITEM_TO_CANCEL_INFO*> ITEM_TO_CANCEL_MAP;

//---------------------------------------------------------------------------

class TChefmateClient
{
public:

    TChefmateClient(){};
	~TChefmateClient();

	CMC_ERROR Open(TIBDatabase* inIBDatabase,TChitNumber* inChitNumber = NULL );
	CMC_ERROR Close();

	CMC_ERROR SendCompleteOrder( TPaymentTransaction* inTransaction, UnicodeString orderType = "",  UnicodeString paymentStatus = "");
	CMC_ERROR SendCancelOrder( TList* inElementsToCancel,AnsiString inTabTableName );
	CMC_ERROR SendTransferOrder( TList* orderList ,UnicodeString SourceTableName);
	CMC_ERROR SendCreditOrder( TContainerOrders* inOrders );
    CMC_ERROR SendCreditOrder( TList* inOrdersList );
	CMC_ERROR SendReplacementOrder( TPaymentTransaction* inTransaction );
	CMC_ERROR SendCallAwayOrder( TCallAwayComplete* inCallAway );
	CMC_ERROR SendCompleteEmptyOrder();

private:
	TChefmateInterface ChefmateInterface;
	TChitNumber *_chitNumber;

	void chefmateSetDB( TIBDatabase *inIBDatabase );
	bool chefmateInit();
	void chefmateClear();
	void chefmateInitOrderNumberGenerator();
	bool chefmateAvailable();
	bool chefmateOpen();
	void populateOrdersToCancel(TList* inElementsToCancel,ITEM_TO_CANCEL_MAP& outOrdersToCancel );
	void readSavedOrdersFromElements(TList* inElementsToCancel,	TList* outSavedOrderList );
	bool elementIsItem( TPnMOrder* element );
	bool orderFromElement(TPnMOrder* inElement,TList* inSavedOrderList, TItemComplete* &outOrder );
	bool orderAndSideFromElement(TPnMOrder* inElement,TList* inSavedOrderList,TItemComplete*    &outOrder, TItemCompleteSub* &outSide );
	bool elementIsSideInThisOrder(TPnMOrder* inElement,	TItemComplete* inOrder,	TItemCompleteSub* &outSide );
	void addToSidesToCancelMap(TItemComplete*  inOrder,	TItemCompleteSub* inSide, ITEM_TO_CANCEL_MAP& outOrdersToCancel );
	void addToOrdersToCancelMap(TItemComplete* inOrder, ITEM_TO_CANCEL_MAP& outOrdersToCancel );
	void orderItemToChefmate( TItemComplete *inOrderItem );
	void orderItemToCancelToChefmate(HITEM inItem,	HITEM_TO_CANCEL_INFO* inInfo );
	void callAwayServingCourseToChefmate( UnicodeString inServingCourseName );
	bool getChefMateHost(TItemComplete* inOrderItem,TCMHostname&   outHostname );
	void getChefMateHostNameList( HostnameLIST&  outList );
	void getChefMateHostNameList(TItemComplete* inOrderItem, HostnameLIST&  outList );
	void orderItemOptionsToChefmate(TCMHostname    inCMHostname,HITEM inItemHandle,	TItemComplete *inOrderItem );
	void orderItemSidesToChefmate(TCMHostname    inCMHostname,	HITEM  inItemHandle,TItemComplete *inOrderItem );
	void orderItemSidesToCancelToChefmate(TCMHostname inCMHostname,	HITEM  inItemHandle,HSIDE_LIST  inHSideList );
	void orderItemOptionToChefmate(TCMHostname cmHostname,HITEM inItemHandle,TItemOption* inItemOption );
	void orderItemSideToChefmate(TCMHostname cmHostname,HITEM inItemHandle,	TItemCompleteSub* inItemSide );
	void orderItemSideToCancelToChefmate(TCMHostname cmHostname,HITEM inItemHandle,	TItemCompleteSub* inItemSide );
	void orderSideOptionToChefmate(TCMHostname  cmHostname,	HSIDE inSideHandle, TItemOption* inItemOption );

	// Commit order in a separate thread
	OERROR commitOrder();

	void clearCustomerNameAndOrderType();

	__int32       cmOrderDBKey( TItemComplete* inOrder );
	UnicodeString cmServerName();
	__int32       cmOrderNumber();
	UnicodeString cmChitValue();
	UnicodeString cmChitValue( TChitNumber* inChitNumber );
	UnicodeString cmChitValue( TPaymentTransaction* inPaymentTransaction );
	UnicodeString cmTableTabName( TItemComplete* inOrder );
	UnicodeString cmTransferNote( TItemComplete* inOrder );
	UnicodeString cmOrderType();
	UnicodeString cmOrderType( TPaymentTransaction* inPaymentTransaction );
	UnicodeString cmCustomerName();
	UnicodeString cmCustomerName( TPaymentTransaction* inPaymentTransaction );
	UnicodeString cmPartyName( TItemComplete* inOrder );
    UnicodeString cmDeliveryTime( TPaymentTransaction* inTransaction );
    UnicodeString cmDeliveryTime( );
	// NOT USED .. SO FAR.
	UnicodeString cmPatronCount( ORDER_VECTOR inOrderVector );
	UnicodeString cmPatronCount( TList* inOrderList );
	UnicodeString cmPatronCount( TPaymentTransaction* inPaymentTransaction );
	UnicodeString cmSaleStartTime( TItemComplete* inOrder );
    UnicodeString cmStaffName(TItemComplete* inOrder);
    TMMContactInfo cmCustomerDetails(TPaymentTransaction* inPaymentTransaction);
};

//---------------------------------------------------------------------------

#endif
