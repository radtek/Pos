using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PocketVoucherIntegration.RedemptionDetails
{
    [DataContract]
    public class VoucherRedemptionDetails
    {
        [DataMember]
        public string VoucherCode
        {
            get;
            set;
        }

        [DataMember]
        public string VoucherAmount
        {
            get;
            set;
        }

        [DataMember]
        public string TransactionAmount
        {
            get;
            set;
        }

        [DataMember]
        public string MerchantId
        {
            get;
            set;
        }

        [DataMember]
        public string MerchantReference
        {
            get;
            set;
        }
        
        [DataMember]
        public string TerminalId
        {
            get;
            set;
        }

        [DataMember]
        public string UserName
        {
            get;
            set;
        }

        [DataMember]
        public string Password
        {
            get;
            set;
        }
    }
}

