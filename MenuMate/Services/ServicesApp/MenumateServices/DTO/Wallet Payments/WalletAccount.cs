using System.Runtime.Serialization;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.Wallet_Payments
{
    [DataContract]
    public class WalletAccount
    {
        [DataMember]
        public int WalletType { get; set; }
        [DataMember]
        public string MerchentId { get; set; }
        [DataMember]
        public string TerminalId { get; set; }
        [DataMember]
        public string Password { get; set; }
        [DataMember]
        public string UserName { get; set; }
        [DataMember]
        public string SignKey { get; set; }
    }
}
