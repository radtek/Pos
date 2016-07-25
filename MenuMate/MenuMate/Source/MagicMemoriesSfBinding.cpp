// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : E:\MagicMemoriesWAPI.wsdl
//  >Import : E:\MagicMemoriesWAPI.wsdl>0
//  >Import : E:\MagicMemoriesWAPI.wsdl>1
//  >Import : E:\MagicMemoriesWAPI.wsdl>2
// Encoding : UTF-8
// Codegen  : [wfVerbose-, wfGenerateWarnings-, wfMapArraysToClasses+, wfUseSettersAndGetters+]
// Version  : 1.0
// (17/7/2012 12:07:51 PM - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(__h_magic_memories_sf_binding__)
#include "MagicMemoriesSfBinding.h"
#endif

namespace SfIntegration {

_di_Soap GetSoap(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "E:\enterprise.wsdl";
  static const char* defURL = "https://login.salesforce.com/services/Soap/c/25.0";
  static const char* defSvc = "SforceService";
  static const char* defPrt = "Soap";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_Soap service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall SearchRecord::~SearchRecord()
{
  delete Frecord;
}

__fastcall DeletedRecord::~DeletedRecord()
{
  delete FdeletedDate;
}

__fastcall GetServerTimestampResult::~GetServerTimestampResult()
{
  delete Ftimestamp;
}

__fastcall LoginResult::~LoginResult()
{
  delete FuserInfo;
}

__fastcall WebLink::~WebLink()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Vote::~Vote()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall UserProfileFeed::~UserProfileFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall UserPreference::~UserPreference()
{
  delete FSystemModstamp;
}

__fastcall UserFeed::~UserFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall TraceFlag::~TraceFlag()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FExpirationDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FScope;
  delete FSystemModstamp;
  delete FTracedEntity;
}

__fastcall Time__c_::~Time__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDay__r;
  delete FEnd_Time__c;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FStaff__r;
  delete FStart_Date__c;
  delete FSystemModstamp;
}

__fastcall TaskStatus::~TaskStatus()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall TaskPriority::~TaskPriority()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall TaskFeed::~TaskFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall StaticResource::~StaticResource()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Staff__c_::~Staff__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSite__r;
  delete FStaff_Title__r;
  delete FSystemModstamp;
  delete FTimes__r;
  delete FTransactions__r;
}

__fastcall Staff_Title__c_::~Staff_Title__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSite__r;
  delete FStaff__r;
  delete FSystemModstamp;
}

__fastcall SolutionStatus::~SolutionStatus()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall SolutionHistory::~SolutionHistory()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FSolution;
}

__fastcall SolutionFeed::~SolutionFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall SiteHistory::~SiteHistory()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FSite;
}

__fastcall SiteFeed::~SiteFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall SiteDomain::~SiteDomain()
{
  delete FSite;
  delete FSystemModstamp;
}

__fastcall Site::~Site()
{
  delete FAdmin;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSiteDomains;
  delete FSystemModstamp;
}

__fastcall SetupEntityAccess::~SetupEntityAccess()
{
  delete FParent;
  delete FSystemModstamp;
}

__fastcall SelfServiceUser::~SelfServiceUser()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastLoginDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall ReportFeed::~ReportFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Report::~Report()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLastRunDate;
  delete FSystemModstamp;
}

__fastcall QueueSobject::~QueueSobject()
{
  delete FCreatedBy;
  delete FQueue;
  delete FSystemModstamp;
}

__fastcall PushTopic::~PushTopic()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Product2Feed::~Product2Feed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall ProcessInstanceWorkitem::~ProcessInstanceWorkitem()
{
  delete FActor;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FOriginalActor;
  delete FProcessInstance;
  delete FSystemModstamp;
}

__fastcall ProcessInstanceStep::~ProcessInstanceStep()
{
  delete FActor;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FOriginalActor;
  delete FProcessInstance;
  delete FSystemModstamp;
}

__fastcall ProcessInstanceHistory::~ProcessInstanceHistory()
{
  delete FActor;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FOriginalActor;
  delete FProcessInstance;
  delete FSystemModstamp;
  delete FTargetObject;
}

__fastcall ProcessInstance::~ProcessInstance()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSteps;
  delete FStepsAndWorkitems;
  delete FSystemModstamp;
  delete FTargetObject;
  delete FWorkitems;
}

__fastcall PermissionSetAssignment::~PermissionSetAssignment()
{
  delete FAssignee;
  delete FPermissionSet;
  delete FSystemModstamp;
}

__fastcall UserLicense::~UserLicense()
{
  delete FSystemModstamp;
}

__fastcall Period::~Period()
{
  delete FEndDate;
  delete FFiscalYearSettings;
  delete FStartDate;
  delete FSystemModstamp;
}

__fastcall PartnerRole::~PartnerRole()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Partner::~Partner()
{
  delete FAccountFrom;
  delete FAccountTo;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
  delete FSystemModstamp;
}

__fastcall PLU_Stock__c::~PLU_Stock__c()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDay__r;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
}

__fastcall Organization::~Organization()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
  delete FTrialExpirationDate;
}

__fastcall OrgWideEmailAddress::~OrgWideEmailAddress()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall OpportunityStage::~OpportunityStage()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall OpportunityShare::~OpportunityShare()
{
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
}

__fastcall OpportunityPartner::~OpportunityPartner()
{
  delete FAccountTo;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
  delete FSystemModstamp;
}

__fastcall PricebookEntry::~PricebookEntry()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunityLineItems;
  delete FPricebook2;
  delete FProduct2;
  delete FSystemModstamp;
}

__fastcall OpportunityLineItem::~OpportunityLineItem()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
  delete FPricebookEntry;
  delete FServiceDate;
  delete FSystemModstamp;
}

__fastcall OpportunityHistory::~OpportunityHistory()
{
  delete FCloseDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FOpportunity;
  delete FSystemModstamp;
}

__fastcall OpportunityFieldHistory::~OpportunityFieldHistory()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FOpportunity;
}

__fastcall OpportunityFeed::~OpportunityFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall OpportunityContactRole::~OpportunityContactRole()
{
  delete FContact;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
  delete FSystemModstamp;
}

__fastcall OpportunityCompetitor::~OpportunityCompetitor()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
  delete FSystemModstamp;
}

__fastcall Pricebook2::~Pricebook2()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunities;
  delete FPricebookEntries;
  delete FSystemModstamp;
}

__fastcall OpenActivity::~OpenActivity()
{
  delete FAccount;
  delete FActivityDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FReminderDateTime;
  delete FSystemModstamp;
  delete FWhat;
  delete FWho;
}

__fastcall ObjectPermissions::~ObjectPermissions()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall NoteAndAttachment::~NoteAndAttachment()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Note::~Note()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall NewsFeed::~NewsFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall UserRole::~UserRole()
{
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
  delete FUsers;
}

__fastcall Profile::~Profile()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
  delete FUserLicense;
  delete FUsers;
}

__fastcall Monthly_Variables__c::~Monthly_Variables__c()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDate__c;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSite__r;
  delete FSystemModstamp;
}

__fastcall MailmergeTemplate::~MailmergeTemplate()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLastUsedDate;
  delete FSystemModstamp;
}

__fastcall LoginHistory::~LoginHistory()
{
  delete FLoginTime;
}

__fastcall Transaction__c_::~Transaction__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDate__c;
  delete FDay__r;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLine_Items__r;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FStaff__r;
  delete FSystemModstamp;
}

__fastcall Product__c_::~Product__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLine_Items__r;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSite__r;
  delete FSystemModstamp;
}

__fastcall Line_Item__c_::~Line_Item__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FPLU_Product__r;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
  delete FTransaction__r;
}

__fastcall LeadStatus::~LeadStatus()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall LeadShare::~LeadShare()
{
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLead;
}

__fastcall LeadHistory::~LeadHistory()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLead;
}

__fastcall LeadFeed::~LeadFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Lead::~Lead()
{
  delete FActivityHistories;
  delete FAttachments;
  delete FConvertedAccount;
  delete FConvertedContact;
  delete FConvertedDate;
  delete FConvertedOpportunity;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEmailBouncedDate;
  delete FEmailStatuses;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastActivityDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FMasterRecord;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FShares;
  delete FSystemModstamp;
  delete FTasks;
}

__fastcall RecordType::~RecordType()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall KPI__c_::~KPI__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FKPI_Values__r;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
}

__fastcall KPI_Value__c_::~KPI_Value__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDay__r;
  delete FFeedSubscriptionsForEntity;
  delete FKPI__r;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FRecordType;
  delete FSystemModstamp;
}

__fastcall IdeaComment::~IdeaComment()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FIdea;
  delete FSystemModstamp;
}

__fastcall Idea::~Idea()
{
  delete FComments;
  delete FCommunity;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastComment;
  delete FLastCommentDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
  delete FVotes;
}

__fastcall IDEWorkspace::~IDEWorkspace()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
  delete FUser;
}

__fastcall IDEPerspective::~IDEPerspective()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
  delete FUser;
}

__fastcall Holiday::~Holiday()
{
  delete FActivityDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FRecurrenceEndDateOnly;
  delete FRecurrenceStartDate;
  delete FSystemModstamp;
}

__fastcall GroupMember::~GroupMember()
{
  delete FGroup;
  delete FSystemModstamp;
}

__fastcall Group::~Group()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDelegatedUsers;
  delete FGroupMembers;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FQueueSobjects;
  delete FSystemModstamp;
}

__fastcall ForecastingQuota::~ForecastingQuota()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FQuotaOwner;
  delete FStartDate;
  delete FSystemModstamp;
}

__fastcall ForecastShare::~ForecastShare()
{
  delete FLastModifiedBy;
  delete FLastModifiedDate;
}

__fastcall FiscalYearSettings::~FiscalYearSettings()
{
  delete FEndDate;
  delete FPeriods;
  delete FStartDate;
  delete FSystemModstamp;
}

__fastcall PermissionSet::~PermissionSet()
{
  delete FAssignments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFieldPerms;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FObjectPerms;
  delete FProfile;
  delete FSetupEntityAccessItems;
  delete FSystemModstamp;
  delete FUserLicense;
}

__fastcall FieldPermissions::~FieldPermissions()
{
  delete FParent;
  delete FSystemModstamp;
}

__fastcall FeedLike::~FeedLike()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FInsertedBy;
}

__fastcall FeedItem::~FeedItem()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall FeedComment::~FeedComment()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FInsertedBy;
}

__fastcall EventFeed::~EventFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall EventAttendee::~EventAttendee()
{
  delete FAttendee;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvent;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FRespondedDate;
  delete FSystemModstamp;
}

__fastcall Event::~Event()
{
  delete FAccount;
  delete FActivityDate;
  delete FActivityDateTime;
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEndDateTime;
  delete FEventAttendees;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FRecurrenceEndDateOnly;
  delete FRecurrenceStartDateTime;
  delete FRecurringEvents;
  delete FReminderDateTime;
  delete FStartDateTime;
  delete FSystemModstamp;
  delete FWhat;
  delete FWho;
}

__fastcall EntitySubscription::~EntitySubscription()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FParent;
  delete FSubscriber;
}

