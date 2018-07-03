using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalLibraries.Domain
{
    
    public class FiscalResponseDetails
    {
        
        public bool IsSuccessful { get; set; }
        
        public string Response { get; set; }
    }
}
