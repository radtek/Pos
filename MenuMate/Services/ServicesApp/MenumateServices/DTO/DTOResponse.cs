using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;

namespace MenumateServices.DTO
{
    [DataContract]
    public class DTOResponse
    {
        private UInt32 _deviceID;
        private UInt16 _code;
        private string _msg;
        private List<DTOPrompt> _promptList = new List<DTOPrompt>();

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt32 DeviceID
        {
            get { return _deviceID; }
            set { _deviceID = value; }
        }

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

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTOPrompt[] PromptList
        {
            get { return _promptList.ToArray<DTOPrompt>(); }
            set 
            { 
                _promptList.Clear();

                foreach(DTOPrompt prompt in value)
                {
                    _promptList.Add(prompt);
                } 
            }
        }
    }
}
