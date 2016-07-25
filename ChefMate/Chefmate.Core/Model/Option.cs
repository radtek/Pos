using Chefmate.Core.Enums;

namespace Chefmate.Core.Model
{
    public class Option : ModelBase
    {
        private string _name;
        private string _displayName;
        private bool _isPlus;
        private OrderStatus _status;
        private DisplayAttributes _displayAttributes;

        public Option()
        {
            _status = OrderStatus.Normal;
            DisplayAttributes = new DisplayAttributes(); 
        }
        public int OptionKey { get; set; }

        public int OptionPosKey { get; set; }

        public int OrderOptionKey { get; set; }
        public string DisplayName
        {
            get { return IsPlus ? "+ " + _name : "- " + _name; }
            set
            {
                _displayName = value;
                OnPropertyChanged("DisplayName");
            }
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
        public bool IsPlus
        {
            get { return _isPlus; }
            set
            {
                _isPlus = value;
                OnPropertyChanged("IsPlus");
            }
        }
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
        public Item Item { get; set; }
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
    }
}