__fastcall EmailTemplate::~EmailTemplate()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFolder;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLastUsedDate;
  delete FOwner;
  delete FSystemModstamp;
}

__fastcall Task::~Task()
{
  delete FAccount;
  delete FActivityDate;
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FRecurrenceEndDateOnly;
  delete FRecurrenceStartDateOnly;
  delete FRecurringTasks;
  delete FReminderDateTime;
  delete FSystemModstamp;
  delete FWhat;
  delete FWho;
}

__fastcall EmailStatus::~EmailStatus()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFirstOpenDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLastOpenDate;
  delete FTask;
  delete FWho;
}

__fastcall EmailServicesFunction::~EmailServicesFunction()
{
  delete FAddresses;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall EmailServicesAddress::~EmailServicesAddress()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFunction;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall DocumentAttachmentMap::~DocumentAttachmentMap()
{
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall Document::~Document()
{
  delete FAuthor;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFolder;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Payment_Type__c_::~Payment_Type__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDeposit_Details__r;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
}

__fastcall Deposit__c_::~Deposit__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDay__r;
  delete FDeposit_Details__r;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
}

__fastcall Deposit_Detail__c_::~Deposit_Detail__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDeposit__r;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FPayment_Type__r;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
}

__fastcall Site__c_::~Site__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDays__r;
  delete FFeedSubscriptionsForEntity;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FMonthly_Variables__r;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FProducts__r;
  delete FStaff_Titles__r;
  delete FStaff__r;
  delete FSystemModstamp;
}

__fastcall Day__c_::~Day__c_()
{
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDate__c;
  delete FDeposits__r;
  delete FFeedSubscriptionsForEntity;
  delete FKPI_Values__r;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FPLU_Stocks__r;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSite__r;
  delete FSystemModstamp;
  delete FTimes__r;
  delete FTransactions__r;
}

__fastcall DashboardFeed::~DashboardFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall DashboardComponentFeed::~DashboardComponentFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall DashboardComponent::~DashboardComponent()
{
  delete FDashboard;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
}

__fastcall Folder::~Folder()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Dashboard::~Dashboard()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDashboardComponents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FFolder;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FRunningUser;
  delete FSystemModstamp;
}

__fastcall CronTrigger::~CronTrigger()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEndTime;
  delete FLastModifiedBy;
  delete FNextFireTime;
  delete FPreviousFireTime;
  delete FStartTime;
}

