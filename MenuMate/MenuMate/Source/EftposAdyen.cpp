//---------------------------------------------------------------------------


#pragma hdrstop

#include "EftposAdyen.h"
#include "GlobalSettings.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "DBAdyen.h"
#define BasicVar  "Basic"
#define StandardVar  "Extended"
#define ExtendedVar  "Extended"
#define Request  "Request"
#define ResponseVar  "Response"
#define Notification  "Notification"
#define Abort  "Abort"
#define Admin  "Admin"
#define BalanceInquiry  "BalanceInquiry"
#define Batch  "Batch"
#define CardAcquisition  "CardAcquisition"
#define CardReaderAPDU  "CardReaderAPDU"
#define CardReaderInit  "CardReaderInit"
#define CardReaderPowerOff  "CardReaderPowerOff"
#define Diagnosis  "Diagnosis"
#define Display  "Display"
#define EnableService  "EnableService"
#define Event  "Event"
#define GetTotals  "GetTotals"
#define Input  "Input"
#define InputUpdate  "InputUpdate"
#define Login  "Login"
#define Logout  "Logout"
#define Loyalty  "Loyalty"
#define Payment  "Payment"
#define PIN  "PIN"
#define Print  "Print"
#define Reconciliation  "Reconciliation"
#define Reversal  "Reversal"
#define Sound  "Sound"
#define StoredValue  "StoredValue"
#define TransactionStatus  "TransactionStatus"
#define Transmit  "Transmit"
#define Service  "Service"
#define Device  "Device"
#define Event  "Event"
#define Normal  "Normal"
#define Refund  "Refund"
#define OneTimeReservation  "OneTimeReservation"
#define FirstReservation  "FirstReservation"
#define UpdateReservation  "UpdateReservation"
#define Completion  "Completion"
#define CashAdvance  "CashAdvance"
#define CashDeposit  "CashDeposit"
#define Recurring  "Recurring"
#define Instalment  "Instalment"
#define IssuerInstalment  "IssuerInstalment"
#define PaidOut  "PaidOut"
#define CustCancel  "CustCancel"
#define MerchantCancel  "MerchantCancel"
#define Malfunction  "Malfunction"
#define Unable2Compl  "Unable2Compl"

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
    return retVal ;
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
//               DTO_SmartLinkTransactionResponse *wcfResponse;
               CoInitialize(NULL);
               switch (TxnType)
                  {
                    case TransactionType_PURCHASE :
                        MessageBox("TransactionType_PURCHASE","TransactionType_PURCHASE",MB_OK);
                    break;
                    case TransactionType_CASH_ADVANCE :
                        MessageBox("TransactionType_CASH_ADVANCE","TransactionType_CASH_ADVANCE",MB_OK);
                    break;
                    case TransactionType_PURCHASE_PLUS_CASH :
                        MessageBox("TransactionType_PURCHASE_PLUS_CASH","TransactionType_PURCHASE_PLUS_CASH",MB_OK);
                    break;
                    case TransactionType_REFUND :
                        MessageBox("TransactionType_REFUND","TransactionType_REFUND",MB_OK);
                    break;
                    case TransactionType_INQUIRY :
                        MessageBox("TransactionType_INQUIRY","TransactionType_INQUIRY",MB_OK);
                    break;
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
   try
    {

    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TEftposAdyen::DoLogOut()
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

AnsiString TEftposAdyen::GetRefNumber()
{
//  return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}
// ---------------------------------------------------------------------------
Envelop* TEftposAdyen::GetPingTerminalEnvelop()
{
    Envelop* envelop = new Envelop();
    envelop->SaleToPOIRequest = new SaleToPOIRequest();
    envelop->SaleToPOIRequest->MessageHeader = new MessageHeader();
    envelop->SaleToPOIRequest->MessageHeader->MessageCategory = Diagnosis;
    envelop->SaleToPOIRequest->MessageHeader->MessageClass    = Service;
    envelop->SaleToPOIRequest->MessageHeader->MessageType     = Request;
    envelop->SaleToPOIRequest->MessageHeader->POIID           = TGlobalSettings::Instance().EFTPosDeviceID;
    envelop->SaleToPOIRequest->MessageHeader->ProtocolVersion = "3.0";
    envelop->SaleToPOIRequest->MessageHeader->SaleID          = TDeviceRealTerminal::Instance().ID.Name;
    envelop->SaleToPOIRequest->MessageHeader->ServiceID       = TDBAdyen::GetServiceID();

//    envelop->SaleToPOIRequest->PaymentRequest = new PaymentRequest();
//    envelop->SaleToPOIRequest->PaymentRequest->SaleData = new SaleData();
//    envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID = new SaleTransactionID();
//    envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID->TransactionID = "";
//    envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleTransactionID->TimeStamp = "";
//    envelop->SaleToPOIRequest->PaymentRequest->SaleData->SaleReferenceID = "";
//    envelop->SaleToPOIRequest->PaymentRequest->SaleData-> = "";
    envelop->SaleToPOIRequest->DiagnosisRequest = new DiagnosisRequest();
    return envelop;
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
