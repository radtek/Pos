using System;
using NLog;
using System.IO.Ports;

namespace IntaMate
{
    class IO_RS232 : IntaMate.I_IO_Interface
    {
        protected static Logger logger = LogManager.GetCurrentClassLogger();
        SerialPort Serial = new SerialPort();

        public IO_RS232(int baud, int readTimeout, int writeTimeout)
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

        public void Write(byte[] Data, int Index, int ByteCount)
        {
            Serial.Write(Data, Index, ByteCount);
        }

        public byte ReadByte()
        {
            return (byte)Serial.ReadByte();
        }

        public int Read(byte[] Data, int Index, int ByteCount)
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
}