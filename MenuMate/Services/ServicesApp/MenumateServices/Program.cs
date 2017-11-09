using System;
using System.Linq;
using System.ServiceProcess;
using System.Configuration.Install;
using System.Reflection;

namespace MenumateServices
{
    static class Program
    {
        /// <summary>
        /// Three menumate services: Accounting Integration, LoyaltyMate, WebMate, ClippIntegration, ThorLink, PocketVoucher, SalesForce,sihot,walletpayments
        /// </summary>
        private const int ServiceCount = 10;
        static string _serviceConfigFileName = Properties.Settings.Default.ServiceConfigFilename;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] argv)
        {
            if (Environment.UserInteractive)
            {
                RunWithParams(argv);
            }
            else
            {
                RunService();
            }
        }

        static void RunService()
        {
            ServiceBase[] servicesToRun = MenumateServiceLoader.Instance.LoadServices().ToArray<ServiceBase>();
            ServiceBase.Run(servicesToRun);
        }

        static void RunWithParams(string[] argv)
        {
            if (argv.Length == 1)
                ParseParam(argv[0]);
            else
                DisplayUsage();
        }

        /*
            We use callbacks so that we can avoid having a single
            gigantic function. Hopefully this'll make the code here
            more maintainable and easier for us to grok.
        */
        delegate void CommandHandler(ServiceController serviceController);

        static void ParseParam(string inParam)
        {
            /*
                These arrays are used instead of a Dictionary or List as the
                number of commands is incredibly small and thus a linear
                search of string comparisons will be instant. Besides, it's
                reasonably straight forward
            */
            var commands = new[] 
            {
                "--install",
                "--remove",
                "--start",
                "--stop"
            };

            var commandHandlers = new CommandHandler[]
            {
                InstallServiceEndPoint, RemoveServiceEndPoint, StartServiceEndPoint, StopServiceEndPoint
            };

            ServiceController serviceController = null;

            for (int i = 0; i < commands.Length; i++)
            {
                if (inParam == commands[i])
                {
                    commandHandlers[i](serviceController);
                    return;
                }
            }

            Console.WriteLine("Invalid command.");
            DisplayUsage();
        }

        static void DisplayUsage()
        { }

        static void InstallServiceEndPoint(ServiceController serviceController)
        {
            try
            {
                Console.WriteLine(string.Format(@"Installing Service App: {0} ...", Assembly.GetCallingAssembly().Location));
                ServiceLogger.Log(string.Format(@"Installing Service App: {0} ...", Assembly.GetCallingAssembly().Location));

                ManagedInstallerClass.InstallHelper(new string[] { "/i", Assembly.GetCallingAssembly().Location });

                Console.WriteLine("The \"Menumate Service App\" service has been installed.");
                ServiceLogger.Log("The \"Menumate Service App\" service has been installed.");
            }
            catch (Exception exc)
            {
                Console.WriteLine(string.Format(@"Failed to install Service App: {0}: {1}",
                                    Assembly.GetCallingAssembly().Location,
                                    exc.Message));

                ServiceLogger.Log(string.Format(@"Failed to install Service App: {0}: {1}",
                                    Assembly.GetCallingAssembly().Location,
                                    exc.Message));
            }
        }

        static void RemoveServiceEndPoint(ServiceController serviceController)
        {
            try
            {
                Console.WriteLine(string.Format(@"Removing Service App: {0} ...", Assembly.GetCallingAssembly().Location));
                ServiceLogger.Log(string.Format(@"Removing Service App: {0} ...", Assembly.GetCallingAssembly().Location));

                ManagedInstallerClass.InstallHelper(new string[] { "/u", Assembly.GetCallingAssembly().Location });

                Console.WriteLine("The \"Menumate Service App\" service has been removed.");
                ServiceLogger.Log("The \"Menumate Service App\" service has been removed.");
            }
            catch (Exception exc)
            {
                Console.WriteLine(string.Format(@"Failed to uninstall Service App: {0}: {1}",
                                    Assembly.GetCallingAssembly().Location,
                                    exc.Message));

                ServiceLogger.Log(string.Format(@"Failed to uninstall Service App: {0}: {1}",
                                    Assembly.GetCallingAssembly().Location,
                                    exc.Message));
            }
        }

        static void StartServiceEndPoint(ServiceController serviceController)
        {
            ServiceInfo serviceInfo = ServiceInfo.Instance;
            ServiceLogger.Log(string.Format(@"Menumate Services: Loading config file: {0}", serviceInfo.CinfigurationFilePath()));

            ServiceController[] svcs = ServiceController.GetServices();

            int srvCounter = 0;
            foreach (ServiceController svc in svcs)
            {
                if (IsMenumateService(svc.ServiceName))
                {
                    srvCounter++;

                    try
                    {
                        svc.Start();
                    }
                    catch (Exception exc)
                    {
                        Console.WriteLine(string.Format(@"Failed to start Service: {0}: {1}",
                                            svc.DisplayName,
                                            exc.Message));

                        ServiceLogger.Log(string.Format(@"Failed to start Service: {0}: {1}",
                                            svc.DisplayName,
                                            exc.Message));
                    }

                    if (srvCounter == ServiceCount)
                    {
                        break;
                    }
                }
            }
            Console.WriteLine("The \"Menumate Service App\" service has started.");
            ServiceLogger.Log("The \"Menumate Service App\" service has started.");
        }

        static void StopServiceEndPoint(ServiceController serviceController)
        {
            ServiceController[] serviceControllers = ServiceController.GetServices();

            int srvCounter = 0;
            foreach (ServiceController svc in serviceControllers)
            {
                if (IsMenumateService(svc.ServiceName))
                {
                    srvCounter++;

                    try
                    {
                        svc.Stop();
                    }
                    catch (Exception exc)
                    {
                        Console.WriteLine(string.Format(@"Failed to stop Service: {0}: {1}",
                                            svc.DisplayName,
                                            exc.Message));

                        ServiceLogger.Log(string.Format(@"Failed to stop Service: {0}: {1}",
                                            svc.DisplayName,
                                            exc.Message));
                    }

                    if (srvCounter == ServiceCount)
                    {
                        break;
                    }
                }
            }

            Console.WriteLine("The \"Menumate Service App\" service has been stopped.");
            ServiceLogger.Log("The \"Menumate Service App\" service has been stopped.");
        }

        static bool IsMenumateService(string inServiceName)
        {
            return (inServiceName == "MenumateServiceAccounting") ||
                   (inServiceName == "MenumateServiceLoyaltyMate") ||
                   (inServiceName == "MenumateServiceWebMate") ||
                   (inServiceName == "MenumateServiceSmartLink") ||
                   (inServiceName == "MenumateServiceClippIntegration") ||
                   (inServiceName == "MenumateServiceThorlink") ||
                   (inServiceName == "MenumateServicePocketVoucher") ||
                   (inServiceName == "MenumateServiceSalesForce") ||
                   (inServiceName == "MenumateServiceSiHotIntegration")||
                   (inServiceName == "MenumateServiceWalletPayments");
        }

        static ServiceController GetServiceEndPoint()
        {
            ServiceController[] svcs = ServiceController.GetServices();
            return svcs.FirstOrDefault(svc => svc.ServiceName == "MenumateServiceAccounting");
        }

    }
}
