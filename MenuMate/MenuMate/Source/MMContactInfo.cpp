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
#include <ctype.h>
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
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
    MemberType = 1;
    PreviousYearPoint = 0;
    CurrentYearPoint = 0;
    AvailableBDPoint = 0;
    AvailableFVPoint = 0;
    MemberCode = "";
    PointRule = 0;
    IsFirstVisitRewarded = false;
    MemberVouchers.clear();
    HasTransactions = false;
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
    bool retValue = false;
    if(TGlobalSettings::Instance().LoyaltyMateEnabled)
        retValue = SiteID != 0 && MembershipNumber.Length();
    else
     	retValue = SiteID != 0 && MembershipNumber.Length() && ValidateCheckedDuplicateEmail();
    return retValue;
}

bool TMMContactInfo::ValidateCheckedDuplicateEmail()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    bool isEmailValid  = true;
    DBTransaction.StartTransaction();
    int emailcount = 0;
    try
    {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL =:EMAIL AND CONTACTS_KEY <> :CONTACTS_KEY AND CONTACT_TYPE <> :CONTACT_TYPE AND CARD_CREATION_DATE <> :CARD_CREATION_DATE";
      IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eDeletedMember;

      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
      IBInternalQuery->ParamByName("EMAIL")->AsString = EMail;
      IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = CardCreationDate;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
      emailcount = IBInternalQuery->Fields[0]->AsInteger;
      if(emailcount > 0)
      {
        isEmailValid = false;
      }
    }
    catch(Exception & E)
    {
        isEmailValid = false;
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
    }
    return isEmailValid;
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
   UnicodeString blankValue = "";
   Stream->Position = 0;
   if(TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
        StreamWrite(Stream, SiteID);
        StreamWrite(Stream, MembershipNumber);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, CardCreationDate);
        StreamWrite(Stream, LastModified);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, DateOfBirth);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, EMail);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, AutoAppliedDiscountsIDDummy);
        StreamWrite(Stream, Points.PointsRules);
   }
   else
   {
        StreamWrite(Stream, SiteID);
        StreamWrite(Stream, MembershipNumber);
        StreamWrite(Stream, Name);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, CardCreationDate);
        StreamWrite(Stream, LastModified);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, DateOfBirth);
        StreamWrite(Stream, Phone);
        StreamWrite(Stream, Mobile);
        StreamWrite(Stream, EMail);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, blankValue);
        StreamWrite(Stream, AutoAppliedDiscountsIDDummy);
        StreamWrite(Stream, Points.PointsRules);
   }
}

void TMMContactInfo::SaveToStream(int CardVersion, TMemoryStream *Stream)
{
    UnicodeString blankValue = "";
    if(CardVersion >= SMART_CARD_VERSION_SIX)
    {
       Stream->Position = 0;
       if(TGlobalSettings::Instance().LoyaltyMateEnabled)
       {
           StreamWrite(Stream, SiteID);
           StreamWrite(Stream, MembershipNumber);
           StreamWrite(Stream, CloudUUID);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, CardCreationDate);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, DateOfBirth);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, EMail);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, AutoAppliedDiscountsIDDummy);
           StreamWrite(Stream, Points.PointsRules);
       }
       else
       {
           StreamWrite(Stream, SiteID);
           StreamWrite(Stream, MembershipNumber);
           StreamWrite(Stream, CloudUUID);
           StreamWrite(Stream, Name);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, CardCreationDate);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, DateOfBirth);
           StreamWrite(Stream, Phone);
           StreamWrite(Stream, Mobile);
           StreamWrite(Stream, EMail);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, blankValue);
           StreamWrite(Stream, AutoAppliedDiscountsIDDummy);
           StreamWrite(Stream, Points.PointsRules);
       }
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
		 StreamRead(Stream, SiteID);
		 StreamRead(Stream, MembershipNumber);
         StreamRead(Stream, CloudUUID);
         UnicodeString membername = "";
		 StreamRead(Stream, membername);
         ExtractNames(membername);
		 StreamRead(Stream, Alias);
		 StreamRead(Stream, CardCreationDate);
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

void TMMContactInfo::ExtractNames(AnsiString memberFullName)
{
   int breakPosition = memberFullName.Pos(" ");
   if(breakPosition > 0 && memberFullName.Length() > breakPosition)
   {
      Name = memberFullName.SubString(0,breakPosition-1).Trim();
      Surname = memberFullName.SubString(breakPosition + 1, memberFullName.Length() - (breakPosition)).Trim();
   }
   else
   {
      Name = memberFullName;
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

bool TMMContactInfo::ValidateFirstName(AnsiString& message)
{
    bool isValid = false;
    AnsiString firstName = Name.Trim();
    isValid = ValidateName(firstName,message);
    return isValid;
}

bool TMMContactInfo::ValidateLastName(AnsiString& message)
{
    bool isValid = false;
    AnsiString surname = Surname.Trim();
    isValid = ValidateName(surname,message);
    return isValid;
}

bool TMMContactInfo::ValidateName(AnsiString& name,AnsiString& message)
{
    bool isValid = false;
    if(name.Length() < 2)
    {
             message = message + "must be at least two characters long";
             return isValid;
    }
    int nameCounter = 0;
    int specialCharacterCounter = 0;
    int lCount = 0;
    int asciiValues[] = { 33,34,35,36,37,40,41,42,43,47,58,59,60,61,62,63,64,91,92,93,94,123,124,125,126 };
    std::vector<int> AsciiValuesException(asciiValues, asciiValues+25);

    const char* temp_name_char = name.c_str();
    for(int char_index = 0; char_index < name.Length(); char_index++)
    {
        int charAscii = temp_name_char[char_index];
        if(find(AsciiValuesException.begin(),AsciiValuesException.end(),charAscii) != AsciiValuesException.end())        {
                nameCounter++;
                break;
        }
        if(charAscii == 32 && (char_index+1) < name.Length())
        {
             if(temp_name_char[char_index+1] == 32)
             {
                nameCounter++;
                break;
             }
        }
    }
    if(nameCounter > 0)
    {
        message = message + "can contain [a-z], [0-9], [space], [-], [_], ['], [`], [,], [&] only";
        lCount++;
    }
    if(name.Length() > 20)
    {
       if(nameCounter == 0)
       {
             message = message + "should be less than 21 characters";
       }
       else
       {
             message = message + " and should be less than 21 characters";
       }
     lCount++;
    }
    if(lCount == 0)
    {
         isValid = true;
    }
    return isValid;
}
