using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
{
    [DataContract]
    public class SiteMenuInfo
    {
        [DataMember]
        public long CompanyId { get; set; }
        [DataMember]
        public long SiteId { get; set; }
        [DataMember]
        public List<MenuConsumableInfo> MenuConsumables { get; set; }
    }
}
