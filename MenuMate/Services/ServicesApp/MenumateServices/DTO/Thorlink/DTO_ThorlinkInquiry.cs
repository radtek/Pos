using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_ThorlinkInquiry
    {
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string cardNo
        {
            get;
            set;
        }
    }
}
