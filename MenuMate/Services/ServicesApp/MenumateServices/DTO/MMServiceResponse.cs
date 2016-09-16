using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO
{
    /// <summary>
    /// Describes a WebMate Order Response
    ///     Succesful:    True if the function succeeds 
    ///     Message:      Response's message
    ///     Description:  Detailed description of the response's message
    ///     ResponseCode: The transaction's response code.  Take a look at "enum ResponseCode"
    /// </summary>
    [DataContract]
    public class MMServiceResponse
    {
        bool _succesful;
        string _message;
        string _description;

        internal MMServiceResponse(bool inSuccesful,string inMessage,string inDescription)
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
