using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ChefMate.Communication.Tcp
{
    /// <summary>
    /// Information required to send a Stream across the network
    /// </summary>
    public class TcpStreamMessage
    {
        private IPAddress _ipAddress;
        private int _portNumber;
        private MemoryStream _dataStream;
        private TcpStreamMessageType _messageType;

        private const int DefaultIpPort = 56100;
        private const string DefaultIpAddress = "127.0.0.1";

        public TcpStreamMessage(string inIpAddress = DefaultIpAddress, int inPortNumber = DefaultIpPort, TcpStreamMessageType inMessageType = TcpStreamMessageType.None)
        {
            IPAddress.TryParse(inIpAddress, out _ipAddress);
            _portNumber = inPortNumber;
            _messageType = inMessageType;
            this._dataStream = new MemoryStream();
        }

        public TcpStreamMessage(IPAddress inIpAddress, int inPortNumber = DefaultIpPort, TcpStreamMessageType inMessageType = TcpStreamMessageType.None)
        {
            this._ipAddress = inIpAddress;
            this._portNumber = inPortNumber;
            this._dataStream = new MemoryStream();
            this._messageType = inMessageType;
        }

        #region PUBLIC

        public void AddData(String dataString)
        {
            byte[] data = Encoding.ASCII.GetBytes(dataString);
            this._dataStream.Write(data, 0, data.Length);
        }
        public void AddData(int dataInt)
        {
            byte[] data = BitConverter.GetBytes(dataInt);
            this._dataStream.Write(data, 0, data.Length);
        }
        public void SetIpAddressFromString(string ipAddress)
        {
            IPAddress.TryParse(ipAddress, out this._ipAddress);
        }
        public string GetMessageTypeString()
        {
            return Convert.ToString(_messageType);
        }
        public IPAddress IpAddress
        {
            get { return this._ipAddress; }
            set { this._ipAddress = value; }
        }
        public int PortNumber
        {
            get { return _portNumber; }
            set { PortNumber = value; }
        }
        public MemoryStream DataStream
        {
            get { return _dataStream; }
            set { _dataStream = value; }
        }
        public TcpStreamMessageType MessageType
        {
            get { return _messageType; }
            set { _messageType = value; }
        }

        #endregion


    }
}
