
using System;
using MenumateServices.DTO.LoyaltyMate;
using System.Net;
using Loyaltymate.Model;
using Loyaltymate.Sevices;
using Loyaltymate.Exceptions;
using System.Collections.Generic;


namespace MenumateServices.LoyaltyMate
{
    public class LoyaltyTier : LoyaltyResponsive
    {
        private static volatile LoyaltyTier _instance;
        private static object syncRoot = new Object();

        private LoyaltyTier()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static LoyaltyTier Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new LoyaltyTier();
                    }
                }

                return _instance;
            }
        }

        public LoyaltyTierResponse SaveTierLevel(string inSyndicateCode, TierLevelInfo inInfo)
        {
            return CreateTierInTheCloud(inSyndicateCode,  inInfo);
        }

        public LoyaltyResponse Delete(string inSyndicateCode,  int tierLevelId)
        {
            return DeleteTierFromTheCloud(inSyndicateCode, tierLevelId);
        }

        public LoyaltyTierResponse GetTierLevel(string inSyndicateCode,  int tierId)
        {
            return GetTierFromTheCloud(inSyndicateCode,  tierId);
        }

        public LoyaltyTierListResponse GetAllTierLevel(string inSyndicateCode)
        {
            return GetAllTierLevelFromCloud(inSyndicateCode);
        }

        LoyaltyTierListResponse GetAllTierLevelFromCloud(string inSyndicateCode)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetAllTierLevels(inSyndicateCode);
                return CreateTierListResponseNoError(CreateTierLevelInfoList(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateTierListResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, new List<TierLevelInfo>());
            }
            catch (Exception exc)
            {
                return CreateTierListResponseError(
                             @"Failed to request tier's info from the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.DeleteTierFailed,
                             new List<TierLevelInfo>());
            }
        }

        LoyaltyTierResponse CreateTierInTheCloud(string inSyndicateCode,  TierLevelInfo inInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.SaveTierLevel(CreateTierObject(inInfo), inSyndicateCode);
                return CreateTierResponseNoError(CreateTierInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateTierResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, CreateTierInfo(0));
            }
            catch (Exception exc)
            {
                return CreateTierResponseError(@"Failed to create a tier in the Cloud", exc.Message,
                             LoyaltyResponseCode.CreateTierFailed, CreateTierInfo(0));
            }
        }

        LoyaltyResponse DeleteTierFromTheCloud(string inSyndicateCode, int tierLevelId)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.DeleteTierLevel(tierLevelId, inSyndicateCode);
                return CreateResponseNoError();
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception exc)
            {
                return CreateResponseError(@"Failed to delete a tier from the Cloud", exc.Message,
                                            LoyaltyResponseCode.DeleteTierFailed);
            }
        }

        LoyaltyTierResponse GetTierFromTheCloud(string inSyndicateCode,  int tierId)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetTierLevel(tierId, inSyndicateCode);
                return CreateTierResponseNoError(CreateTierInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateTierResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, CreateTierInfo(tierId));
            }
            catch (Exception exc)
            {
                return CreateTierResponseError(
                             @"Failed to request tier's info from the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.DeleteTierFailed,
                             CreateTierInfo(tierId));
            }
        }

        List<TierLevelInfo> CreateTierLevelInfoList(IEnumerable<TierLevelViewModel> inCloudTierLevlList)
        {
            var result = new List<TierLevelInfo>();
            foreach (var member in inCloudTierLevlList)
            {
                result.Add(CreateTierInfo(member));
            }
            return result;
        }

        TierLevelInfo CreateTierInfo(TierLevelViewModel inTierInfo)
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

        TierLevelInfo CreateTierInfo(int tierId)
        {
            var result = new TierLevelInfo();
            result.TierId = tierId;
            return result;
        }

        TierLevelViewModel CreateTierObject(TierLevelInfo inInfo)
        {
            var result = new TierLevelViewModel();
            result.Name = inInfo.Name;
            result.TierLevelNumber = inInfo.Level;
            result.BirthdayBonus = inInfo.BirthdayBonus;
            result.PointsRequired = inInfo.PointsRequired;
            result.RedeemPointsRate = inInfo.PricedRedemptionRate;
            result.WeightedRedeemPoints = inInfo.WeighedRedemptionPoints;
            result.WeightedRedeemPointsValue = inInfo.WeighedRedemptionWeight;
            result.SendMailToHeadQuarter = inInfo.SendMailToHeadOffice;
            result.ChangeCard = inInfo.ChangeCard;
            result.SendMailToUser = inInfo.SendMailToMember;
            result.AllowEarntLoyaltyRedemption = inInfo.AllowEarntLoyaltyRedemption;
            return result;
        }



    }
}
