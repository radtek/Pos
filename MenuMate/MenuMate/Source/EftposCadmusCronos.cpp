//---------------------------------------------------------------------------


#pragma hdrstop

#include "EftposCadmusCronos.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CadmusEftposLib_OCX"
#include "Enum.h"
#include "Comms.h"
#include "Printout.h"
#include "DeviceRealTerminal.h"
#include "MMMessageBox.h"
#include "DropDown.h"
#include "DropDownVar.h"
#include "CSV.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosCadmus
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TStateResultMsg::TStateResultMsg()
{
   Result = eProcessing;
   ResultText = "Undefined";
   EventCompleted = false;
   AdditionalInfo = false;
   Action = NULL;
}

TStateResultMsg::TStateResultMsg(eTransactionResult inResult, AnsiString inResultText, bool inEventCompleted, bool inAdditionalInfo,TMMAction inAction)
{
   Result = inResult;
   ResultText = inResultText;
   EventCompleted = inEventCompleted;
   AdditionalInfo = inAdditionalInfo;
   Action = inAction;
}

void TStateResultMsg::AssignTo(TEftPosTransaction *EftTrans)
{
   EftTrans->Result = Result;
   EftTrans->ResultText = ResultText;
   EftTrans->EventCompleted = EventCompleted;
}

void TStateResultMsg::Assign(eTransactionResult inResult, AnsiString inResultText, bool inEventCompleted)
{
   Result = inResult;
   ResultText = inResultText;
   EventCompleted = inEventCompleted;
}

TEftPosCadmusCronos::TEftPosCadmusCronos()
{
	EFTPOSType = eTEftPosCadmusCronos;
	MerchantNumber = 1;
	ChequeAccountMaxLength = 10;
	ChequeBranchMaxLength = 8;
	ChequeSerialMaxLength = 6;
   State = EftCC_Idle;
   StateResult = TStateResultMsg();
   Online = false;
}


TEftPosCadmusCronos::~TEftPosCadmusCronos()
{
}

void TEftPosCadmusCronos::Initialise()
{
	TEftPos::Initialise();

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
	DBTransaction.Commit();
	try
	{
		if(TGlobalSettings::Instance().EnableEftPosCadmusCronos)
		{
         ChronosEFTPOSInterface.OnCronosEFTPOSMessage = EFT2POSMessage;
         InitialiseEFTPOS();
		}
		else
		{
			Enabled = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Initialise TEftPosCadmusCronos " + E.Message);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Initialise TEftPosCadmusCronos "  + E.Message);
		Enabled = false;
	}
}

void TEftPosCadmusCronos::DoControlPannel()
{
   if(!Enabled) return;
   
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
		DBTransaction.Commit();

		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("EFTPOS Logon",&DoLogon);
		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Settlement  CutOver",&DoSettlementCutover);
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
		frmDropDown->AddButton("EFTPOS Utils",&DoEFTUtils);
		frmDropDown->AddButton("Re-Initialise EFTPOS",&InitialiseEFTPOS);
		if(frmDropDown->ShowModal() == mrOk)
		{
			frmDropDown->FunctionToCall();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"DoControlPannel Failed" + E.Message);
	}
}

void __fastcall TEftPosCadmusCronos::InitialiseEFTPOS()
{
   Enabled = true;
   if(!DoConnected())
   {
      MessageBox("Unable To Start Cadmus Eftpos.\r Check the Cadmus Eftpos program is running & connected.", "Error",MB_OK + MB_ICONWARNING);
      Online = false;
   }
   else
   {
      Online = true;
   }
}

