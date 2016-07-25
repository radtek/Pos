using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Diagnostics;
using NLog;
using System.IO;

namespace IntaMate
{
    class TCPServer
    {
        PMSInterfaceFactory PMSFactory;
        Thread thread;

        private static Logger logger = LogManager.GetLogger("IntaMate");

        private readonly int port;
        private readonly IPAddress serverAddress;
        private TcpListener tcpListener;
        public bool Active = false;

        public TCPServer(IPAddress inserverAddress, int inport, PMSInterfaceFactory inAPI)
        {
            serverAddress = inserverAddress;
            port = inport;
            PMSFactory = inAPI;
        }

        ~TCPServer()
        {
            Stop();
        }

        public bool Start() 
        {           
            try 
            {
                if (thread != null && thread.IsAlive)
                {
                    Stop();
                }
                tcpListener = new TcpListener(serverAddress, port);			
                tcpListener.Start();

                logger.Info("Server Ready - Listening for new connections ...");
                thread = new Thread(new ThreadStart(StartListen));
                thread.Start();
                Active = true;
            }
            catch(Exception e) 
            {
                logger.Fatal("Error while listening :" + e.ToString());
                Active = false;
            }
            return Active;
        }

        /// <summary>Stop server listening</summary>
        public void Stop() 
        {
            if (thread != null)
            {
                thread.Abort();
            }
            if (tcpListener != null)
            {
                tcpListener.Stop();
            }
        }
         
        /// <summary>
        /// Wait for client connections and start new communication thread
        /// </summary>
        private void StartListen() 
        {
            try 
            {
                while(thread.ThreadState == System.Threading.ThreadState.Running) 
                {		
                    // AcceptSocket blocks until new connection has established
                    Socket socket = tcpListener.AcceptSocket();					
                    socket.Blocking = true; // Blocks until a operation has completed	
                    
                    if(socket.Connected) 
                    {
                        logger.Info("Client connected: {0}" + socket.RemoteEndPoint);

                        /* Serilisation wants to be done right here at the TCPIP level.
                               * The Server will only talk with one client at once.
                               * Its starts a blocking listen socket in another thread so the
                               * application remains responsive.
                        * Use the following lines if the interface is thread safe, in our
                        * case it isnt becasue we have to que up for hte serial port anyway.
                        * 
                        * PhoenixProcessor PhoenixProcessor = new PhoenixProcessor(socket);
                        * Thread thread = new Thread(new ThreadStart(PhoenixProcessor.Process)); 
                        * thread.Start();					
                        */
                        // Read in and examine the Data for Phoenix or XML Data.
                        NetworkStream networkStream = new NetworkStream(socket);
                        StreamReader streamReader = new StreamReader(networkStream);                       
                        try
                        {                          
                            string Data = string.Empty;

                            // While not at the end of the file, read lines from the file.
                            while (streamReader.Peek() > -1)
                            {
                                Data += streamReader.ReadLine();
                            }

							if(Data.Length > 0)
							{
								switch (Data[0])
	                            {
	                                case '\\':
	                                    PhoenixProcessor PhoenixProcessor = new PhoenixProcessor(networkStream, PMSFactory.PMSInterface);
	                                    PhoenixProcessor.Process(Data);
	                                    break;
	                                case '<':
	                                    XMLProcessor XMLProcessor = new XMLProcessor(networkStream);
	                                    //XMLProcessor.Process(Data, );
	                                    break;
	                            }
							}
                        }
                        finally
                        {
                            streamReader.Close();
                            networkStream.Close();
                            socket.Close();
                        }

                        // Create new SMTPProcessor and start in new thread:
                        // Abstract this processor out into a base class for expansion.  
                    }				
                }
            }
            catch (SocketException ex) {
              // An exception will be thrown when tcpListerner.Stop() is called	
              logger.Fatal("SocketException: {0}" + ex.Message);
            }		
        }        
    }
}
