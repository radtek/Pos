//---------------------------------------------------------------------------
#pragma hdrstop

#include "FiscalDataManager.h"
#include "TCPFiscal.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TFiscalDataManager::TFiscalDataManager()
{
}
//---------------------------------------------------------------------------
TFiscalDataManager::~TFiscalDataManager()
{
}
//---------------------------------------------------------------------------
UnicodeString TFiscalDataManager::GetDataFromTransaction(TPaymentTransaction PaymentTransaction)
{
    return "";
}
//---------------------------------------------------------------------------
bool TFiscalDataManager::CheckFiscalDetails()
{
//    std::auto_ptr<TFiscalDataUtility> dataUtility(new TFiscalDataUtility());
//    AnsiString request = dataUtility->CreateVersionRequest();
//    std::auto_ptr<TTCPFiscal> tcpFiscal (new TTCPFiscal());
//    AnsiString response = tcpFiscal->SendAndFetch(request, VerNumber);
////    MessageBox(response,"response for Enabling",MB_OK);
//    if(response.Length() > 0)
//        return true;
//    else
//        return false;
}
//---------------------------------------------------------------------------
bool TFiscalDataManager::PostFiscalData(AnsiString data)
{
    return false;
}
//---------------------------------------------------------------------------