void TEftPosCadmusCronos::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
									 UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear)
{
   TEftPosCadmusCronosStates State = EftCC_Unknown;
	if(Online)
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
		DBTransaction.Commit();

		long PrimaryAmount = 0;
		long SecondaryAmount = 0;
		AnsiString Packet;
		switch (TxnType)
		{
			case TransactionType_PURCHASE_PLUS_CASH :
			case TransactionType_PURCHASE :
			switch(PANSource)
			{
				case PANSource_None:
				case PANSource_CardReader:
               State = EftCC_Purchase;
					Packet = BuildPurchasePacket(AmtPurchase,AmtCash);
				break;
				case PANSource_Manual:
               State = EftCC_ManualPAN;
					Packet = BuildManualPurchasePacket(AmtPurchase,CardSwipe,ExpiryMonth,ExpiryYear);
				break;
			}
			break;
			case TransactionType_CASH_ADVANCE :
            State = EftCC_Purchase;
				Packet = BuildCashOutPacket(AmtCash);
			break;
			case TransactionType_REFUND :
            State = EftCC_Refund;
				Packet = BuildRefundPacket(AmtPurchase,CardSwipe);
			break;
			case TransactionType_INQUIRY :
         {
            TEftPosTransaction *EftTrans = GetTransactionEvent();
            if(EftTrans != NULL)
            {
               EftTrans->Result = eManualQuery;
               EftTrans->ResultText = "Confirm Eftpos Transaction.";
               EftTrans->EventCompleted = true;
            }
         }
         break;
			case TransactionType_NONE :
			default :
			break;
		}

      if(State != EftCC_Unknown)
      {
         if(!BeginState(State,Packet))
         { // No responce from EFTPOS.
            TEftPosTransaction *EftTrans = GetTransactionEvent();
            if(EftTrans != NULL)
            {
               StateResult.AssignTo(EftTrans);
            }
         }
      }
	}
}

bool TEftPosCadmusCronos::DoConnected()
{
   bool RetVal = false;
   try
   {
      if(BeginState(EftCC_Connecting,BuildInitPacket()))
      {
         RetVal = (StateResult.Result == eAccepted);
      }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
      throw;
   }
   return RetVal;
}

