using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using NLog;

namespace IntaMate
{    

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
            /* return an Ack so that Stub comes online */
            byte[] buffer = new byte[1];
            buffer[0] = ProtocolUtility.ACK;
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
