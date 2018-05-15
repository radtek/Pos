//---------------------------------------------------------------------------


#pragma hdrstop

#include "EFTPOSPaymentSense.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEftPosPaymentSense::TEftPosPaymentSense()
{
    InitPaymentSenseClient();
    InitializeProperties();
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
         if(PingTerminal(TransactionType_NONE))
         {
            Enabled = true;
         }
         else
         {
             UnicodeString strValue = "Payment Sense EFTPOS Terminal was not available.\r";
             strValue += "Please ensure below mentioned things:-.\r";
             strValue += "1. Details are correct.\r";
             strValue += "2. POS & EFTPOS terminal are connected to network.";
             MessageBox(strValue,"Information",MB_OK+MB_ICONINFORMATION);
             Enabled = false;
         }
    }
    else
    {
        Enabled = false;
    }
}
//----------------------------------------------------------------------------
void TEftPosPaymentSense::InitializeProperties()
{
    authorizationDetails = new AuthorizationDetails();
    authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL;
    authorizationDetails->UserName = TGlobalSettings::Instance().EFTPosDeviceID;
    authorizationDetails->Password = TGlobalSettings::Instance().EFTPosAPIKey;
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
     //   frmDropDown->AddButton("Terminal List",&GetAllTerminals);
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
bool  TEftPosPaymentSense::PingTerminal(eEFTTransactionType TxnType)
{
    bool retVal = false;
    try
    {
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/" + TGlobalSettings::Instance().EftPosTerminalId;
        CoInitialize(NULL);
        PACTerminal *response = paymentSenseClient->PingTerminal(authorizationDetails);
        if(response != NULL)
        {
            if(response->Status.UpperCase().Pos("AVAILABLE") != 0)
                retVal = true;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return retVal;
}
//-----------------------------------------------------------------------------------------
void TEftPosPaymentSense::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase, Currency AmtCash, UnicodeString TxnRef,
                   ePANSource PANSource, UnicodeString CardSwipe, int ExpiryMonth, int ExpiryYear)
{
   if(Enabled)
   {
        try
        {
               TransactionData *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
               {
                    case TransactionType_PURCHASE :
                        wcfResponse = DoPurchase(AmtPurchase);
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
std::vector<AnsiString> TEftPosPaymentSense::GetAllTerminals()
{
    std::vector<AnsiString> terminalIdList;
    try
    {
        CoInitialize(NULL);
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL;
        // = new PACTerminalWrapper();
//        authorizationDetails = new AuthorizationDetails();
//        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL;
//        authorizationDetails->UserName = TGlobalSettings::Instance().EFTPosDeviceID;
//        authorizationDetails->Password = TGlobalSettings::Instance().EFTPosAPIKey;
        MessageBox(authorizationDetails->URL,authorizationDetails->URL,MB_OK);
        PACTerminalWrapper* terminalList = paymentSenseClient->GetAllCardTerminals(authorizationDetails);
        //MessageBox("2","2",MB_OK);
       // ArrayOfPACTerminal terminalArray;
        for(int index = 0; index < terminalList->Terminals.Length; index++)
        {
            PACTerminal *terminal = terminalList->Terminals[index];
            terminalIdList.push_back(terminal->TPI);
            MessageBox(terminal->TPI,"terminal->TPI",MB_OK);
        }
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
    return terminalIdList;
}
//------------------------------------------------------------------------------
TransactionData* TEftPosPaymentSense::DoPurchase(Currency amtPurchase)
{
    TransactionData* responseData = new TransactionData();
    try
    {
        TransactionRequest * request  = new TransactionRequest();
        request->Currency = "GBP"; //CurrencyString;
        request->amount = double(amtPurchase*100);
        request->TransactionType = "SALE";
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/" + TGlobalSettings::Instance().EftPosTerminalId + "/transactions";
        responseData = paymentSenseClient->DoTransaction(authorizationDetails, request);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
    return responseData;
}
