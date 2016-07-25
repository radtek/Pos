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
    public class DTO_WebOrderHeader
    {
        #region PUBLIC
        [DataMember]
        public string StoreName { get; set; }

        [DataMember]
        public string GUID { get; set; }

        [DataMember]
        public UInt16 Scheduled { get; set; }

        [DataMember]
        public bool Respond { get; set; }

        [DataMember]
        public DateTime OrderDate { get; set; }

        [DataMember]
        public DateTime ExpectedDate { get; set; }

        [DataMember]
        public Decimal OrderTotal { get; set; }
        #endregion
    }
}
