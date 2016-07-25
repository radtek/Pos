// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpOutlk.pas' rev: 6.00

#ifndef OpOutlkHPP
#define OpOutlkHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Variants.hpp>	// Pascal unit
#include <OpOlkXP.hpp>	// Pascal unit
#include <OpOlk98.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opoutlk
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TOpOlItemType { OlitMail, OlitAppointment, OlitContact, OlitTask, OlitJournal, OlitNote, OlitPost };
#pragma option pop

#pragma option push -b-
enum TOpOlImportance { olimpLow, olImpNormal, olImpHigh };
#pragma option pop

#pragma option push -b-
enum TOpOlSensitivity { olSensNormal, olSensPersonal, olSensPrivate, olSensConfidential };
#pragma option pop

#pragma option push -b-
enum TOpOlDisplayType { oldtUser, oldtDistList, oldtForum, oldtAgent, oldtOrganization, oldtPrivateDistList, oldtRemoteUser };
#pragma option pop

#pragma option push -b-
enum TOpOlTrackingStatus { oltsNone, oltsDelivered, oltsNotDelivered, oltsNotRead, oltsRecallFailure, oltsRecallSuccess, oltsRead, oltsReplied };
#pragma option pop

#pragma option push -b-
enum TOpOlMailRecipientType { olmrtOriginator, olmrtTo, olmrtCC, olmrtBCC };
#pragma option pop

#pragma option push -b-
enum TOpOlInspectorClose { olicSave, olicDiscard, olicPromptForSave };
#pragma option pop

#pragma option push -b-
enum TOpOlGender { olGendUnspecified, olGendFemale, olGendMale };
#pragma option pop

#pragma option push -b-
enum TOpOlNoteColor { olncBlue, olncGreen, olncPink, olncYellow, olncWhite };
#pragma option pop

#pragma option push -b-
enum TOpOlMailingAddress { olmaNone, olmaHome, olmaBusiness, olmaOther };
#pragma option pop

#pragma option push -b-
enum TOpOlMeetingResponse { olmrTentative, olmrAccepted, olmrDeclined };
#pragma option pop

#pragma option push -b-
enum TOpOlBusyStatus { olbsFree, olbsTentative, olbsBusy, olbsOutOfOffice };
#pragma option pop

#pragma option push -b-
enum TOpOlMeetingStatus { olmsNonMeeting, olmsMeeting, olmsReceived, olmsCanceled };
#pragma option pop

#pragma option push -b-
enum TOpOlNetMeetingType { olnmtNetMeeting, olnmtChat, olnmtNetShow };
#pragma option pop

#pragma option push -b-
enum TOpOlRecurrenceState { olrsApptNotRecurring, olrsApptMaster, olrsApptOccurrence, olrsApptException };
#pragma option pop

#pragma option push -b-
enum TOpOlResponseStatus { olrespstNone, olrespstOrganized, olrespstTentative, olrespstAccepted, olrespstDeclined, olrespstNotResponded };
#pragma option pop

#pragma option push -b-
enum TOpOlTaskDelegationState { oltdsNotDelegated, oltdsUnknown, oltdsAccepted, oltdsDeclined };
#pragma option pop

#pragma option push -b-
enum TOpOlTaskOwnership { oltoNewTask, oltoDelegatedTask, oltoOwnTask };
#pragma option pop

#pragma option push -b-
enum TOpOlTaskResponse { oltrSimple, oltrAssign, oltrAccept, oltrDecline };
#pragma option pop

#pragma option push -b-
enum TOpOlTaskStatus { olTskStatNotStarted, olTskStatInProgress, olTskStatComplete, olTskStatWaiting, olTskStatDeferred };
#pragma option pop

#pragma option push -b-
enum TOpOlSaveAsType { olsatTXT, olsatRTF, olsatTemplate, olsatMSG, olsatDoc, olsatHTML, olsatVCard, olsatVCal };
#pragma option pop

typedef void __fastcall (__closure *TItemSendEvent)(_di_IDispatch Item, Word &Cancel);

typedef void __fastcall (__closure *TReminderEvent)(_di_IDispatch Item);

typedef void __fastcall (__closure *TOptionsPagesAddEvent)(Opolkxp::_di_PropertyPages Pages);

class DELPHICLASS TOpOutlookItem;
class PASCALIMPLEMENTATION TOpOutlookItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TOpOutlookItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TOpOutlookItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpAppointmentItem;
class DELPHICLASS TOpAttachmentList;
class DELPHICLASS TOpMailListProp;
class DELPHICLASS TOpMailListItem;
class PASCALIMPLEMENTATION TOpMailListProp : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	bool FDestroying;
	Opshared::TOpFreeList* FFreeList;
	TOpOutlookItem* FOwner;
	
protected:
	virtual int __fastcall GetCount(void) = 0 ;
	
public:
	__fastcall TOpMailListProp(TOpOutlookItem* AOwner);
	__fastcall virtual ~TOpMailListProp(void);
	void __fastcall AddItem(TOpMailListItem* Item);
	void __fastcall RemoveItem(TOpMailListItem* Item);
	__property int Count = {read=GetCount, nodefault};
	__property bool Destroying = {read=FDestroying, nodefault};
};


