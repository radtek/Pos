//---------------------------------------------------------------------------


#pragma hdrstop

#include "EftposAdyen.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TEftposAdyen::TEftposAdyen()
{
}
// ---------------------------------------------------------------------------
TEftposAdyen::~TEftposAdyen()
{
//
}
// ---------------------------------------------------------------------------
void TEftposAdyen::Initialise()
{
    TEftPos::Initialise();
    if(TGlobalSettings::Instance().EnableEftPosAdyen)
      {
         MessageBox("Enabled Adyen","",MB_OK);
         Enabled = true;
      }
    else
      {
         MessageBox("Disabled Adyen","",MB_OK);
         Enabled = false;
      }
}
// ---------------------------------------------------------------------------
void TEftposAdyen::DoControlPannel()
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
void TEftposAdyen::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase, Currency AmtCash, UnicodeString TxnRef,
                   ePANSource PANSource, UnicodeString CardSwipe, int ExpiryMonth, int ExpiryYear)
{
  if(Enabled)
   {
        try
        {
           if(PingTerminal(TxnType))
           {
//               DTO_SmartLinkTransactionResponse *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
                  {
                    case TransactionType_PURCHASE :
//                         wcfResponse = smartLinkClient->Purchase(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase);

                    break;
                    case TransactionType_CASH_ADVANCE :
//                         wcfResponse = smartLinkClient->CashOnly(TGlobalSettings::Instance().EftPosSmartPayIp,AmtCash);
                    break;
                    case TransactionType_PURCHASE_PLUS_CASH :
//                         wcfResponse = smartLinkClient->PurchasePlusCash(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase,AmtCash);
                    break;
                    case TransactionType_REFUND :
//                         wcfResponse = smartLinkClient->Refund(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase);
                    break;
                    case TransactionType_INQUIRY :
//                         TEftPosTransaction *EftTrans = GetTransactionEvent();
//                         if(EftTrans != NULL)
//                            {
//                               EftTrans->Result = eManualQuery;
//                               EftTrans->ResultText = "Confirm Eftpos Transaction.";
//                               EftTrans->EventCompleted = true;
//                            }
                    break;
                  }
//                  if(wcfResponse->Successful)
//                   {
//                      AcquirerRefSmartPay = wcfResponse->AcquirerRef;
//                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
//                      if(EftTrans != NULL)
//                       {
//                          EftTrans->EventCompleted = true;
//                          EftTrans->ResultText = "Eftpos Transaction Completed.";
//                          EftTrans->Result = eAccepted;
//                          EftTrans->TimeOut = wcfResponse->TimeOut;
//                   }    }
//                  else
//                   {
//                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
//                      if(EftTrans != NULL)
//                       {
//                          EftTrans->EventCompleted = true;
//                          EftTrans->Result = eDeclined;
//                          EftTrans->ResultText = wcfResponse->ErrorText;
//                          EftTrans->TimeOut = wcfResponse->TimeOut;
//                       }
//                   }
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
//----------------------------------------------------------------------------
bool  TEftposAdyen::PingTerminal(eEFTTransactionType TxnType)
{
   bool retVal = false;
//   DTO_SmartLinkTransactionResponse *wcfResponse;
   CoInitialize(NULL);
//   wcfResponse = smartLinkClient->PingTerminal(TGlobalSettings::Instance().EftPosSmartPayIp);
//   retVal = wcfResponse->Successful;
//   if(!wcfResponse->Successful)
//   {
//      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
//      if(EftTrans != NULL)
//       {
//          EftTrans->EventCompleted = true;
//          EftTrans->Result = eDeclined;
//          EftTrans->ResultText = wcfResponse->ErrorText;
//       }
//   }
   return retVal ;
}
//----------------------------------------------------------------------------
void TEftposAdyen::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
                   AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftposAdyen::CheckChequeBranch(AnsiString Branch)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftposAdyen::CheckChequeSerial(AnsiString ChequeSerial)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftposAdyen::CheckChequeAccount(AnsiString ChequeAccount)
{
 //
}
// ---------------------------------------------------------------------------
void __fastcall TEftposAdyen::DoSettlementCutover()
{
   try
    {
//       DTO_SmartLinkTransactionResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->SettlementCutover(TGlobalSettings::Instance().EftPosSmartPayIp);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }


}
// ---------------------------------------------------------------------------
void __fastcall TEftposAdyen::DoSettlementEnquiry()
{
   try
    {
//       DTO_SmartLinkTransactionResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->SettlementInquiry(TGlobalSettings::Instance().EftPosSmartPayIp);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TEftposAdyen::ReprintReceipt()
{
   try
    {
//       DTO_SmartLinkTransactionResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->ReprintLastReceipt(TGlobalSettings::Instance().EftPosSmartPayIp);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftposAdyen::DoLogon()
{
   try
    {
//       DTO_SmartLinkTransactionResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->Login(TGlobalSettings::Instance().EftPosSmartPayIp,"Menumate v6");
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------

AnsiString TEftposAdyen::GetRefNumber()
{
//  return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}
// ---------------------------------------------------------------------------
void TEftposAdyen::initAdyenClient()
{
//    bool useWSDL = false;
//    AnsiString smartLinkURL = "http://localhost:8738/MenumateServices/SmartLink";
//    smartLinkClient = GetIWCFServiceSmartLink(useWSDL, smartLinkURL, NULL );
}
// ---------------------------------------------------------------------------
