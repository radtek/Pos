using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalLibraries.Domain
{
    
    public class FiscalService
    {
        
        public string TableNo { get; set; }
        
        public string GuestName { get; set; }
        
        public string ChitNumber { get; set; }
        
        public string MemberName { get; set; }
        
        public string PartyName { get; set; }
        
        public string ItemDescription { get; set; }
        
        public string SizeName { get; set; }
        
        public string quantity { get; set; }
        
        public string ItemCategory { get; set; }
        
        public string PricePerUnit { get; set; }
        
        public string PriceTotal { get; set; }
        
        public string VATPercentage { get; set; }
    }
}
