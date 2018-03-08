using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalPrinterIntegration.Domain
{
    [DataContract]
    public class FiscalDataDetails
    {
        [DataMember]
        public string InvoiceNumber { get; set; }
        [DataMember]
        public List<FiscalService> ItemList { get; set; }
        [DataMember]
        public List<FiscalPayment> PaymentList { get; set; }
        [DataMember]
        public List<DiscountDetails> DiscountList { get; set; }
        [DataMember]
        public string Date { get; set; }
        [DataMember]
        public string Time { get; set; }
        [DataMember]
        public string Billno { get; set; }
        [DataMember]
        public string Cashno { get; set; }
        [DataMember]
        public string Cashier { get; set; }
        [DataMember]
        public string Source { get; set; }
    }
}
