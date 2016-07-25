using System;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public  class PointsInfo
    {
        [DataMember]
        public DateTime EndDate { get; set; }
        [DataMember]
        public double Balance { get; set; }
        [DataMember]
        public int PointsType { get; set; }
        [DataMember]
        public DateTime StartDate { get; set; }
        [DataMember]
        public string UniqueId { get; set; }
    }
}
