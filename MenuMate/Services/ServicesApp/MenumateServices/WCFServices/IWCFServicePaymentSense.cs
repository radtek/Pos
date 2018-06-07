using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using PaymentSenseIntegration.Domain;
using PaymentSenseIntegration.Domain.ResponseData;
using PaymentSenseIntegration.Domain.SignatureRequest;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServicePaymentSense" in both code and config file together.
    [ServiceContract]
    public interface IWCFServicePaymentSense
    {
        [OperationContract]
        PACTerminalWrapper GetAllCardTerminals(AuthorizationDetails autorizationDetails);
        [OperationContract]
        PACTerminal PingTerminal(AuthorizationDetails autorizationDetails);
        [OperationContract]
        PostRequestResponse DoTransaction(AuthorizationDetails autorizationDetails, TransactionRequest request);
        [OperationContract]
        ReportResponseData PrintReports(AuthorizationDetails autorizationDetails, Reports reportData);
        [OperationContract]
        TransactionDataResponse GetResponseForRequestedId(AuthorizationDetails autorizationDetails);
        [OperationContract]
        TransactionDataResponse SignatureVerificationForRequestedId(AuthorizationDetails autorizationDetails, SignatureRequest signRequest);
    }
}
