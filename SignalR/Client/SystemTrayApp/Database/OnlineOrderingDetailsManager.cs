using System.Collections.Generic;
using System.IO;

namespace OnlineOrdersManager
{
    public class OnlineOrderingDetailsManager
    {
        private string PathDB = "";
        private string Address = "";
        public int ProfileKey = 0;

        public List<string> GetDetailsFromFile()
        {
            List<string> list = new List<string>(); 
            string path = System.IO.Path.GetDirectoryName(
                      System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


            string location = Path.Combine(path, "OnlineOrderingSeed.txt");
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
                    list.Add(line);
                }
            }
            return list;
        }
    }
}
