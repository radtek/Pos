﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyaltymate.Enum;

namespace MenumateServices.DTO.OnlineOrdering.DBOrders
{
    public class OrderAttributes
    {
        public long OrderId { get; set; }
        public long OnlinerderId { get; set; }
        public long TabKey { get; set; }
        public string Location { get; set; }
        public OrderContainerType ContainerType { get; set; }
        public string ContainerName { get; set; }
        public string OrderGuid { get; set; }
        public long MembershipProfileId { get; set; }
        public UserType UserType { get; set; }
        public string TerminalName { get; set; }
        public DateTime TransactionDate { get; set; }
        public SiteSettingType TransactionType { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public long SiteItemId { get; set; }
        public double Price { get; set; }
        public OrderType OrderType { get; set; }
        public long ItemSizeId { get; set; }
        public double Quantity { get; set; }
        public double MenuPrice { get; set; }
        public double PriceInclusive { get; set; }
        public double BasePrice { get; set; }
        public string SizeName { get; set; }
        public string SizeKitchenName { get; set; }
        public int ContainerNumber { get; set; }
        public string TableName { get; set; }
        public long TramsNo { get; set; }
        public long PLU { get; set; }
        public long MenuItemKey { get; set; }
        public long ThirdPartyCodeKey { get; set; }
        public double GSTPercent { get; set; }
        public double CostGSTPercent { get; set; }
        public long SetMenuMask { get; set; }
        public double Cost { get; set; }
        public int PatronCount { get; set; }
        public long SecurityRef { get; set; }
        public long SideOrderKey { get; set; }
        public long CategoryKey { get; set; }
        public string ItemCategory { get; set; }
        public string ItemKitchenName { get; set; }
        public string CourseName { get; set; }
        public string CourseKitchenName { get; set; }
        public string MenuName { get; set; }
        public long SetvingCourseKey { get; set; }
        public int TimeKey { get; set; }
        public double PointsPercent { get; set; }
        public int ItemSizeKey { get; set; }
        public List<int> TaxProfileKeys { get; set; }
        public long SiteId { get; set; }
        public long OrderItemId { get; set; }
        public long OrderItemSizeId { get; set; }
        public long ReferenceOrderItemSizeId { get; set; }
        public string Email { get; set; }
        public string ItemUniqueId { get; set; }
        public string ItemSizeUniqueId { get; set; }
        public long ItemId { get; set; }
        public string MasterContainer { get; set; }
    }
}