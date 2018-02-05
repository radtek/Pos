//---------------------------------------------------------------------------

#ifndef OracleDataObjectsH
#define OracleDataObjectsH
#include <system.hpp>
#include <vector>
//---------------------------------------------------------------------------
///////////////  Room Request /////////////////////////////////
enum OracleRequestType {eLinkDecription = 1, eRoomInquiry, eRoomPost};
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
    AnsiString PathId;
    AnsiString SequenceNumber;
//    AnsiString PostNum;
    AnsiString HotelId;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
//    AnsiString SalesOutlet;
    AnsiString WaiterId;
//    AnsiString UserID;
    AnsiString WorkstationId;
//    AnsiString WSNum;
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
//    AnsiString GuestNum;
    AnsiString LastName;
//    AnsiString GuestName;
//    AnsiString GuestFirstname;
    AnsiString FirstName;
    AnsiString Title;
    AnsiString GuestTitle;
    AnsiString GuestVip;
    AnsiString NoPost;
    AnsiString PaymentMethod;
    AnsiString CreditLimit;
    AnsiString ProfileId;
    AnsiString HotelId;
//    AnsiString DefRef0;
//    AnsiString DefRef1;
//    AnsiString DefRef2;
//    AnsiString DefRef3;
//    AnsiString DefRef4;
//    AnsiString DefRef5;
//    AnsiString DefRef6;
//    AnsiString DefRef7;
//    AnsiString DefRef8;
//    AnsiString DefRef9;
//    AnsiString DefRef10;
    AnsiString Reference1;
    AnsiString Reference2;
    AnsiString Reference3;
    AnsiString Reference4;
    AnsiString Reference5;
    AnsiString Reference6;
    AnsiString Reference7;
    AnsiString Reference8;
    AnsiString Reference9;
    AnsiString Reference110;
//    AnsiString PathId;
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
    bool IsSuccessful;
    AnsiString resultText;
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
    bool IsSuccessful;
    AnsiString resultText;
};

class TPostRequestAnswer
{
  public:
    TPostRequestAnswer();
    ~TPostRequestAnswer();
    AnsiString Defref0;
    AnsiString RoomNumber;
    AnsiString ReservationId;
//    AnsiString GuestNum;
//    AnsiString PostNum;
    AnsiString LastName;
//    AnsiString GuestName;
    AnsiString AnswerStatus;
//    AnsiString AnswerStat;
//    AnsiString ClearText;
    AnsiString ResponseText;
    AnsiString CheckNumber;
//    AnsiString CheckNum;
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString Date;
    AnsiString Time;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
//    AnsiString SalesOutlet;
    AnsiString WaiterId;
//    AnsiString UserID;
    AnsiString WorkstationId;
//    AnsiString WSNum;
    bool IsSuccessful;
    AnsiString resultText;
    AnsiString PathId;
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
    public:
        AnsiString Name;
        double Value;
};
class TLinkDescription
{
    public:
        AnsiString Date;
        AnsiString Time;
        AnsiString VerNum;
};
class TLinkAlive
{
    public:
        AnsiString Date;
        AnsiString Time;
};
#endif
