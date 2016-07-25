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
        public DateTime SaleStartTime { get; set; }
        public DateTime SaleFinishTime { get; set; }
        public DateTime DeliveryTime { get; set; }
        public DateTime ArrivalTime { get; set; }
        public bool BeenSentToOutput { get; set; }

        #endregion

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
        public void FilterOrders()
        {
            ServingCourseGroups.ToList().ForEach(s => s.Items.Clear());
            CourseGroups.ToList().ForEach(s => s.Items.Clear());
            foreach (var item in Items)
            {
                item.SCourseGroup.Items.Add(item);
                item.CourseGroup.Items.Add(item);
            }
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

    }
}
