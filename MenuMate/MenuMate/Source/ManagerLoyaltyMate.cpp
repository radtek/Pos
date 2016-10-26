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

    StartLoyaltyThreadTimer();
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
    TLoyaltyMateMemberOperation operation(syndicateCode,info);
    uploadQueue.push_back(operation);
    InitiateLoyaltyThread();
    StartLoyaltyThread();
}
//---------------------------------------------------------------------------
bool TManagerLoyaltyMate::CreateMemberOnCloud(TSyndCode syndicateCode,TMMContactInfo &info)
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

    return result;
}
//---------------------------------------------------------------------------
bool TLoyaltyMateThread::PostMemberTransactionsToCloud(TLoyaltyMateTransaction transaction)
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
                                                                                transaction.PointsType,
                                                                                transaction.InvoiceNumber);
            if(!postTransactionResponse.IsSuccesful)
                throw new Exception(postTransactionResponse.Message);
            else
                result = true;
        }
    }
    catch(Exception &exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,exc.Message);
    }
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
   try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			ThreadTerminated();
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
}
//---------------------------------------------------------------------------
void TLoyaltyMateDownloadMemberThread::DownloadMemberFromCloudUsingUUID()
{
	try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			ThreadTerminated();
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
void TLoyaltyMateDownloadMemberThread::DownloadMemberFromCloudUsingCode()
{
	try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			ThreadTerminated();
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

		OperationSuccessful = TManagerLoyaltyMate::Instance()->CreateMemberOnCloud(_syndicateCode, *_memberInfo);

		if(!OperationSuccessful)
		{
            ErrorMessage = "Failed to create member on the server";
            TGlobalSettings::Instance().IsPOSOffline=true;
            throw new Exception(ErrorMessage);
        }
        else
            TGlobalSettings::Instance().IsPOSOffline = false;
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
 try
    {
        if(Terminated)
        {
            ThreadTerminated();
            return;
        }

        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
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
 try
    {
        if(Terminated)
        {
                ThreadTerminated();
                return;
        }

        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
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
        ErrorMessage = E.Message;
    }

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
   try
	{
        double voucherBalance = 0;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
		if(Terminated)
		{
			ThreadTerminated();
			return;
        }
        MMLoyaltyServiceResponse response = LoyaltyMateInterface->GetGiftVoucherBalance(_syndicateCode, VoucherNumber,voucherBalance);
        OperationSuccessful = response.IsSuccesful;
        if(!OperationSuccessful)
        {
           ErrorMessage = response.Message;
           throw Exception(response.Message);
        }
		else
		{
           GiftCardBalance = voucherBalance;
        }
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
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
   try
	{
		TVoucherDetail voucherDetail;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
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
try
	{
		TMMContactInfo contactInfo;
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
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
   try
	{
        TLoyaltyMateInterface* LoyaltyMateInterface = new TLoyaltyMateInterface();
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
}
//---------------------------------------------------------------------------
void __fastcall TLoyaltyMateReleaseVoucherThread::Execute()
{
   ReleaseVouchers();
}
//---------------------------------------------------------------------------
