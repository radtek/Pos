
using System;
using System.Data;
using FirebirdSql.Data.FirebirdClient;
using System.IO;


    public class UtilDataReader
    {
        IDataReader Reader = null;
        public UtilDataReader(IDataReader inReader)
        {
            Reader = inReader;
        }

        public int GetInt32(String FieldName)
        {
            int Index = Reader.GetOrdinal(FieldName);
            return Reader.IsDBNull(Index) ? 0 : Reader.GetInt32(Index);
        }

        public int GetInt32(int Index)
        {
            return Reader.IsDBNull(Index) ? 0 : Reader.GetInt32(Index);
        }

        public String GetString(String FieldName)
        {
            int Index = Reader.GetOrdinal(FieldName);
            return Reader.IsDBNull(Index) ? "" : Reader.GetString(Index);
        }

        public String GetString(int Index)
        {
            return Reader.IsDBNull(Index) ? "" : Reader.GetString(Index);
        }

        public bool IsDBNull(String FieldName)
        {
            return Reader.IsDBNull(Reader.GetOrdinal(FieldName));
        }

        public bool IsDBNull(int Index)
        {
            return Reader.IsDBNull(Index);
        }


        public decimal GetDecimal(String FieldName)
        {
            int Index = Reader.GetOrdinal(FieldName);
            return Reader.IsDBNull(Index) ? 0m : Reader.GetDecimal(Index);
        }

        public decimal GetDecimal(int Index)
        {
            return Reader.IsDBNull(Index) ? 0m : Reader.GetDecimal(Index);
        }

        public bool GetBool(String FieldName)
        {
            int Index = Reader.GetOrdinal(FieldName);
            return Reader.IsDBNull(Index) ? false : (Reader.GetString(Index).Equals("T") ? true : false);
        }

        public bool GetBool(int Index)
        {
            return Reader.IsDBNull(Index) ? false : (Reader.GetString(Index).Equals("T") ? true : false);
        }

        public DateTime GetDateTime(String FieldName)
        {
            int Index = Reader.GetOrdinal(FieldName);
            return Reader.IsDBNull(Index) ? new DateTime(0) : this.GetDateTime(Index);
        }

        public DateTime GetDateTime(int Index)
        {
            return Reader.IsDBNull(Index) ? new DateTime(0) : Reader.GetDateTime(Index);
        }

        private string GetBytes(int Index)
        {

            using (MemoryStream ms = new MemoryStream())
            {
                const int BUFFER_SIZE = 1024;
                byte[] buffer = new byte[BUFFER_SIZE];
                int bytesRead, offset = 0;
                while ((bytesRead = (int)Reader.GetBytes(Index, offset, buffer, 0, BUFFER_SIZE)) > 0)
                {
                    ms.Write(buffer, 0, bytesRead);
                    offset += bytesRead;
                }
                ms.Position = 0;
                return "[BASE64]" + System.Convert.ToBase64String(ms.ToArray(), 0, (int)ms.Length);
            }
        }

        public String GetValue(String FieldName)
        {
            int Index = Reader.GetOrdinal(FieldName);
            return GetValue(Index);
        }

        public String GetValue(int Index)
        {
            Type FieldType = Reader.GetFieldType(Index);
            switch (FieldType.Name)
            {
                case "Int32":
                    return this.GetInt32(Index).ToString();
                    break;
                case "String":
                    return this.GetString(Index);
                    break;
                case "Decimal":
                    return this.GetDecimal(Index).ToString();
                    break;
                case "DateTime":
                    return this.GetDateTime(Index).ToString("dd/MM/yyyy HH:mm:ss");
                    break;
                case "Byte[]":
                    return this.GetBytes(Index);
                    break;
                default:
                    return Reader.GetValue(Index).ToString();
            }
        }


    }

