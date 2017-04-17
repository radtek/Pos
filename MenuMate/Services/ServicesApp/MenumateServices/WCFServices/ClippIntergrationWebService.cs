using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Web;
using MenumateServices.DTO.Clipp;
using MenumateServices.Utilities;
using System.Runtime.Serialization.Json;
using System.Text;
using Newtonsoft.Json;
using System.Diagnostics;

namespace MenumateServices.WCFServices
{
    public class ClippIntergrationWebService : IClippIntergrationWebService
    {
        private readonly string _baseAddress;
        private readonly string _port;
        private const string ClippAddress = "Clipp";
        private readonly HttpClientHandler _httpClientHandler;

        public ClippIntergrationWebService()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;
            _baseAddress = configurationManager.ClippIpAddress;
            _port = configurationManager.ClippPort;

            _httpClientHandler = new HttpClientHandler()
            {
                Credentials = new NetworkCredential("CLIPP", "12345678"),
                PreAuthenticate = true
            };
        }

        public List<ClippMessage> GetOpenMessages()
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var response = client.GetAsync(string.Format("{0}/Messages", ClippAddress));

                    response.Wait();
                    var result = response.Result;
                    if (result.IsSuccessStatusCode)
                    {
                        var clippMessageTask = result.Content.ReadAsStringAsync();
                        clippMessageTask.Wait();

                        var clippMessages = JsonConvert.DeserializeObject<ClippMessage[]>(clippMessageTask.Result);

                        return new List<ClippMessage>(clippMessages);
                    }
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In GetOpenMessages Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 1, short.MaxValue);
                ServiceLogger.LogException("Exception in GetOpenMessages", exception);
            }

            return new List<ClippMessage>(0);
        }

        public List<ClippMessage> GetPreviousMessages()
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var response = client.GetAsync(string.Format("{0}/Messages/1", ClippAddress));

                    response.Wait();
                    var result = response.Result;
                    if (result.IsSuccessStatusCode)
                    {
                        var clippMessageTask = result.Content.ReadAsStringAsync();
                        clippMessageTask.Wait();

                        var clippMessages = JsonConvert.DeserializeObject<ClippMessage[]>(clippMessageTask.Result);

                        return new List<ClippMessage>(clippMessages);
                    }
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In GetPreviousMessages Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 2, short.MaxValue);
                ServiceLogger.LogException("Exception in GetPreviousMessages", exception);
            }

            return new List<ClippMessage>(0);
        }

        public void UpdateTabDetails(ClippTabDetailRequest clippTabDetailRequest)
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var methodAddress = string.Format("{0}/Actions/SendTabDetails?clippTabRef={1}",
                        ClippAddress, Guid.Parse(clippTabDetailRequest.ClippTabRef));

                    if (clippTabDetailRequest.MessageId != "")
                    {
                        methodAddress = methodAddress + string.Format("&clippMessageRef={0}", Guid.Parse(clippTabDetailRequest.MessageId));
                    }

                    var serializedClippTabDetail = JsonConvert.SerializeObject(clippTabDetailRequest.TabData, typeof(ClippTabDetail), new JsonSerializerSettings { });
                    HttpContent httpContent = new StringContent(serializedClippTabDetail, Encoding.UTF8, "application/json");

                    var response = client.PostAsync(methodAddress, httpContent);

                    response.Wait();
                    var result = response.Result;

                    if (!result.IsSuccessStatusCode)
                        throw new InvalidOperationException("There was an error executing the operation, please try again later.");
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In UpdateTabDetails Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 3, short.MaxValue);
                ServiceLogger.LogException("Exception in UpdateTabDetails", exception);
            }
        }

        public void UpdateTabDetailsOnError(ClippErrorDetail clippErrorDetail)
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var methodAddress = string.Format("{0}/Actions/SendTabDetails?clippTabRef={1}",
                        ClippAddress, Guid.Parse(clippErrorDetail.ClippTabRef));

                    if (clippErrorDetail.MessageId != "")
                    {
                        methodAddress = methodAddress + string.Format("&clippMessageRef={0}", Guid.Parse(clippErrorDetail.MessageId));
                    }

                    var serializedClippErrorDetail = JsonConvert.SerializeObject(clippErrorDetail, typeof(ClippErrorDetail), new JsonSerializerSettings { });
                    HttpContent httpContent = new StringContent(serializedClippErrorDetail, Encoding.UTF8, "application/json");

                    var response = client.PostAsync(methodAddress, httpContent);

                    response.Wait();
                    var result = response.Result;

                    if (!result.IsSuccessStatusCode)
                        throw new InvalidOperationException("There was an error executing the operation, please try again later.");
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In UpdateTabDetailsOnError Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in UpdateTabDetailsOnError", exception);
            }
        }

        public ClippMessageDetail CloseTab(ClippTabDetailRequest clippTabDetailRequest)
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var methodAddress = string.Format("{0}/Actions/CloseTab?clippTabRef={1}&amount={2}", ClippAddress,
                        Guid.Parse(clippTabDetailRequest.ClippTabRef), clippTabDetailRequest.Amount);

                    if (clippTabDetailRequest.MessageId != "")
                    {
                        methodAddress = methodAddress + string.Format("&clippMessageId={0}", Guid.Parse(clippTabDetailRequest.MessageId));
                    }

                    var serializedClippTabDetail = JsonConvert.SerializeObject(clippTabDetailRequest.TabData, typeof(ClippTabDetail), new JsonSerializerSettings { });
                    HttpContent httpContent = new StringContent(serializedClippTabDetail, Encoding.UTF8, "application/json");

                    var response = client.PostAsync(methodAddress, httpContent);

                    response.Wait();
                    var result = response.Result;

                    if (result.IsSuccessStatusCode)
                    {
                        var clippMessageTask = result.Content.ReadAsStringAsync();
                        clippMessageTask.Wait();

                        var jsonSerializer = new DataContractJsonSerializer(typeof(ClippMessageDetail));
                        var memoryStream = new MemoryStream(Encoding.ASCII.GetBytes(clippMessageTask.Result));

                        var clippMessageDetail = (ClippMessageDetail)jsonSerializer.ReadObject(memoryStream);

                        return clippMessageDetail;
                    }
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In CloseTab Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 5, short.MaxValue);
                ServiceLogger.LogException("Exception in CloseTab", exception);
            }
            return null;
        }

        public void CloseTabOnError(ClippErrorDetail clippErrorDetail)
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var methodAddress = string.Format("{0}/Actions/CloseTab?clippTabRef={1}&amount={2}", ClippAddress,
                        Guid.Parse(clippErrorDetail.ClippTabRef), clippErrorDetail.TotalAmount);

                    if (clippErrorDetail.MessageId != "")
                    {
                        methodAddress = methodAddress + string.Format("&clippMessageRef={0}", Guid.Parse(clippErrorDetail.MessageId));
                    }

                    var serializedClippErrorDetail = JsonConvert.SerializeObject(clippErrorDetail, typeof(ClippErrorDetail), new JsonSerializerSettings { });
                    HttpContent httpContent = new StringContent(serializedClippErrorDetail, Encoding.UTF8, "application/json");

                    var response = client.PostAsync(methodAddress, httpContent);

                    response.Wait();
                    var result = response.Result;

                    if (!result.IsSuccessStatusCode)
                        throw new InvalidOperationException("There was an error executing the operation, please try again later.");
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In CloseTabOnError Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 6, short.MaxValue);
                ServiceLogger.LogException("Exception in CloseTabOnError)", exception);
            }
        }

        public PaymentDetailResponse RequestTabPayment(PaymentDetailRequest paymentDetailRequest)
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var methodAddress = string.Format("{0}/Actions/RequestTabPayment?clippTabRef={1}&amount={2}&clippMessageId={3}",
                            ClippAddress, Guid.Parse(paymentDetailRequest.ClippTabRef), paymentDetailRequest.Amount,
                            Guid.Parse(paymentDetailRequest.ClippMessageId));

                    var response = client.PostAsync(methodAddress, null);

                    response.Wait();
                    var result = response.Result;

                    if (result.IsSuccessStatusCode)
                    {
                        var paymentDetailTask = result.Content.ReadAsStringAsync();
                        paymentDetailTask.Wait();

                        var jsonSerializer = new DataContractJsonSerializer(typeof(PaymentDetailResponse));
                        var memoryStream = new MemoryStream(Encoding.ASCII.GetBytes(paymentDetailTask.Result));

                        var paymentDetail = (PaymentDetailResponse)jsonSerializer.ReadObject(memoryStream);

                        return paymentDetail;
                    }
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In RequestTabPayment Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 7, short.MaxValue);
                ServiceLogger.LogException("Exception in RequestTabPayment", exception);
            }
            return null;
        }

        public bool ForceCloseAllTabs()
        {
            try
            {
                using (var client = new HttpClient(_httpClientHandler))
                {
                    client.BaseAddress = new Uri(string.Format("http://{0}:{1}/", _baseAddress, _port));
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                    var response = client.GetAsync(string.Format("{0}/Actions/ForceCloseAllTabs", ClippAddress));

                    response.Wait();
                    var result = response.Result;

                    return result.IsSuccessStatusCode;
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In ForceCloseAllTabs Clipp", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 8, short.MaxValue);
                ServiceLogger.LogException("Exception in ForceCloseAllTabs", exception);
            }
            return false;
        }
    }
}
