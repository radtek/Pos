using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using MenumateServices.DTO.LoyaltyMate;
using Loyaltymate.Sevices;

namespace MenumateServices.Internal_Classes.LoyaltyMate
{
    public class LoyaltyOnlineOrdering
    {
        private static volatile LoyaltyOnlineOrdering _instance;
        private static object syncRoot = new Object();

        private LoyaltyOnlineOrdering()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static LoyaltyOnlineOrdering Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new LoyaltyOnlineOrdering();
                    }
                }

                return _instance;
            }
        }
        public LoyaltyOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int siteCode)
        {
            return GetOrderingInformation(inSyndicateCode, siteCode);
        }

        LoyaltyOnlineOrderingResponse GetOrderingInformation(string inSyndicateCode, int siteCode)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetOnlineOrderingInformation(inSyndicateCode, siteCode);
                return CreateOrderingResponseNoError(response.IsSuccessful,response.Message);

            }
            catch (Exception exc)
            {
                return CreateOrderingResponseError("Unsuccessful sync for online ordering.");
            }
            return new LoyaltyOnlineOrderingResponse();
        }

        private LoyaltyOnlineOrderingResponse CreateOrderingResponseError(string inMessage)
        {
            return new LoyaltyOnlineOrderingResponse
            {
                IsSuccessful = false,
                ResponseText = inMessage
            };
        }
        private LoyaltyOnlineOrderingResponse CreateOrderingResponseNoError(bool inSuccesful, string inMessage)
        {
            return new LoyaltyOnlineOrderingResponse
            {
                IsSuccessful = inSuccesful,
                ResponseText = inMessage
            };
        }
    }
}
