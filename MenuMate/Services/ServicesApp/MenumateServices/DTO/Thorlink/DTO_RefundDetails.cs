using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_RefundDetails
    {
        [DataMember]
        public string CardNo
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
        public double CreditValue
        {
            get;
            set;
        }
    }
}
