using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for the order item struct used by ChefMate to store and retrieve order items in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateOrderItem
    {
        #region PUBLIC
        [DataMember]
        public int OrderItemKey{ get; set; }

        [DataMember]
        public int ItemPosDbKey { get; set; }

        [DataMember]
        public int OrderItemPosDbKey { get; set; }

        [DataMember]
        public string ItemName { get; set; }

        [DataMember]
        public string OrderItemStatus { get; set; }

        [DataMember]
        public string OrderItemNote{ get; set; }
        #endregion
    }
}
