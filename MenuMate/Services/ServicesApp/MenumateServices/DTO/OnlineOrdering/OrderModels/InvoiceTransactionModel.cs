using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.Enum;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.OnlineOrdering.OrderModels
{
    [DataContract]
    public class InvoiceTransactionModel
    {
        [DataMember]
        public long InvoiceTransactionId { get; set; }
        [DataMember]
        public string InvoiceNumber { get; set; }
        [DataMember]
        public double TotalSaleAmount { get; set; }
        [DataMember]
        public DateTime TransactionDate { get; set; }
        [DataMember]
        public long SiteId { get; set; }
        [DataMember]
        public string TerminalName { get; set; }
        [DataMember]
        public byte[] Receipt { get; set; }
        [DataMember]
        public string ReceiptPath { get; set; }
        [DataMember]
        public double Rounding { get; set; }
        [DataMember]
        public long UserReferenceId { get; set; }
        [DataMember]
        public UserType UserType { get; set; }
    }
}
