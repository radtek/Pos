using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class VoucherTransactionResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        DateTime? _giftCardExpiryDate;

        internal VoucherTransactionResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode,
                        DateTime? giftCardExpiryDate)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _giftCardExpiryDate = giftCardExpiryDate;
        }


        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public DateTime? GiftCardExpiryDate
        {
            get { return _giftCardExpiryDate; }
            set { _giftCardExpiryDate = value; }
        }


    }
}
