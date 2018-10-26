using System;
using System.Windows.Forms;
using SystemTrayApp.SignalR;
using System.Collections.Generic;
using System.Net.NetworkInformation;
using Microsoft.Win32;
using System.IO;

namespace SystemTrayApp
{
	/// <summary>
	/// 
	/// </summary>
	static class Program
	{
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        private static List<string> logsList;
        [STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
            logsList = new List<string>();
			// Show the system tray icon.					
			using (ProcessIcon pi = new ProcessIcon())
			{
				pi.Display();
			    Client.SetNotifyIcon(pi.GetNotifyIcon());
                Client.InitializeClient();
                // Make sure the application runs!
                Application.Run();
			}
		}
        static void AvailabilityChanged(object sender, NetworkAvailabilityEventArgs e)
        {
            try
            {
                if (e.IsAvailable)
                {
                    logsList.Add("Calling for connect on internet connectivity");
                    Client.InitializeClient();
                }
            }
            catch (Exception exception)
            {

            }
            MakeLogFile();
        }

        static void SystemEvents_PowerModeChanged(object sender, PowerModeChangedEventArgs e)
        {
            try
            {
                if (e.Mode == PowerModes.Suspend)
                {
                    logsList.Add("Calling for disconnect on suspend");
                    Client.Disconnect();
                }
                else if (e.Mode == PowerModes.Resume)
                {
                    logsList.Add("Calling for connect on resume");
                    Client.InitializeClient();
                }
            }
            catch (Exception exception)
            {
            }
            MakeLogFile();
        }



        static void MakeLogFile()
        {
            try
            {
                logsList.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Signal Client Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "SignalR " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
                string fileName = Path.Combine(location, name2);

                if (fileName.Contains(@"file:\"))
                {
                    fileName = fileName.Replace(@"file:\", "");
                }
                if (!File.Exists(fileName))
                {

                    using (StreamWriter sw = File.CreateText(fileName))
                    {
                        for (int i = 0; i < logsList.Count; i++)
                        {
                            sw.WriteLine(logsList[i]);
                        }
                    }
                }
                else
                {
                    using (var sw = File.AppendText(fileName))
                    {
                        for (int i = 0; i < logsList.Count; i++)
                        {
                            sw.WriteLine(logsList[i]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
            }
            logsList.Clear();
        }
	}
}