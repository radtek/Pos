using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace FiscalLibraries.Domain
{
    
    public class DiscountDetails
    {        
        public string Type { get; set; }
        
        public string Amount { get; set; }
        
        public string Description { get; set; }
        
        public string DiscountGroup { get; set;}
        
        public string DiscountMode { get; set; }
    }
}
