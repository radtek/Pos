using System.Collections;
using System.Collections.Generic;

namespace Chefmate.Infrastructure.Output
{
    class DocketLayout : IEnumerable<PrinterInstruction>
    {
        private IList<PrinterInstruction> _printerInstructions;

        public DocketLayout()
        {
            _printerInstructions = new List<PrinterInstruction>();
        }

        public void AddInstruction(PrinterInstruction inInstruction)
        {
            _printerInstructions.Add(inInstruction);
        }

        public IEnumerator<PrinterInstruction> GetEnumerator()
        {
            return _printerInstructions.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public PrinterInstruction GetLastInstruction()
        {
            return _printerInstructions[_printerInstructions.Count - 1];
        }


    }
}