class DELPHICLASS TOpAttachment;
class PASCALIMPLEMENTATION TOpAttachmentList : public TOpMailListProp 
{
	typedef TOpMailListProp inherited;
	
private:
	Opolkxp::_di_Attachments FIntf;
	TOpAttachment* __fastcall GetItems(int Index);
	
protected:
	virtual int __fastcall GetCount(void);
	
public:
	__fastcall TOpAttachmentList(TOpOutlookItem* AOwner, Opolkxp::_di_Attachments Intf);
	TOpAttachment* __fastcall Add(const AnsiString FileName);
	void __fastcall Remove(int Index);
	__property int Count = {read=GetCount, nodefault};
	__property TOpAttachment* Items[int Index] = {read=GetItems};
	__property Opolkxp::_di_Attachments AttachmentsIntf = {read=FIntf};
public:
	#pragma option push -w-inl
	/* TOpMailListProp.Destroy */ inline __fastcall virtual ~TOpAttachmentList(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpRecipientList;
class DELPHICLASS TOpRecipient;
class PASCALIMPLEMENTATION TOpRecipientList : public TOpMailListProp 
{
	typedef TOpMailListProp inherited;
	
private:
	Opolkxp::_di_Recipients FIntf;
	TOpRecipient* __fastcall GetItems(int Index);
	
protected:
	virtual int __fastcall GetCount(void);
	
public:
	__fastcall TOpRecipientList(TOpOutlookItem* AOwner, Opolkxp::_di_Recipients Intf);
	TOpRecipient* __fastcall Add(const AnsiString Name);
	void __fastcall Remove(int Index);
	bool __fastcall ResolveAll(void);
	__property int Count = {read=GetCount, nodefault};
	__property TOpRecipient* Items[int Index] = {read=GetItems};
	__property Opolkxp::_di_Recipients RecipientsIntf = {read=FIntf};
public:
	#pragma option push -w-inl
	/* TOpMailListProp.Destroy */ inline __fastcall virtual ~TOpRecipientList(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpMailItem;
class PASCALIMPLEMENTATION TOpAppointmentItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	Opolkxp::_di__AppointmentItem FAppointmentItem;
	TOpAttachmentList* FAttachments;
	TOpRecipientList* FRecipients;
	bool __fastcall GetAllDayEvent(void);
	AnsiString __fastcall GetBillingInformation();
	AnsiString __fastcall GetBody();
	TOpOlBusyStatus __fastcall GetBusyStatus(void);
	AnsiString __fastcall GetCategories();
	AnsiString __fastcall GetCompanies();
	AnsiString __fastcall GetConversationIndex();
	AnsiString __fastcall GetConversationTopic();
	System::TDateTime __fastcall GetCreationTime(void);
	int __fastcall GetDuration(void);
	System::TDateTime __fastcall GetEnd_(void);
	AnsiString __fastcall GetEntryID();
	Opolkxp::_di_FormDescription __fastcall GetFormDescription();
	TOpOlImportance __fastcall GetImportance(void);
	Opolkxp::_di__Inspector __fastcall GetInspector();
	bool __fastcall GetIsOnlineMeeting(void);
	bool __fastcall GetIsRecurring(void);
	System::TDateTime __fastcall GetLastModificationTime(void);
	AnsiString __fastcall GetLocation();
	TOpOlMeetingStatus __fastcall GetMeetingStatus(void);
	AnsiString __fastcall GetMessageClass();
	AnsiString __fastcall GetMileage();
	bool __fastcall GetNetMeetingAutoStart(void);
	AnsiString __fastcall GetNetMeetingOrganizerAlias();
	AnsiString __fastcall GetNetMeetingServer();
	TOpOlNetMeetingType __fastcall GetNetMeetingType(void);
	bool __fastcall GetNoAging(void);
	AnsiString __fastcall GetOptionalAttendees();
	AnsiString __fastcall GetOrganizer();
	int __fastcall GetOutlookInternalVersion(void);
	AnsiString __fastcall GetOutlookVersion();
	TOpOlRecurrenceState __fastcall GetRecurrenceState(void);
	int __fastcall GetReminderMinutesBeforeStart(void);
	bool __fastcall GetReminderOverrideDefault(void);
	bool __fastcall GetReminderPlaySound(void);
	bool __fastcall GetReminderSet(void);
	AnsiString __fastcall GetReminderSoundFile();
	System::TDateTime __fastcall GetReplyTime(void);
	AnsiString __fastcall GetRequiredAttendees();
	AnsiString __fastcall GetResources();
	bool __fastcall GetResponseRequested(void);
	TOpOlResponseStatus __fastcall GetResponseStatus(void);
	bool __fastcall GetSaved(void);
	TOpOlSensitivity __fastcall GetSensitivity(void);
	int __fastcall GetSize(void);
	System::TDateTime __fastcall GetStart(void);
	AnsiString __fastcall GetSubject();
	bool __fastcall GetUnRead(void);
	void __fastcall SetAllDayEvent(bool Value);
	void __fastcall SetBillingInformation(const AnsiString Value);
	void __fastcall SetBody(const AnsiString Value);
	void __fastcall SetBusyStatus(TOpOlBusyStatus Value);
	void __fastcall SetCategories(const AnsiString Value);
	void __fastcall SetCompanies(const AnsiString Value);
	void __fastcall SetDuration(int Value);
	void __fastcall SetEnd_(const System::TDateTime Value);
	void __fastcall SetImportance(TOpOlImportance Value);
	void __fastcall SetIsOnlineMeeting(bool Value);
	void __fastcall SetLocation(const AnsiString Value);
	void __fastcall SetMeetingStatus(TOpOlMeetingStatus Value);
	void __fastcall SetMessageClass(const AnsiString Value);
	void __fastcall SetMileage(const AnsiString Value);
	void __fastcall SetNetMeetingAutoStart(bool Value);
	void __fastcall SetNetMeetingOrganizerAlias(const AnsiString Value);
	void __fastcall SetNetMeetingServer(const AnsiString Value);
	void __fastcall SetNetMeetingType(TOpOlNetMeetingType Value);
	void __fastcall SetNoAging(bool Value);
	void __fastcall SetOptionalAttendees(const AnsiString Value);
	void __fastcall SetReminderMinutesBeforeStart(int Value);
	void __fastcall SetReminderOverrideDefault(bool Value);
	void __fastcall SetReminderPlaySound(bool Value);
	void __fastcall SetReminderSet(bool Value);
	void __fastcall SetReminderSoundFile(const AnsiString Value);
	void __fastcall SetReplyTime(const System::TDateTime Value);
	void __fastcall SetRequiredAttendees(const AnsiString Value);
	void __fastcall SetResources(const AnsiString Value);
	void __fastcall SetResponseRequested(bool Value);
	void __fastcall SetSensitivity(TOpOlSensitivity Value);
	void __fastcall SetStart(const System::TDateTime Value);
	void __fastcall SetSubject(const AnsiString Value);
	void __fastcall SetUnRead(bool Value);
	
public:
	__fastcall TOpAppointmentItem(Opolkxp::_di__AppointmentItem ApptItem);
	__fastcall virtual ~TOpAppointmentItem(void);
	void __fastcall Close(TOpOlInspectorClose SaveMode);
	TOpAppointmentItem* __fastcall Copy(void);
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	void __fastcall PrintOut(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString Path, TOpOlSaveAsType SaveAsType);
	void __fastcall ClearRecurrencePattern(void);
	TOpMailItem* __fastcall ForwardAsVcal(void);
	Opolkxp::_di_RecurrencePattern __fastcall GetRecurrencePattern();
	Opolkxp::_di__MeetingItem __fastcall Respond(TOpOlMeetingResponse Response, bool NoUI, bool AdditionalTextDialog);
	void __fastcall Send(void);
	__property Opolkxp::_di__AppointmentItem AppointmentItem = {read=FAppointmentItem};
	__property TOpAttachmentList* Attachments = {read=FAttachments};
	__property AnsiString BillingInformation = {read=GetBillingInformation, write=SetBillingInformation};
	__property AnsiString Body = {read=GetBody, write=SetBody};
	__property AnsiString Categories = {read=GetCategories, write=SetCategories};
	__property AnsiString Companies = {read=GetCompanies, write=SetCompanies};
	__property AnsiString ConversationIndex = {read=GetConversationIndex};
	__property AnsiString ConversationTopic = {read=GetConversationTopic};
	__property System::TDateTime CreationTime = {read=GetCreationTime};
	__property AnsiString EntryID = {read=GetEntryID};
	__property Opolkxp::_di_FormDescription FormDescription = {read=GetFormDescription};
	__property TOpOlImportance Importance = {read=GetImportance, write=SetImportance, nodefault};
	__property Opolkxp::_di__Inspector ItemInspector = {read=GetInspector};
	__property System::TDateTime LastModificationTime = {read=GetLastModificationTime};
	__property AnsiString MessageClass = {read=GetMessageClass, write=SetMessageClass};
	__property AnsiString Mileage = {read=GetMileage, write=SetMileage};
	__property bool NoAging = {read=GetNoAging, write=SetNoAging, nodefault};
	__property int OutlookInternalVersion = {read=GetOutlookInternalVersion, nodefault};
	__property AnsiString OutlookVersion = {read=GetOutlookVersion};
	__property bool Saved = {read=GetSaved, nodefault};
	__property TOpOlSensitivity Sensitivity = {read=GetSensitivity, write=SetSensitivity, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property AnsiString Subject = {read=GetSubject, write=SetSubject};
	__property bool UnRead = {read=GetUnRead, write=SetUnRead, nodefault};
	__property bool AllDayEvent = {read=GetAllDayEvent, write=SetAllDayEvent, nodefault};
	__property TOpOlBusyStatus BusyStatus = {read=GetBusyStatus, write=SetBusyStatus, nodefault};
	__property int Duration = {read=GetDuration, write=SetDuration, nodefault};
	__property System::TDateTime End_ = {read=GetEnd_, write=SetEnd_};
	__property bool IsOnlineMeeting = {read=GetIsOnlineMeeting, write=SetIsOnlineMeeting, nodefault};
	__property bool IsRecurring = {read=GetIsRecurring, nodefault};
	__property AnsiString Location = {read=GetLocation, write=SetLocation};
	__property TOpOlMeetingStatus MeetingStatus = {read=GetMeetingStatus, write=SetMeetingStatus, nodefault};
	__property bool NetMeetingAutoStart = {read=GetNetMeetingAutoStart, write=SetNetMeetingAutoStart, nodefault};
	__property AnsiString NetMeetingOrganizerAlias = {read=GetNetMeetingOrganizerAlias, write=SetNetMeetingOrganizerAlias};
	__property AnsiString NetMeetingServer = {read=GetNetMeetingServer, write=SetNetMeetingServer};
	__property TOpOlNetMeetingType NetMeetingType = {read=GetNetMeetingType, write=SetNetMeetingType, nodefault};
	__property AnsiString OptionalAttendees = {read=GetOptionalAttendees, write=SetOptionalAttendees};
	__property AnsiString Organizer = {read=GetOrganizer};
	__property TOpRecipientList* Recipients = {read=FRecipients};
	__property TOpOlRecurrenceState RecurrenceState = {read=GetRecurrenceState, nodefault};
	__property int ReminderMinutesBeforeStart = {read=GetReminderMinutesBeforeStart, write=SetReminderMinutesBeforeStart, nodefault};
	__property bool ReminderOverrideDefault = {read=GetReminderOverrideDefault, write=SetReminderOverrideDefault, nodefault};
	__property bool ReminderPlaySound = {read=GetReminderPlaySound, write=SetReminderPlaySound, nodefault};
	__property bool ReminderSet = {read=GetReminderSet, write=SetReminderSet, nodefault};
	__property AnsiString ReminderSoundFile = {read=GetReminderSoundFile, write=SetReminderSoundFile};
	__property System::TDateTime ReplyTime = {read=GetReplyTime, write=SetReplyTime};
	__property AnsiString RequiredAttendees = {read=GetRequiredAttendees, write=SetRequiredAttendees};
	__property AnsiString Resources = {read=GetResources, write=SetResources};
	__property bool ResponseRequested = {read=GetResponseRequested, write=SetResponseRequested, nodefault};
	__property TOpOlResponseStatus ResponseStatus = {read=GetResponseStatus, nodefault};
	__property System::TDateTime Start = {read=GetStart, write=SetStart};
};


class DELPHICLASS TOpContactItem;
class PASCALIMPLEMENTATION TOpContactItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	Opolkxp::_di__ContactItem FContactItem;
	AnsiString __fastcall GetAccount();
	System::TDateTime __fastcall GetAnniversary(void);
	AnsiString __fastcall GetAssistantName();
	AnsiString __fastcall GetAssistantTelephoneNumber();
	AnsiString __fastcall GetBillingInformation();
	System::TDateTime __fastcall GetBirthday(void);
	AnsiString __fastcall GetBusiness2TelephoneNumber();
	AnsiString __fastcall GetBusinessAddress();
	AnsiString __fastcall GetBusinessAddressCity();
	AnsiString __fastcall GetBusinessAddressCountry();
	AnsiString __fastcall GetBusinessAddressPostalCode();
	AnsiString __fastcall GetBusinessAddressPostOfficeBox();
	AnsiString __fastcall GetBusinessAddressState();
	AnsiString __fastcall GetBusinessAddressStreet();
	AnsiString __fastcall GetBusinessFaxNumber();
	AnsiString __fastcall GetBusinessHomePage();
	AnsiString __fastcall GetBusinessTelephoneNumber();
	AnsiString __fastcall GetCallbackTelephoneNumber();
	AnsiString __fastcall GetCarTelephoneNumber();
	AnsiString __fastcall GetCategories();
	AnsiString __fastcall GetChildren();
	AnsiString __fastcall GetCompanies();
	AnsiString __fastcall GetCompanyAndFullName();
	AnsiString __fastcall GetCompanyLastFirstNoSpace();
	AnsiString __fastcall GetCompanyLastFirstSpaceOnly();
	AnsiString __fastcall GetCompanyMainTelephoneNumber();
	AnsiString __fastcall GetCompanyName();
	AnsiString __fastcall GetComputerNetworkName();
	System::TDateTime __fastcall GetCreationTime(void);
	AnsiString __fastcall GetCustomerID();
	AnsiString __fastcall GetDepartment();
	AnsiString __fastcall GetEmail1Address();
	AnsiString __fastcall GetEmail1AddressType();
	AnsiString __fastcall GetEmail1DisplayName();
	AnsiString __fastcall GetEmail1EntryID();
	AnsiString __fastcall GetEmail2Address();
	AnsiString __fastcall GetEmail2AddressType();
	AnsiString __fastcall GetEmail2DisplayName();
	AnsiString __fastcall GetEmail2EntryID();
	AnsiString __fastcall GetEmail3Address();
	AnsiString __fastcall GetEmail3AddressType();
	AnsiString __fastcall GetEmail3DisplayName();
	AnsiString __fastcall GetEmail3EntryID();
	AnsiString __fastcall GetEntryID();
	AnsiString __fastcall GetFileAs();
	AnsiString __fastcall GetFirstName();
	AnsiString __fastcall GetFTPSite();
	AnsiString __fastcall GetFullName();
	AnsiString __fastcall GetFullNameAndCompany();
	TOpOlGender __fastcall GetGender(void);
	Opolkxp::_di__Inspector __fastcall GetInspector();
	AnsiString __fastcall GetGovernmentIDNumber();
	AnsiString __fastcall GetHobby();
	AnsiString __fastcall GetHome2TelephoneNumber();
	AnsiString __fastcall GetHomeAddress();
	AnsiString __fastcall GetHomeAddressCity();
	AnsiString __fastcall GetHomeAddressCountry();
	AnsiString __fastcall GetHomeAddressPostalCode();
	AnsiString __fastcall GetHomeAddressPostOfficeBox();
	AnsiString __fastcall GetHomeAddressState();
	AnsiString __fastcall GetHomeAddressStreet();
	AnsiString __fastcall GetHomeFaxNumber();
	AnsiString __fastcall GetHomeTelephoneNumber();
	Activex::TOleEnum __fastcall GetImportance(void);
	AnsiString __fastcall GetInitials();
	AnsiString __fastcall GetInternetFreeBusyAddress();
	AnsiString __fastcall GetISDNNumber();
	AnsiString __fastcall GetJobTitle();
	bool __fastcall GetJournal(void);
	AnsiString __fastcall GetLanguage();
	AnsiString __fastcall GetLastFirstAndSuffix();
	AnsiString __fastcall GetLastFirstNoSpace();
	AnsiString __fastcall GetLastFirstNoSpaceCompany();
	AnsiString __fastcall GetLastFirstSpaceOnly();
	AnsiString __fastcall GetLastFirstSpaceOnlyCompany();
	System::TDateTime __fastcall GetLastModificationTime(void);
	AnsiString __fastcall GetLastName();
	AnsiString __fastcall GetLastNameAndFirstName();
	AnsiString __fastcall GetMailingAddress();
	AnsiString __fastcall GetMailingAddressCity();
	AnsiString __fastcall GetMailingAddressCountry();
	AnsiString __fastcall GetMailingAddressPostalCode();
	AnsiString __fastcall GetMailingAddressPostOfficeBox();
	AnsiString __fastcall GetMailingAddressState();
	AnsiString __fastcall GetMailingAddressStreet();
	AnsiString __fastcall GetManagerName();
	AnsiString __fastcall GetMiddleName();
	AnsiString __fastcall GetMobileTelephoneNumber();
	AnsiString __fastcall GetNetMeetingAlias();
	AnsiString __fastcall GetNetMeetingServer();
	AnsiString __fastcall GetNickName();
	bool __fastcall GetNoAging(void);
	AnsiString __fastcall GetOfficeLocation();
	AnsiString __fastcall GetOrganizationalIDNumber();
	AnsiString __fastcall GetOtherAddress();
	AnsiString __fastcall GetOtherAddressCity();
	AnsiString __fastcall GetOtherAddressCountry();
	AnsiString __fastcall GetOtherAddressPostalCode();
	AnsiString __fastcall GetOtherAddressPostOfficeBox();
	AnsiString __fastcall GetOtherAddressState();
	AnsiString __fastcall GetOtherAddressStreet();
	AnsiString __fastcall GetOtherFaxNumber();
	AnsiString __fastcall GetOtherTelephoneNumber();
	AnsiString __fastcall GetPagerNumber();
	AnsiString __fastcall GetPersonalHomePage();
	AnsiString __fastcall GetPrimaryTelephoneNumber();
	AnsiString __fastcall GetProfession();
	AnsiString __fastcall GetRadioTelephoneNumber();
	AnsiString __fastcall GetReferredBy();
	bool __fastcall GetSaved(void);
	TOpOlMailingAddress __fastcall GetSelectedMailingAddress(void);
	int __fastcall GetSize(void);
	AnsiString __fastcall GetSpouse();
	AnsiString __fastcall GetSuffix();
	AnsiString __fastcall GetTelexNumber();
	AnsiString __fastcall GetTitle();
	AnsiString __fastcall GetTTYTDDTelephoneNumber();
	AnsiString __fastcall GetUser1();
	AnsiString __fastcall GetUser2();
	AnsiString __fastcall GetUser3();
	AnsiString __fastcall GetUser4();
	AnsiString __fastcall GetUserCertificate();
	Opolkxp::_di_UserProperties __fastcall GetUserProperties();
	AnsiString __fastcall GetWebPage();
	AnsiString __fastcall GetYomiCompanyName();
	AnsiString __fastcall GetYomiFirstName();
	AnsiString __fastcall GetYomiLastName();
	void __fastcall SetAccount(const AnsiString Value);
	void __fastcall SetAnniversary(const System::TDateTime Value);
	void __fastcall SetAssistantName(const AnsiString Value);
	void __fastcall SetAssistantTelephoneNumber(const AnsiString Value);
	void __fastcall SetBillingInformation(const AnsiString Value);
	void __fastcall SetBirthday(const System::TDateTime Value);
	void __fastcall SetBusiness2TelephoneNumber(const AnsiString Value);
	void __fastcall SetBusinessAddress(const AnsiString Value);
	void __fastcall SetBusinessAddressCity(const AnsiString Value);
	void __fastcall SetBusinessAddressCountry(const AnsiString Value);
	void __fastcall SetBusinessAddressPostalCode(const AnsiString Value);
	void __fastcall SetBusinessAddressPostOfficeBox(const AnsiString Value);
	void __fastcall SetBusinessAddressState(const AnsiString Value);
	void __fastcall SetBusinessAddressStreet(const AnsiString Value);
	void __fastcall SetBusinessFaxNumber(const AnsiString Value);
	void __fastcall SetBusinessHomePage(const AnsiString Value);
	void __fastcall SetBusinessTelephoneNumber(const AnsiString Value);
	void __fastcall SetCallbackTelephoneNumber(const AnsiString Value);
	void __fastcall SetCarTelephoneNumber(const AnsiString Value);
	void __fastcall SetCategories(const AnsiString Value);
	void __fastcall SetChildren(const AnsiString Value);
	void __fastcall SetCompanies(const AnsiString Value);
	void __fastcall SetCompanyMainTelephoneNumber(const AnsiString Value);
	void __fastcall SetCompanyName(const AnsiString Value);
	void __fastcall SetComputerNetworkName(const AnsiString Value);
	void __fastcall SetCustomerID(const AnsiString Value);
	void __fastcall SetDepartment(const AnsiString Value);
	void __fastcall SetEmail1Address(const AnsiString Value);
	void __fastcall SetEmail1AddressType(const AnsiString Value);
	void __fastcall SetEmail2Address(const AnsiString Value);
	void __fastcall SetEmail2AddressType(const AnsiString Value);
	void __fastcall SetEmail3Address(const AnsiString Value);
	void __fastcall SetEmail3AddressType(const AnsiString Value);
	void __fastcall SetFileAs(const AnsiString Value);
	void __fastcall SetFirstName(const AnsiString Value);
	void __fastcall SetFTPSite(const AnsiString Value);
	void __fastcall SetFullName(const AnsiString Value);
	void __fastcall SetGender(TOpOlGender Value);
	void __fastcall SetGovernmentIDNumber(const AnsiString Value);
	void __fastcall SetHobby(const AnsiString Value);
	void __fastcall SetHome2TelephoneNumber(const AnsiString Value);
	void __fastcall SetHomeAddress(const AnsiString Value);
	void __fastcall SetHomeAddressCity(const AnsiString Value);
	void __fastcall SetHomeAddressCountry(const AnsiString Value);
	void __fastcall SetHomeAddressPostalCode(const AnsiString Value);
	void __fastcall SetHomeAddressPostOfficeBox(const AnsiString Value);
	void __fastcall SetHomeAddressState(const AnsiString Value);
	void __fastcall SetHomeAddressStreet(const AnsiString Value);
	void __fastcall SetHomeFaxNumber(const AnsiString Value);
	void __fastcall SetHomeTelephoneNumber(const AnsiString Value);
	void __fastcall SetImportance(Activex::TOleEnum Value);
	void __fastcall SetInitials(const AnsiString Value);
	void __fastcall SetInternetFreeBusyAddress(const AnsiString Value);
	void __fastcall SetISDNNumber(const AnsiString Value);
	void __fastcall SetJobTitle(const AnsiString Value);
	void __fastcall SetJournal(bool Value);
	void __fastcall SetLanguage(const AnsiString Value);
	void __fastcall SetLastName(const AnsiString Value);
	void __fastcall SetMailingAddress(const AnsiString Value);
	void __fastcall SetMailingAddressCity(const AnsiString Value);
	void __fastcall SetMailingAddressCountry(const AnsiString Value);
	void __fastcall SetMailingAddressPostalCode(const AnsiString Value);
	void __fastcall SetMailingAddressPostOfficeBox(const AnsiString Value);
	void __fastcall SetMailingAddressState(const AnsiString Value);
	void __fastcall SetMailingAddressStreet(const AnsiString Value);
	void __fastcall SetManagerName(const AnsiString Value);
	void __fastcall SetMiddleName(const AnsiString Value);
	void __fastcall SetMobileTelephoneNumber(const AnsiString Value);
	void __fastcall SetNetMeetingAlias(const AnsiString Value);
	void __fastcall SetNetMeetingServer(const AnsiString Value);
	void __fastcall SetNickName(const AnsiString Value);
	void __fastcall SetNoAging(bool Value);
	void __fastcall SetOfficeLocation(const AnsiString Value);
	void __fastcall SetOrganizationalIDNumber(const AnsiString Value);
	void __fastcall SetOtherAddress(const AnsiString Value);
	void __fastcall SetOtherAddressCity(const AnsiString Value);
	void __fastcall SetOtherAddressCountry(const AnsiString Value);
	void __fastcall SetOtherAddressPostalCode(const AnsiString Value);
	void __fastcall SetOtherAddressPostOfficeBox(const AnsiString Value);
	void __fastcall SetOtherAddressState(const AnsiString Value);
	void __fastcall SetOtherAddressStreet(const AnsiString Value);
	void __fastcall SetOtherFaxNumber(const AnsiString Value);
	void __fastcall SetOtherTelephoneNumber(const AnsiString Value);
	void __fastcall SetPagerNumber(const AnsiString Value);
	void __fastcall SetPersonalHomePage(const AnsiString Value);
	void __fastcall SetPrimaryTelephoneNumber(const AnsiString Value);
	void __fastcall SetProfession(const AnsiString Value);
	void __fastcall SetRadioTelephoneNumber(const AnsiString Value);
	void __fastcall SetReferredBy(const AnsiString Value);
	void __fastcall SetSelectedMailingAddress(TOpOlMailingAddress Value);
	void __fastcall SetSpouse(const AnsiString Value);
	void __fastcall SetSuffix(const AnsiString Value);
	void __fastcall SetTelexNumber(const AnsiString Value);
	void __fastcall SetTitle(const AnsiString Value);
	void __fastcall SetTTYTDDTelephoneNumber(const AnsiString Value);
	void __fastcall SetUser1(const AnsiString Value);
	void __fastcall SetUser2(const AnsiString Value);
	void __fastcall SetUser3(const AnsiString Value);
	void __fastcall SetUser4(const AnsiString Value);
	void __fastcall SetUserCertificate(const AnsiString Value);
	void __fastcall SetWebPage(const AnsiString Value);
	void __fastcall SetYomiCompanyName(const AnsiString Value);
	void __fastcall SetYomiFirstName(const AnsiString Value);
	void __fastcall SetYomiLastName(const AnsiString Value);
	
public:
	__fastcall TOpContactItem(Opolkxp::_di__ContactItem AContactItem);
	__property Opolkxp::_di__ContactItem ContactItem = {read=FContactItem};
	void __fastcall Close(TOpOlInspectorClose SaveMode);
	_di_IDispatch __fastcall Copy();
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	_di_IDispatch __fastcall Move(const Opolkxp::_di_MAPIFolder DestFldr);
	void __fastcall PrintOut(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString Path, TOpOlSaveAsType SaveAsType);
	TOpMailItem* __fastcall ForwardAsVcard(void);
	__property AnsiString BillingInformation = {read=GetBillingInformation, write=SetBillingInformation};
	__property AnsiString Categories = {read=GetCategories, write=SetCategories};
	__property AnsiString Companies = {read=GetCompanies, write=SetCompanies};
	__property System::TDateTime CreationTime = {read=GetCreationTime};
	__property AnsiString EntryID = {read=GetEntryID};
	__property Activex::TOleEnum Importance = {read=GetImportance, write=SetImportance, nodefault};
	__property System::TDateTime LastModificationTime = {read=GetLastModificationTime};
	__property bool NoAging = {read=GetNoAging, write=SetNoAging, nodefault};
	__property bool Saved = {read=GetSaved, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property Opolkxp::_di_UserProperties UserProperties = {read=GetUserProperties};
	__property AnsiString Account = {read=GetAccount, write=SetAccount};
	__property System::TDateTime Anniversary = {read=GetAnniversary, write=SetAnniversary};
	__property AnsiString AssistantName = {read=GetAssistantName, write=SetAssistantName};
	__property AnsiString AssistantTelephoneNumber = {read=GetAssistantTelephoneNumber, write=SetAssistantTelephoneNumber};
	__property System::TDateTime Birthday = {read=GetBirthday, write=SetBirthday};
	__property AnsiString Business2TelephoneNumber = {read=GetBusiness2TelephoneNumber, write=SetBusiness2TelephoneNumber};
	__property AnsiString BusinessAddress = {read=GetBusinessAddress, write=SetBusinessAddress};
	__property AnsiString BusinessAddressCity = {read=GetBusinessAddressCity, write=SetBusinessAddressCity};
	__property AnsiString BusinessAddressCountry = {read=GetBusinessAddressCountry, write=SetBusinessAddressCountry};
	__property AnsiString BusinessAddressPostalCode = {read=GetBusinessAddressPostalCode, write=SetBusinessAddressPostalCode};
	__property AnsiString BusinessAddressPostOfficeBox = {read=GetBusinessAddressPostOfficeBox, write=SetBusinessAddressPostOfficeBox};
	__property AnsiString BusinessAddressState = {read=GetBusinessAddressState, write=SetBusinessAddressState};
	__property AnsiString BusinessAddressStreet = {read=GetBusinessAddressStreet, write=SetBusinessAddressStreet};
	__property AnsiString BusinessFaxNumber = {read=GetBusinessFaxNumber, write=SetBusinessFaxNumber};
	__property AnsiString BusinessHomePage = {read=GetBusinessHomePage, write=SetBusinessHomePage};
	__property AnsiString BusinessTelephoneNumber = {read=GetBusinessTelephoneNumber, write=SetBusinessTelephoneNumber};
	__property AnsiString CallbackTelephoneNumber = {read=GetCallbackTelephoneNumber, write=SetCallbackTelephoneNumber};
	__property AnsiString CarTelephoneNumber = {read=GetCarTelephoneNumber, write=SetCarTelephoneNumber};
	__property AnsiString Children = {read=GetChildren, write=SetChildren};
	__property AnsiString CompanyAndFullName = {read=GetCompanyAndFullName};
	__property AnsiString CompanyLastFirstNoSpace = {read=GetCompanyLastFirstNoSpace};
	__property AnsiString CompanyLastFirstSpaceOnly = {read=GetCompanyLastFirstSpaceOnly};
	__property AnsiString CompanyMainTelephoneNumber = {read=GetCompanyMainTelephoneNumber, write=SetCompanyMainTelephoneNumber};
	__property AnsiString CompanyName = {read=GetCompanyName, write=SetCompanyName};
	__property AnsiString ComputerNetworkName = {read=GetComputerNetworkName, write=SetComputerNetworkName};
	__property AnsiString CustomerID = {read=GetCustomerID, write=SetCustomerID};
	__property AnsiString Department = {read=GetDepartment, write=SetDepartment};
	__property AnsiString Email1Address = {read=GetEmail1Address, write=SetEmail1Address};
	__property AnsiString Email1AddressType = {read=GetEmail1AddressType, write=SetEmail1AddressType};
	__property AnsiString Email1DisplayName = {read=GetEmail1DisplayName};
	__property AnsiString Email1EntryID = {read=GetEmail1EntryID};
	__property AnsiString Email2Address = {read=GetEmail2Address, write=SetEmail2Address};
	__property AnsiString Email2AddressType = {read=GetEmail2AddressType, write=SetEmail2AddressType};
	__property AnsiString Email2DisplayName = {read=GetEmail2DisplayName};
	__property AnsiString Email2EntryID = {read=GetEmail2EntryID};
	__property AnsiString Email3Address = {read=GetEmail3Address, write=SetEmail3Address};
	__property AnsiString Email3AddressType = {read=GetEmail3AddressType, write=SetEmail3AddressType};
	__property AnsiString Email3DisplayName = {read=GetEmail3DisplayName};
	__property AnsiString Email3EntryID = {read=GetEmail3EntryID};
	__property AnsiString FileAs = {read=GetFileAs, write=SetFileAs};
	__property AnsiString FirstName = {read=GetFirstName, write=SetFirstName};
	__property AnsiString FTPSite = {read=GetFTPSite, write=SetFTPSite};
	__property AnsiString FullName = {read=GetFullName, write=SetFullName};
	__property AnsiString FullNameAndCompany = {read=GetFullNameAndCompany};
	__property TOpOlGender Gender = {read=GetGender, write=SetGender, nodefault};
	__property AnsiString GovernmentIDNumber = {read=GetGovernmentIDNumber, write=SetGovernmentIDNumber};
	__property AnsiString Hobby = {read=GetHobby, write=SetHobby};
	__property AnsiString Home2TelephoneNumber = {read=GetHome2TelephoneNumber, write=SetHome2TelephoneNumber};
	__property AnsiString HomeAddress = {read=GetHomeAddress, write=SetHomeAddress};
	__property AnsiString HomeAddressCity = {read=GetHomeAddressCity, write=SetHomeAddressCity};
	__property AnsiString HomeAddressCountry = {read=GetHomeAddressCountry, write=SetHomeAddressCountry};
	__property AnsiString HomeAddressPostalCode = {read=GetHomeAddressPostalCode, write=SetHomeAddressPostalCode};
	__property AnsiString HomeAddressPostOfficeBox = {read=GetHomeAddressPostOfficeBox, write=SetHomeAddressPostOfficeBox};
	__property AnsiString HomeAddressState = {read=GetHomeAddressState, write=SetHomeAddressState};
	__property AnsiString HomeAddressStreet = {read=GetHomeAddressStreet, write=SetHomeAddressStreet};
	__property AnsiString HomeFaxNumber = {read=GetHomeFaxNumber, write=SetHomeFaxNumber};
	__property AnsiString HomeTelephoneNumber = {read=GetHomeTelephoneNumber, write=SetHomeTelephoneNumber};
	__property AnsiString Initials = {read=GetInitials, write=SetInitials};
	__property Opolkxp::_di__Inspector ItemInspector = {read=GetInspector};
	__property AnsiString InternetFreeBusyAddress = {read=GetInternetFreeBusyAddress, write=SetInternetFreeBusyAddress};
	__property AnsiString ISDNNumber = {read=GetISDNNumber, write=SetISDNNumber};
	__property AnsiString JobTitle = {read=GetJobTitle, write=SetJobTitle};
	__property bool Journal = {read=GetJournal, write=SetJournal, nodefault};
	__property AnsiString Language = {read=GetLanguage, write=SetLanguage};
	__property AnsiString LastFirstAndSuffix = {read=GetLastFirstAndSuffix};
	__property AnsiString LastFirstNoSpace = {read=GetLastFirstNoSpace};
	__property AnsiString LastFirstNoSpaceCompany = {read=GetLastFirstNoSpaceCompany};
	__property AnsiString LastFirstSpaceOnly = {read=GetLastFirstSpaceOnly};
	__property AnsiString LastFirstSpaceOnlyCompany = {read=GetLastFirstSpaceOnlyCompany};
	__property AnsiString LastName = {read=GetLastName, write=SetLastName};
	__property AnsiString LastNameAndFirstName = {read=GetLastNameAndFirstName};
	__property AnsiString MailingAddress = {read=GetMailingAddress, write=SetMailingAddress};
	__property AnsiString MailingAddressCity = {read=GetMailingAddressCity, write=SetMailingAddressCity};
	__property AnsiString MailingAddressCountry = {read=GetMailingAddressCountry, write=SetMailingAddressCountry};
	__property AnsiString MailingAddressPostalCode = {read=GetMailingAddressPostalCode, write=SetMailingAddressPostalCode};
	__property AnsiString MailingAddressPostOfficeBox = {read=GetMailingAddressPostOfficeBox, write=SetMailingAddressPostOfficeBox};
	__property AnsiString MailingAddressState = {read=GetMailingAddressState, write=SetMailingAddressState};
	__property AnsiString MailingAddressStreet = {read=GetMailingAddressStreet, write=SetMailingAddressStreet};
	__property AnsiString ManagerName = {read=GetManagerName, write=SetManagerName};
	__property AnsiString MiddleName = {read=GetMiddleName, write=SetMiddleName};
	__property AnsiString MobileTelephoneNumber = {read=GetMobileTelephoneNumber, write=SetMobileTelephoneNumber};
	__property AnsiString NetMeetingAlias = {read=GetNetMeetingAlias, write=SetNetMeetingAlias};
	__property AnsiString NetMeetingServer = {read=GetNetMeetingServer, write=SetNetMeetingServer};
	__property AnsiString NickName = {read=GetNickName, write=SetNickName};
	__property AnsiString OfficeLocation = {read=GetOfficeLocation, write=SetOfficeLocation};
	__property AnsiString OrganizationalIDNumber = {read=GetOrganizationalIDNumber, write=SetOrganizationalIDNumber};
	__property AnsiString OtherAddress = {read=GetOtherAddress, write=SetOtherAddress};
	__property AnsiString OtherAddressCity = {read=GetOtherAddressCity, write=SetOtherAddressCity};
	__property AnsiString OtherAddressCountry = {read=GetOtherAddressCountry, write=SetOtherAddressCountry};
	__property AnsiString OtherAddressPostalCode = {read=GetOtherAddressPostalCode, write=SetOtherAddressPostalCode};
	__property AnsiString OtherAddressPostOfficeBox = {read=GetOtherAddressPostOfficeBox, write=SetOtherAddressPostOfficeBox};
	__property AnsiString OtherAddressState = {read=GetOtherAddressState, write=SetOtherAddressState};
	__property AnsiString OtherAddressStreet = {read=GetOtherAddressStreet, write=SetOtherAddressStreet};
	__property AnsiString OtherFaxNumber = {read=GetOtherFaxNumber, write=SetOtherFaxNumber};
	__property AnsiString OtherTelephoneNumber = {read=GetOtherTelephoneNumber, write=SetOtherTelephoneNumber};
	__property AnsiString PagerNumber = {read=GetPagerNumber, write=SetPagerNumber};
	__property AnsiString PersonalHomePage = {read=GetPersonalHomePage, write=SetPersonalHomePage};
	__property AnsiString PrimaryTelephoneNumber = {read=GetPrimaryTelephoneNumber, write=SetPrimaryTelephoneNumber};
	__property AnsiString Profession = {read=GetProfession, write=SetProfession};
	__property AnsiString RadioTelephoneNumber = {read=GetRadioTelephoneNumber, write=SetRadioTelephoneNumber};
	__property AnsiString ReferredBy = {read=GetReferredBy, write=SetReferredBy};
	__property TOpOlMailingAddress SelectedMailingAddress = {read=GetSelectedMailingAddress, write=SetSelectedMailingAddress, nodefault};
	__property AnsiString Spouse = {read=GetSpouse, write=SetSpouse};
	__property AnsiString Suffix = {read=GetSuffix, write=SetSuffix};
	__property AnsiString TelexNumber = {read=GetTelexNumber, write=SetTelexNumber};
	__property AnsiString Title = {read=GetTitle, write=SetTitle};
	__property AnsiString TTYTDDTelephoneNumber = {read=GetTTYTDDTelephoneNumber, write=SetTTYTDDTelephoneNumber};
	__property AnsiString User1 = {read=GetUser1, write=SetUser1};
	__property AnsiString User2 = {read=GetUser2, write=SetUser2};
	__property AnsiString User3 = {read=GetUser3, write=SetUser3};
	__property AnsiString User4 = {read=GetUser4, write=SetUser4};
	__property AnsiString UserCertificate = {read=GetUserCertificate, write=SetUserCertificate};
	__property AnsiString WebPage = {read=GetWebPage, write=SetWebPage};
	__property AnsiString YomiCompanyName = {read=GetYomiCompanyName, write=SetYomiCompanyName};
	__property AnsiString YomiFirstName = {read=GetYomiFirstName, write=SetYomiFirstName};
	__property AnsiString YomiLastName = {read=GetYomiLastName, write=SetYomiLastName};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TOpContactItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpJournalItem;
class PASCALIMPLEMENTATION TOpJournalItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	Opolkxp::_di__JournalItem FJournalItem;
	TOpAttachmentList* FAttachments;
	AnsiString __fastcall GetBillingInformation();
	AnsiString __fastcall GetBody();
	AnsiString __fastcall GetCategories();
	AnsiString __fastcall GetCompanies();
	AnsiString __fastcall GetContactNames();
	AnsiString __fastcall GetConversationIndex();
	AnsiString __fastcall GetConversationTopic();
	System::TDateTime __fastcall GetCreationTime(void);
	bool __fastcall GetDocPosted(void);
	bool __fastcall GetDocPrinted(void);
	bool __fastcall GetDocRouted(void);
	bool __fastcall GetDocSaved(void);
	int __fastcall GetDuration(void);
	System::TDateTime __fastcall GetEnd_(void);
	AnsiString __fastcall GetEntryID();
	TOpOlImportance __fastcall GetImportance(void);
	System::TDateTime __fastcall GetLastModificationTime(void);
	AnsiString __fastcall GetMessageClass();
	AnsiString __fastcall GetMileage();
	bool __fastcall GetNoAging(void);
	int __fastcall GetOutlookInternalVersion(void);
	AnsiString __fastcall GetOutlookVersion();
	bool __fastcall GetSaved(void);
	TOpOlSensitivity __fastcall GetSensitivity(void);
	int __fastcall GetSize(void);
	System::TDateTime __fastcall GetStart(void);
	AnsiString __fastcall GetSubject();
	AnsiString __fastcall GetType_();
	bool __fastcall GetUnRead(void);
	void __fastcall SetBillingInformation(const AnsiString Value);
	void __fastcall SetBody(const AnsiString Value);
	void __fastcall SetCategories(const AnsiString Value);
	void __fastcall SetCompanies(const AnsiString Value);
	void __fastcall SetContactNames(const AnsiString Value);
	void __fastcall SetDocPosted(bool Value);
	void __fastcall SetDocPrinted(bool Value);
	void __fastcall SetDocRouted(bool Value);
	void __fastcall SetDocSaved(bool Value);
	void __fastcall SetDuration(int Value);
	void __fastcall SetEnd_(const System::TDateTime Value);
	void __fastcall SetImportance(TOpOlImportance Value);
	void __fastcall SetMessageClass(const AnsiString Value);
	void __fastcall SetMileage(const AnsiString Value);
	void __fastcall SetNoAging(bool Value);
	void __fastcall SetSensitivity(TOpOlSensitivity Value);
	void __fastcall SetStart(const System::TDateTime Value);
	void __fastcall SetSubject(const AnsiString Value);
	void __fastcall SetType_(const AnsiString Value);
	void __fastcall SetUnRead(bool Value);
	Opolkxp::_di__Inspector __fastcall GetInspector();
	
public:
	__fastcall TOpJournalItem(Opolkxp::_di__JournalItem AJournalItem);
	__fastcall virtual ~TOpJournalItem(void);
	void __fastcall Close(TOpOlInspectorClose SaveMode);
	TOpJournalItem* __fastcall Copy(void);
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	void __fastcall PrintOut(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString Path, TOpOlSaveAsType SaveAsType);
	TOpMailItem* __fastcall Forward(void);
	TOpMailItem* __fastcall Reply(void);
	TOpMailItem* __fastcall ReplyAll(void);
	void __fastcall StartTimer(void);
	void __fastcall StopTimer(void);
	__property TOpAttachmentList* Attachments = {read=FAttachments};
	__property AnsiString BillingInformation = {read=GetBillingInformation, write=SetBillingInformation};
	__property AnsiString Body = {read=GetBody, write=SetBody};
	__property AnsiString Categories = {read=GetCategories, write=SetCategories};
	__property AnsiString Companies = {read=GetCompanies, write=SetCompanies};
	__property AnsiString ConversationIndex = {read=GetConversationIndex};
	__property AnsiString ConversationTopic = {read=GetConversationTopic};
	__property System::TDateTime CreationTime = {read=GetCreationTime};
	__property AnsiString EntryID = {read=GetEntryID};
	__property TOpOlImportance Importance = {read=GetImportance, write=SetImportance, nodefault};
	__property Opolkxp::_di__Inspector ItemInspector = {read=GetInspector};
	__property Opolkxp::_di__JournalItem JournalItem = {read=FJournalItem};
	__property System::TDateTime LastModificationTime = {read=GetLastModificationTime};
	__property AnsiString MessageClass = {read=GetMessageClass, write=SetMessageClass};
	__property AnsiString Mileage = {read=GetMileage, write=SetMileage};
	__property bool NoAging = {read=GetNoAging, write=SetNoAging, nodefault};
	__property int OutlookInternalVersion = {read=GetOutlookInternalVersion, nodefault};
	__property AnsiString OutlookVersion = {read=GetOutlookVersion};
	__property bool Saved = {read=GetSaved, nodefault};
	__property TOpOlSensitivity Sensitivity = {read=GetSensitivity, write=SetSensitivity, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property AnsiString Subject = {read=GetSubject, write=SetSubject};
	__property bool UnRead = {read=GetUnRead, write=SetUnRead, nodefault};
	__property AnsiString ContactNames = {read=GetContactNames, write=SetContactNames};
	__property bool DocPosted = {read=GetDocPosted, write=SetDocPosted, nodefault};
	__property bool DocPrinted = {read=GetDocPrinted, write=SetDocPrinted, nodefault};
	__property bool DocRouted = {read=GetDocRouted, write=SetDocRouted, nodefault};
	__property bool DocSaved = {read=GetDocSaved, write=SetDocSaved, nodefault};
	__property int Duration = {read=GetDuration, write=SetDuration, nodefault};
	__property System::TDateTime End_ = {read=GetEnd_, write=SetEnd_};
	__property AnsiString Type_ = {read=GetType_, write=SetType_};
	__property System::TDateTime Start = {read=GetStart, write=SetStart};
};


class DELPHICLASS TOpNoteItem;
class PASCALIMPLEMENTATION TOpNoteItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	Opolkxp::_di__NoteItem FNoteItem;
	AnsiString __fastcall GetBody();
	AnsiString __fastcall GetCategories();
	TOpOlNoteColor __fastcall GetColor(void);
	System::TDateTime __fastcall GetCreationTime(void);
	AnsiString __fastcall GetEntryID();
	int __fastcall GetHeight(void);
	System::TDateTime __fastcall GetLastModificationTime(void);
	int __fastcall GetLeft(void);
	AnsiString __fastcall GetMessageClass();
	bool __fastcall GetSaved(void);
	int __fastcall GetSize(void);
	AnsiString __fastcall GetSubject();
	int __fastcall GetTop(void);
	int __fastcall GetWidth(void);
	void __fastcall SetBody(const AnsiString Value);
	void __fastcall SetCategories(const AnsiString Value);
	void __fastcall SetColor(const TOpOlNoteColor Value);
	void __fastcall SetHeight(int Value);
	void __fastcall SetLeft(int Value);
	void __fastcall SetMessageClass(const AnsiString Value);
	void __fastcall SetTop(int Value);
	void __fastcall SetWidth(int Value);
	
public:
	__fastcall TOpNoteItem(Opolkxp::_di__NoteItem ANoteItem);
	void __fastcall Close(TOpOlInspectorClose SaveMode);
	TOpNoteItem* __fastcall Copy(void);
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	void __fastcall PrintOut(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString Path, TOpOlSaveAsType SaveAsType);
	__property AnsiString Body = {read=GetBody, write=SetBody};
	__property AnsiString Categories = {read=GetCategories, write=SetCategories};
	__property TOpOlNoteColor Color = {read=GetColor, write=SetColor, nodefault};
	__property System::TDateTime CreationTime = {read=GetCreationTime};
	__property AnsiString EntryID = {read=GetEntryID};
	__property int Height = {read=GetHeight, write=SetHeight, nodefault};
	__property System::TDateTime LastModificationTime = {read=GetLastModificationTime};
	__property int Left = {read=GetLeft, write=SetLeft, nodefault};
	__property AnsiString MessageClass = {read=GetMessageClass, write=SetMessageClass};
	__property Opolkxp::_di__NoteItem NoteItem = {read=FNoteItem};
	__property bool Saved = {read=GetSaved, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property AnsiString Subject = {read=GetSubject};
	__property int Top = {read=GetTop, write=SetTop, nodefault};
	__property int Width = {read=GetWidth, write=SetWidth, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TOpNoteItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpPostItem;
class PASCALIMPLEMENTATION TOpPostItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	TOpAttachmentList* FAttachments;
	Opolkxp::_di__PostItem FPostItem;
	AnsiString __fastcall GetBillingInformation();
	AnsiString __fastcall GetBody();
	AnsiString __fastcall GetCategories();
	AnsiString __fastcall GetCompanies();
	AnsiString __fastcall GetConversationIndex();
	AnsiString __fastcall GetConversationTopic();
	System::TDateTime __fastcall GetCreationTime(void);
	AnsiString __fastcall GetEntryID();
	System::TDateTime __fastcall GetExpiryTime(void);
	AnsiString __fastcall GetHTMLBody();
	TOpOlImportance __fastcall GetImportance(void);
	Opolkxp::_di__Inspector __fastcall GetInspector();
	System::TDateTime __fastcall GetLastModificationTime(void);
	AnsiString __fastcall GetMessageClass();
	AnsiString __fastcall GetMileage();
	bool __fastcall GetNoAging(void);
	int __fastcall GetOutlookInternalVersion(void);
	AnsiString __fastcall GetOutlookVersion();
	System::TDateTime __fastcall GetReceivedTime(void);
	bool __fastcall GetSaved(void);
	AnsiString __fastcall GetSenderName();
	TOpOlSensitivity __fastcall GetSensitivity(void);
	System::TDateTime __fastcall GetSentOn(void);
	int __fastcall GetSize(void);
	AnsiString __fastcall GetSubject();
	bool __fastcall GetUnRead(void);
	void __fastcall SetBillingInformation(const AnsiString Value);
	void __fastcall SetBody(const AnsiString Value);
	void __fastcall SetCategories(const AnsiString Value);
	void __fastcall SetCompanies(const AnsiString Value);
	void __fastcall SetExpiryTime(const System::TDateTime Value);
	void __fastcall SetHTMLBody(const AnsiString Value);
	void __fastcall SetImportance(TOpOlImportance Value);
	void __fastcall SetMessageClass(const AnsiString Value);
	void __fastcall SetMileage(const AnsiString Value);
	void __fastcall SetNoAging(bool Value);
	void __fastcall SetSensitivity(TOpOlSensitivity Value);
	void __fastcall SetSubject(const AnsiString Value);
	void __fastcall SetUnRead(bool Value);
	
public:
	__fastcall TOpPostItem(Opolkxp::_di__PostItem APostItem);
	__fastcall virtual ~TOpPostItem(void);
	void __fastcall Close(TOpOlInspectorClose SaveMode);
	TOpPostItem* __fastcall Copy(void);
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	void __fastcall PrintOut(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString Path, TOpOlSaveAsType SaveAsType);
	void __fastcall ClearConversationIndex(void);
	TOpMailItem* __fastcall Forward(void);
	void __fastcall Post(void);
	TOpMailItem* __fastcall Reply(void);
	__property TOpAttachmentList* Attachments = {read=FAttachments};
	__property AnsiString BillingInformation = {read=GetBillingInformation, write=SetBillingInformation};
	__property AnsiString Body = {read=GetBody, write=SetBody};
	__property AnsiString Categories = {read=GetCategories, write=SetCategories};
	__property AnsiString Companies = {read=GetCompanies, write=SetCompanies};
	__property AnsiString ConversationIndex = {read=GetConversationIndex};
	__property AnsiString ConversationTopic = {read=GetConversationTopic};
	__property System::TDateTime CreationTime = {read=GetCreationTime};
	__property AnsiString EntryID = {read=GetEntryID};
	__property TOpOlImportance Importance = {read=GetImportance, write=SetImportance, nodefault};
	__property Opolkxp::_di__Inspector ItemInspector = {read=GetInspector};
	__property System::TDateTime LastModificationTime = {read=GetLastModificationTime};
	__property AnsiString MessageClass = {read=GetMessageClass, write=SetMessageClass};
	__property AnsiString Mileage = {read=GetMileage, write=SetMileage};
	__property bool NoAging = {read=GetNoAging, write=SetNoAging, nodefault};
	__property int OutlookInternalVersion = {read=GetOutlookInternalVersion, nodefault};
	__property AnsiString OutlookVersion = {read=GetOutlookVersion};
	__property Opolkxp::_di__PostItem PostItem = {read=FPostItem};
	__property bool Saved = {read=GetSaved, nodefault};
	__property TOpOlSensitivity Sensitivity = {read=GetSensitivity, write=SetSensitivity, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property AnsiString Subject = {read=GetSubject, write=SetSubject};
	__property bool UnRead = {read=GetUnRead, write=SetUnRead, nodefault};
	__property System::TDateTime ExpiryTime = {read=GetExpiryTime, write=SetExpiryTime};
	__property AnsiString HTMLBody = {read=GetHTMLBody, write=SetHTMLBody};
	__property System::TDateTime ReceivedTime = {read=GetReceivedTime};
	__property AnsiString SenderName = {read=GetSenderName};
	__property System::TDateTime SentOn = {read=GetSentOn};
};


class DELPHICLASS TOpTaskItem;
class PASCALIMPLEMENTATION TOpTaskItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	TOpAttachmentList* FAttachments;
	TOpRecipientList* FRecipients;
	Opolkxp::_di__TaskItem FTaskItem;
	int __fastcall GetActualWork(void);
	AnsiString __fastcall GetBillingInformation();
	AnsiString __fastcall GetBody();
	AnsiString __fastcall GetCardData();
	AnsiString __fastcall GetCategories();
	AnsiString __fastcall GetCompanies();
	bool __fastcall GetComplete(void);
	AnsiString __fastcall GetContactNames();
	AnsiString __fastcall GetContacts();
	AnsiString __fastcall GetConversationIndex();
	AnsiString __fastcall GetConversationTopic();
	System::TDateTime __fastcall GetCreationTime(void);
	System::TDateTime __fastcall GetDateCompleted(void);
	TOpOlTaskDelegationState __fastcall GetDelegationState(void);
	AnsiString __fastcall GetDelegator();
	System::TDateTime __fastcall GetDueDate(void);
	AnsiString __fastcall GetEntryID();
	Opolkxp::_di__Inspector __fastcall GetInspector();
	TOpOlImportance __fastcall GetImportance(void);
	bool __fastcall GetIsRecurring(void);
	System::TDateTime __fastcall GetLastModificationTime(void);
	AnsiString __fastcall GetMessageClass();
	AnsiString __fastcall GetMileage();
	bool __fastcall GetNoAging(void);
	int __fastcall GetOrdinal(void);
	int __fastcall GetOutlookInternalVersion(void);
	AnsiString __fastcall GetOutlookVersion();
	AnsiString __fastcall GetOwner();
	TOpOlTaskOwnership __fastcall GetOwnership(void);
	int __fastcall GetPercentComplete(void);
	bool __fastcall GetReminderOverrideDefault(void);
	bool __fastcall GetReminderPlaySound(void);
	bool __fastcall GetReminderSet(void);
	AnsiString __fastcall GetReminderSoundFile();
	System::TDateTime __fastcall GetReminderTime(void);
	TOpOlTaskResponse __fastcall GetResponseState(void);
	AnsiString __fastcall GetRole();
	bool __fastcall GetSaved(void);
	AnsiString __fastcall GetSchedulePlusPriority();
	TOpOlSensitivity __fastcall GetSensitivity(void);
	int __fastcall GetSize(void);
	System::TDateTime __fastcall GetStartDate(void);
	TOpOlTaskStatus __fastcall GetStatus(void);
	AnsiString __fastcall GetStatusOnCompletionRecipients();
	AnsiString __fastcall GetStatusUpdateRecipients();
	AnsiString __fastcall GetSubject();
	bool __fastcall GetTeamTask(void);
	int __fastcall GetTotalWork(void);
	bool __fastcall GetUnRead(void);
	void __fastcall SetActualWork(int Value);
	void __fastcall SetBillingInformation(const AnsiString Value);
	void __fastcall SetBody(const AnsiString Value);
	void __fastcall SetCardData(const AnsiString Value);
	void __fastcall SetCategories(const AnsiString Value);
	void __fastcall SetCompanies(const AnsiString Value);
	void __fastcall SetComplete(bool Value);
	void __fastcall SetContactNames(const AnsiString Value);
	void __fastcall SetContacts(const AnsiString Value);
	void __fastcall SetDateCompleted(const System::TDateTime Value);
	void __fastcall SetDueDate(const System::TDateTime Value);
	void __fastcall SetImportance(TOpOlImportance Value);
	void __fastcall SetMessageClass(const AnsiString Value);
	void __fastcall SetMileage(const AnsiString Value);
	void __fastcall SetNoAging(bool Value);
	void __fastcall SetOrdinal(int Value);
	void __fastcall SetOwner(const AnsiString Value);
	void __fastcall SetPercentComplete(int Value);
	void __fastcall SetReminderOverrideDefault(bool Value);
	void __fastcall SetReminderPlaySound(bool Value);
	void __fastcall SetReminderSet(bool Value);
	void __fastcall SetReminderSoundFile(const AnsiString Value);
	void __fastcall SetReminderTime(const System::TDateTime Value);
	void __fastcall SetRole(const AnsiString Value);
	void __fastcall SetSchedulePlusPriority(const AnsiString Value);
	void __fastcall SetSensitivity(TOpOlSensitivity Value);
	void __fastcall SetStartDate(const System::TDateTime Value);
	void __fastcall SetStatus(TOpOlTaskStatus Value);
	void __fastcall SetStatusOnCompletionRecipients(const AnsiString Value);
	void __fastcall SetStatusUpdateRecipients(const AnsiString Value);
	void __fastcall SetSubject(const AnsiString Value);
	void __fastcall SetTeamTask(bool Value);
	void __fastcall SetTotalWork(int Value);
	void __fastcall SetUnRead(bool Value);
	
public:
	__fastcall TOpTaskItem(Opolkxp::_di__TaskItem ATaskItem);
	__fastcall virtual ~TOpTaskItem(void);
	void __fastcall Close(TOpOlInspectorClose SaveMode);
	TOpTaskItem* __fastcall Copy(void);
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	void __fastcall PrintOut(void);
	void __fastcall Save(void);
	void __fastcall SaveAs(const AnsiString Path, TOpOlSaveAsType SaveAsType);
	TOpTaskItem* __fastcall Assign(void);
	void __fastcall CancelResponseState(void);
	void __fastcall ClearRecurrencePattern(void);
	Opolkxp::_di_RecurrencePattern __fastcall GetRecurrencePattern();
	void __fastcall MarkComplete(void);
	TOpTaskItem* __fastcall Respond(TOpOlTaskResponse Response, bool NoUI, bool AdditionalTextDialog);
	void __fastcall Send(void);
	bool __fastcall SkipRecurrence(void);
	_di_IDispatch __fastcall StatusReport();
	__property TOpAttachmentList* Attachments = {read=FAttachments};
	__property AnsiString BillingInformation = {read=GetBillingInformation, write=SetBillingInformation};
	__property AnsiString Body = {read=GetBody, write=SetBody};
	__property AnsiString Categories = {read=GetCategories, write=SetCategories};
	__property AnsiString Companies = {read=GetCompanies, write=SetCompanies};
	__property AnsiString ConversationIndex = {read=GetConversationIndex};
	__property AnsiString ConversationTopic = {read=GetConversationTopic};
	__property System::TDateTime CreationTime = {read=GetCreationTime};
	__property AnsiString EntryID = {read=GetEntryID};
	__property Opolkxp::_di__Inspector ItemInspector = {read=GetInspector};
	__property TOpOlImportance Importance = {read=GetImportance, write=SetImportance, nodefault};
	__property System::TDateTime LastModificationTime = {read=GetLastModificationTime};
	__property AnsiString MessageClass = {read=GetMessageClass, write=SetMessageClass};
	__property AnsiString Mileage = {read=GetMileage, write=SetMileage};
	__property bool NoAging = {read=GetNoAging, write=SetNoAging, nodefault};
	__property int OutlookInternalVersion = {read=GetOutlookInternalVersion, nodefault};
	__property AnsiString OutlookVersion = {read=GetOutlookVersion};
	__property bool Saved = {read=GetSaved, nodefault};
	__property TOpOlSensitivity Sensitivity = {read=GetSensitivity, write=SetSensitivity, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property AnsiString Subject = {read=GetSubject, write=SetSubject};
	__property bool UnRead = {read=GetUnRead, write=SetUnRead, nodefault};
	__property int ActualWork = {read=GetActualWork, write=SetActualWork, nodefault};
	__property AnsiString CardData = {read=GetCardData, write=SetCardData};
	__property bool Complete = {read=GetComplete, write=SetComplete, nodefault};
	__property AnsiString Contacts = {read=GetContacts, write=SetContacts};
	__property AnsiString ContactNames = {read=GetContactNames, write=SetContactNames};
	__property System::TDateTime DateCompleted = {read=GetDateCompleted, write=SetDateCompleted};
	__property TOpOlTaskDelegationState DelegationState = {read=GetDelegationState, nodefault};
	__property AnsiString Delegator = {read=GetDelegator};
	__property System::TDateTime DueDate = {read=GetDueDate, write=SetDueDate};
	__property bool IsRecurring = {read=GetIsRecurring, nodefault};
	__property int Ordinal = {read=GetOrdinal, write=SetOrdinal, nodefault};
	__property AnsiString Owner = {read=GetOwner, write=SetOwner};
	__property TOpOlTaskOwnership Ownership = {read=GetOwnership, nodefault};
	__property int PercentComplete = {read=GetPercentComplete, write=SetPercentComplete, nodefault};
	__property TOpRecipientList* Recipients = {read=FRecipients};
	__property System::TDateTime ReminderTime = {read=GetReminderTime, write=SetReminderTime};
	__property bool ReminderOverrideDefault = {read=GetReminderOverrideDefault, write=SetReminderOverrideDefault, nodefault};
	__property bool ReminderPlaySound = {read=GetReminderPlaySound, write=SetReminderPlaySound, nodefault};
	__property bool ReminderSet = {read=GetReminderSet, write=SetReminderSet, nodefault};
	__property AnsiString ReminderSoundFile = {read=GetReminderSoundFile, write=SetReminderSoundFile};
	__property TOpOlTaskResponse ResponseState = {read=GetResponseState, nodefault};
	__property AnsiString Role = {read=GetRole, write=SetRole};
	__property AnsiString SchedulePlusPriority = {read=GetSchedulePlusPriority, write=SetSchedulePlusPriority};
	__property System::TDateTime StartDate = {read=GetStartDate, write=SetStartDate};
	__property TOpOlTaskStatus Status = {read=GetStatus, write=SetStatus, nodefault};
	__property AnsiString StatusOnCompletionRecipients = {read=GetStatusOnCompletionRecipients, write=SetStatusOnCompletionRecipients};
	__property AnsiString StatusUpdateRecipients = {read=GetStatusUpdateRecipients, write=SetStatusUpdateRecipients};
	__property Opolkxp::_di__TaskItem TaskItem = {read=FTaskItem};
	__property bool TeamTask = {read=GetTeamTask, write=SetTeamTask, nodefault};
	__property int TotalWork = {read=GetTotalWork, write=SetTotalWork, nodefault};
};


class PASCALIMPLEMENTATION TOpMailItem : public TOpOutlookItem 
{
	typedef TOpOutlookItem inherited;
	
private:
	TOpAttachmentList* FAttachments;
	Opolkxp::_di__MailItem FMailItem;
	TOpRecipientList* FRecipients;
	bool __fastcall GetAutoForwarded(void);
	AnsiString __fastcall GetBCC();
	AnsiString __fastcall GetBody();
	AnsiString __fastcall GetCC();
	System::TDateTime __fastcall GetDeferredDeliveryTime(void);
	AnsiString __fastcall GetHTMLBody();
	TOpOlImportance __fastcall GetImportance(void);
	AnsiString __fastcall GetMsgTo();
	bool __fastcall GetSaved(void);
	AnsiString __fastcall GetSenderName();
	TOpOlSensitivity __fastcall GetSensitivity(void);
	bool __fastcall GetSent(void);
	int __fastcall GetSize(void);
	AnsiString __fastcall GetSubject();
	bool __fastcall GetUnRead(void);
	void __fastcall SetAutoForwarded(bool Value);
	void __fastcall SetBCC(const AnsiString Value);
	void __fastcall SetBody(const AnsiString Value);
	void __fastcall SetCC(const AnsiString Value);
	void __fastcall SetDeferredDeliveryTime(const System::TDateTime Value);
	void __fastcall SetHTMLBody(const AnsiString Value);
	void __fastcall SetImportance(TOpOlImportance Value);
	void __fastcall SetMsgTo(const AnsiString Value);
	void __fastcall SetSensitivity(TOpOlSensitivity Value);
	void __fastcall SetSubject(const AnsiString Value);
	void __fastcall SetUnRead(bool Value);
	
public:
	__fastcall TOpMailItem(Opolkxp::_di__MailItem AMailItem);
	__fastcall virtual ~TOpMailItem(void);
	void __fastcall ClearConversationIndex(void);
	void __fastcall Delete(void);
	void __fastcall Display(bool Modal);
	TOpMailItem* __fastcall Forward(void);
	void __fastcall PrintOut(void);
	TOpMailItem* __fastcall Reply(void);
	TOpMailItem* __fastcall ReplyAll(void);
	void __fastcall Save(void);
	void __fastcall Send(void);
	__property TOpAttachmentList* Attachments = {read=FAttachments};
	__property bool AutoForwarded = {read=GetAutoForwarded, write=SetAutoForwarded, nodefault};
	__property AnsiString Body = {read=GetBody, write=SetBody};
	__property System::TDateTime DeferredDeliveryTime = {read=GetDeferredDeliveryTime, write=SetDeferredDeliveryTime};
	__property AnsiString HTMLBody = {read=GetHTMLBody, write=SetHTMLBody};
	__property TOpOlImportance Importance = {read=GetImportance, write=SetImportance, nodefault};
	__property Opolkxp::_di__MailItem MailItem = {read=FMailItem};
	__property AnsiString MsgBCC = {read=GetBCC, write=SetBCC};
	__property AnsiString MsgCC = {read=GetCC, write=SetCC};
	__property AnsiString MsgTo = {read=GetMsgTo, write=SetMsgTo};
	__property TOpRecipientList* Recipients = {read=FRecipients};
	__property bool Saved = {read=GetSaved, nodefault};
	__property AnsiString SenderName = {read=GetSenderName};
	__property TOpOlSensitivity Sensitivity = {read=GetSensitivity, write=SetSensitivity, nodefault};
	__property bool Sent = {read=GetSent, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property AnsiString Subject = {read=GetSubject, write=SetSubject};
	__property bool UnRead = {read=GetUnRead, write=SetUnRead, nodefault};
};


class PASCALIMPLEMENTATION TOpMailListItem : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	TOpMailListProp* FListProp;
	
protected:
	__property TOpMailListProp* ListProp = {read=FListProp};
	
public:
	__fastcall TOpMailListItem(TOpMailListProp* AListProp);
	__fastcall virtual ~TOpMailListItem(void);
};


class PASCALIMPLEMENTATION TOpRecipient : public TOpMailListItem 
{
	typedef TOpMailListItem inherited;
	
private:
	Opolkxp::_di_Recipient FIntf;
	TOpRecipientList* FOwner;
	AnsiString __fastcall GetAddress();
	TOpOlDisplayType __fastcall GetDisplayType(void);
	AnsiString __fastcall GetEntryID();
	int __fastcall GetIndex(void);
	AnsiString __fastcall GetName();
	TOpOlMailRecipientType __fastcall GetRecipientType(void);
	bool __fastcall GetResolved(void);
	TOpOlTrackingStatus __fastcall GetTrackingStatus(void);
	System::TDateTime __fastcall GetTrackingStatusTime(void);
	void __fastcall SetRecipientType(TOpOlMailRecipientType Value);
	void __fastcall SetTrackingStatus(TOpOlTrackingStatus Value);
	void __fastcall SetTrackingStatusTime(const System::TDateTime Value);
	Opolkxp::_di_AddressEntry __fastcall GetAddressEntry();
	void __fastcall SetAddressEntry(const Opolkxp::_di_AddressEntry Value);
	
public:
	__fastcall TOpRecipient(TOpRecipientList* AOwner, Opolkxp::_di_Recipient Intf);
	__fastcall virtual ~TOpRecipient(void);
	bool __fastcall Resolve(void);
	__property AnsiString Address = {read=GetAddress};
	__property Opolkxp::_di_AddressEntry AddressEntry = {read=GetAddressEntry, write=SetAddressEntry};
	__property TOpOlDisplayType DisplayType = {read=GetDisplayType, nodefault};
	__property AnsiString EntryID = {read=GetEntryID};
	__property int Index = {read=GetIndex, nodefault};
	__property AnsiString Name = {read=GetName};
	__property Opolkxp::_di_Recipient RecipientIntf = {read=FIntf};
	__property bool Resolved = {read=GetResolved, nodefault};
	__property TOpOlTrackingStatus TrackingStatus = {read=GetTrackingStatus, write=SetTrackingStatus, nodefault};
	__property System::TDateTime TrackingStatusTime = {read=GetTrackingStatusTime, write=SetTrackingStatusTime};
	__property TOpOlMailRecipientType RecipientType = {read=GetRecipientType, write=SetRecipientType, nodefault};
};


class PASCALIMPLEMENTATION TOpAttachment : public TOpMailListItem 
{
	typedef TOpMailListItem inherited;
	
private:
	TOpAttachmentList* FOwner;
	Opolkxp::_di_Attachment FIntf;
	AnsiString __fastcall GetFileName();
	AnsiString __fastcall GetPathName();
	AnsiString __fastcall GetDisplayName();
	
public:
	__fastcall TOpAttachment(TOpAttachmentList* AOwner, Opolkxp::_di_Attachment Intf);
	__fastcall virtual ~TOpAttachment(void);
	__property Opolkxp::_di_Attachment AttachmentIntf = {read=FIntf};
	__property AnsiString DisplayName = {read=GetDisplayName};
	__property AnsiString FileName = {read=GetFileName};
	__property AnsiString PathName = {read=GetPathName};
};


class DELPHICLASS TOpOutlook;
class PASCALIMPLEMENTATION TOpOutlook : public Opolkxp::TOpOutlookBase 
{
	typedef Opolkxp::TOpOutlookBase inherited;
	
private:
	Opolkxp::_di__NameSpace FMapiNamespace;
	bool FNewSession;
	Opolkxp::_di__Application FServer;
	bool FShowLoginDialog;
	bool FLoggedOn;
	TItemSendEvent FOnItemSend;
	Opshared::TOpOfficeEvent FOnNewMail;
	TReminderEvent FOnReminder;
	TOptionsPagesAddEvent FOnOptionsPagesAdd;
	Opshared::TOpOfficeEvent FOnStartup;
	Opshared::TOpOfficeEvent FOnQuit;
	void __fastcall Disconnect(void);
	AnsiString __fastcall GetName();
	HIDESBASE AnsiString __fastcall GetVersion();
	void __fastcall SetNewSession(bool Value);
	
protected:
	virtual void __fastcall DoConnect(void);
	virtual void __fastcall DoDisconnect(void);
	DYNAMIC void __fastcall DoOnItemSend(const _di_IDispatch Item, Word &Cancel);
	DYNAMIC void __fastcall DoOnNewMail(void);
	DYNAMIC void __fastcall DoOnReminder(const _di_IDispatch Item);
	DYNAMIC void __fastcall DoOnOptionsPagesAdd(const Opolkxp::_di_PropertyPages Pages);
	DYNAMIC void __fastcall DoOnStartup(void);
	DYNAMIC void __fastcall DoOnQuit(void);
	virtual bool __fastcall GetConnected(void);
	virtual Opshared::TOpOfficeVersion __fastcall GetOfficeVersion(void);
	
public:
	Opolkxp::_di__Application __fastcall CreateInstance();
	TOpAppointmentItem* __fastcall CreateAppointmentItem(void);
	TOpContactItem* __fastcall CreateContactItem(void);
	virtual _di_IDispatch __fastcall CreateItem(Opshared::TOpNestedCollectionItem* Item);
	TOpJournalItem* __fastcall CreateJournalItem(void);
	TOpMailItem* __fastcall CreateMailItem(void);
	TOpNoteItem* __fastcall CreateNoteItem(void);
	TOpPostItem* __fastcall CreatePostItem(void);
	TOpTaskItem* __fastcall CreateTaskItem(void);
	virtual void __fastcall GetAppInfo(Classes::TStrings* Info);
	virtual void __fastcall HandleEvent(const GUID &IID, int DispId, const tagVARIANT * Params);
	void __fastcall Login(const AnsiString Profile, const AnsiString Password);
	void __fastcall Logoff(void);
	void __fastcall Quit(void);
	void __fastcall SendMailMessage(const AnsiString ToAddrs, const AnsiString CcAddrs, const AnsiString BccAddrs, const AnsiString MsgSubject, Classes::TStrings* MsgText, Classes::TStrings* MsgAttachments);
	__property Opolkxp::_di__NameSpace MapiNamespace = {read=FMapiNamespace};
	__property AnsiString Name = {read=GetName};
	__property Opolkxp::_di__Application Server = {read=FServer};
	__property AnsiString Version = {read=GetVersion};
	
__published:
	__property Connected  = {default=0};
	__property MachineName ;
	__property bool NewSession = {read=FNewSession, write=SetNewSession, nodefault};
	__property PropDirection  = {default=0};
	__property bool ShowLoginDialog = {read=FShowLoginDialog, write=FShowLoginDialog, nodefault};
	__property OnOpConnect ;
	__property OnOpDisconnect ;
	__property OnGetInstance ;
	__property TItemSendEvent OnItemSend = {read=FOnItemSend, write=FOnItemSend};
	__property Opshared::TOpOfficeEvent OnNewMail = {read=FOnNewMail, write=FOnNewMail};
	__property TReminderEvent OnReminder = {read=FOnReminder, write=FOnReminder};
	__property TOptionsPagesAddEvent OnOptionsPagesAdd = {read=FOnOptionsPagesAdd, write=FOnOptionsPagesAdd};
	__property Opshared::TOpOfficeEvent OnStartup = {read=FOnStartup, write=FOnStartup};
	__property Opshared::TOpOfficeEvent OnQuit = {read=FOnQuit, write=FOnQuit};
public:
	#pragma option push -w-inl
	/* TOpOfficeComponent.Create */ inline __fastcall virtual TOpOutlook(Classes::TComponent* AOwner) : Opolkxp::TOpOutlookBase(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpOfficeComponent.Destroy */ inline __fastcall virtual ~TOpOutlook(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opoutlk */
using namespace Opoutlk;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpOutlk
