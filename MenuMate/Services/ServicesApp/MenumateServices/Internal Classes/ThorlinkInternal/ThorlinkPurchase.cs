using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.Thorlink;
using Thorlink.Net;

namespace MenumateServices.Internal_Classes.ThorlinkInternal
{
    class ThorlinkPurchase
    {
        public DTO_TPurchaseInfo FormatThorPurchaseResponse(object request, object response)
        {
            DTO_TPurchaseInfo purchaseInfo = new DTO_TPurchaseInfo();
            purchaseInfo.ResponseCode = ((ThorResponse)response).ResponseCode;
            purchaseInfo.ResponseMessage = ((ThorResponse)response).ResponseDescription;
            ThorPurchase lastPurchase = (ThorPurchase)request;
            purchaseInfo.CardNumber = lastPurchase.CardNo;
            purchaseInfo.CreditValue = lastPurchase.CreditValue;
            purchaseInfo.LoyaltyValue = lastPurchase.LoyaltyValue;
            purchaseInfo.TransactionNumber = lastPurchase.TransactionNo;
            purchaseInfo.TransactionValue = lastPurchase.TransactionValue;
            
            return purchaseInfo;
        }
    }
}
