using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
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
        public delegate void NavigationDisplayDelegate();
        public event OrderArrivedDelegate OrderArrivedEvent;
        public delegate void StatusChangeDelegate(string status);
        public event StatusChangeDelegate StatusChangeEvent;
        public event NavigationDisplayDelegate NavigationUpdateEvent;
        private static ChefmateController _instance;
        public static volatile object _syncRoot = new object();
        private ChefmateController()
        {
            PageIndex = 0;
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
        public int PageIndex { get; set; }
        public int PageRows { get; set; }
        public int PageColumns { get; set; }
        #endregion

        public void ManageOrderDisplay()
        {
            try
            {
                TotalOrders.RemoveAll(s => s.Items.Count == 0);
                CurrentDisplayOrders.Clear();
                if (TotalOrders.Count > 0)
                {
                    var orderInPage = PageRows * PageColumns;
                    int remainingOrders = 0;
                    if (TotalOrders.Count > PageIndex * orderInPage)
                    {
                        remainingOrders = TotalOrders.Count - PageIndex * orderInPage;
                    }
                    else
                    {
                        PageIndex = PageIndex > 0 ? PageIndex-- : PageIndex;
                        remainingOrders = TotalOrders.Count - PageIndex * orderInPage;
                    }

                    remainingOrders = remainingOrders >= orderInPage ? orderInPage : remainingOrders;
                    var orders = TotalOrders.ToList().GetRange(PageIndex * orderInPage, remainingOrders);
                    foreach (var order in orders)
                    {
                        order.UpdateOrderInfoDisplay(CurrentSettings.OrderInfoDisplay);
                        ManageOrderGroups(order);
                        CurrentDisplayOrders.Add(order);
                    }
                    if (NavigationUpdateEvent != null)
                        NavigationUpdateEvent();
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ManageOrderDisplay", ex.Message);
            }
        }
        public void ManageOrderGroups(Order order)
        {
            if (order != null)
            {
                order.FilterOrders();
                order.DisplayGroups = CurrentSettings.GroupType == GroupType.ServingCourse
                    ? order.ServingCourseGroups
                    : order.CourseGroups;
                order.DisplayGroups.RemoveAll(s => s.Items.Count == 0);
            }
        }
        public void OrderArrived(Order order)
        {
            if (OrderArrivedEvent != null)
                OrderArrivedEvent(order);
        }
        public void AddOrder(Order order)
        {
            string statusString = "";
            try
            {
                lock (_syncRoot)
                {
                    switch (order.OrderState)
                    {
                        case OrderState.Complete:
                            AddCompleteOrder(order);
                            statusString = "New Order";
                            break;
                        case OrderState.Transfer:
                            AddTransferOrder(order);
                            statusString = string.Format("Transfer Order | Source Table Name: {0}  ", order.SourceTableName);
                            break;
                        case OrderState.Cancel:
                            AddCancelOrder(order);
                            statusString = "Cancelled Order ";
                            break;
                        case OrderState.Credit:
                            AddCreditOrder(order);
                            statusString = "Refund Order";
                            break;
                        case OrderState.CallAway:
                            CallAwayOrder(order);
                            statusString = "Callaway Order";
                            break;
                        case OrderState.Runner:
                            AddRunnerOrder(order);
                            statusString = "Bumped Order";
                            break;

                    }

                    statusString = string.Format("Last Order:- {0} | Table/Tab Name: {1} | Server Name: {2}", statusString, order.TableTabName,
                        order.ServerName);
                    if (StatusChangeEvent != null)
                        StatusChangeEvent(statusString);
                    ManageOrderDisplay();
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ChefmateController.AddOrder", ex.StackTrace);
            }


        }
        public void LoadAllOrders()
        {
            try
            {
                var orders = DatabaseOrderBuilder.Instance.GetAllOrders(CurrenTerminal.TerminalId);
                foreach (var order in orders)
                {
                    TotalOrders.Add(order);
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ChefmateController.LoadAllOrders", ex.Message);
                throw ex;
            }

        }

        #region Cancelled/Credit Order
        private void AddCancelOrder(Order order)
        {
            List<Item> processedItem = new List<Item>();
            List<Order> sourceOrders = TotalOrders.Where(s => s.TableTabName == order.TableTabName).ToList();
            foreach (var sourceOrder in sourceOrders)
            {
                foreach (var newItem in order.Items)
                {
                    var existingItem = sourceOrder.Items.FirstOrDefault(s => s.OrderItemPosKey == newItem.OrderItemPosKey && !processedItem.Contains(newItem));
                    if (existingItem != null)
                    {
                        processedItem.Add(newItem);
                        if (existingItem.DisplayAttributes.IsTransferredItem == 1)
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
        }
        #endregion

        #region Transfer
        private void AddTransferOrder(Order order)
        {
            ManageOrderGroups(order);
            List<Order> sourceOrders = TotalOrders.Where(s => s.TableTabName == order.SourceTableName).ToList();
            int itemCount =0;
            int itemCountWithZeroKey = 0;
            int CountAllItems = 0;
            sourceOrders.ForEach(s => CountAllItems += s.Items.Count);
            sourceOrders.ForEach(s =>
                {
                    itemCount = itemCount + s.Items.Count(z => z.ItemKey > 0);
                    itemCountWithZeroKey = itemCountWithZeroKey + s.Items.Count(z => z.ItemKey == 0);
                   
                });

            if (itemCount == order.Items.Count || itemCountWithZeroKey == order.Items.Count || CountAllItems == order.Items.Count)
            {
                order.DisplayAttributes.SourceTabTableText = order.SourceTableName;
            }
            var destinationOrders = TotalOrders.FirstOrDefault(s => s.TableTabName == order.TableTabName);
            if (destinationOrders != null)
            {
                ProcessTransferWhenDestinationExist(sourceOrders, order, destinationOrders);
                destinationOrders.DisplayAttributes.SourceTabTableText = order.DisplayAttributes.SourceTabTableText;
            }
            else
            {
                order.OrderState = OrderState.Complete;
                DbOrder.AddOrder(order, CurrenTerminal.TerminalId, false);
                ProcessTransferWhenDestinationNotExist(sourceOrders, order);
                TotalOrders.Add(order);
                AnalyticalData.TotalOrdersCount++;
                AnalyticalData.CurrentOrdersCount++;
            }
            if (itemCount == order.Items.Count || itemCountWithZeroKey == order.Items.Count || CountAllItems == order.Items.Count)
            {                
                sourceOrders.ForEach(s =>
                {
                    s.Items.RemoveAll(z => z.ItemKey == 0);
                });     
            }
            if (CurrentSettings.ConfirmOnTransfer)
            {
               ShowMessageBox("Order Transferred", order.SourceTableName + " is transferred to " + order.TableTabName);
            }
            ManageOrderDisplay();
        }
        private void ProcessTransferWhenDestinationExist(List<Order> existingOrders, Order newOrder, Order destinationOrder)
        {
            List<Item> processedItem = new List<Item>();
            foreach (var order in existingOrders)
            {
                foreach (var newItem in newOrder.Items)
                {
                    var existingItem = order.Items.FirstOrDefault(
                          s =>
                              s.Name == newItem.Name && s.Sides.Count == newItem.Sides.Count &&
                              s.Options.Count == newItem.Options.Count && !processedItem.Contains(s) );
                    if (existingItem != null)
                    {
                        CopyBasiDetails(existingItem, newItem);                        
                        processedItem.Add(existingItem);
                        if (!string.IsNullOrWhiteSpace(newOrder.DisplayAttributes.SourceTabTableText))
                        {
                            order.Items.Remove(existingItem);
                        }
                        else
                        {
                            UpdateTransferProperty(existingItem);                            
                        }
                        existingItem.ItemKey = 0;
                        UpdateGroupsForTransferredItem(newItem, destinationOrder);
                    }
                }
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
                    var existingItem = order.Items.FirstOrDefault(
                          s =>
                              s.Name == newItem.Name && s.Sides.Count == newItem.Sides.Count &&
                              s.Options.Count == newItem.Options.Count && !processedItem.Contains(s));
                    if (existingItem != null)
                    {
                        CopyBasiDetails(existingItem, newItem);
                        processedItem.Add(existingItem);
                        if (!string.IsNullOrWhiteSpace(newOrder.DisplayAttributes.SourceTabTableText))
                        {
                            order.Items.Remove(existingItem);
                        }
                        else
                        {
                            UpdateTransferProperty(existingItem);
                        }
                        existingItem.ItemKey = 0;
                        DbOrderItem.UpdateGroupInformation(newItem);
                    }
                }
                if (order.Items.Count == 0)
                {
                    AnalyticalData.TotalOrdersCount--;
                    AnalyticalData.CurrentOrdersCount--;
                }
                order.FilterOrders();
            }
            newOrder.Items.RemoveAll(s => s.OrderItemKey == 0);
        }
        private void CopyBasiDetails(Item source, Item destination)
        {
            destination.OrderItemKey = source.OrderItemKey;
            destination.OrderStatus = source.OrderStatus;
            destination.ArrivalTime = source.ArrivalTime;
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
            newItem.DisplayAttributes.IsTransferredItem = 1;
            foreach (var side in newItem.Sides)
            {
                side.DisplayAttributes.IsTransferredItem = 1;
            }
            foreach (var option in newItem.Options)
            {
                option.DisplayAttributes.IsTransferredItem = 1;
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
                OutputController.Instance.OutputOrder(order);
            }
            else
            {
                if (order.OrderState == OrderState.Complete || order.OrderState == OrderState.Runner)
                {
                    AnalyticalData.TotalOrdersCount++;
                    AnalyticalData.CurrentOrdersCount++;
                    AnalyticalData.CurrentItems += order.Items.Count;
                }
                TotalOrders.Add(order);
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
                        DbOrderGroup.CallAwayGroup(existingGroup.OrderGroupKey);
                    }
                }
            }
        }
        #endregion

        #region Runner Order
        private void AddRunnerOrder(Order inOrder)
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
            // ManageOrderDisplay();
        }
        #endregion

        #region Recall Order
        public void AddRecallOrder(Order order)
        {
            lock (_syncRoot)
            {
                AnalyticalData.CurrentOrdersCount++;
                AnalyticalData.CurrentItems += order.Items.Count;
                TimeSpan recalledOrderMakeTime = order.BumpTime - order.ArrivalTime;
                AnalyticalData.TotalMakeTime = new TimeSpan(AnalyticalData.TotalMakeTime.Ticks - recalledOrderMakeTime.Ticks);
                foreach (var item in order.Items)
                {
                    DbOrderItem.UpdateOrderItemTerminalKey(item.OrderItemKey, CurrenTerminal.TerminalId);
                }
                TotalOrders.Add(order);
                ManageOrderDisplay();
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
            CurrenTerminal = new Core.Model.Terminal()
            {
                DisplayName = terminalDisplayName,
                TerminalName = Environment.MachineName,
                TerminalIpAddress = terminalAddress,
                TerminalType = TerminalType.Kitchen
            };

            if (DbTerminal.AddTerminal(CurrenTerminal))
            {
                CurrentSettings = new Settings
                {
                    TerminalType = TerminalType.Kitchen,
                    DbIpAddress = dbAddress,
                    DbPath = dbPath,
                    TerminalIpAddress = terminalAddress,
                    DisplayName = terminalDisplayName
                };
                DbSettings.AddSettings(CurrentSettings, CurrenTerminal.TerminalId);
                GetPageInformation();
            }
            else
            {
                LoadSettings();
                CurrentSettings.TerminalType = TerminalType.Kitchen;
                CurrentSettings.DbIpAddress = dbAddress;
                CurrentSettings.DbPath = dbPath;
                CurrentSettings.TerminalIpAddress = terminalAddress;
                CurrentSettings.DisplayName = terminalDisplayName;
                DbSettings.SaveSettings(CurrentSettings, CurrenTerminal.TerminalId);
            };
        }

        public void LoadSettings()
        {
            CurrentSettings = DbSettings.GetSttings(CurrenTerminal.TerminalId);
            CurrentSettings.DisplayName = CurrenTerminal.DisplayName;
            GetPageInformation();
        }
        public void SaveSettings()
        {
            DbSettings.SaveSettings(CurrentSettings, CurrenTerminal.TerminalId);
            CurrenTerminal.TerminalIpAddress = CurrentSettings.TerminalIpAddress;
            CurrenTerminal.DisplayName = string.IsNullOrWhiteSpace(CurrentSettings.DisplayName) ? Environment.MachineName : CurrentSettings.DisplayName;
            CurrenTerminal.TerminalType = CurrentSettings.TerminalType;
            DbTerminal.UpdateTerminal(CurrenTerminal);
        }
        private void GetPageInformation()
        {
            PageRows = 2;
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
        public void RemoveOrder(Order order)
        {
            TotalOrders.Remove(order);
            ManageOrderDisplay();
        }
    }
}