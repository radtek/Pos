//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "MMCustomerDisplayThread.h"

#include "SharedMemory.h"

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
            unsigned __int32         inOrderSeqNo,
            byte*                    inOrderData,
            unsigned __int32         inOrderDataSize,
            TSharedMemoryController* inMemController )
{
     order_seq_no    = inOrderSeqNo;
     order_data_size = inOrderDataSize;
     order_data      = new BYTE[order_data_size];
     CopyMemory( ( PVOID )( order_data ), ( PVOID )( inOrderData ), inOrderDataSize );

     mem_controller = inMemController;
}
//---------------------------------------------------------------------------

void __fastcall TMMCustomerDisplayThread::Execute()
{
    if( ( order_data != NULL ) && ( mem_controller != NULL ) )
    {
        writeOrderDataInMem();

        delete[] order_data;
    }
}
//---------------------------------------------------------------------------

void TMMCustomerDisplayThread::writeOrderDataInMem()
{
    mem_controller->Write(      SM_ORDER_SEQ_NO_POSITION, order_seq_no );
    mem_controller->Write(      SM_ORDER_SIZE_POSITION,   order_data_size );
    mem_controller->WriteBlock( SM_ORDER_POSITION,        order_data_size, order_data );
}
//---------------------------------------------------------------------------
