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
        double _giftCardBalance;

        internal LoyaltyGiftCardResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode,
                        double giftCardBalance)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _giftCardBalance = giftCardBalance;
        }

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public double GiftCardBalance
        {
            get { return _giftCardBalance; }
            set { _giftCardBalance = value; }
        }
    
    }
}
