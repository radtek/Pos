using System.Windows.Media;

namespace Chefmate.Core.Model
{
    public class DisplayAttributes : ModelBase
    {
        //don't create in databse;
        private Brush _orderColor;
        private bool _executePopUp;
        private string _patronDisplayText;
        private string _tabtableText;
        private string _orderNumberText;
        private string _timerDisplayText;
        private bool _isBlinkingEnable;
        private bool _isOriginalItem;
        private string _sourceTabTableText;
        private bool _isHeaderVisible;
        private bool _isSourceTableExist;
        public DisplayAttributes()
        {
            IsHeaderVisible = true;
            _isOriginalItem = true;
        }
        public DisplayAttributes(DisplayAttributes inDisplayAttributes) : this()
        {
            IsHeaderVisible = inDisplayAttributes.IsHeaderVisible;
            ExecutePopUp = inDisplayAttributes.ExecutePopUp;
            IsBlinkingEnable = inDisplayAttributes.ExecutePopUp;
            TimerDisplayText = inDisplayAttributes.TimerDisplayText;
            PatronDisplayText = inDisplayAttributes.PatronDisplayText;
            TabtableText = inDisplayAttributes.TabtableText;
            OrderNumberText = inDisplayAttributes.OrderNumberText;
            BackGroundColor = inDisplayAttributes.BackGroundColor;
            IsOriginalItem = inDisplayAttributes.IsOriginalItem;
            SourceTabTableText = inDisplayAttributes.SourceTabTableText;
        }
        public bool IsHeaderVisible
        {
            get { return _isHeaderVisible; }
            set
            {
                _isHeaderVisible = value;
                OnPropertyChanged("IsHeaderVisible");
            }
        }
        public bool ExecutePopUp
        {
            get { return _executePopUp; }
            set
            {
                _executePopUp = value;
                OnPropertyChanged("ExecutePopUp");
            }
        }
        public bool IsBlinkingEnable
        {
            get { return _isBlinkingEnable; }
            set
            {
                _isBlinkingEnable = value;
                OnPropertyChanged("IsBlinkingEnable");
            }
        }
        public string TimerDisplayText
        {
            get { return _timerDisplayText; }
            set
            {
                _timerDisplayText = value;
                OnPropertyChanged("TimerDisplayText");
            }
        }
        public string PatronDisplayText
        {
            get { return _patronDisplayText; }
            set
            {
                _patronDisplayText = value;
                OnPropertyChanged("PatronDisplayText");
            }
        }
        public string TabtableText
        {
            get { return _tabtableText; }
            set
            {
                _tabtableText = value;
                OnPropertyChanged("TabtableText");
            }
        }
        public string OrderNumberText
        {
            get { return _orderNumberText; }
            set
            {
                _orderNumberText = value;
                OnPropertyChanged("OrderNumberText");
            }
        }
        public Brush BackGroundColor
        {
            get { return _orderColor; }
            set
            {
                _orderColor = value;
                OnPropertyChanged("BackGroundColor");
            }
        }
        public bool IsOriginalItem
        {
            get
            {
                return _isOriginalItem;
            }
            set
            {
                _isOriginalItem = value;
                OnPropertyChanged("IsOriginalItem");
            }
        }
        public string SourceTabTableText
        {
            get
            {
                return _sourceTabTableText;
            }
            set
            {
                _sourceTabTableText = value;
                OnPropertyChanged("SourceTabTableText");
                IsSourceTableExist = !string.IsNullOrWhiteSpace(value);
            }
        }
        public bool IsSourceTableExist
        {
            get
            {
                return _isSourceTableExist;
            }
            set
            {
                _isSourceTableExist = value;
                OnPropertyChanged("IsSourceTableExist");
            }
        }
    }
}