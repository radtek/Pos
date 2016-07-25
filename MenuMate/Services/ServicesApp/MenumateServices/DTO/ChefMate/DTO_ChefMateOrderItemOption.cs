using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for the order item option struct used by ChefMate to store and retrieve order item options in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateOrderItemOption
    {
        #region PUBLIC
        [DataMember]
        public int OrderItemOptionKey{ get; set; }

        [DataMember]
        public string OptionName{ get; set; }

        [DataMember]
        public bool IsPlus{ get; set; }
        #endregion
    }
}
