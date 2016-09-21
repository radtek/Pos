using System;
using System.Collections.ObjectModel;
using System.Linq;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;

namespace Chefmate.Core.Model
{
    public class Order : ModelBase
    {
        #region Private Members
        private int _orderKey;
        private int _orderNumber;
        private int _patronCount;
        private string _chitValue;
        private string _tableTabName;
        private string _partyName;
        private string _serverName;
        private string _customerName;
        private string _orderType;
        private string _sourceTableName;
        private OrderState _state;
        private OrderStatus _orderStatus;
        private ObservableCollection<Group> _sCourseGroups;
        private ObservableCollection<Group> _courseGroups;
        private ObservableCollection<Group> _displayGroups;
        private ObservableCollection<Item> _items;
        private DisplayAttributes _displayAttributes;
        private DateTime _bumpTime;
        private string _customerPhone;
        private string _customerEmail;        
        private string _customerAddress;
        private string _paymentStatus;
        private string _orderAction;
        
        #endregion

        #region Public Properties

        public ObservableCollection<Item> Items
        {
            get { return _items; }
            set
            {
                _items = value;
                OnPropertyChanged("Items");
            }
        }

        public int OrderKey
        {
            get { return _orderKey; }
            set
            {
                _orderKey = value;
                OnPropertyChanged("OrderKey");
            }
        }
        public int OrderNumber
        {
            get { return _orderNumber; }
            set
            {
                _orderNumber = value;
                OnPropertyChanged("OrderNumber");
            }
        }
        public int OrderPosKey { get; set; }
        public int PatronCount
        {
            get { return _patronCount; }
            set
            {
                _patronCount = value == 0 ? 1 : value;
                OnPropertyChanged("PatronCount");
            }
        }
        public string ChitValue
        {
            get { return _chitValue; }
            set
            {
                _chitValue = value;
                OnPropertyChanged("ChitValue");
            }
        }
        public string TableTabName
        {
            get { return _tableTabName; }
            set
            {
                _tableTabName = value;
                OnPropertyChanged("TableTabName");
            }
        }
        public string PartyName
        {
            get { return _partyName; }
            set
            {
                _partyName = value;
                OnPropertyChanged("PartyName");
            }
        }
        public string ServerName
        {
            get { return _serverName; }
            set
            {
                _serverName = value;
                OnPropertyChanged("ServerName");
            }
        }
        public string CustomerName
        {
            get { return _customerName; }
            set
            {
                _customerName = value;
                OnPropertyChanged("CustomerName");
            }
        }
        public string OrderType
        {
            get { return _orderType; }
            set
            {
                _orderType = value;
                OnPropertyChanged("OrderType");
            }
        }
        public string SourceTableName
        {
            get { return _sourceTableName; }
            set
            {
                _sourceTableName = value;
                OnPropertyChanged("SourceTableName");
            }
        }
        public OrderState OrderState
        {
            get { return _state; }
            set
            {
                _state = value;
                OnPropertyChanged("OrderState");
            }
        }
        public OrderStatus OrderStatus
        {
            get { return _orderStatus; }
            set
            {
                _orderStatus = value;
                OnPropertyChanged("OrderStatus");
                ChangeStatus();
            }
        }
        public ObservableCollection<Group> ServingCourseGroups
        {
            get { return _sCourseGroups; }
            set
            {
                _sCourseGroups = value;
                OnPropertyChanged("ServingCourseGroups");
            }
        }
        public ObservableCollection<Group> CourseGroups
        {
            get { return _courseGroups; }
            set
            {
                _courseGroups = value;
                OnPropertyChanged("CourseGroups");
            }
        }
        public ObservableCollection<Group> DisplayGroups
        {
            get { return _displayGroups; }
            set
            {
                _displayGroups = value;
                OnPropertyChanged("DisplayGroups");
            }
        }
        public DisplayAttributes DisplayAttributes
        {
            get
            {
                return _displayAttributes;
            }
            set
            {
                _displayAttributes = value;
                OnPropertyChanged("DisplayAttributes");
            }
        }
        public DateTime BumpTime
        {
            get { return _bumpTime; }
            set
            {
                _bumpTime = value;
                OnPropertyChanged("BumpTime");
            }
        }
        public string CustomerPhone
        {
            get { return _customerPhone; }
            set 
            { 
                _customerPhone = value;
                OnPropertyChanged("CustomerPhone");
            }
        }
        public string CustomerEmail
        {
            get { return _customerEmail; }
            set 
            { 
                _customerEmail = value;
                OnPropertyChanged("CustomerEmail");
            }
        }
        public string CustomerAddress
        {
            get { return _customerAddress; }
            set 
            { 
                _customerAddress = value;
                OnPropertyChanged("CustomerAddress");
            }
        }
        public string PaymentStatus
        {
            get { return _paymentStatus; }
            set
            {
                _paymentStatus = value;
                OnPropertyChanged("PaymentStatus");
            }
        }
        public string OrderAction
        {
            get { return _orderAction; }
            set 
            {
                _orderAction = value;
                OnPropertyChanged("OrderAction");
            }
        }
        public DateTime SaleStartTime { get; set; }
        public DateTime SaleFinishTime { get; set; }
        public DateTime DeliveryTime { get; set; }
        public DateTime ArrivalTime { get; set; }
        public bool BeenSentToOutput { get; set; }
        public double CordX { get; set; }
        public double CordY { get; set; }

