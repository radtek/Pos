using System;
using System.Windows.Forms;
using SystemTrayApp.SignalR;

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
        [STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

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
	}
}