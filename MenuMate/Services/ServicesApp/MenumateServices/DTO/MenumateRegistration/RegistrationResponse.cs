using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO.MenumateRegistration
{
    public enum RegistrationResponseCode
    {
        Successful,
        AuthenticationFailed,
        RegistrationUpdateFailed,
        CompanyInfoValidationFailed
    };

    [DataContract]
    public class RegistrationResponse : MMServiceResponse
    {
        RegistrationResponseCode _responseCode;

        internal RegistrationResponse(bool inSuccesful, string inMessage, string inDescription, RegistrationResponseCode inResponseCode)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
        }

        [DataMember]
        public RegistrationResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

    }
}
