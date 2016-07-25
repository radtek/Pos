using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Authentication;
using Loyaltymate.Enum;
using Loyaltymate.Sevices;
using MenumateServices.DTO.LoyaltyMate;
using System.Net;
using Loyaltymate.Model;
using Loyaltymate.Exceptions;

namespace MenumateServices.LoyaltyMate
{
    public class LoyaltyMember : LoyaltyResponsive
    {
        private static volatile LoyaltyMember _instance;
        private static object _syncRoot = new Object();

        private LoyaltyMember()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static LoyaltyMember Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new LoyaltyMember();
                    }
                }

                return _instance;
            }
        }

        #region Public

        public LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo)
        {
            return CreateMemberInTheCloud(inSyndicateCode, inInfo);
        }


        public LoyaltyResponse DeleteMember(string inSyndicateCode, string inUniqueId)
        {
            return DeleteMemberFromCloud(inSyndicateCode, inUniqueId);
        }

        public LoyaltyMemberResponse GetMemberByUniqueCode(string inSyndicateCode, string inUniqueId)
        {
            return GetMemberFromCloud(inSyndicateCode, inUniqueId);
        }

        public LoyaltyMemberResponse GetByCardCode(string inSyndicateCode, string inMemberCode)
        {
            return GetMemberByCodeFromCloud(inSyndicateCode, inMemberCode);
        }

        public LoyaltyMemberResponse GetByEmail(string inSyndicateCode, string inMemberEmail)
        {
            return GetMemberByEmailFromCloud(inSyndicateCode, inMemberEmail);
        }

        public LoyaltyMemberListResponse GetMemberList(string inSyndicateCode)
        {
            return GetMemberListFromCloud(inSyndicateCode);
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction)
        {
            return UploadTransactionToCloud(inSyndicateCode, CreateTransactionObject(transaction), transaction.UniqueId);
        }

        public LoyaltyPointsInfoResponse GetPointsInRange(string inSyndicateCode, PointsInfo pointsInfo)
        {
            return GetPointsInRangeFromCloud(inSyndicateCode, pointsInfo);
        }

        #endregion

        #region Private

        LoyaltyMemberResponse CreateMemberInTheCloud(string inSyndicateCode, MemberInfo inInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.SaveMember(CreateMemberObject(inInfo), inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            inInfo);
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(
                             @"Failed to create a member in the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.CreateMemberFailed,
                             CreateMemberInfo(@""));
            }
        }

        LoyaltyResponse DeleteMemberFromCloud(string inSyndicateCode, string inUniqueId)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.DeleteMember(inUniqueId, inSyndicateCode);
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
                return CreateResponseError(
                            @"Failed to delete a member from the Cloud",
                            exc.Message,
                            LoyaltyResponseCode.DeleteMemberFailed);
            }
        }

        LoyaltyMemberResponse GetMemberFromCloud(string inSyndicateCode, string inUniqueId)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetMemberByUniqueId(inUniqueId, inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            CreateMemberInfo(inUniqueId));
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(
                             @"Failed to request member's info from the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfo(inUniqueId));
            }
        }

        LoyaltyMemberResponse GetMemberByCodeFromCloud(string inSyndicateCode, string inMemberCode)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetMemberByCardCode(inMemberCode, inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (MemberNotExistException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.MemberNotExist,
                            new MemberInfo() { MemberCardCode = inMemberCode });
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            new MemberInfo() { MemberCardCode = inMemberCode });
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(@"Failed to request member's info from the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfoByCode(inMemberCode));
            }
        }

        private LoyaltyMemberResponse GetMemberByEmailFromCloud(string inSyndicateCode, string inMemberEmail)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetMemberByEmail(inMemberEmail, inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            new MemberInfo() { MemberCardCode = inMemberEmail });
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(@"Failed to request member's info from the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfoByEmail(inMemberEmail));
            }
        }

        public LoyaltyMemberListResponse GetMemberListFromCloud(string inSyndicateCode)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetAllMember(inSyndicateCode);
                return CreateMemberListResponseNoError(CreateMemberInfoList(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberListResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception exc)
            {
                return CreateMemberListResponseError(
                             @"Failed to request member's info list from the Cloud",
                             exc.Message,
                             LoyaltyResponseCode.CreateMemberFailed);
            }
        }

        LoyaltyResponse UploadTransactionToCloud(string inSyndicateCode, PointsTransactionViewModel transaction, string uniqueId)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.PostTransaction(transaction, inSyndicateCode, uniqueId);
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to post member transaction to cloud",
                        "",
                        LoyaltyResponseCode.PostTransactionFailed);
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception ex)
            {
                return CreateResponseError(
                    "@Failed to post member transaction to cloud",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed);
            }
        }

        LoyaltyPointsInfoResponse GetPointsInRangeFromCloud(string inSyndicateCode, PointsInfo pointsInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetPointsInRange(CreatePointsInfoObject(pointsInfo), inSyndicateCode);
                return CreatePointsInfoResponseNoError(CreatePointsInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreatePointsInfoResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, CreatePointsInfo(new PointQuery() { Balance = 0 }));
            }
            catch (Exception ex)
            {
                return CreatePointsInfoResponseError(
                    "@Failed to Get point balance for member",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed, CreatePointsInfo(new PointQuery() { Balance = 0 }));
            }
        }

        MemberInfo createMemberInfo(MemberViewModel inMember)
        {
            var result = new MemberInfo();
            //::::::::::::::::::::::::::::::::::::
            result.UniqueId = Convert.ToString(inMember.UniqueId);
            result.MemberType = Convert.ToInt32(inMember.MemberType);
            result.Email = inMember.Email;
            result.Address1 = inMember.AddressLine1;
            result.Address2 = inMember.AddressLine2;
            result.Mobile = inMember.Mobile;
            result.PhoneNumber = inMember.Phone;
            result.City = inMember.CityName;
            result.Country = inMember.CountryName;
            result.DateOfBirth = inMember.Birthday;
            result.FirstName = inMember.FirstName;
            result.LastName = inMember.LastName;
            result.State = inMember.StateName;
            result.Title = inMember.Title;
            result.ZipCode = inMember.PostalCode;
            DateTime lastModified = inMember.LastModified;
            if (inMember.MembershipProfiles != null)
            {
                var membershipProfile = inMember.MembershipProfiles.SingleOrDefault();
                if (membershipProfile != null)
                {
                    result.BirthdayBenefitDate = membershipProfile.LastBirthdayBenefitDate;
                    result.HomeSiteId = membershipProfile.HomeSiteCode.HasValue ? membershipProfile.HomeSiteCode.Value : 0;
                    result.LastVisitDate = membershipProfile.LastTransactionDate;
                    result.TierId = membershipProfile.TierLevelId.HasValue ? membershipProfile.TierLevelId.Value : 0;
                    result.YearStartDate = membershipProfile.YearStartDate;
                    result.CardNumber = membershipProfile.CardNumber;
                    result.MemberCardCode = membershipProfile.CardCode;
                    result.MembershipNumber = membershipProfile.MembershipNumber;
                    result.PointRule = membershipProfile.PointRule;
                    result.PreviousYearPoint = membershipProfile.PreviousYearPoint;
                    result.CurrentYearPoint = membershipProfile.CurrentYearPoint;
                    result.AvailableBirthDayPoint = membershipProfile.AvailableBirthDayPoint;
                    result.AvailableFirstVisitPoint = membershipProfile.AvailableFirstVisitPoint;
                    result.EarnedPoints = membershipProfile.EarnedPoints;
                    result.LoadedPoints = membershipProfile.LoadedPoints;
                    result.IsFirstVisitRewarded = membershipProfile.IsFirstVisitRewarded;
                    if (membershipProfile.LastModified > lastModified)
                        lastModified = membershipProfile.LastModified;
                }
            }
            result.LastModified = lastModified.ToLocalTime();
            result.Activated = true;
            //result.ActivationToken = inMember.activation_token;
            return result;
        }

        MemberInfo CreateMemberInfo(string inUniqueId)
        {
            var result = new MemberInfo();
            result.UniqueId = inUniqueId;
            return result;
        }

        MemberInfo CreateMemberInfoByCode(string inMemberCode)
        {
            var result = new MemberInfo();
            result.MemberCardCode = inMemberCode;
            return result;
        }

        MemberInfo CreateMemberInfoByEmail(string inMemberEmail)
        {
            var result = new MemberInfo();
            result.Email = inMemberEmail;
            return result;
        }

        List<MemberInfo> CreateMemberInfoList(IEnumerable<MemberViewModel> inCloudMemberList)
        {
            var result = new List<MemberInfo>();
            foreach (var member in inCloudMemberList)
            {
                result.Add(createMemberInfo(member));
            }
            return result;
        }

        MemberViewModel CreateMemberObject(MemberInfo inInfo)
        {
            var result = new MemberViewModel();
            result.Email = inInfo.Email;
            result.MemberType = (MemberType)inInfo.MemberType;
            result.Title = inInfo.Title;
            result.FirstName = inInfo.FirstName;
            result.LastName = inInfo.LastName;
            result.MiddleName = inInfo.MiddleName;
            result.Mobile = inInfo.Mobile;
            result.Phone = inInfo.PhoneNumber;
            result.Gender = (Gender)inInfo.Gender;
            result.AddressLine1 = inInfo.Address1;
            result.AddressLine2 = inInfo.Address2;
            result.CityName = inInfo.City;
            result.StateName = inInfo.State;
            result.CountryName = inInfo.Country;
            result.PostalCode = inInfo.ZipCode;
            result.Birthday = inInfo.DateOfBirth.Value.Year == 1 ? null : inInfo.DateOfBirth;
            result.AnniversaryDate = null;
            Guid guid = Guid.Empty;
            if (Guid.TryParse(inInfo.UniqueId, out guid))
            {
                result.UniqueId = guid;
            }
            else
            {
                result.UniqueId = Guid.Empty;
            }
            result.MembershipProfiles = new List<MembershipProfileViewModel>();
            var membershipProfile = new MembershipProfileViewModel();
            membershipProfile.MembershipNumber = inInfo.MembershipNumber;
            membershipProfile.CardCode = inInfo.MemberCardCode;
            membershipProfile.CardNumber = inInfo.CardNumber;
            membershipProfile.LastTransactionDate = inInfo.LastVisitDate.Value.Year == 1 ? null : inInfo.LastVisitDate;
            membershipProfile.YearStartDate = inInfo.YearStartDate.Value.Year == 1 ? DateTime.Now : inInfo.YearStartDate;
            membershipProfile.LastBirthdayBenefitDate = inInfo.BirthdayBenefitDate.Value.Year == 1 ? null : inInfo.BirthdayBenefitDate;
            membershipProfile.IsFirstVisitRewarded = inInfo.IsFirstVisitRewarded;
            membershipProfile.HomeSiteCode = inInfo.HomeSiteId;
            membershipProfile.TierLevelId = inInfo.TierId;
            membershipProfile.PointRule = inInfo.PointRule;
            membershipProfile.EarnedPoints = inInfo.EarnedPoints;
            membershipProfile.LoadedPoints = inInfo.LoadedPoints;
            if (inInfo.TierId == 0)
                membershipProfile.TierLevelId = null;
            result.MembershipProfiles.Add(membershipProfile);

            return result;
        }

        PointQuery CreatePointsInfoObject(PointsInfo inInfo)
        {
            var result = new PointQuery();
            result.UniqueId = inInfo.UniqueId;
            result.StartDate = inInfo.StartDate;
            result.EndDate = inInfo.EndDate;
            result.PointType = (PointType)inInfo.PointsType;
            return result;
        }

        PointsInfo CreatePointsInfo(PointQuery inInfo)
        {
            return new PointsInfo()
            {
                Balance = inInfo.Balance
            };
        }

        PointsTransactionViewModel CreateTransactionObject(TransactionInfo inTransactionInfo)
        {
            return new PointsTransactionViewModel()
            {
                TransactionDate = inTransactionInfo.TransactionDate,
                PointsDelta = inTransactionInfo.PointsDelta,
                PointType = (PointType)inTransactionInfo.PointsType,
                SiteCode = inTransactionInfo.SiteCode
            };
        }

        #endregion



        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.UpdateMemberCardCode(inSyndicateCode, uniqueId, memberCardCode);
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to update member information to cloud",
                        "",
                        LoyaltyResponseCode.UpdateMemberFailed);
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception ex)
            {
                return CreateResponseError(
                    "@Failed to update member information to cloud",
                    ex.Message,
                    LoyaltyResponseCode.UpdateMemberFailed);
            }
        }
    }
}
