using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using WalletPayments.Controller.Interface;
using WalletPayments.Model;

namespace WalletPayments.Controller.Impl
{
    public class WeChatWalletController : IWalletController
    {
        public void Login(LoginModel loginModel)
        {
            var requestUrl = ApiUrl(string.Format("?c={0}&a={1}&do={2}", "cashier", "account", "login"));
            var timestamp = ConvertDateTimeInt(DateTime.Now);
            string crypt = string.Empty;
            if (loginModel.Password.Length > 30)
            {
                crypt = "sha1";
            }
            var hash = Base64Encode(JsonConvert.SerializeObject(new
            {
                username = loginModel.UserName,
                password = loginModel.Password,
                time = timestamp,
                crypt = crypt
            }));
            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("hashinfo", hash);
            parameters.Add("pnzuid", Base64Encode(loginModel.UserName));
            parameters.Add("ts", timestamp.ToString());
            string sign = MakeSign(parameters, loginModel.SignKey);
            parameters.Add("sign", sign);
            string postData = GetPostDate(parameters);
            string response = PostResponse(requestUrl, postData);

            JObject jo = (JObject)JsonConvert.DeserializeObject(response);
            if (jo["errno"] != null)
            {
                if (int.Parse(jo["errno"].ToString()) == 0)
                {
                    var info = (JObject)JsonConvert.DeserializeObject(jo["results"].ToString());
                    if (info != null)
                    {
                        var acoountInformation = new AccountInformation();
                        acoountInformation.UserId = int.Parse(info["uid"].ToString());
                        acoountInformation.UserName = loginModel.UserName;
                        acoountInformation.Password = loginModel.Password;
                        acoountInformation.ShopId = int.Parse(info["shopid"].ToString());
                        acoountInformation.ShopName = info["shopname"].ToString();
                        acoountInformation.Token = info["token"].ToString();
                        acoountInformation.ExpiresIn = GetTimeStamp() + int.Parse(info["expires_in"].ToString());
                        acoountInformation.RefreshToken = info["refresh_token"].ToString();
                        acoountInformation.CompanyId = int.Parse(info["companyid"].ToString());
                        acoountInformation.MerchantName = acoountInformation.CompanyName = info["cname"].ToString();
                        acoountInformation.MerchentId = int.Parse(info["mid"].ToString());
                        acoountInformation.AccountId = int.Parse(info["acid"].ToString());
                        acoountInformation.IsRefunds = int.Parse(info["is_refunds"].ToString());
                        acoountInformation.SignKey = loginModel.SignKey;
                        //msg = "ok";
                        //return true;
                    }
                }
                else
                {
                    // msg = "Errcode: " + jo["errno"].ToString() + " " + jo["message"].ToString();
                }

            }
            //return false;
        }
    
        public void DoPurchaseTransaction(TransactionModel transactionModel)
        {
            CheckToken();
            string url = ApiUrl(string.Format("?c={0}&a={1}&do={2}", "cashier", "payment", "quick_pay"));
            var timestamp = ConvertDateTimeInt(DateTime.Now);
            var hash = Base64Encode(JsonConvert.SerializeObject(new
            {
                mid = transactionModel.AccountInformation.MerchentId,
                uniacid = transactionModel.AccountInformation.AccountId,
                companyid = transactionModel.AccountInformation.CompanyId,
                store_id = transactionModel.AccountInformation.ShopId,
                user_id = transactionModel.AccountInformation.UserId,
                tname = transactionModel.AccountInformation.ShopName,
                tprice = transactionModel.Amount,
                auth_code = transactionModel.QrCode,
                platform = transactionModel.AccountInformation.Platform
            }));
            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("hashinfo", hash);
            parameters.Add("pnzuid", Base64Encode(transactionModel.AccountInformation.UserName));
            parameters.Add("ts", timestamp.ToString());
            parameters.Add("access_token", Base64Encode(transactionModel.AccountInformation.Token));
            string sign = MakeSign(parameters, transactionModel.AccountInformation.SignKey);
            parameters.Add("sign", sign);
            string postData = GetPostDate(parameters);
            string response = PostResponse(url, postData);
            JObject jo = (JObject)JsonConvert.DeserializeObject(response);

            if (jo["errno"] != null)
            {
                if (int.Parse(jo["errno"].ToString()) == 0)
                {
                    //return jo["results"].ToString();
                }
                else
                {
                    // msg = "Errcode: " + jo["errno"].ToString() + " " + jo["message"].ToString();
                }

            }
            //return string.Empty;
        }
     
