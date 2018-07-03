using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using AdyenIntegration.Domain;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain.RequestEnvelop;
using AdyenIntegration.Domain.ResponseEnvelop;
using AdyenIntegration.Domain.Constants;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IAdyenIntegrationWebService
    {
        [OperationContract]
        SaleToPOIResponse PingTerminal(Envelop envelop, ResourceDetails details);

        [OperationContract]
        SaleToPOIResponse Purchase(Envelop envelop, ResourceDetails details);

        [OperationContract]
        SaleToPOIResponse Refund(Envelop envelop, ResourceDetails details);

        [OperationContract]
        SaleToPOIResponse GetTransactionStatus(Envelop envelop, ResourceDetails details);

        [OperationContract]
        SaleToPOIResponse LoginToSystem(Envelop envelop, ResourceDetails details);

        [OperationContract]
        SaleToPOIResponse LogoutSystem(Envelop envelop, ResourceDetails details);
    }
}
