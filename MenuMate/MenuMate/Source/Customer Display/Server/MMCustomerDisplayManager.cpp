//---------------------------------------------------------------------------


#pragma hdrstop

#include <process.h>

#include "MMCustomerDisplayManager.h"
#include "MMCustomerDisplayThread.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TMMCustomerDisplayManager* TMMCustomerDisplayManager::manager = NULL;

//:::::::::::::
//  PUBLIC
//:::::::::::::

TMMCustomerDisplayManager* TMMCustomerDisplayManager::Instance()
{
  if( manager == NULL )
  {
    manager = new TMMCustomerDisplayManager();
  }

  return manager;
}
//...........................................................................

void TMMCustomerDisplayManager::Done()
{
  delete manager;
  manager = NULL;
}
//...........................................................................

void TMMCustomerDisplayManager::OpenCustomerDisplay()
{
    launchCustomerDisplay();
}
//...........................................................................

void TMMCustomerDisplayManager::CloseCustomerDisplay()
{
    closeCustomerDisplay( true );
    FOpen = false;
}
//...........................................................................

void TMMCustomerDisplayManager::SetTimer( TTimer* inTimer )
{
    order_timer = inTimer;

    order_timer->Enabled  = false;
    order_timer->Interval = 100;
    order_timer->OnTimer  = onOrderTimer;
    order_timer->Enabled  = true;
}
//...........................................................................

void TMMCustomerDisplayManager::SetFinishTimer( TTimer* inTimer )
{
    order_finish_timer = inTimer;

    order_finish_timer->Enabled  = false;
    order_finish_timer->Interval = 5000;
    order_finish_timer->OnTimer  = onOrderFinishTimer;
    order_finish_timer->Enabled  = false;
}
//...........................................................................

void TMMCustomerDisplayManager::Reset()
{
  order_seq_no = 0;
  clearQueue();
  resetData( ( byte )1 );
}
//...........................................................................

void TMMCustomerDisplayManager::SendOrderFromFile( AnsiString inFileName )
{
  TiXmlDocument *orderXML = createOrderXMLFromFile( inFileName );

  enqueueOrderXML( orderXML );

  delete orderXML;
}
//...........................................................................

void TMMCustomerDisplayManager::SendOrder(
                                  std::vector <TSeatOrders*> inPOSOrder,
                                   TCustomerDisp inChangeStr )
{
  TiXmlDocument *orderXML = createOrderXMLFromList( inPOSOrder, inChangeStr );

  orderXML->SaveFile( "customer display order.xml" );

  resetData( ( byte )0 );
  stopOrderFinishTimer();

  enqueueOrderXML( orderXML );

  delete orderXML;
}
//...........................................................................

void TMMCustomerDisplayManager::SendOrderPaid(
                                  Currency inLastTotal,
                                  Currency inLastChange,
                                  Currency inTotalServiceCharge,
                                  Currency inTotalTax )
{
  TiXmlDocument *orderXML = createOrderPaidXMLFromList(
                                inLastTotal,
                                inLastChange,
                                inTotalServiceCharge,
                                inTotalTax );

  orderXML->SaveFile( "customer display order.xml" );

  resetData( ( byte )0 );
  startOrderFinishTimer();

  enqueueOrderXML( orderXML );

  delete orderXML;
}
//...........................................................................

void TMMCustomerDisplayManager::UpdateImages()
{
}
//...........................................................................

//:::::::::::::
//  PRIVATE
//:::::::::::::

 TMMCustomerDisplayManager::TMMCustomerDisplayManager()
{
    order_being_sent = false;

    TSharedMemControllerBuilder smcBuilder = TSharedMemControllerBuilder::Instance();

    mem_controller = smcBuilder.BuildServerController(
                                        smtMemory,
                                        SM_CLIENT_NAME,
                                        SM_SERVER_CAPACITY,
                                        SM_CLIENT_CAPACITY );

    customer_display_adapter = new TMMCustomerDisplayAdapter();

    // closeCustomerDisplay( false ) must be called before launching the
    // Customer Display otherwise it will close automaticaly
    closeCustomerDisplay( false );

    FOpen = false;
}
//...........................................................................
TMMCustomerDisplayManager::~TMMCustomerDisplayManager()
{
    delete customer_display_adapter;
    delete mem_controller;
}
//...........................................................................

void TMMCustomerDisplayManager::launchCustomerDisplay()
{
    UnicodeString customerDisplayURI       = ".\\Customer Display\\Menumate Second Display.exe";
    UnicodeString customerDisplayMainForm  = "MainForm";
    UnicodeString customerDisplayMainTitle = "Menumate Customer Display";

    _wspawnl( P_NOWAIT,
              customerDisplayURI.c_str(),
              customerDisplayURI.c_str(),
              NULL );

    ::Sleep(1000);
    HWND PrevWnd = FindWindow(
                      _T( ( const char* )customerDisplayMainForm.c_str()  ),
                      _T( ( const char* )customerDisplayMainTitle.c_str() ) );

    FOpen = true;

    if (PrevWnd != NULL)
    {
         PrevWnd = GetWindow(PrevWnd, GW_OWNER);
         if (PrevWnd != NULL)
         {
            if (IsIconic(PrevWnd))
            {
               ShowWindow(PrevWnd, SW_RESTORE);
            }
            else
            {
               SetForegroundWindow(PrevWnd);
            }
         }
    }
}
//...........................................................................

