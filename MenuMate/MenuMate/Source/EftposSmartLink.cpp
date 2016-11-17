//---------------------------------------------------------------------------

#pragma hdrstop

#include "EftposSmartLink.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TEftPosSmartLink::TEftPosSmartLink()
{
  initSmartLinkClient();
}
// ---------------------------------------------------------------------------
TEftPosSmartLink::~TEftPosSmartLink()
{
//
}
// ---------------------------------------------------------------------------
void TEftPosSmartLink::Initialise()
{
    TEftPos::Initialise();
    if(TGlobalSettings::Instance().EnableEftPosSmartPay)
      {
         Enabled = true;
      }
    else
      {
         Enabled = false;
      }
}
// ---------------------------------------------------------------------------
void TEftPosSmartLink::DoControlPannel()
{
 if(!Enabled) return;
	try
	{
		/*Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
		PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
		DBTransaction.Commit();*/

		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("EFTPOS Logon",&DoLogon);
		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Settlement  CutOver",&DoSettlementCutover);
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
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
// ---------------------------------------------------------------------------
void TEftPosSmartLink::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase, Currency AmtCash, UnicodeString TxnRef,
                   ePANSource PANSource, UnicodeString CardSwipe, int ExpiryMonth, int ExpiryYear)
{
  if(Enabled)
   {
        try
        {
           if(PingTerminal(TxnType))
           {
               DTO_SmartLinkTransactionResponse *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
                  {
                    case TransactionType_PURCHASE :
                         wcfResponse = smartLinkClient->Purchase(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase);

                    break;
                    case TransactionType_CASH_ADVANCE :
                         wcfResponse = smartLinkClient->CashOnly(TGlobalSettings::Instance().EftPosSmartPayIp,AmtCash);
                    break;
                    case TransactionType_PURCHASE_PLUS_CASH :
                         wcfResponse = smartLinkClient->PurchasePlusCash(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase,AmtCash);
                    break;
                    case TransactionType_REFUND :
                         wcfResponse = smartLinkClient->Refund(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase);
                    break;
                    case TransactionType_INQUIRY :
                         TEftPosTransaction *EftTrans = GetTransactionEvent();
                         if(EftTrans != NULL)
                            {
                               EftTrans->Result = eManualQuery;
                               EftTrans->ResultText = "Confirm Eftpos Transaction.";
                               EftTrans->EventCompleted = true;
                            }
                    break;
                  }
                  if(wcfResponse->Successful)
                   {
                      AcquirerRefSmartPay = wcfResponse->AcquirerRef;
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          EftTrans->ResultText = "Eftpos Transaction Completed.";
                          EftTrans->Result = eAccepted;
                          EftTrans->TimeOut = wcfResponse->TimeOut;
                   }    }
                  else
                   {
//                      MessageBox("Transaction Failed", "EFTPOS ",MB_OK + MB_ICONERROR);
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          EftTrans->Result = eDeclined;
                          EftTrans->ResultText = wcfResponse->ErrorText;
                          EftTrans->TimeOut = wcfResponse->TimeOut;
                       }
                   }
             }
        }
        catch( Exception& exc )
        {
              TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
              if(EftTrans != NULL)
               {
                  EftTrans->EventCompleted = true;
                  EftTrans->Result = eDeclined;
                  EftTrans->ResultText = exc.Message;
               }
        }
   }
}


bool  TEftPosSmartLink::PingTerminal(eEFTTransactionType TxnType)
{
   bool retVal = false;
   DTO_SmartLinkTransactionResponse *wcfResponse;
   CoInitialize(NULL);
   wcfResponse = smartLinkClient->PingTerminal(TGlobalSettings::Instance().EftPosSmartPayIp);
   retVal = wcfResponse->Successful;
   if(!wcfResponse->Successful)
   {
      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
      if(EftTrans != NULL)
       {
          EftTrans->EventCompleted = true;
          EftTrans->Result = eDeclined;
          EftTrans->ResultText = wcfResponse->ErrorText;
       }
   }
   return retVal ;
}


// ---------------------------------------------------------------------------
void TEftPosSmartLink::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
                   AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosSmartLink::CheckChequeBranch(AnsiString Branch)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosSmartLink::CheckChequeSerial(AnsiString ChequeSerial)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosSmartLink::CheckChequeAccount(AnsiString ChequeAccount)
{
 //
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartLink::DoSettlementCutover()
{
   try
    {
       DTO_SmartLinkTransactionResponse *wcfResponse;
       CoInitialize(NULL);
       wcfResponse = smartLinkClient->SettlementCutover(TGlobalSettings::Instance().EftPosSmartPayIp);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }


}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartLink::DoSettlementEnquiry()
{
   try
    {
       DTO_SmartLinkTransactionResponse *wcfResponse;
       CoInitialize(NULL);
       wcfResponse = smartLinkClient->SettlementInquiry(TGlobalSettings::Instance().EftPosSmartPayIp);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartLink::ReprintReceipt()
{
   try
    {
       DTO_SmartLinkTransactionResponse *wcfResponse;
       CoInitialize(NULL);
       wcfResponse = smartLinkClient->ReprintLastReceipt(TGlobalSettings::Instance().EftPosSmartPayIp);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartLink::DoLogon()
{
   try
    {
       DTO_SmartLinkTransactionResponse *wcfResponse;
       CoInitialize(NULL);
       wcfResponse = smartLinkClient->Login(TGlobalSettings::Instance().EftPosSmartPayIp,"Menumate v6");
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------

AnsiString TEftPosSmartLink::GetRefNumber()
{
  return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}
// ---------------------------------------------------------------------------
void TEftPosSmartLink::initSmartLinkClient()
{
    bool useWSDL = false;
    AnsiString smartLinkURL = "http://localhost:8738/MenumateServices/SmartLink";
    smartLinkClient = GetIWCFServiceSmartLink(useWSDL, smartLinkURL, NULL );
}
