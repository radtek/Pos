using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using OnlineOrdering.Model.MenuModels;
using OnlineOrdering.Services;
using OnlineOrdering.Model.TaxSettingsModels;
using OnlineOrdering.Utility;
using OnlineOrdering.Model.OrderModels;
using MenumateServices.DTO.MenumateOnlineOrdering.OrderModels;
using MenumateServices.DTO.MenumateOnlineOrdering.MenuModels;
using MenumateServices.DTO.MenumateOnlineOrdering;

namespace MenumateServices.Internal_Classes.MenumateOnlineOrdering
{
    public class LoyaltySite : OOLoyaltyResponsive
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


        public OOLoyaltyResponse SyncSiteMenu(string inSyndicateCode, SiteMenuInfo siteMenus, List<string> stringList)
        {
            try
            {
                IOnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                stringList.Add("Syncing Site Menu                                  ");
                var response = onlineOrderingService.SyncSiteMenu(inSyndicateCode, CreateSiteMenuViewModel(siteMenus), stringList);
                if (response)
                {
                    stringList.Add("Creating Response With No Error                    ");
                    return CreateResponseNoError();
                }
                else
                {
                    stringList.Add("Creating Response With Error                       ");
                    return CreateResponseError(
                        "@Failed to update menu to server.",
                        "",
                        OOLoyaltyResponseCode.MenuSyncingFailed);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }
        }

        public OOLoyaltyResponse SyncSiteTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings, List<string> stringList)
        {
            try
            {
                IOnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                stringList.Add("Syncing Site Tax Settings                          ");
                var response = onlineOrderingService.SyncSiteTaxSettings(inSyndicateCode, CreateSiteTaxSettingsViewModel(siteTaxSettings), stringList);
                if (response)
                {
                    stringList.Add("Creating Response With No Error                    ");
                    return CreateResponseNoError();
                }
                else
                {
                    stringList.Add("Creating Response With Error                       ");
                    return CreateResponseError(
                        "@Failed to update tax settings to server.",
                        "",
                        OOLoyaltyResponseCode.TaxSettingSyncingFailed);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }
        }

        public OOLoyaltyResponse UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel, List<string> stringList)
        {
            try
            {
                IOnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                stringList.Add("Updating Order Status                              ");
                var response = onlineOrderingService.UpdateOrderStatus(inSyndicateCode, siteOrderViewModel, stringList);
                if (response)
                {
                    stringList.Add("Creating Response With No Error                    ");
                    return CreateResponseNoError();
                }
                else
                {
                    stringList.Add("Creating Response With Error                       ");
                    return CreateResponseError(
                        "@Failed to update order status.",
                        "",
                        OOLoyaltyResponseCode.UpdateOnlineOrderStatusFailed);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }
        }

        public OOLoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel, List<string> stringList)
        {
            try
            {
                IOnlineOrderingService onlineOrderingService = new OnlineOrderingService();
                stringList.Add("Posting Online Orde rInvoice Info                  ");
                var response = onlineOrderingService.PostOnlineOrderInvoiceInfo(inSyndicateCode, CreateSiteOrderViewModelForInvoice(siteOrderModel), stringList);

                if (response)
                {
                    stringList.Add("Creating Response With No Error                    ");
                    return CreateResponseNoError();
                }
                else
                {
                    stringList.Add("Creating Response With Error                       ");
                    return CreateResponseError("@Failed to post invoice info.", "", OOLoyaltyResponseCode.PostOnlineOrderInvoiceInfoFailed);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }
        }
        public SiteOrderModel CreateSiteOrderModel(ApiSiteOrderViewModel siteOrderViewModel, List<string> stringList)
        {
            SiteOrderModel siteOrderModel = new SiteOrderModel();
            try
            {
                stringList.Add("Creating Site Order Model                          ");
                siteOrderModel.CompanyId = siteOrderViewModel.CompanyId;
                siteOrderModel.SiteId = siteOrderViewModel.SiteId;
                siteOrderModel.OrderId = siteOrderViewModel.OrderId;
                siteOrderModel.Location = siteOrderViewModel.Location;
                siteOrderModel.TotalAmount = siteOrderViewModel.TotalAmount;
                siteOrderModel.ContainerNumber = siteOrderViewModel.ContainerNumber;
                siteOrderModel.ContainerType = siteOrderViewModel.ContainerType == OnlineOrdering.Enum.OrderContainerType.Tab ? DTO.Enum.OrderContainerType.Tab: DTO.Enum.OrderContainerType.Table;
                siteOrderModel.ContainerName = siteOrderViewModel.ContainerName;
                siteOrderModel.OrderGuid = siteOrderViewModel.OrderGuid;
                siteOrderModel.UserReferenceId = siteOrderViewModel.UserReferenceId;
                siteOrderModel.UserType = siteOrderViewModel.UserType == OnlineOrdering.Enum.UserType.Member? DTO.Enum.UserType.Member : DTO.Enum.UserType.Staff;
                siteOrderModel.TerminalName = siteOrderViewModel.TerminalName;
                siteOrderModel.TransactionDate = siteOrderViewModel.TransactionDate;
                siteOrderModel.OrderType = LoadOrderType(siteOrderViewModel.OrderType, stringList);
                siteOrderModel.OrderItems = LoadOrderItems(siteOrderViewModel.OrderItems, stringList);
                siteOrderModel.TransactionType = LoadTransactionType(siteOrderViewModel.TransactionType, stringList);
                //siteOrderModel.IsConfirmed = siteOrderViewModel.IsConfirmed;
                siteOrderModel.IsConfirmed = siteOrderViewModel.OrderStatus == OnlineOrdering.Enum.OrderStatus.IsConfirmed ? true : false;
                siteOrderModel.UserEmailId = siteOrderViewModel.UserEmailId;
                siteOrderModel.MemberGuid = siteOrderViewModel.MemberGuid;
                siteOrderModel.OrderInvoiceTransaction = LoadInvoiceTransaction(siteOrderViewModel.OrderInvoiceTransaction, stringList);
                siteOrderModel.IsHappyHourApplied = siteOrderViewModel.IsHappyHourApplied;
 
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }

            return siteOrderModel;
        }
        public OrderInvoiceTransactionModel LoadInvoiceTransaction(ApiOrderInvoiceTransactionViewModel apiOrderInvoiceTransactionViewModel, List<string> stringList)
        {
            OrderInvoiceTransactionModel orderInvoiceTransactionModel = new OrderInvoiceTransactionModel();
            try
            {
                stringList.Add("Loading Invoice Transaction                        ");
                orderInvoiceTransactionModel.OrderInvoiceTransactionId = apiOrderInvoiceTransactionViewModel.OrderInvoiceTransactionId;
                orderInvoiceTransactionModel.OrderId = apiOrderInvoiceTransactionViewModel.OrderId;
                orderInvoiceTransactionModel.InvoiceTransactionId = apiOrderInvoiceTransactionViewModel.InvoiceTransactionId;
                orderInvoiceTransactionModel.InvoiceTransaction = LoadInvoiceTransactionItem(apiOrderInvoiceTransactionViewModel.InvoiceTransaction, stringList);
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }
            return orderInvoiceTransactionModel;
        }
        public InvoiceTransactionModel LoadInvoiceTransactionItem(ApiInvoiceTransactionViewModel apiInvoiceTransactionViewModel, List<string> stringList)
        {
            InvoiceTransactionModel invoiceTransactionModel = new InvoiceTransactionModel();
            try
            {
                stringList.Add("Loading Invoice Transaction Item                   ");
                invoiceTransactionModel.InvoiceTransactionId = apiInvoiceTransactionViewModel.InvoiceTransactionId;
                invoiceTransactionModel.InvoiceNumber = apiInvoiceTransactionViewModel.InvoiceNumber;
                invoiceTransactionModel.TotalSaleAmount = apiInvoiceTransactionViewModel.TotalSaleAmount;
                invoiceTransactionModel.TransactionDate = apiInvoiceTransactionViewModel.TransactionDate;
                invoiceTransactionModel.SiteId = apiInvoiceTransactionViewModel.SiteId;
                invoiceTransactionModel.TerminalName = apiInvoiceTransactionViewModel.TerminalName;
                invoiceTransactionModel.Receipt = apiInvoiceTransactionViewModel.Receipt;
                invoiceTransactionModel.ReceiptPath = apiInvoiceTransactionViewModel.ReceiptPath;
                invoiceTransactionModel.Rounding = apiInvoiceTransactionViewModel.Rounding;
                invoiceTransactionModel.UserReferenceId = apiInvoiceTransactionViewModel.UserReferenceId;
                invoiceTransactionModel.UserType = apiInvoiceTransactionViewModel.UserType == OnlineOrdering.Enum.UserType.Member ? DTO.Enum.UserType.Member : DTO.Enum.UserType.Staff;
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                return null;
            }
            return invoiceTransactionModel;
        }
        public DTO.Enum.SiteSettingType LoadTransactionType(OnlineOrdering.Enum.SiteSettingType apiSiteSettingType, List<string> stringList)
        {
            DTO.Enum.SiteSettingType siteSettingType = 0;
            try
            {
                stringList.Add("Loading Transaction Type                           ");
                if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.ApplyServiceChargeTax)
                    siteSettingType = DTO.Enum.SiteSettingType.ApplyServiceChargeTax;
                else if(apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.CalculateScPreDiscountedPrice)
                    siteSettingType = DTO.Enum.SiteSettingType.CalculateScPreDiscountedPrice;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.CalculateTaxAfterDiscount)
                    siteSettingType = DTO.Enum.SiteSettingType.CalculateTaxAfterDiscount;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.CalculateTaxBeforeDiscount)
                    siteSettingType = DTO.Enum.SiteSettingType.CalculateTaxBeforeDiscount;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.DineIn)
                    siteSettingType = DTO.Enum.SiteSettingType.DineIn;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.ItemPriceIncludeServiceCharge)
                    siteSettingType = DTO.Enum.SiteSettingType.ItemPriceIncludeServiceCharge;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.ItemPriceIncludeTax)
                    siteSettingType = DTO.Enum.SiteSettingType.ItemPriceIncludeTax;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.PickUp)
                    siteSettingType = DTO.Enum.SiteSettingType.PickUp;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.ReCalculateScAfterDiscount)
                    siteSettingType = DTO.Enum.SiteSettingType.ReCalculateScAfterDiscount;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.ServiceChargeTaxRate)
                    siteSettingType = DTO.Enum.SiteSettingType.ServiceChargeTaxRate;
                else if (apiSiteSettingType == OnlineOrdering.Enum.SiteSettingType.TakeAway)
                    siteSettingType = DTO.Enum.SiteSettingType.TakeAway;
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return siteSettingType;
        }
        public List<OrderItemModel> LoadOrderItems(List<ApiOrderItemViewModel> orderItemViewModelList, List<string> stringList)
        {
            List<OrderItemModel> orderItemModelList = new List<OrderItemModel>();
            try
            {
                foreach(var orderItem in orderItemViewModelList)
                {
                    stringList.Add("Loading Order Items List                           ");
                    OrderItemModel orderItemModel = new OrderItemModel();
                    orderItemModel.Description = orderItem.Description;
                    orderItemModel.ItemUniqueId = orderItem.ItemUniqueId;
                    orderItemModel.Name = orderItem.Name;
                    orderItemModel.OrderId = orderItem.OrderId;
                    orderItemModel.OrderItemSizes = LoadOrderItemSizes(orderItem.OrderItemSizes, stringList);
                    orderItemModel.Price = orderItem.Price;
                    orderItemModel.SiteItemId = orderItem.SiteItemId;

                    orderItemModelList.Add(orderItemModel);

                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return orderItemModelList;
        }
        public List<OrderItemSizeModel> LoadOrderItemSizes(List<ApiOrderItemSizeViewModel> apiOrderItemSizeViewModelList, List<string> stringList)
        {
            List<OrderItemSizeModel> orderItemSizeModelList = new List<OrderItemSizeModel>();
            try
            {
                foreach(var apiOrderItemSizeViewModel in apiOrderItemSizeViewModelList)
                {
                    stringList.Add("Loading Order Items Size List                      ");
                    OrderItemSizeModel orderItemSizeModel = new OrderItemSizeModel();
                    orderItemSizeModel.BasePrice = apiOrderItemSizeViewModel.BasePrice;
                    orderItemSizeModel.ItemSizeId = apiOrderItemSizeViewModel.ItemSizeId; 
                    orderItemSizeModel.ItemSizeUniqueId = apiOrderItemSizeViewModel.ItemSizeUniqueId; 
                    orderItemSizeModel.MenuPrice = apiOrderItemSizeViewModel.MenuPrice; 
                    orderItemSizeModel.Name = apiOrderItemSizeViewModel.Name; 
                    orderItemSizeModel.OrderItemId = apiOrderItemSizeViewModel.OrderItemId;
                    orderItemSizeModel.OrderItemSizeDiscounts = LoadItemSizeDiscounts(apiOrderItemSizeViewModel.OrderItemSizeDiscounts, stringList);
                    orderItemSizeModel.OrderItemSizeId = apiOrderItemSizeViewModel.OrderItemSizeId;
                    orderItemSizeModel.OrderItemSizeTaxProfiles = LoadItemSizeTaxProfiles(apiOrderItemSizeViewModel.OrderItemSizeTaxProfiles, stringList);
                    orderItemSizeModel.Price = apiOrderItemSizeViewModel.Price;
                    orderItemSizeModel.PriceInclusive = apiOrderItemSizeViewModel.PriceInclusive;
                    orderItemSizeModel.Quantity = apiOrderItemSizeViewModel.Quantity;
                    orderItemSizeModel.ReferenceOrderItemSizeId = apiOrderItemSizeViewModel.ReferenceOrderItemSizeId;

                    orderItemSizeModelList.Add(orderItemSizeModel);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return orderItemSizeModelList;
        }
        public List<OrderItemSizeDiscountModel> LoadItemSizeDiscounts(List<ApiOrderItemSizeDiscountViewModel> apiOrderItemSizeDiscountViewModelList, List<string> stringList)
        {
            List<OrderItemSizeDiscountModel> orderItemSizeDiscountModelList = new List<OrderItemSizeDiscountModel>();
            try
            {
                foreach(var apiOrderItemSizeDiscountViewModel in apiOrderItemSizeDiscountViewModelList)
                {
                    stringList.Add("Loading Item Size Discounts List                   ");
                    OrderItemSizeDiscountModel orderItemSizeDiscountModel = new OrderItemSizeDiscountModel();
                    orderItemSizeDiscountModel.Code = apiOrderItemSizeDiscountViewModel.Code;
                    orderItemSizeDiscountModel.Name = apiOrderItemSizeDiscountViewModel.Name;
                    orderItemSizeDiscountModel.OrderItemSizeDiscountId = apiOrderItemSizeDiscountViewModel.OrderItemSizeDiscountId;
                    orderItemSizeDiscountModel.OrderItemSizeId = apiOrderItemSizeDiscountViewModel.OrderItemSizeId;
                    orderItemSizeDiscountModel.Value = apiOrderItemSizeDiscountViewModel.Value;
                    
                    orderItemSizeDiscountModelList.Add(orderItemSizeDiscountModel);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return orderItemSizeDiscountModelList;
        }
        public List<OrderItemSizeTaxProfileModel> LoadItemSizeTaxProfiles(List<ApiOrderItemSizeTaxProfileViewModel> apiOrderItemSizeTaxProfileViewModelList, List<string> stringList)
        {
            List<OrderItemSizeTaxProfileModel> orderItemSizeTaxProfileModelList = new List<OrderItemSizeTaxProfileModel>();
            try
            {
                foreach (var apiOrderItemSizeTaxProfileViewModel in apiOrderItemSizeTaxProfileViewModelList)
                {
                    stringList.Add("Loading Item Size Tax Profiles List                ");
                    OrderItemSizeTaxProfileModel orderItemSizeTaxProfileModel = new OrderItemSizeTaxProfileModel();
                    orderItemSizeTaxProfileModel.CompanyId = apiOrderItemSizeTaxProfileViewModel.CompanyId;
                    orderItemSizeTaxProfileModel.Description = apiOrderItemSizeTaxProfileViewModel.Description;
                    orderItemSizeTaxProfileModel.ItemSizeTaxProfileId = apiOrderItemSizeTaxProfileViewModel.ItemSizeTaxProfileId;
                    orderItemSizeTaxProfileModel.Name = apiOrderItemSizeTaxProfileViewModel.Name;
                    orderItemSizeTaxProfileModel.OrderItemSizeId = apiOrderItemSizeTaxProfileViewModel.OrderItemSizeId;
                    orderItemSizeTaxProfileModel.OrderItemSizeTaxProfileId = apiOrderItemSizeTaxProfileViewModel.OrderItemSizeTaxProfileId;
                    orderItemSizeTaxProfileModel.Percentage = apiOrderItemSizeTaxProfileViewModel.Percentage;
                    orderItemSizeTaxProfileModel.Priority = apiOrderItemSizeTaxProfileViewModel.Priority;
                    orderItemSizeTaxProfileModel.Rate = apiOrderItemSizeTaxProfileViewModel.Rate;
                    orderItemSizeTaxProfileModel.TaxProfileType = LoadTaxProfileType(apiOrderItemSizeTaxProfileViewModel.TaxProfileType, stringList);
                    orderItemSizeTaxProfileModel.Value = apiOrderItemSizeTaxProfileViewModel.Value;

                    orderItemSizeTaxProfileModelList.Add(orderItemSizeTaxProfileModel);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return orderItemSizeTaxProfileModelList;
        }
        public DTO.Enum.TaxProfileType LoadTaxProfileType(OnlineOrdering.Enum.TaxProfileType apiTaxProfileType, List<string> stringList)
        {
            DTO.Enum.TaxProfileType taxProfileType = 0;
            try
            {
                stringList.Add("Loading Tax Profile Type                           ");
                if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.LocalTax)
                    taxProfileType = DTO.Enum.TaxProfileType.LocalTax;
                else if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.ProfitTax)
                    taxProfileType = DTO.Enum.TaxProfileType.ProfitTax;
                else if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.ProfitTax)
                    taxProfileType = DTO.Enum.TaxProfileType.Purchasetax;
                else if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.ProfitTax)
                    taxProfileType = DTO.Enum.TaxProfileType.Purchasetax;
                else if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.SalesTax)
                    taxProfileType = DTO.Enum.TaxProfileType.SalesTax;
                else if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.ServiceCharge)
                    taxProfileType = DTO.Enum.TaxProfileType.ServiceCharge;
                else if (apiTaxProfileType == OnlineOrdering.Enum.TaxProfileType.ServiceChargeTax)
                    taxProfileType = DTO.Enum.TaxProfileType.ServiceChargeTax;

            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return taxProfileType;
        }
        public DTO.Enum.OrderType LoadOrderType(OnlineOrdering.Enum.OrderType apiOrderType, List<string> stringList)
        {
            DTO.Enum.OrderType orderType = 0;
            try
            {
                stringList.Add("Loading Order Type                                 ");
                if (apiOrderType == OnlineOrdering.Enum.OrderType.CanceledOrder)
                    orderType = DTO.Enum.OrderType.CanceledOrder;
                else if (apiOrderType == OnlineOrdering.Enum.OrderType.CreditNonExistingOrder)
                    orderType = DTO.Enum.OrderType.CreditNonExistingOrder;
                else if (apiOrderType == OnlineOrdering.Enum.OrderType.NormalOrder)
                    orderType = DTO.Enum.OrderType.NormalOrder;
                else if (apiOrderType == OnlineOrdering.Enum.OrderType.Unused)
                    orderType = DTO.Enum.OrderType.Unused;
            }
            catch (Exception ex)
            {
                stringList.Add("Exception is                                       " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                //return null;
            }
            return orderType;
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
            menuConsumableViewModel.Type = (OnlineOrdering.Enum.MenuType)menuConsumableInfo.Type;
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
            itemSizeTaxProfileViewModel.Type = (OnlineOrdering.Enum.TaxProfileType)taxProfile.Type;

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
            taxSetting.SettingType = (OnlineOrdering.Enum.SiteSettingType)taxSettingInfo.SettingType;
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
            siteOrderViewModel.ContainerType = (OnlineOrdering.Enum.OrderContainerType)siteOrderModel.ContainerType;
            //siteOrderViewModel.IsConfirmed = siteOrderModel.IsConfirmed;
            siteOrderViewModel.OrderStatus = siteOrderModel.IsConfirmed ? OnlineOrdering.Enum.OrderStatus.IsConfirmed : OnlineOrdering.Enum.OrderStatus.PosOrderError;
            siteOrderViewModel.Location = siteOrderModel.Location;
            siteOrderViewModel.OrderGuid = siteOrderModel.OrderGuid;
            siteOrderViewModel.OrderId = siteOrderModel.OrderId;
            siteOrderViewModel.OrderType = (OnlineOrdering.Enum.OrderType)siteOrderModel.OrderType;
            siteOrderViewModel.SiteId = siteOrderModel.SiteId;
            siteOrderViewModel.TerminalName = siteOrderModel.TerminalName;
            siteOrderViewModel.TotalAmount = siteOrderModel.TotalAmount;
            siteOrderViewModel.TransactionDate = siteOrderModel.TransactionDate;
            siteOrderViewModel.TransactionType = (OnlineOrdering.Enum.SiteSettingType)siteOrderModel.TransactionType;
            siteOrderViewModel.UserReferenceId = siteOrderModel.UserReferenceId;
            siteOrderViewModel.UserType = (OnlineOrdering.Enum.UserType)siteOrderModel.UserType;
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
            orderItemSizeTaxViewModel.TaxProfileType = (OnlineOrdering.Enum.TaxProfileType)itemSizeTaxProfile.TaxProfileType;
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
            invoiceTransactionViewModel.UserType = (OnlineOrdering.Enum.UserType)invoiceTransactionModel.UserType;
            return invoiceTransactionViewModel;
        }
        #endregion



    }
}
