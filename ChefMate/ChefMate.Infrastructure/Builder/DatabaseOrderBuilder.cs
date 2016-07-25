using System;
using System.Collections.ObjectModel;
using System.Linq;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Controller;
using Chefmate.Infrastructure.Utility;

namespace Chefmate.Infrastructure.Builder
{
    public class DatabaseOrderBuilder
    {
        private static DatabaseOrderBuilder _instance = null;
        private static object syncRoot = new Object();
        DatabaseOrderBuilder()
        {
        }

        public static DatabaseOrderBuilder Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new DatabaseOrderBuilder();
                    }
                }

                return _instance;
            }
        }
        public ObservableCollection<Order> GetAllOrders(int terminalKey)
        {
            var orders = DbOrder.GetAllOrders(terminalKey);
            foreach (var order in orders)
            {
                order.OrderState = ChefmateController.Instance.CurrentSettings.TerminalType == TerminalType.Runner ? OrderState.Runner : OrderState.Complete;
                SetOrderStatus(order);
            }
            return orders;
        }
        private void SetOrderStatus(Order order)
        {
            foreach (var servingCourse in order.ServingCourseGroups)
            {
                SetGroupStatus(servingCourse);
            }
            foreach (var course in order.CourseGroups)
            {
                SetGroupStatus(course);
            }
            if (order.Items.Any(s => s.OrderStatus == OrderStatus.Started))
                order.OrderStatus = OrderStatus.Started;
        }
        private void SetGroupStatus(Group group)
        {
            if (group.Items.Any(s => s.OrderStatus == OrderStatus.Started))
                group.OrderStatus = OrderStatus.Started;
        }
        public Order BuildOrderFromOrderKey(int inOrderDbKey)
        {
            var order = DbOrder.GetOrder(inOrderDbKey, ChefmateController.Instance.CurrenTerminal.TerminalId);
            SetOrderState(order);
            return order;
        }
        public Order BuildOrderFromOrderGroupKey(int inOrderGroupDbKey)
        {
            var order = DbOrder.GetOrderFromOrderGroupKey(inOrderGroupDbKey, ChefmateController.Instance.CurrenTerminal.TerminalId);
            SetOrderState(order);
            return order;
        }
        public Order BuildOrderFromOrderItemKey(int inOrderItemDbKey)
        {
            var order = DbOrder.GetOrderFromOrderItemKey(inOrderItemDbKey, ChefmateController.Instance.CurrenTerminal.TerminalId);
            SetOrderState(order);
            return order;
        }
        private void SetOrderState(Order order)
        {
            PlaySoundByOrderStatus(order.OrderStatus);
            if (ChefmateController.Instance.CurrenTerminal.TerminalType == TerminalType.Runner)
                order.OrderState = OrderState.Runner;
        }
        private void PlaySoundByOrderStatus(OrderStatus orderStatus)
        {
            switch (orderStatus)
            {
                case OrderStatus.Normal:
                    SoundManager.PlaySound(SoundEvents.BumpedOrderArrivedFromCm);
                    break;
                case OrderStatus.Transfered:
                    SoundManager.PlaySound(SoundEvents.TransferOrderArrivedFromCm);
                    break;
            }
        }

    }
}
