using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using Thorlink.Net;
using MenumateServices.DTO.Thorlink;
using MenumateServices.InternalClasses.ThorlinkInternal;
using System.Threading;
using System.Diagnostics;
using MenumateServices.Internal_Classes.ThorlinkInternal;
using System.Threading.Tasks;

namespace MenumateServices.WCFServices
{
    public class WCFServiceThorlink : IWCFServiceThorlink
    {
        public WCFServiceThorlink()
        {
            _waitflag = false;
            _response = new DTO_TMemberInfo();
            _purchaseResponseInfo = new DTO_TPurchaseInfo();
        }

        private DTO_TMemberInfo _response;
        private DTO_TPurchaseInfo _purchaseResponseInfo;
        private bool _waitflag;

        public DTO_TPurchaseInfo GetTenderInformation(DTO_TenderList tenderDetailsList)
        {
            ThorPurchase purchase = new ThorPurchase();
            foreach (DTO_ItemDetails items in tenderDetailsList.itemDetailsList)
            {
                purchase.ItemDetail.Add(new ThorItemDetail()
                {
                    ProductCode = items.thirdPartyCode,
                    UnitPrice = items.unitPrice,
                    Quantity = items.qty,
                });
            }
            foreach (DTO_TenderDetails tendered in tenderDetailsList.tenderItemDetails)
            {

                purchase.AccessToken = ThorAuthCheck.Instance.ThorlinkAccessToken;
                string tenderIdent = tendered.tenderIdentifier;
                purchase.CardNo = tendered.cardNo;
                int tenderCode = 0;
                if (tendered.thorTenderType == TenderTypeThor.eThorCash)
                {
                    tenderCode = (int)ThorTenderType.Cash;
                }
                else if (tendered.thorTenderType == TenderTypeThor.eThorPoints)
                {
                    tenderCode = (int)ThorTenderType.Loyalty;
                }
                else if (tendered.thorTenderType == TenderTypeThor.eThorVoucher)
                {
                    tenderCode = (int)ThorTenderType.Voucher;
                    tendered.tenderValue = 0.0;
                }
                else if (tendered.thorTenderType == TenderTypeThor.eThorDebitCard)
                {
                    tenderCode = (int)ThorTenderType.DebitCard;
                }
                else if (tendered.thorTenderType == TenderTypeThor.eThorCheque)
                {
                    tenderCode = (int)ThorTenderType.Cheque;
                }
                purchase.TenderDetail.Add(new ThorTenderDetail()
                {
                    TenderTypeCode = tenderCode,
                    TenderIdentifier = tenderIdent,
                    TenderValue = tendered.tenderValue,
                });
                if (tendered.sendTransactionValue == false)
                {
                    purchase.TransactionValue += 0;
                }
                else
                {
                    purchase.TransactionValue += tendered.tenderValue;
                }
            }
            DoProcess(purchase);
            return _purchaseResponseInfo;
        }

        public DTO_TPurchaseInfo GetRefundInformation(DTO_RefundDetails refundDetails)
        {
            ThorPurchaseRefund refund = new ThorPurchaseRefund();
            refund.AccessToken = ThorAuthCheck.Instance.ThorlinkAccessToken;
            refund.CardNo = refundDetails.CardNo;
            refund.CreditValue = refundDetails.CreditValue;
            refund.LoyaltyValue = refundDetails.LoyaltyValue;
            refund.TransactionValue = refundDetails.TransactionValue;
            refund.Callback = TransactionResponseHandler;
            refund.Submit();
            
            return _purchaseResponseInfo;
        }
        public void Initialize(DTO_ThorAuth dto_thorAuth)
        {
            try
            {
                if (dto_thorAuth != null)
                {
                    if (ThorAuthCheck.Instance.ThorlinkAccessToken == null)
                    {
                        ThorAuthCheck.Instance.CallThorInitMethod(dto_thorAuth);
                        Thread.Sleep(500);
                    }
                    else 
                    {
                        ServiceLogger.Log("Token already exists :-" + ThorAuthCheck.Instance.ThorlinkAccessToken);
                    }
                }
                else
                {
                    throw new ArgumentNullException("Initialize received a null argument!");
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log(" exception is ---" + ex.Message);
            }
        }

        public DTO_TMemberInfo GetMemberInformation(DTO_ThorlinkInquiry inquiry)
        {
            try
            {
                //                if (!string.IsNullOrWhiteSpace(ThorAuthCheck.Instance.ThorlinkAccessToken))
                //                {
                ThorInquiry thorInquiry = new ThorInquiry();
                thorInquiry.CardNo = inquiry.cardNo;
                thorInquiry.AccessToken = ThorAuthCheck.Instance.ThorlinkAccessToken;
                string s = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                ServiceLogger.Log("Request Prepared with cardNo:- " + thorInquiry.CardNo);
                DoInquiry(thorInquiry);
                //                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log(" exception is ---" + ex.Message);
            }
            return _response;
        }

        private void Reset()
        {
            _response = new DTO_TMemberInfo();
            _purchaseResponseInfo = new DTO_TPurchaseInfo();
            _waitflag = true;
        }

        private void WaitForResponse()
        {
            try
            {
                Reset();
                Stopwatch watch = new Stopwatch();
                watch.Reset();
                watch.Start();
                while (_waitflag)
                {
                    if (watch.Elapsed.Seconds > 20)
                    {
                        _waitflag = false;

                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log(" exception is ---" + ex.Message);
            }

        }
        private void DoInquiry(ThorInquiry inquiry)
        {
            try
            {
                inquiry.Callback = InquiryResponseHandler;

                inquiry.Submit();
                WaitForResponse();
            }
            catch (Exception ex)
            {
                ServiceLogger.Log(" exception is ---" + ex.Message);
                _waitflag = false;
            }
        }

        private void DoProcess(ThorPurchase process)
        {
            try
            {
                process.Callback = TransactionResponseHandler;

                process.Submit();
                WaitForResponse();
            }
            catch (Exception ex)
            {
                ServiceLogger.Log(" exception is ---" + ex.Message);
                _waitflag = false;
            }
        }
        private void TransactionResponseHandler(object request, object response)
        {
            Task.Factory.StartNew(new Action(() =>
            {
                if (request == null)
                {
                    throw new Exception("TransactionResponseHandler received a null argument!");
                }
                try
                {
                    ThorlinkPurchase thorlinkPurchase = new ThorlinkPurchase();
                    if (response != null)
                    {

                        _purchaseResponseInfo = thorlinkPurchase.FormatThorPurchaseResponse(request,response);
                        _waitflag = false;
                    }
                }
                catch (Exception ex)
                {
                    ServiceLogger.Log("TransactionResponseHandler exception is ---" + ex.Message);
                }
            }));
        }

        private void InquiryResponseHandler(object request, object response)
        {
            if (request == null)
            {
                ServiceLogger.Log("InquiryResponseHandler received a null argument!");
                throw new Exception("InquiryResponseHandler received a null argument!");
            }
            ThorlinkMember thorlinkMember = new ThorlinkMember();
            try
            {
                if (response != null)
                {
                    _response = thorlinkMember.PopulateDTOMember(response);
                    _waitflag = false;
                    ServiceLogger.Log("Response received with voucher:- " + ((ThorInquiryResponse)response).VoucherDetail.Count);
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("InquiryResponseHandler exception is ---" + ex.Message);
            }
        }
    }
}
