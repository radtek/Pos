using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using Chefmate.Database.Model;
using Chefmate.Logger;
using FirebirdSql.Data.FirebirdClient;

namespace ChefMate.Database
{
    public class DatabaseCore
    {
        private FbConnection _fbConnection;
        private static DatabaseCore _instance;
        private static volatile object _syncRoot = new object();
        private DatabaseCore() { }
        public static DatabaseCore Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new DatabaseCore();
                    }
                }
                return _instance;
            }
        }

        #region Query Execution

        public int GetGeneratorValue(string generatorName)
        {
            OpenConnection();
            var commandText = "SELECT GEN_ID(" + generatorName + ", 1) FROM RDB$DATABASE";
            var command = CreateCommand(commandText);
            var genValue = command.ExecuteScalar();
            CloseConnection();
            return Convert.ToInt32(genValue);
        }
        public bool ExecuteNonQuery(string query, List<QueryParameter> queryParameters)
        {
            try
            {
                OpenConnection();
                var command = CreateCommand(query, queryParameters);
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteNonQuery : ", ex.Message);
                return false;
            }
            finally
            {
                CloseConnection();
            }
        }
        public bool ExecuteNonQuery(string query)
        {
            try
            {
                OpenConnection();
                var command = CreateCommand(query);
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteNonQuery : ", ex.Message);
                return false;
            }
            finally
            {
                CloseConnection();
            }
        }
        public T ExecuteScalar<T>(string query)
        {
            try
            {
                OpenConnection();
                var command = CreateCommand(query);
                var retVal = command.ExecuteScalar();
                CloseConnection();
                if (retVal == null)
                    return default(T);
                return (T)retVal;
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteScalar : ", ex.Message);
                return default(T);
            }
            finally
            {
                CloseConnection();
            }
        }
        public T ExecuteScalar<T>(string query, List<QueryParameter> queryParameters)
        {
            try
            {
                OpenConnection();
                var command = CreateCommand(query, queryParameters);
                var retVal = command.ExecuteScalar();
                CloseConnection();
                if (retVal == null)
                    return default(T);
                return (T)retVal;
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteScalar : ", ex.Message);
                return default(T);
            }
            finally
            {
                CloseConnection();
            }
        }
        public DataTable ExecuteDataSetQuery(string query)
        {
            try
            {
                var command = CreateCommand(query);
                var dataSet = new DataSet();
                var dataAdapter = new FbDataAdapter(command);
                dataAdapter.Fill(dataSet);
                return dataSet.Tables[0];
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteDataSetQuery", ex.Message);
                return new DataTable();
            }
        }
        public DataTable ExecuteDataSetQuery(string query, List<QueryParameter> queryParameters)
        {
            try
            {
                var command = CreateCommand(query, queryParameters);
                var dataSet = new DataSet();
                var dataAdapter = new FbDataAdapter(command);
                dataAdapter.Fill(dataSet);
                return dataSet.Tables[0];
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteDataSetQuery", ex.Message);
                return new DataTable();
            }
        }

        public bool IsRecorExist(string query)
        {
            try
            {
                OpenConnection();
                var command = CreateCommand(query);
                return command.ExecuteNonQuery() > 0;
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("ExecuteNonQuery : ", ex.Message);
                return false;
            }
            finally
            {
                CloseConnection();
            }
        }

        #endregion

        #region Database

        public void CreateDatabase(string dataSource, string dbPath)
        {
            var connectionString = BuildConnectionString(dataSource, dbPath);
            FbConnection.CreateDatabase(connectionString);
        }
        public bool InitializeDatabase(string databaseAddress, string databasePath)
        {
            try
            {
                string connectionString = BuildConnectionString(databaseAddress, databasePath);
                _fbConnection = new FbConnection(connectionString);
                return TestConnection(databaseAddress, databasePath);
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("InitializeDatabae", ex.Message);
                return false;
            }
        }
        public bool TestConnection(string ipAddress, string dbpath)
        {
            var fbConnection = new FbConnection(BuildConnectionString(ipAddress, dbpath));
            try
            {
                fbConnection.Open();
                fbConnection.Close();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        public string BuildConnectionString(string ipAddress, string dbpath)
        {
            string connectionString = "User=SYSDBA;Password=masterkey;Database=" + dbpath + ";DataSource=" + ipAddress +
                                      ";Port=3050;Dialect=3;Charset=NONE;Role=;Connection lifetime=15;Pooling=true;" +
                                      "MinPoolSize=0;MaxPoolSize=50;Packet Size=8192;ServerType=0;";
            return connectionString;
        }
        private FbCommand CreateCommand(string commandText)
        {
            var fbCommand = new FbCommand(commandText, _fbConnection);
            return fbCommand;
        }
        private FbCommand CreateCommand(string commandText, List<QueryParameter> queryParameters)
        {
            var fbCommand = CreateCommand(commandText);
            AddParameters(fbCommand, queryParameters);
            return fbCommand;

        }
        private void OpenConnection()
        {
            if (_fbConnection.State == ConnectionState.Closed || _fbConnection.State == ConnectionState.Broken)
                _fbConnection.Open();
			else
			{
				CloseConnection();
				_fbConnection.Open();
			}
        }
        private void CloseConnection()
        {
            if (_fbConnection.State == ConnectionState.Open)
                _fbConnection.Close();
        }
        private void AddParameters(FbCommand fbCommand, List<QueryParameter> queryParameters)
        {
            foreach (var parameter in queryParameters)
            {
                fbCommand.Parameters.Add(CreateParameter(parameter));
            }
        }
        private FbParameter CreateParameter(QueryParameter parameter)
        {
            return new FbParameter(parameter.ParameterName, parameter.ParameterValue);
        }

        #endregion

        #region Query Builder
        public string BuildInsertQuery(string tableName, List<QueryParameter> parameters)
        {
            string query = "INSERT INTO " + tableName + " (";
            parameters.ForEach(s =>
            {
                query += s.ParameterName + ",";
            });
            query = query.Substring(0, query.Length - 1);
            query += ") VALUES(";
            parameters.ForEach(s =>
            {
                query += "@" + s.ParameterName + ",";
            });
            query = query.Substring(0, query.Length - 1);
            query += ")";
            return query;
        }
        public string BuildUpdateQuery(string tableName, List<QueryParameter> parameters, string whereCondition)
        {
            string query = "UPDATE " + tableName + " SET ";
            parameters.ForEach(s =>
            {
                query += s.ParameterName + " = @" + s.ParameterName + ",";
            });
            query = query.Substring(0, query.Length - 1);
            query += " " + whereCondition;
            return query;
        }
        public string BuildSelectAllQuery(string tableName, string whereCondition = "")
        {
            return "SELECT * FROM " + tableName + " " + whereCondition;
        }

        public string BuildRecordCountQuery(string tableName, string whereCondition = "")
        {
            return "SELECT count(*) FROM " + tableName + " " + whereCondition;
        }

        #endregion

        #region Constraints
        public bool CheckGeneratorExist(string inGeneratorName)
        {
            var queryString = "SELECT COUNT(*) FROM RDB$GENERATORS WHERE RDB$GENERATOR_NAME = @GENERATOR_NAME";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("GENERATOR_NAME", inGeneratorName));
            return CheckObjectExist(queryString, queryParameters);
        }
        public bool CheckColumnExist(string inTableName, string inFieldName)
        {
            var queryString = "SELECT COUNT(*) FROM RDB$RELATION_FIELDS WHERE RDB$RELATION_NAME = @RELATION_NAME AND RDB$FIELD_NAME = @FIELD_NAME";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("RELATION_NAME", inTableName));
            queryParameters.Add(new QueryParameter("FIELD_NAME", inFieldName));
            return CheckObjectExist(queryString, queryParameters);
        }
        public bool CheckTableExist(string inTableName)
        {
            var queryString = "SELECT COUNT(*) FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = @RELATION_NAME";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("RELATION_NAME", inTableName));
            return CheckObjectExist(queryString, queryParameters);
        }
        private bool CheckObjectExist(string queryString, List<QueryParameter> parameter)
        {
            var result = ExecuteScalar<int>(queryString, parameter);
            return result > 0;
        }

        #endregion
    }
}