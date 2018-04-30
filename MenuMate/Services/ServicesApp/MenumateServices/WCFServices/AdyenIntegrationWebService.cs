using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.WCFServices
{
    public class AdyenIntegrationWebService : IAdyenIntegrationWebService
    {
        public AdyenIntegrationWebService()
        { 
        }
        public bool MakeNormalSale()
        { 
            return true;
        }

        public bool MakeRefundSale()
        {
            return true;
        }

        public bool GetTransactionStatus()
        {
            return true;
        }
    }
}
