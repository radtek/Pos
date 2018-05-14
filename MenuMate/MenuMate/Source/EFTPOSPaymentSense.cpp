//---------------------------------------------------------------------------


#pragma hdrstop

#include "EFTPOSPaymentSense.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEftPosPaymentSense::TEftPosPaymentSense()
{
    InitPaymentSenseClient();
    Initialise();
}
//---------------------------------------------------------------------------
TEftPosPaymentSense::~TEftPosPaymentSense()
{
}
//---------------------------------------------------------------------------
void TEftPosPaymentSense::InitPaymentSenseClient()
{
    bool useWSDL = false;
    AnsiString paymentSenseURL = "http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/";
    paymentSenseClient = GetIWCFServicePaymentSense(useWSDL, paymentSenseURL, NULL );
}
//---------------------------------------------------------------------------
void TEftPosPaymentSense::Initialise()
{
    TEftPos::Initialise();
    if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
//        if(Enabled)
//            GetAllTerminals();
        Enabled = true;

    }
    else
    {
        Enabled = false;
    }
}
//----------------------------------------------------------------------------
void TEftPosPaymentSense::InitializeProperties()
{
//    authorizationDetails = new AuthorizationDetails();
//    authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL;
//    authorizationDetails->UserName = TGlobalSettings::Instance().EFTPosDeviceID;
//    authorizationDetails->Password = TGlobalSettings::Instance().EFTPosAPIKey;
}
//---------------------------------------------------------------------
void TEftPosPaymentSense::DoControlPannel()
{
    if(!Enabled) return;
	try
	{
		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		//frmDropDown->AddButton("EFTPOS Logon",&DoLogon);
		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Settlement  CutOver",&DoSettlementCutover);
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
        frmDropDown->AddButton("Terminal List",&GetAllTerminals);
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
//------------------------------------------------------------------------------
void TEftPosPaymentSense::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase, Currency AmtCash, UnicodeString TxnRef,
                   ePANSource PANSource, UnicodeString CardSwipe, int ExpiryMonth, int ExpiryYear)
{
   if(Enabled)
   {
        try
        {
              // SmartConnectResponse *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
               {
                    case TransactionType_PURCHASE :
                       // wcfResponse = smartConnectClient->Purchase(transactionType, AmtPurchase);
                        break;
                    case TransactionType_CASH_ADVANCE :
                     //   wcfResponse = smartConnectClient->CashOutOnly(transactionType, AmtCash);
                        break;
                    case TransactionType_PURCHASE_PLUS_CASH :
                     //    wcfResponse = smartConnectClient->PurchasePlusCash(transactionType, AmtPurchase, AmtCash);
                        break;
                    case TransactionType_REFUND :
                     //    wcfResponse = smartConnectClient->Refund(transactionType, AmtPurchase);
                        break;
                    case TransactionType_INQUIRY :
                      //   wcfResponse = smartConnectClient->GetTransactionResult(transactionType);
                      break;
                }
//                  if(wcfResponse->ResponseSuccessful)
//                   {
//                      AcquirerRefSmartConnect = wcfResponse->Data->AcquirerRef;
//                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
//                      if(EftTrans != NULL)
//                       {
//                          EftTrans->EventCompleted = true;
//                          EftTrans->FinalAmount = wcfResponse->Data->AmountTotal;
//                          EftTrans->ResultText = "Eftpos Transaction Completed.";
//                          EftTrans->Result = eAccepted;
//                          EftTrans->CardType = wcfResponse->Data->CardType;
//                          EftTrans->TipAmount = wcfResponse->Data->AmountTip;
//                          EftTrans->SurchargeAmount = wcfResponse->Data->AmountSurcharge;
//                          LoadEftPosReceipt(wcfResponse->Data->Receipt) ;
//                        }
//                   }
//                  else
//                   {
//                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
//                      if(EftTrans != NULL)
//                       {
//                          EftTrans->EventCompleted = true;
//                          EftTrans->Result = eDeclined;
//                          EftTrans->ResultText = wcfResponse->Data->TransactionResult;
//                          if(wcfResponse->Data->TransactionResult.UpperCase().Pos("CANCELLED") != 0)
//                            EftTrans->TimeOut = true;
//                       }
//                   }
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
// ---------------------------------------------------------------------------
void TEftPosPaymentSense::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
                   AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
 //
}
// ---------------------------------------------------------------------------
bool TEftPosPaymentSense::CheckChequeBranch(AnsiString Branch)
{
    return false;
}
// ---------------------------------------------------------------------------
bool TEftPosPaymentSense::CheckChequeSerial(AnsiString ChequeSerial)
{
    return false;
}
// ---------------------------------------------------------------------------
bool TEftPosPaymentSense::CheckChequeAccount(AnsiString ChequeAccount)
{
    return false;
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosPaymentSense::DoSettlementEnquiry()
{
    try
    {

    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftPosPaymentSense::DoSettlementCutover()
{
    try
    {

    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void _fastcall TEftPosPaymentSense::ReprintReceipt()
{
}
// ---------------------------------------------------------------------------
AnsiString TEftPosPaymentSense::GetRefNumber()
{
  return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}
// ---------------------------------------------------------------------------
void _fastcall TEftPosPaymentSense::GetAllTerminals()
{
//    PACTerminalWrapper terminalList;
    CoInitialize(NULL);
    MessageBox("1","1",MB_OK);
    authorizationDetails = new AuthorizationDetails();
    authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL;
    MessageBox("1.1","1",MB_OK);
    authorizationDetails->UserName = TGlobalSettings::Instance().EFTPosDeviceID;
    authorizationDetails->Password = TGlobalSettings::Instance().EFTPosAPIKey;
    MessageBox(authorizationDetails->URL,authorizationDetails->URL,MB_OK);
    paymentSenseClient->GetAllCardTerminals(authorizationDetails);
    MessageBox("2","2",MB_OK);
}

