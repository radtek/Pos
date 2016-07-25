using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for the order struct used by ChefMate to store and retrieve order in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateOrder
    {
        #region PUBLIC
        [DataMember]
        public int OrderDbKey { get; set; }

        [DataMember]
        public int OrderNumber { get; set; }

        [DataMember]
        public int OrderPosDbKey { get; set; }

        [DataMember]
        public int PatronCount { get; set; }

        [DataMember]
        public string ChitValue { get; set; }

        [DataMember]
        public string TableTabName { get; set; }

        [DataMember]
        public string PartyName { get; set; }

        [DataMember]
        public string ServerName { get; set; }

        [DataMember]
        public string CustomerName { get; set; }

        [DataMember]
        public string OrderType { get; set; }

        [DataMember]
        public DateTime SaleStartTime { get; set; }

        [DataMember]
        public DateTime SaleFinishTime { get; set; }

        [DataMember]
        public DateTime DeliveryTime { get; set; }
        #endregion
    }
}
