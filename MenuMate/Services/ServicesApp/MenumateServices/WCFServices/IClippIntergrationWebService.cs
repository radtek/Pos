using System.Collections.Generic;
using System.ServiceModel;
using MenumateServices.DTO.Clipp;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IClippIntergrationWebService
    {
        [OperationContract]
        List<ClippMessage> GetOpenMessages();

        [OperationContract]
        List<ClippMessage> GetPreviousMessages();

        [OperationContract]
        void UpdateTabDetails(ClippTabDetailRequest clippTabDetail);

        [OperationContract(Action = "UpdateTabDetailsOnError", Name = "UpdateTabDetailsOnError")]
        void UpdateTabDetailsOnError(ClippErrorDetail clippErrorDetail);

        [OperationContract]
        ClippMessageDetail CloseTab(ClippTabDetailRequest clippTabDetail);

        [OperationContract(Action = "CloseTabOnError", Name = "CloseTabOnError")]
        void CloseTabOnError(ClippErrorDetail clippErrorDetail);

        [OperationContract]
        PaymentDetailResponse RequestTabPayment(PaymentDetailRequest paymentDetailRequest);

        [OperationContract]
        bool ForceCloseAllTabs();
    }
}
