using System;
using System.Collections.ObjectModel;
using System.Linq;
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

        public Group(Group inGroup) : this()
        {
            GroupKey = inGroup.GroupKey;
            GroupPosKey = inGroup.GroupPosKey;
            OrderGroupKey = inGroup.OrderGroupKey;
            DisplayOrder = inGroup.DisplayOrder;
            GroupName = inGroup.GroupName;
            GroupDisplayName = inGroup.GroupDisplayName;
            IsVisible = inGroup.IsVisible;
            BeenSentToOutput = inGroup.BeenSentToOutput;
            CalledAway = inGroup.CalledAway;
            OrderStatus = inGroup.OrderStatus;
            GroupType = inGroup.GroupType;
            OrderKey = inGroup.OrderKey;
            CompletionTime = inGroup.CompletionTime;
            DisplayAttributes = new DisplayAttributes(inGroup.DisplayAttributes);
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

        #endregion

        public double GetGroupActualHeight()
        {
            var groupHeight = ChefmateConstants.UnitHeight;
            foreach (var item in Items)
            {
                var itemHeight = item.GetItemActualHeight();
                groupHeight += itemHeight;
            }
            return groupHeight;
        }
    }
}
