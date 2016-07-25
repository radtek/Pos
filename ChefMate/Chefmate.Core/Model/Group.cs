using System;
using System.Collections.ObjectModel;
using System.Windows.Media;
using Chefmate.Core.Enums;

namespace Chefmate.Core.Model
{
    public class Group : ModelBase
    {
        private int _orderGroupKey;
        private string _groupName;
        private string _groupDisplayName;
        private bool _calledAway;
        private OrderStatus _groupStatus;
        private GroupType _groupType;
        private ObservableCollection<Item> _items;
        private DisplayAttributes _displayAttributes;
        //don't create in databse;
        private Order _order;
        private int _orderKey;
        public Group()
        {
            IsVisible = true;
            DisplayAttributes = new DisplayAttributes();
            DisplayAttributes.BackGroundColor = ChefmateConstants.NormalGroupColor;
            Items = new ObservableCollection<Item>();
        }

        #region Public Members
        public int GroupKey { get; set; }
        public int GroupPosKey { get; set; }
        public int OrderGroupKey
        {
            get
            {
                return _orderGroupKey;
            }
            set
            {
                _orderGroupKey = value;
                OnPropertyChanged("OrderGroupKey");
            }
        }
        public int DisplayOrder { get; set; }
        public string GroupName
        {
            get
            {
                return _groupName;
            }
            set
            {
                _groupName = value;
                OnPropertyChanged("GroupName");
            }
        }
        public string GroupDisplayName
        {
            get
            {
                return _calledAway ? _groupName + "- Call Away " : _groupName;
            }
            set
            {
                _groupDisplayName = value;
                OnPropertyChanged("GroupDisplayName");
            }
        }
        public bool IsVisible { get; set; }
        public bool BeenSentToOutput { get; set; }
        public bool CalledAway
        {
            get
            {
                return _calledAway;
            }
            set
            {
                _calledAway = value;
                OnPropertyChanged("CalledAway");
                if (_calledAway)
                {
                    DisplayAttributes.IsBlinkingEnable = true;
                   
                }
            }
        }
        public OrderStatus OrderStatus
        {
            get
            {
                return _groupStatus;
            }
            set
            {
                _groupStatus = value;
                OnPropertyChanged("OrderStatus");
                if (_groupStatus == OrderStatus.Bumped)
                    IsVisible = false;
            }
        }
        public GroupType GroupType
        {
            get
            {
                return _groupType;
            }
            set
            {
                _groupType = value;
                OnPropertyChanged("GroupType");
            }
        }
        public ObservableCollection<Item> Items
        {
            get
            {
                return _items;
            }
            set
            {
                _items = value;
                OnPropertyChanged("Items");
            }
        }
        public Order Order
        {
            get { return _order; }
            set
            {
                _order = value;
                OnPropertyChanged("Order");
                if (_order != null)
                    OrderKey = _order.OrderKey;
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
        public DateTime CompletionTime { get; set; }
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
        #endregion
    }
}
