using System;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class MemberInfo
    {
        [DataMember]
        public string Title { get; set; }
        [DataMember]
        public string FirstName { get; set; }
        [DataMember]
        public string MiddleName { get; set; }
        [DataMember]
        public string LastName { get; set; }
        [DataMember]
        public string Email { get; set; }
        [DataMember]
        public string Mobile { get; set; }
        [DataMember]
        public int MemberType { get; set; }
        [DataMember]
        public string PhoneNumber { get; set; }
        [DataMember]
        public string UniqueId { get; set; }
        [DataMember]
        public string Address1 { get; set; }
        [DataMember]
        public string Address2 { get; set; }
        [DataMember]
        public string City { get; set; }
        [DataMember]
        public string State { get; set; }
        [DataMember]
        public string ZipCode { get; set; }
        [DataMember]
        public string Country { get; set; }
        [DataMember]
        public string CardNumber { get; set; }
        [DataMember]
        public string MemberCardCode { get; set; }
        [DataMember]
        public bool Activated { get; set; }
        [DataMember]
        public Nullable<DateTime> DateOfBirth { get; set; }
        [DataMember]
        public Nullable<DateTime> BirthdayBenefitDate { get; set; }
        [DataMember]
        public Nullable<DateTime> LastVisitDate { get; set; }
        [DataMember]
        public Nullable<DateTime> YearStartDate { get; set; }
        [DataMember]
        public double EarnedPoints { get; set; }
        [DataMember]
        public double LoadedPoints { get; set; }
        [DataMember]
        public long TierId { get; set; }
        [DataMember]
        public long HomeSiteId { get; set; }
        [DataMember]
        public double CurrentYearPoint { get; set; }
        [DataMember]
        public double PreviousYearPoint { get; set; }
        [DataMember]
        public double AvailableBirthDayPoint { get; set; }
        [DataMember]
        public double AvailableFirstVisitPoint { get; set; }
        [DataMember]
        public int Gender { get; set; }
        [DataMember]
        public string MembershipNumber { get; set; }
        [DataMember]
        public int PointRule { get; set; }
        [DataMember]
        public DateTime LastModified { get; set; }
        [DataMember]
        public bool IsFirstVisitRewarded { get; set; }
    }
}
