using System;
using System.Net.Sockets;
using System.Net;
using System.IO;
using PDServer;
using System.Collections.Generic;


namespace post
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			// Construct the command from the args.
			// Attach to the Server.
			String IPAddress = "localhost";
			int port = 65006;
			String FileName = "";
			postClient Client = new postClient ();
			
			if (args.Length > 0) {
				FileName = args[0];
			}
			
			if (args.Length > 1) {
				IPAddress = args[1];
			}
			
			if (args.Length > 2) {
				port = int.Parse(args[2]);
			}
			
			try {
				Client.Connect (IPAddress,port);
				Client.putFile(FileName);
                Console.WriteLine(Client.getdata());
			} catch (Exception e) {
				Console.WriteLine (e.Message);
			}
		}
	}
}
