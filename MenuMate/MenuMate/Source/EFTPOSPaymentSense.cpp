//---------------------------------------------------------------------------


#pragma hdrstop

#include "EFTPOSPaymentSense.h"
#include "SelectZed.h"
#include "DeviceRealTerminal.h"
#include "Main.h"
#include "EftPosDialogs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEftPosPaymentSense::TEftPosPaymentSense()
{
    lastNotification = "";
    InitPaymentSenseClient();
    InitializeProperties();
}
//---------------------------------------------------------------------------
TEftPosPaymentSense::~TEftPosPaymentSense()
{
    delete authorizationDetails;
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
             strValue += "2. POS & EFTPOS terminal are connected to network.\r";
             strValue += "3. EFTPOS terminal is not busy.";
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
    authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL  + "/pac/terminals";
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
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
        frmDropDown->AddButton("Settlement  CutOver",&PrintZedReport);
       // frmDropDown->AddButton("Reprint ZED", &ReprintZedReport);
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
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/pac/terminals/" + TGlobalSettings::Instance().EftPosTerminalId;
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
               TransactionDataResponse *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
               {
                    case TransactionType_PURCHASE :
                        wcfResponse = DoTransaction(AmtPurchase, "SALE");
                        break;
                    case TransactionType_REFUND :
                        wcfResponse = DoTransaction(AmtPurchase, "REFUND");
                        break;
                    case TransactionType_INQUIRY :
                        wcfResponse = DoTransaction(1, "DUPLICATE");
                      break;
               }

               TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
               if(EftTrans != NULL)
               {
                   if(GetResponseStatus(TxnType, wcfResponse))
                   {
                      EftTrans->EventCompleted = true;
                      EftTrans->FinalAmount = wcfResponse->AmountTotal;
                      EftTrans->ResultText = "Eftpos Transaction Completed.";
                      EftTrans->Result = eAccepted;
                      EftTrans->CardType = wcfResponse->CardSchemeName;
                      EftTrans->TipAmount = wcfResponse->AmountGratuity;
                      EftTrans->CashOutAmount = wcfResponse->AmountCashBack;
                      //LoadEftPosReceipt(wcfResponse->ReceiptLines);  //id integration to receipt needs then uncomment it
                   }
                   else
                   {
                          EftTrans->EventCompleted = true;
                          EftTrans->Result = eDeclined;
                          EftTrans->ResultText = wcfResponse->TransactionResult;
                          if(!wcfResponse->TransactionResult.UpperCase().Compare("TIMED_OUT"))
                          {
                                UnicodeString strVal = "Communication with the PDQ has failed and payment may or may not have been taken.\r";
                                              strVal = strVal + "Please manually check if the transaction was successful on the PDQ.";
                                EftTrans->ResultText = strVal;
                                MessageBox(strVal,"EFTPOS Error",MB_OK);
                                EftTrans->FinalAmount = CurrToStr(AmtPurchase);
                                EftTrans->ResultText = "Eftpos Transaction Completed.";
                                EftTrans->Result = eAccepted;
                                EftTrans->CardType = wcfResponse->CardSchemeName;
                          }
                          if(wcfResponse->TransactionResult.UpperCase().Pos("CANCELLED") != 0 )
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
    try
    {
        CoInitialize(NULL);
        DoTransaction(1, "DUPLICATE");
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//----------------------------------------------------------------------------
void _fastcall TEftPosPaymentSense::ReprintZedReport()
{
    try
    {
        ShowPreviousZED();
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
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
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/pac/terminals";
        PACTerminalWrapper* terminalList = paymentSenseClient->GetAllCardTerminals(authorizationDetails);
        for(int index = 0; index < terminalList->Terminals.Length; index++)
        {
            PACTerminal *terminal = terminalList->Terminals[index];
            terminalIdList.push_back(terminal->TPI);
        }
        delete terminalList;
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
    return terminalIdList;
}
//------------------------------------------------------------------------------
TransactionDataResponse* TEftPosPaymentSense::DoTransaction(Currency amtPurchase, UnicodeString transactionType)
{
    TransactionDataResponse* response = new TransactionDataResponse();
    try
    {
        TransactionRequest * request  = new TransactionRequest();
        request->currency = CurrencyString;
        request->amount = double(amtPurchase);
        request->transactionType = transactionType;
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/pac/terminals/" + TGlobalSettings::Instance().EftPosTerminalId + "/transactions";
        PostRequestResponse* responseData = paymentSenseClient->DoTransaction(authorizationDetails, request);

        if(responseData->RequestId != NULL || responseData->RequestId.Trim() != "")
        {
            authorizationDetails->URL = authorizationDetails->URL + "/" + responseData->RequestId;
            response = paymentSenseClient->GetResponseForRequestedId(authorizationDetails);
            response = WaitAndGetResponse(response);
        }
        else
        {
            response->TransactionResult = "Unavailable terminal or no data available. Please Check that PDQ is available.";
        }
        delete request;
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
    return response;
}
//------------------------------------------------------------------------------------------------
bool TEftPosPaymentSense::GetResponseStatus(eEFTTransactionType TxnType, TransactionDataResponse* response)
{
    bool retValue = false;
    switch (TxnType)
    {
        case TransactionType_PURCHASE:
        case TransactionType_REFUND:
        case TransactionType_INQUIRY :
        {
            retValue = false;
            if(response)
            {
                if(!response->TransactionResult.UpperCase().Compare("SUCCESSFUL"))
                    retValue = true;
            }
        }
        break;
    }
    return retValue;
}
//------------------------------------------------------------------------------------------------
void TEftPosPaymentSense::LoadEftPosReceipt(ReceiptLines* receiptLines)
{
    try
    {
        LastEftPosReceipt->Clear();
        for(int i = 0; i < receiptLines->MerchantReceipt.Length; i++)
        {
            AnsiString Data = receiptLines->MerchantReceipt[i].Trim().t_str();
            LastEftPosReceipt->Add(Data);
            AddNewLine(Data);
        }
        LastEftPosReceipt->Add("\r");
        for(int i = 0; i < receiptLines->CustomerReceipt.Length; i++)
        {
            AnsiString Data = receiptLines->CustomerReceipt[i].Trim().t_str();
            LastEftPosReceipt->Add(Data);
            AddNewLine(Data);
        }
        LastEftPosReceipt->Add("\r");
    }
    catch(Exception &Ex)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//--------------------------------------------------------------------------------------------------------
bool TEftPosPaymentSense::IsCashOutSupported()
{
    return false;
}
//----------------------------------------------------------------------------
void TEftPosPaymentSense::AddNewLine(AnsiString data)
{
    if(data.Pos("HANDSET") || data.Pos("PPC") || data.Pos("CONTACTLESS") || data.Pos("TOTAL") || data.Pos("TXN") || data.Pos("SIGN BELOW")
            ||data.Pos("PLEASE RETAIN RECEIPT"))
                LastEftPosReceipt->Add("\r");
}
// ---------------------------------------------------------------------------
void _fastcall TEftPosPaymentSense::PrintZedReport()
{
    try
    {
        CoInitialize(NULL);
        PrintReports("END_OF_DAY");
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TEftPosPaymentSense::PrintReports(UnicodeString reportType)
{
    try
    {
        CoInitialize(NULL);
        Reports* report = new Reports();
        report->reportType =  reportType;
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/pac/terminals/" + TGlobalSettings::Instance().EftPosTerminalId + "/reports";
        paymentSenseClient->PrintReports(authorizationDetails, report);
        delete report;
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TEftPosPaymentSense::ShowPreviousZED()
{
    try
    {
        TfrmSelectZed *frmSelectZed = new TfrmSelectZed(frmMain, TDeviceRealTerminal::Instance().DBControl);
        frmSelectZed->Initialize(eEFTPOSZED);
		frmSelectZed->ShowModal();
        delete frmSelectZed;
    }
    catch(Exception &Ex)
    {
        MessageBox(Ex.Message,"Exception in ShowPreviousZED",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------
bool TEftPosPaymentSense::IsTransfactionFinished(TransactionDataResponse* response )
{
    bool retval = false;
    try
    {
        if(response->Notifications.Length)
        {
            if(response->Notifications[0].UpperCase().Compare(lastNotification))
            {
                TDeviceRealTerminal::Instance().ProcessingController.Pop();
                TMMProcessingState State(Screen->ActiveForm, response->Notifications[0], "Processing EftPos Transaction");
                                    TDeviceRealTerminal::Instance().ProcessingController.Push(State);
            }

            lastNotification = response->Notifications[0];

            if(!response->Notifications[0].UpperCase().Compare("TRANSACTION_FINISHED"))
            {
                retval = true;
                TDeviceRealTerminal::Instance().ProcessingController.Pop();
            }
            else if(!response->Notifications[0].UpperCase().Compare("SIGNATURE_VERIFICATION"))
            {
                TDeviceRealTerminal::Instance().ProcessingController.Pop();
                std::auto_ptr <TfrmEftPos> frmEftPos(TfrmEftPos::Create <TfrmEftPos> (Screen->ActiveForm));
                bool  isSignatureAccepted = frmEftPos->SignatureOk() == mrYes;
                SignatureRequest *signRequest = new SignatureRequest();
                signRequest->accepted = "false";

                if(isSignatureAccepted)
                    signRequest->accepted = "true";
                CoInitialize(NULL);
                paymentSenseClient->SignatureVerificationForRequestedId(authorizationDetails, signRequest);
                delete signRequest;
            }
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,Ex.Message);
        throw;
    }
    return retval;
}
//----------------------------------------------------------------------------------------------
TransactionDataResponse*  TEftPosPaymentSense::WaitAndGetResponse(TransactionDataResponse *response)
{
    try
    {
        lastNotification = "";
        while(!IsTransfactionFinished(response))
        {
            ::Sleep(1000);
            CoInitialize(NULL);
            response = paymentSenseClient->GetResponseForRequestedId(authorizationDetails);
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,Ex.Message);
        throw;
    }
    return response;
}
