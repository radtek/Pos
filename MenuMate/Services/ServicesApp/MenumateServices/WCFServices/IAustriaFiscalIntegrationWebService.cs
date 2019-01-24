using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using AustriaFiscalIntegration.Domain;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IAustriaFiscalIntegrationWebService
    {
        [OperationContract]
        string InitAustriaFiscal(string url, string cashBoxId, string accessToken);
        [OperationContract]
        ReceiptReponseLocal PostData(ReceiptRequestLocal receiptRequest, string url, string cashBoxId, string accessToken);
        [OperationContract]
        bool CommissionAustriaFiscal(string url, string cashBoxId, string terminalId, string accessToken);
        [OperationContract]
        bool SendZeroReceipt(string url, string cashBoxId, string terminalId, string accessToken);
        [OperationContract]
        bool SendMonthlyReceipt(string url, string cashBoxId, string terminalId, string accessToken);
        [OperationContract]
        bool SendAnnualReceipt(string url, string cashBoxId, string terminalId, string accessToken);
    }
}
