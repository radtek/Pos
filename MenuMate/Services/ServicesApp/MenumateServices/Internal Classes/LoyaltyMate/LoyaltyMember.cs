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

        public LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo, List<string> loyaltyLogs)
        {
            return CreateMemberInTheCloud(inSyndicateCode, inInfo, loyaltyLogs);
        }


        public LoyaltyMemberResponse GetMemberByUniqueCode(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            return GetMemberFromCloud(inSyndicateCode, requestInfo, loyaltyLogs);
        }

        public LoyaltyMemberResponse GetByCardCode(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            return GetMemberByCodeFromCloud(inSyndicateCode, requestInfo, loyaltyLogs);
        }

        public LoyaltyMemberResponse GetByEmail(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            return GetMemberByEmailFromCloud(inSyndicateCode, requestInfo, loyaltyLogs);
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction, List<string> loyaltyLogs)
        {
            if (transaction.PointsType == 9)
            {
                loyaltyLogs.Add("Post Invoice for Points Type 9                                   ");
                return UploadInvoiceTransactionToCloud(inSyndicateCode, CreateInvoiceTransactionObject(transaction), loyaltyLogs);
            }
            else
            {
                loyaltyLogs.Add("Post Invoice for Points Type except 9                            ");
                return UploadTransactionToCloud(inSyndicateCode, CreateTransactionObject(transaction), loyaltyLogs);
            }
        }

        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Updating Member Card Code                                        ");
                var response = loyaltymateService.UpdateMemberCardCode(inSyndicateCode, CreateUpdateCardCodeRequest(uniqueId, memberCardCode), loyaltyLogs);
                if (response)
                {
                    loyaltyLogs.Add("Creating Response With No Error                              ");
                    return CreateResponseNoError();
                }
                else
                {
                    loyaltyLogs.Add("Creating Response With Error                                 ");
                    return CreateResponseError(
                        "@Failed to update member information to server",
                        "",
                        LoyaltyResponseCode.UpdateMemberFailed);
                }
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception                                  " + ex.Message);
                loyaltyLogs.Add("Time is                                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Card Code"))
                {
                    loyaltyLogs.Add("Loyaltymate Operation Exception                               " + ex.Message);
                    loyaltyLogs.Add("Time is                                                       " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateResponseError(ex.Message,
                            @"Card Code Already In Use.",
                            LoyaltyResponseCode.UpdateMemberFailed);
                }
                else
                {
                    loyaltyLogs.Add("Loyaltymate Operation Exception                               " + ex.Message);
                    loyaltyLogs.Add("Time is                                                       " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.UpdateMemberFailed);
                }
            }
            catch (Exception ex)
            {
                loyaltyLogs.Add("Exception is                                                      " + ex.Message);
                loyaltyLogs.Add("Time is                                                           " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(
                    "@Failed to update member information to server",
                    ex.Message,
                    LoyaltyResponseCode.UpdateMemberFailed);
            }
        }

        #endregion

        #region Private

        LoyaltyMemberResponse CreateMemberInTheCloud(string inSyndicateCode, MemberInfo inInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Saving Member                                       ");
                var response = loyaltymateService.SaveMember(CreateMemberObject(inInfo), inSyndicateCode, loyaltyLogs);
                return CreateMemberResponseNoError(createMemberInfo(response));
                
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                  "+ ex.Message);
                loyaltyLogs.Add("Time is                                             " + DateTime.Now.ToString("hh:mm:ss tt"));
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
                    loyaltyLogs.Add("Loyaltymate Operation Exception is               " + ex.Message);
                    loyaltyLogs.Add("Time is                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateMemberResponseError(ex.Message,
                            @"Card Code Already In Use.",
                            LoyaltyResponseCode.CreateMemberFailed,
                            CreateMemberInfo(@""));
                }
                else
                {
                    loyaltyLogs.Add("Loyaltymate error Exception                      " + ex.Message);
                    loyaltyLogs.Add("Time is                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateMemberResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.CreateMemberFailed,
                            CreateMemberInfo(@""));
                }
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                         " + exc.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(
                             @"Failed to create a member in the server",
                             exc.Message,
                             LoyaltyResponseCode.CreateMemberFailed,
                             CreateMemberInfo(@""));
            }
        }

        LoyaltyMemberResponse GetMemberFromCloud(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Getting Member By Unique Id                          ");
                var response = loyaltymateService.GetMemberByUniqueId(CreateRequest(requestInfo), inSyndicateCode, loyaltyLogs);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            CreateMemberInfo(requestInfo.RequestKey));
            }
            catch (MultipleGUIDException ex)
            {
                loyaltyLogs.Add("Multiple GUID Exception is                           " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(@"Multiple GUID exists", ex.Message, LoyaltyResponseCode.MultipleGUIDExist, CreateMemberInfo(requestInfo.RequestKey));
            }
            catch (GUIDNotFoundException ex)
            {
                loyaltyLogs.Add("GUID Not Found Exception is                          " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(@"GUID Not found",  ex.Message,  LoyaltyResponseCode.GUIDNotFound, CreateMemberInfo(requestInfo.RequestKey));
            }     
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                         " + exc.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(
                             @"Failed to request member's info from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfo(requestInfo.RequestKey));
            }
        }

        LoyaltyMemberResponse GetMemberByCodeFromCloud(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Getting Member By Card Code                              ");
                var response = loyaltymateService.GetMemberByCardCode(CreateRequest(requestInfo), inSyndicateCode, loyaltyLogs);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Member Not"))
                {
                    loyaltyLogs.Add("Loyaltymate Operation Exception is                   " + ex.Message);
                    loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateMemberResponseError(ex.Message,
                            @"Member Not Exist.",
                            LoyaltyResponseCode.MemberNotExist,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
                else
                {
                    loyaltyLogs.Add("Loyaltymate Operation Exception is                   " + ex.Message);
                    loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateMemberResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.GetMemberFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                       " + ex.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
            }
            catch (MultipleGUIDException ex)
            {
                loyaltyLogs.Add("Multiple GUID Exception is                              " + ex.Message);
                loyaltyLogs.Add("Time is                                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(@"Multiple GUID exists", ex.Message, LoyaltyResponseCode.MultipleGUIDExist, CreateMemberInfo(requestInfo.RequestKey));
            }
            catch (GUIDNotFoundException ex)
            {
                loyaltyLogs.Add("GUID Not Found Exception is                              " + ex.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(@"GUID Not found", ex.Message, LoyaltyResponseCode.GUIDNotFound, CreateMemberInfo(requestInfo.RequestKey));
            }     
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                             " + exc.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(@"Failed to request member's info from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfoByCode(requestInfo.RequestKey));
            }
        }

        private LoyaltyMemberResponse GetMemberByEmailFromCloud(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Getting Member By Email                                 ");
                var response = loyaltymateService.GetMemberByEmail(CreateRequest(requestInfo), inSyndicateCode, loyaltyLogs);
                return CreateMemberResponseNoError(createMemberInfo(response));
            }
            catch (LoyaltymateOperationException ex)
            {
                if (ex.Message.Contains("Member Not"))
                {
                    loyaltyLogs.Add("Loyaltymate Operation Exception is                  " + ex.Message);
                    loyaltyLogs.Add("Time is                                             " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateMemberResponseError(ex.Message,
                            @"Member Not Exist.",
                            LoyaltyResponseCode.MemberNotExist,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
                else
                {
                    loyaltyLogs.Add("Loyaltymate Operation Exception is                   " + ex.Message);
                    loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateMemberResponseError(ex.Message,
                            @"Loyaltymate error.",
                            LoyaltyResponseCode.GetMemberFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
                }
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                       " + ex.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            new MemberInfo() { MemberCardCode = requestInfo.RequestKey });
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                             " + exc.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateMemberResponseError(@"Failed to request member's info from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetMemberFailed,
                             CreateMemberInfoByCode(requestInfo.RequestKey));
            }
        }

        LoyaltyResponse UploadTransactionToCloud(string inSyndicateCode, PointsTransactionViewModel transaction, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Post transaction                                         ");
                var response = loyaltymateService.PostTransaction(transaction, inSyndicateCode, loyaltyLogs);
                
                if (response)
                {
                    loyaltyLogs.Add("Creating Response With No Error                      ");
                    return CreateResponseNoError();
                }
                else
                {
                    loyaltyLogs.Add("Creating Response Error                              ");
                    return CreateResponseError(
                        "@Failed to post member transaction to server",
                        "",
                        LoyaltyResponseCode.PostTransactionFailed);
                }
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                       " + ex.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (MultipleGUIDException ex)
            {
                loyaltyLogs.Add("Multiple GUID Exception is                               " + ex.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(@"Multiple GUID exists", ex.Message, LoyaltyResponseCode.MultipleGUIDExist);
            }
            catch (GUIDNotFoundException ex)
            {
                loyaltyLogs.Add("GUID Not Found Exception is                              " + ex.Message);
                loyaltyLogs.Add("Time is                                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(@"GUID Not found", ex.Message, LoyaltyResponseCode.GUIDNotFound);
            }     
            catch (Exception ex)
            {
                loyaltyLogs.Add("Exception is                                            " + ex.Message);
                loyaltyLogs.Add("Time is                                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(
                    "@Failed to post member transaction to server",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed);
            }
        }

        LoyaltyResponse UploadInvoiceTransactionToCloud(string inSyndicateCode, ApiMemberInvoiceTransactionViewModel transaction, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Post Invoice transaction                                ");
                var response = loyaltymateService.PostInvoiceTransaction(transaction, inSyndicateCode, loyaltyLogs);
                if (response)
                {
                    loyaltyLogs.Add("Creating Response With No Error                     ");
                    return CreateResponseNoError();
                }
                else
                {
                    loyaltyLogs.Add("Creating Response Error                             ");
                    return CreateResponseError(
                        "@Failed to post member transaction to server",
                        "",
                        LoyaltyResponseCode.PostTransactionFailed);
                }
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (MultipleGUIDException ex)
            {
                loyaltyLogs.Add("Multiple GUID Exception is                           " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(@"Multiple GUID exists", ex.Message, LoyaltyResponseCode.MultipleGUIDExist);
            }
            catch (GUIDNotFoundException ex)
            {
                loyaltyLogs.Add("GUID Not Found Exception is                          " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(@"GUID Not found", ex.Message, LoyaltyResponseCode.GUIDNotFound);
            }      
            catch (Exception ex)
            {
                loyaltyLogs.Add("Exception is                                         " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
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
