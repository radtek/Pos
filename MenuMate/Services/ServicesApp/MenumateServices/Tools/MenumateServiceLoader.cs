using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceProcess;
using System.Xml;

using System.Configuration;

namespace MenumateServices
{
    class MenumateServiceLoader
    {
        MenumateServiceLoader()
        { }

        public static MenumateServiceLoader Instance
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
            { }

            internal static readonly MenumateServiceLoader instance = new MenumateServiceLoader();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public List<ServiceBase> LoadServices()
        {
            var serviceList = new List<ServiceBase>();

            AddService(serviceList, MenumateServicesIdentifier.AccountingIntegration); // Standard Windows Service: AccountingIntegration
            AddService(serviceList, MenumateServicesIdentifier.LoyaltyMate);     // WCF Service: LoyaltyMate
            AddService(serviceList, MenumateServicesIdentifier.WebMate);         // WCF Service: WebMate
            AddService(serviceList, MenumateServicesIdentifier.ClippIntegration); // WCF Service: Clipp
            AddService(serviceList, MenumateServicesIdentifier.SmartLink);
            AddService(serviceList, MenumateServicesIdentifier.ThorLink);
            AddService(serviceList, MenumateServicesIdentifier.PocketVoucher);
            AddService(serviceList, MenumateServicesIdentifier.SalesForceIntegration);
            AddService(serviceList, MenumateServicesIdentifier.SiHotIntegration);
            AddService(serviceList, MenumateServicesIdentifier.WalletPayments);
            if (serviceList.Count == 0)
            {
                ServiceLogger.Log(@"No Menumate Services to be loaded.");
            }

            return serviceList;
        }

        private void AddService(List<ServiceBase> services, MenumateServicesIdentifier serviceIdentifier)
        {
            try
            {
                services.Add(LoadService(serviceIdentifier));
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(String.Format(@"Error loading Service {0}. Does this file really exist?", serviceName(serviceIdentifier)), e);
            }
        }

        public List<ServiceBase> LoadServices(string configFilename)
        {
            ServiceLogger.Log(string.Format(@"Loading Config File: {0} ...", configFilename));
            try
            {
                return LoadServicesFromXML(ServiceInfo.Instance.GetXmlDocumentFromFile(configFilename));
            }
            catch (XmlException xe)
            {
                string message = String.Format("Error loading Service Loader config file: {0} (problem with XML format?)", configFilename);
                ServiceLogger.LogException(message, xe);

                return defaultService();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException("Error loading Service Loader config file", e);

                return defaultService();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <returns></returns>
        public List<ServiceBase> LoadServicesFromXML(XmlDocument inDoc)
        {
            List<ServiceBase> result = new List<ServiceBase>();

            //::::::::::::::::::::::::::::::::::::::

            XmlNodeList serviceNodeList = null;
            string serviceName = "";

            serviceNodeList = inDoc.DocumentElement.SelectNodes("Services/Service");

            foreach (XmlNode serviceNode in serviceNodeList)
            {
                serviceName = serviceNode.Attributes["name"].Value;

                try
                {
                    bool loadThisService = serviceNode.Attributes["load"].Value.ToUpper() == @"YES";
                    bool asWebService = serviceNode.Attributes["asWebService"].Value.ToUpper() == @"YES";

                    if (loadThisService)
                    {
                        result.Add(LoadService(strToServiceID(serviceNode.Attributes["id"].Value)));
                    }
                }
                catch (Exception e)
                {
                    ServiceLogger.LogException(String.Format(@"Error loading Service {0}. Does this file really exist?", serviceName), e);
                }
            }

            if (result.Count == 0)
            {
                //result.Add(new ServiceBase());
                ServiceLogger.Log(@"No Menumate Services to be loaded.");
            }

            return result;
        }

        public ServiceBase LoadService(MenumateServicesIdentifier serviceIdentifier)
        {
            ServiceLogger.Log(string.Format(@"Loading Service: {0} ...", serviceName(serviceIdentifier)));

            switch (serviceIdentifier)
            {
                case MenumateServicesIdentifier.AccountingIntegration:
                    return new MenumateServiceAccounting();
                case MenumateServicesIdentifier.LoyaltyMate:
                    return new MenumateServiceLoyaltyMate();
                case MenumateServicesIdentifier.WebMate:
                    return new MenumateServiceWebMate();
                case MenumateServicesIdentifier.ClippIntegration:
                    return new MenumateServiceClippIntegration();
                case MenumateServicesIdentifier.SmartLink:
                    return new MenumateServiceSmartLink();
                case MenumateServicesIdentifier.ThorLink:
                    return new MenumateServiceThorlink();
                case MenumateServicesIdentifier.PocketVoucher:
                    return new MenumateServicePocketVoucher();
                case MenumateServicesIdentifier.SalesForceIntegration:
                    return new MenumateServiceSalesForce();
                case MenumateServicesIdentifier.SiHotIntegration:
                    return new MenumateServiceSiHotIntegration();
                case MenumateServicesIdentifier.WalletPayments:
                    return new MenumateServiceWalletPayments();
                default: throw new Exception(String.Format("Unknown Service: {0}", serviceIdentifier));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        private string serviceName(MenumateServicesIdentifier inID)
        {
            return ServiceNameManager.Instance.ServiceNameWithID(inID);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public List<ServiceBase> defaultService()
        {
            List<ServiceBase> result = new List<ServiceBase>();

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStr"></param>
        /// <returns></returns>
        public MenumateServicesIdentifier strToServiceID(string inStr)
        {
            return (MenumateServicesIdentifier)Convert.ToInt16(inStr);
        }
    }
}
