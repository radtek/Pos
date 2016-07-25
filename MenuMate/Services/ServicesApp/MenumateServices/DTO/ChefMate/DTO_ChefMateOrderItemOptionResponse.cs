using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for responses to ChefMate database requests that return order item option stucts.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateOrderItemOptionResponse : DTO_ChefMateResponse
    {
        #region PUBLIC
        [DataMember]
        public DTO_ChefMateOrderItemOption ReturnValue { get; set; }
        #endregion
    }
}
