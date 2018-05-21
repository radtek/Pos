using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using PaymentSenseIntegration.Domain;

namespace PaymentSenseIntegration.Domain
{
    public class PACTerminalWrapper
    {
        public PACTerminal[] Terminals { get; set; }
    }
}
