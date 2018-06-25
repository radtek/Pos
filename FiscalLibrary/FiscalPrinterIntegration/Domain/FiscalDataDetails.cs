using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalLibraries.Domain
{
    
    public class FiscalDataDetails
    {
        
        public string InvoiceNumber { get; set; }
        
        public List<FiscalService> ItemList { get; set; }
        
        public List<FiscalPayment> PaymentList { get; set; }
        
        public List<DiscountDetails> DiscountList { get; set; }
        
        public string Date { get; set; }
        
        public string Time { get; set; }
        
        public string Billno { get; set; }
        
        public string Cashno { get; set; }
        
        public string Cashier { get; set; }
        
        public string Source { get; set; }
    }
}
