using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for the Order Item Side struct used by ChefMate to store and retrieve Order Item Sides in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateOrderItemSide
    {
        #region PUBLIC
        [DataMember]
        public int OrderItemSideKey{ get; set; }

        [DataMember]
        public int OrderItemSidePosKey{ get; set; }

        [DataMember]
        public string SideName{ get; set; }

        [DataMember]
        public bool IsCanceled{ get; set; }
        #endregion
    }
}
