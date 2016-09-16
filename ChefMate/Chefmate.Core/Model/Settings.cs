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
        private int _outputTerminal;
        private int _firstWarningTime;
        private int _secondWarningTime;
        private int _recallCount;
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
        private int _cmFontSize;
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
        public int OutputTerminal
        {
            get { return _outputTerminal; }
            set
            {
                _outputTerminal = value;
                OnPropertyChanged("OutputTerminal");
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