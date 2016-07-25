using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.ServiceProcess;

using System.ServiceModel;
using Menumate.WinServices.Implementation;

namespace Menumate.WinServices.Tools
{
    enum WebServicesID { PosIntegration = 1, Reservations };

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

        static public List<ServiceHost> LoadWebServices(string inConfigFilename)
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

        static public List<ServiceHost> defaultWebService()
        {
            List<ServiceHost> result = new List<ServiceHost>();

            //result.Add(new MMPosIntegrationWinService());

            return result;
        }

        //:::::::::::::::::::::::::::::::::

        static public List<ServiceHost> LoadWebServicesFromXML(XmlDocument inDoc)
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

        static public WebServicesID strToServiceID(string inStr)
        {
            return (WebServicesID)Convert.ToInt16(inStr);
        }

        //:::::::::::::::::::::::::::::::::

        static public ServiceHost LoadWebService(WebServicesID inServiceID)
        {
            switch (inServiceID)
            {
                case WebServicesID.PosIntegration: return new ServiceHost(typeof(PosIntegrationService));
                case WebServicesID.Reservations: return new ServiceHost(typeof(ReservationsService));
                default: throw new Exception(String.Format("Unknown Web Service: {0}", inServiceID));
            }
        }

        //:::::::::::::::::::::::::::::::::

    }
}
