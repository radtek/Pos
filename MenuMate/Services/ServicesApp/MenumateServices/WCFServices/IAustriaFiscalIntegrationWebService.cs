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
        string InitAustriaFiscal(string url, string cashBoxId);
        [OperationContract]
        ReceiptReponseLocal PostData(ReceiptRequestLocal receiptRequest);
        [OperationContract]
        bool CommissionAustriaFiscal(string url, string cashBoxId, string terminalId);
        [OperationContract]
        bool SendZeroReceipt(string url, string cashBoxId, string terminalId);
        [OperationContract]
        bool SendMonthlyReceipt(string url, string cashBoxId, string terminalId);
        [OperationContract]
        bool SendAnnualReceipt(string url, string cashBoxId, string terminalId);
    }
}
