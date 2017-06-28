//---------------------------------------------------------------------------

#ifndef OracleDataObjectsH
#define OracleDataObjectsH
#include <system.hpp>
#include <vector>
#include "OracleDataObjects.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
///////////////  Room Request /////////////////////////////////
class TRoomInquiryItem;
class TPostRoomInquiry
{
  public:
    TPostRoomInquiry();
    ~TPostRoomInquiry();
    AnsiString InquiryInformation;
    AnsiString MaximumReturnedMatches;
    AnsiString SequenceNumber;
    AnsiString RequestType;
    AnsiString PaymentMethod;
    AnsiString Date;
    AnsiString Time;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};

class TRoomInquiryResult
{
  public:
    TRoomInquiryResult();
    ~TRoomInquiryResult();
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
    AnsiString Date;
    AnsiString Time;
    std::vector<TRoomInquiryItem> RoomInquiryItem;
    bool IsSuccessful;
    AnsiString resultText;
};

class TRoomInquiryItem
{
  public:
    TRoomInquiryItem();
    ~TRoomInquiryItem();
    AnsiString RoomNumber;
    AnsiString ReservationId;
    AnsiString LastName;
    AnsiString FirstName;
    AnsiString Title;
    AnsiString NoPost;
    AnsiString CreditLimit;
    AnsiString ProfileId;
    AnsiString HotelId;
};

class TRoomInquiryAnswer
{
  public:
    TRoomInquiryAnswer();
    ~TRoomInquiryAnswer();
    AnsiString AnswerStatus;
    AnsiString ResponseText;
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString Date;
    AnsiString Time;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};


////////////// Sales Data //////////////////////////

class TPostRequest
{
  public:
    TPostRequest();
    ~TPostRequest();
    AnsiString RoomNumber;
    AnsiString ReservationId;
    AnsiString ProfileId;
    AnsiString LastName;
    AnsiString HotelId;
    AnsiString RequestType;
    AnsiString InquiryInformation;
    AnsiString MatchfromPostList;
    AnsiString SequenceNumber;
    AnsiString TotalAmount;
    AnsiString CreditLimitOverride;
    AnsiString PaymentMethod;
    AnsiString Covers;
    AnsiString RevenueCenter;
    AnsiString ServingTime;
    AnsiString CheckNumber;
    std::vector<AnsiString> Subtotal1;
    std::vector<AnsiString> Discount;
    AnsiString Tip;
    std::vector<AnsiString> ServiceCharge;
    std::vector<AnsiString> Tax;
    AnsiString Date;
    AnsiString Time;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};

class TPostRequestAnswer
{
  public:
    TPostRequestAnswer();
    ~TPostRequestAnswer();
    AnsiString RoomNumber;
    AnsiString ReservationId;
    AnsiString LastName;
    AnsiString AnswerStatus;
    AnsiString ResponseText;
    AnsiString CheckNumber;
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString Date;
    AnsiString Time;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};
class TTax
{
  public:
    int Type;
    AnsiString Name;
    Currency Percentage;
    double Value;
    TTax(const TTax &tax);
    ~TTax();
    TTax();
};
class TServiceCharge
{
    AnsiString Name;
    double Value;
};
#endif
