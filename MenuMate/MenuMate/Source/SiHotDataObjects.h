//---------------------------------------------------------------------------

#ifndef SiHotDataObjectsH
#define SiHotDataObjectsH
#include <memory>
#include <vector>
#include <Classes.hpp>
//---------------------------------------------------------------------------
struct TRoomRequest
{
    UnicodeString TransactionNumber;
    UnicodeString RoomNumber;
};
struct TRoomResponse
{
    UnicodeString TransactionNumber;
    UnicodeString AccountNumber;
    UnicodeString FirstName;
    UnicodeString LastName;
    UnicodeString Limit;
    UnicodeString From;
    UnicodeString To;
    UnicodeString Title;
    UnicodeString Vip;
    UnicodeString VipCode;
    UnicodeString Vip2;
    UnicodeString VipCode2;
    UnicodeString SalesCategory;
    UnicodeString SalesCategoryCode;
    UnicodeString GeneralRemark;
    UnicodeString FbRemark;
    UnicodeString KitchenRemark;
    UnicodeString Pciid;
    UnicodeString RoomNumber;
};
struct TRoomCharge
{

};
struct TRoomChargeResponse
{

};


#endif
