using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FiscalPrinterIntegration.Domain;

namespace MenumateServices.WCFServices
{
    public class WCFServiceFiscalPrinter : IWCFServiceFiscalPrinter
    {
        public WCFServiceFiscalPrinter()
        {
        }

        public FiscalResponseDetails PrintFiscalReceipt(FiscalDataDetails fiscalDataDetails)
        {
            FiscalResponseDetails response = new FiscalResponseDetails();
            try
            {
                int i = 0;
            }
            catch (Exception ex)
            {
            }
            return response;
        }
    }
}
