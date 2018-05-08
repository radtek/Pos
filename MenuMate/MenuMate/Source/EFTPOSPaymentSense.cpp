//---------------------------------------------------------------------------


#pragma hdrstop

#include "EFTPOSPaymentSense.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TEftPosPaymentSense::TEftPosPaymentSense()
{
}
//---------------------------------------------------------------------------
TEftPosPaymentSense::~TEftPosPaymentSense()
{
}
//---------------------------------------------------------------------------
void TEftPosPaymentSense::Initialise()
{
    TEftPos::Initialise();
    if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
    {
        Enabled = true;
    }
    else
    {
        Enabled = false;
    }
}
//----------------------------------------------------------------------------
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

