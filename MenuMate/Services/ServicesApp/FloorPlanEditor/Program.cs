using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace FloorPlanIntegration
{
    static class Program
    {
        #region Mutex

        /// <summary>
        /// Win32 API Constants for ShowWindowAsync()
        /// </summary>

        private const int SW_HIDE = 0;
        private const int SW_SHOWNORMAL = 1;
        private const int SW_SHOWMINIMIZED = 2;
        private const int SW_SHOWMAXIMIZED = 3;
        private const int SW_SHOWNOACTIVATE = 4;
        private const int SW_RESTORE = 9;
        private const int SW_SHOWDEFAULT = 10;

        [DllImport("user32.dll")]
        public static extern int ShowWindowAsync(IntPtr hWnd, int swCommand);

        [DllImport("user32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        private static extern bool IsIconic(IntPtr hWnd);

        [DllImport("user32.dll")]
        static extern bool CloseWindow(IntPtr hWnd);

        private static Mutex _appMutex;

        #endregion

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            setUnhandledExceptionEventHandler();
            setMutexObject(); // _appMutex

            if (_appMutex.WaitOne(0, false))
            {
                startApp();
            }
            else
            {
                showApp();
            }

            /*
                        Application.EnableVisualStyles();
                        Application.SetCompatibleTextRenderingDefault(false);
                        Application.Run(new MainForm());
            */
        }

        /// <summary>
        /// 
        /// </summary>
        static void setUnhandledExceptionEventHandler()
        {
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            Application.ThreadException += new ThreadExceptionEventHandler(Application_ThreadException);
        }

        /// <summary>
        /// 
        /// </summary>
        static void setMutexObject()
        {
            _appMutex = new Mutex(true, "Menumate FloorPlan Editor");
        }

        /// <summary>
        /// 
        /// </summary>
        static void startApp()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.DoEvents();

            Application.Run(new MainForm());
        }

        /// <summary>
        /// 
        /// </summary>
        static void showApp()
        {
            Process currentProcess = Process.GetCurrentProcess();
            Process[] runningProcesses = Process.GetProcessesByName(currentProcess.ProcessName);

            foreach (Process p in runningProcesses)
            {
                // find the other process
                if (p.Id != currentProcess.Id)
                {
                    SetForegroundWindow(p.MainWindowHandle);

                    if (IsIconic(p.MainWindowHandle))
                    {
                        ShowWindowAsync(p.MainWindowHandle, SW_RESTORE);
                    }
                    else
                    {
                        //MessageBox.Show(string.Format(@"Summa Proccess ID: {0}", p.Id));

                        //Application.OpenForms[0].Show();
                        ShowWindowAsync(p.MainWindowHandle, SW_SHOWMAXIMIZED);
                    }
                    break;
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        static void Application_ThreadException(object sender, ThreadExceptionEventArgs e)
        {
            UnhandledExceptionHandler(e.Exception);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            UnhandledExceptionHandler((Exception)e.ExceptionObject);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="e"></param>
        private static void UnhandledExceptionHandler(Exception e)
        {
/*
            _singletonLogger.LogException(FileLogger.LogErrorLevel.AppFailedToStart,
                                          FileLogger.LogErrorLevelFile.None,
                                          @"Unhandled Exception",
                                          e);
*/
            Application.Exit();
        }
    }
}
