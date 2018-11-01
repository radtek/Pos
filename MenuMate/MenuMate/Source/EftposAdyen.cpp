//---------------------------------------------------------------------------


#pragma hdrstop

#include "EftposAdyen.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "DBAdyen.h"
#include "Printout.h"
#include "Comms.h"
#define BasicVar                                "Basic"
#define StandardVar                             "Standard"
#define ExtendedVar                             "Extended"
#define RequestVar                              "Request"
#define ResponseVar                             "Response"
#define NotificationVar                         "Notification"
#define AbortVar                                "Abort"
#define AdminVar                                "Admin"
#define BalanceInquiryVar                       "BalanceInquiry"
#define BatchVar                                "Batch"
#define CardAcquisitionVar                      "CardAcquisition"
#define CardReaderAPDUVar                       "CardReaderAPDU"
#define CardReaderInitVar                       "CardReaderInit"
#define CardReaderPowerOffVar                   "CardReaderPowerOff"
#define DiagnosisVar                            "Diagnosis"
#define DisplayVar                              "Display"
#define EnableServiceVar                        "EnableService"
#define EventVar                                "Event"
#define GetTotalsVar                            "GetTotals"
#define InputVar                                "Input"
#define InputUpdateVar                          "InputUpdate"
#define LoginVar                                "Login"
#define LogoutVar                               "Logout"
#define LoyaltyVar                              "Loyalty"
#define PaymentVar                              "Payment"
#define PINVar                                  "PIN"
#define PrintVar                                "Print"
#define ReconciliationVar                       "Reconciliation"
#define ReversalVar                             "Reversal"
#define SoundVar                                "Sound"
#define StoredValueVar                          "StoredValue"
#define TransactionStatusVar                    "TransactionStatus"
#define TransmitVar                             "Transmit"
#define ServiceVar                              "Service"
#define DeviceVar                               "Device"
#define EventVar                                "Event"
#define NormalVar                               "Normal"
#define RefundVar                               "Refund"
#define OneTimeReservationVar                   "OneTimeReservation"
#define FirstReservationVar                     "FirstReservation"
#define UpdateReservationVar                    "UpdateReservation"
#define CompletionVar                           "Completion"
#define CashAdvanceVar                          "CashAdvance"
#define CashDepositVar                          "CashDeposit"
#define RecurringVar                            "Recurring"
#define InstalmentVar                           "Instalment"
#define IssuerInstalmentVar                     "IssuerInstalment"
#define PaidOut                                 "PaidOut"
#define CustCancelVar                           "CustCancel"
#define MerchantCancelVar                       "MerchantCancel"
#define MalfunctionVar                          "Malfunction"
#define Unable2ComplVar                         "Unable2Compl"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TEftposAdyen::TEftposAdyen()
{
    initAdyenClient();
}
// ---------------------------------------------------------------------------
TEftposAdyen::~TEftposAdyen()
{
//
}
// ---------------------------------------------------------------------------
void TEftposAdyen::initAdyenClient()
{
    bool useWSDL = false;
    AnsiString adyenURL = "http://localhost:8745/MenumateServices/AdyenService";
    AdyenClient = GetIAdyenIntegrationWebService(useWSDL, adyenURL, NULL );
}
// ---------------------------------------------------------------------------
void TEftposAdyen::Initialise()
{
    TEftPos::Initialise();
    if(TGlobalSettings::Instance().EnableEftPosAdyen)
      {
         // Gets the status of machine if it is available or not
         if(PingTerminal(TransactionType_NONE))
         {
            Enabled = true;
         }
         else
         {
             UnicodeString strValue = "Adyen EFTPOS Terminal was not available.\r";
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
bool  TEftposAdyen::PingTerminal(eEFTTransactionType TxnType)
{
    bool retVal = false;
    try
    {
        Envelop *envelop = GetPingTerminalEnvelop();
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        SaleToPOIResponse *response = AdyenClient->PingTerminal(envelop,details);
        if(response != NULL)
        {
            if(response->DiagnosisResponse->Response->Result.Pos("Success") != 0)
                retVal = true;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    //return retVal ;
    return retVal;
}
// ---------------------------------------------------------------------------
void TEftposAdyen::DoControlPannel()
{
 if(!Enabled) return;
	try
	{
		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("EFTPOS Login",&DoLogon);
        frmDropDown->AddButton("EFTPOS Log Out",&DoLogOut);
		if(frmDropDown->ShowModal() == mrOk)
		{
//			frmDropDown->FunctionToCall();
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
           if(Enabled)
           {
               SaleToPOIResponse *response;
               CoInitialize(NULL);
               switch (TxnType)
                  {
                    case TransactionType_PURCHASE :
                    {
                        TDBAdyen::SetRefundTransaction(false);
                        response = TriggerSaleTransaction(AmtPurchase,TxnRef);
                    }
                    break;
                    case TransactionType_REFUND :
                    {
                        TDBAdyen::SetRefundTransaction(true);
                        response = TriggerRefundTransaction(AmtPurchase,TxnRef);
                    }
                    break;
                    case TransactionType_INQUIRY :
                    {
                        response = TriggerEnquiry(AmtPurchase,TxnRef);
                    }
                    break;
                  }
                  if(GetResponseStatus(TxnType, response))
                  {
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(TxnType != TransactionType_INQUIRY)
                      {
                          AcquirerRefAdyen = response->PaymentResponse->PaymentResult->PaymentAcquirerData->AcquirerTransactionID->TransactionID;
                          if(EftTrans != NULL)
                          {
                              EftTrans->EventCompleted = true;
                              EftTrans->ResultText = "Eftpos Transaction Completed.";
                              EftTrans->Result = eAccepted;
                              EftTrans->TimeOut = true;
                              EftTrans->FinalAmount = response->PaymentResponse->PaymentResult->AmountsResp->AuthorizedAmount;
                              EftTrans->TipAmount = response->PaymentResponse->PaymentResult->AmountsResp->TipAmount;
                              EftTrans->CardType = response->PaymentResponse->PaymentResult->PaymentInstrumentData->CardData->PaymentBrand;
                              EftTrans->EftposTransactionID = response->PaymentResponse->PaymentResult->PaymentAcquirerData->AcquirerTransactionID->TransactionID;
                              EftTrans->MerchantAccount = response->PaymentResponse->PaymentResult->PaymentAcquirerData->MerchantID;

                              // Receipt index 1 corresponds to Customer receipt & index 2 corresponds to Cashier receipt
                              if(TGlobalSettings::Instance().PrintCardHolderReceipt || TGlobalSettings::Instance().EnableEftPosPreAuthorisation)
                                    LoadEftPosReceipt(response->PaymentResponse->PaymentReceiptUsable1);

                              if(TGlobalSettings::Instance().PrintMerchantReceipt || TGlobalSettings::Instance().EnableEftPosPreAuthorisation)
                                    LoadEftPosReceiptSecond(response->PaymentResponse->PaymentReceiptUsable2);
                           }
                      }
                      else
                      {
                          if(response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody != NULL)
                          {
                             if(response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->Response->Result.Pos("Success") != 0)
                             {
                                  AcquirerRefAdyen = response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->PaymentResult->PaymentAcquirerData->AcquirerTransactionID->TransactionID;
                                  if(EftTrans != NULL)
                                  {
                                      EftTrans->EventCompleted = true;
                                      EftTrans->ResultText = "Eftpos Transaction Completed.";
                                      EftTrans->Result = eAccepted;
                                      EftTrans->TimeOut = true;
                                      EftTrans->CardType = response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->PaymentResult->PaymentInstrumentData->CardData->PaymentBrand;
                                      EftTrans->FinalAmount =
                                            response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->PaymentResult->AmountsResp->AuthorizedAmount;
                                      EftTrans->TipAmount =
                                            response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->PaymentResult->AmountsResp->TipAmount;

                                      EftTrans->EftposTransactionID = response->PaymentResponse->PaymentResult->PaymentAcquirerData->AcquirerTransactionID->TransactionID;
                                      EftTrans->MerchantAccount = response->PaymentResponse->PaymentResult->PaymentAcquirerData->MerchantID;

                                      // Receipt index 1 corresponds to Customer receipt & index 2 corresponds to Cashier receipt
                                      if(TGlobalSettings::Instance().PrintCardHolderReceipt || TGlobalSettings::Instance().EnableEftPosPreAuthorisation)
                                            LoadEftPosReceipt(response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->PaymentReceiptUsable1);

                                      if(TGlobalSettings::Instance().PrintMerchantReceipt || TGlobalSettings::Instance().EnableEftPosPreAuthorisation)
                                            LoadEftPosReceiptSecond(response->TransactionStatusResponse->RepeatedMessageResponse->RepeatedResponseMessageBody->PaymentResponse->PaymentReceiptUsable2);
                                   }
                              }
                              else
                              {
                                      EftTrans->EventCompleted = true;
                                      EftTrans->ResultText = "Eftpos Transaction Completed.";
                                      EftTrans->Result = eAccepted;
                                      EftTrans->TimeOut = "2";
                                      EftTrans->FinalAmount = "0";
                                      EftTrans->TipAmount = "0" ;
                              }
                          }
                      }
                  }
                  else
                  {
//                      MessageBox(4,2,MB_OK);
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                      {
                          EftTrans->EventCompleted = true;
                          EftTrans->Result = eDeclined;
                          EftTrans->ResultText = "Manual Error";
                          EftTrans->TimeOut = true;
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
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftposAdyen::DoLogon()
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetLoginLogOutEnvelop(eAdyenLoginRequest);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->LoginToSystem(envelop,details);
        if(response != NULL)
        {
            if(response->LoginResponse->Response->Result.Pos("Success") != 0)
            {
                int i = 0;
            }
            else
            {
            }
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void __fastcall TEftposAdyen::DoLogOut()
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetLoginLogOutEnvelop(eAdyenLogOutRequest);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->LogoutSystem(envelop,details);
        if(response != NULL)
        {
            if(response->LogoutResponse->Response->Result.Pos("Success") != 0)
            {
            }
            else
            {
            }
        }
        else
        {
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
// ---------------------------------------------------------------------------
Envelop* TEftposAdyen::GetLoginLogOutEnvelop(AdyenRequestType requestType)
{
    Envelop* envelop = new Envelop();
    try
    {
        envelop->SaleToPOIRequest = new SaleToPOIRequest();

        envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenLoginRequest, TDBAdyen::GetServiceID());

         if(requestType == eAdyenLoginRequest)
         {
            envelop->SaleToPOIRequest->LoginRequest = new LoginRequest();
            envelop->SaleToPOIRequest->LoginRequest->DateTime = "";
            envelop->SaleToPOIRequest->LoginRequest->SaleSoftware = new  SaleSoftware();
            envelop->SaleToPOIRequest->LoginRequest->SaleSoftware->ManufacturerID = "Menumate Limited";
            envelop->SaleToPOIRequest->LoginRequest->SaleSoftware->ApplicationName = "Menumate POS";
            envelop->SaleToPOIRequest->LoginRequest->SaleSoftware->SoftwareVersion = GetFileVersion();
            envelop->SaleToPOIRequest->LoginRequest->SaleSoftware->CertificationCode = "";
            envelop->SaleToPOIRequest->LoginRequest->SaleTerminalData = new SaleTerminalData();
            envelop->SaleToPOIRequest->LoginRequest->SaleTerminalData->TotalsGroupID = TDBAdyen::GetCompanyName();
            envelop->SaleToPOIRequest->LoginRequest->OperatorID = TDeviceRealTerminal::Instance().ID.Name;
            envelop->SaleToPOIRequest->LoginRequest->OperatorLanguage = "en";
         }
         else if(requestType == eAdyenLogOutRequest)
         {
            envelop->SaleToPOIRequest->LogoutRequest = new LogoutRequest();
         }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return envelop;
}
// ---------------------------------------------------------------------------
AnsiString TEftposAdyen::GetRefNumber()
{
  return TDBAdyen::GetServiceID();
}
//----------------------------------------------------------------------------
SaleToPOIResponse* TEftposAdyen::TriggerSaleTransaction(Currency AmtPurchase, UnicodeString TxnRef)
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetSaleEnvelop(AmtPurchase, eAdyenNormalSale, TxnRef);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->Purchase(envelop,details);
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return response;
}
//----------------------------------------------------------------------------
SaleToPOIResponse* TEftposAdyen::TriggerRefundTransaction(Currency AmtPurchase, UnicodeString TxnRef)
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetSaleEnvelop(AmtPurchase, eAdyenRefundSale, TxnRef);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->Refund(envelop,details);
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return response;
}
//----------------------------------------------------------------------------
SaleToPOIResponse* TEftposAdyen::TriggerEnquiry(Currency AmtPurchase, UnicodeString TxnRef)
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetEnquiryEnvelop(AmtPurchase, eAdyenTransactionStatus, TxnRef);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->GetTransactionStatus(envelop,details);
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return response;
}
//----------------------------------------------------------------------------
Envelop* TEftposAdyen::GetEnquiryEnvelop(Currency AmtPurchase, AdyenRequestType requestType, UnicodeString TxnRef)
{
    Envelop* envelop = new Envelop();
    try
    {
        envelop->SaleToPOIRequest = new SaleToPOIRequest();

        envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenTransactionStatus,TDBAdyen::GetServiceID());

        envelop->SaleToPOIRequest->TransactionStatusRequest = new TransactionStatusRequest();
        envelop->SaleToPOIRequest->TransactionStatusRequest->ReceiptReprintFlag = true;
        envelop->SaleToPOIRequest->TransactionStatusRequest->MessageReference = new MessageReference();
        if(!TGlobalSettings::Instance().AdyenRecoveryTransactionIsRefund)
            envelop->SaleToPOIRequest->TransactionStatusRequest->MessageReference->MessageCategory = PaymentVar;
        else
            envelop->SaleToPOIRequest->TransactionStatusRequest->MessageReference->MessageCategory = ReversalVar;
        envelop->SaleToPOIRequest->TransactionStatusRequest->MessageReference->SaleID = TDeviceRealTerminal::Instance().ID.Name;
        envelop->SaleToPOIRequest->TransactionStatusRequest->MessageReference->ServiceID = TxnRef;
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return envelop;
}
//----------------------------------------------------------------------------
Envelop* TEftposAdyen::GetSaleEnvelop(Currency AmtPurchase, AdyenRequestType requestType, UnicodeString TxnRef)
{
    Envelop* envelop = new Envelop();
    try
    {
        envelop->SaleToPOIRequest = new SaleToPOIRequest();

        envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenNormalSale,TxnRef);

        UnicodeString transactionId = TDBAdyen::GetTransactionID();
        UnicodeString saleRefId     = "SaleRef" + transactionId;
        double amount = (double)AmtPurchase;
        envelop->SaleToPOIRequest->PaymentRequest = new PaymentRequest();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData = new SaleData();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID = new SaleTransactionID();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID->TransactionID = transactionId;
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID->TimeStamp = "";
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleReferenceID = saleRefId;
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->TokenRequestedType = "Customer";

        if(requestType == eAdyenNormalSale && TGlobalSettings::Instance().EnableDPSTipping )
        {
            envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleToAcquirerData = "tenderOption=AskGratuity&authorisationType=PreAuth";
        }
        else
        {
            envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleToAcquirerData = "authorisationType=PreAuth";
        }

        envelop->SaleToPOIRequest->PaymentRequest->PaymentTransaction = new PaymentTransaction();
        envelop->SaleToPOIRequest->PaymentRequest->PaymentTransaction->AmountsReq = new AmountsReq();
        envelop->SaleToPOIRequest->PaymentRequest->PaymentTransaction->AmountsReq->Currency = CurrencyString;
        envelop->SaleToPOIRequest->PaymentRequest->PaymentTransaction->AmountsReq->RequestedAmount = amount;

        if(requestType == eAdyenRefundSale)
        {
            envelop->SaleToPOIRequest->PaymentRequest->PaymentData = new PaymentData();
            envelop->SaleToPOIRequest->PaymentRequest->PaymentData->PaymentType = RefundVar;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return envelop;
}
//----------------------------------------------------------------------------
MessageHeader* TEftposAdyen::GetMessageHeader(AdyenRequestType requestType, UnicodeString TxnRef)
{
    MessageHeader* messageHeader = new MessageHeader();
    if(requestType == eAdyenPingRequest)
        messageHeader->MessageCategory = DiagnosisVar;
    else if(requestType == eAdyenNormalSale)
        messageHeader->MessageCategory = PaymentVar;
    else if(requestType == eAdyenLoginRequest)
        messageHeader->MessageCategory = LoginVar;
    else if(requestType == eAdyenLogOutRequest)
        messageHeader->MessageCategory = LogoutVar;
    else if(requestType == eAdyenTransactionStatus)
        messageHeader->MessageCategory = TransactionStatusVar;

    messageHeader->MessageClass    = ServiceVar;
    messageHeader->MessageType     = RequestVar;
    messageHeader->POIID           = TGlobalSettings::Instance().EFTPosDeviceID;
    messageHeader->ProtocolVersion = "3.0";
    messageHeader->SaleID          = TDeviceRealTerminal::Instance().ID.Name;
    messageHeader->ServiceID       = TxnRef;//TDBAdyen::GetServiceID();
    return messageHeader;
}
//----------------------------------------------------------------------------
Envelop* TEftposAdyen::GetPingTerminalEnvelop()
{
    Envelop* envelop = new Envelop();
    envelop->SaleToPOIRequest = new SaleToPOIRequest();
    envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenPingRequest, TDBAdyen::GetServiceID());

    envelop->SaleToPOIRequest->DiagnosisRequest = new DiagnosisRequest();
    return envelop;
}
//----------------------------------------------------------------------------
void TEftposAdyen::LoadEftPosReceipt(ArrayOfstring receipt)
{
    try
    {
        LastEftPosReceipt->Clear();
        for(int i = 0; i < receipt.Length; i++)
        {
            AnsiString Data = receipt[i].t_str();
            LastEftPosReceipt->Add(Data);
        }
        if(LastEftPosReceipt->Count && TGlobalSettings::Instance().DuplicateEftPosReceipt)
        {
            PrintEFTPOSReceipt(LastEftPosReceipt);
        }
    }
    catch(Exception &Ex)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void TEftposAdyen::LoadEftPosReceiptSecond(ArrayOfstring receipt)
{
    try
    {
        SecondEftPosReceipt->Clear();
        for(int i = 0; i < receipt.Length; i++)
        {
            AnsiString Data = receipt[i].t_str();
            SecondEftPosReceipt->Add(Data);
        }
        if(SecondEftPosReceipt->Count)
        {
            PrintEFTPOSReceipt(SecondEftPosReceipt);
        }
    }
    catch(Exception &Ex)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
bool TEftposAdyen::GetResponseStatus(eEFTTransactionType TxnType, SaleToPOIResponse *response)
{
    bool retValue = false;
    switch (TxnType)
    {
        case TransactionType_PURCHASE :
        {
            if(response)
            {
                if(response->PaymentResponse)
                {
                    if(response->PaymentResponse->Response->Result.Pos("Success") != 0)
                        retValue = true;
                    else
                        retValue = false;
                }
            }
            else
                retValue = false;
        }
        break;
        case TransactionType_REFUND :
        {
            if(response)
            {
                if(response->PaymentResponse)
                {
                    if(response->PaymentResponse->Response->Result.Pos("Success") != 0)
                        retValue = true;
                    else
                        retValue = false;
                }
            }
            else
                retValue = false;
        }
        break;
        case TransactionType_INQUIRY :
        {
            if(response)
            {
                if(response->TransactionStatusResponse->Response->Result.Pos("Success") != 0)
                {
                    retValue = true;
                }
                else
                {
                    retValue = false;
                }
            }
            else
            {
                retValue = false;
            }
        }
        break;
    }
    return retValue;
}
//----------------------------------------------------------------------------
ResourceDetails* TEftposAdyen::GetResourceDetails()
{
    ResourceDetails *details = new ResourceDetails();
    details->APIKey = TGlobalSettings::Instance().EFTPosAPIKey;
    details->URL = TGlobalSettings::Instance().EFTPosURL;
    return details;
}
//----------------------------------------------------------------------------
bool TEftposAdyen::IsCashOutSupported()
{
    return false;
}
//----------------------------------------------------------------------------
void TEftposAdyen::PrintEFTPOSReceipt(std::auto_ptr<TStringList> &eftPosReceipt)
{
    if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		TManagerLogs::Instance().Add("NA",EFTPOSLOG,"No Recipt Printer Configured for EFTPOS output.");
		TManagerLogs::Instance().Add("NA",ERRORLOG,"No Recipt Printer Configured for EFTPOS output.");
		return;
	}

	TPrintout *Printout = new TPrintout;
	Printout->Printer = TComms::Instance().ReceiptPrinter;

   try
   {
        Printout->PrintFormat->Line->FontInfo.Reset();
        Printout->PrintFormat->Line->ColCount = 1;
        Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;

        Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;

        Printout->PrintFormat->Line->Columns[0]->Text = Printout->PrintFormat->DocumentName;
        Printout->PrintFormat->AddLine();
        Printout->PrintFormat->NewLine();

        for (int i = 0; i < eftPosReceipt->Count; i++)
        {
            Printout->PrintFormat->Line->Columns[0]->Text = eftPosReceipt->Strings[i];
            Printout->PrintFormat->AddLine();
        }

        Printout->PrintFormat->PartialCut();
        Printout->Print();
		delete Printout;
   }
   catch (Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
		delete Printout;
   }
}
//------------------------------------------------------------------------------
void TEftposAdyen::LogEFTPOSEnabling(AdyenTriggerLocation triggerType)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(triggerType == eUI)
        {
            List->Add("Note- "+ (AnsiString)"Enabling Adyen as Selected from UI" +"\n");
        }
        else if(triggerType == eBoot)
        {
            List->Add("Note- "+ (AnsiString)"Enabling Adyen at start of Menumate" +"\n");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }

}
//------------------------------------------------------------------------------
void TEftposAdyen::UpdateEFTPOSLogs(bool status)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        if(status)
        {
            List->Insert((List->Count-1),"<<< Adyen Enabled >>>");
        }
        else
        {
            List->Insert((List->Count-1),"<<< Adyen Interface Not Enabled >>>");
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//------------------------------------------------------------------------------
AnsiString TEftposAdyen::GetLogFileName()
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "Menumate Services";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\Adyen Post Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "AdyenPosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "\\" + name;
    return fileName;
}
//------------------------------------------------------------------------------
void TEftposAdyen::UpdateEFTPOSLogsForInvoiceNumber(AnsiString invoiceNumber)
{
    try
    {
        if(invoiceNumber != NULL && invoiceNumber != "")
        {
            AnsiString fileName = GetLogFileName();
            std::auto_ptr<TStringList> List(new TStringList);
            if (FileExists(fileName) )
              List->LoadFromFile(fileName);
            List->Insert((List->Count-1),"Invoice Number is:- " + invoiceNumber);
            List->SaveToFile(fileName );
        }
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//------------------------------------------------------------------------------
bool TEftposAdyen::AllowsTipsOnTransactions()
{
    return TGlobalSettings::Instance().EnableEftPosPreAuthorisation;
}
//------------------------------------------------------------------------------bool TEftposAdyen::ProcessTip(WideString OriginalDpsTxnRef, Currency OriginalAmount, Currency TipAmount, UnicodeString MerchantRef)
{
    bool retVal = false;
    try
    {
        int val = (OriginalAmount + TipAmount)*100;
        AdjustAuthorisation *adjustAuthorisation =  new AdjustAuthorisation();
        adjustAuthorisation->merchantAccount = MerchantRef;
        adjustAuthorisation->modificationAmount = new ModificationAmount();
        adjustAuthorisation->modificationAmount->currency = CurrencyString;

        adjustAuthorisation->modificationAmount->value = val;
        adjustAuthorisation->originalReference = OriginalDpsTxnRef;
        adjustAuthorisation->reference = (OriginalDpsTxnRef + "_1");

        if(TipAmount > OriginalAmount)
        {
            adjustAuthorisation->additionalData = new AdditionalData();
            adjustAuthorisation->additionalData->industryUsage = "DelayedCharge";

            ResourceDetails *details = GetResourceDetails();
            details->URL =  TGlobalSettings::Instance().EftPosTerminalId + "/adjustAuthorisation";

            CoInitialize(NULL);
            AdjustAndCaptureResponse* response = AdyenClient->AdjustAuthorisation(adjustAuthorisation,details);

            if(response->response.Pos("[adjustAuthorisation-received]") != 0)
            {
                retVal = CaptureAmount(adjustAuthorisation);
            }
        }
        else
        {
            retVal = CaptureAmount(adjustAuthorisation);
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
        retVal = false;
    }
    return retVal;
}
//--------------------------------------------------------------------------
bool TEftposAdyen::CaptureAmount(AdjustAuthorisation* adjustAuthorisation )
{
    bool retVal = false;
    try
    {
        CaptureModifiedAmount *captureModifiedAmount =  new CaptureModifiedAmount();
        captureModifiedAmount->merchantAccount = adjustAuthorisation->merchantAccount;
        captureModifiedAmount->modificationAmount = new ModificationAmount();
        captureModifiedAmount->modificationAmount->currency = CurrencyString;
        captureModifiedAmount->modificationAmount->value = adjustAuthorisation->modificationAmount->value;
        captureModifiedAmount->originalReference = adjustAuthorisation->originalReference;
        captureModifiedAmount->reference = adjustAuthorisation->reference;

        ResourceDetails *details = GetResourceDetails();
        details->URL =  TGlobalSettings::Instance().EftPosTerminalId + "/capture";

        CoInitialize(NULL);
        AdjustAndCaptureResponse* response = AdyenClient->CaptureModifiedAmount(captureModifiedAmount, details);

        if(response->response.Pos("[capture-received]") != 0)
        {
            TDBAdyen::UpdateEFTPOSReference(adjustAuthorisation->originalReference, adjustAuthorisation->reference, response->pspReference);
            retVal = true;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
        retVal = false;
    }
    return retVal;
}

