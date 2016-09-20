//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMContactInfo.h"
#include "Stream.h"
#include "SmartCardVer1API.h"
#include "SmartCardVer2API.h"
#include "SmartCardVer3API.h"
#include "SmartCardVer4API.h"
#include "SmartCardVer5API.h"
#include "SmartCardVer6API.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TMMContactInfo::TMMContactInfo()
{
    SiteID = 0;
    MembershipNumber = "";
    ContactID = 0;
    ContactKey = 0;
    ExternalKey = 0;
    AccessLevel = 0;
    ContactKey = 0;
    PalmTimeOut = 30;
    PayRollID = 0;
    TabEnabled = false;
    StartTime1 = 0;
    StartTime2 = 0;
    StartTime3 = 0;
    StartTime4 = 0;
    StartTime5 = 0;
    StartTime6 = 0;
    DateOfBirth = 0;
    CardCreationDate = 0;
    LastModified = 0;
    TotalVisits = 0;
    RewardVisits = 0;
    LastVisit = 0;
    LastBirthdayProcessed = 0;
    TierLevel = 0;
    ATRStr = "";
    Surname = "";
    Points.Clear();
    AutoAppliedDiscounts.clear();
    AutoAppliedDiscountsID.clear();
    AutoAppliedDiscountsIDDummy.clear();
    Charges = 0;
    currentGroups.clear();
    availableGroups.clear();
    summagroup.Name = "";
    summagroup.Key = NULL;
    summagroup.Type = NULL;
    Expires = 0;
    HourlyRate = 0;
    DiscountGroup = -1;
    CloudUUID="";
    ActivationToken="";
    TierLevel = 0;
    ActivationDate = 0;
    MemberType =0;
    PreviousYearPoint = 0;
    CurrentYearPoint = 0;
    AvailableBDPoint = 0;
    AvailableFVPoint = 0;
    MemberCode = "";
    PointRule = 0;
    IsFirstVisitRewarded = false;
    MemberVouchers.clear();
}

TMMContactInfo::TMMContactInfo( UnicodeString inName)
{
	Name = inName;
	TMMContactInfo();
}
// ---------------------------------------------------------------------------
void TMMContactInfo::Clear()
{
   *this = TMMContactInfo();
   Points.Clear();
}

bool TMMContactInfo::Valid()
{
	return SiteID != 0
	       && MembershipNumber.Length();
}

bool TMMContactInfo::ValidEmail()
{
	bool validEmail = true;
	int emailAtPos = EMail.Pos("@");
	UnicodeString emailBeforeAt = EMail.SubString(1, emailAtPos - 1);
	UnicodeString emailAfterAt  = EMail.SubString(emailAtPos + 1, EMail.Length() - emailAtPos);
	validEmail = emailAfterAt.Pos("@")  == 0
			  && emailBeforeAt.Pos("@") == 0
			  && emailAfterAt.Pos(".")  != 0
			  && emailAfterAt.Pos(".")  != 1
			  && emailBeforeAt.Pos(".") != 1
			  && emailAfterAt.LastDelimiter(".")  != emailAfterAt.Length()
			  && emailBeforeAt.LastDelimiter(".") != emailBeforeAt.Length();
	return validEmail;
}

void TMMContactInfo::SaveToStream(TMemoryStream *Stream)
{
   Stream->Position = 0;
   StreamWrite(Stream, SiteID);
   StreamWrite(Stream, MembershipNumber);
   StreamWrite(Stream, Name);
   StreamWrite(Stream, Alias);
   StreamWrite(Stream, CardCreationDate);
   StreamWrite(Stream, LastModified);
   StreamWrite(Stream, PoleDisplayName);
   StreamWrite(Stream, DateOfBirth);
   StreamWrite(Stream, Phone);
   StreamWrite(Stream, Mobile);
   StreamWrite(Stream, EMail);
   StreamWrite(Stream, MailingAddress);
   StreamWrite(Stream, Note);
   StreamWrite(Stream, AutoAppliedDiscountsIDDummy);
   StreamWrite(Stream, Points.PointsRules);
}

