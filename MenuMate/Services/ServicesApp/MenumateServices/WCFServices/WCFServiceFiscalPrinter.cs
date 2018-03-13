using System;
using FiscalPrinterIntegration;
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
                FiscalPrinter_FP81II fiscalPrinter = new FiscalPrinterIntegration.FiscalPrinter_FP81II();
                response = fiscalPrinter.PrintDetails(fiscalDataDetails);

            }
            catch (Exception Ex)
            {

            }
            return response;
        }

        public FiscalResponseDetails PrintZSettlement()
        {
            FiscalResponseDetails response = new FiscalResponseDetails();
            try
            {
                FiscalPrinter_FP81II fiscalPrinter = new FiscalPrinterIntegration.FiscalPrinter_FP81II();
                response = fiscalPrinter.PrintZReport();

            }
            catch (Exception Ex)
            {

            }
            return response;
        }

       
    }
}
