using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentInstrumentData
    {
        [DataMember]
        public string PaymentInstrumentType { get; set; }
        [DataMember]
        public string ProtectedCardData { get; set; }
        [DataMember]
        public CardData CardData { get; set; }
        [DataMember]
        public CheckData CheckData { get; set; }
        [DataMember]
        public MobileData MobileData { get; set; }
        [DataMember]
        public StoredValueAccountID StoredValueAccountID { get; set; }
    }
}
