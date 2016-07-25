using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.Winservices.Client.Console
{
    class Program
    {
        static void Main(string[] args)
        {
            TestService();
        }

        static void TestService()
        {
            PosIntegrationClient client = new PosIntegrationClient();

            // Use the 'client' variable to call operations on the service.

            try
            {
                DTOLocation[] locations = client.GetAllLocations();

                foreach (DTOLocation location in locations)
                {
                    System.Console.WriteLine(location.Name);

                    byte[] bckgImg = client.GetBackgroundImgDataForLocation(location.Id);

                    DTOReservable[] tables = client.GetTablesForLocation(location.Id);

                    foreach (DTOReservable table in tables)
                    {
                        System.Console.WriteLine(string.Format(@"{4} Table: {0}  Position:{1}, {2}  Shape: {3}", table.Id, table.X, table.Y, table.Shape, @"\t"));
                    }
                }
            }
            catch (Exception e)
            {
                System.Console.WriteLine(e.Message);
            }
            finally
            {
                System.Console.ReadLine();

                // Always close the client.
                client.Close();
            }
        }
    }
}
