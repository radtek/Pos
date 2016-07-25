using System;
using Loyaltymate.Exceptions;
using Loyaltymate.Model;
using System.Collections.Generic;
using System.Net;
using System.IO;
using Loyaltymate.Utility;

namespace Loyaltymate.Sevices
{
    public class LoyaltymateService : ILoyaltymateService
    {
        public bool PostTransaction(PointsTransactionViewModel pointsTransaction, string syndicateCode, string uniqueId)
        {
            bool response = false;
            pointsTransaction.UniqueId = uniqueId;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.PostTransaction, syndicateCode, null,
                WebRequestMethods.Http.Post, pointsTransaction);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
                return false;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                    response = true;
                }
            }
            return response;
        }
  
        public bool UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            bool response = false;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("uniqueId", uniqueId));
            parameters.Add(new KeyValuePair<string, string>("memberCardCode", memberCardCode));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.UpdateMemberCardCode, inSyndicateCode, parameters,
                WebRequestMethods.Http.Post);
            request.ContentLength = 0;
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
                else
                {
                    var memberStream = new StreamReader(webResponse.GetResponseStream());
                    string message = memberStream.ReadToEnd();
                    if (message.Contains("Card Code Already In Use."))
                    {
                        throw new CardCodeInUse();
                    }
                }
                return false;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                    response = true;
                }
            }
            return response;
        }

        public MemberViewModel SaveMember(MemberViewModel member, string syndicateCode)
        {
            MemberViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SaveMember, syndicateCode, null,
                WebRequestMethods.Http.Post, member);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<MemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public MemberViewModel GetMemberByUniqueId(string uniqueId, string syndicateCode)
        {
            MemberViewModel response = null;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("uniqueId", uniqueId));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetMemberByUniqueId, syndicateCode, parameters, WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<MemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public MemberViewModel GetMemberByCardCode(string cardCode, string syndicateCode)
        {
            MemberViewModel response = null;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("cardCode", cardCode));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetMemberByCardCode, syndicateCode, parameters, WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<MemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
                else
                {
                    var memberStream = new StreamReader(webResponse.GetResponseStream());
                    string message = memberStream.ReadToEnd();
                    if (message.Contains("Member Not Exist."))
                    {
                        throw new MemberNotExistException();
                    }
                }
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public MemberViewModel GetMemberByEmail(string inMemberEmail, string inSyndicateCode)
        {
            MemberViewModel response = null;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("email", inMemberEmail));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetMemberByEmail, inSyndicateCode, parameters, WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<MemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public IEnumerable<MemberViewModel> GetAllMember(string syndicateCode)
        {
            List<MemberViewModel> response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetAllMember, syndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<List<MemberViewModel>>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public bool DeleteMember(string uniqueId, string syndicateCode)
        {
            bool response = false;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("uniqueId", uniqueId));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.DeleteMember, syndicateCode, parameters, WebRequestMethods.Http.Post);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                response = true;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public TierLevelViewModel SaveTierLevel(TierLevelViewModel tierLevel, string syndicateCode)
        {
            TierLevelViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SaveTierLevel, syndicateCode, null,
                WebRequestMethods.Http.Post, tierLevel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<TierLevelViewModel>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public IEnumerable<TierLevelViewModel> GetAllTierLevels(string syndicateCode)
        {
            List<TierLevelViewModel> response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetAllTierLevel, syndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<List<TierLevelViewModel>>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public TierLevelViewModel GetTierLevel(long tierId, string syndicateCode)
        {
            TierLevelViewModel response = null;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("tierId", Convert.ToString(tierId)));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetTierLevel, syndicateCode, parameters, WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<TierLevelViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public bool DeleteTierLevel(long tierId, string syndicateCode)
        {
            bool response = false;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("tierLevelId", Convert.ToString(tierId)));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.DeleteTierLevel, syndicateCode, parameters, WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                response = true;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        public PointQuery GetPointsInRange(PointQuery pointQuery, string syndicateCode)
        {
            PointQuery response = null;
            var parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("startDate", pointQuery.StartDate.ToString("yyyy-MM-dd HH:mm:ss")));
            parameters.Add(new KeyValuePair<string, string>("endDate", pointQuery.EndDate.ToString("yyyy-MM-dd HH:mm:ss")));
            parameters.Add(new KeyValuePair<string, string>("uniqueId", pointQuery.UniqueId));
            parameters.Add(new KeyValuePair<string, string>("pointType", Convert.ToString(pointQuery.PointType)));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetTierLevel, syndicateCode, parameters, WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<PointQuery>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                if (webResponse == null)
                    HandleExceptions(webResponse);
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }

        private void HandleExceptions(HttpWebResponse webResponse)
        {
            switch ((int)webResponse.StatusCode)
            {
                case 400:
                    throw new Exception("Bad request");
                case 401:
                    throw new AuthenticationFailedException();
                case 404:
                    throw new NotFoundException();
                case 422:
                    throw new InvalidRequestException();
                case 500:
                    throw new ApiDownException();
                default:
                    throw new Exception(webResponse.StatusCode + " : Unkonwn Exception");
            }
        }
    }
}