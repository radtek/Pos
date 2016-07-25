namespace Chefmate.Infrastructure.Output
{
    class EpsonPrinter : IPrinter
    {
        private const string PrinterNewLine = "\r\n";
        private const char   Gs               = '\x1D';
        private const char   CutMode         = '\x56';
        private const char   PartialCut      = '\x42';
        private const char   Terminate        = '\x00';
        private const char   SingleLine      = '\xC4';
        private const char   DoubleLine      = '\xCD';

        private readonly string _printerName;
        private readonly int _printerLineWidth;

        public EpsonPrinter(string inPrinterName, int inPrinterLineWidth)
        {
            _printerName = inPrinterName;
            _printerLineWidth = inPrinterLineWidth;
        }

        public void Print(DocketLayout inDocketLayout)
        {
            string rawPrinterInstructions = "";

            foreach(PrinterInstruction printInstruction in inDocketLayout)
            {
                rawPrinterInstructions += FormatInstruction(printInstruction);
            }

            RawPrinterHelper.SendStringToPrinter(_printerName, rawPrinterInstructions);
        }

        private string FormatInstruction(PrinterInstruction inPrintInstruction)
        {
            string formattedInstruction = "";
            if (inPrintInstruction.DrawLineBefore)
            {
                formattedInstruction += Line();
            }
            formattedInstruction += inPrintInstruction.Value.Replace("\n", PrinterNewLine);
            formattedInstruction += PrinterNewLine;
            if (inPrintInstruction.DrawLineAfter)
            {
                formattedInstruction += Line();
            }
            if (inPrintInstruction.CutAfter)
            {
                formattedInstruction += Cut();
            }

            return formattedInstruction;
        }

        private string Line()
        {
            return new string(SingleLine, _printerLineWidth) + PrinterNewLine;
        }

        private string Cut()
        {
            return   PrinterNewLine 
                   + PrinterNewLine 
                   + PrinterNewLine 
                   + PrinterNewLine 
                   + Gs + CutMode + PartialCut + Terminate 
                   + PrinterNewLine;
        }
    }
}
