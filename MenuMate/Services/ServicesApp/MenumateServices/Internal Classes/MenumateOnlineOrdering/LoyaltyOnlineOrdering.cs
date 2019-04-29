using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using MenumateServices.DTO.MenumateOnlineOrdering;
using OnlineOrdering.Services;
using OnlineOrdering.Model;

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
        
        public LoyaltyOOResponse GetOnlineOrderingInformation(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            return GetOrderingInformation(inSyndicateCode, siteCode, stringList);
        }

        public OnlineOrderingDetails GetOnlineOrderingDetails(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            return GetOrderingDetails(inSyndicateCode, siteCode, stringList);
        }

        public bool UnsetOrderingDetails(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            bool retValue = false;
            try
            {
                OnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                retValue = onlineOrderingService.UnsetOrderingDetails(inSyndicateCode, siteCode, stringList);
            }
            catch (Exception exc)
            {
                retValue = false;
            }
            return retValue;
        }

        LoyaltyOOResponse GetOrderingInformation(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            try
            {
                OnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                var response = onlineOrderingService.GetOnlineOrderingInformation(inSyndicateCode, siteCode, stringList);
                return CreateOrderingResponseNoError(response.IsSuccessful,response.Message);

            }
            catch (Exception exc)
            {
                return CreateOrderingResponseError("Unsuccessful sync for online ordering.");
            }
            return new LoyaltyOOResponse();
        }

        OnlineOrderingDetails GetOrderingDetails(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            try
            {
                OnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                var response = onlineOrderingService.GetOnlineOrderingDetails(inSyndicateCode, siteCode, stringList);
                return CreateOrderingDetailsResponseNoError(response);

            }
            catch (Exception exc)
            {
                return CreateOrderingDetailsResponseError("Unsuccessful sync for online ordering.");
            }
            return new OnlineOrderingDetails();
        }

        private LoyaltyOOResponse CreateOrderingResponseNoError(bool inSuccesful, string inMessage)
        {
            return new LoyaltyOOResponse
            {
                IsSuccessful = inSuccesful,
                ResponseText = inMessage
            };
        }

        private LoyaltyOOResponse CreateOrderingResponseError(string inMessage)
        {
            return new LoyaltyOOResponse
            {
                IsSuccessful = false,
                ResponseText = inMessage
            };
        }

        private OnlineOrderingDetails CreateOrderingDetailsResponseError(string inMessage)
        {
            return new OnlineOrderingDetails
            {
                IsSuccessful = false,
                ResponseText = inMessage,
                IsMemberOrderingEnabled = false,
                IsWaiterOrderingEnabled = false,
            };
        }

        private OnlineOrderingDetails CreateOrderingDetailsResponseNoError(OnlineOrderingDetailsResponse onlineOrderingDetailsResponse)
        {
            OnlineOrderingDetails onlineOrderingDetails =  new OnlineOrderingDetails
            {
                IsSuccessful = onlineOrderingDetailsResponse.IsSuccessful,
                ResponseText = onlineOrderingDetailsResponse.Message,
            };
            if (onlineOrderingDetailsResponse.ApiSiteTransactionSettingMappingViewModels != null && onlineOrderingDetailsResponse.ApiSiteTransactionSettingMappingViewModels.Count > 0)
            {
                onlineOrderingDetails.IsMemberOrderingEnabled = true;
                onlineOrderingDetails.IsWaiterOrderingEnabled = true;
            }
            else
            {
                onlineOrderingDetails.IsWaiterOrderingEnabled = false;
                onlineOrderingDetails.IsMemberOrderingEnabled = false;
            }
            return onlineOrderingDetails;
        }
    }
}
