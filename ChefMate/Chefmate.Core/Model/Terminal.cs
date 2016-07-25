using Chefmate.Core.Enums;

namespace Chefmate.Core.Model
{
    public class Terminal : ModelBase
    {
        private int _termianlId;
        private string _terminalName;
        private string _terminalIpAddress;
        private TerminalType _terminalType;
        private string _displayName;

        public Terminal()
        {
            TerminalType = TerminalType.Kitchen;
        }

        public int TerminalId
        {
            get { return _termianlId; }
            set
            {
                _termianlId = value;
                OnPropertyChanged("TerminalId");
            }
        }
        public string TerminalName
        {
            get { return _terminalName; }
            set
            {
                _terminalName = value;
                OnPropertyChanged("TerminalName");
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
        public TerminalType TerminalType
        {
            get { return _terminalType; }
            set
            {
                _terminalType = value;
                OnPropertyChanged("TerminalType");
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
    }
}