void TMMCustomerDisplayManager::stopOrderFinishTimer()
{
   order_finish_timer->Enabled = false;
}
//...........................................................................
void TMMCustomerDisplayManager::startOrderFinishTimer()
{
   order_finish_timer->Enabled = true;
}
//...........................................................................

TiXmlDocument* TMMCustomerDisplayManager::createOrderXMLFromFile( AnsiString inFileName )
{
  TiXmlDocument* result = new TiXmlDocument();

  //::::::::::::::::::::::::::::::::::::::::::::::

  result->LoadFile( inFileName.c_str() );

  //::::::::::::::::::::::::::::::::::::::::::::::

  return result;
}
//...........................................................................

TiXmlDocument* TMMCustomerDisplayManager::createOrderXMLFromList(
                                              std::vector <TSeatOrders*> inPOSOrder,
                                              TCustomerDisp inChangeStr)
{
  TiXmlDocument* result = new TiXmlDocument();

  //::::::::::::::::::::::::::::::::::::::::::::::

  result = customer_display_adapter->CreateOrderXML( inPOSOrder, inChangeStr );

  //::::::::::::::::::::::::::::::::::::::::::::::

  return result;
}
//...........................................................................

TiXmlDocument* TMMCustomerDisplayManager::createOrderPaidXMLFromList(
                                              Currency inLastTotal,
                                              Currency inLastChange,
                                              Currency inTotalServiceCharge,
                                              Currency inTotalTax )
{
  TiXmlDocument* result = new TiXmlDocument();

  //::::::::::::::::::::::::::::::::::::::::::::::

  result = customer_display_adapter->CreateOrderPaidXML(
                                        inLastTotal,
                                        inLastChange,
                                        inTotalServiceCharge,
                                        inTotalTax );

  //::::::::::::::::::::::::::::::::::::::::::::::

  return result;
}
//...........................................................................

void TMMCustomerDisplayManager::clearQueue()
{
    order_queue.clear();
}
//...........................................................................

void TMMCustomerDisplayManager::enqueueOrderXML( TiXmlDocument *inOrderXML )
{
     char *data;
   __int32 dataSize = createOrderData( inOrderXML, data );

   order_queue[data] = dataSize;
}
//...........................................................................

void __fastcall TMMCustomerDisplayManager::onOrderTimer( TObject *sender )
{
    if( !order_being_sent && clientReady() )
    {
        ORDER_QUEUE::iterator dsIT = order_queue.begin();

        if( dsIT != order_queue.end() )
        {
            // Create thread( order_xml, memController ) suspended and free on terminate
            TMMCustomerDisplayThread *thread = new TMMCustomerDisplayThread( true );

            order_being_sent = true;
            setDataReady( false );

            DATA_STREAM data     = dsIT->first;
                __int32 dataSize = dsIT->second;

            // order_seq_no must be incremented before setting the order in the
            // thread so that the client can accept it
            thread->SetObjects( ++order_seq_no, data, dataSize, mem_controller );
            thread->FreeOnTerminate = true;
            thread->OnTerminate     = onTerminateShowOrderThread;

            order_queue.erase( order_queue.begin() );

            thread->Start();
        }
    }
}
//...........................................................................
void __fastcall TMMCustomerDisplayManager::onTerminateShowOrderThread(TObject* sender)
{
    order_being_sent = false;
    setDataReady( true );
}
//...........................................................................

void __fastcall TMMCustomerDisplayManager::onOrderFinishTimer( TObject *sender )
{
    Reset();
    stopOrderFinishTimer();
}
//...........................................................................

__int32 TMMCustomerDisplayManager::createOrderData(
                                      TiXmlDocument *inOrderXML,
                                      DATA_STREAM   &outData )
{
    std::string data = serializeOut( inOrderXML ).c_str();
    __int32 dataSize = data.size();

    outData  = new char[dataSize];
    __int32 itPos = 0;

    data.copy( outData, dataSize, itPos );

    return dataSize;
}
//...........................................................................
AnsiString TMMCustomerDisplayManager::serializeOut( TiXmlDocument* inOrderXML )
{
   AnsiString result = "";

   //::::::::::::::::::::::::::::::::::::::::::::

   TiXmlPrinter printer;

   printer.SetIndent( _T("\t") );
   inOrderXML->Accept( &printer );

   result = AnsiString( printer.CStr() );

   //::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//............................................................................

void TMMCustomerDisplayManager::closeCustomerDisplay( bool inClose )
{
    mem_controller->Write( SM_ORDER_CLOSE_POSITION, ( byte )inClose );
}
//...........................................................................

bool TMMCustomerDisplayManager::clientReady()
{
    bool result = !( bool )mem_controller->ReadByte( SM_BUSY_POSITION );

    return result;
}
//............................................................................

void TMMCustomerDisplayManager::setDataReady( bool inReady )
{
    mem_controller->Write( SM_ORDER_READY_POSITION, ( byte ) inReady );
}
//............................................................................

void TMMCustomerDisplayManager::resetData( byte inReset )
{
    mem_controller->Write( SM_ORDER_RESET_POSITION, inReset );
}
//............................................................................

