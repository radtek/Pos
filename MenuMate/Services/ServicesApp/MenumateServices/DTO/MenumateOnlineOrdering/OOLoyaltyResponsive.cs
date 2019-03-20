using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.MenumateOnlineOrdering
{
    public class OOLoyaltyResponsive
    {
        protected OOLoyaltyResponse CreateResponseNoError()
        {
            return CreateResponse(true, "", "", OOLoyaltyResponseCode.Successful);
        }

        protected OOLoyaltyResponse CreateResponseError(string inMessage, string inDescription, OOLoyaltyResponseCode inResponseCode)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode);
        }

        #region Private Methods

        private OOLoyaltyResponse CreateResponse(bool inSuccesful, string inMessage, string inDescription, OOLoyaltyResponseCode inResponseCode)
        {
            return new OOLoyaltyResponse(inSuccesful, inMessage, inDescription, inResponseCode);
        }

        #endregion
    }
}
