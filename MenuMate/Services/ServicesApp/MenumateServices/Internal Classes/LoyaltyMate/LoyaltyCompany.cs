
using System;
using MenumateServices.DTO.LoyaltyMate;
using System.Net;
using Loyaltymate.Model;
using Loyaltymate.Sevices;
using Loyaltymate.Exceptions;
using System.Collections.Generic;


namespace MenumateServices.LoyaltyMate
{
    public class LoyaltyCompany : LoyaltyResponsive
    {
        private static volatile LoyaltyCompany _instance;
        private static object syncRoot = new Object();

        private LoyaltyCompany()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static LoyaltyCompany Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new LoyaltyCompany();
                    }
                }

                return _instance;
            }
        }

        public LoyaltyCompanyResponse GetCompanyInformation(string inSyndicateCode, List<string> loyaltyLogs)
        {
            return GetCompanyInformationFromCloud(inSyndicateCode, loyaltyLogs);
        }

        LoyaltyCompanyResponse GetCompanyInformationFromCloud(string inSyndicateCode, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetCompanyDetail(inSyndicateCode, loyaltyLogs);
                loyaltyLogs.Add("Creating Company Response With No Error            ");
                return CreateCompanyResponseNoError(CreateCompanyInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                 " + ex.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateCompanyResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, new CompanyInfo());
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateCompanyResponseError(
                             @"Unable to sync with server",
                             exc.Message,
                             LoyaltyResponseCode.CompanySyncFailed,
                             new CompanyInfo());
            }
        }

        List<TierLevelInfo> CreateTierLevelInfoList(IEnumerable<ApiTierLevelViewModel> inCloudTierLevelList)
        {
            var result = new List<TierLevelInfo>();
            if (inCloudTierLevelList != null)
                foreach (var tierLevel in inCloudTierLevelList)
                {
                    result.Add(CreateTierInfo(tierLevel));
                }
            return result;
        }

        TierLevelInfo CreateTierInfo(ApiTierLevelViewModel inTierInfo)
        {
            var result = new TierLevelInfo();
            result.TierId = Convert.ToInt32(inTierInfo.TierLevelId);
            result.Name = inTierInfo.Name;
            result.Level = inTierInfo.TierLevelNumber;
            result.BirthdayBonus = inTierInfo.BirthdayBonus;
            result.PointsRequired = inTierInfo.PointsRequired;
            result.PricedRedemptionRate = inTierInfo.RedeemPointsRate;
            result.WeighedRedemptionPoints = inTierInfo.WeightedRedeemPoints;
            result.WeighedRedemptionWeight = inTierInfo.WeightedRedeemPointsValue;
            result.SendMailToHeadOffice = inTierInfo.SendMailToHeadQuarter;
            result.ChangeCard = inTierInfo.ChangeCard;
            result.SendMailToMember = inTierInfo.SendMailToUser;
            result.AllowEarntLoyaltyRedemption = inTierInfo.AllowEarntLoyaltyRedemption;
            return result;
        }

        List<DiscountInfo> CreateDiscountInfoList(IEnumerable<ApiDiscountViewModel> inCloudDiscountList)
        {
            var result = new List<DiscountInfo>();
            if (inCloudDiscountList != null)
                foreach (var discount in inCloudDiscountList)
                {
                    result.Add(CreateDiscountInfo(discount));
                }
            return result;
        }

        DiscountInfo CreateDiscountInfo(ApiDiscountViewModel inDiscountInfo)
        {
            var result = new DiscountInfo();
            result.DiscountId = inDiscountInfo.DiscountId;
            result.Name = inDiscountInfo.Name;
            result.Code = inDiscountInfo.Code;
            result.DiscountType = inDiscountInfo.DiscountType;
            result.ImplicationType = inDiscountInfo.ImplicationType;
            result.Value = inDiscountInfo.Value;
            result.RoundToDecimalPlaces = inDiscountInfo.RoundToDecimalPlaces;
            result.DisplayAs = inDiscountInfo.DisplayAs;
            result.Description = inDiscountInfo.Description;
            result.DiscountGroup = inDiscountInfo.DiscountGroup.HasValue ? inDiscountInfo.DiscountGroup.Value : default(int);
            result.IsMembersOnlyDiscount = inDiscountInfo.IsMembersOnlyDiscount;
            result.IsMemberExemptDiscount = inDiscountInfo.IsMemberExemptDiscount;
            result.PriorityOrder = inDiscountInfo.PriorityOrder;
            result.AppearanceOrder = inDiscountInfo.AppearanceOrder;
            result.IsCategoryFilterApplicable = inDiscountInfo.IsCategoryFilterApplicable;
            result.MaximumValue = inDiscountInfo.MaximumValue.HasValue ? inDiscountInfo.MaximumValue.Value : default(decimal);
            result.MinimumNumberOfItemsAllowed = inDiscountInfo.MinimumNumberOfItemsAllowed.HasValue ? inDiscountInfo.MinimumNumberOfItemsAllowed.Value : default(int);
            result.MaximumNumberOfItemsAllowed = inDiscountInfo.MaximumNumberOfItemsAllowed.HasValue ? inDiscountInfo.MaximumNumberOfItemsAllowed.Value : default(int);
            result.DailyUsageAllowedPerMember = inDiscountInfo.DailyUsageAllowedPerMember.HasValue ? inDiscountInfo.DailyUsageAllowedPerMember.Value : default(int);
            result.ProductPriority = inDiscountInfo.ProductPriority;
            result.IsAutoMembersDiscount = inDiscountInfo.IsAutoMembersDiscount;
            return result;
        }

        CompanyInfo CreateCompanyInfo(ApiCompanyViewModel inComapnyInfo)
        {
            var result = new CompanyInfo();
            result.TierLevels = CreateTierLevelInfoList(inComapnyInfo.TierLevels);
            result.Discounts = CreateDiscountInfoList(inComapnyInfo.Discounts);
            result.HasGiftCardsAvailable = inComapnyInfo.HasGiftCardsAvailable;
            result.HasPocketVouchersAvailable = inComapnyInfo.HasPocketVouchersAvailable;
            return result;
        }

    }
}
