using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for responses to ChefMate database requests that return lists of integers.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateIntegerListResponse : DTO_ChefMateResponse
    {
        #region PUBLIC
        [DataMember]
        public IList<int> ReturnValue { get; set; }
        #endregion
    }
}
