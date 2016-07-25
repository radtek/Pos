//---------------------------------------------------------------------------

#pragma hdrstop

#include "vector.h"

#include "tinyxml.h"

#include "SharedMemory.h"

#include "MMCustomerDisplayManager.h"
#include "MMCustomerDisplayThread.h"

#include "SharedMemory.h"

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

void TMMCustomerDisplayManager::SetTimer( TTimer* inTimer )
{
    order_timer = inTimer;

    order_timer->Enabled  = false;
    order_timer->Interval = 1000;
    order_timer->OnTimer  = onOrderTimer;
    order_timer->Enabled  = true;
}
//...........................................................................

void TMMCustomerDisplayManager::Reset()
{
  order_seq_no = 0;
  clearQueue();
}
//...........................................................................

void TMMCustomerDisplayManager::SendOrder( TList* inPOSOrder )
{
  TiXmlDocument *orderXML = createOrderXMLFromList( inPOSOrder );

  enqueueOrderXML( orderXML );

  delete orderXML;
}
//...........................................................................

void TMMCustomerDisplayManager::SendOrderFromFile( AnsiString inFileName )
{
  TiXmlDocument *orderXML = createOrderXMLFromFile( inFileName );

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
    order_seq_no = 0;
    order_being_sent = false;

    TSharedMemControllerBuilder smcBuilder = TSharedMemControllerBuilder::Instance();

    mem_controller = smcBuilder.BuildServerController(
<<<<<<< HEAD
                                    SM_CLIENT_NAME,
                                    SM_SERVER_CAPACITY,
                                    SM_CLIENT_CAPACITY );

    resetOrderSeqNumber();

    can_show_now = true;
=======
                                        smtMemory,
                                        SM_CLIENT_NAME,
                                        SM_SERVER_CAPACITY,
                                        SM_CLIENT_CAPACITY );
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
}
//...........................................................................

TiXmlDocument* TMMCustomerDisplayManager::createOrderXMLFromList( TList* inPOSOrder )
{
  TiXmlDocument* result = new TiXmlDocument();

  //::::::::::::::::::::::::::::::::::::::::::::::

  //::::::::::::::::::::::::::::::::::::::::::::::

  return result;
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

void TMMCustomerDisplayManager::clearQueue()
{
    order_queue.clear();
}
//...........................................................................

void TMMCustomerDisplayManager::enqueueOrderXML( TiXmlDocument *inOrderXML )
{
<<<<<<< HEAD
  if( order_data != NULL ) delete order_data;

  order_data_size = createOrderData( inOrderXML, order_data );

  if( can_show_now )
  {
      incOrderSecNumber();
      showOrderThread( order_seq_number, order_data, order_data_size );
  }
}
//...........................................................................

__int16 TMMCustomerDisplayManager::createOrderData(
                                     TiXmlDocument* inOrderXML,
                                              BYTE* &outData  )
{
  std::string dataStr  = serializeOut( inOrderXML ).c_str();
      __int16 dataSize = dataStr.size();

        outData = new BYTE[dataSize];
  __int32 itPos = 0;

  dataStr.copy( outData, dataSize, itPos );

  return dataSize;
=======
     char *data;
   __int32 dataSize = createOrderData( inOrderXML, data );

   order_queue[data] = dataSize;
}
//...........................................................................

void __fastcall TMMCustomerDisplayManager::onOrderTimer( TObject *sender )
{
    if( !order_being_sent && clientReady() )
    {
        // Create thread( order_xml, memController ) suspended and free on terminate
        TMMCustomerDisplayThread *thread = new TMMCustomerDisplayThread( true );

        ORDER_QUEUE::iterator dsIT = order_queue.begin();

        if( dsIT != order_queue.end() )
        {
            order_being_sent = true;
            setDataReady( false );

            DATA_STREAM data     = dsIT->first;
                __int32 dataSize = dsIT->second;

            thread->SetObjects( ++order_seq_no, data, dataSize, mem_controller );
            thread->FreeOnTerminate = true;
            thread->OnTerminate     = onTerminateShowOrderThread;

            order_queue.erase( order_queue.begin() );

            thread->Start();
        }
    }
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
}
//...........................................................................
void __fastcall TMMCustomerDisplayManager::onTerminateShowOrderThread(TObject* sender)
{
    order_being_sent = false;
    setDataReady( true );
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

<<<<<<< HEAD
// Writes the order in shared meory for the client to catch
// Disposes of inData
void TMMCustomerDisplayManager::showOrderThread(
                                   unsigned __int64   inDataSeqNumber,
                                               BYTE* &inData,
                                            __int16   inDataSize )
=======
bool TMMCustomerDisplayManager::clientReady()
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
{
    bool result = !( bool )mem_controller->ReadByte( SM_BUSY_POSITION );

<<<<<<< HEAD
    // Create thread( order_xml, memController ) suspended and free on terminate
    TMMCustomerDisplayThread* thread = new TMMCustomerDisplayThread( true );

    thread->SetObjects( inDataSeqNumber, inData, inDataSize, mem_controller );
    thread->FreeOnTerminate = true;
    thread->OnTerminate     = onTerminateShowOrderThread;

    clearOrderData( inData );

    thread->Start();
}
//...........................................................................

void __fastcall TMMCustomerDisplayManager::onTerminateShowOrderThread(TObject* sender)
{
    can_show_now = true;

    if( !emptyOrderData( order_data ) )
    {
        showOrderThread( order_seq_number, order_data, order_data_size );
    }
}
//...........................................................................

void TMMCustomerDisplayManager::clearOrderData( BYTE* &inData )
{
    delete[] inData;
    inData = NULL;
}
//...........................................................................
bool TMMCustomerDisplayManager::emptyOrderData( BYTE* inData )
{
  return inData == NULL;
=======
    return result;
}
//............................................................................

void TMMCustomerDisplayManager::setDataReady( bool inReady )
{
    mem_controller->Write( SM_ORDER_READY_POSITION, ( byte ) inReady );
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
}
//............................................................................

void TMMCustomerDisplayManager::resetOrderSeqNumber()
{
    order_seq_number = 0; // NO ORDER to show
}
//---------------------------------------------------------------------------
void TMMCustomerDisplayManager::incOrderSecNumber()
{
    order_seq_number++;

    if( order_seq_number == 0 )
    {
        order_seq_number = 1;
    }
}
//---------------------------------------------------------------------------


