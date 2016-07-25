using System;
using System.Collections.Generic;
using System.Configuration.Install;
using System.Reflection;
using System.ServiceProcess;

using Menumate.WinServices.Tools;

namespace Menumate.WinServices
{
    static class Program
    {
        /*
         * We use callbacks so that we can avoid having a single
         * gigantic function. Hopefully this'll make the code here
         * more maintainable and easier for us to grok.
         */
        delegate void CmdHandler(ServiceController svc_ctrl);

        static void install_svc(ServiceController svc_ctrl)
        {
            ManagedInstallerClass.InstallHelper(
              new string[]{"/i", Assembly.GetCallingAssembly().Location});
            Console.WriteLine(
              "The \"Menumate PosIntegration and Reservations\" service has been installed.");
        }

        static void remove_svc(ServiceController svc_ctrl)
        {
            ManagedInstallerClass.InstallHelper(
              new string[]{"/u", Assembly.GetCallingAssembly().Location});
            Console.WriteLine(
              "The \"Menumate PosIntegration and Reservations\" service has been removed.");
        }

        static void start_svc(ServiceController svc_ctrl)
        {
            svc_ctrl.Start();
            Console.WriteLine(
              "The \"Menumate PosIntegration and Reservations\" service has been started.");
        }

        static void stop_svc(ServiceController svc_ctrl)
        {
            if (svc_ctrl.Status != ServiceControllerStatus.Running) {
                Console.WriteLine("The \"Menumate PosIntegration and Reservations\" service has already been stopped.");
                return;
            }

            svc_ctrl.Stop();
            Console.WriteLine(
              "The \"Menumate PosIntegration and Reservations\" service has been stopped.");
        }

        static ServiceController get_svc_if_present()
        {
            ServiceController[] svcs = ServiceController.GetServices();

            foreach (ServiceController svc in svcs) {
                if (svc.ServiceName == "MMWinService")
                    return svc;
            }

            return null;
        }

        static void parse_cmd_str(string cmd_str)
        {
            /*
             * These arrays are used instead of a Dictionary or List as the
             * number of commands is incredibly small and thus a linear
             * search of string comparisons will be instant. Besides, it's
             * reasonably straight forward :)
             */
            string[] cmds = new string[] {
                "--install",
                "--remove",
                "--start",
                "--stop"
            };
            CmdHandler[] cmd_hdlrs = new CmdHandler[]{
                install_svc, remove_svc, start_svc, stop_svc
            };
            ServiceController svc_ctrl = get_svc_if_present();
            bool[] req_inst_status = new bool[]{
                false,
                true,
                true,
                true
            };
            string[] err_msgs = new string[4]{
                "The \"Menumate PosIntegration and Reservations\" service is already installed.",
                "The \"Menumate PosIntegration and Reservations\" service is not installed.",
                "The \"Menumate PosIntegration and Reservations\" service could not be started.",
                "The \"Menumate PosIntegration and Reservations\" service could not be stopped."
            };

            for (int i = 0; i < cmds.Length; i++) { 
                if (cmd_str == cmds[i]) {
                    if ((svc_ctrl != null) == req_inst_status[i])
                        cmd_hdlrs[i](svc_ctrl);
                    else Console.WriteLine(err_msgs[i]);
                    return;
                }
            }

            Console.WriteLine("Invalid command.");
            display_usage();
        }

        static void display_usage()
        {
        }

        static void Main(string[] argv)
        {
            if (System.Environment.UserInteractive)
            {
                if (argv.Length == 1)
                    parse_cmd_str(argv[0]);
                else
                    display_usage();
            }
            else
            {
                ServiceBase.Run(new MMWinService());
            }
        }
    }
}
