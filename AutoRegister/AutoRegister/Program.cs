using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoRegister
{
    class Program
    {
        static void Main(string[] args)
        {
            String Server = "localhost";
            String Database = @"C:\Program Files (x86)\MenuMate\MenuMate.fdb";

            //if (args.Length > 0)
            //{
            //    Server = args[0];
            //}

            //if (args.Length > 1)
            //{
            //    Database = args[1];
            //}

            TRegInfo Reg = new TRegInfo();
            Reg.Server = Server;
            Reg.Database = Database;

            Reg.RegisterAll();

        }
    }
}
