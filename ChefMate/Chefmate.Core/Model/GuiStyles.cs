using System.Windows;
using System.Windows.Media;

namespace Chefmate.Core.Model
{
    public class GuiStyles : ModelBase
    {
        private FontFamily _groupFontFamily;
        private FontFamily _itemFontFamily;
        private FontFamily _orderFontFamily;
        private FontFamily _analysisFontFamily;
        private FontFamily _itemSummaryFontFamily;
        private int _itemFontSize;
        private int _groupFontSize;
        private int _orderHeaderFontSize;
        private int _analysisFontSize;
        private int _itemSummaryFontSize;
        private FontWeight _groupFontWeight;
        private FontWeight _analysisFontWeight;
        private SolidColorBrush _headerBackGround;
        private SolidColorBrush _headerForeGround;
        private SolidColorBrush _itemSummaryBackGround;

        public int ItemFontSize
        {
            get { return _itemFontSize; }
            set
            {
                _itemFontSize = value;
                OnPropertyChanged("ItemFontSize");
            }
        }
        public int GroupFontSize
        {
            get { return _groupFontSize; }
            set
            {
                _groupFontSize = value;
                OnPropertyChanged("GroupFontSize");
            }
        }
        public int OrderHeaderFontSize
        {
            get { return _orderHeaderFontSize; }
            set
            {
                _orderHeaderFontSize = value;
                OnPropertyChanged("OrderHeaderFontSize");
            }
        }
        public int AnalysisFontSize
        {
            get { return _analysisFontSize; }
            set
            {
                _analysisFontSize = value;
                OnPropertyChanged("AnalysisFontSize");
            }
        }
        public int ItemSummaryFontSize
        {
            get { return _itemSummaryFontSize; }
            set
            {
                _itemSummaryFontSize = value;
                OnPropertyChanged("ItemSummaryFontSize");
            }
        }
        public FontWeight GroupFontWeight
        {
            get { return _groupFontWeight; }
            set
            {
                _groupFontWeight = value;
                OnPropertyChanged("GroupFontWeight");
            }
        }
        public FontWeight AnalysisFontWeight
        {
            get { return _analysisFontWeight; }
            set
            {
                _analysisFontWeight = value;
                OnPropertyChanged("AnalysisFontWeight");
            }
        }
        public FontFamily GroupFontFamily
        {
            get { return _groupFontFamily; }
            set
            {
                _groupFontFamily = value;
                OnPropertyChanged("GroupFontFamily");
            }
        }
        public FontFamily ItemFontFamily
        {
            get { return _itemFontFamily; }
            set
            {
                _itemFontFamily = value;
                OnPropertyChanged("ItemFontFamily");
            }
        }
        public FontFamily AnalysisFontFamily
        {
            get { return _analysisFontFamily; }
            set
            {
                _analysisFontFamily = value;
                OnPropertyChanged("AnalysisFontFamily");
            }
        }
        public FontFamily OrderFontFamily
        {
            get { return _orderFontFamily; }
            set
            {
                _orderFontFamily = value;
                OnPropertyChanged("OrderFontFamily");
            }
        }
        public FontFamily ItemSummaryFontFamily
        {
            get { return _itemSummaryFontFamily; }
            set
            {
                _itemSummaryFontFamily = value;
                OnPropertyChanged("ItemSummaryFontFamily");
            }
        }
        public SolidColorBrush HeaderForeGround
        {
            get { return _headerForeGround; }
            set
            {
                _headerForeGround = value;
                OnPropertyChanged("HeaderForeGround");
            }
        }
        public SolidColorBrush HeaderBackGround
        {
            get { return _headerBackGround; }
            set
            {
                _headerBackGround = value;
                OnPropertyChanged("HeaderBackGround");
            }
        }
        public SolidColorBrush ItemSummaryBackGround
        {
            get { return _itemSummaryBackGround; }
            set
            {
                _itemSummaryBackGround = value;
                OnPropertyChanged("ItemSummaryBackGround");
            }
        }
    }
}