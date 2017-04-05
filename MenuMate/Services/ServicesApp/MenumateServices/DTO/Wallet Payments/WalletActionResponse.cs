using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Wallet_Payments
{
    [DataContract]
    public class WalletActionResponse
    {
        [DataMember]
        public bool ResponseSuccessful { get; set; }
        [DataMember]
        public string ResponseMessage { get; set; }
        [DataMember]
        public int ExpiresIn { get; set; }
        [DataMember]
        public string SecurityToken { get; set; }
        [DataMember]
        public string SignKey { get; set; }
        [DataMember]
        public string OrderId { get; set; }
    }
}