void TMMContactInfo::SaveToStream(int CardVersion, TMemoryStream *Stream)
{
    if(CardVersion >= SMART_CARD_VERSION_SIX)
    {
       Stream->Position = 0;
       StreamWrite(Stream, SiteID);
       StreamWrite(Stream, MembershipNumber);
       StreamWrite(Stream, CloudUUID);
       StreamWrite(Stream, Name);
       StreamWrite(Stream, Alias);
       StreamWrite(Stream, CardCreationDate);
       StreamWrite(Stream, PoleDisplayName);
       StreamWrite(Stream, DateOfBirth);
       StreamWrite(Stream, Phone);
       StreamWrite(Stream, Mobile);
       StreamWrite(Stream, EMail);
       StreamWrite(Stream, MailingAddress);
       StreamWrite(Stream, Note);
       StreamWrite(Stream, AutoAppliedDiscountsIDDummy);
       StreamWrite(Stream, Points.PointsRules);
    }
    else
        SaveToStream(Stream);
}

void TMMContactInfo::LoadFromStream(int CardVersion, TMemoryStream *Stream)
{
   switch(CardVersion)
   {
	case SMART_CARD_VERSION_ONE:
	  {
		 int CardVersion = 0;
		 StreamRead(Stream, CardVersion);
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
		 StreamRead(Stream, Name);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
		 StreamRead(Stream, LastModified);
		 StreamRead(Stream, PoleDisplayName);
		 StreamRead(Stream, DateOfBirth);
		 StreamRead(Stream, Phone);
		 StreamRead(Stream, Mobile);
		 StreamRead(Stream, EMail);
		 StreamRead(Stream, LocationAddress);
		 StreamRead(Stream, MailingAddress);
		 StreamRead(Stream, Note);
		 Points.LoadFromStreamVer1(Stream);
	  }break;
   case SMART_CARD_VERSION_TWO:
	  {
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
		 StreamRead(Stream, Name);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
		 StreamRead(Stream, LastModified);
		 StreamRead(Stream, PoleDisplayName);
		 StreamRead(Stream, DateOfBirth);
		 StreamRead(Stream, Phone);
		 StreamRead(Stream, Mobile);
		 StreamRead(Stream, EMail);
		 StreamRead(Stream, LocationAddress);
		 StreamRead(Stream, MailingAddress);
		 StreamRead(Stream, Note);
	  }break;
	case SMART_CARD_VERSION_THREE:
	  {
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
		 StreamRead(Stream, Name);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
		 StreamRead(Stream, LastModified);
		 StreamRead(Stream, PoleDisplayName);
		 StreamRead(Stream, DateOfBirth);
		 StreamRead(Stream, Phone);
		 StreamRead(Stream, Mobile);
		 StreamRead(Stream, EMail);
		 StreamRead(Stream, LocationAddress);
		 StreamRead(Stream, MailingAddress);
		 StreamRead(Stream, Note);
		 StreamRead(Stream, AutoAppliedDiscountsIDDummy);
         StreamRead(Stream, Points.PointsRules);
	  }break;
	case SMART_CARD_VERSION_FOUR:
	  {
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
		 StreamRead(Stream, Name);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
		 StreamRead(Stream, LastModified);
		 StreamRead(Stream, PoleDisplayName);
		 StreamRead(Stream, DateOfBirth);
		 StreamRead(Stream, Phone);
		 StreamRead(Stream, Mobile);
		 StreamRead(Stream, EMail);
		 StreamRead(Stream, LocationAddress);
		 StreamRead(Stream, MailingAddress);
		 StreamRead(Stream, Note);
   		 StreamRead(Stream, AutoAppliedDiscountsIDDummy);
         StreamRead(Stream, Points.PointsRules);
	  }break;
	case SMART_CARD_VERSION_FIVE:
	  {
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
		 StreamRead(Stream, Name);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
		 StreamRead(Stream, LastModified);
		 StreamRead(Stream, PoleDisplayName);
		 StreamRead(Stream, DateOfBirth);
		 StreamRead(Stream, Phone);
		 StreamRead(Stream, Mobile);
		 StreamRead(Stream, EMail);
		 StreamRead(Stream, MailingAddress);
		 StreamRead(Stream, Note);
 		 StreamRead(Stream, AutoAppliedDiscountsIDDummy);
         StreamRead(Stream, Points.PointsRules);
	  }break;
	case SMART_CARD_VERSION_SIX:
	  {
//		 StreamRead(Stream, SiteID);
//		 StreamRead(Stream, MembershipNumber);
//       StreamRead(Stream, CloudUUID);
//		 StreamRead(Stream, Name);
//		 StreamRead(Stream, Alias);
//		 StreamRead(Stream, CardCreationDate);
//		 StreamRead(Stream, PoleDisplayName);
//		 StreamRead(Stream, DateOfBirth);
//		 StreamRead(Stream, Phone);
//		 StreamRead(Stream, Mobile);
//		 StreamRead(Stream, EMail);
//		 StreamRead(Stream, MailingAddress);
//		 StreamRead(Stream, Note);
//		 StreamRead(Stream, AutoAppliedDiscountsID);
//   	 StreamRead(Stream, AutoAppliedDiscountsIDDummy);
//		 StreamRead(Stream, Points.PointsRules);
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
         StreamRead(Stream, CloudUUID);
		 StreamRead(Stream, Name);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
		 //StreamRead(Stream, LastModified);
		 StreamRead(Stream, PoleDisplayName);
		 StreamRead(Stream, DateOfBirth);
		 StreamRead(Stream, Phone);
		 StreamRead(Stream, Mobile);
		 StreamRead(Stream, EMail);
		 //StreamRead(Stream, LocationAddress);
		 StreamRead(Stream, MailingAddress);
		 StreamRead(Stream, Note);
         StreamRead(Stream, AutoAppliedDiscountsIDDummy);
		 StreamRead(Stream, Points.PointsRules);
         //StreamRead(Stream, Title);
         //StreamRead(Stream, Sex);
	  }break;
   }
}

