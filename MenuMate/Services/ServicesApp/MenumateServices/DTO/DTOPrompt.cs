using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;

namespace MenumateServices.DTO
{
    [DataContract]
    public class DTOPrompt
    {
        private UInt16 _code;
        private string _msg;

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt16 Code
        {
            get { return _code; }
            set { _code = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string Msg
        {
            get { return _msg; }
            set { _msg = value; }
        }
    }
}
