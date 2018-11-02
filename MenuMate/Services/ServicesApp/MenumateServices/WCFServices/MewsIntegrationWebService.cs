using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MewsIntegration;
using MewsIntegration.Domain;
using System.IO;

namespace MenumateServices.WCFServices
{
    public class MewsIntegrationWebService : IMewsIntegrationWebService
    {
        public List<Service> GetMewsServices(string platformAddress, BasicInquiry basicInquiry)
        {
            List<Service> service = new List<Service>();
            List<string> logsList = new List<string>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                service = mewsCommunicationController.GetMewsServices(platformAddress, basicInquiry, logsList);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            WriteToFile(logsList);
            return service;
        }

        public List<AccountingCategory> GetMewsAccountingCategories(string platformAddress, BasicInquiry basicInquiry)
        {
            List<AccountingCategory> accountingCategory = new List<AccountingCategory>();
            List<string> logsList = new List<string>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                accountingCategory = mewsCommunicationController.GetMewsAccountingCategories(platformAddress, basicInquiry, logsList);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            WriteToFile(logsList);
            return accountingCategory;
        }
        public List<Outlet> GetMewsOutlets(string platformAddress, BasicInquiry basicInquiry)
        {
            List<Outlet> outlets = new List<Outlet>();
            List<string> logsList = new List<string>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                outlets = mewsCommunicationController.GetMewsOutlets(platformAddress, basicInquiry, logsList);
            }
            catch (Exception exception)
            {

            }
            WriteToFile(logsList);
            return outlets;
        }
        public List<CustomerDetailsMews> SearchCustomers(string platformAddress, CustomerSearch customerSearch)
        {
            List<string> logsList = new List<string>();
            Customers customers = new Customers();
            List<CustomerDetailsMews> roomInquiryResult = new List<CustomerDetailsMews>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                roomInquiryResult = mewsCommunicationController.GetCustomerSearchResult(platformAddress, customerSearch, logsList);
                if (customers.CustomersList == null)
                    logsList.Add("Guest Count                    " + 0);
                else
                    logsList.Add("Guest Count                    " + customers.CustomersList.Count);
                WriteToFile(logsList);
            }
            catch (Exception exception)
            {
            }
            WriteToFile(logsList);
            return roomInquiryResult;
        }

        public SpaceDetails GetSpaceIds(string platformAddress, BasicInquiry basicInquiry)
        {
            string retValue = "";
            SpaceDetails spaceDetails = new SpaceDetails();
            List<string> logsList = new List<string>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                spaceDetails = mewsCommunicationController.GetSpaceIds(platformAddress, basicInquiry, logsList);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            WriteToFile(logsList);
            return spaceDetails;
        }

        public string PostOrder(string platformAddress, Order order)
        {
            string retValue = "";
            List<string> logsList = new List<string>();
            try
            {
                for (int count = 1; count < 3; count++)
                {
                    logsList.Add("Try Number                          :" + count);
                    MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                    retValue = mewsCommunicationController.PostOrder(platformAddress, order, logsList);
                    if (retValue == "Successful")
                        break;
                }
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            WriteToFile(logsList);
            return retValue;
        }
        public string PostBill(string platformAddress, Order order)
        {
            string retValue = "";
            SpaceDetails spaceDetails = new SpaceDetails();
            List<string> logsList = new List<string>();
            try
            {
                for (int count = 1; count <= 3; count++)
                {
                    if(count != 1)
                        logsList.Add("=================================================================================");
                    logsList.Add("Try Number                          :" + count.ToString());
                    MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                    retValue = mewsCommunicationController.PostBill(platformAddress, order, logsList);
                    if (retValue == "Successful")
                        break;
                }
            }
            catch (Exception exception)
            {
                int i = 0;
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

                string location = path;//Path.Combine(path, "logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }

                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                location = Path.Combine(location, "Mews Post Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }

                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "MewsPosts " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
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
