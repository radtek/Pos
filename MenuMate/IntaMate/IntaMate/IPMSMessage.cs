using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using NLog;

namespace IntaMate
{
    public interface IPMSMessage
    {
        byte[] ToByteArray();
    }

    public abstract class PMSMessage : IPMSMessage
    {
        protected static Logger logger = LogManager.GetCurrentClassLogger();

        protected ProtocolUtility _protocolUtility = new ProtocolUtility();
        protected String _messageType = "";

        protected const int ESTABISHMENT_LENGTH = 14;
        protected const int STATION_LENGTH = 4;
        protected const int PMSREF_LENGTH = 14;
        protected const int DOLLARS_AMOUNT_LENGTH = 8;
        protected const int CENTS_AMOUNT_LENGTH = 2;
        protected const int FOILO_LENGTH = 16;
        protected const int COVERS_LENGTH = 4;

        abstract public byte[] ToByteArray();
    }
}
