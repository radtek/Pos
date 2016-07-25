using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Threading;


namespace PMS_Stub
{
    class Program
    {
        private static PDServer.TCPServerThreaded Server;
        private static bool ServerShutdown;
        static void Main(string[] args)
        {
            Server = new PDServer.TCPServerThreaded(IPAddress.Any,4444);
            if (!Server.Start())
            {
                Console.WriteLine(Server.LogMsg);
            }
            else
            {
                Console.WriteLine("Server Start Ok");
            }

            while (ServerShutdown == false)
            {
                String Exit = Console.ReadLine();
                if (Exit.Length > 0)
                {
                    ServerShutdown = true;
                }
                Thread.Sleep(1000);
            }
            Console.WriteLine("Shutting Down");
            Server.Stop();
        }
    }
}
