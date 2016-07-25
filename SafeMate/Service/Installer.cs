//#define __Safemate_Service_Debug__

using System;
using System.ComponentModel;
using SCII = System.Configuration.Install;
using System.ServiceProcess;
using System.Reflection;
using System.Collections.Generic;

namespace Safemate.Service
{
#if ! __Safemate_Service_Debug__
    [RunInstaller(true)]
    public class Installer : SCII.Installer
    {
        public Installer()
        {
            ServiceInstaller si = new ServiceInstaller();
            ServiceProcessInstaller spi = new ServiceProcessInstaller();

            si.ServiceName = "SafemateBackupService";
            si.DisplayName = "Safemate Backup Service";
            si.StartType = ServiceStartMode.Automatic;
            si.Description =
                "Periodically backs up the Menumate databases to user-defined locations.";

            spi.Account = ServiceAccount.NetworkService;

            Installers.Add(si);
            Installers.Add(spi);
        }
    };

    public class ServiceRunner
    {
        enum RunnerStatusCodes : int {
            OperationSuccessful,
            InsufficientParameters,
            InvalidParameter,
            ServiceIsAlreadyInstalled,
            ServiceIsntInstalled
        }

        delegate void OptionHandler();

        static void InstallHandler()
        {
            installedServices = ServiceController.GetServices();

            foreach (ServiceController s in installedServices)
                if (s.ServiceName == "SafemateBackupService") {
                    Console.WriteLine("Service already installed.");
                    status = RunnerStatusCodes.ServiceIsAlreadyInstalled;
                    return;
                }

            SCII.ManagedInstallerClass.InstallHelper(
                new String[] {"/i", Assembly.GetExecutingAssembly().Location});
        }

        static void RemoveHandler()
        {
            installedServices = ServiceController.GetServices();

            foreach (ServiceController s in installedServices)
                if (s.ServiceName == "SafemateBackupService") {
                    StopHandler();
                    SCII.ManagedInstallerClass.InstallHelper(
                        new String[] {"/u", Assembly.GetExecutingAssembly().Location});
                    return;
                }

            Console.WriteLine("Service not installed.");
            status = RunnerStatusCodes.ServiceIsntInstalled;
        }

        static void RestartHandler()
        {
            StopHandler();
            StartHandler();
        }

        static void StartHandler()
        {
            installedServices = ServiceController.GetServices();

            foreach (ServiceController s in installedServices)
                if (s.ServiceName == "SafemateBackupService") {
                    s.Start();
                    s.WaitForStatus(ServiceControllerStatus.Running);
                    return;
                }

            Console.WriteLine("Service not installed.");
            status = RunnerStatusCodes.ServiceIsntInstalled;
        }

        static void StopHandler()
        {
            installedServices = ServiceController.GetServices();

            foreach (ServiceController s in installedServices)
                if (s.ServiceName == "SafemateBackupService") {
                    if (s.Status == ServiceControllerStatus.Running) {
                        s.Stop();
                        s.WaitForStatus(ServiceControllerStatus.Stopped);
                    }
                    return;
                }

            Console.WriteLine("Service not installed.");
            status = RunnerStatusCodes.ServiceIsntInstalled;
        }

        static void ShowUsageMessage()
        {
            Console.WriteLine(
                @"service [option]
    Options:
        --install-service: Installs the service if it has not already been
                           registered with the system.
         --remove-service: Removes the service from the system. The service
                           must have been registered with the system for this
                           to complete successfully.
        --restart-service: Restarts the Safemate service if it is active.
                           If the service is not active, it will be started.
          --start-service: Starts the Safemate service if not already active.
           --stop-service: Shuts down the service if it's active.");
        }

        static public int Main(String[] argv)
        {
            status = RunnerStatusCodes.OperationSuccessful;

            if (System.Environment.UserInteractive) {
                Dictionary<String, OptionHandler> optionMap =
                    new Dictionary<String, OptionHandler>();

                optionMap.Add("--install-service", InstallHandler);
                optionMap.Add("--remove-service", RemoveHandler);
                optionMap.Add("--restart-service", RestartHandler);
                optionMap.Add("--start-service", StartHandler);
                optionMap.Add("--stop-service", StopHandler);

                if (argv.Length != 1) {
                    ShowUsageMessage();
                    status = RunnerStatusCodes.InsufficientParameters;
                } else if (argv.Length == 1) {
                    installedServices = null;

                    try {
                        optionMap[argv[0]]();
                    } catch {
                        Console.WriteLine("Invalid parameter.");
                        ShowUsageMessage();
                        status = RunnerStatusCodes.InvalidParameter;
                    }
                }
            } else ServiceBase.Run(new Service());

            return (int)status;
        }

        static ServiceController[] installedServices;
        static RunnerStatusCodes status;
    };
#endif
};
