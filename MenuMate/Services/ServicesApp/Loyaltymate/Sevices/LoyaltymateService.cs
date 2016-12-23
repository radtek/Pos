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
        public ApiMemberViewModel SaveMember(ApiMemberViewModel member, string syndicateCode)
        {
            ApiMemberViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SaveMember, syndicateCode, null,
                WebRequestMethods.Http.Post, member);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<ApiMemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public ApiMemberViewModel GetMemberByUniqueId(ApiRequestViewModel requestViewModel, string syndicateCode)
        {
            ApiMemberViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetMemberByUniqueId, syndicateCode, null, WebRequestMethods.Http.Post, requestViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<ApiMemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public ApiMemberViewModel GetMemberByCardCode(ApiRequestViewModel requestViewModel, string syndicateCode)
        {
            ApiMemberViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetMemberByCardCode, syndicateCode, null, WebRequestMethods.Http.Post, requestViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<ApiMemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public ApiMemberViewModel GetMemberByEmail(ApiRequestViewModel requestViewModel, string inSyndicateCode)
        {
            ApiMemberViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetMemberByEmail, inSyndicateCode, null, WebRequestMethods.Http.Post, requestViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<ApiMemberViewModel>(memberStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public bool PostTransaction(PointsTransactionViewModel pointsTransaction, string syndicateCode)
        {
            bool response = false;
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

        public bool PostInvoiceTransaction(ApiMemberInvoiceTransactionViewModel pointsTransaction, string syndicateCode)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.PostInvoiceTransaction, syndicateCode, null,
                WebRequestMethods.Http.Post, pointsTransaction);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public ApiCompanyViewModel GetCompanyDetail(string inSyndicateCode)
        {
            ApiCompanyViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetCompanyDetail, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<ApiCompanyViewModel>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public GiftCardApiViewModel GetGiftCardBalance(string inSyndicateCode, ApiRequestViewModel requestViewModel)
        {
            GiftCardApiViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetGiftCardInfo, inSyndicateCode, null, WebRequestMethods.Http.Post, requestViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<GiftCardApiViewModel>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public ApiPocketVoucherViewModel GetPocketVoucherDetail(string inSyndicateCode, ApiRequestViewModel requestViewModel)
        {
            ApiPocketVoucherViewModel response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.GetPocketVoucher, inSyndicateCode, null, WebRequestMethods.Http.Post, requestViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<ApiPocketVoucherViewModel>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public List<ApiProcessingResult> PostVoucherTransactions(string inSyndicateCode, ApiVouchersUsageViewModel voucherTransaction)
        {
            voucherTransaction.RequestTime = DateTime.Now;
            List<ApiProcessingResult> response = null;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.ProcessVoucherTransaction, inSyndicateCode, null,
                WebRequestMethods.Http.Post, voucherTransaction);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var responseStream = new StreamReader(webResponse.GetResponseStream());
                response = JsonUtility.Deserialize<List<ApiProcessingResult>>(responseStream.ReadToEnd());
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        public bool ReleaseVouchers(string inSyndicateCode, ApiReleasedVoucherViewModel voucherTransaction)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.ReleaseVouchers, inSyndicateCode, null,
                WebRequestMethods.Http.Post, voucherTransaction);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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

        private void HandleExceptions(HttpWebResponse webResponse)
        {
            if (webResponse != null)
            {
                if ((int)webResponse.StatusCode == 401)
                {
                    throw new AuthenticationFailedException();
                }
                else
                {
                    var memberStream = new StreamReader(webResponse.GetResponseStream());
                    string message = memberStream.ReadToEnd();
                    var customException = JsonUtility.Deserialize<CustomException>(message);
                    throw new LoyaltymateOperationException(customException.ExceptionMessage);
                }
            }
            else
            {
                throw new LoyaltymateOperationException("Not able to connect with server.");
            }
        }
    }
}