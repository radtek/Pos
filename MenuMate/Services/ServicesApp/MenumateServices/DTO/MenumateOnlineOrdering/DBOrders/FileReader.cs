using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class FileReader
    {
        public static List<string> GetDetailsFromFile(string locationPath)
        {
            List<string> list = new List<string>();
            string path = System.IO.Path.GetDirectoryName(
                      System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


            string location = Path.Combine(path, locationPath);
            if (location.Contains(@"file:\"))
            {
                location = location.Replace(@"file:\", "");
            }
            bool exists = File.Exists(location);
            using (StreamReader r = new StreamReader(location))
            {
                // Use while != null pattern for loopOnlineOrderDBConnection
                string line;
                while ((line = r.ReadLine()) != null)
                {
                    list.Add(line);
                }
            }
            return list;
        }
    }
}
