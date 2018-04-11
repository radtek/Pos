using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OracleTCPServer
{
    public class OracleDBConnection
    {
        private string PathDB = "";
        private string Address = "";
        public int ProfileKey = 0;

        public string makeFbConnectionString()
        {
            string connectionString = "";
            List<string> list = new List<string>();
            list = GetDetailsFromFile();
            if (list.Count >= 3)
            {
                ProfileKey = Convert.ToInt32(list[0]);
                Address = list[1];
                PathDB = list[2];
            }
            connectionString += "User=" + "sysdba" + "; ";
            connectionString += "Password=" + "masterkey" + "; ";
            connectionString += "Database=" + PathDB + "; ";
            connectionString += "Datasource=" + Address + "; ";

            connectionString += "Charset=NONE;";
            connectionString += "Connection lifetime=15;";
            connectionString += "Packet Size=8192;";
            connectionString += "Pooling=false";

            return connectionString;
        }
        private List<string> GetDetailsFromFile()
        {
            List<string> list = new List<string>();
            string path = System.IO.Path.GetDirectoryName(
                      System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


            string location = Path.Combine(path, "OracleSeed.txt");
            if (location.Contains(@"file:\"))
            {
                location = location.Replace(@"file:\", "");
            }
            bool exists = File.Exists(location);
            using (StreamReader r = new StreamReader(location))
            {
                // 3
                // Use while != null pattern for loop
                string line;
                while ((line = r.ReadLine()) != null)
                {
                    // 4
                    // Insert logic here.
                    // ...
                    // The "line" value is a line in the file.
                    // Add it to our List.
                    list.Add(line);
                }
            }
            return list;
        }
    }
}
