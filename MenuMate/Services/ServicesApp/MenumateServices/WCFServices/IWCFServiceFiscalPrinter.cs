using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FiscalPrinterIntegration.Domain;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFServiceFiscalPrinter
    {
        [OperationContract]
        FiscalResponseDetails PrintFiscalReceipt(FiscalDataDetails fiscalDataDetails);
        [OperationContract]
        FiscalResponseDetails PrintZSettlement();
        [OperationContract]
        FiscalResponseDetails GetPrinterStatus();
    }
}
