using System;
using System.Diagnostics;
using System.Linq;
using System.Windows;
using Chefmate.Core;
using Chefmate.Core.Enums;
using Chefmate.Infrastructure.Controller;
using Chefmate.Infrastructure.Utility;
using Chefmate.Logger;
using Chefmate.UI.Views;
using ChefMate.Database;
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
                Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
                if (!CheckChefmateRunning())
                {
                    Current.Exit += Current_Exit;
                    InitializeChefmate();
                    var mainWindow = new MainWindow();
                    mainWindow.Show();
                }
                else
                {
                    Application.Current.Shutdown();
                }
            }
            catch (Exception ex)
            {
                // ignored
            }
        }
        private void InitializeChefmate()
        {
            try
            {
                if (!UI.Properties.Settings.Default.TerminalInitialized)
                {
                    InitializeTerminal();
                }
                else
                {
                    bool result = DatabaseCore.Instance.InitializeDatabase(UI.Properties.Settings.Default.DatabaseAddress, UI.Properties.Settings.Default.DatabasePath);
                    if (result)
                    {
                        CheckVersionInformation();
                        bool terminalExist = ChefmateController.Instance.LoadTerminal(Environment.MachineName);
                        if (!terminalExist)
                        {
                            InitializeTerminal();
                        }
                    }
                    else
                    {
                        InitializeTerminal();
                    }
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("InitializeChefmate", ex.Message);
            }

        }
        private void InitializeTerminal()
        {
            try
            {
                StartupView startupView = new StartupView();
                startupView.ShowInTaskbar = false;
                startupView.Topmost = true;
                startupView.ShowDialog();
                if (startupView.ModalResult == System.Windows.Forms.DialogResult.Cancel)
                {
                    Application.Current.Shutdown();
                }
                else
                {
                    UI.Properties.Settings.Default.DatabaseAddress = startupView.DbIpAddress;
                    UI.Properties.Settings.Default.DatabasePath = startupView.DbPath;
                    UI.Properties.Settings.Default.TerminalInitialized = true;
                    DatabaseCore.Instance.InitializeDatabase(UI.Properties.Settings.Default.DatabaseAddress, UI.Properties.Settings.Default.DatabasePath);
                    ChefmateController.Instance.AddTerminal(startupView.DisplayName, startupView.TerminalIpAddress, startupView.DbIpAddress, startupView.DbPath);
                    UI.Properties.Settings.Default.Save();
                    CheckVersionInformation();
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("InitializeTerminal", ex.Message);
            }
        }
        private void CheckVersionInformation()
        {
            try
            {
                string versionErrorMessage = "";
                var versionMode = VersionChecker.Instance.CheckVersionInformation(ChefmateConstants.DatabaseVersion, ref versionErrorMessage);
                if (versionMode == DatabaseMode.Correct)
                {
                    ChefmateInitializer.Start();
                }
                else
                {
                    ChefmateController.Instance.ShowMessageBox("Version Error", versionErrorMessage);
                    var chefmateParser = new ChefmateParserView(UI.Properties.Settings.Default.DatabaseAddress, UI.Properties.Settings.Default.DatabasePath);
                    chefmateParser.ShowInTaskbar = false;
                    chefmateParser.Topmost = true;
                    chefmateParser.ShowDialog();
                    ChefmateInitializer.Start();

                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("CheckVersionInformation", ex.Message);
                Application.Current.Shutdown();
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
