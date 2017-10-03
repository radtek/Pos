using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyVoucherResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        VoucherInfo _voucherInfo;

        internal LoyaltyVoucherResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode,
                        VoucherInfo inVoucherInfo)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _voucherInfo = CreateVoucherInfo(inVoucherInfo);
        }


        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public VoucherInfo VoucherInfo
        {
            get { return _voucherInfo; }
            set { _voucherInfo = value; }
        }

        VoucherInfo CreateVoucherInfo(VoucherInfo inVoucherInfo)
        {
            var result = new VoucherInfo();
            result.DiscountCode = inVoucherInfo.DiscountCode;
            result.VoucherName = inVoucherInfo.VoucherName;
            result.VoucherDescription = inVoucherInfo.VoucherDescription;
            result.NumberOfUsesRemaining = inVoucherInfo.NumberOfUsesRemaining;
            return result;
        }

    }
}
