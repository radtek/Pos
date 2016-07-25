//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerLoyaltyMate.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ManagerLoyaltyMate Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TManagerLoyaltyMate* TManagerLoyaltyMate::instance = NULL;

//---------------------------------------------------------------------------
TManagerLoyaltyMate::TManagerLoyaltyMate()
    :loyaltyThreadTerminated(true),
     loyaltyThread(NULL)
{
    loyaltyThreadTimer = new TTimer(NULL);
    loyaltyThreadTimer->Enabled = false;
    loyaltyThreadTimer->Interval = 1000; //TODO: Change to 5000
    loyaltyThreadTimer->OnTimer = OnLoyaltyThreadTimerTick;

    startLoyaltyThreadTimer();
}
//---------------------------------------------------------------------------
TManagerLoyaltyMate::~TManagerLoyaltyMate()
{
    delete loyaltyThread;
    delete loyaltyThreadTimer;
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::SyncMemberDetailsWithCloud(TSyndCode syndicateCode, TMMContactInfo info)
{
    TLoyaltyMateMemberOperation operation(
                                    syndicateCode,
                                    info);
    uploadQueue.push_back(operation);

    initiateLoyaltyThread();
    startLoyaltyThread();
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::CreateMemberOnCloud(
                                    TSyndCode syndicateCode,
                                    TMMContactInfo &info)
{
    bool result = false;

    try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->CreateMember(syndicateCode,info, info.CloudUUID);

        if(createResponse.IsSuccesful)
        {
            info.CloudUUID = createResponse.UUID;

            result = true;
        }
        else
        {
            throw Exception(createResponse.Message);
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}

//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::GetMemberDetailsFromUUID(
                                    TSyndCode syndicateCode,
                                    AnsiString uuid,
                                    TMMContactInfo &outInfo,
                                    bool replacePoints)
{
    bool result = false;

    try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetails(syndicateCode,uuid,outInfo,replacePoints);

        if(!response.IsSuccesful)
            throw Exception(response.Message);

        result = response.IsSuccesful;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}

//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::GetMemberDetailsFromCode(
                                    TSyndCode syndicateCode,
                                    AnsiString memberCode,
                                    TMMContactInfo &outInfo,
                                    bool replacePoints)
{
    bool result = false;

    try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetailsByCode(syndicateCode,memberCode,outInfo,replacePoints);

        if(!response.IsSuccesful)
            throw Exception(response.Message);

        result = response.IsSuccesful;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}

//---------------------------------------------------------------------------
void __fastcall TManagerLoyaltyMate::loyaltyTheadTerminate( TObject* sender )
{
    loyaltyThreadTerminated = true;
    startLoyaltyThreadTimer();
}
//---------------------------------------------------------------------------
void __fastcall TManagerLoyaltyMate::OnLoyaltyThreadTimerTick(TObject *Sender)
{
    if(loyaltyThreadTerminated
        && (uploadQueue.size() > 0 || TLoyaltyMateUtilities::GetAllContactsWithPendingTransactions().size() > 0))
    {
        initiateLoyaltyThread();
        startLoyaltyThread();
    }
    else
        stopLoyaltyThreadTimer();
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::initiateLoyaltyThread()
{
    // initiate the thread if it is invalid
    if( loyaltyThreadTerminated )
    {
        loyaltyThread = new TLoyaltyMateThread();
        loyaltyThread->OnTerminate = loyaltyTheadTerminate;
    }
}
//--------------------------------------------------------------------------
void TManagerLoyaltyMate::startLoyaltyThread()
{
    if( loyaltyThreadTerminated )
    {
        loyaltyThreadTerminated = false;

        loyaltyThread->SetUploadQueue(uploadQueue);
        uploadQueue.clear();
        stopLoyaltyThreadTimer();
        loyaltyThread->Start();
    }
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::startLoyaltyThreadTimer()
{
    loyaltyThreadTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::stopLoyaltyThreadTimer()
{
    loyaltyThreadTimer->Enabled = false;
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::CreateTierOnCloud(TSyndCode syndicateCode,TTierLevel* tierLevel)
{
  bool result = false;
  try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->CreateTier(syndicateCode,tierLevel);

        if(!createResponse.IsSuccesful
            && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            result = true;
        }
        else
        {
            throw Exception("Failed to add tier level.");
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::UpdateTierOnCloud(TSyndCode syndicateCode,TTierLevel* tierLevel)
{
  bool result = false;
  try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->UpdateTier(syndicateCode,tierLevel);

        if(!createResponse.IsSuccesful
            && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            result = true;
        }
        else
        {
            throw Exception("Failed to update tier level.");
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::DeleteTierOnCloud(TSyndCode syndicateCode,TTierLevel* tierLevel)
{
  bool result = false;
  try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->DeleteTier(syndicateCode,tierLevel);

        if(!createResponse.IsSuccesful
            && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            result = true;
        }
        else
        {
            throw Exception("Failed to delete tier level.");
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::SyncTierLevelWithCloud(TSyndCode syndicateCode)
{
  bool result = false;
  try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->SyncTierLevels(syndicateCode);

        if(!createResponse.IsSuccesful
            && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            result = true;
        }
        else
        {
            throw Exception("Failed to sync tier levels.");
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::UpdateMemberCardCodeOnCloud(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode,AnsiString &ErrorMessage)
{
  bool result = false;
  try
    {
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->UpdateMemberCardCode(syndicateCode,uniqueId,memberCardCode);

        if(!createResponse.IsSuccesful
            && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            result = true;
        }
        else
        {
            if(createResponse.Description == "Card code already in use.")
              ErrorMessage = "Card Code Already In Use.";
            else
              ErrorMessage = "Failed to update member information.";
            throw Exception(ErrorMessage);
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    return result;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// LoyaltyMateThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateThread::TLoyaltyMateThread()
    :TThread(true),
     processingMembers(false)
{
    FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::SetUploadQueue(std::vector<TLoyaltyMateMemberOperation> queue)
{
    uploadMap.assign(queue.begin(),queue.end());
    processingMembers = uploadMap.size() > 0;
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateThread::Execute()
{
    uploadPendingTransactions();

    if(processingMembers)
       updateMembers();
}
//---------------------------------------------------------------------------
bool TLoyaltyMateThread::updateMemberOnCloud(TSyndCode syndicateCode, const TMMContactInfo* info)
{
    bool result = false;
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
    MMLoyaltyServiceResponse updateResponse =LoyaltyMateInterface->UpdateMember(syndicateCode,info->CloudUUID,info);

    if(!updateResponse.IsSuccesful
        && updateResponse.ResponseCode == AuthenticationFailed)
    {
        throw Exception("Authentication failed with Loyaltymate Service");
    }
    else if(!updateResponse.IsSuccesful)
    {
        throw Exception(updateResponse.Message);
    }
    else
    {
        result = true;
    }

    return result;
}
//---------------------------------------------------------------------------
bool TLoyaltyMateThread::postMemberTransactionsToCloud(TLoyaltyMateTransaction transaction)
{
    bool result = false;
    try
    {
        if(transaction.CloudUUID.Length() > 0)
        {
           TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
           MMLoyaltyServiceResponse postTransactionResponse = LoyaltyMateInterface->PostTransaction(
                                                                                transaction.SyndicateCode,
                                                                                transaction.CloudUUID,
                                                                                transaction.PointsBalance,
                                                                                transaction.PointsDelta,
                                                                                transaction.OccurredDate,
                                                                                transaction.PointsType);
            if(!postTransactionResponse.IsSuccesful)
                throw new Exception(postTransactionResponse.Message);
            else
                result = true;
        }
    }
    catch(Exception &exc)
    {
        TManagerLogs::Instance().Add(
                __FUNC__,
                ERRORLOG,
                exc.Message);
    }
    return result;
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::logErrorToDB(UnicodeString Function, UnicodeString Type, UnicodeString Msg, UnicodeString Terminal)
{
    TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TManagerLogs::Instance().Add(
                Function,
                Type,
                Msg);

    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::updateMembers()
{
    std::vector<TLoyaltyMateMemberOperation>::iterator it = uploadMap.begin();

    while(it != uploadMap.end())
    {
        TMMContactInfo info = it->MemberInfo;
        TSyndCode syndicateCode = it->SyndicateCode;

        try
        {
            TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();

            // get member uuid if registered in the database
            AnsiString dbUUID = TLoyaltyMateUtilities::GetMemberCloudIdIfRegistered(DBTransaction, info.ContactKey, info.SiteID);

            if(info.CloudUUID.Length() > 0)
            {
                if(dbUUID.Length() == 0)
                {
                    // uuid is not in the database. Insert the record
                    TLoyaltyMateUtilities::SetMemberCloudId(DBTransaction, info.ContactKey, info.SiteID, info.CloudUUID, info.ActivationToken);
                }
                else
                {
                    // this member is already on both places. try issuing an update call to the cloud
                    updateMemberOnCloud(syndicateCode, &info);
                }
            }

            if(info.CloudUUID.Length() == 0)
                throw new Exception("Cannot upload member on the cloud. UUID is empty");

            DBTransaction.Commit();
        }
        catch(Exception &err)
        {
            UnicodeString message = "Cloud sync failed for member- key:" + info.ContactKey;
            message += " name: " + info.Name;
            message += ". Error: " + err.Message;

            logErrorToDB(
                __FUNC__,
                ERRORLOG,
                message);
        }

        it++;
    }

    processingMembers = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::uploadPendingTransactions()
{
    std::vector<int> allMembersWithPendingTransactions = TLoyaltyMateUtilities::GetAllContactsWithPendingTransactions();
    std::vector<int>::iterator it = allMembersWithPendingTransactions.begin();

    while(!processingMembers
            && it != allMembersWithPendingTransactions.end())
    {
        std::vector<TLoyaltyMateTransaction> memberTransactions = TLoyaltyMateUtilities::GetPendingTransactionForContact(*it);
        for(std::vector<TLoyaltyMateTransaction>::iterator itTrans = memberTransactions.begin();
            itTrans != memberTransactions.end();
            itTrans++)
        {
            if(postMemberTransactionsToCloud(*itTrans))
            {
                TLoyaltyMateUtilities::DeleteTransaction(
                                                        *it,
                                                        itTrans->TransactionNumber);
            }
            else
                break;
        }

        advance(it,1);
    }
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateDownloadNewMemberThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateDownloadMemberThread::TLoyaltyMateDownloadMemberThread(TSyndCode inSyndicateCode, bool inReplacePoints)
	:TThread(true),
	 syndicateCode(inSyndicateCode),
	 replacePoints(inReplacePoints),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::threadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Member download thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::downloadMemberFromCloudUsingEmail()
{
   try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			threadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetailsByEmail(syndicateCode, MemberEmail, contactInfo, replacePoints);
        OperationSuccessful = response.IsSuccesful;
        if(!response.IsSuccesful)
        {
                throw Exception(response.Message);
        }
		if(OperationSuccessful)
		{
                   ReturnContactInfo = contactInfo;
        }
		else
		{
			throw new Exception("Failed to get member with UUID.");
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
	}
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::downloadMemberFromCloudUsingUUID()
{
	try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			threadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetails(syndicateCode, UUID, contactInfo, replacePoints);
        OperationSuccessful = response.IsSuccesful;
        if(!response.IsSuccesful)
        {
                throw Exception(response.Message);
        }
		if(OperationSuccessful)
		{
                   ReturnContactInfo = contactInfo;
        }
		else
		{
			throw new Exception("Failed to get member with UUID.");
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
	}
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::downloadMemberFromCloudUsingCode()
{
	try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			threadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetailsByCode(syndicateCode, MemberCode, contactInfo, replacePoints);
        OperationSuccessful = response.IsSuccesful;
        if(!response.IsSuccesful)
            {
                if(response.Description == "Member Not Exist")
                 {
                   ErrorMessage  = "Member Not Exist.";
                   throw Exception(ErrorMessage);
                 }
                throw Exception(response.Message);
            }
        if(OperationSuccessful)
        {
           ReturnContactInfo = contactInfo;
        }
		else
		{
			throw new Exception("Failed to get member with Card Code.");
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateDownloadMemberThread::Execute()
{
    if(DownLoadFromUUID)
    {
      downloadMemberFromCloudUsingUUID();
    }
    else if(DownLoadFromCode)
    {
      downloadMemberFromCloudUsingCode();
    }
    else if(DownLoadFromEmail)
    {
      downloadMemberFromCloudUsingEmail();
    }
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateCreateMemberThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateCreateMemberThread::TLoyaltyMateCreateMemberThread(TSyndCode inSyndicateCode, TMMContactInfo* inContactInfo)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 _memberInfo(inContactInfo),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateCreateMemberThread::threadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Member creation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateCreateMemberThread::createMemberOnCloud()
{
	try
	{
		if(Terminated)
		{
			threadTerminated();
			return;
        }

		OperationSuccessful = TManagerLoyaltyMate::Instance()->CreateMemberOnCloud(_syndicateCode, *_memberInfo);

		if(!OperationSuccessful)
		{
            ErrorMessage = "Failed to create member on the cloud";
            TGlobalSettings::Instance().IsPOSOffline=true;
            throw new Exception(ErrorMessage);
        }
        else
            TGlobalSettings::Instance().IsPOSOffline=false;
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateCreateMemberThread::Execute()
{
    createMemberOnCloud();
    ReturnValue = 1;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateCreateTierThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateTierThread::TLoyaltyMateTierThread(TSyndCode inSyndicateCode, TTierLevel* tierLevel)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 _tierLevel(tierLevel),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateTierThread::threadTerminated()
{
    OperationSuccessful = false;
    ErrorMessage = "Tier operation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateTierThread::createTierOnCloud()
{
    try
    {
            if(Terminated)
            {
                    threadTerminated();
                    return;
            }

            OperationSuccessful = TManagerLoyaltyMate::Instance()->CreateTierOnCloud(_syndicateCode, _tierLevel);

            if(!OperationSuccessful)
            {
                ErrorMessage = "Failed to create tier on the cloud";
                throw new Exception(ErrorMessage);
            }
   }
    catch(Exception &E)
    {
                TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
                ErrorMessage = E.Message;
    }
}
//---------------------------------------------------------------------------
void TLoyaltyMateTierThread::updateTierOnCloud()
{
    try
    {
            if(Terminated)
            {
                    threadTerminated();
                    return;
            }

            OperationSuccessful = TManagerLoyaltyMate::Instance()->UpdateTierOnCloud(_syndicateCode, _tierLevel);

            if(!OperationSuccessful)
            {
                ErrorMessage = "Failed to create tier on the cloud";
                throw new Exception(ErrorMessage);
            }
   }
    catch(Exception &E)
    {
                TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
                ErrorMessage = E.Message;
        }
}
//---------------------------------------------------------------------------
void TLoyaltyMateTierThread::deleteTierOnCloud()
{
    try
    {
            if(Terminated)
            {
                    threadTerminated();
                    return;
            }

            OperationSuccessful = TManagerLoyaltyMate::Instance()->DeleteTierOnCloud(_syndicateCode, _tierLevel);

            if(!OperationSuccessful)
            {
                ErrorMessage = "Failed to create tier on the cloud";
                throw new Exception(ErrorMessage);
            }
   }
    catch(Exception &E)
    {
                TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
                ErrorMessage = E.Message;
        }
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateTierThread::Execute()
{
    if(IsDelete)
      deleteTierOnCloud();
    else
     {
        if(IsUpdate)
          updateTierOnCloud();
        else
         createTierOnCloud();
     }
    ReturnValue = 1;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateTierSyncThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateTierSyncThread::TLoyaltyMateTierSyncThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = false;
}

//---------------------------------------------------------------------------
void TLoyaltyMateTierSyncThread::threadTerminated()
{
    OperationSuccessful = false;
    ErrorMessage = "Tier operation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateTierSyncThread::Execute()
{
    SyncTierLevels();
    ReturnValue = 1;
}
//---------------------------------------------------------------------------
void TLoyaltyMateTierSyncThread::SyncTierLevels()
{
 try
    {
        if(Terminated)
        {
                threadTerminated();
                return;
        }

        OperationSuccessful = TManagerLoyaltyMate::Instance()->SyncTierLevelWithCloud(_syndicateCode);

        if(!OperationSuccessful)
        {
            ErrorMessage = "Failed to sync tier levels from cloud";
            throw new Exception(ErrorMessage);
        }
   }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        ErrorMessage = E.Message;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateUpdateCardThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
TLoyaltyMateUpdateCardThread::TLoyaltyMateUpdateCardThread(TSyndCode inSyndicateCode, AnsiString uniqueId,AnsiString memberCardCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
     _uniqueId(uniqueId),
     _memberCardCode(memberCardCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateUpdateCardThread::threadTerminated()
{
    OperationSuccessful = false;
    ErrorMessage = "Member operation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateUpdateCardThread::Execute()
{
    UpdateMemberCardCodeOnCloud();
    ReturnValue = 1;
}
//---------------------------------------------------------------------------
void TLoyaltyMateUpdateCardThread::UpdateMemberCardCodeOnCloud()
{
 try
    {
        if(Terminated)
        {
                threadTerminated();
                return;
        }

        OperationSuccessful = TManagerLoyaltyMate::Instance()->UpdateMemberCardCodeOnCloud(_syndicateCode,_uniqueId,_memberCardCode,ErrorMessage);

        if(!OperationSuccessful)
        {
            throw new Exception(ErrorMessage);
        }
   }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        ErrorMessage = E.Message;
    }
}
