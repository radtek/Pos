﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.LoyaltyMate;
using Loyaltymate.Model.OnlineOrderingModel;
using Loyaltymate.Sevices;
using MenumateServices.DTO.LoyaltyMate;
using System.Net;
using MenumateServices.DTO.OnlineOrdering;
using Loyaltymate.Model.OnlineOrderingModel.TaxSettingModel;
using Loyaltymate.Utility;
using MenumateServices.DTO.OnlineOrdering.OrderModels;
using MenumateServices.DTO.OnlineOrdering.MenuModels;
using Loyaltymate.Model.OnlineOrderingModel.OrderModels;

namespace MenumateServices.Internal_Classes.LoyaltyMate
{
    public class LoyaltySite : LoyaltyResponsive
    {
        private static volatile LoyaltySite _instance;
        private static object _syncRoot = new Object();

        private LoyaltySite()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static LoyaltySite Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new LoyaltySite();
                    }
                }

                return _instance;
            }
        }

        #region Public


        public LoyaltyResponse SyncSiteMenu(string inSyndicateCode, SiteMenuInfo siteMenus)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.SyncSiteMenu(inSyndicateCode, CreateSiteMenuViewModel(siteMenus));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to update menu to server.",
                        "",
                        LoyaltyResponseCode.MenuSyncingFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        public LoyaltyResponse SyncSiteTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.SyncSiteTaxSettings(inSyndicateCode, CreateSiteTaxSettingsViewModel(siteTaxSettings));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to update tax settings to server.",
                        "",
                        LoyaltyResponseCode.TaxSettingSyncingFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        public LoyaltyResponse UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.UpdateOrderStatus(inSyndicateCode, siteOrderViewModel);
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to update order status.",
                        "",
                        LoyaltyResponseCode.UpdateOnlineOrderStatusFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        public LoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.PostOnlineOrderInvoiceInfo(inSyndicateCode, CreateSiteOrderViewModelForInvoice(siteOrderModel));

                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError("@Failed to post invoice info.", "", LoyaltyResponseCode.PostOnlineOrderInvoiceInfoFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        #endregion

        #region private

        //Creating Menu View Models
        private ApiSiteMenuViewModel CreateSiteMenuViewModel(SiteMenuInfo siteMenuInfo)
        {
            var siteMenuViewModel = new ApiSiteMenuViewModel();
            siteMenuViewModel.CompanyId = siteMenuInfo.CompanyId;
            siteMenuViewModel.SiteId = siteMenuInfo.SiteId;
            siteMenuViewModel.MenuConsumables = new List<ApiMenuConsumableViewModel>();
            if (siteMenuInfo.MenuConsumables != null)
            {
                foreach (var menu in siteMenuInfo.MenuConsumables)
                {
                    siteMenuViewModel.MenuConsumables.Add(CreateMenuConsumableViewModel(menu));
                }
            }
            var requestData = JsonUtility.Serialize<ApiSiteMenuViewModel>(siteMenuViewModel);//just to test json
            return siteMenuViewModel;
        }

        private ApiMenuConsumableViewModel CreateMenuConsumableViewModel(MenuConsumableInfo menuConsumableInfo)
        {
            var menuConsumableViewModel = new ApiMenuConsumableViewModel();
            menuConsumableViewModel.Description = menuConsumableInfo.Description;
            menuConsumableViewModel.Name = menuConsumableInfo.Name;
            menuConsumableViewModel.IsPalmable = menuConsumableInfo.IsPalmable;
            menuConsumableViewModel.MenuId = menuConsumableInfo.MenuId;
            menuConsumableViewModel.SiteMenuCourseId = menuConsumableInfo.SiteMenuCourseId;
            menuConsumableViewModel.Type = (Loyaltymate.Enum.MenuType)menuConsumableInfo.Type;
            menuConsumableViewModel.SiteCourses = new List<ApiCourseViewModel>();
            if (menuConsumableInfo.SiteCourses != null)
            {
                foreach (var siteCourse in menuConsumableInfo.SiteCourses)
                {
                    menuConsumableViewModel.SiteCourses.Add(CreateCourseViewModel(siteCourse));
                }
            }
            return menuConsumableViewModel;
        }

        private ApiCourseViewModel CreateCourseViewModel(CourseInfo courseInfo)
        {
            var courseViewModel = new ApiCourseViewModel();
            courseViewModel.CourseId = courseInfo.CourseId;
            courseViewModel.Description = courseInfo.Description;
            courseViewModel.Name = courseInfo.Name;
            courseViewModel.ServingCourseDescription = courseInfo.ServingCourseDescription;
            courseViewModel.ServingCourseName = courseInfo.ServingCourseName;
            courseViewModel.SiteMenuCourseId = courseInfo.SiteMenuCourseId;
            courseViewModel.Items = new List<ApiSiteItemViewModel>();
            if (courseInfo.Items != null)
            {
                foreach (var item in courseInfo.Items)
                {
                    courseViewModel.Items.Add(CreateItemSiteViewModel(item));
                }
            }
            return courseViewModel;
        }

        private ApiSiteItemViewModel CreateItemSiteViewModel(SiteItemInfo siteItem)
        {
            var siteItemViewModel = new ApiSiteItemViewModel();
            siteItemViewModel.CompanyId = siteItem.CompanyId;
            siteItemViewModel.Description = siteItem.Description;
            siteItemViewModel.ItemUniqueId = siteItem.ItemUniqueId;
            siteItemViewModel.Name = siteItem.Name;
            siteItemViewModel.OnlyAsSide = siteItem.OnlyAsSide;
            siteItemViewModel.SiteItemId = siteItem.SiteItemId;
            siteItemViewModel.SiteMenuCourseId = siteItem.SiteMenuCourseId;
            siteItemViewModel.ItemSizes = new List<ApiItemSizeViewModel>();
            if (siteItem.ItemSizes != null)
            {
                foreach (var itemSize in siteItem.ItemSizes)
                {
                    siteItemViewModel.ItemSizes.Add(CreateItemSizeViewModel(itemSize));
                }
            }
            siteItemViewModel.SideGroups = new List<ApiSideGroupViewModel>();
            if (siteItem.SideGroups != null)
            {
                foreach (var sideGroup in siteItem.SideGroups)
                {
                    siteItemViewModel.SideGroups.Add(CreateSideGroupViewModel(sideGroup));
                }
            }
            return siteItemViewModel;
        }

        private ApiSideGroupViewModel CreateSideGroupViewModel(SideGroupInfo sideGroupInfo)
        {
            var sideGroupViewModel = new ApiSideGroupViewModel();
            sideGroupViewModel.AllowSkip = sideGroupInfo.AllowSkip;
            sideGroupViewModel.Description = sideGroupInfo.Description;
            sideGroupViewModel.MaxSelect = sideGroupInfo.MaxSelect;
            sideGroupViewModel.Name = sideGroupInfo.Name;
            sideGroupViewModel.SideGroupId = sideGroupInfo.SideGroupId;
            sideGroupViewModel.SiteItemId = sideGroupInfo.SiteItemId;
            sideGroupViewModel.ItemSides = new List<ApiItemSideViewModel>();
            if (sideGroupInfo.ItemSides != null)
            {
                foreach (var itemSide in sideGroupInfo.ItemSides)
                {
                    sideGroupViewModel.ItemSides.Add(CreateItemSideViewModel(itemSide));
                }
            }
            return sideGroupViewModel;
        }

        private ApiItemSideViewModel CreateItemSideViewModel(ItemSideInfo itemSideInfo)
        {
            var itemSideViewModel = new ApiItemSideViewModel();
            itemSideViewModel.Description = itemSideInfo.Description;
            itemSideViewModel.ItemUniqueId = itemSideInfo.ItemUniqueId;
            itemSideViewModel.Name = itemSideInfo.Name;
            itemSideViewModel.OnlyAsSide = itemSideInfo.OnlyAsSide;
            itemSideViewModel.SiteItemId = itemSideInfo.SiteItemId;
            itemSideViewModel.ItemSizes = new List<ApiItemSizeViewModel>();
            if (itemSideInfo.ItemSizes != null)
            {
                foreach (var itemSize in itemSideInfo.ItemSizes)
                {
                    itemSideViewModel.ItemSizes.Add(CreateItemSizeViewModel(itemSize));
                }
            }
            return itemSideViewModel;
        }

        private ApiItemSizeViewModel CreateItemSizeViewModel(ItemSizeInfo itemSize)
        {
            var itemSizeViewModel = new ApiItemSizeViewModel();
            itemSizeViewModel.CanBePaidUsingPoints = itemSize.CanBePaidUsingPoints;
            itemSizeViewModel.DefaultPatronCount = itemSize.DefaultPatronCount;
            itemSizeViewModel.Description = itemSize.Description;
            itemSizeViewModel.IsFree = itemSize.IsFree;
            itemSizeViewModel.IsWeighted = itemSize.IsWeighted;
            itemSizeViewModel.ItemSizeId = itemSize.ItemSizeId;
            itemSizeViewModel.Name = itemSize.Name;
            itemSizeViewModel.OrderingUniqueId = itemSize.OrderingUniqueId;
            itemSizeViewModel.PointsPercentage = itemSize.PointsPercentage;
            itemSizeViewModel.PointsPrice = itemSize.PointsPrice;
            itemSizeViewModel.Price = itemSize.Price;
            itemSizeViewModel.SiteItemId = itemSize.SiteItemId;
            itemSizeViewModel.ThirdPartyId = itemSize.ThirdPartyId;
            itemSizeViewModel.ItemSizeTaxProfiles = new List<ApiItemSizeTaxProfileViewModel>();
            if (itemSize.ItemSizeTaxProfiles != null)
            {
                foreach (var taxProfile in itemSize.ItemSizeTaxProfiles)
                {
                    itemSizeViewModel.ItemSizeTaxProfiles.Add(CreateItemSizeTaxProfileViewModel(taxProfile));
                }
            }

            return itemSizeViewModel;
        }

        private ApiItemSizeTaxProfileViewModel CreateItemSizeTaxProfileViewModel(ItemSizeTaxProfileInfo taxProfile)
        {
            var itemSizeTaxProfileViewModel = new ApiItemSizeTaxProfileViewModel();
            itemSizeTaxProfileViewModel.Description = taxProfile.Description;
            itemSizeTaxProfileViewModel.ItemSizeTaxProfileId = taxProfile.ItemSizeTaxProfileId;
            itemSizeTaxProfileViewModel.Name = taxProfile.Name;
            itemSizeTaxProfileViewModel.Priority = taxProfile.Priority;
            itemSizeTaxProfileViewModel.Rate = taxProfile.Rate;
            itemSizeTaxProfileViewModel.TaxProfileId = taxProfile.TaxProfileId;
            itemSizeTaxProfileViewModel.Type = (Loyaltymate.Enum.TaxProfileType)taxProfile.Type;

            return itemSizeTaxProfileViewModel;
        }

        private ApiSiteTaxSettings CreateSiteTaxSettingsViewModel(SiteTaxSettingsinfo siteTaxSettingsInfo)
        {
            var siteTaxSettings = new ApiSiteTaxSettings();
            siteTaxSettings.SiteId = siteTaxSettingsInfo.SiteId;
            siteTaxSettings.ApiTaxSettings = new List<ApiTaxSettings>();

            if (siteTaxSettingsInfo.SiteTaxSettings != null)
            {
                foreach (var taxSetting in siteTaxSettingsInfo.SiteTaxSettings)
                {
                    siteTaxSettings.ApiTaxSettings.Add(CreateTaxSettingViewModel(taxSetting));
                }
            }

            var requestData = JsonUtility.Serialize<ApiSiteTaxSettings>(siteTaxSettings);//just to test json
            return siteTaxSettings;
        }

        private ApiTaxSettings CreateTaxSettingViewModel(TaxSettingsInfo taxSettingInfo)
        {
            var taxSetting = new ApiTaxSettings();
            taxSetting.SettingType = (Loyaltymate.Enum.SiteSettingType)taxSettingInfo.SettingType;
            taxSetting.Value = taxSettingInfo.Value;

            return taxSetting;
        }

        //Creating orderview Model received from signal R for storing order info for pos. 
        private ApiSiteOrderViewModel CreateSiteOrderViewModel(string ordersString)
        {
            ApiSiteOrderViewModel siteOrderViewModel = new ApiSiteOrderViewModel();
            try
            {
                siteOrderViewModel = JsonUtility.Deserialize<ApiSiteOrderViewModel>(ordersString);
            }
            catch (Exception ex)
            {
                return null;
            }
            return siteOrderViewModel;
        }

        //Creating orderview models for invoice info received from pos after billing.
        private ApiSiteOrderViewModel CreateSiteOrderViewModelForInvoice(SiteOrderModel siteOrderModel)
        {
            var siteOrderViewModel = new ApiSiteOrderViewModel();
            siteOrderViewModel.CompanyId = siteOrderModel.CompanyId;
            siteOrderViewModel.ContainerName = siteOrderModel.ContainerName;
            siteOrderViewModel.ContainerNumber = siteOrderModel.ContainerNumber;
            siteOrderViewModel.ContainerType = (Loyaltymate.Enum.OrderContainerType)siteOrderModel.ContainerType;
            siteOrderViewModel.IsConfirmed = siteOrderModel.IsConfirmed;
            siteOrderViewModel.Location = siteOrderModel.Location;
            siteOrderViewModel.OrderGuid = siteOrderModel.OrderGuid;
            siteOrderViewModel.OrderId = siteOrderModel.OrderId;
            siteOrderViewModel.OrderType = (Loyaltymate.Enum.OrderType)siteOrderModel.OrderType;
            siteOrderViewModel.SiteId = siteOrderModel.SiteId;
            siteOrderViewModel.TerminalName = siteOrderModel.TerminalName;
            siteOrderViewModel.TotalAmount = siteOrderModel.TotalAmount;
            siteOrderViewModel.TransactionDate = siteOrderModel.TransactionDate;
            siteOrderViewModel.TransactionType = (Loyaltymate.Enum.SiteSettingType)siteOrderModel.TransactionType;
            siteOrderViewModel.UserReferenceId = siteOrderModel.UserReferenceId;
            siteOrderViewModel.UserType = (Loyaltymate.Enum.UserType)siteOrderModel.UserType;
            siteOrderViewModel.UserEmailId = siteOrderModel.UserEmailId;
            siteOrderViewModel.MemberGuid = siteOrderModel.MemberGuid;
            siteOrderViewModel.IsHappyHourApplied = siteOrderModel.IsHappyHourApplied;
            siteOrderViewModel.OrderItems = new List<ApiOrderItemViewModel>();            
            siteOrderViewModel.OrderInvoiceTransaction = new ApiOrderInvoiceTransactionViewModel();

            if (siteOrderModel.OrderItems != null)
            {
                foreach (var item in siteOrderModel.OrderItems)
                {
                    siteOrderViewModel.OrderItems.Add(CreateItemsViewModelForInvoice(item));
                }
            }

            if (siteOrderModel.OrderInvoiceTransaction != null)
            {
                siteOrderViewModel.OrderInvoiceTransaction = CreateOrderInvoiceTransactionViewModel(siteOrderModel.OrderInvoiceTransaction);    
            }
            var requestData = JsonUtility.Serialize<ApiSiteOrderViewModel>(siteOrderViewModel);//just to test json

            return siteOrderViewModel;
        }

        private ApiOrderItemViewModel CreateItemsViewModelForInvoice(OrderItemModel item)
        {
            var orderItemViewModel = new ApiOrderItemViewModel();
            orderItemViewModel.Description = item.Description;
            orderItemViewModel.Name = item.Name;
            orderItemViewModel.OrderId = item.OrderId;
            orderItemViewModel.OrderItemId = item.OrderItemId;
            orderItemViewModel.Price = item.Price;
            orderItemViewModel.SiteItemId = item.SiteItemId;
            orderItemViewModel.ItemUniqueId = item.ItemUniqueId;
            orderItemViewModel.OrderItemSizes = new List<ApiOrderItemSizeViewModel>();

            if (item.OrderItemSizes != null)
            {
                foreach (var itemSize in item.OrderItemSizes)
                {
                    orderItemViewModel.OrderItemSizes.Add(CreateItemSizeViewModelForInvoice(itemSize));
                }
            }
            return orderItemViewModel;
        }

        private ApiOrderItemSizeViewModel CreateItemSizeViewModelForInvoice(OrderItemSizeModel itemSize)
        {
            var orderItemSizeModel = new ApiOrderItemSizeViewModel();
            orderItemSizeModel.BasePrice = itemSize.BasePrice;
            orderItemSizeModel.ItemSizeId = itemSize.ItemSizeId;
            orderItemSizeModel.MenuPrice = itemSize.MenuPrice;
            orderItemSizeModel.Name = itemSize.Name;
            orderItemSizeModel.OrderItemId = itemSize.OrderItemId;
            orderItemSizeModel.OrderItemSizeId = itemSize.ItemSizeId;
            orderItemSizeModel.Price = itemSize.Price;
            orderItemSizeModel.PriceInclusive = itemSize.Price;
            orderItemSizeModel.Quantity = itemSize.Quantity;
            orderItemSizeModel.ReferenceOrderItemSizeId = itemSize.ReferenceOrderItemSizeId;
            orderItemSizeModel.ItemSizeUniqueId = itemSize.ItemSizeUniqueId;
            orderItemSizeModel.OrderItemSizeDiscounts = new List<ApiOrderItemSizeDiscountViewModel>();
            orderItemSizeModel.OrderItemSizeTaxProfiles = new List<ApiOrderItemSizeTaxProfileViewModel>();

            if (itemSize.OrderItemSizeDiscounts != null)
            {
                foreach (var itemSizeDiscount in itemSize.OrderItemSizeDiscounts)
                {
                    orderItemSizeModel.OrderItemSizeDiscounts.Add(CreateItemSizeDiscountViewModelForInvoice(itemSizeDiscount));
                }
            }

            if (itemSize.OrderItemSizeTaxProfiles != null)
            {
                foreach (var itemSizeTaxProfile in itemSize.OrderItemSizeTaxProfiles)
                {
                    orderItemSizeModel.OrderItemSizeTaxProfiles.Add(CreateItemSizeTaxProfileViewModelForInvoice(itemSizeTaxProfile));
                }
            }

            return orderItemSizeModel;
        }

        private ApiOrderItemSizeDiscountViewModel CreateItemSizeDiscountViewModelForInvoice(OrderItemSizeDiscountModel itemSizeDiscount)
        {
            var orderItemSizeDiscountViewModel = new ApiOrderItemSizeDiscountViewModel();
            orderItemSizeDiscountViewModel.Code = itemSizeDiscount.Code;
            orderItemSizeDiscountViewModel.Name = itemSizeDiscount.Name;
            orderItemSizeDiscountViewModel.OrderItemSizeDiscountId = itemSizeDiscount.OrderItemSizeDiscountId;
            orderItemSizeDiscountViewModel.OrderItemSizeId = itemSizeDiscount.OrderItemSizeId;
            orderItemSizeDiscountViewModel.Value = itemSizeDiscount.Value;
            return orderItemSizeDiscountViewModel;
        }

        private ApiOrderItemSizeTaxProfileViewModel CreateItemSizeTaxProfileViewModelForInvoice(OrderItemSizeTaxProfileModel itemSizeTaxProfile)
        {
            var orderItemSizeTaxViewModel = new ApiOrderItemSizeTaxProfileViewModel();
            orderItemSizeTaxViewModel.CompanyId = itemSizeTaxProfile.CompanyId;
            orderItemSizeTaxViewModel.Description = itemSizeTaxProfile.Description;
            orderItemSizeTaxViewModel.ItemSizeTaxProfileId = itemSizeTaxProfile.ItemSizeTaxProfileId;
            orderItemSizeTaxViewModel.Name = itemSizeTaxProfile.Name;
            orderItemSizeTaxViewModel.OrderItemSizeId = itemSizeTaxProfile.OrderItemSizeId;
            orderItemSizeTaxViewModel.OrderItemSizeTaxProfileId = itemSizeTaxProfile.OrderItemSizeTaxProfileId;
            orderItemSizeTaxViewModel.Percentage = itemSizeTaxProfile.Percentage;
            orderItemSizeTaxViewModel.Priority = itemSizeTaxProfile.Priority;
            orderItemSizeTaxViewModel.Rate = itemSizeTaxProfile.Rate;
            orderItemSizeTaxViewModel.TaxProfileType = (Loyaltymate.Enum.TaxProfileType)itemSizeTaxProfile.TaxProfileType;
            orderItemSizeTaxViewModel.Value = itemSizeTaxProfile.Value;
            return orderItemSizeTaxViewModel;
        }

        private ApiOrderInvoiceTransactionViewModel CreateOrderInvoiceTransactionViewModel(OrderInvoiceTransactionModel invoiceTransactionModel)
        {
            var orderinvoiceTransactionViewModel = new ApiOrderInvoiceTransactionViewModel();
            orderinvoiceTransactionViewModel.InvoiceTransactionId = invoiceTransactionModel.InvoiceTransactionId;
            orderinvoiceTransactionViewModel.OrderId = invoiceTransactionModel.OrderId;
            orderinvoiceTransactionViewModel.OrderInvoiceTransactionId = invoiceTransactionModel.OrderInvoiceTransactionId;
            orderinvoiceTransactionViewModel.InvoiceTransaction = new ApiInvoiceTransactionViewModel();
            if (invoiceTransactionModel.InvoiceTransaction != null)
            {
                orderinvoiceTransactionViewModel.InvoiceTransaction = CreateInvoiceTransactionViewModel(invoiceTransactionModel.InvoiceTransaction);
            }
            return orderinvoiceTransactionViewModel;
        }

        private ApiInvoiceTransactionViewModel CreateInvoiceTransactionViewModel(InvoiceTransactionModel invoiceTransactionModel)
        {
            var invoiceTransactionViewModel = new ApiInvoiceTransactionViewModel();
            invoiceTransactionViewModel.InvoiceNumber = invoiceTransactionModel.InvoiceNumber;
            invoiceTransactionViewModel.InvoiceTransactionId = invoiceTransactionModel.InvoiceTransactionId;
            invoiceTransactionViewModel.Receipt = invoiceTransactionModel.Receipt;
            invoiceTransactionViewModel.ReceiptPath = invoiceTransactionModel.ReceiptPath;
            invoiceTransactionViewModel.Rounding = invoiceTransactionModel.Rounding;
            invoiceTransactionViewModel.SiteId = invoiceTransactionModel.SiteId;
            invoiceTransactionViewModel.TerminalName = invoiceTransactionModel.TerminalName;
            invoiceTransactionViewModel.TotalSaleAmount = invoiceTransactionModel.TotalSaleAmount;
            invoiceTransactionViewModel.TransactionDate = invoiceTransactionModel.TransactionDate;
            invoiceTransactionViewModel.UserReferenceId = invoiceTransactionModel.UserReferenceId;
            invoiceTransactionViewModel.UserType = (Loyaltymate.Enum.UserType)invoiceTransactionModel.UserType;
            return invoiceTransactionViewModel;
        }
        
        #endregion



    }
}