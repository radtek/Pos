using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.ServiceProcess;

using System.ServiceModel;

using MenumateServices.WCFServices;

namespace MenumateServices
{
    /// <summary>
    /// List of Menumate's WCF Services
    /// </summary>
    enum WebServicesID { PosIntegration = 1, Reservations, EFTPOS, Xero, WebMate };

    /// <summary>
    ///  WCF Service loader
    ///  It's used in all the ServiceWebWrapper_ Classes derived from ServiceWebWrapper Class
    /// </summary>
    class WebServicesLoader
    {
        //:::::::::::::::::::::::::::::::::

        WebServicesLoader()
        {
        }

        public static WebServicesLoader Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly WebServicesLoader instance = new WebServicesLoader();
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// Load all WCF Services: The Service IDs are in an XML configuration file
        /// </summary>
        /// <param name="inConfigFilename"></param>
        /// <returns></returns>
        public List<ServiceHost> LoadWebServices(string inConfigFilename)
        {
            try
            {
                return LoadWebServicesFromXML(ServiceInfo.GetXMLDocFromFile(inConfigFilename));
            }
            catch (System.Xml.XmlException xe)
            {
                string message = String.Format("Error loading Menumate.WinServices config file: {0} (problem with XML format?)", inConfigFilename);
                ServiceLogger.LogException(message, xe);

                return defaultWebService();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException("Error loading Menumate.WinServices config file", e);

                return defaultWebService();
            }
        }

        //:::::::::::::::::::::::::::::::::

        public List<ServiceHost> defaultWebService()
        {
            List<ServiceHost> result = new List<ServiceHost>();

            return result;
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// Load all WCF Services: The Service IDs are in an XmlDocument object
        /// </summary>
        /// <param name="inDoc"></param>
        /// <returns></returns>
        public List<ServiceHost> LoadWebServicesFromXML(XmlDocument inDoc)
        {
            List<ServiceHost> result = new List<ServiceHost>();

            //::::::::::::::::::::::::::::::::::::::

            XmlNodeList serviceNodeList = null;
            string serviceName = "";

            serviceNodeList = inDoc.DocumentElement.SelectNodes("WebServices/WebService");

            foreach (XmlNode serviceNode in serviceNodeList)
            {
                serviceName = serviceNode.Attributes["name"].Value;

                try
                {
                    string loadThisService = serviceNode.Attributes["load"].Value;

                    if (loadThisService.ToUpper() == @"YES")
                    {
                        result.Add(LoadWebService(strToServiceID(serviceNode.Attributes["id"].Value)));
                        ServiceLogger.Log(String.Format(@"Web Service {0} created.", serviceName));
                    }
                }
                catch (Exception e)
                {
                    ServiceLogger.LogException(String.Format(@"Error loading Web Service {0}. Does this file really exist?", serviceName), e);
                }
            }

            if (result.Count == 0)
            {
                //result.Add(new ServiceBase());
                ServiceLogger.Log(@"No Menumate Web Services to be loaded.");
            }

            return result;
        }

        //:::::::::::::::::::::::::::::::::

        public WebServicesID strToServiceID(string inStr)
        {
            return (WebServicesID)Convert.ToInt16(inStr);
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// Load a WCF Service with a given ServiceID
        /// </summary>
        /// <param name="inServiceID"></param>
        /// <returns></returns>
        public ServiceHost LoadWebService(WebServicesID inServiceID)
        {
            switch (inServiceID)
            {
                //case WebServicesID.PosIntegration: return new ServiceHost(typeof(PosIntegrationService));
                //case WebServicesID.Reservations: return new ServiceHost(typeof(ReservationsService));
                //case WebServicesID.Xero:   return new ServiceHost(typeof(WCFServiceXero));

                case WebServicesID.EFTPOS: return new ServiceHost(typeof(WCFServiceEFTPOS));
                case WebServicesID.WebMate: return new ServiceHost(typeof(WCFServiceWebMate));

                default: throw new Exception(String.Format("Unknown Web Service: {0}", inServiceID));
            }
        }

        //:::::::::::::::::::::::::::::::::

    }
}
