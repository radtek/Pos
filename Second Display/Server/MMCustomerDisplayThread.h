//---------------------------------------------------------------------------

#ifndef MMCustomerDisplayThreadH
#define MMCustomerDisplayThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------

<<<<<<< HEAD
// Sequence order number 1 - MAX(__int64). 0 means no order
const __int64 SM_ORDER_NUMBER_POSITION = 0;
const __int64 SM_ORDER_SIZE_POSITION   = 4;
const __int64 SM_ORDER_POSITION        = 8;

const __int64 SM_ORDER_SHOWED_POSITION = 0;
=======
// Server Side
const __int64 SM_ORDER_READY_POSITION  = 0;
const __int64 SM_ORDER_SEQ_NO_POSITION = 1;
const __int64 SM_ORDER_SIZE_POSITION   = 5;
const __int64 SM_ORDER_POSITION        = 9;

// Client Side
const __int64 SM_BUSY_POSITION = 0;
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2

class TiXmlDocument;
class TSharedMemoryController;

class TMMCustomerDisplayThread : public TThread
{
public:
    __fastcall TMMCustomerDisplayThread ( bool CreateSuspended );

    void SetObjects(
<<<<<<< HEAD
                 unsigned __int64    inOrderSeqNumber,
                 const BYTE*         inOrderData,
                 const unsigned int  inOrderDataSize,
=======
            unsigned __int32         inOrderSeqNo,
            byte*                    inOrderData,
            unsigned __int32         inOrderDataSize,
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
            TSharedMemoryController* inMemController );

protected:
    void __fastcall Execute();

private:
<<<<<<< HEAD
            unsigned __int64  order_seq_number;
            unsigned __int32  order_data_size;
                        BYTE *order_data;
=======
            unsigned __int32  order_seq_no;
            unsigned __int32  order_data_size;
                        byte *order_data;
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
     TSharedMemoryController *mem_controller;

     void writeOrderDataInMem();
};
//---------------------------------------------------------------------------
#endif
