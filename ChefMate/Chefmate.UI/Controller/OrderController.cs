using System;
using System.Linq;
using System.Threading.Tasks;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Controller;
using Chefmate.Logger;
using Chefmate.UI.Views;
using ChefMate.Communication.Tcp;
using Chefmate.UI.KeyBoards;

namespace Chefmate.UI.Controller
{
    public class OrderController
    {
        private static OrderController _instance;
        private static volatile object _syncRoot = new object();

        private OrderController() { }

        public static OrderController Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new OrderController();
                    }
                }
                return _instance;
            }
        }
        public void OrderCommandHandler(string command, object dataContext)
        {

            try
            {
                switch (command)
                {
                    case "Start":
                        Instance.Start(dataContext);
                        break;
                    case "Hold":
                        Instance.Hold(dataContext);
                        break;
                    case "Bump":
                        Instance.Bump(dataContext);
                        break;
                    case "Transfer":
                        Instance.Transfer(dataContext);
                        break;
                    case "Show":
                        Instance.HideSow(dataContext, true);
                        break;
                    case "Hide":
                        Instance.HideSow(dataContext, false);
                        break;
                    case "Hide All":
                        Instance.HideSow(dataContext, false);
                        break;
                    case "Note":
                        Instance.ItemNote(dataContext);
                        break;

                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("OrderCommandHandler", ex.Message);
            }

        }

        #region Bump
        public void Bump(object sender)
        {
            try
            {
                if (sender != null)
                {
                    if (sender.GetType() == typeof(Order))
                        BumpOrder(sender as Order);
                    if (sender.GetType() == typeof(Group))
                        BumpGroup(sender as Group);
                    if (sender.GetType() == typeof(Item))
                        BumpItem(sender as Item);
                }
            }
            catch (Exception Ex)
            {
                ChefmateLogger.Instance.LogError("OrderController.Bump", Ex.Message);
            }

        }
        public void BumpOrder(Order order)
        {
            foreach (var group in order.DisplayGroups)
            {
                if (group.OrderStatus != OrderStatus.Bumped && group.OrderStatus != OrderStatus.Canceled)
                {
                    BumpGroup(group, true);
                }
            }

            if (ChefmateController.Instance.CurrentSettings.OutputTerminal <= 0)
            {
                foreach (var item in order.Items)
                {
                    if (item.OrderItemKey > 0)
                        DbOrderItem.UpdateOrderItemTerminalKey(item.OrderItemKey, 0);
                }
            }

            if (ChefmateController.Instance.CurrentSettings.OutputTime == OutputTime.OrderBumped)
            {
                OutputOrder(order);
            }
            if (order.OrderState == OrderState.Complete || order.OrderState == OrderState.Runner)
            {
                ChefmateController.Instance.AnalyticalData.CurrentItems -= order.Items.Count;
                ChefmateController.Instance.AnalyticalData.CurrentOrdersCount--;
                var currentOrderMakeTime = DateTime.Now.Subtract(order.ArrivalTime);
                ChefmateController.Instance.AnalyticalData.TotalMakeTime =
                    new TimeSpan(ChefmateController.Instance.AnalyticalData.TotalMakeTime.Ticks +
                                 currentOrderMakeTime.Ticks);
            }
            ChefmateController.Instance.RemoveOrder(order);
            if (order.OrderKey > 0)
                DbOrder.OrderBumped(order.OrderKey, DateTime.Now);

        }
        private void BumpGroup(Group group, bool isBumbedFromOrder = false)
        {
            foreach (var item in group.Items)
            {
                if (item.OrderStatus != OrderStatus.Bumped && item.OrderStatus != OrderStatus.Canceled)
                {
                    BumpItem(item, true);
                }
            }
            ChangeGroupStatus(group, OrderStatus.Bumped);
            if ((ChefmateController.Instance.CurrentSettings.OutputTime == OutputTime.GroupBumped))
            {
                OutputGroup(group);
            }
            if (!isBumbedFromOrder)
            {
                if (group.Order.OrderStatus == OrderStatus.Bumped)
                {
                    BumpOrder(group.Order);
                }
            }
        }
        private void BumpItem(Item item, bool isBumbedFromGroup = false)
        {
            if (item.DisplayAttributes.IsTransferredItem == 1)
            {
                item.DisplayAttributes.IsTransferredItem = 0;
                foreach (var side in item.Sides)
                {
                    side.DisplayAttributes.IsTransferredItem = 0;
                }
                foreach (var option in item.Options)
                {
                    option.DisplayAttributes.IsTransferredItem = 0;
                }
            }
            ChangeItemStatus(item, OrderStatus.Bumped);
            foreach (var side in item.Sides)
            {
                side.ItemStatus = OrderStatus.Bumped;
            }
            foreach (var option in item.Options)
            {
                option.ItemStatus = OrderStatus.Bumped;
            }

            if (ChefmateController.Instance.CurrentSettings.OutputTime == OutputTime.ItemBumped)
            {
                OutputItem(item);
            }

            var group = ChefmateController.Instance.CurrentSettings.GroupType == GroupType.ServingCourse
                ? item.SCourseGroup
                : item.CourseGroup;

            if (group.Order.OrderState == OrderState.Complete && ChefmateController.Instance.CurrentSettings.OutputType != OutputType.KitchenTerminal &&
                item.OrderItemKey > 0)
            {
                DbOrderItem.ItemBumped(item.OrderItemKey, DateTime.Now);
            }

            if (!isBumbedFromGroup && group.Order.OrderStatus == OrderStatus.Bumped)
            {
                BumpOrder(group.Order);
            }
        }
        private void OutputOrder(Order inOrderToOutput)
        {
            Task.Factory.StartNew(new Action(() =>
            {
                SendOrderToOutput(inOrderToOutput);
            }));

        }
        private void OutputGroup(Group inGroupToOutput)
        {
            switch (ChefmateController.Instance.CurrentSettings.OutputScope)
            {
                case OutputScope.Order:
                    SendOrderToOutput(inGroupToOutput.Order);
                    break;
                case OutputScope.Group:
                    SendGroupToOutput(inGroupToOutput);
                    break;
            }
        }
        private void OutputItem(Item inItemToOutput)
        {
            switch (ChefmateController.Instance.CurrentSettings.OutputScope)
            {
                case OutputScope.Order:
                    SendOrderToOutput(inItemToOutput.SCourseGroup.Order);
                    break;
                case OutputScope.Group:
                    SendGroupToOutput(inItemToOutput.SCourseGroup);
                    break;
                case OutputScope.Item:
                    SendItemToOutput(inItemToOutput);
                    break;
            }
        }
        private void SendOrderToOutput(Order inOrderToOutput)
        {
            if (!inOrderToOutput.BeenSentToOutput)
            {
                if (inOrderToOutput.OrderState != OrderState.Credit || inOrderToOutput.OrderState != OrderState.Cancel)
                {
                    inOrderToOutput.BeenSentToOutput = true;
                    OutputController.Instance.OutputOrder(inOrderToOutput);
                }
            }
        }
        private void SendGroupToOutput(Group inGroupToOutput)
        {
            if (!inGroupToOutput.BeenSentToOutput)
            {
                if (inGroupToOutput.Order.OrderState != OrderState.Credit || inGroupToOutput.Order.OrderState != OrderState.Cancel)
                {
                    inGroupToOutput.BeenSentToOutput = true;
                    OutputController.Instance.OutputGroup(inGroupToOutput);
                }
            }
        }
        private void SendItemToOutput(Item inItemToOutput)
        {
            if (!inItemToOutput.BeenSentToOutput)
            {
                if (inItemToOutput.SCourseGroup.Order.OrderState != OrderState.Credit || inItemToOutput.SCourseGroup.Order.OrderState != OrderState.Cancel)
                {
                    inItemToOutput.BeenSentToOutput = true;
                    OutputController.Instance.OutputItem(inItemToOutput);
                }
            }
        }
        #endregion

        #region Transfer
        public void Transfer(object sender)
        {
            try
            {
                if (sender != null)
                {
                    if (sender.GetType() == typeof(Order))
                        TransferOrder(sender as Order);
                }
            }
            catch (Exception Ex)
            {
                ChefmateLogger.Instance.LogError("OrderController.Transfer", Ex.Message);
            }
        }
        private void TransferOrder(Order order)
        {
            TransferView transferView = new TransferView();
            transferView.Order = order;
            transferView.ShowInTaskbar = false;
            transferView.ShowDialog();
            if (transferView.ModalResul)
            {
                foreach (var servingCourse in order.DisplayGroups)
                {
                    foreach (var item in servingCourse.Items)
                    {
                        DbOrderItem.UpdateItemOnTransfer(item.OrderItemKey, transferView.SelectedTerminal.TerminalId, OrderStatus.Transfered);
                    }
                }
                TcpStreamSender transferStream = new TcpStreamSender();
                TcpStreamMessage transferMessage = new TcpStreamMessage(transferView.SelectedTerminal.TerminalIpAddress, inMessageType: TcpStreamMessageType.Order);
                transferMessage.AddData(order.OrderKey);
                var transferSuccessful = transferStream.SendMessage(transferMessage);
                if (transferSuccessful)
                {
                    ChefmateController.Instance.AnalyticalData.TotalOrdersCount--;
                    ChefmateController.Instance.AnalyticalData.CurrentOrdersCount--;
                    ChefmateController.Instance.AnalyticalData.CurrentItems -= order.Items.Count;
                    ChefmateController.Instance.RemoveOrder(order);
                }
            }

        }
        #endregion

        #region Start
        public void Start(object sender)
        {
            if (sender != null)
            {
                if (sender.GetType() == typeof(Order))
                    StartOrder(sender as Order);
                if (sender.GetType() == typeof(Group))
                    StartGroup(sender as Group);
                if (sender.GetType() == typeof(Item))
                    StartItem(sender as Item);
            }
        }
        private void StartOrder(Order order)
        {
            order.OrderStatus = OrderStatus.Started;
            foreach (var group in order.ServingCourseGroups)
            {
                StartGroup(group);
            }
            foreach (var group in order.CourseGroups)
            {
                StartGroup(group);
            }
        }
        private void StartGroup(Group group)
        {
            if (group.OrderStatus != OrderStatus.Bumped)
            {
                group.OrderStatus = OrderStatus.Started;
                group.Order.OrderStatus = OrderStatus.Started;
                foreach (var item in group.Items)
                {
                    StartItem(item);
                }
            }
        }
        private void StartItem(Item item)
        {
            item.OrderStatus = OrderStatus.Started;
            item.SCourseGroup.OrderStatus = OrderStatus.Started;
            item.CourseGroup.OrderStatus = OrderStatus.Started;
            item.SCourseGroup.Order.OrderStatus = OrderStatus.Started;
            DbOrderItem.UpdateOrderItemStatus(item.OrderItemKey, OrderStatus.Started);
        }
        #endregion

        #region Hold
        public void Hold(object sender)
        {
            if (sender != null)
            {
                if (sender.GetType() == typeof(Order))
                    HoldOrder(sender as Order);
                if (sender.GetType() == typeof(Group))
                    HoldGroup(sender as Group);
                if (sender.GetType() == typeof(Item))
                    HoldItem(sender as Item);
            }
        }
        private void HoldOrder(Order order)
        {
            order.OrderStatus = OrderStatus.Hold;
            foreach (var group in order.ServingCourseGroups)
            {
                HoldGroup(group);
            }
            foreach (var group in order.CourseGroups)
            {
                HoldGroup(group);
            }
        }
        private void HoldGroup(Group group)
        {
            if (group.OrderStatus != OrderStatus.Bumped)
            {
                group.OrderStatus = OrderStatus.Hold;
                foreach (var item in group.Items)
                {
                    HoldItem(item);
                }
            }
        }
        private void HoldItem(Item item)
        {
            item.OrderStatus = OrderStatus.Hold;
        }        
        #endregion

        #region Hide
        public void HideSow(object sender, bool visibility)
        {
            if (sender != null)
            {
                if (sender.GetType() == typeof(Order))
                    HideShowOrder(sender as Order, visibility);
                if (sender.GetType() == typeof(Group))
                    HideShowGroup(sender as Group, visibility);
                if (sender.GetType() == typeof(Item))
                    HideShowItem(sender as Item, visibility);
            }
        }
        private void HideShowOrder(Order order, bool visibility)
        {
            foreach (var sGroup in order.ServingCourseGroups)
            {
                HideShowGroup(sGroup, visibility);
            }
        }
        private void HideShowGroup(Group group, bool visibility)
        {
            group.IsVisible = visibility;
            foreach (var item in group.Items)
            {
                HideShowItem(item, visibility, false);
            }
        }
        private void HideShowItem(Item item, bool visibility, bool isSenderItem = true)
        {
            if (isSenderItem)
            {
                item.IsChildVisible = visibility;
            }
            else
            {
                item.IsChildVisible = visibility;
                item.IsVisible = visibility;
            }

        }
        #endregion

        #region ItemNote
        private void ItemNote(object sender)
        {
            AddNote(sender as Item);
        }

        private void AddNote(Item item)
        {
            item.Note = KeyboardController.Instance.OpenKeyBoard("Enter Note", item.Note);
            if (!string.IsNullOrWhiteSpace(item.Note) && item.Note.Length > 80)
            {
                item.Note = item.Note.Substring(0, 80);
            }
            item.IsNoteVisible = true;
            DbOrderItem.UpdateOrderItemNote(item.OrderItemKey, item.Note);
        }

        #endregion

        #region Keypad Operation
        public bool BumpByChitTableNumber(string chitTableNumber)
        {
            bool bumpChit = BumpByChitNumber(chitTableNumber);
            bool bumpTable = BumpByTableNumber(chitTableNumber);
            return bumpTable || bumpChit;
        }

        private bool BumpByChitNumber(string chitNumber)
        {
            var order = ChefmateController.Instance.CurrentDisplayOrders.FirstOrDefault(s => s.ChitValue.Contains(chitNumber));
            if (order != null)
            {
                BumpOrder(order);
                return true;
            }
            return false;
        }

        private bool BumpByTableNumber(string tableNumber)
        {
            var order = ChefmateController.Instance.CurrentDisplayOrders.FirstOrDefault(s => s.TableTabName.Contains(tableNumber));
            if (order != null)
            {
                BumpOrder(order);
                return true;
            }
            return false;
        }
        #endregion

        #region StatusChange
        public void CheckStatus(Group group)
        {
            OrderStatus itemStatus = group.Items[0].OrderStatus;

            if (itemStatus != OrderStatus.Started)
            {
                foreach (Item item in group.Items)
                {
                    OrderStatus currentItemStatus = item.OrderStatus;
                    if (itemStatus == OrderStatus.Canceled || itemStatus == OrderStatus.OutOfStock)
                    {
                        itemStatus = currentItemStatus;
                    }
                    else if ((itemStatus == OrderStatus.Transfered && currentItemStatus == OrderStatus.Bumped)
                                || currentItemStatus == OrderStatus.Canceled
                                || currentItemStatus == OrderStatus.OutOfStock)
                    {
                        continue;
                    }
                    else if (itemStatus != currentItemStatus)
                    {
                        itemStatus = OrderStatus.Started;
                        break;
                    }
                }
            }
            switch (itemStatus)
            {
                case OrderStatus.Normal:
                    group.OrderStatus = OrderStatus.Normal;
                    break;
                case OrderStatus.Started:
                    group.OrderStatus = OrderStatus.Started;
                    break;
                case OrderStatus.Hold:
                    group.OrderStatus = OrderStatus.Hold;
                    break;
                case OrderStatus.Transfered:
                    group.OrderStatus = OrderStatus.Transfered;
                    break;
                case OrderStatus.Bumped:
                case OrderStatus.Canceled:
                case OrderStatus.OutOfStock:
                    group.OrderStatus = OrderStatus.Completed;
                    break;
                default:
                    group.OrderStatus = OrderStatus.Normal;
                    break;
            }

            if (group.OrderStatus == OrderStatus.Completed && ChefmateController.Instance.CurrentSettings.AutoHide)
            {
                HideShowGroup(group, false);
            }

            CheckStatus(group.Order);
        }
        public void CheckStatus(Order order)
        {
            OrderStatus groupStatus = order.DisplayGroups[0].OrderStatus;

            if (groupStatus != OrderStatus.Started)
            {
                foreach (var group in order.DisplayGroups)
                {
                    OrderStatus currentGroupStatus = group.OrderStatus;
                    if (groupStatus != currentGroupStatus)
                    {
                        groupStatus = OrderStatus.Started;
                        break;
                    }
                }
            }

            if (groupStatus == OrderStatus.Started || groupStatus == OrderStatus.Completed)
            {
                order.OrderStatus = OrderStatus.Started;
            }
            else
            {
                order.OrderStatus = groupStatus;
            }
        }
        public void ChangeItemStatus(Item item, OrderStatus inStatus)
        {
            if (inStatus != OrderStatus.Canceled)
            {
                item.OrderStatus = inStatus;
                if (item.OrderItemKey > 0)
                    DbOrderItem.UpdateOrderItemStatus(item.OrderItemKey, inStatus);

                var group = ChefmateController.Instance.CurrentSettings.GroupType == GroupType.ServingCourse ? item.SCourseGroup : item.CourseGroup;
                if (group != null)
                {
                    CheckStatus(group);
                }
            }
        }
        public void ChangeOrderStatus(Order order, OrderStatus inOrderStatus)
        {
            if (order != null)
            {
                // Set the orders status
                order.OrderStatus = inOrderStatus;

                foreach (Group group in order.DisplayGroups)
                {
                    foreach (Item item in group.Items)
                    {
                        DbOrderItem.UpdateOrderItemStatus(item.OrderItemKey, inOrderStatus);
                    }
                }
            }
        }
        public void ChangeGroupStatus(Group inGroup, OrderStatus inGroupStatus)
        {
            inGroup.OrderStatus = inGroupStatus;
            CheckStatus(inGroup.Order);
        }
        #endregion


    }
}