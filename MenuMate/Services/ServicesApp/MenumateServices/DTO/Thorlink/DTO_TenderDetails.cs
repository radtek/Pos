using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public enum TenderTypeThor
    {
        [EnumMember]
        eThorCash,
        [EnumMember]
        eThorDebitCard,
        [EnumMember]
        eThorPoints,
        [EnumMember]
        eThorVoucher,
        [EnumMember]
        eThorCheque
    };

    [DataContract]
    public class DTO_TenderDetails
    {
        [DataMember]
        public string cardNo
        {
            get;
            set;
        }
        [DataMember]
        public string tenderIdentifier
        {
            get;
            set;
        }

        [DataMember]
        public double tenderValue
        {
            get;
            set;
        }
        [DataMember]
        public bool sendTransactionValue
        {
            get;
            set;
        }
        [DataMember]
        public List<DTO_ItemDetails> itemDetails
        {
            get;
            set;
        }
        [DataMember]
        public TenderTypeThor thorTenderType
        {
            get;
            set;
        }
    }
}