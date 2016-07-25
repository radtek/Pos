using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Collections;
using System.Net.NetworkInformation;


namespace PDServer
{
    public class TCPServerThreaded
    {
        private static TcpListener tcpListener;
        private static Thread Listenthread;
        private static Thread ThreadReclaim;

        private static ArrayList ClientSockets = new ArrayList();
        private static bool Halt = false;

        private readonly int port;
        private readonly IPAddress serverAddress;

        public String LogMsg = "";

        public TCPServerThreaded(IPAddress inserverAddress, int inport)
        {
            serverAddress = inserverAddress;
            port = inport;
        }

        ~TCPServerThreaded()
        {
            Stop();
        }

        public bool Start()
        {
            bool Active = false;
            Halt = false;
            try
            {
                LogMsg = "Starting Listner on " + serverAddress + ":" + port.ToString();
                Console.WriteLine(LogMsg);

                if (Listenthread != null && Listenthread.IsAlive)
                {
                    LogMsg = "TCPServerThreaded on " + serverAddress + ":" + port.ToString() + " Already Started, Restarting...";
                    Console.WriteLine(LogMsg);
                    Stop();
                }

                ThreadReclaim = new Thread(new ThreadStart(Reclaim));
                ThreadReclaim.Start();

                String Busy = "Port Busy, Can't Start till it's free, waiting... " + port.ToString();
                while (!CheckAvailableServerPort(port))
                {
                    Console.WriteLine(Busy);
                    Thread.Sleep(60000);
                }

                tcpListener = new TcpListener(serverAddress, port);
                Listenthread = new Thread(new ThreadStart(ListenForClients));
                Listenthread.Start();

                LogMsg = "TCPServerThreaded Started on " + serverAddress + ":" + port.ToString();
                Console.WriteLine(LogMsg);

                Active = true;
            }
            catch (Exception e)
            {
                LogMsg = "Error Starting TCPServerThreaded  " + serverAddress + ":" + port.ToString() + " " + e.ToString();
                Console.WriteLine(LogMsg);
            }
            return Active;
        }

        public bool Active()
        {
            return (Listenthread.ThreadState == System.Threading.ThreadState.Running);
        }

        /// <summary>Stop server listening</summary>
        public void Stop()
        {
            Halt = true;
            LogMsg = "Stopping TCPServerThreaded " + port.ToString();
            Console.WriteLine(LogMsg);

            if (tcpListener != null)
            {
                tcpListener.Stop();
            }

            LogMsg = "Joining Relcaim Thread " + port.ToString();
            Console.WriteLine(LogMsg);

            if (ThreadReclaim != null && !ThreadReclaim.Join(5000))
            {
                LogMsg = "Aboirting Relcaim Thread " + port.ToString();
                Console.WriteLine(LogMsg);
                ThreadReclaim.Abort();
            }

            LogMsg = "Joining Listner Thread " + port.ToString();
            Console.WriteLine(LogMsg);

            if (Listenthread != null && !Listenthread.Join(5000))
            {
                LogMsg = "Aboirting Listner Thread " + port.ToString();
                Console.WriteLine(LogMsg);

                Listenthread.Abort();
            }

            foreach (Object Client in ClientSockets)
            {
                LogMsg = "Stopping Clients " + port.ToString();
                Console.WriteLine(LogMsg);
                ((ClientHandler)Client).Stop();
            }

            LogMsg = "TCPServerThreaded Stop Complete " + port.ToString();
            Console.WriteLine(LogMsg);
        }

        private bool CheckAvailableServerPort(int port)
        {
            bool isAvailable = true;

            // Evaluate current system tcp connections. This is the same information provided
            // by the netstat command line application, just in .Net strongly-typed object
            // form.  We will look through the list, and if our port we would like to use
            // in our TcpClient is occupied, we will set isAvailable to false.
            IPGlobalProperties ipGlobalProperties = IPGlobalProperties.GetIPGlobalProperties();
            IPEndPoint[] tcpConnInfoArray = ipGlobalProperties.GetActiveTcpListeners();

            foreach (IPEndPoint endpoint in tcpConnInfoArray)
            {
                if (endpoint.Port == port)
                {
                    isAvailable = false;
                    break;
                }
            }
            return isAvailable;
        }

        /// <summary>
        /// Wait for client connections and start new communication thread
        /// </summary>
        private void ListenForClients()
        {
            try
            {
                tcpListener.Start();
                while (Listenthread.ThreadState == System.Threading.ThreadState.Running && !Halt)
                {
                    try
                    {
                        // AcceptSocket blocks until new connection has established
                        LogMsg = "Thread Listening on " + serverAddress + ":" + port.ToString();
                        Console.WriteLine(LogMsg);
                        TcpClient client = tcpListener.AcceptTcpClient();

                        LogMsg = "Client connected : " + client.Client.RemoteEndPoint;
                        Console.WriteLine(LogMsg);
                        using (TimedLock.Lock(ClientSockets.SyncRoot))
                        {
                            int i = ClientSockets.Add(new ClientHandler(client));
                            ((ClientHandler)ClientSockets[i]).Start();
                        }
                    }
                    catch (SystemException ex)
                    {
                        // An exception will be thrown when tcpListerner.Stop() is called	
                        LogMsg = "Listen Thread SystemException: " + port.ToString() + " " + ex.Message;
                        Console.WriteLine(LogMsg);
                    }
                }
            }
            finally
            {
                LogMsg = "Server Listen Thread EXITED! " + port.ToString();
                Console.WriteLine(LogMsg);
            }
        }

        private static void Reclaim()
        {
            try
            {
                while (!Halt)
                {
                    using (TimedLock.Lock(ClientSockets.SyncRoot))
                    {
                        for (int x = ClientSockets.Count - 1; x >= 0; x--)
                        {
                            Object Client = ClientSockets[x];
                            if (!((ClientHandler)Client).Alive)
                            {
                                ClientSockets.Remove(Client);
                                Console.WriteLine("Client Thread Removed");
                            }
                        }
                    }
                    Thread.Sleep(200);
                }
                String LogMsg = "Reclaim Thread Exited ";
                Console.WriteLine(LogMsg);
            }
            catch (SystemException ex)
            {
                // An exception will be thrown when tcpListerner.Stop() is called	
                string LogMsg = "Reclaim Thread SystemException: " + ex.Message;
                Console.WriteLine(LogMsg);
            }
        }
    }
}
