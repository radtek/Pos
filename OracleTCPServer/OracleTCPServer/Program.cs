using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OracleTCPServer
{
    class Program
    {
        static TcpClient tcpclnt;
        private static List<string> listLogs = new List<string>();

        static void Main(string[] args)
        {
            bool isCLientInitialized = InitializeClient();
            TcpListener server = null;
            try
            {
                Int32 port = 0;
                string address= "";
                GetListnerDetails(ref port, ref address);
                IPAddress localAddr = IPAddress.Parse(address);

                server = new TcpListener(localAddr, port);

                listLogs.Add("IP Address for listener                           " + address);
                listLogs.Add("Port Number for listener                          " + port.ToString());
                // Start listening for client requests.
                server.Start();
                listLogs.Add("Status of Oracle POS Listener                     " + "After Start");

                MakeLogs(listLogs);
                listLogs.Clear();
                // Buffer for reading data
                Byte[] bytes = new Byte[2000];
                String data = null;

                // Enter the listening loop.
                while (true)
                {
                    TcpClient client = server.AcceptTcpClient();
                    data = null;
                    Thread.Sleep(1000);
                    NetworkStream stream = client.GetStream();
                    int countBytes;

                    while ((countBytes = stream.Read(bytes, 0, bytes.Length)) != 0)
                    {
                        data = System.Text.Encoding.ASCII.GetString(bytes, 0, countBytes);
                        //data = data.Trim();
                        listLogs.Add("Data Received from Client                         " + data);
                        byte[] msg = System.Text.Encoding.ASCII.GetBytes(data);
                        byte[] responseBytes = SendRequestToOracle(msg);
                        string responseString = System.Text.Encoding.ASCII.GetString(responseBytes, 0, responseBytes.Length);
                        stream.Write(responseBytes, 0, responseBytes.Length);
                        listLogs.Add("Data sent to Client                               " + responseString);

                        MakeLogs(listLogs);
                        listLogs.Clear();
                    }
                    client.Close();
                }
            }
            catch (Exception exc)
            {
                listLogs.Add("Exception                                         " + exc.Message);
                listLogs.Add("Time of Exception                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                MakeLogs(listLogs);
                listLogs.Clear();
            }

        }
        private static byte[] SendRequestToOracle(byte[] buffer)
        {
            byte[] byteReponse = new byte[2000];
            try
            {
                Stream stm = tcpclnt.GetStream();
                tcpclnt.SendTimeout = 3000;
                byte[] STX = new byte[] { 0x02 };
                byte[] ETX = new byte[] { 0x03 };
                stm.Write(STX, 0, 1);
                stm.Write(buffer, 0, buffer.Length);
                stm.Write(ETX, 0, 1);

                string dataWritten = System.Text.Encoding.ASCII.GetString(buffer, 0, buffer.Length);
                listLogs.Add("Data written to Oracle APP                        " + dataWritten + " with STX in start and ETX in end");

                int k = stm.Read(byteReponse, 0, byteReponse.Length);

                dataWritten = System.Text.Encoding.ASCII.GetString(byteReponse, 0, k);
                //listLogs.Add("Data Before Trim         " + dataWritten);
                //dataWritten = dataWritten.Trim();
                //listLogs.Add("Data After Trim          " + dataWritten);
                byteReponse = System.Text.Encoding.ASCII.GetBytes(dataWritten);
                listLogs.Add("Data read from Oracle APP                         " + dataWritten);
                listLogs.Add("Time                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                listLogs.Add("Exception in Communication with Oracle App        " + ex.Message);
                listLogs.Add("Time of Exception                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                string value = "";
                byteReponse = System.Text.Encoding.ASCII.GetBytes(value);
            }
            MakeLogs(listLogs);
            listLogs.Clear();
            return byteReponse;
        }
        private static bool InitializeClient()
        {
            tcpclnt = new TcpClient();
            try
            {
                string ipAddress = "";
                int portNumber = 0;
                OracleServerDB od = new OracleServerDB();
                od.ReadOracleServerAppDetails(ref ipAddress, ref portNumber);
                tcpclnt.Connect(ipAddress, portNumber);

                listLogs.Add("IP Address for Oracle APP                         " + ipAddress);
                listLogs.Add("Port Number for Oracle APP                        " + portNumber.ToString());
                if (tcpclnt.Connected)
                    listLogs.Add("Status of Oracle App Connection                   " + "Connected");
                else
                    listLogs.Add("Status of Oracle App Connection                   " + "Not connected");
                listLogs.Add("Time                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                listLogs.Add("Exception                                         " + ex.Message);
                listLogs.Add("Time of Exception                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            MakeLogs(listLogs);
            listLogs.Clear();
            return tcpclnt.Connected;
        }
        private static void GetListnerDetails(ref int port, ref string address)
        {
            OracleServerDB od = new OracleServerDB();
            od.ReadOracleListnerDetails(ref address, ref port);
        }
        private static void MakeLogs(List<string> listLogs)
        {
            LogsUtility logs = new LogsUtility();
            logs.WriteToLogFile(listLogs);
        }
    }
}
