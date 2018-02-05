//---------------------------------------------------------------------------


#pragma hdrstop

#include "PocketVoucherInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TPocketVoucherInterface::TPocketVoucherInterface()
{
   initPVClient();
}
void TPocketVoucherInterface::initPVClient()
{
    bool useWSDL = false;
    AnsiString addr = AnsiString("http://localhost:8740/MenumateServices/PocketVoucherService/");
    pVClient = GetIPocketVoucherWebService(useWSDL,  addr, NULL);
}
PocketVoucherDetails TPocketVoucherInterface::GetPocketVoucherDetail(UnicodeString accName)
{
    PocketVoucherDetail *pvDetails = new PocketVoucherDetail();
    pvDetails = pVClient->GetPocketVoucherDetail(accName);
    PocketVoucherDetails pvDetailsLocal;
    if(pvDetails != NULL)
    {
        pvDetailsLocal.MerchantId = pvDetails->MerchantId;
        pvDetailsLocal.Password = pvDetails->Password;
        pvDetailsLocal.PocketVoucherField = pvDetails->PocketVoucher;
        pvDetailsLocal.Url = pvDetails->Url;
        pvDetailsLocal.Username = pvDetails->UserName;
    }

    return pvDetailsLocal;
}
void TPocketVoucherInterface::EnablePocketVoucher(UnicodeString accName)
{
    pVClient->EnablePocketVouchers(accName);
}
void TPocketVoucherInterface::DisablePocketVoucher(UnicodeString accName)
{
    pVClient->DisablePocketVouchers(accName);
}
RedemptionResponse TPocketVoucherInterface::PrepareRedeemRequest(Request request)
{
    try
    {
        VoucherRedemptionDetails *redemptionDetails = new VoucherRedemptionDetails();
        RedemptionResponseDetails *response = new RedemptionResponseDetails();
        RedemptionResponse responseReceived;
        redemptionDetails->VoucherCode = request.voucherCode;
        redemptionDetails->TransactionAmount = request.transactionAmount;
        redemptionDetails->VoucherAmount = request.voucherAmount;
        redemptionDetails->MerchantId = request.merchantId;
        redemptionDetails->MerchantReference = request.merchantReference;
        redemptionDetails->TerminalId = request.terminalId;
        redemptionDetails->Password = request.Password;
        redemptionDetails->UserName = request.User;
        try
        {
            response = pVClient->RedeemVoucher(redemptionDetails);
        }
        catch(Exception & E)
        {

        }
        if(response->RedemptionStatus.Length() != 0)
        {
            responseReceived.status = response->RedemptionStatus;
            responseReceived.responseCode = response->ResponseCode;
            responseReceived.responseMessage = response->ResponseMessage;
        }
        return responseReceived;
    }
	catch(Exception & E)
    {
	}
}
