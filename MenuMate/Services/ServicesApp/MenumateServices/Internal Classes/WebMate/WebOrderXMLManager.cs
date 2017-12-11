using System;
using System.Collections.Generic;
using System.Linq;

using System.Xml;
using System.IO;

using System.Globalization;

using XMLManager;
using MenumateServices.WebMate.DTO;
using System.Diagnostics;


namespace MenumateServices.WebMate.InternalClasses
{
    /// <summary>
    /// 
    /// </summary>
    public class FileUrlEventArgs : EventArgs
    {
        string file_url;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        public FileUrlEventArgs(string inURL)
        {
            file_url = inURL;
        }

        /// <summary>
        /// 
        /// </summary>
        public string URL
        {
            get { return file_url; }
            set { file_url = value; }
        }
    }

    /// <summary>
    ///  Singleton Class: Save WebOrders in XML files
    /// </summary>
    public class WebOrderXMLManager : WebOrderResponsive
    {
        ServiceInfo service_info;
        WebOrderURLManager order_url_manager;

        private static string DATE_FORMAT_STRING = "yyyy-MM-dd";
        private static string TIME_FORMAT_STRING = "HH:mm:ss";
        private static string DATE_TIME_FORMAT_STRING = "yyyy-MM-dd HH:mm:ss";

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public WebOrderXMLManager()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public void Init()
        {
            service_info = new ServiceInfo();
            order_url_manager = WebOrderURLManager.Instance;

            checkAllOrderURLs();
        }

