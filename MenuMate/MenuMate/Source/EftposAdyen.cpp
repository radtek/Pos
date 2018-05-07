//---------------------------------------------------------------------------


#pragma hdrstop

#include "EftposAdyen.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "DBAdyen.h"
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
             MessageBox("Adyen EFTPOS Terminal is not available.\rAdyen EFTPOS is disabled.","Information",MB_OK+MB_ICONINFORMATION);
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
		/*Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
		PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
		DBTransaction.Commit();*/

		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("EFTPOS Login",&DoLogon);
        frmDropDown->AddButton("EFTPOS Log Out",&DoLogOut);
//		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
//		frmDropDown->AddButton("Settlement  CutOver",&DoSettlementCutover);
//		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
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
                        response = TriggerSaleTransaction(AmtPurchase);
                    }
                    break;
                    case TransactionType_REFUND :
                        response = TriggerRefundTransaction(AmtPurchase);
                    break;
                    case TransactionType_INQUIRY :
                    break;
                  }
                  if(GetResponseStatus(TxnType, response))
                  {
                      AcquirerRefSmartPay = response->PaymentResponse->PaymentResult->PaymentAcquirerData->AcquirerTransactionID->TransactionID;
                      //wcfResponse->AcquirerRef;
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                      {
                          EftTrans->EventCompleted = true;
                          EftTrans->ResultText = "Eftpos Transaction Completed.";
                          EftTrans->Result = eAccepted;
                          EftTrans->TimeOut = "2";
                       }
                  }
                  else
                  {
//                      MessageBox("Transaction Failed", "EFTPOS ",MB_OK + MB_ICONERROR);
                      TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                      if(EftTrans != NULL)
                      {
                          EftTrans->EventCompleted = true;
                          EftTrans->Result = eDeclined;
                          EftTrans->ResultText = "Manual Error";
                          EftTrans->TimeOut = "2";
                      }
                  }
            }
        }
        catch( Exception& exc )
        {
              MessageBox("In Exception of ProcessEftPos","",MB_OK);
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
            if(response->DiagnosisResponse->Response->Result.Pos("Success") != 0)
                int i = 0;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
Envelop* TEftposAdyen::GetLoginLogOutEnvelop(AdyenRequestType requestType)
{
    Envelop* envelop = new Envelop();
    try
    {
        envelop->SaleToPOIRequest = new SaleToPOIRequest();

        envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenLoginRequest);

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
            envelop->SaleToPOIRequest->LoginRequest->SaleTerminalData->TotalsGroupID = "ABC";
            envelop->SaleToPOIRequest->LoginRequest->OperatorID = TDeviceRealTerminal::Instance().ID.Name;
            envelop->SaleToPOIRequest->LoginRequest->OperatorLanguage = "English";
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
void __fastcall TEftposAdyen::DoLogOut()
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetLoginLogOutEnvelop(eAdyenLoginRequest);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->LogoutSystem(envelop,details);
        if(response != NULL)
        {
            if(response->DiagnosisResponse->Response->Result.Pos("Success") != 0)
                int i = 0;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
// ---------------------------------------------------------------------------

AnsiString TEftposAdyen::GetRefNumber()
{
//  return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}
//----------------------------------------------------------------------------
SaleToPOIResponse* TEftposAdyen::TriggerSaleTransaction(Currency AmtPurchase)
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetSaleEnvelop(AmtPurchase, eAdyenNormalSale);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->Purchase(envelop,details);
        if(response != NULL)
        {
            if(response->DiagnosisResponse->Response->Result.Pos("Success") != 0)
                int i = 0;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return response;
}
//----------------------------------------------------------------------------
SaleToPOIResponse* TEftposAdyen::TriggerRefundTransaction(Currency AmtPurchase)
{
    SaleToPOIResponse* response;
    try
    {
        Envelop *envelop = GetSaleEnvelop(AmtPurchase, eAdyenRefundSale);
        ResourceDetails *details = GetResourceDetails();
        CoInitialize(NULL);
        response = AdyenClient->Refund(envelop,details);
        if(response != NULL)
        {
            if(response->DiagnosisResponse->Response->Result.Pos("Success") != 0)
            int i = 0;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return response;
}
//----------------------------------------------------------------------------
Envelop* TEftposAdyen::GetSaleEnvelop(Currency AmtPurchase, AdyenRequestType requestType)
{
    Envelop* envelop = new Envelop();
    try
    {
        envelop->SaleToPOIRequest = new SaleToPOIRequest();

        envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenNormalSale);

        UnicodeString transactionId = TDBAdyen::GetTransactionID();
        UnicodeString saleRefId     = "SaleRef" + transactionId;
        double amount = (double)AmtPurchase;
        envelop->SaleToPOIRequest->PaymentRequest = new PaymentRequest();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData = new SaleData();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID = new SaleTransactionID();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID->TransactionID = transactionId;
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID->TimeStamp = TDBAdyen::GetTransactionID();
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleReferenceID = saleRefId;
        envelop->SaleToPOIRequest->PaymentRequest->SaleData->TokenRequestedType = "Customer";

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
MessageHeader* TEftposAdyen::GetMessageHeader(AdyenRequestType requestType)
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

    messageHeader->MessageClass    = ServiceVar;
    messageHeader->MessageType     = RequestVar;
    messageHeader->POIID           = TGlobalSettings::Instance().EFTPosDeviceID;
    messageHeader->ProtocolVersion = "3.0";
    messageHeader->SaleID          = TDeviceRealTerminal::Instance().ID.Name;
    messageHeader->ServiceID       = TDBAdyen::GetServiceID();
    return messageHeader;
}
//----------------------------------------------------------------------------
Envelop* TEftposAdyen::GetPingTerminalEnvelop()
{
    Envelop* envelop = new Envelop();
    envelop->SaleToPOIRequest = new SaleToPOIRequest();
    envelop->SaleToPOIRequest->MessageHeader = GetMessageHeader(eAdyenPingRequest);

    envelop->SaleToPOIRequest->DiagnosisRequest = new DiagnosisRequest();
    return envelop;
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
                if(response->PaymentResponse->Response->Result.Pos("Success") != 0)
                    retValue = true;
                else
                    retValue = false;
            }
            else
                retValue = false;
        }
        break;
        case TransactionType_REFUND :
        {
            int i = 0;
        }
        break;
        case TransactionType_INQUIRY :
        {
            int i = 0;
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
