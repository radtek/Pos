using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Data;
using FirebirdSql.Data.FirebirdClient;

namespace OracleTCPServer
{
    public class OracleServerDB
    {
        public FbConnection connection_;
        public FbTransaction transaction_;
        OracleDetails od;
        private int ProfileKey = 0;
        private int IPENUMListner = 2100;
        private int PORTENUMListner = 2101;
        private int IPENUMOracle = 9623;
        private int PORTENUMOracle = 9624;
        List<string> listLogs = new List<string>();

        public OracleServerDB()
        {

        }
        private void GetOracleDetails()
        {
            OracleDetails od = new OracleDetails();
            List<string> listDetails = new List<string>();
            od = new OracleDetails();
            od.PortNumber =Convert.ToInt32(listDetails[0]);
            od.IPAddress = listDetails[1];
        }
        private FbConnection PrepareConnectionObject()
        {
            OracleDBConnection oracleConnection = new OracleDBConnection();
            string connectionString = oracleConnection.makeFbConnectionString();
            ProfileKey = oracleConnection.ProfileKey;
            listLogs.Add("Connection string                                 " + connectionString);
            var fbConnection = new FbConnection(connectionString);
            fbConnection.Open();
            if(fbConnection.State == ConnectionState.Open)
                listLogs.Add("Connection state                                  " + "Open");
            else
                listLogs.Add("Connection state                                  " + "Not Open");
            MakeLogs(listLogs);
            listLogs.Clear();
            return fbConnection;
        }
        public void ReadOracleServerAppDetails(ref string ipAddress, ref int portNumber)
        {
            try
            {
                listLogs.Add("Inside ReadOracleServerAppDetails                 ");
                using (connection_ = PrepareConnectionObject())
                {
                    using (transaction_ = connection_.BeginTransaction())
                    {
                        listLogs.Add("Transaction state                                 " + "Open");
                        FbCommand command = new FbCommand(@"", connection_, transaction_);
                        command.CommandText = @"SELECT * FROM VARSPROFILE WHERE " +
                            "(VARIABLES_KEY = @PORTENUM OR VARIABLES_KEY = @IPENUM) AND " +
                            "PROFILE_KEY = @PROFILEKEY";
                        command.Parameters.AddWithValue("@PORTENUM", PORTENUMOracle);
                        command.Parameters.AddWithValue("@IPENUM", IPENUMOracle); 
                        command.Parameters.AddWithValue("@PROFILEKEY", ProfileKey);
                        var dataSet = new DataSet();
                        var dataAdapter = new FbDataAdapter(command);
                        dataAdapter.Fill(dataSet);
                        var resultSet = dataSet.Tables[0];
                        listLogs.Add("Row Count                                         " + resultSet.Rows.Count);
                        for (int i = 0; i < resultSet.Rows.Count; i++)
                        {
                            DataRow dataRow = resultSet.Rows[i];
                            if (Convert.ToInt32(dataRow["VARIABLES_KEY"]) == PORTENUMOracle)
                                portNumber = dataRow["INTEGER_VAL"] == null ? default(int) : Convert.ToInt32(dataRow["INTEGER_VAL"]);
                            else if (Convert.ToInt32(dataRow["VARIABLES_KEY"]) == IPENUMOracle)
                                ipAddress = Convert.ToString(dataRow["VARCHAR_VAL"]);
                        }
                    }
                }
                MakeLogs(listLogs);
                listLogs.Clear();
            }
            catch (Exception ex)
            {
                listLogs.Add("Exception in ReadOracleServerAppDetails           " + ex.Message);
                MakeLogs(listLogs);
                listLogs.Clear();
            }
        }
        public void ReadOracleListnerDetails(ref string ipAddress, ref int portNumber)
        {
            try
            {
                listLogs.Add("Inside ReadOracleListenerDetails                 ");
                using (connection_ = PrepareConnectionObject())
                {
                    using (transaction_ = connection_.BeginTransaction())
                    {
                        listLogs.Add("Transaction state                                 " + "Open");
                        FbCommand command = new FbCommand(@"", connection_, transaction_);
                        command.CommandText = @"SELECT * FROM VARSPROFILE WHERE " +
                            "(VARIABLES_KEY = @PORTENUM OR VARIABLES_KEY = @IPENUM) AND " +
                            "PROFILE_KEY = @PROFILEKEY";
                        command.Parameters.AddWithValue("@PORTENUM", PORTENUMListner);
                        command.Parameters.AddWithValue("@IPENUM", IPENUMListner);
                        command.Parameters.AddWithValue("@PROFILEKEY", ProfileKey);
                        var dataSet = new DataSet();
                        var dataAdapter = new FbDataAdapter(command);
                        dataAdapter.Fill(dataSet);
                        var resultSet = dataSet.Tables[0];
                        listLogs.Add("Row Count                                         " + resultSet.Rows.Count);
                        for (int i = 0; i < resultSet.Rows.Count; i++)
                        {
                            DataRow dataRow = resultSet.Rows[i];
                            if (Convert.ToInt32(dataRow["VARIABLES_KEY"]) == PORTENUMListner)
                                portNumber = dataRow["INTEGER_VAL"] == null ? default(int) : Convert.ToInt32(dataRow["INTEGER_VAL"]);
                            else if (Convert.ToInt32(dataRow["VARIABLES_KEY"]) == IPENUMListner)
                                ipAddress = Convert.ToString(dataRow["VARCHAR_VAL"]);
                        }
                    }
                }
                MakeLogs(listLogs);
                listLogs.Clear();
            }
            catch (Exception ex)
            {
                listLogs.Add("Exception in ReadOracleListenerDetails            " + ex.Message);
                MakeLogs(listLogs);
                listLogs.Clear();
            }
        }
        private void MakeLogs(List<string> listLogs)
        {
            LogsUtility logs = new LogsUtility();
            logs.WriteToLogFile(listLogs);
        }
    }
}
