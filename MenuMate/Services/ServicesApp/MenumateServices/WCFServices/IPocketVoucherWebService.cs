using System.ServiceModel;
using SalesForceCommunication.Domain;
using PocketVoucherIntegration.RedemptionDetails;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IPocketVoucherWebService
    {
        [OperationContract]
        PocketVoucherDetail GetPocketVoucherDetail(string accountName);

        [OperationContract]
        bool EnablePocketVouchers(string accountName);

        [OperationContract]
        bool DisablePocketVouchers(string accountName);

        [OperationContract]
        RedemptionResponseDetails RedeemVoucher(VoucherRedemptionDetails redemptionDetails);
    }
}