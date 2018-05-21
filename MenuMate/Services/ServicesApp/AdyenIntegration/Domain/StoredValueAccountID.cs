using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class StoredValueAccountID
    {
        [DataMember]
        public string StoredValueAccountType { get; set; }
        [DataMember]
        public string StoredValueProvider { get; set; }
        [DataMember]
        public string OwnerName { get; set; }
        [DataMember]
        public string ExpiryDate { get; set; }
        [DataMember]
        public string EntryMode { get; set; }
        [DataMember]
        public string IdentificationType { get; set; }
        [DataMember]
        public string StoredValueID { get; set; }
    }
}
