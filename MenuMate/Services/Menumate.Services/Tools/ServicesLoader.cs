using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.ServiceProcess;

namespace Menumate.WinServices.Tools
{
    enum ServicesID { PosIntegration = 1, Reservations };

    class ServicesLoader
    {
        //:::::::::::::::::::::::::::::::::

        ServicesLoader() 
        {   
        }

        public static ServicesLoader Instance
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

            internal static readonly ServicesLoader instance = new ServicesLoader();
        }

        //:::::::::::::::::::::::::::::::::

        static public List<ServiceBase> LoadServices(string inConfigFilename)
        {
            try
            {
               return LoadServicesFromXML(ServiceInfo.GetXMLDocFromFile(inConfigFilename));
            }
            catch (System.Xml.XmlException xe)
            {
                string message = String.Format("Error loading Menumate.WinServices config file: {0} (problem with XML format?)", inConfigFilename);
                ServiceLogger.LogException(message, xe);

                return defaultService();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException("Error loading Menumate.WinServices config file", e);

                return defaultService();
            }
        }

        //:::::::::::::::::::::::::::::::::

        static public List<ServiceBase> defaultService()   
        {
            List<ServiceBase> result = new List<ServiceBase>();

            result.Add(new MMPosIntegrationWinService());

            return result;
        }

        //:::::::::::::::::::::::::::::::::

        static public List<ServiceBase> LoadServicesFromXML(XmlDocument inDoc)
        {
            List<ServiceBase> result = new List<ServiceBase>();

            //::::::::::::::::::::::::::::::::::::::
            
            XmlNodeList serviceNodeList = null;
            string      serviceName     = "";

            serviceNodeList = inDoc.DocumentElement.SelectNodes("Services/Service");

            foreach (XmlNode serviceNode in serviceNodeList)
            {
                serviceName = serviceNode.Attributes["name"].Value;

                try
                {
                    string loadThisService = serviceNode.Attributes["load"].Value;

                    if (loadThisService.ToUpper() == @"YES" )
                    {
                        result.Add(LoadService(strToServiceID(serviceNode.Attributes["id"].Value)));
                        ServiceLogger.Log(String.Format(@"Service {0} created.", serviceName));
                    }
                }
                catch (Exception e)
                {
                    ServiceLogger.LogException(String.Format(@"Error loading service {0}. Does this file really exist?", serviceName), e);
                }
            }

            if (result.Count == 0)
            {
                result.Add(new ServiceBase());
                ServiceLogger.Log(@"No Menumate Services to be loaded.");
            }

            return result;
        }

        //:::::::::::::::::::::::::::::::::

        static public ServicesID strToServiceID(string inStr)
        {
            return (ServicesID)Convert.ToInt16(inStr);
        }

        //:::::::::::::::::::::::::::::::::

        static public ServiceBase LoadService(ServicesID inServiceID)
        {
            switch (inServiceID)
            {
                case ServicesID.PosIntegration: return new MMPosIntegrationWinService();
                case ServicesID.Reservations: return new MMReservationsWinService();
                default: throw new Exception(String.Format("Unknown service: {0}", inServiceID));
            }
        }

        //:::::::::::::::::::::::::::::::::

    }
}
