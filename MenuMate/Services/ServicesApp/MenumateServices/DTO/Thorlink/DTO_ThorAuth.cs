using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_ThorAuth
    {
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string appKey
        {
            get;
            set;
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string siteNo
        {
            get;
            set;
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string merchantCode
        {
            get;
            set;
        }
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string deviceCode
        {
            get;
            set;
        }
    }
}
