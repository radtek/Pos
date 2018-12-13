//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerLoyaltyMate.h"
#include "SendEmail.h"

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

    StartLoyaltyThreadTimer();
}
//---------------------------------------------------------------------------
TManagerLoyaltyMate::~TManagerLoyaltyMate()
{
    delete loyaltyThread;
    delete loyaltyThreadTimer;
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::TriggerPointSync()
{
    InitiateLoyaltyThread();
    StartLoyaltyThread();
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::SyncMemberDetailsWithCloud(TSyndCode syndicateCode, TMMContactInfo info)
{
    TLoyaltyMateMemberOperation operation(syndicateCode,info);
    uploadQueue.push_back(operation);
    InitiateLoyaltyThread();
    StartLoyaltyThread();
}
//---------------------------------------------------------------------------
TaskResult TManagerLoyaltyMate::CreateMemberOnCloud(TSyndCode syndicateCode,TMMContactInfo &info)
{
    TaskResult result;
    result.OperationSuccussful = false;
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
    try
    {
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->CreateMember(syndicateCode,info, info.CloudUUID);
        if(createResponse.IsSuccesful)
        {
            info.CloudUUID = createResponse.UUID;
            info.MemberCode = createResponse.MemberCode;
            result.OperationSuccussful = true;
        }
        else
        {
            result.ErrorMessage = createResponse.Message;
        }
    }
    catch(Exception &E)
    {
        result.ErrorMessage = E.Message ;
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
    delete LoyaltyMateInterface;
    return result;
}
//---------------------------------------------------------------------------
void __fastcall TManagerLoyaltyMate::LoyaltyTheadTerminate( TObject* sender )
{
    loyaltyThreadTerminated = true;
    StartLoyaltyThreadTimer();
}
//---------------------------------------------------------------------------
void __fastcall TManagerLoyaltyMate::OnLoyaltyThreadTimerTick(TObject *Sender)
{
    if(loyaltyThreadTerminated
        && (uploadQueue.size() > 0 || TLoyaltyMateUtilities::GetAllContactsWithPendingTransactions().size() > 0))
    {
        InitiateLoyaltyThread();
        StartLoyaltyThread();
    }
    else
        StopLoyaltyThreadTimer();
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::InitiateLoyaltyThread()
{
    // initiate the thread if it is invalid
    if( loyaltyThreadTerminated )
    {
        loyaltyThread = new TLoyaltyMateThread();
        loyaltyThread->OnTerminate = LoyaltyTheadTerminate;
    }
}
//--------------------------------------------------------------------------
void TManagerLoyaltyMate::StartLoyaltyThread()
{
    if( loyaltyThreadTerminated )
    {
        loyaltyThreadTerminated = false;
        loyaltyThread->SetUploadQueue(uploadQueue);
        uploadQueue.clear();
        StopLoyaltyThreadTimer();
        loyaltyThread->Start();
    }
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::StartLoyaltyThreadTimer()
{
    loyaltyThreadTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void TManagerLoyaltyMate::StopLoyaltyThreadTimer()
{
    loyaltyThreadTimer->Enabled = false;
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
    UploadPendingTransactions();

    if(processingMembers)
       UpdateMembers();
}
//---------------------------------------------------------------------------
bool TLoyaltyMateThread::UpdateMemberOnCloud(TSyndCode syndicateCode, const TMMContactInfo* info)
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
    delete LoyaltyMateInterface;
    return result;
}
//---------------------------------------------------------------------------
bool TLoyaltyMateThread::PostMemberTransactionsToCloud(TLoyaltyMateTransaction transaction)
{
    bool result = false;
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
    try
    {
        if(transaction.CloudUUID.Length() > 0)
        {
           MMLoyaltyServiceResponse postTransactionResponse = LoyaltyMateInterface->PostTransaction(
                                                                                transaction.SyndicateCode,
                                                                                transaction.CloudUUID,
                                                                                transaction.PointsBalance,
                                                                                transaction.PointsDelta,
                                                                                transaction.OccurredDate,
                                                                                transaction.PointsType,
                                                                                transaction.InvoiceNumber);
            if(!postTransactionResponse.IsSuccesful)
            {
                Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                DBTransaction.StartTransaction();

                if(postTransactionResponse.ResponseCode == 19)
                {
                    TMMContactInfo contactInfo;
                    UnicodeString email = TDBContacts::GetEmailIdOfMember(DBTransaction,transaction.ContactKey);
                    MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetailsByEmail( transaction.SyndicateCode,
                                                                                email, contactInfo, false);

                    if(response.IsSuccesful && response.ResponseCode != MemberNotExist)
                    {
                        TLoyaltyMateUtilities::UpdateUUID(DBTransaction, transaction.ContactKey, contactInfo.CloudUUID);
                    }
                    else
                    {
                        TLoyaltyMateUtilities::UpdatePendingTransactions(DBTransaction, transaction.ContactKey, "F");
                    }
                }
                else if(postTransactionResponse.ResponseCode == 18)
                {
                    TLoyaltyMateUtilities::UpdatePendingTransactions(DBTransaction, transaction.ContactKey, "F");
                    SendEmail(DBTransaction, transaction);
                }
                else
                {
                    TLoyaltyMateUtilities::UpdatePendingTransactions(DBTransaction, transaction.ContactKey, "F");
                    DBTransaction.Commit();
                    throw new Exception(postTransactionResponse.Message);
                }
                DBTransaction.Commit();
            }
            else
                result = true;
        }
    }
    catch(Exception &exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,exc.Message);
    }
    delete LoyaltyMateInterface;
    return result;
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::LogErrorToDB(UnicodeString Function, UnicodeString Type, UnicodeString Msg, UnicodeString Terminal)
{
    TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerLogs::Instance().Add(Function,Type,Msg);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::UpdateMembers()
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
                    UpdateMemberOnCloud(syndicateCode, &info);
                }
            }

            if(info.CloudUUID.Length() == 0)
                throw new Exception("Cannot upload member on the server. UUID is empty");

            DBTransaction.Commit();
        }
        catch(Exception &err)
        {
            UnicodeString message = "server sync failed for member- key:" + info.ContactKey;
            message += " name: " + info.Name;
            message += ". Error: " + err.Message;

            LogErrorToDB(__FUNC__,ERRORLOG,message);
        }

        it++;
    }

    processingMembers = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateThread::UploadPendingTransactions()
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
            if(PostMemberTransactionsToCloud(*itTrans))
            {
                TLoyaltyMateUtilities::DeleteTransaction(*it, itTrans->TransactionNumber);
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
void TLoyaltyMateDownloadMemberThread::ThreadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Member download thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::DownloadMemberFromCloudUsingEmail()
{
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
	{
		TMMContactInfo contactInfo;

		if(Terminated)
		{
			ThreadTerminated();
            delete LoyaltyMateInterface;
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetailsByEmail(syndicateCode, MemberEmail, contactInfo, replacePoints);
        OperationSuccessful = response.IsSuccesful;
        if(response.ResponseCode == MemberNotExist)
        {
            ErrorMessage = "Member Not Exist.";
            throw Exception(ErrorMessage);
        }
        if(OperationSuccessful)
        {
           ReturnContactInfo = contactInfo;
        }
		else
		{
			throw new Exception("Failed to get member with Email.");
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
	}
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::DownloadMemberFromCloudUsingUUID()
{
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
	try
	{
		TMMContactInfo contactInfo;

		if(Terminated)
		{
			ThreadTerminated();
            delete LoyaltyMateInterface;
			return;
        }
        contactInfo.EMail = MemberEmail;
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
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::DownloadMemberFromCloudUsingCode()
{
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
	try
	{
		TMMContactInfo contactInfo;

		if(Terminated)
		{
			ThreadTerminated();
            delete LoyaltyMateInterface;
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetMemberDetailsByCode(syndicateCode, MemberCode, contactInfo, replacePoints);
        OperationSuccessful = response.IsSuccesful;
        if(response.ResponseCode == MemberNotExist)
        {
            ErrorMessage = "Member Not Exist.";
            throw Exception(ErrorMessage);
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
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateDownloadMemberThread::Execute()
{
    if(DownLoadFromUUID)
    {
       DownloadMemberFromCloudUsingUUID();
    }
    else if(DownLoadFromCode)
    {
      DownloadMemberFromCloudUsingCode();
    }
    else if(DownLoadFromEmail)
    {
      DownloadMemberFromCloudUsingEmail();
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
void TLoyaltyMateCreateMemberThread::ThreadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Member creation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateCreateMemberThread::CreateMemberOnCloud()
{
	try
	{
		if(Terminated)
		{
			ThreadTerminated();
			return;
        }

        TaskResult result =  TManagerLoyaltyMate::Instance()->CreateMemberOnCloud(_syndicateCode, *_memberInfo);
		OperationSuccessful = result.OperationSuccussful;

		if(!OperationSuccessful)
		{
            ErrorMessage = result.ErrorMessage;
            TGlobalSettings::Instance().IsPOSOffline = true;
            throw new Exception(ErrorMessage);
        }
        else
            TGlobalSettings::Instance().IsPOSOffline = false;
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
        TGlobalSettings::Instance().IsPOSOffline = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateCreateMemberThread::Execute()
{
    CreateMemberOnCloud();
    ReturnValue = 1;
}
//---------------------------------------------------------------------------


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateSyncThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateSyncThread::TLoyaltyMateSyncThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateSyncThread::ThreadTerminated()
{
    OperationSuccessful = false;
    ErrorMessage = "Sync operation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateSyncThread::Execute()
{
    SyncCompanyDetails();
    ReturnValue = 1;
}
//---------------------------------------------------------------------------
void TLoyaltyMateSyncThread::SyncCompanyDetails()
{
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
    {
        if(Terminated)
        {
            ThreadTerminated();
            delete LoyaltyMateInterface;
            return;
        }
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->SyncCompanyDetails(_syndicateCode);

        if(!createResponse.IsSuccesful  && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            OperationSuccessful = true;
        }
        else
        {
            ErrorMessage = createResponse.Message;
            throw Exception(createResponse.Message);
        }
   }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        ErrorMessage = E.Message;
    }
    delete LoyaltyMateInterface;
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
void TLoyaltyMateUpdateCardThread::ThreadTerminated()
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
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
    {
        if(Terminated)
        {
            ThreadTerminated();
            delete LoyaltyMateInterface;
            return;
        }
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->UpdateMemberCardCode(_syndicateCode,_uniqueId,_memberCardCode);
        if(!createResponse.IsSuccesful && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            OperationSuccessful = true;
        }
        else
        {
            if(createResponse.Description == "Card Code Already In Use.")
              ErrorMessage = "Card Code Already In Use.";
            else
              ErrorMessage = "Failed to update member information.";
            throw Exception(ErrorMessage);
        }
   }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        ErrorMessage = E.Message;
    }
    delete LoyaltyMateInterface;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateGiftVoucherThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
TLoyaltyMateGiftVoucherThread::TLoyaltyMateGiftVoucherThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMateGiftVoucherThread::ThreadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Gift Voucher Process thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateGiftVoucherThread::GetGiftVoucherBalance()
{
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
	{
        TGiftCardDetail giftCardDetail;
		if(Terminated)
		{
			ThreadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetGiftVoucherBalance(_syndicateCode, GiftCardNumber,giftCardDetail);
        OperationSuccessful = response.IsSuccesful;
        if(!OperationSuccessful)
        {
           ErrorMessage = response.Message;
           throw Exception(response.Message);
        }
		else
		{
            GiftCardDetail.ResponseMessage = giftCardDetail.ResponseMessage;
            GiftCardDetail.StatusCode = giftCardDetail.StatusCode;
            GiftCardDetail.PointBalance = giftCardDetail.PointBalance;
            GiftCardDetail.ExpiryDate = giftCardDetail.ExpiryDate;
            GiftCardDetail.StartDate = giftCardDetail.StartDate;
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateGiftVoucherThread::Execute()
{
  GetGiftVoucherBalance();
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMatePocketVoucherThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
TLoyaltyMatePocketVoucherThread::TLoyaltyMatePocketVoucherThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMatePocketVoucherThread::ThreadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Pocket Voucher Process thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMatePocketVoucherThread::GetPocketVoucherDetail()
{
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
	{
		TVoucherDetail voucherDetail;
		if(Terminated)
		{
			ThreadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetPocketVoucherDetail(_syndicateCode, VoucherNumber, voucherDetail);
        OperationSuccessful = response.IsSuccesful;
        if(!OperationSuccessful)
        {
           ErrorMessage = response.Message;
           throw Exception(response.Message);
        }
        else
        {
            VoucherDetail.VoucherName  = voucherDetail.VoucherName;
            VoucherDetail.DiscountCode  = voucherDetail.DiscountCode;
            VoucherDetail.NumberOfUsesRemaining  = voucherDetail.NumberOfUsesRemaining;
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMatePocketVoucherThread::Execute()
{
   GetPocketVoucherDetail();
}
//---------------------------------------------------------------------------


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateVoucherProcessThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
TLoyaltyMateVoucherProcessThread::TLoyaltyMateVoucherProcessThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMateVoucherProcessThread::ThreadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Voucher Process thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateVoucherProcessThread::ProcessVouchers()
{
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
    try
	{
		TMMContactInfo contactInfo;

		if(Terminated)
		{
			ThreadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->ProcessVoucherTransaction(_syndicateCode,VoucherUsageDetail);
        OperationSuccessful = response.IsSuccesful;
        if(!OperationSuccessful)
        {
           throw Exception(response.Message);
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		ErrorMessage = E.Message;
	}
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateVoucherProcessThread::Execute()
{
    ProcessVouchers();
}
//---------------------------------------------------------------------------



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateReleaseVoucherThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
TLoyaltyMateReleaseVoucherThread::TLoyaltyMateReleaseVoucherThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMateReleaseVoucherThread::ThreadTerminated()
{
	OperationSuccessful = false;
    ErrorMessage = "Voucher Process thread terminated before completion.";
}
//---------------------------------------------------------------------------
void TLoyaltyMateReleaseVoucherThread::ReleaseVouchers()
{
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
	{
		if(Terminated)
		{
			ThreadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->ReleaseVouchers(_syndicateCode,ReleasedVoucherDetail);
        OperationSuccessful = response.IsSuccesful;
        if(!OperationSuccessful)
        {
           ErrorMessage = response.Message;
           throw Exception(response.Message);
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateReleaseVoucherThread::Execute()
{
   ReleaseVouchers();
}
//---------------------------------------------------------------------------
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TLoyaltyMateOnlineOrderingThread Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TLoyaltyMateOnlineOrderingThread::TLoyaltyMateOnlineOrderingThread(TSyndCode inSyndicateCode)
	:TThread(true),
	 _syndicateCode(inSyndicateCode),
	 OperationSuccessful(false),
	 ErrorMessage("")
{
	FreeOnTerminate = true;
    UnsetSignalRStatus = false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateOnlineOrderingThread::ThreadTerminated()
{
    OperationSuccessful = false;
    ErrorMessage = "Sync operation thread terminated before completion.";
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateOnlineOrderingThread::Execute()
{
    if(UnsetSignalRStatus)
        UnsetSignalRStatusAtCloud();
    else
        SyncOnlineOrderingDetails();
    ReturnValue = 1;
}
//---------------------------------------------------------------------------
void TLoyaltyMateOnlineOrderingThread::UnsetSignalRStatusAtCloud()
{
    TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
    try
    {
        if(Terminated)
        {
            ThreadTerminated();
            delete LoyaltyMateInterface;
            return;
        }
        bool isSuccessful = LoyaltyMateInterface->UnsetOrderingDetails(_syndicateCode,TGlobalSettings::Instance().SiteID);
        if(isSuccessful)
            OperationSuccessful = true;
        else
            OperationSuccessful = false;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        ErrorMessage = E.Message;
    }
    delete LoyaltyMateInterface;
}
//---------------------------------------------------------------------------
void TLoyaltyMateOnlineOrderingThread::SyncOnlineOrderingDetails()
{
   TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
   try
    {
        if(Terminated)
        {
            ThreadTerminated();
            delete LoyaltyMateInterface;
            return;
        }
        MMLoyaltyServiceResponse createResponse = LoyaltyMateInterface->SyncOnlineOrderingDetails(_syndicateCode,TGlobalSettings::Instance().SiteID);

        if(!createResponse.IsSuccesful  && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Loyaltymate Service");
        }
        else if(createResponse.IsSuccesful)
        {
            OperationSuccessful = true;
        }
        else
        {
            ErrorMessage = createResponse.Message;
            throw Exception(createResponse.Message);
        }
   }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        ErrorMessage = E.Message;
    }
    delete LoyaltyMateInterface;
}
//------------------------------------------------------------------------------------
void TLoyaltyMateThread::SendEmail(Database::TDBTransaction &DBTransaction, TLoyaltyMateTransaction transaction)
{
    try
    {
        UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;
        AnsiString Dir = ExtractFilePath(Application->ExeName) + "MemberEmails";
        if (!DirectoryExists(Dir))
        {
           CreateDir(Dir);
        }

        AnsiString filename = Dir + "\\" + "MemberDetail.txt";
        AnsiString emailId = "development@menumate.com";

        AnsiString emailBody = "Multiple Email exist for same GUID having details as:\r";
                      emailBody += "Email:- " + TDBContacts::GetEmailIdOfMember(DBTransaction,transaction.ContactKey) + "\r";
                      emailBody += "Original Syndicate Code:- " + transaction.SyndicateCode.OriginalSyndCode + "\r";
                      emailBody += "Cloud UUID:- " + transaction.CloudUUID + "\r";
                      emailBody += "Invoice Number:- " + transaction.InvoiceNumber + "\r";
                      emailBody += "Site ID " + IntToStr(TGlobalSettings::Instance().SiteID) + "\r";
                      emailBody += "Contact Key:- " + IntToStr(transaction.ContactKey) + "\r\r";
                      emailBody += "Thank You";
        if(TGlobalSettings::Instance().EmailHost.Trim() != "" && TGlobalSettings::Instance().EmailHostPort != 0 &&
                    TGlobalSettings::Instance().EmailId.Trim() != "" && TGlobalSettings::Instance().EmailPassword.Trim() != "")
        {
            TMMProcessingState State(Screen->ActiveForm, "Sending Emails...", "Sending Emails...");
            TDeviceRealTerminal::Instance().ProcessingController.Push(State);
            SendEmail::Send(filename, "Multiple Email exist for same GUID.", emailId, emailBody);
            TDeviceRealTerminal::Instance().ProcessingController.Pop();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
