using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AustriaFiscalIntegration;
using AustriaFiscalIntegration.Domain;
using System.IO;

namespace MenumateServices.WCFServices
{
    public class AustriaFiscalIntegrationWebService : IAustriaFiscalIntegrationWebService
    {
        public ReceiptReponseLocal PostData(ReceiptRequestLocal receiptRequest)
        {
            ReceiptReponseLocal response = new ReceiptReponseLocal();
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Post:-                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                response = controller.ProcessSaleReceipt(receiptRequest,logsList);
            }
            catch (Exception ex)
            {
                
            }
            WriteToFile(logsList);
            return response;
        }
        public string InitAustriaFiscal(string url, string cashBoxId)
        {
            string response = "";
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Echo:-                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                response = controller.InitAustriaFiscal(url, cashBoxId,logsList);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return response;
        }
        public bool CommissionAustriaFiscal(string url, string cashBoxId)
        {
            bool retValue = false;
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Commission:-                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                retValue = controller.CommissionAustriaFiscal(url, cashBoxId, logsList);
            }
            catch (Exception ex)
            { 
            }
            WriteToFile(logsList);
            return retValue;
        }
        public bool SendZeroReceipt(string url, string cashBoxId)
        {
            bool retValue = false;
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Commission:-                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                retValue = controller.SendZeroReceipt(url, cashBoxId, logsList);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return retValue;
        }
        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                location = Path.Combine(location, "Austria Fiscal Post Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "AustriaFiscalPosts " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
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
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Making File" + ex.Message);
            }
        }
    }
}
