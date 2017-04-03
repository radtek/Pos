using System;
using FirebirdSql.Data.FirebirdClient;
using System.Data;
using AutoRegister;
using System.Security.Cryptography;
using System.Text;

public class TRegInfo
{

    public String Server = "localhost";
    public String Database = @"C:\Databases\DougnutKing\MenuMate.fdb";
    public FbConnectionStringBuilder cs = new FbConnectionStringBuilder();

    bool Register(TMachineInfo Info)
    {
        Info.RegistrationKey = GenerateRegistrationKey(Info);
        try
        {
            string connectionString = cs.ToString();
            IDbConnection dbcon = new FbConnection(connectionString);
            dbcon.Open();
            IDbCommand dbcmd = dbcon.CreateCommand();

            string sql = @"UPDATE REGISTRATION 
                SET 
                    REGKEY = @REGKEY ,
                    POS_LICENCES = @POS_LICENCES ,
                    PALM_LICENCES = @PALM_LICENCES ,
                    COMPANY = @COMPANY ,
                    MM_MODULES = @MM_MODULES ,
                    OFFICE_LICENCES = @OFFICE_LICENCES
                WHERE 
                    MACHINEKEY = @MACHINEKEY AND PRODUCT = @PRODUCT;";

            dbcmd.CommandText = sql;
            dbcmd.Parameters.Add(new FbParameter("@REGKEY", Info.RegistrationKey.ToLower()));
            dbcmd.Parameters.Add(new FbParameter("@POS_LICENCES", Info.POSLicences));
            dbcmd.Parameters.Add(new FbParameter("@PALM_LICENCES", Info.PalmLicences));
            dbcmd.Parameters.Add(new FbParameter("@COMPANY", Info.CompanyName));
            dbcmd.Parameters.Add(new FbParameter("@MM_MODULES", Info.MenuMateModules));
            dbcmd.Parameters.Add(new FbParameter("@OFFICE_LICENCES", Info.OfficeLicences));
            dbcmd.Parameters.Add(new FbParameter("@MACHINEKEY", Info.HardwareKey));
            dbcmd.Parameters.Add(new FbParameter("@PRODUCT", Info.ProductName));
            dbcmd.ExecuteNonQuery();
            dbcmd.Dispose();
            dbcmd = null;           
            dbcon.Close();
            dbcon = null;
        }
        catch (Exception ex)
        {
        }
        return false;
    }

    public TRegInfo()
	{
        
	}

    String GenerateRegistrationKey(TMachineInfo Info)
    {
        Info.CustomerKey = CalculateMD5Hash(Info.CompanyName + Info.HardwareKey + Info.ProductName + Info.TerminalName).ToLower();
        
        if((Info.MenuMateModules & 0x0040) != 0)
        {
	        return CalculateMD5Hash(	Info.CustomerKey +
                                            Convert.ToString((int)Info.POSLicences, 16).PadLeft(4, '0').ToUpper() +
                                            Convert.ToString((int)Info.PalmLicences, 16).PadLeft(4, '0').ToUpper() +
                                            Convert.ToString((int)Info.MenuMateModules, 16).PadLeft(4, '0').ToUpper() +
                                            Info.Expiry.ToString("dd-mm-yyyy") +
                                            Convert.ToString((int)Info.OfficeLicences, 16).PadLeft(4, '0').ToUpper() +                                           
                                            "".PadLeft(32,'0')); // Allow for future additions.
        }
        else
        {
            return CalculateMD5Hash(Info.CustomerKey +
                                Convert.ToString((int)Info.POSLicences, 16).PadLeft(4, '0').ToUpper() +
                                Convert.ToString((int)Info.PalmLicences, 16).PadLeft(4, '0').ToUpper() +
                                Convert.ToString((int)Info.MenuMateModules, 16).PadLeft(4, '0').ToUpper() +
                                Convert.ToString((int)Info.OfficeLicences, 16).PadLeft(4, '0').ToUpper() +
                                "".PadLeft(32, '0')); // Allow for future additions.
        }
    }

    internal void RegisterAll()
    {
        cs.DataSource = Server;
        cs.Database = Database;
        cs.UserID = "SYSDBA";
        cs.Password = "masterkey";
        cs.Dialect = 3;
        cs.Port = 3050;
       
        try
        {
            string connectionString = cs.ToString();
            IDbConnection dbcon = new FbConnection(connectionString);
            dbcon.Open();
            IDbCommand dbcmd = dbcon.CreateCommand();

            string sql = @"SELECT * FROM REGISTRATION;";

            dbcmd.CommandText = sql;

            IDataReader reader = dbcmd.ExecuteReader();
            UtilDataReader dr = new UtilDataReader(reader);
            while (reader.Read())
            {
                TMachineInfo Info = new TMachineInfo();
                Info.HardwareKey = dr.GetString("MACHINEKEY");
                Info.TerminalName = dr.GetString("TERMINAL_NAME");
                Info.POSLicences = (UInt32)dr.GetInt32("POS_LICENCES") == 0 ? 10 : (UInt32)dr.GetInt32("POS_LICENCES") ;
                Info.PalmLicences = (UInt32)dr.GetInt32("PALM_LICENCES") == 0 ? 10 : (UInt32)dr.GetInt32("PALM_LICENCES");
                Info.ProductName = dr.GetString("PRODUCT");
                Info.CompanyName = dr.GetString("COMPANY") == "" ? "MenuMate Demo" : dr.GetString("COMPANY");
                Info.RegistrationKey = dr.GetString("REGKEY")  == "" ? "" : dr.GetString("REGKEY").Substring(0,32).ToLower();
                Info.MenuMateModules = (UInt32)dr.GetInt32("MM_MODULES") == 0 ? 4285 : (UInt32)dr.GetInt32("MM_MODULES"); ;
                Info.OfficeLicences = (UInt32)dr.GetInt32("OFFICE_LICENCES") == 0 ? 10 : (UInt32)dr.GetInt32("OFFICE_LICENCES");
                Info.Expiry = dr.GetDateTime("EXPIRY");
                Register(Info);
            }
            // clean up 
            reader.Close();
            reader = null;
            dbcmd.Dispose();
            dbcmd = null;
            dbcon.Close();
            dbcon = null;
           
        }
        catch (Exception ex)
        {
        }
    }

    public string CalculateMD5Hash(string input)
    {
        // step 1, calculate MD5 hash from input
        MD5 md5 = System.Security.Cryptography.MD5.Create();
        byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
        byte[] hash = md5.ComputeHash(inputBytes);

        // step 2, convert byte array to hex string
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < hash.Length; i++)
        {
            sb.Append(hash[i].ToString("X2"));
        }
        return sb.ToString();
    }
}
