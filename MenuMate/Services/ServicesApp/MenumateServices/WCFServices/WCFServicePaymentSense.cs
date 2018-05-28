using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using System.IO;
using System.Diagnostics;
using PaymentSenseIntegration;
using PaymentSenseIntegration.Domain;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServicePaymentSense" in both code and config file together.
    public class WCFServicePaymentSense : IWCFServicePaymentSense
    {
        private List<string> stringList;
        public WCFServicePaymentSense()
        {
            stringList = new List<string>();
        }

        public PACTerminalWrapper GetAllCardTerminals(AuthorizationDetails autorizationDetails)
        {
            stringList.Add("Request to GetAllCardTerminals");
            WriteAndClearStringList();
            PACTerminalWrapper response = new PACTerminalWrapper();
            stringList.Add("Constructor created.");
            WriteAndClearStringList();
            try
            {
                 stringList.Add("Before calling controller.");
            WriteAndClearStringList();
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                stringList.Add("After creating instance of controller.");
            WriteAndClearStringList();
                response = controller.GetAllTerminals(autorizationDetails);
                    stringList.Add("After calling GetAllTerminals function");
            WriteAndClearStringList();
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetAllCardTerminals " + ex.Message);
            }
            return response;
        }

        public PACTerminal PingTerminal(AuthorizationDetails autorizationDetails)
        {
            PACTerminal response = new PACTerminal();
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                response = controller.PingTerminal(autorizationDetails);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetAllCardTerminals " + ex.Message);
            }
            return response;
        }

        public TransactionDataResponse DoTransaction(AuthorizationDetails autorizationDetails, TransactionRequest request)
        {
            TransactionDataResponse response = new TransactionDataResponse();
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                response = controller.DoTransaction(autorizationDetails, request);
                
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in DoPurchase " + ex.Message);
            }
            return response;
        }

        public bool PrintReports(AuthorizationDetails autorizationDetails, Reports reportData)
        {
            bool reportPrinted = false;
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                reportPrinted = controller.PrintReports(autorizationDetails, reportData);

            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in DoPurchase " + ex.Message);
            }
            return reportPrinted;
        }

        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Payment Sense Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "PaymentSenseLogs " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
                string fileName = Path.Combine(location, name2);

                if (fileName.Contains(@"file:\"))
                {
                    fileName = fileName.Replace(@"file:\", "");
                }
                if (!File.Exists(fileName))
                {

                    using (StreamWriter sw = File.CreateText(fileName))
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            sw.WriteLine(list[i]);
                        }
                    }
                }
                else
                {
                    using (var sw = File.AppendText(fileName))
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            sw.WriteLine(list[i]);
                        }
                    }
                }
                list.Add("  ");
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Making File" + ex.Message);
            }
        }

        private void WriteAndClearStringList()
        {
            WriteToFile(stringList);
            stringList.Clear();
        }
    }
}
