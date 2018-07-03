using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class Instalment
    {
        [DataMember]
        public string InstalmentType { get; set; }
        [DataMember]
        public int SequenceNumber { get; set; }
        [DataMember]
        public string PlanID { get; set; }
        [DataMember]
        public int Period { get; set; }
        [DataMember]
        public string PeriodUnit { get; set; }
        [DataMember]
        public string FirstPaymentDate { get; set; }
        [DataMember]
        public int MyPropTotalNbOfPaymentserty { get; set; }
        [DataMember]
        public double CumulativeAmount { get; set; }
        [DataMember]
        public double FirstAmount { get; set; }
        [DataMember]
        public double Charges { get; set; }
    }
}
