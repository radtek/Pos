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
        #region RestImplemenation
        public string InitAustriaFiscal(string url, string cashBoxId, string accessToken)
        {
            string response = "";
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Echo:-                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                response = controller.InitAustriaFiscal(url, cashBoxId, accessToken, logsList);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return response;
        }
        public ReceiptReponseLocal PostData(ReceiptRequestLocal receiptRequest, string url, string cashBoxId, string accessToken)
        {
            ReceiptReponseLocal response = new ReceiptReponseLocal();
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Post:-                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                response = controller.ProcessSaleReceipt(receiptRequest, url, cashBoxId, accessToken, logsList);
            }
            catch (Exception ex)
            {

            }
            WriteToFile(logsList);
            return response;
        }
        public bool CommissionAustriaFiscal(string url, string cashBoxId, string terminalId, string accessToken)
        {
            bool retValue = false;
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Commission:-                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                retValue = controller.CommissionAustriaFiscal(url, cashBoxId, logsList, terminalId, accessToken);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return retValue;
        }
        public int SendZeroReceipt(string url, string cashBoxId, string terminalId, string accessToken)
        {
            bool retValue = false;
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Zero Receipt:-                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                retValue = controller.SendZeroReceipt(url, cashBoxId, logsList, terminalId, accessToken);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return retValue;
        }
        public bool SendMonthlyReceipt(string url, string cashBoxId, string terminalId, string accessToken)
        {
            bool retValue = false;
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Monthly Receipt:-                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                retValue = controller.SendMonthlyReceipt(url, cashBoxId, logsList, terminalId, accessToken);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return retValue;
        }
        public bool SendAnnualReceipt(string url, string cashBoxId, string terminalId, string accessToken)
        {
            bool retValue = false;
            List<string> logsList = new List<string>();
            try
            {
                logsList.Add("Request for Annual Receipt:-                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                AustriaFiscalController controller = new AustriaFiscalController();
                retValue = controller.SendAnnualReceipt(url, cashBoxId, logsList, terminalId, accessToken);
            }
            catch (Exception ex)
            {
            }
            WriteToFile(logsList);
            return retValue;
        }
        #endregion

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
