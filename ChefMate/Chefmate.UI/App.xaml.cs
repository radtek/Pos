using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using Chefmate.Infrastructure.Utility;
using Chefmate.UI.Views;
using Application = System.Windows.Application;

namespace Chefmate.UI
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void App_OnStartup(object sender, StartupEventArgs e)
        {
            try
            {
                if (!CheckChefmateRunning())
                {
                    Current.Exit += Current_Exit;
                    var mainWindow = new MainWindow();
                    mainWindow.Show();
                }
                else
                {
                    Application.Current.Shutdown();
                }
            }
            catch (Exception)
            {
                // ignored
            }
        }

        private void Current_Exit(object sender, ExitEventArgs e)
        {
            try
            {
                ChefmateInitializer.Stop();
            }
            catch (Exception)
            {
                // ignored
            }
        }

        private bool CheckChefmateRunning()
        {
            var process = Process.GetProcessesByName("Chefmate");
            return process.Count() > 1;
        }


    }
}
