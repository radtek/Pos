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
using OracleTCPServer.Utility;

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
                        listLogs.Add("Count of bytes received                           " + countBytes);
                        data = System.Text.Encoding.ASCII.GetString(bytes, 0, countBytes);
                        //data = data.Trim();
                        listLogs.Add("Data Received from Client                         " + data);
                        listLogs.Add("Time of reception                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                        byte[] msg = System.Text.Encoding.ASCII.GetBytes(data);
                        byte[] responseBytes = SendRequestToOracle(msg);
                        string responseString = System.Text.Encoding.ASCII.GetString(responseBytes, 0, responseBytes.Length);
                        listLogs.Add("Writing back to client at                         " + DateTime.Now.ToString("hh:mm:ss tt"));
                        stream.Write(responseBytes, 0, responseBytes.Length);
                        listLogs.Add("Data sent to Client                               " + responseString);
                    }
                    client.Close();
                    listLogs.Add("Connection from MM client is closed at            " + DateTime.Now.ToString("hh:mm:ss tt"));
                    MakeLogs(listLogs);
                    listLogs.Clear();
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
                byteReponse = PostToOracle(buffer);
            }
            catch (Exception ex)
            {
                listLogs.Add("Exception in Communication with Oracle App        " + ex.Message);
                listLogs.Add("Time of Exception                                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                string value = "";
                byteReponse = System.Text.Encoding.ASCII.GetBytes(value);
                listLogs.Add("************Re-establishing the connection with Oracle as no response was receieved*************");
                RestartOracleConnection();
            }
            MakeLogs(listLogs);
            listLogs.Clear();
            return byteReponse;
        }
        private static void RestartOracleConnection()
        {
            listLogs.Add("Entered method RestartOracleConnection at            " + DateTime.Now.ToString("hh:mm:ss tt"));
            if (tcpclnt.Connected)
            {
                tcpclnt.Close();
                listLogs.Add("Existing connection closed at                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            if (InitializeClient())
            {
                listLogs.Add("Connection established at                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                SendLinkDescription();
            }
        }
        private static void SendLinkDescription()
        {
            LinkDescription ld = new LinkDescription();
            ld.Date = DateTime.Now.ToString("yyMMdd");
            ld.Time = DateTime.Now.ToString("HHmmss");
            ld.VerNum = GetVersionNumber();
            PostLinkDescription(ld);
        }
        private static string GetVersionNumber()
        {
            string retValue = "";
            OracleServerDB od = new OracleServerDB();
            retValue = od.GetVersionNumber();
            return retValue;
        }
        private static byte[] PostLinkDescription(LinkDescription linkDescription)
        {
            string data = XMLHelper.Serialize<LinkDescription>(linkDescription);
            byte[] buffer = System.Text.Encoding.ASCII.GetBytes(data);
            byte[] byteReponse = new byte[2000];

            try
            {
                PostToOracle(buffer);
            }
            catch (Exception ex)
            {
                listLogs.Add("Exception in Communication with Oracle App during Reconnection        " + ex.Message);
                listLogs.Add("Time of Exception                                                     " + DateTime.Now.ToString("hh:mm:ss tt"));
                string value = "";
                byteReponse = System.Text.Encoding.ASCII.GetBytes(value);
            }
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
                tcpclnt.SendTimeout = 3000;
                tcpclnt.ReceiveTimeout = 9000;
                listLogs.Add("Going to connect to Oracle at                     " + DateTime.Now.ToString("hh:mm:ss tt"));
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
        private static byte[] PostToOracle(byte[] buffer)
        {
            byte[] byteResponse = new byte[2000];

            if (tcpclnt.Available > 0)
            {
                listLogs.Add("Found existing data " + tcpclnt.Available);
                Stream old = tcpclnt.GetStream();
                byte[] oldBytes = new byte[2000];
                int l = old.Read(oldBytes, 0, oldBytes.Length);
                listLogs.Add("Existing data is     " + System.Text.Encoding.ASCII.GetString(oldBytes, 0, l));
            }
            
            Stream stm = tcpclnt.GetStream();
            byte[] STX = new byte[] { 0x02 };
            byte[] ETX = new byte[] { 0x03 };
            listLogs.Add("Writing back to Oracle APP at                     " + DateTime.Now.ToString("hh:mm:ss tt"));
            stm.Write(STX, 0, 1);
            stm.Write(buffer, 0, buffer.Length);
            stm.Write(ETX, 0, 1);

            string dataWritten = System.Text.Encoding.ASCII.GetString(buffer, 0, buffer.Length);
            listLogs.Add("Data written to Oracle APP                        " + dataWritten);

            int k = stm.Read(byteResponse, 0, byteResponse.Length);
            listLogs.Add("Data read from Oracle APP at                      " + DateTime.Now.ToString("hh:mm:ss tt"));
            dataWritten = System.Text.Encoding.ASCII.GetString(byteResponse, 0, k);
            byteResponse = System.Text.Encoding.ASCII.GetBytes(dataWritten);
            listLogs.Add("Data read from Oracle APP                         " + dataWritten);
            listLogs.Add("Time                                              " + DateTime.Now.ToString("hh:mm:ss tt"));

            return byteResponse; 
        }
    }
}
