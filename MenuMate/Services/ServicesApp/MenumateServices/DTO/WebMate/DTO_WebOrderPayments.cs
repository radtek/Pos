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
    public class DTO_WebOrderPayments
    {
        #region PUBLIC
        [DataMember]
        public bool PaymentRequired { get; set; }
        #endregion
    }
}
