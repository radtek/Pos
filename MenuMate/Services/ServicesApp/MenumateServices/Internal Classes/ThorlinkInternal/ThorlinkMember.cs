using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Thorlink.Net;
using MenumateServices.WCFServices;
using MenumateServices.DTO.Thorlink;

namespace MenumateServices.InternalClasses.ThorlinkInternal
{
    public class ThorlinkMember
    {
        public ThorlinkMember() { }
 
        
        public DTO_TMemberInfo PopulateDTOMember(object response)
        {
            DTO_TMemberInfo memberInfo = new DTO_TMemberInfo();
            try
            {
            memberInfo.voucherDetailsOfThor = new List<DTO_VoucherDetails>();
            var thorResponse = (ThorResponse)response;
            if (thorResponse != null)
            {
                if (thorResponse.ResponseDescription != null)
                {
                    memberInfo.responseDescription = thorResponse.ResponseDescription;
                }
            if (thorResponse.ResponseCode == 0)
            {
                ThorInquiryResponse inquiryResponse = (ThorInquiryResponse)response;

                if (inquiryResponse != null)
                {
                memberInfo.traderStatusName = inquiryResponse.TraderStatusName;
                memberInfo.traderId = inquiryResponse.TraderId;

                if (inquiryResponse.TraderDetail != null)
                {

                    ThorTraderDetail trader = inquiryResponse.TraderDetail;
                    if (trader != null)
                    {
                        memberInfo.firstName = trader.FirstName;
                        memberInfo.lastName = trader.LastName;
                        memberInfo.birthDate = trader.BirthDate;
                        memberInfo.emailAddress = trader.EmailAddress;
                        memberInfo.mobileNumber = trader.MobileNumber;
                        memberInfo.address = trader.Address;
                    }

                }

                if (inquiryResponse.TraderBalance != null)
                {
                    ThorTraderBalance balance = inquiryResponse.TraderBalance;
                    if (balance.LoyaltyBalance != null)
                    {
                        memberInfo.loyaltyBalance = balance.LoyaltyBalance;
                    }
                }

                if (inquiryResponse.VoucherDetail != null)
                {
                    foreach (ThorVoucherDetail voucher in inquiryResponse.VoucherDetail)
                    {
                        if (voucher != null)
                        {
                            DTO_VoucherDetails voucherDetails = new DTO_VoucherDetails();
                            voucherDetails.id = voucher.Id;
                            voucherDetails.imageUrl = voucher.ImageUrl;
                            voucherDetails.merchantName = voucher.MerchantName;
                            voucherDetails.name = voucher.Name;
                            voucherDetails.pluCode = voucher.PluCode;
                            voucherDetails.securityCode = voucher.SecurityCode;
                            voucherDetails.startDate = voucher.StartDate;
                            voucherDetails.type = voucher.Type;
                            voucherDetails.value = voucher.Value;
                            voucherDetails.active = voucher.Active;
                            voucherDetails.availableInStore = voucher.AvailableInStore;
                            voucherDetails.code = voucher.Code;
                            voucherDetails.endDate = voucher.EndDate;
                            voucherDetails.expiryDate = voucher.ExpiryDate;
                            memberInfo.voucherDetailsOfThor.Add(voucherDetails);
                        }
                     }
                   }
                 }
               }
            }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log(" exception is ---" + ex.Message);
            }
            return memberInfo;
        }
    }
}

