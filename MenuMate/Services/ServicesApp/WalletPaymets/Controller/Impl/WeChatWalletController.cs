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
using WalletPaymets.Model;
using System.IO;

namespace WalletPayments.Controller.Impl
{
    public class WeChatWalletController : IWalletController
    {
        public WalletResponse Login(WalletAccountInfo inWalletAccountInfo)
        {
            var walletResponse = new WalletResponse();
            var requestUrl = ApiUrl(string.Format("?c={0}&a={1}&do={2}", "cashier", "account", "login"));
            var timestamp = ConvertDateTimeInt(DateTime.Now);
            string crypt = string.Empty;
            if (inWalletAccountInfo.Password.Length > 30)
            {
                crypt = "sha1";
            }
            var hash = Base64Encode(JsonConvert.SerializeObject(new
            {
                username = inWalletAccountInfo.UserName,
                password = inWalletAccountInfo.Password,
                time = timestamp,
                crypt = crypt
            }));
            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("hashinfo", hash);
            parameters.Add("pnzuid", Base64Encode(inWalletAccountInfo.UserName));
            parameters.Add("ts", timestamp.ToString());
            string sign = MakeSign(parameters, inWalletAccountInfo.SignKey);
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
                        acoountInformation.UserName = inWalletAccountInfo.UserName;
                        acoountInformation.Password = inWalletAccountInfo.Password;
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
                        acoountInformation.SignKey = inWalletAccountInfo.SignKey;
                        SaveAccountInfo(acoountInformation);
                        walletResponse.ResponseSuccessful = true;
                    }
                }
                else
                {
                    walletResponse.ResponseSuccessful = false;
                    walletResponse.ResponseMessage = "Errcode: " + jo["errno"].ToString() + " " + jo["message"].ToString();
                }

            }
            return walletResponse; ;
        }

        public WalletResponse DoPurchaseTransaction(WalletTransactionInfo inWalletTransactionInfo)
        {
            var walletResponse = new WalletResponse();
            CheckToken(inWalletTransactionInfo);
            var accountInfo = LoadAccountInfo();
            string url = ApiUrl(string.Format("?c={0}&a={1}&do={2}", "cashier", "payment", "quick_pay"));
            var timestamp = ConvertDateTimeInt(DateTime.Now);
            var hash = Base64Encode(JsonConvert.SerializeObject(new
            {
                mid = accountInfo.MerchentId,
                uniacid = accountInfo.AccountId,
                companyid = accountInfo.CompanyId,
                store_id = accountInfo.ShopId,
                user_id = accountInfo.UserId,
                tname = accountInfo.ShopName,
                tprice = inWalletTransactionInfo.Amount,
                auth_code = inWalletTransactionInfo.ScannedCode,
                platform = accountInfo.Platform
            }));
            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("hashinfo", hash);
            parameters.Add("pnzuid", Base64Encode(accountInfo.UserName));
            parameters.Add("ts", timestamp.ToString());
            parameters.Add("access_token", Base64Encode(accountInfo.Token));
            string sign = MakeSign(parameters, accountInfo.SignKey);
            parameters.Add("sign", sign);
            string postData = GetPostDate(parameters);
            string response = PostResponse(url, postData);
            JObject jo = (JObject)JsonConvert.DeserializeObject(response);

            if (jo["errno"] != null)
            {
                if (int.Parse(jo["errno"].ToString()) == 0)
                {
                    walletResponse.ResponseSuccessful = true;
                    var results = jo["results"].ToString();
                    var info = (JObject)JsonConvert.DeserializeObject(results);
                    walletResponse.OrderId = info["order_no"].ToString();
                }
                else
                {
                    walletResponse.ResponseSuccessful = false;
                    walletResponse.ResponseMessage = "Errcode: " + jo["errno"].ToString() + " " + jo["message"].ToString();
                }

            }
            return walletResponse;
        }

        public WalletResponse DoRefundTransaction(WalletTransactionInfo inWalletTransactionInfo)
        {
            var walletResponse = new WalletResponse();
            CheckToken(inWalletTransactionInfo);
            var accountInfo = LoadAccountInfo();
            string url = ApiUrl(string.Format("?c={0}&a={1}&do={2}", "cashier", "payment", "apply_refund"));
            var timestamp = ConvertDateTimeInt(DateTime.Now);
            var hash = Base64Encode(JsonConvert.SerializeObject(new
            {
                mid = accountInfo.MerchentId,
                order_id = inWalletTransactionInfo.OrderRefernce,
                refund_fee = inWalletTransactionInfo.RefundFee,
                companyid = accountInfo.CompanyId,
                user_id = accountInfo.UserId,
                platform = accountInfo.Platform,
                ts = timestamp
            }));
            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("pnzuid", Base64Encode(accountInfo.UserName));
            parameters.Add("hashinfo", hash);
            parameters.Add("access_token", Base64Encode(accountInfo.Token));
            parameters.Add("ts", timestamp.ToString());
            string sign = MakeSign(parameters, accountInfo.SignKey);
            parameters.Add("sign", sign);
            string postData = GetPostDate(parameters);
            string response = PostResponse(url, postData);
            JObject jo = (JObject)JsonConvert.DeserializeObject(response);

            if (jo["errno"] != null)
            {
                if (int.Parse(jo["errno"].ToString()) == 0)
                {
                    walletResponse.ResponseSuccessful = true;
                    var results = jo["results"].ToString();
                    var info = (JObject)JsonConvert.DeserializeObject(results);
                    walletResponse.RefundTransactionId = info["transaction_id"].ToString();
                    walletResponse.OrderNo = info["order_no"].ToString();
                    walletResponse.RefundFee = info["refund_fee"].ToString();
                    walletResponse.ApplyTime = info["apply_time"].ToString();
                    walletResponse.OutRefundNo = info["out_refund_no"].ToString();
                    walletResponse.RefundStatus = info["status"].ToString();
                }
                else
                {
                    walletResponse.ResponseSuccessful = false;
                    walletResponse.ResponseMessage = jo["message"].ToString();
                }

            }
            return walletResponse;
        }

        public void DoTransactionEnquiry(QueryOrderInfo inQueryOrderInfo)
        {
            throw new System.NotImplementedException();
        }

        #region Private Methods

        private void CheckToken(WalletTransactionInfo inWalletTransactionInfo)
        {
            bool doLogin = false;
            var accountInformation = LoadAccountInfo();
            if (!string.IsNullOrEmpty(accountInformation.Token))
            {
                doLogin = ConvertDateTimeInt(System.DateTime.Now) < accountInformation.ExpiresIn;
            }

            if (!doLogin)
            {
                var walletAccountInfo = new WalletAccountInfo()
                {
                    UserName = inWalletTransactionInfo.AccountInformation.UserName,
                    Password = inWalletTransactionInfo.AccountInformation.Password,
                    SignKey = inWalletTransactionInfo.AccountInformation.SignKey
                };
                Login(walletAccountInfo);
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

        private AccountInformation LoadAccountInfo()
        {
            if (File.Exists(GetFilePath()))
            {
                var data = File.ReadAllText(GetFilePath());
                var accountInfo = JsonConvert.DeserializeObject<AccountInformation>(data);
                return accountInfo;
            }
            else
                return new AccountInformation();
        }

        private void SaveAccountInfo(AccountInformation inAccountInformation)
        {
            var data = JsonConvert.SerializeObject(inAccountInformation);
            File.WriteAllText(GetFilePath(), data);
        }

        private string GetFilePath()
        {
            return Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase).Substring(6) + @"\" + "WeChatAccount.txt";
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

                if (pair.Key != "sign" && !string.IsNullOrWhiteSpace(pair.Value))
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
            //ServicePointManager.ServerCertificateValidationCallback +=(sender, cert, chain, sslPolicyErrors) => true;
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