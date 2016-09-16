using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO.LoyaltyMate
{
    public enum LoyaltyResponseCode
    {
        Successful = 0,
        AuthenticationFailed,
        CreateMemberFailed,
        UpdateMemberFailed,
        DeleteMemberFailed,
        GetMemberFailed,
        PostTransactionFailed,
        MemberNotExist,
        CompanySyncFailed,
        InvalidGiftVoucher,
        InvalidPocketVoucher,
        TransactionFailed,
        GetGiftCardFailed,
        GetPocketVoucherFailed
    };

    [DataContract]
    public class LoyaltyResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;

        internal LoyaltyResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
        }

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

    }
}
