//---------------------------------------------------------------------------


#pragma hdrstop

#include "EFTPOSPaymentSense.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEftPosPaymentSense::TEftPosPaymentSense()
{
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
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
        frmDropDown->AddButton("Settlement  CutOver",&PrintZedReport);
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
               if(GetResponseStatus(TxnType, wcfResponse))
               {
                  TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                  if(EftTrans != NULL)
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
               }
               else
               {    
                  TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnType);
                  if(EftTrans != NULL)
                   {
                      EftTrans->EventCompleted = true;
                      EftTrans->Result = eDeclined;
                      EftTrans->ResultText = wcfResponse->TransactionResult;
                      if(wcfResponse->TransactionResult.UpperCase().Pos("CANCELLED") != 0 || wcfResponse->TransactionResult.UpperCase().Pos("TIME OUT") != 0)
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
        PACTerminalWrapper* terminalList = paymentSenseClient->GetAllCardTerminals(authorizationDetails);
        for(int index = 0; index < terminalList->Terminals.Length; index++)
        {
            PACTerminal *terminal = terminalList->Terminals[index];
            terminalIdList.push_back(terminal->TPI);
        }
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
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/" + TGlobalSettings::Instance().EftPosTerminalId + "/transactions";
        response = paymentSenseClient->DoTransaction(authorizationDetails, request);
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
                if(response->TransactionResult.UpperCase().Pos("SUCCESSFUL") != 0)
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
        authorizationDetails->URL = TGlobalSettings::Instance().EFTPosURL + "/" + TGlobalSettings::Instance().EftPosTerminalId + "/reports";
        paymentSenseClient->PrintReports(authorizationDetails, report);
    }
    catch( Exception& E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
    }
}

