using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;

namespace ChefMate.Communication.Tcp
{
    public class TcpStreamSender
    {
        public TcpStreamSender()
        {

        }

        /// <summary>
        /// sends a tcp stream message
        /// </summary>
        /// <param name="message"></param>
        public bool SendMessage(TcpStreamMessage message)
        {
            bool success = true;

            try
            {
                TcpClient tcpClient = createTcpClient(message);
                NetworkStream networkStream = tcpClient.GetStream();

                writeMessageToStream(networkStream, message);

                networkStream.Close();
            }
            catch
            {
                //todo: log the exception
                success = false;
            }

            return success;
        }

        private TcpClient createTcpClient(TcpStreamMessage message)
        {
            return new TcpClient( message.IpAddress.ToString(), message.PortNumber);
        }

        private void writeMessageToStream(NetworkStream networkStream, TcpStreamMessage message)
        {
            byte[] dataToWrite = prepareMessageBytes(message);

            writeMessageLength(networkStream, dataToWrite.Length);
            writeMessageData(networkStream, dataToWrite);            
        }

        private byte[] prepareMessageBytes(TcpStreamMessage message)
        {
            MemoryStream dataStream = new MemoryStream();
            byte[] msgType = BitConverter.GetBytes(Convert.ToInt32(message.MessageType));

            setByteOrderForEndianMode(ref msgType);
            dataStream.Write(msgType, 0, msgType.Length);

            // copy the TcpStreamMessage data from the start position
            message.DataStream.Position = 0;
            byte[] msgData = message.DataStream.ToArray();

            setByteOrderForEndianMode(ref msgData);
            dataStream.Write(msgData, 0, msgData.Length);

            return dataStream.ToArray();
        }

        private void writeMessageLength(NetworkStream networkStream, int dataSize)
        {
            byte[] dataSizeBytes = BitConverter.GetBytes(dataSize);

            setByteOrderForEndianMode(ref dataSizeBytes);

            networkStream.Write(dataSizeBytes, 0, dataSizeBytes.Length);
        }

        private void writeMessageData(NetworkStream networkStream, byte[] dataToWrite)
        {
            networkStream.Write(dataToWrite, 0, dataToWrite.Length);
        }

        private void setByteOrderForEndianMode(ref byte[] dataArray)
        {
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(dataArray);
            }
        }
    }
}
