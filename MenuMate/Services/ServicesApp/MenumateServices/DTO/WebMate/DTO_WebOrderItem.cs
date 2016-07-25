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
    public class DTO_WebOrderItem : DTO_BaseWebOrderItem
    {
        #region PUBLIC
        [DataMember]
        public DTO_WebOrderItemOption[] ItemOptions;

        [DataMember]
        public DTO_WebOrderItemSide[] ItemSides;

        public DTO_WebOrderItem()
        {
            ItemOptions = new DTO_WebOrderItemOption[] { };
            ItemSides = new DTO_WebOrderItemSide[] { };
        }

        #endregion
    }
}