UnicodeString TMMContactInfo::RefreshPoleDisplayName(eMemberNameOnPoleDisplay inNameOnPoleDisplay )
{
    switch( inNameOnPoleDisplay )
    {
        case eMNPDAlias   : return Alias;
        case eMNPDFullName: return Name + " " +  Surname;

        default: return "";
    }
}

bool TMMContactInfo::HasHotelNumber( void )
{
	std::vector<ContactGroup>::iterator it = currentGroups.begin();
	for( ; it != currentGroups.end(); it++ )
	{
		if( (*it).Type == THotelNumber::GetHotelNumber() )
			return true;
	}
	return false;
}

bool TMMContactInfo::IsCodePresent()
{
   bool retVal = false;
   retVal = MemberCode != NULL && MemberCode != "";
   return retVal;


}

bool TMMContactInfo::ValidateMandatoryField(AnsiString& message)
{
    bool isvalid = false;
    int count = 0;
    if(Name == "")
    {
       message = message + "First Name,";
       count++;
    }
    if(Surname == "")
    {
       message = message + "Last Name,";
       count++;
    }
    if(!ValidEmail())
    {
       message = message + "Email,";
       count++;
    }
    if(Mobile != "" && Mobile.Length() < 5)
    {
       message = message + "Mobile Phone,";
       count++;
    }
    if(Phone != "" && Phone.Length() < 5)
    {
       message = message + "Phone,";
       count++;
    }
    if(count == 0)
    {
         isvalid = true;
    }
    return isvalid;
}
