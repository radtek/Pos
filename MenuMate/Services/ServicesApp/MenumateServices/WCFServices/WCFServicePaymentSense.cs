using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using PaymentSenseIntegration.Domain;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServicePaymentSense" in both code and config file together.
    public class WCFServicePaymentSense : IWCFServicePaymentSense
    {
        public List<CardTerminal> GetAllCardTerminals(AuthorizationDetails autorizationDetails)
        {
            List<CardTerminal> terminalList = new List<CardTerminal>();
            return terminalList;
        }
    }
}
