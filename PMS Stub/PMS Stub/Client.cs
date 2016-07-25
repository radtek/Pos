using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Xml;
using System.Diagnostics;
using HDUtil;
using NLog;

namespace PDServer
{
    public class ClientHandler
    {

        public static String PDLogger = "PMSLogs";
        private static Logger logger = LogManager.GetLogger(PDLogger);

        private TcpClient client = null;
        Thread ClientThread = null;
        UtilByteArray outboundACk = new UtilByteArray();

        public ClientHandler(TcpClient Client)
        {
            this.client = Client;
        }

        public void Start()
        {
            try
            {
                // 4,<Len>,<RegNo>,<RoomNo>,<NameNo>,<Text> (,<Text>)                
                ClientThread = new Thread(new ThreadStart(Process));
                ClientThread.Start();
            }
            catch (Exception e)
            {
                string LogMsg = "ClientHandler Exception :" + e.ToString();
                Console.WriteLine(LogMsg);
            }
        }

        private void Process()
        {
            String RemoteEndPoint = "";
            try
            {
                try
                {
                    Stopwatch s1 = Stopwatch.StartNew();

                    NetworkStream networkStream = client.GetStream();
                    RemoteEndPoint = client.Client.RemoteEndPoint.ToString();

                    UtilByteArray inbound = new UtilByteArray();
                    byte[] buffer = new byte[1024];
                    int n = -1;
                    networkStream.ReadTimeout = 2000;
                    networkStream.WriteTimeout = 2000;
                    // DateTime start = DateTime.Now;
                    // The Timeout is why the network transmission is slow.
                    // COnsider using and EOF char.

                    try
                    {
                        while (n != 0 || networkStream.DataAvailable)
                        {
                            n = networkStream.Read(buffer, 0, buffer.Length);
                            if (n > 0)
                            {
                                inbound.Append(buffer, n);
                            }
                        }
                    }
                    catch (Exception)
                    {
                    }

                    // Save input stream to File.
                    if (inbound.Count > 0)
                    { // Handhelds just connect with no data to test connection.
                        try
                        {
                            outboundACk.Clear();
                            string[] words = inbound.ToString().Split(',');
                            string Room = words[3].Trim();
                            switch (inbound[1])
                            {
                                case (byte)'7' :
                                   //  8,<Length>,<RegisterNo>,<RoomNo>,<Balance>,<CreditLimit>,<NoNames>,<Text>...,<Text>
                                    outboundACk.Append("\\8,0000,1-," + Room + ",10000,100000,3,Mr Brad, Mr Michael, Mr Jorge\r");
                                    break;
                                default :
                                    outboundACk.Append("\\4,0021,1-,"+ Room +",1,ACK\r");
                                break;
                            }

                            SendResult(outboundACk, networkStream);
                        }
                        catch (Exception e)
                        {
                        }
                        string LogMsg1 = "Client Processed : " + RemoteEndPoint;
                        Console.WriteLine(LogMsg1);
                        Console.WriteLine(inbound.ToCharArray());
                        logger.Info(">>: " + inbound.ToString());
                        logger.Info("<<: " + outboundACk.ToString());
                        logger.Info("------------------------------------");
                    }
                    s1.Stop();
                }
                catch (Exception e)
                {
                    string LogMsg = "ClientHandler Excaption :" + e.ToString();
                    Console.WriteLine(LogMsg);
                }
            }
            finally
            {
                string LogMsg = "Client Closed : " + RemoteEndPoint;
                Console.WriteLine(LogMsg);
                client.Close();
            }
        }  // Process()

        private void SendResult(UtilByteArray data, NetworkStream NetStream)
        {
            try
            {
                if (data.Count > 0)
                {
                    NetStream.Write(data.ToArray(), 0, data.ToArray().Length);
                }
            }
            catch (SystemException ex)
            {
                // An exception will be thrown when tcpListerner.Stop() is called	
                string LogMsg = "Send Data SocketException: " + ex.Message;
                Console.WriteLine(LogMsg);
            }
        }

        public void Stop()
        {
            if (ClientThread != null && ClientThread.IsAlive)
            {
                if (!ClientThread.Join(5000))
                {
                    string LogMsg = "Aborting Client Thread";
                    Console.WriteLine(LogMsg);
                    ClientThread.Abort();
                }
            }
        }

        public bool Alive
        {
            get
            {
                return (ClientThread != null && ClientThread.IsAlive);
            }
        }

    } // class ClientHandler 	
}