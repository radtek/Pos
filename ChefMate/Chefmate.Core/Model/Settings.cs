using System;
using System.IO.Packaging;
using Chefmate.Core.Enums;

namespace Chefmate.Core.Model
{
    public class Settings : ModelBase, ICloneable
    {
        private string _newOrderMmSound;
        private string _cancelNoticeMmSound;
        private string _refundNoticeMmSound;
        private string _transferNoticeMmSound;
        private string _orderArrivedCmSound;
        private string _transferOrderCmSound;
        private string _dbIpAddress;
        private string _dbPath;
        private string _terminalIpAddress;
        private string _displayName;
        private string _groupFontFamily;
        private string _itemFontFamily;
        private string _orderFontFamily;
        private string _analysisFontFamily;
        private string _itemSummaryFontFamily;
        private string _firstWarningColor;
        private string _secondWarningColor;
        private string _newOrderColor;
        private string _headerBackgroundColor;
        private string _headerForegroundColor;
        private string _itemSummaryBackgroundColor;
        private int _outputTerminal;
        private int _firstWarningTime;
        private int _secondWarningTime;
        private int _recallCount;
        private int _webOrderTime;
        private int _cmFontSize;
        private int _groupFontSize;
        private int _orderHeaderFontSize;
        private int _analysisFontSize;
        private int _itemSummaryFontSize;
        private bool _groupFontBold;
        private bool _analysisFontBold;
        private bool _autoHide;
        private bool _bumpOnly;
        private bool _confirmOnRefund;
        private bool _confirmOnTransfer;
        private GroupType _groupType;
        private OrderInfoDisplay _orderInfoDisplay;
        private TerminalType _terminalType;
        private OutputTime _outputTime;
        private OutputScope _outputScope;
        private KeypadOperation _keypadOperation;
        private OrderLayout _orderLayout;
        public Settings()
        {
            GroupType = GroupType.ServingCourse;
            OrderInfoDisplay = OrderInfoDisplay.TableTabName;
            TerminalType = TerminalType.Kitchen;
            OutputTime = OutputTime.OrderBumped;
            OutputScope = OutputScope.Order;
            KeypadOperation = KeypadOperation.BumpByLocation;
            OutputType = OutputType.None;
            OrderLayout = OrderLayout.TwoByFour;
            FirstWarningTime = 100;
            SecondWarningTime = 200;
            WebOrderTime = 20;
            RecallCount = 5;
            CmFontSize = 15;
            ItemFontFamily = "Tahoma";
            GroupFontSize = 18;
            GroupFontFamily = "Tahoma";
            GroupFontBold = true;
            OrderHeaderFontSize = 15;
            OrderFontFamily = "Tahoma";            
            AnalysisFontSize = 18;            
            AnalysisFontFamily = "Tahoma";
            NewOrderColor = "#FFB4C7B4";
            FirstWarningColor = "#FFF1DAB4";
            SecondWarningColor = "#FFF4B4B4";
            HeaderBackgroundColor = "#F09422";
            HeaderForegroundColor = "#FFFFFF";
            ItemSummaryFontFamily = "Tahoma";
            ItemSummaryFontSize = 28;
            ItemSummaryBackgroundColor = "#C4D3C4"; 
        }
        public Settings(TerminalType terminalType, string dbAddress, string dbPath, string terminalAddress, string terminalDisplayName)
            : this()
        {
            TerminalType = terminalType;
            DbIpAddress = dbAddress;
            DbPath = dbPath;
            TerminalIpAddress = terminalAddress;
            DisplayName = terminalDisplayName;
        }
        public bool AutoHide
        {
            get { return _autoHide; }
            set
            {
                _autoHide = value;
                OnPropertyChanged("AutoHide");
            }
        }
        public bool BumpOnly
        {
            get { return _bumpOnly; }
            set
            {
                _bumpOnly = value;
                OnPropertyChanged("BumpOnly");
            }
        }
        public bool ConfirmOnRefund
        {
            get { return _confirmOnRefund; }
            set
            {
                _confirmOnRefund = value;
                OnPropertyChanged("ConfirmOnRefund");
            }
        }
        public bool ConfirmOnTransfer
        {
            get { return _confirmOnTransfer; }
            set
            {
                _confirmOnTransfer = value;
                OnPropertyChanged("ConfirmOnTransfer");
            }
        }
        public bool GroupFontBold
        {
            get { return _groupFontBold; }
            set
            {
                _groupFontBold = value;
                OnPropertyChanged("GroupFontBold");
            }
        }
        public bool AnalysisFontBold
        {
            get { return _analysisFontBold; }
            set
            {
                _analysisFontBold = value;
                OnPropertyChanged("AnalysisFontBold");
            }
        }
        public int FirstWarningTime
        {
            get { return _firstWarningTime; }
            set
            {
                _firstWarningTime = value;
                OnPropertyChanged("FirstWarningTime");
            }
        }
        public int SecondWarningTime
        {
            get { return _secondWarningTime; }
            set
            {
                _secondWarningTime = value;
                OnPropertyChanged("SecondWarningTime");
            }
        }
        public int RecallCount
        {
            get { return _recallCount; }
            set
            {
                _recallCount = value;
                OnPropertyChanged("RecallCount");
            }
        }
        public int CmFontSize
        {
            get { return _cmFontSize; }
            set
            {
                _cmFontSize = value;
                OnPropertyChanged("CmFontSize");
            }
        }
        public int WebOrderTime
        {
            get { return _webOrderTime; }
            set
            {
                _webOrderTime = value;
                OnPropertyChanged("WebOrderTime");
            }
        }
        public int OutputTerminal
        {
            get { return _outputTerminal; }
            set
            {
                _outputTerminal = value;
                OnPropertyChanged("OutputTerminal");
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
        public string DbIpAddress
        {
            get { return _dbIpAddress; }
            set
            {
                _dbIpAddress = value;
                OnPropertyChanged("DbIpAddress");
            }
        }
        public string DbPath
        {
            get { return _dbPath; }
            set
            {
                _dbPath = value;
                OnPropertyChanged("DbPath");
            }
        }
        public string TerminalIpAddress
        {
            get { return _terminalIpAddress; }
            set
            {
                _terminalIpAddress = value;
                OnPropertyChanged("TerminalIpAddress");
            }
        }
        public string NewOrderMmSound
        {
            get { return _newOrderMmSound; }
            set
            {
                _newOrderMmSound = value;
                OnPropertyChanged("NewOrderMmSound");
            }
        }
        public string CancelNoticeMmSound
        {
            get { return _cancelNoticeMmSound; }
            set
            {
                _cancelNoticeMmSound = value;
                OnPropertyChanged("CancelNoticeMmSound");
            }
        }
        public string RefundNoticeMmSound
        {
            get { return _refundNoticeMmSound; }
            set
            {
                _refundNoticeMmSound = value;
                OnPropertyChanged("RefundNoticeMmSound");
            }
        }
        public string TransferNoticeMmSound
        {
            get { return _transferNoticeMmSound; }
            set
            {
                _transferNoticeMmSound = value;
                OnPropertyChanged("TransferNoticeMmSound");
            }
        }
        public string OrderArrivedCmSound
        {
            get { return _orderArrivedCmSound; }
            set
            {
                _orderArrivedCmSound = value;
                OnPropertyChanged("NewOrderMmSound");
            }
        }
        public string TransferOrderCmSound
        {
            get { return _transferOrderCmSound; }
            set
            {
                _transferOrderCmSound = value;
                OnPropertyChanged("NewOrderMmSound");
            }
        }
        public string DisplayName
        {
            get { return _displayName; }
            set
            {
                _displayName = value;
                OnPropertyChanged("DisplayName");
            }
        }
        public string GroupFontFamily
        {
            get { return _groupFontFamily; }
            set
            {
                _groupFontFamily = value;
                OnPropertyChanged("GroupFontFamily");
            }
        }
        public string ItemFontFamily
        {
            get { return _itemFontFamily; }
            set
            {
                _itemFontFamily = value;
                OnPropertyChanged("ItemFontFamily");
            }
        }
        public string AnalysisFontFamily
        {
            get { return _analysisFontFamily; }
            set
            {
                _analysisFontFamily = value;
                OnPropertyChanged("AnalysisFontFamily");
            }
        }
        public string OrderFontFamily
        {
            get { return _orderFontFamily; }
            set
            {
                _orderFontFamily = value;
                OnPropertyChanged("OrderFontFamily");
            }
        }
        public string FirstWarningColor
        {
            get { return _firstWarningColor; }
            set
            {
                _firstWarningColor = value;
                OnPropertyChanged("FirstWarningColor");
            }
        }
        public string SecondWarningColor
        {
            get { return _secondWarningColor; }
            set
            {
                _secondWarningColor = value;
                OnPropertyChanged("SecondWarningColor");
            }
        }
        public string NewOrderColor
        {
            get { return _newOrderColor; }
            set
            {
                _newOrderColor = value;
                OnPropertyChanged("NewOrderColor");
            }
        }
        public string HeaderBackgroundColor
        {
            get { return _headerBackgroundColor; }
            set
            {
                _headerBackgroundColor = value;
                OnPropertyChanged("HeaderBackgroundColor");
            }
        }
        public string HeaderForegroundColor
        {
            get { return _headerForegroundColor; }
            set
            {
                _headerForegroundColor = value;
                OnPropertyChanged("HeaderForegroundColor");
            }
        }
        public OutputTime OutputTime
        {
            get { return _outputTime; }
            set
            {
                _outputTime = value;
                OnPropertyChanged("OutputTime");
                UpdateOutputScope(_outputTime);
            }
        }
        public OutputScope OutputScope
        {
            get { return _outputScope; }
            set
            {
                _outputScope = value;
                OnPropertyChanged("OutputScope");
            }
        }
        public KeypadOperation KeypadOperation
        {
            get { return _keypadOperation; }
            set
            {
                _keypadOperation = value;
                OnPropertyChanged("KeypadOperation");
            }
        }
        public TerminalType TerminalType
        {
            get { return _terminalType; }
            set
            {
                _terminalType = value;
                OnPropertyChanged("TerminalType");
            }
        }
        public GroupType GroupType
        {
            get { return _groupType; }
            set
            {
                _groupType = value;
                OnPropertyChanged("GroupType");
            }
        }
        public OrderInfoDisplay OrderInfoDisplay
        {
            get { return _orderInfoDisplay; }
            set
            {
                _orderInfoDisplay = value;
                OnPropertyChanged("OrderInfoDisplay");
            }
        }
        public OutputType OutputType { get; set; }
        public OrderLayout OrderLayout
        {
            get { return _orderLayout; }
            set
            {
                _orderLayout = value;
                OnPropertyChanged("OrderLayout");
            }
        }
        public string ItemSummaryBackgroundColor
        {
            get { return _itemSummaryBackgroundColor; }
            set
            {
                _itemSummaryBackgroundColor = value;
                OnPropertyChanged("ItemSummaryBackgroundColor");
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
        public string ItemSummaryFontFamily
        {
            get { return _itemSummaryFontFamily; }
            set
            {
                _itemSummaryFontFamily = value;
                OnPropertyChanged("ItemSummaryFontFamily");
            }
        }
        private void UpdateOutputScope(OutputTime outputTime)
        {
            switch (outputTime)
            {
                case OutputTime.Immediately:
                case OutputTime.OrderBumped:
                    OutputScope = OutputScope.Order;
                    break;
                case OutputTime.GroupBumped:
                    OutputScope = OutputScope.Group;
                    break;
                case OutputTime.ItemBumped:
                    OutputScope = OutputScope.Item;
                    break;
            }
        }
        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }
}