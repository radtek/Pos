//---------------------------------------------------------------------------

#pragma hdrstop

#include "EftposSmartConnect.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "DeviceRealTerminal.h"
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
		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
        frmDropDown->AddButton("Pairing With Terminal",&DoPairing);
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
//            CoInitialize(NULL);
//            TransactionTypes *transactionType = new TransactionTypes();
//
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Card.Purchase";
//
//            wcfResponse = smartConnectClient->Purchase(transactionType, AmtPurchase);
//
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Card.PurchasePlusCash";
//
//            wcfResponse = smartConnectClient->PurchasePlusCash(transactionType, AmtPurchase, AmtCash);
//            MessageBox("Before Cash out only", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Card.CashAdvance";
//
//            wcfResponse = smartConnectClient->CashOutOnly(transactionType, AmtCash);
//            MessageBox("Before Refund ", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Card.Refund";
//
//            wcfResponse = smartConnectClient->Refund(transactionType, AmtPurchase);
//
//
//
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Card.Authorise";
//
//            wcfResponse = smartConnectClient->Authorise(transactionType, AmtPurchase, TxnRef);
//             MessageBox("Before Finalize ", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Card.Finalise";
//
//            wcfResponse = smartConnectClient->Finalise(transactionType, AmtPurchase, TxnRef);   //todo add cahout amount also
//
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Journal.GetTransResult";
//
//            wcfResponse = smartConnectClient->GetTransactionResult(transactionType);
//
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Terminal.GetStatus";
//
//            wcfResponse = smartConnectClient->GetTerminalStatus(transactionType);
//              MessageBox("Before  Terminal Read Card ", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
//            transactionType->PosBusinessName = "TCafe";
//            transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
//            transactionType->PosVendorName = "MenumateIndia";
//            transactionType->Transactiontype = "Terminal.ReadCard";
//
//            wcfResponse = smartConnectClient->TerminalReadCard(transactionType);
//                    MessageBox("After Terminal Read Card ", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
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
                  }   */
                  if(wcfResponse->ResponseSuccessful)
                   {
                      AcquirerRefSmartConnect = wcfResponse->data->AcquirerRef;
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          //EftTrans->FinalAmount = wcfResponse->data->AmountTotal;
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
                          EftTrans->ResultText = wcfResponse->data->Result;
                         // EftTrans->TimeOut = wcfResponse->TimeOut;
                       }
                   }
             //}
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
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        TransactionTypes *transactionType = new TransactionTypes();
        transactionType->PosBusinessName = "TCafe";
        transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
        transactionType->PosVendorName = "MenumateIndia";
        wcfResponse = smartConnectClient->SettlementCutover(transactionType);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
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
   try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        TransactionTypes *transactionType = new TransactionTypes();
        transactionType->PosBusinessName = "TCafe";
        transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
        transactionType->PosVendorName = "MenumateIndia";
        wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
        delete transactionType;
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::ReprintReceipt()
{
   try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        TransactionTypes *transactionType = new TransactionTypes();
        transactionType->PosBusinessName = "TCafe";
        transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
        transactionType->PosVendorName = "MenumateIndia";
        wcfResponse = smartConnectClient->ReprintLastReceipt(transactionType);
        delete transactionType;
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoLogon()
{
   try
   {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        TransactionTypes *transactionType = new TransactionTypes();
        transactionType->PosBusinessName = "TCafe";
        transactionType->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
        transactionType->PosVendorName = "MenumateIndia";
        transactionType->Transactiontype = "Acquirer.Logon";
        wcfResponse = smartConnectClient->Logon(transactionType);
        delete transactionType;
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
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
//---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoPairing()
{
    try
    {
        std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (Screen->ActiveForm));
        frmTouchKeyboard->MaxLength = 50;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = true;
        frmTouchKeyboard->MustHaveValue = true;
        frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().SmartConnectPairingCode;
        frmTouchKeyboard->Caption = "Please enter pairing code.";
        frmTouchKeyboard->lbeCaption->Font->Size = 18;
        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
            SmartConnectResponse *wcfResponse;
            CoInitialize(NULL);
            PairingTerminal  *pairingTerminal = new PairingTerminal();
            pairingTerminal->PosBusinessName = "TCafe";
            pairingTerminal->PosRegisterId = "7444ae07-dc63-e49c-33e3-59a7c108cc80";
            pairingTerminal->PosRegisterName = "Main Register5";
            pairingTerminal->PosVendorName = "MenumateIndia";
            pairingTerminal->PairingCode = TGlobalSettings::Instance().SmartConnectPairingCode;
            wcfResponse = smartConnectClient->Pairing(pairingTerminal);
            delete pairingTerminal;

            if(wcfResponse->ResponseSuccessful)
            {
                TGlobalSettings::Instance().SmartConnectPairingCode = frmTouchKeyboard->KeyboardText;
                Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
                TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
                dbTransaction.StartTransaction();
                TManagerVariable::Instance().SetDeviceStr(dbTransaction,vmSmartConnectPairingCode,TGlobalSettings::Instance().SmartConnectPairingCode);
                dbTransaction.Commit();
            }
            else
            {
                MessageBox("Invalid Pairing Code. Please make sure you entered the code correctly", "Error", MB_OK + MB_ICONERROR);
            }
        }
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}


