using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using MenumateServices.DTO.Thorlink;
using Thorlink.Net;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFServiceThorlink
    {
        [OperationContract]
        void Initialize(DTO_ThorAuth dto_thorAuth);

        [OperationContract]
        DTO_TMemberInfo GetMemberInformation(DTO_ThorlinkInquiry inquiry);

        [OperationContract]
        DTO_TPurchaseInfo GetTenderInformation(DTO_TenderList tenderDetailsList);

        [OperationContract]
        DTO_TPurchaseInfo GetRefundInformation(DTO_RefundDetails refundDetails);
    }
}
