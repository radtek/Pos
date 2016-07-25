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
    [KnownType(typeof(DTO_BaseWebOrderItem))]
    public class DTO_BaseItem
    {
        #region PUBLIC
        [DataMember]
        public string UID { get; set; }
        #endregion
    }
}
