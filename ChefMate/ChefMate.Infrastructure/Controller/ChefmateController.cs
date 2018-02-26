using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using Chefmate.Core;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Builder;
using Chefmate.Infrastructure.MessageBox;
using Chefmate.Logger;
using ChefMate.Database;

namespace Chefmate.Infrastructure.Controller
{
    public class ChefmateController
    {
        public delegate void OrderArrivedDelegate(Order order);
        public event OrderArrivedDelegate OrderArrivedEvent;
        public event OrderArrivedDelegate AddOrderEvent;
        public delegate void OrderRedrawDelegate();
        public event OrderRedrawDelegate OrderRedrawEvent;
        private static ChefmateController _instance;
        public static volatile object _syncRoot = new object();
        private ChefmateController()
        {
            WebOrders = new ObservableCollection<Order>();
        }
        public static ChefmateController Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new ChefmateController();
                    }
                }
                return _instance;
            }
        }

        #region Properties
        public Settings CurrentSettings { get; set; }
        public Core.Model.Terminal CurrenTerminal { get; set; }
        public AnalyticalData AnalyticalData { get; set; }
        public ObservableCollection<Order> TotalOrders { get; set; }
        public ObservableCollection<Order> CurrentDisplayOrders { get; set; }
        public ObservableCollection<Order> WebOrders { get; set; }
        public int PageColumns { get; set; }

        #endregion

        public void OrderArrived(Order order)
        {
            if (OrderArrivedEvent != null)
                OrderArrivedEvent(order);
        }
        private void PublishAddOrder(Order inOrder)
        {
            if (AddOrderEvent != null)
                AddOrderEvent(inOrder);
        }
        public void AddOrder(Order order)
        {
            try
            {
                lock (_syncRoot)
                {
                    switch (order.OrderState)
                    {
                        case OrderState.Complete:
                            AddCompleteOrder(order);
                            break;
                        case OrderState.Transfer:
                            AddTransferOrder(order);
                            break;
                        case OrderState.Cancel:
                            AddCancelOrder(order);
                            break;
                        case OrderState.Credit:
                            AddCreditOrder(order);
                            break;
                        case OrderState.CallAway:
                            CallAwayOrder(order);
                            break;
                        case OrderState.Runner:
                            AddRunnerOrder(order);
                            break;

                    }
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ChefmateController.AddOrder", ex.StackTrace);
                throw ex;
            }
        }
        public void LoadAllOrders()
        {
            try
            {
                var orders = DatabaseOrderBuilder.Instance.GetAllOrders(CurrenTerminal.TerminalId);
                foreach (var order in orders)
                {
                    if (order.Items.Count > 0)
                    {
                        if (order.OrderAction == ChefmateConstants.WebOrderAction &&
                                        order.DeliveryTime.Subtract(DateTime.Now).TotalMinutes > CurrentSettings.WebOrderTime)
                        {
                            WebOrders.Add(order);
                        }
                        else
                        {
                            TotalOrders.Add(order);
                        }
                    }

                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ChefmateController.LoadAllOrders", ex.Message);
                throw ex;
            }

        }
        public void RemoveOrder(Order order)
        {
            TotalOrders.Remove(order);
        }
        private void PublishRedrawEvent()
        {
            TotalOrders.RemoveAll(s => s.Items.Count == 0);
            if (OrderRedrawEvent != null)
                OrderRedrawEvent();
        }

        #region Cancelled/Credit Order
        private void AddCancelOrder(Order order)
        {
            List<Item> processedItem = new List<Item>();
            List<Item> processedExistingItem = new List<Item>();
            List<Order> sourceOrders = TotalOrders.Where(s => s.TableTabName == order.TableTabName).ToList();
            foreach (var sourceOrder in sourceOrders)
            {
                foreach (var newItem in order.Items)
                {
                    var existingItem = sourceOrder.Items.FirstOrDefault(s => s.OrderItemPosKey == newItem.OrderItemPosKey && !processedItem.Contains(s)
                    && !processedExistingItem.Contains(s));
                    if (existingItem != null)
                    {
                        processedExistingItem.Add(existingItem);
                        processedItem.Add(newItem);
                        if (!existingItem.DisplayAttributes.IsOriginalItem)
                        {
                            UpdateTransferProperty(existingItem);
                        }
                        existingItem.OrderStatus = OrderStatus.Canceled;
                        DbOrderItem.ItemBumped(existingItem.OrderItemKey, DateTime.Now);
                        DbOrderItem.UpdateOrderItemStatus(existingItem.OrderItemKey, OrderStatus.Canceled);
                    }
                }
            }
            order.Items.RemoveAll(s => processedItem.Contains(s));
            if (order.Items.Count > 0)
                AddCreditOrder(order);
        }
        private void AddCreditOrder(Order order)
        {
            TotalOrders.Add(order);
            if (CurrentSettings.ConfirmOnRefund)
                ShowMessageBox("Refund Order", "A Refund Order has come in.");
            PublishAddOrder(order);
        }
        #endregion

        #region Transfer
        private void AddTransferOrder(Order order)
        {
            order.FilterOrders(CurrentSettings.GroupType, CurrentSettings.OrderInfoDisplay);
            List<Order> sourceOrders = TotalOrders.Where(s => s.TableTabName == order.SourceTableName).ToList();

            order.DisplayAttributes.SourceTabTableText = order.SourceTableName;
            var destinationOrders = TotalOrders.FirstOrDefault(s => s.TableTabName == order.TableTabName);
            if (destinationOrders != null)
            {
                ProcessTransferWhenDestinationExist(sourceOrders, order, destinationOrders);
                destinationOrders.DisplayAttributes.SourceTabTableText = order.DisplayAttributes.SourceTabTableText;
            }
            else
            {
                ObservableCollection<Order> ModifiedOrders = new ObservableCollection<Order>();
                order.OrderState = OrderState.Complete;
                DbOrder.AddOrder(order, CurrenTerminal.TerminalId, false);
                ProcessTransferWhenDestinationNotExist(sourceOrders, order);
                bool isTransferredOrderReplaced = false;

                //When one order is transferred to other table then new table should come at the old source table 's place.
                //For that Add newly created order in place of the source table which has been trasferred to destination.
                foreach (var currentOrder in TotalOrders)
                {
                    ModifiedOrders.Add(currentOrder);
                    if (string.Equals(currentOrder.TableTabName, order.SourceTableName) && !isTransferredOrderReplaced)
                    {
                        ModifiedOrders.Add(order);
                        isTransferredOrderReplaced = true;
                    }

                }
                TotalOrders.Clear();
                foreach (var currentOrder in ModifiedOrders)
                {
                    TotalOrders.Add(currentOrder);
                }
                ModifiedOrders.Clear();

                AnalyticalData.TotalOrdersCount++;
                AnalyticalData.CurrentOrdersCount++;
            }

            if (CurrentSettings.ConfirmOnTransfer)
            {
                ShowMessageBox("Order Transferred", order.SourceTableName + " is transferred to " + order.TableTabName);
            }

            PublishRedrawEvent();
        }
        private void ProcessTransferWhenDestinationExist(List<Order> existingOrders, Order newOrder, Order destinationOrder)
        {
            List<Item> processedItem = new List<Item>();
            foreach (var order in existingOrders)
            {
                foreach (var newItem in newOrder.Items)
                {
                    var existingItem = order.Items.FirstOrDefault(s => s.DisplayAttributes.IsOriginalItem && s.Name == newItem.Name && s.Sides.Count == newItem.Sides.Count &&
                              s.Options.Count == newItem.Options.Count && !processedItem.Contains(s));
                    if (existingItem != null)
                    {
                        CopyBasiDetails(existingItem, newItem);
                        processedItem.Add(existingItem);
                        UpdateTransferProperty(existingItem);
                        UpdateGroupsForTransferredItem(newItem, destinationOrder);
                    }
                }
                order.UpdateOrder();
                if (order.Items.Count == 0)
                {
                    AnalyticalData.TotalOrdersCount--;
                    AnalyticalData.CurrentOrdersCount--;
                }
            }
            newOrder.Items.RemoveAll(s => s.OrderItemKey == 0);
        }
        private void ProcessTransferWhenDestinationNotExist(List<Order> existingOrders, Order newOrder)
        {
            List<Item> processedItem = new List<Item>();
            foreach (var order in existingOrders)
            {
                foreach (var newItem in newOrder.Items)
                {
                    var existingItem = order.Items.FirstOrDefault(s => s.DisplayAttributes.IsOriginalItem && s.Name == newItem.Name && s.Sides.Count == newItem.Sides.Count &&
                              s.Options.Count == newItem.Options.Count && !processedItem.Contains(s));
                    if (existingItem != null)
                    {
                        CopyBasiDetails(existingItem, newItem);
                        processedItem.Add(existingItem);
                        UpdateTransferProperty(existingItem);
                        DbOrderItem.UpdateGroupInformation(newItem);
                    }
                }
                order.UpdateOrder();
                if (order.Items.Count == 0)
                {
                    AnalyticalData.TotalOrdersCount--;
                    AnalyticalData.CurrentOrdersCount--;
                }
                //order.FilterOrders(CurrentSettings.GroupType, CurrentSettings.OrderInfoDisplay);
            }
            newOrder.Items.RemoveAll(s => s.OrderItemKey == 0);
        }
        private void CopyBasiDetails(Item source, Item destination)
        {
            destination.OrderItemKey = source.OrderItemKey;
            destination.OrderStatus = source.OrderStatus;
            destination.ArrivalTime = source.ArrivalTime;
            //destination.SCourseGroup.CalledAway = source.SCourseGroup.CalledAway;
            if (!string.IsNullOrWhiteSpace(source.Note))
            {
                destination.Note = source.Note;
                DbOrderItem.UpdateOrderItemNote(destination.OrderItemKey, destination.Note);
            }
        }
        private void UpdateGroupsForTransferredItem(Item newItem, Order destinationOrder)
        {
            var group = destinationOrder.ServingCourseGroups.FirstOrDefault(s => s.GroupName == newItem.SCourseGroup.GroupName);
            if (group != null)
            {
                newItem.SCourseGroup = group;
            }
            else
            {
                newItem.SCourseGroup.Order = destinationOrder;
                newItem.SCourseGroup.OrderKey = destinationOrder.OrderKey;
                destinationOrder.ServingCourseGroups.Add(newItem.SCourseGroup);
            }
            group = destinationOrder.CourseGroups.FirstOrDefault(s => s.GroupName == newItem.CourseGroup.GroupName);
            if (group != null)
            {
                newItem.CourseGroup = group;
            }
            else
            {
                newItem.CourseGroup.Order = destinationOrder;
                newItem.CourseGroup.OrderKey = destinationOrder.OrderKey;
                destinationOrder.CourseGroups.Add(newItem.CourseGroup);
            }
            DbOrderItem.UpdateGroupInformation(newItem);
            destinationOrder.Items.Add(newItem);
        }
        private void UpdateTransferProperty(Item newItem)
        {
            newItem.DisplayAttributes.IsOriginalItem = false;
            newItem.OrderItemKey = 0;
            foreach (var side in newItem.Sides)
            {
                side.DisplayAttributes.IsOriginalItem = false;
            }
            foreach (var option in newItem.Options)
            {
                option.DisplayAttributes.IsOriginalItem = false;
            }
        }
        #endregion

        #region New Order
        private void AddCompleteOrder(Order order)
        {
            if (order.OrderKey == 0)
                DbOrder.AddOrder(order, CurrenTerminal.TerminalId);
            if (OutputTime.Immediately == CurrentSettings.OutputTime)
            {
                DoImmediateBump(order);
            }
            else
            {
                if (order.OrderAction == ChefmateConstants.OrderAction)
                {
                    if (order.OrderState == OrderState.Complete || order.OrderState == OrderState.Runner)
                    {
                        AnalyticalData.TotalOrdersCount++;
                        AnalyticalData.CurrentOrdersCount++;
                        AnalyticalData.CurrentItems += order.Items.Count;
                    }
                    TotalOrders.Add(order);
                }
                else
                {
                    WebOrders.Add(order);
                }
            }
        }

        #endregion

        #region Call Away
        private void CallAwayOrder(Order inOrder)
        {
            var orders = TotalOrders.Where(s => s.TableTabName == inOrder.TableTabName);
            foreach (var order in orders)
            {
                foreach (var group in inOrder.ServingCourseGroups)
                {
                    var existingGroup = order.ServingCourseGroups.FirstOrDefault(s => s.GroupPosKey == group.GroupPosKey);
                    if (existingGroup != null)
                    {
                        existingGroup.CalledAway = true;
                        DbOrderGroup.CallAwayGroup(existingGroup.OrderGroupKey, true);
                    }
                }
            }
        }
        #endregion

        #region Runner Order
        private void AddRunnerOrder(Order inOrder)
        {
            if (OutputTime.Immediately == CurrentSettings.OutputTime)
            {
                DoImmediateBump(inOrder);
            }
            else
            {
                var order = TotalOrders.FirstOrDefault(s => s.OrderKey == inOrder.OrderKey);
                if (order != null)
                {
                    foreach (var item in inOrder.Items)
                    {
                        if (order.Items.FirstOrDefault(s => s.OrderItemKey == item.OrderItemKey) == null)
                        {
                            var servigCourseGroup =
                                order.ServingCourseGroups.FirstOrDefault(
                                    s => s.OrderGroupKey == item.SCourseGroup.OrderGroupKey);
                            if (servigCourseGroup != null)
                            {
                                item.SCourseGroup = servigCourseGroup;
                            }
                            else
                            {
                                item.SCourseGroup.Order = order;
                                order.ServingCourseGroups.Add(item.SCourseGroup);
                            }

                            var courseGroup =
                                order.CourseGroups.FirstOrDefault(
                                    s => s.OrderGroupKey == item.CourseGroup.OrderGroupKey);
                            if (courseGroup != null)
                            {
                                item.CourseGroup = courseGroup;
                            }
                            else
                            {
                                item.CourseGroup.Order = order;
                                order.CourseGroups.Add(item.CourseGroup);
                            }

                            order.Items.Add(item);
                            AnalyticalData.CurrentItems++;
                        }
                    }

                }
                else
                {
                    AnalyticalData.TotalOrdersCount++;
                    AnalyticalData.CurrentOrdersCount++;
                    inOrder.ServingCourseGroups.ToList().ForEach(s => AnalyticalData.CurrentItems += s.Items.Count);
                    TotalOrders.Add(inOrder);
                }
                PublishRedrawEvent();
            }
        }
        #endregion

        #region Recall Order
        public void AddRecallOrder(Order inOrder)
        {
            lock (_syncRoot)
            {
                var order = DbOrder.GetOrder(inOrder.OrderKey, 0, true);
                AnalyticalData.CurrentOrdersCount++;
                AnalyticalData.CurrentItems += order.Items.Count;
                TimeSpan recalledOrderMakeTime = order.BumpTime - order.ArrivalTime;
                AnalyticalData.TotalMakeTime = new TimeSpan(AnalyticalData.TotalMakeTime.Ticks - recalledOrderMakeTime.Ticks);
                foreach (var item in order.Items)
                {
                    DbOrderItem.UpdateOrderItemTerminalKey(item.OrderItemKey, CurrenTerminal.TerminalId);
                }
                TotalOrders.Add(order);
                PublishAddOrder(order);
            }
        }
        #endregion

        #region Terminal
        public bool LoadTerminal(string terminalName)
        {
            CurrenTerminal = DbTerminal.GetTerminal(Environment.MachineName);
            if (CurrenTerminal != null)
            {
                LoadSettings();
                return true;
            }
            return false;
        }
        public void AddTerminal(string terminalDisplayName, string terminalAddress, string dbAddress, string dbPath)
        {
            try
            {
                CurrenTerminal = new Core.Model.Terminal()
                {
                    DisplayName = terminalDisplayName,
                    TerminalName = Environment.MachineName,
                    TerminalIpAddress = terminalAddress,
                    TerminalType = TerminalType.Kitchen
                };
                DbTerminal.AddTerminal(CurrenTerminal);
                CurrentSettings = new Settings(TerminalType.Kitchen, dbAddress, dbPath, terminalAddress,
                    terminalDisplayName);
                DbSettings.AddSettings(CurrentSettings, CurrenTerminal.TerminalId);
                LoadSettings();
                CurrentSettings.TerminalType = TerminalType.Kitchen;
                CurrentSettings.DbIpAddress = dbAddress;
                CurrentSettings.DbPath = dbPath;
                CurrentSettings.TerminalIpAddress = terminalAddress;
                CurrentSettings.DisplayName = terminalDisplayName;
                DbSettings.SaveSettings(CurrentSettings, CurrenTerminal.TerminalId);
                GetPageInformation();
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("AddTerminal : ", ex.Message);
            }
        }

        public void LoadSettings()
        {
            CurrentSettings = DbSettings.GetSttings(CurrenTerminal.TerminalId);
            CurrentSettings.DisplayName = CurrenTerminal.DisplayName;
            GetPageInformation();
        }
        public void SaveSettings()
        {
            try
            {
                DbSettings.SaveSettings(CurrentSettings, CurrenTerminal.TerminalId);
                CurrenTerminal.TerminalIpAddress = CurrentSettings.TerminalIpAddress;
                CurrenTerminal.DisplayName = string.IsNullOrWhiteSpace(CurrentSettings.DisplayName)
                    ? Environment.MachineName
                    : CurrentSettings.DisplayName;
                CurrenTerminal.TerminalType = CurrentSettings.TerminalType;
                DbTerminal.UpdateTerminal(CurrenTerminal);
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("SaveSettings : ", ex.Message);
            }
        }
        private void GetPageInformation()
        {
            switch (CurrentSettings.OrderLayout)
            {
                case OrderLayout.TwoByFour:
                    PageColumns = 4;
                    break;
                case OrderLayout.TwoByFive:
                    PageColumns = 5;
                    break;
                case OrderLayout.TwoBySix:
                    PageColumns = 6;
                    break;
                case OrderLayout.TwoBySeven:
                    PageColumns = 7;
                    break;
                case OrderLayout.TwoByTen:
                    PageColumns = 10;
                    break;
                case OrderLayout.TwoByEleven:
                    PageColumns = 11;
                    break;
                case OrderLayout.TwoByTwelve:
                    PageColumns = 12;
                    break;
            }
        }

        #endregion

        #region MessageBox

        public bool ShowMessageBox(string caption, string message, bool isCancleAvalable = false)
        {
            var customMessageBox = new CustomMessageBox();
            customMessageBox.ShowInTaskbar = false;
            customMessageBox.Message = message;
            customMessageBox.WindowCaption = string.IsNullOrWhiteSpace(caption) ? "Chefmate" : caption;
            customMessageBox.IsCancleVisible = isCancleAvalable;
            customMessageBox.Topmost = true;
            customMessageBox.ShowDialog();
            return customMessageBox.ModalResult;
        }
        #endregion

        private void DoImmediateBump(Order inOrder)
        {
            try
            {
                inOrder.FilterOrders(CurrentSettings.GroupType, CurrentSettings.OrderInfoDisplay);
                if (CurrentSettings.OutputTerminal < 0)
                {
                    foreach (var item in inOrder.Items)
                    {
                        if (item.OrderItemKey > 0)
                            DbOrderItem.UpdateOrderItemTerminalKey(item.OrderItemKey, 0);
                    }
                }
                OutputController.Instance.OutputOrder(inOrder);
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("DoImmediateBump : ", ex.Message);
            }
        }

    }
}