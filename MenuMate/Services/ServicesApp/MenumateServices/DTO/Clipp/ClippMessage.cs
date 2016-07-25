using System;

namespace MenumateServices.DTO.Clipp
{
    public class ClippMessage
    {
        private string _messageType;

        public long Id { get; set; }
        public string MessageId { get; set; }

        public string MessageType
        {
            get { return _messageType; }
            set
            {
                _messageType = value;
                switch (MessageType)
                {
                    case "TAB_OPEN":
                        MessageTypeEnum = Clipp.ClippMessageType.TabOpen;
                        break;
                    case "TAB_CLOSE":
                        MessageTypeEnum = Clipp.ClippMessageType.TabClose;
                        break;
                    case "TAB_UPDATE":
                        MessageTypeEnum = Clipp.ClippMessageType.TabUpdate;
                        break;
                    case "TAB_PAY":
                        MessageTypeEnum = Clipp.ClippMessageType.TabPay;
                        break;
                    case "TAB_STATUS":
                        MessageTypeEnum = Clipp.ClippMessageType.TabStatus;
                        break;
                    default:
                        MessageTypeEnum = Clipp.ClippMessageType.TabOpen;
                        break;
                }
            }
        }

        public ClippMessageType MessageTypeEnum { get; set; }

        public DateTime CreatedDate { get; set; }
        public ClippMessageDetail Data { get; set; }
    }
}