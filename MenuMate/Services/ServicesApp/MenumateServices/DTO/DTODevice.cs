using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;

namespace MenumateServices.DTO
{
    [DataContract]
    public class DTODevice
    {
        private UInt16 _type;
        private UInt32 _id;
        private string _address;
        private UInt16 _port;

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt16 Type
        {
            get { return _type; }
            set { _type = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt32 ID
        {
            get { return _id;  }
            set { _id = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string Address
        {
            get { return _address;  }
            set { _address = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt16 Port
        {
            get { return _port;  }
            set { _port = value; }
        }
    }
}
