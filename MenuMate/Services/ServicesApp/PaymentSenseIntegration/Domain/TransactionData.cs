using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using PaymentSenseIntegration.Enums;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class TransactionData
    {
        [DataMember]
        public string[] Notifications { get; set; }
        [DataMember]
        public string AmountBase { get; set; }
        [DataMember]
        public string AmountCashBack { get; set; }
        [DataMember]
        public string AmountGratuity { get; set; }
        [DataMember]
        public string AmountTotal { get; set; }
        [DataMember]
        public string ApplicationId { get; set; }
        [DataMember]
        public string ApplicationLabel { get; set; }
        [DataMember]
        public string AuthCode { get; set; }
        [DataMember]
        public string CardSchemeName { get; set; }
        [DataMember]
        public string CardholderVerificationMethod { get; set; }
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public string DateOfExpiry { get; set; }
        [DataMember]
        public string DateOfStart { get; set; }
        [DataMember]
        public string PaymentMethod { get; set; }
        [DataMember]
        public string PrimaryAccountNumber { get; set; }
        [DataMember]
        public string PrimaryAccountNumberSequence { get; set; }
        [DataMember]
        public ReceiptLines ReceiptLines { get; set; }
        [DataMember]
        public string TransactionId { get; set; }
        [DataMember]
        public string RequestId { get; set; }
        [DataMember]
        public string TransactionNumber { get; set; }
        [DataMember]
        public string TransactionResult { get; set; }
        [DataMember]
        public string TransactionTime { get; set; }
        [DataMember]
        public string TransactionType { get; set; }
        [DataMember]
        public string Location { get; set; }
    }
}
