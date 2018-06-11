using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalLibraries.Domain
{
    
    public class FiscalPayment
    {
        
        public string Type { get; set; }
        
        public string Amount { get; set; }
        
        public string Billno { get; set; }
        
        public string Cashno { get; set; }
        
        public string Cashier { get; set; }
        
        public string Source { get; set; }
        
        public string Description { get; set; }
    }
}
