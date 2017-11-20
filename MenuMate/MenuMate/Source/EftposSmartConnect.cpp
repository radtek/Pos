//---------------------------------------------------------------------------

#pragma hdrstop

#include "EftposSmartConnect.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
#include "SmartConnectDataObjects.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TEftPosSmartConnect::TEftPosSmartConnect()
{
  InitSmartConnectClient();
}
// ---------------------------------------------------------------------------
TEftPosSmartConnect::~TEftPosSmartConnect()
{
//
}
// ---------------------------------------------------------------------------
void TEftPosSmartConnect::Initialise()
{
    TEftPos::Initialise();
    if(TGlobalSettings::Instance().EnableEftPosSmartConnect)
    {
        Enabled = true;
    }
    else
    {
        Enabled = false;
    }
}
// ---------------------------------------------------------------------------
void TEftPosSmartConnect::DoControlPannel()
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
void TEftPosSmartConnect::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase, Currency AmtCash, UnicodeString TxnRef,
                   ePANSource PANSource, UnicodeString CardSwipe, int ExpiryMonth, int ExpiryYear)
{
  if(Enabled)
   {
        try
        {
            SmartConnectResponse *wcfResponse;
            CoInitialize(NULL);
            PairingTerminal  *pairingTerminal = new PairingTerminal();
            pairingTerminal->PosBusinessName = "TCafe";
            pairingTerminal->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
            pairingTerminal->PosRegisterName = "Main Register5";
            pairingTerminal->PosVendorName = "MenumateIndia";
            pairingTerminal->PairingCode = "63247216";
//            MessageBox("Correct Position3", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
            wcfResponse = smartConnectClient->Pairing(pairingTerminal);
//            if(wcfResponse->ResponseSuccessful)
//            {
                MessageBox("Response Successfull", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
                TransactionTypes *transactionType = new TransactionTypes();
                transactionType->PosBusinessName = "TCafe";
                transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                transactionType->PosVendorName = "MenumateIndia";
                transactionType->Transactiontype = "Acquirer.Logon";

                wcfResponse = smartConnectClient->Logon(transactionType);
                MessageBox("Response Successfull After logon", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);

//                if(wcfResponse->ResponseSuccessful)
//                {
                    //TransactionTypes *transactionType = new TransactionTypes();
                    transactionType->PosBusinessName = "TCafe";
                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                    transactionType->PosVendorName = "MenumateIndia";
                    transactionType->Transactiontype = "Acquirer.Settlement.Inquiry";

                    wcfResponse = smartConnectClient->SettlementInquiry(transactionType);

                    transactionType->PosBusinessName = "TCafe";
                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                    transactionType->PosVendorName = "MenumateIndia";
                    transactionType->Transactiontype = "Acquirer.Settlement.Cutover";

                    wcfResponse = smartConnectClient->SettlementCutover(transactionType);

                    transactionType->PosBusinessName = "TCafe";
                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                    transactionType->PosVendorName = "MenumateIndia";
                    transactionType->Transactiontype = "Card.Purchase";

                    wcfResponse = smartConnectClient->Purchase(transactionType, AmtPurchase);

                    transactionType->PosBusinessName = "TCafe";
                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                    transactionType->PosVendorName = "MenumateIndia";
                    transactionType->Transactiontype = "Card.PurchasePlusCash";

                    wcfResponse = smartConnectClient->PurchasePlusCash(transactionType, AmtPurchase, AmtCash);

                    transactionType->PosBusinessName = "TCafe";
                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                    transactionType->PosVendorName = "MenumateIndia";
                    transactionType->Transactiontype = "Card.CashAdvance";

                    wcfResponse = smartConnectClient->CashOutOnly(transactionType, AmtCash);

                    transactionType->PosBusinessName = "TCafe";
                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
                    transactionType->PosVendorName = "MenumateIndia";
                    transactionType->Transactiontype = "Card.Refund";

                    wcfResponse = smartConnectClient->Refund(transactionType, AmtPurchase);
//
//                    transactionType->PosBusinessName = "TCafe";
//                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//                    transactionType->PosVendorName = "MenumateIndia";
//                    transactionType->Transactiontype = "Acquirer.Settlement.Inquiry";
//
//                    wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
//
//                    transactionType->PosBusinessName = "TCafe";
//                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//                    transactionType->PosVendorName = "MenumateIndia";
//                    transactionType->Transactiontype = "Acquirer.Settlement.Inquiry";
//
//                    wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
//
//                    transactionType->PosBusinessName = "TCafe";
//                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//                    transactionType->PosVendorName = "MenumateIndia";
//                    transactionType->Transactiontype = "Acquirer.Settlement.Inquiry";
//
//                    wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
//
//                    transactionType->PosBusinessName = "TCafe";
//                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//                    transactionType->PosVendorName = "MenumateIndia";
//                    transactionType->Transactiontype = "Acquirer.Settlement.Inquiry";
//
//                    wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
//
//                    transactionType->PosBusinessName = "TCafe";
//                    transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//                    transactionType->PosVendorName = "MenumateIndia";
//                    transactionType->Transactiontype = "Acquirer.Settlement.Inquiry";
//
//                    wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
             //   }

//            }
//             else
//                MessageBox("Response Unsuccessfull", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
           /*if(PingTerminal(TxnType))
           {
               SmartConnectResponse *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
                  {
                    case TransactionType_PURCHASE :
                        // wcfResponse = smartConnectClient->Purchase(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase);

                    break;
                    case TransactionType_CASH_ADVANCE :
                         //wcfResponse = smartConnectClient->CashOnly(TGlobalSettings::Instance().EftPosSmartPayIp,AmtCash);
                    break;
                    case TransactionType_PURCHASE_PLUS_CASH :
                        // wcfResponse = smartConnectClient->PurchasePlusCash(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase,AmtCash);
                    break;
                    case TransactionType_REFUND :
                        // wcfResponse = smartConnectClient->Refund(TGlobalSettings::Instance().EftPosSmartPayIp,AmtPurchase);
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
                  if(wcfResponse->ResponseSuccessful)
                   {
                      AcquirerRefSmartConnect = wcfResponse->SmartConnectData->AcquirerRef;
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          EftTrans->ResultText = "Eftpos Transaction Completed.";
                          EftTrans->Result = eAccepted;
                          //EftTrans->TimeOut = wcfResponse->TimeOut;
                   }    }
                  else
                   {
//                      MessageBox("Transaction Failed", "EFTPOS ",MB_OK + MB_ICONERROR);
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          EftTrans->Result = eDeclined;
                          EftTrans->ResultText = wcfResponse->SmartConnectData->Result;
                         // EftTrans->TimeOut = wcfResponse->TimeOut;
                       }
                   }
             }  */
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


bool  TEftPosSmartConnect::PingTerminal(eEFTTransactionType TxnType)
{
   bool retVal = false;
//   SmartConnectResponse *wcfResponse;
//   CoInitialize(NULL);
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
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoSettlementCutover()
{
//   try
//    {
//       SmartConnectResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->SettlementCutover(TGlobalSettings::Instance().EftPosSmartPayIp);
//    }
//    catch( Exception& E )
//    {
//        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
//    }
}

// ---------------------------------------------------------------------------
void TEftPosSmartConnect::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
                   AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosSmartConnect::CheckChequeBranch(AnsiString Branch)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosSmartConnect::CheckChequeSerial(AnsiString ChequeSerial)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosSmartConnect::CheckChequeAccount(AnsiString ChequeAccount)
{
 //
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoSettlementEnquiry()
{
//   try
//    {
//       SmartConnectResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->SettlementInquiry(TGlobalSettings::Instance().EftPosSmartPayIp);
//    }
//    catch( Exception& E )
//    {
//        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
//    }
}

// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::ReprintReceipt()
{
//   try
//    {
//       SmartConnectResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->ReprintLastReceipt(TGlobalSettings::Instance().EftPosSmartPayIp);
//    }
//    catch( Exception& E )
//    {
//        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
//    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoLogon()
{
//   try
//    {
//       SmartConnectResponse *wcfResponse;
//       CoInitialize(NULL);
//       wcfResponse = smartLinkClient->Login(TGlobalSettings::Instance().EftPosSmartPayIp,"Menumate v6");
//    }
//    catch( Exception& E )
//    {
//        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
//    }
}
// ---------------------------------------------------------------------------

AnsiString TEftPosSmartConnect::GetRefNumber()
{
  return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}
// ---------------------------------------------------------------------------
void TEftPosSmartConnect::InitSmartConnectClient()
{
    bool useWSDL = false;
    AnsiString smartConnectURL = "http://localhost:8744/MenumateServices/SmartConnect";
    smartConnectClient = GetIWCFSmartConnect(useWSDL, smartConnectURL, NULL );
}