__fastcall ContractStatus::~ContractStatus()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall ContractHistory::~ContractHistory()
{
  delete FContract;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall ContractFeed::~ContractFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall ContractContactRole::~ContractContactRole()
{
  delete FContact;
  delete FContract;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall ContentVersionHistory::~ContentVersionHistory()
{
  delete FContentVersion;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall ContentDocumentLink::~ContentDocumentLink()
{
  delete FContentDocument;
  delete FLinkedEntity;
  delete FSystemModstamp;
}

__fastcall ContentDocumentHistory::~ContentDocumentHistory()
{
  delete FContentDocument;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall ContentDocumentFeed::~ContentDocumentFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall ContentVersion::~ContentVersion()
{
  delete FContentDocument;
  delete FContentModifiedDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeaturedContentDate;
  delete FFirstPublishLocation;
  delete FHistories;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FSystemModstamp;
}

__fastcall ContentDocument::~ContentDocument()
{
  delete FArchivedDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLatestPublishedVersion;
  delete FOwner;
  delete FSystemModstamp;
}

__fastcall ContactShare::~ContactShare()
{
  delete FContact;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
}

__fastcall ContactHistory::~ContactHistory()
{
  delete FContact;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall ContactFeed::~ContactFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Community::~Community()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CollaborationInvitation::~CollaborationInvitation()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CollaborationGroupMemberRequest::~CollaborationGroupMemberRequest()
{
  delete FCollaborationGroup;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CollaborationGroupMember::~CollaborationGroupMember()
{
  delete FCollaborationGroup;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CollaborationGroupFeed::~CollaborationGroupFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall CollaborationGroup::~CollaborationGroup()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FGroupMemberRequests;
  delete FGroupMembers;
  delete FLastFeedModifiedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FSystemModstamp;
}

__fastcall ChatterActivity::~ChatterActivity()
{
  delete FSystemModstamp;
}

__fastcall CategoryNode::~CategoryNode()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CategoryData::~CategoryData()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CaseTeamTemplateRecord::~CaseTeamTemplateRecord()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall CaseTeamTemplateMember::~CaseTeamTemplateMember()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CaseTeamTemplate::~CaseTeamTemplate()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CaseTeamRole::~CaseTeamRole()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CaseTeamMember::~CaseTeamMember()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall CaseStatus::~CaseStatus()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Solution::~Solution()
{
  delete FActivityHistories;
  delete FAttachments;
  delete FCaseSolutions;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpenActivities;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
  delete FTasks;
  delete FVotes;
}

__fastcall CaseSolution::~CaseSolution()
{
  delete FCase;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FSolution;
  delete FSystemModstamp;
}

__fastcall CaseShare::~CaseShare()
{
  delete FCase;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
}

__fastcall CaseHistory::~CaseHistory()
{
  delete FCase;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall CaseFeed::~CaseFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall CaseContactRole::~CaseContactRole()
{
  delete FCases;
  delete FContact;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall CaseComment::~CaseComment()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Case::~Case()
{
  delete FAccount;
  delete FActivityHistories;
  delete FAttachments;
  delete FCaseComments;
  delete FCaseContactRoles;
  delete FCaseSolutions;
  delete FCases;
  delete FClosedDate;
  delete FContact;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpenActivities;
  delete FOwner;
  delete FParent;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FShares;
  delete FSolutions;
  delete FSystemModstamp;
  delete FTasks;
  delete FTeamMembers;
  delete FTeamTemplateRecords;
}

__fastcall CallCenter::~CallCenter()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall BusinessProcess::~BusinessProcess()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall BusinessHours::~BusinessHours()
{
  delete FCases;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFridayEndTime;
  delete FFridayStartTime;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FMondayEndTime;
  delete FMondayStartTime;
  delete FSaturdayEndTime;
  delete FSaturdayStartTime;
  delete FSundayEndTime;
  delete FSundayStartTime;
  delete FSystemModstamp;
  delete FThursdayEndTime;
  delete FThursdayStartTime;
  delete FTuesdayEndTime;
  delete FTuesdayStartTime;
  delete FWednesdayEndTime;
  delete FWednesdayStartTime;
}

__fastcall BrandTemplate::~BrandTemplate()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall Attachment::~Attachment()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall AssignmentRule::~AssignmentRule()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall AssetFeed::~AssetFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Product2::~Product2()
{
  delete FActivityHistories;
  delete FAssets;
  delete FAttachments;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FOpportunityLineItems;
  delete FPricebookEntries;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FSystemModstamp;
  delete FTasks;
}

__fastcall Asset::~Asset()
{
  delete FAccount;
  delete FActivityHistories;
  delete FAttachments;
  delete FCases;
  delete FContact;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FInstallDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FProduct2;
  delete FPurchaseDate;
  delete FSystemModstamp;
  delete FTasks;
  delete FUsageEndDate;
}

__fastcall Contract::~Contract()
{
  delete FAccount;
  delete FActivatedBy;
  delete FActivatedDate;
  delete FActivityHistories;
  delete FApprovals;
  delete FAttachments;
  delete FCompanySigned;
  delete FCompanySignedDate;
  delete FContractContactRoles;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FCustomerSigned;
  delete FCustomerSignedDate;
  delete FEndDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastActivityDate;
  delete FLastApprovedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FStartDate;
  delete FSystemModstamp;
  delete FTasks;
}

__fastcall Approval::~Approval()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall ApexTrigger::~ApexTrigger()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall AsyncApexJob::~AsyncApexJob()
{
  delete FApexClass;
  delete FCompletedDate;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall ApexTestResult::~ApexTestResult()
{
  delete FApexClass;
  delete FApexLog;
  delete FAsyncApexJob;
  delete FQueueItem;
  delete FSystemModstamp;
  delete FTestTimestamp;
}

__fastcall ApexTestQueueItem::~ApexTestQueueItem()
{
  delete FApexClass;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FSystemModstamp;
}

__fastcall ApexPage::~ApexPage()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSetupEntityAccessItems;
  delete FSystemModstamp;
}

__fastcall ApexLog::~ApexLog()
{
  delete FLastModifiedDate;
  delete FLogUser;
  delete FStartTime;
  delete FSystemModstamp;
}

__fastcall ApexExecutionOverlayAction::~ApexExecutionOverlayAction()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FExecutableEntity;
  delete FExpirationDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FScope;
  delete FSystemModstamp;
}

__fastcall ApexComponent::~ApexComponent()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall ApexClass::~ApexClass()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSetupEntityAccessItems;
  delete FSystemModstamp;
}

__fastcall AdditionalNumber::~AdditionalNumber()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall ActivityHistory::~ActivityHistory()
{
  delete FAccount;
  delete FActivityDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOwner;
  delete FReminderDateTime;
  delete FSystemModstamp;
  delete FWhat;
  delete FWho;
}

__fastcall AccountShare::~AccountShare()
{
  delete FAccount;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
}

__fastcall Opportunity::~Opportunity()
{
  delete FAccount;
  delete FAccountPartners;
  delete FActivityHistories;
  delete FAttachments;
  delete FCloseDate;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastActivityDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FOpportunityCompetitors;
  delete FOpportunityContactRoles;
  delete FOpportunityHistories;
  delete FOpportunityLineItems;
  delete FOpportunityPartnersFrom;
  delete FOwner;
  delete FPartners;
  delete FPricebook2;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FShares;
  delete FSystemModstamp;
  delete FTasks;
}

__fastcall AccountPartner::~AccountPartner()
{
  delete FAccountFrom;
  delete FAccountTo;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FOpportunity;
  delete FSystemModstamp;
}

__fastcall AccountHistory::~AccountHistory()
{
  delete FAccount;
  delete FCreatedBy;
  delete FCreatedDate;
}

__fastcall Name_::~Name_()
{
  delete FProfile;
  delete FUserRole;
}

__fastcall AccountFeed::~AccountFeed()
{
  delete FCreatedBy;
  delete FCreatedDate;
  delete FFeedComments;
  delete FFeedLikes;
  delete FFeedTrackedChanges;
  delete FInsertedBy;
  delete FLastModifiedDate;
  delete FParent;
  delete FSystemModstamp;
}

__fastcall Contact::~Contact()
{
  delete FAccount;
  delete FAccountContactRoles;
  delete FActivityHistories;
  delete FAssets;
  delete FAttachments;
  delete FBirthdate;
  delete FCaseContactRoles;
  delete FCases;
  delete FContractContactRoles;
  delete FContractsSigned;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEmailBouncedDate;
  delete FEmailStatuses;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastActivityDate;
  delete FLastCURequestDate;
  delete FLastCUUpdateDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FMasterRecord;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FOpportunities;
  delete FOpportunityContactRoles;
  delete FOwner;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FReportsTo;
  delete FShares;
  delete FSystemModstamp;
  delete FTasks;
}

__fastcall AccountContactRole::~AccountContactRole()
{
  delete FAccount;
  delete FContact;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FSystemModstamp;
}

__fastcall User::~User()
{
  delete FContact;
  delete FContractsSigned;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FDelegatedUsers;
  delete FFeedSubscriptions;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FLastLoginDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FLastPasswordChangeDate;
  delete FManager;
  delete FOfflinePdaTrialExpirationDate;
  delete FOfflineTrialExpirationDate;
  delete FPermissionSetAssignments;
  delete FProfile;
  delete FSystemModstamp;
  delete FUserPreferences;
  delete FUserRole;
}

__fastcall Account::~Account()
{
  delete FAccountContactRoles;
  delete FAccountPartnersFrom;
  delete FAccountPartnersTo;
  delete FActivityHistories;
  delete FAssets;
  delete FAttachments;
  delete FCases;
  delete FContacts;
  delete FContracts;
  delete FCreatedBy;
  delete FCreatedDate;
  delete FEvents;
  delete FFeedSubscriptionsForEntity;
  delete FFeeds;
  delete FHistories;
  delete FLastActivityDate;
  delete FLastModifiedBy;
  delete FLastModifiedDate;
  delete FMasterRecord;
  delete FNotes;
  delete FNotesAndAttachments;
  delete FOpenActivities;
  delete FOpportunities;
  delete FOpportunityPartnersTo;
  delete FOwner;
  delete FParent;
  delete FPartnersFrom;
  delete FPartnersTo;
  delete FProcessInstances;
  delete FProcessSteps;
  delete FShares;
  delete FSystemModstamp;
  delete FTasks;
}

__fastcall QueryResult::~QueryResult()
{
  for(int i=0; i<Frecords.Length; i++)
    if (Frecords[i])
      delete Frecords[i];
}

__fastcall SearchResult::~SearchResult()
{
  for(int i=0; i<FsearchRecords.Length; i++)
    if (FsearchRecords[i])
      delete FsearchRecords[i];
}

__fastcall MergeRequest::~MergeRequest()
{
  delete FmasterRecord;
}

__fastcall GetUpdatedResult::~GetUpdatedResult()
{
  delete FlatestDateCovered;
}

__fastcall GetDeletedResult::~GetDeletedResult()
{
  delete FearliestDateAvailable;
  delete FlatestDateCovered;
  for(int i=0; i<FdeletedRecords.Length; i++)
    if (FdeletedRecords[i])
      delete FdeletedRecords[i];
}

__fastcall LeadConvertResult::~LeadConvertResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall EmptyRecycleBinResult::~EmptyRecycleBinResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall UndeleteResult::~UndeleteResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall DeleteResult::~DeleteResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall ProcessResult::~ProcessResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall MergeResult::~MergeResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall UpsertResult::~UpsertResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall SaveResult::~SaveResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall InvalidateSessionsResult::~InvalidateSessionsResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall DescribeSObjectResult::~DescribeSObjectResult()
{
  for(int i=0; i<FchildRelationships.Length; i++)
    if (FchildRelationships[i])
      delete FchildRelationships[i];
  for(int i=0; i<Ffields.Length; i++)
    if (Ffields[i])
      delete Ffields[i];
  for(int i=0; i<FrecordTypeInfos.Length; i++)
    if (FrecordTypeInfos[i])
      delete FrecordTypeInfos[i];
}

__fastcall PicklistForRecordType::~PicklistForRecordType()
{
  for(int i=0; i<FpicklistValues.Length; i++)
    if (FpicklistValues[i])
      delete FpicklistValues[i];
}

__fastcall Field::~Field()
{
  for(int i=0; i<FpicklistValues.Length; i++)
    if (FpicklistValues[i])
      delete FpicklistValues[i];
}

__fastcall DescribeGlobalResult::~DescribeGlobalResult()
{
  for(int i=0; i<Fsobjects.Length; i++)
    if (Fsobjects[i])
      delete Fsobjects[i];
}

__fastcall DataCategory::~DataCategory()
{
  for(int i=0; i<FchildCategories.Length; i++)
    if (FchildCategories[i])
      delete FchildCategories[i];
}

__fastcall DescribeDataCategoryGroupStructureResult::~DescribeDataCategoryGroupStructureResult()
{
  for(int i=0; i<FtopCategories.Length; i++)
    if (FtopCategories[i])
      delete FtopCategories[i];
}

__fastcall DescribeSoftphoneLayoutResult::~DescribeSoftphoneLayoutResult()
{
  for(int i=0; i<FcallTypes.Length; i++)
    if (FcallTypes[i])
      delete FcallTypes[i];
}

__fastcall DescribeSoftphoneLayoutCallType::~DescribeSoftphoneLayoutCallType()
{
  for(int i=0; i<FinfoFields.Length; i++)
    if (FinfoFields[i])
      delete FinfoFields[i];
  for(int i=0; i<FscreenPopOptions.Length; i++)
    if (FscreenPopOptions[i])
      delete FscreenPopOptions[i];
  for(int i=0; i<Fsections.Length; i++)
    if (Fsections[i])
      delete Fsections[i];
}

__fastcall DescribeSoftphoneLayoutSection::~DescribeSoftphoneLayoutSection()
{
  for(int i=0; i<Fitems.Length; i++)
    if (Fitems[i])
      delete Fitems[i];
}

__fastcall DescribeLayoutResult::~DescribeLayoutResult()
{
  for(int i=0; i<Flayouts.Length; i++)
    if (Flayouts[i])
      delete Flayouts[i];
  for(int i=0; i<FrecordTypeMappings.Length; i++)
    if (FrecordTypeMappings[i])
      delete FrecordTypeMappings[i];
}

__fastcall DescribeLayout::~DescribeLayout()
{
  delete FbuttonLayoutSection;
  for(int i=0; i<FdetailLayoutSections.Length; i++)
    if (FdetailLayoutSections[i])
      delete FdetailLayoutSections[i];
  for(int i=0; i<FeditLayoutSections.Length; i++)
    if (FeditLayoutSections[i])
      delete FeditLayoutSections[i];
  for(int i=0; i<FrelatedLists.Length; i++)
    if (FrelatedLists[i])
      delete FrelatedLists[i];
}

__fastcall RecordTypeMapping::~RecordTypeMapping()
{
  for(int i=0; i<FpicklistsForRecordType.Length; i++)
    if (FpicklistsForRecordType[i])
      delete FpicklistsForRecordType[i];
}

__fastcall DescribeLayoutButtonSection::~DescribeLayoutButtonSection()
{
  for(int i=0; i<FdetailButtons.Length; i++)
    if (FdetailButtons[i])
      delete FdetailButtons[i];
}

__fastcall DescribeLayoutSection::~DescribeLayoutSection()
{
  for(int i=0; i<FlayoutRows.Length; i++)
    if (FlayoutRows[i])
      delete FlayoutRows[i];
}

__fastcall RelatedList::~RelatedList()
{
  for(int i=0; i<Fcolumns.Length; i++)
    if (Fcolumns[i])
      delete Fcolumns[i];
  for(int i=0; i<Fsort.Length; i++)
    if (Fsort[i])
      delete Fsort[i];
}

__fastcall DescribeLayoutRow::~DescribeLayoutRow()
{
  for(int i=0; i<FlayoutItems.Length; i++)
    if (FlayoutItems[i])
      delete FlayoutItems[i];
}

__fastcall DescribeLayoutItem::~DescribeLayoutItem()
{
  for(int i=0; i<FlayoutComponents.Length; i++)
    if (FlayoutComponents[i])
      delete FlayoutComponents[i];
}

__fastcall SingleEmailMessage::~SingleEmailMessage()
{
  for(int i=0; i<FfileAttachments.Length; i++)
    if (FfileAttachments[i])
      delete FfileAttachments[i];
}

__fastcall SendEmailResult::~SendEmailResult()
{
  for(int i=0; i<Ferrors.Length; i++)
    if (Ferrors[i])
      delete Ferrors[i];
}

__fastcall DescribeTabSetResult::~DescribeTabSetResult()
{
  for(int i=0; i<Ftabs.Length; i++)
    if (Ftabs[i])
      delete Ftabs[i];
}

__fastcall PackageVersionHeader::~PackageVersionHeader()
{
  for(int i=0; i<FpackageVersions.Length; i++)
    if (FpackageVersions[i])
      delete FpackageVersions[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* Soap */
  InvRegistry()->RegisterInterface(__delphirtti(Soap), L"urn:enterprise.soap.sforce.com", L"UTF-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(Soap), L"");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(Soap), ioDocument);
  InvRegistry()->RegisterExternalMethName(__delphirtti(Soap), L"delete_", L"delete");
  /* SearchRecord */
  RemClassRegistry()->RegisterXSClass(__classid(SearchRecord), L"urn:enterprise.soap.sforce.com", L"SearchRecord");
  /* DeletedRecord */
  RemClassRegistry()->RegisterXSClass(__classid(DeletedRecord), L"urn:enterprise.soap.sforce.com", L"DeletedRecord");
  /* GetServerTimestampResult */
  RemClassRegistry()->RegisterXSClass(__classid(GetServerTimestampResult), L"urn:enterprise.soap.sforce.com", L"GetServerTimestampResult");
  /* SetPasswordResult */
  RemClassRegistry()->RegisterXSClass(__classid(SetPasswordResult), L"urn:enterprise.soap.sforce.com", L"SetPasswordResult");
  /* ResetPasswordResult */
  RemClassRegistry()->RegisterXSClass(__classid(ResetPasswordResult), L"urn:enterprise.soap.sforce.com", L"ResetPasswordResult");
  /* GetUserInfoResult */
  RemClassRegistry()->RegisterXSClass(__classid(GetUserInfoResult), L"urn:enterprise.soap.sforce.com", L"GetUserInfoResult");
  /* LoginResult */
  RemClassRegistry()->RegisterXSClass(__classid(LoginResult), L"urn:enterprise.soap.sforce.com", L"LoginResult");
  /* StatusCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(StatusCode_TypeInfoHolder)), L"urn:enterprise.soap.sforce.com", L"StatusCode");
  /* LeadConvert */
  RemClassRegistry()->RegisterXSClass(__classid(LeadConvert), L"urn:enterprise.soap.sforce.com", L"LeadConvert");
  /* ChildRelationship */
  RemClassRegistry()->RegisterXSClass(__classid(ChildRelationship), L"urn:enterprise.soap.sforce.com", L"ChildRelationship");
  /* RecordTypeInfo */
  RemClassRegistry()->RegisterXSClass(__classid(RecordTypeInfo), L"urn:enterprise.soap.sforce.com", L"RecordTypeInfo");
  /* DescribeGlobalSObjectResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeGlobalSObjectResult), L"urn:enterprise.soap.sforce.com", L"DescribeGlobalSObjectResult");
  /* PicklistEntry */
  RemClassRegistry()->RegisterXSClass(__classid(PicklistEntry), L"urn:enterprise.soap.sforce.com", L"PicklistEntry");
  /* soapType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"urn:enterprise.soap.sforce.com", L"soapType");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"tns_ID", L"tns:ID");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_base64Binary", L"xsd:base64Binary");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_boolean", L"xsd:boolean");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_double", L"xsd:double");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_int", L"xsd:int");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_string", L"xsd:string");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_date", L"xsd:date");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_dateTime", L"xsd:dateTime");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_time", L"xsd:time");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(soapType_TypeInfoHolder)), L"xsd_anyType", L"xsd:anyType");
  /* fieldType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(fieldType_TypeInfoHolder)), L"urn:enterprise.soap.sforce.com", L"fieldType");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(fieldType_TypeInfoHolder)), L"int_", L"int");
  RemClassRegistry()->RegisterExternalPropName(GetClsMemberTypeInfo(__typeinfo(fieldType_TypeInfoHolder)), L"double_", L"double");
  /* DescribeDataCategoryGroupResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeDataCategoryGroupResult), L"urn:enterprise.soap.sforce.com", L"DescribeDataCategoryGroupResult");
  /* DataCategoryGroupSobjectTypePair */
  RemClassRegistry()->RegisterXSClass(__classid(DataCategoryGroupSobjectTypePair), L"urn:enterprise.soap.sforce.com", L"DataCategoryGroupSobjectTypePair");
  /* DescribeSoftphoneLayoutInfoField */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSoftphoneLayoutInfoField), L"urn:enterprise.soap.sforce.com", L"DescribeSoftphoneLayoutInfoField");
  /* DescribeSoftphoneScreenPopOption */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSoftphoneScreenPopOption), L"urn:enterprise.soap.sforce.com", L"DescribeSoftphoneScreenPopOption");
  /* DescribeSoftphoneLayoutItem */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSoftphoneLayoutItem), L"urn:enterprise.soap.sforce.com", L"DescribeSoftphoneLayoutItem");
  /* DescribeLayoutButton */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutButton), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutButton");
  /* layoutComponentType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(layoutComponentType_TypeInfoHolder)), L"urn:enterprise.soap.sforce.com", L"layoutComponentType");
  /* DescribeLayoutComponent */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutComponent), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutComponent");
  /* RelatedListColumn */
  RemClassRegistry()->RegisterXSClass(__classid(RelatedListColumn), L"urn:enterprise.soap.sforce.com", L"RelatedListColumn");
  /* RelatedListSort */
  RemClassRegistry()->RegisterXSClass(__classid(RelatedListSort), L"urn:enterprise.soap.sforce.com", L"RelatedListSort");
  /* EmailPriority */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(EmailPriority_TypeInfoHolder)), L"urn:enterprise.soap.sforce.com", L"EmailPriority");
  /* Email */
  RemClassRegistry()->RegisterXSClass(__classid(Email), L"urn:enterprise.soap.sforce.com", L"Email");
  /* DescribeTab */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeTab), L"urn:enterprise.soap.sforce.com", L"DescribeTab");
  /* PackageVersion */
  RemClassRegistry()->RegisterXSClass(__classid(PackageVersion), L"urn:enterprise.soap.sforce.com", L"PackageVersion");
  /* DebugLevel */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(DebugLevel_TypeInfoHolder)), L"urn:enterprise.soap.sforce.com", L"DebugLevel");
  /* SessionHeader */
  RemClassRegistry()->RegisterXSClass(__classid(SessionHeader), L"urn:enterprise.soap.sforce.com", L"SessionHeader");
  /* LoginScopeHeader */
  RemClassRegistry()->RegisterXSClass(__classid(LoginScopeHeader), L"urn:enterprise.soap.sforce.com", L"LoginScopeHeader");
  /* QueryOptions */
  RemClassRegistry()->RegisterXSClass(__classid(QueryOptions), L"urn:enterprise.soap.sforce.com", L"QueryOptions");
  /* DebuggingHeader */
  RemClassRegistry()->RegisterXSClass(__classid(DebuggingHeader), L"urn:enterprise.soap.sforce.com", L"DebuggingHeader");
  /* DebuggingInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DebuggingInfo), L"urn:enterprise.soap.sforce.com", L"DebuggingInfo");
  /* AllowFieldTruncationHeader */
  RemClassRegistry()->RegisterXSClass(__classid(AllowFieldTruncationHeader), L"urn:enterprise.soap.sforce.com", L"AllowFieldTruncationHeader");
  /* DisableFeedTrackingHeader */
  RemClassRegistry()->RegisterXSClass(__classid(DisableFeedTrackingHeader), L"urn:enterprise.soap.sforce.com", L"DisableFeedTrackingHeader");
  /* StreamingEnabledHeader */
  RemClassRegistry()->RegisterXSClass(__classid(StreamingEnabledHeader), L"urn:enterprise.soap.sforce.com", L"StreamingEnabledHeader");
  /* AllOrNoneHeader */
  RemClassRegistry()->RegisterXSClass(__classid(AllOrNoneHeader), L"urn:enterprise.soap.sforce.com", L"AllOrNoneHeader");
  /* MruHeader */
  RemClassRegistry()->RegisterXSClass(__classid(MruHeader), L"urn:enterprise.soap.sforce.com", L"MruHeader");
  /* EmailHeader */
  RemClassRegistry()->RegisterXSClass(__classid(EmailHeader), L"urn:enterprise.soap.sforce.com", L"EmailHeader");
  /* AssignmentRuleHeader */
  RemClassRegistry()->RegisterXSClass(__classid(AssignmentRuleHeader), L"urn:enterprise.soap.sforce.com", L"AssignmentRuleHeader");
  /* UserTerritoryDeleteHeader */
  RemClassRegistry()->RegisterXSClass(__classid(UserTerritoryDeleteHeader), L"urn:enterprise.soap.sforce.com", L"UserTerritoryDeleteHeader");
  /* LocaleOptions */
  RemClassRegistry()->RegisterXSClass(__classid(LocaleOptions), L"urn:enterprise.soap.sforce.com", L"LocaleOptions");
  /* ExceptionCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ExceptionCode_TypeInfoHolder)), L"urn:fault.enterprise.soap.sforce.com", L"ExceptionCode");
  /* ApiFault */
  RemClassRegistry()->RegisterXSClass(__classid(ApiFault), L"urn:fault.enterprise.soap.sforce.com", L"ApiFault");
  /* ApiQueryFault */
  RemClassRegistry()->RegisterXSClass(__classid(ApiQueryFault), L"urn:fault.enterprise.soap.sforce.com", L"ApiQueryFault");
  /* LoginFault */
  RemClassRegistry()->RegisterXSClass(__classid(LoginFault), L"urn:fault.enterprise.soap.sforce.com", L"LoginFault");
  /* InvalidQueryLocatorFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidQueryLocatorFault), L"urn:fault.enterprise.soap.sforce.com", L"InvalidQueryLocatorFault");
  /* InvalidNewPasswordFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidNewPasswordFault), L"urn:fault.enterprise.soap.sforce.com", L"InvalidNewPasswordFault");
  /* InvalidIdFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidIdFault), L"urn:fault.enterprise.soap.sforce.com", L"InvalidIdFault");
  /* UnexpectedErrorFault */
  RemClassRegistry()->RegisterXSClass(__classid(UnexpectedErrorFault), L"urn:fault.enterprise.soap.sforce.com", L"UnexpectedErrorFault");
  /* InvalidFieldFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidFieldFault), L"urn:fault.enterprise.soap.sforce.com", L"InvalidFieldFault");
  /* InvalidSObjectFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidSObjectFault), L"urn:fault.enterprise.soap.sforce.com", L"InvalidSObjectFault");
  /* MalformedQueryFault */
  RemClassRegistry()->RegisterXSClass(__classid(MalformedQueryFault), L"urn:fault.enterprise.soap.sforce.com", L"MalformedQueryFault");
  /* MalformedSearchFault */
  RemClassRegistry()->RegisterXSClass(__classid(MalformedSearchFault), L"urn:fault.enterprise.soap.sforce.com", L"MalformedSearchFault");
  /* fault */
  RemClassRegistry()->RegisterXSClass(__classid(fault), L"urn:fault.enterprise.soap.sforce.com", L"fault");
  /* LoginFault */
  RemClassRegistry()->RegisterXSClass(__classid(LoginFault2), L"urn:fault.enterprise.soap.sforce.com", L"LoginFault2", L"LoginFault");
  /* InvalidQueryLocatorFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidQueryLocatorFault2), L"urn:fault.enterprise.soap.sforce.com", L"InvalidQueryLocatorFault2", L"InvalidQueryLocatorFault");
  /* InvalidNewPasswordFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidNewPasswordFault2), L"urn:fault.enterprise.soap.sforce.com", L"InvalidNewPasswordFault2", L"InvalidNewPasswordFault");
  /* InvalidIdFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidIdFault2), L"urn:fault.enterprise.soap.sforce.com", L"InvalidIdFault2", L"InvalidIdFault");
  /* UnexpectedErrorFault */
  RemClassRegistry()->RegisterXSClass(__classid(UnexpectedErrorFault2), L"urn:fault.enterprise.soap.sforce.com", L"UnexpectedErrorFault2", L"UnexpectedErrorFault");
  /* InvalidFieldFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidFieldFault2), L"urn:fault.enterprise.soap.sforce.com", L"InvalidFieldFault2", L"InvalidFieldFault");
  /* InvalidSObjectFault */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidSObjectFault2), L"urn:fault.enterprise.soap.sforce.com", L"InvalidSObjectFault2", L"InvalidSObjectFault");
  /* MalformedQueryFault */
  RemClassRegistry()->RegisterXSClass(__classid(MalformedQueryFault2), L"urn:fault.enterprise.soap.sforce.com", L"MalformedQueryFault2", L"MalformedQueryFault");
  /* MalformedSearchFault */
  RemClassRegistry()->RegisterXSClass(__classid(MalformedSearchFault2), L"urn:fault.enterprise.soap.sforce.com", L"MalformedSearchFault2", L"MalformedSearchFault");
  /* Array_Of_string */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_string), L"http://www.w3.org/2001/XMLSchema", L"Array_Of_string");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_string), (TSerializationOptions() << xoInlineArrays));
  /* SendEmailError */
  RemClassRegistry()->RegisterXSClass(__classid(SendEmailError), L"urn:enterprise.soap.sforce.com", L"SendEmailError");
  /* Error */
  RemClassRegistry()->RegisterXSClass(__classid(Error), L"urn:enterprise.soap.sforce.com", L"Error");
  /* sObject */
  RemClassRegistry()->RegisterXSClass(__classid(sObject), L"urn:sobject.enterprise.soap.sforce.com", L"sObject");
  /* WebLink */
  RemClassRegistry()->RegisterXSClass(__classid(WebLink), L"urn:sobject.enterprise.soap.sforce.com", L"WebLink");
  /* Vote */
  RemClassRegistry()->RegisterXSClass(__classid(Vote), L"urn:sobject.enterprise.soap.sforce.com", L"Vote");
  /* UserRecordAccess */
  RemClassRegistry()->RegisterXSClass(__classid(UserRecordAccess), L"urn:sobject.enterprise.soap.sforce.com", L"UserRecordAccess");
  /* UserProfileFeed */
  RemClassRegistry()->RegisterXSClass(__classid(UserProfileFeed), L"urn:sobject.enterprise.soap.sforce.com", L"UserProfileFeed");
  /* UserPreference */
  RemClassRegistry()->RegisterXSClass(__classid(UserPreference), L"urn:sobject.enterprise.soap.sforce.com", L"UserPreference");
  /* UserFeed */
  RemClassRegistry()->RegisterXSClass(__classid(UserFeed), L"urn:sobject.enterprise.soap.sforce.com", L"UserFeed");
  /* TraceFlag */
  RemClassRegistry()->RegisterXSClass(__classid(TraceFlag), L"urn:sobject.enterprise.soap.sforce.com", L"TraceFlag");
  /* Time__c */
  RemClassRegistry()->RegisterXSClass(__classid(Time__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Time__c");
  /* TaskStatus */
  RemClassRegistry()->RegisterXSClass(__classid(TaskStatus), L"urn:sobject.enterprise.soap.sforce.com", L"TaskStatus");
  /* TaskPriority */
  RemClassRegistry()->RegisterXSClass(__classid(TaskPriority), L"urn:sobject.enterprise.soap.sforce.com", L"TaskPriority");
  /* TaskFeed */
  RemClassRegistry()->RegisterXSClass(__classid(TaskFeed), L"urn:sobject.enterprise.soap.sforce.com", L"TaskFeed");
  /* StaticResource */
  RemClassRegistry()->RegisterXSClass(__classid(StaticResource), L"urn:sobject.enterprise.soap.sforce.com", L"StaticResource");
  /* Staff__c */
  RemClassRegistry()->RegisterXSClass(__classid(Staff__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Staff__c");
  /* Staff_Title__c */
  RemClassRegistry()->RegisterXSClass(__classid(Staff_Title__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Staff_Title__c");
  /* SolutionStatus */
  RemClassRegistry()->RegisterXSClass(__classid(SolutionStatus), L"urn:sobject.enterprise.soap.sforce.com", L"SolutionStatus");
  /* SolutionHistory */
  RemClassRegistry()->RegisterXSClass(__classid(SolutionHistory), L"urn:sobject.enterprise.soap.sforce.com", L"SolutionHistory");
  /* SolutionFeed */
  RemClassRegistry()->RegisterXSClass(__classid(SolutionFeed), L"urn:sobject.enterprise.soap.sforce.com", L"SolutionFeed");
  /* SiteHistory */
  RemClassRegistry()->RegisterXSClass(__classid(SiteHistory), L"urn:sobject.enterprise.soap.sforce.com", L"SiteHistory");
  /* SiteFeed */
  RemClassRegistry()->RegisterXSClass(__classid(SiteFeed), L"urn:sobject.enterprise.soap.sforce.com", L"SiteFeed");
  /* SiteDomain */
  RemClassRegistry()->RegisterXSClass(__classid(SiteDomain), L"urn:sobject.enterprise.soap.sforce.com", L"SiteDomain");
  /* Site */
  RemClassRegistry()->RegisterXSClass(__classid(Site), L"urn:sobject.enterprise.soap.sforce.com", L"Site");
  /* SetupEntityAccess */
  RemClassRegistry()->RegisterXSClass(__classid(SetupEntityAccess), L"urn:sobject.enterprise.soap.sforce.com", L"SetupEntityAccess");
  /* SelfServiceUser */
  RemClassRegistry()->RegisterXSClass(__classid(SelfServiceUser), L"urn:sobject.enterprise.soap.sforce.com", L"SelfServiceUser");
  /* ReportFeed */
  RemClassRegistry()->RegisterXSClass(__classid(ReportFeed), L"urn:sobject.enterprise.soap.sforce.com", L"ReportFeed");
  /* Report */
  RemClassRegistry()->RegisterXSClass(__classid(Report), L"urn:sobject.enterprise.soap.sforce.com", L"Report");
  /* QueueSobject */
  RemClassRegistry()->RegisterXSClass(__classid(QueueSobject), L"urn:sobject.enterprise.soap.sforce.com", L"QueueSobject");
  /* PushTopic */
  RemClassRegistry()->RegisterXSClass(__classid(PushTopic), L"urn:sobject.enterprise.soap.sforce.com", L"PushTopic");
  /* Product2Feed */
  RemClassRegistry()->RegisterXSClass(__classid(Product2Feed), L"urn:sobject.enterprise.soap.sforce.com", L"Product2Feed");
  /* ProcessInstanceWorkitem */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessInstanceWorkitem), L"urn:sobject.enterprise.soap.sforce.com", L"ProcessInstanceWorkitem");
  /* ProcessInstanceStep */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessInstanceStep), L"urn:sobject.enterprise.soap.sforce.com", L"ProcessInstanceStep");
  /* ProcessInstanceHistory */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessInstanceHistory), L"urn:sobject.enterprise.soap.sforce.com", L"ProcessInstanceHistory");
  /* ProcessInstance */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessInstance), L"urn:sobject.enterprise.soap.sforce.com", L"ProcessInstance");
  /* PermissionSetAssignment */
  RemClassRegistry()->RegisterXSClass(__classid(PermissionSetAssignment), L"urn:sobject.enterprise.soap.sforce.com", L"PermissionSetAssignment");
  /* UserLicense */
  RemClassRegistry()->RegisterXSClass(__classid(UserLicense), L"urn:sobject.enterprise.soap.sforce.com", L"UserLicense");
  /* Period */
  RemClassRegistry()->RegisterXSClass(__classid(Period), L"urn:sobject.enterprise.soap.sforce.com", L"Period");
  /* PartnerRole */
  RemClassRegistry()->RegisterXSClass(__classid(PartnerRole), L"urn:sobject.enterprise.soap.sforce.com", L"PartnerRole");
  /* Partner */
  RemClassRegistry()->RegisterXSClass(__classid(Partner), L"urn:sobject.enterprise.soap.sforce.com", L"Partner");
  /* PLU_Stock__c */
  RemClassRegistry()->RegisterXSClass(__classid(PLU_Stock__c), L"urn:sobject.enterprise.soap.sforce.com", L"PLU_Stock__c");
  /* Organization */
  RemClassRegistry()->RegisterXSClass(__classid(Organization), L"urn:sobject.enterprise.soap.sforce.com", L"Organization");
  /* OrgWideEmailAddress */
  RemClassRegistry()->RegisterXSClass(__classid(OrgWideEmailAddress), L"urn:sobject.enterprise.soap.sforce.com", L"OrgWideEmailAddress");
  /* OpportunityStage */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityStage), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityStage");
  /* OpportunityShare */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityShare), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityShare");
  /* OpportunityPartner */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityPartner), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityPartner");
  /* PricebookEntry */
  RemClassRegistry()->RegisterXSClass(__classid(PricebookEntry), L"urn:sobject.enterprise.soap.sforce.com", L"PricebookEntry");
  /* OpportunityLineItem */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityLineItem), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityLineItem");
  /* OpportunityHistory */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityHistory), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityHistory");
  /* OpportunityFieldHistory */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityFieldHistory), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityFieldHistory");
  /* OpportunityFeed */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityFeed), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityFeed");
  /* OpportunityContactRole */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityContactRole), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityContactRole");
  /* OpportunityCompetitor */
  RemClassRegistry()->RegisterXSClass(__classid(OpportunityCompetitor), L"urn:sobject.enterprise.soap.sforce.com", L"OpportunityCompetitor");
  /* Pricebook2 */
  RemClassRegistry()->RegisterXSClass(__classid(Pricebook2), L"urn:sobject.enterprise.soap.sforce.com", L"Pricebook2");
  /* OpenActivity */
  RemClassRegistry()->RegisterXSClass(__classid(OpenActivity), L"urn:sobject.enterprise.soap.sforce.com", L"OpenActivity");
  /* ObjectPermissions */
  RemClassRegistry()->RegisterXSClass(__classid(ObjectPermissions), L"urn:sobject.enterprise.soap.sforce.com", L"ObjectPermissions");
  /* NoteAndAttachment */
  RemClassRegistry()->RegisterXSClass(__classid(NoteAndAttachment), L"urn:sobject.enterprise.soap.sforce.com", L"NoteAndAttachment");
  /* Note */
  RemClassRegistry()->RegisterXSClass(__classid(Note), L"urn:sobject.enterprise.soap.sforce.com", L"Note");
  /* NewsFeed */
  RemClassRegistry()->RegisterXSClass(__classid(NewsFeed), L"urn:sobject.enterprise.soap.sforce.com", L"NewsFeed");
  /* UserRole */
  RemClassRegistry()->RegisterXSClass(__classid(UserRole), L"urn:sobject.enterprise.soap.sforce.com", L"UserRole");
  /* Profile */
  RemClassRegistry()->RegisterXSClass(__classid(Profile), L"urn:sobject.enterprise.soap.sforce.com", L"Profile");
  /* Monthly_Variables__c */
  RemClassRegistry()->RegisterXSClass(__classid(Monthly_Variables__c), L"urn:sobject.enterprise.soap.sforce.com", L"Monthly_Variables__c");
  /* MailmergeTemplate */
  RemClassRegistry()->RegisterXSClass(__classid(MailmergeTemplate), L"urn:sobject.enterprise.soap.sforce.com", L"MailmergeTemplate");
  /* LoginHistory */
  RemClassRegistry()->RegisterXSClass(__classid(LoginHistory), L"urn:sobject.enterprise.soap.sforce.com", L"LoginHistory");
  /* Transaction__c */
  RemClassRegistry()->RegisterXSClass(__classid(Transaction__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Transaction__c");
  /* Product__c */
  RemClassRegistry()->RegisterXSClass(__classid(Product__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Product__c");
  /* Line_Item__c */
  RemClassRegistry()->RegisterXSClass(__classid(Line_Item__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Line_Item__c");
  /* LeadStatus */
  RemClassRegistry()->RegisterXSClass(__classid(LeadStatus), L"urn:sobject.enterprise.soap.sforce.com", L"LeadStatus");
  /* LeadShare */
  RemClassRegistry()->RegisterXSClass(__classid(LeadShare), L"urn:sobject.enterprise.soap.sforce.com", L"LeadShare");
  /* LeadHistory */
  RemClassRegistry()->RegisterXSClass(__classid(LeadHistory), L"urn:sobject.enterprise.soap.sforce.com", L"LeadHistory");
  /* LeadFeed */
  RemClassRegistry()->RegisterXSClass(__classid(LeadFeed), L"urn:sobject.enterprise.soap.sforce.com", L"LeadFeed");
  /* Lead */
  RemClassRegistry()->RegisterXSClass(__classid(Lead), L"urn:sobject.enterprise.soap.sforce.com", L"Lead");
  /* RecordType */
  RemClassRegistry()->RegisterXSClass(__classid(RecordType), L"urn:sobject.enterprise.soap.sforce.com", L"RecordType");
  /* KPI__c */
  RemClassRegistry()->RegisterXSClass(__classid(KPI__c_), L"urn:sobject.enterprise.soap.sforce.com", L"KPI__c");
  /* KPI_Value__c */
  RemClassRegistry()->RegisterXSClass(__classid(KPI_Value__c_), L"urn:sobject.enterprise.soap.sforce.com", L"KPI_Value__c");
  /* IdeaComment */
  RemClassRegistry()->RegisterXSClass(__classid(IdeaComment), L"urn:sobject.enterprise.soap.sforce.com", L"IdeaComment");
  /* Idea */
  RemClassRegistry()->RegisterXSClass(__classid(Idea), L"urn:sobject.enterprise.soap.sforce.com", L"Idea");
  /* IDEWorkspace */
  RemClassRegistry()->RegisterXSClass(__classid(IDEWorkspace), L"urn:sobject.enterprise.soap.sforce.com", L"IDEWorkspace");
  /* IDEPerspective */
  RemClassRegistry()->RegisterXSClass(__classid(IDEPerspective), L"urn:sobject.enterprise.soap.sforce.com", L"IDEPerspective");
  /* Holiday */
  RemClassRegistry()->RegisterXSClass(__classid(Holiday), L"urn:sobject.enterprise.soap.sforce.com", L"Holiday");
  /* GroupMember */
  RemClassRegistry()->RegisterXSClass(__classid(GroupMember), L"urn:sobject.enterprise.soap.sforce.com", L"GroupMember");
  /* Group */
  RemClassRegistry()->RegisterXSClass(__classid(Group), L"urn:sobject.enterprise.soap.sforce.com", L"Group");
  /* ForecastingQuota */
  RemClassRegistry()->RegisterXSClass(__classid(ForecastingQuota), L"urn:sobject.enterprise.soap.sforce.com", L"ForecastingQuota");
  /* ForecastShare */
  RemClassRegistry()->RegisterXSClass(__classid(ForecastShare), L"urn:sobject.enterprise.soap.sforce.com", L"ForecastShare");
  /* FiscalYearSettings */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalYearSettings), L"urn:sobject.enterprise.soap.sforce.com", L"FiscalYearSettings");
  /* PermissionSet */
  RemClassRegistry()->RegisterXSClass(__classid(PermissionSet), L"urn:sobject.enterprise.soap.sforce.com", L"PermissionSet");
  /* FieldPermissions */
  RemClassRegistry()->RegisterXSClass(__classid(FieldPermissions), L"urn:sobject.enterprise.soap.sforce.com", L"FieldPermissions");
  /* FeedTrackedChange */
  RemClassRegistry()->RegisterXSClass(__classid(FeedTrackedChange), L"urn:sobject.enterprise.soap.sforce.com", L"FeedTrackedChange");
  /* FeedLike */
  RemClassRegistry()->RegisterXSClass(__classid(FeedLike), L"urn:sobject.enterprise.soap.sforce.com", L"FeedLike");
  /* FeedItem */
  RemClassRegistry()->RegisterXSClass(__classid(FeedItem), L"urn:sobject.enterprise.soap.sforce.com", L"FeedItem");
  /* FeedComment */
  RemClassRegistry()->RegisterXSClass(__classid(FeedComment), L"urn:sobject.enterprise.soap.sforce.com", L"FeedComment");
  /* EventFeed */
  RemClassRegistry()->RegisterXSClass(__classid(EventFeed), L"urn:sobject.enterprise.soap.sforce.com", L"EventFeed");
  /* EventAttendee */
  RemClassRegistry()->RegisterXSClass(__classid(EventAttendee), L"urn:sobject.enterprise.soap.sforce.com", L"EventAttendee");
  /* Event */
  RemClassRegistry()->RegisterXSClass(__classid(Event), L"urn:sobject.enterprise.soap.sforce.com", L"Event");
  /* EntitySubscription */
  RemClassRegistry()->RegisterXSClass(__classid(EntitySubscription), L"urn:sobject.enterprise.soap.sforce.com", L"EntitySubscription");
  /* EmailTemplate */
  RemClassRegistry()->RegisterXSClass(__classid(EmailTemplate), L"urn:sobject.enterprise.soap.sforce.com", L"EmailTemplate");
  /* Task */
  RemClassRegistry()->RegisterXSClass(__classid(Task), L"urn:sobject.enterprise.soap.sforce.com", L"Task");
  /* EmailStatus */
  RemClassRegistry()->RegisterXSClass(__classid(EmailStatus), L"urn:sobject.enterprise.soap.sforce.com", L"EmailStatus");
  /* EmailServicesFunction */
  RemClassRegistry()->RegisterXSClass(__classid(EmailServicesFunction), L"urn:sobject.enterprise.soap.sforce.com", L"EmailServicesFunction");
  /* EmailServicesAddress */
  RemClassRegistry()->RegisterXSClass(__classid(EmailServicesAddress), L"urn:sobject.enterprise.soap.sforce.com", L"EmailServicesAddress");
  /* DocumentAttachmentMap */
  RemClassRegistry()->RegisterXSClass(__classid(DocumentAttachmentMap), L"urn:sobject.enterprise.soap.sforce.com", L"DocumentAttachmentMap");
  /* Document */
  RemClassRegistry()->RegisterXSClass(__classid(Document), L"urn:sobject.enterprise.soap.sforce.com", L"Document");
  /* Payment_Type__c */
  RemClassRegistry()->RegisterXSClass(__classid(Payment_Type__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Payment_Type__c");
  /* Deposit__c */
  RemClassRegistry()->RegisterXSClass(__classid(Deposit__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Deposit__c");
  /* Deposit_Detail__c */
  RemClassRegistry()->RegisterXSClass(__classid(Deposit_Detail__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Deposit_Detail__c");
  /* Site__c */
  RemClassRegistry()->RegisterXSClass(__classid(Site__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Site__c");
  /* Day__c */
  RemClassRegistry()->RegisterXSClass(__classid(Day__c_), L"urn:sobject.enterprise.soap.sforce.com", L"Day__c");
  /* DashboardFeed */
  RemClassRegistry()->RegisterXSClass(__classid(DashboardFeed), L"urn:sobject.enterprise.soap.sforce.com", L"DashboardFeed");
  /* DashboardComponentFeed */
  RemClassRegistry()->RegisterXSClass(__classid(DashboardComponentFeed), L"urn:sobject.enterprise.soap.sforce.com", L"DashboardComponentFeed");
  /* DashboardComponent */
  RemClassRegistry()->RegisterXSClass(__classid(DashboardComponent), L"urn:sobject.enterprise.soap.sforce.com", L"DashboardComponent");
  /* Folder */
  RemClassRegistry()->RegisterXSClass(__classid(Folder), L"urn:sobject.enterprise.soap.sforce.com", L"Folder");
  /* Dashboard */
  RemClassRegistry()->RegisterXSClass(__classid(Dashboard), L"urn:sobject.enterprise.soap.sforce.com", L"Dashboard");
  /* CronTrigger */
  RemClassRegistry()->RegisterXSClass(__classid(CronTrigger), L"urn:sobject.enterprise.soap.sforce.com", L"CronTrigger");
  /* ContractStatus */
  RemClassRegistry()->RegisterXSClass(__classid(ContractStatus), L"urn:sobject.enterprise.soap.sforce.com", L"ContractStatus");
  /* ContractHistory */
  RemClassRegistry()->RegisterXSClass(__classid(ContractHistory), L"urn:sobject.enterprise.soap.sforce.com", L"ContractHistory");
  /* ContractFeed */
  RemClassRegistry()->RegisterXSClass(__classid(ContractFeed), L"urn:sobject.enterprise.soap.sforce.com", L"ContractFeed");
  /* ContractContactRole */
  RemClassRegistry()->RegisterXSClass(__classid(ContractContactRole), L"urn:sobject.enterprise.soap.sforce.com", L"ContractContactRole");
  /* ContentVersionHistory */
  RemClassRegistry()->RegisterXSClass(__classid(ContentVersionHistory), L"urn:sobject.enterprise.soap.sforce.com", L"ContentVersionHistory");
  /* ContentDocumentLink */
  RemClassRegistry()->RegisterXSClass(__classid(ContentDocumentLink), L"urn:sobject.enterprise.soap.sforce.com", L"ContentDocumentLink");
  /* ContentDocumentHistory */
  RemClassRegistry()->RegisterXSClass(__classid(ContentDocumentHistory), L"urn:sobject.enterprise.soap.sforce.com", L"ContentDocumentHistory");
  /* ContentDocumentFeed */
  RemClassRegistry()->RegisterXSClass(__classid(ContentDocumentFeed), L"urn:sobject.enterprise.soap.sforce.com", L"ContentDocumentFeed");
  /* ContentVersion */
  RemClassRegistry()->RegisterXSClass(__classid(ContentVersion), L"urn:sobject.enterprise.soap.sforce.com", L"ContentVersion");
  /* ContentDocument */
  RemClassRegistry()->RegisterXSClass(__classid(ContentDocument), L"urn:sobject.enterprise.soap.sforce.com", L"ContentDocument");
  /* ContactShare */
  RemClassRegistry()->RegisterXSClass(__classid(ContactShare), L"urn:sobject.enterprise.soap.sforce.com", L"ContactShare");
  /* ContactHistory */
  RemClassRegistry()->RegisterXSClass(__classid(ContactHistory), L"urn:sobject.enterprise.soap.sforce.com", L"ContactHistory");
  /* ContactFeed */
  RemClassRegistry()->RegisterXSClass(__classid(ContactFeed), L"urn:sobject.enterprise.soap.sforce.com", L"ContactFeed");
  /* Community */
  RemClassRegistry()->RegisterXSClass(__classid(Community), L"urn:sobject.enterprise.soap.sforce.com", L"Community");
  /* CollaborationInvitation */
  RemClassRegistry()->RegisterXSClass(__classid(CollaborationInvitation), L"urn:sobject.enterprise.soap.sforce.com", L"CollaborationInvitation");
  /* CollaborationGroupMemberRequest */
  RemClassRegistry()->RegisterXSClass(__classid(CollaborationGroupMemberRequest), L"urn:sobject.enterprise.soap.sforce.com", L"CollaborationGroupMemberRequest");
  /* CollaborationGroupMember */
  RemClassRegistry()->RegisterXSClass(__classid(CollaborationGroupMember), L"urn:sobject.enterprise.soap.sforce.com", L"CollaborationGroupMember");
  /* CollaborationGroupFeed */
  RemClassRegistry()->RegisterXSClass(__classid(CollaborationGroupFeed), L"urn:sobject.enterprise.soap.sforce.com", L"CollaborationGroupFeed");
  /* CollaborationGroup */
  RemClassRegistry()->RegisterXSClass(__classid(CollaborationGroup), L"urn:sobject.enterprise.soap.sforce.com", L"CollaborationGroup");
  /* ChatterActivity */
  RemClassRegistry()->RegisterXSClass(__classid(ChatterActivity), L"urn:sobject.enterprise.soap.sforce.com", L"ChatterActivity");
  /* CategoryNode */
  RemClassRegistry()->RegisterXSClass(__classid(CategoryNode), L"urn:sobject.enterprise.soap.sforce.com", L"CategoryNode");
  /* CategoryData */
  RemClassRegistry()->RegisterXSClass(__classid(CategoryData), L"urn:sobject.enterprise.soap.sforce.com", L"CategoryData");
  /* CaseTeamTemplateRecord */
  RemClassRegistry()->RegisterXSClass(__classid(CaseTeamTemplateRecord), L"urn:sobject.enterprise.soap.sforce.com", L"CaseTeamTemplateRecord");
  /* CaseTeamTemplateMember */
  RemClassRegistry()->RegisterXSClass(__classid(CaseTeamTemplateMember), L"urn:sobject.enterprise.soap.sforce.com", L"CaseTeamTemplateMember");
  /* CaseTeamTemplate */
  RemClassRegistry()->RegisterXSClass(__classid(CaseTeamTemplate), L"urn:sobject.enterprise.soap.sforce.com", L"CaseTeamTemplate");
  /* CaseTeamRole */
  RemClassRegistry()->RegisterXSClass(__classid(CaseTeamRole), L"urn:sobject.enterprise.soap.sforce.com", L"CaseTeamRole");
  /* CaseTeamMember */
  RemClassRegistry()->RegisterXSClass(__classid(CaseTeamMember), L"urn:sobject.enterprise.soap.sforce.com", L"CaseTeamMember");
  /* CaseStatus */
  RemClassRegistry()->RegisterXSClass(__classid(CaseStatus), L"urn:sobject.enterprise.soap.sforce.com", L"CaseStatus");
  /* Solution */
  RemClassRegistry()->RegisterXSClass(__classid(Solution), L"urn:sobject.enterprise.soap.sforce.com", L"Solution");
  /* CaseSolution */
  RemClassRegistry()->RegisterXSClass(__classid(CaseSolution), L"urn:sobject.enterprise.soap.sforce.com", L"CaseSolution");
  /* CaseShare */
  RemClassRegistry()->RegisterXSClass(__classid(CaseShare), L"urn:sobject.enterprise.soap.sforce.com", L"CaseShare");
  /* CaseHistory */
  RemClassRegistry()->RegisterXSClass(__classid(CaseHistory), L"urn:sobject.enterprise.soap.sforce.com", L"CaseHistory");
  /* CaseFeed */
  RemClassRegistry()->RegisterXSClass(__classid(CaseFeed), L"urn:sobject.enterprise.soap.sforce.com", L"CaseFeed");
  /* CaseContactRole */
  RemClassRegistry()->RegisterXSClass(__classid(CaseContactRole), L"urn:sobject.enterprise.soap.sforce.com", L"CaseContactRole");
  /* CaseComment */
  RemClassRegistry()->RegisterXSClass(__classid(CaseComment), L"urn:sobject.enterprise.soap.sforce.com", L"CaseComment");
  /* Case */
  RemClassRegistry()->RegisterXSClass(__classid(Case), L"urn:sobject.enterprise.soap.sforce.com", L"Case");
  /* CallCenter */
  RemClassRegistry()->RegisterXSClass(__classid(CallCenter), L"urn:sobject.enterprise.soap.sforce.com", L"CallCenter");
  /* BusinessProcess */
  RemClassRegistry()->RegisterXSClass(__classid(BusinessProcess), L"urn:sobject.enterprise.soap.sforce.com", L"BusinessProcess");
  /* BusinessHours */
  RemClassRegistry()->RegisterXSClass(__classid(BusinessHours), L"urn:sobject.enterprise.soap.sforce.com", L"BusinessHours");
  /* BrandTemplate */
  RemClassRegistry()->RegisterXSClass(__classid(BrandTemplate), L"urn:sobject.enterprise.soap.sforce.com", L"BrandTemplate");
  /* Attachment */
  RemClassRegistry()->RegisterXSClass(__classid(Attachment), L"urn:sobject.enterprise.soap.sforce.com", L"Attachment");
  /* AssignmentRule */
  RemClassRegistry()->RegisterXSClass(__classid(AssignmentRule), L"urn:sobject.enterprise.soap.sforce.com", L"AssignmentRule");
  /* AssetFeed */
  RemClassRegistry()->RegisterXSClass(__classid(AssetFeed), L"urn:sobject.enterprise.soap.sforce.com", L"AssetFeed");
  /* Product2 */
  RemClassRegistry()->RegisterXSClass(__classid(Product2), L"urn:sobject.enterprise.soap.sforce.com", L"Product2");
  /* Asset */
  RemClassRegistry()->RegisterXSClass(__classid(Asset), L"urn:sobject.enterprise.soap.sforce.com", L"Asset");
  /* Contract */
  RemClassRegistry()->RegisterXSClass(__classid(Contract), L"urn:sobject.enterprise.soap.sforce.com", L"Contract");
  /* Approval */
  RemClassRegistry()->RegisterXSClass(__classid(Approval), L"urn:sobject.enterprise.soap.sforce.com", L"Approval");
  /* ApexTrigger */
  RemClassRegistry()->RegisterXSClass(__classid(ApexTrigger), L"urn:sobject.enterprise.soap.sforce.com", L"ApexTrigger");
  /* AsyncApexJob */
  RemClassRegistry()->RegisterXSClass(__classid(AsyncApexJob), L"urn:sobject.enterprise.soap.sforce.com", L"AsyncApexJob");
  /* ApexTestResult */
  RemClassRegistry()->RegisterXSClass(__classid(ApexTestResult), L"urn:sobject.enterprise.soap.sforce.com", L"ApexTestResult");
  /* ApexTestQueueItem */
  RemClassRegistry()->RegisterXSClass(__classid(ApexTestQueueItem), L"urn:sobject.enterprise.soap.sforce.com", L"ApexTestQueueItem");
  /* ApexPage */
  RemClassRegistry()->RegisterXSClass(__classid(ApexPage), L"urn:sobject.enterprise.soap.sforce.com", L"ApexPage");
  /* ApexLog */
  RemClassRegistry()->RegisterXSClass(__classid(ApexLog), L"urn:sobject.enterprise.soap.sforce.com", L"ApexLog");
  /* ApexExecutionOverlayAction */
  RemClassRegistry()->RegisterXSClass(__classid(ApexExecutionOverlayAction), L"urn:sobject.enterprise.soap.sforce.com", L"ApexExecutionOverlayAction");
  /* ApexComponent */
  RemClassRegistry()->RegisterXSClass(__classid(ApexComponent), L"urn:sobject.enterprise.soap.sforce.com", L"ApexComponent");
  /* ApexClass */
  RemClassRegistry()->RegisterXSClass(__classid(ApexClass), L"urn:sobject.enterprise.soap.sforce.com", L"ApexClass");
  /* AdditionalNumber */
  RemClassRegistry()->RegisterXSClass(__classid(AdditionalNumber), L"urn:sobject.enterprise.soap.sforce.com", L"AdditionalNumber");
  /* ActivityHistory */
  RemClassRegistry()->RegisterXSClass(__classid(ActivityHistory), L"urn:sobject.enterprise.soap.sforce.com", L"ActivityHistory");
  /* AccountShare */
  RemClassRegistry()->RegisterXSClass(__classid(AccountShare), L"urn:sobject.enterprise.soap.sforce.com", L"AccountShare");
  /* Opportunity */
  RemClassRegistry()->RegisterXSClass(__classid(Opportunity), L"urn:sobject.enterprise.soap.sforce.com", L"Opportunity");
  /* AccountPartner */
  RemClassRegistry()->RegisterXSClass(__classid(AccountPartner), L"urn:sobject.enterprise.soap.sforce.com", L"AccountPartner");
  /* AccountHistory */
  RemClassRegistry()->RegisterXSClass(__classid(AccountHistory), L"urn:sobject.enterprise.soap.sforce.com", L"AccountHistory");
  /* Name */
  RemClassRegistry()->RegisterXSClass(__classid(Name_), L"urn:sobject.enterprise.soap.sforce.com", L"Name");
  /* AccountFeed */
  RemClassRegistry()->RegisterXSClass(__classid(AccountFeed), L"urn:sobject.enterprise.soap.sforce.com", L"AccountFeed");
  /* Contact */
  RemClassRegistry()->RegisterXSClass(__classid(Contact), L"urn:sobject.enterprise.soap.sforce.com", L"Contact");
  /* AccountContactRole */
  RemClassRegistry()->RegisterXSClass(__classid(AccountContactRole), L"urn:sobject.enterprise.soap.sforce.com", L"AccountContactRole");
  /* User */
  RemClassRegistry()->RegisterXSClass(__classid(User), L"urn:sobject.enterprise.soap.sforce.com", L"User");
  /* Account */
  RemClassRegistry()->RegisterXSClass(__classid(Account), L"urn:sobject.enterprise.soap.sforce.com", L"Account");
  /* AggregateResult */
  RemClassRegistry()->RegisterXSClass(__classid(AggregateResult), L"urn:sobject.enterprise.soap.sforce.com", L"AggregateResult");
  /* Array_Of_sObject */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_sObject), L"urn:sobject.enterprise.soap.sforce.com", L"Array_Of_sObject");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_sObject), (TSerializationOptions() << xoInlineArrays));
  /* QueryResult */
  RemClassRegistry()->RegisterXSClass(__classid(QueryResult), L"urn:enterprise.soap.sforce.com", L"QueryResult");
  /* Array_Of_SearchRecord */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_SearchRecord), L"urn:enterprise.soap.sforce.com", L"Array_Of_SearchRecord");
  /* SearchResult */
  RemClassRegistry()->RegisterXSClass(__classid(SearchResult), L"urn:enterprise.soap.sforce.com", L"SearchResult");
  /* Array_Of_ID */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_ID), L"urn:enterprise.soap.sforce.com", L"Array_Of_ID");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_ID), (TSerializationOptions() << xoInlineArrays));
  /* MassEmailMessage */
  RemClassRegistry()->RegisterXSClass(__classid(MassEmailMessage), L"urn:enterprise.soap.sforce.com", L"MassEmailMessage");
  /* ProcessRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessRequest), L"urn:enterprise.soap.sforce.com", L"ProcessRequest");
  /* ProcessWorkitemRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessWorkitemRequest), L"urn:enterprise.soap.sforce.com", L"ProcessWorkitemRequest");
  /* ProcessSubmitRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessSubmitRequest), L"urn:enterprise.soap.sforce.com", L"ProcessSubmitRequest");
  /* MergeRequest */
  RemClassRegistry()->RegisterXSClass(__classid(MergeRequest), L"urn:enterprise.soap.sforce.com", L"MergeRequest");
  /* GetUpdatedResult */
  RemClassRegistry()->RegisterXSClass(__classid(GetUpdatedResult), L"urn:enterprise.soap.sforce.com", L"GetUpdatedResult");
  /* Array_Of_DeletedRecord */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DeletedRecord), L"urn:enterprise.soap.sforce.com", L"Array_Of_DeletedRecord");
  /* GetDeletedResult */
  RemClassRegistry()->RegisterXSClass(__classid(GetDeletedResult), L"urn:enterprise.soap.sforce.com", L"GetDeletedResult");
  /* Array_Of_Error */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_Error), L"urn:enterprise.soap.sforce.com", L"Array_Of_Error");
  /* LeadConvertResult */
  RemClassRegistry()->RegisterXSClass(__classid(LeadConvertResult), L"urn:enterprise.soap.sforce.com", L"LeadConvertResult");
  /* EmptyRecycleBinResult */
  RemClassRegistry()->RegisterXSClass(__classid(EmptyRecycleBinResult), L"urn:enterprise.soap.sforce.com", L"EmptyRecycleBinResult");
  /* UndeleteResult */
  RemClassRegistry()->RegisterXSClass(__classid(UndeleteResult), L"urn:enterprise.soap.sforce.com", L"UndeleteResult");
  /* DeleteResult */
  RemClassRegistry()->RegisterXSClass(__classid(DeleteResult), L"urn:enterprise.soap.sforce.com", L"DeleteResult");
  /* ProcessResult */
  RemClassRegistry()->RegisterXSClass(__classid(ProcessResult), L"urn:enterprise.soap.sforce.com", L"ProcessResult");
  /* MergeResult */
  RemClassRegistry()->RegisterXSClass(__classid(MergeResult), L"urn:enterprise.soap.sforce.com", L"MergeResult");
  /* UpsertResult */
  RemClassRegistry()->RegisterXSClass(__classid(UpsertResult), L"urn:enterprise.soap.sforce.com", L"UpsertResult");
  /* SaveResult */
  RemClassRegistry()->RegisterXSClass(__classid(SaveResult), L"urn:enterprise.soap.sforce.com", L"SaveResult");
  /* InvalidateSessionsResult */
  RemClassRegistry()->RegisterXSClass(__classid(InvalidateSessionsResult), L"urn:enterprise.soap.sforce.com", L"InvalidateSessionsResult");
  /* Array_Of_ChildRelationship */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_ChildRelationship), L"urn:enterprise.soap.sforce.com", L"Array_Of_ChildRelationship");
  /* Array_Of_Field */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_Field), L"urn:enterprise.soap.sforce.com", L"Array_Of_Field");
  /* Array_Of_RecordTypeInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_RecordTypeInfo), L"urn:enterprise.soap.sforce.com", L"Array_Of_RecordTypeInfo");
  /* DescribeSObjectResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSObjectResult), L"urn:enterprise.soap.sforce.com", L"DescribeSObjectResult");
  /* Array_Of_PicklistEntry */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_PicklistEntry), L"urn:enterprise.soap.sforce.com", L"Array_Of_PicklistEntry");
  /* PicklistForRecordType */
  RemClassRegistry()->RegisterXSClass(__classid(PicklistForRecordType), L"urn:enterprise.soap.sforce.com", L"PicklistForRecordType");
  /* Field */
  RemClassRegistry()->RegisterXSClass(__classid(Field), L"urn:enterprise.soap.sforce.com", L"Field");
  /* Array_Of_DescribeGlobalSObjectResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeGlobalSObjectResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeGlobalSObjectResult");
  /* DescribeGlobalResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeGlobalResult), L"urn:enterprise.soap.sforce.com", L"DescribeGlobalResult");
  /* Array_Of_DataCategory */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DataCategory), L"urn:enterprise.soap.sforce.com", L"Array_Of_DataCategory");
  /* DataCategory */
  RemClassRegistry()->RegisterXSClass(__classid(DataCategory), L"urn:enterprise.soap.sforce.com", L"DataCategory");
  /* DescribeDataCategoryGroupStructureResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeDataCategoryGroupStructureResult), L"urn:enterprise.soap.sforce.com", L"DescribeDataCategoryGroupStructureResult");
  /* Array_Of_DescribeSoftphoneLayoutCallType */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeSoftphoneLayoutCallType), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeSoftphoneLayoutCallType");
  /* DescribeSoftphoneLayoutResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSoftphoneLayoutResult), L"urn:enterprise.soap.sforce.com", L"DescribeSoftphoneLayoutResult");
  /* Array_Of_DescribeSoftphoneLayoutInfoField */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeSoftphoneLayoutInfoField), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeSoftphoneLayoutInfoField");
  /* Array_Of_DescribeSoftphoneScreenPopOption */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeSoftphoneScreenPopOption), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeSoftphoneScreenPopOption");
  /* Array_Of_DescribeSoftphoneLayoutSection */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeSoftphoneLayoutSection), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeSoftphoneLayoutSection");
  /* DescribeSoftphoneLayoutCallType */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSoftphoneLayoutCallType), L"urn:enterprise.soap.sforce.com", L"DescribeSoftphoneLayoutCallType");
  /* Array_Of_DescribeSoftphoneLayoutItem */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeSoftphoneLayoutItem), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeSoftphoneLayoutItem");
  /* DescribeSoftphoneLayoutSection */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeSoftphoneLayoutSection), L"urn:enterprise.soap.sforce.com", L"DescribeSoftphoneLayoutSection");
  /* Array_Of_DescribeLayout */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeLayout), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeLayout");
  /* Array_Of_RecordTypeMapping */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_RecordTypeMapping), L"urn:enterprise.soap.sforce.com", L"Array_Of_RecordTypeMapping");
  /* DescribeLayoutResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutResult), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutResult");
  /* Array_Of_DescribeLayoutSection */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeLayoutSection), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeLayoutSection");
  /* Array_Of_RelatedList */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_RelatedList), L"urn:enterprise.soap.sforce.com", L"Array_Of_RelatedList");
  /* DescribeLayout */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayout), L"urn:enterprise.soap.sforce.com", L"DescribeLayout");
  /* Array_Of_PicklistForRecordType */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_PicklistForRecordType), L"urn:enterprise.soap.sforce.com", L"Array_Of_PicklistForRecordType");
  /* RecordTypeMapping */
  RemClassRegistry()->RegisterXSClass(__classid(RecordTypeMapping), L"urn:enterprise.soap.sforce.com", L"RecordTypeMapping");
  /* Array_Of_DescribeLayoutButton */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeLayoutButton), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeLayoutButton");
  /* DescribeLayoutButtonSection */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutButtonSection), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutButtonSection");
  /* Array_Of_DescribeLayoutRow */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeLayoutRow), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeLayoutRow");
  /* DescribeLayoutSection */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutSection), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutSection");
  /* Array_Of_RelatedListColumn */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_RelatedListColumn), L"urn:enterprise.soap.sforce.com", L"Array_Of_RelatedListColumn");
  /* Array_Of_RelatedListSort */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_RelatedListSort), L"urn:enterprise.soap.sforce.com", L"Array_Of_RelatedListSort");
  /* RelatedList */
  RemClassRegistry()->RegisterXSClass(__classid(RelatedList), L"urn:enterprise.soap.sforce.com", L"RelatedList");
  /* Array_Of_DescribeLayoutItem */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeLayoutItem), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeLayoutItem");
  /* DescribeLayoutRow */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutRow), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutRow");
  /* Array_Of_DescribeLayoutComponent */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeLayoutComponent), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeLayoutComponent");
  /* DescribeLayoutItem */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeLayoutItem), L"urn:enterprise.soap.sforce.com", L"DescribeLayoutItem");
  /* Array_Of_EmailFileAttachment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_EmailFileAttachment), L"urn:enterprise.soap.sforce.com", L"Array_Of_EmailFileAttachment");
  /* SingleEmailMessage */
  RemClassRegistry()->RegisterXSClass(__classid(SingleEmailMessage), L"urn:enterprise.soap.sforce.com", L"SingleEmailMessage");
  /* Array_Of_SendEmailError */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_SendEmailError), L"urn:enterprise.soap.sforce.com", L"Array_Of_SendEmailError");
  /* SendEmailResult */
  RemClassRegistry()->RegisterXSClass(__classid(SendEmailResult), L"urn:enterprise.soap.sforce.com", L"SendEmailResult");
  /* Array_Of_DescribeTab */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeTab), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeTab");
  /* DescribeTabSetResult */
  RemClassRegistry()->RegisterXSClass(__classid(DescribeTabSetResult), L"urn:enterprise.soap.sforce.com", L"DescribeTabSetResult");
  /* Array_Of_DescribeSObjectResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeSObjectResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeSObjectResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_DescribeSObjectResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_DescribeDataCategoryGroupResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeDataCategoryGroupResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeDataCategoryGroupResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_DescribeDataCategoryGroupResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_DataCategoryGroupSobjectTypePair */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DataCategoryGroupSobjectTypePair), L"urn:enterprise.soap.sforce.com", L"Array_Of_DataCategoryGroupSobjectTypePair");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_DataCategoryGroupSobjectTypePair), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_DescribeDataCategoryGroupStructureResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeDataCategoryGroupStructureResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeDataCategoryGroupStructureResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_DescribeDataCategoryGroupStructureResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_DescribeTabSetResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DescribeTabSetResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_DescribeTabSetResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_DescribeTabSetResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_SaveResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_SaveResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_SaveResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_SaveResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_Email */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_Email), L"urn:enterprise.soap.sforce.com", L"Array_Of_Email");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_Email), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_SendEmailResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_SendEmailResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_SendEmailResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_SendEmailResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_UpsertResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_UpsertResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_UpsertResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_UpsertResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_MergeRequest */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_MergeRequest), L"urn:enterprise.soap.sforce.com", L"Array_Of_MergeRequest");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_MergeRequest), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_MergeResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_MergeResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_MergeResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_MergeResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_DeleteResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DeleteResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_DeleteResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_DeleteResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_UndeleteResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_UndeleteResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_UndeleteResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_UndeleteResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_EmptyRecycleBinResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_EmptyRecycleBinResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_EmptyRecycleBinResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_EmptyRecycleBinResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_ProcessRequest */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_ProcessRequest), L"urn:enterprise.soap.sforce.com", L"Array_Of_ProcessRequest");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_ProcessRequest), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_ProcessResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_ProcessResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_ProcessResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_ProcessResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_LeadConvert */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_LeadConvert), L"urn:enterprise.soap.sforce.com", L"Array_Of_LeadConvert");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_LeadConvert), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_LeadConvertResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_LeadConvertResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_LeadConvertResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_LeadConvertResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_InvalidateSessionsResult */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_InvalidateSessionsResult), L"urn:enterprise.soap.sforce.com", L"Array_Of_InvalidateSessionsResult");
  RemClassRegistry()->RegisterSerializeOptions(__delphirtti(Array_Of_InvalidateSessionsResult), (TSerializationOptions() << xoInlineArrays));
  /* Array_Of_PackageVersion */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_PackageVersion), L"urn:enterprise.soap.sforce.com", L"Array_Of_PackageVersion");
  /* PackageVersionHeader */
  RemClassRegistry()->RegisterXSClass(__classid(PackageVersionHeader), L"urn:enterprise.soap.sforce.com", L"PackageVersionHeader");
  /* EmailFileAttachment */
  RemClassRegistry()->RegisterXSClass(__classid(EmailFileAttachment), L"urn:enterprise.soap.sforce.com", L"EmailFileAttachment");
  RemClassRegistry()->RegisterExternalPropName(__typeinfo(EmailFileAttachment), L"inline_", L"inline");
}

#pragma startup RegTypes 32
}; // SfIntegration

