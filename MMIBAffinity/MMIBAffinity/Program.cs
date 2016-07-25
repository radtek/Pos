using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.ComponentModel;

namespace MMIBAffinity {
    class Program {
        static void Main(string[] args) {


            Process[] interbaseProcesses = Process.GetProcessesByName("ibserver");

            if (interbaseProcesses.Length != 1) {
                throw new InvalidOperationException("Found more than 1 interbase server process; stopping");
            }


            try {
                Console.WriteLine(string.Format("Current ibserver affinity is set to {0}",
                    interbaseProcesses[0].ProcessorAffinity));
                Console.WriteLine("Setting ibserver affinity to first processor...");
                interbaseProcesses[0].ProcessorAffinity = (System.IntPtr)1;       // use first core
                Console.WriteLine("done");

            }
            catch (NotSupportedException nse) {
                Console.WriteLine(string.Format("Error setting affinity;{0}{1}",
                    Environment.NewLine, nse.ToString()));
            }
            catch (InvalidOperationException ioe) {
                Console.WriteLine(string.Format("Error setting affinity;{0}{1}",
                    Environment.NewLine, ioe.ToString()));
            }
            catch (Win32Exception w32e) {
                Console.WriteLine(string.Format("Error setting affinity;{0}{1}",
                    Environment.NewLine, w32e.ToString()));
            }
        }
    }
}