        #endregion

        #region Constructors
        public Order()
        {
            Items = new ObservableCollection<Item>();
            ServingCourseGroups = new ObservableCollection<Group>();
            CourseGroups = new ObservableCollection<Group>();
            DisplayGroups = new ObservableCollection<Group>();
            DisplayAttributes = new DisplayAttributes();
            DisplayAttributes.BackGroundColor = ChefmateConstants.NormalOrderColor;
            BeenSentToOutput = false;
            OrderStatus = OrderStatus.Normal;
        }
        public Order(Order inOrder)
            : this()
        {
            OrderKey = inOrder.OrderKey;
            OrderNumber = inOrder.OrderNumber;
            OrderPosKey = inOrder.OrderPosKey;
            PatronCount = inOrder.PatronCount;
            ChitValue = inOrder.ChitValue;
            TableTabName = inOrder.TableTabName;
            PartyName = inOrder.PartyName;
            ServerName = inOrder.ServerName;
            CustomerName = inOrder.CustomerName;
            OrderType = inOrder.OrderType;
            SourceTableName = inOrder.SourceTableName;
            OrderState = inOrder.OrderState;
            OrderStatus = inOrder.OrderStatus;
            BumpTime = inOrder.BumpTime;
            SaleStartTime = inOrder.SaleStartTime;
            SaleFinishTime = inOrder.SaleFinishTime;
            DeliveryTime = inOrder.DeliveryTime;
            ArrivalTime = inOrder.ArrivalTime;
            BeenSentToOutput = inOrder.BeenSentToOutput;
            CustomerPhone = inOrder.CustomerPhone;
            CustomerEmail = inOrder.CustomerEmail;
            CustomerAddress = inOrder.CustomerAddress;
            PaymentStatus = inOrder.PaymentStatus;
            OrderAction = inOrder.OrderAction;
            DisplayAttributes = new DisplayAttributes(inOrder.DisplayAttributes);
        }
        #endregion

        #region Public Methods
        public void FilterOrders(GroupType inGroupType, OrderInfoDisplay orderInfoDisplay)
        {
            ServingCourseGroups.ToList().ForEach(s => s.Items.Clear());
            CourseGroups.ToList().ForEach(s => s.Items.Clear());
            foreach (var item in Items)
            {
                item.SCourseGroup.Items.Add(item);
                item.CourseGroup.Items.Add(item);
            }
            DisplayGroups = inGroupType == GroupType.ServingCourse
                    ? ServingCourseGroups
                    : CourseGroups;
            DisplayGroups.RemoveAll(s => s.Items.Count == 0);
            UpdateOrderInfoDisplay(orderInfoDisplay);
        }
        public void UpdateOrderInfoDisplay(OrderInfoDisplay orderInfoDisplay)
        {
            ChangeStatus();
            switch (orderInfoDisplay)
            {
                case OrderInfoDisplay.ChitNumber:
                    if (!string.IsNullOrWhiteSpace(_chitValue))
                    {
                        DisplayAttributes.TabtableText = _chitValue;
                    }
                    else
                    {
                        DisplayAttributes.TabtableText = _tableTabName;
                    }
                    break;
                default:
                    if (!_tableTabName.EndsWith("Sale") || string.IsNullOrWhiteSpace(_chitValue))
                    {
                        DisplayAttributes.TabtableText = _tableTabName;
                    }
                    else
                    {
                        DisplayAttributes.TabtableText = _chitValue;
                    }
                    break;
            }
        }
        public double GetOrderActualHeight()
        {
            var orderHeight = DisplayAttributes.IsHeaderVisible ? ChefmateConstants.OrderHeaderHeight : 0;
            foreach (var group in DisplayGroups)
            {
                var groupHeight = group.GetGroupActualHeight();
                orderHeight += groupHeight;
            }
            return orderHeight;
        }

        public void UpdateOrder()
        {
            if (Items.Count(s => !s.DisplayAttributes.IsOriginalItem) == Items.Count)
                Items.Clear();
        }

        #endregion

        #region Private Methods
        private void ChangeStatus()
        {
            switch (OrderState)
            {
                case OrderState.Complete:
                case OrderState.Runner:
                    DisplayAttributes.OrderNumberText = ServerName;
                    DisplayAttributes.PatronDisplayText = "Guest # " + _patronCount;
                    break;
                case OrderState.Cancel:
                    DisplayAttributes.OrderNumberText = "Cancel Order";
                    DisplayAttributes.PatronDisplayText = "";
                    break;
                case OrderState.Credit:
                    DisplayAttributes.OrderNumberText = "Refund Order";
                    DisplayAttributes.PatronDisplayText = "";
                    break;
            }
        }
        #endregion
    }
}
