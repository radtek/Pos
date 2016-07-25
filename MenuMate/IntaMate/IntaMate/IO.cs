using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Net.Sockets;
using System.Diagnostics;
using NLog;

namespace IntaMate
{
    
    interface I_IO_Interface
    {
        void DiscardInBuffer();
        void Write(byte[] Data,int Index,int ByteCount);
        byte ReadByte();
        int Read(byte[] Data,int Index,int ByteCount);
        void Close();
        bool Connected();
    }
    
    
    class IO_RS232 : IntaMate.I_IO_Interface 
    {
        protected static Logger logger = LogManager.GetCurrentClassLogger();
        SerialPort Serial = new SerialPort();
        
        public IO_RS232(int baud,int readTimeout, int writeTimeout)
        {
            Serial.Close();
            Serial.BaudRate = baud;
            Serial.DataBits = 8;
            Serial.Parity = Parity.None;
            Serial.StopBits = StopBits.One;
            Serial.ReadTimeout = readTimeout;
            Serial.WriteTimeout = writeTimeout;
            Serial.Handshake = Handshake.RequestToSend;
            Serial.PortName = IntaMate.Properties.Settings.Default.ComPort;
        }

        public void DiscardInBuffer()
        {
            Serial.DiscardInBuffer();    
        }

        public void Write(byte[] Data,int Index,int ByteCount)
        {
            Serial.Write(Data, Index, ByteCount);
        }

        public byte ReadByte()
        {
            return (byte)Serial.ReadByte();
        }
        
        public int Read(byte[] Data,int Index,int ByteCount)
        {
            int Count = 0;
            try
            {
                Count = Serial.Read(Data, Index, ByteCount);
            }
            catch (System.TimeoutException)
            {
                Count = 0;
            }
            return Count;
        }

        public bool Connected()
        {
            bool Active = false;
            try
            {
                if (Serial.PortName != IntaMate.Properties.Settings.Default.ComPort)
                {
                    Serial.Close();
                    Serial.PortName = IntaMate.Properties.Settings.Default.ComPort;
                }

                DateTime TimeOut = DateTime.Now.AddSeconds(10);
                while (Serial.IsOpen == false && TimeOut > DateTime.Now)
                {
                    try
                    {
                        Serial.Open();
                    }
                    catch (System.Exception ex)
                    {
                        Active = false;
                        logger.Error("Error opening Serial Port: " + ex.ToString());
                    }
                }

                if (Serial.IsOpen)
                {
                    Active = true;
                }
                else if (TimeOut < DateTime.Now)
                {
                    Active = false;
                    throw new Exception("Time out opening Serial Port.");
                }
            }
            catch (System.Exception ex)
            {
                logger.Error("Micros Interface Error : " + ex.ToString());
                Active = false;
            }
            return Active;
        }
        
        public void Close()
        {
            Serial.Close();
        }
    }

    class IO_TCPIP : IntaMate.I_IO_Interface
    {
        protected static Logger logger = LogManager.GetCurrentClassLogger();        
        Socket IOSocket;

        private string IPAddress;
        private int IPPort;
        private int ReceiveTimeout;

        public IO_TCPIP(string inIPAddress, int inIPPort, int inReceiveTimeOut)
        {
            IPAddress           = inIPAddress;
            IPPort              = inIPPort;
            ReceiveTimeout      = inReceiveTimeOut;
        }

        public void DiscardInBuffer()
        {
            try
            {
                if (IOSocket.Available != 0)
                {
                    byte[] buffer = new byte[IOSocket.Available];
                    IOSocket.Receive(buffer);
                }
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                //Debug.WriteLine("\r" + ex.Message);
            }
        }

        public bool Connected() 
        {
            bool Active = false;

            if (IOSocket != null)
            {
                IOSocket.Close();
            }

            IOSocket = new Socket(System.Net.Sockets.AddressFamily.InterNetwork,
                                  System.Net.Sockets.SocketType.Stream,
                                  System.Net.Sockets.ProtocolType.Tcp);

            IOSocket.ReceiveTimeout = ReceiveTimeout;
            try
            {
                Active = false;
                DateTime TimeOut = DateTime.Now.AddSeconds(10);
                while (IOSocket.Connected == false && TimeOut > DateTime.Now)
                {
                    try
                    {
                        IOSocket.Connect(IPAddress,IPPort);
                    }
                    catch (System.Exception ex)
                    {
                        Active = false;
                        logger.Error("Error opening TCP Port: " + ex.ToString());
                    }
                }

                if (IOSocket.Connected)
                {
                    Active = true;
                }
                else if (TimeOut < DateTime.Now)
                {
                    Active = false;
                    logger.Error("Time out opening TCP Port.");
                    throw new Exception("Time out opening TCP Port.");
                }
            }
            catch (System.Exception ex)
            {
                logger.Error("Micros Interface Error : " + ex.ToString());                
                Active = false;
            }
            return Active;
        }

        public void Write(byte[] Data, int Index, int ByteCount)
        {
            if (IOSocket.Connected == false)
            {
                Connected();
            }

            IOSocket.Send(Data, Index, ByteCount, System.Net.Sockets.SocketFlags.None);
        }

        public byte ReadByte()
        {
            if (IOSocket.Connected == false)
            {
                Connected();
            }

            byte[] buffer = new byte[1];
            buffer[0] = 0;
            try
            {
                IOSocket.Receive(buffer, 1, System.Net.Sockets.SocketFlags.None);
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                logger.Error("Read Byte Error : " + ex.ToString());
            }
            return buffer[0];
        }

        public int Read(byte[] Data, int Index, int ByteCount)
        {
            if (IOSocket.Connected == false)
            {
                Connected();
            }

            int Count = 0;
            try
            {
                Count = IOSocket.Receive(Data, Index, ByteCount, System.Net.Sockets.SocketFlags.None);
            }
            catch (System.Net.Sockets.SocketException)
            {
                Count = 0;
            }
            return Count;
        }

        public void Close()
        {
            if (IOSocket.Connected)
            {
                IOSocket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
                IOSocket.Disconnect(true);
            }
        }
        
    }

    class IO_Stub : IntaMate.I_IO_Interface
    {
        protected static Logger logger = LogManager.GetCurrentClassLogger();
        public IO_Stub()
        {
        }

        public void DiscardInBuffer()
        {
        }

        public bool Connected()
        {
            bool Active = true;
            logger.Info("\rIO Stub Connected Ok");
            return Active;
        }

        public void Write(byte[] Data, int Index, int ByteCount)
        {
            logger.Info("\rIO Stub Write :" + Encoding.ASCII.GetString(Data,Index,ByteCount));
        }

        public byte ReadByte()
        {
            byte[] buffer = new byte[1];
            buffer[0] = 0;
            return buffer[0];
        }

        public int Read(byte[] Data, int Index, int ByteCount)
        {          
            int Count = 0;
            return Count;
        }

        public void Close()
        {
            logger.Info("\rInterface Stub Closed Ok");               
        }      
    }  
}
