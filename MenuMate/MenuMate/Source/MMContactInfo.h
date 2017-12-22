//---------------------------------------------------------------------------

#ifndef MMContactInfoH
#define MMContactInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <set>
#include <vector>
#include "PalmUserSettings.h"
#include "PointsRulesSet.h"
#include "ContactPoints.h"
#include "Access.h"
#include "ContactGroup.h"
#include "Membership.h"



//---------------------------------------------------------------------------
class TMMContactInfo
{
public:
   TMMContactInfo();
   void Clear();
   bool Valid();
   bool ValidEmail();
   bool ValidateCheckedDuplicateEmail();
   void LoadFromStream(int CardVersion, TMemoryStream *Stream);
   void SaveToStream(TMemoryStream *Stream);

   // overload of the method to be able to work with version 6 card type. Because at this version
   // we strip out the site id and database key
   void SaveToStream(int CardVersion, TMemoryStream *Stream);

   UnicodeString RefreshPoleDisplayName(eMemberNameOnPoleDisplay inNameOnPoleDisplay );
   void ExtractNames(AnsiString memberFullName);
   bool HasHotelNumber( void );
   bool IsCodePresent();

   int ContactID;
   int ContactKey;
   int SiteID;
   int ExternalKey;
   int PalmTimeOut;
   int TotalVisits;
   int RewardVisits;
   int DiscountGroup;
   bool TabEnabled;
   int MemberType;
   TPalmUserSettings PalmUserSettings;

   UnicodeString Name;
   UnicodeString Surname;
   UnicodeString Alias;
   UnicodeString PoleDisplayName;
   UnicodeString Sex;
   UnicodeString Title;
   UnicodeString Initials;
   UnicodeString PIN;
   UnicodeString Phone;
   UnicodeString Mobile;
   UnicodeString EMail;
   UnicodeString Note;
   UnicodeString LocationAddress;
   UnicodeString MailingAddress;
   UnicodeString AccountInfo;

   TDateTime CardCreationDate;
   TDateTime LastModified;
   TDateTime LastVisit;
   TDateTime LastBirthdayProcessed;
   TDateTime Expires;
   TDateTime ActivationDate;
   int TierLevel;
   UnicodeString PayRollID;
   TDateTime DateOfBirth;
   TDateTime StartTime1;
   TDateTime StartTime2;
   TDateTime StartTime3;
   TDateTime StartTime4;
   TDateTime StartTime5;
   TDateTime StartTime6;

   UnicodeString ProxStr;
   UnicodeString CardStr;
   UnicodeString ATRStr;
   UnicodeString MemberCode;
   std::set <UnicodeString> CardsToAdd;
   std::set <UnicodeString> CardsToRemove;
   std::set <UnicodeString> Cards;

   UnicodeString MembershipNumber;
   TContactPoints Points;
   TAccess AccessLevel;

   std::set <int> AutoAppliedDiscounts;
   std::set <AnsiString> AutoAppliedDiscountsID;
//   std::set <int> AutoAppliedDiscountsDummy;
   std::set <byte> AutoAppliedDiscountsIDDummy;
   std::vector<TVoucherDetail> MemberVouchers;
   std::vector<ContactGroup> currentGroups;
   std::vector<ContactGroup> availableGroups;
   ContactGroup summagroup;

    int Charges;

    double HourlyRate;

    AnsiString CloudUUID;     // if this uuid is empty or null it means the member is not in the cloud yet
    UnicodeString ActivationToken; // activation token returned from the server. need to be printed so that user can use it to activate himself
    Currency PreviousYearPoint;
    Currency CurrentYearPoint;
    Currency AvailableBDPoint;
    Currency AvailableFVPoint;

    int LatestLoginContactKey;
    UnicodeString LatestLoginName;
    int PointRule;
    int PointsRuleSubs;
    bool IsFirstVisitRewarded;
    bool HasTransactions;
    bool ValidateMandatoryField(AnsiString& message);
    bool ValidateLastName(AnsiString& message);
    bool ValidateFirstName(AnsiString& message);
    bool ValidateName(AnsiString& name,AnsiString& message);
    UnicodeString SubsType;
    TDateTime SubsPaidDate;
    double SubsPaidAmount;
    UnicodeString SubsReceiptNo;
    UnicodeString SubsPaid;
    UnicodeString IsLocalMember;
protected:
	TMMContactInfo( UnicodeString inName );
};

class TCustomer: public TMMContactInfo
{
	public:
	TCustomer(){}
	TCustomer( int inRoomNumber, int inInvoiceKey )
	: FRoomNumber(inRoomNumber), FInvoiceKey(inInvoiceKey){}
	TCustomer( int inRoomNumber, int inInvoiceKey, AnsiString inName )
	: FRoomNumber(inRoomNumber), FInvoiceKey(inInvoiceKey), TMMContactInfo(inName){}

	__property int RoomNumber = { read = GetRoomNumber, write = SetRoomNumber };
	__property int InvoiceKey = { read = GetInvoiceKey, write = SetInvoiceKey };


	private:
	int FRoomNumber;
	int FInvoiceKey;

	int GetRoomNumber( void ){ return FRoomNumber; }
	void SetRoomNumber( const int inRoomNumber ) { FRoomNumber = inRoomNumber; }
	int GetInvoiceKey( void ){ return FInvoiceKey; }
	void SetInvoiceKey( const int inInvoiceKey ) { FInvoiceKey = inInvoiceKey; }
};

#endif
