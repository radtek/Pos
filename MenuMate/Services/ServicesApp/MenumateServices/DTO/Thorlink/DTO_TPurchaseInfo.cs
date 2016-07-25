using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_TPurchaseInfo
    {
        [DataMember]
        public int ResponseCode
        {
            get;
            set;
        }

        [DataMember]
        public string ResponseMessage
        {
            get;
            set;
        }
        [DataMember]
        public string CardNumber
        {
            get;
            set;
        }
        [DataMember]
        public double CreditValue
        {
            get;
            set;
        }
        [DataMember]
        public double TransactionValue
        {
            get;
            set;
        }
        [DataMember]
        public double LoyaltyValue
        {
            get;
            set;
        }
        [DataMember]
        public string TransactionNumber
        {
            get;
            set;
        }
    }
}
