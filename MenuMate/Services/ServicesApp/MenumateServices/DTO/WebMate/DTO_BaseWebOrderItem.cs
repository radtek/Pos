using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.WebMate.DTO
{
    /// <summary>
    /// 
    /// </summary>
    [DataContract]
    [KnownType(typeof(DTO_WebOrderItem))]
    [KnownType(typeof(DTO_WebOrderItemSide))]
    [KnownType(typeof(DTO_WebOrderItemOption))]
    public class DTO_BaseWebOrderItem : DTO_BaseItem
    {
        #region PUBLIC
        [DataMember]
        public Decimal BasePrice { get; set; }
        
        [DataMember]
        public UInt32 DiscountID { get; set; }

        [DataMember]
        public float Qty { get; set; }

        [DataMember]
        public string Note { get; set; } 
        #endregion
    }
}
