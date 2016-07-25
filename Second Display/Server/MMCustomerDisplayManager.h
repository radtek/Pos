//---------------------------------------------------------------------------

#ifndef MMCustomerDisplayManagerH
#define MMCustomerDisplayManagerH

//---------------------------------------------------------------------------

<<<<<<< HEAD
#include <Classes.hpp>

//---------------------------------------------------------------------------

const AnsiString SM_CLIENT_NAME     = "MM_CUSTOMER_DISPLAY";
const __int32    SM_SERVER_CAPACITY = 65556; // 64KB
const __int32    SM_CLIENT_CAPACITY = 1024;  //  1KB

class TiXmlDocument;
class TSharedMemoryController;
=======
#include <vcl.h>

#include "tinyxml.h"
#include <map.h>
#include <vector.h>

#include "SharedMemory.h"

//---------------------------------------------------------------------------

const UnicodeString SM_CLIENT_NAME     = "MM_CUSTOMER_DISPLAY";
const __int64       SM_SERVER_CAPACITY = 65556; // 64KB
const __int64       SM_CLIENT_CAPACITY = 1024;  //  1KB

typedef                          char* DATA_STREAM;
typedef std::map<DATA_STREAM, __int32> ORDER_QUEUE;
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2

class TMMCustomerDisplayManager
{
public:
    static TMMCustomerDisplayManager *Instance();
<<<<<<< HEAD
    static void Done() ;

    void Reset();
    void Finish();
    void UpdateImages();
    void ShowOrder( TList* inPOSOrder );
    void ShowOrderFromFile( AnsiString inFileName );
=======
    static void Done();

    void SetTimer( TTimer* inTimer );

    void Reset();

    void SendOrder( #include <map.h> inPOSOrder );
    void SendOrderFromFile( AnsiString inFileName );

    void UpdateImages();
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2

private:
    static TMMCustomerDisplayManager *manager;

    TSharedMemoryController *mem_controller;

                bool  can_show_now;
    unsigned __int64  order_seq_number;
             __int32  order_data_size;
                BYTE *order_data;

    TMMCustomerDisplayManager();

    TiXmlDocument* createOrderXMLFromList( TList* inPOSOrder );
    TiXmlDocument* createOrderXMLFromFile( AnsiString inFileName );

          void showOrderXML(    TiXmlDocument *inOrderXML );
       __int16 createOrderData( TiXmlDocument* inOrderXML, BYTE* &outData );
    AnsiString serializeOut(    TiXmlDocument* inOrderXML );

<<<<<<< HEAD
    // Writes the order in shared meory for the client to catch
    // Disposes of inData
    void showOrderThread(
             unsigned __int64   inDataSeqNumber,
                         BYTE* &inData,
                      __int16   inDataSize );

    void __fastcall onTerminateShowOrderThread(TObject* sender);
    void clearOrderData( BYTE* &inData );
    bool emptyOrderData( BYTE*  inData );

     void resetOrderSeqNumber();
     void incOrderSecNumber();
=======
   TSharedMemoryController *mem_controller;

        __int32 order_seq_no;
          bool  order_being_sent;
        TTimer *order_timer;
   ORDER_QUEUE  order_queue;

   TMMCustomerDisplayManager();

    TiXmlDocument* createOrderXMLFromList(  TList* inPOSOrder  );
    TiXmlDocument* createOrderXMLFromFile( AnsiString inFileName );

    void clearQueue();
    void enqueueOrderXML( TiXmlDocument *orderXML );

    void __fastcall onOrderTimer( TObject *sender );
    void __fastcall onTerminateShowOrderThread(TObject* sender);

    __int32 createOrderData( TiXmlDocument *inOrderXML, DATA_STREAM &outData );
    AnsiString serializeOut( TiXmlDocument* inOrderXML );

    bool clientReady();
    void setDataReady( bool inReady );
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
};

//---------------------------------------------------------------------------
#endif
