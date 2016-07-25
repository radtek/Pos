using System;
using System.Diagnostics;
using MenumateServices.DTO.SmartLink;
using Smartpay.Eftpos;
using Smartpay.Eftpos.Logging;
using System.ServiceModel;

namespace MenumateServices.WCFServices
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
    public class WCFServiceSmartLink : IWCFServiceSmartLink
    {
        public WCFServiceSmartLink()
        {
            _waitflag = false;
            _response = new DTO_SmartLinkTransactionResponse();
        }

        private DTO_SmartLinkTransactionResponse _response;
        private bool _waitflag;
        private string SampleText = "****** SAMPLE ******\n" +
            "Terminal print\n" +
            "function test\n" +
            "The following two\n" +
            "lines should be\n" +
            "blank.\n" +
            "                    \n" +
            "\n" +
            "********************\n" +
            "************************\n" +
            "\n\n\n\n\n\n\n\n\n";

        public DTO_SmartLinkTransactionResponse PingTerminal(string ipAddress)
        {
            var response = new DTO_SmartLinkTransactionResponse();
            try
            {
                using (var ping = new System.Net.NetworkInformation.Ping())
                {
                    System.Net.NetworkInformation.PingReply pingReply = ping.Send(ipAddress);
                    response.Successful = (pingReply.Status == System.Net.NetworkInformation.IPStatus.Success);
                    if (!response.Successful)
                    {
                        response.ErrorText = "Login Failed - " + Convert.ToString(pingReply.Status);
                    }
                }
            }
            catch (Exception ex)
            {
                response.Successful = false;
                response.ErrorText = ex.Message;
            }
            return response;
        }

        public DTO_SmartLinkTransactionResponse Login(string ipAddress, string versionString)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.Logon(versionString);
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse SettlementInquiry(string ipAddress)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                MessageObject request = provider.CreateRequest(Function.SettlementInquiry);
                provider.ProcessRequestAsync(request);
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse SettlementCutover(string ipAddress)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                MessageObject request = provider.CreateRequest(Function.SettlementCutover);
                provider.ProcessRequestAsync(request);
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse Purchase(string ipAddress, double amount)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.Purchase(GetAmount(amount));
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse PurchasePlusCash(string ipAddress, double amount, double cash)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.PurchasePlusCash(GetAmount(amount), GetAmount(cash));
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse CashOnly(string ipAddress, double cash)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.CashAdvance(GetAmount(cash));
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse Refund(string ipAddress, double refundAmount)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.Refund(GetAmount(refundAmount));
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse ReprintLastReceipt(string ipAddress)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.ReprintReceipt();
            }, ipAddress);
            return _response;
        }

        public DTO_SmartLinkTransactionResponse PrintReceipt(string ipAddress)
        {
            CallPaymentProvider(delegate(ISimplePaymentProvider provider)
            {
                provider.PrintOnTerminal(SampleText);
            }, ipAddress);
            return _response;
        }


        private void Reset()
        {
            _response = new DTO_SmartLinkTransactionResponse();
            _waitflag = true;
        }

        private void WaitForResponse()
        {
            Reset();
            Stopwatch watch = new Stopwatch();
            watch.Reset();
            watch.Start();
            while (_waitflag)
            {
                if (watch.Elapsed.Seconds > 120)
                {
                    _waitflag = false;
                    _response.TransactionResult = "Failed - Time-Out";
                    _response.Successful = false;
                    _response.ResultText = "Time-Out";
                    _response.ErrorText = "Time-Out";
                }
            }
        }
        private void CallPaymentProvider(Action<ISimplePaymentProvider> paymentFunction, string ipAddress)
        {
            try
            {
                ISimplePaymentProvider provider = new SimplePaymentProvider(ipAddress, 11234, LogEventLevel.Debug);
                provider.OnResponse += Provider_OnResponse;
                paymentFunction(provider);
                WaitForResponse();
            }
            catch (Exception ex)
            {
                _waitflag = false;
                _response.Successful = false;
                _response.ErrorText = ex.Message;
            }
        }

        private void Provider_OnResponse(object sender, ResponseEventArgs e)
        {
            try
            {
                _waitflag = false;
                ParseResponse(e);
            }
            catch (Exception ex)
            {
                _response.Successful = false;
                _response.TransactionResult = "Failed";
                _response.ErrorText = ex.Message;
            }

        }

        private void ParseResponse(ResponseEventArgs responseEventArgs)
        {
            _response.TransactionResult = responseEventArgs.Response.Args["TransactionResult"];
            if (responseEventArgs.Response.Args.ContainsKey("ResultText"))
                _response.ResultText = responseEventArgs.Response.Args["ResultText"];
            _response.ErrorText = _response.ResultText;
            if (_response.TransactionResult == "OK-ACCEPTED")
                _response.Successful = true;

        }

        private uint GetAmount(double amount)
        {
            uint retAmount = 0;
            Decimal decAmount = Convert.ToDecimal(amount);
            retAmount = (uint)(decAmount * 100);
            return retAmount;
        }

    }



}