        public void DoRefundTransaction(TransactionModel transactionModel)
        {
            throw new System.NotImplementedException();
        }
   
        public void DoTransactionEnquiry(TransactionModel transactionModel)
        {
            throw new System.NotImplementedException();
        }

        #region Private Methods
        public void CheckToken()
        {
            bool doLogin = false;
            var accountInformation = new AccountInformation();
            if (!string.IsNullOrEmpty(accountInformation.Token))
            {
                doLogin = ConvertDateTimeInt(System.DateTime.Now) < accountInformation.ExpiresIn;
            }

            if (!doLogin)
            {
                var loginModel = new LoginModel()
                {
                    UserName = accountInformation.UserName,
                    Password = accountInformation.Password,
                    SignKey = accountInformation.SignKey
                };
                Login(loginModel);
            }
        }
        public int ConvertDateTimeInt(DateTime time)
        {
            System.DateTime startTime = TimeZone.CurrentTimeZone.ToLocalTime(new System.DateTime(1970, 1, 1));
            return (int)(time - startTime).TotalSeconds;
        }
        public int GetTimeStamp()
        {
            TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt32(ts.TotalSeconds);
        }
        private string MakeSign(IDictionary<string, string> mValues, string key)
        {
            Dictionary<string, string> mValuesAsc = mValues.OrderBy(o => o.Key).ToDictionary(o => o.Key, p => p.Value);

            string str = ToUrl(mValuesAsc);
            str += "&key=" + key;
            string m2 = Md5Encrypt(str);
            return m2.ToUpper();
        }
        private string Md5Encrypt(string strText)
        {
            System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
            byte[] result = md5.ComputeHash(System.Text.Encoding.UTF8.GetBytes(strText));
            return BitConverter.ToString(result).Replace("-", "");
        }
        private string ToUrl(IDictionary<string, string> mValues)
        {
            string buff = "";
            foreach (KeyValuePair<string, string> pair in mValues)
            {
                if (pair.Value == null)
                {
                    throw new Exception("");
                }

                if (pair.Key != "sign" && string.IsNullOrWhiteSpace(pair.Value))
                {
                    buff += pair.Key + "=" + pair.Value + "&";
                }
            }
            buff = buff.Trim('&');
            return buff;
        }
        private string ApiUrl(string requesturl)
        {
            return WalletConstants.WeChatBaseAddress + requesturl;
        }
        private string GetPostDate(IDictionary<string, string> parameters)
        {
            StringBuilder buffer = new StringBuilder();
            if (!(parameters == null || parameters.Count == 0))
            {

                int i = 0;
                foreach (string key in parameters.Keys)
                {
                    if (i > 0)
                    {
                        buffer.AppendFormat("&{0}={1}", key, parameters[key]);
                    }
                    else
                    {
                        buffer.AppendFormat("{0}={1}", key, parameters[key]);
                    }
                    i++;
                }
            }
            return buffer.ToString();
        }
        public static string Base64Encode(string inStr)
        {
            return Convert.ToBase64String(Encoding.UTF8.GetBytes(inStr));
        }
        public static string Base64Decode(string inStrBase64)
        {
            return Encoding.UTF8.GetString(Convert.FromBase64String(inStrBase64));
        }
        public string PostResponse(string url, string postData)
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;

            HttpContent httpContent = new StringContent(postData, Encoding.UTF8);
            httpContent.Headers.ContentType = new MediaTypeHeaderValue(WalletConstants.MimeType);
            using (HttpClient httpClient = new HttpClient())
            {

                HttpResponseMessage response = httpClient.PostAsync(url, httpContent).Result;

                if (response.IsSuccessStatusCode)
                {
                    string result = response.Content.ReadAsStringAsync().Result;
                    return result;
                }
                return null;
            }
        }
        #endregion
    }
}