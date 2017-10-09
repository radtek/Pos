using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyMemberResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        MemberInfo _memberInfo;

        internal LoyaltyMemberResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode,
                        MemberInfo inMemberInfo)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _memberInfo = createMemberInfo(inMemberInfo);
        }


        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public MemberInfo MemberInfo
        {
            get { return _memberInfo; }
            set { _memberInfo = value; }
        }

        MemberInfo createMemberInfo(MemberInfo inMemberInfo)
        {
            var result = new MemberInfo();
            result.Activated = inMemberInfo.Activated;
            result.Mobile = inMemberInfo.Mobile;
            result.DateOfBirth = inMemberInfo.DateOfBirth;
            result.EarnedPoints = inMemberInfo.EarnedPoints;
            result.Email = inMemberInfo.Email;
            result.LoadedPoints = inMemberInfo.LoadedPoints;
            result.UniqueId = inMemberInfo.UniqueId;
            result.Title = inMemberInfo.Title;
            result.FirstName = inMemberInfo.FirstName;
            result.LastName = inMemberInfo.LastName;
            result.Address1 = inMemberInfo.Address1;
            result.Address2 = inMemberInfo.Address2;
            result.City = inMemberInfo.City;
            result.State = inMemberInfo.State;
            result.ZipCode = inMemberInfo.ZipCode;
            result.Country = inMemberInfo.Country;
            result.BirthdayBenefitDate = inMemberInfo.BirthdayBenefitDate;
            result.TierId = inMemberInfo.TierId;
            result.LastVisitDate = inMemberInfo.LastVisitDate;
            result.HomeSiteId = inMemberInfo.HomeSiteId;
            result.YearStartDate = inMemberInfo.YearStartDate;
            result.CardNumber = inMemberInfo.CardNumber;
            result.MemberType = inMemberInfo.MemberType;
            result.PhoneNumber = inMemberInfo.PhoneNumber;
            result.CurrentYearPoint = inMemberInfo.CurrentYearPoint;
            result.PreviousYearPoint = inMemberInfo.PreviousYearPoint;
            result.AvailableBirthDayPoint = inMemberInfo.AvailableBirthDayPoint;
            result.AvailableFirstVisitPoint = inMemberInfo.AvailableFirstVisitPoint;
            result.MemberCardCode = inMemberInfo.MemberCardCode;
            result.MembershipNumber = inMemberInfo.MembershipNumber;
            result.PointRule = inMemberInfo.PointRule;
            result.LastModified = inMemberInfo.LastModified;
            result.IsFirstVisitRewarded = inMemberInfo.IsFirstVisitRewarded;

            result.MemberVouchers = new List<VoucherInfo>();
            if (inMemberInfo.MemberVouchers != null)
                foreach (var memberVoucher in inMemberInfo.MemberVouchers)
                {
                    var voucher = new VoucherInfo();
                    voucher.DiscountCode = memberVoucher.DiscountCode;
                    voucher.VoucherName = memberVoucher.VoucherName;
                    voucher.VoucherDescription = memberVoucher.VoucherDescription;
                    voucher.NumberOfUsesRemaining = memberVoucher.NumberOfUsesRemaining;
                    result.MemberVouchers.Add(voucher);
                }
            return result;
        }

    }
}
