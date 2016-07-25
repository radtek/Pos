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
        private int _isTransferredItem;
        private string _sourceTabTableText;
        
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
        public int IsTransferredItem
        {
            get 
            { 
                return _isTransferredItem; 
            }
            set 
            { 
                _isTransferredItem = value;
                OnPropertyChanged("IsTransferredItem");
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
            }
        }
        
    }
}