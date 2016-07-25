using System;
using System.IO;
using Chefmate.Communication.EventArguments;
using ChefMate.Communication.Tcp;

namespace ChefMate.Communication.Receiver
{
    public class StreamConverterChefmate : StreamConverter
    {
        const int PosIpPort = 56100;
        public delegate void DbOrderReceivedHandler(object sender, DbOrderReceivedEventArgs e);
        public event DbOrderReceivedHandler DbOrderReceived;
        public StreamConverterChefmate()
            : base()
        {
        }
        ~StreamConverterChefmate()
        {
        }

        protected override void OpenReceiver()
        {
            this.StreamReceiver = new TcpStreamReceiver(PosIpPort);
        }
        protected override void OnDataReceived(object sender, StreamReceiverEventArgs streamArgs)
        {
            TcpStreamMessage streamMessage = new TcpStreamMessage();

            // create a copy of stream data
            MemoryStream stream = streamArgs.Stream;

            // decode the stream into data
            DecodeStreamToMessage(ref stream, ref streamMessage);

            if (streamMessage.MessageType != TcpStreamMessageType.None)
            {
                streamArgs.Response = GetResponseMessage(streamMessage.MessageType);
                RaiseMessageRecievedEvents(streamMessage);
            }
        }

        private void DecodeStreamToMessage(ref MemoryStream stream, ref TcpStreamMessage streamMessage)
        {
            try
            {
                stream.Position = 0;

                // it is essential to read the message type first and then the data in that order.
                // as we encode them in that order when we send the data
                SetMessageTypeFromStream(ref stream, ref streamMessage);
                SetMessageDataFromStream(ref stream, ref streamMessage);
            }
            catch
            {
                streamMessage.MessageType = TcpStreamMessageType.None;
                streamMessage.DataStream.Close();

                // todo: log exceptions
            }
        }

        private void SetMessageTypeFromStream(ref MemoryStream stream, ref TcpStreamMessage streamMessage)
        {
            int intMessageType = 0;

            streamMessage.MessageType = GetInt32FromStream(stream, out intMessageType)
                                            ? (TcpStreamMessageType)intMessageType
                                            : TcpStreamMessageType.None;
        }

        private void SetMessageDataFromStream(ref MemoryStream stream, ref TcpStreamMessage streamMessage)
        {
            // copy the rest of the data to the message's data stream
            stream.CopyTo(streamMessage.DataStream);

            streamMessage.DataStream.Position = 0;

            if (!ValidateMessageData(ref streamMessage))
            {
                throw new Exception("Message data stream is invalid");
            }
        }

        private bool ValidateMessageData(ref TcpStreamMessage streamMessage)
        {
            bool result = false;

            // validation 1 : check whether the stream contains an integer for db key
            result = IsMessageContainsDbKey(ref streamMessage);

            // reset the data stream position to begining
            streamMessage.DataStream.Position = 0;

            return result;
        }

        bool IsMessageContainsDbKey(ref TcpStreamMessage streamMessage)
        {
            bool result = false;

            int value = 0;
            result = GetInt32FromStream(streamMessage.DataStream, out value);

            return result;
        }

        private string GetResponseMessage(TcpStreamMessageType messageType)
        {
            string msg = "";

            switch (messageType)
            {
                case TcpStreamMessageType.Order:
                    //msg = "3,T,Success";
                    break;
                case TcpStreamMessageType.Group:
                    //msg = "5,T,Success";
                    break;
                case TcpStreamMessageType.Item:
                    //msg = "7,T,Success";
                    break;
                default:
                    break;
            }

            // at the implementation of this class, the response message was no longer required. 
            // Keeping the old reponse messages in the comments, just in case if we need them
            return msg;
        }

        private void RaiseMessageRecievedEvents(TcpStreamMessage streamMessage)
        {
            int dbKey = 0;
            GetInt32FromStream(streamMessage.DataStream, out dbKey);

            switch (streamMessage.MessageType)
            {
                case TcpStreamMessageType.Order:
                    RaiseDbMessageReceivedEvent(DbMessageType.Order, dbKey);
                    break;
                case TcpStreamMessageType.Group:
                    RaiseDbMessageReceivedEvent(DbMessageType.Group, dbKey);
                    break;
                case TcpStreamMessageType.Item:
                    RaiseDbMessageReceivedEvent(DbMessageType.Item, dbKey);
                    break;
                default:
                    break;
            }
        }

        private void RaiseDbMessageReceivedEvent(DbMessageType inMessageType, int inDbKey)
        {
            if (DbOrderReceived != null)
            {
                DbOrderReceivedEventArgs dbOrderReceivedEventArgs = new DbOrderReceivedEventArgs(inMessageType, inDbKey);
                DbOrderReceived(this, dbOrderReceivedEventArgs);
            }
        }

        private bool GetInt32FromStream(MemoryStream stream, out int value)
        {
            bool result = false;

            try
            {
                // get the size of an integer
                int integerSize = sizeof(Int32);

                // create the buffer to hold decoded data
                byte[] integerBytes = new byte[integerSize];

                // read the bytes to the buffer
                stream.Read(integerBytes, 0, integerSize);

                if (BitConverter.IsLittleEndian)
                    Array.Reverse(integerBytes);

                // convert the bytes to integer
                value = BitConverter.ToInt32(integerBytes, 0);

                result = true;
            }
            catch
            {
                value = 0;
                result = false;
            }

            return result;
        }
    }
}
