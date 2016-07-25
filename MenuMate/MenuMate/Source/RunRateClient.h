//---------------------------------------------------------------------------

#ifndef RunRateClientH
#define RunRateClientH

//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>


#include "MM_DBCore.h"
#include "SeatOrders.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "ContainerOrders.h"
#include "PnMOrder.h"
#include "ChitNumber.h"
#include "PaymentTransaction.h"
#include "Printing.h"
#include "ChefmateClient.h"
#include "RunRateTcpStreamSender.h"

//---------------------------------------------------------------------------

typedef __int32 CMC_ERROR;

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
//typedef std::vector<HSIDE> HSIDE_LIST;
//struct HITEM_TO_CANCEL_INFO
//{
//	__int32    Qty;
//	bool       Canceled;
//	HSIDE_LIST HSideList;
//};
//typedef std::map<HITEM, HITEM_TO_CANCEL_INFO*> ITEM_TO_CANCEL_MAP;

//---------------------------------------------------------------------------

class TRunRateClient
{
public:

    TRunRateClient(){};
	~TRunRateClient();

    void SendPointDetailsToRunRate ( TPaymentTransaction &inTransaction );
};

//---------------------------------------------------------------------------

#endif
