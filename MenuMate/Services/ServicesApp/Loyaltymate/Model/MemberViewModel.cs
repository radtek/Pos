using System;
using System.Collections.Generic;
using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    [Serializable]
    public class MemberViewModel
    {
        public long MemberId { get; set; }
        public MemberType MemberType { get; set; }
        public string Title { get; set; }
        public string FirstName { get; set; }
        public string MiddleName { get; set; }
        public string LastName { get; set; }
        public string Email { get; set; }
        public string Mobile { get; set; }
        public string Phone { get; set; }
        public Gender Gender { get; set; }
        public DateTime? AnniversaryDate { get; set; }
        public DateTime? Birthday { get; set; }
        public string AddressLine1 { get; set; }
        public string AddressLine2 { get; set; }
        public string CityName { get; set; }
        public string StateName { get; set; }
        public string CountryName { get; set; }
        public string PostalCode { get; set; }
        public List<MembershipProfileViewModel> MembershipProfiles { get; set; }
        public string CardCode { get; set; }
        public Guid UniqueId { get; set; }
        public DateTime LastModified { get; set; }
        
    }
}