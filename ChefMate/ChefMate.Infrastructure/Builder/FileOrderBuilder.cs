using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Xml;
using Chefmate.Core;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Core.PosModel;
using Chefmate.Infrastructure.Controller;
using Chefmate.Infrastructure.Utility;
using ChefMate.Database;
using XMLManager;

namespace Chefmate.Infrastructure.Builder
{
    enum XmlOrderAction { Unknown, Order, Cancel, Credit, Transfer, Replace, CallAway };
    enum ItemsGroupedBy { ServingCourse, Course }

    public class FileOrderBuilder
    {
        private static FileOrderBuilder _instance = null;
        private static readonly object _syncRoot = new Object();
        private FileOrderBuilder()
        {
        }
        public static FileOrderBuilder Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new FileOrderBuilder();
                    }
                }

                return _instance;
            }
        }
        public bool BuildOrder(string inXmluri)
        {
            var posOrder = ChefmateUtility.DeSerializeFromFilePath<PosOrder>(inXmluri);
            var chefmateOrder = ConvertToChefmateOrder(posOrder);
            if (chefmateOrder != null)
            {
                ChefmateController.Instance.OrderArrived(chefmateOrder);
                // play sound when an order comes from POS to chefmate.
                PlaySoundByOrderState(chefmateOrder.OrderState);
                return true;
            }
            return false;
        }
        private Order ConvertToChefmateOrder(PosOrder posOrder)
        {
            var order = new Order();
            order.ArrivalTime = DateTime.Now;
            order.SaleStartTime = posOrder.SaleStartTime.ToDateTime();
            order.DeliveryTime = posOrder.DeliveryTime.ToDateTime();
            order.SaleFinishTime = posOrder.SaleFinishTime.ToDateTime();
            order.BeenSentToOutput = false;
            order.TableTabName = posOrder.TableTabName;
            int colonIndex = order.TableTabName.IndexOf(':');
            if (colonIndex > 0)
            {
                order.TableTabName = order.TableTabName.Substring(0, colonIndex - 1);
            }
            order.SourceTableName = posOrder.SourceTableName;
            order.ChitValue = posOrder.ChitValue != "Chit Number" ? posOrder.ChitValue : "";
            order.ServerName = posOrder.ServerName;
            order.CustomerName = posOrder.CustomerName;
            order.PartyName = posOrder.PartyName;
            order.OrderType = posOrder.OrderType;
            order.PatronCount = posOrder.PatronCount.ToInteger();
            order.OrderNumber = posOrder.OrderNumber.ToInteger();
            order.OrderPosKey = posOrder.DbKey.ToInteger();
            order.OrderState = GetOrderState(posOrder.Action, posOrder.ActionType);
            if (order.OrderState == OrderState.Cancel)
            {
                AssignCancelGroups(order);
            }
            else
            {
                AssignServingCourses(posOrder, order);
                AssignCourses(posOrder, order);
            }
            order.CustomerPhone = posOrder.CustomerPhone;
            order.CustomerEmail = posOrder.CustomerEmail;
            order.CustomerAddress = posOrder.CustomerAddress;
            order.PaymentStatus = posOrder.PaymentStatus;
            order.OrderAction = string.IsNullOrWhiteSpace(posOrder.Action) ? ChefmateConstants.OrderAction : posOrder.Action;
            GetAllItems(posOrder, order);
            return order;
        }
        private OrderState GetOrderState(string action, string actionType)
        {
            switch (action.ToUpper())
            {
                case @"CANCEL": return OrderState.Cancel;
                case @"CREDIT": return OrderState.Credit;
                case @"TRANSFER": return OrderState.Transfer;
                case @"REPLACE": return OrderState.Replacement;
                case @"CALLAWAY": return OrderState.CallAway;
                case @"ORDER":
                    {
                        if (actionType.ToUpper() == @"COMPLETE")
                            return OrderState.Complete;
                        return OrderState.Incomplete;
                    }
            }
            return OrderState.Complete;
        }
        private void AssignCancelGroups(Order order)
        {
            var group = new Group();
            group.GroupName = "Cancel";
            group.GroupPosKey = -1;
            group.GroupType = GroupType.ServingCourse;
            group.Order = order;
            order.ServingCourseGroups.Add(group);
            var cGroup = new Group();
            cGroup.GroupName = "Cancel";
            cGroup.GroupPosKey = -2;
            cGroup.GroupType = GroupType.Course;
            group.Order = order;
            order.CourseGroups.Add(group);
        }
        private void AssignServingCourses(PosOrder posOrder, Order order)
        {
            order.ServingCourseGroups = new ObservableCollection<Group>();
            foreach (var servingCourse in posOrder.ServingCourses)
            {
                var group = CreateGroup(servingCourse, GroupType.ServingCourse);
                group.Order = order;
                order.ServingCourseGroups.Add(group);
            }
        }
        private void AssignCourses(PosOrder posOrder, Order order)
        {
            order.CourseGroups = new ObservableCollection<Group>();
            foreach (var servingCourse in posOrder.Courses)
            {
                var group = CreateGroup(servingCourse, GroupType.Course);
                group.Order = order;
                order.CourseGroups.Add(group);
            }
        }
        private Group CreateGroup(PosGroup posGroup, GroupType groupType)
        {
            var group = new Group();
            group.GroupPosKey = posGroup.Key.ToInteger();
            group.GroupName = posGroup.KitchenName;
            if (string.IsNullOrWhiteSpace(group.GroupName) || group.GroupName.Equals("No Serving Course"))
                group.GroupName = posGroup.Name;
            group.DisplayOrder = posGroup.DisplayOrder.ToInteger();
            group.GroupType = groupType;
            return group;
        }
        private void GetAllItems(PosOrder posOrder, Order order)
        {
            foreach (var posItem in posOrder.Items.Items)
            {
                if (order.OrderState == OrderState.Cancel)
                {
                    int quantity = posItem.Quantity.ToInteger();
                    if (quantity <= 0)
                        quantity = 1;
                    for (int i = 0; i < quantity; i++)
                    {
                        var item = CreateItem(posItem, order.OrderState);
                        item.SCourseGroup = order.ServingCourseGroups.FirstOrDefault();
                        item.CourseGroup = order.CourseGroups.FirstOrDefault();
                        order.Items.Add(item);
                    }
                }
                else
                {
                    var item = CreateItem(posItem, order.OrderState);
                    var servingCourse = order.ServingCourseGroups.FirstOrDefault(s => s.GroupPosKey == posItem.ServingCoursePosKey.ToInteger());
                    if (servingCourse != null)
                    {
                        item.SCourseGroup = servingCourse;
                        //servingCourse.Items.Add(item);
                    }
                    var course = order.CourseGroups.FirstOrDefault(s => s.GroupPosKey == posItem.CoursePosKey.ToInteger());
                    if (course != null)
                    {
                        item.CourseGroup = course;
                        //course.Items.Add(item);
                    }
                    order.Items.Add(item);
                }

            }
        }
        private Item CreateItem(PosItem posItem, OrderState orderState)
        {
            var item = new Item();
            item.ItemPosKey = posItem.Key.ToInteger();
            item.OrderItemPosKey = posItem.OrderItemKey.ToInteger();

            item.Name = string.IsNullOrWhiteSpace(posItem.KitchenName) ? posItem.Name : posItem.KitchenName;
            if (item.Name.EndsWith("Default"))
                item.Name = item.Name.Substring(0, item.Name.LastIndexOf("Default", StringComparison.Ordinal));
            item.Name = item.Name.Trim();


            item.Note = posItem.Note;
            if (!string.IsNullOrWhiteSpace(item.Note) && item.Note.Length > 80)
                item.Note = item.Note.Substring(0, 80);

            if (orderState == OrderState.Cancel)
            {
                bool isCancelled = posItem.Canceled.ToBoolean();
                item.OrderStatus = isCancelled ? OrderStatus.Canceled : OrderStatus.Normal;
            }

            item.Sides = GetAllSides(item, posItem.Sides, orderState);
            item.Options = GetAllOptions(item, posItem.Options);
            item.ArrivalTime = DateTime.Now;
            return item;
        }
        private ObservableCollection<Side> GetAllSides(Item item, PosSides posSides, OrderState orderState)
        {
            var sides = new ObservableCollection<Side>();
            if (posSides != null && posSides.Sides != null)
            {
                foreach (var posSide in posSides.Sides)
                {
                    var side = new Side();
                    side.Item = item;
                    side.SidePosKey = posSide.Key.ToInteger();
                    if (orderState == OrderState.Cancel)
                    {
                        side.IsCanceled = true;
                        side.Name = posSide.Name;
                    }
                    else
                    {
                        side.IsCanceled = false;
                        side.Name = posSide.KitchenName;
                    }
                    sides.Add(side);
                }
            }
            return sides;
        }
        private ObservableCollection<Option> GetAllOptions(Item item, PosOptions posOptions)
        {
            var options = new ObservableCollection<Option>();
            if (posOptions != null && posOptions.Options != null)
            {
                foreach (var posOption in posOptions.Options)
                {
                    var option = new Option();
                    option.Item = item;
                    option.OptionPosKey = posOption.Key.ToInteger();
                    option.IsPlus = posOption.IsPlus.ToBoolean();
                    option.Name = posOption.KitchenName;
                    options.Add(option);
                }
            }
            return options;
        }
        private void PlaySoundByOrderState(OrderState orderState)
        {
            switch (orderState)
            {
                case OrderState.Complete:
                    SoundManager.PlaySound(SoundEvents.NewOrderFromMm);
                    break;
                case OrderState.Cancel:
                    SoundManager.PlaySound(SoundEvents.CancelOrderFromMm);
                    break;
                case OrderState.Credit:
                    SoundManager.PlaySound(SoundEvents.RefundOrderFromMm);
                    break;
                case OrderState.Transfer:
                    SoundManager.PlaySound(SoundEvents.TransferOrderFromMm);
                    break;
            }
        }
        void LogError(string inFuncName, string inMsg)
        {
            string functURI = string.Format(@"ChefMate.Builder.OrderBuilderFromXML.{0}", inFuncName);
            //Logger.CMLogger.Instance.logError(functURI, inMsg);
        }

    }
}
