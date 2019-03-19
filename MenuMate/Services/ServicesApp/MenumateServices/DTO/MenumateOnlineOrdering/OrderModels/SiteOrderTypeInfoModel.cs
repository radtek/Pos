using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
{
    [DataContract]
    public class SiteOrderTypeInfoModel
    {
        [DataMember]
        public long SiteId { get; set; }
        [DataMember]
        public SiteSettingType SiteTransactionType { get; set; }
        [DataMember]
        public string SiteTransactionTypeName { get; set; }
        [DataMember]
        public string Text { get; set; }
    }
}
