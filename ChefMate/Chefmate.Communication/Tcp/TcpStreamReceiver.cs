using System;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.IO;
using Chefmate.Communication.EventArguments;
using Chefmate.Logger;
using ChefMate.Communication.Receiver;

namespace ChefMate.Communication.Tcp
{
    public class TcpStreamReceiver : IStreamReceiver
    {
        public event StreamReceivedEventHandler StreamReceived;

        public TcpStreamReceiver(int inIpPort)
        {
            InitReceiver(inIpPort);
        }

        ~TcpStreamReceiver()
        {
            Stop();
        }

        public int IPPort
        {
            get { return _ipPort; }
        }
        public bool Stopped
        {
            get { return _receiverStopped; }
        }

        public void Start()
        {
            try
            {
                if (_receiverStopped)
                {
                    _tcpListener.Start();
                    _tcpThread = new Thread(new ThreadStart(AcceptClients));
                    _tcpThread.Start();

                    _receiverStopped = false;

                    LogEvent(@"Start()", @"Started to listen to port");
                }
            }
            catch (Exception exc)
            {
                //Bugsense.WPF.BugSense.SendException(exc);
                LogError(@"Start()", exc.Message);
            }
        }
        public void Stop()
        {
            try
            {
                if (!_receiverStopped)
                {
                    _tcpListener.Stop();
                    _tcpThread.Abort();
                    _tcpThread.Join();
                    _tcpThread = null;

                    _receiverStopped = true;

                    LogEvent(@"Stop()", @"Stopped listening to port");
                }
            }
            catch (Exception exc)
            {
                //Bugsense.WPF.BugSense.SendException(exc);
                LogError(@"Stop()", exc.Message);
            }
        }


        #region Private

        private bool _receiverStopped = true;
        private int _ipPort;
        private TcpListener _tcpListener;
        private Thread _tcpThread;

        private void InitReceiver(int inIpPort)
        {
            try
            {
                _ipPort = inIpPort;
                _tcpListener = new TcpListener(IPAddress.Any, _ipPort);
            }
            catch (Exception exc)
            {
                //Bugsense.WPF.BugSense.SendException(exc);
                LogError(@"initReceiver()", exc.Message);
            }
        }
        private void AcceptClients()
        {
            while (_tcpThread.ThreadState == ThreadState.Running)
            {
                Socket socket = null;
                string response = "";
                StreamWriter streamWriter = null;
                NetworkStream networkStream;

                try
                {
                    socket = _tcpListener.AcceptSocket();
                    if (socket.Connected)
                    {
                        MemoryStream stream = ReadStreamFromSocket(socket);
                        response = NotifyStreamReceived(stream);
                        networkStream = new NetworkStream(socket);
                        streamWriter = new StreamWriter(networkStream);
                        sendResponseBack(streamWriter, response);
                    }
                }
                catch (Exception exc)
                {
                    LogError(@"AcceptClients()", exc.Message);
                    if ((socket != null) && (socket.Connected))
                    {
                        try
                        {
                            networkStream = new NetworkStream(socket);  //todo: refactor to a method
                            streamWriter = new StreamWriter(networkStream);
                            sendResponseBack(streamWriter, response);
                        }
                        catch
                        {
                        }
                    }
                }
            }

            LogEvent(@"AcceptClients()", string.Format(@"Accepted client connections", _ipPort));
        }
        private MemoryStream ReadStreamFromSocket(Socket inSocket)
        {
            MemoryStream result = new MemoryStream();
            try
            {
                NetworkStream networkStream = new NetworkStream(inSocket);

                StringBuilder data = new StringBuilder();
                Int32 numberOfBytesRead = 0;

                System.Threading.Thread.Sleep(250);

                if (networkStream.CanRead)
                {
                    Int32 dataSize = ReadStreamSize(networkStream);
                    LogEvent(@"readStreamFromSocket()", string.Format(@"Network steam data size = {0}", dataSize));
                    byte[] readBuffer = new byte[dataSize];
                    do
                    {
                        try
                        {
                            numberOfBytesRead = networkStream.Read(readBuffer, 0, readBuffer.Length);
                        }
                        catch (Exception ex)
                        {
                            ChefmateLogger.Instance.LogError("readStreamFromSocket()", ex.Message);
                        }
                        result.Write(readBuffer, 0, numberOfBytesRead);
                    }
                    while (networkStream.DataAvailable);

                    LogEvent(@"readStreamFromSocket()", string.Format(@"TCP Server has read a data stream", _ipPort));
                }
            }
            catch (Exception exc)
            {
                LogError(@"readStreamFromSocket()", exc.Message);
            }

           return result;
        }
        private Int32 ReadStreamSize(NetworkStream inNetworkStream)
        {
            Int32 streamSize = 0;

            byte[] readBuffer = new byte[sizeof(Int32)];

            Int32 numberOfBytesRead = inNetworkStream.Read(readBuffer, 0, readBuffer.Length);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(readBuffer);

            streamSize = BitConverter.ToInt32(readBuffer, 0);

            return streamSize;
        }
        private MemoryStream DataToStream(StringBuilder inData)
        {
            MemoryStream result = new MemoryStream();
            StreamWriter sw = new StreamWriter(result, Encoding.ASCII);
            sw.WriteLine(inData.ToString());
            sw.Flush();
            return result;
        }
        private string NotifyStreamReceived(MemoryStream inStream)
        {
            string result = @"";
            if (StreamReceived != null)
            {
                StreamReceiverEventArgs streamArgs = new StreamReceiverEventArgs(inStream);
                StreamReceived(this, streamArgs);
                result = streamArgs.Response;
            }
            return result;
        }
        private void sendResponseBack(StreamWriter inStreamWriter, string inResponse)
        {
            byte[] bytes = BitConverter.GetBytes((int)inResponse.Length);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            char[] responseSize = Encoding.UTF8.GetChars(bytes, 0, 4);

            inStreamWriter.Write(responseSize, 0, 4);
            inStreamWriter.WriteLine(inResponse);
        }
        void LogEvent(string inFuncName, string inEventDescription)
        {
            string functName = string.Format(@"ChefMate.Communication.Tcp.TcpStreamReceiver.{0}", inFuncName);
            string text = string.Format(@"IP PORT: {0} | {1}", _ipPort, inEventDescription);
            ChefmateLogger.Instance.LogEvent(functName, text);
        }
        void LogError(string inFuncName, string inMsg)
        {
            string functName = string.Format(@"ChefMate.Communication.Tcp.TcpStreamReceiver.{0}", inFuncName);
            string text = string.Format(@"IP PORT: {0} | {1}", _ipPort, inMsg);
            ChefmateLogger.Instance.LogError(functName, text);
        }

        #endregion
    }
}
