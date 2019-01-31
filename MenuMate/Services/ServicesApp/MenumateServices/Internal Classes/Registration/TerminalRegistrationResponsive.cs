using System.Collections.Generic;
using System;
using MenumateServices.DTO.MenumateRegistration;

namespace MenumateServices.Registration
{
    public class TerminalRegistrationResponsive
    {
        protected RegistrationResponse CreateResponseNoError()
        {
            return CreateResponse(true, "", "", RegistrationResponseCode.Successful);
        }

        protected RegistrationResponse CreateResponseError(string inMessage, string inDescription, RegistrationResponseCode inResponseCode)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode);
        }


        #region Private Methods

        private RegistrationResponse CreateResponse(bool inSuccesful, string inMessage, string inDescription, RegistrationResponseCode inResponseCode)
        {
            return new RegistrationResponse(inSuccesful, inMessage, inDescription, inResponseCode);
        }      
       
        #endregion
    }
}
