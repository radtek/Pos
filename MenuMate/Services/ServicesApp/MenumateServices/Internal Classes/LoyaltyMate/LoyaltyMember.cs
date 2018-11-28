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


        public LoyaltyMemberResponse GetMemberByUniqueCode(string inSyndicateCode, RequestInfo requestInfo)
        {
            return GetMemberFromCloud(inSyndicateCode, requestInfo);
        }

        public LoyaltyMemberResponse GetByCardCode(string inSyndicateCode, RequestInfo requestInfo)
        {
            return GetMemberByCodeFromCloud(inSyndicateCode, requestInfo);
        }

        public LoyaltyMemberResponse GetByEmail(string inSyndicateCode, RequestInfo requestInfo)
        {
            return GetMemberByEmailFromCloud(inSyndicateCode, requestInfo);
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction)
        {
            if (transaction.PointsType == 9)
            {
                return UploadInvoiceTransactionToCloud(inSyndicateCode, CreateInvoiceTransactionObject(transaction));
            }
            else
            {
                return UploadTransactionToCloud(inSyndicateCode, CreateTransactionObject(transaction));
            }
        }

        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.UpdateMemberCardCode(inSyndicateCode, CreateUpdateCardCodeRequest(uniqueId, memberCardCode));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to update member information to server",
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
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Card Code"))
                {
                    return CreateResponseError(ex.Message,
                            @"Card Code Already In Use.",
                            LoyaltyResponseCode.UpdateMemberFailed);
                }
                else
                {
                    return CreateResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.UpdateMemberFailed);
                }
            }
            catch (Exception ex)
            {
                return CreateResponseError(
                    "@Failed to update member information to server",
                    ex.Message,
                    LoyaltyResponseCode.UpdateMemberFailed);
            }
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
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Card Code"))
                {
                    return CreateMemberResponseError(ex.Message,
                            @"Card Code Already In Use.",
                            LoyaltyResponseCode.CreateMemberFailed,
                            CreateMemberInfo(@""));
                }
                else
                {
                    return CreateMemberResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.CreateMemberFailed,
                            CreateMemberInfo(@""));
                }
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(
                             @"Failed to create a member in the server",
                             exc.Message,
                             LoyaltyResponseCode.CreateMemberFailed,
                             CreateMemberInfo(@""));
            }
        }

        LoyaltyMemberResponse GetMemberFromCloud(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetMemberByUniqueId(CreateRequest(requestInfo), inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            CreateMemberInfo(requestInfo.RequestKey));
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(
                             @"Failed to request member's info from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfo(requestInfo.RequestKey));
            }
        }

        LoyaltyMemberResponse GetMemberByCodeFromCloud(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetMemberByCardCode(CreateRequest(requestInfo), inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Member Not"))
                {
                    return CreateMemberResponseError(ex.Message,
                            @"Member Not Exist.",
                            LoyaltyResponseCode.MemberNotExist,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
                else
                {
                    return CreateMemberResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.GetMemberFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(@"Failed to request member's info from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfoByCode(requestInfo.RequestKey));
            }
        }

        private LoyaltyMemberResponse GetMemberByEmailFromCloud(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetMemberByEmail(CreateRequest(requestInfo), inSyndicateCode);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Member Not"))
                {
                    return CreateMemberResponseError(ex.Message,
                            @"Member Not Exist.",
                            LoyaltyResponseCode.MemberNotExist,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
                else
                {
                    return CreateMemberResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.GetMemberFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
            }
            catch (Exception exc)
            {
                return CreateMemberResponseError(@"Failed to request member's info from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfoByCode(requestInfo.RequestKey));
            }
        }

        LoyaltyResponse UploadTransactionToCloud(string inSyndicateCode, PointsTransactionViewModel transaction)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.PostTransaction(transaction, inSyndicateCode);
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to post member transaction to server",
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
                    "@Failed to post member transaction to server",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed);
            }
        }

        LoyaltyResponse UploadInvoiceTransactionToCloud(string inSyndicateCode, ApiMemberInvoiceTransactionViewModel transaction)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.PostInvoiceTransaction(transaction, inSyndicateCode);
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to post member transaction to server",
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
            catch (MultipleGUIDException ex)
            {
                return CreateResponseError(
                            @"Multiple GUID exists",
                            ex.Message,
                            LoyaltyResponseCode.MultipleGUIDExist);
            }
            catch (GUIDNotFoundException ex)
            {
                return CreateResponseError(
                            @"GUID Not found",
                            ex.Message,
                            LoyaltyResponseCode.GUIDNotFound);
            }            
            catch (Exception ex)
            {
                return CreateResponseError(
                    "@Failed to post member transaction to server",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed);
            }
        }

        MemberInfo createMemberInfo(ApiMemberViewModel inMember)
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
                    result.HomeSiteId = membershipProfile.HomeSiteCode;
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
                    result.HasTransactions = membershipProfile.HasTransactions;
                    if (membershipProfile.LastModified > lastModified)
                        lastModified = membershipProfile.LastModified;
                    result.MemberVouchers = new List<VoucherInfo>();
                    if (membershipProfile.Vouchers != null)
                    {
                        foreach (var voucher in membershipProfile.Vouchers)
                        {
                            var memberVoucher = new VoucherInfo()
                            {
                                DiscountCode = voucher.DiscountCode,
                                VoucherName = voucher.Name,
                                VoucherDescription = voucher.Description,
                                NumberOfUsesRemaining = voucher.NumberOfUsesAllowed.HasValue ? voucher.NumberOfUsesAllowed.Value : 0
                            };
                            result.MemberVouchers.Add(memberVoucher);
                        }
                    }

                }
            }
            result.LastModified = lastModified.ToLocalTime();
            result.Activated = true;
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

        ApiMemberViewModel CreateMemberObject(MemberInfo inInfo)
        {
            var result = new ApiMemberViewModel();
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
            result.MembershipProfiles = new List<ApiMembershipProfileViewModel>();
            var membershipProfile = new ApiMembershipProfileViewModel();
            membershipProfile.MembershipNumber = inInfo.MembershipNumber;
            membershipProfile.CardCode = inInfo.MemberCardCode;
            membershipProfile.CardNumber = inInfo.CardNumber;
            membershipProfile.LastTransactionDate = inInfo.LastVisitDate.Value.Year == 1 ? null : inInfo.LastVisitDate;
            membershipProfile.YearStartDate = inInfo.YearStartDate.HasValue && inInfo.YearStartDate.Value.Year != 1 ? inInfo.YearStartDate.Value : DateTime.Now;
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

        PointsTransactionViewModel CreateTransactionObject(TransactionInfo inTransactionInfo)
        {
            return new PointsTransactionViewModel()
            {
                TransactionDate = inTransactionInfo.TransactionDate,
                PointsDelta = Math.Round(inTransactionInfo.PointsDelta, 2),
                PointType = (PointType)inTransactionInfo.PointsType,
                SiteCode = inTransactionInfo.SiteCode,
                InvoiceNumber = inTransactionInfo.InvoiceNumber,
                UniqueId = inTransactionInfo.UniqueId
            };
        }

        ApiMemberInvoiceTransactionViewModel CreateInvoiceTransactionObject(TransactionInfo inTransactionInfo)
        {
            return new ApiMemberInvoiceTransactionViewModel()
            {
                TransactionDate = inTransactionInfo.TransactionDate,
                SpendAmount = inTransactionInfo.PointsDelta,
                SiteCode = inTransactionInfo.SiteCode,
                InvoiceNumber = inTransactionInfo.InvoiceNumber,
                MemberUniqueId = inTransactionInfo.UniqueId
            };
        }

        ApiRequestViewModel CreateRequest(RequestInfo requestInfo)
        {
            return new ApiRequestViewModel()
            {
                RequestKey = requestInfo.RequestKey,
                RequestTime = requestInfo.RequestTime,
                SiteCode = requestInfo.SiteCode
            };
        }
        ApiUpdateCardCodeRequestViewModel CreateUpdateCardCodeRequest(string uniqueId, string memberCardCode)
        {
            return new ApiUpdateCardCodeRequestViewModel()
            {
                UniqueId = uniqueId,
                MemberCardCode = memberCardCode,
            };
        }


        #endregion




    }
}
