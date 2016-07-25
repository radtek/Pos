using System;
using System.Collections.ObjectModel;
using Chefmate.Core.Enums;

namespace Chefmate.Core.Model
{
    public class Item : ModelBase
    {
        private int _orderItemKey;
        private string _name;
        private string _note;
        private OrderStatus _status;
        private ObservableCollection<Option> _options;
        private ObservableCollection<Side> _sides;
        private bool _isVisible;
        private bool _isNoteVisible;
        private bool _isChildVisible;
        private Group _sCourseGroup;
        private Group _courseGroup;
        private DisplayAttributes _displayAttributes;
        public Item()
        {
            Sides = new ObservableCollection<Side>();
            Options = new ObservableCollection<Option>();
            DisplayAttributes = new DisplayAttributes();
            IsVisible = true;
            IsChildVisible = true;
        }

        public int ItemKey { get; set; }
        public int ItemPosKey { get; set; }
        public int OrderItemKey
        {
            get { return _orderItemKey; }
            set
            {
                _orderItemKey = value;
                OnPropertyChanged("OrderItemKey");
            }
        }
        public int OrderItemPosKey { get; set; }
        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }
        public string Note
        {
            get { return _note; }
            set
            {
                _note = value;
                OnPropertyChanged("Note");
            }
        }
        public bool IsVisible
        {
            get { return _isVisible; }
            set
            {
                _isVisible = value;
                OnPropertyChanged("IsVisible");
            }
        }
        public bool IsNoteVisible
        {
            get { return !string.IsNullOrWhiteSpace(_note); }
            set
            {
                _isNoteVisible = value;
                OnPropertyChanged("IsNoteVisible");
            }
        }
        public bool IsChildVisible
        {
            get { return _isChildVisible; }
            set
            {
                _isChildVisible = value;
                OnPropertyChanged("IsChildVisible");
            }
        }
        public OrderStatus OrderStatus
        {
            get { return _status; }
            set
            {
                _status = value;
                OnPropertyChanged("OrderStatus");
            }
        }
        public ObservableCollection<Option> Options
        {
            get { return _options; }
            set
            {
                _options = value;
                OnPropertyChanged("Options");
            }
        }
        public ObservableCollection<Side> Sides
        {
            get { return _sides; }
            set
            {
                _sides = value;
                OnPropertyChanged("Sides");
            }
        }
        public Group CourseGroup
        {
            get
            {
                return _courseGroup;
            }
            set
            {
                _courseGroup = value;
                OnPropertyChanged("CourseGroup");
            }
        }
        public Group SCourseGroup
        {
            get
            {
                return _sCourseGroup;
            }
            set
            {
                _sCourseGroup = value;
                OnPropertyChanged("SCourseGroup");
            }
        }
        public bool BeenSentToOutput { get; set; }
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
        public DateTime ArrivalTime { get; set; }
        public DateTime BumpTime { get; set; }
        public int ServingCourseKey { get; set; }
        public int CourseKey { get; set; }


    }
}