using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for responses to ChefMate database requests that return date times.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateDateTimeResponse : DTO_ChefMateResponse
    {
        #region PUBLIC
        [DataMember]
        public DateTime ReturnValue { get; set; }
        #endregion
    }
}