void __fastcall TEftPosCadmusCronos::DoSettlementCutover()
{
	if(!Online) return;
	try
	{
      if(!BeginState(EftCC_Settlement,BuildSettlementCutoverPacket()))
      {
         MessageBox(StateResult.ResultText, "EFTPOS Error", MB_OK + MB_ICONINFORMATION);      
      }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosCadmusCronos::DoSettlementEnquiry()
{
	if(!Online) return;
	try
	{
      if(!BeginState(EftCC_Settlement,BuildSettlementEnquiryPacket(Now())))
      {
         MessageBox(StateResult.ResultText, "EFTPOS Error", MB_OK + MB_ICONINFORMATION);      
      }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

bool __fastcall TEftPosCadmusCronos::BeginState(TEftPosCadmusCronosStates State,AnsiString Packet)
{
   bool RetVal = false;
   WaitIfBusy();
   SetState(State);
   SendPacket(Packet);
   if(WaitOnStateComplete())
   {
      if(StateResult.Result == eAccepted)
      {
         RetVal = true;
      }
   }
   else
   {
      StateResult = TStateResultMsg(eFailed,"No Response from EFTPOS",true);
   }
   return RetVal;
}

void TEftPosCadmusCronos::SetState(TEftPosCadmusCronosStates inState)
{
   State = inState;
   StateResult = TStateResultMsg();
}

void TEftPosCadmusCronos::SetBusy()
{
   State = EftCC_BusyWithDialog;
}

void TEftPosCadmusCronos::ClearBusy()
{
   State = EftCC_Idle;
}

bool TEftPosCadmusCronos::WaitOnStateComplete(bool StateAllowTimeOut)
{
	if(Online)
	{
      TMMProcessingState State(Screen->ActiveForm,"Waiting on EFTPOS Please Wait...","Waiting on EFTPOS",false,false,1000 * EFT_STATE_TIMEOUT);
      TDeviceRealTerminal::Instance().ProcessingController.Push(State);

		__int64 StartTime = ::GetTickCount();
		__int64 TickCount = ::GetTickCount() + (1000 * EFT_STATE_TIMEOUT);
      bool AllowCancelNow = true;
		while ( ( (StateAllowTimeOut && ::GetTickCount() < TickCount) || (!StateAllowTimeOut) ) &&
                !StateResult.EventCompleted &&
                !TDeviceRealTerminal::Instance().ProcessingController.Cancelled())
		{
			TDeviceRealTerminal::Instance().ProcessingController.SetPosition(::GetTickCount() - StartTime);
			Application->ProcessMessages();

         if(::GetTickCount() > TickCount && AllowCancelNow)
         {
            AllowCancelNow = false;
            TDeviceRealTerminal::Instance().ProcessingController.Pop();
            TMMProcessingState State(Screen->ActiveForm,"Waiting on EFTPOS Please Wait...","Waiting on EFTPOS",true,false,1000 * EFT_STATE_TIMEOUT);
            TDeviceRealTerminal::Instance().ProcessingController.Push(State);
         }
      }
      TDeviceRealTerminal::Instance().ProcessingController.Pop();
	}
	return StateResult.EventCompleted;
}

void TEftPosCadmusCronos::WaitIfBusy()
{
   if(State == EftCC_BusyWithDialog)
   {
      StateResult = TStateResultMsg();
      WaitOnStateComplete(true);
      State = EftCC_Idle;
   }
}

void __fastcall TEftPosCadmusCronos::DoLogon()
{
   try
   {
      if(!BeginState(EftCC_Logon,BuildLogonPacket()))
      {
         MessageBox(StateResult.ResultText, "EFTPOS Error", MB_OK + MB_ICONINFORMATION);
      }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
      throw;
   }
}

void __fastcall TEftPosCadmusCronos::DoEFTUtils()
{
	if(!Online) return;

   try
   {
      if(!BeginState(EftCC_DisplayMenu,BuildUtilityPacket()))
      {
         MessageBox(StateResult.ResultText, "EFTPOS Error", MB_OK + MB_ICONINFORMATION);
      }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
      throw;
   }
}

void __fastcall TEftPosCadmusCronos::DoPrintLocalTotals()
{
}

void TEftPosCadmusCronos::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
							 AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
	if(!Online) return;
	try
	{
		AnsiString Packet = BuildChequeAuthPacket(AmtPurchase,ChequeBranch,ChequeAccount,ChequeSerial);
		SendPacket(Packet);
		TEftPosTransaction *EftTrans = GetTransactionEvent(TxnRef);
		if(EftTrans)
		{
			EftTrans->Result = eManualQuery;
			EftTrans->ResultText = "Confirm Cheque Authorisation.";
			EftTrans->EventCompleted = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

//kbybd_event

void __fastcall TEftPosCadmusCronos::ReprintReceipt()
{
   if(!Online) return;
	try
	{
      if(!BeginState(EftCC_ReqLastReceipt,BuildReprintReceiptPacket()))
      {
         MessageBox(StateResult.ResultText, "EFTPOS Error", MB_OK + MB_ICONINFORMATION);      
      }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

AnsiString TEftPosCadmusCronos::GetRefNumber()
{
	long UniqueID = 0;
	return IntToStr((int)UniqueID);
}

bool TEftPosCadmusCronos::CheckChequeBranch(AnsiString Branch)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Online)
   {
      try
      {
			if(Branch.Length() > ChequeBranchMaxLength)
			{
				ResultText = "Bank branch number must be Max of "+IntToStr(ChequeBranchMaxLength)+" digits.";
         }
         else if(!IsNumeric(Branch))
         {
				ResultText = "Bank branch number must only contain numbers";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosCadmusCronos" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosCadmusCronos"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
      }
	}
   return RetVal;
}

void TEftPosCadmusCronos::CancelTransaction(AnsiString inID)
{
	if(!Online) return;
	try
	{
		AnsiString Packet = BuildCancelTransactionPacket(inID);
		SendPacket(Packet);
		TEftPosTransaction *EftTrans = GetTransactionEvent(inID);
		if(EftTrans)
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = "Transaction Cancelled.";
			EftTrans->EventCompleted = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

bool TEftPosCadmusCronos::CheckChequeSerial(AnsiString ChequeSerial)
{
	bool RetVal = false;
	ResultText = "EFTPOS Disabled";
	if(Online)
	{
		try
		{
			if(ChequeSerial.Length() != ChequeSerialMaxLength)
			{
				ResultText = "Cheque serial number incorrect. "+IntToStr(ChequeSerialMaxLength)+" digits required.";
			}
			else if(!IsNumeric(ChequeSerial))
			{
				ResultText = "Cheque serial number must only contain numbers";
			}
			else
			{
				RetVal = true;
			}
		}
		catch(Exception &E)
		{
			TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosCadmusCronos" + E.Message);
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosCadmusCronos"  + E.Message);
			ResultText = "Cheque Serial Number Verification Failed";
		}
	}
	return RetVal;
}

bool TEftPosCadmusCronos::CheckChequeAccount(AnsiString ChequeAccount)
{
	bool RetVal = false;
	ResultText = "EFTPOS Disabled";
	if(Online)
   {
      try
      {
			if(ChequeAccount.Length() > ChequeAccountMaxLength)
			{
				ResultText = "Cheque Account number to long. Max "+IntToStr(ChequeAccountMaxLength)+" digits required.";
         }
         else if(!IsNumeric(ChequeAccount))
         {
				ResultText = "Cheque Account number must only contain numbers";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosCadmusCronos" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosCadmusCronos"  + E.Message);
			ResultText = "Cheque Serial Account Verification Failed";
		}
	}
	return RetVal;
}

void __fastcall TEftPosCadmusCronos::EFT2POSMessage(ChronosEFTPOS* Sender, AnsiString Text)
{
 // Decode the message.
   LastErrorMessage = "";
   CadmusMessage = Text;
   ParseMessge();
}
//---------------------------------------------------------------------------

void TEftPosCadmusCronos::ParseMessge()
{

   std::map< TStateResultIndex , TStateResultMsg > CadmusResults;

   //enum eTransactionResult {eFailed,eDeclined,eAccepted,eProcessing,eManualQuery};
   CadmusResults[TStateResultIndex(EftCC_Connecting,"APP","0")] = TStateResultMsg(eAccepted,"EFTPOS started successfully",true);
   CadmusResults[TStateResultIndex(EftCC_Connecting,"APP","1")] = TStateResultMsg(eFailed,"Cadmus Pinpad is not initialized",true);
   CadmusResults[TStateResultIndex(EftCC_Connecting,"APP","2")] = TStateResultMsg(eAccepted,"EFTPOS already running",true);
   CadmusResults[TStateResultIndex(EftCC_Connecting,"APP","F")] = TStateResultMsg(eFailed,"EFTPOS had failed to start",true);
   CadmusResults[TStateResultIndex(EftCC_Connecting,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Connecting,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Connecting,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);

   CadmusResults[TStateResultIndex(EftCC_Disconnecting,"APP","0")] = TStateResultMsg(eAccepted,"EFTPOS will be closed",true);
   CadmusResults[TStateResultIndex(EftCC_Disconnecting,"APP","1")] = TStateResultMsg(eFailed,"Cadmus Pinpad is not initialized",true);
   CadmusResults[TStateResultIndex(EftCC_Disconnecting,"APP","3")] = TStateResultMsg(eAccepted,"EFTPOS was not running",true);
   CadmusResults[TStateResultIndex(EftCC_Disconnecting,"APP","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);

   CadmusResults[TStateResultIndex(EftCC_Purchase,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Purchase,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);

   CadmusResults[TStateResultIndex(EftCC_BusyWithDialog,"STA","0")] = TStateResultMsg();

   CadmusResults[TStateResultIndex(EftCC_Refund,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Refund,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);
   CadmusResults[TStateResultIndex(EftCC_Refund,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Refund,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Refund,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);


   CadmusResults[TStateResultIndex(EftCC_Payment,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Payment,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);
   CadmusResults[TStateResultIndex(EftCC_Payment,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Payment,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Payment,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);


   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ChequeAuth,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);


   CadmusResults[TStateResultIndex(EftCC_Logon,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Logon,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);

   CadmusResults[TStateResultIndex(EftCC_Settlement,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_Settlement,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);


   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TRN","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will begin the transaction",false);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TRN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TRN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TRN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"PRT","0")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"PRT","1")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"PRT","2")] = TStateResultMsg(eProcessing,"EFTPOS Printing",false,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",true,false,&this->SetBusy);
   CadmusResults[TStateResultIndex(EftCC_ManualPAN,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",true);

   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"REP","0")] = TStateResultMsg(eAccepted,"Last transaction receipt available",false);
   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"REP","1")] = TStateResultMsg(eFailed,"Last transaction receipt unavailable",true);
   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"REP","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"REP","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);
   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"PRT","0")] = TStateResultMsg(eAccepted,"EFTPOS Printing",true,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"PRT","1")] = TStateResultMsg(eAccepted,"EFTPOS Printing",true,false,&this->Print);
   CadmusResults[TStateResultIndex(EftCC_ReqLastReceipt,"PRT","2")] = TStateResultMsg(eAccepted,"EFTPOS Printing",true,false,&this->Print);

   CadmusResults[TStateResultIndex(EftCC_DisplayMenu,"MNU","0")] = TStateResultMsg(eAccepted,"OK; EFTPOS will display its menu",true);
   CadmusResults[TStateResultIndex(EftCC_DisplayMenu,"MNU","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_DisplayMenu,"MNU","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);

   CadmusResults[TStateResultIndex(EftCC_CancelEFTPOS,"CAN","0")] = TStateResultMsg(eAccepted,"Transaction cancelled successfully for this merchant",true);
   CadmusResults[TStateResultIndex(EftCC_CancelEFTPOS,"CAN","1")] = TStateResultMsg(eFailed,"Error in request message; contains invalid data.",true, true);
   CadmusResults[TStateResultIndex(EftCC_CancelEFTPOS,"CAN","2")] = TStateResultMsg(eFailed,"No transaction was in progress for this merchant.",true);
   CadmusResults[TStateResultIndex(EftCC_CancelEFTPOS,"CAN","3")] = TStateResultMsg(eFailed,"Transaction has reached a point where it cannot be cancelled.",true);
   CadmusResults[TStateResultIndex(EftCC_CancelEFTPOS,"CAN","U")] = TStateResultMsg(eFailed,"EFTPOS Unavailable.",true);
   CadmusResults[TStateResultIndex(EftCC_CancelEFTPOS,"CAN","N")] = TStateResultMsg(eFailed,"EFTPOS application is not started",true);

   CadmusResults[TStateResultIndex(EftCC_TransactionCompletionAdvice,"TXC","A")] = TStateResultMsg(eAccepted,"transaction accepted (approved).",false);
   CadmusResults[TStateResultIndex(EftCC_TransactionCompletionAdvice,"TXC","D")] = TStateResultMsg(eDeclined,"transaction declined.",false);
   CadmusResults[TStateResultIndex(EftCC_TransactionCompletionAdvice,"TXC","F")] = TStateResultMsg(eFailed,"transaction failed.",false);
   CadmusResults[TStateResultIndex(EftCC_TransactionCompletionAdvice,"TXC","C")] = TStateResultMsg(eDeclined,"transaction cancelled by user.",false);
   CadmusResults[TStateResultIndex(EftCC_TransactionCompletionAdvice,"TXC","S")] = TStateResultMsg(eManualQuery,"signature required",false);

   CadmusResults[TStateResultIndex(EftCC_MerchantMessageStatus,"STA","0")] = TStateResultMsg(eAccepted,"merchant message has been removed from display",false);
   CadmusResults[TStateResultIndex(EftCC_MerchantMessageStatus,"STA","10")] = TStateResultMsg(eAccepted,"PIN pad comms lost",false);
   CadmusResults[TStateResultIndex(EftCC_MerchantMessageStatus,"STA","11")] = TStateResultMsg(eAccepted,"PIN pad comms established",false);

   CadmusResults[TStateResultIndex(EftCC_BusyWithDialog,"STA","0")] = TStateResultMsg(eAccepted,"merchant message has been removed from display",true,false,&this->ClearBusy);

   TCsv Csv;
   Csv.Add(CadmusMessage);

	if(State < EftCC_Idle) // Are we expecting a message?
	{
      // Is it the Message we are expecting.
      if(Csv.ColCount[0] > RESULTCOL)
      {
         TStateResultIndex CurrentIndex(State,Csv.Cells[APPCOL][0],Csv.Cells[RESULTCOL][0]);

         if(CadmusResults.find(CurrentIndex) != CadmusResults.end())
         {
            StateResult = CadmusResults[CurrentIndex];
            if(StateResult.Action != NULL)
            {
               StateResult.Action();
            }
         }
         else
         {
            StateResult.Assign(eFailed,"Undefined Result Code",true);
         }
      }
      else
      {
         StateResult.Assign(eFailed,"Undefined Result Code",true);
      }
      if(StateResult.AdditionalInfo && Csv.ColCount[0] > ERRORTXT)
      {
         StateResult.ResultText += " " + Csv.Cells[ERRORTXT][0];
      }
   }
   else // Transaction Started by Cadmus Machine.
   {
      // Is it the Message we are expecting.
      TEftPosCadmusCronosStates MsgType = MessageToCronoState(Csv.Cells[APPCOL][0],Csv.Cells[RESULTCOL][0]);
      switch(MsgType)
      {
         case EftCC_ReqLastReceipt :
            Print();
         break;
         case EftCC_MerchantMessageStatus :
         break;
         default :
            MessageBox("Unknown EFTPOS Message." + CadmusMessage, "EFTPOS Error",MB_OK + MB_ICONWARNING);
      }
   }

   // Complete the EFTPOS Event.
   if(StateResult.EventCompleted)
   {
      TEftPosTransaction *EftTrans = GetTransactionEvent();
      if(EftTrans != NULL)
      {
         StateResult.AssignTo(EftTrans);
      }
   }
}

TEftPosCadmusCronosStates TEftPosCadmusCronos::MessageToCronoState(AnsiString Msg,AnsiString SubType)
{
   if(Msg == "APP")   return EftCC_Connecting;
   else if(Msg == "TRN")
         if(SubType == "PUR")          return EftCC_Purchase;
         else if(SubType == "REF")     return EftCC_Refund;
         else if(SubType == "PAY")     return EftCC_Payment;
         else if(SubType == "CHQ")     return EftCC_ChequeAuth;
         else if(SubType == "LOG")     return EftCC_Logon;
         else if(SubType == "SET")     return EftCC_Settlement;
         else if(SubType == "MAN")     return EftCC_ManualPAN;
         else if(SubType == "SET")     return EftCC_Settlement;
         else                          return EftCC_Unknown;
   else if(Msg == "REP")   return EftCC_ReqLastReceipt;
   else if(Msg == "MNU")   return EftCC_DisplayMenu;
   else if(Msg == "CAN")   return EftCC_CancelEFTPOS;
   else if(Msg == "PRT")   return EftCC_ReqLastReceipt;
   else if(Msg == "TXC")   return EftCC_TransactionCompletionAdvice;
   else if(Msg == "STA")   return EftCC_MerchantMessageStatus;
   else                    return EftCC_Unknown;
}

void __fastcall TEftPosCadmusCronos::TransactionEvent(TEftPosTransaction *EftTrans,eTransactionResult Result)
{
	if(EftTrans != NULL)
	{
		switch(Result)
		{
			case eAccepted :
				EftTrans->Result = eAccepted;
				EftTrans->ResultText = "ACCEPTED";
      		EftTrans->EventCompleted = true;
			break;
			case eFailed :
			case eDeclined :
			case eManualQuery :
				EftTrans->Result = eDeclined;
				EftTrans->ResultText = LastErrorMessage;
      		EftTrans->EventCompleted = true;
			break;
  			case eProcessing :
         break;
		}
	}
}

void TEftPosCadmusCronos::SendPacket(AnsiString Data)
{

   ChronosEFTPOSInterface.Write( Data );

   std::auto_ptr<TStringList> List(new TStringList);
   if(FileExists(ExtractFilePath(Application->ExeName)+ "CadmusEftpos.log"))
   {
      List->LoadFromFile(ExtractFilePath(Application->ExeName)+ "CadmusEftpos.log");
   }
   List->Add(Data);
   while(List->Count > 100)
   {
      List->Delete(0);
   }
   List->SaveToFile(ExtractFilePath(Application->ExeName)+ "CadmusEftpos.log");
}

AnsiString TEftPosCadmusCronos::BuildInitPacket()
{
	AnsiString RetVal = "EFT,APP,S";
   return RetVal;

/*
Result Code, if Control Code was ‘S’:
0 – EFTPOS started successfully
1 – Cadmus Pinpad is not initialized
2 – EFTPOS already running
F – EFTPOS had failed to start
*/
}

AnsiString TEftPosCadmusCronos::BuildQuitPacket()
{
	AnsiString RetVal = "EFT,APP,Q";
   return RetVal;
/*
Result Code, if Control Code was ‘Q’:
0 – EFTPOS will be closed.
1 – Cadmus Pinpad is not initialized
3 – EFTPOS was not running.
U – Unavailable. EFTPOS is currently unavailable to perform this command.
It may be in the middle of processing a transaction or in the user menu.
*/

}

AnsiString TEftPosCadmusCronos::BuildPurchasePacket(Currency Purchase, Currency CashOut)
{
	AnsiString RetVal = "EFT,TRN,PUR,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",Purchase)+ ",";
	RetVal += FormatFloat("000000.00",CashOut);
   return RetVal;

/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildManualPurchasePacket(Currency Purchase,AnsiString Card, int Month, int Year)
{
	AnsiString RetVal = "EFT,TRN,MAN,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",Purchase)+ ",";
	RetVal += FormatFloat("00",Month) + FormatFloat("00",Year) + ",";
	RetVal += "" /*Optional Account Info*/ ",";
	RetVal += "08" /*Optional Account Info*/;
	if(Card != "")
	{
		RetVal += "," + Card;
	}
	return RetVal;

/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}


AnsiString TEftPosCadmusCronos::BuildCashOutPacket(Currency CashOut)
{
   // May now be redundant as is the same as BuildPurchasePacket except purchase = $0.00;
	AnsiString RetVal = "EFT,TRN,PUR,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",0)+ ",";
	RetVal += FormatFloat("000000.00",CashOut);
   return RetVal;
/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildCancelTransactionPacket(AnsiString inID)
{

	AnsiString RetVal = "EFT,CAN,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
/*
EFT Application Name
,CAN Command: Cancel Transaction
,R Result of send request:
0 – Transaction cancelled successfully for this merchant
1 – Error in request message; contains invalid data. See ErrorText below.
2 – No transaction was in progress for this merchant.
3 – Transaction has reached a point where it cannot be cancelled.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildRefundPacket(Currency Refund, AnsiString Card)
{
	AnsiString RetVal = "EFT,TRN,REF,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",Refund);
	if(Card != "")
	{
		RetVal += "," + Card;
	}
	return RetVal;
/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildChequeAuthPacket(Currency ChequeAmount, AnsiString BankAndBranch,
																AnsiString AccountNumber,AnsiString ChequeSerialNumber)
{
	const int MaxBankAndBranch = 8;
	const int MaxAccountNumber = 10;
	const int MaxChequeSerial = 6;

	if(BankAndBranch.Length() < MaxBankAndBranch)
	{
		int LengthToPack = MaxBankAndBranch - BankAndBranch.Length();
		BankAndBranch = AnsiString::StringOfChar(' ',LengthToPack) + BankAndBranch;
	}

	if(AccountNumber.Length() < MaxAccountNumber)
	{
		int LengthToPack = MaxAccountNumber - BankAndBranch.Length();
		AccountNumber = AnsiString::StringOfChar(' ',LengthToPack) + AccountNumber;
	}

	if(ChequeSerialNumber.Length() < MaxChequeSerial)
	{
		int LengthToPack = MaxChequeSerial - ChequeSerialNumber.Length();
		ChequeSerialNumber = AnsiString::StringOfChar(' ',LengthToPack) + ChequeSerialNumber;
	}

	AnsiString RetVal = "EFT,TRN,CHQ,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",ChequeAmount) + ",";
	RetVal += BankAndBranch + ",";
	RetVal += AccountNumber + ",";
	RetVal += ChequeSerialNumber;
	return RetVal;

/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildLogonPacket()
{
	
	AnsiString RetVal = "EFT,TRN,LOG,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += "MenuMate " + TDeviceRealTerminal::Instance().OS.MMSoftwareVersion;
	return RetVal;

/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error
*/
}

AnsiString TEftPosCadmusCronos::BuildCancelPacket()
{
	AnsiString RetVal = "EFT,CAN,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
/*
EFT Application Name
,CAN Command: Cancel Transaction
,R Result of send request:
0 – Transaction cancelled successfully for this merchant
1 – Error in request message; contains invalid data. See ErrorText below.
2 – No transaction was in progress for this merchant.
3 – Transaction has reached a point where it cannot be cancelled.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildSigOkPacket(bool SigOk)
{
	
	AnsiString RetVal = "SIG,";
	if(SigOk)
	{
		RetVal += "YES";
	}
	else
	{
		RetVal += "NO";
	}
	return RetVal;
}

AnsiString TEftPosCadmusCronos::BuildReprintReceiptPacket()
{
	
	AnsiString RetVal = "EFT,REP,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
/*
EFT Application Name
,REP Command: Reprint last transaction receipt
,R Result Code:
0 – Last transaction receipt available. An EFTPOS print message (PRT) will be
sent to the POS immediately after this message is sent.
1 – Last transaction receipt unavailable
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
*/
}

AnsiString TEftPosCadmusCronos::BuildSettlementCutoverPacket()
{
	
	AnsiString RetVal = "EFT,TRN,SET,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += IntToStr(1);
	return RetVal;
/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildSettlementEnquiryPacket(TDateTime Date)
{
	AnsiString RetVal = "EFT,TRN,SET,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += IntToStr(0) + ",";
	RetVal += FormatDateTime("ddmmyyyy",Date);
	return RetVal;
/*
EFT Application Name
,TRN Command: Start Transaction
,R Result Code:
0 – OK; EFTPOS will begin the transaction
1 – Error in request message; contains invalid data. See ErrorText below.
U – Unavailable; EFTPOS is currently unavailable to perform this command.
N – EFTPOS application is not started.
,ErrorText When result code equals 1 ErrorText specifies data field in error.
*/
}

AnsiString TEftPosCadmusCronos::BuildUtilityPacket()
{
	AnsiString RetVal = "EFT,MNU";
	return RetVal;
/*
EFT Application Name
,MNU Command: Display EFTPOS Menu
,R Result Code:
0 – OK; EFTPOS will display its menu
U – Unavailable; EFTPOS is currently unavailable to perform this command
N – EFTPOS application is not started.
*/
}

void TEftPosCadmusCronos::Print()
{
   TCsv Csv;
   Csv.Add(CadmusMessage);
	LastEftPosReceipt->Clear();
	LastEftPosReceipt->Text = Csv.Cells[ERRORTXT][0];

	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		TManagerLogs::Instance().Add("NA",EFTPOSLOG,"No Recipt Printer Configured for EFTPOS output.");
		TManagerLogs::Instance().Add("NA",ERRORLOG,"No Recipt Printer Configured for EFTPOS output.");
		return;
	}

	std::auto_ptr<TPrintout> Printout (new TPrintout);
	Printout->Printer = TComms::Instance().ReceiptPrinter;
	try
	{
      Printout->PrintFormat->DocumentName = "EFTPOS RECEIPT";

//         Printout->PrintFormat->PrintGraphic();

		Printout->PrintFormat->Line->FontInfo.Reset();
		Printout->PrintFormat->Line->ColCount					= 1;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;

		for (int i=0; i< LastEftPosReceipt->Count; i++)
		{
			LastEftPosReceipt->Strings[i] = LastEftPosReceipt->Strings[i].Trim();
			Printout->PrintFormat->Line->Columns[0]->Text	= LastEftPosReceipt->Strings[i];
			Printout->PrintFormat->AddLine();
		}

		Printout->PrintFormat->PartialCut();

		Printout->Print();
	}
	catch (Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
   }
}
