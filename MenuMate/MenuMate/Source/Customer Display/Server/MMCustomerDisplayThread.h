//---------------------------------------------------------------------------

#ifndef MMCustomerDisplayThreadH
#define MMCustomerDisplayThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------

// Server Side
const __int64 SM_ORDER_READY_POSITION  =  0;
const __int64 SM_ORDER_CLOSE_POSITION  =  1;
const __int64 SM_ORDER_RESET_POSITION  =  2;
const __int64 SM_ORDER_SEQ_NO_POSITION =  3;
const __int64 SM_ORDER_SIZE_POSITION   =  1020; //7;
const __int64 SM_ORDER_POSITION        =  1024; //11;

// Client Side
const __int64 SM_BUSY_POSITION  = 0;

class TiXmlDocument;
class TSharedMemoryController;


class TMMCustomerDisplayThread : public TThread
{
public:
    __fastcall TMMCustomerDisplayThread ( bool CreateSuspended );

    void SetObjects(
            unsigned __int32         inOrderSeqNo,
            byte*                    inOrderData,
            unsigned __int32         inOrderDataSize,
            TSharedMemoryController* inMemController );

protected:
    void __fastcall Execute();

private:
            unsigned __int32  order_seq_no;
            unsigned __int32  order_data_size;
                        byte *order_data;
     TSharedMemoryController *mem_controller;

     void writeOrderDataInMem();
};
//---------------------------------------------------------------------------
#endif
