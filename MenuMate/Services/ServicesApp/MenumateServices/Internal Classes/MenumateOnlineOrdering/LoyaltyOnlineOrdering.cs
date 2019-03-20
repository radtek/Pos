using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using MenumateServices.DTO.MenumateOnlineOrdering;
using OnlineOrdering.Services;

namespace MenumateServices.Internal_Classes.MenumateOnlineOrdering
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
        public LoyaltyOOResponse GetOnlineOrderingInformation(string inSyndicateCode, int siteCode)
        {
            return GetOrderingInformation(inSyndicateCode, siteCode);
        }

        public bool UnsetOrderingDetails(string inSyndicateCode, int siteCode)
        {
            bool retValue = false;
            try
            {
                OnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                retValue = onlineOrderingService.UnsetOrderingDetails(inSyndicateCode, siteCode);
            }
            catch (Exception exc)
            {
                retValue = false;
            }
            return retValue;
        }

        LoyaltyOOResponse GetOrderingInformation(string inSyndicateCode, int siteCode)
        {
            try
            {
                OnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                var response = onlineOrderingService.GetOnlineOrderingInformation(inSyndicateCode, siteCode);
                return CreateOrderingResponseNoError(response.IsSuccessful,response.Message);

            }
            catch (Exception exc)
            {
                return CreateOrderingResponseError("Unsuccessful sync for online ordering.");
            }
            return new LoyaltyOOResponse();
        }

        private LoyaltyOOResponse CreateOrderingResponseError(string inMessage)
        {
            return new LoyaltyOOResponse
            {
                IsSuccessful = false,
                ResponseText = inMessage
            };
        }
        private LoyaltyOOResponse CreateOrderingResponseNoError(bool inSuccesful, string inMessage)
        {
            return new LoyaltyOOResponse
            {
                IsSuccessful = inSuccesful,
                ResponseText = inMessage
            };
        }
    }
}
