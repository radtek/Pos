using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO.MenumateOnlineOrdering
{
    [DataContract]
    public class OOServiceResponse
    {
        bool _succesful;
        string _message;
        string _description;

        internal OOServiceResponse(bool inSuccesful,string inMessage,string inDescription)
        {
            _succesful   = inSuccesful;
            _message     = inMessage;
            _description = inDescription;
        }

        [DataMember]
        bool Successful
        {
            set { _succesful = value; }
            get { return _succesful; }
        }

        [DataMember]
        string Message
        {
            set { _message = value; }
            get { return _message; }
        }

        [DataMember]
        string Description
        {
            set { _description = value; }
            get { return _description; }
        }

    } 
}
