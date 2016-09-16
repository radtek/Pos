using System;
using System.Drawing.Printing;

namespace Chefmate.Infrastructure.Output
{
    class PrinterFactory
    {
        private static PrinterFactory _instance;
        private static volatile object _syncRoot = new Object();
        public static PrinterFactory Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new PrinterFactory();
                    }
                }
                return _instance;
            }
        }

        private PrinterFactory()
        {
        }

        public IPrinter BuildPrinter()
        {
            string printerName = getPrinterName();
            IPrinter printer;
            printer = new EpsonPrinter(printerName, 32);
            return printer;
        }

        private string getPrinterName()
        {
            foreach (string printer in PrinterSettings.InstalledPrinters)
            {
                if (printer.Equals("ChefMatePrinter", StringComparison.InvariantCultureIgnoreCase))
                    return printer;
            }
            var settings = new PrinterSettings();
            return settings.PrinterName;
        }
    }
}
