using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace Menumate_Second_Display
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            var exeutingFolder = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
           var logoimagepath = Path.Combine(exeutingFolder, "images\\FirstVisit");
           if (!Directory.Exists(logoimagepath))
            Directory.CreateDirectory(logoimagepath);

           logoimagepath = Path.Combine(exeutingFolder, "images\\Tiers");
           if (!Directory.Exists(logoimagepath))
               Directory.CreateDirectory(logoimagepath);

           logoimagepath = Path.Combine(exeutingFolder, "images\\Birthday");
           if (!Directory.Exists(logoimagepath))
               Directory.CreateDirectory(logoimagepath);

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