        /// <summary>
        /// Creates a new incomplete order
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        public void OpenIncomplete(string inWebOrderHandle)
        {
            string fileName = createXMLIncompleteOrderURI(inWebOrderHandle);
            XmlDocument xmlDoc = openXMLOrderDocument(inWebOrderHandle);
            xmlDoc.Save(fileName);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrder"></param>
        public void LoadIncomplete(string inWebOrderHandle, WebOrder outOrder)
        {
            outOrder.SetHandle(inWebOrderHandle);
            XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
            if (xmlDoc != null)
            {
                loadWebOrder(xmlDoc, outOrder);
            }

        }

        /// <summary>
        /// Saves an order in an XML file
        /// TODO: Notify WebOrderFileSupplier on a creation of a new"StoreName" folder
        /// </summary>
        /// <param name="inWebOrder"></param>
        /// <returns></returns>
        public string SaveComplete(WebOrder inWebOrder)
        {
            string fileURI = createXMLCompleteOrderURI(inWebOrder);
            XmlDocument xmlDoc = openXMLOrderDocument(inWebOrder);

            bool justCreated;
            string completeURL = createCompleteOrderURL(
                                     inWebOrder.FromSection.SiteName,
                                     inWebOrder.HeaderSection.StoreName);
            // Creates the "SiteName\StoreName" folder where the complete order will be saved
            // and notifies to be monitored
            checkURL(completeURL, out justCreated);
            if (justCreated)
            {
                notifyCompleteOrderURLCreated(completeURL);
            }
            xmlDoc.Save(fileURI);

            return fileURI;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="xmlDoc"></param>
        /// <param name="outWebOrder"></param>
        public void LoadComplete(XmlDocument xmlDoc, WebOrder outOrder)
        {
            try
            {
                XmlNode root = xmlDoc.DocumentElement;
                outOrder.SetHandle(root.Attributes["handle"].Value);
                loadWebOrderComplete(xmlDoc, outOrder);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadComplete " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 670, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        public void RemoveComplete(WebOrder inWebOrder)
        {
            string fileName = @"";
            try
            {
                fileName = createXMLCompleteOrderURI(inWebOrder);
                File.Delete(fileName);
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Failed to delete incomplete order: {0}. {1}", fileName, exc.Message));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        public void RemoveIncomplete(WebOrder inWebOrder)
        {
            string fileName = @"";

            try
            {
                fileName = createXMLIncompleteOrderURI(inWebOrder.Handle);
                File.Delete(fileName);
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Failed to delete incomplete order: {0}. {1}", fileName, exc.Message));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="inOrderHeaderSection"></param>
        public void SetIncompleteHeaderSection(string inWebOrderHandle, DTO_WebOrderHeader inOrderHeaderSection)
        {
            XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
            if (xmlDoc != null)
            {
                updateHeaderNode(xmlDoc, inOrderHeaderSection);
                //.............................................................
                string fileName = createXMLIncompleteOrderURI(inWebOrderHandle);
                xmlDoc.Save(fileName);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrderHeaderSection"></param>
        public void LoadIncompleteHeaderSection(string inWebOrderHandle, DTO_WebOrderHeader outOrderHeaderSection)
        {
            XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
            if (xmlDoc != null)
            {
                XmlNode headerSectionNode = XMLDocManager.GetNode(xmlDoc, @"Header");
                outOrderHeaderSection = readHeaderSection(headerSectionNode);
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="inOrderFromSection"></param>
        public void SetIncompleteFromSection(string inWebOrderHandle, DTO_WebOrderFrom inOrderFromSection)
        {
            XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
            if (xmlDoc != null)
            {
                updateFromNode(xmlDoc, inOrderFromSection);
                //.....................................................
                string fileName = createXMLIncompleteOrderURI(inWebOrderHandle);
                xmlDoc.Save(fileName);
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrderFromSection"></param>
        public void LoadIncompleteFromSection(string inWebOrderHandle, DTO_WebOrderFrom outOrderFromSection)
        {
            XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
            if (xmlDoc != null)
            {
                XmlNode fromSectionNode = XMLDocManager.GetNode(xmlDoc, @"From");
                outOrderFromSection = readFromSection(fromSectionNode);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="intOrderAccountSection"></param>
        public void SetIncompleteAccountSection(string inWebOrderHandle, DTO_WebOrderAccount intOrderAccountSection)
        {
            XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
            if (xmlDoc != null)
            {
                updateAccountNode(xmlDoc, intOrderAccountSection);
                //.....................................................
                string fileName = createXMLIncompleteOrderURI(inWebOrderHandle);
                xmlDoc.Save(fileName);
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrderAccountSection"></param>
        public void LoadIncompleteAccountSection(string inWebOrderHandle, DTO_WebOrderAccount outOrderAccountSection)
        {
            try
            {
                XmlDocument xmlDoc = createIncompleteOrderXmlDoc(inWebOrderHandle);
                if(xmlDoc != null)
                {
                    XmlNode accountSectionNode = XMLDocManager.GetNode(xmlDoc, @"Account");
                    outOrderAccountSection = readAccountSection(accountSectionNode);
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadIncompleteAccountSection " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 600, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        public void MoveOrderToSuccesfulFolder(WebOrder inWebOrder)
        {
            XmlDocument xmlDoc = openXMLOrderDocument(inWebOrder);

            bool justCreated;

            // Creates the "SiteName\StoreName" folder where the complete order will be saved
            // and notifies to be monitored
            checkURL(createSucessfulURL(
                        inWebOrder.GetCompleteFromSection().SiteName,
                        inWebOrder.GetCompleteHeaderSection().StoreName), out justCreated);

            string fileURI = createSucessfulURI(
                                inWebOrder.GetCompleteFromSection().SiteName,
                                inWebOrder.GetCompleteHeaderSection().StoreName,
                                inWebOrder.Handle);

            xmlDoc.Save(fileURI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        public void MoveOrderToFailedFolder(WebOrder inWebOrder, string inFailedMessage)
        {
            addOrderToFailedSummary(
                    inWebOrder.FromSection.SiteName,
                    inWebOrder.HeaderSection.StoreName,
                    inWebOrder.Handle,
                    inFailedMessage);

            XmlDocument xmlDoc = openXMLOrderDocument(inWebOrder);

            bool justCreated;

            // Creates the "SiteName\StoreName" folder where the complete order will be saved
            // and notifies to be monitored
            checkURL(createFailedURL(
                        inWebOrder.GetCompleteFromSection().SiteName,
                        inWebOrder.GetCompleteHeaderSection().StoreName), out justCreated);

            string fileURI = createFailedURI(
                                inWebOrder.GetCompleteFromSection().SiteName,
                                inWebOrder.GetCompleteHeaderSection().StoreName,
                                inWebOrder.Handle);

            xmlDoc.Save(fileURI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        public bool WebOrderWaitingToBeAccepted(
                       string inSiteName,
                       string inStoreName,
                       string inOrderHandle)
        {
            string filename = createXMLCompleteOrderURI(
                                    inSiteName,
                                    inStoreName,
                                    inOrderHandle);

            return File.Exists(filename);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        public bool WebOrderNotAccepted(
                       string inSiteName,
                       string inStoreName,
                       string inOrderHandle)
        {
            string filename = createFailedURI(
                                    inSiteName,
                                    inStoreName,
                                    inOrderHandle);

            return File.Exists(filename);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="?"></param>
        /// <param name="?"></param>
        /// <param name="?"></param>
        /// <returns></returns>
        public bool WebOrderIncomplete(
                        string inSiteName,
                        string inStoreName,
                        string inOrderHandle)
        {
            string incompletefilename = createIncompleteURI(inOrderHandle);

            return File.Exists(incompletefilename);

            /*
                        string completefilename   = createXMLCompleteOrderURI(inSiteName, inStoreName, inOrderHandle);
                        string incompletefilename = createIncompleteURI(inOrderHandle);

                        return !File.Exists(completefilename) &&
                                File.Exists(incompletefilename);
            */
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileURI"></param>
        /// <param name="webOrder"></param>
        /// <param name="siteName"></param>
        /// <param name="storeName"></param>
        /// <returns></returns>
        public string ReadCompletOrderFromFile(string inFileURI, out WebOrder outWebOrder)
        {
            XmlDocument webOrderXML = XMLDocManager.CreateXMLDoc(inFileURI);

            XmlNode root = XMLDocManager.GetRoot(webOrderXML);
            XmlNode header = XMLDocManager.GetNode(webOrderXML, @"Header");
            XmlNode from = XMLDocManager.GetNode(webOrderXML, @"From");
            string orderHandle = XMLDocManager.GetAttribute(root, @"handle", @"nohandle");

            outWebOrder = new WebOrder();
            LoadComplete(webOrderXML, outWebOrder);

            return orderHandle;
        }

        #endregion

        #region Private

        /// <summary>
        /// 
        /// </summary>
        void checkAllOrderURLs()
        {
            order_url_manager.CheckOrderURLs();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inURL"></param>
        void checkURL(string inURL, out bool outJustCreated)
        {
            order_url_manager.CheckURL(inURL, out outJustCreated);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <returns></returns>
        XmlDocument createIncompleteOrderXmlDoc(string inWebOrderHandle)
        {
            string fileName = createXMLIncompleteOrderURI(inWebOrderHandle);
            XmlDocument result = loadXMLOrderDocument(fileName);
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <returns></returns>
        string createXMLIncompleteOrderURI(string inWebOrderHandle)
        {
            return string.Format(@"{0}\{1}", createIncompleteOrderURL(),
                                             createXMLIncompleteOrderFileName(inWebOrderHandle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <returns></returns>
        string createIncompleteOrderURL()
        {
            return order_url_manager.IncompleteOrderURL;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLIncompleteOrderFileName(string inOrderHandle)
        {
            return string.Format(@"Weborder {0}.xml", inOrderHandle);
            //return string.Format(@"Weborder {0} {1}.xml", inOrderHandle, DateTime.Now.ToString(@"yyyy-MM-dd hh-mm-ss"));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        /// <returns></returns>
        string createXMLCompleteOrderURI(WebOrder inWebOrder)
        {
            return string.Format(@"{0}\{1}",
                                 createCompleteOrderURL(
                                       inWebOrder.GetCompleteFromSection().SiteName,
                                       inWebOrder.GetCompleteHeaderSection().StoreName),
                                 createXMLCompleteOrderFileName(
                                       inWebOrder.GetCompleteHeaderSection().StoreName,
                                       inWebOrder.Handle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLCompleteOrderURI(
                    string inSiteName,
                    string inStoreName,
                    string inOrderHandle)
        {
            return string.Format(@"{0}\{1}",
                                             createCompleteOrderURL(
                                                   inSiteName,
                                                   inStoreName),
                                             createXMLCompleteOrderFileName(
                                                   inStoreName,
                                                   inOrderHandle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <returns></returns>
        string createCompleteOrderURL(
                            string inSiteName,
                            string inStoreName)
        {
            string result = string.Format(@"{0}\{1}\{2}",
                                                     order_url_manager.CompleteOrderURL,
                                                     inSiteName,
                                                     inStoreName);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLCompleteOrderFileName(
                     string inStoreName,
                     string inOrderHandle)
        {
            return string.Format(@"Weborder {0} {1}.xml",
                                 inStoreName,
                                 inOrderHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createSucessfulURI(
                     string inSiteName,
                     string inStoreName,
                     string inOrderHandle)
        {
            return string.Format(@"{0}\{1}", createSucessfulURL(inSiteName, inStoreName),
                                             createXMLSucessfulFileName(
                                                   inStoreName,
                                                   inOrderHandle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <returns></returns>
        string createSucessfulURL(string inSiteName, string inStoreName)
        {
            string result = string.Format(@"{0}\{1}\{2}",
                                          order_url_manager.SuccessfulOrderURL,
                                          inSiteName,
                                          inStoreName);

            bool justCreated;

            checkURL(result, out justCreated);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLSucessfulFileName(
                     string inStoreName,
                     string inOrderHandle)
        {
            return string.Format(@"Weborder Sucessful {0} {1}.xml",
                                    inStoreName,
                                    inOrderHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderURI"></param>
        /// <returns></returns>
        string createFailedURI(
                     string inSiteName,
                     string inStoreName,
                     string inOrderHandle)
        {
            return string.Format(@"{0}\{1}", createFailedURL(inSiteName, inStoreName),
                                             createXMLFailedFileName(
                                                   inStoreName,
                                                   inOrderHandle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <returns></returns>
        string createFailedURL(
                    string inSiteName,
                    string inStoreName)
        {
            string result = string.Format(@"{0}\{1}\{2}", order_url_manager.FailedOrderURL, inSiteName, inStoreName);

            bool justCreated;
            checkURL(result, out justCreated);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createIncompleteURI(
                        string inOrderHandle)
        {
            string result = string.Format(
                                    @"{0}\{1}",
                                    order_url_manager.IncompleteOrderURL,
                                    createXMLIncompleteFileName(inOrderHandle));

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLFailedFileName(
                     string inStoreName,
                     string inOrderHandle)
        {
            return string.Format(@"Weborder Failed {0} {1}.xml",
                                inStoreName,
                                inOrderHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLIncompleteFileName(
                                string inOrderHandle)
        {
            return string.Format(@"Weborder {0}.xml", inOrderHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inURL"></param>
        /// <param name="inWebOrder"></param>
        protected void notifyCompleteOrderURLCreated(string inURL)
        {
            WebOrderProcessor.Instance.NotifyCompleteOrderURLCreated(inURL);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSourceOrderURI"></param>
        /// <param name="outStoreName"></param>
        /// <param name="outOrderHandle"></param>
        /// <param name="outErrorMessage"></param>
        /// <returns></returns>
        bool getInfoFromFailedXMLOrder(
                    string inSourceOrderURI,
                out string outSiteName,
                out string outStoreName,
                out string outOrderHandle,
                out string outErrorMessage)
        {
            try
            {
                XmlDocument webOrderXML = XMLDocManager.CreateXMLDoc(inSourceOrderURI);

                XmlNode root = XMLDocManager.GetRoot(webOrderXML);
                XmlNode header = XMLDocManager.GetNode(webOrderXML, @"Header");
                XmlNode from = XMLDocManager.GetNode(webOrderXML, @"From");

                outSiteName = XMLDocManager.GetAttribute(from, @"siteName", @"noname");
                outStoreName = XMLDocManager.GetAttribute(header, @"storeName", @"noname");
                outOrderHandle = XMLDocManager.GetAttribute(root, @"handle", @"nohandle");
                outErrorMessage = @"";

                return true;
            }
            catch (Exception exc)
            {
                outSiteName = @"";
                outStoreName = @"";
                outOrderHandle = @"";
                outErrorMessage = exc.Message;

                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        void addOrderToFailedSummary(
                string inSiteName,
                string inStoreName,
                string inOrderHandle,
                string inFailedMessage)
        {
            string summaryURI = createFailedSummaryURI(inSiteName, inStoreName);

            XmlDocument summaryXML = createFailedSummaryXMLDoc(
                                           summaryURI,
                                           inStoreName);

            XmlNode root = XMLDocManager.GetRoot(summaryXML);
            XmlNode summaryEntry = XMLDocManager.AddNode(summaryXML, root, @"OrderFailed");

            XMLDocManager.AddAttribute(summaryXML, summaryEntry, @"handle", inOrderHandle);
            XMLDocManager.AddAttribute(summaryXML, summaryEntry, @"errorMessage", inFailedMessage);
            XMLDocManager.AddAttribute(summaryXML, summaryEntry, @"dateTime", DateTime.Now.ToString(DATE_TIME_FORMAT_STRING));
            summaryXML.Save(summaryURI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createFailedSummaryURI(
                    string inSiteName,
                    string inStoreName)
        {
            return string.Format(@"{0}\{1}", createFailedURL(inSiteName, inStoreName),
                                             createXMLFailedSummaryFileName(inStoreName));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        string createXMLFailedSummaryFileName(string inStoreName)
        {
            return string.Format(@"{0} Failed Summary.xml", inStoreName);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSummaryURI"></param>
        /// <returns></returns>
        XmlDocument createFailedSummaryXMLDoc(
                          string inSummaryURI,
                          string inStoreName)
        {
            XmlDocument result;

            if (File.Exists(inSummaryURI)) // Load existing summary
            {
                result = XMLDocManager.CreateXMLDoc(inSummaryURI);
            }
            else // Create a new summary
            {
                result = new XmlDocument();
                //.................................................................
                // Create the XML Declaration, and append it to XML document
                XmlDeclaration xmlDeclaration = result.CreateXmlDeclaration("1.0", @"UTF-8", @"yes");
                result.AppendChild(xmlDeclaration);
                setSummaryRootNode(result, inStoreName);
                //....................................................................
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inStoreName"></param>
        /// <returns></returns>
        XmlElement setSummaryRootNode(XmlDocument inXMLDocument, string inStoreName)
        {
            XmlElement result = XMLDocManager.CreateRoot(inXMLDocument, @"FailedSummary");
            //....................................................................................
            XMLDocManager.AddAttribute(inXMLDocument, result, @"storeName", inStoreName);
            //....................................................................................
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSourceOrderURI"></param>
        /// <returns></returns>
        string createInvalidOrderURI(string inSourceOrderURI)
        {
            string fileName = Path.GetFileName(inSourceOrderURI);

            return string.Format(@"{0}\{1}", createInvalidOrderURL(),
                                             createXMLInvalidOrderFileName(fileName));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        string createInvalidOrderURL()
        {
            string result = string.Format(@"{0}\{1}", order_url_manager.FailedOrderURL, @"Invalid Orders");

            bool justCreated;
            checkURL(result, out justCreated);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <returns></returns>
        string createXMLInvalidOrderFileName(string inOrderFileName)
        {
            return string.Format(@"{0}.xml", inOrderFileName);
        }

        /// <summary>
        /// NOT USED SO FAR!!! 
        /// </summary>
        /// <param name="inOrderGUID"></param>
        /// <returns></returns>
        string createXMLPOSNotificatioFileName(string inStoreName)
        {
            return string.Format(@"Weborder Notification {0}.xml",
                                 inStoreName);
        }

        /// <summary>
        /// Create a new XML file
        /// </summary>
        /// <param name="inFileName"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        XmlDocument openXMLOrderDocument(string inOrderHandle)
        {
            XmlDocument result = new XmlDocument();

            //.................................................................

            // Create the XML Declaration, and append it to XML document
            XmlDeclaration xmlDeclaration = result.CreateXmlDeclaration("1.0", @"UTF-8", @"yes");
            result.AppendChild(xmlDeclaration);

            setOrderRootNode(result, inOrderHandle);

            //....................................................................

            List<string> commentList = new List<string>();
            List<DTO_WebOrderItem> itemList = new List<DTO_WebOrderItem>();

            //HEADER SECTION
            DTO_WebOrderHeader headerSection = new DTO_WebOrderHeader();
            headerSection.StoreName = @"";
            headerSection.GUID = Guid.Empty.ToString();
            headerSection.Scheduled = 0;
            headerSection.Respond = true;
            headerSection.OrderTotal = 0;
            headerSection.ExpectedDate = DateTime.Now;
            headerSection.OrderDate = DateTime.Now;

            //FROM SECTION
            DTO_WebOrderFrom fromSection = new DTO_WebOrderFrom();
            fromSection.SiteID = Guid.Empty.ToString();
            fromSection.SiteName = @"";

            //ACCOUNT SECTION
            DTO_WebOrderAccount accountSection = new DTO_WebOrderAccount();
            accountSection.Covers = 0;
            accountSection.Name = @"";
            accountSection.MemberNumber = @"";
            accountSection.Delivery = new DTO_WebOrderDelivery();
            accountSection.Delivery.Recipient = @"";
            accountSection.Delivery.StreetNo = @"";
            accountSection.Delivery.StreetName = @"";
            accountSection.Delivery.Suburb = @"";
            accountSection.Delivery.City = @"";
            accountSection.Delivery.Country = @"";
            accountSection.Delivery.Note = @"";
            accountSection.Delivery.Phone = @"";
            accountSection.Comments = commentList.ToArray();
            accountSection.Payments = new DTO_WebOrderPayments();
            accountSection.OrderItems = itemList.ToArray();
            accountSection.Email = @"";
            accountSection.OrderType = @"";
            //....................................................................

            addHeaderNode(result, headerSection);
            addFromNode(result, fromSection);
            addAccountNode(result, accountSection);

            //.................................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        XmlDocument openXMLOrderDocument(WebOrder inOrder)
        {
            XmlDocument result = new XmlDocument();

            //.................................................................

            // Create the XML Declaration, and append it to XML document
            XmlDeclaration xmlDeclaration = result.CreateXmlDeclaration("1.0", @"UTF-8", @"yes");
            result.AppendChild(xmlDeclaration);

            setOrderRootNode(result, inOrder.Handle);

            addHeaderNode(result, inOrder.GetCompleteHeaderSection());
            addFromNode(result, inOrder.GetCompleteFromSection());
            addAccountNode(result, inOrder.GetCompleteAccountSection());

            //.................................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        XmlDocument openXMLPOSNotificationDocument(string inOrderGUID)
        {
            XmlDocument result = new XmlDocument();

            //.................................................................

            // Create the XML Declaration, and append it to XML document
            XmlDeclaration xmlDeclaration = result.CreateXmlDeclaration("1.0", @"UTF-8", @"yes");
            result.AppendChild(xmlDeclaration);
            setOrderNotificationRootNode(result, inOrderGUID);
            //.................................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        /// <returns></returns>
        XmlDocument loadXMLOrderDocument(string inFileName)
        {
            if (File.Exists(inFileName))
            {
                XmlDocument result = XMLDocManager.CreateXMLDoc(inFileName);
                return result;
            }
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrder"></param>
        void loadEmptyWebOrder(WebOrder outOrder)
        {
            try
            {
                outOrder.HeaderSection = outOrder.EmptyHeaderSection;
                outOrder.FromSection = outOrder.EmptyFromSection;
                outOrder.AccountSection = outOrder.EmptyAccountSection;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadEmptyWebOrder " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 500, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXmlDoc"></param>
        /// <param name="inOrder"></param>
        void loadWebOrder(XmlDocument inXmlDoc, WebOrder outOrder)
        {
            try
            {
                XmlNode headerNode = XMLDocManager.GetNode(inXmlDoc, @"Header");
                XmlNode fromNode = XMLDocManager.GetNode(inXmlDoc, @"From");
                XmlNode accountNode = XMLDocManager.GetNode(inXmlDoc, @"Account");

                outOrder.HeaderSection = readHeaderSection(headerNode);
                outOrder.FromSection = readFromSection(fromNode);
                outOrder.AccountSection = readAccountSection(accountNode);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadWebOrder " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 501, short.MaxValue);
            }
        }

        ///
        void loadWebOrderComplete(XmlDocument inXmlDoc, WebOrder outOrder)
        {
            try
            {
                XmlNode headerNode = XMLDocManager.GetNode(inXmlDoc, @"Header");
                XmlNode fromNode = XMLDocManager.GetNode(inXmlDoc, @"From");
                XmlNode accountNode = XMLDocManager.GetNode(inXmlDoc, @"Account");

                outOrder.SetCompleteHeaderSection(headerNode);
                outOrder.SetCompleteFromSection(fromNode);
                outOrder.SetCompleteAccountSection(accountNode);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadWebOrderComplete " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 502, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        XmlElement setOrderRootNode(XmlDocument inXMLDocument, string inOrderHandle)
        {
            XmlElement result = XMLDocManager.CreateRoot(inXMLDocument, @"WebOrder");

            try
            {
                XMLDocManager.AddAttribute(inXMLDocument, result, @"handle", inOrderHandle);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in setOrderRootNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 503, short.MaxValue);
            }

            //....................................................................................

            return result;
        }

        /// <summary>
        /// inOrderGUID = Web order's handle
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        XmlElement setOrderNotificationRootNode(XmlDocument inXMLDocument, string inOrderGUID)
        {
            XmlElement result = XMLDocManager.CreateRoot(inXMLDocument, @"POSWebOrderNotification");

            XMLDocManager.AddAttribute(inXMLDocument, result, @"guid", inOrderGUID);

            //....................................................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderHeaderSection"></param>
        /// <returns></returns>
        XmlNode addHeaderNode(
                   XmlDocument inXMLDocument,
                   DTO_WebOrderHeader inOrderHeaderSection)
        {
            XmlElement root = inXMLDocument.DocumentElement;

            //....................................................................................

            XmlNode result = XMLDocManager.AddNode(inXMLDocument, root, @"Header");

            try
            {
                addXMLOrderAttribute(inXMLDocument, result, @"storeName", inOrderHeaderSection.StoreName);
                addXMLOrderAttribute(inXMLDocument, result, @"guid", inOrderHeaderSection.GUID);
                addXMLOrderAttribute(inXMLDocument, result, @"respond", inOrderHeaderSection.Respond.ToString());
                addXMLOrderAttribute(inXMLDocument, result, @"schedule", inOrderHeaderSection.Scheduled.ToString());
                addXMLOrderAttribute(inXMLDocument, result, @"orderTotal", inOrderHeaderSection.OrderTotal.ToString());
                addXMLOrderAttribute(inXMLDocument, result, @"orderDate", inOrderHeaderSection.OrderDate.ToString(DATE_TIME_FORMAT_STRING));
                addXMLOrderAttribute(inXMLDocument, result, @"expectedDate", inOrderHeaderSection.ExpectedDate.ToString(DATE_TIME_FORMAT_STRING));
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addHeaderNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 504, short.MaxValue);
            }

            //....................................................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderFromSection"></param>
        /// <returns></returns>
        XmlNode addFromNode(
                   XmlDocument inXMLDocument,
                   DTO_WebOrderFrom inOrderFromSection)
        {
            XmlElement root = inXMLDocument.DocumentElement;

            //....................................................................................

            XmlNode result = XMLDocManager.AddNode(inXMLDocument, root, @"From");

            try
            {
                addXMLOrderAttribute(inXMLDocument, result, @"siteID", service_info.NotNullString(inOrderFromSection.SiteID));
                addXMLOrderAttribute(inXMLDocument, result, @"siteName", service_info.NotNullString(inOrderFromSection.SiteName));
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addFromNode " + e.Message, e);
               //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 505, short.MaxValue);
            }

            //....................................................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderAccountSection"></param>
        /// <returns></returns>
        XmlNode addAccountNode(
                   XmlDocument inXMLDocument,
                   DTO_WebOrderAccount inOrderAccountSection)
        {
            XmlElement root = inXMLDocument.DocumentElement;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            XmlNode result = XMLDocManager.AddNode(inXMLDocument, root, @"Account");

            try
            {
                string covers = inOrderAccountSection.Covers.ToString();
                string name = service_info.NotNullString(inOrderAccountSection.Name);
                string memberNumber = service_info.NotNullString(inOrderAccountSection.MemberNumber);
                string email = service_info.NotNullString(inOrderAccountSection.Email);
                string ordertype = service_info.NotNullString(inOrderAccountSection.OrderType);

                addXMLOrderAttribute(inXMLDocument, result, @"covers", covers);
                addXMLOrderAttribute(inXMLDocument, result, @"name", name);
                addXMLOrderAttribute(inXMLDocument, result, @"memberNumber", memberNumber);
                addXMLOrderAttribute(inXMLDocument, result, @"email", email);
                addXMLOrderAttribute(inXMLDocument, result, @"ordertype", ordertype);

                addDeliveryNode(inXMLDocument, result, inOrderAccountSection.Delivery);
                addPaymentsNode(inXMLDocument, result, inOrderAccountSection.Payments);
                addCommentsNode(inXMLDocument, result, inOrderAccountSection.Comments);
                addItemsNode(inXMLDocument, result, inOrderAccountSection.OrderItems);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addAccountNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 506, short.MaxValue);
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inAccountNode"></param>
        /// <param name="inOrderDelivery"></param>
        /// <returns></returns>
        XmlNode addDeliveryNode(
                   XmlDocument inXMLDocument,
                   XmlNode inAccountNode,
                   DTO_WebOrderDelivery inOrderDelivery)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inAccountNode, @"Delivery");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                string recipient = service_info.NotNullString(inOrderDelivery.Recipient);
                string streetNo = service_info.NotNullString(inOrderDelivery.StreetNo);
                string streetName = service_info.NotNullString(inOrderDelivery.StreetName);
                string suburb = service_info.NotNullString(inOrderDelivery.Suburb);
                string city = service_info.NotNullString(inOrderDelivery.City);
                string country = service_info.NotNullString(inOrderDelivery.Country);
                string note = service_info.NotNullString(inOrderDelivery.Note);
                string phone = service_info.NotNullString(inOrderDelivery.Phone);

                addXMLOrderAttribute(inXMLDocument, result, @"recipient", recipient);
                addXMLOrderAttribute(inXMLDocument, result, @"streetNo", streetNo);
                addXMLOrderAttribute(inXMLDocument, result, @"streetName", streetName);
                addXMLOrderAttribute(inXMLDocument, result, @"suburb", suburb);
                addXMLOrderAttribute(inXMLDocument, result, @"city", city);
                addXMLOrderAttribute(inXMLDocument, result, @"country", country);
                addXMLOrderAttribute(inXMLDocument, result, @"note", note);
                addXMLOrderAttribute(inXMLDocument, result, @"phone", phone);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addDeliveryNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 507, short.MaxValue);
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inAccountNode"></param>
        /// <param name="inOrderPayments"></param>
        /// <returns></returns>
        XmlNode addPaymentsNode(
                   XmlDocument inXMLDocument,
                   XmlNode inAccountNode,
                   DTO_WebOrderPayments inOrderPayments)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inAccountNode, @"Payments");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            addXMLOrderAttribute(inXMLDocument, result, @"required", boolToString(inOrderPayments.PaymentRequired));

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inAccountNode"></param>
        /// <param name="inOrderComments"></param>
        /// <returns></returns>
        XmlNode addCommentsNode(
                            XmlDocument inXMLDocument,
                            XmlNode inAccountNode,
                            string[] inOrderComments)
        {
            
            
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inAccountNode, @"Comments");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                for (int i = 0; i < inOrderComments.Length; i++)
                {
                    XmlNode commentNode = XMLDocManager.AddNode(inXMLDocument, result, @"Comment");
                    commentNode.InnerText = inOrderComments[i];

                    //addXMLOrderAttribute(inXMLDocument, commentNode, @"text", inOrderComments[i]);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addCommentsNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 508, short.MaxValue);
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inAccountNode"></param>
        /// <param name="inOrderItems"></param>
        /// <returns></returns>
        XmlNode addItemsNode(
                    XmlDocument inXMLDocument,
                    XmlNode inAccountNode,
                    DTO_WebOrderItem[] inOrderItems)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inAccountNode, @"Items");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            for (int i = 0; i < inOrderItems.Length; i++)
            {
                addItemNode(inXMLDocument, result, inOrderItems[i]);
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inItemsNode"></param>
        /// <param name="inOrderItem"></param>
        /// <returns></returns>
        XmlNode addItemNode(
                   XmlDocument inXMLDocument,
                   XmlNode inItemsNode,
                   DTO_WebOrderItem inOrderItem)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inItemsNode, @"Item");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                string uid = service_info.NotNullString(inOrderItem.UID);

                addXMLOrderAttribute(inXMLDocument, result, @"uid", uid);
                addXMLOrderAttribute(inXMLDocument, result, @"basePrice", string.Format("{0:#.00}", inOrderItem.BasePrice));
                addXMLOrderAttribute(inXMLDocument, result, @"quantity", string.Format("{0:#.00}", inOrderItem.Qty));
                addXMLOrderAttribute(inXMLDocument, result, @"note", inOrderItem.Note);

                addOptionsNode(inXMLDocument, result, inOrderItem.ItemOptions);
                addSidesNode(inXMLDocument, result, inOrderItem.ItemSides);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addItemNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 509, short.MaxValue);
            }


            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderItemNode"></param>
        /// <param name="inOrderItemOptions"></param>
        /// <returns></returns>
        XmlNode addOptionsNode(
                    XmlDocument inXMLDocument,
                    XmlNode inOrderItemNode,
                    DTO_WebOrderItemOption[] inOrderItemOptions)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inOrderItemNode, @"Options");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                for (int i = 0; i < inOrderItemOptions.Length; i++)
                {
                    addItemOptionNode(inXMLDocument, result, inOrderItemOptions[i]);
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addOptionsNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 510, short.MaxValue);
            }
            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderItemNode"></param>
        /// <param name="inOrderItemSides"></param>
        /// <returns></returns>
        XmlNode addSidesNode(
                    XmlDocument inXMLDocument,
                    XmlNode inOrderItemNode,
                    DTO_WebOrderItemSide[] inOrderItemSides)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inOrderItemNode, @"Sides");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                for (int i = 0; i < inOrderItemSides.Length; i++)
                {
                    addItemSideNode(inXMLDocument, result, inOrderItemSides[i]);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addSidesNode " + e.Message, e);
               //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 511, short.MaxValue);
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inItemNode"></param>
        /// <param name="inOrderItemOption"></param>
        /// <returns></returns>
        XmlNode addItemOptionNode(
                    XmlDocument inXMLDocument,
                    XmlNode inItemNode,
                    DTO_WebOrderItemOption inOrderItemOption)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inItemNode, @"Option");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                string uid = service_info.NotNullString(inOrderItemOption.UID);

                addXMLOrderAttribute(inXMLDocument, result, @"uid", uid);
                addXMLOrderAttribute(inXMLDocument, result, @"basePrice", string.Format("{0:#.00}", inOrderItemOption.BasePrice));
                addXMLOrderAttribute(inXMLDocument, result, @"quantity", string.Format("{0:#.00}", inOrderItemOption.Qty));
                addXMLOrderAttribute(inXMLDocument, result, @"note", inOrderItemOption.Note);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addItemOptionNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 512, short.MaxValue);
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inItemNode"></param>
        /// <param name="inOrderItemSide"></param>
        /// <returns></returns>
        XmlNode addItemSideNode(
                    XmlDocument inXMLDocument,
                    XmlNode inItemNode,
                    DTO_WebOrderItemSide inOrderItemSide)
        {
            XmlNode result = XMLDocManager.AddNode(inXMLDocument, inItemNode, @"Side");

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                string uid = service_info.NotNullString(inOrderItemSide.UID);

                addXMLOrderAttribute(inXMLDocument, result, @"uid", uid);
                addXMLOrderAttribute(inXMLDocument, result, @"basePrice", string.Format("{0:#.00}", inOrderItemSide.BasePrice));
                addXMLOrderAttribute(inXMLDocument, result, @"quantity", string.Format("{0:#.00}", inOrderItemSide.Qty));
                addXMLOrderAttribute(inXMLDocument, result, @"note", inOrderItemSide.Note);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in addItemSideNode " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 513, short.MaxValue);
            }
            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderSectionNode"></param>
        /// <param name="outOrderHeaderSection"></param>
        DTO_WebOrderHeader readHeaderSection(XmlNode inHeaderSectionNode)
        {
            DTO_WebOrderHeader result = new DTO_WebOrderHeader();

            //............................................................

            try
            {
                string defaulDateTimeStr = DateTime.Now.ToString(DATE_TIME_FORMAT_STRING);

                string orderDateStr = getXMLOrderAttribute(inHeaderSectionNode, @"orderDate", defaulDateTimeStr);
                string expectedDateStr = getXMLOrderAttribute(inHeaderSectionNode, @"expectedDate", defaulDateTimeStr);

                //....................................................................................

                result.StoreName = getXMLOrderAttribute(inHeaderSectionNode, @"storeName", @"");
                result.GUID = getXMLOrderAttribute(inHeaderSectionNode, @"guid", Guid.Empty.ToString());
                result.Respond = Convert.ToBoolean(getXMLOrderAttribute(inHeaderSectionNode, @"respond", "false"));
                result.Scheduled = Convert.ToUInt16(getXMLOrderAttribute(inHeaderSectionNode, @"schedule", @"1"));
                result.OrderTotal = Convert.ToDecimal(getXMLOrderAttribute(inHeaderSectionNode, @"orderTotal", "0.00"));
                result.OrderDate = DateTime.Parse(orderDateStr, CultureInfo.CreateSpecificCulture("en-NZ"));
                result.ExpectedDate = DateTime.Parse(expectedDateStr, CultureInfo.CreateSpecificCulture("en-NZ"));
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readHeaderSection " + e.Message, e);
               //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 514, short.MaxValue);
            }

            //............................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFromSectionNode"></param>
        /// <param name="outOrderHeaderFrom"></param>
        DTO_WebOrderFrom readFromSection(XmlNode inFromSectionNode)
        {
            DTO_WebOrderFrom result = new DTO_WebOrderFrom();

            //............................................................
            try
            {

                result.SiteID = getXMLOrderAttribute(inFromSectionNode, @"siteID", "");
                result.SiteName = getXMLOrderAttribute(inFromSectionNode, @"siteName", "");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readFromSection " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 515, short.MaxValue);
            }

            //............................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inAccountSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        DTO_WebOrderAccount readAccountSection(XmlNode inAccountSectionNode)
        {
            DTO_WebOrderAccount result = new DTO_WebOrderAccount();

            //............................................................

            try
            {
                result.Covers = Convert.ToUInt32(getXMLOrderAttribute(inAccountSectionNode, @"covers", @"0"));
                result.Name = getXMLOrderAttribute(inAccountSectionNode, @"name", @"");
                result.MemberNumber = getXMLOrderAttribute(inAccountSectionNode, @"memberNumber", @"");
                result.Email = getXMLOrderAttribute(inAccountSectionNode, @"email", @"");
                result.OrderType = getXMLOrderAttribute(inAccountSectionNode, @"ordertype", @"");

                //............................................................

                result.Delivery = new DTO_WebOrderDelivery();
                result.Delivery.Recipient = @"";
                result.Delivery.StreetNo = @"";
                result.Delivery.StreetName = @"";
                result.Delivery.Suburb = @"";
                result.Delivery.City = @"";
                result.Delivery.Country = @"";
                result.Delivery.Note = @"";
                result.Delivery.Phone = @"";

                List<string> commentList = new List<string>();
                List<DTO_WebOrderItem> itemList = new List<DTO_WebOrderItem>();

                result.Comments = commentList.ToArray();
                result.Payments = new DTO_WebOrderPayments();
                result.OrderItems = itemList.ToArray();

                //............................................................

                readWebOrderDelivery(inAccountSectionNode.FirstChild, result);
                readWebOrderPayments(inAccountSectionNode.FirstChild.NextSibling, result);
                readWebOrderComments(inAccountSectionNode.FirstChild.NextSibling.NextSibling, result);
                readWebOrderItems(inAccountSectionNode.FirstChild.NextSibling.NextSibling.NextSibling, result);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readAccountSection " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 516, short.MaxValue);
            }

            //............................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDeliverySectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderDelivery(
                    XmlNode inDeliverySectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            try
            {
                outOrderHeaderAccount.Delivery.Recipient =
                            getXMLOrderAttribute(inDeliverySectionNode, @"recipient", @"");
                outOrderHeaderAccount.Delivery.StreetNo =
                        getXMLOrderAttribute(inDeliverySectionNode, @"streetNo", @"");
                outOrderHeaderAccount.Delivery.StreetName =
                        getXMLOrderAttribute(inDeliverySectionNode, @"streetName", @"");
                outOrderHeaderAccount.Delivery.Suburb =
                        getXMLOrderAttribute(inDeliverySectionNode, @"suburb", @"");
                outOrderHeaderAccount.Delivery.City =
                        getXMLOrderAttribute(inDeliverySectionNode, @"city", @"");
                outOrderHeaderAccount.Delivery.Country =
                        getXMLOrderAttribute(inDeliverySectionNode, @"country", @"");
                outOrderHeaderAccount.Delivery.Note =
                        getXMLOrderAttribute(inDeliverySectionNode, @"note", @"");
                outOrderHeaderAccount.Delivery.Phone =
                        getXMLOrderAttribute(inDeliverySectionNode, @"phone", @"");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readWebOrderDelivery " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 517, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPaymentsSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderPayments(
                    XmlNode inPaymentsSectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            try
            {
                outOrderHeaderAccount.Payments.PaymentRequired =
                            stringToBool(getXMLOrderAttribute(inPaymentsSectionNode, @"required", @"yes"));
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readWebOrderPayments " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 518, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCommentsSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderComments(
                    XmlNode inCommentsSectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            XmlNodeList commentNodeList = XMLDocManager.GetNodeList(inCommentsSectionNode, @"Comment");

            try
            {
                List<string> commentList = new List<string>();
                foreach (XmlNode commentNode in commentNodeList)
                {
                    commentList.Add(commentNode.InnerText);
                }

                outOrderHeaderAccount.Comments = commentList.ToArray();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readWebOrderComments " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 519, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemsSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderItems(
                    XmlNode inItemsSectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            XmlNodeList itemNodeList = XMLDocManager.GetNodeList(inItemsSectionNode, @"Item");

            try
            {
                List<DTO_WebOrderItem> itemList = new List<DTO_WebOrderItem>();
                foreach (XmlNode itemNode in itemNodeList)
                {
                    DTO_WebOrderItem item = new DTO_WebOrderItem();

                    item.UID = getXMLOrderAttribute(itemNode, @"uid", @"");
                    item.BasePrice = Convert.ToDecimal(getXMLOrderAttribute(itemNode, @"basePrice", @"0.00"));
                    item.Qty = Convert.ToSingle(getXMLOrderAttribute(itemNode, @"quantity", @"0.0"));
                    item.DiscountID = Convert.ToUInt32(getXMLOrderAttribute(itemNode, @"discountID", @"0"));
                    item.Note = getXMLOrderAttribute(itemNode, @"note", @"");
                    item.ItemOptions = readWebOrderItemOptions(itemNode.FirstChild); // Options node
                    item.ItemSides = readWebOrderItemSides(itemNode.FirstChild.NextSibling); // Sides node

                    itemList.Add(item);
                }

                outOrderHeaderAccount.OrderItems = itemList.ToArray();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in readWebOrderItems " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 520, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemOptionsNode"></param>
        /// <returns></returns>
        DTO_WebOrderItemOption[] readWebOrderItemOptions(XmlNode inItemOptionsNode)
        {
            XmlNodeList itemNodeList = XMLDocManager.GetNodeList(inItemOptionsNode, @"Option");

            List<DTO_WebOrderItemOption> itemOptionList = new List<DTO_WebOrderItemOption>();
            foreach (XmlNode optionNode in inItemOptionsNode)
            {
                DTO_WebOrderItemOption option = new DTO_WebOrderItemOption();

                option.UID = getXMLOrderAttribute(optionNode, @"uid", @"");
                option.BasePrice = Convert.ToDecimal(getXMLOrderAttribute(optionNode, @"basePrice", @"0.00"));
                option.Qty = Convert.ToSingle(getXMLOrderAttribute(optionNode, @"quantity", @"0.0"));
                option.DiscountID = Convert.ToUInt32(getXMLOrderAttribute(optionNode, @"discountID", @"0"));
                option.Note = getXMLOrderAttribute(optionNode, @"note", @"");

                itemOptionList.Add(option);
            }

            return itemOptionList.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemSidesNode"></param>
        /// <returns></returns>
        DTO_WebOrderItemSide[] readWebOrderItemSides(XmlNode inItemSidesNode)
        {
            XmlNodeList itemNodeList = XMLDocManager.GetNodeList(inItemSidesNode, @"Option");

            List<DTO_WebOrderItemSide> itemSideList = new List<DTO_WebOrderItemSide>();
            foreach (XmlNode sideNode in inItemSidesNode)
            {
                DTO_WebOrderItemSide side = new DTO_WebOrderItemSide();

                side.UID = getXMLOrderAttribute(sideNode, @"uid", @"");
                side.BasePrice = Convert.ToDecimal(getXMLOrderAttribute(sideNode, @"basePrice", @"0.00"));
                side.Qty = Convert.ToSingle(getXMLOrderAttribute(sideNode, @"quantity", @"0.0"));
                side.DiscountID = Convert.ToUInt32(getXMLOrderAttribute(sideNode, @"discountID", @"0"));
                side.Note = getXMLOrderAttribute(sideNode, @"note", @"");

                itemSideList.Add(side);
            }

            return itemSideList.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderHeaderSection"></param>
        void updateHeaderNode(
                   XmlDocument inXMLDocument,
                   DTO_WebOrderHeader inOrderHeaderSection)
        {
            XmlNode orderNode = XMLDocManager.GetRoot(inXMLDocument);
            orderNode.RemoveChild(XMLDocManager.GetNode(inXMLDocument, @"Header"));

            addHeaderNode(
                inXMLDocument,
                inOrderHeaderSection);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderFromSection"></param>
        void updateFromNode(
                    XmlDocument inXMLDocument,
                    DTO_WebOrderFrom inOrderFromSection)
        {
            
            XmlNode orderNode = XMLDocManager.GetRoot(inXMLDocument);
            orderNode.RemoveChild(XMLDocManager.GetNode(inXMLDocument, @"From"));

            addFromNode(
                inXMLDocument,
                inOrderFromSection);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inOrderAccountSection"></param>
        void updateAccountNode(
                    XmlDocument inXMLDocument,
                    DTO_WebOrderAccount inOrderAccountSection)
        {
            XmlNode orderNode = XMLDocManager.GetRoot(inXMLDocument);
            orderNode.RemoveChild(XMLDocManager.GetNode(inXMLDocument, @"Account"));

            addAccountNode(
                inXMLDocument,
                inOrderAccountSection);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXMLDocument"></param>
        /// <param name="inNode"></param>
        /// <param name="inAttrName"></param>
        /// <param name="inAttrValue"></param>
        void addXMLOrderAttribute(
                    XmlDocument inXMLDocument,
                    XmlNode inNode,
                    string inAttrName,
                    string inAttrValue)
        {
            XMLDocManager.AddAttribute(inXMLDocument, inNode, inAttrName, inAttrValue);
        }

        /// <summary>
        /// NOT USED SO FAR!!!!
        /// </summary>
        /// <param name="inNode"></param>
        /// <param name="inAttrName"></param>
        /// <param name="inAttrValue"></param>
        void setXMLOrderAttribute(
                    XmlDocument inXMLDocument,
                    XmlNode inNode,
                    string inAttrName,
                    string inAttrValue)
        {
            XMLDocManager.SetAttribute(inNode, inAttrName, inAttrValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNode"></param>
        /// <param name="inAttrName"></param>
        /// <param name="inDefaultValue"></param>
        /// <returns></returns>
		string getXMLOrderAttribute
		(
			XmlNode inNode,
			string inAttrName,
			string inDefaultValue)
        {
			string returnValue = inDefaultValue;
			try
			{
				returnValue = XMLDocManager.GetAttribute(inNode, inAttrName, inDefaultValue);
			}
			catch (Exception ex)
			{
				
			}
			return returnValue;
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inValue"></param>
        /// <returns></returns>
        string boolToString(bool inValue)
        {
            return inValue ? "true" : "false";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inValue"></param>
        /// <returns></returns>
        bool stringToBool(string inValue)
        {
            return inValue.ToUpper() == @"TRUE";
        }

        #endregion
    }
}
