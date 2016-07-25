using System;
using ChefMate.Communication.Receiver;

namespace Chefmate.Communication.EventArguments
{
    public class DbOrderReceivedEventArgs : EventArgs
    {
        public DbMessageType MessageType { get; private set; }
        public int Key { get; private set; }

        public DbOrderReceivedEventArgs(DbMessageType inMessageType, int inKey)
        {
            MessageType = inMessageType;
            Key = inKey;
        }
    }
}
