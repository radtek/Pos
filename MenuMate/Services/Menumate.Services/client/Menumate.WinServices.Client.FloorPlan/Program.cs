﻿using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using NLog;

//using System.Collections.Generic;
//using System.Linq;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.WinServices.Client.FloorPlan
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

        [DllImport("User32.dll")]
        public static extern int ShowWindowAsync(IntPtr hWnd, int swCommand);

        [DllImport("user32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        private static extern bool IsIconic(IntPtr hWnd);

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
        }

        static void setUnhandledExceptionEventHandler()
        {
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            Application.ThreadException += new ThreadExceptionEventHandler(Application_ThreadException);
        }

        static void setMutexObject()
        {
            _appMutex = new Mutex(true, "Menumate.WinServices.Client.Reservations.Gui");
        }

        static void startApp()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.DoEvents();

            Application.Run(new MainForm());
        }

        static void showApp()
        {
            Process currentProcess = Process.GetCurrentProcess();
            Process[] runningProcesses = Process.GetProcessesByName(Process.GetCurrentProcess().ProcessName);

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
                      ShowWindowAsync(p.MainWindowHandle, SW_SHOWNOACTIVATE);
                   }
                   break;
                 }
             }
        }

        static void Application_ThreadException(object sender, ThreadExceptionEventArgs e)
        {
            UnhandledExceptionHandler(e.Exception);
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            UnhandledExceptionHandler((Exception)e.ExceptionObject);
        }

        private static void UnhandledExceptionHandler(Exception e)
        {
            LogManager.GetLogger("Crash").FatalException("Unhandled Exception", e);
            //new ExceptionViewer(e).ShowDialog();
            Application.Exit();
        }

    }
}
