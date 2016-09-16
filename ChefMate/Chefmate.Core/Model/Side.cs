using Chefmate.Core.Enums;

namespace Chefmate.Core.Model
{

    public class Side : ModelBase
    {
        private string _name;
        private string _displayName;
        private bool _isCanceled;
        private OrderStatus _status;
        private DisplayAttributes _displayAttributes;

        public Side()
        {
            _status = OrderStatus.Normal;
            DisplayAttributes = new DisplayAttributes();
        }

        public Side(Side inSide) : this()
        {
            Name = inSide.Name;
            DisplayName = inSide.DisplayName;
            IsCanceled = inSide.IsCanceled;
            SideKey = inSide.SideKey;
            SidePosKey = inSide.SidePosKey;
            OrderSideKey = inSide.OrderSideKey;
            ItemStatus = inSide.ItemStatus;
            OrderItemKey = inSide.OrderItemKey;
            DisplayAttributes = new DisplayAttributes(inSide.DisplayAttributes);
        }


        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }
        public string DisplayName
        {
            get { return "> " + _name; }
            set
            {
                _displayName = value;
                OnPropertyChanged("DisplayName");
            }
        }
        public bool IsCanceled
        {
            get { return _isCanceled; }
            set
            {
                _isCanceled = value;
                OnPropertyChanged("IsCanceled");
            }
        }
        public int SideKey { get; set; }
        public int SidePosKey { get; set; }
        public int OrderSideKey { get; set; }
        public OrderStatus ItemStatus
        {
            get
            {
                return _status;
            }
            set
            {
                _status = value;
                OnPropertyChanged("ItemStatus");
            }
        }
        public int OrderItemKey { get; set; }
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
        public Item Item { get; set; }
    }
}
