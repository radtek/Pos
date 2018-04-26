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
    InitializeProperties();
}
// ---------------------------------------------------------------------------
TEftPosSmartConnect::~TEftPosSmartConnect()
{
    delete transactionType;
    delete pairingTerminal;
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
               CoInitialize(NULL);

               switch (TxnType)
               {
                    case TransactionType_PURCHASE :
                        wcfResponse = smartConnectClient->Purchase(transactionType, AmtPurchase);
                        break;
                    case TransactionType_CASH_ADVANCE :
                        wcfResponse = smartConnectClient->CashOutOnly(transactionType, AmtCash);
                        break;
                    case TransactionType_PURCHASE_PLUS_CASH :
                         wcfResponse = smartConnectClient->PurchasePlusCash(transactionType, AmtPurchase, AmtCash);
                        break;
                    case TransactionType_REFUND :
                         wcfResponse = smartConnectClient->Refund(transactionType, AmtPurchase);
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
                  if(wcfResponse->ResponseSuccessful)
                   {    
                      AcquirerRefSmartConnect = wcfResponse->Data->AcquirerRef;
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          EftTrans->FinalAmount = wcfResponse->Data->AmountTotal;
                          EftTrans->ResultText = "Eftpos Transaction Completed.";
                          EftTrans->Result = eAccepted;
                          EftTrans->CardType = wcfResponse->Data->CardType;
                          SmartPayReceipt(wcfResponse) ;
                        }
                   }
                  else
                   {                      
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                       {
                          EftTrans->EventCompleted = true;
                          EftTrans->Result = eDeclined;
                          EftTrans->ResultText = wcfResponse->Data->TransactionResult;
                          if(wcfResponse->Data->TransactionResult.UpperCase().Pos("CANCELLED") != 0)
                            EftTrans->TimeOut = true;
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
//----------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoSettlementCutover()
{
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
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
    return false;
}
// ---------------------------------------------------------------------------
bool TEftPosSmartConnect::CheckChequeSerial(AnsiString ChequeSerial)
{
    return false;
}
// ---------------------------------------------------------------------------
bool TEftPosSmartConnect::CheckChequeAccount(AnsiString ChequeAccount)
{
    return false;
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosSmartConnect::DoSettlementEnquiry()
{
   try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = smartConnectClient->SettlementInquiry(transactionType);
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
        wcfResponse = smartConnectClient->ReprintLastReceipt(transactionType);
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
        wcfResponse = smartConnectClient->Logon(transactionType);
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
            pairingTerminal->PairingCode = frmTouchKeyboard->KeyboardText;
            wcfResponse = smartConnectClient->Pairing(pairingTerminal);

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
//------------------------------------------------------------------------------------------------
void TEftPosSmartConnect::InitializeProperties()
{
    transactionType = new TransactionTypes();
    Database::TDBControl DBControl;
    TRegInfo regInfo(DBControl);
    transactionType->PosRegisterId = regInfo.GetMACAddress();

    pairingTerminal = new PairingTerminal();
    pairingTerminal->PosRegisterId = transactionType->PosRegisterId;
    pairingTerminal->PosRegisterName = TDeviceRealTerminal::Instance().ID.Name;

    bool Registered = false;
    UnicodeString pRegisteredName = "";
    TDeviceRealTerminal::Instance().Registered(&Registered,&pRegisteredName);
    if(Registered)
    {
        transactionType->PosBusinessName  = pRegisteredName;
        pairingTerminal->PosBusinessName = pRegisteredName;
    }
    else
    {
        transactionType->PosBusinessName =  "Menumate Demo";
        pairingTerminal->PosBusinessName =  "Menumate Demo";
    }
    transactionType->PosVendorName = "Menumate Ltd.";
     pairingTerminal->PosVendorName = "Menumate Ltd.";
}
//---------------------------------------------------------------------
void TEftPosSmartConnect::AuthoriseTransaction()
{
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        transactionType->Transactiontype = "Card.Authorise";
        //wcfResponse = smartConnectClient->Authorise(transactionType, AmtPurchase, TxnRef);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//---------------------------------------------------------------------
void TEftPosSmartConnect::FinalizeTransaction()
{
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        transactionType->Transactiontype = "Card.Finalise";
        //wcfResponse = smartConnectClient->Finalise(transactionType, AmtPurchase, TxnRef);   //todo add cahout amount also
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//---------------------------------------------------------------------
void TEftPosSmartConnect::GetTransResult()
{
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        transactionType->Transactiontype = "Journal.GetTransResult";
        //wcfResponse = smartConnectClient->GetTransactionResult(transactionType);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//---------------------------------------------------------------------
void TEftPosSmartConnect::GetStatus()
{
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        transactionType->Transactiontype = "Terminal.GetStatus";
//            wcfResponse = smartConnectClient->GetTerminalStatus(transactionType);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//---------------------------------------------------------------------
void TEftPosSmartConnect::ReadCard()
{
    try
    {
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);
        transactionType->Transactiontype = "Terminal.ReadCard";
//        wcfResponse = smartConnectClient->TerminalReadCard(transactionType);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//-----------------------------------------------------------------------
bool TEftPosSmartConnect::DoQRCodeTransaction(TPayment &Payment)
{
    try
    {
        bool retval = false;
        SmartConnectResponse *wcfResponse;
        CoInitialize(NULL);

        if(Payment.GetPayTendered() > 0)
        {
            wcfResponse = smartConnectClient->MerchantPurchaseWithQRCode(transactionType, Payment.GetPayTendered());
        }
        else
        {
            wcfResponse = smartConnectClient->QRCodeRefund(transactionType, Payment.GetPayTendered());
        }

        if(wcfResponse->ResponseSuccessful)
        {
            retval = true;
        }
        else
        {
            if(MessageBox("Transaction Cancelled/Timed-Out.", "EFTPOS Response",MB_RETRYCANCEL) == IDRETRY)
                DoQRCodeTransaction(Payment);
        }
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//-----------------------------------------------------------------------
void TEftPosSmartConnect::SmartPayReceipt(SmartConnectResponse *wcfResponse)
{
    try
    {

         LastEftPosReceipt->Clear();
         AnsiString EftReceipt = wcfResponse->Data->Receipt;
         int width =  25;

          for( int i = 1 ; i < EftReceipt.Length();)
          {
                AnsiString Data = EftReceipt.SubString(i,EftReceipt.Length());//.Trim();
                int width =  Data.Pos("\n");
                Data = Data.SubString(1,width);
                if(Data == "")
                    break;
                LastEftPosReceipt->Add(Data);
                i += width;
          }

    }

    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }

}

