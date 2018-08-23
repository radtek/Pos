using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.LoyaltyMate;
using Loyaltymate.Model.OnlineOrderingModel;
using Loyaltymate.Sevices;
using MenumateServices.DTO.LoyaltyMate;
using System.Net;

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


        public LoyaltyResponse SyncSiteMenu(SiteMenuInfo siteMenus)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.SyncSiteMenu(CreateSiteMenuViewModel(siteMenus));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to update menu to server",
                        "",
                        LoyaltyResponseCode.UpdateMemberFailed);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        #endregion

        #region private

        private ApiSiteMenuViewModel CreateSiteMenuViewModel(SiteMenuInfo siteMenuInfo)
        {
            var siteMenuViewModel = new ApiSiteMenuViewModel();
            siteMenuViewModel.CompanyId = siteMenuInfo.CompanyId;
            siteMenuViewModel.SiteId = siteMenuInfo.SiteId;
            foreach (var menu in siteMenuInfo.MenuConsumables)
            {
                siteMenuViewModel.MenuConsumables.Add(CreateMenuConsumableViewModel(menu));
            }
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
            //menuConsumableViewModel.Type = menuConsumableInfo.Type;
            foreach (var siteCourse in menuConsumableInfo.SiteCourses)
            {
                menuConsumableViewModel.SiteCourses.Add(CreateCourseViewModel(siteCourse));
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
            foreach(var item in courseInfo.Items)
            {
                courseViewModel.Items.Add(CreateItemSiteViewModel(item));
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

            foreach (var itemSize in siteItem.ItemSizes)
            {
                siteItemViewModel.ItemSizes.Add(CreateItemSizeViewModel(itemSize));
            }

            foreach (var sideGroup in siteItem.SideGroup)
            {
                siteItemViewModel.SideGroup.Add(CreateSideGroupViewModel(sideGroup));
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
            foreach(var itemSide in sideGroupInfo.ItemSides)
            {
                sideGroupViewModel.ItemSides.Add(CreateItemSideViewModel(itemSide));
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
            foreach(var itemSize in itemSideInfo.ItemSizes)
            {
                itemSideViewModel.ItemSizes.Add(CreateItemSizeViewModel(itemSize));
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

            foreach (var taxProfile in itemSize.ItemSizeTaxProfiles)
            {
                var itemSizeTaxProfileInfo = new ApiItemSizeTaxProfileViewModel();
                itemSizeTaxProfileInfo = CreateItemSizeTaxProfileViewModel(taxProfile);
                itemSizeViewModel.ItemSizeTaxProfiles.Add(itemSizeTaxProfileInfo);
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
            //itemSizeTaxProfileInfo.Type = taxProfile.Type;

            return itemSizeTaxProfileViewModel;
        }

        #endregion



    }
}
