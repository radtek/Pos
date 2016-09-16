using System;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    /// <summary>
    /// DTO for member transactions
    /// </summary>
    [DataContract]
    public class TransactionInfo
    {
        [DataMember]
        public string UniqueId { get; set; }
        [DataMember]
        public DateTime TransactionDate { get; set; }
        [DataMember]
        public double PointsDelta { get; set; }
        [DataMember]
        public int PointsType { get; set; }
        [DataMember]
        public long SiteCode { get; set; }
        [DataMember]
        public string InvoiceNumber { get; set; }
    }
}
