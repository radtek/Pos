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
    public class DTO_WebOrderFrom
    {
        #region PUBLIC
        /// <summary>
        /// Web Site ID
        /// </summary>
        [DataMember]
        public string SiteID { get; set; }

        /// <summary>
        /// Web Site name
        /// </summary>
        [DataMember]
        public string SiteName { get; set; }
        #endregion
    }
}
