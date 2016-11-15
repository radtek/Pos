using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyGiftCardResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        GiftCardInfo _giftCardInfo;

        internal LoyaltyGiftCardResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode,
                        GiftCardInfo giftCardInfo)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _giftCardInfo = giftCardInfo;
        }

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public GiftCardInfo GiftCardInfo
        {
            get { return _giftCardInfo; }
            set { _giftCardInfo = value; }
        }
    
    }
}
