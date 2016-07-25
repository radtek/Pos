//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "SharedMemory.h"
#include "MMCustomerDisplayThread.h"

<<<<<<< HEAD
//---------------------------------------------------------------------------
=======
#include "SharedMemory.h"
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2

#pragma package(smart_init)

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMMCustomerDisplayThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMMCustomerDisplayThread::TMMCustomerDisplayThread( bool CreateSuspended )
                                    : TThread( CreateSuspended )
{
     order_data_size = 0;
     order_data      = NULL;
     mem_controller  = NULL;
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayThread::SetObjects(
<<<<<<< HEAD
                   unsigned __int64  inOrderSeqNumber,
                         const BYTE* inOrderData,
                 const unsigned int  inOrderDataSize,
            TSharedMemoryController* inMemController )
{
     order_seq_number = inOrderSeqNumber;
     order_data_size  = inOrderDataSize;
     order_data       = new BYTE[order_data_size];
=======
            unsigned __int32         inOrderSeqNo,
            byte*                    inOrderData,
            unsigned __int32         inOrderDataSize,
            TSharedMemoryController* inMemController )
{
     order_seq_no    = inOrderSeqNo;
     order_data_size = inOrderDataSize;
     order_data      = new BYTE[order_data_size];
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
     CopyMemory( ( PVOID )( order_data ), ( PVOID )( inOrderData ), inOrderDataSize );

     mem_controller = inMemController;
}
//---------------------------------------------------------------------------

void __fastcall TMMCustomerDisplayThread::Execute()
{
<<<<<<< HEAD
    // TRUE if the previous order has already been showed on the Customer Display
    bool orderShowed = ( bool )mem_controller->ReadByte( SM_ORDER_SHOWED_POSITION );

    if( orderShowed && ( order_data != NULL ) && ( mem_controller != NULL ) )
=======
    if( ( order_data != NULL ) && ( mem_controller != NULL ) )
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
    {
        writeOrderDataInMem();

        delete[] order_data;
    }
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayThread::writeOrderDataInMem()
{
<<<<<<< HEAD
    mem_controller->Write(      SM_ORDER_SIZE_POSITION,   order_data_size );
    mem_controller->WriteBlock( SM_ORDER_POSITION,        order_data_size, order_data );

    mem_controller->Write(      SM_ORDER_NUMBER_POSITION, order_seq_number );
=======
    mem_controller->Write(      SM_ORDER_SEQ_NO_POSITION, order_seq_no );
    mem_controller->Write(      SM_ORDER_SIZE_POSITION,   order_data_size );
    mem_controller->WriteBlock( SM_ORDER_POSITION,        order_data_size, order_data );
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
}
//---------------------------------------------------------------------------

