//---------------------------------------------------------------------------

#ifndef MMCustomerDisplayManagerH
#define MMCustomerDisplayManagerH

//---------------------------------------------------------------------------

#include <vcl.h>

#include "tinyxml.h"
#include <map.h>
#include <vector.h>

#include "SharedMemory.h"
#include "MMCustomerDisplayAdapter.h"

#include "SeatOrders.h"

//---------------------------------------------------------------------------

const UnicodeString SM_CLIENT_NAME     = "MM_CUSTOMER_DISPLAY";
const __int64       SM_SERVER_CAPACITY = 65556; // 64KB
const __int64       SM_CLIENT_CAPACITY = 1024;  //  1KB

typedef                          char* DATA_STREAM;
typedef std::map<DATA_STREAM, __int32> ORDER_QUEUE;


class TMMCustomerDisplayManager
{
public:
    static TMMCustomerDisplayManager *Instance();

    static void Done();

    void OpenCustomerDisplay();
    void CloseCustomerDisplay();

    void SetTimer( TTimer* inTimer );
    void SetFinishTimer( TTimer* inTimer );

    void Reset();

    void SendOrderFromFile( AnsiString inFileName );

    void SendOrder(
            std::vector <TSeatOrders*> inPOSOrder,
            TCustomerDisp inChangeStr );

    void SendOrderPaid(
            Currency inLastTotal,
            Currency inLastChange,
            Currency inTotalServiceCharge,
            Currency inTotalTax );

    void UpdateImages();

    __property bool Open = { read = FOpen };

private:
   static TMMCustomerDisplayManager *manager;

   bool FOpen;

   TSharedMemoryController   *mem_controller;
   TMMCustomerDisplayAdapter *customer_display_adapter;

        __int32 order_seq_no;
          bool  order_being_sent;

        TTimer *order_finish_timer;  // Timer to reset the Customer Display after the order has been paid
        TTimer *order_timer;         // Timer to send orders to the Customer Display

   ORDER_QUEUE  order_queue;

    TMMCustomerDisplayManager();
   ~TMMCustomerDisplayManager();

    void launchCustomerDisplay();

    void stopOrderFinishTimer();
    void startOrderFinishTimer();

    TiXmlDocument* createOrderXMLFromList(
                       std::vector <TSeatOrders*> inPOSOrder,
                       TCustomerDisp  inChangeStr );

    TiXmlDocument* createOrderPaidXMLFromList(
                       Currency inLastTotal,
                       Currency inLastChange,
                       Currency inTotalServiceCharge,
                       Currency inTotalTax );

    TiXmlDocument* createOrderXMLFromFile( AnsiString inFileName );

    void clearQueue();
    void enqueueOrderXML( TiXmlDocument *orderXML );

    void __fastcall onOrderTimer( TObject *sender );
    void __fastcall onTerminateShowOrderThread(TObject* sender);

    void __fastcall onOrderFinishTimer( TObject *sender );

    __int32 createOrderData( TiXmlDocument *inOrderXML, DATA_STREAM &outData );
    AnsiString serializeOut( TiXmlDocument* inOrderXML );

    void closeCustomerDisplay( bool inClose );
    bool clientReady();
    void setDataReady( bool inReady );
    void resetData( byte inReset );
};

//---------------------------------------------------------------------------
#endif
